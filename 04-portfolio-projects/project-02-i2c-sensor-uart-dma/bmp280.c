#include "bmp280.h"

#define REG_CHIP_ID       (0xD0)
#define REG_RESET         (0xE0)
#define REG_CTRL_MEAS     (0xF4)
#define REG_CONFIG        (0xF5)
#define REG_PRESS_MSB     (0xF7)
#define REG_CALIB_START   (0x88)

bool BMP280_Init(BMP280_Dev_t *dev, uint8_t i2c_addr, bmp280_read_fptr_t read_fn, bmp280_write_fptr_t write_fn) {
    if (!dev || !read_fn || !write_fn) return false;
    dev->dev_addr = i2c_addr;
    dev->read     = read_fn;
    dev->write    = write_fn;
    dev->t_fine   = 0;

    /* 1. Read Chip ID */
    uint8_t chip_id = 0;
    if (dev->read(dev->dev_addr, REG_CHIP_ID, &chip_id, 1) != 0 || chip_id != BMP280_CHIP_ID) {
        return false; /* Sensor not found or communication failed */
    }

    /* 2. Read Factory Trimming Parameters (24 bytes) */
    uint8_t calib_buf[24];
    if (dev->read(dev->dev_addr, REG_CALIB_START, calib_buf, 24) != 0) {
        return false;
    }

    dev->calib.dig_T1 = (uint16_t)(((uint16_t)calib_buf[1] << 8) | calib_buf[0]);
    dev->calib.dig_T2 = (int16_t)(((int16_t)calib_buf[3] << 8) | calib_buf[2]);
    dev->calib.dig_T3 = (int16_t)(((int16_t)calib_buf[5] << 8) | calib_buf[4]);

    dev->calib.dig_P1 = (uint16_t)(((uint16_t)calib_buf[7] << 8) | calib_buf[6]);
    dev->calib.dig_P2 = (int16_t)(((int16_t)calib_buf[9] << 8) | calib_buf[8]);
    dev->calib.dig_P3 = (int16_t)(((int16_t)calib_buf[11] << 8) | calib_buf[10]);
    dev->calib.dig_P4 = (int16_t)(((int16_t)calib_buf[13] << 8) | calib_buf[12]);
    dev->calib.dig_P5 = (int16_t)(((int16_t)calib_buf[15] << 8) | calib_buf[14]);
    dev->calib.dig_P6 = (int16_t)(((int16_t)calib_buf[17] << 8) | calib_buf[16]);
    dev->calib.dig_P7 = (int16_t)(((int16_t)calib_buf[19] << 8) | calib_buf[18]);
    dev->calib.dig_P8 = (int16_t)(((int16_t)calib_buf[21] << 8) | calib_buf[20]);
    dev->calib.dig_P9 = (int16_t)(((int16_t)calib_buf[23] << 8) | calib_buf[22]);

    /* 3. Configure Normal mode: osrs_t x2, osrs_p x16, normal mode (0x57) */
    uint8_t ctrl_meas = (0x02 << 5) | (0x05 << 2) | 0x03;
    if (dev->write(dev->dev_addr, REG_CTRL_MEAS, &ctrl_meas, 1) != 0) {
        return false;
    }

    return true;
}

static int32_t Compensate_Temperature(BMP280_Dev_t *dev, int32_t adc_T) {
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dev->calib.dig_T1 << 1))) * ((int32_t)dev->calib.dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dev->calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)dev->calib.dig_T1))) >> 12) * ((int32_t)dev->calib.dig_T3)) >> 14;
    dev->t_fine = var1 + var2;
    return (dev->t_fine * 5 + 128) >> 8;
}

static uint32_t Compensate_Pressure(BMP280_Dev_t *dev, int32_t adc_P) {
    int64_t var1 = ((int64_t)dev->t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)dev->calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)dev->calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)dev->calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dev->calib.dig_P3) >> 8) + ((var1 * (int64_t)dev->calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dev->calib.dig_P1) >> 33;

    if (var1 == 0) return 0; /* Avoid division by zero */

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dev->calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dev->calib.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dev->calib.dig_P7) << 4);

    return (uint32_t)p;
}

bool BMP280_ReadTemperatureAndPressure(BMP280_Dev_t *dev, float *temperature_c, float *pressure_hpa) {
    if (!dev || !temperature_c || !pressure_hpa) return false;

    uint8_t raw_data[6];
    if (dev->read(dev->dev_addr, REG_PRESS_MSB, raw_data, 6) != 0) {
        return false;
    }

    int32_t adc_P = (int32_t)((((uint32_t)raw_data[0]) << 12) | (((uint32_t)raw_data[1]) << 4) | ((uint32_t)raw_data[2] >> 4));
    int32_t adc_T = (int32_t)((((uint32_t)raw_data[3]) << 12) | (((uint32_t)raw_data[4]) << 4) | ((uint32_t)raw_data[5] >> 4));

    int32_t temp_comp = Compensate_Temperature(dev, adc_T);
    uint32_t press_comp = Compensate_Pressure(dev, adc_P);

    *temperature_c = (float)temp_comp / 100.0f;
    *pressure_hpa  = (float)press_comp / 25600.0f;

    return true;
}
