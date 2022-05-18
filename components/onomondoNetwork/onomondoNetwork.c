#include "onomondoNetwork.h"

#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_modem.h"
#include "esp_modem_netif.h"
#include "esp_netif.h"
#include "esp_netif_ppp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lwip/dns.h"
#include "sim7600.h"
#include "sim800.h"

#define SEM_GIVE xSemaphoreGive(at_cmd_sem);

/*  -- GLOABAL VARS --  */

static const char *TAG = "cellular";
static EventGroupHandle_t event_group = NULL;
static const int CONNECT_BIT = BIT0;
static const int STOP_BIT = BIT1;

void *modem_netif_adapter = NULL;
esp_netif_t *esp_netif = NULL;
modem_dte_t *dte = NULL;
modem_dce_t *dce = NULL;

esp_timer_handle_t TIMER;

float signalQuality = 0;
int initialized = 0;
int socket_ = -1;

/*  -- BOOK KEEPING --*/
// internal modem state
enum modemStates {
    MODEM_OFF,
    MODEM_NOT_SEARCHING,
    MODEM_SEARCHING,
    MODEM_ATTACHED,
    MODEM_PPP,
    MODEM_PPP_PAUSE,
    MODEM_PPP_REQUESTED
};
volatile int modemState = MODEM_OFF;

// track attach process
typedef struct {
    uint8_t _reg_denied_count;
    uint64_t _reg_update_count;
    uint32_t _failed_at_commands_count;
} cellularAttachHelper_t;

cellularAttachHelper_t reg_stats = {
    ._reg_denied_count = 0,
    ._reg_update_count = 0,
    ._failed_at_commands_count = 0};

/*  -- EVENT LOOPS --  */
// cellular events can't use the default loop as it can be blocking.
esp_event_loop_handle_t cellular_event_loop_hdl; /*!< Event loop handle */
ESP_EVENT_DEFINE_BASE(CELLULAR_EVENT);

/*  -- SYNC --  */
SemaphoreHandle_t at_cmd_sem = NULL;

uint8_t SEM_TAKE(uint32_t x) {
    return at_cmd_sem != NULL ? xSemaphoreTake(at_cmd_sem, pdMS_TO_TICKS((x))) : false;
}
#define SEM_GIVE xSemaphoreGive(at_cmd_sem);

/*  -- PROTOTYPES --  */
esp_err_t switchToPPP();

static void timer_callback(void *arg) {
    if (SEM_TAKE(0)) {
        if (modemState != MODEM_PPP && modemState != MODEM_OFF && dce) {
            if (dce->checkNetwork(dce) != ESP_OK) {
                reg_stats._failed_at_commands_count++;
                if (reg_stats._failed_at_commands_count > 20) {
                    // modem is dead. Probably.
                    esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_POWERED_DOWN, NULL, 0, 0);
                }
            }
        }
        SEM_GIVE
    }
}

esp_err_t clearFPLMMN() {
    // halt other communications
    if (SEM_TAKE(5000)) {
        esp_modem_dce_clear_fplmn(dce);
        SEM_GIVE
    } else {
        return ESP_FAIL;
    }
    return ESP_OK;
}

