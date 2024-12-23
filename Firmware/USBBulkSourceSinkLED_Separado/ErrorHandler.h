/*
 * ErrorHandler.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include "cyfxbulksrcsink.h"

void
CyFxAppErrorHandler (
        CyU3PReturnStatus_t apiRetStatus    /* API return status */
        );

#endif /* ERRORHANDLER_H_ */
