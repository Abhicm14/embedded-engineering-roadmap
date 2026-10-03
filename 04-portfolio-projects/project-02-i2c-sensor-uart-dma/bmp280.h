#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>
#include <stdbool.h>

#define BMP280_I2C_ADDR_PRIM   (0x76)
#define BMP280_I2C_ADDR_SEC    (0x77)
#define BMP280_CHIP_ID         (0x58)

/* BMP280 Calibration Trimming Parameters Structure */
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} BMP280_CalibData_t;

/* I2C Read/Write Function Pointer Prototypes */
typedef int8_t (*bmp280_read_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef int8_t (*bmp280_write_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint16_t len);

typedef struct {
    uint8_t              dev_addr;
    bmp280_read_fptr_t   read;
    bmp280_write_fptr_t  write;
    BMP280_CalibData_t   calib;
    int32_t              t_fine;
} BMP280_Dev_t;

/* Public API */
bool BMP280_Init(BMP280_Dev_t *dev, uint8_t i2c_addr, bmp280_read_fptr_t read_fn, bmp280_write_fptr_t write_fn);
bool BMP280_ReadTemperatureAndPressure(BMP280_Dev_t *dev, float *temperature_c, float *pressure_hpa);

#endif /* BMP280_H */
