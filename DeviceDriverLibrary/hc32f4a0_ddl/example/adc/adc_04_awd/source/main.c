/**
 *******************************************************************************
 * @file  adc/adc_04_awd/source/main.c
 * @brief Main program ADC AWD for the Device Driver Library.
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
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup ADC_AWD
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*
 * Function control of ADC.
 * Defines the following macro as non-zero to enable the corresponding function.
 */
#define APP_AWD_USE_COMB_MODE               (0U)

/*
 * ADC unit instance for this example.
 * Marco 'APP_ADC_UNIT' can be defined as M4_ADCx(x=1, 2, 3).
 * Definition of 'APP_PERIP_CLK_ADC' depends on 'APP_ADC_UNIT'.
 */
#define APP_ADC_UNIT                        (M4_ADC2)
#define APP_ADC_PERIP_CLK                   (PWC_FCG3_ADC2)

/* 
 * ADC channels definition for this example.
 * 'APP_ADC_AWDx_CH_NUM'(x=0, 1) can be defined as ADC_CH_NUM_0 ~ ADC_CH_NUM_15.
 */
#define APP_ADC_AWD0_CH_NUM                 (ADC_CH_NUM_3)
#define APP_ADC_AWD1_CH_NUM                 (ADC_CH_NUM_4)
#define APP_ADC_AWD0_CH                     (1UL << APP_ADC_AWD0_CH_NUM)
#define APP_ADC_AWD1_CH                     (1UL << APP_ADC_AWD1_CH_NUM)

#define APP_ADC_SA_CH                       (APP_ADC_AWD0_CH | APP_ADC_AWD1_CH)
#define APP_ADC_SA_CH_COUNT                 (2U)

/* Sampling time of ADC channels. */
/*                                        APP_ADC_AWD0_CH  APP_ADC_AWD1_CH */
#define APP_ADC_SA_SAMPLE_TIME              { 30,               30 }

/*
 * Calculate the voltage.
 * 'APP_ADC_VREF': ADC reference voltage. The voltage of pin VREFH. Different board maybe different voltage.
 * 'APP_ADC_ACCURACY': ADC accuracy. According to the resolution of ADC.
 */
#define APP_ADC_VREF                        (3.3018f)
#define APP_ADC_ACCURACY                    (1UL << 12U)
#define APP_ADC_CAL_VALUE(vol)              ((uint16_t)(((float)(vol) * (float)APP_ADC_ACCURACY) / (float)APP_ADC_VREF))
#define APP_CAL_VOL(adcVal)                 ((float)(adcVal) * APP_ADC_VREF) / ((float)APP_ADC_ACCURACY)
/*
 * Add the channels which were included in sequence A or sequence B to average channel if needed.
 * The average channels will be sampled a specified number of times(specified by 'APP_ADC_AVG_CNT'),\
 *   and the final ADC value is the average of the specified number of samples.
 * Define 'APP_ADC_AVG_CH' as 0 to disable average channel.
 */
#define APP_ADC_AVG_CNT                     (ADC_AVG_CNT_16)
#define APP_ADC_AVG_CH                      (APP_ADC_SA_CH)

/*
 * Trigger source definitions.
 * Set event 'EVT_TMR2_1_CMPA' to trigger sequence A every second.
 */
#define APP_ADC_SA_TRIG_SRC_TYPE            (ADC_TRIG_SRC_EVENT0)
#define APP_ADC_SA_TRIG_SRC_EVENT           (EVT_TMR2_1_CMPA)

/* Combination mode definitions. */
#if (APP_AWD_USE_COMB_MODE > 0U)
    #define APP_ADC_AWD_COMB_MODE           (ADC_AWD_COMB_OR)
#endif

/*
 * Definitions of interrupt.
 * All interrupts of ADC can use independent interrupt vectors Int000_IRQn ~ Int031_IRQn and Int122_IRQn ~ Int127_IRQn
 */
#if (APP_AWD_USE_COMB_MODE > 0U)
    #define APP_ADC_AWD_COMB_IRQ_SRC        INT_ADC2_CMP1
    #define APP_ADC_AWD_COMB_IRQn           (Int017_IRQn)
    #define APP_ADC_AWD_COMB_INT_PRIO       (DDL_IRQ_PRIORITY_03)
