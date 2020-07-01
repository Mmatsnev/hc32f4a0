/**
 *******************************************************************************
 * @file  usbd_req.c
 * @brief Standard USB requests following chapter 9.
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
#include "usbd_req.h"
#include "usbd_ioreq.h"
#include "usbd_desc.h"

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

/** @defgroup USBD_REQ
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
/**
 * @defgroup USBD_REQ_Global_Variables USBD Request Global Variables
 * @{
 */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint32_t USBD_ep_status __USB_ALIGN_END  = 0U;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint32_t  USBD_default_cfg __USB_ALIGN_END  = 0U;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint32_t  USBD_cfg_status __USB_ALIGN_END  = 0U;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ] __USB_ALIGN_END;

/**
 * @}
 */

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_REQ_Local_Functions USBD Core Local Functions
 * @{
 */
static void USBD_GetDescriptor(USB_OTG_CORE_HANDLE  *pdev,
                               const USB_SETUP_REQ *req);
static void USBD_SetAddress(USB_OTG_CORE_HANDLE  *pdev,
                            const USB_SETUP_REQ *req);
static void USBD_SetConfig(USB_OTG_CORE_HANDLE  *pdev,
                           const USB_SETUP_REQ *req);
static void USBD_GetConfig(USB_OTG_CORE_HANDLE  *pdev,
                           const USB_SETUP_REQ *req);
static void USBD_GetStatus(USB_OTG_CORE_HANDLE  *pdev,
                           const USB_SETUP_REQ *req);
static void USBD_GetIntf(USB_OTG_CORE_HANDLE  *pdev);
static void USBD_SetFeature(USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req);
static void USBD_ClrFeature(USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req);
static uint8_t USBD_GetLen(uint8_t *buf);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
extern __IO USB_OTG_DCTL_TypeDef SET_TEST_MODE;

/**
 * @defgroup USBD_REQ_Global_Functions USBD Request Global Functions
 * @{
 */

/**
 * @brief  USBD_StdDevReq
 *         Handle standard usb device requests
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
USBD_Status  USBD_StdDevReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req)
{
    USBD_Status ret = USBD_OK;
    if (req->bRequest & 0x80U)
    {
        pdev->dev.ep0_state = USB_OTG_EP0_DATA_IN;
    }
    else
    {
        pdev->dev.ep0_state = USB_OTG_EP0_DATA_OUT;
    }

    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:
        USBD_GetDescriptor (pdev, req) ;
        break;
    case USB_REQ_SET_ADDRESS:
        USBD_SetAddress(pdev, req);
        break;
    case USB_REQ_SET_CONFIGURATION:
        USBD_SetConfig (pdev , req);
        break;
    case USB_REQ_GET_CONFIGURATION:
        USBD_GetConfig (pdev , req);
        break;
    case USB_REQ_GET_STATUS:
        USBD_GetStatus (pdev , req);
        break;
    case USB_REQ_SET_FEATURE:
        USBD_SetFeature (pdev , req);
        break;
    case USB_REQ_CLEAR_FEATURE:
        USBD_ClrFeature (pdev , req);
        break;
    default:
        if (pdev->dev.class_cb->Setup (pdev, req))
        {
            USBD_CtlError(pdev);
        }
        break;
    }
    return ret;
}

/**
 * @brief  USBD_StdItfReq
 *         Handle standard usb interface requests
 * @param  pdev: USB OTG device instance
 * @param  req: usb request
 * @retval status
 */
USBD_Status  USBD_StdItfReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req)
{
    USBD_Status ret = USBD_OK;
    uint8_t u8RetFlag = 0U;
    if (req->bRequest == USB_REQ_GET_STATUS)
    {
        USBD_GetStatus (pdev , req);
        //return ret;
        u8RetFlag = 1U;
    }
    else if (req->bRequest == USB_REQ_GET_INTERFACE)
    {
        //USBD_CtlError(pdev);
        if (req->bmRequest != 0x21U)
        {
            USBD_GetIntf (pdev);
            //return ret;
            u8RetFlag = 1U;
        }
    }
    else if (req->bRequest == 0xfeU)
    {
        //USBD_CtlError(pdev);
        USBD_GetIntf (pdev);
        //return ret;
        u8RetFlag = 1U;
    }
    else
    {
        //
    }

    if(1U != u8RetFlag)
    {
        switch (pdev->dev.device_status)
        {
            case USB_OTG_CONFIGURED:
                if (LOBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
                {
                    pdev->dev.class_cb->Setup (pdev, req);
                    //if((req->wLength == 0U)&& (ret == USBD_OK)) /* MISRAC2004*/
                    if(req->wLength == 0U)
                    {
                        USBD_CtlSendStatus(pdev);
                    }
                }
                else
                {
                    USBD_CtlError(pdev);
                }
                break;

            default:
                USBD_CtlError(pdev);
                break;
        }
    }
    return ret;
}

/**
 * @brief  USBD_StdEPReq
 *         Handle standard usb endpoint requests
 * @param  pdev: USB OTG device instance
 * @param  req: usb request
 * @retval status
 */
USBD_Status  USBD_StdEPReq (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_REQ  *req)
{
    uint8_t   ep_addr;
    USBD_Status ret = USBD_OK;

    ep_addr  = LOBYTE(req->wIndex);

    switch (req->bRequest)
    {
        case USB_REQ_SET_FEATURE :
        switch (pdev->dev.device_status)
            {
                case USB_OTG_ADDRESSED:
                    if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                    {
                    DCD_EP_Stall(pdev , ep_addr);
                    }
                    break;
                case USB_OTG_CONFIGURED:
                    if (req->wValue == USB_FEATURE_EP_HALT)
                    {
                        if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                        {
                            DCD_EP_Stall(pdev , ep_addr);
                        }
                    }
                    pdev->dev.class_cb->Setup (pdev, req);
                    USBD_CtlSendStatus(pdev);
                    break;
                default:
                    USBD_CtlError(pdev);
                    break;
            }
            break;
        case USB_REQ_CLEAR_FEATURE :
            switch (pdev->dev.device_status)
            {
                case USB_OTG_ADDRESSED:
                    if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                    {
                        DCD_EP_Stall(pdev , ep_addr);
                    }
                    break;
                case USB_OTG_CONFIGURED:
                    if (req->wValue == USB_FEATURE_EP_HALT)
                    {
                        if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                        {
                            DCD_EP_ClrStall(pdev , ep_addr);
                            pdev->dev.class_cb->Setup (pdev, req);
                        }
                        USBD_CtlSendStatus(pdev);
                    }
                    break;
                default:
                    USBD_CtlError(pdev);
                    break;
            }
            break;
        case USB_REQ_GET_STATUS:
            switch (pdev->dev.device_status)
            {
                case USB_OTG_ADDRESSED:
                    if ((ep_addr != 0x00U) && (ep_addr != 0x80U))
                    {
                    DCD_EP_Stall(pdev , ep_addr);
                    }
                    break;
                case USB_OTG_CONFIGURED:
                    if ((ep_addr & 0x80U)== 0x80U)
                    {
                        if(pdev->dev.in_ep[ep_addr % USB_OTG_MAX_TX_FIFOS].is_stall)
                        {
                            USBD_ep_status = 0x0001U;
                        }
                        else
                        {
                            USBD_ep_status = 0x0000U;
                        }
                    }
                    else if ((ep_addr & 0x80U)== 0x00U)
                    {
                        if(pdev->dev.out_ep[ep_addr % USB_OTG_MAX_TX_FIFOS].is_stall)
                        {
                            USBD_ep_status = 0x0001U;
                        }
                        else
                        {
                            USBD_ep_status = 0x0000U;
                        }
                    }
                    else
                    {
                        //
                    }
                    USBD_CtlSendData (pdev,(uint8_t *)&USBD_ep_status, 2U);
                    break;

                default:
                    USBD_CtlError(pdev);
                    break;
            }
            break;
        default:
        break;
    }
    return ret;
}

/**
 * @brief  USBD_ParseSetupRequest
 *         Copy buffer into setup structure
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval None
 */
void USBD_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req)
{
    req->bmRequest     = *(uint8_t *)  (pdev->dev.setup_packet);
    req->bRequest      = *(uint8_t *)  (pdev->dev.setup_packet +  1U);
    req->wValue        = SWAPBYTE      (pdev->dev.setup_packet +  2U);
    req->wIndex        = SWAPBYTE      (pdev->dev.setup_packet +  4U);
    req->wLength       = SWAPBYTE      (pdev->dev.setup_packet +  6U);

    pdev->dev.in_ep[0].ctl_data_len = req->wLength  ;
    pdev->dev.device_state = USB_OTG_EP0_SETUP;
}

