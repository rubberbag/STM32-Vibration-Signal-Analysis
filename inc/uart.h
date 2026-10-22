#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>
#include "packet.h"

/**
 * Initializes the target USART peripheral block using direct register offsets.
 * Sets up bitmasks for the clock, baud rate generation registers, and status control.
 */
void uart_init();


void USART1_SendChar(char c);
void USART1_SendString(const char *str);

/**
 * Sends a single raw character byte over the physical transmission pin.
 */
void uart_send_byte(uint8_t byte);

/**
 * Serializes the complete structured data telemetry chunk over the active UART hardware link.
 * Transmits the binary block memory layout directly to achieve optimal data throughput.
 */
void uart_transmit_packet(const struct DiagnosticPacket *packet);

#endif
