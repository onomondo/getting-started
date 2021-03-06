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
extern "C"
{
#endif

#include "esp_err.h"
#include "esp_modem_dte.h"
#include "esp_types.h"

    typedef struct modem_dce modem_dce_t;
    typedef struct modem_dte modem_dte_t;
    typedef struct esp_modem_dce esp_modem_dce_t;

/**
 * @brief Result Code from DCE
 *
 */
#define MODEM_RESULT_CODE_SUCCESS "OK"              /*!< Acknowledges execution of a command */
#define MODEM_RESULT_CODE_CONNECT "CONNECT"         /*!< A connection has been established */
#define MODEM_RESULT_CODE_RING "RING"               /*!< Detect an incoming call signal from network */
#define MODEM_RESULT_CODE_NO_CARRIER "NO CARRIER"   /*!< Connection termincated or establish a connection failed */
#define MODEM_RESULT_CODE_ERROR "ERROR"             /*!< Command not recognized, command line maximum length exceeded, parameter value invalid */
#define MODEM_RESULT_CODE_NO_DIALTONE "NO DIALTONE" /*!< No dial tone detected */
#define MODEM_RESULT_CODE_BUSY "BUSY"               /*!< Engaged signal detected */
#define MODEM_RESULT_CODE_NO_ANSWER "NO ANSWER"     /*!< Wait for quiet answer */

/**
 * @brief Specific Length Constraint
 *
 */
#define MODEM_MAX_NAME_LENGTH (32)     /*!< Max Module Name Length */
#define MODEM_MAX_OPERATOR_LENGTH (32) /*!< Max Operator Name Length */
#define MODEM_IMEI_LENGTH (15)         /*!< IMEI Number Length */
#define MODEM_IMSI_LENGTH (15)         /*!< IMSI Number Length */
#define MODEM_MAX_NETWORKNAME (15)
#define MOODM_MAX_NUMBER_OF_NETWORKS (10)

/**
 * @brief Specific Timeout Constraint, Unit: millisecond
 *
 */
#define MODEM_COMMAND_TIMEOUT_DEFAULT (500)      /*!< Default timeout value for most commands */
#define MODEM_COMMAND_TIMEOUT_OPERATOR (75000)   /*!< Timeout value for getting operator status */
#define MODEM_COMMAND_TIMEOUT_MODE_CHANGE (5000) /*!< Timeout value for changing working mode */
#define MODEM_COMMAND_TIMEOUT_HANG_UP (90000)    /*!< Timeout value for hang up */
#define MODEM_COMMAND_TIMEOUT_POWEROFF (1000)    /*!< Timeout value for power down */

/**
 * @brief GSM Attachment status 
 *
 */
#define ATTACH_SEARCHING 2
#define ATTACH_DENIED 3
#define ATTACH_ROAMING 5
#define ATTACH_HOME_NETWORK 1
#define ATTACH_NOT_SEARCHING 0

    /**
 * @brief Working state of DCE
 *
 */
    typedef enum
    {
        MODEM_STATE_PROCESSING, /*!< In processing */
        MODEM_STATE_SUCCESS,    /*!< Process successfully */
        MODEM_STATE_FAIL        /*!< Process failed */
    } modem_state_t;

    typedef struct network_t
    {
        uint8_t accessTechnology;
        char name[MODEM_MAX_NETWORKNAME + 1];
        char mccmnc[7];
        uint8_t status; // 0,1,3 (3 == forbidden)
        /* data */
    } network_t;

    struct networks_t
    {
        network_t availableNetworks[MOODM_MAX_NUMBER_OF_NETWORKS];
        int numberOfNetworks;
    };

    /**
 * @brief DCE(Data Communication Equipment)
 *
 */
    typedef struct
    {
        uint8_t CREG;
        uint8_t CGREG;
        uint8_t CEREG;
    } reg_status_t;

    struct modem_dce
    {
        char imei[MODEM_IMEI_LENGTH + 1];     /*!< IMEI number */
        char imsi[MODEM_IMSI_LENGTH + 1];     /*!< IMSI number */
        char name[MODEM_MAX_NAME_LENGTH];     /*!< Module name */
        char oper[MODEM_MAX_OPERATOR_LENGTH]; /*!< Operator name */
        uint8_t act;                          /*!< Access technology */
        unsigned int attached;
        bool eDRX;
        bool PSM;
        bool psm_enter_notified;
        bool power_down_notified;
        struct networks_t networks;
        reg_status_t network_status;
        uint8_t operatorMode;
        const char *prompt;                                                               /*!< Modem prompt string */
        modem_state_t state;                                                              /*!< Modem working state */
        modem_mode_t mode;                                                                /*!< Working mode */
        modem_dte_t *dte;                                                                 /*!< DTE which connect to DCE */
        esp_err_t (*handle_line)(modem_dce_t *dce, const char *line);                     /*!< Handle line strategy */
        esp_err_t (*sync)(modem_dce_t *dce);                                              /*!< Synchronization */
        esp_err_t (*echo_mode)(modem_dce_t *dce, bool on);                                /*!< Echo command on or off */
        esp_err_t (*store_profile)(modem_dce_t *dce);                                     /*!< Store user settings */
        esp_err_t (*set_flow_ctrl)(modem_dce_t *dce, modem_flow_ctrl_t flow_ctrl);        /*!< Flow control on or off */
        esp_err_t (*get_signal_quality)(modem_dce_t *dce, uint32_t *rssi, uint32_t *ber); /*!< Get signal quality */
        esp_err_t (*get_battery_status)(modem_dce_t *dce, uint32_t *bcs,
                                        uint32_t *bcl, uint32_t *voltage); /*!< Get battery status */
        esp_err_t (*get_operator_name)(modem_dce_t *dce);                  /*!< Get operator name */
        esp_err_t (*define_pdp_context)(modem_dce_t *dce, uint32_t cid,
                                        const char *type, const char *apn); /*!< Set PDP Contex */
        esp_err_t (*set_working_mode)(modem_dce_t *dce, modem_mode_t mode); /*!< Set working mode */
        esp_err_t (*hang_up)(modem_dce_t *dce);                             /*!< Hang up */
        esp_err_t (*power_down)(modem_dce_t *dce);                          /*!< Normal power down */
        esp_err_t (*deinit)(modem_dce_t *dce);                              /*!< Deinitialize */
        esp_err_t (*checkNetwork)(modem_dce_t *dce);
        esp_err_t (*attach)(modem_dce_t *dce, uint8_t autoSelect);
        esp_err_t (*detach)(modem_dce_t *dce);
        esp_err_t (*set_default_bands)(modem_dce_t *dce);
        esp_err_t (*enable_psm)(modem_dce_t *dce, uint8_t enable);
        esp_err_t (*enable_edrx)(modem_dce_t *dce, uint8_t enable);

        esp_err_t (*scanNetworks)(modem_dce_t *dce);
    };

    /**
  * @brief ESP Modem with private resource
  *
  */
    struct esp_modem_dce
    {
        void *priv_resource; /*!< Private resource */
        modem_dce_t parent;  /*!< DCE parent class */
    };

#ifdef __cplusplus
}
#endif
