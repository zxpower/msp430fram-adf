#include "main.h"
#include "gpio.h"
#include "led.h"

// LED setup
void led_setup(void) {
  pinAsData(LEDSBLOCK1,LED1);
  pinAsOutput(LEDSBLOCK1,LED1);
  pinClear(LEDSBLOCK1,LED1);

  pinAsData(LEDSBLOCK1,LED2);
  pinAsOutput(LEDSBLOCK1,LED2);
  pinClear(LEDSBLOCK1,LED2);

  pinAsData(LEDSBLOCK1,LED3);
  pinAsOutput(LEDSBLOCK1,LED3);
  pinClear(LEDSBLOCK1,LED3);

  pinAsData(LEDSBLOCK1,LED4);
  pinAsOutput(LEDSBLOCK1,LED4);
  pinClear(LEDSBLOCK1,LED4);

  pinAsData(LEDSBLOCK2,LED5);
  pinAsOutput(LEDSBLOCK2,LED5);
  pinClear(LEDSBLOCK2,LED5);

  pinAsData(LEDSBLOCK2,LED6);
  pinAsOutput(LEDSBLOCK2,LED6);
  pinClear(LEDSBLOCK2,LED6);

  pinAsData(LEDSBLOCK2,LED7);
  pinAsOutput(LEDSBLOCK2,LED7);
  pinClear(LEDSBLOCK2,LED7);

  pinAsData(LEDSBLOCK2,LED8);
  pinAsOutput(LEDSBLOCK2,LED8);
  pinClear(LEDSBLOCK2,LED8);
}
