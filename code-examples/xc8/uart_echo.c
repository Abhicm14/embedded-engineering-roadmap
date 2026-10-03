/**
 * @file uart_echo.c
 * @brief Full-duplex USART 9600-baud echo terminal for PIC16F877A
 * 
 * Demonstrates:
 * - USART hardware configuration (SPBRG, TXSTA, RCSTA)
 * - Accurate baud rate calculation with BRGH high-speed mode (+0.16% error)
 * - Blocking and polled character transmission and reception
 * - Receiver overrun error (OERR) recovery
 * 
 * Hardware Connections:
 * - TX: RC6 (Pin 25) connected to USB-UART Adapter RXD
 * - RX: RC7 (Pin 26) connected to USB-UART Adapter TXD
 * - Common GND between PIC and USB-UART adapter
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void UART_Init(void) {
    // 1. Configure pin directions (RC6=TX Output, RC7=RX Input)
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;

    // 2. Set Baud Rate generator for 9600 baud with BRGH=1 (High Speed)
    // Formula: SPBRG = (Fosc / (16 * Baud)) - 1
    // SPBRG = (4000000 / (16 * 9600)) - 1 = 25.04 -> SPBRG = 25 (0.16% error)
    SPBRG = 25;

    // 3. Configure TXSTA:
    // TXEN = 1 (Transmit Enabled)
    // BRGH = 1 (High Speed mode)
    // SYNC = 0 (Asynchronous mode)
    TXSTA = 0b00100100;

    // 4. Configure RCSTA:
    // SPEN = 1 (Serial Port Enabled: configures RC6/RC7 as serial pins)
    // CREN = 1 (Continuous Receive Enabled)
    RCSTA = 0b10010000;
}

void UART_WriteChar(char c) {
    // Wait until Transmit Shift Register buffer is empty
    while (!PIR1bits.TXIF);
    TXREG = c;                      // Load character into transmit register
}

void UART_WriteString(const char *str) {
    while (*str) {
        UART_WriteChar(*str++);
    }
}

char UART_ReadChar(void) {
    // Check and clear receiver overrun error if it occurred
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;         // Reset receiver logic
        RCSTAbits.CREN = 1;
    }

    // Wait until character is received in RCREG
    while (!PIR1bits.RCIF);
    return RCREG;                   // Reading RCREG automatically clears RCIF flag
}

void main(void) {
    UART_Init();

    __delay_ms(100);
    UART_WriteString("\r\n========================================\r\n");
    UART_WriteString("  PIC16F877A USART Echo Terminal (9600)\r\n");
    UART_WriteString("  Type any character to receive echo...\r\n");
    UART_WriteString("========================================\r\n> ");

    while (1) {
        char ch = UART_ReadChar();
        
        // Echo character back to terminal
        UART_WriteChar(ch);

        // If Enter key was pressed, print newline prompt
        if (ch == '\r') {
            UART_WriteChar('\n');
            UART_WriteString("> ");
        }
    }
}
