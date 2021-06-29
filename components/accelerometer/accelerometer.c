#include "accelerometer.h"

#include <stdio.h>

#include "driver/i2c.h"
#include "esp_log.h"
#include "stdint.h"

#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_FREQ_HZ

#define TAG "ACC"

#define ADDR 0x0E

#define READ_BIT 1
#define WRITE_BIT 0

#define ACK_VAL 0x0
#define NACK_VAL 0x1

#define readRegister(out, addr) i2c_master_read_slave_reg(I2C_NUM_1, ADDR, addr, out, 1)
#define readRegister16(out, addr) i2c_master_read_slave_reg(I2C_NUM_1, ADDR, addr, out, 2)
#define writeRegister(addr, out) i2c_master_write_slave_reg(I2C_NUM_1, ADDR, addr, out, 1)

/* PRIVATE VARIABLES */
uint8_t addr = 0x0F;

static esp_err_t i2c_master_read_slave_reg(i2c_port_t i2c_num, uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_rd, size_t size) {
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // first, send device address (indicating write) & register to be read
    i2c_master_write_byte(cmd, (i2c_addr << 1), 1);
    // send register we want
    i2c_master_write_byte(cmd, i2c_reg, 1);
    // Send repeated start
    i2c_master_start(cmd);
    // now send device address (indicating read) & read data
    i2c_master_write_byte(cmd, (i2c_addr << 1) | READ_BIT, 1);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write i2c slave device with registered interface
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 * ____________________________________________________________________________________
 * | start | slave_addr + wr_bit + ack | register + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------|----------------------|------|
 *
 */
static esp_err_t i2c_master_write_slave_reg(i2c_port_t i2c_num, uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_wr, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // first, send device address (indicating write) & register to be written
    i2c_master_write_byte(cmd, (i2c_addr << 1) | WRITE_BIT, 1);
    // send register we want
    i2c_master_write_byte(cmd, i2c_reg, 1);
    // write the data
    i2c_master_write(cmd, data_wr, size, 1);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t standby(uint8_t enter) {
    uint8_t _ctrl;
    readRegister(&_ctrl, KXTJ3_CTRL_REG1);

    if (enter)
        _ctrl &= 0x7E;
    else
        _ctrl |= (0x01 << 7);  // disable standby-mode -> Bit7 = 1 = operating mode

    return writeRegister(KXTJ3_CTRL_REG1, &_ctrl);
}

esp_err_t acc_interrupt_init(uint16_t threshold, uint8_t moveDur, uint8_t naDur, uint8_t polarity);

esp_err_t acc_init() {
    //wait two ms
    vTaskDelay(pdMS_TO_TICKS(2));

    int i2c_master_port = I2C_NUM_1;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = 100000,
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    ESP_LOGI(TAG, "Config i2c param");
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG, "Install i3c driver");
    err = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);

    if (err != ESP_OK) {
        return err;
    }

    //setup params and enable wakeup interrupt.
    uint8_t _whoAmI;
    ESP_LOGI(TAG, "Reading register....");
    err = i2c_master_read_slave_reg(I2C_NUM_1, ADDR, KXTJ3_WHO_AM_I, &_whoAmI, 1);

    uint8_t tmp = 0x00;
    writeRegister(0x1B, &tmp);

    //clear latched information
    readRegister(&tmp, 0x1A);

    //set output data rate
    tmp = 0x03;
    writeRegister(0x1D, &tmp);

    //set wake up counter... datarate is 3Hz, we want quick wakeup
    tmp = 0x01;
    writeRegister(0x29, &tmp);

    //0.5G threshold.. 0x08
    tmp = 0x02;
    writeRegister(0x6A, &tmp);
    tmp = 0x00;
    writeRegister(0x6B, &tmp);

    //setup hardware interrupt
    tmp = 0x30;
    writeRegister(0x1E, &tmp);

    //this should enable low res power save mode and wake up functionality..
    tmp = 0x02 | (1 << 7);
    writeRegister(0x1B, &tmp);

    //standby(1);

    // uint8_t tmp = 0;

    // //sample rate 3.125Hz
    // tmp |= 0x0A;
    // writeRegister(KXTJ3_DATA_CTRL_REG, &tmp);

    // //low power mode
    // tmp = 0x80;
    // //2G acc range
    // tmp |= (0x00 << 2);
    // writeRegister(KXTJ3_CTRL_REG1, &tmp);

    // err = acc_interrupt_init(100, 10, 10, 1);

    // if (err != ESP_OK) {
    //     ESP_LOGW(TAG, "Failed to initialize acc");
    // } else {
    //     ESP_LOGI(TAG, "Yey, Should be initialized by now!");
    // }
    // err = standby(0);

    return ESP_OK;
}

