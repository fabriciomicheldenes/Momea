/*
 * AppInit.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef APPINIT_H_
#define APPINIT_H_

#include "cyfxgpiftousb.h"
#include "USBSetup.h"
#include "USBEvent.h"
#include "LPMRqt.h"
#include "ErrorHandler.h"
#include "PibDllParameters.h"

/* This function initializes the USB Module, sets the enumeration descriptors.
 * This function does not start the bulk streaming and this is done only when
 * SET_CONF event is received. */
void
CyFxApplnInit (void);

#endif /* APPINIT_H_ */
