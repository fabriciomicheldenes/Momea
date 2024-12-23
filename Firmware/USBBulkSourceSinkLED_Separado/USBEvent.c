/*
 * USBEvent.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "USBEvent.h"
#include "AppStart.h"
#include "AppStop.h"

/* This is the callback function to handle the USB events. */
void
CyFxBulkSrcSinkApplnUSBEventCB (
    CyU3PUsbEventType_t evtype, /* Event type */
    uint16_t            evdata  /* Event data */
    )
{
    static uint32_t num_connect    = 0;
    static uint32_t num_disconnect = 0;

    switch (evtype)
    {
    case CY_U3P_USB_EVENT_CONNECT:
      ++num_connect;
      CyU3PDebugPrint (CY_FX_DEBUG_PRIORITY, "CY_U3P_USB_EVENT_CONNECT detected (%d)\n\r",
                       num_connect);
      break;

    case CY_U3P_USB_EVENT_SETCONF:
        /* If the application is already active
         * stop it before re-enabling. */
        if (glIsApplnActive)
        {
            CyFxBulkSrcSinkApplnStop ();
        }

        /* Start the source sink function. */
        CyFxBulkSrcSinkApplnStart ();
        break;

    case CY_U3P_USB_EVENT_RESET:
    case CY_U3P_USB_EVENT_DISCONNECT:
        glForceLinkU2 = CyFalse;

        /* Stop the source sink function. */
        if (glIsApplnActive)
        {
            CyFxBulkSrcSinkApplnStop ();
        }
        glDataTransStarted = CyFalse;

        if (evtype == CY_U3P_USB_EVENT_DISCONNECT) {
            ++num_disconnect;
            CyU3PDebugPrint (CY_FX_DEBUG_PRIORITY, "CY_U3P_USB_EVENT_DISCONNECT detected (%d)\n\r",
                             num_disconnect);
        }
        break;

    case CY_U3P_USB_EVENT_EP0_STAT_CPLT:
        glEp0StatCount++;
        break;

    case CY_U3P_USB_EVENT_VBUS_REMOVED:
        if (StandbyModeEnable)
        {
            TriggerStandbyMode = CyTrue;
            StandbyModeEnable  = CyFalse;
        }
        break;

    default:
        break;
    }
}
