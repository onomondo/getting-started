

//284217

#include <driver/adc.h>
#include <string.h>

#include "accelerometer.h"
#include "esp32/clk.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "esp_pm.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "onomondoNetwork.h"
#include "temperatureSensor.h"
#include "touchPad.h"
#include "ping/ping_sock.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_random.h"
#define LED_LOGO 25
#define LED_1 4
#define LED_2 5

enum EVENT_BITS
{
    TOUCH_EVENT = BIT0
};

enum NET_STATUS
{
    DETACHED,
    ATTACHED,
    IP,
    TRANSMITTING
};

// Global var. for app state.
// probably not suitable for atomic writes....
RTC_DATA_ATTR struct
{
    enum NET_STATUS net_status;
    uint8_t timeout_count;
    uint8_t error_state;
} app_state;

//global sync.
static EventGroupHandle_t eventGroup = NULL;

//ADC characteristics -> used for calib and conversion.
esp_adc_cal_characteristics_t *adc_char;

//ESP log tag
static const char *TAG = "main";

//threads
void led_task(void *param);
void watchdog_task(void *param);

//function prototypes
void powerOff(uint32_t RTCSleepInS);
void init_adc();
float get_batt_voltage();
void fault_state();
static void test_on_ping_success(esp_ping_handle_t hdl, void *args);
static void test_on_ping_timeout(esp_ping_handle_t hdl, void *args);
static void test_on_ping_end(esp_ping_handle_t hdl, void *args);
void initialize_ping();

void app_main(void)
{
    // esp_deep_sleep_start();

    // Initialize NVS. This will be needed for OTA portion...
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // 1.OTA app partition table has a smaller NVS partition size than the non-OTA
        // partition table. This size mismatch may cause NVS initialization to fail.
        // 2.NVS partition contains data in new format and cannot be recognized by this version of code.
        // If this happens, we erase NVS partition and initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    app_state.timeout_count = 0;

    ESP_LOGI("Error state", "%d", app_state.error_state);
    // char *tmp = "a";
    // testline(tmp);

    //handle low batt
    init_adc();
    float batt = get_batt_voltage();

    // if low bat is detected go to deep sleep.
    // device wont wake up before connected to a charger
    if (batt < 3.0 && batt > 2.5)
    {

        ESP_LOGI(TAG, "Low battery: %f", batt);
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
        esp_deep_sleep_start();
    }

    ESP_LOGI(TAG, "Batt: %f", batt);

    // cpu clock config. w. dynamic freq. scaling.
    // esp_pm_config_esp32_t conf = {.max_freq_mhz = 40, .min_freq_mhz = 40, .light_sleep_enable = 0};
    // esp_pm_configure(&conf);
    int freq = esp_clk_cpu_freq() / 1000000;
    ESP_LOGI(TAG, "Cpu freq: %d MhZ", freq);

    //init app status
    app_state.net_status = DETACHED;
    // app_state.error_state = 1;
    // handle the output
    TaskHandle_t xHandle = NULL, xHandleWatchdog = NULL;
    xTaskCreate(led_task, "LED_TASK", 4000, NULL, tskIDLE_PRIORITY, &xHandle);
    xTaskCreate(watchdog_task, "WATCHDOG_TASK", 4000, NULL, tskIDLE_PRIORITY, &xHandleWatchdog);

    //init sensors
    acc_init();
    tmp_init();

    float temp, hum;
    tmp_read(&temp, &hum);

    ESP_LOGI(TAG, "Temperature: %f", temp);
    eventGroup = xEventGroupCreate();

    //init touchpad. Uses the eventgroup to signal main thre    ad.
    touchpad_init(&eventGroup, TOUCH_EVENT);

    //initialize the cellular connection. Registers the PPPoS with on the network stack.
    esp_err_t status = initCellular(SIM800, 0);

    if (status != ESP_OK)
        fault_state();

    // app_state.error_state = 0;
    app_state.net_status = ATTACHED;

    // while (1)
    //     vTaskDelay(10);

    vTaskDelay(pdMS_TO_TICKS(2000));

    // initialize_ping();
    esp_http_client_config_t config = {
        .disable_auto_redirect = true,
        .url = "http://google.org",
        .port = 6443};

    esp_http_client_handle_t client = esp_http_client_init(&config);

    // POST
    // const char *post_data = (const char *)malloc(2000);
    const char *post_data = calloc(2000, sizeof(uint8_t));
    char url[150];

    do
    {
        uint8_t postErr = 0;
        uint32_t random = esp_random();

        sprintf(url, "http://%7u.watchdog.icanhaziot.com:6000", random);

        ESP_LOGI("HTTP(s)", "%s", url);

        esp_http_client_set_url(client, url);
        esp_http_client_set_method(client, HTTP_METHOD_POST);
        // esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_set_post_field(client, post_data, 2000);
        err = esp_http_client_perform(client);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
                     esp_http_client_get_status_code(client),
                     esp_http_client_get_content_length(client));
            app_state.error_state = 0;
        }
        else
        {
            ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
            app_state.error_state = 1;
            postErr++;
        }

        esp_http_client_cleanup(client);

        if (!postErr)
            vTaskDelay(pdMS_TO_TICKS(20000));

        powerOff(1);

        vTaskDelay(pdMS_TO_TICKS(30000));
    } while (app_state.error_state == 0);

    // while (!app_state.error_state)
    //     vTaskDelay(100);

    // //create a socket.
    // status = openSocket("8.8.8.8", 53);

    // if (status != ESP_OK)
    //     fault_state();

    // app_state.net_status = IP;

    // while (1)
    // {
    //     // EventBits_t uxBits;
    //     // const TickType_t xTicksToWait = 30000 / portTICK_PERIOD_MS;

    //     // uxBits = xEventGroupWaitBits(eventGroup, TOUCH_EVENT, pdTRUE, pdTRUE, xTicksToWait);

    //     // if (!(uxBits & TOUCH_EVENT))
    //     //     break;

    //     if (status == ESP_OK)
    //     {
    //         // tmp_read(&temp, &hum);
    //         // float signal = getSignalQuality();
    //         // float battery = get_batt_voltage();

    //         char payload[100] = "Hello...!";

    //         // sprintf(payload, "{\"battery\": %f,\"signal\": %f,\"temperature\": %f}", battery, signal, temp);
    //         app_state.net_status = TRANSMITTING;
    //         status = sendData(payload, strlen(payload), 0);

    //         app_state.net_status = IP;
    //         if (status == ESP_OK)
    //         {
    //             ESP_LOGI("Transmit", "%s", payload);
    //         }
    //         else
    //         {
    //             ESP_LOGE(TAG, "Failed to transmit...");
    //             app_state.error_state = 1;
    //             break;
    //         }
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(2000));
    // }

    // closeSocket();

    powerOff(10);
    vTaskDelete(xHandle);
}

