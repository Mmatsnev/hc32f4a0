/**
 *******************************************************************************
 * @file  hc32f4a0_usb_core.c
 * @brief USB Core Layer.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin         First version
   2020-08-21       Wangmin         Modify Comment for parameter pdev
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
#include "hc32f4a0_usb_core.h"
#include "usb_bsp.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_USB_CORE USB CORE
 * @brief USB core level function.
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
 * @defgroup USB_Core_Local_Functions USB Core Local Functions
 * @{
 */

static void USB_OTG_EnableCommonInt(USB_OTG_CORE_HANDLE *pdev);
static USB_OTG_STS USB_OTG_CoreReset(USB_OTG_CORE_HANDLE *pdev);
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
 * @addtogroup USB_Core_Local_Functions USB Core Local Functions
 * @{
 */

/**
 * @brief  USB_OTG_EnableCommonInt
 *         Initializes the commmon interrupts, used in both device and modes
 * @param  [in] pdev     Instance for USB core
 * @retval None
 */
static void USB_OTG_EnableCommonInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTMSK_TypeDef  int_mask;

    int_mask.d32 = 0UL;
    /* Clear any pending USB_OTG Interrupts */
#ifndef USE_OTG_MODE
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GOTGINT, 0xFFFFFFFFUL);
#endif
    /* Clear any pending interrupts */
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, 0xBFFFFFFFUL);
    /* Enable the interrupts in the INTMSK */
    int_mask.b.wkupintr = 1U;
    int_mask.b.usbsuspend = 1U;

#ifdef USE_OTG_MODE
    int_mask.b.vbusvint = 1U;
    int_mask.b.conidstschng = 1U;
#endif
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTMSK, int_mask.d32);
}

/**
 * @brief  USB_OTG_CoreReset : Soft reset of the core
 * @param  [in] pdev         Instance for USB core
 * @retval USB_OTG_STS status
 */
static USB_OTG_STS USB_OTG_CoreReset(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    __IO USB_OTG_GRSTCTL_TypeDef  greset;
    uint32_t count = 0UL;
    *(__IO uint32_t*)&greset.b = 0UL; /* C-STAT */
    /* Wait for AHB master IDLE state. */
    do
    {
        USB_OTG_BSP_uDelay(1UL);
        greset.b = *(__IO stc_bUSB_OTG_GRSTCTL_t*)&USB_OTG_READ_REG32(&pdev->regs.GREGS->GRSTCTL); /* C-STAT */
        if (++count > 100000UL)
        {
            status = USB_OTG_FAIL;
        }
    }
    while (greset.b.ahbidle == 0U);

    if(USB_OTG_OK == status)
    {
        /* Core Soft Reset */
        count = 0UL;
        greset.b.csftrst = 1U;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRSTCTL, *(__IO uint32_t*)&greset.b ); /* C-STAT */
        do
        {
            greset.b = *(__IO stc_bUSB_OTG_GRSTCTL_t*)&USB_OTG_READ_REG32(&pdev->regs.GREGS->GRSTCTL); /* C-STAT */
            if (++count > 100000UL)
            {
              break;
            }
            USB_OTG_BSP_uDelay(1UL);
        }
        while (greset.b.csftrst == 1U);
        /* Wait for 3 PHY Clocks*/
        USB_OTG_BSP_uDelay(3UL);
    }
    return status;
}

/**
 * @}
 */

/**
 * @defgroup USB_Core_Global_Functions USB Core Global Functions
 * @{
 */

/**
 * @brief  USB_OTG_WritePacket Writes a packet into the Tx FIFO associated
 *         with the EP
 * @param  [in] pdev         Instance for USB core
 * @param  [in] src          Source pointer
 * @param  [in] ch_ep_num    End point number
 * @param  [in] len          No. of bytes
 * @retval USB_OTG_STS  Status
 */
USB_OTG_STS USB_OTG_WritePacket(USB_OTG_CORE_HANDLE *pdev,
                                uint8_t             *src,
                                uint8_t             ch_ep_num,
                                uint16_t            len)
{
    uint32_t u32pAdr;
    uint32_t count32b;
    uint32_t i;
    __IO uint32_t *fifo;

    if (pdev->cfg.dma_enable == 0U)
    {
        count32b =  (len + 3UL) / 4UL;
        fifo = pdev->regs.DFIFO[ch_ep_num];
        for (i = 0UL; i < count32b; i++)
        {
            //USB_OTG_WRITE_REG32( fifo, *((__packed uint32_t *)src) );
            USB_OTG_WRITE_REG32( fifo, *((uint32_t *)src) );
            //src+=4UL;   /* C-STAT */
            u32pAdr = (uint32_t)src;
            src = (uint8_t*)(u32pAdr+4UL);
        }
    }
    return USB_OTG_OK;
}


/**
 * @brief  USB_OTG_ReadPacket  Reads a packet from the Rx FIFO
 * @param  [in] pdev     Instance for USB core
 * @param  [in] dest     Destination Pointer
 * @param  [in] len      No. of bytes
 * @retval None
 */
void *USB_OTG_ReadPacket(USB_OTG_CORE_HANDLE *pdev,
                         uint8_t *dest,
                         uint16_t len)
{
    uint32_t i;
    const uint32_t count32b = (len + 3UL) / 4UL;
    uint32_t u32pAdr;

    __IO uint32_t *fifo = pdev->regs.DFIFO[0UL];

    for ( i = 0UL; i < count32b; i++)
    {
        //*(__packed uint32_t *)dest = USB_OTG_READ_REG32(fifo);
        *(uint32_t *)dest = USB_OTG_READ_REG32(fifo);
        //dest += 4UL;   /* MISRAC 2004*/
        u32pAdr = (uint32_t)dest;
        dest = (uint8_t*)(u32pAdr + 4UL);
    }
    return ((void *)dest);
}


/**
 * @brief  USB_OTG_GetDeviceSpeed
 *         Get the device speed from the device status register
 * @param  [in] pdev             Instance for USB core
 * @retval status
 */
enum USB_OTG_SPEED USB_OTG_GetDeviceSpeed (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DSTS_TypeDef  dsts;
    enum USB_OTG_SPEED speed = USB_SPEED_UNKNOWN;

    dsts.b = *(__IO stc_bUSB_OTG_DSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS); /* C-STAT */

    switch (dsts.b.enumspd)
    {
        case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
            speed = USB_SPEED_HIGH;
            break;
        case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
        case DSTS_ENUMSPD_FS_PHY_48MHZ:
            speed = USB_SPEED_FULL;
            break;
        case DSTS_ENUMSPD_LS_PHY_6MHZ:
            speed = USB_SPEED_LOW;
            break;
        default:
            break;
    }

    return speed;
}

/**
 * @brief  USB_OTG_GetMode : Get current mode
 * @param  [in] pdev     Instance for USB core
 * @retval current mode
 */
uint32_t USB_OTG_GetMode(USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS ) & 0x1UL);
}

/**
 * @brief  USB_OTG_IsDeviceMode : Check if it is device mode
 * @param  [in] pdev     Instance for USB core
 * @retval num_in_ep
 */
uint8_t USB_OTG_IsDeviceMode(USB_OTG_CORE_HANDLE *pdev)
{
    return (uint8_t)(USB_OTG_GetMode(pdev) != (uint32_t)HOST_MODE);
}


/**
 * @brief  USB_OTG_ReadCoreItr : returns the Core Interrupt register
 * @param  [in] pdev     Instance for USB core
 * @retval Status
 */
uint32_t USB_OTG_ReadCoreItr(USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t v;
    v = USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS);
    v &= USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTMSK);
    return v;
}

/**
 * @brief  USB_OTG_SelectCore
 *         Initialize core registers address.
 * @param  [in] pdev         Instance for USB core
 * @param  [in] coreID       USB OTG Core ID
 * @retval USB_OTG_STS  status
 */