static void modem_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case ESP_MODEM_EVENT_PPP_START:
            ESP_LOGI(TAG, "Modem PPP Started");
            modemState = MODEM_PPP;
            //    esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_PPP_STARTED, NULL, 0, 1);
            break;
        case ESP_MODEM_EVENT_PPP_STOP:
            modemState = MODEM_PPP_PAUSE;
            ESP_LOGI(TAG, "Modem PPP Stopped");
            esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_NOT_AVAILABLE, NULL, 0, 1);
            xEventGroupSetBits(event_group, STOP_BIT);
            break;
        case ESP_MODEM_EVENT_UNKNOWN:
            break;
        case ESP_MODEM_EVENT_NETWORK_STATUS:
            if (event_data) {
                reg_status_t *n = (reg_status_t *)event_data;
                ESP_LOGI(TAG, "Network registration update: CREG:%d, CEREG:%d, CGREG:%d", n->CREG, n->CEREG, n->CGREG);
                // ESP_LOGI(TAG, "CREG:  %d", n->CREG);
                // ESP_LOGI(TAG, "CGREG  %d", n->CGREG);
                // ESP_LOGI(TAG, "CEREG  %d", n->CEREG);

                if (!(n->CREG || n->CGREG || n->CEREG)) {
                    ESP_LOGI(TAG, "Modem stopped searching and is not attached.");
                    esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_STOPPED_SEARCHING, NULL, 0, 1);
                }

                if ((n->CEREG == 5 || n->CGREG == 5) && modemState != MODEM_ATTACHED) {
                    esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_ATTACHED, NULL, 0, 1);
                    modemState = MODEM_ATTACHED;
                    reg_stats._reg_denied_count = 0;
                    reg_stats._reg_update_count = 0;
                    esp_timer_stop(TIMER);
                    esp_timer_start_periodic(TIMER, TIMER_PERIOD * 10);
                }

                if (!(n->CEREG == 5 || n->CGREG == 5)) {
                    if (modemState == MODEM_ATTACHED) {
                        // signal we lost connection
                        esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_SEARCHING, NULL, 0, 1);
                    }
                    modemState = MODEM_SEARCHING;
                }

                if ((n->CREG == 3 || n->CGREG == 3 || n->CEREG == 3) && modemState != MODEM_ATTACHED) {
                    reg_stats._reg_denied_count++;
                }

                if (reg_stats._reg_denied_count > 100) {
                    if (SEM_TAKE(5000)) {
                        vTaskDelay(100);
                        esp_modem_dce_clear_fplmn(dce);
                        SEM_GIVE
                    }

                    reg_stats._reg_denied_count = 0;
                }
                if (modemState == MODEM_SEARCHING || modemState == MODEM_NOT_SEARCHING)
                    reg_stats._reg_update_count++;

                if (reg_stats._reg_update_count > 500)  //
                {
                    if (SEM_TAKE(5000)) {
                        vTaskDelay(100);

                        esp_modem_dce_clear_fplmn(dce);

                        SEM_GIVE
                    }

                    esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_NOT_AVAILABLE, NULL, 0, 1);
                }
            }
            break;
        case ESP_MODEM_EVENT_OPERATOR_SELECTION:
            break;
        case ESP_MODEM_EVENT_POWER_DOWN:
            esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_POWERED_DOWN, NULL, 0, 1);
            break;

        default:
            break;
    }
}

static void cellular_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case CELLULAR_STOPPED_SEARCHING:
            if (dce) {
                if (SEM_TAKE(1000)) {
                    dce->attach(dce, 1);
                    SEM_GIVE
                }
            }
            break;
        case CELLULAR_PPP_STARTED:
            modemState = MODEM_PPP;
            break;
        case CELLULAR_PPP_STOPPED:
            modemState = MODEM_PPP_PAUSE;
            break;

        case CELLULAR_ATTACHED:;
            uint32_t rssi = 0, ber = 0;
            if (dce)
                if (SEM_TAKE(1000)) {
                    dce->get_signal_quality(dce, &rssi, &ber);
                    dce->enable_edrx(dce, 1);
                    SEM_GIVE
                }
            signalQuality = rssi;
            break;
        case MODEM_EVENT_PPP_DISCONNECT:
            esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_NOT_AVAILABLE, NULL, 0, 1);
            break;

        default:
            break;
    }
}

static void on_ppp_changed(void *arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "PPP state changed event %d", event_id);
    if (event_id == NETIF_PPP_ERRORUSER) {
        /* User interrupted event from esp-netif */
        esp_netif_t *netif = *(esp_netif_t **)event_data;
        ESP_LOGI(TAG, "User interrupted event from netif:%p", netif);
    }
}

