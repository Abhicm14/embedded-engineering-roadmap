/**
 * @file lcd_hello.c
 * @brief 4-Bit HD44780 alphanumeric LCD driver and greeting demo on PIC16F877A
 * 
 * Demonstrates:
 * - Direct register manipulation of PORTD lines to control HD44780 LCD
 * - Proper power-on initialization sequence in 4-bit nibble mode
 * - Command vs Data bus transactions via RS and EN strobe lines
 * - Custom coordinate positioning (DDRAM addresses 0x80 and 0xC0)
 * 
 * Hardware Connections (Standard PICSimLab Board 1 mapping):
 * - RS:  RD2 (Pin 21)
 * - EN:  RD3 (Pin 22)
 * - D4:  RD4 (Pin 27)
 * - D5:  RD5 (Pin 28)
 * - D6:  RD6 (Pin 29)
 * - D7:  RD7 (Pin 30)
 * - RW:  GND (Write-only mode)
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

#define LCD_RS PORTDbits.RD2
#define LCD_EN PORTDbits.RD3
#define LCD_D4 PORTDbits.RD4
#define LCD_D5 PORTDbits.RD5
#define LCD_D6 PORTDbits.RD6
#define LCD_D7 PORTDbits.RD7

static void LCD_PulseEnable(void) {
    LCD_EN = 1;
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(50);
}

static void LCD_SendNibble(unsigned char nibble) {
    LCD_D4 = (nibble >> 0) & 1;
    LCD_D5 = (nibble >> 1) & 1;
    LCD_D6 = (nibble >> 2) & 1;
    LCD_D7 = (nibble >> 3) & 1;
    LCD_PulseEnable();
}

void LCD_Command(unsigned char cmd) {
    LCD_RS = 0;                     // Instruction register
    LCD_SendNibble(cmd >> 4);       // High 4 bits
    LCD_SendNibble(cmd & 0x0F);     // Low 4 bits
    if (cmd == 0x01 || cmd == 0x02) {
        __delay_ms(2);              // Clear display and Return Home take > 1.52 ms
    }
}

void LCD_Char(char ch) {
    LCD_RS = 1;                     // Data register
    LCD_SendNibble(ch >> 4);
    LCD_SendNibble(ch & 0x0F);
}

void LCD_Print(const char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(unsigned char row, unsigned char col) {
    // Row 0 DDRAM base: 0x80, Row 1 DDRAM base: 0xC0
    unsigned char address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Command(address);
}

void LCD_Init(void) {
    TRISD = 0x00;                   // Configure PORTD as outputs
    PORTD = 0x00;
    __delay_ms(20);                 // Wait for LCD internal reset > 15 ms

    // Special 3-step sequence to ensure 4-bit mode synchronization
    LCD_RS = 0;
    LCD_SendNibble(0x03);
    __delay_ms(5);
    LCD_SendNibble(0x03);
    __delay_us(150);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02);           // Function Set: 4-bit interface

    LCD_Command(0x28);              // 2 lines, 5x8 character matrix
    LCD_Command(0x0C);              // Display ON, cursor OFF, blink OFF
    LCD_Command(0x06);              // Increment cursor right, no display shift
    LCD_Command(0x01);              // Clear display
}

void main(void) {
    LCD_Init();

    LCD_SetCursor(0, 0);
    LCD_Print("PIC16F877A XC8");

    LCD_SetCursor(1, 0);
    LCD_Print("Register Master");

    while (1) {
        // Static text demo
    }
}
