# 🔌 PIC16F877A Custom Driver Development

> **Your Personal Driver Suite for the Microchip PIC16F877A**  
> Target Simulator: **PICSimLab (Board 1: PICGenios or Breadboard)** | Toolchain: **MPLAB X IDE + XC8**

---

## 🎯 Peripheral Driver Checklist

Track your progress as you write your own drivers from the PIC16F877A silicon datasheet:

- [ ] **1. GPIO Digital Output Driver (`drivers/pic_gpio_out.c`)**
  - Controls LEDs on PORTB/PORTD.
  - Registers: `TRISB`, `PORTB`.
- [ ] **2. GPIO Digital Input with Weak Pull-Ups (`drivers/pic_gpio_in.c`)**
  - Debounced pushbutton reading on RB0.
  - Registers: `TRISB`, `OPTION_REGbits.nRBPU`, `PORTB`.
- [ ] **3. Matrix Keypad Scanner (`drivers/pic_keypad.c`)**
  - $4 \times 4$ keypad row-driving and column-reading.
  - Registers: `TRISD`, `PORTD`.
- [ ] **4. Hardware Timer0 Interrupt Driver (`drivers/pic_timer0.c`)**
  - Accurate periodic tick interrupt at $4\text{ MHz}$.
  - Registers: `OPTION_REG`, `TMR0`, `INTCON`.
- [ ] **5. Hardware Timer1 Driver (`drivers/pic_timer1.c`)**
  - 16-bit timing / external counter.
  - Registers: `T1CON`, `TMR1H`, `TMR1L`, `PIE1bits.TMR1IE`.
- [ ] **6. 10-Bit ADC Driver (`drivers/pic_adc.c`)**
  - Reads analog voltage from potentiometer on RA0/AN0.
  - Registers: `TRISA`, `ADCON0`, `ADCON1`, `ADRESH`, `ADRESL`.
- [ ] **7. Hardware PWM Driver (`drivers/pic_pwm.c`)**
  - Variable duty cycle PWM on RC2 (CCP1) driven by Timer2.
  - Registers: `TRISC`, `PR2`, `T2CON`, `CCP1CON`, `CCPR1L`.
- [ ] **8. USART Serial Driver (`drivers/pic_uart.c`)**
  - 9600 baud full-duplex communication with host PC CLI.
  - Registers: `TRISC`, `SPBRG`, `TXSTA`, `RCSTA`, `TXREG`, `RCREG`.
- [ ] **9. Master I2C Driver (`drivers/pic_i2c.c`)**
  - Master Synchronous Serial Port reading RTC or external EEPROM on RC3/RC4.
  - Registers: `TRISC`, `SSPCON`, `SSPCON2`, `SSPSTAT`, `SSPADD`, `SSPBUF`.
- [ ] **10. Master SPI Driver (`drivers/pic_spi.c`)**
  - High-speed synchronous serial data transfer on RC3/RC4/RC5.
  - Registers: `TRISC`, `SSPCON`, `SSPSTAT`, `SSPBUF`.
- [ ] **11. HD44780 4-Bit Alphanumeric LCD Driver (`drivers/pic_lcd.c`)**
  - 16x2 character display control on PORTD.
  - Data bus `RD4-RD7`, Control pins `RD2 (RS)`, `RD3 (EN)`.
- [ ] **12. Internal Non-Volatile EEPROM Driver (`drivers/pic_eeprom.c`)**
  - Reading and writing calibration parameters with flash unlock sequence.
  - Registers: `EEADR`, `EEDATA`, `EECON1`, `EECON2`.

---

## 🧭 How to Begin

1. Read the step-by-step instructions in [**`pic-mplab-xc8/datasheet-driver-guide.md`**](../../pic-mplab-xc8/datasheet-driver-guide.md).
2. Check the starter template in [`templates/my_gpio_driver_template.h`](templates/my_gpio_driver_template.h) and [`templates/my_gpio_driver_template.c`](templates/my_gpio_driver_template.c).
3. Save your completed driver files into `drivers/`.
4. Compile your MPLAB X project, load the generated `.hex` file into **PICSimLab**, and test!
