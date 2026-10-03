# 🟡 Intermediate Portfolio Projects (Projects 3, 4 & 5)

---

## Project 3: Sensor Data Logger (I2C, SPI & DMA)

> **Core Focus:** I2C sensor driver implementation, Bosch calibration compensation, SPI NOR Flash page logging (W25Qxx), and DMA circular streaming.

### 1. Hardware Architecture
- **I2C Bus (PB6 SCL, PB7 SDA):** Bosch BMP280 Digital Barometric Pressure & Temperature Sensor ($4.7\text{ k}\Omega$ pull-up resistors).
- **SPI Bus (PA5 SCK, PA6 MISO, PA7 MOSI, PB0 CS):** Winbond W25Q64 64Mbit SPI Flash memory.
- **DMA Controller:** DMA1 Stream 5 routes continuous I2C data without blocking CPU time.

```
       STM32F401
    ┌─────────────┐       I2C Bus (400 kHz)
    │     PB6/PB7 ├─────────────────────────► [ Bosch BMP280 Sensor ]
    │             │
    │     PA5-PA7 │       SPI Bus (20 MHz)
    │        +PB0 ├─────────────────────────► [ Winbond W25Q64 Flash ]
    │             │
    │  DMA Stream ├────────(Zero CPU Overhead Data Moves)
    └─────────────┘
```

### 2. Flash Log Record Binary Schema
Each record is packed into a compact 16-byte aligned binary struct:
```c
typedef struct __attribute__((packed)) {
    uint32_t timestamp_ms; // 4 bytes: monotonic uptime
    int32_t  temp_centi_c; // 4 bytes: e.g. 2450 = 24.50 deg C
    uint32_t press_pa;     // 4 bytes: e.g. 101325 Pa
    uint16_t record_seq;   // 2 bytes: sequence counter
    uint16_t checksum;     // 2 bytes: CRC-16 checksum
} LogEntry_t; // Total: Exactly 16 bytes (16 records fit in one 256-byte Flash page!)
```

---

## Project 4: PWM Fan/Motor Controller with Closed-Loop PID

> **Core Focus:** Hardware timer PWM, tachometer pulse capture via Timer Input Capture, ADC target setpoint sampling, and discrete PID control loops.

### 1. Closed-Loop Control Architecture

```
 Target Speed (Potentiometer on ADC1 Ch0) ──┐
                                            ▼
                                  ┌───────────────────┐
                                  │   PID Controller  │ ──► Timer PWM Duty Cycle
                                  └─────────▲─────────┘          │
                                            │                    ▼
                                            │              [ N-MOS Driver ]
                                            │                    │
                                            │                    ▼
                                            └────────────── [ 12V DC Motor ]
                                      Measured RPM               │
                                  (Tachometer pulses             ▼
                                   on TIM3 Input Capture)  [ Hall Sensor ]
```

### 2. Discrete PID Implementation in C
```c
typedef struct {
    float Kp, Ki, Kd;
    float prev_error;
    float integral;
    float max_output;
} PID_Controller_t;

float PID_Update(PID_Controller_t *pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;
    pid->integral += error * dt;
    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error = error;

    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    if (output > pid->max_output) output = pid->max_output;
    if (output < 0.0f) output = 0.0f;
    return output;
}
```

---

## Project 5: FreeRTOS Environmental Monitor

> **Core Focus:** Preemptive real-time multitasking, Inter-Task Queues, Mutex with Priority Inheritance Protocol (PIP), Software Timers, and low-power Tickless Idle.

### 1. Multi-Task Concurrency Architecture

| Task Name | Priority | Period | Stack | Function |
| :--- | :---: | :---: | :---: | :--- |
| **`vSensorAcqTask`** | 3 (High) | 100 ms | 256 words | Polls BMP280 over I2C (mutex-protected) and posts to Queue. |
| **`vStorageTask`** | 2 (Medium) | Event-Driven | 512 words | Blocks on Queue; writes records in blocks to SPI Flash. |
| **`vOledTask`** | 1 (Low) | 500 ms | 256 words | Updates SSD1306 OLED display with running statistics. |
| **`vHeartbeat`** | Timer Daemon | 1000 ms | Shared | Toggles green status LED on timer daemon. |

```
 [ BMP280 Sensor ]
        │ (I2C Bus guarded by xI2CMutex)
        ▼
 ┌─────────────────┐
 │ vSensorAcqTask  │ (High Priority)
 └────────┬────────┘
          │ xQueueSend(xDataQueue, &sample, 10)
          ▼
 ┌─────────────────────────────────────────┐
 │          Sensor Sample Queue            │
 └────────────────────┬────────────────────┘
                      │ xQueueReceive(xDataQueue, &sample, portMAX_DELAY)
                      ▼
          ┌───────────────────────┐
          │     vStorageTask      │ (Medium Priority)
          └───────────┬───────────┘
                      │ SPI Bus
                      ▼
              [ SPI NOR Flash ]
```

---

## 🔌 PIC Intermediate Track: Projects P3, P4 & P5

### Project P3: Multi-Channel ADC Voltmeter & 16x2 Character LCD
- **Platform:** PIC16F877A on breadboard or PICSimLab (Board 1 with LCD HD44780).
- **Core Focus:** 10-bit Successive Approximation ADC configuration (`ADCON0`, `ADCON1`), acquisition delay (`Tacq`), and 4-bit nibble LCD driver on `PORTD`.
- **Hardware Architecture:**
  - Potentiometer connected to `RA0/AN0` (Pin 2).
  - HD44780 16x2 LCD: `RS` on `RD2`, `EN` on `RD3`, `D4:D7` on `RD4:RD7`.
- **Implementation Guide & Code:**
  - Step-by-Step Tutorial: [PIC Peripherals Mastery (Steps 4–8)](../pic-mplab-xc8/intermediate.md)
  - Standalone Code: [`code-examples/xc8/adc_voltage.c`](../code-examples/xc8/adc_voltage.c)
  - LCD Driver: [`code-examples/xc8/lcd_hello.c`](../code-examples/xc8/lcd_hello.c)

### Project P4: CCP1 Hardware PWM Dimmer & Motor Speed Controller
- **Platform:** PIC16F877A Capture/Compare/PWM (CCP1) module driven by Timer2.
- **Core Focus:** Calculating PR2 period register, 10-bit duty cycle resolution (`CCPR1L` and `CCP1CONbits.CCP1X/Y`), and generating flicker-free 1 kHz PWM.
- **Hardware Architecture:**
  - Output pin: `RC2/CCP1` (Pin 17) driving LED or low-side N-channel MOSFET motor stage.
- **Implementation Guide & Code:**
  - Standalone Code: [`code-examples/xc8/pwm_dimmer.c`](../code-examples/xc8/pwm_dimmer.c)

### Project P5: Hardware I2C Master Digital Sensor Monitor
- **Platform:** PIC16F877A MSSP hardware peripheral in I2C Master mode.
- **Core Focus:** Clock generation via `SSPADD`, start/stop state machine, ACK polling, and I2C transaction framing.
- **Hardware Architecture:**
  - `RC3` (SCL) and `RC4` (SDA) with 4.7kΩ pull-up resistors connected to Microchip TC74 or LM75 I2C temperature sensor.
- **Implementation Guide & Code:**
  - Standalone Code: [`code-examples/xc8/i2c_temp.c`](../code-examples/xc8/i2c_temp.c)