/**
 * @brief  USBD_CtlError
 *         Handle USB low level Error
 * @param  pdev: device instance
 * @retval None
 */
void USBD_CtlError( USB_OTG_CORE_HANDLE  *pdev)  /* MISRAC 2004*/
{
    DCD_EP_Stall(pdev , 0x80U);
    DCD_EP_Stall(pdev , 0U);
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("ctl error\n");
#endif
    USB_OTG_EP0_OutStart(pdev);
}


/**
 * @brief  USBD_GetString
 *         Convert Ascii string into unicode one
 * @param  desc : descriptor buffer
 * @param  unicode : Formatted string buffer (unicode)
 * @param  len : descriptor length
 * @retval None
 */
void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t idx = 0U;

    if (desc != NULL)
    {
        *len =  (uint16_t)USBD_GetLen(desc) * 2U + 2U;
        unicode[idx++] = (uint8_t)*len;
        unicode[idx++] =  USB_DESC_TYPE_STRING;

        while (*desc != (uint8_t)0U)
        {
            unicode[idx++] = *desc++;
            unicode[idx++] =  0x00U;  /* MISRAC 2004*/
        }
    }
}

/**
 * @}
 */

/**
 * @addtogroup USBD_REQ_Local_Functions USBD Request Local Functions
 * @{
 */

