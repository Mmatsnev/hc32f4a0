/**
 *******************************************************************************
 * @file  hc32f4a0_hrpwm.c
 * @brief This file provides firmware functions to manage the High Resolution
 *        Pulse-Width Modulation(HRPWM).
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f4a0_hrpwm.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_HRPWM HRPWM
 * @brief HRPWM Driver Library
 * @{
 */

#if (DDL_HRPWM_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup HRPWM_Local_Macros HRPWM Local Macros
 * @{
 */
/* About 1mS timeout */
#define HRPWM_CAL_TIMEOUT             (HCLK_VALUE/1000UL)
#define HRPWM_PCLK0_MIN               (120000000UL)

#define HRPWM_SYSCLKSOURCE_HRC        (0x00U)
#define HRPWM_SYSCLKSOURCE_MRC        (0x01U)
#define HRPWM_SYSCLKSOURCE_LRC        (0x02U)
#define HRPWM_SYSCLKSOURCE_XTAL       (0x03U)
#define HRPWM_SYSCLKSOURCE_XTAL32     (0x04U)
#define HRPWM_SYSCLKSOURCE_PLLH       (0x05U)

#define HRPWM_PLLSRC_XTAL             (0x00UL)
#define HRPWM_PLLSRC_HRC              (0x01UL)

/**
 * @defgroup HRPWM_Check_Parameters_Validity HRPWM Check Parameters Validity
 * @{
 */

/*! Parameter valid check for HRPWM output channel */
#define IS_VALID_HRPWM_CH(x)                                                   \
(   ((x) >= HRPWM_CH_MIN)                       &&                             \
    ((x) <= HRPWM_CH_MAX))

/*! Parameter valid check for HRPWM caliration unit */
#define IS_VALID_HRPWM_CAL_UNIT(x)                                             \
(   (HRPWM_CAL_UNIT0 == (x))                   ||                             \
    (HRPWM_CAL_UNIT1 == (x)))

/*! Parameter valid check for HRPWM delay number */
#define IS_VALID_HRPWM_DELAY_NUM(x)                                            \
(   ((x) >= HRPWM_CH_DELAY_NUM_MIN)             &&                             \
    ((x) <= HRPWM_CH_DELAY_NUM_MAX))

/**
 * @}
 */

/**
 * @}
 */

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
 * @defgroup HRPWM_Global_Functions HRPWM Global Functions
 * @{
 */

/**
 * @brief  Process for getting HRPWM Calibrate function code
 * @param  [in] u32Unit     Calibrate unit, the parameter should be HRPWM_CAL_UNIT0 or HRPWM_CAL_UNIT1
 * @param  [out] pu8Code    The pointer to get calibrate code.
 * @retval Ok:                      Success
 * @retval ErrorTimeout:            Time out
 * @retval ErrorInvalidParameter:   Parameter error
 */
en_result_t HRPWM_CalibrateProcess(uint32_t u32Unit, uint8_t* pu8Code)
{
    uint32_t u32Timeout = HRPWM_CAL_TIMEOUT;
    en_result_t enRet = Ok;

    if(NULL != pu8Code)
    {
        /* Enable calibrate */
        HRPWM_CalibrateCmd(u32Unit, Enable);
        /* Wait calibrate finish flag */
        while(Disable == HRPWM_GetCalibrateStd(u32Unit))
        {
            if(0UL == u32Timeout--)
            {
                enRet = ErrorTimeout;
                break;
            }
        }

        if(Ok == enRet)
        {
            /* Get calibrate code */
            *pu8Code = HRPWM_GetCalCode(u32Unit);
        }
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }
    return enRet;
}

/**
 * @brief  HRPWM Calibrate function enable or disable for specified unit
 * @param  [in] u32Unit     Calibrate unit, the parameter should be HRPWM_CAL_UNIT0 or HRPWM_CAL_UNIT1
 * @param  [in] enNewState  Disable or Enable the function
 * @retval None
 */
void HRPWM_CalibrateCmd(uint32_t u32Unit, en_functional_state_t enNewState)
{
    __IO uint32_t *CALCRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CAL_UNIT(u32Unit));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    CALCRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CALCR0) + 4UL*u32Unit);

    if(Enable == enNewState)
    {
        SET_REG32_BIT(*CALCRx, HRPWM_CALCR_CALEN);

    }
    else
    {
        CLEAR_REG32_BIT(*CALCRx, HRPWM_CALCR_CALEN);
    }
}

