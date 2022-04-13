// serial.h:
// Hardware Abstraction Module: Serial port

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>
#include "uart_irda_cir.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "beaglebone.h"
#include "consoleUtils.h"
#include "hw_types.h"

void Serial_init();
void Serial_setRxIsrCallback(void (*rxIsrCallback)(uint8_t));

#endif