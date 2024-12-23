/*
 * USBSetup.h
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#ifndef USBSETUP_H_
#define USBSETUP_H_

#include "cyfxbulksrcsink.h"

CyBool_t
CyFxBulkSrcSinkApplnUSBSetupCB (
        uint32_t setupdat0, /* SETUP Data 0 */
        uint32_t setupdat1  /* SETUP Data 1 */
    );

#endif /* USBSETUP_H_ */
