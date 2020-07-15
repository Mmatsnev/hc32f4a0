/**
 *******************************************************************************
 * @file  timer6/timer6_valid_period/source/main.c
 * @brief This example demonstrates Timer6 compare output valid period function. 
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin         First version
   2020-07-15       Wangmin         Modify after Refine macro define in driver
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
 * @addtogroup TMR6_Valid_Period
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define SCMA_ValidPeriod  1

#define         TMR6_1_PWMA_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMA_PIN        (GPIO_PIN_09)

#define         TEST_GPIO_PORT         (GPIO_PORT_B)
#define         TEST_GPIO_PIN          (GPIO_PIN_08)

/*
 * ADC unit instance for this example.
 * Marco 'APP_ADC_UNIT' can be defined as M4_ADCx(x=1, 2, 3).
 * Definition of 'APP_PERIP_CLK_ADC' depends on 'APP_ADC_UNIT'.
 */
#define APP_ADC_UNIT                        (M4_ADC1)
#define APP_PERIP_CLK_ADC                   (PWC_FCG3_ADC1)
#define APP_FCG3_PERIP_CLK                  (APP_PERIP_CLK_ADC)

/* ADC channel sampling time.
 * NOTE!!! ADC_CH16 ~ ADC_CH19 of ADC3 share one sampling time.
                                          ADC_CH0    ADC_CH2   ADC_CH4   APP_POT_CH */
#define APP_ADC_SA_SAMPLE_TIME              { 30,       30,       35,         45 }

/*
 * ADC channels definition for this example.
 * 'APP_POT_CH' is the potentiometer input channel on the EV-Kit.
 * NOTE!!! Sequence A and sequence B CANNOT contain the same channel.
 */
#define APP_POT_CH                          (ADC_CH9)
#define APP_ADC_SA_CH                       (ADC_CH0 | ADC_CH2 | ADC_CH4 | APP_POT_CH)
#define APP_ADC_SA_CH_COUNT                 (4U)


/*
 * Definitions of interrupt.
 * All interrupts of ADC can use independent interrupt vectors Int000_IRQn ~ Int031_IRQn and Int122_IRQn ~ Int127_IRQn,
 *     sharing interrupt vector Int143_IRQn.
 */
#define APP_ADC_SA_IRQ_CB               ADC_1_SeqA_IrqHandler
#define APP_ADC_SA_IRQ_SRC              (INT_ADC1_EOCA)
#define APP_ADC_SA_IRQn                 (Int016_IRQn)
#define APP_ADC_SA_INT_PRIO             (DDL_IRQ_PRIORITY_03)


/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcIrqConfig(void);
static void AdcTrigSrcConfig(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

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
 * @brief  TIMER6 interrupt handler callback.
 * @param  None
 * @retval None
 */
void Tmr6_CallBack(void)
{
    static uint8_t i;

    if( 0U == i)
    {
        TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, 0x3000U);
#ifdef SCMA_ValidPeriod
        TMR6_SetSpecialCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, 0x3000U);
#endif

        i = 1U;
    }
    else
    {
        TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, 0x6000U);
#ifdef SCMA_ValidPeriod
        TMR6_SetSpecialCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, 0x6000U);
#endif

        i = 0U;
    }
}

/**
 * @brief  ADC interrupt handler callback.
 * @param  None
 * @retval None
 */
void ADC_1_SeqA_IrqHandler()
{
    GPIO_TogglePins(TEST_GPIO_PORT, TEST_GPIO_PIN);
}

/**
 * @brief  Configures ADC clock.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{

}

/**
 * @brief  Initializes ADC.
 * @param  None
 * @retval None
 */
static void AdcInitConfig(void)
{
    stc_adc_init_t stcInit;

    /* Set a default value. */
    ADC_StructInit(&stcInit);

    /* 1. Modify the default value depends on the application. */
    stcInit.u16ScanMode = ADC_MODE_SA_SSHOT;

    /* 2. Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_FCG3_PERIP_CLK, Enable);

    /* 3. Initializes ADC. */
    ADC_Init(APP_ADC_UNIT, &stcInit);
}

/**
 * @brief  Set the pin(s) corresponding to the specified channel(s) to analog mode.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  M4_ADC1:                     ADC unit 1 instance register base.
 *   @arg  M4_ADC2:                     ADC unit 2 instance register base.
 *   @arg  M4_ADC3:                     ADC unit 3 instance register base.
 * @param  [in]  u32Channel             The specified channel(s).
 * @retval None
 */
