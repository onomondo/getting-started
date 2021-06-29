#include "temperatureSensor.h"

#include <stdio.h>

#include "driver/i2c.h"

#define TAG "TMP"

#define ADDR 0b1001000
#define READ_BIT 1
#define WRITE_BIT 0
#define ACK_VAL 0x0
#define NACK_VAL 0x1

#define readRegister(out, addr) i2c_master_read_slave_reg(I2C_NUM_1, ADDR, addr, out, 1)
#define readRegister16(out, addr) i2c_master_read_slave_reg(I2C_NUM_1, ADDR, addr, out, 2)
#define writeRegister(addr, out) i2c_master_write_slave_reg(I2C_NUM_1, ADDR, addr, out, 1)
#define writeRegister16(addr, out) i2c_master_write_slave_reg(I2C_NUM_1, ADDR, addr, (uint8_t*)out, 2)

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

esp_err_t tmp_init() {
    uint8_t conf = 0b11000000;  //single shot, 11bit res.

    return writeRegister(TMP75_RCONF, &conf);
}

esp_err_t tmp_read(float* temperature, float* humidity) {
    uint8_t T_buffer[2];
    uint16_t temp = 0;

    int n = 0;
    esp_err_t err;
    while (n < 10 && temp == 0) {
        err = readRegister16(T_buffer, TMP75_RTEMP);

        if (err == ESP_OK) {
            temp = ((T_buffer[0] << 8) | T_buffer[1]) >> 4;
            *temperature = (float)temp * 0.0625f;
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
    return err;
}