USB_OTG_STS USB_OTG_SelectCore(USB_OTG_CORE_HANDLE *pdev,
                               USB_OTG_CORE_ID_TypeDef coreID)
{
    uint32_t i;
    uint32_t baseAddress;
    USB_OTG_STS status = USB_OTG_OK;
    pdev->cfg.dma_enable       = 0U;

    /* at startup the core is in FS mode */
    pdev->cfg.speed            = USB_OTG_SPEED_FULL;
    pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;

    /* initialize device cfg following its address */
    if (coreID == USB_OTG_FS_CORE_ID)
    {
        baseAddress                = USB_OTG_FS_BASE_ADDR;
        pdev->cfg.coreID           = (uint8_t)USB_OTG_FS_CORE_ID;
        pdev->cfg.host_channels    = 16U;
        pdev->cfg.dev_endpoints    = 16U;
        pdev->cfg.TotalFifoSize    = 640U; /* in 32-bits */
        pdev->cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;

#ifdef USB_OTG_FS_SOF_OUTPUT_ENABLED
        pdev->cfg.Sof_output       = 1U;
#endif
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        pdev->cfg.dma_enable       = 1U;
#endif

#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
        pdev->cfg.low_power        = 1U;
#endif
    }
    //else if (coreID == USB_OTG_HS_CORE_ID)
    else
    {
        baseAddress                = USB_OTG_HS_BASE_ADDR;
        pdev->cfg.coreID           = (uint8_t)USB_OTG_HS_CORE_ID;
        pdev->cfg.host_channels    = 16U;
        pdev->cfg.dev_endpoints    = 16U;
        pdev->cfg.TotalFifoSize    = 2048U;/* in 32-bits */

#ifdef USB_OTG_ULPI_PHY_ENABLED
        pdev->cfg.phy_itface       = USB_OTG_ULPI_PHY;
#else
#ifdef USB_OTG_EMBEDDED_PHY_ENABLED
        pdev->cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;
#endif
#endif

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        pdev->cfg.dma_enable       = 1U;
#endif

#ifdef USB_OTG_HS_SOF_OUTPUT_ENABLED
        pdev->cfg.Sof_output       = 1U;
#endif

#ifdef USB_OTG_HS_LOW_PWR_MGMT_SUPPORT
        pdev->cfg.low_power        = 1U;
#endif
  }

    pdev->regs.GREGS = (USB_OTG_GREGS *)(baseAddress + \
        USB_OTG_CORE_GLOBAL_REGS_OFFSET);
    pdev->regs.DREGS =  (USB_OTG_DREGS  *)  (baseAddress + \
        USB_OTG_DEV_GLOBAL_REG_OFFSET);

    for (i = 0UL; i < pdev->cfg.dev_endpoints; i++)
    {
        pdev->regs.INEP_REGS[i]  = (USB_OTG_INEPREGS *)  \
            (baseAddress + USB_OTG_DEV_IN_EP_REG_OFFSET + \
            (i * USB_OTG_EP_REG_OFFSET));
        pdev->regs.OUTEP_REGS[i] = (USB_OTG_OUTEPREGS *) \
            (baseAddress + USB_OTG_DEV_OUT_EP_REG_OFFSET + \
            (i * USB_OTG_EP_REG_OFFSET));
    }
    pdev->regs.HREGS = (USB_OTG_HREGS *)(baseAddress + USB_OTG_HOST_GLOBAL_REG_OFFSET);
    pdev->regs.HPRT0 = (uint32_t *)(baseAddress + USB_OTG_HOST_PORT_REGS_OFFSET);

    for (i = 0UL; i < pdev->cfg.host_channels; i++)
    {
        pdev->regs.HC_REGS[i] = (USB_OTG_HC_REGS *)(baseAddress + \
            USB_OTG_HOST_CHAN_REGS_OFFSET + (i * USB_OTG_CHAN_REGS_OFFSET));
    }
    for (i = 0UL; i < pdev->cfg.host_channels; i++)
    {
        pdev->regs.DFIFO[i] = (uint32_t *)(baseAddress + USB_OTG_DATA_FIFO_OFFSET +\
            (i * USB_OTG_DATA_FIFO_SIZE));
    }
    pdev->regs.PCGCCTL = (uint32_t *)(baseAddress + USB_OTG_PCGCCTL_OFFSET);

    return status;
}
/* *********************        PCD Core Layer       **************************/

/**
 * @brief  USB_OTG_InitDevSpeed :Initializes the DevSpd field of DCFG register
 *         depending the PHY type and the enumeration speed of the device.
 * @param  [in] pdev     Instance for USB core
 * @param  [in] speed    Speed for USB
 * @retval None
 */
void USB_OTG_InitDevSpeed(USB_OTG_CORE_HANDLE *pdev , uint8_t speed)
{
    USB_OTG_DCFG_TypeDef   dcfg;

    dcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCFG);
    dcfg.b.devspd = speed;
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCFG, dcfg.d32);
}

/**
 * @brief  USB_OTG_CoreInit
 *         Initializes the USB_OTG controller registers and prepares the core
 *         device mode or host mode operation.
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_CoreInit(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_GUSBCFG_TypeDef  usbcfg;
    USB_OTG_GAHBCFG_TypeDef  ahbcfg;

    usbcfg.d32 = 0UL;
    ahbcfg.d32 = 0UL;

    USB_OTG_CoreReset(pdev);

    if (pdev->cfg.phy_itface == USB_OTG_ULPI_PHY)
    {
        /* Init The ULPI Interface */
        usbcfg.d32 = 0UL;
        usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);

        usbcfg.b.physel            = 0U; /* HS Interface */
#ifdef USB_OTG_INTERNAL_VBUS_ENABLED
        usbcfg.b.ulpi_ext_vbus_drv = 0U; /* Use internal VBUS */
#else
#ifdef USB_OTG_EXTERNAL_VBUS_ENABLED
        usbcfg.b.ulpi_ext_vbus_drv = 1U; /* Use external VBUS */
#endif
#endif
        usbcfg.b.ulpi_utmi_sel = 1U;
        USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);

        /* Reset after a PHY select  */
        USB_OTG_CoreReset(pdev);

        if(pdev->cfg.dma_enable == 1U)
        {

          ahbcfg.b.hburstlen = 5U; /* 64 x 32-bits*/
          ahbcfg.b.dmaenable = 1U;
          USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32);

        }
    }
    else /* FS interface (embedded Phy) */
    {
        usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
        usbcfg.b.physel  = 1U; /* FS Interface */
        USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
        /* Reset after a PHY select and set Host mode */
        USB_OTG_CoreReset(pdev);

        USB_OTG_BSP_mDelay(20UL);
    }

    /* case the HS core is working in FS mode */
    if(pdev->cfg.dma_enable == 1U)
    {
        ahbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GAHBCFG);
        ahbcfg.b.hburstlen = 5U; /* 64 x 32-bits*/
        ahbcfg.b.dmaenable = 1U;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32);
    }
    /* initialize OTG features */
#ifdef  USE_OTG_MODE
    usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
    usbcfg.b.hnpcap = 1U;
    usbcfg.b.srpcap = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
    USB_OTG_EnableCommonInt(pdev);
#endif
    return status;
}

/**
 * @brief  USB_OTG_EnableGlobalInt
 *         Enables the controller's Global Int in the AHB Config reg
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_EnableGlobalInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_GAHBCFG_TypeDef  ahbcfg;

    ahbcfg.d32 = 0UL;
    ahbcfg.b.glblintrmsk = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GAHBCFG, 0UL, ahbcfg.d32);
    return status;
}


/**
 * @brief  USB_OTG_DisableGlobalInt
 *         Enables the controller's Global Int in the AHB Config reg
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_GAHBCFG_TypeDef  ahbcfg;
    ahbcfg.d32 = 0UL;
    ahbcfg.b.glblintrmsk = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GAHBCFG, ahbcfg.d32, 0UL);
    return status;
}

USB_OTG_STS USB_OTG_stop_xfer (USB_OTG_CORE_HANDLE *pdev , uint32_t num )
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef  depctl;
    USB_OTG_DIEPINTn_TypeDef diepint;

    if (USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[num]->DIEPTSIZ) == 0UL)
    {
        status = USB_OTG_OK;
    }
    else
    {
        depctl.d32 = 0UL;

        depctl.b.epdis = 1U;
        depctl.b.snak = 1U;
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[num]->DIEPCTL, depctl.d32 );

        do
        {
            diepint.b = *(__IO stc_bUSB_OTG_DIEPINTn_t*)&USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[num]->DIEPINT); /* C-STAT */
        } while (0UL == diepint.b.epdisabled);
        *(uint32_t*)&diepint.b = 0UL;   /* C-STAT */
        diepint.b.epdisabled = 1U;
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[num]->DIEPINT, *(uint32_t*)&diepint.b ); /* C-STAT */

        depctl.b.epdis = 0U;
        depctl.b.epena = 0U;
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[num]->DIEPCTL, depctl.d32 );
    }
    return status;
}

/**
 * @brief  USB_OTG_FlushTxFifo : Flush a Tx FIFO
 * @param  [in] pdev     Instance for USB core
 * @param  [in] num      FO num
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_FlushTxFifo (USB_OTG_CORE_HANDLE *pdev , uint32_t num )
{
    USB_OTG_STS status = USB_OTG_OK;
    __IO USB_OTG_GRSTCTL_TypeDef  greset;

    uint32_t count = 0UL;
    greset.d32 = 0UL;
    greset.b.txfflsh = 1U;
    greset.b.txfnum  = num;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
    do
    {
        greset.b = *(__IO stc_bUSB_OTG_GRSTCTL_t*)&(USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL)); /* C-STAT */
        if (++count > 200000UL)
        {
            ;//break;
        }
        USB_OTG_BSP_uDelay(1UL);
    }
    while (greset.b.txfflsh == 1U);
    /* Wait for 3 PHY Clocks*/
    USB_OTG_BSP_uDelay(3UL);
    return status;
}

