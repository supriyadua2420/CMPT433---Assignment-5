/* 
    joystick.h - Read joystick input in order to control LED flash period
*/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdint.h>
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "consoleUtils.h"
#include "hw_types.h"      // For HWREG(...) macro
#include "led.h"

void Joystick_init(void);

void doBackgroundJoystickWork(void);

#endif