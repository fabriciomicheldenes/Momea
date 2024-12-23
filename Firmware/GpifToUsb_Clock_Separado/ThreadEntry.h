/*
 * ThreadEntry.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef THREADENTRY_H_
#define THREADENTRY_H_

#include "cyfxgpiftousb.h"
#include "AppInit.h"
#include "DebugInit.h"

/* Entry function for the glAppThread. */
void
CyFxAppThread_Entry (
        uint32_t input);

#endif /* THREADENTRY_H_ */
