/**
 *******************************************************************************
 * @file  usbd_hid_mouse_core.c
 * @brief USB Device HID Mouse Function Core Layer.
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
#include "usbd_hid_mouse_core.h"
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

/** @defgroup USBD_HID_MOUSE_CORE
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
 * @defgroup USBD_HID_MOUSE_CORE_Local_Functions USBD HID Mouse Core Local Functions
 * @{
 */

static uint8_t  USBD_HID_Init (void  *pdev,
                               uint8_t cfgidx);
static uint8_t  USBD_HID_DeInit (void  *pdev,
                                 uint8_t cfgidx);
static uint8_t  USBD_HID_Setup (void  *pdev,
                                USB_SETUP_REQ *req);
static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length);
static uint8_t  USBD_HID_DataIn (void  *pdev, uint8_t epnum);
static uint8_t  USBD_HID_DataOut (void  *pdev, uint8_t epnum);
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @defgroup USBD_HID_MOUSE_CORE_Global_Variables USBD HID Mouse Core Global Variables
 * @{
 */
USBD_Class_cb_TypeDef  USBD_HID_cb =
{
    &USBD_HID_Init,
    &USBD_HID_DeInit,
    &USBD_HID_Setup,
    NULL, /*EP0_TxSent*/
    NULL, /*EP0_RxReady*/
    &USBD_HID_DataIn, /*DataIn*/
    &USBD_HID_DataOut, /*DataOut*/
    NULL, /*SOF */
    NULL,
    NULL,
    &USBD_HID_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
    &USBD_HID_GetCfgDesc, /* use same config as per FS */
#endif
};
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_HID_MOUSE_CORE_Local_Variables USBD HID Mouse Core Local Variables
 * @{
 */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN static uint32_t  USBD_HID_AltSet  __USB_ALIGN_END = 0U;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN static uint32_t  USBD_HID_Protocol  __USB_ALIGN_END = 0U;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN static uint32_t  USBD_HID_IdleState __USB_ALIGN_END = 0U;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB HID device Configuration Descriptor */
__USB_ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __USB_ALIGN_END =
{
    0x09U, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    USB_HID_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00U,
    0x01U,         /*bNumInterfaces: 1 interface*/
    0x01U,         /*bConfigurationValue: Configuration value*/
    0x00U,         /*iConfiguration: Index of string descriptor describing
    the configuration*/
    (uint8_t)0x80U | (uint8_t)SELF_POWERED,         /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32U,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09U,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00U,         /*bInterfaceNumber: Number of Interface*/
    0x00U,         /*bAlternateSetting: Alternate setting*/
    0x02U,         /*bNumEndpoints*/
    0x03U,         /*bInterfaceClass: HID*/
    0x01U,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x02U,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0X00U,         /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09U,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11U,         /*bcdHID: HID Class Spec release number*/
    0x01U,
    0x00U,         /*bCountryCode: Hardware target country*/
    0x01U,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22U,         /*bDescriptorType*/
    HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
    0x00U,
    /******************** Descriptor of Mouse endpoint ********************/
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

    HID_OUT_EP,    /*bEndpointAddress: Endpoint Address (IN)*/
    0x03U,         /*bmAttributes: Interrupt endpoint*/
    HID_OUT_PACKET,/*wMaxPacketSize: 4 Byte max */
    0x00U,
    0x0AU,         /*bInterval: Polling Interval (10 ms)*/
    /* 41 */
} ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
/* USB HID device Configuration Descriptor */
__USB_ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __USB_ALIGN_END=
{
    /* 18 */
    0x09U,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11U,         /*bcdHID: HID Class Spec release number*/
    0x01U,
    0x00U,         /*bCountryCode: Hardware target country*/
    0x01U,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22U,         /*bDescriptorType*/
    HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
    0x00U,
};
#endif


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] __USB_ALIGN_END =
{
    0x05U,   0x01U,
    0x09U,   0x02U,
    0xA1U,   0x01U,
    0x09U,   0x01U,

    0xA1U,   0x00U,
    0x05U,   0x09U,
    0x19U,   0x01U,
    0x29U,   0x03U,

    0x15U,   0x00U,
    0x25U,   0x01U,
    0x95U,   0x03U,
    0x75U,   0x01U,

    0x81U,   0x02U,
    0x95U,   0x01U,
    0x75U,   0x05U,
    0x81U,   0x01U,

    0x05U,   0x01U,
    0x09U,   0x30U,
    0x09U,   0x31U,
    0x09U,   0x38U,

    0x15U,   0x81U,
    0x25U,   0x7FU,
    0x75U,   0x08U,
    0x95U,   0x03U,

    0x81U,   0x06U,
    0xC0U,   0x09U,
    0x3cU,   0x05U,
    0xffU,   0x09U,

    0x01U,   0x15U,
    0x00U,   0x25U,
    0x01U,   0x75U,
    0x01U,   0x95U,

    0x02U,   0xb1U,
    0x22U,   0x75U,
    0x06U,   0x95U,
    0x01U,   0xb1U,

    0x01U,   0xc0U
};

static uint8_t u8HidRevBuf[4U];

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_HID_MOUSE_CORE_Global_Functions USBD HID Mouse Core Global Functions
 * @{
 */

/**
 * @brief  USBD_HID_Init
 *         Initialize the HID interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
uint8_t  USBD_HID_Init (void  *pdev,
                               uint8_t cfgidx)
{
    /* Open EP IN */
    DCD_EP_Open(pdev,
                HID_IN_EP,
                HID_IN_PACKET,
                USB_OTG_EP_INT);

    /* Open EP OUT */
    DCD_EP_Open(pdev,
                HID_OUT_EP,
                HID_OUT_PACKET,
                USB_OTG_EP_INT);

    /* config EP2 for OUT */
    DCD_EP_PrepareRx(pdev,
                     HID_OUT_EP,
                     u8HidRevBuf,
                     (uint16_t)(sizeof(u8HidRevBuf)/sizeof(uint8_t)));
    DCD_SetEPStatus (pdev , HID_OUT_EP , USB_OTG_EP_RX_VALID);

    return USBD_OK;
}

/**
 * @brief  USBD_HID_Init
 **         DeInitialize the HID layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
uint8_t  USBD_HID_DeInit (void  *pdev,
                                 uint8_t cfgidx)
{
    /* Close HID EPs */
    DCD_EP_Close (pdev , HID_IN_EP);
    DCD_EP_Close (pdev , HID_OUT_EP);
    return USBD_OK;
}

