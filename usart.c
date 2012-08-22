#include "main.h"
#include "gpio.h"
#include "usart.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// Global variable
USARTCallback_t usartRecvCb[USART_COUNT];
static char * _print_buf;

void usart_setup(void) {
  pinAsFunction2(USCI_A0_RXTX_PORT, USCI_A0_TX_PIN);
  pinAsFunction2(USCI_A0_RXTX_PORT, USCI_A0_RX_PIN);

  // Configure UART 0
  UCA0CTL1 |= UCSWRST; 
  UCA0CTL1 = UCSSEL_1;                      // Set ACLK = 32768 as UCBRCLK
  UCA0BR0 = 3;                              // 9600 baud
  UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
                                            // UCBRSx value = 0x53 (See UG)  
  UCA0BR1 = 0;            
  UCA0CTL1 &= ~UCSWRST;                     // release from reset
  UCA0IE |= UCRXIE;                         // enable RX interrupt
}

void usart_send_byte(u08 data)
{
    while (!(UCA0IFG & UCTXIFG));
    // Send data
    UCA0TXBUF = data;
}


void usart_send_string(char *s) {
    for (; *s; ++s) {
        usart_send_byte(*s);
        if (*s == '\n') {
            // HACK: fix the newlines
            usart_send_byte('\r');
        }
    }
}

void usart_send_data(u08 *data, u16 len) {
    u08 *p;
    for (p = data; p < data + len; ++p) {
        usart_send_byte(*p);
    }
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  bool error = UCA0STATW & UCRXERR;
  u08 data = UCA0RXBUF;
  if (error || UCA0STATW & UCOE) {
    // There was an error or a register overflow; clear UCOE and exit
    data = UCA0RXBUF;
    return;
  }
  
  if (usartRecvCb[0]) {
    usartRecvCb[0](data);
  }
}

void print_init(void)
{
    PRINT_INIT_NEW(PRINT_BUFFER_SIZE);
}

void serial_print(char* str)
{
    usart_send_string(str);
}

void debug_printf(char* str, ...)
{
    va_list args;

    if (!_print_buf) return;

    va_start(args, str);
    vsnprintf(_print_buf, PRINT_BUFFER_SIZE, str, args);
    PRINT(_print_buf);
}

#ifdef DEBUG
 void debug_hexdump(void *data_, u16 len) {
    static const char digits[] = "0123456789abcdef";
    u08 *data = (u08 *) data_;
    u16 i;
    char line[5 * 16 + 3] = {0};
    char *p = line;
    for (i = 0; i < len; ++i) {
        *p++ = '0';
        *p++ = 'x';
        *p++ = digits[data[i] >> 4];
        *p++ = digits[data[i] & 0xF];
        if ((i & 15) == 15) {
            *p++ = ',';
            *p++ = '\n';
            *p = '\t';
            PRINT(line);
            p = line;
        } else {
            *p++ = ',';
        }
    }
    if (i & 15) {
        *p++ = '\n';
        *p = '\0';
        PRINT(line);
    }
}
#else // DEBUG not defined
void debugHexdump(void *data, u16 len)
{
}
#endif // DEBUG