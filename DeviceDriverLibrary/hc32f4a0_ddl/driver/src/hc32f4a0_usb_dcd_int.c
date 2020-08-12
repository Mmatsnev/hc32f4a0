/**
 *******************************************************************************
 * @file  hc32f4a0_usb_dcd_int.c
 * @brief Peripheral Device interrupt subroutines.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin         First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 *******************************************************************************
 */


/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "hc32f4a0_usb_dcd_int.h"
#include "usb_bsp.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_USB_DCD_INT USB DCD INT
 * @brief USB interrupt handle for device mode.
 * @{
 */

#if (DDL_USBFS_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @defgroup USB_DCD_INT_Local_Functions USB DCD INT Local Functions
 * @{
 */

static uint32_t DCD_ReadDevInEP (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);

/* Interrupt Handlers */
static uint32_t DCD_HandleInEP_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleSof_ISR(USB_OTG_CORE_HANDLE *pdev);

static uint32_t DCD_HandleRxStatusQueueLevel_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pdev , uint32_t epnum);

static uint32_t DCD_HandleUsbReset_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleEnumDone_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleResume_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_HandleUSBSuspend_ISR(USB_OTG_CORE_HANDLE *pdev);

static uint32_t DCD_IsoINIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t DCD_IsoOUTIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev);
#ifdef VBUS_SENSING_ENABLED
static uint32_t DCD_SessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev);
//static uint32_t DCD_OTG_ISR(USB_OTG_CORE_HANDLE *pdev);
#endif

/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @defgroup USB_DCD_INT_Global_Functions USB DCD INT Global Functions
 * @{
 */

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
 * @brief  USBD_OTG_EP1OUT_ISR_Handler
 *        handles all USB Interrupts
 * @param  [in] pdev     device instance
 * @retval status
 */
uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DOEPINTn_TypeDef  doepint;
    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;

    doepint.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[1U]->DOEPINT);
    doepint.d32&= USB_OTG_READ_REG32(&pdev->regs.DREGS->DOUTEP1MSK);

    /* Transfer complete */
    if ( doepint.b.xfercompl )
    {
        /* Clear the bit in DOEPINTn for this interrupt */
        CLEAR_OUT_EP_INTR(1U, out_xfercompl);
        if (pdev->cfg.dma_enable == 1U)
        {
            deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[1U]->DOEPTSIZ));
            /*ToDo : handle more than one single MPS size packet */
            pdev->dev.out_ep[1].xfer_count = pdev->dev.out_ep[1U].maxpacket - \
            deptsiz.b.xfersize;
        }
        /* Inform upper layer: data ready */
        /* RX COMPLETE */
        USBD_DCD_INT_fops->DataOutStage(pdev , 1U);
    }

    /* Endpoint disable  */
    if ( doepint.b.epdisabled )
    {
        /* Clear the bit in DOEPINTn for this interrupt */
        CLEAR_OUT_EP_INTR(1U, out_epdisabled);
    }

    return 1UL;
}

/**
 * @brief  USBD_OTG_EP1IN_ISR_Handler
 *         handles all USB Interrupts
 * @param  [in] pdev     device instance
 * @retval status
 */
uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DIEPINTn_TypeDef  diepint;
    uint32_t fifoemptymsk, msk, emp;

    msk = USB_OTG_READ_REG32(&pdev->regs.DREGS->DINEP1MSK);
    emp = USB_OTG_READ_REG32(&pdev->regs.DREGS->DIEPEMPMSK);
    msk |= ((emp >> 1U ) & 0x1U) << 7U;
    diepint.d32  = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[1U]->DIEPINT) & msk;

    if ( diepint.b.xfercompl )
    {
        fifoemptymsk = 0x1U << 1U;
        USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0U);
        CLEAR_IN_EP_INTR(1U, in_xfercompl);
        /* TX COMPLETE */
        USBD_DCD_INT_fops->DataInStage(pdev , 1U);
    }
    if ( diepint.b.epdisabled )
    {
        CLEAR_IN_EP_INTR(1U, in_epdisabled);
    }
    if ( diepint.b.timeout )
    {
        CLEAR_IN_EP_INTR(1U, in_timeout);
    }
    if (diepint.b.intktxfemp)
    {
        CLEAR_IN_EP_INTR(1U, in_intktxfemp);
    }
    if (diepint.b.inepnakeff)
    {
        CLEAR_IN_EP_INTR(1U, in_inepnakeff);
    }
    if (diepint.b.emptyintr)
    {
        DCD_WriteEmptyTxFifo(pdev , 1U);
        CLEAR_IN_EP_INTR(1U, in_emptyintr);
    }
    return 1;
}
#endif /* USB_OTG_HS_DEDICATED_EP1_ENABLED */

