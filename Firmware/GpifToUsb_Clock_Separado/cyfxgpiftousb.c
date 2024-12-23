/*
 ## Cypress USB 3.0 Platform source file (cyfxgpiftousb.c)
 ## ===========================
 ##
 ##  Copyright Cypress Semiconductor Corporation, 2010-2018,
 ##  All Rights Reserved
 ##  UNPUBLISHED, LICENSED SOFTWARE.
 ##
 ##  CONFIDENTIAL AND PROPRIETARY INFORMATION
 ##  WHICH IS THE PROPERTY OF CYPRESS.
 ##
 ##  Use of this file is governed
 ##  by the license agreement included in the file
 ##
 ##     <install>/license/license.txt
 ##
 ##  where <install> is the Cypress software
 ##  installation root directory path.
 ##
 ## ===========================
*/

/*
    This is a sample application that demonstrates data transfer from the FX3 GPIF port to the
    USB port.

    The application makes use a simple GPIF configuration which continually latches the state
    of the GPIF data pins and fills them into full data packets to be sent to the USB host through
    a BULK-IN endpoint. The GPIF configuration and DMA data path are setup to facilitate the fastest
    possible data transfer. By default, the data streaming is done through an AUTO DMA channel;
    but this can be changed to a MANUAL DMA channel using the STREAMING_MANUAL pre-processor
    definition below. When the MANUAL DMA channel is used, the firmware modifies the first and
    last bytes of each 1 KB of data with a sequential number pattern; so that we can check for
    any missing data.

    The application also implements a pair of BULK-OUT and BULK-IN endpoints configured in a
    data loop back configuration. This data loop-back is done with firmware intervention using
    a pair of MANUAL-IN and MANUAL-OUT DMA channels. This can be changed to a hardware based
    AUTO loopback using the LOOPBACK_AUTO pre-processor definition.

    This application also demonstrates the use of the endpoint specific CYU3P_USBEP_SS_RESET_EVT
    event to detect and recover from potential USB data corruption due to protocol level errors.
 */

#include "cyfxgpiftousb.h"

//CyU3PDmaChannel glChHandleBulkSink;      /* DMA MANUAL_IN channel handle.          */
//CyU3PDmaChannel glChHandleBulkSrc;       /* DMA MANUAL_OUT channel handle.         */

//uint32_t glDMARxCount = 0;               /* Counter to track the number of buffers received. */
//uint32_t glDMATxCount = 0;               /* Counter to track the number of buffers transmitted. */
//CyBool_t glDataTransStarted = CyFalse;   /* Whether DMA transfer has been started after enumeration. */
//CyBool_t StandbyModeEnable  = CyFalse;   /* Whether standby mode entry is enabled. */
//CyBool_t TriggerStandbyMode = CyFalse;   /* Request to initiate standby entry. */

//volatile uint32_t glEp0StatCount = 0;           /* Number of EP0 status events received. */
//uint16_t on_time = 0, off_time = 0;

//CyU3PThread     BulkLpAppThread;	        /* Bulk loop application thread structure */
//CyU3PDmaChannel glChHandleBulkLpUtoP;       /* DMA Channel handle  for U2P transfer.*/
//uint8_t currentState;

CyBool_t glIsApplnActive = CyFalse;     /* Whether the application is active or not. */
CyBool_t glForceLinkU2   = CyFalse;     /* Whether the device should try to initiate U2 mode. */

/*
 * USB event logging: We use a 4 KB buffer to store USB driver event data, which can then be viewed
 * through JTAG or USB vendor request.
 */
uint8_t *gl_UsbLogBuffer = NULL;

//volatile uint8_t  DataSignature = 0;     /* Variable used to update streaming data with a sequence number. */


/* Enable this to change the loopback channel into an AUTO channel. */
//#define LOOPBACK_AUTO

/* Enable this to change the streaming channel into a MANUAL channel. */
//#define STREAMING_MANUAL


/*
 * Main function
 */
int
main (void)
{
    CyU3PIoMatrixConfig_t io_cfg;
    CyU3PReturnStatus_t status = CY_U3P_SUCCESS;

    /* Initialize the device */
    CyU3PSysClockConfig_t clockConfig;

    clockConfig.setSysClk400  = CyFalse;
    clockConfig.cpuClkDiv     = 2;
    clockConfig.dmaClkDiv     = 2;
    clockConfig.mmioClkDiv    = 2;
    clockConfig.useStandbyClk = CyFalse;
    clockConfig.clkSrc        = CY_U3P_SYS_CLK;
    status = CyU3PDeviceInit (&clockConfig);
    if (status != CY_U3P_SUCCESS)
    {
        goto handle_fatal_error;
    }

    /* Initialize the caches. The D-Cache is not enabled because it will cause a significant slowing down for
     * an application which does not touch the data in the DMA buffers.
     */
    status = CyU3PDeviceCacheControl (CyTrue, CyFalse, CyFalse);
    if (status != CY_U3P_SUCCESS)
    {
        goto handle_fatal_error;
    }

    CyU3PMemSet ((uint8_t *)&io_cfg, 0, sizeof (io_cfg));
    io_cfg.isDQ32Bit = CyTrue;
    io_cfg.useUart   = CyTrue;
    io_cfg.useI2C    = CyFalse;
    io_cfg.useI2S    = CyFalse;
    io_cfg.useSpi    = CyFalse;
    io_cfg.lppMode   = CY_U3P_IO_MATRIX_LPP_DEFAULT;
    io_cfg.s0Mode    = CY_U3P_SPORT_INACTIVE;
    io_cfg.s1Mode    = CY_U3P_SPORT_INACTIVE;

    /* No GPIOs are enabled. */
    io_cfg.gpioSimpleEn[0]  = 0;
    io_cfg.gpioSimpleEn[1]  = 0;
    io_cfg.gpioComplexEn[0] = 0;
    io_cfg.gpioComplexEn[1] = 0;
    status = CyU3PDeviceConfigureIOMatrix (&io_cfg);
    if (status != CY_U3P_SUCCESS)
    {
        goto handle_fatal_error;
    }

    /* This is a non returnable call for initializing the RTOS kernel */
    CyU3PKernelEntry ();

    /* Dummy return to make the compiler happy */
    return 0;

handle_fatal_error:

    /* Cannot recover from this error. */
    while (1);
}

/* [ ] */

