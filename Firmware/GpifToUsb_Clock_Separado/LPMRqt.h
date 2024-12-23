/*
 * LPMRqt.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef LPMRQT_H_
#define LPMRQT_H_

#include "cyfxgpiftousb.h"

/* Callback function to handle LPM requests from the USB 3.0 host. This function is invoked by the API
   whenever a state change from U0 -> U1 or U0 -> U2 happens. If we return CyTrue from this function, the
   FX3 device is retained in the low power state. If we return CyFalse, the FX3 device immediately tries
   to trigger an exit back to U0.

   This application does not have any state in which we should not allow U1/U2 transitions; and therefore
   the function always return CyTrue.
 */
CyBool_t
CyFxApplnLPMRqtCB (
        CyU3PUsbLinkPowerMode link_mode);

#endif /* LPMRQT_H_ */
