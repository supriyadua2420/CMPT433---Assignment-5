/* 
    led.h - Interface to control LED flash interval 
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

void LED_setFlashSpeed(uint8_t new_speed);

void LED_incrementFlashSpeed(void);

void LED_decrementFlashSpeed(void);

#endif