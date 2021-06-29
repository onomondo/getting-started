

//284217

#include <string.h>

#include "accelerometer.h"
#include "esp_log.h"
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

enum EVENT_BITS {
    TOUCH_EVENT = BIT0
};

//global sync.
static EventGroupHandle_t eventGroup = NULL;

static const char* TAG = "main";

RTC_DATA_ATTR int wake_count;  //this will keep the mem. section powered no though... That is not really something we want..

void powerOff(uint32_t RTCSleepInS);

void app_main(void) {
    //  led thread
    //  sensor thread
    //  celluar thread. .
    //  event group used to sync...
    //      e.g. sensor reports when values are ready, when sensors are powered down

    // Initialize NVS. This will be needed for OTA portion...
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // 1.OTA app partition table has a smaller NVS partition size than the non-OTA
        // partition table. This size mismatch may cause NVS initialization to fail.
        // 2.NVS partition contains data in new format and cannot be recognized by this version of code.
        // If this happens, we erase NVS partition and initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    gpio_config_t pinCfg;
    pinCfg.mode = GPIO_MODE_OUTPUT;
    pinCfg.pin_bit_mask = (1UL << LED_LOGO) | (1UL << LED_1) | (1UL << LED_2);
    pinCfg.intr_type = GPIO_INTR_DISABLE;
    pinCfg.pull_up_en = 0;
    pinCfg.pull_down_en = 0;
    gpio_config(&pinCfg);
    gpio_set_level(LED_LOGO, 1);
    gpio_set_level(LED_1, 0);
    gpio_set_level(LED_2, 0);

    ESP_LOGI(TAG, "Setting up accelerometer....");
    acc_init();
    tmp_init();

    float temp, hum;
    tmp_read(&temp, &hum);

    ESP_LOGI(TAG, "Temperature: %f", temp);
    eventGroup = xEventGroupCreate();
    touchpad_init(&eventGroup, TOUCH_EVENT);

    esp_err_t status = initCellular(SIM800, wake_count);

    if (status != ESP_OK) {
        powerOff(0);
    }

    status = openSocket("1.2.3.4", 4321);

    if (status != ESP_OK)
        powerOff(0);

    while (1) {
        EventBits_t uxBits;
        const TickType_t xTicksToWait = 15000 / portTICK_PERIOD_MS;
        uxBits = xEventGroupWaitBits(eventGroup, TOUCH_EVENT, pdTRUE, pdTRUE, xTicksToWait);

        if (!(uxBits & TOUCH_EVENT)) {
            //timeout..
            ESP_LOGI(TAG, "Timeout...");
            break;
        }

        gpio_set_level(LED_1, 1);
        gpio_set_level(LED_2, 1);
        ESP_LOGI(TAG, "Yey, touch event");

        if (status == ESP_OK) {
            tmp_read(&temp, &hum);
            float signal = getSignalQuality();
            float battery = 4.4;

            char payload[100];  // = "Hello from onomondo...!";

            sprintf(payload, "{\"battery\": %f,\"signal\": %f,\"temperature\": %f}", battery, signal, temp);

            status = sendData(payload, strlen(payload), 0);

            if (status == ESP_OK) {
                ESP_LOGI(TAG, "Succesful transmit");
            } else {
                ESP_LOGE(TAG, "Failed to transmit...");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200));

        gpio_set_level(LED_1, 0);
        gpio_set_level(LED_2, 0);
    }

    gpio_set_level(LED_LOGO, 0);

    closeSocket();

    ESP_LOGE(TAG, "Powering down...");

    powerOff(0);

    int i = 0;

    while (1) {
        if (status == ESP_OK) {
            char* payload = "Hello from onomondo...!";

            status = sendData(payload, strlen(payload), 0);

            if (status == ESP_OK) {
                ESP_LOGI(TAG, "Succesful transmit");
            } else {
                ESP_LOGE(TAG, "Failed to transmit...");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));

        if (i++ > 5)
            break;
    }

    //Start RTC timer and power down....

    esp_sleep_enable_timer_wakeup(30 * 1000 * 1000);  // 10 secs
    esp_deep_sleep_start();
}

void powerOff(uint32_t RTCSleepInS) {
    detachAndPowerDown();

    ++wake_count;
    touch_deinit();
    acc_resetInterrupt();
    uint64_t mask = (uint64_t)1 << 34;
    esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_HIGH);

    if (RTCSleepInS) {
        esp_sleep_enable_timer_wakeup((uint64_t)RTCSleepInS * (uint64_t)1000 * (uint64_t)1000);  // 10 secs
    }
    esp_deep_sleep_start();
}