/**
 * @brief  USBF_OTG_ISR_Handler
 *         handles all USB Interrupts
 * @param  [in] pdev     Device instance
 * @retval Status
 */
uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintr_status;
    uint32_t retval = 0UL;
    uint32_t ReadCoreItrTemp;   /* C-STAT */

    if (USB_OTG_IsDeviceMode(pdev) != 0U) /* ensure that we are in device mode */
    {
        ReadCoreItrTemp = USB_OTG_ReadCoreItr(pdev);      /* C-STAT */
        gintr_status.b = *(stc_bUSB_OTG_GINTSTS_t*)&ReadCoreItrTemp;   /* C-STAT */
        /* avoid spurious interrupt */
        if (*(uint32_t*)&gintr_status.b == 0UL) /* C-STAT */
        {
            retval = 0UL;  /* MISRAC 2004*/
        }
        /* Out endpoint interrupt */
        else if (gintr_status.b.outepintr != 0U)
        {
            retval |= DCD_HandleOutEP_ISR(pdev);
        }
        /* In endpoint interrupt */
        else if (gintr_status.b.inepint != 0U)
        {
            retval |= DCD_HandleInEP_ISR(pdev);
        }
        /* Mode mismatch interrupt */
        else if (gintr_status.b.modemismatch != 0U)
        {
            USB_OTG_GINTSTS_TypeDef  gintsts;

            /* Clear interrupt */
            gintsts.d32 = 0UL;
            gintsts.b.modemismatch = 1U;
            USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
        }
        /* Resume/remote wakeup detected interrupt */
        else if (gintr_status.b.wkupintr != 0U)
        {
            retval |= DCD_HandleResume_ISR(pdev);
        }
        /* USB suspend interrupt */
        else if (gintr_status.b.usbsuspend != 0U)
        {
            retval |= DCD_HandleUSBSuspend_ISR(pdev);
        }
        /* Start of frame interrupt */
        else if (gintr_status.b.sofintr != 0U)
        {
            retval |= DCD_HandleSof_ISR(pdev);
        }
        /* RxFIFO non-empty interrupt */
        else if (gintr_status.b.rxstsqlvl != 0U)
        {
            retval |= DCD_HandleRxStatusQueueLevel_ISR(pdev);
        }
        /* USB reset interrupt */
        else if (gintr_status.b.usbreset != 0U)
        {
            retval |= DCD_HandleUsbReset_ISR(pdev);
        }
        /* Enumeration done interrupt */
        else if (gintr_status.b.enumdone != 0U)
        {
            retval |= DCD_HandleEnumDone_ISR(pdev);
        }
        /* Incomplete periodic transfer */
        else if (gintr_status.b.incomplisoin != 0U)
        {
            retval |= DCD_IsoINIncomplete_ISR(pdev);
        }
        /* Incomplete isochronous IN transfer */
        else if (gintr_status.b.incomplisoout != 0U)
        {
            retval |= DCD_IsoOUTIncomplete_ISR(pdev);
        }

#ifdef VBUS_SENSING_ENABLED
        /* VBUS valid interrupt */
        else if (gintr_status.b.vbusvint != 0U)
        {
#if (DDL_PRINT_ENABLE == DDL_ON)
            printf("vbus\n");
#endif
            retval |= DCD_SessionRequest_ISR(pdev);
        }

    //    else if (gintr_status.b.otgintr)
    //    {
    //      retval |= DCD_OTG_ISR(pdev);
    //    }
        else
        {
            //
        }
#endif
    }
    return retval;
}

