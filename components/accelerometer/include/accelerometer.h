
#pragma once

#include "esp_err.h"
#include "esp_log.h"
#include "esp_types.h"
#ifdef __cplusplus
extern "C" {
#endif

/*KXTJ3-1057*/

esp_err_t acc_init();

float acc_read_data(int axis);

void acc_resetInterrupt();

//Device Registers
#define KXTJ3_WHO_AM_I 0x0F
#define KXTJ3_DCST_RESP 0x0C  // used to verify proper integrated circuit functionality.It always has a byte value of 0x55

#define KXTJ3_OUT_X_L 0x06
#define KXTJ3_OUT_X_H 0x07
#define KXTJ3_OUT_Y_L 0x08
#define KXTJ3_OUT_Y_H 0x09
#define KXTJ3_OUT_Z_L 0x0A
#define KXTJ3_OUT_Z_H 0x0B

#define KXTJ3_STATUS 0x18
#define KXTJ3_INT_SOURCE1 0x16
#define KXTJ3_INT_SOURCE2 0x17

#define KXTJ3_CTRL_REG1 0x1B  // *
#define KXTJ3_CTRL_REG2 0x1D  // *

#define KXTJ3_INT_CTRL_REG1 0x1E  // *
#define KXTJ3_INT_CTRL_REG2 0x1F  // *

#define KXTJ3_DATA_CTRL_REG 0x21   // *
#define KXTJ3_WAKEUP_COUNTER 0x29  // *
#define KXTJ3_NA_COUNTER 0x2A      // *

#define KXTJ3_WAKEUP_THRD_H 0x6A  // *
#define KXTJ3_WAKEUP_THRD_L 0x6B  // *

#define KXTJ3_INT_REL 0x1A

#ifdef __cplusplus
}
#endif