static void on_ip_event(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "IP event! %d", event_id);
    if (event_id == IP_EVENT_PPP_GOT_IP) {
        esp_netif_dns_info_t dns_info;

        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        esp_netif_t *netif = event->esp_netif;

        ESP_LOGI(TAG, "Modem Connect to PPP Server");
        ESP_LOGI(TAG, "~~~~~~~~~~~~~~");
        ESP_LOGI(TAG, "IP          : " IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Netmask     : " IPSTR, IP2STR(&event->ip_info.netmask));
        ESP_LOGI(TAG, "Gateway     : " IPSTR, IP2STR(&event->ip_info.gw));
        esp_netif_get_dns_info(netif, 0, &dns_info);
        ESP_LOGI(TAG, "Name Server1: " IPSTR, IP2STR(&dns_info.ip.u_addr.ip4));
        esp_netif_get_dns_info(netif, 1, &dns_info);
        ESP_LOGI(TAG, "Name Server2: " IPSTR, IP2STR(&dns_info.ip.u_addr.ip4));
        ESP_LOGI(TAG, "~~~~~~~~~~~~~~");
        xEventGroupSetBits(event_group, CONNECT_BIT);
        esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_PPP_STARTED, NULL, 0, 1);

        ESP_LOGI(TAG, "GOT ip event!!!");
    } else if (event_id == IP_EVENT_PPP_LOST_IP) {
        ESP_LOGI(TAG, "Modem Disconnect from PPP Server");
        esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_NOT_AVAILABLE, NULL, 0, 1);  // or
    } else if (event_id == IP_EVENT_GOT_IP6) {
        ESP_LOGI(TAG, "GOT IPv6 event!");

        ip_event_got_ip6_t *event = (ip_event_got_ip6_t *)event_data;
        ESP_LOGI(TAG, "Got IPv6 address " IPV6STR, IPV62STR(event->ip6_info.ip));
    }
}

esp_err_t initCellular() {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &on_ip_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(NETIF_PPP_STATUS, ESP_EVENT_ANY_ID, &on_ppp_changed, NULL));

    esp_event_loop_args_t loop_args = {
        .queue_size = 5,
        .task_name = "loop_task",  // task will be created
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY};

    ESP_ERROR_CHECK(esp_event_loop_create(&loop_args, &cellular_event_loop_hdl));
    // ESP_ERROR_CHECK(esp_event_handler_register(CELLULAR_EVENT, ESP_EVENT_ANY_ID, &cellular_event_handler, NULL));

    modemState = MODEM_SEARCHING;
    cellular_set_event_handler(cellular_event_handler, ESP_EVENT_ANY_ID, NULL);

    at_cmd_sem = xSemaphoreCreateBinary();

    if (!at_cmd_sem) {
        ESP_LOGE(TAG, "FAILED TO CREATE SEM");
        return ESP_FAIL;
    }

    event_group = xEventGroupCreate();

    esp_modem_dte_config_t config = ESP_MODEM_DTE_DEFAULT_CONFIG();

    /* setup UART specific configuration based on kconfig options */
    config.tx_io_num = TX_PIN;
    config.rx_io_num = RX_PIN;
    config.rts_io_num = RTS_PIN;
    config.cts_io_num = CTS_PIN;
    config.rx_buffer_size = CONFIG_EXAMPLE_MODEM_UART_RX_BUFFER_SIZE;
    config.tx_buffer_size = CONFIG_EXAMPLE_MODEM_UART_TX_BUFFER_SIZE;
    config.pattern_queue_size = CONFIG_EXAMPLE_MODEM_UART_PATTERN_QUEUE_SIZE;
    config.event_queue_size = CONFIG_EXAMPLE_MODEM_UART_EVENT_QUEUE_SIZE;
    config.event_task_stack_size = CONFIG_EXAMPLE_MODEM_UART_EVENT_TASK_STACK_SIZE;
    config.event_task_priority = CONFIG_EXAMPLE_MODEM_UART_EVENT_TASK_PRIORITY;
    config.line_buffer_size = CONFIG_EXAMPLE_MODEM_UART_RX_BUFFER_SIZE / 2;
    // config.baud_rate = 9600;
    // config.flow_control = SOFTWARE_FLOW

    dte = esp_modem_dte_init(&config);

    /* Register event handler */
    ESP_ERROR_CHECK(esp_modem_set_event_handler(dte, modem_event_handler, ESP_EVENT_ANY_ID, NULL));

    // Init netif object
    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_PPP();

    esp_netif = esp_netif_new(&cfg);
    assert(esp_netif);

    modem_netif_adapter = esp_modem_netif_setup(dte);
    esp_modem_netif_set_default_handlers(modem_netif_adapter, esp_netif);

    dce = sim800_init(dte);

    ESP_LOGI(TAG, " ~~~~~~~~~~~~~~ print some modem stuff ~~~~~~~~~~~~~~");
    ESP_LOGI(TAG, "Modem IMEI: %s", dce->imei);
    ESP_LOGI(TAG, "Modem IMSI: %s", dce->imsi);
    ESP_LOGI(TAG, "Modem ICCID: %s", dce->iccid);
    ESP_LOGI(TAG, " ~~~~~~~~~~~~~~ ");

    if (dce == NULL)
        return ESP_FAIL;

    // dce->attach(dce, 0);

    assert(dce != NULL);
    ESP_ERROR_CHECK(dce->set_flow_ctrl(dce, MODEM_FLOW_CONTROL_NONE));

    // Create and start the event sources
    esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
    };

    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &TIMER));
    ESP_ERROR_CHECK(esp_timer_start_periodic(TIMER, TIMER_PERIOD));

    SEM_GIVE return ESP_OK;
}