esp_err_t acc_interrupt_init(uint16_t threshold, uint8_t moveDur, uint8_t naDur, uint8_t polarity) {
    // Note that to properly change the value of this register, the PC1 bit in CTRL_REG1must first be set to “0”.
    standby(true);

    esp_err_t returnError = ESP_OK;

    // Build INT_CTRL_REG1

    uint8_t dataToWrite = 0x22;  // Interrupt enabled, active LOW, non-latched

    if (polarity)
        dataToWrite |= (0x01 << 5);  // Active HIGH

    //non latched intrupt
    dataToWrite |= 0x01 << 3;
    // _DEBBUG("KXTJ3_INT_CTRL_REG1: 0x", dataToWrite);
    returnError = writeRegister(KXTJ3_INT_CTRL_REG1, &dataToWrite);

    // WUFE – enables the Wake-Up (motion detect) function.

    uint8_t _reg1;

    returnError = readRegister(&_reg1, KXTJ3_CTRL_REG1);

    _reg1 |= (0x01 << 1);

    returnError = writeRegister(KXTJ3_CTRL_REG1, &_reg1);

    // Build INT_CTRL_REG2

    dataToWrite = 0xBF;  // enable interrupt on all axis any direction - Unlatched

    // _DEBBUG("KXTJ3_INT_CTRL_REG1: 0x", dataToWrite);
    returnError = writeRegister(KXTJ3_INT_CTRL_REG2, &dataToWrite);

    // Set WAKE-UP (motion detect) Threshold

    dataToWrite = (uint8_t)(threshold >> 4);

    // _DEBBUG("KXTJ3_WAKEUP_THRD_H: 0x", dataToWrite);
    returnError = writeRegister(KXTJ3_WAKEUP_THRD_H, &dataToWrite);

    dataToWrite = (uint8_t)(threshold << 4);

    // _DEBBUG("KXTJ3_WAKEUP_THRD_L: 0x", dataToWrite);
    returnError = writeRegister(KXTJ3_WAKEUP_THRD_L, &dataToWrite);

    // WAKEUP_COUNTER -> Sets the time motion must be present before a wake-up interrupt is set
    // WAKEUP_COUNTER (counts) = Wake-Up Delay Time (sec) x Wake-Up Function ODR(Hz)

    dataToWrite = moveDur;

    // _DEBBUG("KXTJ3_WAKEUP_COUNTER: 0x", dataToWrite);
    returnError = writeRegister(KXTJ3_WAKEUP_COUNTER, &dataToWrite);

    // Non-Activity register sets the non-activity time required before another wake-up interrupt will be reported.
    // NA_COUNTER (counts) = Non-ActivityTime (sec) x Wake-Up Function ODR(Hz)

    dataToWrite = naDur;

    // _DEBBUG("KXTJ3_NA_COUNTER: 0x", dataToWrite);
    returnError = writeRegister(KXTJ3_NA_COUNTER, &dataToWrite);

    // Set IMU to Operational mode
    returnError = standby(false);

    return returnError;
}

void acc_resetInterrupt() {
    uint8_t tmp;
    readRegister(&tmp, KXTJ3_INT_REL);

    if (tmp & 1 << 1) {
        ESP_LOGW(TAG, "Wakeup interrupt active...,%#08x", tmp);
    } else {
        ESP_LOGW(TAG, "Wakeup interrupt NOT active...,%#08x", tmp);
    }
}

float acc_read_data(int axis) {
    int16_t outRAW;
    uint8_t regToRead = 0;
    switch (axis) {
        case 0:
            // X axis
            regToRead = KXTJ3_OUT_X_L;
            break;
        case 1:
            // Y axis
            regToRead = KXTJ3_OUT_Y_L;
            break;
        case 2:
            // Z axis
            regToRead = KXTJ3_OUT_Z_L;
            break;

        default:
            // Not valid axis return NAN
            return 0;
            break;
    }

    readRegister16((uint8_t*)&outRAW, regToRead);

    float outFloat;
    int accelRange = 2;
    switch (accelRange) {
        case 2:
            outFloat = (float)outRAW / 15987;
            break;
        case 4:
            outFloat = (float)outRAW / 7840;
            break;
        case 8:
            outFloat = (float)outRAW / 3883;
            break;
        case 16:
            outFloat = (float)outRAW / 1280;
            break;
        default:
            outFloat = 0;
            break;
    }

    return outFloat;
}

