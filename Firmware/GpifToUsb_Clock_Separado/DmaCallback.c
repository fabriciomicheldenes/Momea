/*
 * DmaCallback.c
 *
 *  Created on: 22/12/2024
 *      Author: FMichelD
 */

#include "DmaCallback.h"

void
GpifToUsbDmaCallback (
        CyU3PDmaChannel   *chHandle,
        CyU3PDmaCbType_t   type,
        CyU3PDmaCBInput_t *input)
{
#ifdef STREAMING_MANUAL
    if (type == CY_U3P_DMA_CB_PROD_EVENT)
    {
        /* Change the first and last data bytes for tracking on a USB trace. */
        input->buffer_p.buffer[0x0000] = DataSignature++;
        input->buffer_p.buffer[input->buffer_p.count - 1] = DataSignature++;

        CyU3PDmaChannelCommitBuffer (chHandle, input->buffer_p.count, 0);
    }

    if (type == CY_U3P_DMA_CB_CONS_EVENT)
    {
        /* Data transfer has been started. Enable the LPM disable loop. */
    }
#endif
}

void
LoopBackDmaCallback (
        CyU3PDmaChannel   *chHandle,
        CyU3PDmaCbType_t   type,
        CyU3PDmaCBInput_t *input)
{
    CyU3PDmaBuffer_t    dmaInfo;
    CyU3PReturnStatus_t status;

    /* Copy and commit the data on the IN endpoint and discard the current buffer. */
    if (type == CY_U3P_DMA_CB_PROD_EVENT)
    {

        /* Note: We expect that data will be read from the IN endpoint on a timely basis,
         *       and that we do not have a case where all the buffers fill up on the device
         *       side. We are only using a 15ms timeout because the USB driver might end up
         *       holding a Mutex lock on the channel for upto 10 ms while processing EP0
         *       vendor commands.
         */
        status = CyU3PDmaChannelGetBuffer (&glLoopInChHandle, &dmaInfo, 15);
        if (status == CY_U3P_SUCCESS)
        {
            CyU3PMemCopy (dmaInfo.buffer, input->buffer_p.buffer, input->buffer_p.count);
            CyU3PDmaChannelCommitBuffer (&glLoopInChHandle, input->buffer_p.count, 0);
            CyU3PDmaChannelDiscardBuffer (chHandle);
        }

    }
}
