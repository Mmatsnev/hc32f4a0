/**
 *******************************************************************************
 * @file  usbd_cdc_vcp_core.c
 * @brief This file provides the CDC VCP core functions.
 *
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       zhangxl         First version for USB CDC VCP demo.
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
#include "usbd_cdc_vcp_core.h"
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

/** @defgroup USBD_CDC_VCP_CORE USB CDC Device core
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
 * @defgroup USBD_CDC_VCP_CORE_Local_Functions USBD CDC VCP Core Local Functions
 * @{
 */

/* CDC specific management functions */
static void Handle_USBAsynchXfer  (void *pdev);
static uint8_t  *USBD_cdc_GetCfgDesc (uint8_t speed, uint16_t *length);
#ifdef USE_USB_OTG_HS
static uint8_t  *USBD_cdc_GetOtherCfgDesc (uint8_t speed, uint16_t *length);
#endif

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
extern CDC_IF_Prop_TypeDef  APP_FOPS;

/**
 * @defgroup USBD_CDC_VCP_CORE_Global_Variables USBD CDC VCP Core Global Variables
 * @{
 */

/* CDC interface class callbacks structure */
USBD_Class_cb_TypeDef  USBD_CDC_cb =
{
    &usbd_cdc_Init,
    &usbd_cdc_DeInit,
    &usbd_cdc_Setup,
    NULL,                 /* EP0_TxSent, */
    &usbd_cdc_EP0_RxReady,
    &usbd_cdc_DataIn,
    &usbd_cdc_DataOut,
    &usbd_cdc_SOF,
    NULL,
    NULL,
    &USBD_cdc_GetCfgDesc,
#ifdef USE_USB_OTG_HS
    &USBD_cdc_GetOtherCfgDesc, /* use same cobfig as per FS */
#endif /* USE_USB_OTG_HS  */
};

