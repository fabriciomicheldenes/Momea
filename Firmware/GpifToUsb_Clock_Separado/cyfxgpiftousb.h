/*
 ## Cypress USB 3.0 Platform header file (cyfxgpiftousb.h)
 ## ===========================
 ##
 ##  Copyright Cypress Semiconductor Corporation, 2010-2018,
 ##  All Rights Reserved
 ##  UNPUBLISHED, LICENSED SOFTWARE.
 ##
 ##  CONFIDENTIAL AND PROPRIETARY INFORMATION
 ##  WHICH IS THE PROPERTY OF CYPRESS.
 ##
 ##  Use of this file is governed
 ##  by the license agreement included in the file
 ##
 ##     <install>/license/license.txt
 ##
 ##  where <install> is the Cypress software
 ##  installation root directory path.
 ##
 ## ===========================
*/

/* This file contains the constants used by the bulk source sink application example */

#ifndef _INCLUDED_CYFXGPIFTOUSB_H_
#define _INCLUDED_CYFXGPIFTOUSB_H_

#include "cyu3types.h"
#include "cyu3usbconst.h"
#include "cyu3externcstart.h"
#include "cyu3system.h"
#include "cyu3os.h"
#include "cyu3dma.h"
#include "cyu3error.h"
#include "cyu3usb.h"
#include "cyu3uart.h"
#include "cyu3gpio.h"
#include "cyu3utils.h"
#include "cyu3pib.h"
#include "cyu3gpif.h"

#define CY_FX_GPIFTOUSB_DMA_TX_SIZE        (0)                       /* DMA transfer size is set to infinite */
#define CY_FX_GPIFTOUSB_THREAD_STACK       (0x1000)                  /* Bulk loop application thread stack size */
#define CY_FX_GPIFTOUSB_THREAD_PRIORITY    (8)                       /* Bulk loop application thread priority */
#define CY_FX_GPIFTOUSB_PATTERN            (0xAA)                    /* 8-bit pattern to be loaded to the source buffers. */

/* Endpoint and socket definitions for the bulk source sink application */

/* To change the producer and consumer EP enter the appropriate EP numbers for the #defines.
 * In the case of IN endpoints enter EP number along with the direction bit.
 * For eg. EP 6 IN endpoint is 0x86
 *     and EP 6 OUT endpoint is 0x06.
 * To change sockets mention the appropriate socket number in the #defines. */

/* Note: For USB 2.0 the endpoints and corresponding sockets are one-to-one mapped
         i.e. EP 1 is mapped to UIB socket 1 and EP 2 to socket 2 so on */

#define CY_FX_EP_CONSUMER               0x81    /* EP 1 IN */
#define CY_FX_EP_CONSUMER_SOCKET        CY_U3P_UIB_SOCKET_CONS_1    /* Socket 1 is consumer */
#define CY_FX_GPIF_PRODUCER_SOCKET      CY_U3P_PIB_SOCKET_0

/* Burst mode definitions: Only for super speed operation. The maximum burst mode 
 * supported is limited by the USB hosts available. The maximum value for this is 16
 * and the minimum (no-burst) is 1. */

/* Burst length in 1 KB packets. Only applicable to USB 3.0. */
#ifndef CY_FX_EP_BURST_LENGTH
#define CY_FX_EP_BURST_LENGTH           (16)
#endif

/* Size of DMA buffers used by the application. */
#ifndef CY_FX_DMA_BUF_SIZE
#define CY_FX_DMA_BUF_SIZE              (32768)
#endif

/* Number of DMA buffers to be used on the channel. */
#ifndef CY_FX_DMA_BUF_COUNT
#define CY_FX_DMA_BUF_COUNT             (4)
#endif

/* Parameters for loop-back function. */
#define CY_FX_EP_LOOP_OUT               (0x02)
#define CY_FX_EP_LOOP_IN                (0x82)
#define CY_FX_LOOP_PRODUCER_SOCK        CY_U3P_UIB_SOCKET_PROD_2
#define CY_FX_LOOP_CONSUMER_SOCK        CY_U3P_UIB_SOCKET_CONS_2

/* Extern definitions for the USB Descriptors */
extern const uint8_t CyFxUSB20DeviceDscr[];
extern const uint8_t CyFxUSB30DeviceDscr[];
extern const uint8_t CyFxUSBDeviceQualDscr[];
extern const uint8_t CyFxUSBFSConfigDscr[];
extern const uint8_t CyFxUSBHSConfigDscr[];
extern const uint8_t CyFxUSBBOSDscr[];
extern const uint8_t CyFxUSBSSConfigDscr[];
extern const uint8_t CyFxUSBStringLangIDDscr[];
extern const uint8_t CyFxUSBManufactureDscr[];
extern const uint8_t CyFxUSBProductDscr[];
extern const uint8_t CyFxUsbOSDscr[];

