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
#include "esp_modem_dce_service.h"

#include <string.h>

#include "esp_log.h"

static const char *DCE_TAG = "dce_service";

/**
 * @brief Macro defined for error checking
 *
 */
#define DCE_CHECK(a, str, goto_tag, ...)                                              \
    do {                                                                              \
        if (!(a)) {                                                                   \
            ESP_LOGE(DCE_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                            \
        }                                                                             \
    } while (0)

esp_err_t esp_modem_dce_handle_response_default(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    }
    return err;
}

esp_err_t esp_modem_dce_handle_csq(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    esp_modem_dce_t *esp_dce = __containerof(dce, esp_modem_dce_t, parent);
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else if (!strncmp(line, "+CSQ", strlen("+CSQ"))) {
        /* store value of rssi and ber */
        uint32_t **csq = esp_dce->priv_resource;
        /* +CSQ: <rssi>,<ber> */
        sscanf(line, "%*s%d,%d", csq[0], csq[1]);
        err = ESP_OK;
    }
    return err;
}

esp_err_t esp_modem_dce_handle_cbc(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    esp_modem_dce_t *esp_dce = __containerof(dce, esp_modem_dce_t, parent);
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else if (!strncmp(line, "+CBC", strlen("+CBC"))) {
        /* store value of bcs, bcl, voltage */
        uint32_t **cbc = esp_dce->priv_resource;
        /* +CBC: <bcs>,<bcl>,<voltage> */
        sscanf(line, "%*s%d,%d,%d", cbc[0], cbc[1], cbc[2]);
        err = ESP_OK;
    }
    return err;
}

esp_err_t esp_modem_dce_handle_exit_data_mode(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;

    ESP_LOGI("EXIT DATA MODE", "Received: %s", line);

    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_NO_CARRIER)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    }
    return err;
}

esp_err_t esp_modem_dce_handle_atd_ppp(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_CONNECT)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    }
    return err;
}

esp_err_t esp_modem_dce_handle_cgmm(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else {
        int len = snprintf(dce->name, MODEM_MAX_NAME_LENGTH, "%s", line);
        if (len > 2) {
            /* Strip "\r\n" */
            strip_cr_lf_tail(dce->name, len);
            err = ESP_OK;
        }
    }
    return err;
}

esp_err_t esp_modem_dce_handle_cgsn(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else {
        int len = snprintf(dce->imei, MODEM_IMEI_LENGTH + 1, "%s", line);
        if (len > 2) {
            /* Strip "\r\n" */
            strip_cr_lf_tail(dce->imei, len);
            err = ESP_OK;
        }
    }
    return err;
}

esp_err_t esp_modem_dce_handle_response_netscan(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else if (!strncmp(line, "+COPS", strlen("+COPS"))) {
        dce->networks.numberOfNetworks = 0;

        // pointer to each network field
        char *networks[10];
        uint8_t n = 0;
        // make local copy as strtok will change string
        size_t len = strlen(line);
        char *l = malloc(len + 1);
        strcpy(l, line);

        networks[0] = strtok(l, "(");
        while (n < MOODM_MAX_NUMBER_OF_NETWORKS && networks[n] != NULL) {  // get pointer to each beginning of (net_act, <long format>, <short. . .. ), (net_act, <long. ..),,(1,2,3),(xxxxx)
            networks[++n] = strtok(NULL, "(");
        }

        uint8_t netIdx = 0;
        for (uint8_t network = 0; network < n - 2; network++)  // two last fields not relevant
        {
            /* code */
            char line_copy[50];  // alloc on stack is fine

            // technically it could work without another copy with the strtok_r
            strcpy(line_copy, networks[network]);

            char *entries[6];  // each field for each network found.

            entries[0] = strtok(line_copy, ",");

            uint8_t i = 0;
            while (entries[i]) {
                entries[++i] = strtok(NULL, ",)");
                if (i > 5) {
                    ESP_LOGE("Netscan", "Rouge string: %s", line_copy);
                    free(l);
                    return ESP_FAIL;
                }
            }

            if (i == 5)  // all fields present.
            {
                dce->networks.availableNetworks[netIdx].status = atoi(entries[0]);
                dce->networks.availableNetworks[netIdx].accessTechnology = atoi(entries[4]);
                strncpy(dce->networks.availableNetworks[netIdx].mccmnc, entries[3], 6);
                strncpy(dce->networks.availableNetworks[netIdx].name, entries[1], MODEM_MAX_NETWORKNAME);
                dce->networks.numberOfNetworks = netIdx + 1;
                netIdx++;
            }
        }

        free(l);

        return ESP_OK;
    }

    return err;
}