/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_CDC_VCP_CORE_Local_Variables USBD CDC VCP Core Local Variables
 * @{
 */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t usbd_cdc_CfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __USB_ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t usbd_cdc_OtherCfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __USB_ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
//__USB_ALIGN_BEGIN static __IO uint32_t  usbd_cdc_AltSet  __USB_ALIGN_END = 0UL;  /* MISRAC 2004*/
__USB_ALIGN_BEGIN static uint32_t  usbd_cdc_AltSet  __USB_ALIGN_END = 0UL;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t USB_Rx_Buffer   [CDC_DATA_MAX_PACKET_SIZE] __USB_ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
//__USB_ALIGN_BEGIN uint8_t APP_Rx_Buffer   [APP_RX_DATA_SIZE] __USB_ALIGN_END ;
uint8_t APP_Rx_Buffer   [APP_RX_DATA_SIZE];

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t CmdBuff[CDC_CMD_PACKET_SZE] __USB_ALIGN_END ;

uint32_t APP_Rx_ptr_in  = 0UL;
uint32_t APP_Rx_ptr_out = 0UL;
uint32_t APP_Rx_length  = 0UL;

uint8_t  USB_Tx_State = 0U;

static uint32_t cdcCmd = 0xFFUL;
static uint32_t cdcLen = 0UL;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB CDC device Configuration Descriptor */
__USB_ALIGN_BEGIN uint8_t usbd_cdc_CfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __USB_ALIGN_END =
{
    /*Configuration Descriptor*/
    0x09U,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    USB_CDC_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
    0x00U,
    0x02U,   /* bNumInterfaces: 2 interface */
    0x01U,   /* bConfigurationValue: Configuration value */
    0x00U,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0U,   /* bmAttributes: self powered */
    0x32U,   /* MaxPower 0 mA */

    /* IAD */
    0x08U,    /* bLength: Interface Descriptor size */
    0x0bU,    /* INTERFACE ASSOCIATION Descriptor */
    0x00U,    /* Interface number of the first interface that is associated with this function */
    0x02U,    /* Number of contiguous interfaces that are associated with this function */
    0x02U,    /* Class code: Communication Interface Class*/
    0x02U,    /* Subclass code: Abstract Control Model */
    0x01U,    /* protocol: Common AT commands */
    0x04U,
    /*---------------------------------------------------------------------------*/

    /*Interface Descriptor */
    0x09U,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00U,   /* bInterfaceNumber: Number of Interface */
    0x00U,   /* bAlternateSetting: Alternate setting */
    0x01U,   /* bNumEndpoints: One endpoints used */
    0x02U,   /* bInterfaceClass: Communication Interface Class */
    0x02U,   /* bInterfaceSubClass: Abstract Control Model */
    0x01U,   /* bInterfaceProtocol: Common AT commands */
    0x00U,   /* iInterface: */

    /*Header Functional Descriptor*/
    0x05U,   /* bLength: Endpoint Descriptor size */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x00U,   /* bDescriptorSubtype: Header Func Desc */
    0x10U,   /* bcdCDC: spec release number */
    0x01U,

    /*Call Management Functional Descriptor*/
    0x05U,   /* bFunctionLength */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x01U,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00U,   /* bmCapabilities: D0+D1 */
    0x01U,   /* bDataInterface: 1 */

    /*ACM Functional Descriptor*/
    0x04U,   /* bFunctionLength */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x02U,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02U,   /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05U,   /* bFunctionLength */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x06U,   /* bDescriptorSubtype: Union func desc */
    0x00U,   /* bMasterInterface: Communication class interface */
    0x01U,   /* bSlaveInterface0: Data Class Interface */

    /*Endpoint 2 Descriptor*/
    0x07U,                          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                     /* bEndpointAddress */
    0x03U,                          /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SZE),     /* wMaxPacketSize: */
    HIBYTE(CDC_CMD_PACKET_SZE),
#ifdef USE_USB_OTG_HS
    0x10U,                           /* bInterval: */
#else
    0xFFU,                           /* bInterval: */
#endif /* USE_USB_OTG_HS */

    /*---------------------------------------------------------------------------*/

    /*Data class interface descriptor*/
    0x09U,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    0x01U,   /* bInterfaceNumber: Number of Interface */
    0x00U,   /* bAlternateSetting: Alternate setting */
    0x02U,   /* bNumEndpoints: Two endpoints used */
    0x0AU,   /* bInterfaceClass: CDC */
    0x00U,   /* bInterfaceSubClass: */
    0x00U,   /* bInterfaceProtocol: */
    0x00U,   /* iInterface: */

    /*Endpoint OUT Descriptor*/
    0x07U,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                        /* bEndpointAddress */
    0x02U,                             /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
    0x00U,                              /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07U,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
    CDC_IN_EP,                         /* bEndpointAddress */
    0x02U,                             /* bmAttributes: Bulk */
    LOBYTE(CDC_DATA_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
    HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
    0x00U                              /* bInterval: ignore for Bulk transfer */
} ;

#ifdef USE_USB_OTG_HS
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t usbd_cdc_OtherCfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __USB_ALIGN_END =
{
    0x09U,   /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,
    USB_CDC_CONFIG_DESC_SIZ,
    0x00U,
    0x02U,   /* bNumInterfaces: 2 interfaces */
    0x01U,   /* bConfigurationValue: */
    0x04U,   /* iConfiguration: */
    0xC0U,   /* bmAttributes: */
    0x32U,   /* MaxPower 100 mA */

    /*Interface Descriptor */
    0x09U,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00U,   /* bInterfaceNumber: Number of Interface */
    0x00U,   /* bAlternateSetting: Alternate setting */
    0x01U,   /* bNumEndpoints: One endpoints used */
    0x02U,   /* bInterfaceClass: Communication Interface Class */
    0x02U,   /* bInterfaceSubClass: Abstract Control Model */
    0x01U,   /* bInterfaceProtocol: Common AT commands */
    0x00U,   /* iInterface: */

    /*Header Functional Descriptor*/
    0x05U,   /* bLength: Endpoint Descriptor size */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x00U,   /* bDescriptorSubtype: Header Func Desc */
    0x10U,   /* bcdCDC: spec release number */
    0x01U,

    /*Call Management Functional Descriptor*/
    0x05U,   /* bFunctionLength */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x01U,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00U,   /* bmCapabilities: D0+D1 */
    0x01U,   /* bDataInterface: 1 */

    /*ACM Functional Descriptor*/
    0x04U,   /* bFunctionLength */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x02U,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02U,   /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05U,   /* bFunctionLength */
    0x24U,   /* bDescriptorType: CS_INTERFACE */
    0x06U,   /* bDescriptorSubtype: Union func desc */
    0x00U,   /* bMasterInterface: Communication class interface */
    0x01U,   /* bSlaveInterface0: Data Class Interface */

    /*Endpoint 2 Descriptor*/
    0x07U,                          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                     /* bEndpointAddress */
    0x03U,                          /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SZE),     /* wMaxPacketSize: */
    HIBYTE(CDC_CMD_PACKET_SZE),
    0xFFU,                          /* bInterval: */

    /*---------------------------------------------------------------------------*/

    /*Data class interface descriptor*/
    0x09U,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    0x01U,   /* bInterfaceNumber: Number of Interface */
    0x00U,   /* bAlternateSetting: Alternate setting */
    0x02U,   /* bNumEndpoints: Two endpoints used */
    0x0AU,   /* bInterfaceClass: CDC */
    0x00U,   /* bInterfaceSubClass: */
    0x00U,   /* bInterfaceProtocol: */
    0x00U,   /* iInterface: */

    /*Endpoint OUT Descriptor*/
    0x07U,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                        /* bEndpointAddress */
    0x02U,                             /* bmAttributes: Bulk */
    0x40U,                             /* wMaxPacketSize: */
    0x00U,
    0x00U,                             /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07U,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: Endpoint */
    CDC_IN_EP,                        /* bEndpointAddress */
    0x02U,                            /* bmAttributes: Bulk */
    0x40U,                            /* wMaxPacketSize: */
    0x00U,
    0x00U                             /* bInterval */
};
#endif /* USE_USB_OTG_HS  */

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup USBD_CDC_VCP_CORE_Global_Functions USBD CDC VCP Core Global Functions
 * @{
 */

/**
 * @brief  usbd_cdc_Init
 *         Initialize the CDC interface
 * @param  [in] pdev: device instance
 * @param  [in] cfgidx: Configuration index
 * @retval status
 */
uint8_t usbd_cdc_Init (void  *pdev, uint8_t cfgidx)
{
    uint8_t *pbuf;

    /* Open EP IN */
    DCD_EP_Open(pdev,
                CDC_IN_EP,
                CDC_DATA_IN_PACKET_SIZE,
                USB_OTG_EP_BULK);

    /* Open EP OUT */
    DCD_EP_Open(pdev,
                CDC_OUT_EP,
                CDC_DATA_OUT_PACKET_SIZE,
                USB_OTG_EP_BULK);

    /* Open Command IN EP */
    DCD_EP_Open(pdev,
                CDC_CMD_EP,
                CDC_CMD_PACKET_SZE,
                USB_OTG_EP_INT);

    pbuf = (uint8_t *)USBD_DeviceDesc;
    pbuf[4U] = DEVICE_CLASS_CDC;
    pbuf[5U] = DEVICE_SUBCLASS_CDC;

    /* Initialize the Interface physical components */
    APP_FOPS.pIf_Init();

    /* Prepare Out endpoint to receive next packet */
    DCD_EP_PrepareRx(pdev,
                     CDC_OUT_EP,
                     (uint8_t*)(USB_Rx_Buffer),
                     CDC_DATA_OUT_PACKET_SIZE);

    return USBD_OK;
}

/**
 * @brief  usbd_cdc_Init
 *         DeInitialize the CDC layer
 * @param  [in] pdev: device instance
 * @param  [in] cfgidx: Configuration index
 * @retval status
 */
uint8_t  usbd_cdc_DeInit (void  *pdev, uint8_t cfgidx)
{
    /* Open EP IN */
    DCD_EP_Close(pdev, CDC_IN_EP);

    /* Open EP OUT */
    DCD_EP_Close(pdev, CDC_OUT_EP);

    /* Open Command IN EP */
    DCD_EP_Close(pdev, CDC_CMD_EP);

    /* Restore default state of the Interface physical components */
    APP_FOPS.pIf_DeInit();

    return USBD_OK;
}

/**
 * @brief  usbd_cdc_Setup
 *         Handle the CDC specific requests
 * @param  [in] pdev: instance
 * @param  [in] req: usb requests
 * @retval status
 */
uint8_t  usbd_cdc_Setup (void  *pdev,
                                USB_SETUP_REQ *req)
{
    uint16_t len=USB_CDC_DESC_SIZ;
    uint8_t  *pbuf=usbd_cdc_CfgDesc + 9U;
    uint8_t u8Res = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        /* CDC Class Requests -------------------------------*/
        case USB_REQ_TYPE_CLASS :
            /* Check if the request is a data setup packet */
            if (req->wLength)
            {
                /* Check if the request is Device-to-Host */
                if (req->bmRequest & 0x80U)
                {
                    /* Get the data to be sent to Host from interface layer */
                    APP_FOPS.pIf_Ctrl(req->bRequest, CmdBuff, req->wLength);

                    /* Send the data to the host */
                    USBD_CtlSendData (pdev,
                                      CmdBuff,
                                      req->wLength);
                }
                else /* Host-to-Device requeset */
                {
                    /* Set the value of the current command to be processed */
                    cdcCmd = req->bRequest;
                    cdcLen = req->wLength;

                    /* Prepare the reception of the buffer over EP0
                    Next step: the received data will be managed in usbd_cdc_EP0_TxSent()
                    function. */
                    USBD_CtlPrepareRx (pdev,
                                       CmdBuff,
                                       req->wLength);
                }
            }
            else /* No Data request */
            {
                /* Transfer the command to the interface layer */
                APP_FOPS.pIf_Ctrl(req->bRequest, NULL, 0U);
            }
            break;
        /* Standard Requests -------------------------------*/
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR:
                    if( (req->wValue >> 8U) == CDC_DESCRIPTOR_TYPE)
                    {
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
                        //pbuf = usbd_cdc_Desc;
                        pbuf = usbd_cdc_CfgDesc + 9U + (9U * USBD_ITF_MAX_NUM);
#else
                        pbuf = usbd_cdc_CfgDesc + 9U + (9U * USBD_ITF_MAX_NUM);
#endif
                        len = __MIN(USB_CDC_DESC_SIZ , req->wLength);
                    }
                    USBD_CtlSendData (pdev,
                                      pbuf,
                                      len);
                    break;

                case USB_REQ_GET_INTERFACE :
                    USBD_CtlSendData (pdev,
                                  (uint8_t *)&usbd_cdc_AltSet,
                                  1U);
                    break;

                case USB_REQ_SET_INTERFACE :
                    if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
                    {
                        usbd_cdc_AltSet = (uint8_t)(req->wValue);
                    }
                    else
                    {
                        /* Call the error management function (command will be nacked */
                        USBD_CtlError (pdev);
                    }
                    break;
                default:
                    break;
            }
            break;

        default:
            USBD_CtlError (pdev);
            u8Res = USBD_FAIL;
            break;
    }
    return u8Res;
}

