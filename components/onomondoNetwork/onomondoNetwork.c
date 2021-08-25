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
#include "sim7600.h"
#include "sim800.h"

static const char *TAG = "cellular";
static EventGroupHandle_t event_group = NULL;
static const int CONNECT_BIT = BIT0;
static const int STOP_BIT = BIT1;

void *modem_netif_adapter = NULL;
esp_netif_t *esp_netif = NULL;
modem_dte_t *dte = NULL;
modem_dce_t *dce = NULL;

float signalQuality = 0;

int initialized = 0;

// internal ref for the socket.
// for ease of use this is kept private to the 'user'

int socket_ = -1;

static esp_err_t default_handle(modem_dce_t *dce, const char *line)
{
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS))
    {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    }
    else if (strstr(line, MODEM_RESULT_CODE_ERROR))
    {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    }
    return err;
}

static void modem_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case ESP_MODEM_EVENT_PPP_START:
        ESP_LOGI(TAG, "Modem PPP Started");
        break;
    case ESP_MODEM_EVENT_PPP_STOP:
        ESP_LOGI(TAG, "Modem PPP Stopped");
        xEventGroupSetBits(event_group, STOP_BIT);
        break;
    case ESP_MODEM_EVENT_UNKNOWN:
        break;
    default:
        break;
    }
}

static void on_ppp_changed(void *arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "PPP state changed event %d", event_id);
    if (event_id == NETIF_PPP_ERRORUSER)
    {
        /* User interrupted event from esp-netif */
        esp_netif_t *netif = *(esp_netif_t **)event_data;
        ESP_LOGI(TAG, "User interrupted event from netif:%p", netif);
    }
}

static void on_ip_event(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "IP event! %d", event_id);
    if (event_id == IP_EVENT_PPP_GOT_IP)
    {
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

        ESP_LOGI(TAG, "GOT ip event!!!");
    }
    else if (event_id == IP_EVENT_PPP_LOST_IP)
    {
        ESP_LOGI(TAG, "Modem Disconnect from PPP Server");
    }
    else if (event_id == IP_EVENT_GOT_IP6)
    {
        ESP_LOGI(TAG, "GOT IPv6 event!");

        ip_event_got_ip6_t *event = (ip_event_got_ip6_t *)event_data;
        ESP_LOGI(TAG, "Got IPv6 address " IPV6STR, IPV62STR(event->ip6_info.ip));
    }
}

