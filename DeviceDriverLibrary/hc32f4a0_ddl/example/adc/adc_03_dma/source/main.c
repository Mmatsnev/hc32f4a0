/**
 *******************************************************************************
 * @file  adc/adc_03_dma/source/main.c
 * @brief Main program ADC DMA for the Device Driver Library.
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
 * @addtogroup ADC_DMA
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*
 * ADC unit instance for this example.
 * Marco 'APP_ADC_UNIT' can be defined as M4_ADCx(x=1, 2, 3).
 * Definition of 'APP_PERIP_CLK_ADC' depends on 'APP_ADC_UNIT'.
 */
#define APP_ADC_UNIT                        (M4_ADC1)
#define APP_ADC_PERIP_CLK                   (PWC_FCG3_ADC1)

/*
 * Specifies the ADC channels according to the application.
 * NOTE!!! Sequence A and sequence B CANNOT contain the same channel.
 */
#define APP_ADC_SA_CH                       (ADC_CH2 | ADC_CH3 | ADC_CH5)
#define APP_ADC_SA_CH_COUNT                 (3U)

#define APP_ADC_SA_CH_NUM_MIN               (ADC_CH_NUM_2)
#define APP_ADC_SA_CH_NUM_MAX               (ADC_CH_NUM_5)

/* Sampling time of ADC channels. */
/*                                        ADC_CH2    ADC_CH3   ADC_CH5 */
#define APP_ADC_SA_SAMPLE_TIME              { 30,       30,       35 }

/*
 * Add the channels which were included in sequence A or sequence B to average channel if needed.
 * The average channels will be sampled a specified number of times(specified by 'APP_ADC_AVG_CNT'),\
 *   and the final ADC value is the average of the specified number of samples.
 * Define 'APP_ADC_AVG_CH' as 0 to disable average channel.
 */
#define APP_ADC_AVG_CNT                     (ADC_AVG_CNT_8)
#define APP_ADC_AVG_CH                      (APP_ADC_SA_CH)

/* 
 * Trigger source definitions.
 * Set pin ADTRG1(PE7) to trigger sequence A. Make a falling edge on ADTRG1 and hold the low level \
 *   at least 1.5 cycles of PCLK4 to trigger sequence A.
 * Both sequences can be set triggered by event or the falling edge of pin ADTRGx(x is ADC unit number, x=1, 2, 3).
 * Only the falling edge of pin ADTRGx and the low level of pin ADTRGx must be held at least 1.5 cycles of PCLK4 can trigger ADC start sampling.
 * ADTRG1: PB7, PD6, PD14, PE7.
 * ADTRG2: PB6, PB10, PD7, PD15.
 * ADTRG3: PB5, PE9, PG2, PG9.
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
#define APP_DMA_BLOCK_SIZE                  (APP_ADC_SA_CH_NUM_MAX - APP_ADC_SA_CH_NUM_MIN + 1U)
#define APP_DMA_TRANS_COUNT                 (0UL)
#define APP_DMA_DATA_WIDTH                  (DMA_DATAWIDTH_16BIT)
#define APP_DMA_TRIG_SRC                    (EVT_ADC1_EOCA)
#define APP_DMA_SRC_ADDR                    (&M4_ADC1->DR2)

#define APP_DMA_IRQ_SRC                     (INT_DMA1_BTC0)
#define APP_DMA_IRQn                        (Int010_IRQn)
#define APP_DMA_INT_PRIO                    (DDL_IRQ_PRIORITY_03)
#define APP_DMA_TRANS_CPLT_FLAG             (DMA_BTC_INT_CH0)

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
static void AdcDmaConfig(void);
static void AdcDmaIrqConfig(void);
static void AdcTrigSrcConfig(void);

static void DMA_Btc0_IrqCallback(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t m_au16AdcSaVal[APP_DMA_BLOCK_SIZE];
static uint32_t m_u32AdcValUpdated = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_03_dma project
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

    /***************** Configuration end, application start **************/

    while (1U)
    {
        /*
         * Make a falling edge on the specified pin ADTRGx and hold the low level \
         *   at least 1.5 cycles of PCLK4 to trigger sequence A.
         */
        if ((m_u32AdcValUpdated & ADC_SEQ_FLAG_EOCA) != 0U)
        {
            DBG("ADC scan completed. The data has been read via DMA.\n");
            m_u32AdcValUpdated &= (uint32_t)(~ADC_SEQ_FLAG_EOCA);
        }
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
 *         channel configuration, DMA configuration and trigger source configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcDmaConfig();
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
    CLK_ClkDiv((CLK_CATE_PCLK2 | CLK_CATE_PCLK4), (CLK_PCLK2_DIV4 | CLK_PCLK4_DIV2));

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
 * @brief  Trigger source configuration. Specify trigger source for sequence A and sequence B.
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
    ADC_TrigSrcConfig(APP_ADC_UNIT, ADC_SEQ_A, &stcTrigCfg);
    ADC_TrigSrcCmd(APP_ADC_UNIT, ADC_SEQ_A, Enable);
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
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;
    DMA_Init(APP_DMA_UNIT, APP_DMA_CH, &stcDmaInit);

    DMA_RepeatStructInit(&stcDmaRptInit);
    stcDmaRptInit.u32SrcRptEn    = DMA_SRC_RPT_ENABLE;
    stcDmaRptInit.u32SrcRptSize  = APP_DMA_BLOCK_SIZE;
    stcDmaRptInit.u32DestRptEn   = DMA_DEST_RPT_ENABLE;
    stcDmaRptInit.u32DestRptSize = APP_DMA_BLOCK_SIZE;
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
    DMA_ClearTransIntStatus(APP_DMA_UNIT, APP_DMA_TRANS_CPLT_FLAG);
    m_u32AdcValUpdated |= ADC_SEQ_FLAG_EOCA;
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
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
