/*
 * USBSetup.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef USBSETUP_H_
#define USBSETUP_H_

#include "cyfxgpiftousb.h"

/* Callback to handle the USB setup requests. */
CyBool_t
CyFxApplnUSBSetupCB (
        uint32_t setupdat0, /* SETUP Data 0 */
        uint32_t setupdat1  /* SETUP Data 1 */
    );

#endif /* USBSETUP_H_ */
