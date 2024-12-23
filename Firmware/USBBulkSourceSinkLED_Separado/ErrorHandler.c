/*
 * ErrorHandler.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "ErrorHandler.h"

/* Application Error Handler */
void
CyFxAppErrorHandler (
        CyU3PReturnStatus_t apiRetStatus    /* API return status */
        )
{
    /* Application failed with the error code apiRetStatus */

    /* TODO: Error Handling*/

    /* Loop Indefinitely */
    for (;;)
    {
        /* Thread sleep : 100 ms */
        CyU3PThreadSleep (100);
    }
}
