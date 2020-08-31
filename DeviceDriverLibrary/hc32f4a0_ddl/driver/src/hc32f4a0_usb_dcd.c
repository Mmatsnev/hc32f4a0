/**
 *******************************************************************************
 * @file  hc32f4a0_usb_dcd.c
 * @brief Peripheral Device Interface Layer.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin         First version
   2020-08-18       Wangmin         Modify DCD_Init() function
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
#include "hc32f4a0_usb_dcd.h"
#include "usb_bsp.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_USB_DCD USB DCD
 * @brief USB device core function
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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @defgroup USB_DCD_Global_Functions USB DCD Global Functions
 * @{
 */

/**
 * @brief  Device intialize
 * @param pdev      Device instance
 * @param coreID    Core ID
 * @retval None
 */
void DCD_Init(USB_OTG_CORE_HANDLE *pdev ,
              USB_OTG_CORE_ID_TypeDef coreID)
{
    uint32_t i;
    USB_OTG_EP *ep;

    USB_OTG_SelectCore (pdev , coreID);

    pdev->dev.device_status = (uint8_t)USB_OTG_DEFAULT;
    pdev->dev.device_address = 0U;

    /* Init ep structure */
    for (i = 0UL; i < pdev->cfg.dev_endpoints ; i++)
    {
        ep = &pdev->dev.in_ep[i];
        /* Init ep structure */
        ep->is_in = 1U;
        ep->num = (uint8_t)i;
        ep->tx_fifo_num = (uint16_t)i;
        /* Control until ep is actvated */
        ep->type = EP_TYPE_CTRL;
        ep->maxpacket =  USB_OTG_MAX_EP0_SIZE;
        ep->xfer_buff = 0U;
        ep->xfer_len = 0U;
    }

    for (i = 0UL; i < pdev->cfg.dev_endpoints; i++)
    {
        ep = &pdev->dev.out_ep[i];
        /* Init ep structure */
        ep->is_in = 0U;
        ep->num = (uint8_t)i;
        ep->tx_fifo_num = (uint16_t)i;
        /* Control until ep is activated */
        ep->type = EP_TYPE_CTRL;
        ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
        ep->xfer_buff = 0U;
        ep->xfer_len = 0U;
    }

    USB_OTG_DisableGlobalInt(pdev);

    /*Init the Core (common init.) */
    USB_OTG_CoreInit(pdev);

    /* Force Device Mode*/
#ifndef USE_OTG_MODE
    USB_OTG_SetCurrentMode(pdev, DEVICE_MODE);
#endif

    /* Init Device */
    USB_OTG_CoreInitDev(pdev);

    /* Enable USB Global interrupt */
    USB_OTG_EnableGlobalInt(pdev);
}

/**
 * @brief  Configure an EP
 * @param pdev      Device instance
 * @param ep_addr   Endpoint address
 * @param ep_mps    Endpoint max packet size
 * @param ep_type   Endpoint type
 * @retval Status
 */
uint32_t DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev ,
                     uint8_t ep_addr,
                     uint16_t ep_mps,
                     uint8_t ep_type)
{
    USB_OTG_EP *ep;

    if ((ep_addr & 0x80U) == 0x80U)
    {
        ep = &pdev->dev.in_ep[ep_addr & 0x7FU];
    }
    else
    {
        ep = &pdev->dev.out_ep[ep_addr & 0x7FU];
    }
    ep->num   = ep_addr & 0x7FU;

    ep->is_in = (uint8_t)((0x80U & ep_addr) != 0U);
    ep->maxpacket = ep_mps;
    ep->type = ep_type;
    if (ep->is_in != 0U)
    {
        /* Assign a Tx FIFO */
        ep->tx_fifo_num = ep->num;
    }
    /* Set initial data PID. */
    if (ep_type == USB_OTG_EP_BULK )
    {
        ep->data_pid_start = 0U;
    }
    USB_OTG_EPActivate(pdev, ep);
    return 0U;
}

/**
 * @brief  called when an EP is disabled
 * @param pdev      Device instance
 * @param ep_addr   Endpoint address
 * @retval Status
 */
uint32_t DCD_EP_Close(USB_OTG_CORE_HANDLE *pdev , uint8_t  ep_addr)
{
    USB_OTG_EP *ep;

    if ((ep_addr&0x80U) == 0x80U)
    {
        ep = &pdev->dev.in_ep[ep_addr & 0x7FU];
    }
    else
    {
        ep = &pdev->dev.out_ep[ep_addr & 0x7FU];
    }
    ep->num   = ep_addr & 0x7FU;
    ep->is_in = (uint8_t)((0x80U & ep_addr) != 0U);
    USB_OTG_EPDeactivate(pdev, ep);
    return 0U;
}

/**
 * @brief DCD_EP_PrepareRx
 * @param pdev      Device instance
 * @param ep_addr   Endpoint address
 * @param pbuf      Pointer to Rx buffer
 * @param buf_len   Data length
 * @retval Status
 */
uint32_t DCD_EP_PrepareRx( USB_OTG_CORE_HANDLE *pdev,
                           uint8_t ep_addr,
                           uint8_t *pbuf,
                           uint16_t  buf_len)
{
    USB_OTG_EP *ep;

    ep = &pdev->dev.out_ep[ep_addr & 0x7FU];

    /*setup and start the Xfer */
    ep->xfer_buff = pbuf;
    ep->xfer_len = buf_len;
    ep->xfer_count = 0U;
    ep->is_in = 0U;
    ep->num = ep_addr & 0x7FU;

    if (pdev->cfg.dma_enable == 1U)
    {
        ep->dma_addr = (uint32_t)pbuf;
    }

    if ( ep->num == 0U)
    {
        USB_OTG_EP0StartXfer(pdev, ep);
    }
    else
    {
        USB_OTG_EPStartXfer(pdev, ep);
    }
    return 0U;
}

