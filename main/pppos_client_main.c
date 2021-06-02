
#include <string.h>

#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "onomondoNetwork.h"

static const char* TAG = "main";

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

    esp_err_t status = initCellular(SIM800);

    status = openSocket("1.2.3.4", 4321);

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
    closeSocket();

    ESP_LOGE(TAG, "Powering down...");
    detachAndPowerDown();

    //vTaskDelay(pdMS_TO_TICKS(2000));

    esp_sleep_enable_timer_wakeup(30 * 1000 * 1000);  // 10 secs
    esp_deep_sleep_start();
}
