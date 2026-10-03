# 📑 Official Datasheets & Silicon References

> Primary silicon documentation from chip manufacturers and IP designers (👶 Beginner-friendly, 💎 In-depth reference).

---

> ⚠️ **Mandatory Silicon Notice:** Microcontroller peripheral register base addresses, bitfield offsets, and timing values vary by exact silicon stepping and package. Always cross-reference the manufacturer Reference Manual for your specific chip part number!

---

## 1. ARM Architecture & Core Technical Reference

- 📑 💎 [**ARMv7-M Architecture Reference Manual (ARM DDI 0403E.e)**](https://developer.arm.com/documentation/ddi0403/latest/)  
  *Covers:* Cortex-M3, Cortex-M4, and Cortex-M7 cores. Complete specification of instruction sets (Thumb-2), NVIC registers, fault handling, and memory models.
- 📑 💎 [**Cortex-M4 Generic User Guide (ARM DUI 0553A)**](https://developer.arm.com/documentation/dui0553/latest/)  
  *Covers:* SysTick registers, MPU configuration, SCB (System Control Block), and FPU floating-point instructions.

---

## 2. STMicroelectronics Silicon References

- 📑 💎 [**STM32F401xB/C and STM32F401xD/E Reference Manual (RM0368)**](https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)  
  *Covers:* 850+ pages detailing STM32F401 RCC clock trees, GPIO registers, DMA streams, I2C, SPI, USART, and hardware timers.
- 📑 👶 [**STM32F401xC / STM32F401xE Datasheet**](https://www.st.com/resource/en/datasheet/stm32f401cd.pdf)  
  *Covers:* Electrical characteristics, absolute maximum ratings, pinout multiplexing tables (Alternate Functions AF0-AF15), and current consumption.
- 📑 💎 [**STM32 Cortex-M4 Programming Manual (PM0214)**](https://www.st.com/resource/en/programming_manual/pm0214-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf)  
  *Covers:* ST-specific core exceptions, interrupt priority grouping, and assembly instructions.

---

## 3. Sensor & Transceiver Datasheets

- 📑 👶 [**Bosch Sensortec BMP280 Digital Pressure Sensor Datasheet**](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp280-ds001.pdf)  
  *Covers:* I2C/SPI framing, register map (`0xD0` Chip ID, `0xF4` control), and factory trimming compensation formulas.
- 📑 👶 [**InvenSense MPU-6050 6-Axis MotionTracking Device Datasheet**](https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/)  
  *Covers:* 3-axis gyroscope, 3-axis accelerometer, I2C slave interface (`0x68`/`0x69`), and FIFO buffer.
- 📑 💎 [**Texas Instruments SN65HVD230 3.3V CAN Transceiver Datasheet**](https://www.ti.com/product/SN65HVD230)  
  *Covers:* High-speed CAN differential signaling, slope control resistor, loopback mode, and electrical protection specs.
