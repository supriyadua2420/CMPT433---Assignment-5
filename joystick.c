/* 
    joystick.c
*/

#include "joystick.h"

// Internal Macro Defs
#define NUM_PINS (2)
#define UP (0)
#define JS_UP_GPIO_BASE (SOC_GPIO_0_REGS)
#define JS_UP_GPIO_PIN (26)
#define DOWN (1)
#define JS_DOWN_GPIO_BASE (SOC_GPIO_1_REGS)
#define JS_DOWN_GPIO_PIN (14)

struct pin_info_t {
    uint32_t base;
    uint8_t pin;
    _Bool last_state;
};

struct pin_info_t JS_pins[NUM_PINS];

// Internal function declarations
static _Bool readButton(uint32_t base, uint8_t pin, _Bool* last_state);

void Joystick_init(void)
{

	// Enable GPIO clocks
	GPIO0ModuleClkConfig();
	GPIO1ModuleClkConfig();

    // JS Up
    JS_pins[UP].base = JS_UP_GPIO_BASE;
    JS_pins[UP].pin = JS_UP_GPIO_PIN;
    // JS Down
    JS_pins[DOWN].base = JS_DOWN_GPIO_BASE;
    JS_pins[DOWN].pin = JS_DOWN_GPIO_PIN;

    for (uint8_t i = 0; i < NUM_PINS; i ++){
        GPIOModuleEnable(JS_pins[i].base);
        GPIOModuleReset(JS_pins[i].base);
        GPIODirModeSet(JS_pins[i].base,
                    JS_pins[i].pin,
                    GPIO_DIR_INPUT);
        JS_pins[i].last_state = false;
    }
}


void doBackgroundJoystickWork(void)
{
    for (uint8_t i = 0; i < NUM_PINS; i++){
        if (readButton(JS_pins[i].base, JS_pins[i].pin, &JS_pins[i].last_state)){
            switch(i)
            {
                case (UP):
                    // do stuff with LEDs
                    ConsoleUtilsPrintf("JS Up\n");
                    break;
                case (DOWN):
                    // do stuff with LEDs
                    ConsoleUtilsPrintf("JS Down\n");
                    break;
            }
        }
    }
}

static _Bool readButton(uint32_t base, uint8_t pin, _Bool* last_state)
{
    _Bool curr_state = (GPIOPinRead(base, pin) == 0);

    if (*last_state != curr_state){
        *last_state = curr_state;
        if (!curr_state){
            *last_state = curr_state;
            return true;    // released joystick
        }
    }
	return false;
}