/**
 * @brief Transmit data over USB
 * @param pdev      Device instance
 * @param ep_addr   Endpoint address
 * @param pbuf      pointer to Tx buffer
 * @param buf_len   Data length
 * @retval Status
 */
uint32_t  DCD_EP_Tx ( USB_OTG_CORE_HANDLE *pdev,
                     uint8_t   ep_addr,
                     uint8_t   *pbuf,
                     uint32_t   buf_len)
{
    USB_OTG_EP *ep;

    ep = &pdev->dev.in_ep[ep_addr & 0x7FU];

    /* Setup and start the Transfer */
    ep->is_in = 1U;
    ep->num = ep_addr & 0x7FU;
    ep->xfer_buff = pbuf;
    ep->dma_addr = (uint32_t)pbuf;
    ep->xfer_count = 0U;
    ep->xfer_len  = buf_len;

    if ( ep->num == 0U)
    {
        USB_OTG_EP0StartXfer(pdev, ep);
    }
    else
    {
        USB_OTG_EPStartXfer(pdev, ep);
    }
    return 0U;
}

/**
 * @brief  Stall an endpoint.
 * @param pdev      Device instance
 * @param epnum     Endpoint address
 * @retval Status
 */
uint32_t DCD_EP_Stall (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    USB_OTG_EP *ep;
    if ((0x80U & epnum) == 0x80U)
    {
        ep = &pdev->dev.in_ep[epnum & 0x7FU];
    }
    else
    {
        ep = &pdev->dev.out_ep[epnum];
    }

    ep->is_stall = 1U;
    ep->num   = epnum & 0x7FU;
    ep->is_in = (uint8_t)((epnum & 0x80U) == 0x80U);

    USB_OTG_EPSetStall(pdev, ep);
    return (0U);
}

/**
 * @brief  Clear stall condition on endpoints.
 * @param pdev      Device instance
 * @param epnum     Endpoint address
 * @retval Status
 */
uint32_t  DCD_EP_ClrStall (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    USB_OTG_EP *ep;
    if ((0x80U & epnum) == 0x80U)
    {
        ep = &pdev->dev.in_ep[epnum & 0x7FU];
    }
    else
    {
        ep = &pdev->dev.out_ep[epnum];
    }

    ep->is_stall = 0U;
    ep->num   = epnum & 0x7FU;
    ep->is_in = (uint8_t)((epnum & 0x80U) == 0x80U);

    USB_OTG_EPClearStall(pdev, ep);
    return (0U);
}

/**
 * @brief  This Function flushes the FIFOs.
 * @param pdev      Device instance
 * @param epnum     Endpoint address
 * @retval Status
 */
uint32_t  DCD_EP_Flush (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
    if ((epnum & 0x80U) == 0x80U)
    {
        USB_OTG_FlushTxFifo(pdev, (uint32_t)epnum & 0x7FUL);
    }
    else
    {
        USB_OTG_FlushRxFifo(pdev);
    }
    return (0U);
}

/**
 * @brief  This Function set USB device address
 * @param pdev      Device instance
 * @param address   New device address
 * @retval Status
 */
void  DCD_EP_SetAddress (USB_OTG_CORE_HANDLE *pdev, uint8_t address)
{
    USB_OTG_DCFG_TypeDef  dcfg;
    dcfg.d32 = 0UL;
    dcfg.b.devaddr = address;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCFG, 0UL, dcfg.d32);
}

/**
 * @brief  Connect device (enable internal pull-up)
 * @param pdev      Device instance
 * @retval None
 */
void  DCD_DevConnect (USB_OTG_CORE_HANDLE *pdev)
{
#ifndef USE_OTG_MODE
    USB_OTG_DCTL_TypeDef  dctl;
    dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
    /* Connect device */
    dctl.b.sftdiscon  = 0U;
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, dctl.d32);
    USB_OTG_BSP_mDelay(3UL);
#endif
}

/**
 * @brief  Disconnect device (disable internal pull-up)
 * @param pdev      Device instance
 * @retval None
 */
void  DCD_DevDisconnect (USB_OTG_CORE_HANDLE *pdev)
{
#ifndef USE_OTG_MODE
    USB_OTG_DCTL_TypeDef  dctl;
    dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
    /* Disconnect device for 3ms */
    dctl.b.sftdiscon  = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, dctl.d32);
    USB_OTG_BSP_mDelay(3UL);
#endif
}

/**
 * @brief  returns the EP Status
 * @param  [in] pdev     Instance for USB core
 * @param  [in] epnum    Endpoint address
 * @retval EP status
 */
uint32_t DCD_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,uint8_t epnum)
{
    USB_OTG_EP *ep;
    uint32_t Status;

    if ((0x80U & epnum) == 0x80U)
    {
        ep = &pdev->dev.in_ep[epnum & 0x7FU];
    }
    else
    {
        ep = &pdev->dev.out_ep[epnum];
    }

    Status = USB_OTG_GetEPStatus(pdev ,ep);
    /* Return the current status */
    return Status;
}

/**
 * @brief  Set the EP Status
 * @param  [in] pdev         Instance for USB core
 * @param  [in] Status       New Status
 * @param  [in] epnum        EP address
 * @retval None
 */
void DCD_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum , uint32_t Status)
{
    USB_OTG_EP *ep;

    if ((0x80U & epnum) == 0x80U)
    {
        ep = &pdev->dev.in_ep[epnum & 0x7FU];
    }
    else
    {
        ep = &pdev->dev.out_ep[epnum];
    }

    USB_OTG_SetEPStatus(pdev, ep, Status);
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
