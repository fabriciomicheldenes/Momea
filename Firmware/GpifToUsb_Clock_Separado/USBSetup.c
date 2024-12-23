/*
 * USBSetup.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "USBSetup.h"

uint8_t glEp0Buffer[64] __attribute__ ((aligned (32))); /* Local buffer used for vendor command handling. */
volatile uint8_t  vendorRqtCnt = 0;
volatile uint32_t underrunCnt = 0;
volatile CyBool_t glRstRqt = CyFalse;

/* Callback to handle the USB setup requests. */
CyBool_t
CyFxApplnUSBSetupCB (
        uint32_t setupdat0, /* SETUP Data 0 */
        uint32_t setupdat1  /* SETUP Data 1 */
    )
{
    /* Fast enumeration is used. Only requests addressed to the interface, class,
     * vendor and unknown control requests are received by this function.
     * This application does not support any class or vendor requests. */

    uint8_t  bRequest, bReqType;
    uint8_t  bType, bTarget;
    uint16_t wValue, wIndex, wLength;
    CyBool_t isHandled = CyFalse;
    uint16_t temp;
    CyU3PReturnStatus_t status;

    /* Decode the fields from the setup request. */
    bReqType = (setupdat0 & CY_U3P_USB_REQUEST_TYPE_MASK);
    bType    = (bReqType & CY_U3P_USB_TYPE_MASK);
    bTarget  = (bReqType & CY_U3P_USB_TARGET_MASK);
    bRequest = ((setupdat0 & CY_U3P_USB_REQUEST_MASK) >> CY_U3P_USB_REQUEST_POS);
    wValue   = ((setupdat0 & CY_U3P_USB_VALUE_MASK)   >> CY_U3P_USB_VALUE_POS);
    wIndex   = ((setupdat1 & CY_U3P_USB_INDEX_MASK)   >> CY_U3P_USB_INDEX_POS);
    wLength  = ((setupdat1 & CY_U3P_USB_LENGTH_MASK)  >> CY_U3P_USB_LENGTH_POS);

    if (bType == CY_U3P_USB_VENDOR_RQT)
    {
        /* Vendor command is sent by test applications. Start the loop that tries to keep the link
         * in U0.
         */
        switch (bRequest)
        {
            case 0x76: /* Request used to check for endpoint corruption. */
                glEp0Buffer[0] = vendorRqtCnt;
                glEp0Buffer[1] = 0xDE;
                glEp0Buffer[2] = 0x5A;
                glEp0Buffer[3] = ~vendorRqtCnt;
                status = CyU3PUsbSendEP0Data (wLength, glEp0Buffer);
                if (status != CY_U3P_SUCCESS)
                {
                    CyU3PDebugPrint (2, "Send data failed\r\n");
                }

                vendorRqtCnt++;
                isHandled = CyTrue;
                break;

            case 0x77: /* Get current USB log index. */
                if (wLength >= 2)
                {
                    temp = CyU3PUsbGetEventLogIndex ();
                    CyU3PMemCopy ((uint8_t *)glEp0Buffer, (uint8_t *)&temp, 2);
                    CyU3PUsbSendEP0Data (2, glEp0Buffer);
                }
                else
                    CyU3PUsbStall (0, CyTrue, CyFalse);
                isHandled = CyTrue;
                break;

            case 0x78: /* Get USB event log data. */
                if (wLength != 0)
                {
                    if (wLength < CYFX_USBLOG_SIZE)
                        CyU3PUsbSendEP0Data (wLength, gl_UsbLogBuffer);
                    else
                        CyU3PUsbSendEP0Data (CYFX_USBLOG_SIZE, gl_UsbLogBuffer);
                }
                else
                    CyU3PUsbAckSetup ();
                isHandled = CyTrue;
                break;

            case 0x95:
                CyU3PUsbAckSetup ();
                isHandled = CyTrue;
                break;

            case 0xE0: /* Device reset request for automation. */
                glRstRqt = CyTrue;
                CyU3PUsbAckSetup ();
                isHandled = CyTrue;
                break;

            default:
                break;
        }
    }

    if (bType == CY_U3P_USB_STANDARD_RQT)
    {
        /* Handle SET_FEATURE(FUNCTION_SUSPEND) and CLEAR_FEATURE(FUNCTION_SUSPEND)
         * requests here. It should be allowed to pass if the device is in configured
         * state and failed otherwise. */
        if ((bTarget == CY_U3P_USB_TARGET_INTF) && ((bRequest == CY_U3P_USB_SC_SET_FEATURE)
                    || (bRequest == CY_U3P_USB_SC_CLEAR_FEATURE)) && (wValue == 0))
        {
            if (glIsApplnActive)
            {
                CyU3PUsbAckSetup ();

                /* As we have only one interface, the link can be pushed into U2 state as soon as
                   this interface is suspended.
                 */
                if (bRequest == CY_U3P_USB_SC_SET_FEATURE)
                {
                    glForceLinkU2 = CyTrue;
                }
                else
                {
                    glForceLinkU2 = CyFalse;
                }
            }
            else
                CyU3PUsbStall (0, CyTrue, CyFalse);

            isHandled = CyTrue;
        }

        /* CLEAR_FEATURE request for endpoint is always passed to the setup callback
         * regardless of the enumeration model used. When a clear feature is received,
         * the previous transfer has to be flushed and cleaned up. This is done at the
         * protocol level. Since this is just a loopback operation, there is no higher
         * level protocol. So flush the EP memory and reset the DMA channel associated
         * with it. If there are more than one EP associated with the channel reset both
         * the EPs. The endpoint stall and toggle / sequence number is also expected to be
         * reset. Return CyFalse to make the library clear the stall and reset the endpoint
         * toggle. Or invoke the CyU3PUsbStall (ep, CyFalse, CyTrue) and return CyTrue.
         * Here we are clearing the stall. */
        if ((bTarget == CY_U3P_USB_TARGET_ENDPT) && (bRequest == CY_U3P_USB_SC_CLEAR_FEATURE)
                && (wValue == CY_U3P_USBX_FS_EP_HALT))
        {
            if (glIsApplnActive)
            {
                if (wIndex == CY_FX_EP_CONSUMER)
                {
                    CyU3PUsbSetEpNak (CY_FX_EP_CONSUMER, CyTrue);
                    CyU3PBusyWait (125);

                    CyU3PDmaChannelReset (&glDmaChHandle);
                    CyU3PUsbFlushEp(CY_FX_EP_CONSUMER);
                    CyU3PUsbResetEp (CY_FX_EP_CONSUMER);
                    CyU3PDmaChannelSetXfer (&glDmaChHandle, CY_FX_GPIFTOUSB_DMA_TX_SIZE);
                    CyU3PUsbStall (wIndex, CyFalse, CyTrue);

                    CyU3PUsbSetEpNak (CY_FX_EP_CONSUMER, CyFalse);
                    isHandled = CyTrue;
                    CyU3PUsbAckSetup ();
                }

                if (wIndex == CY_FX_EP_LOOP_IN)
                {
                    CyU3PUsbSetEpNak (CY_FX_EP_LOOP_IN, CyTrue);
                    CyU3PBusyWait (125);

#ifdef LOOPBACK_AUTO
                    CyU3PDmaChannelReset (&glLoopOutChHandle);
                    CyU3PUsbFlushEp (CY_FX_EP_LOOP_IN);
                    CyU3PUsbResetEp (CY_FX_EP_LOOP_IN);
                    CyU3PDmaChannelSetXfer (&glLoopOutChHandle, 0);
                    CyU3PUsbStall (wIndex, CyFalse, CyTrue);
                    isHandled = CyTrue;
                    CyU3PUsbAckSetup ();
#else
                    CyU3PDmaChannelReset (&glLoopOutChHandle);
                    CyU3PDmaChannelReset (&glLoopInChHandle);
                    CyU3PUsbFlushEp (CY_FX_EP_LOOP_IN);
                    CyU3PUsbResetEp (CY_FX_EP_LOOP_IN);
                    CyU3PDmaChannelSetXfer (&glLoopOutChHandle, 0);
                    CyU3PDmaChannelSetXfer (&glLoopInChHandle, 0);
                    CyU3PUsbStall (wIndex, CyFalse, CyTrue);
                    isHandled = CyTrue;
                    CyU3PUsbAckSetup ();
#endif

                    CyU3PUsbSetEpNak (CY_FX_EP_LOOP_IN, CyFalse);
                }
            }
        }
    }

    return isHandled;
}