/**
 * @}
 */


/**
 * @addtogroup USB_DCD_INT_Local_Functions USB DCD INT Local Functions
 * @{
 */

#ifdef VBUS_SENSING_ENABLED
/**
 * @brief  DCD_SessionRequest_ISR
 *         Indicates that the USB_OTG controller has detected a connection
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_SessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_PCGCCTL_TypeDef  power;

    USBD_DCD_INT_fops->DevConnected (pdev);
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("SessionRequest !!\n");
#endif
    /* Clear interrupt */
    gintsts.d32 = 0UL;
    gintsts.b.vbusvint = 1U;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    if(pdev->cfg.low_power != 0U)
    {
        /* un-gate USB Core clock */
        power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0U;
        power.b.stoppclk = 0U;
        USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
    }

    return 1UL;
}

/**
 * @brief  DCD_OTG_ISR
 *         Indicates that the USB_OTG controller has detected an OTG event:
 *                used to detect the end of session i.e. disconnection
 * @param  [in] pdev     device instance
 * @retval Status
 */
//static uint32_t DCD_OTG_ISR(USB_OTG_CORE_HANDLE *pdev)
//{
//
//  USB_OTG_GOTGINT_TypeDef  gotgint;
//#if (DDL_PRINT_ENABLE == DDL_ON)
//  printf("\tDCD_OTG_ISR !!\n");
//#endif
//  gotgint.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GOTGINT);
//
//  if (gotgint.b.sesenddet)
//  {
//    USBD_DCD_INT_fops->DevDisconnected (pdev);
//  }
//  /* Clear OTG interrupt */
//  USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GOTGINT, gotgint.d32);
//  return 1;
//}
#endif  /* VBUS_SENSING_ENABLED */

/**
 * @brief  DCD_HandleResume_ISR
 *         Indicates that the USB_OTG controller has detected a resume or
 *                 remote Wake-up sequence
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleResume_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_DCTL_TypeDef     devctl;
    USB_OTG_PCGCCTL_TypeDef  power;
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("resume !!\n");
#endif
    if(pdev->cfg.low_power != 0U)
    {
        /* un-gate USB Core clock */
        power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0U;
        power.b.stoppclk = 0U;
        USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
    }

    /* Clear the Remote Wake-up Signaling */
    devctl.d32 = 0UL;
    devctl.b.rmtwkupsig = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, devctl.d32, 0UL);

    USB_OTG_BSP_mDelay(1UL);
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, devctl.d32, 0UL);

    /* Inform upper layer by the Resume Event */
    USBD_DCD_INT_fops->Resume (pdev);

    /* Clear interrupt */
    gintsts.d32 = 0UL;
    gintsts.b.wkupintr = 1U;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1UL;
}

/**
 * @brief  USB_OTG_HandleUSBSuspend_ISR
 *        Indicates that SUSPEND state has been detected on the USB
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleUSBSuspend_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_PCGCCTL_TypeDef  power;
    USB_OTG_DSTS_TypeDef     dsts;
    __IO uint8_t prev_status = 0U;
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("suspend !!\n");
#endif
    prev_status = pdev->dev.device_status;
    USBD_DCD_INT_fops->Suspend (pdev);

    dsts.b = *(__IO stc_bUSB_OTG_DSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);   /* C-STAT */

    /* Clear interrupt */
    gintsts.d32 = 0UL;
    gintsts.b.usbsuspend = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    if((prev_status  == USB_OTG_CONFIGURED) &&
       (pdev->cfg.low_power != 0U) && (dsts.b.suspsts == 1U)  &&
       (pdev->dev.connection_status == 1U))
    {
        /*  switch-off the clocks */
        power.d32 = 0UL;
        power.b.stoppclk = 1U;
        USB_OTG_MODIFY_REG32(pdev->regs.PCGCCTL, 0UL, power.d32);

        power.b.gatehclk = 1U;
        USB_OTG_MODIFY_REG32(pdev->regs.PCGCCTL, 0UL, power.d32);

        /* Request to enter Sleep mode after exit from current ISR */
        // SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
    }
    return 1UL;
}

