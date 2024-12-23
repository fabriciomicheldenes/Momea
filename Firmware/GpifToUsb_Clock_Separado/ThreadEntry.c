/*
 * ThreadEntry.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "ThreadEntry.h"

/* Entry function for the glAppThread. */
void
CyFxAppThread_Entry (
        uint32_t input)
{
    CyU3PReturnStatus_t stat;
    CyU3PUsbLinkPowerMode curState;

    /* Initialize the debug module */
    CyFxApplnDebugInit();
    CyU3PDebugPrint (1, "\n\ndebug initialized\r\n");

    /* Initialize the application */
    CyFxApplnInit();

    while (!glIsApplnActive)
        CyU3PThreadSleep (100);

    for (;;)
    {
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

        if (glRstRqt)
        {
            glRstRqt = CyFalse;
            CyU3PConnectState (CyFalse, CyTrue);
            CyU3PThreadSleep (1000);
            CyU3PDeviceReset (CyFalse);
            while (1)
                CyU3PThreadSleep (1);
        }

        CyU3PThreadSleep (10);
    }
}
