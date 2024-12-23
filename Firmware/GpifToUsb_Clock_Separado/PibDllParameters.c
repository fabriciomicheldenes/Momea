/*
 * PibDllParameters.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "PibDllParameters.h"

void
CyFxApplnSetPibDllParameters (
        CyBool_t isEnable,              /* Whether to enable the DLL. */
        uint8_t  corePhase,             /* Delay selection for the PIB core clock.
                                           Takes a value between 0 and 15, and applies a delay of
                                           (corePhase * 22.5 degrees) from the master clock. */
        uint8_t  syncPhase,             /* Delay selection for the data synchronizer clock used in async modes. */
        uint8_t  opPhase                /* Delay selection for the output clock driven by FX3. */
        )
{
    /* Disable DLL */
    CY_FX3_PIB_DLL_CTRL_REG &= ~(CY_FX3_PIB_DLL_ENABLE);
    CyU3PBusyWait (1);

    if (!isEnable)
        return;

    /* Configure and enable the DLL. */
    CY_FX3_PIB_DLL_CTRL_REG = (
            ((corePhase & 0x0F) << CY_FX3_PIB_DLL_CORE_PHASE_POS) |
            ((syncPhase & 0x0F) << CY_FX3_PIB_DLL_SYNC_PHASE_POS) |
            ((opPhase & 0x0F)   << CY_FX3_PIB_DLL_OP_PHASE_POS) |
            CY_FX3_PIB_DLL_HIGH_FREQ |
            CY_FX3_PIB_DLL_ENABLE
            );

    /* Reset the DLL */
    CY_FX3_PIB_DLL_CTRL_REG &= ~(CY_FX3_PIB_DLL_RESET_N);
    CyU3PBusyWait (1);

    /* Clear Reset */
    CY_FX3_PIB_DLL_CTRL_REG |= CY_FX3_PIB_DLL_RESET_N;
    CyU3PBusyWait (1);

    /* Wait for DLL to lock */
    while (!(CY_FX3_PIB_DLL_CTRL_REG & CY_FX3_PIB_DLL_LOCK_STAT));
}

