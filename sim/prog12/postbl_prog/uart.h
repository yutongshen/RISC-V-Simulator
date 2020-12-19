#ifndef __UART__
#define __UART__

#include <stdint.h>

#define UART_TXEN 1
#define UART_RXEN 1

#define RG_UART_TXFIFO 0x00
#define RG_UART_RXFIFO 0x01
#define RG_UART_TXCTRL 0x02
#define RG_UART_RXCTRL 0x03
#define RG_UART_IE 0x04
#define RG_UART_IP 0x05
#define RG_UART_DIV 0x06

#endif
