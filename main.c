// Fake Typing bare metal sample application
// On the serial port, fakes

#include "consoleUtils.h"
#include <stdint.h>

// My hardware abstraction modules

#include "timer.h"
#include "watchdog_timer.h"
#include "serial.h"

// My application's modules
//#include "fakeTyper.h"

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
		// Tell a joke
		if (s_rxByte == '?') {
			ConsoleUtilsPrintf("\nDisplaying help command\n");
			//FakeTyper_setMessage(jokes[s_curJoke]);
			//s_curJoke = (s_curJoke + 1) % NUM_JOKES;
		}
		else if(s_rxByte == 'x'){
			ConsoleUtilsPrintf("\nGoing to hit the watchdog\n");
		}
		else if(s_rxByte >=0 && s_rxByte <=9){
			ConsoleUtilsPrintf("\nChanging speed of LED now\n");
		}

		s_rxByte = 0;
	}
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
	//FakeTyper_init();

	// Setup callbacks from hardware abstraction modules to application:
	Serial_setRxIsrCallback(serialRxIsrCallback);
	//Timer_setTimerIsrCallback(FakeTyper_notifyOnTimeIsr);

	// Display startup messages to console:
	ConsoleUtilsPrintf("\nWelcome message\n");
	ConsoleUtilsPrintf("  Program by Supriya Dua\n");

	// Main loop:
	while(1) {
		// Handle background processing
		
		doBackgroundSerialWork();
		//FakeTyper_doBackgroundWork();
		
		// Timer ISR signals intermittent background activity.
		if(Timer_isIsrFlagSet()) {
			//ConsoleUtilsPrintf("  inside this block\n");
			Timer_clearIsrFlag();
			Watchdog_hit();
		}
	}
}