/**
 * @brief  USB_OTG_FlushRxFifo : Flush a Rx FIFO
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_FlushRxFifo( USB_OTG_CORE_HANDLE *pdev )
{
    USB_OTG_STS status = USB_OTG_OK;
    __IO USB_OTG_GRSTCTL_TypeDef  greset;
    uint32_t count = 0UL;

    greset.d32 = 0UL;
    greset.b.rxfflsh = 1U;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
    do
    {
        greset.b = *(__IO stc_bUSB_OTG_GRSTCTL_t*)&USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL); /* C-STAT */
        if (++count > 200000UL)
        {
            ;//break;
        }
        USB_OTG_BSP_uDelay(1UL);
    }
    while (greset.b.rxfflsh == 1U);
    /* Wait for 3 PHY Clocks*/
    USB_OTG_BSP_uDelay(3UL);
    return status;
}


/**
 * @brief  USB_OTG_SetCurrentMode : Set ID line
 * @param  [in] pdev     Instance for USB core
 * @param  [in] mode     (Host/device)
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_SetCurrentMode(USB_OTG_CORE_HANDLE *pdev , uint8_t mode)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_GUSBCFG_TypeDef  usbcfg;

    usbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);

    usbcfg.b.force_host_mode = 0U;
    usbcfg.b.force_dev_mode = 0U;

    if ( mode == HOST_MODE)
    {
        usbcfg.b.force_host_mode = 1U;
    }
    else
    {
        usbcfg.b.force_dev_mode = 1U;
    }

    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, usbcfg.d32);
    USB_OTG_BSP_mDelay(50UL);
    return status;
}

/**
 * @brief  USB_OTG_CoreInitDev : Initializes the USB_OTG controller registers
 *         for device mode
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_CoreInitDev (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS             status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef  depctl;
    uint32_t i;
    USB_OTG_DCFG_TypeDef    dcfg;
    USB_OTG_FSIZ_TypeDef    nptxfifosize;
//#ifdef USB_OTG_HS_CORE
    USB_OTG_FSIZ_TypeDef    txfifosize;
//#endif
//    USB_OTG_GRSTCTL_TypeDef greset = {.d32 = 0 };
    USB_OTG_DCTL_TypeDef        dctl;

    *(uint32_t*)&depctl.b = 0UL;  /* C-STAT */
    dcfg.d32 = 0UL;
    *(uint32_t*)&nptxfifosize.b = 0UL;  /* C-STAT */
//#ifdef USB_OTG_HS_CORE
    *(uint32_t*)&txfifosize.b = 0UL;  /* C-STAT */
//#endif
    /* Device configuration register */
    dcfg.d32 = USB_OTG_READ_REG32( &pdev->regs.DREGS->DCFG);
    dcfg.b.perfrint = DCFG_FRAME_INTERVAL_80;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCFG, dcfg.d32 );

#ifdef USB_OTG_FS_CORE
    if(pdev->cfg.coreID == (uint8_t)USB_OTG_FS_CORE_ID)
    {
        /* Set Full speed phy */
        USB_OTG_InitDevSpeed (pdev , USB_OTG_SPEED_PARAM_FULL);

        /* set Rx FIFO size */
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRXFSIZ, (uint32_t)RX_FIFO_FS_SIZE);

        /* EP0 TX*/
        nptxfifosize.b.depth     = (uint32_t)TX0_FIFO_FS_SIZE;
        nptxfifosize.b.startaddr = (uint32_t)RX_FIFO_FS_SIZE;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GNPTXFSIZ, *(uint32_t*)&nptxfifosize.b ); /* C-STAT */

        #if 1
        /* EP1 TX*/
        txfifosize.b.startaddr = nptxfifosize.b.startaddr + nptxfifosize.b.depth;
        txfifosize.b.depth = (uint32_t)TX1_FIFO_FS_SIZE;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->DPTXFSIZ[0], *(uint32_t*)&txfifosize.b );  /* C-STAT */

        /* EP2 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = (uint32_t)TX2_FIFO_FS_SIZE;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->DPTXFSIZ[1], *(uint32_t*)&txfifosize.b ); /* C-STAT */

        /* EP3 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = (uint32_t)TX3_FIFO_FS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[2], *(uint32_t*)&txfifosize.b );  /* C-STAT */

        /* EP4 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = (uint32_t)TX4_FIFO_FS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[3], *(uint32_t*)&txfifosize.b );  /* C-STAT */

        /* EP5 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = (uint32_t)TX5_FIFO_FS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[4], *(uint32_t*)&txfifosize.b );  /* C-STAT */
        #endif
    }
#endif
#ifdef USB_OTG_HS_CORE
    if(pdev->cfg.coreID == USB_OTG_HS_CORE_ID  )
    {
        /* Set High speed phy */
        if(pdev->cfg.phy_itface  == USB_OTG_ULPI_PHY)
        {
          USB_OTG_InitDevSpeed (pdev , USB_OTG_SPEED_PARAM_HIGH);
        }
        else /* set High speed phy in Full speed mode */
        {
          USB_OTG_InitDevSpeed (pdev , USB_OTG_SPEED_PARAM_HIGH_IN_FULL);
        }

        /* set Rx FIFO size */
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRXFSIZ, RX_FIFO_HS_SIZE);

        /* EP0 TX*/
        nptxfifosize.b.depth     = TX0_FIFO_HS_SIZE;
        nptxfifosize.b.startaddr = RX_FIFO_HS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GNPTXFSIZ, nptxfifosize.d32 );

        /* EP1 TX*/
        txfifosize.b.startaddr = nptxfifosize.b.startaddr + nptxfifosize.b.depth;
        txfifosize.b.depth = TX1_FIFO_HS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[0U], txfifosize.d32 );

        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[1U], 0U );
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[2U], 0U );
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[3U], 0U );
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[4U], 0U );
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[5U], 0U );
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DPTXFSIZ[6U], 0U );

        #if 0
        /* EP2 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = TX2_FIFO_HS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[1], txfifosize.d32 );

        /* EP3 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = TX3_FIFO_HS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[2], txfifosize.d32 );

        /* EP4 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = TX4_FIFO_HS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[3], txfifosize.d32 );

        /* EP5 TX*/
        txfifosize.b.startaddr += txfifosize.b.depth;
        txfifosize.b.depth = TX5_FIFO_HS_SIZE;
        USB_OTG_WRITE_REG32( &pdev->regs.GREGS->DIEPTXF[4], txfifosize.d32 );
        #endif
    }
#endif
    /* Clear all pending Device Interrupts */
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, 0UL );
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, 0UL );
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFFUL );
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, 0UL );

    for (i = 0UL; i < pdev->cfg.dev_endpoints; i++)
    {
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[i]->DIEPCTL);  /* C-STAT */
        if (depctl.b.epena != 0UL)
        {
            *(uint32_t*)&depctl.b = 0UL;  /* C-STAT */
            depctl.b.epdis = 1U;
            depctl.b.snak = 1U;
        }
        else
        {
            *(uint32_t*)&depctl.b = 0UL;  /* C-STAT */
        }
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPCTL, *(uint32_t*)&depctl.b);   /* C-STAT */
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPTSIZ, 0UL);
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFFUL);
    }
    for (i = 0UL; i <  pdev->cfg.dev_endpoints; i++)
    {
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[i]->DOEPCTL); /* C-STAT */
        if (depctl.b.epena != 0UL)
        {
            *(uint32_t*)&depctl.b = 0UL;  /* C-STAT */
            depctl.b.epdis = 1U;
            depctl.b.snak = 1U;
        }
        else
        {
            *(uint32_t*)&depctl.b = 0UL;  /* C-STAT */
        }
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPCTL, *(uint32_t*)&depctl.b);  /* C-STAT */
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPTSIZ, 0UL);
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFFUL);
    }
//  msk.d32 = 0U;
//  msk.b.txfifoundrn = 1U;
//  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPMSK, msk.d32, msk.d32);
//
//  if (pdev->cfg.dma_enable == 1U)
//  {
//    dthrctl.d32 = 0U;
//    dthrctl.b.non_iso_thr_en = 1U;
//    dthrctl.b.iso_thr_en = 1U;
//    dthrctl.b.tx_thr_len = 64U;
//    dthrctl.b.rx_thr_en = 1U;
//    dthrctl.b.rx_thr_len = 64U;
//    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DTHRCTL, dthrctl.d32);
//  }
    /* Clear the DCTL.SftDiscon bit */
    dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
    dctl.b.sftdiscon = 0U;
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, dctl.d32);

    USB_OTG_EnableDevInt(pdev);
    return status;
}

/**
 * @brief  USB_OTG_EnableDevInt : Enables the Device mode interrupts
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_EnableDevInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_GINTMSK_TypeDef  intmsk;

    intmsk.d32 = 0UL;

    /* Disable all interrupts. */
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTMSK, 0UL);
    /* Clear any pending interrupts */
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, 0xBFFFFFFFUL);
    /* Enable the common interrupts */
    USB_OTG_EnableCommonInt(pdev);

    if (pdev->cfg.dma_enable == 0U)
    {
        intmsk.b.rxstsqlvl = 1U;
    }

    /* Enable interrupts matching to the Device mode ONLY */
    intmsk.b.usbsuspend = 1UL;
    intmsk.b.usbreset   = 1UL;
    intmsk.b.enumdone   = 1UL;
    intmsk.b.inepintr   = 1UL;
    intmsk.b.outepintr  = 1UL;
    intmsk.b.sofintr    = 1UL;

    intmsk.b.incomplisoin    = 1U;
    intmsk.b.incomplisoout    = 1U;
