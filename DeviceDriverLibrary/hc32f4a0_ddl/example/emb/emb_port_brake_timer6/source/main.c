/**
 *******************************************************************************
 * @file  emb/emb_port_brake_timer6/source/main.c
 * @brief This example demonstrates how to use the port input brake function of 
 *        EMB function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
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
 * @addtogroup EMB_Port_Brake_Timer6
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* EMB unit & fcg & interrupt number definition */
#define EMB_UNIT                        (M4_EMB0)
#define EMB_FUNCTION_CLK_GATE           (PWC_FCG2_EMB)
#define EMB_INT_SRC                     (INT_EMB_GR0)
#define EMB_INT_IRQn                    (Int000_IRQn)

/* EMB Port/Pin definition */
#define EMB_PORT                        (GPIO_PORT_B)    /* PB2: EMB_Port1 */
#define EMB_PIN                         (GPIO_PIN_02)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static uint32_t Tmr6PclkFreq(void);
static void Tmr6PwmConfig(void);
static void EMB_IrqCallback(void);

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
    PWC_Unlock(PWC_UNLOCK_CODE_0);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
//    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
//    EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
//    EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
//    SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
//    EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
//    EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  Get TIMER6 PCLK frequency.
 * @param  None
 * @retval None
 */
static uint32_t Tmr6PclkFreq(void)
{
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    return stcClkFreq.pclk0Freq;
}

/**
 * @brief  Configure Timer6 PWM
 * @param  None
 * @retval None
 */
static void Tmr6PwmConfig(void)
{
    uint32_t u32Period;
    stc_tmr6_basecnt_cfg_t stcTmr6BaseCntCfg;
    stc_tmr6_port_output_cfg_t stcTmr6PortOutCfg;

    TMR6_BaseCntStructInit(&stcTmr6BaseCntCfg);
    TMR6_PortOutputStructInit(&stcTmr6PortOutCfg);

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);

    /* Timer6 PWM output port configuration */
    GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_09, GPIO_FUNC_3_TIM61, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_08, GPIO_FUNC_3_TIM61, PIN_SUBFUNC_DISABLE);

    /* Timer6 general count function configuration */
    stcTmr6BaseCntCfg.u32CntMode = TMR6_MODE_SAWTOOTH;
    stcTmr6BaseCntCfg.u32CntDir = TMR6_CNT_INCREASE;
    stcTmr6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV128;
    stcTmr6BaseCntCfg.u32CntStpAftOvf = TMR6_CNT_CONTINUOUS;
    TMR6_Init(M4_TMR6_1, &stcTmr6BaseCntCfg);

    /* Set Period register/Compare RegisterA/Compare RegisterB Value */
    u32Period = Tmr6PclkFreq() / (2UL * (1UL << (uint32_t)(stcTmr6BaseCntCfg.u32CntClkDiv >> TMR6_GCONR_CKDIV_POS))); /* Period_Value(500ms) */
    TMR6_SetPeriodReg(M4_TMR6_1, TMR6_PERIOD_REG_A, u32Period);
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, u32Period/2UL);
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_B, u32Period/2UL);

     /* Configurate PWM output */
    stcTmr6PortOutCfg.u32PortMode = TMR6_PORT_COMPARE_OUTPUT;
    stcTmr6PortOutCfg.u32NextPeriodForceSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTmr6PortOutCfg.u32DownCntMatchAnotherCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTmr6PortOutCfg.u32UpCntMatchAnotherCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTmr6PortOutCfg.u32DownCntMatchCmpRegSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTmr6PortOutCfg.u32UpCntMatchCmpRegSta = TMR6_PORT_OUTPUT_STA_HIGH;
    stcTmr6PortOutCfg.u32UnderflowSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTmr6PortOutCfg.u32OverflowSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTmr6PortOutCfg.u32StopSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTmr6PortOutCfg.u32StartSta = TMR6_PORT_OUTPUT_STA_LOW;
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTmr6PortOutCfg);
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMB, &stcTmr6PortOutCfg);

    /* Start Timer6 */
    TMR6_CountCmd(M4_TMR6_1, Enable);
}

/**
 * @brief  EMB IRQ callback function.
 * @param  None
 * @retval None
 */
static void EMB_IrqCallback(void)
{
    if (Set == EMB_GetFlag(EMB_UNIT, EMB_FLAG_PORT1))
    {
        EMB_ClearFlag(EMB_UNIT, EMB_FLAG_PORT1);  /* Clear Port  Brake */
    }
}

/**
 * @brief  Main function of EMB port input brake
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_tmr6_emb_cfg_t stcTmr6EmbCfg;
    stc_emb_tmr6_init_t stcEmbInit;
    stc_irq_signin_config_t stcIrqSigninCfg;

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Configure Timer6 PWM. */
    Tmr6PwmConfig();

    /* Initialize EMB I/O */
    GPIO_SetFunc(EMB_PORT, EMB_PIN, GPIO_FUNC_6_EMB_PORT, PIN_SUBFUNC_DISABLE);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Configure Timer6 EMB. */
    TMR6_EMBCfgStructInit(&stcTmr6EmbCfg);
    stcTmr6EmbCfg.u32ValidCh = TMR6_EMB_EVENT_VALID_CH0;
    stcTmr6EmbCfg.u32ReleaseMode = TMR6_EMB_RELEASE_IMMEDIATE;
    stcTmr6EmbCfg.u32PortSta = TMR6_EMB_PORTSTA_LOW;
    TMR6_EMBConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTmr6EmbCfg);
    TMR6_EMBConfig(M4_TMR6_1, TMR6_IO_PWMB, &stcTmr6EmbCfg);

    /* Configure EMB. */
    PWC_Fcg2PeriphClockCmd(EMB_FUNCTION_CLK_GATE, Enable);
    EMB_Tmr6StructInit(&stcEmbInit);
    stcEmbInit.stcPort1.u32PortSel = EMB_PORT1_ENABLE;
    stcEmbInit.stcPort1.u32PortLevel = EMB_DETECT_PORT1_LEVEL_LOW;
    stcEmbInit.stcPort1.u32PortFilterDiv = EMB_PORT1_FILTER_CLK_DIV8;
    EMB_Tmr6Init(EMB_UNIT, &stcEmbInit);

    EMB_IntCmd(EMB_UNIT, EMB_INT_PORT1, Enable);
    EMB_SetReleasePwmMode(EMB_UNIT, EMB_EVENT_PORT1, EMB_RELEASE_PWM_SEL_FLAG_ZERO);

    /* Register IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = EMB_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = EMB_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &EMB_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    while (1)
    {
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
