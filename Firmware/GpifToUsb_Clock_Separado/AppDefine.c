/*
 * AppDefine.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "AppDefine.h"

/* Application define function which creates the threads. */
void
CyFxApplicationDefine (
        void)
{
    void *ptr = NULL;
    uint32_t ret = CY_U3P_SUCCESS;

    /* Allocate the memory for the threads */
    ptr = CyU3PMemAlloc (CY_FX_GPIFTOUSB_THREAD_STACK);

    /* Create the thread for the application */
    ret = CyU3PThreadCreate (&glAppThread,                      /* App thread structure */
                          "21:Gpit_to_USB",                     /* Thread ID and thread name */
                          CyFxAppThread_Entry,                  /* App thread entry function */
                          0,                                    /* No input parameter to thread */
                          ptr,                                  /* Pointer to the allocated thread stack */
                          CY_FX_GPIFTOUSB_THREAD_STACK,         /* App thread stack size */
                          CY_FX_GPIFTOUSB_THREAD_PRIORITY,      /* App thread priority */
                          CY_FX_GPIFTOUSB_THREAD_PRIORITY,      /* App thread priority */
                          CYU3P_NO_TIME_SLICE,                  /* No time slice for the application thread */
                          CYU3P_AUTO_START                      /* Start the thread immediately */
                          );

    /* Check the return code */
    if (ret != 0)
    {
        /* Thread Creation failed with the error code retThrdCreate */

        /* Add custom recovery or debug actions here */

        /* Application cannot continue */
        /* Loop indefinitely */
        while(1);
    }
}
