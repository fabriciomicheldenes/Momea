/*
 * ThreadEntry.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */


#include "ThreadEntry.h"
#include "DebugInit.h"
#include "AppInit.h"
#include "AppStop.h"
#include "ErrorHandler.h"

uint8_t glEp0Buffer[32] __attribute__ ((aligned (32))); /* Local buffer used for vendor command handling. */

void
BulkSrcSinkAppThread_Entry (
        uint32_t input)
{
    CyU3PReturnStatus_t stat;
    uint32_t eventMask = CYFX_USB_CTRL_TASK | CYFX_USB_HOSTWAKE_TASK;   /* Mask representing events that we are interested in. */
    uint32_t eventStat;                                                 /* Variable to hold current status of the events. */

    uint16_t prevUsbLogIndex = 0, tmp1, tmp2;
    CyU3PUsbLinkPowerMode curState;

    /* Initialize the debug module */
    CyFxBulkSrcSinkApplnDebugInit();
    CyU3PDebugPrint (1, "UART debug initialized\r\n");

    /* Initialize the application */
    CyFxBulkSrcSinkApplnInit();

    for (;;)
    {

    	/* TODO: User can write the user specific code here: Main thread */

    	/* LED toggling function start */

    	if(on_time != 0)
    	{
			CyU3PGpioSetValue (LED_GPIO, CyFalse);			/* Turn LED-ON	*/
			CyU3PThreadSleep(on_time);						/* ON-time	*/
    	}

    	if (off_time != 0)
    	{
			CyU3PGpioSetValue (LED_GPIO, CyTrue);			/* Turn LED-OFF */
			CyU3PThreadSleep(off_time);						/* OFF-time	*/
    	}
    	/* LED toggling function end  */


        /* The following call will block until at least one of the events enabled in eventMask is received.
           The eventStat variable will hold the events that were active at the time of returning from this API.
           The CLEAR flag means that all events will be atomically cleared before this function returns.

           We cause this event wait to time out every 10 milli-seconds, so that we can periodically get the FX3
           device out of low power modes.
         */
        stat = CyU3PEventGet (&glBulkLpEvent, eventMask, CYU3P_EVENT_OR_CLEAR, &eventStat, 10);
        if (stat == CY_U3P_SUCCESS)
        {
            /* If the HOSTWAKE task is set, send a DEV_NOTIFICATION (FUNCTION_WAKE) or remote wakeup signalling
               based on the USB connection speed. */
            if (eventStat & CYFX_USB_HOSTWAKE_TASK)
            {
                CyU3PThreadSleep (1000);
                if (CyU3PUsbGetSpeed () == CY_U3P_SUPER_SPEED)
                    stat = CyU3PUsbSendDevNotification (1, 0, 0);
                else
                    stat = CyU3PUsbDoRemoteWakeup ();

                if (stat != CY_U3P_SUCCESS)
                    CyU3PDebugPrint (2, "Remote wake attempt failed with code: %d\r\n", stat);
            }

            /* If there is a pending control request, handle it here. */
            if (eventStat & CYFX_USB_CTRL_TASK)
            {
                uint8_t  bRequest, bReqType;
                uint16_t wLength, temp;
                uint16_t wValue, wIndex;

                /* Decode the fields from the setup request. */
                bReqType = (gl_setupdat0 & CY_U3P_USB_REQUEST_TYPE_MASK);
                bRequest = ((gl_setupdat0 & CY_U3P_USB_REQUEST_MASK) >> CY_U3P_USB_REQUEST_POS);
                wLength  = ((gl_setupdat1 & CY_U3P_USB_LENGTH_MASK)  >> CY_U3P_USB_LENGTH_POS);
                wValue   = ((gl_setupdat0 & CY_U3P_USB_VALUE_MASK) >> CY_U3P_USB_VALUE_POS);
                wIndex   = ((gl_setupdat1 & CY_U3P_USB_INDEX_MASK) >> CY_U3P_USB_INDEX_POS);

                if ((bReqType & CY_U3P_USB_TYPE_MASK) == CY_U3P_USB_VENDOR_RQT)
                {
                    switch (bRequest)
                    {

                    /* TODO: User can implement their own custom vendor command here */

                    case 0x77:      /* Trigger remote wakeup. */
                        CyU3PUsbAckSetup ();
                        CyU3PEventSet (&glBulkLpEvent, CYFX_USB_HOSTWAKE_TASK, CYU3P_EVENT_OR);
                        break;

                    case 0x78:      /* Get count of EP0 status events received. */
                        CyU3PMemCopy ((uint8_t *)glEp0Buffer, ((uint8_t *)&glEp0StatCount), 4);
                        CyU3PUsbSendEP0Data (4, glEp0Buffer);
                        break;

                    case 0x79:      /* Request with no data phase. Insert a delay and then ACK the request. */
                        CyU3PThreadSleep (5);
                        CyU3PUsbAckSetup ();
                        break;

                    case 0x80:      /* Request with OUT data phase. Just get the data and ignore it for now. */
                        CyU3PUsbGetEP0Data (sizeof (glEp0Buffer), (uint8_t *)glEp0Buffer, &wLength);
                        break;

                    case 0x81:
                        /* Get the current event log index and send it to the host. */
                        if (wLength == 2)
                        {
                            temp = CyU3PUsbGetEventLogIndex ();
                            CyU3PMemCopy ((uint8_t *)glEp0Buffer, (uint8_t *)&temp, 2);
                            CyU3PUsbSendEP0Data (2, glEp0Buffer);
                        }
                        else
                            CyU3PUsbStall (0, CyTrue, CyFalse);
                        break;

                    case 0x82:
                        /* Send the USB event log buffer content to the host. */
                        if (wLength != 0)
                        {
                            if (wLength < CYFX_USBLOG_SIZE)
                                CyU3PUsbSendEP0Data (wLength, gl_UsbLogBuffer);
                            else
                                CyU3PUsbSendEP0Data (CYFX_USBLOG_SIZE, gl_UsbLogBuffer);
                        }
                        else
                            CyU3PUsbAckSetup ();
                        break;

                    case 0x83:
                        {
                            uint32_t addr = ((uint32_t)wValue << 16) | (uint32_t)wIndex;
                            CyU3PReadDeviceRegisters ((uvint32_t *)addr, 1, (uint32_t *)glEp0Buffer);
                            CyU3PUsbSendEP0Data (4, glEp0Buffer);
                        }
                        break;

                    case 0x84:
                        {
                            uint8_t major, minor, patch;

                            if (CyU3PUsbGetBooterVersion (&major, &minor, &patch) == CY_U3P_SUCCESS)
                            {
                                glEp0Buffer[0] = major;
                                glEp0Buffer[1] = minor;
                                glEp0Buffer[2] = patch;
                                CyU3PUsbSendEP0Data (3, glEp0Buffer);
                            }
                            else
                                CyU3PUsbStall (0, CyTrue, CyFalse);
                        }
                        break;

                    case 0x90:
                        /* Request to switch control back to the boot firmware. */

                        /* Complete the control request. */
                        CyU3PUsbAckSetup ();
                        CyU3PThreadSleep (10);

                        /* Get rid of the DMA channels and EP configuration. */
                        CyFxBulkSrcSinkApplnStop ();

                        /* De-initialize the Debug and UART modules. */
                        CyU3PDebugDeInit ();
                        CyU3PUartDeInit ();

                        /* Now jump back to the boot firmware image. */
                        CyU3PUsbSetBooterSwitch (CyTrue);
                        CyU3PUsbJumpBackToBooter (0x40078000);
                        while (1)
                            CyU3PThreadSleep (100);
                        break;

                    case 0xE0:
                        /* Request to reset the FX3 device. */
                        CyU3PUsbAckSetup ();
                        CyU3PThreadSleep (2000);							/* Delay for 2 seconds */
                        CyU3PConnectState (CyFalse, CyTrue);
                        CyU3PThreadSleep (1000);							/* Delay for 1 second */
                        CyU3PDeviceReset (CyFalse);
                        CyU3PThreadSleep (1000);							/* Delay for 1 second */
                        break;

                    case 0xE1:
                        /* Request to place FX3 in standby when VBus is next disconnected. */
                        StandbyModeEnable = CyTrue;
                        CyU3PUsbAckSetup ();
                        break;

                    case LED_BLINK_RATE_CHANGE:
                    	/* Vendor command for changing LED blink rate.	*/
                    	CyU3PUsbGetEP0Data (wLength, glEp0Buffer, NULL);
                    	if(glEp0Buffer[0] > 9)
                    	{
                    	   glEp0Buffer[0] = 5;
                    	   CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Value entered greater than 9...Setting ON time to 0.5 seconds\n\r");

                    	}
                    	on_time = glEp0Buffer[0] * 10;
                    	off_time = (10 - glEp0Buffer[0]) * 10;
                    	if(glEp0Buffer[0] == 0)
                    	{

                    	   CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Value entered is 0...LED2 will remain OFF\n\r");
                    	   on_time = 0;
                    	}
                    	else
                    	{
                    		CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Blinking LED2: ON time changed to 0.%d seconds\n\r", glEp0Buffer[0]);
                    	}
                    	break;

                    case 0xBB:
                    	CyU3PUsbGetEP0Data (wLength, glEp0Buffer, NULL);
                    	CyU3PDebugPrint(1, "***Valor recebido: %s\n\r", glEp0Buffer);
                    	CyU3PDebugPrint(1, "***Valor recebido tem: %d caracteres\n\r", strlen(glEp0Buffer));
                    	//on_time = glEp0Buffer[0];
                    	//off_time = glEp0Buffer[0];
                    	break;

                    default:        /* Unknown request. Stall EP0. */
                        CyU3PUsbStall (0, CyTrue, CyFalse);
                        break;
                    }
                }
                else
                {
                    /* Only vendor requests are to be handled here. */
                    CyU3PUsbStall (0, CyTrue, CyFalse);
                }
            }
        }

        /* Try to get the USB 3.0 link back to U0. */
        if (glForceLinkU2)
        {
            stat = CyU3PUsbGetLinkPowerState (&curState);
            while ((glForceLinkU2) && (stat == CY_U3P_SUCCESS) && (curState == CyU3PUsbLPM_U0))
            {
                /* Repeatedly try to go into U2 state.*/
                CyU3PUsbSetLinkPowerState (CyU3PUsbLPM_U2);
                CyU3PThreadSleep (5);
                stat = CyU3PUsbGetLinkPowerState (&curState);
            }
        }
        else
        {

            /* Once data transfer has started, we keep trying to get the USB link to stay in U0. If this is done
               before data transfers have started, there is a likelihood of failing the TD 9.24 U1/U2 test. */
            if ((CyU3PUsbGetSpeed () == CY_U3P_SUPER_SPEED) && (glDataTransStarted))
            {
                /* If the link is in U1/U2 states, try to get back to U0. */
                stat = CyU3PUsbGetLinkPowerState (&curState);
                while ((stat == CY_U3P_SUCCESS) && (curState >= CyU3PUsbLPM_U1) && (curState <= CyU3PUsbLPM_U3) &&
                        (glDataTransStarted))
                {
                    CyU3PUsbSetLinkPowerState (CyU3PUsbLPM_U0);
                    CyU3PThreadSleep (1);
                    stat = CyU3PUsbGetLinkPowerState (&curState);
                }
            }
        }

        if (TriggerStandbyMode)
        {
            TriggerStandbyMode = CyFalse;

            CyU3PConnectState (CyFalse, CyTrue);
            CyU3PUsbStop ();
            CyU3PDebugDeInit ();
            CyU3PUartDeInit ();

            /* VBus has been turned off. Go into standby mode and wait for VBus to be turned on again.
               The I-TCM content and GPIO register state will be backed up in the memory area starting
               at address 0x40060000. */
            stat = CyU3PSysEnterStandbyMode (CY_U3P_SYS_USB_VBUS_WAKEUP_SRC, CY_U3P_SYS_USB_VBUS_WAKEUP_SRC,
                    (uint8_t *)0x40060000);
            if (stat != CY_U3P_SUCCESS)
            {
                CyFxBulkSrcSinkApplnDebugInit ();
                CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "Enter standby returned %d\r\n", stat);
                CyFxAppErrorHandler (stat);
            }

            /* If the entry into standby succeeds, the CyU3PSysEnterStandbyMode function never returns. The
               firmware application starts running again from the main entry point. Therefore, this code
               will never be executed. */
            CyFxAppErrorHandler (1);
        }
        else
        {
            /* Compare the current USB driver log index against the previous value. */
            tmp1 = CyU3PUsbGetEventLogIndex ();
            if (tmp1 != prevUsbLogIndex)
            {
                tmp2 = prevUsbLogIndex;
                while (tmp2 != tmp1)
                {
                    tmp2++;
                    if (tmp2 == CYFX_USBLOG_SIZE)
                        tmp2 = 0;
                }
            }

            /* Store the current log index. */
            prevUsbLogIndex = tmp1;
        }
    }
}