#ifdef VBUS_SENSING_ENABLED
    intmsk.b.vbusvint    = 1U;
#endif
    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, intmsk.d32, intmsk.d32);
    return status;
}

/**
 * @brief  returns the EP Status
 * @param  [in] pdev     Instance for USB core
 * @param  [in] ep       Endpoint structure
 * @retval EP status
 */
uint32_t USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,USB_OTG_EP *ep)
{
    USB_OTG_DEPCTL_TypeDef  depctl;
    __IO uint32_t *depctl_addr;
    uint32_t Status;

    *(uint32_t*)&depctl.b = 0UL;  /* C-STAT */
    if (ep->is_in == 1U)
    {
        depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(depctl_addr); /* C-STAT */

        if (depctl.b.stall == 1U)
        {
            Status = USB_OTG_EP_TX_STALL;
        }
        else if (depctl.b.naksts == 1U)
        {
            Status = USB_OTG_EP_TX_NAK;
        }
        else
        {
            Status = USB_OTG_EP_TX_VALID;
        }
    }
    else
    {
        depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(depctl_addr);   /* C-STAT */
        if (depctl.b.stall == 1U)
        {
            Status = USB_OTG_EP_RX_STALL;
        }
        else if (depctl.b.naksts == 1U)
        {
            Status = USB_OTG_EP_RX_NAK;
        }
        else
        {
            Status = USB_OTG_EP_RX_VALID;
        }
    }
    /* Return the current status */
    return Status;
}

/**
 * @brief  enables EP0 OUT to receive SETUP packets and configures EP0
 *   for transmitting packets
 * @param  [in] pdev         USB_OTG_EP0Activate
 * @retval USB_OTG_STS  Status
 */
USB_OTG_STS  USB_OTG_EP0Activate(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS             status = USB_OTG_OK;
    USB_OTG_DSTS_TypeDef    dsts;
    USB_OTG_DEPCTL_TypeDef  diepctl;
    USB_OTG_DCTL_TypeDef    dctl;

    dctl.d32 = 0UL;
    /* Read the Device Status and Endpoint 0 Control registers */
    dsts.b = *(__IO stc_bUSB_OTG_DSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);  /* C-STAT */
    diepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL);
    /* Set the MPS of the IN EP based on the enumeration speed */
    switch (dsts.b.enumspd)
    {
        case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
        case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
        case DSTS_ENUMSPD_FS_PHY_48MHZ:
            diepctl.b.mps = DEP0CTL_MPS_64;
            break;
        case DSTS_ENUMSPD_LS_PHY_6MHZ:
            diepctl.b.mps = DEP0CTL_MPS_8;
            break;
        default:
            break;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[0U]->DIEPCTL, diepctl.d32);
    dctl.b.cgnpinnak = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, dctl.d32);
    return status;
}


/**
 * @brief  USB_OTG_EPActivate : Activates an EP
 * @param  [in] pdev             Instance for USB core
 * @param  [in] ep               Endpoint
 * @retval USB_OTG_STS      Status
 */
USB_OTG_STS USB_OTG_EPActivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef  depctl;
    USB_OTG_DAINT_TypeDef  daintmsk;
    __IO uint32_t *addr;

    *(uint32_t*)&depctl.b = 0UL;   /* C-STAT */
    daintmsk.d32 = 0UL;
    /* Read DEPCTLn register */
    if (ep->is_in == 1U)
    {
        addr = &pdev->regs.INEP_REGS[ep->num]->DIEPCTL;
        daintmsk.ep.in = (uint32_t)1UL << ep->num;
    }
    else
    {
        addr = &pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL;
        daintmsk.ep.out = (uint32_t)1UL << ep->num;
    }
    /* If the EP is already active don't change the EP Control
    * register. */
    depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(addr);   /* C-STAT */
    if (depctl.b.usbactep == 0UL)
    {
        depctl.b.mps    = ep->maxpacket;
        depctl.b.eptype = ep->type;
        depctl.b.txfnum = ep->tx_fifo_num;
        depctl.b.setd0pid = 1U;
        depctl.b.usbactep = 1U;
        USB_OTG_WRITE_REG32(addr, *(uint32_t*)&depctl.d32);   /* C-STAT */
    }
    /* Enable the Interrupt for this EP */
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    if((ep->num == 1)&&(pdev->cfg.coreID == USB_OTG_HS_CORE_ID))
    {
        USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DEACHMSK, 0UL, daintmsk.d32);
    }
    else
#endif
    {
        USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DAINTMSK, 0UL, daintmsk.d32);
    }
    return status;
}


/**
 * @brief  USB_OTG_EPDeactivate : Deactivates an EP
 * @param  [in] pdev             Instance for USB core
 * @param  [in] ep               Endpoint
 * @retval USB_OTG_STS      Status
 */
USB_OTG_STS USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef  depctl;
    USB_OTG_DAINT_TypeDef  daintmsk;
    __IO uint32_t *addr;

    depctl.d32 = 0UL;
    daintmsk.d32 = 0UL;
    /* Read DEPCTLn register */
    if (ep->is_in == 1U)
    {
        addr = &pdev->regs.INEP_REGS[ep->num]->DIEPCTL;
        daintmsk.ep.in = (uint32_t)1UL << ep->num;
    }
    else
    {
        addr = &pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL;
        daintmsk.ep.out = (uint32_t)1UL << ep->num;
    }
    depctl.b.usbactep = 0UL;
    USB_OTG_WRITE_REG32(addr, depctl.d32);
    /* Disable the Interrupt for this EP */
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    if((ep->num == 1U)&&(pdev->cfg.coreID == USB_OTG_HS_CORE_ID))
    {
        USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DEACHMSK, daintmsk.d32, 0UL);
    }
    else
#endif
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DAINTMSK, daintmsk.d32, 0UL);
    return status;
}


/**
 * @brief  USB_OTG_EPStartXfer : Handle the setup for data xfer for an EP and
 *         starts the xfer
 * @param  [in] pdev             Instance for USB core
 * @param  [in] ep               Endpoint
 * @retval USB_OTG_STS      Status
 */
USB_OTG_STS USB_OTG_EPStartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef     depctl;
    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
    USB_OTG_DSTS_TypeDef       dsts;
    uint32_t fifoemptymsk;

    depctl.d32 = 0UL;
    *(uint32_t*)&deptsiz.b = 0UL;  /* C-STAT */
    /* IN endpoint */
    if (ep->is_in == 1U)
    {
        depctl.d32  = USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[ep->num]->DIEPCTL));
        deptsiz.b = *(__IO stc_bUSB_OTG_DEPXFRSIZ_t*)&USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ)); /* C-STAT */
        /* Zero Length Packet? */
        if (ep->xfer_len == 0UL)
        {
            deptsiz.b.xfersize = 0U;
            deptsiz.b.pktcnt = 1U;
        }
        else
        {
            /* Program the transfer size and packet count
            * as follows: xfersize = N * maxpacket +
            * short_packet pktcnt = N + (short_packet
            * exist ? 1 : 0)
            */
            deptsiz.b.xfersize = ep->xfer_len;
            deptsiz.b.pktcnt = (ep->xfer_len - 1U + ep->maxpacket) / ep->maxpacket;

            if (ep->type == EP_TYPE_ISOC)
            {
                deptsiz.b.mc = 1U;
            }
        }
        USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ, *(uint32_t*)&deptsiz.b);  /* C-STAT */

        if (pdev->cfg.dma_enable == 1U)
        {
            USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPDMA, ep->dma_addr);
        }
        else
        {
            if (ep->type != EP_TYPE_ISOC)
            {
                /* Enable the Tx FIFO Empty Interrupt for this EP */
                if (ep->xfer_len > 0U)
                {
                    fifoemptymsk = (uint32_t)1UL << ep->num;
                    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0UL, fifoemptymsk);
                }
            }
        }

        if (ep->type == EP_TYPE_ISOC)
        {
            dsts.b = *(__IO stc_bUSB_OTG_DSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);  /* C-STAT */

            if (((dsts.b.soffn)&0x1U) == 0U)
            {
                depctl.b.setd1pid = 1U;
            }
            else
            {
                depctl.b.setd0pid = 1U;
            }
        }

        /* EP enable, IN data in FIFO */
        depctl.b.cnak = 1U;
        depctl.b.epena = 1U;
        USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPCTL, depctl.d32);

        if (ep->type == EP_TYPE_ISOC)
        {
            USB_OTG_WritePacket(pdev, ep->xfer_buff, ep->num, (uint16_t)ep->xfer_len);
        }
    }
    else
    {
        /* OUT endpoint */
        depctl.d32  = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL));
        deptsiz.b = *(__IO stc_bUSB_OTG_DEPXFRSIZ_t*)&USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ));  /* C-STAT */
        /* Program the transfer size and packet count as follows:
        * pktcnt = N
        * xfersize = N * maxpacket
        */
        if (ep->xfer_len == 0U)
        {
            deptsiz.b.xfersize = ep->maxpacket;
            deptsiz.b.pktcnt = 1U;
        }
        else
        {
            deptsiz.b.pktcnt = (ep->xfer_len + (ep->maxpacket - 1U)) / ep->maxpacket;
            deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
            ep->xfer_len = deptsiz.b.xfersize;
        }
        USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ, *(uint32_t*)&deptsiz.b);  /* C-STAT */

        if (pdev->cfg.dma_enable == 1U)
        {
            USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPDMA, ep->dma_addr);
        }

        if (ep->type == EP_TYPE_ISOC)
        {
            if (ep->even_odd_frame != 0U)
            {
                depctl.b.setd1pid = 1U;
            }
            else
            {
                depctl.b.setd0pid = 1U;
            }
        }
        /* EP enable */
        depctl.b.cnak = 1U;
        depctl.b.epena = 1U;
        USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL, depctl.d32);
    }
    return status;
}

