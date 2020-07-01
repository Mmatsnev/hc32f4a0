/**
 *******************************************************************************
 * @file  adc/adc_02_event_trigger/source/main.c
 * @brief Main program ADC event trigger for the Device Driver Library.
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
 * @addtogroup ADC_Event_Trigger
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
#define APP_ADC_USE_INTERRUPT               (1U)

/*
 * ADC unit instance for this example.
 * Marco 'APP_ADC_UNIT' can be defined as M4_ADCx(x=1, 2, 3).
 * Definition of 'APP_ADC_PERIP_CLK' depends on 'APP_ADC_UNIT'.
 * 
 * NOTE!!! The following definitions are depend on the definitions of 'APP_ADC_UNIT'.
 *   APP_ADC_SA_IRQ_HANDLER
 *   APP_ADC_SB_IRQ_HANDLER
 *   APP_ADC_SA_IRQ_SRC
 *   APP_ADC_SB_IRQ_SRC
 *   APP_ADC_SB_TRIG_SRC_PORT
 *   APP_ADC_SB_TRIG_SRC_PIN 
 */
#define APP_ADC_UNIT                        (M4_ADC1)
#define APP_ADC_PERIP_CLK                   (PWC_FCG3_ADC1)

/*
 * Definitions of interrupt.
 * All interrupts of ADC can use independent interrupt vectors Int000_IRQn ~ Int031_IRQn
 *     and Int122_IRQn ~ Int127_IRQn, sharing interrupt vector Int143_IRQn.
 * 'APP_ADC_USE_SHARE_INTERRUPT': Defines as non-zero to use sharing interrupt.
 */
#if (APP_ADC_USE_INTERRUPT > 0U)
    #define APP_ADC_USE_SHARE_INTERRUPT     (0U)
    #define ADC_SHARE_IRQn                  (Int143_IRQn)
    #define APP_ADC_SA_IRQ_HANDLER          ADC_1_SeqA_IrqHandler
    #define APP_ADC_SB_IRQ_HANDLER          ADC_1_SeqB_IrqHandler
    #define APP_ADC_SA_IRQ_SRC              (INT_ADC1_EOCA)
    #define APP_ADC_SB_IRQ_SRC              (INT_ADC1_EOCB)
    #define APP_ADC_SA_IRQn                 (Int016_IRQn)
    #define APP_ADC_SB_IRQn                 (Int017_IRQn)
    #define APP_ADC_SA_INT_PRIO             (DDL_IRQ_PRIORITY_03)
    #define APP_ADC_SB_INT_PRIO             (DDL_IRQ_PRIORITY_04)
#endif

/*
 * Specifies the ADC channels according to the application.
 * NOTE!!! Sequence A and sequence B CANNOT contain the same channel.
 */
#define APP_ADC_SA_CH                       (ADC_CH0 | ADC_CH2 | ADC_CH3 | ADC_CH14)
#define APP_ADC_SA_CH_COUNT                 (4U)

#define APP_ADC_SB_CH                       (ADC_CH10 | ADC_CH11 | ADC_CH15)
#define APP_ADC_SB_CH_COUNT                 (3U)

/* Check if sequence A and sequence B contain the same channel. */
#if ((APP_ADC_SA_CH & APP_ADC_SB_CH) != 0U)
#error "Sequence A and sequence B contain the same channel!"
#endif

/* Sampling time of ADC channels. */
/*                                        ADC_CH0    ADC_CH2   ADC_CH3   ADC_CH14 */
#define APP_ADC_SA_SAMPLE_TIME              { 30,       30,       35,         45 }
/*                                        ADC_CH10   ADC_CH11  ADC_CH15 */
#define APP_ADC_SB_SAMPLE_TIME              { 35,       35,       35 }

/*
 * Add the channels which were included in sequence A or sequence B to average channel if needed.
 * The average channels will be sampled a specified number of times(specified by 'APP_ADC_AVG_CNT'),\
 *   and the final ADC value is the average of the specified number of samples.
 * Define 'APP_ADC_AVG_CH' as 0 to disable average channel.
 */
