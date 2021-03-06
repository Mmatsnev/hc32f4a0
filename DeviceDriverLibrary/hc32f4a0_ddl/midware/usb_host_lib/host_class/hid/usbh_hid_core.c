/**
 *******************************************************************************
 * @file  usbh_hid_core.c
 * @brief HID Layer Handlers for USB Host HID class.
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
#include "usbh_hid_core.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_HOST_LIB
 * @{
 */

/**
 * @addtogroup USB_HOST_CLASS
 * @{
 */

/** @defgroup USBH_HID USB host HID
 * @{
 */

#if (DDL_USBFS_ENABLE == DDL_ON)

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_Local_Functions USBH HID Local Functions
 * @{
 */
static USBH_Status USBH_HID_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev, void *phost);

static void  USBH_ParseHIDDesc (USBH_HIDDesc_TypeDef *desc, uint8_t *buf);

static void USBH_HID_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev, void *phost);

static USBH_Status USBH_HID_Handle(USB_OTG_CORE_HANDLE *pdev, void *phost);

static USBH_Status USBH_HID_ClassRequest(USB_OTG_CORE_HANDLE *pdev, void *phost);

static USBH_Status USBH_Get_HID_ReportDescriptor (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint16_t length);

static USBH_Status USBH_Get_HID_Descriptor (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint16_t length);

static USBH_Status USBH_Set_Idle (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint8_t duration, uint8_t reportId);

