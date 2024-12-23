/*
 * EndPointCallback.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef ENDPOINTCALLBACK_H_
#define ENDPOINTCALLBACK_H_

#include "cyfxgpiftousb.h"

/* Endpoint specific event callback. For now, we only keep a count of the endpoint events that occur. */
void
CyFxApplnEpCallback (
        CyU3PUsbEpEvtType evtype,
        CyU3PUSBSpeed_t   usbSpeed,
        uint8_t           epNum);

#endif /* ENDPOINTCALLBACK_H_ */