/**
 * @brief  usbd_cdc_EP0_RxReady
 *         Data received on control endpoint
 * @param  [in] pdev: device device instance
 * @retval status
 */
uint8_t usbd_cdc_EP0_RxReady (void *pdev)
{
    if (cdcCmd != NO_CMD)
    {
        /* Process the data */
        APP_FOPS.pIf_Ctrl(cdcCmd, CmdBuff, cdcLen);

        /* Reset the command variable to default value */
        cdcCmd = NO_CMD;
    }
    return USBD_OK;
}

/**
 * @brief  usbd_audio_DataIn
 *         Data sent on non-control IN endpoint
 * @param  [in] pdev: device instance
 * @param  [in] epnum: endpoint number
 * @retval status
 */
uint8_t usbd_cdc_DataIn (void *pdev, uint8_t epnum)
{
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length;

    if (USB_Tx_State == 1U)
    {
        if (APP_Rx_length == 0U)
        {
            USB_Tx_State = 0U;
        }
        else
        {
            if (APP_Rx_length > CDC_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_ptr = (uint16_t)APP_Rx_ptr_out;
                USB_Tx_length = (uint16_t)CDC_DATA_IN_PACKET_SIZE;
                APP_Rx_ptr_out += CDC_DATA_IN_PACKET_SIZE;
                APP_Rx_length -= CDC_DATA_IN_PACKET_SIZE;
            }
            else
            {
                USB_Tx_ptr = (uint16_t)APP_Rx_ptr_out;
                USB_Tx_length = (uint16_t)APP_Rx_length;
                APP_Rx_ptr_out += APP_Rx_length;
                APP_Rx_length = 0U;
            }
            /* Prepare the available data buffer to be sent on IN endpoint */
            DCD_EP_Tx (pdev,
                       CDC_IN_EP,
                       (uint8_t*)&APP_Rx_Buffer[USB_Tx_ptr],
                       (uint32_t)USB_Tx_length);
        }
    }
    return USBD_OK;
}

