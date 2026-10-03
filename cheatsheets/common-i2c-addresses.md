# 📍 Common I2C 7-Bit Device Address Reference

> Quick lookup table for common embedded sensors, displays, real-time clocks, and EEPROMs.

---

| 7-Bit Address (Hex) | Common IC / Device | Typical Function | Address Pin Config |
| :---: | :--- | :--- | :--- |
| **`0x18` / `0x19`** | LIS3DH | 3-Axis Accelerometer | `SA0` pin to GND (`0x18`) or VDD (`0x19`) |
| **`0x20` - `0x27`** | PCF8574 / MCP23008 | 8-Bit I/O Port Expander | Configurable via `A0, A1, A2` pins |
| **`0x38`** | AHT10 / AHT20 | Temperature & Humidity Sensor | Fixed address |
| **`0x3C` / `0x3D`** | SSD1306 / SH1106 | 0.96" OLED Display (128x64) | `SA0` / `DC` pin to GND (`0x3C`) or VDD (`0x3D`) |
| **`0x40`** | INA219 | High-Side DC Current & Voltage Shunt | `A0, A1` pins to GND |
| **`0x44` / `0x45`** | SHT31 | Precision Humidity & Temp | `ADDR` pin to GND (`0x44`) or VDD (`0x45`) |
| **`0x48` - `0x4B`** | ADS1115 / LM75 | 16-Bit 4-Channel Precision ADC | `ADDR` pin to GND (`0x48`), VDD (`0x49`) |
| **`0x50` - `0x57`** | AT24C32 / AT24C64 | I2C Serial EEPROM (32K/64K) | Configurable via `A0, A1, A2` pins |
| **`0x5A`** | MLX90614 | Non-Contact Infrared Thermometer | Factory default |
| **`0x68`** | DS1307 / DS3231 | High-Precision Real-Time Clock (RTC)| Fixed address |
| **`0x68` / `0x69`** | MPU-6050 / MPU-9250 | 6-Axis / 9-Axis Motion IMU | `AD0` pin to GND (`0x68`) or VDD (`0x69`) |
| **`0x76` / `0x77`** | BMP280 / BME280 / BME680 | Pressure, Humidity, Temperature | `SDO` pin to GND (`0x76`) or VDD (`0x77`) |
| **`0x70` - `0x77`** | TCA9548A | 8-Channel I2C Bus Multiplexer | Configurable via `A0, A1, A2` pins |

> 💡 **Tip:** Remember that 7-bit addresses are shifted left by 1 bit on the wire (`Address << 1 | R/W bit`). For example, BMP280 at `0x76` transmits as `0xEC` on write and `0xED` on read!
