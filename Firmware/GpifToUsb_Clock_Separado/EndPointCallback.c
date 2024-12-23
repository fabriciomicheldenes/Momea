/*
 * EndPointCallback.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "EndPointCallback.h"

static uint32_t BulkRstCnt = 0;
static uint32_t LoopRstCnt = 0;
volatile uint32_t BulkEpEvtCount = 0;    /* Number of endpoint events received on streaming endpoint. */
volatile uint32_t InEpEvtCount = 0;      /* Number of endpoint events received on loopback IN endpoint. */
volatile uint32_t OutEpEvtCount = 0;     /* Number of endpoint events received on loopback OUT endpoint. */

/* Endpoint specific event callback. For now, we only keep a count of the endpoint events that occur. */
void
CyFxApplnEpCallback (
        CyU3PUsbEpEvtType evtype,
        CyU3PUSBSpeed_t   usbSpeed,
        uint8_t           epNum)
{
    CyU3PDebugPrint (2, "EP Event: ep=%x event=%d\r\n", epNum, evtype);
    if (epNum == CY_FX_EP_CONSUMER)
        BulkEpEvtCount++;
    if (epNum == CY_FX_EP_LOOP_IN)
        InEpEvtCount++;
    if (epNum == CY_FX_EP_LOOP_OUT)
        OutEpEvtCount++;

    if (evtype == CYU3P_USBEP_SS_RESET_EVT)
    {
        if (epNum == CY_FX_EP_CONSUMER)
        {
            CyU3PDebugPrint (2, "Halting USB Streaming EP: %d\r\n", BulkRstCnt++);
            CyU3PUsbStall (CY_FX_EP_CONSUMER, CyTrue, CyFalse);
        }
        if (epNum == CY_FX_EP_LOOP_IN)
        {
            CyU3PDebugPrint (2, "Halting USB Loopback EP: %d\r\n", LoopRstCnt++);
            CyU3PUsbStall (CY_FX_EP_LOOP_IN, CyTrue, CyFalse);
        }
    }
}
