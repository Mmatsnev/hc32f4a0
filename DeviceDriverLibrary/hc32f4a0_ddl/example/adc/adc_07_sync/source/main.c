/**
 *******************************************************************************
 * @file  adc/adc_07_sync/source/main.c
 * @brief Main program ADC synchronous for the Device Driver Library.
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
 * @addtogroup ADC_Sync
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* 'ADC_SYNC_ADC1_ADC2' and 'ADC_SYNC_ADC1_ADC2_ADC3' can be used. */
#define APP_ADC_SYNC_UNITS                  (ADC_SYNC_ADC1_ADC2)

/* ADC synchronous mode. @ref ADC_Synchronous_Mode */
#define APP_ADC_SYNC_MODE                   (ADC_SYNC_SSHOT_SEQUENTIAL)

#if (APP_ADC_SYNC_MODE == ADC_SYNC_SSHOT_SEQUENTIAL)
    #define APP_ADC_SYNC_SAMPLE_TIME        (11U)
    #define APP_ADC_SYNC_TRIG_DELAY         (12U)
    #define APP_ADC_MODE                    (ADC_MODE_SA_CONT)

#elif (APP_ADC_SYNC_MODE == ADC_SYNC_SSHOT_SIMULTANEOUS)
    #define APP_ADC_SYNC_SAMPLE_TIME        (11U)
    #define APP_ADC_SYNC_TRIG_DELAY         (20U)
    #define APP_ADC_MODE                    (ADC_MODE_SA_SSHOT)

#elif (APP_ADC_SYNC_MODE == ADC_SYNC_CONT_SEQUENTIAL)
    #define APP_ADC_SYNC_SAMPLE_TIME        (11U)
    #define APP_ADC_SYNC_TRIG_DELAY         (17U)
    #define APP_ADC_MODE                    (ADC_MODE_SA_SSHOT)

#elif (APP_ADC_SYNC_MODE == ADC_SYNC_CONT_SIMULTANEOUS)
    #define APP_ADC_SYNC_SAMPLE_TIME        (11U)
    #define APP_ADC_SYNC_TRIG_DELAY         (40U)
    #define APP_ADC_MODE                    (ADC_MODE_SA_SSHOT)

#else
    #error "This sync mode is NOT supported."

#endif /* #if (APP_SYNC_MODE == APP_SYNC_MODE_SSHOT_SEQUENTIAL) */

/* ADC peripheral clock for this example. */
#if (APP_ADC_SYNC_UNITS == ADC_SYNC_ADC1_ADC2)
    #define APP_ADC_PERIP_CLK               (PWC_FCG3_ADC1 | PWC_FCG3_ADC2)
#else
    #define APP_ADC_PERIP_CLK               (PWC_FCG3_ADC1 | PWC_FCG3_ADC2 | PWC_FCG3_ADC3)
#endif

/* 
 * ADC channels definition for this example.
 * NOTE!!! DO NOT enable sequence B, otherwise it will disturb the synchronization timing.
 */
#if (APP_ADC_SYNC_UNITS == ADC_SYNC_ADC1_ADC2) && (APP_ADC_SYNC_MODE == APP_SYNC_MODE_SSHOT_SEQUENTIAL)
/* These definitions just for a sampling rate of 5Mbps. */
#define APP_ADC_SA_CH                       (ADC_CH3)
#define APP_ADC_SA_CH_COUNT                 (1U)
#define APP_ADC1_SA_CH                      (APP_ADC_SA_CH)
#define APP_ADC2_SA_CH                      (APP_ADC_SA_CH)
/* ADC channel sampling time.                        ADC_CH0 */
#define APP_ADC_SA_SAMPLE_TIME              { APP_ADC_SYNC_SAMPLE_TIME }

#else
#define APP_ADC_SA_CH_COUNT                 (3U)
#define APP_ADC1_SA_CH                      (ADC_CH0 | ADC_CH1 | ADC_CH2)
#define APP_ADC2_SA_CH                      (ADC_CH5 | ADC_CH6 | ADC_CH7)
#define APP_ADC3_SA_CH                      (ADC_CH3 | ADC_CH4 | ADC_CH5)
/* ADC channel sampling time.                        3 channels */
#define APP_ADC_SA_SAMPLE_TIME              { APP_ADC_SYNC_SAMPLE_TIME,  APP_ADC_SYNC_SAMPLE_TIME, APP_ADC_SYNC_SAMPLE_TIME}
#endif

