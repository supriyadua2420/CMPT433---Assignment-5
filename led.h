/* 
    led.c
*/

#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"
#include "hw_types.h"      // For HWREG(...) macro
#include "watchdog_timer.h"
#include "consoleUtils.h"

void LED_init(void);

void doBackgroundLEDWork(void);

void LED_notifyOnTimeIsr(void);

#endif