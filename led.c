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
#define UPPER_LIMIT_SPEED (9)
#define LOWER_LIMIT_SPEED (0)
#define DEFAULT_SPEED (5)

// Global variables
static _Bool curr_direction;	// true: forward, false: backward
static uint8_t curr_led;		// LED currently on
static uint8_t speed;
static uint32_t counter;
static uint32_t period;

// Static function declarations
static void flashLED(uint8_t led_pin);
static void clearLED(uint8_t led_pin);
static uint32_t calculatePeriod(uint8_t new_speed);

void LED_init(void)
{
	/* Enabling functional clocks for GPIO1 instance. */
	GPIO1ModuleClkConfig();

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

	curr_direction = true;
	curr_led = 0;
	speed = DEFAULT_SPEED;
	counter = 0;
	period = calculatePeriod(speed);
}


void doBackgroundLEDWork(void)
{
	if (counter >= period){
		clearLED(curr_led + LED0_PIN);

		// pick the next LED to flash
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
		counter = 0;
	}
}

// Flashes at a given speed (0-9)
void LED_setFlashSpeed(uint8_t new_speed)
{
	if (new_speed < LOWER_LIMIT_SPEED || new_speed > UPPER_LIMIT_SPEED) return;
	period = calculatePeriod(new_speed);
	speed = new_speed;
}

// Increase flash speed by one if possible
void LED_incrementFlashSpeed(void)
{
	if (speed >= UPPER_LIMIT_SPEED) return;
	period /= 2;
	speed++;
}

// Decrease flash speed by one if possible
void LED_decrementFlashSpeed(void)
{
	if (speed <= LOWER_LIMIT_SPEED) return;
	period *= 2;
	speed--;
}

void LED_notifyOnTimeIsr(void)
{
	counter++;
}

// Set LED bit
static void flashLED(uint8_t led_pin)
{
	GPIOPinWrite(LED_GPIO_BASE, led_pin, GPIO_PIN_HIGH);
}

// Clear LED bit
static void clearLED(uint8_t led_pin)
{
	GPIOPinWrite(LED_GPIO_BASE, led_pin, GPIO_PIN_LOW);
}    

static uint32_t calculatePeriod(uint8_t new_speed)
{
	return (1 << (UPPER_LIMIT_SPEED - new_speed));
}