/**
 * @brief  USB_OTG_EP0StartXfer : Handle the setup for a data xfer for EP0 and
 *         starts the xfer
 * @param  [in] pdev             Instance for USB core
 * @param  [in] ep               Endpoint
 * @retval USB_OTG_STS      Status
 */
USB_OTG_STS USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS                 status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef      depctl;
    USB_OTG_DEP0XFRSIZ_TypeDef  deptsiz;
    USB_OTG_INEPREGS          *in_regs;
    uint32_t fifoemptymsk = 0UL;

    depctl.d32   = 0UL;
    deptsiz.d32  = 0UL;
    /* IN endpoint */
    if (ep->is_in == 1U)
    {
        in_regs = pdev->regs.INEP_REGS[0U];
        depctl.d32  = USB_OTG_READ_REG32(&in_regs->DIEPCTL);
        deptsiz.d32 = USB_OTG_READ_REG32(&in_regs->DIEPTSIZ);
        /* Zero Length Packet? */
        if (ep->xfer_len == 0U)
        {
            deptsiz.b.xfersize = 0U;
            deptsiz.b.pktcnt = 1U;
        }
        else
        {
            if (ep->xfer_len > ep->maxpacket)
            {
                ep->xfer_len = ep->maxpacket;
                deptsiz.b.xfersize = ep->maxpacket;
            }
            else
            {
                deptsiz.b.xfersize = ep->xfer_len;
            }
            deptsiz.b.pktcnt = 1U;
        }
        USB_OTG_WRITE_REG32(&in_regs->DIEPTSIZ, deptsiz.d32);

        if (pdev->cfg.dma_enable == 1U)
        {
            USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPDMA, ep->dma_addr);
        }

        /* EP enable, IN data in FIFO */
        depctl.b.cnak = 1U;
        depctl.b.epena = 1U;
        USB_OTG_WRITE_REG32(&in_regs->DIEPCTL, depctl.d32);

        if (pdev->cfg.dma_enable == 0U)
        {
            /* Enable the Tx FIFO Empty Interrupt for this EP */
            if (ep->xfer_len > 0U)
            {
                fifoemptymsk |= (uint32_t)1UL << ep->num;
                USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0UL, fifoemptymsk);
            }
        }
    }
    else
    {
        /* OUT endpoint */
        depctl.d32  = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
        deptsiz.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ);
        /* Program the transfer size and packet count as follows:
        * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
        * pktcnt = N           */
        if (ep->xfer_len == 0U)
        {
            //deptsiz.b.xfersize = ep->maxpacket;
            deptsiz.b.xfersize = 0U;
            deptsiz.b.pktcnt = 1U;
        }
        else
        {
            //ep->xfer_len = ep->maxpacket;
            //deptsiz.b.xfersize = ep->maxpacket;
            ep->xfer_len = __MIN(ep->rem_data_len, ep->maxpacket);
            deptsiz.b.xfersize = ep->xfer_len;
            deptsiz.b.pktcnt = 1U;
        }
        USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ, deptsiz.d32);
        if (pdev->cfg.dma_enable == 1U)
        {
            USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPDMA, ep->dma_addr);
        }
        /* EP enable */
        depctl.b.cnak = 1U;
        depctl.b.epena = 1U;
        USB_OTG_WRITE_REG32 (&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL), depctl.d32);
    }
    return status;
}

/**
 * @brief  USB_OTG_EPSetStall : Set the EP STALL
 * @param  [in] pdev             Instance for USB core
 * @param  [in] ep               Endpoint
 * @retval USB_OTG_STS      Status
 */
USB_OTG_STS USB_OTG_EPSetStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef  depctl;
    __IO uint32_t *depctl_addr;

    *(uint32_t*)&depctl.b = 0UL; /* C-STAT */
    if (ep->is_in == 1U)
    {
        depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(depctl_addr);  /* C-STAT */
        /* set the disable and stall bits */
        if (depctl.b.epena != 0U)
        {
            depctl.b.epdis = 1U;
        }
        depctl.b.stall = 1U;
        USB_OTG_WRITE_REG32(depctl_addr, *(uint32_t*)&depctl.b);   /* C-STAT */
    }
    else
    {
        depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(depctl_addr);  /* C-STAT */
        /* set the stall bit */
        depctl.b.stall = 1U;
        USB_OTG_WRITE_REG32(depctl_addr, *(uint32_t*)&depctl.b);/* C-STAT */
    }
    return status;
}


/**
 * @brief  Clear the EP STALL
 * @param  [in] pdev             Instance for USB core
 * @param  [in] ep               Endpoint
 * @retval USB_OTG_STS      Status
 */
USB_OTG_STS USB_OTG_EPClearStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_DEPCTL_TypeDef  depctl;
    __IO uint32_t *depctl_addr;

    depctl.d32 = 0UL;

    if (ep->is_in == 1U)
    {
        depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
    }
    else
    {
        depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    }
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    /* clear the stall bits */
    depctl.b.stall = 0U;
    if ((ep->type == EP_TYPE_INTR) || (ep->type == EP_TYPE_BULK))
    {
        depctl.b.setd0pid = 1U; /* DATA0 */
    }
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
    return status;
}
/**
 * @brief  USB_OTG_ReadDevAllOutEp_itr : returns OUT endpoint interrupt bits
 * @param  [in] pdev : Instance for USB core
 * @retval OUT endpoint interrupt bits
 */
uint32_t USB_OTG_ReadDevAllOutEp_itr(USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t v;
    v  = USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINT);
    v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINTMSK);
    return ((v & 0xFFFF0000UL) >> 16U);
}


/**
 * @brief  USB_OTG_ReadDevOutEP_itr : returns Device OUT EP Interrupt register
 * @param  [in] pdev     Instance for USB core
 * @param  [in] epnum    Endpoint number
 * @retval Device OUT EP Interrupt register
 */
uint32_t USB_OTG_ReadDevOutEP_itr(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
    uint32_t v;
    v  = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT);
    v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DOEPMSK);
    return v;
}


/**
 * @brief  USB_OTG_ReadDevAllInEPItr : Get int status register
 * @param  [in] pdev : Instance for USB core
 * @retval int status register
 */
uint32_t USB_OTG_ReadDevAllInEPItr(USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t v;
    v = USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINT);
    v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINTMSK);
    return (v & 0xFFFFUL);
}

/**
 * @brief  configures EPO to receive SETUP packets
 * @param  [in] pdev     Instance for USB core
 * @retval None
 */
void USB_OTG_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DEP0XFRSIZ_TypeDef  doeptsize0;

    doeptsize0.d32 = 0UL;
    doeptsize0.b.supcnt = 3U;
    doeptsize0.b.pktcnt = 1U;

    doeptsize0.b.xfersize = 64U;
    pdev->dev.out_ep[0].xfer_len = 64U;
    pdev->dev.out_ep[0].rem_data_len = 64U;
    pdev->dev.out_ep[0].total_data_len = 64U;
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0U]->DOEPTSIZ, doeptsize0.d32 );
    pdev->dev.ep0_state = USB_OTG_EP0_IDLE;

    if (pdev->cfg.dma_enable == 1U)
    {
        USB_OTG_DEPCTL_TypeDef  doepctl;
        doepctl.d32 = 0UL;
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPDMA, (uint32_t)&pdev->dev.setup_packet);

        /* EP enable */
        doepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[0]->DOEPCTL);
        doepctl.b.epena = 1U;
        doepctl.d32 = 0x80008000UL;
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0U]->DOEPCTL, doepctl.d32);
    }
}
/**
 * @brief  USB_OTG_RemoteWakeup : active remote wakeup signalling
 * @param  [in] pdev     Instance for USB core
 * @retval None
 */
void USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DCTL_TypeDef     dctl;
    USB_OTG_DSTS_TypeDef     dsts;
    USB_OTG_PCGCCTL_TypeDef  power;

    if (pdev->dev.DevRemoteWakeup != 0UL)
    {
        dsts.b = *(__IO stc_bUSB_OTG_DSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);  /* C-STAT */
        if(dsts.b.suspsts == 1U)
        {
            if(pdev->cfg.low_power != 0U)
            {
                /* un-gate USB Core clock */
                power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
                power.b.gatehclk = 0U;
                power.b.stoppclk = 0U;
                USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
            }
        }
    }
    /* active Remote wakeup signaling */
    dctl.d32 = 0U;
    dctl.b.rmtwkupsig = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, 0UL, dctl.d32);
    USB_OTG_BSP_mDelay(5UL);
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0UL );
}
/**
 * @brief  Set the EP Status
 * @param  [in] pdev     Instance for USB core
 * @param  [in] Status   New Status
 * @param  [in] ep       EP structure
 * @retval None
 */
void USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep , uint32_t Status)
{
    USB_OTG_DEPCTL_TypeDef  depctl;
    __IO uint32_t *depctl_addr;
    uint8_t u8RetFlag = 0U;

    *(uint32_t*)&depctl.b = 0UL; /* C-STAT */

    /* Process for IN endpoint */
    if (ep->is_in == 1U)
    {
        depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(depctl_addr);   /* C-STAT */

        if (Status == USB_OTG_EP_TX_STALL)
        {
            USB_OTG_EPSetStall(pdev, ep);
            u8RetFlag = 1U;   //return;
        }
        else if (Status == USB_OTG_EP_TX_NAK)
        {
            depctl.b.snak = 1U;
        }
        else if (Status == USB_OTG_EP_TX_VALID)
        {
            if (depctl.b.stall == 1U)
            {
                ep->even_odd_frame = 0U;
                USB_OTG_EPClearStall(pdev, ep);
                u8RetFlag = 1U;   //return;
            }
            depctl.b.cnak = 1U;
            depctl.b.usbactep = 1U;
            depctl.b.epena = 1U;
        }
        else if (Status == USB_OTG_EP_TX_DIS)
        {
            depctl.b.usbactep = 0U;
        }
        else
        {
            //else
            ;
        }
    }
    else /* Process for OUT endpoint */
    {
        depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
        depctl.b = *(__IO stc_bUSB_OTG_DEPCTL_t*)&USB_OTG_READ_REG32(depctl_addr);  /* C-STAT */

        if (Status == USB_OTG_EP_RX_STALL)
        {
            depctl.b.stall = 1U;
        }
        else if (Status == USB_OTG_EP_RX_NAK)
        {
            depctl.b.snak = 1U;
        }
        else if (Status == USB_OTG_EP_RX_VALID)
        {
            if (depctl.b.stall == 1U)
            {
                ep->even_odd_frame = 0U;
                USB_OTG_EPClearStall(pdev, ep);
                u8RetFlag = 1U; //return;
            }
            depctl.b.cnak = 1U;
            depctl.b.usbactep = 1U;
            depctl.b.epena = 1U;
        }
        else if (Status == USB_OTG_EP_RX_DIS)
        {
            depctl.b.usbactep = 0U;
        }
        else
        {
            //else
            ;
        }
    }

    if(1U != u8RetFlag)
    {
        USB_OTG_WRITE_REG32(depctl_addr, *(uint32_t*)&depctl.b);  /* C-STAT */
    }
}


#ifdef USE_HOST_MODE
/**
 * @brief  USB_OTG_CoreInitHost : Initializes USB_OTG controller for host mode
 * @param  [in] pdev     Instance for USB core
 * @retval status
 */
USB_OTG_STS USB_OTG_CoreInitHost(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS                     status = USB_OTG_OK;
    USB_OTG_FSIZ_TypeDef            nptxfifosize;
    USB_OTG_FSIZ_TypeDef            ptxfifosize;
    USB_OTG_HCFG_TypeDef            hcfg;

#ifdef USE_OTG_MODE
    USB_OTG_GOTGCTL_TypeDef          gotgctl;
#endif
    uint32_t                        i = 0UL;
    nptxfifosize.d32 = 0UL;
    ptxfifosize.d32 = 0UL;
#ifdef USE_OTG_MODE
    gotgctl.d32 = 0UL;
#endif
    hcfg.d32 = 0UL;

    /* configure charge pump IO */
    USB_OTG_BSP_ConfigVBUS(pdev);

    /* Restart the Phy Clock */
    USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, 0UL);

    /* Initialize Host Configuration Register */
    if (pdev->cfg.phy_itface == USB_OTG_ULPI_PHY)
    {
        USB_OTG_InitFSLSPClkSel(pdev , HCFG_30_60_MHZ);
        //USB_OTG_InitFSLSPClkSel(pdev , HCFG_48_MHZ);
    }
    else
    {
        //USB_OTG_InitFSLSPClkSel(pdev , HCFG_48_MHZ);
        USB_OTG_InitFSLSPClkSel(pdev , HCFG_6_MHZ);
    }
    USB_OTG_ResetPort(pdev);

    hcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.HREGS->HCFG);
    hcfg.b.fslssupp = 0U;
    USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HCFG, hcfg.d32);

    /* Configure data FIFO sizes */
    /* Rx FIFO */
#ifdef USB_OTG_FS_CORE
    if(pdev->cfg.coreID == USB_OTG_FS_CORE_ID)
    {
        /* set Rx FIFO size */
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRXFSIZ, (uint32_t)RX_FIFO_FS_SIZE);
        nptxfifosize.b.startaddr = RX_FIFO_FS_SIZE;
        nptxfifosize.b.depth = TXH_NP_FS_FIFOSIZ;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GNPTXFSIZ, nptxfifosize.d32);

        ptxfifosize.b.startaddr = RX_FIFO_FS_SIZE + TXH_NP_FS_FIFOSIZ;
        ptxfifosize.b.depth     = TXH_P_FS_FIFOSIZ;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->HPTXFSIZ, ptxfifosize.d32);
    }
#endif
#ifdef USB_OTG_HS_CORE
    if (pdev->cfg.coreID == USB_OTG_HS_CORE_ID)
    {
        /* set Rx FIFO size */
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GRXFSIZ, RX_FIFO_HS_SIZE);
        nptxfifosize.b.startaddr = RX_FIFO_HS_SIZE;
        nptxfifosize.b.depth = TXH_NP_HS_FIFOSIZ;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GNPTXFSIZ, nptxfifosize.d32);

        ptxfifosize.b.startaddr = RX_FIFO_HS_SIZE + TXH_NP_HS_FIFOSIZ;
        ptxfifosize.b.depth     = TXH_P_HS_FIFOSIZ;
        USB_OTG_WRITE_REG32(&pdev->regs.GREGS->HPTXFSIZ, ptxfifosize.d32);
    }
#endif

#ifdef USE_OTG_MODE
    /* Clear Host Set HNP Enable in the USB_OTG Control Register */
    gotgctl.b.hstsethnpen = 1U;
    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GOTGCTL, gotgctl.d32, 0U);
#endif

    /* Make sure the FIFOs are flushed. */
    USB_OTG_FlushTxFifo(pdev, 0x10UL);         /* all Tx FIFOs */
    USB_OTG_FlushRxFifo(pdev);

    /* Clear all pending HC Interrupts */
    for (i = 0UL; i < pdev->cfg.host_channels; i++)
    {
        USB_OTG_WRITE_REG32( &pdev->regs.HC_REGS[i]->HCINT, 0xFFFFFFFFUL );
        USB_OTG_WRITE_REG32( &pdev->regs.HC_REGS[i]->HCINTMSK, 0UL );
    }
#ifndef USE_OTG_MODE
    USB_OTG_DriveVbus(pdev, 1U);
#endif

    USB_OTG_EnableHostInt(pdev);
    return status;
}

/**
 * @brief  USB_OTG_DriveVbus : set/reset vbus
 * @param  [in] pdev         Instance for USB core
 * @param  [in] state        VBUS state
 * @retval None
 */
