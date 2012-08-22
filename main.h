#ifndef LYNXNETEMERGENCY_MAIN_H
#define LYNXNETEMERGENCY_MAIN_H

#include "msp430fr5739.h"
#include "intrinsics.h"

#define CPU_MHZ			8
#define DEBUG			0

#define udelay(x) __delay_cycles(x * CPU_MHZ)
#define mdelay(x) __delay_cycles(x * CPU_MHZ * 1000ul)
#define clock_delay(x) __delay_cycles(x * 4)

#define PIN_SS                     1  //SPI SS pin = P1.1

#define ADF7012_CS_PORT            1
#define ADF7012_CS_PIN             1

#define ADF7012_PWR_PORT           1
#define ADF7012_PWR_PIN            0

#define ADF7012_DATA_PORT          2
#define ADF7012_DATA_PIN           0

#define ADF7012_TXDATA_PORT        1
#define ADF7012_TXDATA_PIN         2

#define ADF7012_MUXOUT_PORT        2
#define ADF7012_MUXOUT_PIN         6

// REG0 binary value = 00000100110010000010000000000000
#define ADF7012_REG0_MSB           0x0048
#define ADF7012_REG0_LSB           0x2000

// REG1 binary value = 000010100101001010010101
#define ADF7012_REG1_MSB           0x0A
#define ADF7012_REG1_LSB           0x5295

// REG2 binary value = 00000000000000000000111100001010
#define ADF7012_REG2_MSB           0x0000
#define ADF7012_REG2_LSB           0x070E

// REG3 binary value = 00000000010100011001100000100111
#define ADF7012_REG3_MSB           0x0050
#define ADF7012_REG3_LSB_PAOFF     0xA0D7
#define ADF7012_REG3_LSB_PAON      0xA0DF

#define WPM 15
#define UNITTIME(x) ((x) * (1200/WPM))
#define ONDELAY 250
#define OFFDELAY 100
#define TONE 2500

#define sbi(a, b) ((a) |= 1 << (b))     //sets bit B in variable A
#define cbi(a, b) ((a) &= ~(1 << (b)))  //clears bit B in variable A
#define tbi(a, b) ((a) ^= 1 << (b))     //toggles bit B in variable A

typedef unsigned char               u08;
typedef signed char                 s08;
typedef short unsigned int          u16;
typedef short signed int            s16;
typedef unsigned long               u32;
typedef signed long                 s32;
typedef volatile unsigned char      vu08;
typedef volatile signed char        vs08;
typedef volatile unsigned short int vu16;
typedef volatile signed short int   vs16;
typedef volatile unsigned long      vu32;
typedef volatile signed long        vs32;

#define bool u08

void tx(void);
void off(void);
void morse_string(char* input);
void morse_char(char inchar);
void tone(u16 timer, u08 on);

void spi_out(u08 byte);
void spi_out16(u16 word);
void spi_out24(u08 msb, u16 lsb);
void spi_out32(u16 msb, u16 lsb);

#endif /*LYNXNETEMERGENCY_MAIN_H*/
