/**
 *******************************************************************************
 * @file  adc/adc_05_pga/source/main.c
 * @brief Main program ADC PGA for the Device Driver Library.
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
 * @addtogroup ADC_PGA
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
 * ADC1 contains PGA1, PGA2 and PGA3, corresponds to channel 0 ~ 3 of ADC1.
 * ADC2 contains PGA4, corresponds to channel 0 of ADC2.
 * ADC3 DO NOT support PGA.
 */
#define APP_ADC_UNIT1                       (M4_ADC1)
#define APP_ADC_UNIT2                       (M4_ADC2)
#define APP_ADC_PERIP_CLK                   (PWC_FCG3_ADC2 | PWC_FCG3_ADC1)
#define APP_PGA_PERIP_CLK                   (PWC_FCG3_CMBIAS)

/* Definitions of PGA. */
#define APP_ADC_PGA1                        (ADC_PGA_1)
#define APP_ADC_PGA3                        (ADC_PGA_3)
#define APP_ADC_PGA4                        (ADC_PGA_4)

#define APP_ADC_PGA1_GAIN                   (ADC_PGA_GAIN_2P286)
#define APP_ADC_PGA3_GAIN                   (ADC_PGA_GAIN_6P4)
#define APP_ADC_PGA4_GAIN                   (ADC_PGA_GAIN_10P667)

#define APP_ADC_PGA1_VSS                    (ADC_PGA_VSS_AVSS)
#define APP_ADC_PGA3_VSS                    (ADC_PGA_VSS_AVSS)
#define APP_ADC_PGA4_VSS                    (ADC_PGA_VSS_AVSS)

/*
 * ADC channels definition for this example.
 * PGA1: Input pin is ADC123_IN0(PA0), which corresponds to channel 0 of ADC1 by default.
 * PGA2: Input pin is ADC123_IN1(PA1), which corresponds to channel 1 of ADC1 by default.
 * PGA3: Input pin is ADC123_IN2(PA2), which corresponds to channel 2 of ADC1 by default.
 * PGA4: Input pin is ADC12_IN6(PA6), which corresponds to channel 6 of ADC2 by default.
 */
#define ADC_PGA1_CH                         (ADC_CH0)
#define ADC_PGA3_CH                         (ADC_CH2)
#define ADC_PGA4_CH                         (ADC_CH6)

#define APP_ADC1_PGA_CH                     (ADC_PGA1_CH | ADC_PGA3_CH)
#define APP_ADC1_SA_CH                      (APP_ADC1_PGA_CH)
#define APP_ADC1_SA_CH_COUNT                (2U)

#define APP_ADC2_PGA_CH                     (ADC_PGA4_CH)
#define APP_ADC2_SA_CH                      (APP_ADC2_PGA_CH)
#define APP_ADC2_SA_CH_COUNT                (1U)

/* ADC1 channel sampling time.              ADC_CH0   ADC_CH2 */
#define APP_ADC1_SA_SAMPLE_TIME             { 30,       30 }
/* ADC2 channel sampling time.              ADC_CH6 */
#define APP_ADC2_SA_SAMPLE_TIME             { 30 }

/* ADC reference voltage. The voltage of pin VREFH. */
#define APP_ADC_VREF                        (3.2551)

/* ADC accuracy(according to the resolution of ADC). */
#define APP_ADC_ACCURACY                    (1UL << 12U)

/* Calculate the voltage. */
#define APP_CAL_VOL(adcVal)                 ((float)(adcVal) * APP_ADC_VREF) / ((float)APP_ADC_ACCURACY)

/* Timeout value definitions. */
#define APP_TIMEOUT_MS                      (10U)

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
static void AdcPgaConfig(void);

static void AdcSetChannelPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
static void AdcSetPinAnalogMode(const M4_ADC_TypeDef *ADCx, uint8_t u8PinNum);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
#if (DDL_PRINT_ENABLE == DDL_ON)
static uint16_t m_au16Adc1SaInVal[APP_ADC1_SA_CH_COUNT];
static uint16_t m_au16Adc2SaInVal[APP_ADC2_SA_CH_COUNT];
#endif