/**
 * @brief  DCD_HandleInEP_ISR
 *        Indicates that an IN EP has a pending Interrupt
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleInEP_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DIEPINTn_TypeDef  diepint;

    uint32_t ep_intr;
    uint32_t epnum = 0UL;
    uint32_t fifoemptymsk;
    uint32_t u32ReadInEpTmp; /* C-STAT */

    *(uint32_t*)&diepint.b = 0UL;    /* C-STAT */
    ep_intr = USB_OTG_ReadDevAllInEPItr(pdev);

    while (ep_intr != 0UL)
    {
        if ((ep_intr&0x1UL) != 0UL) /* In ITR */
        {
            //diepint.d32 = DCD_ReadDevInEP(pdev , epnum); /* Get In ITR status */  /* C-STAT */
            u32ReadInEpTmp = DCD_ReadDevInEP(pdev , (uint8_t)epnum);
            diepint.b = *(stc_bUSB_OTG_DIEPINTn_t*)&u32ReadInEpTmp;

            if ( diepint.b.xfercompl != 0U)
            {
                fifoemptymsk = 0x1UL << epnum;
                USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0UL);
                CLEAR_IN_EP_INTR(epnum, in_xfercompl);
                /* TX COMPLETE */
                USBD_DCD_INT_fops->DataInStage(pdev , epnum);

                if (pdev->cfg.dma_enable == 1U)
                {
                    if((epnum == 0U) && (pdev->dev.device_state == USB_OTG_EP0_STATUS_IN))
                    {
                        /* prepare to rx more setup packets */
#if (DDL_PRINT_ENABLE == DDL_ON)
                        //printf("status in\n");
#endif
                        USB_OTG_EP0_OutStart(pdev);
                        pdev->dev.device_state = USB_OTG_EP0_IDLE;
                    }
                }
#if (DDL_PRINT_ENABLE == DDL_ON)
                //printf("ctrl-in %x\n",USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[epnum]->DIEPTSIZ)));
#endif
            }
            if ( diepint.b.timeout != 0U)
            {
                CLEAR_IN_EP_INTR(epnum, in_timeout);
            }
            if (diepint.b.intktxfemp != 0U)
            {
                CLEAR_IN_EP_INTR(epnum, in_intktxfemp);
            }
            if (diepint.b.inepnakeff != 0U)
            {
                CLEAR_IN_EP_INTR(epnum, in_inepnakeff);
            }
            if ( diepint.b.epdisabled != 0U)
            {
                CLEAR_IN_EP_INTR(epnum, in_epdisabled);
            }
            if (diepint.b.emptyintr != 0U)
            {
#if (DDL_PRINT_ENABLE == DDL_ON)
                //printf("empty ep%d\n",epnum);
#endif
                DCD_WriteEmptyTxFifo(pdev , epnum);
                CLEAR_IN_EP_INTR(epnum, in_emptyintr);
            }
        }
        epnum++;
        ep_intr >>= 1U;
    }

    return 1UL;
}