static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel)
{
    uint8_t u8PinNum;

    u8PinNum = 0U;
    while (u32Channel != 0U)
    {
        if (u32Channel & 0x1U)
        {
            AdcSetPinAnalogMode(ADCx, u8PinNum);
        }

        u32Channel >>= 1U;
        u8PinNum++;
    }
}

/**
 * @brief  Set specified ADC pin to analog mode.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  M4_ADC1:                     ADC unit 1 instance register base.
 *   @arg  M4_ADC2:                     ADC unit 2 instance register base.
 *   @arg  M4_ADC3:                     ADC unit 3 instance register base.
 * @param  [in]  u8PinNum               The ADC pin number.
 *                                      This parameter can be a value of @ref ADC_Pin_Number
 * @retval None
 */
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum)
{
    typedef struct
    {
        uint8_t  u8Port;
        uint16_t u16Pin;
    } stc_adc_pin_t;

    stc_gpio_init_t stcGpioInit;

    stc_adc_pin_t astcADC12[ADC1_CH_COUNT] = { \
            {GPIO_PORT_A, GPIO_PIN_00}, {GPIO_PORT_A, GPIO_PIN_01}, \
            {GPIO_PORT_A, GPIO_PIN_02}, {GPIO_PORT_A, GPIO_PIN_03}, \
            {GPIO_PORT_A, GPIO_PIN_04}, {GPIO_PORT_A, GPIO_PIN_05}, \
            {GPIO_PORT_A, GPIO_PIN_06}, {GPIO_PORT_A, GPIO_PIN_07}, \
            {GPIO_PORT_B, GPIO_PIN_00}, {GPIO_PORT_B, GPIO_PIN_01}, \
            {GPIO_PORT_C, GPIO_PIN_00}, {GPIO_PORT_C, GPIO_PIN_01}, \
            {GPIO_PORT_C, GPIO_PIN_02}, {GPIO_PORT_C, GPIO_PIN_03}, \
            {GPIO_PORT_C, GPIO_PIN_04}, {GPIO_PORT_C, GPIO_PIN_05}, \
    };
    stc_adc_pin_t astcADC3[ADC3_CH_COUNT] = { \
            {GPIO_PORT_A, GPIO_PIN_00}, {GPIO_PORT_A, GPIO_PIN_01}, \
            {GPIO_PORT_A, GPIO_PIN_02}, {GPIO_PORT_A, GPIO_PIN_03}, \
            {GPIO_PORT_F, GPIO_PIN_06}, {GPIO_PORT_F, GPIO_PIN_07}, \
            {GPIO_PORT_F, GPIO_PIN_08}, {GPIO_PORT_F, GPIO_PIN_09}, \
            {GPIO_PORT_F, GPIO_PIN_10}, {GPIO_PORT_F, GPIO_PIN_03}, \
            {GPIO_PORT_C, GPIO_PIN_00}, {GPIO_PORT_C, GPIO_PIN_01}, \
            {GPIO_PORT_C, GPIO_PIN_02}, {GPIO_PORT_C, GPIO_PIN_03}, \
            {GPIO_PORT_F, GPIO_PIN_04}, {GPIO_PORT_F, GPIO_PIN_05}, \
            {GPIO_PORT_H, GPIO_PIN_02}, {GPIO_PORT_H, GPIO_PIN_03}, \
            {GPIO_PORT_H, GPIO_PIN_04}, {GPIO_PORT_H, GPIO_PIN_05}, \
    };

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;

    if ((ADCx == M4_ADC1) || (ADCx == M4_ADC2))
    {
        GPIO_Init(astcADC12[u8PinNum].u8Port, astcADC12[u8PinNum].u16Pin, &stcGpioInit);
    }
    else
    {
        GPIO_Init(astcADC3[u8PinNum].u8Port, astcADC3[u8PinNum].u16Pin, &stcGpioInit);
    }
}


/**
 * @brief  Configures ADC channel(s).
 * @param  None
 * @retval None
 */
static void AdcChannelConfig(void)
{
    uint8_t au8AdcSASplTime[] = APP_ADC_SA_SAMPLE_TIME;

    /* 1. Set the ADC pin to analog input mode. */
    AdcSetChannelPinAnalogMode(APP_ADC_UNIT, APP_ADC_SA_CH);

    /* 2. Enable the ADC channels. */
    ADC_ChannelCmd(APP_ADC_UNIT, ADC_SEQ_A, \
                   APP_ADC_SA_CH, au8AdcSASplTime, \
                   Enable);

}

