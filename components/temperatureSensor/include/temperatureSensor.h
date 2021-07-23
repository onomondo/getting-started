
#pragma once

#include "esp_err.h"
#include "esp_log.h"
#include "esp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Register addresses */
#define TMP75_RTEMP 0x00   /// Temperature register
#define TMP75_RCONF 0x01   /// Configuration register
#define TMP75_RTEMPL 0x02  /// TLOW register
#define TMP75_RTEMPH 0x03  /// THIGH register

// note
// i2c must be initialized before.

esp_err_t tmp_init();

esp_err_t tmp_read(float* temperature, float* humidity);

#ifdef __cplusplus
}
#endif
