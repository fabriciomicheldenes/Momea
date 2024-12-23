/*
 * PibDllParameters.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef PIBDLLPARAMETERS_H_
#define PIBDLLPARAMETERS_H_

#include "cyfxgpiftousb.h"

void
CyFxApplnSetPibDllParameters (
        CyBool_t isEnable,              /* Whether to enable the DLL. */
        uint8_t  corePhase,             /* Delay selection for the PIB core clock.
                                           Takes a value between 0 and 15, and applies a delay of
                                           (corePhase * 22.5 degrees) from the master clock. */
        uint8_t  syncPhase,             /* Delay selection for the data synchronizer clock used in async modes. */
        uint8_t  opPhase                /* Delay selection for the output clock driven by FX3. */
        );

#endif /* PIBDLLPARAMETERS_H_ */
