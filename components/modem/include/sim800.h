// Copyright 2015-2018 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_modem.h"
#include "esp_modem_dce_service.h"

// power on module!

#ifdef GEN_1

#define RST_PIN 5
#define PWR_PIN 4
#define PWR_ON_PIN 23
#define PULSE_ON 0
#define PULSE_OFF 1

#elif GEN_2

#define PWR_PIN 27
#define PULSE_ON 1
#define PULSE_OFF 0

#else

#define PWR_PIN 33
#define PULSE_ON 1
#define PULSE_OFF 0

#endif

/**
 * @brief Create and initialize SIM800 object
 *
 * @param dte Modem DTE object
 * @return modem_dce_t* Modem DCE object
 */
modem_dce_t *sim800_init(modem_dte_t *dte, uint8_t modem_reset_requested);

#ifdef __cplusplus
}
#endif
