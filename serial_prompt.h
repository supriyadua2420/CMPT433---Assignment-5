/* 
    serial_prompt.h - Provides welcome message and help information
                    - Handles serial port input
*/

#ifndef _SERIAL_PROMPT_H_
#define _SERIAL_PROMPT_H_

#include <stdint.h>
// #include "soc_AM335x.h"
// #include "beaglebone.h"
#include "hw_types.h"      // For HWREG(...) macro
#include "consoleUtils.h"
#include "led.h"
#include "watchdog_timer.h"

void SerialPrompt_init(void);

void doBackgroundSerialWork(void);

void SerialPrompt_RxIsrCallback(uint8_t rxByte);

#endif