/**
 * @brief  HRPWM Calibrate function status get for specified unit
 * @param  [in] u32Unit     Calibrate unit, the parameter should be HRPWM_CAL_UNIT0 or HRPWM_CAL_UNIT1
 * @retval Enable:          Calibration function is on.
 * @retval Disable:         Calibration function is off.
 */
en_functional_state_t HRPWM_GetCalibrateStd(uint32_t u32Unit)
{
    en_functional_state_t enRet;
    __IO uint32_t *CALCRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CAL_UNIT(u32Unit));

    CALCRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CALCR0) + 4UL*u32Unit);

    if( 0UL != READ_REG32_BIT(*CALCRx, HRPWM_CALCR_ENDF))
    {
        enRet = Enable;
    }
    else
    {
        enRet = Disable;
    }
    return enRet;
}

/**
 * @brief  HRPWM Calibrate code get for specified unit
 * @param  [in] u32Unit     Calibrate unit, the parameter should be HRPWM_CAL_UNIT0 or HRPWM_CAL_UNIT1
 * @retval uint8_t:         The calibration code.
 */
uint8_t HRPWM_GetCalCode(uint32_t u32Unit)
{
    __IO uint32_t *CALCRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CAL_UNIT(u32Unit));

    CALCRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CALCR0) + 4UL*u32Unit);

    return ((uint8_t)(READ_REG32(*CALCRx)));
}

/**
 * @brief  HRPWM function enable or disable for specified channel
 * @param  [in] u32Ch       Channel, the parameter should range from HRPWM_CH_MIN to HRPWM_CH_MAX
 * @param  [in] enNewState  Disable or Enable the function
 * @retval None
 */
void HRPWM_CHCmd(uint32_t u32Ch, en_functional_state_t enNewState)
{
    __IO uint32_t *CRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CH(u32Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    CRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CR1) + 4UL*(u32Ch - 1UL));
    if(Enable == enNewState)
    {
        SET_REG32_BIT(*CRx, HRPWM_CR_EN);
    }
    else
    {
        CLEAR_REG32_BIT(*CRx, HRPWM_CR_EN);
    }
}

/**
 * @brief  HRPWM positive edge adjust enable or disable for specified channel
 * @param  [in] u32Ch       Channel, the parameter should range from HRPWM_CH_MIN to HRPWM_CH_MAX
 * @param  [in] enNewState  Disable or Enable the function
 * @retval None
 */
void HRPWM_CHPositAdjCmd(uint32_t u32Ch, en_functional_state_t enNewState)
{
    __IO uint32_t *CRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CH(u32Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    CRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CR1) + 4UL*(u32Ch - 1UL));
    if(Enable == enNewState)
    {
        SET_REG32_BIT(*CRx, HRPWM_CR_PE);
    }
    else
    {
        CLEAR_REG32_BIT(*CRx, HRPWM_CR_PE);
    }
}

/**
 * @brief  HRPWM negative edge adjust enable or disable for specified channel
 * @param  [in] u32Ch       Channel, the parameter should range from HRPWM_CH_MIN to HRPWM_CH_MAX
 * @param  [in] enNewState  Disable or Enable the function
 * @retval None
 */
void HRPWM_CHNegatAdjCmd(uint32_t u32Ch, en_functional_state_t enNewState)
{
    __IO uint32_t *CRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CH(u32Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    CRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CR1) + 4UL*(u32Ch - 1UL));
    if(Enable == enNewState)
    {
        SET_REG32_BIT(*CRx, HRPWM_CR_NE);
    }
    else
    {
        CLEAR_REG32_BIT(*CRx, HRPWM_CR_NE);
    }
}

/**
 * @brief  HRPWM positive edge adjust delay counts configration for specified channel
 * @param  [in] u32Ch        Channel, the parameter should range from HRPWM_CH_MIN to HRPWM_CH_MAX
 * @param  [in] u32DelayNum  Delay counts of minimum delay time.
 * @retval None
 */