//****************************************************************************//
//
//  Default construction is I2C mode, address 0x0E.
//
// //****************************************************************************//
// KXTJ3::KXTJ3(uint8_t inputArg = 0x0E) {
//     I2CAddress = inputArg;
// }

// kxtj3_status_t KXTJ3::begin(float SampleRate, uint8_t accRange) {
//     _DEBBUG("Configuring IMU");

//     kxtj3_status_t returnError = IMU_SUCCESS;

//     Wire.begin();

// // Start-up time, Figure 1: Typical StartUp Time - DataSheet
// #ifdef HIGH_RESOLUTION
//     if (SampleRate < 1)
//         delay(1300);
//     else if (SampleRate < 3)
//         delay(650);
//     else if (SampleRate < 6)
//         delay(350);
//     else if (SampleRate < 25)
//         delay(180);
//     else
//         delay(45);
// #else
//     delay(2);
// #endif

//     //Check the ID register to determine if the operation was a success.
//     uint8_t _whoAmI;

//     readRegister(&_whoAmI, KXTJ3_WHO_AM_I);

//     if (_whoAmI != 0x35) {
//         returnError = IMU_HW_ERROR;
//     }

//     accelSampleRate = SampleRate;
//     accelRange = accRange;

//     _DEBBUG("Apply settings");
//     applySettings();

//     return returnError;
// }

// //****************************************************************************//
// //  ReadRegisterRegion
// //
// //  Parameters:
// //    *outputPointer -- Pass &variable (base address of) to save read data to
// //    offset -- register to read
// //    length -- number of bytes to read
// //****************************************************************************//
// kxtj3_status_t KXTJ3::readRegisterRegion(uint8_t* outputPointer, uint8_t offset, uint8_t length) {
//     kxtj3_status_t returnError = IMU_SUCCESS;

//     //define pointer that will point to the external space
//     uint8_t i = 0;
//     uint8_t c = 0;

//     Wire.beginTransmission(I2CAddress);
//     offset |= 0x80;  //turn auto-increment bit on, bit 7 for I2C
//     Wire.write(offset);
//     if (Wire.endTransmission() != 0) {
//         returnError = IMU_HW_ERROR;
//     } else  //OK, all worked, keep going
//     {
//         // request 6 bytes from slave device
//         Wire.requestFrom(I2CAddress, length);
//         while ((Wire.available()) && (i < length))  // slave may send less than requested
//         {
//             c = Wire.read();  // receive a byte as character
//             *outputPointer = c;
//             outputPointer++;
//             i++;
//         }
//     }

//     return returnError;
// }

// //****************************************************************************//
// //  ReadRegister
// //
// //  Parameters:
// //    *outputPointer -- Pass &variable (address of) to save read data to
// //    offset -- register to read
// //****************************************************************************//
// kxtj3_status_t KXTJ3::readRegister(uint8_t* outputPointer, uint8_t offset) {
//     //Return value
//     uint8_t result = 0;
//     uint8_t numBytes = 1;
//     kxtj3_status_t returnError = IMU_SUCCESS;

//     Wire.beginTransmission(I2CAddress);
//     Wire.write(offset);

//     if (Wire.endTransmission() != 0) {
//         returnError = IMU_HW_ERROR;
//     }

//     Wire.requestFrom(I2CAddress, numBytes);

//     while (Wire.available())  // slave may send less than requested
//     {
//         result = Wire.read();  // receive a byte as a proper uint8_t
//     }

//     _DEBBUG("Read register 0x", offset, " = ", result);

//     *outputPointer = result;
//     return returnError;
// }

