/**
 *******************************************************************************
 * @file  hc32f4a0_ots.c
 * @brief This file provides firmware functions to manage the OTS.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
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
#include "hc32f4a0_ots.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_OTS OTS
 * @brief OTS Driver Library
 * @{
 */

#if (DDL_OTS_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup OTS_Configuration_Bit_Mask OTS Configuration Bit Mask
 * @{
 */
#define OTS_CTL_INIT_MSK            (OTS_CTL_OTSCK | OTS_CTL_TSSTP)
#define OTS_COM_TRIG_EN_MSK         (OTS_TRIG_COM1_ON_COM2_OFF)
/**
 * @}
 */

/**
 * @defgroup OTS_Check_Parameters_Validity OTS check parameters validity
 * @{
 */
#define IS_OTS_CLK_SEL(x)                                                      \
(   ((x) == OTS_CLK_HRC)                    ||                                 \
    ((x) == OTS_CLK_XTAL))

#define IS_OTS_AUTO_OFF_EN(x)                                                  \
(   ((x) == OTS_AUTO_OFF_DISABLE)           ||                                 \
    ((x) == OTS_AUTO_OFF_ENABLE))

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
static uint8_t m_u16OtsClkSel = OTS_CLK_HRC;
static float32_t m_f32ParaK = 0.0f;
static float32_t m_f32ParaM = 0.0f;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup OTS_Global_Functions OTS Global Functions
 * @{
 */

/**
 * @brief  Initializes OTS according to the specified parameters in the structure stc_ots_init_t.
 * @param  [in]  pstcInit               Pointer to a stc_ots_init_t structure value that
 *                                      contains the configuration information for OTS.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       pstcInit == NULL.
 */
en_result_t OTS_Init(const stc_ots_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcInit != NULL)
    {
        DDL_ASSERT(IS_OTS_CLK_SEL(pstcInit->u16ClkSel));
        DDL_ASSERT(IS_OTS_AUTO_OFF_EN(pstcInit->u16AutOffEn));

        /* Stop OTS sampling. */
        OTS_Stop();

        MODIFY_REG16(M4_OTS->CTL, OTS_CTL_INIT_MSK, (pstcInit->u16ClkSel|pstcInit->u16AutOffEn));

        m_f32ParaK = pstcInit->f32ParaK;
        m_f32ParaM = pstcInit->f32ParaM;

        if (pstcInit->u16ClkSel == OTS_CLK_XTAL)
        {
            m_u16OtsClkSel = OTS_CLK_XTAL;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Set a default value for OTS initialization structure.
 * @param  [in]  pstcInit               Pointer to a stc_ots_init_t structure that
 *                                      contains configuration information.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       pstcInit == NULL.
 */
en_result_t OTS_StructInit(stc_ots_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (pstcInit != NULL)
    {
        pstcInit->u16ClkSel   = OTS_CLK_HRC;
        pstcInit->u16AutOffEn = OTS_AUTO_OFF_ENABLE;
        pstcInit->f32ParaK    = 0.0f;
        pstcInit->f32ParaM    = 0.0f;
        pstcInit->u32ClkMHz   = (HCLK_VALUE / 1000000UL);
        pstcInit->enUseDefaultPara = Disable;

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  De-initializes OTS peripheral. Reset the registers of OTS.
 * @param  None
 * @retval None
 */
void OTS_DeInit(void)
{
    /* Stop OTS. */
    OTS_Stop();
    /* Set the value of all registers to the reset value. */
    WRITE_REG16(M4_OTS->CTL, 0U);
    WRITE_REG16(M4_OTS->DR1, 0U);
    WRITE_REG16(M4_OTS->DR2, 0U);
    WRITE_REG16(M4_OTS->ECR, 0U);
}

/**
 * @brief  Get temperature via normal mode.
 * @param  [out] pf32Temp               Pointer to a float32_t type address that the temperature value to be stored.
 * @param  [in]  u32Timeout             Timeout value(millisecond).
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorTimeout:                Works timeout.
 *   @arg  ErrorInvalidParameter:      -pf32Temp == NULL.
 *                                     -u32Timeout == 0.
 */
en_result_t OTS_Polling(float32_t *pf32Temp, uint32_t u32Timeout)
{
    uint32_t  u32TimeCnt;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pf32Temp != NULL) && (u32Timeout != 0U))
    {
        u32TimeCnt = u32Timeout * (HCLK_VALUE / 10U / 1000U);
        enRet = ErrorTimeout;

        OTS_Start();
        while (u32TimeCnt--)
        {
            if (bM4_OTS->CTL_b.OTSST == 0U)
            {
                *pf32Temp = OTS_CalculateTemp();
                enRet = Ok;
                break;
            }
        }
        OTS_Stop();
    }

    return enRet;
}

/**
 * @brief  Enable or disable the OTS interrutp.
 * @param  [in]  enNewState             An en_functional_state_t enumeration type value.
 *   @arg Enable:                       Enable the OTS interrupt.
 *   @arg Disable:                      Disable the OTS interrupt.
 * @retval None
 */
void OTS_IntCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    WRITE_REG32(bM4_OTS->CTL_b.OTSIE, enNewState);
}

/**
 * @brief  Specifies the event which is used to trigger OTS start.
 * @param  [in]  enEvent                An event of other peripheral.
 *                                      This parameter can be a value of @ref en_event_src_t except 'EVT_OTS'.
 * @retval None
 */
void OTS_SetTriggerSrc(en_event_src_t enEvent)
{
    MODIFY_REG32(M4_AOS->OTS_TRG, AOS_OTS_TRG_TRGSEL, enEvent);
}

/**
 * @brief  Enable or disable common trigger event to start OTS.
 * @param  [in]  u32ComTrigEn           Common trigger event enable bit mask.
 *                                      This parameter can be a value of @ref OTS_Common_Trigger_Sel
 *   @arg  OTS_TRIG_COM1_COM2_OFF:      Disable common trigger event 1 and event 2 to start OTS.
 *   @arg  OTS_TRIG_COM1_ON_COM2_OFF:   Enable common trigger event 1 and disable event 2 to start OTS.
 *   @arg  OTS_TRIG_COM1_OFF_COM2_ON:   Disable common trigger event 1 and enable event 1 to start OTS.
 *   @arg  OTS_TRIG_COM1_COM2_ON:       Enable common trigger event 1 and event 2 to start OTS.
 * @retval None
 */
void OTS_ComTrigCmd(uint32_t u32ComTrigEn)
{
    MODIFY_REG32(M4_AOS->OTS_TRG, OTS_COM_TRIG_EN_MSK, u32ComTrigEn);
}

/**
 * @brief  OTS scaling experiment. Get the value of the data register at the specified temperature to calculate K and M.
 * @param  [out] pu16Dr1:               Pointer to an address to store the value of data register 1.
 * @param  [out] pu16Dr2:               Pointer to an address to store the value of data register 2.
 * @param  [out] pu16Ecr:               Pointer to an address to store the value of register ECR.
 * @param  [out] pf32A:                 Pointer to an address to store the parameter A.
 * @param  [in]  u32Timeout:            Timeout value.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorTimeout:                Works timeout.
 *   @arg  ErrorInvalidParameter:      -pu16Dr1 == NULL.
 *                                     -pu16Dr2 == NULL.
 *                                     -pu16Ecr == NULL.
 *                                     -pf32A == NULL.
 *                                     -u32Timeout == 0.
 */
en_result_t OTS_ScalingExperiment(uint16_t *pu16Dr1, uint16_t *pu16Dr2, \
                                        uint16_t *pu16Ecr, float32_t *pf32A,  \
                                  uint32_t u32Timeout)
{
    float32_t f32Dr1;
    float32_t f32Dr2;
    float32_t f32Ecr;
    uint32_t  u32TimeCnt;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pu16Dr1) && (NULL != pu16Dr2) && \
        (NULL != pu16Ecr) && (NULL != pf32A)   && \
        (u32Timeout != 0U))
    {
        u32TimeCnt = u32Timeout * (HCLK_VALUE / 10U / 1000U);
        enRet = ErrorTimeout;

        OTS_Start();
        while (u32TimeCnt-- != 0U)
        {
            if (bM4_OTS->CTL_b.OTSST == 0U)
            {
                enRet = Ok;
                break;
            }
        }
        OTS_Stop();

        if (enRet == Ok)
        {
            *pu16Dr1 = M4_OTS->DR1;
            *pu16Dr2 = M4_OTS->DR2;
            *pu16Ecr = M4_OTS->ECR;

            f32Dr1 = (float32_t)(*pu16Dr1);
            f32Dr2 = (float32_t)(*pu16Dr2);

            if (m_u16OtsClkSel == OTS_CLK_HRC)
            {
                f32Ecr = (float32_t)(*pu16Ecr);
            }
            else
            {
                f32Ecr = 1.0f;
            }

            if ((*pu16Dr1 != 0U) && (*pu16Dr2 != 0U) && (*pu16Ecr != 0U))
            {
                *pf32A = (((1.0f / f32Dr1) - (1.0f / f32Dr2)) * f32Ecr);
            }
        }
    }

    return enRet;
}

/**
 * @brief  Calculate the value of temperature.
 * @param  None
 * @retval A float32_t type value of temperature value.
 */
float OTS_CalculateTemp(void)
{
    float32_t f32Ret = 0.0f;
    uint16_t u16Dr1  = M4_OTS->DR1;
    uint16_t u16Dr2  = M4_OTS->DR2;
    uint16_t u16Ecr  = M4_OTS->ECR;
    float32_t f32Dr1 = (float32_t)u16Dr1;
    float32_t f32Dr2 = (float32_t)u16Dr2;
    float32_t f32Ecr = (float32_t)u16Ecr;

    if (m_u16OtsClkSel == OTS_CLK_XTAL)
    {
        f32Ecr = 1.0f;
    }

    if ((u16Dr1 != 0U) && (u16Dr2 != 0U) && (u16Ecr != 0U))
    {
        f32Ret = m_f32ParaK * ((1.0f / f32Dr1) - (1.0f / f32Dr2)) * f32Ecr + m_f32ParaM;
    }

    return f32Ret;
}

/**
 * @}
 */

#endif /* DDL_OTS_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
