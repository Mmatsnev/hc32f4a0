/**
 *******************************************************************************
 * @file  tca9539.h
 * @brief This file contains all the functions prototypes of the TCA9539 driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
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
#ifndef __TCA9539_H__
#define __TCA9539_H__

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

/** @addtogroup TCA9539
  * @{
  */

#if (BSP_TCA9539_ENABLE == BSP_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup TCA9539_Global_Macros TCA9539 Global Macros
 * @{
 */
#define TCA9539_REG_INPUT_0         ((uint8_t)0x00)
#define TCA9539_REG_INPUT_1         ((uint8_t)0x01)
#define TCA9539_REG_OUTPUT_0        ((uint8_t)0x02)
#define TCA9539_REG_OUTPUT_1        ((uint8_t)0x03)
#define TCA9539_REG_INVERT_0        ((uint8_t)0x04)
#define TCA9539_REG_INVERT_1        ((uint8_t)0x05)
#define TCA9539_REG_CONFIG_0        ((uint8_t)0x06)
#define TCA9539_REG_CONFIG_1        ((uint8_t)0x07)

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
 * @defgroup TCA9539_Global_Functions TCA9539 Global Functions
 * @{
 */
void TCA9539_Init(void);
void TCA9539_IntInit(void);
void TCA9539_Reset(void);
en_result_t TCA9539_WriteReg(uint8_t u8Reg, uint8_t u8Val);
en_result_t TCA9539_ReadReg(uint8_t u8Reg, uint8_t *u8Val);

void EIO_Init(void);
void EIO_Reset(void);
void EIO_Delay(uint32_t u32Delay);
en_result_t EIO_Write(uint8_t u8Reg, uint8_t u8Val);
en_result_t EIO_Read(uint8_t u8Reg, uint8_t *u8Val);
void EIO_IntInit(void);

/**
 * @}
 */

#endif /* BSP_TCA9539_ENABLE */

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

#endif /* __TCA9539_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