// //****************************************************************************//
// //  readRegisterInt16
// //
// //  Parameters:
// //    *outputPointer -- Pass &variable (base address of) to save read data to
// //    offset -- register to read
// //****************************************************************************//
// kxtj3_status_t KXTJ3::readRegisterInt16(int16_t* outputPointer, uint8_t offset) {
//     //offset |= 0x80; //turn auto-increment bit on
//     uint8_t myBuffer[2];
//     kxtj3_status_t returnError = readRegisterRegion(myBuffer, offset, 2);  //Does memory transfer
//     int16_t output = (int16_t)myBuffer[0] | int16_t(myBuffer[1] << 8);

//     _DEBBUG("12 bit from 0x", offset, " = ", output);
//     *outputPointer = output;
//     return returnError;
// }

// //****************************************************************************//
// //  writeRegister
// //
// //  Parameters:
// //    offset -- register to write
// //    dataToWrite -- 8 bit data to write to register
// //****************************************************************************//
// kxtj3_status_t KXTJ3::writeRegister(uint8_t offset, uint8_t dataToWrite) {
//     kxtj3_status_t returnError = IMU_SUCCESS;

//     //Write the byte
//     Wire.beginTransmission(I2CAddress);
//     Wire.write(offset);
//     Wire.write(dataToWrite);
//     if (Wire.endTransmission() != 0) {
//         returnError = IMU_HW_ERROR;
//     }

//     return returnError;
// }

// // Read axis acceleration as Float
// float KXTJ3::axisAccel(axis_t _axis) {
//     int16_t outRAW;
//     uint8_t regToRead = 0;
//     switch (_axis) {
//         case 0:
//             // X axis
//             regToRead = KXTJ3_OUT_X_L;
//             break;
//         case 1:
//             // Y axis
//             regToRead = KXTJ3_OUT_Y_L;
//             break;
//         case 2:
//             // Z axis
//             regToRead = KXTJ3_OUT_Z_L;
//             break;

//         default:
//             // Not valid axis return NAN
//             return NAN;
//             break;
//     }

//     readRegisterInt16(&outRAW, regToRead);

//     float outFloat;

//     switch (accelRange) {
//         case 2:
//             outFloat = (float)outRAW / 15987;
//             break;
//         case 4:
//             outFloat = (float)outRAW / 7840;
//             break;
//         case 8:
//             outFloat = (float)outRAW / 3883;
//             break;
//         case 16:
//             outFloat = (float)outRAW / 1280;
//             break;
//         default:
//             outFloat = 0;
//             break;
//     }

//     return outFloat;
// }

// kxtj3_status_t KXTJ3::standby(bool _en) {
//     uint8_t _ctrl;

//     // "Backup" KXTJ3_CTRL_REG1
//     readRegister(&_ctrl, KXTJ3_CTRL_REG1);

//     if (_en)
//         _ctrl &= 0x7E;
//     else
//         _ctrl |= (0x01 << 7);  // disable standby-mode -> Bit7 = 1 = operating mode

//     return writeRegister(KXTJ3_CTRL_REG1, _ctrl);
// }

// //****************************************************************************//
// //
// //  Apply settings passed to .begin();
// //
// //****************************************************************************//
// void KXTJ3::applySettings(void) {
//     uint8_t dataToWrite = 0;  //Temporary variable

//     standby(true);

//     //Build DATA_CTRL_REG

//     //  Convert ODR
//     if (accelSampleRate < 1)
//         dataToWrite |= 0x08;  // 0.781Hz
//     else if (accelSampleRate < 2)
//         dataToWrite |= 0x09;  // 1.563Hz
//     else if (accelSampleRate < 4)
//         dataToWrite |= 0x0A;  // 3.125Hz
//     else if (accelSampleRate < 8)
//         dataToWrite |= 0x0B;  // 6.25Hz
//     else if (accelSampleRate < 16)
//         dataToWrite |= 0x00;  // 12.5Hz
//     else if (accelSampleRate < 30)
//         dataToWrite |= 0x01;  // 25Hz
//     else if (accelSampleRate < 60)
//         dataToWrite |= 0x02;  // 50Hz
//     else if (accelSampleRate < 150)
//         dataToWrite |= 0x03;  // 100Hz
//     else if (accelSampleRate < 250)
//         dataToWrite |= 0x04;  // 200Hz
//     else if (accelSampleRate < 450)
//         dataToWrite |= 0x05;  // 400Hz
//     else if (accelSampleRate < 850)
//         dataToWrite |= 0x06;  // 800Hz
//     else
//         dataToWrite |= 0x07;  // 1600Hz

