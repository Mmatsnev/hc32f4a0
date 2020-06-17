/**
 *******************************************************************************
 * @file  usbd_def.h
 * @brief General defines for the usb device library.
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
#ifndef __USBD_DEF_H
#define __USBD_DEF_H

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_conf.h"

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

/** @defgroup USBD_DEF
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USBD_DEF_Global_Macros USBD Define Global Macros
 * @{
 */
#ifndef NULL
#define NULL    0U
#endif

#define  USB_LEN_DEV_QUALIFIER_DESC                     (0x0AU)
#define  USB_LEN_DEV_DESC                               (0x12U)
#define  USB_LEN_CFG_DESC                               (0x09U)
#define  USB_LEN_IF_DESC                                (0x09U)
#define  USB_LEN_EP_DESC                                (0x07U)
#define  USB_LEN_OTG_DESC                               (0x03U)

#define  USBD_IDX_LANGID_STR                            (0x00U)
#define  USBD_IDX_MFC_STR                               (0x01U)
#define  USBD_IDX_PRODUCT_STR                           (0x02U)
#define  USBD_IDX_SERIAL_STR                            (0x03U)
#define  USBD_IDX_CONFIG_STR                            (0x04U)
#define  USBD_IDX_INTERFACE_STR                         (0x05U)

#define  USB_REQ_TYPE_STANDARD                          (0x00U)
#define  USB_REQ_TYPE_CLASS                             (0x20U)
#define  USB_REQ_TYPE_VENDOR                            (0x40U)
#define  USB_REQ_TYPE_MASK                              (0x60U)

#define  USB_REQ_RECIPIENT_DEVICE                       (0x00U)
#define  USB_REQ_RECIPIENT_INTERFACE                    (0x01U)
#define  USB_REQ_RECIPIENT_ENDPOINT                     (0x02U)
#define  USB_REQ_RECIPIENT_MASK                         (0x03U)

#define  USB_REQ_GET_STATUS                             (0x00U)
#define  USB_REQ_CLEAR_FEATURE                          (0x01U)
#define  USB_REQ_SET_FEATURE                            (0x03U)
#define  USB_REQ_SET_ADDRESS                            (0x05U)
#define  USB_REQ_GET_DESCRIPTOR                         (0x06U)
#define  USB_REQ_SET_DESCRIPTOR                         (0x07U)
#define  USB_REQ_GET_CONFIGURATION                      (0x08U)
#define  USB_REQ_SET_CONFIGURATION                      (0x09U)
#define  USB_REQ_GET_INTERFACE                          (0x0AU)
#define  USB_REQ_SET_INTERFACE                          (0x0BU)
#define  USB_REQ_SYNCH_FRAME                            (0x0CU)

#define  USB_DESC_TYPE_DEVICE                              (1U)
#define  USB_DESC_TYPE_CONFIGURATION                       (2U)
#define  USB_DESC_TYPE_STRING                              (3U)
#define  USB_DESC_TYPE_INTERFACE                           (4U)
#define  USB_DESC_TYPE_ENDPOINT                            (5U)
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    (6U)
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           (7U)


#define USB_CONFIG_REMOTE_WAKEUP                           (2U)
#define USB_CONFIG_SELF_POWERED                            (1U)

#define USB_FEATURE_EP_HALT                                (0U)
#define USB_FEATURE_REMOTE_WAKEUP                          (1U)
#define USB_FEATURE_TEST_MODE                              (2U)

#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (uint16_t)(((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))

#define LOBYTE(x)  ((uint8_t)((uint16_t)(x) & 0x00FFU))
#define HIBYTE(x)  ((uint8_t)(((uint16_t)(x) & 0xFF00U) >>8U))

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

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

#endif /* __USBD_DEF_H */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