/**
 * @brief  USBD_HID_Setup
 *         Handle the HID specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
uint8_t  USBD_HID_Setup (void  *pdev,
                                USB_SETUP_REQ *req)
{
    uint16_t len = 0U;
    uint8_t  *pbuf = NULL;
    uint8_t u8Res = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        case USB_REQ_TYPE_CLASS :
            switch (req->bRequest)
            {
                case HID_REQ_SET_PROTOCOL:
                    USBD_HID_Protocol = (uint8_t)(req->wValue);
                    break;
                case HID_REQ_GET_PROTOCOL:
                    USBD_CtlSendData (pdev,
                    (uint8_t *)&USBD_HID_Protocol,
                    1U);
                    break;
                case HID_REQ_SET_IDLE:
                    USBD_HID_IdleState = (uint8_t)(req->wValue >> 8U);
                    break;
                case HID_REQ_GET_IDLE:
                    USBD_CtlSendData (pdev,
                                    (uint8_t *)&USBD_HID_IdleState,
                                    1U);
                    break;
                default:
                    USBD_CtlError (pdev);
                    u8Res = USBD_FAIL;
                    break;
            }
            break;
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR:
                    if((req->wValue >> 8U) == (uint16_t)HID_REPORT_DESC)
                    {
                        len = (uint16_t)__MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
                        pbuf = HID_MOUSE_ReportDesc;
                    }
                    else if((req->wValue >> 8U) == (uint16_t)HID_DESCRIPTOR_TYPE)
                    {
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
                        pbuf = USBD_HID_Desc;
#else
                        pbuf = USBD_HID_CfgDesc + 0x12U;
#endif
                        len = (uint16_t)__MIN(USB_HID_DESC_SIZ , req->wLength);
                    }
                    else
                    {
                        //
                    }
                    USBD_CtlSendData (pdev,
                                        pbuf,
                                        len);
                    break;
                case USB_REQ_GET_INTERFACE :
                    USBD_CtlSendData (pdev,
                                        (uint8_t *)&USBD_HID_AltSet,
                                        1U);
                    break;
                case USB_REQ_SET_INTERFACE :
                    USBD_HID_AltSet = (uint8_t)(req->wValue);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return u8Res;
}

/**
 * @brief  USBD_HID_SendReport
 *         Send HID Report
 * @param  pdev: device instance
 * @param  buff: pointer to report
 * @retval status
 */
uint8_t USBD_HID_SendReport     (USB_OTG_CORE_HANDLE  *pdev,
                                 uint8_t *report,
                                 uint16_t len)
{
    if (pdev->dev.device_status == USB_OTG_CONFIGURED )
    {
        DCD_EP_Tx (pdev, HID_IN_EP, report, (uint32_t)len);
    }
    return USBD_OK;
}

/**
 * @brief  USBD_HID_GetCfgDesc
 *         return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof (USBD_HID_CfgDesc);
    return USBD_HID_CfgDesc;
}

/**
 * @brief  USBD_HID_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
uint8_t  USBD_HID_DataIn (void  *pdev,
                              uint8_t epnum)
{
    /* Ensure that the FIFO is empty before a new transfer, this condition could
    be caused by  a new transfer before the end of the previous transfer */
    DCD_EP_Flush(pdev, HID_IN_EP);
    return USBD_OK;
}

uint8_t  USBD_HID_DataOut (void  *pdev,
                              uint8_t epnum)
{

    DCD_EP_PrepareRx(pdev,
                     HID_OUT_EP,
                     u8HidRevBuf,
                     (uint16_t)(sizeof(u8HidRevBuf)/sizeof(uint8_t)));
    DCD_SetEPStatus (pdev , HID_OUT_EP , USB_OTG_EP_RX_VALID);

    return USBD_OK;
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