void powerOff(uint32_t RTCSleepInS)
{
    // detachAndPowerDown();
    forcePowerDown();
    // ++wake_count;
    touch_deinit();
    acc_resetInterrupt();

    // esp_sleep_enable_timer_wakeup
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    uint64_t mask = (uint64_t)1 << 34;
    esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_HIGH);
    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    if (RTCSleepInS)
    {
        esp_sleep_enable_timer_wakeup((uint64_t)RTCSleepInS * (uint64_t)1000 * (uint64_t)1000); // 10 secs
    }

    gpio_set_level(LED_LOGO, 0);
    gpio_set_level(LED_1, 0);
    gpio_set_level(LED_2, 0);

    esp_deep_sleep_start();
}

void led_task(void *param)
{
    //figure out the state of the connection and/or error state.

    gpio_config_t pinCfg;
    pinCfg.mode = GPIO_MODE_OUTPUT;
    pinCfg.pin_bit_mask = (1UL << LED_LOGO) | (1UL << LED_1) | (1UL << LED_2);
    pinCfg.intr_type = GPIO_INTR_DISABLE;
    pinCfg.pull_up_en = 0;
    pinCfg.pull_down_en = 0;
    gpio_config(&pinCfg);
    //gpio_set_level(LED_LOGO, 1);
    gpio_set_level(LED_1, 0);
    gpio_set_level(LED_2, 0);

    uint8_t error, status;
    bool status_led = 0, error_led = 0;
    uint32_t timing = 0;

    while (1)
    {
        error = app_state.error_state;
        status = app_state.net_status;

        //error led
        if (error)
        {
            if (timing % 2 == 0)
            {
                error_led = error_led ? 0 : 1; //flip at fixed interval...
            }
        }
        else
        {
            error_led = 0;
        }

        //status led.. We want quick flashes.

        status_led = 0;

        switch (status)
        {
        case DETACHED:
            status_led = timing % 20 == 0 ? 1 : 0;
            break;
        case ATTACHED:
            status_led = timing % 10 == 0 ? 1 : 0;
            break;
        case IP:
            status_led = 1;
            break;
        case TRANSMITTING:
            status_led = timing % 2 == 0 ? 1 : 0;
            break;
        default:
            break;
        }

        gpio_set_level(LED_2, error_led);
        gpio_set_level(LED_1, status_led);

        timing++;

        vTaskDelay(pdMS_TO_TICKS(40));
    }
}