/* 
 * Trigger source definitions.
 * Set pin ADTRG1(PE7) to trigger sequence A. Make a falling edge on ADTRG1 and hold the low level \
 *   at least 1.5 cycles of PCLK4 to trigger sequence A.
 * Both sequences can be set triggered by event or the falling edge of pin ADTRGx(x is ADC unit number, x=1, 2, 3).
 * Only the falling edge of pin ADTRGx and the low level of pin ADTRGx must be held at least 1.5 cycles of PCLK4 can trigger ADC start sampling.
 * ADTRG1: PB7, PD6, PD14, PE7.
 */
#define APP_ADC_SA_TRIG_SRC_TYPE            (ADC_TRIG_SRC_ADTRG)
#define APP_ADC_SA_TRIG_SRC_PORT            (GPIO_PORT_E)
#define APP_ADC_SA_TRIG_SRC_PIN             (GPIO_PIN_07)
#define APP_ADC_SA_TRIG_PIN_FUNC            (GPIO_FUNC_1_ADTRG)

/*
 * Definitions of DMA.
 * 'APP_DMA_BLOCK_SIZE': 1~1024, inclusive. 1~16 for ADC1 and ADC2; 1~20 for ADC3.
 * 'APP_DMA_TRANS_COUNT': 0~65535, inclusive. 0: always transmit.
 */
#define APP_DMA_UNIT                        (M4_DMA1)
#define APP_DMA_CH                          (DMA_CH0)
#define APP_DMA_PERIP_CLK                   (PWC_FCG0_DMA1)
#define APP_DMA_BLOCK_SIZE                  (1U)
#define APP_DMA_TRANS_COUNT                 (100UL)
#define APP_DMA_DATA_WIDTH                  (DMA_DATAWIDTH_16BIT)
#define APP_DMA_TRIG_SRC                    (EVT_ADC1_EOCA)
#define APP_DMA_SRC_ADDR                    (&M4_ADC1->DR3)

#define APP_DMA_IRQ_SRC                     (INT_DMA1_TC0)
#define APP_DMA_IRQn                        (Int010_IRQn)
#define APP_DMA_INT_PRIO                    (DDL_IRQ_PRIORITY_03)
#define APP_DMA_TRANS_CPLT_FLAG             (DMA_TC_INT_CH0)

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
static void AdcDmaConfig(void);
static void AdcDmaIrqConfig(void);
static void AdcSyncConfig(void);
static void AdcTrigSrcConfig(void);

