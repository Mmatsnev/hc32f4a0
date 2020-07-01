/**
 *******************************************************************************
 * @file  hrpwm/hrpwm_output/source/main.c
 * @brief This example demonstrates HRPWM output function with timer6 PWM.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin          First version
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
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup TMR6_Hrpwm_Output
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define         TMR6_1_PWMA_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMA_PIN        (GPIO_PIN_09)
#define         TMR6_1_PWMB_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMB_PIN        (GPIO_PIN_08)

#define         TMR6_2_PWMA_PORT       (GPIO_PORT_B)
#define         TMR6_2_PWMA_PIN        (GPIO_PIN_07)
#define         TMR6_2_PWMB_PORT       (GPIO_PORT_B)
#define         TMR6_2_PWMB_PIN        (GPIO_PIN_14)

#define         TMR6_3_PWMA_PORT       (GPIO_PORT_A)
#define         TMR6_3_PWMA_PIN        (GPIO_PIN_10)
#define         TMR6_3_PWMB_PORT       (GPIO_PORT_B)
#define         TMR6_3_PWMB_PIN        (GPIO_PIN_15)

#define         TMR6_4_PWMA_PORT       (GPIO_PORT_E)
#define         TMR6_4_PWMA_PIN        (GPIO_PIN_14)
#define         TMR6_4_PWMB_PORT       (GPIO_PORT_B)
#define         TMR6_4_PWMB_PIN        (GPIO_PIN_10)

#define         TMR6_5_PWMA_PORT       (GPIO_PORT_C)
#define         TMR6_5_PWMA_PIN        (GPIO_PIN_10)
#define         TMR6_5_PWMB_PORT       (GPIO_PORT_D)
#define         TMR6_5_PWMB_PIN        (GPIO_PIN_00)

#define         TMR6_6_PWMA_PORT       (GPIO_PORT_A)
#define         TMR6_6_PWMA_PIN        (GPIO_PIN_02)
#define         TMR6_6_PWMB_PORT       (GPIO_PORT_A)
#define         TMR6_6_PWMB_PIN        (GPIO_PIN_03)

#define         TMR6_7_PWMA_PORT       (GPIO_PORT_A)
#define         TMR6_7_PWMA_PIN        (GPIO_PIN_04)
#define         TMR6_7_PWMB_PORT       (GPIO_PORT_A)
#define         TMR6_7_PWMB_PIN        (GPIO_PIN_05)

#define         TMR6_8_PWMA_PORT       (GPIO_PORT_C)
#define         TMR6_8_PWMA_PIN        (GPIO_PIN_02)
#define         TMR6_8_PWMB_PORT       (GPIO_PORT_C)
#define         TMR6_8_PWMB_PIN        (GPIO_PIN_03)

/* Close the definition to disale the HRPWM function for the example. */
#define         EXAM_HRPWM_FUNC_ON      1U
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
 * @brief  MCU Peripheral registers write unprotected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WE(void)
{
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    //EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static __attribute__((unused)) void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  Main function of TIMER6 compare output mode project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t                       u32Period;
    uint32_t                       u32Compare;
    stc_tmr6_basecnt_cfg_t         stcTIM6BaseCntCfg;
    stc_tmr6_port_output_cfg_t     stcTIM6PortOutCfg;
    stc_gpio_init_t                stcGpioInit;
    stc_clk_freq_t                 stcClkFreq;
    uint8_t                        u8CalCode0, u8CalCode1;
    __UNUSED float                 fPrecision0, fPrecision1;

    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    TMR6_BaseCntStructInit(&stcTIM6BaseCntCfg);
    TMR6_PortOutputStructInit(&stcTIM6PortOutCfg);
    GPIO_StructInit(&stcGpioInit);

#ifdef EXAM_HRPWM_FUNC_ON
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_HRPWM, Enable);

    CLK_GetClockFreq(&stcClkFreq);

    /* Make sure that F(PCLK0) > 120M */
    if(Disable == HRPWM_ConditionConfirm())
    {
        BSP_LED_On(LED_RED);
        while(1);
    }

    if(Ok != HRPWM_CalibrateProcess(HRPWM_CAL_UNIT0, &u8CalCode0))
    {
        BSP_LED_On(LED_RED);
        while(1);
    }

    if(Ok != HRPWM_CalibrateProcess(HRPWM_CAL_UNIT1, &u8CalCode1))
    {
        BSP_LED_On(LED_RED);
        while(1);
    }

    /* Calculate calibrate precision for channel 1~channel 12, unit nanosecond */
    fPrecision0 = (float)1000000000/(float)stcClkFreq.pclk0Freq/(float)u8CalCode0;
    /* Calculate calibrate precision for channel 13~channel 16, unit nanosecond */
    fPrecision1 = (float)1000000000/(float)stcClkFreq.pclk0Freq/(float)u8CalCode1;

    /* HRPWM function configurate for channel 1, PB9*/
    HRPWM_CHPositCfg(1U, 1U);
    HRPWM_CHPositAdjCmd(1U, Enable);
    HRPWM_CHNegatCfg(1U, 1U);
    HRPWM_CHNegatAdjCmd(1U, Enable);
    HRPWM_CHCmd(1U, Enable);

    /* HRPWM function configurate for channel 5, PA10*/
    HRPWM_CHPositCfg(5U, u8CalCode0/2U);
    HRPWM_CHPositAdjCmd(5U, Enable);
    HRPWM_CHNegatCfg(5U, u8CalCode0/2U);
    HRPWM_CHNegatAdjCmd(5U, Enable);
    HRPWM_CHCmd(5U, Enable);