static USBH_Status USBH_Set_Protocol (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint8_t protocol);
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_Global_Variables USBH HID Global Variables
 * @{
 */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN HID_Machine_TypeDef        HID_Machine __USB_ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN HID_Report_TypeDef         HID_Report __USB_ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN USB_Setup_TypeDef          HID_Setup __USB_ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN USBH_HIDDesc_TypeDef       HID_Desc __USB_ALIGN_END ;

__IO uint8_t start_toggle = 0U;

USBH_Class_cb_TypeDef  USBH_HID_cb =
{
    &USBH_HID_InterfaceInit,
    &USBH_HID_InterfaceDeInit,
    &USBH_HID_ClassRequest,
    &USBH_HID_Handle
};

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @defgroup USBH_HID_Global_Functions USBH HID Global Functions
 * @{
 */

/**
* @brief  USBH_HID_InterfaceDeInit
*         The function DeInit the Host Channels used for the HID class.
* @param  pdev: Instance for USB core
* @param  phost: Host
* @retval None
*/
void USBH_HID_InterfaceDeInit ( USB_OTG_CORE_HANDLE *pdev, void *phost)
{
    //USBH_HOST *pphost = phost;

    if(HID_Machine.hc_num_in != 0x00U)
    {
        USB_OTG_HC_Halt(pdev, HID_Machine.hc_num_in);
        USBH_Free_Channel  (pdev, HID_Machine.hc_num_in);
        HID_Machine.hc_num_in = 0U;     /* Reset the Channel as Free */
    }

    if(HID_Machine.hc_num_out != 0x00U)
    {
        USB_OTG_HC_Halt(pdev, HID_Machine.hc_num_out);
        USBH_Free_Channel  (pdev, HID_Machine.hc_num_out);
        HID_Machine.hc_num_out = 0U;     /* Reset the Channel as Free */
    }

    start_toggle = 0U;
}

/**
* @brief  USBH_Set_Report
*         Issues Set Report
* @param  pdev: Instance for USB core
* @param  phost: Host
* @param  reportType  : Report type to be sent
* @param  reportId    : Targetted report ID for Set Report request
* @param  reportLen   : Length of data report to be send
* @param  reportBuff  : Report Buffer
* @retval USBH_Status : Response for USB Set Idle request
*/
USBH_Status USBH_Set_Report (USB_OTG_CORE_HANDLE *pdev,
                                    USBH_HOST *phost,
                                    uint8_t reportType,
                                    uint8_t reportId,
                                    uint8_t reportLen,
                                    uint8_t* reportBuff)
{
    phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
    phost->Control.setup.b.bRequest = USB_HID_SET_REPORT;
    phost->Control.setup.b.wValue.w = (reportType << 8U ) | reportId;

    phost->Control.setup.b.wIndex.w = 0U;
    phost->Control.setup.b.wLength.w = reportLen;

    return USBH_CtlReq(pdev, phost, reportBuff , reportLen );
}

/**
 * @}
 */

/**
 * @defgroup USBH_HID_Local_Functions USBH HID Local Functions
 * @{
 */

/**
* @brief  USBH_HID_InterfaceInit
*         The function init the HID class.
* @param  pdev: Instance for USB core
* @param  phost: Host
* @retval  USBH_Status :Response for USB HID driver initialize
*/
static USBH_Status USBH_HID_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev,
                                           void *phost)
{
    uint8_t maxEP;
    USBH_HOST *pphost = phost;

    uint8_t num =0U;
    USBH_Status status = USBH_BUSY ;
    HID_Machine.state = HID_ERROR;


    if(pphost->device_prop.Itf_Desc[0U].bInterfaceSubClass  == HID_BOOT_CODE)
    {
        /*Decode Bootclass Protocl: Mouse or Keyboard*/
        if(pphost->device_prop.Itf_Desc[0U].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE)
        {
            HID_Machine.cb = &HID_KEYBRD_cb;
        }
        else if(pphost->device_prop.Itf_Desc[0U].bInterfaceProtocol  == HID_MOUSE_BOOT_CODE)
        {
            HID_Machine.cb = &HID_MOUSE_cb;
        }

        HID_Machine.state     = HID_IDLE;
        HID_Machine.ctl_state = HID_REQ_IDLE;
        HID_Machine.ep_addr   = pphost->device_prop.Ep_Desc[0U][0U].bEndpointAddress;
        HID_Machine.length    = pphost->device_prop.Ep_Desc[0U][0U].wMaxPacketSize;
        HID_Machine.poll      = pphost->device_prop.Ep_Desc[0U][0U].bInterval ;

        if (HID_Machine.poll  < HID_MIN_POLL)
        {
            HID_Machine.poll = HID_MIN_POLL;
        }


        /* Check fo available number of endpoints */
        /* Find the number of EPs in the Interface Descriptor */
        /* Choose the lower number in order not to overrun the buffer allocated */
        maxEP = ( (pphost->device_prop.Itf_Desc[0].bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ?
                pphost->device_prop.Itf_Desc[0].bNumEndpoints :
                    USBH_MAX_NUM_ENDPOINTS);

        /* Decode endpoint IN and OUT address from interface descriptor */
        for (num=0; num < maxEP; num++)
        {
            if(pphost->device_prop.Ep_Desc[0][num].bEndpointAddress & 0x80U)
            {
                HID_Machine.HIDIntInEp = (pphost->device_prop.Ep_Desc[0][num].bEndpointAddress);
                HID_Machine.hc_num_in  =\
                      USBH_Alloc_Channel(pdev,
                                          pphost->device_prop.Ep_Desc[0][num].bEndpointAddress);

                /* Open channel for IN endpoint */
                USBH_Open_Channel  (pdev,
                                    HID_Machine.hc_num_in,
                                    pphost->device_prop.address,
                                    pphost->device_prop.speed,
                                    EP_TYPE_INTR,
                                    HID_Machine.length);
            }
            else
            {
                HID_Machine.HIDIntOutEp = (pphost->device_prop.Ep_Desc[0U][num].bEndpointAddress);
                HID_Machine.hc_num_out  =\
                        USBH_Alloc_Channel(pdev,
                                          pphost->device_prop.Ep_Desc[0U][num].bEndpointAddress);

                /* Open channel for OUT endpoint */
                USBH_Open_Channel  (pdev,
                                    HID_Machine.hc_num_out,
                                    pphost->device_prop.address,
                                    pphost->device_prop.speed,
                                    EP_TYPE_INTR,
                                    HID_Machine.length);
            }
        }
        start_toggle = 0U;
        status = USBH_OK;
    }
    else
    {
        pphost->usr_cb->DeviceNotSupported();
    }

    return status;
}

/**
* @brief  USBH_HID_ClassRequest
*         The function is responsible for handling HID Class requests
*         for HID class.
* @param  pdev: Instance for USB core
* @param  phost: Host
* @retval  USBH_Status :Response for USB Set Protocol request
*/
static USBH_Status USBH_HID_ClassRequest(USB_OTG_CORE_HANDLE *pdev ,
                                         void *phost)
{
    USBH_HOST *pphost = phost;

    USBH_Status status         = USBH_BUSY;
    USBH_Status classReqStatus = USBH_BUSY;

    /* Switch HID state machine */
    switch (HID_Machine.ctl_state)
    {
        case HID_IDLE:
        case HID_REQ_GET_HID_DESC:

            /* Get HID Desc */
            if (USBH_Get_HID_Descriptor (pdev, pphost, USB_HID_DESC_SIZE)== USBH_OK)
            {

              USBH_ParseHIDDesc(&HID_Desc, pdev->host.Rx_Buffer);
              HID_Machine.ctl_state = HID_REQ_GET_REPORT_DESC;
            }
            break;
        case HID_REQ_GET_REPORT_DESC:
            /* Get Report Desc */
            if (USBH_Get_HID_ReportDescriptor(pdev , pphost, HID_Desc.wItemLength) == USBH_OK)
            {
              HID_Machine.ctl_state = HID_REQ_SET_IDLE;
            }
            break;
        case HID_REQ_SET_IDLE:
            classReqStatus = USBH_Set_Idle (pdev, pphost, 0U, 0U);
            /* set Idle */
            if (classReqStatus == USBH_OK)
            {
                HID_Machine.ctl_state = HID_REQ_SET_PROTOCOL;
            }
            else if(classReqStatus == USBH_NOT_SUPPORTED)
            {
                HID_Machine.ctl_state = HID_REQ_SET_PROTOCOL;
            }
            break;
        case HID_REQ_SET_PROTOCOL:
            /* set protocol */
            if (USBH_Set_Protocol (pdev ,pphost, 0U) == USBH_OK)
            {
                HID_Machine.ctl_state = HID_REQ_IDLE;

                /* all requests performed*/
                status = USBH_OK;
            }
            break;
        default:
            break;
    }
    return status;
}


/**
* @brief  USBH_HID_Handle
*         The function is for managing state machine for HID data transfers
* @param  pdev: Instance for USB core
* @param  phost: Host
* @retval USBH_Status
*/
static USBH_Status USBH_HID_Handle(USB_OTG_CORE_HANDLE *pdev , void *phost)
{
    USBH_HOST *pphost = phost;
    USBH_Status status = USBH_OK;

    switch (HID_Machine.state)
    {
        case HID_IDLE:
          HID_Machine.cb->Init();
          HID_Machine.state = HID_SYNC;

        case HID_SYNC:

          /* Sync with start of Even Frame */
          if(USB_OTG_IsEvenFrame(pdev) == TRUE)
          {
              HID_Machine.state = HID_GET_DATA;
          }
          break;

        case HID_GET_DATA:
            USBH_InterruptReceiveData(pdev,
                              HID_Machine.buff,
                              HID_Machine.length,
                              HID_Machine.hc_num_in);
            start_toggle = 1U;
            HID_Machine.state = HID_POLL;
            HID_Machine.timer = HCD_GetCurrentFrame(pdev);
            break;
        case HID_POLL:
            if(( HCD_GetCurrentFrame(pdev) - HID_Machine.timer) >= HID_Machine.poll)
            {
                HID_Machine.state = HID_GET_DATA;
            }
            else if(HCD_GetURB_State(pdev , HID_Machine.hc_num_in) == URB_DONE)
            {
                if(start_toggle == 1U) /* handle data once */
                {
                    start_toggle = 0U;
                    HID_Machine.cb->Decode(HID_Machine.buff);
                }
            }
            else if(HCD_GetURB_State(pdev, HID_Machine.hc_num_in) == URB_STALL) /* IN Endpoint Stalled */
            {

                /* Issue Clear Feature on interrupt IN endpoint */
                if( (USBH_ClrFeature(pdev,
                                    pphost,
                                    HID_Machine.ep_addr,
                                    HID_Machine.hc_num_in)) == USBH_OK)
                {
                    /* Change state to issue next IN token */
                    HID_Machine.state = HID_GET_DATA;
                }

            }
            break;
        default:
            break;
    }
    return status;
}


/**
* @brief  USBH_Get_HID_ReportDescriptor
*         Issue report Descriptor command to the device. Once the response
*         received, parse the report descriptor and update the status.
* @param  pdev : Instance for USB core
* @param  phost : Host
* @param  length : HID Report Descriptor Length
* @retval USBH_Status : Response for USB HID Get Report Descriptor Request
*/
static USBH_Status USBH_Get_HID_ReportDescriptor (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint16_t length)
{
    USBH_Status status;

    status = USBH_GetDescriptor(pdev,
                                phost,
                                USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,
                                USB_DESC_HID_REPORT,
                                pdev->host.Rx_Buffer,
                                length);

    /* HID report descriptor is available in pdev->host.Rx_Buffer.
    In case of USB Boot Mode devices for In report handling ,
    HID report descriptor parsing is not required.
    In case, for supporting Non-Boot Protocol devices and output reports,
    user may parse the report descriptor*/
    return status;
}

/**
* @brief  USBH_Get_HID_Descriptor
*         Issue HID Descriptor command to the device. Once the response
*         received, parse the report descriptor and update the status.
* @param  pdev   : Instance for USB core
* @param  phost   : Host
* @param  length : HID Descriptor Length
* @retval USBH_Status : Response for USB HID Get Report Descriptor Request
*/
static USBH_Status USBH_Get_HID_Descriptor (USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint16_t length)
{
    USBH_Status status;

    status = USBH_GetDescriptor(pdev,
                                phost,
                                USB_REQ_RECIPIENT_INTERFACE
                                  | USB_REQ_TYPE_STANDARD,
                                  USB_DESC_HID,
                                  pdev->host.Rx_Buffer,
                                  length);

    return status;
}

/**
* @brief  USBH_Set_Idle
*         Set Idle State.
* @param  pdev: Instance for USB core
* @param  phost: Host
* @param  duration: Duration for HID Idle request
* @param  reportId : Targetted report ID for Set Idle request
* @retval USBH_Status : Response for USB Set Idle request
*/
static USBH_Status USBH_Set_Idle (USB_OTG_CORE_HANDLE *pdev,
                                  USBH_HOST *phost,
                                  uint8_t duration,
                                  uint8_t reportId)
{
    phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;

    phost->Control.setup.b.bRequest = USB_HID_SET_IDLE;
    phost->Control.setup.b.wValue.w = (duration << 8 ) | reportId;

    phost->Control.setup.b.wIndex.w = 0U;
    phost->Control.setup.b.wLength.w = 0U;

    return USBH_CtlReq(pdev, phost, 0U, 0U );
}



/**
* @brief  USBH_Set_Protocol
*         Set protocol State.
* @param  pdev: Instance for USB core
* @param  phost: Host
* @param  protocol : Set Protocol for HID : boot/report protocol
* @retval USBH_Status : Response for USB Set Protocol request
*/
static USBH_Status USBH_Set_Protocol(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost, uint8_t protocol)
{
    phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;

    phost->Control.setup.b.bRequest = USB_HID_SET_PROTOCOL;

    if(protocol != 0U)
    {
        /* Boot Protocol */
        phost->Control.setup.b.wValue.w = 0U;
    }
    else
    {
        /*Report Protocol*/
        phost->Control.setup.b.wValue.w = 1U;
    }

    phost->Control.setup.b.wIndex.w = 0U;
    phost->Control.setup.b.wLength.w = 0U;

    return USBH_CtlReq(pdev, phost, 0U, 0U);

}

/**
* @brief  USBH_ParseHIDDesc
*         This function Parse the HID descriptor
* @param  desc: Descriptor structure
* @param  buf: Buffer where the source descriptor is available
* @retval None
*/
static void  USBH_ParseHIDDesc (USBH_HIDDesc_TypeDef *desc, uint8_t *buf)
{
    desc->bLength                  = *(uint8_t  *) (buf + 0U);
    desc->bDescriptorType          = *(uint8_t  *) (buf + 1U);
    desc->bcdHID                   =  LE16  (buf + 2U);
    desc->bCountryCode             = *(uint8_t  *) (buf + 4U);
    desc->bNumDescriptors          = *(uint8_t  *) (buf + 5U);
    desc->bReportDescriptorType    = *(uint8_t  *) (buf + 6U);
    desc->wItemLength              =  LE16  (buf + 7U);
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
