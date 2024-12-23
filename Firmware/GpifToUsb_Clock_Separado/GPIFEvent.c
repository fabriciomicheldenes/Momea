/*
 * GPIFEvent.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "GPIFEvent.h"

CyU3PDmaChannel glChHandleBulkLpPtoU;       /* DMA Channel handle  for P2U transfer.*/

void
CyFxApplnGPIFEventCB (
		CyU3PGpifEventType event,               /* Event type that is being notified. */
		uint8_t            currentState         /* Current state of the State Machine. */
)
{
	switch (event)
	{
	case CYU3P_GPIF_EVT_SM_INTERRUPT:
	{
		CyU3PDmaChannelSetWrapUp(&glChHandleBulkLpPtoU);

	}
	break;

	default:
		break;
	}
}