esp_err_t cellular_set_event_handler(esp_event_handler_t handler, int32_t event_id, void *handler_args) {
    if (!cellular_event_loop_hdl)
        return ESP_FAIL;
    return esp_event_handler_register_with(cellular_event_loop_hdl, CELLULAR_EVENT, event_id, handler, handler_args);
}

int getSignalQuality() {
    if (modemState == MODEM_ATTACHED) {
        uint32_t rssi = 0, ber = 0;

        dce->get_signal_quality(dce, &rssi, &ber);

        signalQuality = rssi;
    }
    return signalQuality;
}

// handle internal
esp_err_t openSocket(char *host, int port) {
    if (socket_ != -1) {
        ESP_LOGI(TAG, "Using existing socket");
        return ESP_OK;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_family = AF_INET;
    addr.sin_port = lwip_htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host, port);
    socket_ = sock;
    int err = connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        closeSocket();
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t sendData(char *data, int len, int timeout) {
    if (socket_ < 1) {
        return ESP_FAIL;
    }

    if (!data)
        return ESP_FAIL;

    // blocking socket api.
    int err = send(socket_, data, len, 0);

    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        closeSocket();
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t closeSocket(void) {
    if (socket_ < 0 || modemState != MODEM_PPP_MODE)
        return ESP_FAIL;

    shutdown(socket_, 0);
    close(socket_);

    socket_ = -1;
    return ESP_OK;
}

void dns_found_cb(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    // don't care for this example..
    ESP_LOGI(TAG, "DNS OK");
}

esp_err_t dnsLookup(const char *host) {
    ip_addr_t addr;
    IP_ADDR4(&addr, 0, 0, 0, 0);

    if (dns_gethostbyname(host, &addr, dns_found_cb, NULL) == ERR_ARG)
        return ESP_FAIL;

    return ESP_OK;
}

esp_err_t killandclean() {  // TODO
    closeSocket();

    return ESP_OK;
}

esp_err_t forcePowerDown() {
    if (dce) {
        esp_timer_stop(TIMER);

        // effectively stops and wait for other processes to be done
        if (modemState != MODEM_PPP)
            SEM_TAKE(10000);

        if (!dce->power_down_notified)
            dce->power_down(dce);

        if (modemState == MODEM_PPP) {
            esp_event_post_to(cellular_event_loop_hdl, CELLULAR_EVENT, CELLULAR_POWERED_DOWN, NULL, 0, 0);
        }
    } else
        return ESP_FAIL;
    return ESP_OK;
}

esp_err_t requestPPP() {
    if (modemState != MODEM_ATTACHED)
        return ESP_FAIL;

    if (modemState == MODEM_PPP_REQUESTED || modemState == MODEM_PPP_MODE)
        return ESP_OK;

    return switchToPPP();
}

esp_err_t switchToPPP() {
    ESP_LOGI(TAG, "PPP requested.");
    esp_timer_stop(TIMER);
    SEM_TAKE(9999);  // make sure no one else is pushing AT commands!
    // not 'legal' from this point!
    modemState = MODEM_PPP_REQUESTED;

    return esp_netif_attach(esp_netif, modem_netif_adapter);
}

void getICCID(char *ICCID) {
    if (dce) {
        strncpy(ICCID, dce->iccid, strlen(dce->iccid));
    }
};
// void getIMEI(char *IMEI);
// void getIMSI(char *IMSI);