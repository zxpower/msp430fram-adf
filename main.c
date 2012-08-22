//******************************************************************************
//   MSP430FR5739 - ADF7012
//
//   Description: MCU transmits ASK signal to ADF7012 
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~= 1MHz.
//
//
//                   MSP430FR5739
//                 -----------------
//             /|\|             P1.0|-> CE/Pwr (GPIO)
//              | |             P1.1|-> LE (GPIO)
//              --|RST          P1.2|-> TxData Out (TA1.1)
//                |                 |
//          XIN <-|PJ.4         P2.0|-> Data Out (UCA0TXD)
//         XOUT <-|PJ.5         P2.1|<- Data In (UCA0RXD)
//                |             P2.4|-> Serial Clock Out (UCA1CLK)
//                |             P2.5|-> Data Out (UCA1SIMO)
//                |             P2.6|<- Data Int (UCA1SOMI)
//
//
//   Built with IAR Embedded Workbench Version: 5.40.3
//   Built by Reinholds Zviedris - http://reinholds.zviedris.lv/
//   Partly based on Wombat code - https://github.com/adamgreig/wombat
//******************************************************************************

#include "main.h"
#include "adf.h"
#include "clock.h"
#include "gpio.h"
#include "led.h"
#include "usart.h"
#include "watchdog.h"

volatile u08 radioOn = 0;

void main(void)
{

  watchdog_stop();
  led_setup();
  led_turn_on(LEDSBLOCK1,LED1);
  clock_setup();
  print_init();
  adf_hw_setup();
  
  PRINT("\r\n\r\n");
  PRINT("************************************************************\r\n");
  PRINT("*          LYNXNET EMERGENCY BOARD IS STARTING UP          *\r\n");
  PRINT("************************************************************\r\n");
  PRINT("\r\n\r\n");
  
  // Setup TA1.1
/*  P1SEL0 |= BIT2;
  P1DIR |= BIT2;*/
  
  // Set registers
  udelay(100);
  spi_out32(ADF7012_REG0_MSB,ADF7012_REG0_LSB);	//R0 Set
  P3OUT |= BIT4;
  
  udelay(100);
  spi_out24(ADF7012_REG1_MSB,ADF7012_REG1_LSB);	//R1 Set
  P3OUT &= ~BIT4;
  
  udelay(100);
  spi_out32(ADF7012_REG2_MSB,ADF7012_REG2_LSB);	//R2 Set
  P3OUT |= BIT4;
  
  udelay(100);
  spi_out32(ADF7012_REG3_MSB,ADF7012_REG3_LSB_PAOFF);  // R3 Set
  P3OUT &= ~BIT4;

  while(1)
  {
    led_toggle(LEDSBLOCK1,LED1);
    led_toggle(LEDSBLOCK2,LED5);
    if( radioOn == 1 ) {
      off();
      radioOn = 0;
    } else {
      radioOn = 1;
      tx();
      /*mdelay(10);
      off();
      mdelay(10);
      tx();
      mdelay(10);
      off();
      mdelay(10);
      tx();
      mdelay(10);
      off();*/
    }
    mdelay(100);
  }
  
/*  while(1)
  {
    //-.- .--- -.... -.- ... -
    //            K   J    6     K   S   T
    //if(!(P1IN & 0x08))
      mode = (mode==5) ? 0 : mode+1;

    switch(mode)
    {
      case 0:
        tone(TONE,0);
        off();
        break;
      case 1:
        if(i==40)
        {
          morse_string("212012220211110212011102");
          i = 0;
        }
        else
          i++;
        break;
      case 2:
        i = 40;
        tone(TONE,0);
        tx();
        break;
      case 3:
        tone(2500,0);
        tx();
        tone(2500,1);
        break;
      case 4:
        tone(1000,0);
        tx();
        tone(1000,1);
        break;
      default:
        tone(TONE,0);
        off();
        break;
    }
				
    _delay_ms(250);
  }*/
}

/*#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void)
{
	//volatile unsigned int i;
	
	//P2OUT = cosn[i];
	i = (i==15) ? 0 : i+1;
}*/

void spi_out(u08 byte)
{
	cbi(P1OUT,PIN_SS);
	UCA0TXBUF = byte;
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	sbi(P1OUT,PIN_SS);
}

void spi_out16(u16 word)
{
	cbi(P1OUT,PIN_SS);
	UCA0TXBUF = ((word>>8) & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	UCA0TXBUF = (word & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	sbi(P1OUT,PIN_SS);
}

void spi_out24(u08 msb, u16 lsb)
{
	cbi(P1OUT,PIN_SS);
	UCA0TXBUF = msb;
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	UCA0TXBUF = ((lsb>>8) & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	UCA0TXBUF = (lsb & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	sbi(P1OUT,PIN_SS);
}

void spi_out32(u16 msb, u16 lsb)
{
	cbi(P1OUT,PIN_SS);
	UCA0TXBUF = ((msb>>8) & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	UCA0TXBUF = (msb & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	UCA0TXBUF = ((lsb>>8) & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	UCA0TXBUF = (lsb & 0xFF);
	while(UCA0STATW & UCBUSY); //Wait until USCI not busy
	sbi(P1OUT,PIN_SS);
}

void tone(u16 timer, u08 on)
{
	if(on)
	{
		TA1CCR0 = timer;		//Default to 1200*16 Hz
		TA1CCTL0 = (1<<4);		//Enable CCR0 interrupt
	}
	else
	{
		TA1CCTL0 = (0<<4);		//Disable CCR0 interrupt
		//P2OUT = 32;
	}
}

void tx(void)
{
	spi_out32(ADF7012_REG3_MSB,ADF7012_REG3_LSB_PAON);	//PA On
}

void off(void)
{
	spi_out32(ADF7012_REG3_MSB,ADF7012_REG3_LSB_PAOFF);	//PA Off
}

void morse_char(char inchar)
{
   if(inchar == 0) //Letter Space
   {
      tone(TONE,0);
      mdelay(UNITTIME(3));
      tone(TONE,0);
   }
   else if(inchar == 1) //Dit
   {
      tone(TONE,1);
      mdelay(UNITTIME(1));
      tone(TONE,0);
      mdelay(UNITTIME(1));
   }
   else if(inchar == 2) //Dah
   {
      tone(TONE,1);
      mdelay(UNITTIME(3));
      tone(TONE,0);
      mdelay(UNITTIME(1));
   }
   else //Word Space
   {
      tone(TONE,0);
      mdelay(UNITTIME(7));
      tone(TONE,0);
   }
}

//Dit = 1, Dah = 2, Letter Space = 0, Word Space = 3
void morse_string(char* input)
{
   	int i = 0;
   
   	tx();
   	mdelay(ONDELAY);
   	while(input[i] != '\0')
   	{
		morse_char(input[i++]-48);
	}
	mdelay(OFFDELAY);
	off();
}