//     //Now, write the patched together data
//     _DEBBUG("KXTJ3_DATA_CTRL_REG: 0x", dataToWrite);
//     writeRegister(KXTJ3_DATA_CTRL_REG, dataToWrite);

//     //Build CTRL_REG1

//     // LOW power, 8-bit mode
//     dataToWrite = 0x80;

// #ifdef HIGH_RESOLUTION
//     dataToWrite = 0xC0;
// #endif

//     //  Convert scaling
//     switch (accelRange) {
//         default:
//         case 2:
//             dataToWrite |= (0x00 << 2);
//             break;
//         case 4:
//             dataToWrite |= (0x02 << 2);
//             break;
//         case 8:
//             dataToWrite |= (0x04 << 2);
//             break;
//         case 16:
//             dataToWrite |= (0x01 << 2);
//             break;
//     }

//     //Now, write the patched together data
//     _DEBBUG("KXTJ3_CTRL_REG1: 0x", dataToWrite);
//     writeRegister(KXTJ3_CTRL_REG1, dataToWrite);
// }

// //****************************************************************************//
// //  Configure interrupt, stop or move, threshold and duration
// //	Durationsteps and maximum values depend on the ODR chosen.
// //****************************************************************************//
// kxtj3_status_t KXTJ3::intConf(uint16_t threshold, uint8_t moveDur, uint8_t naDur, bool polarity) {
//     // Note that to properly change the value of this register, the PC1 bit in CTRL_REG1must first be set to “0”.
//     standby(true);

//     kxtj3_status_t returnError = IMU_SUCCESS;

//     // Build INT_CTRL_REG1

//     uint8_t dataToWrite = 0x22;  // Interrupt enabled, active LOW, non-latched

//     if (polarity == HIGH)
//         dataToWrite |= (0x01 << 5);  // Active HIGH

//     _DEBBUG("KXTJ3_INT_CTRL_REG1: 0x", dataToWrite);
//     returnError = writeRegister(KXTJ3_INT_CTRL_REG1, dataToWrite);

//     // WUFE – enables the Wake-Up (motion detect) function.

//     uint8_t _reg1;

//     returnError = readRegister(&_reg1, KXTJ3_CTRL_REG1);

//     _reg1 |= (0x01 << 1);

//     returnError = writeRegister(KXTJ3_CTRL_REG1, _reg1);

//     // Build INT_CTRL_REG2

//     dataToWrite = 0xBF;  // enable interrupt on all axis any direction - Unlatched

//     _DEBBUG("KXTJ3_INT_CTRL_REG1: 0x", dataToWrite);
//     returnError = writeRegister(KXTJ3_INT_CTRL_REG2, dataToWrite);

//     // Set WAKE-UP (motion detect) Threshold

//     dataToWrite = (uint8_t)(threshold >> 4);

//     _DEBBUG("KXTJ3_WAKEUP_THRD_H: 0x", dataToWrite);
//     returnError = writeRegister(KXTJ3_WAKEUP_THRD_H, dataToWrite);

//     dataToWrite = (uint8_t)(threshold << 4);

//     _DEBBUG("KXTJ3_WAKEUP_THRD_L: 0x", dataToWrite);
//     returnError = writeRegister(KXTJ3_WAKEUP_THRD_L, dataToWrite);

//     // WAKEUP_COUNTER -> Sets the time motion must be present before a wake-up interrupt is set
//     // WAKEUP_COUNTER (counts) = Wake-Up Delay Time (sec) x Wake-Up Function ODR(Hz)

//     dataToWrite = moveDur;

//     _DEBBUG("KXTJ3_WAKEUP_COUNTER: 0x", dataToWrite);
//     returnError = writeRegister(KXTJ3_WAKEUP_COUNTER, dataToWrite);

//     // Non-Activity register sets the non-activity time required before another wake-up interrupt will be reported.
//     // NA_COUNTER (counts) = Non-ActivityTime (sec) x Wake-Up Function ODR(Hz)

//     dataToWrite = naDur;

//     _DEBBUG("KXTJ3_NA_COUNTER: 0x", dataToWrite);
//     returnError = writeRegister(KXTJ3_NA_COUNTER, dataToWrite);

//     // Set IMU to Operational mode
//     returnError = standby(false);

//     return returnError;
// }
