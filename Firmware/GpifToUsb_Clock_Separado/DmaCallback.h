/*
 * DmaCallback.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef DMACALLBACK_H_
#define DMACALLBACK_H_

#include "cyfxgpiftousb.h"

void
GpifToUsbDmaCallback (
        CyU3PDmaChannel   *chHandle,
        CyU3PDmaCbType_t   type,
        CyU3PDmaCBInput_t *input);


void
LoopBackDmaCallback (
        CyU3PDmaChannel   *chHandle,
        CyU3PDmaCbType_t   type,
        CyU3PDmaCBInput_t *input);

#endif /* DMACALLBACK_H_ */