/**
 * @brief  DCD_HandleOutEP_ISR
 *         Indicates that an OUT EP has a pending Interrupt
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t ep_intr;
    USB_OTG_DOEPINTn_TypeDef  doepint;
    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
    uint32_t epnum = 0UL;
    uint32_t xfer;   /* MISRAC*/
    uint32_t u32ReadOutEpTmp; /* C-STAT */
    uint32_t u32ReadEpSizeTmp; /* C-STAT */

    *(__IO uint32_t*)&doepint.b = 0UL;  /* C-STAT */

    /* Read in the device interrupt bits */
    ep_intr = USB_OTG_ReadDevAllOutEp_itr(pdev);

    while ( ep_intr != 0UL)
    {
        if ((ep_intr&0x1UL) != 0UL)
        {
            //doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);  /* C-STAT */
            u32ReadOutEpTmp = USB_OTG_ReadDevOutEP_itr(pdev, (uint8_t)epnum);
            doepint.b = *(__IO stc_bUSB_OTG_DOEPINTn_t*)&u32ReadOutEpTmp;

            /* Transfer complete */
            if ( doepint.b.xfercompl != 0U)
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, out_xfercompl);
                if (pdev->cfg.dma_enable == 1U)
                {
                    //deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));  /* C-STAT */
                    u32ReadEpSizeTmp = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));
                    deptsiz.b = *(stc_bUSB_OTG_DEPXFRSIZ_t*)&u32ReadEpSizeTmp;

                    /*ToDo : handle more than one single MPS size packet */
                    xfer = __MIN(pdev->dev.out_ep[epnum].maxpacket,pdev->dev.out_ep[epnum].xfer_len);
                    pdev->dev.out_ep[epnum].xfer_count = xfer - deptsiz.b.xfersize;
                    if (epnum != 0U)
                    {
                        pdev->dev.out_ep[epnum].xfer_count = pdev->dev.out_ep[epnum].xfer_len - deptsiz.b.xfersize;
                    }
                }
                /* Inform upper layer: data ready */
                /* RX COMPLETE */
                USBD_DCD_INT_fops->DataOutStage(pdev , epnum);

                if (pdev->cfg.dma_enable == 1U)
                {
                    if((epnum == 0U) && (pdev->dev.device_state == USB_OTG_EP0_STATUS_OUT))
                    {
                        /* prepare to rx more setup packets */
#if (DDL_PRINT_ENABLE == DDL_ON)
                        //printf("status out\n");
#endif
                        USB_OTG_EP0_OutStart(pdev);
                        pdev->dev.device_state = USB_OTG_EP0_IDLE;
                    }
                }
            }
            /* Endpoint disable  */
            if ( doepint.b.epdisabled != 0U)
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, out_epdisabled);
            }

            //doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);  /* C-STAT */
            u32ReadOutEpTmp = USB_OTG_ReadDevOutEP_itr(pdev, (uint8_t)epnum);
            doepint.b = *(stc_bUSB_OTG_DOEPINTn_t*)&u32ReadOutEpTmp;

            /* Setup Phase Done (control EPs) */
            if ( doepint.b.setup != 0U)
            {
                /* inform the upper layer that a setup packet is available */
                /* SETUP COMPLETE */
                USBD_DCD_INT_fops->SetupStage(pdev);
                CLEAR_OUT_EP_INTR(epnum, out_setup);
            }
        }
        epnum++;
        ep_intr >>= 1U;
    }
    return 1UL;
}

/**
 * @brief  DCD_HandleSof_ISR
 *         Handles the SOF Interrupts
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleSof_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  GINTSTS;


    USBD_DCD_INT_fops->SOF(pdev);

    /* Clear interrupt */
    GINTSTS.d32 = 0UL;
    GINTSTS.b.sofintr = 1U;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, GINTSTS.d32);

    return 1UL;
}

/**
 * @brief  DCD_HandleRxStatusQueueLevel_ISR
 *         Handles the Rx Status Queue Level Interrupt
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleRxStatusQueueLevel_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTMSK_TypeDef  int_mask;
    USB_OTG_DRXSTS_TypeDef   status;
    USB_OTG_EP *ep;
    uint32_t u32Tmp;

    /* Disable the Rx Status Queue Level interrupt */
    int_mask.d32 = 0UL;
    int_mask.b.rxstsqlvl = 1U;
    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, int_mask.d32, 0UL);

    /* Get the Status from the top of the FIFO */
    //status.b = *(__IO stc_bUSB_OTG_DRXSTS_t*)&USB_OTG_READ_REG32( &pdev->regs.GREGS->GRXSTSP ); /* C-STAT */
    u32Tmp = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRXSTSP );
    status.b = *(__IO stc_bUSB_OTG_DRXSTS_t*)&u32Tmp; /* C-STAT */
    ep = &pdev->dev.out_ep[status.b.epnum];

    switch (status.b.pktsts)
    {
        case STS_GOUT_NAK:
            break;
        case STS_DATA_UPDT:
            if (status.b.bcnt != 0U)
            {
                //if (status.b.epnum == 2)
#if (DDL_PRINT_ENABLE == DDL_ON)
                //    printf("ep%d cnt %d\n",status.b.epnum,status.b.bcnt);
#endif
                USB_OTG_ReadPacket(pdev,ep->xfer_buff, (uint16_t)status.b.bcnt);
                ep->xfer_buff += status.b.bcnt;
                ep->xfer_count += status.b.bcnt;
            } else
            {
#if (DDL_PRINT_ENABLE == DDL_ON)
                ;//printf("ep%d cnt %d\n",status.b.epnum,status.b.bcnt);
#endif
            }
            break;
        case STS_XFER_COMP:
            break;
        case STS_SETUP_COMP:
            break;
        case STS_SETUP_UPDT:
#if (DDL_PRINT_ENABLE == DDL_ON)
            /* Copy the setup packet received in FIFO into the setup buffer in RAM */
            //printf("ep%d setup %d\n",status.b.epnum,status.b.bcnt);
#endif
            USB_OTG_ReadPacket(pdev , pdev->dev.setup_packet, 8U);
            ep->xfer_count += status.b.bcnt;
            break;
        default:
            break;
    }
    /* Enable the Rx Status Queue Level interrupt */
    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, 0UL, int_mask.d32);
    return 1UL;
}

