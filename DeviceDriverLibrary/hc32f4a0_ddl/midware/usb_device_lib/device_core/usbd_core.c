/**
 *******************************************************************************
 * @file  usbd_core.c
 * @brief USBD core functions.
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
#include "usbd_core.h"
#include "usbd_req.h"
#include "usbd_ioreq.h"
#include "hc32f4a0_usb_dcd_int.h"
#include "usb_bsp.h"

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_DEVICE_LIB
 * @{
 */

/**
 * @addtogroup USB_DEVICE_CORE
 * @{
 */

/** @defgroup USBD_CORE USB device core
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
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_CORE_Local_Functions USBD Core Local Functions
 * @{
 */

static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev);
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev);
#ifdef VBUS_SENSING_ENABLED
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev);
#endif
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev);
static uint8_t USBD_RunTestMode (USB_OTG_CORE_HANDLE  *pdev) ;
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__IO USB_OTG_DCTL_TypeDef SET_TEST_MODE;
/**
 * @defgroup USBD_CORE_Local_Variables USBD Core Local Variables
 * @{
 */
USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb =
{
    &USBD_DataOutStage,
    &USBD_DataInStage,
    &USBD_SetupStage,
    &USBD_SOF,
    &USBD_Reset,
    &USBD_Suspend,
    &USBD_Resume,
    &USBD_IsoINIncomplete,
    &USBD_IsoOUTIncomplete,
#ifdef VBUS_SENSING_ENABLED
    &USBD_DevConnected,
    &USBD_DevDisconnected,
#endif
};
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @defgroup USBD_CORE_Global_Variables USBD Core Global Variables
 * @{
 */
USBD_DCD_INT_cb_TypeDef  *USBD_DCD_INT_fops = &USBD_DCD_INT_cb;
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_CORE_Global_Functions USBD Core Global Functions
 * @{
 */

/**
 * @brief  USBD_Init
 *         Initailizes the device stack and load the class driver
 * @param  [in] pdev: device instance
 * @param  [in] coreID: USB OTG core ID
 * @param  [in] pDevice: USB device
 * @param  [in] class_cb: Class callback structure address
 * @param  [in] usr_cb: User callback structure address
 * @retval None
 */
void USBD_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBD_DEVICE *pDevice,
               USBD_Class_cb_TypeDef *class_cb,
               USBD_Usr_cb_TypeDef *usr_cb)
{
    /* Hardware Init */
    USB_OTG_BSP_Init(pdev);
    USBD_DeInit(pdev);

    /*Register class and user callbacks */
    pdev->dev.class_cb = class_cb;
    pdev->dev.usr_cb = usr_cb;
    pdev->dev.usr_device = pDevice;

    /* set USB OTG core params */
    DCD_Init(pdev , coreID);

    /* Upon Init call usr callback */
    pdev->dev.usr_cb->Init();
    pdev->dev.device_state = USB_OTG_EP0_IDLE;

    /* config NVIC for usb interrupt */
    USB_OTG_BSP_EnableInterrupt();
}

/**
 * @brief  USBD_DeInit
 *         Re-Initialize th device library
 * @param  [in] pdev: device instance
 * @retval status: status
 */
USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev)
{
    /* Software Init */
    return USBD_OK;
}

/**
 * @brief  USBD_SetCfg
 *        Configure device and start the interface
 * @param  [in] pdev: device instance
 * @param  [in] cfgidx: configuration index
 * @retval status
 */
USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx)
{
    pdev->dev.class_cb->Init(pdev, cfgidx);

    /* Upon set config call usr call back */
    pdev->dev.usr_cb->DeviceConfigured();
    return USBD_OK;
}

/**
 * @brief  USBD_ClrCfg
 *         Clear current configuration
 * @param  [in] pdev: device instance
 * @param  [in] cfgidx: configuration index
 * @retval status: USBD_Status
 */
USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, uint8_t cfgidx)
{
    pdev->dev.class_cb->DeInit(pdev, cfgidx);
    return USBD_OK;
}

/**
 * @}
 */

/**
 * @addtogroup USBD_ORE_Local_Functions USBD Core Local Functions
 * @{
 */

/**
 * @brief  USBD_SetupStage
 *         Handle the setup stage
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev)
{
    USB_SETUP_REQ req;

    USBD_ParseSetupRequest(pdev , &req);

    switch (req.bmRequest & 0x1FU)
    {
        case USB_REQ_RECIPIENT_DEVICE:
            USBD_StdDevReq (pdev, &req);
            break;

        case USB_REQ_RECIPIENT_INTERFACE:
            USBD_StdItfReq(pdev, &req);
            break;

        case USB_REQ_RECIPIENT_ENDPOINT:
            USBD_StdEPReq(pdev, &req);
            break;

        default:
            DCD_EP_Stall(pdev , req.bmRequest & 0x80U);
            break;
    }
    return USBD_OK;
}

/**
 * @brief  USBD_DataOutStage
 *         Handle data out stage
 * @param  [in] pdev: device instance
 * @param  [in] epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
    USB_OTG_EP *ep;

    if(epnum == 0U)
    {
        ep = &pdev->dev.out_ep[0U];
        if ( pdev->dev.device_state == USB_OTG_EP0_DATA_OUT)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;

                if(pdev->cfg.dma_enable == 1U)
                {
                    /* in slave mode this, is handled by the RxSTSQLvl ISR */
                    ep->xfer_buff += ep->maxpacket;
                }
#if (DDL_PRINT_ENABLE == DDL_ON)
                //printf("ContinueRx\n");
