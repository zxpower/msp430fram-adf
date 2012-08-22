#ifndef LYNXNETEMERGENCY_LED_H
#define LYNXNETEMERGENCY_LED_H

#include "gpio.h"

#define led_turn_on( po, pi ) PIN_SET( po, pi )
#define led_turn_off( po, pi ) PIN_CLEAR( po, pi )
#define led_toggle( po, pi ) PIN_TOGGLE( po, pi )

void led_setup(void);

#define LEDSBLOCK1   J
#define LED1         0
#define LED2         1
#define LED3         2
#define LED4         3

#define LEDSBLOCK2   3
#define LED5         4
#define LED6         5
#define LED7         6
#define LED8         7

#endif /*LYNXNETEMERGENCY_LED_H*/
