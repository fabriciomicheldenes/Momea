/*
 * USBEvent.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "USBEvent.h"

/* This is the callback function to handle the USB events. */
void
CyFxApplnUSBEventCB (
    CyU3PUsbEventType_t evtype, /* Event type */
    uint16_t            evdata  /* Event data */
    )
{
    if ((evtype != CY_U3P_USB_EVENT_EP0_STAT_CPLT) && (evtype != CY_U3P_USB_EVENT_RESUME))
        CyU3PDebugPrint (2, "USB event: %d %d\r\n", evtype, evdata);

    switch (evtype)
    {
        case CY_U3P_USB_EVENT_CONNECT:
            CyU3PDebugPrint (8, "CY_U3P_USB_EVENT_CONNECT detected\r\n");
            break;

        case CY_U3P_USB_EVENT_SETCONF:
            /* If the application is already active
             * stop it before re-enabling. */
            if (glIsApplnActive)
            {
                CyFxApplnStop ();
            }

            /* Start the function. */
            CyFxApplnStart ();
            break;

        case CY_U3P_USB_EVENT_RESET:
        case CY_U3P_USB_EVENT_DISCONNECT:
            glForceLinkU2 = CyFalse;

            /* Stop the function. */
            if (glIsApplnActive)
            {
                CyFxApplnStop ();
            }

            if (evtype == CY_U3P_USB_EVENT_DISCONNECT) {
                CyU3PDebugPrint (8, "CY_U3P_USB_EVENT_DISCONNECT detected\r\n");
            }
            break;

        case CY_U3P_USB_EVENT_EP_UNDERRUN:
            underrunCnt++;
            CyU3PDebugPrint (4, "EP Underrun on %d \n\r",evdata);
            CyU3PUsbResetEndpointMemories ();
            break;

        case CY_U3P_USB_EVENT_EP0_STAT_CPLT:
            break;

        default:
            break;
    }
}
