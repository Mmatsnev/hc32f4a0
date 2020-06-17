/**
 *******************************************************************************
 * @file  usbd_hid_msc_wrapper.c
 * @brief Composite HID_MSC
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
#include "usbd_hid_custom_core.h"
#include "usbd_msc_core.h"
#include "usbd_hid_msc_wrapper.h"
#include "usbd_desc.h"
#include "usbd_req.h"

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_DEVICE_LIB
 * @{
 */

/**
 * @addtogroup USB_DEVICE_CLASS
 * @{
 */

/** @defgroup USBD_HID_MSC_WRAPPER
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
 * @defgroup USBD_HID_MSC_WRAPPER_Local_Functions USBD HID MSC Wrapper Local Functions
 * @{
 */
static uint8_t USBD_MSC_HID_Init (void *pdev, uint8_t cfgidx);

static uint8_t USBD_MSC_HID_DeInit (void *pdev, uint8_t cfgidx);

static uint8_t USBD_MSC_HID_Setup (void *pdev, USB_SETUP_REQ *req);

static uint8_t *USBD_MSC_HID_GetCfgDesc (uint8_t speed, uint16_t *length);

static uint8_t USBD_MSC_HID_DataIn (void *pdev, uint8_t epnum);
static uint8_t USBD_MSC_HID_DataOut(void *pdev , uint8_t epnum);
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @defgroup USBD_HID_MSC_WRAPPER_Global_Variables USBD HID MSC Wrapper Global Variables
 * @{
 */
USBD_Class_cb_TypeDef  USBD_MSC_HID_cb =
{
    &USBD_MSC_HID_Init,
    &USBD_MSC_HID_DeInit,
    &USBD_MSC_HID_Setup,
    NULL, /*EP0_TxSent*/
    NULL, /*EP0_RxReady*/
    &USBD_MSC_HID_DataIn, /*DataIn*/
    &USBD_MSC_HID_DataOut, /*DataOut*/
    NULL, /*SOF */
    NULL,
    NULL,
    &USBD_MSC_HID_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
    &USBD_MSC_HID_GetCfgDesc, /* use same config as per FS */
#endif
};
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_HID_MSC_WRAPPER_Local_Variables USBD HID MSC Wrapper Local Variables
 * @{
 */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB HID device Configuration Descriptor */
__USB_ALIGN_BEGIN static uint8_t USBD_MSC_HID_CfgDesc[USB_MSC_HID_CONFIG_DESC_SIZ] __USB_ALIGN_END =
{
    0x09U, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    USB_MSC_HID_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00U,
    0x02U,         /*bNumInterfaces: 2 interface*/
    0x01U,         /*bConfigurationValue: Configuration value*/
    0x00U,         /*iConfiguration: Index of string descriptor describing
    the configuration*/
    0xE0U,         /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32U,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of HID interface ****************/
    /* 09 */
    0x09U,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    HID_INTERFACE,         /*bInterfaceNumber: Number of Interface*/
    0x00U,         /*bAlternateSetting: Alternate setting*/
    0x02U,         /*bNumEndpoints*/
    0x03U,         /*bInterfaceClass: HID*/
    0x01U,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x02U,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0X00U,         /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 18 */
    0x09U,         /*bLength: HID Descriptor size*/
    CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11U,         /*bcdHID: HID Class Spec release number*/
    0x01U,
    0x00U,         /*bCountryCode: Hardware target country*/
    0x01U,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22U,         /*bDescriptorType*/
    CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
    0x00U,
    /******************** Descriptor of HID endpoint ********************/
    /* 27 */
    0x07U,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    HID_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03U,         /*bmAttributes: Interrupt endpoint*/
    HID_IN_PACKET, /*wMaxPacketSize: 4 Byte max */
    0x00U,
    0x0AU,         /*bInterval: Polling Interval (10 ms)*/
    /* 34 */
    0x07U,         /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    HID_OUT_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03U,          /*bmAttributes: Interrupt endpoint*/
    HID_OUT_PACKET, /*wMaxPacketSize: 4 Byte max */
    0x00U,
    0x0AU,          /*bInterval: Polling Interval (10 ms)*/
    /* 41 */

    /********************  Mass Storage interface ********************/
    0x09U,   /* bLength: Interface Descriptor size */
    0x04U,   /* bDescriptorType: */
    MSC_INTERFACE,   /* bInterfaceNumber: Number of Interface */
    0x00U,   /* bAlternateSetting: Alternate setting */
    0x02U,   /* bNumEndpoints*/
    0x08U,   /* bInterfaceClass: MSC Class */
    0x06U,   /* bInterfaceSubClass : SCSI transparent*/
    0x50U,   /* nInterfaceProtocol */
    0x05U,          /* iInterface: */
    /********************  Mass Storage Endpoints ********************/
    0x07U,   /*Endpoint descriptor length = 7*/
    0x05U,   /*Endpoint descriptor type */
    MSC_IN_EP,   /*Endpoint address (IN, address 1) */
    0x02U,   /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00U,   /*Polling interval in milliseconds */

    0x07U,   /*Endpoint descriptor length = 7 */
    0x05U,   /*Endpoint descriptor type */
    MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
    0x02U,   /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00U    /*Polling interval in milliseconds*/
} ;

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @addtogroup USBD_HID_MSC_WRAPPER_Local_Functions USBD HID MSC Wrapper Local Functions
 * @{
 */

/**
  * @brief  USBD_MSC_HID_Init
  *         Initialize the MSC-HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_Init (void  *pdev, uint8_t cfgidx)
{
    /* HID initialization */
    USBD_CUSTOM_HID_Init (pdev,cfgidx);

    /* MSC initialization */
    USBD_MSC_Init (pdev,cfgidx);

    return USBD_OK;
}

/**
  * @brief  USBD_MSC_HID_DeInit
  *         DeInitialize the MSC_HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_DeInit (void  *pdev, uint8_t cfgidx)
{
    /* HID De-initialization */
    USBD_CUSTOM_HID_DeInit (pdev,cfgidx);

    /* MSC De-initialization */
    USBD_MSC_DeInit (pdev,cfgidx);

    return USBD_OK;
}

/**
  * @brief  USBD_MSC_HID_Setup
  *         Handle the MSC_HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_MSC_HID_Setup (void  *pdev, USB_SETUP_REQ *req)
{
    uint8_t u8Res = USBD_OK;
    switch (req->bmRequest & USB_REQ_RECIPIENT_MASK)
    {
        case USB_REQ_RECIPIENT_INTERFACE:
            if (req->wIndex == HID_INTERFACE)
            {
              u8Res = USBD_CUSTOM_HID_Setup (pdev, req);
            }
            else
            {
              u8Res = USBD_MSC_Setup(pdev, req);
            }
            break;

        case USB_REQ_RECIPIENT_ENDPOINT:
            if (req->wIndex == HID_IN_EP)
            {
              u8Res = USBD_CUSTOM_HID_Setup (pdev, req);
            }
            else
            {
              u8Res = USBD_MSC_Setup(pdev, req);
            }
            break;
        default:
            break;
    }
    return u8Res;
}

/**
  * @brief  USBD_MSC_HID_GetCfgDesc
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MSC_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof (USBD_MSC_HID_CfgDesc);
    return USBD_MSC_HID_CfgDesc;
}

/**
  * @brief  USBD_MSC_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_DataIn (void  *pdev, uint8_t epnum)
{
    uint8_t u8Ret = 0U;
    /*DataIN can be for MSC or HID */
    if (epnum == (uint8_t)(MSC_IN_EP&((uint8_t)~0x80U)))
    {
        u8Ret = USBD_MSC_DataIn(pdev, epnum);
    }
    else
    {
        u8Ret = USBD_CUSTOM_HID_DataIn(pdev, epnum);
    }
    return u8Ret;
}

/**
  * @brief  USBD_MSC_HID_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MSC_HID_DataOut(void *pdev , uint8_t epnum)
{
    uint8_t u8Ret = 0U;
    /*DataOut can be for MSC*/
    if (epnum == (uint8_t)(MSC_OUT_EP&((uint8_t)~0x80U)))
    {
        u8Ret = USBD_MSC_DataOut(pdev, epnum);
    }
    else
    {
        /*DataOut can be for HID */
        u8Ret = USBD_CUSTOM_HID_DataOut(pdev, epnum);
    }
    return u8Ret;
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