esp_err_t initCellular(enum supportedModems modem, bool fullModemInit)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &on_ip_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(NETIF_PPP_STATUS, ESP_EVENT_ANY_ID, &on_ppp_changed, NULL));

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

    switch (modem)
    {
    case SIM800:
        dce = sim800_init(dte);
        break;
    case SIM7xxx:
        dce = sim7600_init(dte);
        break;
    default:
        killandclean();
        return ESP_FAIL;
        break;
    }

    if (dce == NULL)
        return ESP_FAIL;

    assert(dce != NULL);
    ESP_ERROR_CHECK(dce->set_flow_ctrl(dce, MODEM_FLOW_CONTROL_NONE));
    // ESP_ERROR_CHECK(dce->store_profile(dce));

    for (size_t k = 0; k < 8; k++)
    {
        dce->checkNetwork(dce);
        if (dce->attached == ATTACH_ROAMING || dce->attach == ATTACH_HOME_NETWORK)
            break;

        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // dce->scanNetworks(dce);

    //print available networks...
    for (int k = 0; k < dce->networks.numberOfNetworks; k++)
    {
        struct network_t *net = &dce->networks.availableNetworks[k];

        switch (net->accessTechnology)
        {
        case 0: //GSM
            ESP_LOGI("Network: ", "Type: GSM, Name: %s, mccmnc: %s", net->name, net->mccmnc);
            break;
        case 7: //LTE
            ESP_LOGI("Network: ", "Type: LTE-M, Name: %s, mccmnc: %s", net->name, net->mccmnc);
            break;

        default:
            break;
        }
    }

    if (dce->attached != ATTACH_ROAMING)
    { //at+cops = 4/0
        dce->attach(dce, 0);
    }

    ESP_LOGI(TAG, "Module: %s", dce->name);
    dce->checkNetwork(dce);
    //wait for modem to attach.
    int tries = 0;
    int errorCount = 0;
    while (!(dce->attached == ATTACH_ROAMING || dce->attached == ATTACH_HOME_NETWORK))
    {
        if (dce->checkNetwork(dce) != ESP_OK)
            errorCount++;

        if (errorCount > 10)
            return ESP_FAIL;

        switch (dce->attached)
        {
        case ATTACH_NOT_SEARCHING:
            dce->attach(dce, 1);
            errorCount++;
            return ESP_FAIL;
            break;
        case ATTACH_SEARCHING:
            //keep going
            break;
        case ATTACH_ROAMING:
            //success
            break;
        case ATTACH_DENIED:
            //we might get this in case of network whitelist. The device can connect to other networks though!
            // dce->attach(dce, 1);
            break;
        default:
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
        ++tries;
        if (tries == 120)
            //clear the FPLMN.
            esp_modem_dce_clear_fplmn(dce);

        if (tries > 240)
            return ESP_FAIL;
    }

    /* Print Module ID, Operator, IMEI, IMSI */
    ESP_LOGI(TAG, "Operator: %s", dce->oper);
    ESP_LOGI(TAG, "IMEI: %s", dce->imei);
    ESP_LOGI(TAG, "IMSI: %s", dce->imsi);
    /* Get signal quality */
    uint32_t rssi = 0, ber = 0;
    ESP_ERROR_CHECK(dce->get_signal_quality(dce, &rssi, &ber));
    ESP_LOGI(TAG, "rssi: %d, ber: %d", rssi, ber);
    signalQuality = rssi;

    // setup psm and edrx
    // In this case PSM does not add any value, as the modem is quite slow to boot.
    // Furthermore, we can tear down the connection quicker by toggling power with the powerpin.
    dce->enable_edrx(dce, 1);
    dce->enable_psm(dce, 0);

    esp_netif_attach(esp_netif, modem_netif_adapter);
    /* Wait for IP address */
    xEventGroupWaitBits(event_group, CONNECT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

    initialized = 1;
    return ESP_OK;
}

int getSignalQuality()
{
    return signalQuality;
}

// handle internal
esp_err_t openSocket(char *host, int port)
{
    if (socket_ != -1)
    {
        return ESP_FAIL;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_family = AF_INET;
    addr.sin_port = lwip_htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host, port);
    socket_ = sock;
    int err = connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

    if (err != 0)
    {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        closeSocket();
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t sendData(char *data, int len, int timeout)
{
    if (socket_ < 1)
    {
        return ESP_FAIL;
    }

    if (!data)
        return ESP_FAIL;

    //blocking socket api.
    int err = send(socket_, data, len, 0);

    if (err < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t closeSocket(void)
{
    shutdown(socket_, 0);
    close(socket_);

    socket_ = -1;
    return ESP_OK;
}

esp_err_t killandclean()
{

    closeSocket();

    return ESP_OK;
}

esp_err_t forcePowerDown()
{
    //if psm is supported, do a soft power down
    if (dce->PSM)
    {
        if (dce && dte)
        {
            //initialize the stop sequence
            ESP_LOGI(TAG, "Exit PPP");
            esp_err_t alive = esp_modem_stop_ppp(dte);

            // we just wait for the psm enter notification...
            // xEventGroupWaitBits(event_group, STOP_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

            int i = 0;
            while (!dce->psm_enter_notified && i < 50)
            {
                vTaskDelay(pdMS_TO_TICKS(200));
                ++i;
            }

            if (dce->psm_enter_notified)
            {
                return ESP_OK;
            }
        }
    }

    //no psm or we never get the psm enter notification. Toggle pin to power down..
    //bit harder stop, but modem gets to detach as needed.
    dce->power_down(dce);
    return ESP_OK;
}