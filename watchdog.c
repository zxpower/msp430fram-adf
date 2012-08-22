#include "main.h"
#include "watchdog.h"

// Stop watchdog timer
void watchdog_stop(void) {
	WDTCTL = WDTPW+WDTHOLD;
}