/**
 * @brief  Interrupt configuration.
 * @param  None
 * @retval None
 */
static void AdcIrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    /* Configures interrupt of sequence A. */
    stcCfg.enIntSrc    = APP_ADC_SA_IRQ_SRC;
    stcCfg.enIRQn      = APP_ADC_SA_IRQn;
    stcCfg.pfnCallback = &APP_ADC_SA_IRQ_CB;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_ADC_SA_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the interrupts of both sequences. */
    ADC_SeqIntCmd(APP_ADC_UNIT, ADC_SEQ_A, Enable);
}

/**
 * @brief  Trigger source configuration. Specify trigger source for sequence A and sequence B.
 * @param  None
 * @retval None
 */
static void AdcTrigSrcConfig(void)
{
    stc_adc_trig_cfg_t stcTrigCfg;

    ADC_TrigSrcStructInit(&stcTrigCfg);

    /*
     * If select an event as the trigger source of sequence A or sequence B, \
     *    the AOS function must be enabled at first.
     */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);

    /*
     * Configures the trigger source of sequence A.
     * An event that generated by other peripheral is configured as the trigger source of sequence A.
     */
    stcTrigCfg.u16TrigSrc = ADC_TRIG_SRC_EVENT0;
#ifdef SCMA_ValidPeriod
    stcTrigCfg.enEvent0   = EVT_TMR6_1_SCMA;
#else
    stcTrigCfg.enEvent0   = EVT_TMR6_1_GUDF;
#endif
    ADC_TrigSrcConfig(APP_ADC_UNIT, ADC_SEQ_A, &stcTrigCfg);
    ADC_TrigSrcCmd(APP_ADC_UNIT, ADC_SEQ_A, Enable);
}

/**
 * @brief  ADC configuration for example
 * @param  None
 * @retval None
 */
void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcIrqConfig();
    AdcTrigSrcConfig();

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
    stc_irq_signin_config_t        stcIrqRegiConf;
    stc_tmr6_buf_func_cfg_t        stcBufCfg;
    stc_tmr6_valid_period_func_cfg_t stcValidPeriodCfg;
    stc_gpio_init_t                stcGpioCfg;

    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    TMR6_BaseCntStructInit(&stcTIM6BaseCntCfg);
    TMR6_PortOutputStructInit(&stcTIM6PortOutCfg);
    TMR6_BufFuncStructInit(&stcBufCfg);
    TMR6_ValidPeriodStructInit(&stcValidPeriodCfg);

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);

    /* Timer6 PWM output port configuration */
    GPIO_SetFunc(TMR6_1_PWMA_PORT, TMR6_1_PWMA_PIN, GPIO_FUNC_3_TIM61_PWMA, PIN_SUBFUNC_DISABLE);

    /* Configate GPIO for example */
    GPIO_StructInit(&stcGpioCfg);
    stcGpioCfg.u16PinDir = PIN_DIR_OUT;
    GPIO_Init(TEST_GPIO_PORT, TEST_GPIO_PIN, &stcGpioCfg);

    /* Timer6 general count function configuration */
    stcTIM6BaseCntCfg.u32CntMode = TMR6_MODE_TRIANGLE;
    stcTIM6BaseCntCfg.u32CntDir = TMR6_CNT_INCREASE;
    stcTIM6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV64;
    stcTIM6BaseCntCfg.u32CntStpAftOvf = TMR6_CNT_CONTINUOUS;
    TMR6_Init(M4_TMR6_1, &stcTIM6BaseCntCfg);

    /* Period register set */
    u32Period = 0x8340U;
    TMR6_SetPeriodReg(M4_TMR6_1, TMR6_PERIOD_REG_A, u32Period);

    /* Set General Compare RegisterA and RegisterC Value */
    u32Compare = 0x3000U;
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, u32Compare);

    /* General compare buffer function configurate */
    stcBufCfg.u32BufFunCmd = TMR6_BUF_FUNC_ON;
    stcBufCfg.u32BufNum = TMR6_BUF_FUNC_SINGLE;
    stcBufCfg.u32BufTransTim = TMR6_BUF_TRANS_TIM_UNDERFLOW;
    TMR6_GenCmpBufCfg(M4_TMR6_1, TMR6_CH_A, &stcBufCfg);

    /* Set special compare register */
    u32Compare = 0x3000U;
    TMR6_SetSpecialCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, u32Compare);
    TMR6_SetSpecialCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, u32Compare);

    /* Special compare buffer function configurate */
    stcBufCfg.u32BufFunCmd = TMR6_BUF_FUNC_ON;
    stcBufCfg.u32BufNum = TMR6_BUF_FUNC_SINGLE;
    stcBufCfg.u32BufTransTim = TMR6_BUF_TRANS_TIM_UNDERFLOW;
    TMR6_SpecialCmpBufCfg(M4_TMR6_1, TMR6_CH_A, &stcBufCfg);

     /* Configurate PWM output */
    stcTIM6PortOutCfg.u32PortMode = TMR6_PORT_COMPARE_OUTPUT;
    stcTIM6PortOutCfg.u32NextPeriodForceSta = TMR6_FORCE_PORT_OUTPUT_INVALID;
    stcTIM6PortOutCfg.u32DownCntMatchAnotherCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32UpCntMatchAnotherCmpRegSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32DownCntMatchCmpRegSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTIM6PortOutCfg.u32UpCntMatchCmpRegSta = TMR6_PORT_OUTPUT_STA_HIGH;
    stcTIM6PortOutCfg.u32UnderflowSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32OverflowSta = TMR6_PORT_OUTPUT_STA_HOLD;
    stcTIM6PortOutCfg.u32StopSta = TMR6_PORT_OUTPUT_STA_LOW;
    stcTIM6PortOutCfg.u32StartSta = TMR6_PORT_OUTPUT_STA_LOW;
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTIM6PortOutCfg);

