/*
 * DmaCallback.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef DMACALLBACK_H_
#define DMACALLBACK_H_

#include "cyfxbulksrcsink.h"

void
CyFxBulkSrcSinkDmaCallback (
        CyU3PDmaChannel   *chHandle, /* Handle to the DMA channel. */
        CyU3PDmaCbType_t  type,      /* Callback type.             */
        CyU3PDmaCBInput_t *input);   /* Callback status.           */

#endif /* DMACALLBACK_H_ */
