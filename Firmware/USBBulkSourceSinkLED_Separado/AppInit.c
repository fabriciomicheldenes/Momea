/*
 * AppInit.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */


#include "AppInit.h"
#include "ErrorHandler.h"
#include "AppStart.h"
#include "AppStop.h"

/* This function initializes the USB Module, sets the enumeration descriptors.
 * This function does not start the bulk streaming and this is done only when
 * SET_CONF event is received. */
void
CyFxBulkSrcSinkApplnInit (void)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyBool_t no_renum = CyFalse;

    CyU3PGpioClock_t  gpioClock;
	CyU3PGpioSimpleConfig_t gpioConfig;

	/* Initialize the GPIO block. If we are transitioning from the boot app, we can verify whether the GPIO
	   state is retained. */
	gpioClock.fastClkDiv = 2;
	gpioClock.slowClkDiv = 32;
	gpioClock.simpleDiv  = CY_U3P_GPIO_SIMPLE_DIV_BY_16;
	gpioClock.clkSrc     = CY_U3P_SYS_CLK_BY_2;
	gpioClock.halfDiv    = 0;
	apiRetStatus = CyU3PGpioInit (&gpioClock, NULL);


	/* Override GPIO 54 as this pin is associated with UART CTS signal.
		 * The IO cannot be selected as GPIO by CyU3PDeviceConfigureIOMatrix call
		 * as it is part of the GPIF IOs. Override API call must be made with
		 * caution as this will change the functionality of the pin. If the IO
		 * line is used as part of UART and is connected to some external device,
		 * then the line will no longer behave as a UART CTS line.. Here CTS line is
		 * not used and so it is safe to override.  */
		apiRetStatus = CyU3PDeviceGpioOverride (LED_GPIO, CyTrue);
		if (apiRetStatus != 0)
		{
			/* Error Handling */
			CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDeviceGpioOverride failed, error code = %d\n",
					apiRetStatus);
			CyFxAppErrorHandler(apiRetStatus);
		}

		/* Configure GPIO 54 as output */
		gpioConfig.outValue = CyTrue;
		gpioConfig.driveLowEn = CyTrue;
		gpioConfig.driveHighEn = CyTrue;
		gpioConfig.inputEn = CyFalse;
		gpioConfig.intrMode = CY_U3P_GPIO_NO_INTR;
		apiRetStatus = CyU3PGpioSetSimpleConfig(LED_GPIO, &gpioConfig);	/* Set GPIO54 as an output pin to drive the LED */
		if (apiRetStatus != CY_U3P_SUCCESS)
		{
			/* Error handling */
			CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PGpioSetSimpleConfig failed, error code = %d\n",
					apiRetStatus);
			CyFxAppErrorHandler(apiRetStatus);
		}


    /* Start the USB functionality. */
    apiRetStatus = CyU3PUsbStart();
    if (apiRetStatus == CY_U3P_ERROR_NO_REENUM_REQUIRED)
        no_renum = CyTrue;
    else if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PUsbStart failed to Start, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* The fast enumeration is the easiest way to setup a USB connection,
     * where all enumeration phase is handled by the library. Only the
     * class / vendor requests need to be handled by the application. */
    CyU3PUsbRegisterSetupCallback(CyFxBulkSrcSinkApplnUSBSetupCB, CyTrue);

    /* Setup the callback to handle the USB events. */
    CyU3PUsbRegisterEventCallback(CyFxBulkSrcSinkApplnUSBEventCB);

    /* Register a callback to handle LPM requests from the USB 3.0 host. */
    CyU3PUsbRegisterLPMRequestCallback(CyFxBulkSrcSinkApplnLPMRqtCB);

    /* Set the USB Enumeration descriptors */

    /* Super speed device descriptor. */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_SS_DEVICE_DESCR, NULL, (uint8_t *)CyFxUSB30DeviceDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set device descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* High speed device descriptor. */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_HS_DEVICE_DESCR, NULL, (uint8_t *)CyFxUSB20DeviceDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set device descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* BOS descriptor */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_SS_BOS_DESCR, NULL, (uint8_t *)CyFxUSBBOSDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set configuration descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Device qualifier descriptor */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_DEVQUAL_DESCR, NULL, (uint8_t *)CyFxUSBDeviceQualDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set device qualifier descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Super speed configuration descriptor */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_SS_CONFIG_DESCR, NULL, (uint8_t *)CyFxUSBSSConfigDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set configuration descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* High speed configuration descriptor */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_HS_CONFIG_DESCR, NULL, (uint8_t *)CyFxUSBHSConfigDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB Set Other Speed Descriptor failed, Error Code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Full speed configuration descriptor */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_FS_CONFIG_DESCR, NULL, (uint8_t *)CyFxUSBFSConfigDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB Set Configuration Descriptor failed, Error Code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* String descriptor 0 */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_STRING_DESCR, 0, (uint8_t *)CyFxUSBStringLangIDDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set string descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* String descriptor 1 */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_STRING_DESCR, 1, (uint8_t *)CyFxUSBManufactureDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set string descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* String descriptor 2 */
    apiRetStatus = CyU3PUsbSetDesc(CY_U3P_USB_SET_STRING_DESCR, 2, (uint8_t *)CyFxUSBProductDscr);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB set string descriptor failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Register a buffer into which the USB driver can log relevant events. */
    gl_UsbLogBuffer = (uint8_t *)CyU3PDmaBufferAlloc (CYFX_USBLOG_SIZE);
    if (gl_UsbLogBuffer)
        CyU3PUsbInitEventLog (gl_UsbLogBuffer, CYFX_USBLOG_SIZE);

    CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "About to connect to USB host\r\n");

    /* Connect the USB Pins with super speed operation enabled. */
    if (!no_renum) {
        apiRetStatus = CyU3PConnectState(CyTrue, CyTrue);
        if (apiRetStatus != CY_U3P_SUCCESS)
        {
            CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "USB Connect failed, Error code = %d\n", apiRetStatus);
            CyFxAppErrorHandler(apiRetStatus);
        }
    }
    else
    {
        /* USB connection is already active. Configure the endpoints and DMA channels. */
        if (glIsApplnActive)
            CyFxBulkSrcSinkApplnStop ();
        CyFxBulkSrcSinkApplnStart ();
    }
    CyU3PDebugPrint (CY_FX_DEBUG_PRIORITY, "CyFxBulkSrcSinkApplnInit complete\r\n");
}