#else
    #define APP_ADC_AWD0_IRQ_SRC            (INT_ADC2_CMP0)
    #define APP_ADC_AWD1_IRQ_SRC            (INT_ADC2_CMP1)
    #define APP_ADC_AWD0_IRQn               (Int016_IRQn)
    #define APP_ADC_AWD1_IRQn               (Int017_IRQn)
    #define APP_ADC_AWD0_INT_PRIO           (DDL_IRQ_PRIORITY_03)
    #define APP_ADC_AWD1_INT_PRIO           (DDL_IRQ_PRIORITY_04)
#endif /* #if (APP_AWD_USE_COMB_MODE > 0U) */

/*
 * AWD compare mode and compare windows.
 * AWD0_LOW_VOL <= The voltage of AWD0 pin <= AWD0_UP_VOL: The flag of AWD0 set.
 * The voltage of AWD1 pin < AWD1_LOW_VOL or > AWD1_UP_VOL: The flag of AWD1 set.
 */
#define AWD0_LOW_VOL                        (1.5f)
#define AWD0_UP_VOL                         (2.5f)
#define AWD1_LOW_VOL                        (0.1f)
#define AWD1_UP_VOL                         (3.1f)
#define APP_ADC_AWD0_CMP_MODE               (ADC_AWD_CMP_IN_RANGE)
#define APP_ADC_AWD1_CMP_MODE               (ADC_AWD_CMP_OUT_RANGE)
#define APP_ADC_AWD0_LOW_LIMIT              (APP_ADC_CAL_VALUE(AWD0_LOW_VOL))
#define APP_ADC_AWD0_UP_LIMIT               (APP_ADC_CAL_VALUE(AWD0_UP_VOL))
#define APP_ADC_AWD1_LOW_LIMIT              (APP_ADC_CAL_VALUE(AWD1_LOW_VOL))
#define APP_ADC_AWD1_UP_LIMIT               (APP_ADC_CAL_VALUE(AWD1_UP_VOL))

/* Debug printing definition. */
#if (DDL_PRINT_ENABLE == DDL_ON)
#define DBG         printf
#else
#define DBG(...)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcAwdConfig(void);
static void AdcIrqConfig(void);

#if (APP_AWD_USE_COMB_MODE > 0U)
static void ADC_Cmp1Comb_IrqCallback(void);
#endif
static void ADC_Cmp0_IrqCallback(void);
static void ADC_Cmp1Ind_IrqCallback(void);

static void AdcTrigSrcConfig(void);
static void AdcStartTrigSrc(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16AdcSaVal[APP_ADC_SA_CH_COUNT];
static uint32_t m_u32AdcIrqFlag = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_04_awd project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The default system clock is MRC(8MHz). */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures ADC. */
    AdcConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Start the trigger-source peripheral. */
    AdcStartTrigSrc();

    /***************** Configuration end, application start **************/

    while (1U)
    {
        if (ADC_SeqGetStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCA) == Set)
        {
            ADC_SeqClrStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCA);
            ADC_GetChannelData(APP_ADC_UNIT, APP_ADC_SA_CH, m_au16AdcSaVal, APP_ADC_SA_CH_COUNT);
            DBG("AWD0 voltage: %.3f\n", APP_CAL_VOL(m_au16AdcSaVal[0U]));
            DBG("AWD1 voltage: %.3f\n", APP_CAL_VOL(m_au16AdcSaVal[1U]));
        }
#if (APP_AWD_USE_COMB_MODE > 0U)
        if ((m_u32AdcIrqFlag & ADC_AWD_FLAG_COMB) != 0U)
        {
            DBG("--->> AWD combine-flag set.\n");
            m_u32AdcIrqFlag &= (uint32_t)(~ADC_AWD_FLAG_COMB);
        }
#else
        if ((m_u32AdcIrqFlag & ADC_AWD_FLAG_AWD0) != 0U)
        {
            DBG("--->> AWD0 flag set, voltage: %.3f.\n", APP_CAL_VOL(m_au16AdcSaVal[0U]));
            m_u32AdcIrqFlag &= (uint32_t)(~ADC_AWD_FLAG_AWD0);
        }

        if ((m_u32AdcIrqFlag & ADC_AWD_FLAG_AWD1) != 0U)
        {
            DBG("--->> AWD1 flag set, voltage: %.3f.\n", APP_CAL_VOL(m_au16AdcSaVal[1U]));
            m_u32AdcIrqFlag &= (uint32_t)(~ADC_AWD_FLAG_AWD1);
        }