#endif
                USBD_CtlContinueRx (pdev,
                ep->xfer_buff,
                (uint16_t)__MIN(ep->rem_data_len ,ep->maxpacket));
            }
            else
            {
#if (DDL_PRINT_ENABLE == DDL_ON)
                //printf("out end\n");
#endif
                if (ep->xfer_count > ep->rem_data_len)
                {
#if (DDL_PRINT_ENABLE == DDL_ON)
                    printf("%ld %ld\n",ep->xfer_count, ep->rem_data_len);
#endif
                }
                ep->rem_data_len = 0U;
                if((pdev->dev.class_cb->EP0_RxReady != NULL)&&
                    (pdev->dev.device_status == USB_OTG_CONFIGURED))
                {
                    pdev->dev.class_cb->EP0_RxReady(pdev);
                }
                USBD_CtlSendStatus(pdev);
            }
        }
    }
    else if((pdev->dev.class_cb->DataOut != NULL)&&(pdev->dev.device_status == USB_OTG_CONFIGURED))
    {
        pdev->dev.class_cb->DataOut(pdev, epnum);
    }
    else
    {
        //
    }
    return USBD_OK;
}

/**
 * @brief  USBD_DataInStage
 *         Handle data in stage
 * @param  [in] pdev: device instance
 * @param  [in] epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum)
{
    USB_OTG_EP *ep;

    if(epnum == 0U)
    {
        ep = &pdev->dev.in_ep[0U];
        if ( pdev->dev.device_state == USB_OTG_EP0_DATA_IN)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;
                if(pdev->cfg.dma_enable == 1U)
                {
                    /* in slave mode this, is handled by the TxFifoEmpty ISR */
                    ep->xfer_buff += ep->maxpacket;
                }
                USBD_CtlContinueSendData (pdev,
                              ep->xfer_buff,
                              (uint16_t)ep->rem_data_len);
            }
            else
            {
                /* last packet is MPS multiple, so send ZLP packet */
                if((ep->total_data_len % ep->maxpacket == 0U) &&
                    (ep->total_data_len >= ep->maxpacket) &&
                    (ep->total_data_len < ep->ctl_data_len ))
                {
                    USBD_CtlContinueSendData(pdev , NULL, 0U);
                    ep->ctl_data_len = 0U;
                }
                else
                {
                    if((pdev->dev.class_cb->EP0_TxSent != NULL)&&
                        (pdev->dev.device_status == USB_OTG_CONFIGURED))
                    {
                        pdev->dev.class_cb->EP0_TxSent(pdev);
                    }
                    USBD_CtlReceiveStatus(pdev);
                }
            }
        }
        if (pdev->dev.test_mode == 1U)
        {
            USBD_RunTestMode(pdev);
            pdev->dev.test_mode = 0U;
        }
        else
        {
        }
    }
    else if((pdev->dev.class_cb->DataIn != NULL)&&(pdev->dev.device_status == USB_OTG_CONFIGURED))
    {
        pdev->dev.class_cb->DataIn(pdev, epnum);
    }
    else
    {
        //
    }
    return USBD_OK;
}

/**
 * @brief  USBD_RunTestMode
 *         Launch test mode process
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t  USBD_RunTestMode (USB_OTG_CORE_HANDLE  *pdev)
{
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, SET_TEST_MODE.d32);
    return USBD_OK;
}

/**
 * @brief  USBD_Reset
 *         Handle Reset event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev)
{
    /* Open EP0 OUT */
    DCD_EP_Open(pdev,
                0x00U,
                USB_OTG_MAX_EP0_SIZE,
                EP_TYPE_CTRL);

    /* Open EP0 IN */
    DCD_EP_Open(pdev,
                0x80U,
                USB_OTG_MAX_EP0_SIZE,
                EP_TYPE_CTRL);

    /* Upon Reset call usr call back */
    pdev->dev.device_status = USB_OTG_DEFAULT;
    pdev->dev.usr_cb->DeviceReset(pdev->cfg.speed);

    return USBD_OK;
}

/**
 * @brief  USBD_Resume
 *         Handle Resume event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev)
{
    /* Upon Resume call usr call back */
    pdev->dev.usr_cb->DeviceResumed();
    pdev->dev.device_status = pdev->dev.device_old_status;
    pdev->dev.device_status = USB_OTG_CONFIGURED;
    return USBD_OK;
}

/**
 * @brief  USBD_Suspend
 *         Handle Suspend event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.device_old_status = pdev->dev.device_status;
    pdev->dev.device_status  = USB_OTG_SUSPENDED;
    /* Upon Resume call usr call back */
    pdev->dev.usr_cb->DeviceSuspended();
    return USBD_OK;
}

/**
 * @brief  USBD_SOF
 *         Handle SOF event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev)
{
    if(pdev->dev.class_cb->SOF)
    {
        pdev->dev.class_cb->SOF(pdev);
    }
    return USBD_OK;
}

/**
 * @brief  USBD_IsoINIncomplete
 *         Handle iso in incomplete event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.class_cb->IsoINIncomplete(pdev);
    return USBD_OK;
}

/**
 * @brief  USBD_IsoOUTIncomplete
 *         Handle iso out incomplete event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.class_cb->IsoOUTIncomplete(pdev);
    return USBD_OK;
}

#ifdef VBUS_SENSING_ENABLED
/**
 * @brief  USBD_DevConnected
 *         Handle device connection event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.usr_cb->DeviceConnected();
    pdev->dev.connection_status = 1U;
    return USBD_OK;
}

/**
 * @brief  USBD_DevDisconnected
 *         Handle device disconnection event
 * @param  [in] pdev: device instance
 * @retval status
 */
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev)
{
    pdev->dev.usr_cb->DeviceDisconnected();
    pdev->dev.class_cb->DeInit(pdev, 0U);
    pdev->dev.connection_status = 0U;
    return USBD_OK;
}
#endif /* VBUS_SENSING_ENABLED */

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
