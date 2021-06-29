#pragma once

#include "esp_err.h"
#include "esp_log.h"
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#ifdef __cplusplus
extern "C" {
#endif

esp_err_t touchpad_init(EventGroupHandle_t* eventgroup, EventBits_t eventBits);
esp_err_t touch_deinit();

#ifdef __cplusplus
}
#endif
