#ifndef LYNXNETEMERGENCY_GPIO_H
#define LYNXNETEMERGENCY_GPIO_H

#define pinSet( po, pi ) PIN_SET( po, pi )
#define pinSetMask( po, pi ) PIN_SET_MASK( po, pi )
#define pinClear( po, pi ) PIN_CLEAR( po, pi )
#define pinClearMask( po, pi ) PIN_CLEAR_MASK( po, pi )
#define pinToggle( po, pi ) PIN_TOGGLE( po, pi )
#define pinToggleMask( po, pi ) PIN_TOGGLE_MASK( po, pi )
#define pinRead( po, pi ) PIN_READ( po, pi )
#define pinWrite( po, pi, val ) PIN_WRITE( po, pi, val )

#define pinAsOutput( po, pi ) PIN_AS_OUTPUT( po, pi )
#define pinAsInput( po, pi ) PIN_AS_INPUT( po, pi )
#define pinAsData( po, pi ) PIN_AS_DATA( po, pi )
#define pinAsFunction1( po, pi ) PIN_AS_FUNCTION1( po, pi )
#define pinAsFunction2( po, pi ) PIN_AS_FUNCTION2( po, pi )
#define pinAsFunction3( po, pi ) PIN_AS_FUNCTION3( po, pi )

#define portAsOutput( po ) PORT_AS_OUTPUT( po )
#define portAsInput( po ) PORT_AS_INPUT( po )
#define portRead( po ) PORT_READ( po )
#define portWrite( po, val ) PORT_WRITE( po, val )

// Interrupts
#define pinEnableInt( po, pi ) PIN_ENABLE_INT( po, pi )
#define pinDisableInt( po, pi ) PIN_DISABLE_INT( po, pi )
#define pinIntRising( po, pi ) PIN_INT_RISING( po, pi )
#define pinIntFalling( po, pi ) PIN_INT_FALLING( po, pi )
#define pinIsIntRising( po, pi ) PIN_IS_INT_RISING( po, pi )
#define pinReadIntFlag( po, pi ) PIN_READ_INT_FLAG( po, pi )
#define pinClearIntFlag( po, pi ) PIN_CLEAR_INT_FLAG( po, pi )

//===========================================================
//  Macros
//===========================================================


#define PORT_AS_OUTPUT(portnum)  P##portnum##DIR = 0xff;
#define PORT_AS_INPUT(portnum)   P##portnum##DIR = 0x00;

#define PIN_AS_OUTPUT(portnum, pinnum)   \
  (P##portnum##DIR |= (1 << (pinnum)))

#define PIN_AS_INPUT(portnum, pinnum)    \
  (P##portnum##DIR &= (~(1 << (pinnum))))

#define PIN_AS_FUNCTION1(portnum, pinnum)     \
  { \
    P##portnum##SEL0 |= (1 << (pinnum)); \
    P##portnum##SEL1 &= (~(1 << (pinnum))); \
  }

#define PIN_AS_FUNCTION2(portnum, pinnum)     \
  { \
    P##portnum##SEL0 &= (~(1 << (pinnum))); \
    P##portnum##SEL1 |= (1 << (pinnum)); \
  }

#define PIN_AS_FUNCTION3(portnum, pinnum)     \
  (P##portnum##SELC |= (1 << (pinnum)))

#define PIN_AS_DATA(portnum, pinnum)     \
  (P##portnum##SELC &= (~(1 << (pinnum))))

#define PORT_READ(portnum) P##portnum##OUT

#define PORT_WRITE(portnum, val) P##portnum##OUT = (val)

#define PIN_READ(portnum, pinnum) \
    ((P##portnum##IN & (1 << (pinnum))) ? 1 : 0)

#define PIN_WRITE(portnum, pinnum, val)          \
    P##portnum##OUT =                            \
          (val) ?                                \
          (P##portnum##OUT | (1 << (pinnum))) :  \
          (P##portnum##OUT & (~(1 << (pinnum)))) \

#define PIN_SET(portnum, pinnum)                 \
      P##portnum##OUT |= (1 << (pinnum))      

#define PIN_SET_MASK(portnum, mask)              \
      P##portnum##OUT |= (mask)      

#define PIN_CLEAR(portnum, pinnum)               \
      P##portnum##OUT &= (~(1 << (pinnum)))

#define PIN_CLEAR_MASK(portnum, mask)            \
      P##portnum##OUT &= ~(mask)      

#define PIN_TOGGLE(portnum, pinnum)              \
      P##portnum##OUT ^= (1 << (pinnum))    

#define PIN_TOGGLE_MASK(portnum, mask)           \
      P##portnum##OUT ^= (mask)     

// interrupts
#define PIN_ENABLE_INT(portnum, pinnum)          \
      P##portnum##IE |= (1 << (pinnum))     

#define PIN_DISABLE_INT(portnum, pinnum)         \
      P##portnum##IE &= ~(1 << (pinnum))     

// catch interrupt on rising/falling edge
#define PIN_INT_RISING(portnum, pinnum)          \
      P##portnum##IES &= ~(1 << (pinnum))     

#define PIN_INT_FALLING(portnum, pinnum)         \
      P##portnum##IES |= (1 << (pinnum))     

// check whether pin int is caught on rising edge
#define PIN_IS_INT_RISING(portnum, pinnum)       \
      ((P##portnum##IES & (1 << (pinnum))) ? 0 : 1)     

// int flag is set on interrupt and must be cleared afterwards
#define PORT_INT_FLAG(portnum)                   \
      P##portnum##IFG     

#define PIN_READ_INT_FLAG(portnum, pinnum)       \
      (P##portnum##IFG & (1 << (pinnum)))

#define PIN_CLEAR_INT_FLAG(portnum, pinnum)      \
       (P##portnum##IFG &= ~(1 << (pinnum)))     

//===========================================================

#endif /*LYNXNETEMERGENCY_GPIO_H*/