#endif
    }
}

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
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Unlock SRAM register: WTCR */
    // SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    // SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    // EFM_Unlock();
    /* Unlock EFM register: FWMC */
    // EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    // EFM_OTP_WP_Unlock();
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
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Lock SRAM register: WTCR */
    // SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock all EFM registers */
    // EFM_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
}

/**
 * @brief  ADC configuration, including clock configuration, initial configuration
 *         and channel configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcAwdConfig();
    AdcIrqConfig();
    AdcTrigSrcConfig();
}

/**
 * @brief  Configures ADC clock.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{
    /*
     * 1. Configures the clock divider of PCLK2 and PCLK4 here or in the function of configuring the system clock.
     *    In this example, the default system clock is MRC(8MHz).
     *    PCLK4 is the digital interface clock, and PCLK2 is the analog circuit clock.
     *    PCLK4 : PCLK2 = 1:1, 2:1, 4:1, 8:1, 1:2, 1:4.
     *    PCLK2 is in range [1MHz, 60MHz].
     */
    CLK_ClkDiv((CLK_CATE_PCLK2 | CLK_CATE_PCLK4), (CLK_PCLK2_DIV4 | CLK_PCLK4_DIV1));

    /* 2. Specify the clock source of ADC. Only 'CLK_PERI_CLK_PCLK' can be used in this example. */
    CLK_PERI_ClkConfig(CLK_PERI_CLK_PCLK);
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

    /* 1. Modify the default value depends on the application. Not needed here. */

    /* 2. Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_ADC_PERIP_CLK, Enable);

    /* 3. Initializes ADC. */
    ADC_Init(APP_ADC_UNIT, &stcInit);
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

    /* 3. Set the number of averaging sampling and enable the channel, if needed. */
#if (defined APP_ADC_AVG_CH) && (APP_ADC_AVG_CH != 0U)
    ADC_AvgChannelConfig(APP_ADC_UNIT, APP_ADC_AVG_CNT);
    ADC_AvgChannelCmd(APP_ADC_UNIT, APP_ADC_AVG_CH, Enable);
#endif
}

/**
 * @brief  AWD configuration.
 * @param  None
 * @retval None
 */
static void AdcAwdConfig(void)
{
    stc_adc_awd_cfg_t stcCfg;

    stcCfg.u8ChNum       = APP_ADC_AWD0_CH_NUM;
    stcCfg.u16CmpMode    = APP_ADC_AWD0_CMP_MODE;
    stcCfg.u16LowerLimit = APP_ADC_AWD0_LOW_LIMIT;
    stcCfg.u16UpperLimit = APP_ADC_AWD0_UP_LIMIT;
    ADC_AWD_Config(APP_ADC_UNIT, ADC_AWD_0, &stcCfg);

    stcCfg.u8ChNum       = APP_ADC_AWD1_CH_NUM;
    stcCfg.u16CmpMode    = APP_ADC_AWD1_CMP_MODE;
    stcCfg.u16LowerLimit = APP_ADC_AWD1_LOW_LIMIT;
    stcCfg.u16UpperLimit = APP_ADC_AWD1_UP_LIMIT;
    ADC_AWD_Config(APP_ADC_UNIT, ADC_AWD_1, &stcCfg);

#if (APP_AWD_USE_COMB_MODE > 0U)
    ADC_AWD_CombModeCmd(APP_ADC_UNIT, APP_ADC_AWD_COMB_MODE, Enable);
#endif

    ADC_AWD_Cmd(APP_ADC_UNIT, ADC_AWD_0, Enable);
    ADC_AWD_Cmd(APP_ADC_UNIT, ADC_AWD_1, Enable);
}

/**
 * @brief  Trigger source configuration. Specify trigger source for sequence A and sequence B.
 * @param  None
 * @retval None
 */
