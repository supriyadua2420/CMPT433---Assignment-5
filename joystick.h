/* 
    joystick.h
*/

#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdint.h>
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "consoleUtils.h"
#include "hw_types.h"      // For HWREG(...) macro

void Joystick_init(void);

void doBackgroundJoystickWork(void);

#endif