#define APP_ADC_AVG_CNT                     (ADC_AVG_CNT_16)
#define APP_ADC_AVG_CH                      (ADC_CH3 | APP_ADC_SB_CH)

/*
 * Trigger source definitions.
 * Set event 'EVT_TMR2_1_CMPA' to trigger sequence A every second.
 * Set pin ADTRG1(PE7) to trigger sequence B. Make a falling edge on ADTRG1 and hold the low level \
 *   at least 1.5 cycles of PCLK4 to trigger sequence B.
 * Both sequences can be set triggered by event or the falling edge of pin ADTRGx(x is ADC unit number, x=1, 2, 3).
 * Only the falling edge of pin ADTRGx and the low level of pin ADTRGx must be held at least 1.5 cycles of PCLK4 can trigger ADC start sampling.
 * ADTRG1: PB7, PD6, PD14, PE7.
 * ADTRG2: PB6, PB10, PD7, PD15.
 * ADTRG3: PB5, PE9, PG2, PG9.
 *
 * NOTE!!! Sequence B CAN only be started by event or the falling edge of pin ADTRGx(x is ADC unit number, x=1, 2, 3).
 */
#define APP_ADC_SA_TRIG_SRC_TYPE            (ADC_TRIG_SRC_EVENT0)
#define APP_ADC_SA_TRIG_SRC_EVENT           (EVT_TMR2_1_CMPA)

#define APP_ADC_SB_TRIG_SRC_TYPE            (ADC_TRIG_SRC_ADTRG)
#define APP_ADC_SB_TRIG_SRC_PORT            (GPIO_PORT_E)
#define APP_ADC_SB_TRIG_SRC_PIN             (GPIO_PIN_07)
#define APP_ADC_SB_TRIG_PIN_FUNC            (GPIO_FUNC_1_ADTRG)

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

static void SystemClockConfig(void);

static void AdcConfig(void);
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
#if (APP_ADC_USE_INTERRUPT > 0U)
    static void AdcIrqConfig(void);
    static void ADC_SeqA_IrqCallback(void);
    static void ADC_SeqB_IrqCallback(void);
#endif
static void AdcTrigSrcConfig(void);
static void AdcStartTrigSrc(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16AdcSaVal[APP_ADC_SA_CH_COUNT];
static uint16_t m_au16AdcSbVal[APP_ADC_SB_CH_COUNT];

#if (APP_ADC_USE_INTERRUPT > 0U)
    static uint32_t m_u32AdcIrqFlag = 0U;
#endif /* #if (APP_ADC_USE_INTERRUPT > 0U) */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_02_event_trigger project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the PLLHP(200MHz) as the system clock. */
    SystemClockConfig();
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
        /*
         * Timer2 triggers sequence A one time every second.
         * Make a falling edge on the specified pin ADTRGx and hold the low level \
         *   at least 1.5 cycles of PCLK4 to trigger sequence B.
         */
#if (APP_ADC_USE_INTERRUPT > 0U)
        if ((m_u32AdcIrqFlag & ADC_SEQ_FLAG_EOCA) != 0U)
        {
            DBG("ADC sequence A is triggered by Timer2 every second.\n");
            m_u32AdcIrqFlag &= (uint32_t)(~ADC_SEQ_FLAG_EOCA);
        }
        if ((m_u32AdcIrqFlag & ADC_SEQ_FLAG_EOCB) != 0U)
        {
            DBG("ADC sequence B is triggered by the falling edge of the specified pin ADTRGx.\n");
            m_u32AdcIrqFlag &= (uint32_t)(~ADC_SEQ_FLAG_EOCB);
        }
#else
        if (ADC_SeqGetStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCA) == Set)
        {
            ADC_SeqClrStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCA);
            ADC_GetChannelData(APP_ADC_UNIT, APP_ADC_SA_CH, m_au16AdcSaVal, APP_ADC_SA_CH_COUNT);
            DBG("ADC sequence A is triggered by Timer2 every second.\n");
        }

        if (ADC_SeqGetStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCB) == Set)
        {
            ADC_SeqClrStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCB);
            ADC_GetChannelData(APP_ADC_UNIT, APP_ADC_SB_CH, m_au16AdcSbVal, APP_ADC_SB_CH_COUNT);
            DBG("ADC sequence B is triggered by the falling edge of the specified pin ADTRGx.\n");
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
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    // SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
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
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
}

