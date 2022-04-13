// Fake Typing bare metal sample application
// On the serial port, fakes

#include "consoleUtils.h"
#include <stdint.h>

// My hardware abstraction modules

// #include "beaglebone.h"
#include "hw_types.h"      // For HWREG(...) macro
#include "timer.h"
#include "watchdog_timer.h"
#include "serial.h"

// My application's modules
//#include "fakeTyper.h"

#define PRM_DEV				0x44E00F00	// base
#define PRM_RSTST_OFFSET	0x08		// offset

/******************************************************************************
 **              SERIAL PORT HANDLING
 ******************************************************************************/
static volatile uint8_t s_rxByte = 0;
static void serialRxIsrCallback(uint8_t rxByte) {
	s_rxByte = rxByte;
}

// static void doBackgroundSerialWork(void)
// {
// 	if (s_rxByte != 0) {
// 		// Tell a joke
// 		if (s_rxByte == '?') {
// 			ConsoleUtilsPrintf("\nDisplaying help command\n");
// 			//FakeTyper_setMessage(jokes[s_curJoke]);
// 			//s_curJoke = (s_curJoke + 1) % NUM_JOKES;
// 		}
// 		else if(s_rxByte == 'x'){
// 			ConsoleUtilsPrintf("\nGoing to hit the watchdog\n");
// 		}
// 		else if(s_rxByte >=0 && s_rxByte <=9){
// 			ConsoleUtilsPrintf("\nChanging speed of LED now\n");
// 		}

// 		s_rxByte = 0;
// 	}
// }


/******************************************************************************
 **              1.2 Print Reset Sources
 ******************************************************************************/
void get_reset_source(void){
	// read reset source register
	uint32_t rst_src_reg_val = HWREG(PRM_DEV + PRM_RSTST_OFFSET);
	ConsoleUtilsPrintf("Reset source (0x%x) = ", rst_src_reg_val);

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

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(serialRxIsrCallback);
	//Timer_setTimerIsrCallback(FakeTyper_notifyOnTimeIsr);

	// Display startup messages to console:
	ConsoleUtilsPrintf("\nWelcome message\n");
	ConsoleUtilsPrintf("  Program by Supriya Dua\n");
	
	// reset sources
	get_reset_source();
	

	// Main loop:
	while(1) {
		// Handle background processing
		
		// doBackgroundSerialWork();
		//FakeTyper_doBackgroundWork();
		
		// Timer ISR signals intermittent background activity.
		if(Timer_isIsrFlagSet()) {
			//ConsoleUtilsPrintf("  inside this block\n");
			Timer_clearIsrFlag();
			Watchdog_hit();
		}
	}
}