static void DMA_Btc0_IrqCallback(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

static void IndicateConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/*static uint16_t m_au16AdcSaVal[APP_ADC_SA_CH_COUNT];*/
static uint16_t m_au16AdcSaVal[APP_DMA_TRANS_COUNT];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_07_sync project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the PLLHP(240MHz) as the system clock. */
    SystemClockConfig();
    /* Configures ADC. */
    AdcConfig();
    /* Use TimerA's PWM to indicate the scan timing of ADC synchronous mode. */
    IndicateConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /***************** Configuration end, application start **************/

    while (1U)
    {
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
 * @brief  Configures the PLLHP(240MHz) as the system clock.
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
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 | \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV4 | CLK_EXCLK_DIV2 | \
                CLK_HCLK_DIV1));

    CLK_PLLHStrucInit(&stcPLLHInit);
    /*
     * PLLP_freq = ((PLL_source / PLLM) * PLLN) / PLLP
     *           = (8 / 1) * 120 / 4
     *           = 240
     */
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = (1UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLN = (120UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLP = (4UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLQ = (16UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLR = (16UL  - 1UL);

    /* stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL; */
    CLK_PLLHInit(&stcPLLHInit);

    /* Set SRAM wait cycles. */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    /* Set EFM wait cycle. 5 wait cycles needed when system clock is 240MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);

    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
    PWC_Lock(PWC_UNLOCK_CODE_0);
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
    AdcDmaConfig();
    AdcSyncConfig();
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
    CLK_PERI_ClkConfig(CLK_PERI_CLK_PLLHQ);
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

    /* 1. Modify the default value depends on the application. It is not needed in this example. */
    stcInit.u16ScanMode = APP_ADC_MODE;

    /* 2. Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_ADC_PERIP_CLK, Enable);

    /* 3. Initializes ADC. */
    ADC_Init(M4_ADC1, &stcInit);
    ADC_Init(M4_ADC2, &stcInit);

#if (APP_ADC_SYNC_UNITS == ADC_SYNC_ADC1_ADC2_ADC3)
    ADC_Init(M4_ADC3, &stcInit);
#endif
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
    AdcSetChannelPinAnalogMode(M4_ADC1, APP_ADC1_SA_CH);
    AdcSetChannelPinAnalogMode(M4_ADC2, APP_ADC2_SA_CH);

#if (APP_ADC_SYNC_UNITS == ADC_SYNC_ADC1_ADC2_ADC3)
    AdcSetChannelPinAnalogMode(M4_ADC3, APP_ADC3_SA_CH);
#endif

    /* 2. Enable the ADC channels. */
    ADC_ChannelCmd(M4_ADC1, ADC_SEQ_A, \
                   APP_ADC1_SA_CH, au8AdcSASplTime, \
                   Enable);

    ADC_ChannelCmd(M4_ADC2, ADC_SEQ_A, \
                   APP_ADC2_SA_CH, au8AdcSASplTime, \
                   Enable);

#if (APP_ADC_SYNC_UNITS == ADC_SYNC_ADC1_ADC2_ADC3)
    ADC_ChannelCmd(M4_ADC3, ADC_SEQ_A, \
                   APP_ADC3_SA_CH, au8AdcSASplTime, \
                   Enable);
#endif
}

/**
 * @brief  Synchronous mode configuration.
 * @param  None
 * @retval None
 */
static void AdcSyncConfig(void)
{
    ADC_SYNC_Config(APP_ADC_SYNC_UNITS, APP_ADC_SYNC_MODE, APP_ADC_SYNC_TRIG_DELAY);
    ADC_SYNC_Cmd(Enable);
}

/**
 * @brief  Trigger source configuration. Specify trigger source synchronous mode.
 * @param  None
 * @retval None
 */
static void AdcTrigSrcConfig(void)
{
    stc_adc_trig_cfg_t stcTrigCfg;

    /*
     * Configures the trigger source of sequence A.
     * One of ADTRGx(x is the ADC unit number, x=1, 2, 3) pins is configured to trigger sequence A.
     * Make a falling edge on ADTRGx and hold the low level at least 1.5 PCLK4 cycles to trigger sequence A.
     */
    ADC_TrigSrcStructInit(&stcTrigCfg);
    stcTrigCfg.u16TrigSrc = APP_ADC_SA_TRIG_SRC_TYPE;
    GPIO_SetFunc(APP_ADC_SA_TRIG_SRC_PORT, APP_ADC_SA_TRIG_SRC_PIN, \
                 APP_ADC_SA_TRIG_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    ADC_TrigSrcConfig(M4_ADC1, ADC_SEQ_A, &stcTrigCfg);
    ADC_TrigSrcCmd(M4_ADC1, ADC_SEQ_A, Enable);
}

/**
 * @brief  DMA configuration.
 * @param  None
 * @retval None
 */
static void AdcDmaConfig(void)
{
    stc_dma_init_t stcDmaInit;
    stc_dma_rpt_init_t stcDmaRptInit;

    /* Enable DMA peripheral clock and AOS function. */
    PWC_Fcg0PeriphClockCmd((APP_DMA_PERIP_CLK | PWC_FCG0_AOS), Enable);

    DMA_SetTriggerSrc(APP_DMA_UNIT, APP_DMA_CH, APP_DMA_TRIG_SRC);

    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = APP_DMA_BLOCK_SIZE;
    stcDmaInit.u32TransCnt  = APP_DMA_TRANS_COUNT;
    stcDmaInit.u32DataWidth = APP_DMA_DATA_WIDTH;
    stcDmaInit.u32DestAddr  = (uint32_t)(&m_au16AdcSaVal[0U]);
    stcDmaInit.u32SrcAddr   = (uint32_t)APP_DMA_SRC_ADDR;
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;
    DMA_Init(APP_DMA_UNIT, APP_DMA_CH, &stcDmaInit);

    DMA_RepeatStructInit(&stcDmaRptInit);
    stcDmaRptInit.u32SrcRptEn    = DMA_SRC_RPT_ENABLE;
    stcDmaRptInit.u32SrcRptSize  = APP_DMA_BLOCK_SIZE;
    stcDmaRptInit.u32DestRptEn   = DMA_DEST_RPT_ENABLE;
    stcDmaRptInit.u32DestRptSize = APP_DMA_TRANS_COUNT;
    DMA_RepeatInit(APP_DMA_UNIT, APP_DMA_CH, &stcDmaRptInit);

    AdcDmaIrqConfig();

    DMA_Cmd(APP_DMA_UNIT, Enable);
    DMA_ChannelCmd(APP_DMA_UNIT, APP_DMA_CH, Enable);
}

/**
 * @brief  Interrupt configuration.
 * @param  None
 * @retval None
 */
static void AdcDmaIrqConfig(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;

    stcIrqSignConfig.enIntSrc    = APP_DMA_IRQ_SRC;
    stcIrqSignConfig.enIRQn      = APP_DMA_IRQn;
    stcIrqSignConfig.pfnCallback = &DMA_Btc0_IrqCallback;

    INTC_IrqSignIn(&stcIrqSignConfig);
    DMA_ClearTransIntStatus(APP_DMA_UNIT, APP_DMA_TRANS_CPLT_FLAG);

    /* NVIC setting */
    NVIC_ClearPendingIRQ(APP_DMA_IRQn);
    NVIC_SetPriority(APP_DMA_IRQn, APP_DMA_INT_PRIO);
    NVIC_EnableIRQ(APP_DMA_IRQn);
}

/**
 * @brief  DMA IRQ handler.
 * @param  None
 * @retval None
 */
static void DMA_Btc0_IrqCallback(void)
{
	DMA_SetTransCnt(APP_DMA_UNIT, APP_DMA_CH, APP_DMA_TRANS_COUNT);
	DMA_ChannelCmd(APP_DMA_UNIT, APP_DMA_CH, Enable);
    DMA_ClearTransIntStatus(APP_DMA_UNIT, APP_DMA_TRANS_CPLT_FLAG);
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
 * @brief  Use TimerA's PWM to indicate the scan timing of ADC synchronous mode.
 *         The inversion of PWM level indicates the end of ADC scanning.
 *         ADC1: PE11
 *         ADC2: PB10
 *         ADC3: PE15
 * @param  None
 * @retval None
 */
static void IndicateConfig(void)
{
    stc_tmra_init_t stcTmrAInit;
    stc_tmra_pwm_cfg_t stcPwmCfg;

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMRA_1 | PWC_FCG2_TMRA_2 | PWC_FCG2_TMRA_7, Enable);

    TMRA_StructInit(&stcTmrAInit);
    stcTmrAInit.u32ClkSrc    = TMRA_CLK_HW_UP_EVENT;
    stcTmrAInit.u32PeriodVal = 0U;
    TMRA_Init(M4_TMRA_1, &stcTmrAInit);
    TMRA_Init(M4_TMRA_2, &stcTmrAInit);
    TMRA_Init(M4_TMRA_7, &stcTmrAInit);
    TMRA_SetTriggerSrc(M4_TMRA_1, TMRA_EVENT_USAGE_CNT, EVT_ADC1_EOCA);
    TMRA_SetTriggerSrc(M4_TMRA_2, TMRA_EVENT_USAGE_CNT, EVT_ADC2_EOCA);
    TMRA_SetTriggerSrc(M4_TMRA_7, TMRA_EVENT_USAGE_CNT, EVT_ADC3_EOCA);

    TMRA_SetFuncMode(M4_TMRA_1, TMRA_CH_2, TMRA_FUNC_COMPARE);
    TMRA_SetFuncMode(M4_TMRA_2, TMRA_CH_3, TMRA_FUNC_COMPARE);
    TMRA_SetFuncMode(M4_TMRA_7, TMRA_CH_4, TMRA_FUNC_COMPARE);

    TMRA_SetCmpVal(M4_TMRA_1, TMRA_CH_2, 0UL);
    TMRA_SetCmpVal(M4_TMRA_2, TMRA_CH_3, 0UL);
    TMRA_SetCmpVal(M4_TMRA_7, TMRA_CH_4, 0UL);

    TMRA_PWM_StructInit(&stcPwmCfg);
    TMRA_PWM_Config(M4_TMRA_1, TMRA_CH_2, &stcPwmCfg);
    TMRA_PWM_Config(M4_TMRA_2, TMRA_CH_3, &stcPwmCfg);
    TMRA_PWM_Config(M4_TMRA_7, TMRA_CH_4, &stcPwmCfg);

    TMRA_PWM_Cmd(M4_TMRA_1, TMRA_CH_2, Enable);
    TMRA_PWM_Cmd(M4_TMRA_2, TMRA_CH_3, Enable);
    TMRA_PWM_Cmd(M4_TMRA_7, TMRA_CH_4, Enable);

    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_11, \
                 GPIO_FUNC_4_TIMA1_PWM2, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_10, \
                 GPIO_FUNC_4_TIMA2_PWM3, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_15, \
                 GPIO_FUNC_4_TIMA7_PWM4, PIN_SUBFUNC_DISABLE);

    TMRA_Start(M4_TMRA_1);
    TMRA_Start(M4_TMRA_2);
    TMRA_Start(M4_TMRA_7);
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
