

//284217

#include <driver/adc.h>
#include <string.h>
#include "driver/rtc_io.h"
#include "accelerometer.h"
#include "esp32/clk.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "esp_pm.h"
#include "esp_sleep.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "onomondoNetwork.h"
#include "temperatureSensor.h"
#include "touchPad.h"

#define LED_POWER 14
#define LED_CLEAR 27
#define LED_DNS 26
#define LED_CONNECTOR 25

#define BTN_0 32
#define BTN_1 13
#define BTN_2 4
#define BTN_3 23

#define BTN_POWER BTN_3
#define BTN_CLEAR BTN_2
#define BTN_DNS BTN_0
#define BTN_CONNECTORS BTN_1
#define ESP_INTR_FLAG_DEFAULT 0

// ESP_EVENT_DECLARE_BASE(CELLULAR_EVENT);
ESP_EVENT_DEFINE_BASE(USER_EVENTS);

enum user_events
{
    CLEAR_FPLMN_EVENT,
    POWER_DOWN_EVENT,
    SEND_CONNECTORS_EVENT,
    SEND_DNS_EVENT
};

typedef struct
{
    uint8_t network_available;
    uint8_t ppp_mode;
    uint8_t modem_initialized;
} app_state_t;

app_state_t app_state = {.network_available = 0, .ppp_mode = 0, .modem_initialized = 0};

static void cellular_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

static void user_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

// ADC characteristics -> used for calib and conversion.
esp_adc_cal_characteristics_t *adc_char;

// gpio input handler
static xQueueHandle gpio_evt_queue = NULL;

//ESP log tag
static const char *TAG = "main";

//threads
void led_task(void *param);
void watchdog_task(void *param);
static void gpio_filter_task(void *arg);

//function prototypes
void powerOff(uint32_t RTCSleepInS);
void init_adc();
float get_batt_voltage();
void fault_state();
void configure_io();

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

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

    // char *tmp = "a";
    // testline(tmp);

    //handle low batt
    init_adc();
    float batt = get_batt_voltage();
    configure_io();
    // if low bat is detected go to deep sleep.
    // device wont wake up before connected to a charger
    if (batt < 3.0 && batt > 2.5)
    {
        ESP_LOGI(TAG, "Low battery: %f", batt);
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL); // usb insert will reset and reenable device.
        esp_deep_sleep_start();
    }

    ESP_LOGI(TAG, "Batt: %f", batt);

    int freq = esp_clk_cpu_freq() / 1000000;
    ESP_LOGI(TAG, "Cpu freq: %d MhZ", freq);

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

    //initialize the cellular connection.
    esp_err_t status = initCellular();
    app_state.modem_initialized = 1;

    if (status != ESP_OK)
        powerOff(1);

    // loop should take inputs from user and from modem events.
    esp_event_handler_register(CELLULAR_EVENT, ESP_EVENT_ANY_ID, &cellular_event_handler, NULL);
    esp_event_handler_register(USER_EVENTS, ESP_EVENT_ANY_ID, &user_event_handler, NULL);

    return;
}

static void cellular_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case CELLULAR_PPP_STARTED:
        app_state.ppp_mode = 1;
        break;
    case CELLULAR_ATTACHED:
        app_state.network_available = 1;
        break;
    case CELLULAR_PPP_STOPPED:
        app_state.ppp_mode = 0;
        break;
    case CELLULAR_STOPPED_SEARCHING:
        app_state.network_available = 0;
        break;
    case CELLULAR_POWERED_DOWN:
        ESP_LOGI(TAG, "Modem off");
        powerOff(0);
        break;
    }
}

static void user_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    //
    //     CLEAR_FPLMN_EVENT,
    //     POWER_DOWN_EVENT,
    //     SEND_CONNECTORS_EVENT,
    //     SEND_DNS_EVENT
    switch (event_id)
    {
    case CLEAR_FPLMN_EVENT:
        if (!app_state.ppp_mode)
            clearFPLMMN();

        break;
    case POWER_DOWN_EVENT:
        ESP_LOGI(TAG, "Powering down");
        forcePowerDown();
        if (app_state.ppp_mode)
            powerOff(0);
        break;
    case SEND_CONNECTORS_EVENT:
        if (!app_state.network_available)
            break;

        if (!app_state.ppp_mode)
        {
            //start ppp and schedule event again.
            requestPPP();
            break;
            // esp_event_post(USER_EVENTS, SEND_CONNECTORS_EVENT, NULL, 0, 1); //could schedule for later... This will spam the interface until ready..
        }
        openSocket("1.2.3.4", 4321);

        sendData("TEST", strlen("TEST"), 0);
        closeSocket();
        break;
    case SEND_DNS_EVENT:
        ESP_LOGI(TAG, "Network Status:%d, PPP Status:%d", app_state.network_available, app_state.ppp_mode);
        if (!app_state.network_available)
            break;

        if (!app_state.ppp_mode)
        {
            //start ppp and schedule event again.
            requestPPP();
            //esp_event_post(USER_EVENTS, SEND_DNS_EVENT, NULL, 0, 1);
            break;
        }
        //get random hostname
        char host[30];
        uint16_t timeMs = (uint16_t)esp_timer_get_time();
        timeMs |= 0x000F;
        sprintf(host, "%d.onomondo.com", timeMs);
        dnsLookup(host);
        break;
    }
}