/**
 * @brief  DCD_WriteEmptyTxFifo
 *         check FIFO for the next packet to be loaded
 * @param  [in] pdev     Device instance
 * @param  [in] epnum    Endpoint number
 * @retval Status
 */
static uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pdev, uint32_t epnum)
{
    USB_OTG_DTXFSTSn_TypeDef  txstatus;
    USB_OTG_EP *ep;
    uint32_t len;
    uint32_t len32b;

    *(uint32_t*)&txstatus.b = 0UL;  /* C-STAT */

    ep = &pdev->dev.in_ep[epnum];
    len = ep->xfer_len - ep->xfer_count;

    if (len > ep->maxpacket)
    {
        len = ep->maxpacket;
    }

    len32b = (len + 3U) / 4U;
    txstatus.b = *(__IO stc_bUSB_OTG_DTXFSTSn_t*)&USB_OTG_READ_REG32( &pdev->regs.INEP_REGS[epnum]->DTXFSTS);  /* C-STAT */
#if (DDL_PRINT_ENABLE == DDL_ON)
    //printf("ep%d tx len %d\n",epnum,len);
#endif

    while ((txstatus.b.txfspcavail > len32b) && (ep->xfer_count < ep->xfer_len) && (ep->xfer_len != 0U))
    {
        /* Write the FIFO */
        len = ep->xfer_len - ep->xfer_count;

        if (len > ep->maxpacket)
        {
            len = ep->maxpacket;
        }
        len32b = (len + 3U) / 4U;

        USB_OTG_WritePacket (pdev , ep->xfer_buff, (uint8_t)epnum, (uint16_t)len);

        ep->xfer_buff  += len;
        ep->xfer_count += len;

        txstatus.b = *(__IO stc_bUSB_OTG_DTXFSTSn_t*)&USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[epnum]->DTXFSTS);  /* C-STAT */
    }
    return 1UL;
}

