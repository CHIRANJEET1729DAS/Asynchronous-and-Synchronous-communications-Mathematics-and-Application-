#include "/home/chiranjeet/kernel_making/UART_protocol/protocol/header_files/uart.h"
#include <stdint.h>

// Circular buffer for received data
char *circular_buffer;  // Removed static
volatile int buffer_head = 0;  // Removed static
volatile int buffer_tail = 0;  // Removed static
UART_Config uart_config;  // Removed static

// UART Initialization with user-defined settings
void uart_init(UART_Config *config) {
    uart_config = *config;  // Store configuration

    uart_send_string("Initializing UART with the following configuration:\n");
    uart_send_string("Baud Rate: ");
    uart_send_char(uart_config.baud_rate + '0');  // For simplicity assuming baud_rate is a single digit
    uart_send_string("\n");
    uart_send_string("Data Bits: ");
    uart_send_char(uart_config.data_bits + '0');  // For simplicity assuming data_bits is a single digit
    uart_send_string("\n");
    uart_send_string("Stop Bits: ");
    uart_send_char(uart_config.stop_bits + '0');  // For simplicity assuming stop_bits is a single digit
    uart_send_string("\n");
    uart_send_string("Parity: ");
    uart_send_char(uart_config.parity + '0');  // For simplicity assuming parity is a single digit
    uart_send_string("\n");
    uart_send_string("Buffer Size: ");
    uart_send_char(uart_config.buffer_size + '0');  // For simplicity assuming buffer_size is a single digit
    uart_send_string("\n");

    UARTIMSC |= (1 << 4);  // Enable RX interrupt
}

// Transmit a single character
void uart_send_char(char c) {
    while (UARTFR & UARTFR_TXFF) {  // Wait until FIFO is not full
        // Busy-waiting loop until space is available in the FIFO
    }
    UARTDR = c;  // Write the character to the Data Register
}

// Transmit a string
void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

// Receive a single character
char uart_receive_char() {
    while (buffer_head == buffer_tail) {  // Wait until data is available
        // Busy-waiting loop for data
    }
    char data = circular_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % uart_config.buffer_size;
    return data;
}

// Check for errors in the Receive Status Register
int uart_check_errors() {
    uint32_t error_flags = UARTRSR & (UARTRSR_OE | UARTRSR_BE | UARTRSR_PE | UARTRSR_FE);
    if (error_flags) {
        if (error_flags & UARTRSR_OE) {
            uart_send_string("Error: Overrun Error (OERR)\n");
        }
        if (error_flags & UARTRSR_FE) {
            uart_send_string("Error: Framing Error (FERR)\n");
        }
        if (error_flags & UARTRSR_PE) {
            uart_send_string("Error: Parity Error (PERR)\n");
        }
        if (error_flags & UARTRSR_BE) {
            uart_send_string("Error: Break Error (BER)\n");
        }
        return 1;  // Errors detected
    }
    return 0;  // No errors
}

// Clear UART error flags
void uart_clear_errors() {
    UARTRSR = 0;  // Writing to this register clears error flags
}

// UART Interrupt Handler (Simulated)
void UART_Handler() {
    if (UARTIMSC & (1 << 4)) {  // Check RX interrupt
        char received_char = UARTDR;  // Read the received character

        if (uart_check_errors()) {
            uart_clear_errors();  // Clear any errors
        } else {
            int next_head = (buffer_head + 1) % uart_config.buffer_size;
            if (next_head != buffer_tail) {
                circular_buffer[buffer_head] = received_char;
                buffer_head = next_head;
            } else {
                uart_send_string("Error: Buffer Overflow\n");
            }
        }

        UARTICR = (1 << 4);  // Clear the RX interrupt flag
    }
}