esp_err_t esp_modem_dce_handle_cimi(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else {
        int len = snprintf(dce->imsi, MODEM_IMSI_LENGTH + 1, "%s", line);
        if (len > 2) {
            /* Strip "\r\n" */
            strip_cr_lf_tail(dce->imsi, len);
            err = ESP_OK;
        }
    }
    return err;
}

esp_err_t esp_modem_dce_handle_ccid(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else {
        int len = snprintf(dce->iccid, MODEM_ICCID_LENGTH + 1, "%s", line);
        if (len > 2) {
            /* Strip "\r\n" */
            strip_cr_lf_tail(dce->iccid, len);
            err = ESP_OK;
        }
    }
    return err;
}

esp_err_t esp_modem_dce_handle_cops(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;
    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else if (!strncmp(line, "+COPS", strlen("+COPS"))) {
        /* there might be some random spaces in operator's name, we can not use sscanf to parse the result */
        /* strtok will break the string, we need to create a copy */
        size_t len = strlen(line);
        char *line_copy = malloc(len + 1);
        strcpy(line_copy, line);
        /* +COPS: <mode>[, <format>[, <oper>[, <Act>]]] */
        char *str_ptr = NULL;
        char *p[5];
        uint8_t i = 0;
        /* strtok will broke string by replacing delimiter with '\0' */
        p[i] = strtok_r(line_copy, ",", &str_ptr);
        while (p[i]) {
            p[++i] = strtok_r(NULL, ",", &str_ptr);
        }
        if (i > 2) {
            dce->operatorMode = (uint8_t)strtol(p[1], NULL, 0);
        }

        if (i >= 3) {
            int len = snprintf(dce->oper, MODEM_MAX_OPERATOR_LENGTH, "%s", p[2]);
            if (len > 2) {
                /* Strip "\r\n" */
                strip_cr_lf_tail(dce->oper, len);
                err = ESP_OK;
            }
        }
        if (i >= 4) {
            dce->act = (uint8_t)strtol(p[3], NULL, 0);
        }
        free(line_copy);
    }
    return err;
}

esp_err_t esp_modem_dce_handle_creg(modem_dce_t *dce, const char *line) {
    esp_err_t err = ESP_FAIL;

    // ESP_LOGI("AT CREG:", "%s", line);

    if (strstr(line, MODEM_RESULT_CODE_SUCCESS)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
    } else if (strstr(line, MODEM_RESULT_CODE_ERROR)) {
        err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
    } else if (strstr(line, "REG")) {
        char *start = strstr(line, "+");
        int matches = 0, status = 0, mode = 0;

        if (!start)
            return ESP_FAIL;

        if (strstr(line, "CREG:")) {
            if ((matches = sscanf(start, "+CREG: %d,%d", &mode, &status) == 2))
                dce->network_status.CREG = status;
        } else if (strstr(line, "CEREG:")) {
            if ((matches = sscanf(start, "+CEREG: %d,%d", &mode, &status) == 2))
                dce->network_status.CEREG = status;
        } else if (strstr(line, "CGREG:")) {
            if ((matches = sscanf(start, "+CGREG: %d,%d", &mode, &status) == 2))
                dce->network_status.CGREG = status;
        }

        return ESP_FAIL;
    }

    return err;
}

// esp_err_t esp_modem_dce_handle_creg(modem_dce_t *dce, const char *line)
// {
//     esp_err_t err = ESP_FAIL;

//     // ESP_LOGI("AT CREG:", "%s", line);

//     if (strstr(line, MODEM_RESULT_CODE_SUCCESS))
//     {
//         err = esp_modem_process_command_done(dce, MODEM_STATE_SUCCESS);
//     }
//     else if (strstr(line, MODEM_RESULT_CODE_ERROR))
//     {
//         err = esp_modem_process_command_done(dce, MODEM_STATE_FAIL);
//     }
//     else if (!strncmp(line, "+CREG", strlen("+CREG")))
//     {