void USB_OTG_DriveVbus (USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{
    USB_OTG_HPRT0_TypeDef     hprt0;
    uint32_t u32ReadHprtTemp = 0UL;  /* C-STAT */

    *(uint32_t*)&hprt0.b = 0UL;  /* C-STAT */

    /* enable disable the external charge pump */
    USB_OTG_BSP_DriveVBUS(pdev, state);

    /* Turn on the Host port power. */
    //hprt0.d32 = USB_OTG_ReadHPRT0(pdev);  /* C-STAT */
    u32ReadHprtTemp = USB_OTG_ReadHPRT0(pdev);
    hprt0.b = *(stc_bUSB_OTG_HPRT0_t*)&u32ReadHprtTemp; /* C-STAT */

    if ((hprt0.b.prtpwr == 0U ) && (state == 1U ))
    {
        hprt0.b.prtpwr = 1U;
        USB_OTG_WRITE_REG32(pdev->regs.HPRT0, *(uint32_t*)&hprt0.b);  /* C-STAT */
    }
    if ((hprt0.b.prtpwr == 1U ) && (state == 0U ))
    {
        hprt0.b.prtpwr = 0U;
        USB_OTG_WRITE_REG32(pdev->regs.HPRT0, *(uint32_t*)&hprt0.b);   /* C-STAT */
    }

    USB_OTG_BSP_mDelay(200UL);
}

/**
 * @brief  USB_OTG_EnableHostInt: Enables the Host mode interrupts
 * @param  [in] pdev     Instance for USB core
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_EnableHostInt(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_STS       status = USB_OTG_OK;
    USB_OTG_GINTMSK_TypeDef  intmsk;
    intmsk.d32 = 0UL;
    /* Disable all interrupts. */
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTMSK, 0UL);

    /* Clear any pending interrupts. */
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, 0xFFFFFFFFUL);

    /* Enable the common interrupts */
    USB_OTG_EnableCommonInt(pdev);

    if (pdev->cfg.dma_enable == 0U)
    {
        intmsk.b.rxstsqlvl  = 1U;
    }
    intmsk.b.portintr   = 1U;
    intmsk.b.hcintr     = 1U;
    intmsk.b.disconnect = 1U;
    intmsk.b.sofintr    = 1U;
    intmsk.b.incomplisoout  = 1U;
    USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GINTMSK, intmsk.d32, intmsk.d32);
    return status;
}


/**
 * @brief  USB_OTG_ReadHPRT0 : Reads HPRT0 to modify later
 * @param  [in] pdev     Instance for USB core
 * @retval HPRT0 value
 */
uint32_t USB_OTG_ReadHPRT0(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_HPRT0_TypeDef  hprt0;

    hprt0.d32 = USB_OTG_READ_REG32(pdev->regs.HPRT0);
    hprt0.b.prtena = 0U;
    hprt0.b.prtconndet = 0U;
    hprt0.b.prtenchng = 0U;
    hprt0.b.prtovrcurrchng = 0U;
    return hprt0.d32;
}

/**
 * @brief  USB_OTG_ReadHostAllChannels_intr : Register PCD Callbacks
 * @param  [in] pdev     Instance for USB core
 * @retval Status
 */
uint32_t USB_OTG_ReadHostAllChannels_intr (USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_READ_REG32 (&pdev->regs.HREGS->HAINT));
}

/**
 * @brief  USB_OTG_ResetPort : Reset Host Port
 * @param  [in] pdev     Instance for USB core
 * @retval status
 * @note    (1)The application must wait at least 10 ms (+ 10 ms security)
 *   before clearing the reset bit.
 */
uint32_t USB_OTG_ResetPort(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_HPRT0_TypeDef  hprt0;

    hprt0.d32 = USB_OTG_ReadHPRT0(pdev);
    hprt0.b.prtrst = 1U;
    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, hprt0.d32);
    USB_OTG_BSP_mDelay (10UL);                                /* See Note #1 */
    hprt0.b.prtrst = 0U;
    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, hprt0.d32);
    USB_OTG_BSP_mDelay (20UL);
    return 1UL;
}
uint32_t USB_OTG_testmode(USB_OTG_CORE_HANDLE *pdev,int mode)
{
    USB_OTG_HPRT0_TypeDef  hprt0;

    hprt0.d32 = USB_OTG_ReadHPRT0(pdev);
    hprt0.b.prttstctl = (uint32_t)mode;
    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, hprt0.d32);
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("test mode stated\n");
#endif
    return 1UL;
}

/**
 * @brief  USB_OTG_HC_Init : Prepares a host channel for transferring packets
 * @param  [in] pdev     Instance for USB core
 * @param  [in] hc_num   Channel number
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_HC_Init(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
    USB_OTG_STS status = USB_OTG_OK;
    uint32_t intr_enable = 0UL;
    USB_OTG_HCINTMSK_TypeDef  hcintmsk;
    USB_OTG_GINTMSK_TypeDef    gintmsk;
    USB_OTG_HCCHAR_TypeDef     hcchar;
    USB_OTG_HCINTn_TypeDef     hcint;

    gintmsk.d32 = 0UL;
    hcintmsk.d32 = 0UL;
    hcchar.d32 = 0UL;

    /* Clear old interrupt conditions for this host channel. */
    hcint.d32 = 0xFFFFFFFFUL;
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCINT, hcint.d32);

    /* Enable channel interrupts required for this transfer. */
    hcintmsk.d32 = 0UL;

    if (pdev->cfg.dma_enable == 1U)
    {
        hcintmsk.b.ahberr = 1U;
    }

    switch (pdev->host.hc[hc_num].ep_type)
    {
        case EP_TYPE_CTRL:
        case EP_TYPE_BULK:
            hcintmsk.b.xfercompl = 1U;
            hcintmsk.b.stall = 1U;
            hcintmsk.b.xacterr = 1U;
            hcintmsk.b.datatglerr = 1U;
            hcintmsk.b.nak = 1U;
            if (pdev->host.hc[hc_num].ep_is_in)
            {
                hcintmsk.b.bblerr = 1U;
            }
            else
            {
                hcintmsk.b.nyet = 1U;
                if (pdev->host.hc[hc_num].do_ping)
                {
                    hcintmsk.b.ack = 1U;
                }
            }
            break;
        case EP_TYPE_INTR:
            hcintmsk.b.xfercompl = 1U;
            hcintmsk.b.nak = 1U;
            hcintmsk.b.stall = 1U;
            hcintmsk.b.xacterr = 1U;
            hcintmsk.b.datatglerr = 1U;
            hcintmsk.b.frmovrun = 1U;

            if (pdev->host.hc[hc_num].ep_is_in)
            {
                hcintmsk.b.bblerr = 1U;
            }
            break;
        case EP_TYPE_ISOC:
            hcintmsk.b.xfercompl = 1U;
            hcintmsk.b.frmovrun = 1U;
            hcintmsk.b.ack = 1U;

            if (pdev->host.hc[hc_num].ep_is_in)
            {
                hcintmsk.b.xacterr = 1U;
                hcintmsk.b.bblerr = 1U;
            }
            break;
    }

    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCINTMSK, hcintmsk.d32);

    /* Enable the top level host channel interrupt. */
    intr_enable = ((uint32_t)1 << hc_num);
    USB_OTG_MODIFY_REG32(&pdev->regs.HREGS->HAINTMSK, 0UL, intr_enable);

    /* Make sure host channel interrupts are enabled. */
    gintmsk.b.hcintr = 1UL;
    USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GINTMSK, 0UL, gintmsk.d32);

    /* Program the HCCHAR register */
    hcchar.d32 = 0UL;
    hcchar.b.devaddr = pdev->host.hc[hc_num].dev_addr;
    hcchar.b.epnum   = pdev->host.hc[hc_num].ep_num;
    hcchar.b.epdir   = pdev->host.hc[hc_num].ep_is_in;
    hcchar.b.lspddev = (pdev->host.hc[hc_num].speed == HPRT0_PRTSPD_LOW_SPEED);
    hcchar.b.eptype  = pdev->host.hc[hc_num].ep_type;
    hcchar.b.mps     = pdev->host.hc[hc_num].max_packet;
    if (pdev->host.hc[hc_num].ep_type == HCCHAR_INTR)
    {
        hcchar.b.oddfrm  = 1U;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, hcchar.d32);
    return status;
}

