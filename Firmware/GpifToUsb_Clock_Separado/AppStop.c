/*
 * AppStop.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "AppStop.h"

/* This function stops the application. This shall be called whenever a RESET
 * or DISCONNECT event is received from the USB host. The endpoints are
 * disabled and the DMA pipe is destroyed by this function. */
void
CyFxApplnStop (
        void)
{
    CyU3PEpConfig_t epCfg;
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;

    /* Update the flag so that the application thread is notified of this. */
    glIsApplnActive = CyFalse;

    CyU3PGpifDisable (CyTrue);

    CyU3PDmaChannelDestroy (&glLoopOutChHandle);
#ifndef LOOPBACK_AUTO
    CyU3PDmaChannelDestroy (&glLoopInChHandle);
#endif

    /* Destroy the channels */
    CyU3PDmaChannelDestroy (&glDmaChHandle);

    /* Flush the endpoint memory */
    CyU3PUsbFlushEp(CY_FX_EP_CONSUMER);

    /* Disable endpoints. */
    CyU3PMemSet ((uint8_t *)&epCfg, 0, sizeof (epCfg));
    epCfg.enable = CyFalse;

    /* Disable the GPIF->USB endpoint. */
    apiRetStatus = CyU3PSetEpConfig(CY_FX_EP_CONSUMER, &epCfg);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint (4, "CyU3PSetEpConfig failed, Error code = %d\n", apiRetStatus);
        CyFxAppErrorHandler (apiRetStatus);
    }

    /* Disable the loopback endpoints. */
    CyU3PSetEpConfig (CY_FX_EP_LOOP_OUT, &epCfg);
    CyU3PSetEpConfig (CY_FX_EP_LOOP_IN, &epCfg);
}
