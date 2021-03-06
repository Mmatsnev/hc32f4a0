/**
 *******************************************************************************
 * @file  usbh_hcs.c
 * @brief Implements functions for opening and closing host channels
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
#include "usbh_hcs.h"

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_HOST_LIB
 * @{
 */

/**
 * @addtogroup USB_HOST_CORE
 * @{
 */

/** @defgroup USBH_HCS USB host HCS
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
 * @defgroup USBH_HCS_Local_Functions USBH Channel Control Local Functions
 * @{
 */
static uint16_t USBH_GetFreeChannel (USB_OTG_CORE_HANDLE *pdev);
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
 * @defgroup USBH_HCS_Global_Functions USBH Channel Control Global Functions
 * @{
 */

/**
 * @brief  USBH_Open_Channel
 *         Open a  pipe
 * @param  [in] pdev : Instance for USB core
 * @param  [in] hc_num: Host channel Number
 * @param  [in] dev_address: USB Device address allocated to attached device
 * @param  [in] speed : USB device speed (Full/Low)
 * @param  [in] ep_type: end point type (Bulk/int/ctl)
 * @param  [in] mps: max pkt size
 * @retval Status
 */
uint8_t USBH_Open_Channel  (USB_OTG_CORE_HANDLE *pdev,
                            uint8_t hc_num,
                            uint8_t dev_address,
                            uint8_t speed,
                            uint8_t ep_type,
                            uint16_t mps)
{
    pdev->host.hc[hc_num].ep_num =(uint8_t) pdev->host.channel[hc_num]& 0x7FU;
    pdev->host.hc[hc_num].ep_is_in = (pdev->host.channel[hc_num] & 0x80U ) == 0x80U;
    pdev->host.hc[hc_num].dev_addr = dev_address;
    pdev->host.hc[hc_num].ep_type = ep_type;
    pdev->host.hc[hc_num].max_packet = mps;
    pdev->host.hc[hc_num].speed = speed;
    pdev->host.hc[hc_num].toggle_in = 0U;
    pdev->host.hc[hc_num].toggle_out = 0U;
    if(speed == HPRT0_PRTSPD_HIGH_SPEED)
    {
        pdev->host.hc[hc_num].do_ping = 1U;
    }

    USB_OTG_HC_Init(pdev, hc_num) ;
    return (uint8_t)HC_OK;
}

/**
 * @brief  USBH_Modify_Channel
 *         Modify a  pipe
 * @param  [in] pdev : Instance for USB core
 * @param  [in] hc_num: Host channel Number
 * @param  [in] dev_address: USB Device address allocated to attached device
 * @param  [in] speed : USB device speed (Full/Low)
 * @param  [in] ep_type: end point type (Bulk/int/ctl)
 * @param  [in] mps: max pkt size
 * @retval Status
 */
uint8_t USBH_Modify_Channel (USB_OTG_CORE_HANDLE *pdev,
                            uint8_t hc_num,
                            uint8_t dev_address,
                            uint8_t speed,
                            uint8_t ep_type,
                            uint16_t mps)
{
    if(dev_address != 0U)
    {
        pdev->host.hc[hc_num].dev_addr = dev_address;
    }

    if((pdev->host.hc[hc_num].max_packet != mps) && (mps != 0U))
    {
        pdev->host.hc[hc_num].max_packet = mps;
    }

    if((pdev->host.hc[hc_num].speed != speed ) && (speed != 0U))
    {
        pdev->host.hc[hc_num].speed = speed;
    }

    USB_OTG_HC_Init(pdev, hc_num);
    return (uint8_t)HC_OK;
}

/**
 * @brief  USBH_Alloc_Channel
 *         Allocate a new channel for the pipe
 * @param  [in] pdev: Select device
 * @param  [in] ep_addr: End point for which the channel to be allocated
 * @retval hc_num: Host channel number
 */
uint8_t USBH_Alloc_Channel  (USB_OTG_CORE_HANDLE *pdev, uint8_t ep_addr)
{
    uint16_t hc_num;

    hc_num =  USBH_GetFreeChannel(pdev);

    if (hc_num != HC_ERROR)
    {
        pdev->host.channel[hc_num & (USB_OTG_MAX_TX_FIFOS-1U)] = HC_USED | ep_addr;  /* C-STAT */
    }
    return (uint8_t)hc_num;
}

/**
 * @brief  USBH_Free_Pipe
 *         Free the USB host channel
 * @param  [in] pdev: Select device
 * @param  [in] idx: Channel number to be freed
 * @retval Status
 */
uint8_t USBH_Free_Channel  (USB_OTG_CORE_HANDLE *pdev, uint8_t idx)
{
    if(idx < HC_MAX)
    {
        pdev->host.channel[idx & (USB_OTG_MAX_TX_FIFOS-1U)] &= HC_USED_MASK;
    }
    return USBH_OK;
}

/**
 * @brief  USBH_DeAllocate_AllChannel
 *         Free all USB host channel
 * @param  [in] pdev : core instance
 * @retval Status
 */
uint8_t USBH_DeAllocate_AllChannel  (USB_OTG_CORE_HANDLE *pdev)
{
    uint8_t idx;

    for (idx = 2U; idx < HC_MAX ; idx ++)
    {
        pdev->host.channel[idx & (USB_OTG_MAX_TX_FIFOS-1U)] = 0U;
    }
    return USBH_OK;
}
/**
 * @}
 */

/**
 * @addtogroup USBH_HCS_Local_Functions USBH Channel Control Local Functions
 * @{
 */

/**
 * @brief  USBH_GetFreeChannel
 *         Get a free channel number for allocation to a device endpoint
 * @param  [in] pdev: Select device
 * @retval idx: Free Channel number
 */
static uint16_t USBH_GetFreeChannel (USB_OTG_CORE_HANDLE *pdev)
{
    uint8_t idx = 0U;
    uint16_t u16Ret = HC_ERROR;

    for (idx = 0U ; idx < HC_MAX ; idx++)
    {
        if ((pdev->host.channel[idx & (USB_OTG_MAX_TX_FIFOS-1U)] & HC_USED) == 0U)
        {
            //return idx;
            u16Ret = HC_OK;
            break;
        }
    }

    if(u16Ret == HC_OK)
    {
        u16Ret = idx;
    }
    else
    {
        //
    }

    return u16Ret;
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

/**
 * @}
 */

/**
* @}
*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
