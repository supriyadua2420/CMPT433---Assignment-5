/* 
    Assignment 5: Bare Metal LED Flasher

	By: Supriya Dua: <insert s.id> &
		Gaurav Modi: 301263574
	
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

// Internal Macro Defs
#define PRM_DEV				(0x44E00F00)	// base
#define PRM_RSTST_OFFSET	(0x08)		// offset

/******************************************************************************
 **              SERIAL PORT HANDLING
 ******************************************************************************/
static volatile uint8_t s_rxByte = 0;
static void serialRxIsrCallback(uint8_t rxByte) {
	s_rxByte = rxByte;
}

static void doBackgroundSerialWork(void)
 {
 	if (s_rxByte != 0) {
 		if (s_rxByte == '?') {
 			ConsoleUtilsPrintf("\nCommands:\n");
			ConsoleUtilsPrintf(" ?:\tDisplay this help message\n");
			ConsoleUtilsPrintf(" 0-9:\tSet speed 0 (slow) to 9 (fast)\n");
			ConsoleUtilsPrintf(" x:\tStop hitting the watchdog\n");
			ConsoleUtilsPrintf(" JOY:\tUp (faster), Down (slower)\n");
 		}
 		else if(s_rxByte == 'x'){
 			ConsoleUtilsPrintf("\nNo longer hitting the watchdog.\n");
 		}
 		else if(s_rxByte >= '0' && s_rxByte <= '9'){
 			ConsoleUtilsPrintf("\nSetting LED speed to %c.\n", s_rxByte);
			 //LED STUFF
			 //setLEDFast();
 		}
		else{
			ConsoleUtilsPrintf("\nCommand not recognised.\n");
		}

 		s_rxByte = 0;
 	}
 }

/******************************************************************************
 **              1.2 Print Reset Sources
 ******************************************************************************/
void get_reset_source(void){
	// read reset source register
	uint8_t rst_src_reg_val = HWREG(PRM_DEV + PRM_RSTST_OFFSET);
	ConsoleUtilsPrintf("Reset source (0x%02x) = ", rst_src_reg_val);

	// print to serial port
	if ((rst_src_reg_val & 0x01) == 0x01){ 
		ConsoleUtilsPrintf("Cold, ");
	}
	if ((rst_src_reg_val & 0x02) == 0x02){ 
		ConsoleUtilsPrintf("Software, ");
	}
	if ((rst_src_reg_val & 0x10) == 0x10){ 
		ConsoleUtilsPrintf("Watchdog, ");
	}
	if ((rst_src_reg_val & 0x20) == 0x20){ 
		ConsoleUtilsPrintf("External, ");
	}
	ConsoleUtilsPrintf("\n");
	
	// clear register
	HWREG(PRM_DEV + PRM_RSTST_OFFSET) |= 0x33;
}


/******************************************************************************
 **              MAIN
 ******************************************************************************/
int main(void)
{
	// Initialization
	Serial_init(serialRxIsrCallback);
	Timer_init();
	Watchdog_init();
	LED_init();
	Joystick_init();

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(serialRxIsrCallback);
	Timer_setTimerIsrCallback(LED_notifyOnTimeIsr);

	// Display startup messages to console:
	ConsoleUtilsPrintf("\nWelcome message\n");
	ConsoleUtilsPrintf("  Program by Supriya Dua and Gaurav Modi\n");
	
	// Print reset sources
	get_reset_source();

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