//         size_t len = strlen(line);
//         char *line_copy = malloc(len + 1);
//         strcpy(line_copy, line);
//         /* +CREG: <mode>[, <stat>[....] */
//         char *str_ptr = NULL;
//         char *p[5];
//         uint8_t i = 0;
//         /* strtok will broke string by replacing delimiter with '\0' */
//         p[i] = strtok_r(line_copy, ",", &str_ptr);
//         while (p[i])
//         {
//             p[++i] = strtok_r(NULL, ",", &str_ptr);
//         }

//         if (i >= 2)
//         {
//             sscanf(p[1], "%u", &dce->attached);
//             dce->network_status.CREG = dce->attached;
//             //ESP_LOGI("CREG: ", "Attachment: %u", dce->attached);
//             err = ESP_OK;
//         }
//         free(line_copy);
//     }
//     return err;
// }

esp_err_t esp_modem_dce_get_check_attach(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_creg;
    DCE_CHECK(dte->send_cmd(dte, "AT+CREG?\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);

    dce->handle_line = esp_modem_dce_handle_creg;
    DCE_CHECK(dte->send_cmd(dte, "AT+CEREG?\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);

    dce->handle_line = esp_modem_dce_handle_creg;
    DCE_CHECK(dte->send_cmd(dte, "AT+CGREG?\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);

    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "CREG failed", err);
    ESP_LOGD(DCE_TAG, "Check attach ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_sync(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "AT\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "sync failed", err);
    ESP_LOGD(DCE_TAG, "sync ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_scan_network(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_netscan;
    DCE_CHECK(dte->send_cmd(dte, "AT+COPS=?\r", 180000) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "Modem not responding", err);

    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_power_test(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "AT\r", 100) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "Modem not responding", err);
    ESP_LOGD(DCE_TAG, "Modem on");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_echo(modem_dce_t *dce, bool on) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    if (on) {
        DCE_CHECK(dte->send_cmd(dte, "ATE1\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
        DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "enable echo failed", err);
        ESP_LOGD(DCE_TAG, "enable echo ok");
    } else {
        DCE_CHECK(dte->send_cmd(dte, "ATE0\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
        DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "disable echo failed", err);
        ESP_LOGD(DCE_TAG, "disable echo ok");
    }
    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CURCCFG=\"SYS\",1\r", MODEM_COMMAND_TIMEOUT_DEFAULT);
    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CURCCFG=\"SIMCARD\",1\r", MODEM_COMMAND_TIMEOUT_DEFAULT);
    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CURCCFG=\"SMS\",0\r", MODEM_COMMAND_TIMEOUT_DEFAULT);
    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CURCCFG=\"NETWORK\",1\r", MODEM_COMMAND_TIMEOUT_DEFAULT);

    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CREG=1\r", MODEM_COMMAND_TIMEOUT_DEFAULT);
    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CGREG=1\r", MODEM_COMMAND_TIMEOUT_DEFAULT);
    dce->handle_line = esp_modem_dce_handle_response_default;
    dte->send_cmd(dte, "AT+CEREG=1\r", MODEM_COMMAND_TIMEOUT_DEFAULT);

    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_store_profile(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "AT&W\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "save settings failed", err);
    ESP_LOGD(DCE_TAG, "save settings ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_set_flow_ctrl(modem_dce_t *dce, modem_flow_ctrl_t flow_ctrl) {
    modem_dte_t *dte = dce->dte;
    char command[16];
    int len = snprintf(command, sizeof(command), "AT+IFC=%d,%d\r", dte->flow_ctrl, flow_ctrl);
    DCE_CHECK(len < sizeof(command), "command too long: %s", err, command);
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, command, MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "set flow control failed", err);
    ESP_LOGD(DCE_TAG, "set flow control ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_define_pdp_context(modem_dce_t *dce, uint32_t cid, const char *type, const char *apn) {
    modem_dte_t *dte = dce->dte;
    char command[64];
    int len = snprintf(command, sizeof(command), "AT+CGDCONT=%d,\"%s\",\"%s\"\r", cid, type, apn);
    DCE_CHECK(len < sizeof(command), "command too long: %s", err, command);
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, command, MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "define pdp context failed", err);
    ESP_LOGI(DCE_TAG, "define pdp context ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_signal_quality(modem_dce_t *dce, uint32_t *rssi, uint32_t *ber) {
    modem_dte_t *dte = dce->dte;
    esp_modem_dce_t *esp_dce = __containerof(dce, esp_modem_dce_t, parent);
    uint32_t *resource[2] = {rssi, ber};
    esp_dce->priv_resource = resource;
    dce->handle_line = esp_modem_dce_handle_csq;
    DCE_CHECK(dte->send_cmd(dte, "AT+CSQ\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "inquire signal quality failed", err);
    ESP_LOGD(DCE_TAG, "inquire signal quality ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_battery_status(modem_dce_t *dce, uint32_t *bcs, uint32_t *bcl, uint32_t *voltage) {
    modem_dte_t *dte = dce->dte;
    esp_modem_dce_t *esp_dce = __containerof(dce, esp_modem_dce_t, parent);
    uint32_t *resource[3] = {bcs, bcl, voltage};
    esp_dce->priv_resource = resource;
    dce->handle_line = esp_modem_dce_handle_cbc;
    DCE_CHECK(dte->send_cmd(dte, "AT+CBC\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "inquire battery status failed", err);
    ESP_LOGD(DCE_TAG, "inquire battery status ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_module_name(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_cgmm;
    DCE_CHECK(dte->send_cmd(dte, "AT+CGMM\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "get module name failed", err);
    ESP_LOGD(DCE_TAG, "get module name ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_imei_number(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_cgsn;
    DCE_CHECK(dte->send_cmd(dte, "AT+CGSN\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "get imei number failed", err);
    ESP_LOGD(DCE_TAG, "get imei number ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_imsi_number(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_cimi;
    DCE_CHECK(dte->send_cmd(dte, "AT+CIMI\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "get imsi number failed", err);
    ESP_LOGD(DCE_TAG, "get imsi number ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_iccid_number(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_ccid;
    DCE_CHECK(dte->send_cmd(dte, "AT+CCID\r", MODEM_COMMAND_TIMEOUT_DEFAULT) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "get ccid number failed", err);
    ESP_LOGD(DCE_TAG, "get ccid number ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_get_operator_name(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_cops;
    DCE_CHECK(dte->send_cmd(dte, "AT+COPS?\r", MODEM_COMMAND_TIMEOUT_OPERATOR) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "get network operator failed", err);
    ESP_LOGD(DCE_TAG, "get network operator ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_hang_up(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "ATH\r", MODEM_COMMAND_TIMEOUT_HANG_UP) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "hang up failed", err);
    ESP_LOGD(DCE_TAG, "hang up ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_clear_fplmn(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "at+crsm=214,28539,0,0,12,\"FFFFFFFFFFFFFFFFFFFFFFFF\"\r", 8000) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "Clearing the fplmn list failed....", err);
    ESP_LOGD(DCE_TAG, "hang up ok");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_attach(modem_dce_t *dce, uint8_t autoSelect) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    if (autoSelect)
        DCE_CHECK(dte->send_cmd(dte, "AT+COPS=0\r", 120000) == ESP_OK, "send command failed", err);
    else
        DCE_CHECK(dte->send_cmd(dte, "AT+COPS=4,2,\"23801\",7\r", 20000) == ESP_OK, "send command failed", err);

    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "Could not start attach", err);
    ESP_LOGI(DCE_TAG, "Auto attach OK");
    return ESP_OK;
err:
    return ESP_FAIL;
}
esp_err_t esp_modem_dce_detach(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "AT+COPS=2\r", 500) == ESP_OK, "send command failed", err);

    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "Could not start attach", err);
    ESP_LOGD(DCE_TAG, "Auto attach OK");
    return ESP_OK;
err:
    return ESP_FAIL;
}

esp_err_t esp_modem_dce_set_default_bands(modem_dce_t *dce) {
    modem_dte_t *dte = dce->dte;
    dce->handle_line = esp_modem_dce_handle_response_default;
    DCE_CHECK(dte->send_cmd(dte, "AT+CBANDCFG=\"CAT-M\"=3,8,20\r", 500) == ESP_OK, "send command failed", err);
    DCE_CHECK(dce->state == MODEM_STATE_SUCCESS, "Failed to set bands", err);
    ESP_LOGD(DCE_TAG, "Set default bands OK");
    return ESP_OK;
err:
    ESP_LOGI(DCE_TAG, "Failed to set the bands ...... ");
    return ESP_FAIL;
}