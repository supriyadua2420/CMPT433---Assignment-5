/* 
    led.c
*/

#include "led.h"

// LED GPIO Macros
#define LED_GPIO_BASE           (SOC_GPIO_1_REGS)
#define NUM_PINS (4)
#define LED0_PIN (21)
#define LED1_PIN (22)
#define LED2_PIN (23)
#define LED3_PIN (24)
// #define LED_MASK ((1<<LED0_PIN) | (1<<LED1_PIN) | (1<<LED2_PIN) | (1<<LED3_PIN))

// Global variables
static volatile _Bool s_isTimeToSwitchLED;
static uint8_t curr_led;
static _Bool curr_direction;	// true: forward, false: backward

// Static function declarations
static void flashLED(uint8_t led_pin);
static void clearLED(uint8_t led_pin);

void LED_init(void)
{
	/* Enabling functional clocks for GPIO1 instance. */
	GPIO1ModuleClkConfig();

	/* Selecting GPIO1[23] pin for use. */
	//GPIO1Pin23PinMuxSetup();

	/* Enabling the GPIO module. */
	GPIOModuleEnable(LED_GPIO_BASE);

	/* Resetting the GPIO module. */
	GPIOModuleReset(LED_GPIO_BASE);

	/* Setting the GPIO pin as an output pin. */
	GPIODirModeSet(LED_GPIO_BASE,
			LED0_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED1_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED2_PIN,
			GPIO_DIR_OUTPUT);
	GPIODirModeSet(LED_GPIO_BASE,
			LED3_PIN,
			GPIO_DIR_OUTPUT);

	s_isTimeToSwitchLED = false;
	curr_led = 0;
	curr_direction = true;
}


void doBackgroundLEDWork(void)
{
	if (s_isTimeToSwitchLED){

		s_isTimeToSwitchLED = false;
		clearLED(curr_led + LED0_PIN);

		if (curr_direction){
			if (curr_led == (NUM_PINS-1)){
				curr_direction = !curr_direction;
				curr_led --;
			}else{
				curr_led ++;
			}
		}else{
			if (curr_led == 0){
				curr_direction = !curr_direction;
				curr_led ++;
			}else{
				curr_led --;
			}
		}

		flashLED(curr_led + LED0_PIN);
	}
}

void LED_notifyOnTimeIsr(void)
{
	s_isTimeToSwitchLED = true;
}

// Flash a LED
static void flashLED(uint8_t led_pin)
{
	ConsoleUtilsPrintf("Flash LED: %d\n", led_pin);
	// HWREG(LED_GPIO_BASE + GPIO_DATAOUT) &= ~(1 << led_pin);
	GPIOPinWrite(LED_GPIO_BASE, led_pin, GPIO_PIN_LOW);
}

// Clear all the LEDs:
static void clearLED(uint8_t led_pin)
{
	// HWREG(LED_GPIO_BASE + GPIO_DATAOUT) |= (1 << led_pin);
	GPIOPinWrite(LED_GPIO_BASE, led_pin, GPIO_PIN_HIGH);
}    

