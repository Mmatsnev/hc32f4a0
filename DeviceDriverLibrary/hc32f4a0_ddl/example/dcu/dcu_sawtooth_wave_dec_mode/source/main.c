/**
 *******************************************************************************
 * @file  dcu/dcu_sawtooth_wave_dec_mode/source/main.c
 * @brief This example demonstrates DCU sawtooth wave mode function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
   2020-07-15       Hongjh          Replace DAC_ChannelCmd by DAC_Start
   2020-07-23       Hongjh          1. Replace DCU_IntFuncCmd by DCU_GlobalIntCmd;
                                    2. Modify paramters after refine DCU_IntCmd.
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
 * @addtogroup DCU_Sawtooth_Wave_Decreasing_Mode
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DCU unit definition */
#define DCU_UNIT                        (M4_DCU1)
#define DCU_WAVE_UPPER_LIMIT            (3999U)
#define DCU_FUNCTION_CLK_GATE           (PWC_FCG0_DCU1)

/* DCU unit interrupt definition */
#define DCU_INT_SRC                     (INT_DCU1)
#define DCU_INT_IRQn                    (Int000_IRQn)

/* TMR0 1s count value definition */
#define TMR0_CNT_1S_VAL(div)            ((Tmr0PclkFreq() / (div))) /* Period_Value(1000ms) */

/* DAC unit definition */
#define DAC_UNIT                        (M4_DAC1)
#define DAC_CH                          (DAC_CH_1)
#define DAC_FUNCTION_CLK_GATE           (PWC_FCG3_DAC1)

/* DAC unit definition */
#define DAC_PORT                        (GPIO_PORT_A)
#define DAC_PIN                         (GPIO_PIN_04)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static uint32_t Tmr0PclkFreq(void);
static void TMR0_CmpIrqCallback(void);
static void Tmr0Config(void);
static void DCU_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static __IO uint16_t m_u16Timer0IrqCnt = 0U;

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
 * @brief  Get TIMER4 PCLK frequency.
 * @param  None
 * @retval None
 */
static uint32_t Tmr0PclkFreq(void)
{
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    return stcClkFreq.pclk1Freq;
}

/**
 * @brief  TMR0_1 channelA compare IRQ callback
 * @param  None
 * @retval None
 */
static void TMR0_CmpIrqCallback(void)
{
    if (DCU_WAVE_UPPER_LIMIT == m_u16Timer0IrqCnt++)
    {
        m_u16Timer0IrqCnt = 0U;
        DCU_ClearStatus(DCU_UNIT, DCU_FLAG_WAVE_SAWTOOTH_RELOAD);
        DCU_GlobalIntCmd(DCU_UNIT, Enable);
    }

    /* Clear the compare matching flag */
    TMR0_ClearStatus(M4_TMR0_1, TMR0_CH_A);
}

/**
 * @brief  Configure TMR0.
 * @param  None
 * @retval None
 */
static void Tmr0Config(void)
{
    stc_tmr0_init_t stcTmr0Init;
    stc_irq_signin_config_t stcIrqSignConfig;

    /* Enable timer0 peripheral clock */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR0_1, Enable);

    /* TIMER0 basetimer function initialize */
    TMR0_StructInit(&stcTmr0Init);
    stcTmr0Init.u32ClockDivision = TMR0_CLK_DIV1;          /* Config clock division */
    stcTmr0Init.u32ClockSource = TMR0_CLK_SRC_PCLK1;        /* Chose clock source */
    stcTmr0Init.u32Tmr0Func = TMR0_FUNC_CMP;                /* Timer0 compare mode */
    stcTmr0Init.u16CmpValue = (uint16_t)(TMR0_CNT_1S_VAL(1UL)/(2UL * (DCU_WAVE_UPPER_LIMIT + 1UL)));
    TMR0_Init(M4_TMR0_1, TMR0_CH_A, &stcTmr0Init);
    TMR0_IntCmd(M4_TMR0_1, TMR0_CH_A, Enable);

    /* Register IRQ handler && configure NVIC. */
    stcIrqSignConfig.enIRQn = Int014_IRQn;
    stcIrqSignConfig.enIntSrc = INT_TMR0_1_CMPA;
    stcIrqSignConfig.pfnCallback = &TMR0_CmpIrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);

    /* Start timer*/
    TMR0_Cmd(M4_TMR0_1, TMR0_CH_A, Enable);
}

/**
 * @brief  Configure DAC.
 * @param  None
 * @retval None
 */
static void DacConfig(void)
{
    stc_dac_init_t stcInit;
    stc_gpio_init_t stcGpioInit;

    /* Set DAC pin */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    GPIO_Init(DAC_PORT, DAC_PIN, &stcGpioInit);

    /* Enable DAC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(DAC_FUNCTION_CLK_GATE, Enable);

    /* Initialize DAC */
    stcInit.enOutput = Enable;
    stcInit.u16Src = DAC_DATA_SRC_DCU;
    DAC_Init(DAC_UNIT, DAC_CH, &stcInit);

    /* Set DAC data alignment */
    DAC_DataRegAlignConfig(DAC_UNIT, DAC_DATA_ALIGN_R);

    /* Enable DAC */
    DAC_Start(DAC_UNIT, DAC_CH);
}

/**
 * @brief  DCU irq callback function.
 * @param  None
 * @retval None
 */
static void DCU_IrqCallback(void)
{
    GPIO_TogglePins(GPIO_PORT_E, GPIO_PIN_04);

    DCU_GlobalIntCmd(DCU_UNIT, Disable);
    DCU_ClearStatus(DCU_UNIT, DCU_FLAG_WAVE_SAWTOOTH_RELOAD);
}

/**
 * @brief  Main function of DCU decreasing sawtooth wave mode project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_dcu_init_t stcDcuInit;
    stc_dcu_wave_cfg_t stcCfg;
    stc_gpio_init_t stcGpioInit;
    stc_irq_signin_config_t stcIrqSigninCfg;

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO for testing DCU reload time. */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    GPIO_Init(GPIO_PORT_E, GPIO_PIN_04, &stcGpioInit);

    /* Enable peripheral clock: DCU  */
    PWC_Fcg0PeriphClockCmd((DCU_FUNCTION_CLK_GATE | PWC_FCG0_AOS), Enable);

    /* Configure DCU wave amplitude && step */
    stcCfg.u32LowerLimit = 0x000UL;
    stcCfg.u32UpperLimit = DCU_WAVE_UPPER_LIMIT;
    stcCfg.u32Step = 0x1UL;
    DCU_WaveCfg(DCU_UNIT, &stcCfg);

    /* Initialize DCU increasing sawtooth mode */
    DCU_StructInit(&stcDcuInit);
    stcDcuInit.u32IntEn = DCU_INT_ENABLE;
    stcDcuInit.u32Mode = DCU_SAWTOOTH_WAVE_DEC;
    stcDcuInit.u32DataSize = DCU_DATA_SIZE_16BIT;
    DCU_Init(DCU_UNIT, &stcDcuInit);
    DCU_IntCmd(DCU_UNIT, DCU_INT_WAVE_MD, DCU_INT_WAVE_SAWTOOTH_RELOAD, Enable);

    stcIrqSigninCfg.enIRQn = DCU_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = DCU_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &DCU_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_00);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    /* Set hardware trigger source */
    DCU_SetTriggerSrc(DCU_UNIT, EVT_TMR0_1_CMPA);

    /* Initialize DAC. */
    DacConfig();

    /* Initialize Timer0. */
    Tmr0Config();

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

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
