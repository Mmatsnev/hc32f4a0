/**
 *******************************************************************************
 * @file  hc32f4a0_mau.h
 * @brief This file contains all the functions prototypes of the MAU driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao          First version
   2020-07-15       Hexiao          Modify MAU_SqrtStartCmd to MAU_SqrtStart
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
#ifndef __HC32F4A0_MAU_H__
#define __HC32F4A0_MAU_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_MAU
 * @{
 */

#if (DDL_MAU_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup MAU_Global_Macros MAU Global Macros
 * @{
 */

#define MAU_SQRT_TIMEOUT               (HCLK_VALUE / 10000UL)/* About 1mS timeout */
#define MAU_SQRT_OUTPUT_LSHIFT_MAX     (16U)
#define MAU_SIN_Q15_SCALAR             (0x8000UL)
#define MAU_SIN_ANGIDX_TOTAL           (0x1000UL)

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
 * @addtogroup MAU_Global_Functions
 * @{
 */

void MAU_SqrtInit(M4_MAU_TypeDef *MAUx, uint8_t u8LShBitsNumber, en_functional_state_t enIntNewState);
void MAU_SqrtDeInit(M4_MAU_TypeDef *MAUx);

void MAU_SqrtResultLShiftCfg(M4_MAU_TypeDef *MAUx, uint8_t u8LShBitsNumber);
void MAU_SqrtIntCmd(M4_MAU_TypeDef *MAUx, en_functional_state_t enNewState);
void MAU_SqrtWriteDataReg(M4_MAU_TypeDef *MAUx, uint32_t u32Radicand);
en_flag_status_t MAU_SqrtGetStatus(const M4_MAU_TypeDef *MAUx);
uint32_t MAU_SqrtReadDataReg(const M4_MAU_TypeDef *MAUx);
void MAU_SqrtStart(M4_MAU_TypeDef* MAUx);

en_result_t MAU_Sqrt(M4_MAU_TypeDef *MAUx, uint32_t u32Radicand, uint32_t *pu32Result);

int16_t MAU_Sin(M4_MAU_TypeDef *MAUx, uint16_t u16AngleIdx);

/**
 * @}
 */

#endif /* DDL_MAU_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_MAU_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
