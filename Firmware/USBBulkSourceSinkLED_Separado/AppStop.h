/*
 * AppStop.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef APPSTOP_H_
#define APPSTOP_H_

#include "cyfxbulksrcsink.h"

/* This function stops the application. This shall be called whenever a RESET
 * or DISCONNECT event is received from the USB host. The endpoints are
 * disabled and the DMA pipe is destroyed by this function. */
void CyFxBulkSrcSinkApplnStop (void);

#endif /* APPSTOP_H_ */