static void gpio_filter_task(void *arg)
{
    uint8_t btn[4] = {0, 0, 0, 0};
    TickType_t delay = portMAX_DELAY;
    uint32_t io_num = 0;
    for (;;)
    {

        // vTaskDelay(pdMS_TO_TICKS(10));
        // ESP_LOGI("GPIO", "%d,%d,%d,%d", gpio_get_level(BTN_0), gpio_get_level(BTN_1), gpio_get_level(BTN_2), gpio_get_level(BTN_3));

        // continue;

        delay = (btn[0] || btn[1] || btn[2] || btn[3]) ? pdMS_TO_TICKS(200) : portMAX_DELAY; //if button marked as active wait at most 50 ms for debounce.
        if (xQueueReceive(gpio_evt_queue, &io_num, delay))
        {
            switch (io_num)
            {
            case BTN_CLEAR:
                btn[0] = 1;
                break;
            case BTN_POWER:
                btn[1] = 1;
                break;
            case BTN_CONNECTORS:
                btn[2] = 1;
                break;
            case BTN_DNS:
                btn[3] = 1;
                break;
            }
        }
        else
        {
            // receive timeout. BTN must be stabilized.
            uint32_t events[4] = {CLEAR_FPLMN_EVENT, POWER_DOWN_EVENT, SEND_CONNECTORS_EVENT, SEND_DNS_EVENT};

            for (size_t i = 0; i < 4; i++)
            {
                if (btn[i])
                {
                    esp_event_post(USER_EVENTS, events[i], NULL, 0, 1);
                    btn[i] = 0;
                }
            }
        }
    }
}

void configure_io()
{

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_filter_task, "gpio_handler", 2048, NULL, 10, NULL);

    gpio_config_t pinCfg;
    pinCfg.mode = GPIO_MODE_OUTPUT;
    pinCfg.pin_bit_mask = (1ULL << LED_POWER) | (1ULL << LED_CLEAR) | (1ULL << LED_DNS) | (1ULL << LED_CONNECTOR);
    pinCfg.intr_type = GPIO_INTR_DISABLE;
    pinCfg.pull_up_en = 0;
    pinCfg.pull_down_en = 0;
    gpio_config(&pinCfg);

    pinCfg.mode = GPIO_MODE_INPUT;
    pinCfg.pull_down_en = 1;
    pinCfg.pin_bit_mask = (1ULL << BTN_DNS | 1ULL << BTN_CLEAR | 1ULL << BTN_POWER | 1ULL << BTN_CONNECTORS);
    pinCfg.intr_type = GPIO_INTR_POSEDGE;

    gpio_config(&pinCfg);

    // install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // hook ISR handlers
    gpio_isr_handler_add(BTN_DNS, gpio_isr_handler, (void *)BTN_DNS);
    gpio_isr_handler_add(BTN_CLEAR, gpio_isr_handler, (void *)BTN_CLEAR);
    gpio_isr_handler_add(BTN_POWER, gpio_isr_handler, (void *)BTN_POWER);
    gpio_isr_handler_add(BTN_CONNECTORS, gpio_isr_handler, (void *)BTN_CONNECTORS);
}

void powerOff(uint32_t RTCSleepInS)
{
    // detachAndPowerDown();
    // forcePowerDown();
    // ++wake_count;
    // acc_resetInterrupt();

    // esp_sleep_enable_timer_wakeup
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    rtc_gpio_pulldown_en(BTN_0);
    rtc_gpio_pulldown_en(BTN_1);
    rtc_gpio_pulldown_en(BTN_2);

    rtc_gpio_pullup_dis(BTN_0);
    rtc_gpio_pullup_dis(BTN_1);
    rtc_gpio_pullup_dis(BTN_2);

    uint64_t mask = (1LL << BTN_0 | 1LL << BTN_1 | 1LL << BTN_2);

    esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_HIGH);

    // esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    if (RTCSleepInS)
    {
        esp_sleep_enable_timer_wakeup((uint64_t)RTCSleepInS * (uint64_t)1000 * (uint64_t)1000); // 10 secs
    }

    //gpio_set_level(LED_LOGO, 1);
    gpio_set_level(LED_POWER, 0);
    gpio_set_level(LED_CLEAR, 0);
    gpio_set_level(LED_DNS, 0);
    gpio_set_level(LED_CONNECTOR, 0);

    esp_deep_sleep_start();
}

void led_task(void *param)
{
    //figure out the state of the connection and/or error state.

    //gpio_set_level(LED_LOGO, 1);
    gpio_set_level(LED_POWER, 1);
    gpio_set_level(LED_CLEAR, 0);
    gpio_set_level(LED_DNS, 0);
    gpio_set_level(LED_CONNECTOR, 0);

    uint32_t timing = 0;
    uint8_t led_ppp = 0, led_clear = 0;
    while (1)
    {
        if (app_state.modem_initialized)
        {
            led_clear = 1;

            //attached but no pdp context yet
            if (app_state.network_available && !app_state.ppp_mode)
            {
                led_ppp = timing % 8 == 0 ? (led_ppp ? 0 : 1) : led_ppp;
            }

            //attached but and ppp context
            if (app_state.network_available && app_state.ppp_mode)
            {
                led_ppp = 1;
                led_clear = 0; //clear no longer available...
            }
        }

        gpio_set_level(LED_CLEAR, led_clear);
        gpio_set_level(LED_DNS, led_ppp);
        gpio_set_level(LED_CONNECTOR, led_ppp);

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

    vTaskDelay(pdMS_TO_TICKS(400));
    powerOff(0);
}

void watchdog_task(void *param)
{
    // last resort watchdog. If device has been on for too long we reboot it... ->
    // this should hopefully never happen, but if the socket API stalls this should 'handle' it.
    ESP_LOGI(TAG, "Watchdog start");
    vTaskDelay(pdMS_TO_TICKS(1000 * 60 * 10)); // seven minutes

    ESP_LOGI(TAG, "Watchdog timeout");
    powerOff(1); //sleep one second and reboot.
}