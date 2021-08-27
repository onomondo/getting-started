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

static esp_err_t i2c_master_read_slave_reg(i2c_port_t i2c_num, uint8_t i2c_addr, uint8_t i2c_reg, uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
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
    if (size > 1)
    {
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
static esp_err_t i2c_master_write_slave_reg(i2c_port_t i2c_num, uint8_t i2c_addr, uint8_t i2c_reg, uint8_t *data_wr, size_t size)
{
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

static esp_err_t standby(uint8_t enter)
{
    uint8_t _ctrl;
    readRegister(&_ctrl, KXTJ3_CTRL_REG1);

    if (enter)
        _ctrl &= 0x7E;
    else
        _ctrl |= (0x01 << 7); // disable standby-mode -> Bit7 = 1 = operating mode

    return writeRegister(KXTJ3_CTRL_REG1, &_ctrl);
}

esp_err_t acc_interrupt_init(uint16_t threshold, uint8_t moveDur, uint8_t naDur, uint8_t polarity);

esp_err_t acc_init()
{
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
    if (err != ESP_OK)
    {
        return err;
    }

    ESP_LOGI(TAG, "Install i2c driver");
    err = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);

    if (err != ESP_OK)
    {
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

    return ESP_OK;
}

///////////
//UNUSED///
///////////

esp_err_t acc_interrupt_init(uint16_t threshold, uint8_t moveDur, uint8_t naDur, uint8_t polarity)
{
    // Note that to properly change the value of this register, the PC1 bit in CTRL_REG1must first be set to “0”.
    standby(true);

    esp_err_t returnError = ESP_OK;

    // Build INT_CTRL_REG1

    uint8_t dataToWrite = 0x22; // Interrupt enabled, active LOW, non-latched

    if (polarity)
        dataToWrite |= (0x01 << 5); // Active HIGH

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

    dataToWrite = 0xBF; // enable interrupt on all axis any direction - Unlatched

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

void acc_resetInterrupt()
{
    uint8_t tmp;
    readRegister(&tmp, KXTJ3_INT_REL);

    if (tmp & 1 << 1)
    {
        ESP_LOGW(TAG, "Wakeup interrupt active...,%#08x", tmp);
    }
    else
    {
        ESP_LOGW(TAG, "Wakeup interrupt NOT active...,%#08x", tmp);
    }
}

float acc_read_data(int axis)
{
    int16_t outRAW;
    uint8_t regToRead = 0;
    switch (axis)
    {
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

    readRegister16((uint8_t *)&outRAW, regToRead);

    float outFloat;
    int accelRange = 2;
    switch (accelRange)
    {
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
