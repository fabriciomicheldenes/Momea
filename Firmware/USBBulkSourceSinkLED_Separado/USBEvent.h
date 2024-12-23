/*
 * USBEvent.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef USBEVENT_H_
#define USBEVENT_H_

#include "cyfxbulksrcsink.h"

void
CyFxBulkSrcSinkApplnUSBEventCB (
    CyU3PUsbEventType_t evtype, /* Event type */
    uint16_t            evdata  /* Event data */
    );

#endif /* USBEVENT_H_ */
