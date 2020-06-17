/**
 *******************************************************************************
 * @file  emb/emb_pwm_brake_timer4/source/main.c
 * @brief This example demonstrates how to use the PWM same phase brake function
 *        of EMB function.
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
 * @addtogroup EMB_PWM_Brake_Timer4
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Key port&pin definition */
#define KEY_PORT                        (GPIO_PORT_A)
#define KEY_PIN                         (GPIO_PIN_00)

/* EMB unit & fcg & interrupt number definition */
#define EMB_UNIT                        (M4_EMB4)
#define EMB_FUNCTION_CLK_GATE           (PWC_FCG2_EMB)
#define EMB_INT_SRC                     (INT_EMB_GR4)
#define EMB_INT_IRQn                    (Int000_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static en_flag_status_t KeyState(void);
static uint32_t Tmr4PclkFreq(void);
static void Tmr4PwmConfig(void);
static void EMB_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static __IO uint8_t m_u8EmbFlag = 0U;

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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
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
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
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
 * @brief  Get key state
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Set: Released after key is pressed
 *           - Reset: Key isn't pressed
 */
static en_flag_status_t KeyState(void)
{
    en_flag_status_t enKeyState = Reset;

    if (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
    {
        DDL_DelayMS(50UL);

        if (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
        {
            while (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
            {
            }
            enKeyState = Set;
        }
    }

    return enKeyState;
}

/**
 * @brief  Get TIMER4 PCLK frequency.
 * @param  None
 * @retval None
 */
static uint32_t Tmr4PclkFreq(void)
{
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    return stcClkFreq.pclk0Freq;
}

/**
 * @brief  Configure Timer4 PWM
 * @param  None
 * @retval None
 */
static void Tmr4PwmConfig(void)
{
    stc_tmr4_cnt_init_t stcTmr4CntInit;
    stc_tmr4_oco_init_t stcTmr4OcoInit;
    stc_tmr4_pwm_init_t stcTmr4PwmInit;
    stc_oco_high_ch_compare_mode_t stcHighChCmpMode;
    stc_oco_low_ch_compare_mode_t stcLowChCmpMode;

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR4_1, Enable);

    /* Initialize TIMER4 Counter */
    TMR4_CNT_StructInit(&stcTmr4CntInit);
    stcTmr4CntInit.u16PclkDiv = TMR4_CNT_PCLK_DIV1024;
    stcTmr4CntInit.u16CycleVal = (uint16_t)(Tmr4PclkFreq() / (4UL * (1UL << (uint32_t)(stcTmr4CntInit.u16PclkDiv)))); /* Period_Value(500ms) */
    TMR4_CNT_Init(M4_TMR4_1, &stcTmr4CntInit);

    /* Initialize TIMER4 OCO high&&low channel */
    TMR4_OCO_StructInit(&stcTmr4OcoInit);
    stcTmr4OcoInit.u16CompareVal = stcTmr4CntInit.u16CycleVal/2U;
    TMR4_OCO_Init(M4_TMR4_1, TMR4_OCO_UH, &stcTmr4OcoInit);
    TMR4_OCO_Init(M4_TMR4_1, TMR4_OCO_UL, &stcTmr4OcoInit);

    /* OCMR[15:0] = 0x0FFF = b 0000 1111 1111 1111 */
    stcHighChCmpMode.OCMRx_f.OCFDCH = TMR4_OCO_OCF_SET;   /* bit[0] 1 */
    stcHighChCmpMode.OCMRx_f.OCFPKH = TMR4_OCO_OCF_SET;   /* bit[1] 1 */
    stcHighChCmpMode.OCMRx_f.OCFUCH = TMR4_OCO_OCF_SET;   /* bit[2] 1 */
    stcHighChCmpMode.OCMRx_f.OCFZRH = TMR4_OCO_OCF_SET;   /* bit[3] 1 */
    stcHighChCmpMode.OCMRx_f.OPDCH = TMR4_OCO_OP_INVERT;  /* Bit[5:4]    11 */
    stcHighChCmpMode.OCMRx_f.OPPKH = TMR4_OCO_OP_INVERT;  /* Bit[7:6]    11 */
    stcHighChCmpMode.OCMRx_f.OPUCH = TMR4_OCO_OP_INVERT;  /* Bit[9:8]    11 */
    stcHighChCmpMode.OCMRx_f.OPZRH = TMR4_OCO_OP_INVERT;  /* Bit[11:10]  11 */
    stcHighChCmpMode.OCMRx_f.OPNPKH = TMR4_OCO_OP_HOLD;   /* Bit[13:12]  00 */
    stcHighChCmpMode.OCMRx_f.OPNZRH = TMR4_OCO_OP_HOLD;   /* Bit[15:14]  00 */
    stcHighChCmpMode.u16ExtendMatch = TMR4_OCO_EXTEND_MATCH_DISABLE;
    TMR4_OCO_SetHighChCompareMode(M4_TMR4_1, TMR4_OCO_UH, &stcHighChCmpMode);  /* Set OCO high channel compare mode */

    /* OCMR[31:0] 0x0FF0 0FFF = b 0000 1111 1111 0000   0000 1111 1111 1111 */
    stcLowChCmpMode.OCMRx_f.OCFDCL = TMR4_OCO_OCF_SET;    /* bit[0] 1 */
    stcLowChCmpMode.OCMRx_f.OCFPKL = TMR4_OCO_OCF_SET;    /* bit[1] 1 */
    stcLowChCmpMode.OCMRx_f.OCFUCL = TMR4_OCO_OCF_SET;    /* bit[2] 1 */
    stcLowChCmpMode.OCMRx_f.OCFZRL = TMR4_OCO_OCF_SET;    /* bit[3] 1 */
    stcLowChCmpMode.OCMRx_f.OPDCL = TMR4_OCO_OP_INVERT;   /* bit[5:4]    11 */
    stcLowChCmpMode.OCMRx_f.OPPKL = TMR4_OCO_OP_INVERT;   /* bit[7:6]    11 */
    stcLowChCmpMode.OCMRx_f.OPUCL = TMR4_OCO_OP_INVERT;   /* bit[9:8]    11 */
    stcLowChCmpMode.OCMRx_f.OPZRL = TMR4_OCO_OP_INVERT;   /* bit[11:10]  11 */
    stcLowChCmpMode.OCMRx_f.OPNPKL = TMR4_OCO_OP_HOLD;    /* bit[13:12]  00 */
    stcLowChCmpMode.OCMRx_f.OPNZRL = TMR4_OCO_OP_HOLD;    /* bit[15:14]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNDCL = TMR4_OCO_OP_HOLD;   /* bit[17:16]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNUCL = TMR4_OCO_OP_HOLD;   /* bit[19:18]  00 */
    stcLowChCmpMode.OCMRx_f.EOPDCL = TMR4_OCO_OP_INVERT;  /* bit[21:20]  11 */
    stcLowChCmpMode.OCMRx_f.EOPPKL = TMR4_OCO_OP_INVERT;  /* bit[23:22]  11 */
    stcLowChCmpMode.OCMRx_f.EOPUCL = TMR4_OCO_OP_INVERT;  /* bit[25:24]  11 */
    stcLowChCmpMode.OCMRx_f.EOPZRL = TMR4_OCO_OP_INVERT;  /* bit[27:26]  11 */
    stcLowChCmpMode.OCMRx_f.EOPNPKL = TMR4_OCO_OP_HOLD;   /* bit[29:28]  00 */
    stcLowChCmpMode.OCMRx_f.EOPNZRL = TMR4_OCO_OP_HOLD;   /* bit[31:30]  00 */
    stcLowChCmpMode.u16ExtendMatch = TMR4_OCO_EXTEND_MATCH_DISABLE;
    TMR4_OCO_SetLowChCompareMode(M4_TMR4_1, TMR4_OCO_UL, &stcLowChCmpMode);  /* Set OCO low channel compare mode */

    /* Initialize PWM I/O */
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_09, GPIO_FUNC_2, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_08, GPIO_FUNC_2, PIN_SUBFUNC_DISABLE);

    /* Initialize Timer4 PWM */
    TMR4_PWM_StructInit(&stcTmr4PwmInit);
    stcTmr4PwmInit.u16TransformOcoPol = TMR4_PWM_OP_OXH_HOLD_OXL_INVERT;
    stcTmr4PwmInit.u32OxHPortOutMode = TMR4_PWM_PORT_OUTPUT_NORMAL;
    stcTmr4PwmInit.u32EmbOxHPortState = TMR4_PWM_EMB_PORT_OUTPUT_LOW;
    stcTmr4PwmInit.u32OxLPortOutMode = TMR4_PWM_PORT_OUTPUT_NORMAL;
    stcTmr4PwmInit.u32EmbOxLPortState = TMR4_PWM_EMB_PORT_OUTPUT_LOW;
    TMR4_PWM_Init(M4_TMR4_1, TMR4_PWM_U, &stcTmr4PwmInit);
    TMR4_PWM_MasterOutputCmd(M4_TMR4_1, Enable);
    TMR4_PWM_AutoOutputCmd(M4_TMR4_1, Enable);

    /* Start TIMER4 counter. */
    TMR4_CNT_Start(M4_TMR4_1);
}

/**
 * @brief  EMB IRQ callback function
 * @param  None
 * @retval None
 */
static void EMB_IrqCallback(void)
{
    if (Set == EMB_GetFlag(EMB_UNIT, EMB_FLAG_PWMS))
    {
        m_u8EmbFlag = 1U;

        /* Wait key pressed */
        while (Reset == KeyState())
        {
        }

        TMR4_PWM_SetOcoPolarityTransform(M4_TMR4_1,
                                         TMR4_PWM_U,
                                         TMR4_PWM_OP_OXH_HOLD_OXL_INVERT);

        EMB_ClearFlag(EMB_UNIT, EMB_FLAG_PWMS);  /* Clear PWM Brake */
    }
}

/**
 * @brief  Main function of EMB PWM brake
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_emb_tmr4_init_t stcEmbInit;
    stc_irq_signin_config_t stcIrqSigninCfg;

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO. */
    BSP_IO_Init();

    /* Configure Timer4 PWM. */
    Tmr4PwmConfig();

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Configure EMB. */
    PWC_Fcg2PeriphClockCmd(EMB_FUNCTION_CLK_GATE, Enable);
    EMB_Tmr4StructInit(&stcEmbInit);
    stcEmbInit.stcTmr4PwmU.u32PwmSel = EMB_TMR4_PWM_U_ENABLE;
    stcEmbInit.stcTmr4PwmU.u32PwmLevel = EMB_DETECT_TMR4_PWM_U_BOTH_HIGH;
    EMB_Tmr4Init(EMB_UNIT, &stcEmbInit);

    EMB_IntCmd(EMB_UNIT, EMB_INT_PWMS, Enable);
    EMB_SetReleasePwmMode(EMB_UNIT, EMB_EVENT_PWMS, EMB_RELEASE_PWM_SEL_FLAG_ZERO);

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
        /* Wait key pressed */
        while (Reset == KeyState())
        {
        }

        TMR4_PWM_SetOcoPolarityTransform(M4_TMR4_1,
                                         TMR4_PWM_U,
                                         TMR4_PWM_OP_OXH_HOLD_OXL_HOLD);

        while (!m_u8EmbFlag)
        {
        }

        m_u8EmbFlag = 0U;
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
