#ifndef UART_H
#define UART_H

// i)#include "/usr/lib/gcc/arm-none-eabi/14.2.0/include/stdint.h"
#include <stdint.h>
// this  sample  uses arm-non-eabi-gcc compiler 
//ISSUE :  this compiler used here is not able to locate stdint.h if used without whole path needs a fix 

typedef struct {
   uint32_t baud_rate;  // Correct type: uint32_t
   uint8_t data_bits;   // Correct type: uint8_t
   uint8_t stop_bits;   // Correct type: uint8_t
   uint8_t parity;      // Correct type: uint8_t
   uint32_t buffer_size; // Correct type: uint32_t
} UART_Config;

// Define base address and register offsets
#define UART0_BASE 0x101f1000
#define UARTFR (*(volatile uint32_t *)(UART0_BASE + 0x18)) // Flag Register
#define UARTDR (*(volatile uint32_t *)(UART0_BASE + 0x00)) // Data Register
#define UARTRSR (*(volatile uint32_t *)(UART0_BASE + 0x04)) // Receive Status Register
#define UARTIMSC (*(volatile uint32_t *)(UART0_BASE + 0x38)) // Interrupt Mask Set/Clear Register
#define UARTICR (*(volatile uint32_t *)(UART0_BASE + 0x44)) // Interrupt Clear Register

// UART Flag Bits
#define UARTFR_TXFF (1 << 5)  // Transmit FIFO Full
#define UARTFR_RXFE (1 << 4)  // Receive FIFO Empty

// Receive Status Register (UARTRSR) error bits
#define UARTRSR_OE (1 << 3)  // Overrun Error
#define UARTRSR_BE (1 << 2)  // Break Error
#define UARTRSR_PE (1 << 1)  // Parity Error
#define UARTRSR_FE (1 << 0)  // Framing Error

// Function prototypes
void uart_init(UART_Config *config);                // UART Initialization
void uart_send_char(char c);                        // Send a single character
void uart_send_string(const char *str);             // Send a string
char uart_receive_char();                           // Receive a single character
int uart_check_errors();                            // Check for UART errors
void uart_clear_errors();                           // Clear UART error flags
void UART_Handler();                                // UART Interrupt Handler

#endif
