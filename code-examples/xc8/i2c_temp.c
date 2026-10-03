/**
 * @file i2c_temp.c
 * @brief Hardware I2C Master reading a digital temperature sensor (TC74 / LM75)
 * 
 * Demonstrates:
 * - Master Synchronous Serial Port (MSSP) configuration in I2C Master Mode
 * - Proper bus state wait polling (SSPCON2 idle checks and SSPIF handling)
 * - Start, Repeated Start, Byte Write with ACK/NACK, and Stop sequences
 * - Reading ambient temperature from an industry I2C sensor (e.g., Microchip TC74)
 * 
 * Hardware Connections:
 * - SCL: RC3 (Pin 18) with 4.7k ohm pull-up to +5V
 * - SDA: RC4 (Pin 23) with 4.7k ohm pull-up to +5V
 * - TC74A0 I2C 7-bit Address = 0x48 (0b1001000)
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

#define TC74_ADDR 0x48          // Microchip TC74A0 7-bit I2C address

void I2C_WaitIdle(void) {
    // Wait until start, stop, read, ack sequences complete and transmit is idle
    while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_nW));
}

void I2C_Master_Init(void) {
    // 1. In I2C mode, pins must be configured as inputs to allow open-drain operation
    TRISCbits.TRISC3 = 1;       // RC3 = SCL
    TRISCbits.TRISC4 = 1;       // RC4 = SDA

    // 2. Set Clock Speed to 100 kHz at 4 MHz Fosc
    // Clock = Fosc / (4 * (SSPADD + 1))
    // SSPADD = (4000000 / (4 * 100000)) - 1 = 9
    SSPADD = 9;

    // 3. Configure SSPSTAT: Slew rate disabled for standard 100 kHz mode
    SSPSTATbits.SMP = 1;

    // 4. Configure SSPCON:
    // SSPEN = 1 (Enable MSSP)
    // SSPM3:SSPM0 = 1000 (I2C Master mode, clock = Fosc / (4 * (SSPADD + 1)))
    SSPCON = 0b00101000;
}

void I2C_Start(void) {
    I2C_WaitIdle();
    SSPCON2bits.SEN = 1;        // Initiate Start condition on SDA and SCL
    while (SSPCON2bits.SEN);    // Hardware clears bit when Start condition completes
}

void I2C_RepeatedStart(void) {
    I2C_WaitIdle();
    SSPCON2bits.RSEN = 1;       // Initiate Repeated Start condition
    while (SSPCON2bits.RSEN);
}

void I2C_Stop(void) {
    I2C_WaitIdle();
    SSPCON2bits.PEN = 1;        // Initiate Stop condition
    while (SSPCON2bits.PEN);
}

unsigned char I2C_Write(unsigned char data) {
    I2C_WaitIdle();
    SSPBUF = data;              // Load byte into transmit buffer
    while (SSPSTATbits.BF);     // Wait until buffer transmission completes
    I2C_WaitIdle();
    return !SSPCON2bits.ACKSTAT;// Returns 1 if ACK received (ACKSTAT=0), 0 if NACK
}

unsigned char I2C_Read(unsigned char ack_bit) {
    unsigned char received_byte;
    I2C_WaitIdle();
    
    SSPCON2bits.RCEN = 1;       // Enable receive mode for I2C
    while (!SSPSTATbits.BF);    // Wait until buffer receives full 8 bits
    received_byte = SSPBUF;
    
    I2C_WaitIdle();
    SSPCON2bits.ACKDT = (ack_bit ? 0 : 1); // 0 = Send ACK, 1 = Send NACK
    SSPCON2bits.ACKEN = 1;      // Transmit ACK/NACK sequence
    while (SSPCON2bits.ACKEN);
    
    return received_byte;
}

signed char TC74_ReadTemp(void) {
    signed char temp = 0;

    I2C_Start();
    // Send 7-bit slave address + Write bit (0)
    if (I2C_Write((TC74_ADDR << 1) | 0)) {
        // Send command code 0x00 (RTR - Read Temperature Register)
        I2C_Write(0x00);
        
        // Repeated Start to switch from Write to Read
        I2C_RepeatedStart();
        
        // Send 7-bit slave address + Read bit (1)
        I2C_Write((TC74_ADDR << 1) | 1);
        
        // Read 1-byte 8-bit two's complement temperature and send NACK
        temp = (signed char)I2C_Read(0);
    }
    I2C_Stop();

    return temp;
}

void main(void) {
    I2C_Master_Init();

    // Use PORTB for visual binary display of temperature
    TRISB = 0x00;
    PORTB = 0x00;

    while (1) {
        signed char current_temp = TC74_ReadTemp();
        PORTB = (unsigned char)current_temp; // Display raw temp on PORTB LEDs
        __delay_ms(1000);
    }
}
