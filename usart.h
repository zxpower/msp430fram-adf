#ifndef LYNXNETEMERGENCY_USART_H
#define LYNXNETEMERGENCY_USART_H

#include "main.h"

#define PRINT_INIT_DEF(len) \
    static char _print_buf_local[ len +1 ];       \
    _print_buf = _print_buf_local;

// callback for USART RX interrupt
typedef void (* USARTCallback_t)(u08);

void usart_setup(void);

void usart_send_string(char *string);
void usart_send_data(u08 *data, u16 len);

static inline void usart_enable_tx(void) { }
static inline void usart_disable_tx(void) { }
static inline void usart_enable_rx(void) { UCA0IE |= UCRXIE; }
static inline void usart_disable_rx(void) { UCA0IE &= ~UCRXIE; }

#define PRINT_INIT_NEW(len) \
  PRINT_INIT_DEF(len);      \
  usart_setup();            \
  usart_enable_tx()
#define PRINT serial_print
#define PRINTF debug_printf
#define PRINTLN(x) PRINT(x "\n")

void serial_print(char* str );
void debug_printf(char* str, ...);
void debug_hexdump(void *data, u16 len);
void print_init(void);

#define USCI_A0_RXTX_PORT	2
#define USCI_A0_TX_PIN		0
#define USCI_A0_RX_PIN		1

#define USART_COUNT         2
#define PRINTF_USART_ID     1

// the default size of print buffer
#ifndef PRINT_BUFFER_SIZE
#define PRINT_BUFFER_SIZE 127
#endif

#endif /*LYNXNETEMERGENCY_USART_H*/