static void AdcTrigSrcConfig(void)
{
    stc_adc_trig_cfg_t stcTrigCfg;
    stc_tmr2_init_t stcInit;

    /*
     * If select an event as the trigger source of sequence A or sequence B, \
     *   the AOS function must be enabled at first.
     */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);

    /*
     * Configures the trigger source of sequence A.
     * An event that generated by other peripheral is configured as the trigger source of sequence A.
     * Sequence A is triggered by Timer2 every second.
     */
    TMR2_StructInit(&stcInit);
    stcInit.u32ClkSrc = TMR2_CLK_SYNC_PCLK1;
    stcInit.u32ClkDiv = TMR2_CLK_DIV128;
    stcInit.u32CmpVal = 62500UL;
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR2_1, Enable);
    TMR2_Init(M4_TMR2_1, TMR2_CH_A, &stcInit);

    ADC_TrigSrcStructInit(&stcTrigCfg);
    stcTrigCfg.u16TrigSrc = APP_ADC_SA_TRIG_SRC_TYPE;
    stcTrigCfg.enEvent0   = APP_ADC_SA_TRIG_SRC_EVENT;
    ADC_TrigSrcConfig(APP_ADC_UNIT, ADC_SEQ_A, &stcTrigCfg);
    ADC_TrigSrcCmd(APP_ADC_UNIT, ADC_SEQ_A, Enable);
}

/**
 * @brief  Start the peripheral which is used to generate the trigger source to trigger ADC.
 * @param  None
 * @retval None
 */
static void AdcStartTrigSrc(void)
{
    /* Start the peripheral anywhere you want. */
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);
}

/**
 * @brief  Interrupt configuration.
 * @param  None
 * @retval None
 */
static void AdcIrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

#if (APP_AWD_USE_COMB_MODE > 0U)
    /* Configures interrupt of AWD combination mode. */
    stcCfg.enIntSrc    = APP_ADC_AWD_COMB_IRQ_SRC;
    stcCfg.enIRQn      = APP_ADC_AWD_COMB_IRQn;
    stcCfg.pfnCallback = &ADC_Cmp1Comb_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_ADC_AWD_COMB_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the interrupt of AWD combination mode. */
    ADC_AWD_IntCmd(APP_ADC_UNIT, ADC_AWD_1, Enable);
#else
    /* Configures interrupt of AWD0. */
    stcCfg.enIntSrc    = APP_ADC_AWD0_IRQ_SRC;
    stcCfg.enIRQn      = APP_ADC_AWD0_IRQn;
    stcCfg.pfnCallback = &ADC_Cmp0_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_ADC_AWD0_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Configures interrupt of AWD1. */
    stcCfg.enIntSrc    = APP_ADC_AWD1_IRQ_SRC;
    stcCfg.enIRQn      = APP_ADC_AWD1_IRQn;
    stcCfg.pfnCallback = &ADC_Cmp1Ind_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_ADC_AWD1_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the interrupts of both AWDs. */
    ADC_AWD_IntCmd(APP_ADC_UNIT, ADC_AWD_0, Enable);
    ADC_AWD_IntCmd(APP_ADC_UNIT, ADC_AWD_1, Enable);
#endif /* #if (APP_AWD_USE_COMB_MODE > 0U) */
}

#if (APP_AWD_USE_COMB_MODE > 0U)
/**
 * @brief  AWD combination mode IRQ handler.
 * @param  None
 * @retval None
 */
static void ADC_Cmp1Comb_IrqCallback(void)
{
    if (ADC_AWD_GetStatus(APP_ADC_UNIT, ADC_AWD_FLAG_COMB) == Set)
    {
        ADC_AWD_ClrStatus(APP_ADC_UNIT, ADC_AWD_FLAG_COMB);
        m_u32AdcIrqFlag |= ADC_AWD_FLAG_COMB;
    }
}
#else
/**
 * @brief  AWD0 IRQ handler.
 * @param  None
 * @retval None
 */
static void ADC_Cmp0_IrqCallback(void)
{
    if (ADC_AWD_GetStatus(APP_ADC_UNIT, ADC_AWD_FLAG_AWD0) == Set)
    {
        ADC_AWD_ClrStatus(APP_ADC_UNIT, ADC_AWD_FLAG_AWD0);
        m_u32AdcIrqFlag |= ADC_AWD_FLAG_AWD0;
    }
}

/**
 * @brief  AWD1 IRQ handler.
 * @param  None
 * @retval None
 */
static void ADC_Cmp1Ind_IrqCallback(void)
{
    if (ADC_AWD_GetStatus(APP_ADC_UNIT, ADC_AWD_FLAG_AWD1) == Set)
    {
        ADC_AWD_ClrStatus(APP_ADC_UNIT, ADC_AWD_FLAG_AWD1);
        m_u32AdcIrqFlag |= ADC_AWD_FLAG_AWD1;
    }
}
#endif /* #if (APP_AWD_USE_COMB_MODE > 0U) */

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
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
