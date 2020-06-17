/**
 *******************************************************************************
 * @file  hc32f4a0_hrpwm.h
 * @brief Head file for HRPWM module.
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
#ifndef __HC32F4A0_HRPWM_H__
#define __HC32F4A0_HRPWM_H__

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

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_HRPWM
 * @{
 */

#if (DDL_HRPWM_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup HRPWM_Global_Macros HRPWM Global Macros
 * @{
 */

#define   HRPWM_CH_MIN                (1UL)
#define   HRPWM_CH_MAX                (16UL)

#define   HRPWM_CH_DELAY_NUM_MIN      (1U)
#define   HRPWM_CH_DELAY_NUM_MAX      (256U)

/** @defgroup HRPWM_Calibrate_Unit_Define HRPWM Calibrate unit define
 * @{
 */
#define   HRPWM_CAL_UNIT0             (0x00UL)
#define   HRPWM_CAL_UNIT1             (0x01UL)
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
 * @addtogroup HRPWM_Global_Functions
 * @{
 */
/* HRPWM Judge the condition of calibration function */
en_functional_state_t HRPWM_ConditionConfirm(void);

/* Process for getting HRPWM Calibrate function code */
en_result_t HRPWM_CalibrateProcess(uint32_t u32Unit, uint8_t* pu8Code);

/* HRPWM Calibrate function enable or disable for specified unit */
void HRPWM_CalibrateCmd(uint32_t u32Unit, en_functional_state_t enNewState);
/* HRPWM Calibrate function status get for specified unit */
en_functional_state_t HRPWM_GetCalibrateStd(uint32_t u32Unit);
/* HRPWM Calibrate code get for specified unit */
uint8_t HRPWM_GetCalCode(uint32_t u32Unit);

/* HRPWM function enable or disable for specified channel */
void HRPWM_CHCmd(uint32_t u32Ch, en_functional_state_t enNewState);
/* HRPWM positive edge adjust enable or disable for specified channel */
void HRPWM_CHPositAdjCmd(uint32_t u32Ch, en_functional_state_t enNewState);
/* HRPWM negative edge adjust enable or disable for specified channel */
void HRPWM_CHNegatAdjCmd(uint32_t u32Ch, en_functional_state_t enNewState);
/* HRPWM positive edge adjust delay counts configration for specified channel */
void HRPWM_CHPositCfg(uint32_t u32Ch, uint32_t u32DelayNum);
/* HRPWM negative edge adjust delay counts configration for specified channel */
void HRPWM_CHNegatCfg(uint32_t u32Ch, uint32_t u32DelayNum);

/**
 * @}
 */

#endif /* DDL_HRPWM_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_HRPWM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