/**
 * @brief  USBD_GetDescriptor
 *         Handle Get Descriptor requests
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_GetDescriptor(USB_OTG_CORE_HANDLE  *pdev,
                               const USB_SETUP_REQ *req)  /* MISRAC 2004 16.7*/
{
    uint16_t len = 0U;
    uint8_t *pbuf = NULL;
    uint8_t u8ErrFlag = 0U;

    switch (req->wValue >> 8U)
    {
        case USB_DESC_TYPE_DEVICE:
            pbuf = pdev->dev.usr_device->GetDeviceDescriptor(pdev->cfg.speed, &len);
            if ((req->wLength == 64U) ||( pdev->dev.device_status == USB_OTG_DEFAULT))
            {
                len = 8U;
            }
            break;
        case USB_DESC_TYPE_CONFIGURATION:
            pbuf   = (uint8_t *)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len);
#ifdef USB_OTG_HS_CORE
            if((pdev->cfg.speed == USB_OTG_SPEED_FULL )&&(pdev->cfg.phy_itface  == USB_OTG_ULPI_PHY))
            {
                pbuf = (uint8_t *)pdev->dev.class_cb->GetOtherConfigDescriptor(pdev->cfg.speed, &len);
            }
#endif
            pbuf[1U] = USB_DESC_TYPE_CONFIGURATION;
            pdev->dev.pConfig_descriptor = pbuf;
            break;
        case USB_DESC_TYPE_STRING:
            switch ((uint8_t)(req->wValue))
            {
                case USBD_IDX_LANGID_STR:
                    pbuf = pdev->dev.usr_device->GetLangIDStrDescriptor(pdev->cfg.speed, &len);
                    break;
                case USBD_IDX_MFC_STR:
                    pbuf = pdev->dev.usr_device->GetManufacturerStrDescriptor(pdev->cfg.speed, &len);
                    break;
                case USBD_IDX_PRODUCT_STR:
                    pbuf = pdev->dev.usr_device->GetProductStrDescriptor(pdev->cfg.speed, &len);
                    break;
                case USBD_IDX_SERIAL_STR:
                    pbuf = pdev->dev.usr_device->GetSerialStrDescriptor(pdev->cfg.speed, &len);
                    break;
                case USBD_IDX_CONFIG_STR:
                    pbuf = pdev->dev.usr_device->GetConfigurationStrDescriptor(pdev->cfg.speed, &len);
                    break;
                case USBD_IDX_INTERFACE_STR:
                    pbuf = pdev->dev.usr_device->GetInterfaceStrDescriptor(pdev->cfg.speed, &len);
                    break;
                default:
#ifdef USB_SUPPORT_USER_STRING_DESC
                    pbuf = pdev->dev.class_cb->GetUsrStrDescriptor(pdev->cfg.speed, (req->wValue) , &len);
                    break;
#else
                    USBD_CtlError(pdev);
                    u8ErrFlag = 1U;
                    break;
#endif /* USBD_CtlError(pdev); */
                }
                break;
        case USB_DESC_TYPE_DEVICE_QUALIFIER:
#ifdef USB_OTG_HS_CORE
            if(pdev->cfg.speed == USB_OTG_SPEED_HIGH  )
            {
                pbuf   = (uint8_t *)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len);

                USBD_DeviceQualifierDesc[4]= pbuf[14U];
                USBD_DeviceQualifierDesc[5]= pbuf[15U];
                USBD_DeviceQualifierDesc[6]= pbuf[16U];

                pbuf = USBD_DeviceQualifierDesc;
                len  = USB_LEN_DEV_QUALIFIER_DESC;
                break;
            }
            else
            {
                USBD_CtlError(pdev);
                u8ErrFlag = 1U;
                break;
            }