/**
 * @brief  USB_OTG_HC_StartXfer : Start transfer
 * @param  [in] pdev     Instance for USB core
 * @param  [in] hc_num   Channel number
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_HC_StartXfer(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_HCCHAR_TypeDef   hcchar;
    USB_OTG_HCTSIZn_TypeDef  hctsiz;
    USB_OTG_HNPTXSTS_TypeDef hnptxsts;
    USB_OTG_HPTXSTS_TypeDef  hptxsts;
    USB_OTG_GINTMSK_TypeDef  intmsk;
    uint16_t                 len_words = 0U;

    uint16_t num_packets;
    uint16_t max_hc_pkt_count;

    max_hc_pkt_count = 256U;
    hctsiz.d32 = 0UL;
    hcchar.d32 = 0UL;
    intmsk.d32 = 0UL;

    /* Compute the expected number of packets associated to the transfer */
    if (pdev->host.hc[hc_num].xfer_len > 0U)
    {
        num_packets = (uint16_t)((pdev->host.hc[hc_num].xfer_len + \
        (uint32_t)pdev->host.hc[hc_num].max_packet - 1UL) / (uint32_t)pdev->host.hc[hc_num].max_packet);

        if (num_packets > max_hc_pkt_count)
        {
            num_packets = max_hc_pkt_count;
            pdev->host.hc[hc_num].xfer_len = (uint32_t)num_packets * \
            (uint32_t)pdev->host.hc[hc_num].max_packet;
        }
    }
    else
    {
        num_packets = 1U;
    }
    if (pdev->host.hc[hc_num].ep_is_in)
    {
        pdev->host.hc[hc_num].xfer_len = (uint32_t)num_packets * \
        (uint32_t)pdev->host.hc[hc_num].max_packet;
    }
    /* Initialize the HCTSIZn register */
    hctsiz.b.xfersize = pdev->host.hc[hc_num].xfer_len;
    hctsiz.b.pktcnt = num_packets;
    hctsiz.b.pid = pdev->host.hc[hc_num].data_pid;
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCTSIZ, hctsiz.d32);

    if (pdev->cfg.dma_enable == 1U)
    {
        USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCDMA, (unsigned int)pdev->host.hc[hc_num].xfer_buff);
    }

    hcchar.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
    hcchar.b.oddfrm = USB_OTG_IsEvenFrame(pdev);

    /* Set host channel enable */
    hcchar.b.chen = 1U;
    hcchar.b.chdis = 0U;
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, hcchar.d32);

    if (pdev->cfg.dma_enable == 0U) /* Slave mode */
    {
        if((pdev->host.hc[hc_num].ep_is_in == 0U) && (pdev->host.hc[hc_num].xfer_len > 0U))
        {
            switch(pdev->host.hc[hc_num].ep_type)
            {
                /* Non periodic transfer */
                case EP_TYPE_CTRL:
                case EP_TYPE_BULK:
                    hnptxsts.b = *(__IO stc_bUSB_OTG_HNPTXSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.GREGS->GNPTXSTS);  /* C-STAT */
                    len_words = (uint16_t)((pdev->host.hc[hc_num].xfer_len + 3UL) / 4UL);

                    /* check if there is enough space in FIFO space */
                    if(len_words > hnptxsts.b.nptxfspcavail)
                    {
                        /* need to process data in nptxfempty interrupt */
                        intmsk.b.nptxfempty = 1U;
                        USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, 0UL, intmsk.d32);
                    }
                    break;
                /* Periodic transfer */
                case EP_TYPE_INTR:
                case EP_TYPE_ISOC:
                    hptxsts.b = *(__IO stc_bUSB_OTG_HPTXSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.HREGS->HPTXSTS);   /* C-STAT */
                    len_words = (uint16_t)((pdev->host.hc[hc_num].xfer_len + 3UL) / 4UL);
                    /* check if there is enough space in FIFO space */
                    if(len_words > hptxsts.b.ptxfspcavail) /* split the transfer */
                    {
                        /* need to process data in ptxfempty interrupt */
                        intmsk.b.ptxfempty = 1U;
                        USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, 0UL, intmsk.d32);
                    }
                    break;
                default:
                    break;
            }

            /* Write packet into the Tx FIFO. */
            USB_OTG_WritePacket(pdev,
            pdev->host.hc[hc_num].xfer_buff ,
            hc_num, (uint16_t)pdev->host.hc[hc_num].xfer_len);
        }
    }
    return status;
}

/**
 * @brief  USB_OTG_HC_Halt : Halt channel
 * @param  [in] pdev     Instance for USB core
 * @param  [in] hc_num   Channel number
 * @retval USB_OTG_STS status
 */
USB_OTG_STS USB_OTG_HC_Halt(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
    USB_OTG_STS status = USB_OTG_OK;
    USB_OTG_HNPTXSTS_TypeDef            nptxsts;
    USB_OTG_HPTXSTS_TypeDef             hptxsts;
    USB_OTG_HCCHAR_TypeDef              hcchar;

    *(uint32_t*)&nptxsts.b = 0UL;  /* C-STAT */
    *(uint32_t*)&hptxsts.b = 0UL;  /* C-STAT */
    hcchar.b = *(__IO stc_bUSB_OTG_HCCHAR_t*)&USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);  /* C-STAT */
    hcchar.b.chen = 1U;
    hcchar.b.chdis = 1U;

    /* Check for space in the request queue to issue the halt. */
    if ((hcchar.b.eptype == HCCHAR_CTRL) || (hcchar.b.eptype == HCCHAR_BULK))
    {
        nptxsts.b = *(__IO stc_bUSB_OTG_HNPTXSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.GREGS->GNPTXSTS);  /* C-STAT */
        if (nptxsts.b.nptxqspcavail == 0U)
        {
            hcchar.b.chen = 0U;
        }
    }
    else
    {
        hptxsts.b = *(__IO stc_bUSB_OTG_HPTXSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.HREGS->HPTXSTS);  /* C-STAT */
        if (hptxsts.b.ptxqspcavail == 0U)
        {
            hcchar.b.chen = 0U;
        }
    }
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, *(uint32_t*)&hcchar.b);  /* C-STAT */
    return status;
}

/**
 * @brief  Issue a ping token
 * @param  [in] pdev     Instance for USB core
 * @param  [in] hc_num   Channel number
 * @retval None
 */
USB_OTG_STS USB_OTG_HC_DoPing(USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num)
{
    USB_OTG_STS               status = USB_OTG_OK;
    USB_OTG_HCCHAR_TypeDef    hcchar;
    USB_OTG_HCTSIZn_TypeDef   hctsiz;

    hctsiz.d32 = 0UL;
    hctsiz.b.dopng = 1U;
    hctsiz.b.pktcnt = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCTSIZ, hctsiz.d32);

    hcchar.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
    hcchar.b.chen = 1U;
    hcchar.b.chdis = 0U;
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, hcchar.d32);
    return status;
}


/**
 * @brief  USB_OTG_IsHostMode : Check if it is host mode
 * @param  [in] pdev     Instance for USB core
 * @retval num_in_ep
 */
uint8_t USB_OTG_IsHostMode(USB_OTG_CORE_HANDLE *pdev)
{
    return (USB_OTG_GetMode(pdev) == HOST_MODE);
}

/**
 * @brief  USB_OTG_IsEvenFrame
*          This function returns the frame number for sof packet
 * @param  [in] pdev     Instance for USB core
 * @retval Frame number
 */
uint8_t USB_OTG_IsEvenFrame (USB_OTG_CORE_HANDLE *pdev)
{
    return !(USB_OTG_READ_REG32(&pdev->regs.HREGS->HFNUM) & 0x1U);
}

/**
 * @brief  USB_OTG_InitFSLSPClkSel : Initializes the FSLSPClkSel field of the
 *         HCFG register on the PHY type
 * @param  [in] pdev     Instance for USB core
 * @param  [in] freq     Clock frequency
 * @retval None
 */
void USB_OTG_InitFSLSPClkSel(USB_OTG_CORE_HANDLE *pdev , uint8_t freq)
{
    USB_OTG_HCFG_TypeDef   hcfg;

    hcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.HREGS->HCFG);
    hcfg.b.fslspclksel = freq;
    USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HCFG, hcfg.d32);
}

/**
 * @brief  Stop the device and clean up fifo's
 * @param  [in] pdev     Instance for USB core
 * @retval None
 */
void USB_OTG_StopHost(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_HCCHAR_TypeDef  hcchar;
    uint32_t                i;

    USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HAINTMSK , 0UL);
    USB_OTG_WRITE_REG32(&pdev->regs.HREGS->HAINT, 0xFFFFFFFFUL);
    /* Flush out any leftover queued requests. */
    for (i = 0UL; i < pdev->cfg.host_channels; i++)
    {
        hcchar.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[i]->HCCHAR);
        hcchar.b.chen = 0U;
        hcchar.b.chdis = 1U;
        hcchar.b.epdir = 0U;
        USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[i]->HCCHAR, hcchar.d32);
    }

    /* Flush the FIFO */
    USB_OTG_FlushRxFifo(pdev);
    USB_OTG_FlushTxFifo(pdev, 0x10UL);
}
void USB_OTG_PRTSUSP(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_HPRT0_TypeDef     port0;
    USB_OTG_PCGCCTL_TypeDef   power;

    port0.d32 = USB_OTG_ReadHPRT0(pdev);
    port0.b.prtsusp = 1U;
    port0.b.prtres = 0U;
    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, port0.d32);

    /* Low power mode */
    power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
    power.b.stoppclk = 1U;
    USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
    power.b.gatehclk = 1U;
    USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
}

void USB_OTG_ACTIVE_WAKEUP(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_HPRT0_TypeDef     port0;
    USB_OTG_PCGCCTL_TypeDef   power;
    /* Release low power mode */
    power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
    power.b.gatehclk = 0U;
    power.b.stoppclk = 0U;
    USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);

    USB_OTG_BSP_mDelay(1UL);
    /* Port resume on */
    port0.d32 = USB_OTG_ReadHPRT0(pdev);
    port0.b.prtsusp = 0U;
    port0.b.prtres = 1U;
    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, port0.d32);

    USB_OTG_BSP_mDelay(20UL);

    /* Port resume off */
    port0.b.prtres = 0U;
    USB_OTG_WRITE_REG32(pdev->regs.HPRT0, port0.d32);
}

//void USB_OTG_INTI_SRP(USB_OTG_CORE_HANDLE *pdev)
//{
//}


#endif /* USE_HOST_MODE */


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
