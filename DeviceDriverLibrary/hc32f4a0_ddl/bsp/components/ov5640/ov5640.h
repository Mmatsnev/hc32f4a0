/**
 *******************************************************************************
 * @file  ov5640.h
 * @brief This file contains all the functions prototypes of the OV5640 driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-08-20       Zhangxl         First version
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
#ifndef __OV5640_H__
#define __OV5640_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/** @defgroup OV5640
  * @{
  */

#if (BSP_OV5640_ENABLE == BSP_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup OV5640_Global_Macros OV5640 Global Macros
 * @{
 */

/**
 * @brief  OV5640 Chip ID
 */
#define OV5640_CHIPIDH              (0x300AU)
#define OV5640_CHIPIDL              (0x300BU)
/**
 * @}
 */

/**
 * @brief  OV5640 ID
 */
#define OV5640_ID                   (0x5640U)
/**
 * @}
 */

/**
 * @defgroup OV5640_Light_Control OV5640 Light Control
 */
#define OV5640_LIGHT_ON             (1U)
#define OV5640_LIGHT_OFF            (0U)
/**
 * @}
 */

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
 * @defgroup OV5640_Global_Functions OV5640 Global Functions
 * @{
 */
void OV5640_Init(void);
uint16_t OV5640_ReadID(void);
void OV5640_RGB565_Mode(void);
void OV5640_SetOutSize(uint16_t u16X,
                        uint16_t u16Y,
                        uint16_t u16Width,
                        uint16_t u16Height);
void OV5640_LightContrl(uint8_t u8Switch);
void OV5640_TestPattern(uint8_t u8Mode);

/* Implement the below functions for the specified BSP board */
en_result_t BSP_OV5640_Init(void);
en_result_t BSP_OV5640_ReadReg(uint16_t u16Reg,
                                uint8_t *pu8RxBuf,
                                uint32_t u32Len);
en_result_t BSP_OV5640_WriteReg(uint16_t u16Reg,
                                const uint8_t *pu8TxBuf,
                                uint32_t u32Len);

/**
 * @}
 */

#endif /* BSP_OV5640_ENABLE */

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

#endif /* __OV5640_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