/**
 * @brief  Configures the PLLHP(200MHz) as the system clock.
 *         The input source of PLLH is XTAL(8MHz).
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_pllh_init_t stcPLLHInit;
    stc_clk_xtal_init_t stcXtalInit;

    /* Configures XTAL. PLLH input source is XTAL. */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EX BUS Max 120MHz */
    CLK_ClkDiv(CLK_CATE_ALL,                                       \
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV8 | CLK_PCLK2_DIV8 | \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV8 | CLK_EXCLK_DIV2 | \
                CLK_HCLK_DIV1));

    CLK_PLLHStrucInit(&stcPLLHInit);
    /*
     * PLLP_freq = ((PLL_source / PLLM) * PLLN) / PLLP
     *           = (8 / 1) * 100 / 4
     *           = 200
     */
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = (1UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLN = (100UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLP = (4UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLQ = (16UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLR = (16UL  - 1UL);

    /* stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL; */
    CLK_PLLHInit(&stcPLLHInit);

    /* Set SRAM wait cycles. */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    /* Set EFM wait cycle. 4 wait cycles needed when system clock is 200MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_4);

    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
}

/**
 * @brief  ADC configuration, including clock configuration, initial configuration
 *         channel configuration, interrupt configuration and trigger source configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
#if (APP_ADC_USE_INTERRUPT > 0U)
    AdcIrqConfig();
#endif
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
     * Specify the clock source of ADC.
     * 'CLK_PERI_CLK_PCLK' , 'CLK_PERI_CLK_PLLHQ' and 'CLK_PERI_CLK_PLLHR' can be used.
     */
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

    /* 1. Modify the default value depends on the application. */
    stcInit.u16ScanMode    = ADC_MODE_SA_SB_SSHOT;
    stcInit.u16SAResumePos = ADC_SA_RESUME_POS_INT_CH;

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
    uint8_t au8AdcSBSplTime[] = APP_ADC_SB_SAMPLE_TIME;

    /* 1. Set the ADC pin to analog input mode. */
    AdcSetChannelPinAnalogMode(APP_ADC_UNIT, APP_ADC_SA_CH | APP_ADC_SB_CH);

    /* 2. Enable the ADC channels. */
    ADC_ChannelCmd(APP_ADC_UNIT, ADC_SEQ_A, \
                   APP_ADC_SA_CH, au8AdcSASplTime, \
                   Enable);

    ADC_ChannelCmd(APP_ADC_UNIT, ADC_SEQ_B, \
                   APP_ADC_SB_CH, au8AdcSBSplTime, \
                   Enable);

    /* 3. Set the number of averaging sampling and enable the channel, if needed. */
#if (defined APP_ADC_AVG_CH) && (APP_ADC_AVG_CH != 0U)
    ADC_AvgChannelConfig(APP_ADC_UNIT, APP_ADC_AVG_CNT);
    ADC_AvgChannelCmd(APP_ADC_UNIT, APP_ADC_AVG_CH, Enable);
#endif
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
    stcInit.u32ClkDiv = TMR2_CLK_DIV512;
    stcInit.u32CmpVal = 48828UL;
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR2_1, Enable);
    TMR2_Init(M4_TMR2_1, TMR2_CH_A, &stcInit);

    ADC_TrigSrcStructInit(&stcTrigCfg);
    stcTrigCfg.u16TrigSrc = APP_ADC_SA_TRIG_SRC_TYPE;
    stcTrigCfg.enEvent0   = APP_ADC_SA_TRIG_SRC_EVENT;
    ADC_TrigSrcConfig(APP_ADC_UNIT, ADC_SEQ_A, &stcTrigCfg);
    ADC_TrigSrcCmd(APP_ADC_UNIT, ADC_SEQ_A, Enable);

    /*
     * Configures the trigger source of sequence B.
     * One of ADTRGx(x is the ADC unit number, x=1, 2, 3) pins is configured to trigger sequence B.
     * Make a falling edge on ADTRGx and hold the low level at least 1.5 PCLK4 cycles to trigger sequence B.
     */
    stcTrigCfg.u16TrigSrc = APP_ADC_SB_TRIG_SRC_TYPE;
    GPIO_SetFunc(APP_ADC_SB_TRIG_SRC_PORT, APP_ADC_SB_TRIG_SRC_PIN, \
                 APP_ADC_SB_TRIG_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    ADC_TrigSrcConfig(APP_ADC_UNIT, ADC_SEQ_B, &stcTrigCfg);
    ADC_TrigSrcCmd(APP_ADC_UNIT, ADC_SEQ_B, Enable);
}

/**
 * @brief  Start the peripheral which is used to generate the trigger source to trigger ADC.
 * @param  None
 * @retval None
 */
static void AdcStartTrigSrc(void)
{
    /* For sequence A. Start the peripheral anywhere you want. */
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);

    /*
     * For sequence B. Make a falling edge on the specified ADTRGx pin and hold the low level \
     *   at least 1.5 PCLK4 cycles to trigger sequence B.
     */
}

#if (APP_ADC_USE_INTERRUPT > 0U)
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
    if (stcCfg.enIRQn == ADC_SHARE_IRQn)
    {
        /* Sharing interrupt. */
        INTC_ShareIrqCmd(stcCfg.enIntSrc, Enable);
    }
    else
    {
        stcCfg.pfnCallback = &ADC_SeqA_IrqCallback;
        /* Independent interrupt. */
        INTC_IrqSignIn(&stcCfg);
    }
    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_ADC_SA_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Configures interrupt of sequence B. */
    stcCfg.enIntSrc    = APP_ADC_SB_IRQ_SRC;
    stcCfg.enIRQn      = APP_ADC_SB_IRQn;
    if (stcCfg.enIRQn == ADC_SHARE_IRQn)
    {
        /* Sharing interrupt. */
        INTC_ShareIrqCmd(stcCfg.enIntSrc, Enable);
    }
    else
    {
        stcCfg.pfnCallback = &ADC_SeqB_IrqCallback;
        /* Independent interrupt. */
        INTC_IrqSignIn(&stcCfg);
    }
    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_ADC_SB_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the interrupts of both sequences. */
    ADC_SeqIntCmd(APP_ADC_UNIT, ADC_SEQ_A, Enable);
    ADC_SeqIntCmd(APP_ADC_UNIT, ADC_SEQ_B, Enable);
}

/**
 * @brief  Sequence A IRQ handler.
 * @param  None
 * @retval None
 */
#if (APP_ADC_USE_SHARE_INTERRUPT > 0U)
void APP_ADC_SA_IRQ_HANDLER(void)
#else
static void ADC_SeqA_IrqCallback(void)
#endif
{
    if (ADC_SeqGetStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCA) == Set)
    {
        ADC_SeqClrStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCA);
        ADC_GetChannelData(APP_ADC_UNIT, APP_ADC_SA_CH, m_au16AdcSaVal, APP_ADC_SA_CH_COUNT);
        m_u32AdcIrqFlag |= ADC_SEQ_FLAG_EOCA;
    }
}

/**
 * @brief  Sequence B IRQ handler.
 * @param  None
 * @retval None
 */
#if (APP_ADC_USE_SHARE_INTERRUPT > 0U)
void APP_ADC_SB_IRQ_HANDLER(void)
#else
static void ADC_SeqB_IrqCallback(void)
#endif
{
    if (ADC_SeqGetStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCB) == Set)
    {
        ADC_SeqClrStatus(APP_ADC_UNIT, ADC_SEQ_FLAG_EOCB);
        ADC_GetChannelData(APP_ADC_UNIT, APP_ADC_SB_CH, m_au16AdcSbVal, APP_ADC_SB_CH_COUNT);
        m_u32AdcIrqFlag |= ADC_SEQ_FLAG_EOCB;
    }
}
#endif /* #if (APP_ADC_USE_INTERRUPT > 0U) */

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
