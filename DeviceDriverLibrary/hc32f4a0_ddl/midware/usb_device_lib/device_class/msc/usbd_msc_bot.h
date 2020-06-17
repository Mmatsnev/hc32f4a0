/**
 *******************************************************************************
 * @file  usbd_msc_bot.h
 * @brief Header for the usbd_msc_bot.c file
 *        
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12      Zhangxl         First version
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
#ifndef __USBD_MSC_BOT_H
#define __USBD_MSC_BOT_H

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_core.h"

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

/** @addtogroup USBD_MSC_BOT
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct
{
    uint32_t dSignature;
    uint32_t dTag;
    uint32_t dDataLength;
    uint8_t  bmFlags;
    uint8_t  bLUN;
    uint8_t  bCBLength;
    uint8_t  CB[16U];
}
MSC_BOT_CBW_TypeDef;

typedef struct
{
    uint32_t dSignature;
    uint32_t dTag;
    uint32_t dDataResidue;
    uint8_t  bStatus;
}
MSC_BOT_CSW_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define BOT_IDLE                      (0U)       /* Idle state */
#define BOT_DATA_OUT                  (1U)       /* Data Out state */
#define BOT_DATA_IN                   (2U)       /* Data In state */
#define BOT_LAST_DATA_IN              (3U)       /* Last Data In Last */
#define BOT_SEND_DATA                 (4U)       /* Send Immediate data */

#define BOT_CBW_SIGNATURE             (0x43425355UL)
#define BOT_CSW_SIGNATURE             (0x53425355UL)
#define BOT_CBW_LENGTH                (31U)
#define BOT_CSW_LENGTH                (13U)

/* CSW Status Definitions */
#define CSW_CMD_PASSED                (0x00U)
#define CSW_CMD_FAILED                (0x01U)
#define CSW_PHASE_ERROR               (0x02U)

/* BOT Status */
#define BOT_STATE_NORMAL              (0U)
#define BOT_STATE_RECOVERY            (1U)
#define BOT_STATE_ERROR               (2U)

#define DIR_IN                        (0U)
#define DIR_OUT                       (1U)
#define BOTH_DIR                      (2U)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern uint8_t MSC_BOT_Data[MSC_MEDIA_PACKET];
extern uint16_t MSC_BOT_DataLen;
extern uint8_t MSC_BOT_State;
extern uint8_t MSC_BOT_BurstMode;
extern MSC_BOT_CBW_TypeDef MSC_BOT_cbw;
extern MSC_BOT_CSW_TypeDef MSC_BOT_csw;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
void MSC_BOT_Init(USB_OTG_CORE_HANDLE  *pdev);
void MSC_BOT_Reset(USB_OTG_CORE_HANDLE  *pdev);
void MSC_BOT_DeInit(USB_OTG_CORE_HANDLE  *pdev);
void MSC_BOT_DataIn(USB_OTG_CORE_HANDLE  *pdev,
                    uint8_t epnum);

void MSC_BOT_DataOut(USB_OTG_CORE_HANDLE  *pdev,
                     uint8_t epnum);

void MSC_BOT_SendCSW(USB_OTG_CORE_HANDLE  *pdev,
                     uint8_t CSW_Status);

void MSC_BOT_CplClrFeature(USB_OTG_CORE_HANDLE  *pdev,
                            uint8_t epnum);


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

#endif /* __USBD_MSC_BOT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
