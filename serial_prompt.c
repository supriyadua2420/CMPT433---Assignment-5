/*
    serial_prompt.c
*/

#include "serial_prompt.h"

// Internal Macro Defs
#define PRM_DEV				(0x44E00F00)	// base
#define PRM_RSTST_OFFSET	(0x08)		// offset

// Global variables
static volatile uint8_t s_rxByte = 0;

// Static function declarations
static void get_reset_source(void);

void SerialPrompt_init(void)
{
	// Display startup messages to console:
	ConsoleUtilsPrintf("\nWelcome message\n");
	ConsoleUtilsPrintf("  Program by Supriya Dua and Gaurav Modi\n");

    get_reset_source();
}

void doBackgroundSerialWork(void)
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
            Watchdog_stopHitting();
 		}
 		else if(s_rxByte >= '0' && s_rxByte <= '9'){
 			ConsoleUtilsPrintf("\nSetting LED speed to %c.\n", s_rxByte);
			LED_setFlashSpeed(s_rxByte - '0');
 		}
		else{
			ConsoleUtilsPrintf("\nCommand not recognised.\n");
		}

 		s_rxByte = 0;
 	}
}

void SerialPrompt_RxIsrCallback(uint8_t rxByte)
{
	s_rxByte = rxByte;
}

static void get_reset_source(void)
{
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