/**
 * @brief  usbd_cdc_DataOut
 *         Data received on non-control Out endpoint
 * @param  [in] pdev: device instance
 * @param  [in] epnum: endpoint number
 * @retval status
 */
uint8_t  usbd_cdc_DataOut (void *pdev, uint8_t epnum)
{
    uint16_t USB_Rx_Cnt;

    /* Get the received data buffer and update the counter */
    USB_Rx_Cnt = (uint16_t)((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;

    /* USB data will be immediately processed, this allow next USB traffic being
      NAKed till the end of the application Xfer */
    APP_FOPS.pIf_DataRx(USB_Rx_Buffer, USB_Rx_Cnt);

    /* Prepare Out endpoint to receive next packet */
    DCD_EP_PrepareRx(pdev,
                    CDC_OUT_EP,
                    (uint8_t*)(USB_Rx_Buffer),
                    CDC_DATA_OUT_PACKET_SIZE);

    return USBD_OK;
}

/**
 * @brief  usbd_audio_SOF
 *         Start Of Frame event management
 * @param  [in] pdev: instance
 * @retval status
 */
uint8_t  usbd_cdc_SOF (void *pdev)
{
    static uint32_t FrameCount = 0UL;

    if (FrameCount++ == CDC_IN_FRAME_INTERVAL)
    {
        /* Reset the frame counter */
        FrameCount = 0UL;

        /* Check the data to be sent through IN pipe */
        Handle_USBAsynchXfer(pdev);
    }
    return USBD_OK;
}

/**
 * @}
 */

/**
 * @addtogroup USBD_CDC_VCP_CORE_Local_Functions USBD CDC VCP Core Local Functions
 * @{
 */

/**
 * @brief  Handle_USBAsynchXfer
 *         Send data to USB
 * @param  [in] pdev: instance
 * @retval None
 */
static void Handle_USBAsynchXfer (void *pdev)
{
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length;

    if(USB_Tx_State != 1U)
    {
        if (APP_Rx_ptr_out == APP_RX_DATA_SIZE)
        {
            APP_Rx_ptr_out = 0UL;
        }
        if(APP_Rx_ptr_out == APP_Rx_ptr_in)
        {
            USB_Tx_State = 0U;
            //return;
        }
        else
        {
            if(APP_Rx_ptr_out > APP_Rx_ptr_in) /* rollback */
            {
                APP_Rx_length = APP_RX_DATA_SIZE - APP_Rx_ptr_out;
            }
            else
            {
                APP_Rx_length = APP_Rx_ptr_in - APP_Rx_ptr_out;
            }
    #ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
            APP_Rx_length &= ~0x03U;
    #endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

            if (APP_Rx_length > CDC_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_ptr = (uint16_t)APP_Rx_ptr_out;
                USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;

                APP_Rx_ptr_out += CDC_DATA_IN_PACKET_SIZE;
                APP_Rx_length -= CDC_DATA_IN_PACKET_SIZE;
            }
            else
            {
                USB_Tx_ptr = (uint16_t)APP_Rx_ptr_out;
                USB_Tx_length = (uint16_t)APP_Rx_length;

                APP_Rx_ptr_out += APP_Rx_length;
                APP_Rx_length = 0U;
            }
            USB_Tx_State = 1U;

            DCD_EP_Tx (pdev,
                      CDC_IN_EP,
                      (uint8_t*)&APP_Rx_Buffer[USB_Tx_ptr],
                      (uint32_t)USB_Tx_length);
        }
    }
}

/**
 * @brief  USBD_cdc_GetCfgDesc
 *         Return configuration descriptor
 * @param  [in] speed : current device speed
 * @param  [in] length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t  *USBD_cdc_GetCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof (usbd_cdc_CfgDesc);
    return usbd_cdc_CfgDesc;
}

/**
 * @brief  USBD_cdc_GetCfgDesc
 *         Return configuration descriptor
 * @param  [in] speed : current device speed
 * @param  [in] length : pointer data length
 * @retval pointer to descriptor buffer
 */
#ifdef USE_USB_OTG_HS
static uint8_t  *USBD_cdc_GetOtherCfgDesc (uint8_t speed, uint16_t *length)
{
    *length = sizeof (usbd_cdc_OtherCfgDesc);
    return usbd_cdc_OtherCfgDesc;
}
#endif /* USE_USB_OTG_HS */

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
