/* 
    Assignment 5: Bare Metal LED Flasher

	By: Supriya Dua &
		Gaurav Modi
	
	April 11, 2022
*/

#include <stdint.h>
#include "consoleUtils.h"

// My hardware abstraction modules

#include "hw_types.h"      // For HWREG(...) macro
#include "timer.h"
#include "watchdog_timer.h"
#include "serial.h"

// My application's modules
#include "led.h"
#include "joystick.h"
#include "serial_prompt.h"


/******************************************************************************
 **              MAIN
 ******************************************************************************/
int main(void)
{
	// Initialization
	Serial_init(SerialPrompt_RxIsrCallback);
	SerialPrompt_init();
	Timer_init();
	Watchdog_init();
	LED_init();
	Joystick_init();

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(SerialPrompt_RxIsrCallback);
	Timer_setTimerIsrCallback(LED_notifyOnTimeIsr);

	// Main loop:
	while(1) {
		// Handle background processing
		doBackgroundSerialWork();
		doBackgroundLEDWork();
		doBackgroundJoystickWork();
		
		// Timer ISR signals intermittent background activity.
		if(Timer_isIsrFlagSet()) {
			Timer_clearIsrFlag();
			Watchdog_hit();
		}
	}
}
