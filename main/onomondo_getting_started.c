

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
volatile struct
{
    enum NET_STATUS net_status;
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
    app_state.error_state = 0;

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

    //init touchpad. Uses the eventgroup to signal main thread.
    touchpad_init(&eventGroup, TOUCH_EVENT);

    //initialize the cellular connection. Registers the PPPoS with on the network stack.
    esp_err_t status = initCellular(SIM800, 0);

    if (status != ESP_OK)
        fault_state();

    app_state.net_status = ATTACHED;

    // while (1)
    //     vTaskDelay(10);

    //create a socket.
    status = openSocket("8.8.8.8", 53);

    if (status != ESP_OK)
        fault_state();

    app_state.net_status = IP;

    while (1)
    {
        EventBits_t uxBits;
        const TickType_t xTicksToWait = 30000 / portTICK_PERIOD_MS;

        uxBits = xEventGroupWaitBits(eventGroup, TOUCH_EVENT, pdTRUE, pdTRUE, xTicksToWait);

        if (!(uxBits & TOUCH_EVENT))
            break;

        if (status == ESP_OK)
        {
            tmp_read(&temp, &hum);
            float signal = getSignalQuality();
            float battery = get_batt_voltage();

            char payload[100]; // = "Hello from onomondo...!";

            sprintf(payload, "{\"battery\": %f,\"signal\": %f,\"temperature\": %f}", battery, signal, temp);
            app_state.net_status = TRANSMITTING;
            status = sendData(payload, strlen(payload), 0);

            app_state.net_status = IP;
            if (status == ESP_OK)
            {
                ESP_LOGI("Transmit", "%s", payload);
            }
            else
            {
                ESP_LOGE(TAG, "Failed to transmit...");
                app_state.error_state = 1;
                vTaskDelay(pdMS_TO_TICKS(500));
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }

    closeSocket();

    vTaskDelete(xHandle);
    powerOff(0);
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
            if (timing % 8 == 0)
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
    powerOff(0);
}

void watchdog_task(void *param)
{
    // last resort watchdog. If device has been on for too long we reboot it... ->
    // this should hopefully never happen, but if the socket API stalls this should 'handle' it.
    ESP_LOGI(TAG, "Watchdog start");
    vTaskDelay(pdMS_TO_TICKS(7 * 1000 * 60)); // seven minutes

    ESP_LOGI(TAG, "Watchdog timeout");
    powerOff(1); //sleep one second and reboot.
}