#else
            USBD_CtlError(pdev);
            u8ErrFlag = 1U;
            break;
#endif
        case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
#ifdef USB_OTG_HS_CORE
            if(pdev->cfg.speed == USB_OTG_SPEED_HIGH  )
            {
                pbuf = (uint8_t *)pdev->dev.class_cb->GetOtherConfigDescriptor(pdev->cfg.speed, &len);
                pbuf[1U] = USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION;
                break;
            }
            else
            {
                USBD_CtlError(pdev);
                u8ErrFlag = 1U;
                break;
            }
#else
            USBD_CtlError(pdev);
            u8ErrFlag = 1U;
            break;
#endif
        default:
            USBD_CtlError(pdev);
            u8ErrFlag = 1U;
            break;
    }

    if(0U == u8ErrFlag)
    {
        if((len != 0U)&& (req->wLength != 0U))
        {
            len = __MIN(len , req->wLength);
            USBD_CtlSendData (pdev,
                                pbuf,
                                len);
        }
    }
}

/**
 * @brief  USBD_SetAddress
 *         Set device address
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_SetAddress(USB_OTG_CORE_HANDLE  *pdev,
                            const USB_SETUP_REQ *req)/* MISRAC 2004 16.7*/
{
    uint8_t  dev_addr;

    if ((req->wIndex == 0U) && (req->wLength == 0U))
    {
        dev_addr = (uint8_t)(req->wValue) & 0x7FU;

        if (pdev->dev.device_status == USB_OTG_CONFIGURED)
        {
            USBD_CtlError(pdev);
        }
        else
        {
            pdev->dev.device_address = dev_addr;
            DCD_EP_SetAddress(pdev, dev_addr);
            USBD_CtlSendStatus(pdev);

            if (dev_addr != 0U)
            {
                pdev->dev.device_status  = USB_OTG_ADDRESSED;
            }
            else
            {
                pdev->dev.device_status  = USB_OTG_DEFAULT;
            }
        }
    }
    else
    {
        USBD_CtlError(pdev);
    }
}

/**
 * @brief  USBD_SetConfig
 *         Handle Set device configuration request
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_SetConfig(USB_OTG_CORE_HANDLE  *pdev,
                           const USB_SETUP_REQ *req)/* MISRAC 2004 16.7*/
{
    static uint8_t  cfgidx;

    cfgidx = (uint8_t)(req->wValue);

    if (cfgidx > USBD_CFG_MAX_NUM )
    {
        USBD_CtlError(pdev);
    }
    else
    {
        switch (pdev->dev.device_status)
        {
            case USB_OTG_ADDRESSED:
                if (cfgidx)
                {
                    pdev->dev.device_config = cfgidx;
                    pdev->dev.device_status = USB_OTG_CONFIGURED;
                    USBD_SetCfg(pdev , cfgidx);
                    USBD_CtlSendStatus(pdev);
                }
                else
                {
                    USBD_CtlSendStatus(pdev);
                }
                break;
            case USB_OTG_CONFIGURED:
                if (cfgidx == 0U)
                {
                    pdev->dev.device_status = USB_OTG_ADDRESSED;
                    pdev->dev.device_config = cfgidx;
                    USBD_ClrCfg(pdev , cfgidx);
                    USBD_CtlSendStatus(pdev);
                }
                else if(cfgidx != pdev->dev.device_config)
                {
                    /* Clear old configuration */
                    USBD_ClrCfg(pdev , pdev->dev.device_config);
                    /* set new configuration */
                    pdev->dev.device_config = cfgidx;
                    USBD_SetCfg(pdev , cfgidx);
                    USBD_CtlSendStatus(pdev);
                }
                else
                {
                    USBD_CtlSendStatus(pdev);
                }
                break;
            case USB_OTG_SUSPENDED:
#if (DDL_PRINT_ENABLE == DDL_ON)
                printf("set cfg fail,suspended\n");
#endif
                break;
            default:
                USBD_CtlError(pdev);
                break;
        }
    }
}

