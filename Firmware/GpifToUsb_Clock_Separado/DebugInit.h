/*
 * DebugInit.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef DEBUGINIT_H_
#define DEBUGINIT_H_

#include "cyfxgpiftousb.h"
#include "ErrorHandler.h"

/* This function initializes the debug module. The debug prints
 * are routed to the UART and can be seen using a UART console
 * running at 115200 baud rate. */
void
CyFxApplnDebugInit (void);

#endif /* DEBUGINIT_H_ */