#endif
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_2, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_3, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_4, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_5, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_6, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_7, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_8, Enable);

    /* Timer6 PWM port configuration */
    GPIO_SetFunc(TMR6_1_PWMA_PORT, TMR6_1_PWMA_PIN, GPIO_FUNC_3_TIM61_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_1_PWMB_PORT, TMR6_1_PWMB_PIN, GPIO_FUNC_3_TIM61_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_2_PWMA_PORT, TMR6_2_PWMA_PIN, GPIO_FUNC_3_TIM62_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_2_PWMB_PORT, TMR6_2_PWMB_PIN, GPIO_FUNC_3_TIM62_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_3_PWMA_PORT, TMR6_3_PWMA_PIN, GPIO_FUNC_3_TIM63_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_3_PWMB_PORT, TMR6_3_PWMB_PIN, GPIO_FUNC_3_TIM63_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_4_PWMA_PORT, TMR6_4_PWMA_PIN, GPIO_FUNC_3_TIM64_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_4_PWMB_PORT, TMR6_4_PWMB_PIN, GPIO_FUNC_3_TIM64_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_5_PWMA_PORT, TMR6_5_PWMA_PIN, GPIO_FUNC_3_TIM65_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_5_PWMB_PORT, TMR6_5_PWMB_PIN, GPIO_FUNC_3_TIM65_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_6_PWMA_PORT, TMR6_6_PWMA_PIN, GPIO_FUNC_3_TIM66_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_6_PWMB_PORT, TMR6_6_PWMB_PIN, GPIO_FUNC_3_TIM66_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_7_PWMA_PORT, TMR6_7_PWMA_PIN, GPIO_FUNC_3_TIM67_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_7_PWMB_PORT, TMR6_7_PWMB_PIN, GPIO_FUNC_3_TIM67_PWMB, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(TMR6_8_PWMA_PORT, TMR6_8_PWMA_PIN, GPIO_FUNC_3_TIM68_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_8_PWMB_PORT, TMR6_8_PWMB_PIN, GPIO_FUNC_3_TIM68_PWMB, PIN_SUBFUNC_DISABLE);

    /* Timer6 general count function configuration */
    stcTIM6BaseCntCfg.u32CntMode = TMR6_MODE_TRIANGLE;
    stcTIM6BaseCntCfg.u32CntDir = TMR6_CNT_INCREASE;
    stcTIM6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV1;
    stcTIM6BaseCntCfg.u32CntStpAftOvf = TMR6_CNT_CONTINUOUS;
    TMR6_Init(M4_TMR6_1, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_2, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_3, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_4, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_5, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_6, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_7, &stcTIM6BaseCntCfg);
    TMR6_Init(M4_TMR6_8, &stcTIM6BaseCntCfg);

    /* Period register set */
    u32Period = 50U;
    TMR6_SetPeriodReg(M4_TMR6_1, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_2, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_3, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_4, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_5, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_6, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_7, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetPeriodReg(M4_TMR6_8, TMR6_PERIOD_REG_A, u32Period);

    /* Compare register set */
    u32Compare = u32Period-8UL;
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_2, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_3, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_4, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_5, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_6, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_7, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_8, TMR6_CMP_REG_A, u32Compare);

    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_2, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_3, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_4, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_5, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_6, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_7, TMR6_CMP_REG_B, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_8, TMR6_CMP_REG_B, u32Compare);

     /* Configurate PWM output */
    stcTIM6PortOutCfg.u32PortMode = TMR6_PORT_COMPARE_OUTPUT;
    stcTIM6PortOutCfg.u32NextPeriodForceSta = TMR6_FORCE_PORT_OUTPUT_INVALID;
    stcTIM6PortOutCfg.u32DownCntMatchAnotherCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32UpCntMatchAnotherCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32DownCntMatchCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32UpCntMatchCmpRegSta = TMR6_PORT_OUTPUT_STA_REVERSE;
    stcTIM6PortOutCfg.u32UnderflowSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32OverflowSta = TMR6_PORT_OUTPUT_STA_REVERSE;
    stcTIM6PortOutCfg.u32StopSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTIM6PortOutCfg.u32StartSta = TMR6_PORT_OUTPUT_STA_LOW;
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_2, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_3, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_4, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_5, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_6, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_7, TMR6_IO_PWMA, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_8, TMR6_IO_PWMA, &stcTIM6PortOutCfg);

    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_2, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_3, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_4, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_5, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_6, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_7, TMR6_IO_PWMB, &stcTIM6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_8, TMR6_IO_PWMB, &stcTIM6PortOutCfg);

    /* Start timer6 */
    TMR6_SwSyncStart(TMR6_SOFT_SYNC_CTRL_U1 | TMR6_SOFT_SYNC_CTRL_U2 | TMR6_SOFT_SYNC_CTRL_U3
                     |TMR6_SOFT_SYNC_CTRL_U4 | TMR6_SOFT_SYNC_CTRL_U5 | TMR6_SOFT_SYNC_CTRL_U6
                     |TMR6_SOFT_SYNC_CTRL_U7 | TMR6_SOFT_SYNC_CTRL_U8);
    while(1)
    {
        ;
    }

}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