/**
 * @brief  USBD_GetConfig
 *         Handle Get device configuration request
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_GetConfig(USB_OTG_CORE_HANDLE  *pdev,
                           const USB_SETUP_REQ *req)/* MISRAC 2004 16.7*/
{
    if (req->wLength != 1U)
    {
        USBD_CtlError(pdev);
    }
    else
    {
        switch (pdev->dev.device_status )
        {
            case USB_OTG_ADDRESSED:
                USBD_CtlSendData (pdev,
                                    (uint8_t *)&USBD_default_cfg,
                                    1U);
                break;
            case USB_OTG_CONFIGURED:
                USBD_CtlSendData (pdev,
                                    &pdev->dev.device_config,
                                    1U);
                break;
            default:
                USBD_CtlError(pdev);
                break;
        }
    }
}

/**
 * @brief  USBD_GetStatus
 *         Handle Get Status request
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_GetStatus(USB_OTG_CORE_HANDLE  *pdev,
                           const USB_SETUP_REQ *req)/* MISRAC 2004 16.7*/
{
    switch (pdev->dev.device_status)
    {
        case USB_OTG_ADDRESSED:
        case USB_OTG_CONFIGURED:
#ifdef USBD_SELF_POWERED
            USBD_cfg_status = USB_CONFIG_SELF_POWERED;
#else
            USBD_cfg_status = 0x00U;
#endif
            if (pdev->dev.DevRemoteWakeup)
            {
            USBD_cfg_status |= USB_CONFIG_REMOTE_WAKEUP;
            }
            USBD_CtlSendData (pdev,
                                (uint8_t *)&USBD_cfg_status,
                                2U);
            break;
        default :
            USBD_CtlError(pdev);
            break;
    }
}

static void USBD_GetIntf(USB_OTG_CORE_HANDLE  *pdev)  /* MISRAC2004*/
{
    USBD_cfg_status = 0U;
    USBD_CtlSendData (pdev,
                        (uint8_t *)&USBD_cfg_status,
                        1U);
}


/**
 * @brief  USBD_SetFeature
 *         Handle Set device feature request
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_SetFeature(USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req)
{
    USB_OTG_DCTL_TypeDef     dctl;
    uint8_t test_mode = 0U;

    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
    {
        pdev->dev.DevRemoteWakeup = 1U;
        pdev->dev.class_cb->Setup (pdev, req);
        USBD_CtlSendStatus(pdev);
    }
    else if ((req->wValue == USB_FEATURE_TEST_MODE) && ((req->wIndex & 0xFFU) == 0U))
    {
        dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);

        test_mode = (uint8_t)req->wIndex >> 8U;
        switch (test_mode)
        {
            case 1U: // TEST_J
                dctl.b.tstctl = 1U;
                break;
            case 2U: // TEST_K
                dctl.b.tstctl = 2U;
                break;
            case 3U: // TEST_SE0_NAK
                dctl.b.tstctl = 3U;
                break;
            case 4U: // TEST_PACKET
                dctl.b.tstctl = 4U;
                break;
            case 5U: // TEST_FORCE_ENABLE
                dctl.b.tstctl = 5U;
                break;
        }
        SET_TEST_MODE = dctl;
        pdev->dev.test_mode = 1U;
        USBD_CtlSendStatus(pdev);
    }
    else
    {
        //
    }
}


/**
 * @brief  USBD_ClrFeature
 *         Handle clear device feature request
 * @param  pdev: device instance
 * @param  req: usb request
 * @retval status
 */
static void USBD_ClrFeature(USB_OTG_CORE_HANDLE  *pdev,
                            USB_SETUP_REQ *req)
{
    switch (pdev->dev.device_status)
    {
        case USB_OTG_ADDRESSED:
        case USB_OTG_CONFIGURED:
            if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
            {
            pdev->dev.DevRemoteWakeup = 0U;
            pdev->dev.class_cb->Setup (pdev, req);
            USBD_CtlSendStatus(pdev);
            }
            break;
        default :
            USBD_CtlError(pdev);
            break;
    }
}

/**
 * @brief  USBD_GetLen
 *         return the string length
 * @param  buf : pointer to the ascii string buffer
 * @retval string length
 */
static uint8_t USBD_GetLen(uint8_t *buf)
{
    uint8_t  len = 0U;

    while (*buf != (uint8_t)0U)
    {
        len++;
        buf++;
    }

    return len;
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
