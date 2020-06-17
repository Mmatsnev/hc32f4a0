/**
 *******************************************************************************
 * @file  usbd_hid_custom_core.h
 * @brief Header file for the usbd_customhid_core.c file.
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
#ifndef __USB_CUSTOMHID_CORE_H_
#define __USB_CUSTOMHID_CORE_H_

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usbd_ioreq.h"

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

/** @addtogroup USBD_HID_CUSTOM_CORE
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup USBD_HID_CUSTOM_CORE_Global_Macros USBD HID Custom Core Global Macros
 * @{
 */

#define CUSTOM_HID_REPORT_DESC_SIZE         (116U)

#define USB_CUSTOM_HID_CONFIG_DESC_SIZ      (41U)
#define USB_CUSTOM_HID_DESC_SIZ             (9U)

#define CUSTOM_HID_DESCRIPTOR_TYPE          (0x21U)
#define CUSTOM_HID_REPORT_DESC              (0x22U)

#define CUSTOM_HID_REQ_SET_PROTOCOL         (0x0BU)
#define CUSTOM_HID_REQ_GET_PROTOCOL         (0x03U)

#define CUSTOM_HID_REQ_SET_IDLE             (0x0AU)
#define CUSTOM_HID_REQ_GET_IDLE             (0x02U)

#define CUSTOM_HID_REQ_SET_REPORT           (0x09U)
#define CUSTOM_HID_REQ_GET_REPORT           (0x01U)

#define LED1_REPORT_ID                      (0x01U)
#define LED1_REPORT_COUNT                   (0x01U)

#define LED2_REPORT_ID                      (0x02U)
#define LED2_REPORT_COUNT                   (0x01U)

#define LED3_REPORT_ID                      (0x03U)
#define LED3_REPORT_COUNT                   (0x01U)

#define LED4_REPORT_ID                      (0x04U)
#define LED4_REPORT_COUNT                   (0x01U)

#define KEY_REPORT_ID                       (0x05U)
#define TAMPER_REPORT_ID                    (0x06U)
#define ADC_REPORT_ID                       (0x07U)
/**
 * @}
 */


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/**
 * @addtogroup USBD_HID_CUSTOM_CORE_Global_Variables
 * @{
 */
extern USBD_Class_cb_TypeDef  USBD_CUSTOMHID_cb;
extern uint8_t Report_buf[2U];
extern uint8_t Send_Buf[2U];
/**
 * @}
 */

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup USBD_HID_CUSTOM_CORE_Global_Functions
 * @{
 */
uint8_t USBD_CUSTOM_HID_SendReport (USB_OTG_CORE_HANDLE  *pdev,
                                 uint8_t *report,
                                 uint16_t len);

uint8_t USBD_CUSTOM_HID_Init (void  *pdev,
                               uint8_t cfgidx);
uint8_t USBD_CUSTOM_HID_DeInit (void  *pdev,
                                 uint8_t cfgidx);
uint8_t USBD_CUSTOM_HID_Setup (void  *pdev,
                                USB_SETUP_REQ *req);
uint8_t *USBD_CUSTOM_HID_GetCfgDesc (uint8_t speed, uint16_t *length);
uint8_t USBD_CUSTOM_HID_DataIn (void  *pdev, uint8_t epnum);
uint8_t USBD_CUSTOM_HID_DataOut (void  *pdev, uint8_t epnum);
uint8_t USBD_CUSTOM_HID_EP0_RxReady (void  *pdev);
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

#endif  /* __USB_CUSTOMHID_CORE_H_ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