static uint16_t m_au16Adc1SaVal[APP_ADC1_SA_CH_COUNT];
static uint16_t m_au16Adc2SaVal[APP_ADC2_SA_CH_COUNT];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of adc_05_pga project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The default system clock source is MRC(8MHz). */

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
#if (DDL_PRINT_ENABLE == DDL_ON)
        ADC_PGA_Cmd(APP_ADC_PGA1, Disable);
        ADC_PGA_Cmd(APP_ADC_PGA3, Disable);
        ADC_PGA_Cmd(APP_ADC_PGA4, Disable);
        ADC_PollingSA(APP_ADC_UNIT1, m_au16Adc1SaInVal, APP_ADC1_SA_CH_COUNT, APP_TIMEOUT_MS);
        ADC_PollingSA(APP_ADC_UNIT2, m_au16Adc2SaInVal, APP_ADC2_SA_CH_COUNT, APP_TIMEOUT_MS);

        ADC_PGA_Cmd(APP_ADC_PGA1, Enable);
        ADC_PGA_Cmd(APP_ADC_PGA3, Enable);
        ADC_PGA_Cmd(APP_ADC_PGA4, Enable);
        ADC_PollingSA(APP_ADC_UNIT1, m_au16Adc1SaVal, APP_ADC1_SA_CH_COUNT, APP_TIMEOUT_MS);
        ADC_PollingSA(APP_ADC_UNIT2, m_au16Adc2SaVal, APP_ADC2_SA_CH_COUNT, APP_TIMEOUT_MS);

        DBG("\t-------------------------------------------------------------------------------------\n");
        DBG("\t--->> PGA1 input voltage: %.3f, output voltage: %.3f.\n", APP_CAL_VOL(m_au16Adc1SaInVal[0U]), APP_CAL_VOL(m_au16Adc1SaVal[0U]));
        DBG("\t--->> PGA3 input voltage: %.3f, output voltage: %.3f.\n", APP_CAL_VOL(m_au16Adc1SaInVal[1U]), APP_CAL_VOL(m_au16Adc1SaVal[1U]));
        DBG("\t--->> PGA4 input voltage: %.3f, output voltage: %.3f.\n", APP_CAL_VOL(m_au16Adc2SaInVal[0U]), APP_CAL_VOL(m_au16Adc2SaVal[0U]));
        DDL_DelayMS(1000U);
#else
        ADC_PollingSA(APP_ADC_UNIT1, m_au16Adc1SaVal, APP_ADC1_SA_CH_COUNT, APP_TIMEOUT_MS);
        ADC_PollingSA(APP_ADC_UNIT2, m_au16Adc2SaVal, APP_ADC2_SA_CH_COUNT, APP_TIMEOUT_MS);
        /* TODO: Use the ADC values. */
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
    // PWC_FCG0_Unlock();
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
    // PWC_FCG0_Lock();
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
 *         channel configuration and PGA configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcClockConfig();
    AdcInitConfig();
    AdcChannelConfig();
    AdcPgaConfig();
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

    /* 1. Modify the default value depends on the application. It is not needed in this example. */

    /* 2. Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_ADC_PERIP_CLK, Enable);

    /* 3. Initializes ADC. */
    ADC_Init(APP_ADC_UNIT1, &stcInit);
    ADC_Init(APP_ADC_UNIT2, &stcInit);
}

/**
 * @brief  Configures ADC channel(s).
 * @param  None
 * @retval None
 */
static void AdcChannelConfig(void)
{
    uint8_t au8Adc1SASplTime[] = APP_ADC1_SA_SAMPLE_TIME;
    uint8_t au8Adc2SASplTime[] = APP_ADC2_SA_SAMPLE_TIME;

    /* 1. Set the ADC pin to analog input mode. */
    AdcSetChannelPinAnalogMode(APP_ADC_UNIT1, APP_ADC1_SA_CH);
    AdcSetChannelPinAnalogMode(APP_ADC_UNIT2, APP_ADC2_SA_CH);

    /* 2. Enable the ADC channels. */
    ADC_ChannelCmd(APP_ADC_UNIT1, ADC_SEQ_A, \
                   APP_ADC1_SA_CH, au8Adc1SASplTime, \
                   Enable);
    ADC_ChannelCmd(APP_ADC_UNIT2, ADC_SEQ_A, \
                   APP_ADC2_SA_CH, au8Adc2SASplTime, \
                   Enable);
}

/**
 * @brief  ADC PGA configuration.
 * @param  None
 * @retval None
 */
static void AdcPgaConfig(void)
{
    /* 1. Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_PGA_PERIP_CLK, Enable);

    /* 2. Delay 2us is needed by PGA. */
    DDL_DelayUS(5U);

    /* 3. Specify the gain factor and the VSS. */
    ADC_PGA_Config(APP_ADC_PGA1, APP_ADC_PGA1_GAIN, APP_ADC_PGA1_VSS);
    ADC_PGA_Config(APP_ADC_PGA3, APP_ADC_PGA3_GAIN, APP_ADC_PGA3_VSS);
    ADC_PGA_Config(APP_ADC_PGA4, APP_ADC_PGA4_GAIN, APP_ADC_PGA4_VSS);

    /* 4. Enable the PGA. */
    ADC_PGA_Cmd(APP_ADC_PGA1, Enable);
    ADC_PGA_Cmd(APP_ADC_PGA3, Enable);
    ADC_PGA_Cmd(APP_ADC_PGA4, Enable);
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
