#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <twi.h>

#define MPU6050_ADDR 0x68

#define AX_REG_H ((int8_t)59)
#define AX_REG_L ((int8_t)60)
#define AX_REG ((((int16_t)AX_REG_H) << 8) | AX_REG_L)

#define AY_REG_H ((int8_t)61)
#define AY_REG_L ((int8_t)62)
#define AY_REG ((((int16_t)AY_REG_H) << 8) | AY_REG_L)

#define AZ_REG_H ((int8_t)63)
#define AZ_REG_L ((int8_t)64)
#define AZ_REG ((((int16_t)AZ_REG_H) << 8) | AZ_REG_L)

#define GX_REG_H ((int8_t)67)
#define GX_REG_L ((int8_t)68)
#define GX_REG ((((int16_t)GX_REG_H) << 8) | GX_REG_L)

#define GY_REG_H ((int8_t)69)
#define GY_REG_L ((int8_t)70)
#define GY_REG ((((int16_t)GY_REG_H) << 8) | GY_REG_L)

#define GZ_REG_H ((int8_t)71)
#define GZ_REG_L ((int8_t)72)
#define GZ_REG ((((int16_t)GZ_REG_H) << 8) | GZ_REG_L)

#define PWR_MGMT_1 107

void MPU6050_start();

void MPU6050_stop();

void MPU6050_read8(int8_t reg, int8_t* data);

void MPU6050_read16(int16_t reg, int16_t* data);

#ifdef __cplusplus
}
#endif