#include <cyu3externcend.h>

#define CY_FX3_PIB_DLL_CTRL_REG         (*(uvint32_t *)(0xe0010028))

#define CY_FX3_PIB_DLL_ENABLE           (0x00000001)                    /* Bit to enable the DLL. */
#define CY_FX3_PIB_DLL_HIGH_FREQ        (0x00000002)                    /* Set DLL into high freq (> 70 MHz) mode. */
#define CY_FX3_PIB_DLL_LOCK_STAT        (0x00000004)                    /* DLL lock status. */
#define CY_FX3_PIB_DLL_CORE_PHASE_POS   (4)                             /* Position of core clock phase field. */
#define CY_FX3_PIB_DLL_SYNC_PHASE_POS   (8)                             /* Position of sync clock phase field. */
#define CY_FX3_PIB_DLL_OP_PHASE_POS     (12)                            /* Position of output clock phase field. */
#define CY_FX3_PIB_DLL_RESET_N          (0x40000000)                    /* DLL reset request bit. */



extern CyU3PDmaChannel glChHandleBulkSink;      /* DMA MANUAL_IN channel handle.          */
extern CyU3PDmaChannel glChHandleBulkSrc;       /* DMA MANUAL_OUT channel handle.         */

//extern uint32_t glDMARxCount;               /* Counter to track the number of buffers received. */
//extern uint32_t glDMATxCount;               /* Counter to track the number of buffers transmitted. */
//extern CyBool_t glDataTransStarted;   /* Whether DMA transfer has been started after enumeration. */
//extern CyBool_t StandbyModeEnable;   /* Whether standby mode entry is enabled. */
//extern CyBool_t TriggerStandbyMode;   /* Request to initiate standby entry. */

//extern volatile uint32_t glEp0StatCount;           /* Number of EP0 status events received. */
//extern uint16_t on_time;
//extern uint16_t off_time;

/* Control request related variables. */
CyU3PEvent glBulkLpEvent;       /* Event group used to signal the thread that there is a pending request. */
uint32_t   gl_setupdat0;        /* Variable that holds the setupdat0 value (bmRequestType, bRequest and wValue). */
uint32_t   gl_setupdat1;        /* Variable that holds the setupdat1 value (wIndex and wLength). */
#define CYFX_USB_CTRL_TASK      (1 << 0)        /* Event that indicates that there is a pending USB control request. */
#define CYFX_USB_HOSTWAKE_TASK  (1 << 1)        /* Event that indicates the a Remote Wake should be attempted. */

/* GPIO used for testing IO state retention when switching from boot firmware to full firmware. */
#define FX3_GPIO_TEST_OUT               (50)
#define FX3_GPIO_TO_LOFLAG(gpio)        (1 << (gpio))
#define FX3_GPIO_TO_HIFLAG(gpio)        (1 << ((gpio) - 32))

CyU3PThread     glAppThread;            /* Application thread structure */
CyU3PDmaChannel glDmaChHandle;
CyU3PDmaChannel glLoopOutChHandle;
CyU3PDmaChannel glLoopInChHandle;

extern CyBool_t glIsApplnActive;     /* Whether the application is active or not. */
extern CyBool_t glForceLinkU2;     /* Whether the device should try to initiate U2 mode. */

extern volatile uint8_t  vendorRqtCnt;
extern volatile uint32_t underrunCnt;
extern volatile CyBool_t glRstRqt;

/*
 * USB event logging: We use a 4 KB buffer to store USB driver event data, which can then be viewed
 * through JTAG or USB vendor request.
 */
#define CYFX_USBLOG_SIZE (0x1000)
extern uint8_t *gl_UsbLogBuffer;

extern volatile uint32_t BulkEpEvtCount;    /* Number of endpoint events received on streaming endpoint. */
extern volatile uint32_t InEpEvtCount;      /* Number of endpoint events received on loopback IN endpoint. */
extern volatile uint32_t OutEpEvtCount;     /* Number of endpoint events received on loopback OUT endpoint. */
extern volatile uint8_t  DataSignature;     /* Variable used to update streaming data with a sequence number. */

#endif /* _INCLUDED_CYFXGPIFTOUSB_H_ */

/*[]*/