/**
 * @brief  DCD_HandleUsbReset_ISR
 *         This interrupt occurs when a USB Reset is detected
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleUsbReset_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DAINT_TypeDef    daintmsk;
    USB_OTG_DOEPMSK_TypeDef  doepmsk;
    USB_OTG_DIEPMSK_TypeDef  diepmsk;
    USB_OTG_DCFG_TypeDef     dcfg;
    USB_OTG_DCTL_TypeDef     dctl;
    USB_OTG_GINTSTS_TypeDef  gintsts;
    uint32_t i;

    dctl.d32 = 0UL;
    daintmsk.d32 = 0UL;
    doepmsk.d32 = 0UL;
    diepmsk.d32 = 0UL;
    dcfg.d32 = 0UL;
    gintsts.d32 = 0UL;

#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("%s\n",__FUNCTION__);
#endif
    /* Clear the Remote Wake-up Signaling */
    dctl.b.rmtwkupsig = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0UL );

    /* Flush the Tx FIFO */
    USB_OTG_FlushTxFifo(pdev, 0UL);

    for (i = 0UL; i < pdev->cfg.dev_endpoints ; i++)
    {
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFFUL);
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFFUL);
    }
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFFUL );

    daintmsk.ep.in = 1U;
    daintmsk.ep.out = 1U;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, daintmsk.d32 );

    doepmsk.b.setup = 1U;
    doepmsk.b.xfercompl = 1U;
    doepmsk.b.epdisabled = 1U;
    doepmsk.b.stsphsercvd = 1U;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, doepmsk.d32 );
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOUTEP1MSK, doepmsk.d32 );
#endif
    diepmsk.b.xfercompl = 1U;
    diepmsk.b.timeout = 1U;
    diepmsk.b.epdisabled = 1U;

    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, diepmsk.d32 );
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DINEP1MSK, diepmsk.d32 );
#endif
    /* Reset Device Address */
    dcfg.d32 = USB_OTG_READ_REG32( &pdev->regs.DREGS->DCFG);
    dcfg.b.devaddr = 0U;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCFG, dcfg.d32);

    /* setup EP0 to receive SETUP packets */
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("reset\n");
#endif
    USB_OTG_EP0_OutStart(pdev);

    /* Clear interrupt */
    gintsts.d32 = 0UL;
    gintsts.b.usbreset = 1UL;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    /*Reset internal state machine */
    USBD_DCD_INT_fops->Reset(pdev);
    return 1UL;
}

/**
 * @brief  DCD_HandleEnumDone_ISR
 *         Read the device status register and set the device speed
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_HandleEnumDone_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_GUSBCFG_TypeDef  gusbcfg;

    USB_OTG_EP0Activate(pdev);

    /* Set USB turn-around time based on device speed and PHY interface. */
    gusbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);

    /* Full or High speed */
    if ( USB_OTG_GetDeviceSpeed(pdev) == USB_SPEED_HIGH)
    {
        pdev->cfg.speed            = USB_OTG_SPEED_HIGH;
        pdev->cfg.mps              = USB_OTG_HS_MAX_PACKET_SIZE ;
        gusbcfg.b.usbtrdtim = 9U;
    }
    else
    {
        pdev->cfg.speed            = USB_OTG_SPEED_FULL;
        pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;
        gusbcfg.b.usbtrdtim = 9U;
    }

    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, gusbcfg.d32);

    /* Clear interrupt */
    gintsts.d32 = 0UL;
    gintsts.b.enumdone = 1U;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, gintsts.d32 );
    return 1UL;
}

/**
 * @brief  DCD_IsoINIncomplete_ISR
 *         handle the ISO IN incomplete interrupt
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_IsoINIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef gintsts;

    gintsts.d32 = 0UL;

    USBD_DCD_INT_fops->IsoINIncomplete (pdev);

    /* Clear interrupt */
    gintsts.b.incomplisoin = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    return 1UL;
}

/**
 * @brief  DCD_IsoOUTIncomplete_ISR
 *         handle the ISO OUT incomplete interrupt
 * @param  [in] pdev     device instance
 * @retval Status
 */
static uint32_t DCD_IsoOUTIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef gintsts;

    gintsts.d32 = 0UL;

    USBD_DCD_INT_fops->IsoOUTIncomplete (pdev);

    /* Clear interrupt */
    gintsts.b.incomplisoout = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1UL;
}

/**
 * @brief  DCD_ReadDevInEP Reads ep flags
 * @param  [in] pdev     Device instance
 * @param  [in] epnum    Endpoint number
 * @retval Status
 */
static uint32_t DCD_ReadDevInEP (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    uint32_t v;
    uint32_t msk;
    uint32_t emp;
    msk = USB_OTG_READ_REG32(&pdev->regs.DREGS->DIEPMSK);
    emp = USB_OTG_READ_REG32(&pdev->regs.DREGS->DIEPEMPMSK);
    msk |= ((emp >> epnum) & 0x1UL) << 7U;
    v = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT) & msk;
    return v;
}

/**
 * @}
 */

#endif /* DDL_USBFS_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
