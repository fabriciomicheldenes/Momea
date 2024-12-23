/*
 * GPIFEvent.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef GPIFEVENT_H_
#define GPIFEVENT_H_

#include "cyfxgpiftousb.h"

void
CyFxApplnGPIFEventCB (
		CyU3PGpifEventType event,               /* Event type that is being notified. */
		uint8_t            currentState         /* Current state of the State Machine. */
);

#endif /* GPIFEVENT_H_ */
