/*
 * AppStart.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef APPSTART_H_
#define APPSTART_H_

#include "cyfxbulksrcsink.h"

/* This function starts the application. This is called
 * when a SET_CONF event is received from the USB host. The endpoints
 * are configured and the DMA pipe is setup in this function. */
void CyFxBulkSrcSinkApplnStart (void);

#endif /* APPSTART_H_ */
