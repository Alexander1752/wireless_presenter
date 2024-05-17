#include "mpu6050.h"

void MPU6050_start() {
    twi_start();
    twi_write(MPU6050_ADDR << 1);
    twi_write(PWR_MGMT_1);
    twi_write(0b00001000);

    twi_stop();
}

void MPU6050_stop() {
    twi_start();
    twi_write(MPU6050_ADDR << 1);
    twi_write(PWR_MGMT_1);
    twi_write(0b00000000);

    twi_stop();
}

void MPU6050_read8(int8_t reg, int8_t* data) {
    twi_start();
    twi_write(MPU6050_ADDR << 1);
    twi_write(reg);

    twi_start();
    twi_write(MPU6050_ADDR << 1 | 1);
    twi_read_nack(data);

    twi_stop();

    return;
}

void MPU6050_read16(int16_t reg, int16_t* data) {
    MPU6050_read8(reg & 0xFF, (int8_t*)data);
    MPU6050_read8((reg >> 8) & 0xFF, (int8_t*)data + 1);

    return;
}