void HRPWM_CHPositCfg(uint32_t u32Ch, uint32_t u32DelayNum)
{
    __IO uint32_t *CRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CH(u32Ch));
    DDL_ASSERT(IS_VALID_HRPWM_DELAY_NUM(u32DelayNum));

    CRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CR1) + 4UL*(u32Ch - 1UL));
    MODIFY_REG32(*CRx, HRPWM_CR_PSEL, (u32DelayNum-1UL) << HRPWM_CR_PSEL_POS);
}

/**
 * @brief  HRPWM negative edge adjust delay counts configration for specified channel
 * @param  [in] u32Ch       Channel, the parameter should range from HRPWM_CH_MIN to HRPWM_CH_MAX
 * @param  [in] u32DelayNum Delay counts of minimum delay time.
 * @retval None
 */
void HRPWM_CHNegatCfg(uint32_t u32Ch, uint32_t u32DelayNum)
{
    __IO uint32_t *CRx;
    /* Check parameters */
    DDL_ASSERT(IS_VALID_HRPWM_CH(u32Ch));
    DDL_ASSERT(IS_VALID_HRPWM_DELAY_NUM(u32DelayNum));

    CRx = (__IO uint32_t*)(((uint32_t)&M4_HRPWM->CR1) + 4UL*(u32Ch - 1UL));
    MODIFY_REG32(*CRx, HRPWM_CR_NSEL, (u32DelayNum-1UL) << HRPWM_CR_NSEL_POS);
}

/**
 * @brief  HRPWM Judge the condition of calibration function.
 * @param  None
 * @retval Enable:          Condition is ready.
 * @retval Disable:         Condition is not ready.
 */
en_functional_state_t HRPWM_ConditionConfirm(void)
{
    en_functional_state_t enRet = Enable;
    uint32_t plln;
    uint32_t pllp;
    uint32_t pllm;
    uint32_t sysclkFreq;
    uint32_t pclk0Freq;

    switch (READ_REG8_BIT(M4_CMU->CKSWR, CMU_CKSWR_CKSW))
    {
        case HRPWM_SYSCLKSOURCE_HRC:
            /* HRC is used to system clock */
            sysclkFreq = HRC_VALUE;
            break;
        case HRPWM_SYSCLKSOURCE_MRC:
            /* MRC is used to system clock */
            sysclkFreq = MRC_VALUE;
            break;
        case HRPWM_SYSCLKSOURCE_LRC:
            /* LRC is used to system clock */
            sysclkFreq = LRC_VALUE;
            break;
        case HRPWM_SYSCLKSOURCE_XTAL:
            /* XTAL is used to system clock */
            sysclkFreq = XTAL_VALUE;
            break;
        case HRPWM_SYSCLKSOURCE_XTAL32:
            /* XTAL32 is used to system clock */
            sysclkFreq = HRC_VALUE;
            break;
        case HRPWM_SYSCLKSOURCE_PLLH:
            /* PLLHP is used as system clock. */
            pllp = (uint32_t)((M4_CMU->PLLHCFGR >> CMU_PLLHCFGR_PLLHP_POS) & 0x0FUL);
            plln = (uint32_t)((M4_CMU->PLLHCFGR >> CMU_PLLHCFGR_PLLHN_POS) & 0xFFUL);
            pllm = (uint32_t)((M4_CMU->PLLHCFGR >> CMU_PLLHCFGR_PLLHM_POS) & 0x03UL);

            /* fpll = ((pllin / pllm) * plln) / pllp */
            if (HRPWM_PLLSRC_XTAL == READ_REG32_BIT(M4_CMU->PLLHCFGR, CMU_PLLHCFGR_PLLSRC))
            {
                sysclkFreq = ((XTAL_VALUE/(pllm + 1UL))*(plln + 1UL))/(pllp + 1UL);
            }
            else
            {
                sysclkFreq = ((HRC_VALUE/(pllm + 1UL))*(plln + 1UL))/(pllp + 1UL);
            }
            break;
        default:
            sysclkFreq = HRC_VALUE;
            enRet = Disable;
            break;
    }

    if(Enable == enRet)
    {
        /* Get pclk0. */
        pclk0Freq = sysclkFreq >> (READ_REG32_BIT(M4_CMU->SCFGR, CMU_SCFGR_PCLK0S) >> CMU_SCFGR_PCLK0S_POS);

        if(pclk0Freq < HRPWM_PCLK0_MIN)
        {
            enRet = Disable;
        }
    }
    return enRet;
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
