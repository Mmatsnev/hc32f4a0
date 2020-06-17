/**
 *******************************************************************************
 * @file  usbh_hid_core.h
 * @brief Header for the usbh_hid_core.c file
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
#ifndef __USBH_HID_CORE_H__
#define __USBH_HID_CORE_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbh_core.h"
#include "usbh_stdreq.h"
#include "usb_bsp.h"
#include "usbh_ioreq.h"
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
 * @addtogroup USB_HOST_CLASS
 * @{
 */

/** @addtogroup USBH_HID_CORE
 * @{
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_CORE_Global_Macros USBH HID Core Global Macros
 * @{
 */

#define USB_HID_REQ_GET_REPORT       (0x01U)
#define USB_HID_GET_IDLE             (0x02U)
#define USB_HID_GET_PROTOCOL         (0x03U)
#define USB_HID_SET_REPORT           (0x09U)
#define USB_HID_SET_IDLE             (0x0AU)
#define USB_HID_SET_PROTOCOL         (0x0BU)

#define HID_MIN_POLL                 (10U)
/**
 * @}
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_CORE_Global_Types USBH HID Core Global Types
 * @{
 */

/* States for HID State Machine */
typedef enum
{
  HID_IDLE= 0U,
  HID_SEND_DATA,
  HID_BUSY,
  HID_GET_DATA,
  HID_SYNC,
  HID_POLL,
  HID_ERROR,
}
HID_State;

typedef enum
{
  HID_REQ_IDLE = 0U,
  HID_REQ_GET_REPORT_DESC,
  HID_REQ_GET_HID_DESC,
  HID_REQ_SET_IDLE,
  HID_REQ_SET_PROTOCOL,
  HID_REQ_SET_REPORT,

}
HID_CtlState;

typedef struct
{
  void  (*Init)   (void);
  void  (*Decode) (uint8_t *data);

} HID_cb_TypeDef;

typedef  struct  _HID_Report
{
    uint8_t   ReportID;
    uint8_t   ReportType;
    uint16_t  UsagePage;
    uint32_t  Usage[2];
    uint32_t  NbrUsage;
    uint32_t  UsageMin;
    uint32_t  UsageMax;
    int32_t   LogMin;
    int32_t   LogMax;
    int32_t   PhyMin;
    int32_t   PhyMax;
    int32_t   UnitExp;
    uint32_t  Unit;
    uint32_t  ReportSize;
    uint32_t  ReportCnt;
    uint32_t  Flag;
    uint32_t  PhyUsage;
    uint32_t  AppUsage;
    uint32_t  LogUsage;
}
HID_Report_TypeDef;

/* Structure for HID process */
typedef struct
{
  uint8_t              buff[64U];
  uint8_t              hc_num_in;
  uint8_t              hc_num_out;
  HID_State            state;
  uint8_t              HIDIntOutEp;
  uint8_t              HIDIntInEp;
  HID_CtlState         ctl_state;
  uint16_t             length;
  uint8_t              ep_addr;
  uint16_t             poll;
  __IO uint16_t        timer;
  HID_cb_TypeDef       *cb;
}
HID_Machine_TypeDef;

/**
 * @}
 */


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_CORE_Global_Variable USBH HID Core Global Variable
 * @{
 */
extern USBH_Class_cb_TypeDef  USBH_HID_cb;
/**
 * @}
 */

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup USBH_HID_CORE_Global_Functions
 * @{
 */
USBH_Status USBH_Set_Report (USB_OTG_CORE_HANDLE *pdev,
                             USBH_HOST *phost,
                                  uint8_t reportType,
                                  uint8_t reportId,
                                  uint8_t reportLen,
                                  uint8_t* reportBuff);

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

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_HID_CORE_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
