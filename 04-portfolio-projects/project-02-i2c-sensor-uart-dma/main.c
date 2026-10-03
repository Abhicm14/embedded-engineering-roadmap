#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ring_buffer.h"
#include "bmp280.h"

static RingBuffer_t s_uart_rx_fifo;
static BMP280_Dev_t s_bmp_sensor;

/* Simulated Hardware I2C read/write stubs for cross-compilation testing */
static int8_t HW_I2C_Read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    (void)dev_addr; (void)reg_addr;
    /* Return simulated temperature and pressure bytes if hardware not attached */
    for (uint16_t i = 0; i < len; i++) {
        data[i] = (uint8_t)(0x50 + i);
    }
    return 0;
}

static int8_t HW_I2C_Write(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint16_t len) {
    (void)dev_addr; (void)reg_addr; (void)data; (void)len;
    return 0;
}

/* Transmit string over UART */
static void UART_SendString(const char *str) {
    printf("%s", str);
}

/* Process incoming line of command text */
static void CLI_ExecuteCommand(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        UART_SendString("\r\n=== Embedded CLI Commands ===\r\n");
        UART_SendString("  help      - Print available commands\r\n");
        UART_SendString("  temp      - Read temperature in Celsius\r\n");
        UART_SendString("  press     - Read barometric pressure in hPa\r\n");
        UART_SendString("  status    - Read hardware peripheral status\r\n");
        UART_SendString("=============================\r\n");
    } else if (strcmp(cmd, "temp") == 0) {
        float temp = 0.0f, press = 0.0f;
        BMP280_ReadTemperatureAndPressure(&s_bmp_sensor, &temp, &press);
        char buf[64];
        snprintf(buf, sizeof(buf), "\r\n[BMP280] Temperature: %.2f C\r\n", temp);
        UART_SendString(buf);
    } else if (strcmp(cmd, "press") == 0) {
        float temp = 0.0f, press = 0.0f;
        BMP280_ReadTemperatureAndPressure(&s_bmp_sensor, &temp, &press);
        char buf[64];
        snprintf(buf, sizeof(buf), "\r\n[BMP280] Pressure: %.2f hPa\r\n", press);
        UART_SendString(buf);
    } else if (strcmp(cmd, "status") == 0) {
        char buf[128];
        snprintf(buf, sizeof(buf), "\r\n[SYS] RingBuffer Free: %lu bytes, I2C: OK\r\n", 
                 (unsigned long)(RING_BUFFER_CAPACITY - RingBuffer_Available(&s_uart_rx_fifo)));
        UART_SendString(buf);
    } else if (strlen(cmd) > 0) {
        UART_SendString("\r\nUnknown command. Type 'help' for options.\r\n");
    }
    UART_SendString("\r\nembedded-cli> ");
}

int main(void) {
    RingBuffer_Init(&s_uart_rx_fifo);
    BMP280_Init(&s_bmp_sensor, BMP280_I2C_ADDR_PRIM, HW_I2C_Read, HW_I2C_Write);

    UART_SendString("\r\n==========================================\r\n");
    UART_SendString(" STM32 DMA UART CLI & I2C Sensor Driver \r\n");
    UART_SendString(" Type 'help' to see command listing.\r\n");
    UART_SendString("==========================================\r\n");
    UART_SendString("\r\nembedded-cli> ");

    char line_buffer[64];
    uint8_t line_idx = 0;

    /* Simulate feeding sample commands into the ring buffer */
    const char *test_input = "help\rtemp\rpress\rstatus\r";
    for (size_t i = 0; i < strlen(test_input); i++) {
        RingBuffer_Push(&s_uart_rx_fifo, (uint8_t)test_input[i]);
    }

    /* Main processing loop */
    uint8_t byte = 0;
    while (RingBuffer_Pop(&s_uart_rx_fifo, &byte)) {
        if (byte == '\r' || byte == '\n') {
            line_buffer[line_idx] = '\0';
            CLI_ExecuteCommand(line_buffer);
            line_idx = 0;
        } else if (line_idx < (sizeof(line_buffer) - 1)) {
            line_buffer[line_idx++] = (char)byte;
        }
    }

    return 0;
}