void init_adc()
{
    //ADC1_CH4
    adc_gpio_init(ADC_UNIT_1, ADC1_CHANNEL_4);
    adc_char = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_11db, ADC_WIDTH_BIT_12, 1, adc_char);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_11db);
}

float get_batt_voltage()
{
    int raw = adc1_get_raw(ADC1_CHANNEL_4);

    return (float)esp_adc_cal_raw_to_voltage(raw, adc_char) * 2 / 1000.0; // x2 due to voltage division..
}

void fault_state()
{
    app_state.error_state = 1;
    vTaskDelay(pdMS_TO_TICKS(400));
    powerOff(1);
}

void watchdog_task(void *param)
{
    // last resort watchdog. If device has been on for too long we reboot it... ->
    // this should hopefully never happen, but if the socket API stalls this should 'handle' it.
    ESP_LOGI(TAG, "Watchdog start");

    //as long as we receive ping do nothing :P
    do
    {
        vTaskDelay(pdMS_TO_TICKS(3 * 1000 * 60)); //

    } while (!app_state.error_state);

    ESP_LOGI(TAG, "Watchdog timeout");
    powerOff(1); //sleep one second and reboot.
}

//////////////////////////
// ping related stuff //
static void test_on_ping_success(esp_ping_handle_t hdl, void *args)
{
    // optionally, get callback arguments
    // const char* str = (const char*) args;
    // printf("%s\r\n", str); // "foo"
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TTL, &ttl, sizeof(ttl));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    ESP_LOGI("PING:", "%d bytes from %s icmp_seq=%d ttl=%d time=%d ms\n",
             recv_len, inet_ntoa(target_addr.u_addr.ip4), seqno, ttl, elapsed_time);

    app_state.error_state = 0;
    app_state.timeout_count = 0;
}

static void test_on_ping_timeout(esp_ping_handle_t hdl, void *args)
{

    app_state.timeout_count++;

    uint16_t seqno;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    ESP_LOGI("PING:", "From %s icmp_seq=%d timeout\n", inet_ntoa(target_addr.u_addr.ip4), seqno);

    if (app_state.timeout_count > 1)
        app_state.error_state = 1;
}

static void test_on_ping_end(esp_ping_handle_t hdl, void *args)
{
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;

    esp_ping_get_profile(hdl, ESP_PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    esp_ping_get_profile(hdl, ESP_PING_PROF_REPLY, &received, sizeof(received));
    esp_ping_get_profile(hdl, ESP_PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    ESP_LOGI("PING:", "%d packets transmitted, %d received, time %dms\n", transmitted, received, total_time_ms);
}

void initialize_ping()
{
    /* convert URL to IP address */
    ip_addr_t target_addr = IPADDR4_INIT_BYTES(8, 8, 8, 8);

    esp_ping_config_t ping_config = ESP_PING_DEFAULT_CONFIG();
    ping_config.target_addr = target_addr;       // target IP address
    ping_config.count = ESP_PING_COUNT_INFINITE; // ping in infinite mode, esp_ping_stop can stop it
    ping_config.interval_ms = 30 * 1000;
    ping_config.timeout_ms = 800;
    /* set callback functions */
    esp_ping_callbacks_t cbs;
    cbs.on_ping_success = test_on_ping_success;
    cbs.on_ping_timeout = test_on_ping_timeout;
    cbs.on_ping_end = test_on_ping_end;
    cbs.cb_args = NULL; // arguments that will feed to all callback functions, can be NULL

    esp_ping_handle_t ping;
    esp_err_t err = esp_ping_new_session(&ping_config, &cbs, &ping);

    switch (err)
    {
    case ESP_ERR_INVALID_ARG:
        ESP_LOGI("PING", "Invalid arg");
        break;
    case ESP_FAIL:
        ESP_LOGI("PING", "Invalid arg");
        break;
    case ESP_OK:
        ESP_LOGI("PING", "OK");

    default:
        break;
    }

    esp_ping_start(ping);
}
