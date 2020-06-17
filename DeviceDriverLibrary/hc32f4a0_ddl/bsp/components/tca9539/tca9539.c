/**
 *******************************************************************************
 * @file  tca9539.c
 * @brief This file provides firmware functions for IO expand IC TCA9539.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "tca9539.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/** @defgroup TCA9539 IO Expand IC TCA9539
  * @{
  */

#if (BSP_TCA9539_ENABLE == BSP_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup TCA9539_Global_Functions TCA9539 Global Functions
 * @{
 */
/**
 * @brief  Initialize IO expand IC TCA9539.
 * @param  None
 * @retval None
 */
void TCA9539_Init(void)
{
    EIO_Init();
}

/**
 * @brief  Reset IO expand IC TCA9539.
 * @param  None
 * @retval None
 */
void TCA9539_Reset(void)
{
    EIO_Reset();
}

/**
 * @brief  Initialize IO expand IC TCA9539 interrupt.
 * @param  None
 * @retval None
 */
void TCA9539_IntInit(void)
{
    EIO_IntInit();
}

/**
 * @brief  Reset IO expand IC TCA9539.
 * @param  [in] u8Reg TCA9539 register.
 *   @arg  TCA9539_REG_INPUT_0
 *   @arg  TCA9539_REG_INPUT_1
 *   @arg  TCA9539_REG_OUTPUT_0
 *   @arg  TCA9539_REG_OUTPUT_1
 *   @arg  TCA9539_REG_INVERT_0
 *   @arg  TCA9539_REG_INVERT_1
 *   @arg  TCA9539_REG_CONFIG_0
 *   @arg  TCA9539_REG_CONFIG_1
 * @param  [in] u8Val The value to written.
 * @retval en_result_t
 */
en_result_t TCA9539_WriteReg(uint8_t u8Reg, uint8_t u8Val)
{
    return EIO_Write(u8Reg, u8Val);
}

/**
 * @brief  Reset IO expand IC TCA9539.
 * @param  [in] u8Reg TCA9539 register.
 *   @arg  TCA9539_REG_INPUT_0
 *   @arg  TCA9539_REG_INPUT_1
 *   @arg  TCA9539_REG_OUTPUT_0
 *   @arg  TCA9539_REG_OUTPUT_1
 *   @arg  TCA9539_REG_INVERT_0
 *   @arg  TCA9539_REG_INVERT_1
 *   @arg  TCA9539_REG_CONFIG_0
 *   @arg  TCA9539_REG_CONFIG_1
 * @param  [out] *u8Val TCA9539 register value to read.
 * @retval en_result_t
 */
en_result_t TCA9539_ReadReg(uint8_t u8Reg, uint8_t *u8Val)
{
    return EIO_Read(u8Reg, u8Val);
}
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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
