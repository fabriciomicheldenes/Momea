/*
 * AppStart.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "AppStart.h"
#include "DMACallback.h"
#include "ErrorHandler.h"

/* This function starts the application. This is called
 * when a SET_CONF event is received from the USB host. The endpoints
 * are configured and the DMA pipe is setup in this function. */
void
CyFxBulkSrcSinkApplnStart (
        void)
{
    uint16_t size = 0, index = 0;
    CyU3PEpConfig_t epCfg;
    CyU3PDmaBuffer_t buf_p;
    CyU3PDmaChannelConfig_t dmaCfg;
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PUSBSpeed_t usbSpeed = CyU3PUsbGetSpeed();


    /* First identify the usb speed. Once that is identified,
     * create a DMA channel and start the transfer on this. */

    /* Based on the Bus Speed configure the endpoint packet size */
    switch (usbSpeed)
    {
    case CY_U3P_FULL_SPEED:
        size = CY_FX_FULL_SPEED_EP_SIZE;							/* If connected to full speed, the endpoint size should be 64 bytes */
        on_time = 0;						/* if connected to full-speed, the LED should be always OFF */
        off_time = 1000;
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Connected to Full-Speed host \r\n");
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"LED2 (Blue LED) remains OFF... \r\n");
        break;

    case CY_U3P_HIGH_SPEED:
        size = CY_FX_HIGH_SPEED_EP_SIZE;							/* If connected to high speed, the endpoint size should be 512 bytes */
        on_time = 1000;						/* if connected to high-speed, the LED should be always ON */
        off_time = 200;
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Connected to High-Speed host \r\n");
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"LED2 (Blue LED) remains ON... \r\n");
        break;

    case  CY_U3P_SUPER_SPEED:
        size = CY_FX_SUPER_SPEED_EP_SIZE;						/* If connected to Super-speed, the endpoint size should be 1024 bytes */
        on_time = SUPERSPEED_BLINK_RATE;	/* if connected to Super-speed, the LED should blink */
        off_time = SUPERSPEED_BLINK_RATE;
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Connected to Super-Speed host \r\n");
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY,"Blinking LED2 (Blue LED) with ON time of 0.005 seconds... \r\n");
        break;

    default:
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "Error! Invalid USB speed.\n");
        CyFxAppErrorHandler (CY_U3P_ERROR_FAILURE);
        break;
    }

    CyU3PMemSet ((uint8_t *)&epCfg, 0, sizeof (epCfg));
    epCfg.enable = CyTrue;
    epCfg.epType = CY_U3P_USB_EP_BULK;
    epCfg.burstLen = (usbSpeed == CY_U3P_SUPER_SPEED) ?
        (CY_FX_EP_BURST_LENGTH) : 1;
    epCfg.streams = 0;
    epCfg.pcktSize = size;

    /* Producer endpoint configuration */
    apiRetStatus = CyU3PSetEpConfig(CY_FX_EP_PRODUCER, &epCfg);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PSetEpConfig failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler (apiRetStatus);
    }

    /* Consumer endpoint configuration */
    apiRetStatus = CyU3PSetEpConfig(CY_FX_EP_CONSUMER, &epCfg);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PSetEpConfig failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler (apiRetStatus);
    }

    /* Flush the endpoint memory */
    CyU3PUsbFlushEp(CY_FX_EP_PRODUCER);
    CyU3PUsbFlushEp(CY_FX_EP_CONSUMER);

    /* Create a DMA MANUAL_IN channel for the producer socket. */
    CyU3PMemSet ((uint8_t *)&dmaCfg, 0, sizeof (dmaCfg));

    /* Set the DMA buffer size and count based on constants defined in the header file. */
    dmaCfg.size  = CY_FX_BULKSRCSINK_DMA_BUF_SIZE;
    dmaCfg.count = CY_FX_BULKSRCSINK_DMA_BUF_COUNT;
    dmaCfg.prodSckId = CY_FX_EP_PRODUCER_SOCKET;
    dmaCfg.consSckId = CY_U3P_CPU_SOCKET_CONS;
    dmaCfg.dmaMode = CY_U3P_DMA_MODE_BYTE;
    dmaCfg.notification = CY_U3P_DMA_CB_PROD_EVENT;
    dmaCfg.cb = CyFxBulkSrcSinkDmaCallback;
    dmaCfg.prodHeader = 0;
    dmaCfg.prodFooter = 0;
    dmaCfg.consHeader = 0;
    dmaCfg.prodAvailCount = 0;

    apiRetStatus = CyU3PDmaChannelCreate (&glChHandleBulkSink,
            CY_U3P_DMA_TYPE_MANUAL_IN, &dmaCfg);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDmaChannelCreate failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Create a DMA MANUAL_OUT channel for the consumer socket. */
    dmaCfg.notification = CY_U3P_DMA_CB_CONS_EVENT;
    dmaCfg.prodSckId = CY_U3P_CPU_SOCKET_PROD;
    dmaCfg.consSckId = CY_FX_EP_CONSUMER_SOCKET;
    apiRetStatus = CyU3PDmaChannelCreate (&glChHandleBulkSrc,
            CY_U3P_DMA_TYPE_MANUAL_OUT, &dmaCfg);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDmaChannelCreate failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Set DMA Channel transfer size */
    apiRetStatus = CyU3PDmaChannelSetXfer (&glChHandleBulkSink, CY_FX_BULKSRCSINK_DMA_TX_SIZE);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDmaChannelSetXfer failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    apiRetStatus = CyU3PDmaChannelSetXfer (&glChHandleBulkSrc, CY_FX_BULKSRCSINK_DMA_TX_SIZE);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDmaChannelSetXfer failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler(apiRetStatus);
    }

    /* Now preload all buffers in the MANUAL_OUT pipe with the required data. */
    for (index = 0; index < CY_FX_BULKSRCSINK_DMA_BUF_COUNT; index++)
    {
        apiRetStatus = CyU3PDmaChannelGetBuffer (&glChHandleBulkSrc, &buf_p, CYU3P_NO_WAIT);
        if (apiRetStatus != CY_U3P_SUCCESS)
        {
            CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDmaChannelGetBuffer failed, Error code = %d\n", apiRetStatus);
            CyFxAppErrorHandler(apiRetStatus);
        }
        CyU3PMemSet (buf_p.buffer, CY_FX_BULKSRCSINK_PATTERN, buf_p.size);
        apiRetStatus = CyU3PDmaChannelCommitBuffer (&glChHandleBulkSrc, buf_p.size, 0);
        if (apiRetStatus != CY_U3P_SUCCESS)
        {
            CyU3PDebugPrint(CY_FX_DEBUG_PRIORITY, "CyU3PDmaChannelCommitBuffer failed, Error code = %d\n", apiRetStatus);
            CyFxAppErrorHandler(apiRetStatus);
        }
    }

    /* Update the flag so that the application thread is notified of this. */
    glIsApplnActive = CyTrue;
}