#if 0
    /* Set DeadTime value */
    TMR6_SetDeadTimeReg(M4_TMR6_1, TMR6_DEADTIME_REG_UP_A, 3360U);

    /* DeadTime function configurate */
    stcDeadTimCfg.u32EnDtBufDwn = TMR6_DEADTIME_CNT_DOWN_BUF_OFF;  /* Disable buffer transfer */
    stcDeadTimCfg.u32EnDtBufUp = TMR6_DEADTIME_CNT_UP_BUF_OFF; /* Disable buffer transfer */
    stcDeadTimCfg.u32DtEqualUpDwn = TMR6_DEADTIME_EQUAL_ON; /* Make the down count dead time value equal to the up count dead time setting */
    stcDeadTimCfg.u32DtUpdCond = TMR6_DEADTIME_TRANS_COND_NONE;
    TMR6_DeadTimeCfg(M4_TMR6_1, &stcDeadTimCfg);
    /* Enable DeadTime function */
    TMR6_DeadTimeFuncCmd(M4_TMR6_1, Enable);
#endif

#ifdef SCMA_ValidPeriod
    /* Valid period function configurate */
    stcValidPeriodCfg.u32CntCond = TMR6_VALID_PERIOD_CNT_COND_BOTH;
    stcValidPeriodCfg.u32PeriodInterval = TMR6_VALID_PERIOD_CNT_1;
    stcValidPeriodCfg.u32StatChA = TMR6_VALID_PERIOD_FUNC_CHA_ON;
    stcValidPeriodCfg.u32StatChB = TMR6_VALID_PERIOD_FUNC_CHB_OFF;
    TMR6_ValidPeriodCfg(M4_TMR6_1, &stcValidPeriodCfg);

#endif

    /* Enable interrupt */
    TMR6_IntCmd(M4_TMR6_1, TMR6_IRQ_EN_UNDERFLOW, Enable);

    stcIrqRegiConf.enIRQn = Int002_IRQn;                    /* Register INT_TMR61_GOVF Int to Vect.No.002 */
    stcIrqRegiConf.enIntSrc = INT_TMR6_1_GUDF;               /* Select Event interrupt of tmr61 */
    stcIrqRegiConf.pfnCallback = &Tmr6_CallBack;   /* Callback function */
    INTC_IrqSignIn(&stcIrqRegiConf);                        /* Registration IRQ */

    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);            /* Clear Pending */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);/* Set priority */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);                  /* Enable NVIC */

    AdcConfig();

    /* Start timer6 */
    TMR6_CountCmd(M4_TMR6_1, Enable);

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
