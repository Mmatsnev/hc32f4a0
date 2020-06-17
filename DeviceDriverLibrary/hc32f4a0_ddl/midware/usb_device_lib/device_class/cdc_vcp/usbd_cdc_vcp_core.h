/**
 *******************************************************************************
 * @file  usbd_cdc_vcp_core.h
 * @brief Header for usbd_cdc_vcp_core.c file.
 *        
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       zhangxl         First version
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
#ifndef __USBD_CDC_VCP_CORE_H__
#define __USBD_CDC_VCP_CORE_H__

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

/** @addtogroup USBD_CDC_VCP_CORE
 * @{
 */


/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 * @defgroup USBD_CDC_VCP_CORE_Global_Types USBD CDC VCP Core Global Types
 * @{
 */

typedef struct
{
  uint16_t (*pIf_Init)     (void);
  uint16_t (*pIf_DeInit)   (void);
  uint16_t (*pIf_Ctrl)     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
  uint16_t (*pIf_DataTx)   (uint32_t Len);
  uint16_t (*pIf_DataRx)   (uint8_t* Buf, uint32_t Len);
}CDC_IF_Prop_TypeDef;
/**
 * @}
 */


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup USBD_CDC_VCP_CORE_Global_Macros USBD CDC VCP Core Global Macros
 * @{
 */

#define USB_CDC_CONFIG_DESC_SIZ                (75U)
#define USB_CDC_DESC_SIZ                       (75U-9U)

#define CDC_DESCRIPTOR_TYPE                    (0x21U)

#define DEVICE_CLASS_CDC                       (0x02U)
#define DEVICE_SUBCLASS_CDC                    (0x00U)

#define STANDARD_ENDPOINT_DESC_SIZE            (0x09U)

#define CDC_DATA_IN_PACKET_SIZE                (CDC_DATA_MAX_PACKET_SIZE)

#define CDC_DATA_OUT_PACKET_SIZE               (CDC_DATA_MAX_PACKET_SIZE)

/* CDC Requests */
#define SEND_ENCAPSULATED_COMMAND               (0x00U)
#define GET_ENCAPSULATED_RESPONSE               (0x01U)
#define SET_COMM_FEATURE                        (0x02U)
#define GET_COMM_FEATURE                        (0x03U)
#define CLEAR_COMM_FEATURE                      (0x04U)
#define SET_LINE_CODING                         (0x20U)
#define GET_LINE_CODING                         (0x21U)
#define SET_CONTROL_LINE_STATE                  (0x22U)
#define SEND_BREAK                              (0x23U)
#define NO_CMD                                  (0xFFU)

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/**
 * @addtogroup USBD_CDC_VCP_CORE_Global_Variables
 * @{
 */
extern USBD_Class_cb_TypeDef  USBD_CDC_cb;

/**
 * @}
 */

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup USBD_CDC_VCP_CORE_Global_Functions
 * @{
 */
/* CDC Device library callbacks */
uint8_t  usbd_cdc_Init        (void  *pdev, uint8_t cfgidx);
uint8_t  usbd_cdc_DeInit      (void  *pdev, uint8_t cfgidx);
uint8_t  usbd_cdc_Setup       (void  *pdev, USB_SETUP_REQ *req);
uint8_t  usbd_cdc_EP0_RxReady (void *pdev);
uint8_t  usbd_cdc_DataIn      (void *pdev, uint8_t epnum);
uint8_t  usbd_cdc_DataOut     (void *pdev, uint8_t epnum);
uint8_t  usbd_cdc_SOF         (void *pdev);
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

#endif  /* __USBD_CDC_VCP_CORE_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
