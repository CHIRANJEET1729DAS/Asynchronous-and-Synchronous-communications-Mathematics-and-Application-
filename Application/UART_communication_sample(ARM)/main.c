#include "/home/chiranjeet/kernel_making/UART_protocol/protocol/header_files/uart.h"

int main() {
    // Configure UART settings
    UART_Config uart_config = {
        .baud_rate = 9600,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0,  // No parity
        .buffer_size = 64
    };

    uart_init(&uart_config);  // Initialize UART with user-defined settings
    uart_send_string("UART Initialized. Type something:\n");

    while (1) {
        // Receive a character and echo it back
        char received = uart_receive_char();
        uart_send_char(received);

        // Process errors if any
        if (uart_check_errors()) {
            uart_clear_errors();  // Clear errors after handling
        }
    }

    return 0;
}

