/*
 * USBEvent.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef USBEVENT_H_
#define USBEVENT_H_

#include "cyfxgpiftousb.h"
//#include "AppStart.h"
#include "AppStop.h"

/* This is the callback function to handle the USB events. */
void
CyFxApplnUSBEventCB (
    CyU3PUsbEventType_t evtype, /* Event type */
    uint16_t            evdata  /* Event data */
    );

#endif /* USBEVENT_H_ */
