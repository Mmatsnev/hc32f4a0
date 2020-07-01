/**
 *******************************************************************************
 * @file  timer4/timer4_sevt_compare_trigger/source/main.c
 * @brief This example demonstrates how to use the compare trigger function of 
 *        Timer4 SEVT function.
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
 * @addtogroup TMR4_SEVT_Compare_Trigger
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Timer4 unit && fcg && Counter period value definition */
#define TMR4_UNIT                       (M4_TMR4_1)
#define TMR4_FUNCTION_CLK_GATE          (PWC_FCG2_TMR4_1)
#define TMR4_CNT_CYCLE_VAL(div)         ((uint16_t)((Tmr4PclkFreq() /  (1UL << (uint32_t)(div)) / 4UL))) /* Period_Value(250ms) */

/* Timer4 SEVT port & channel */
#define TMR4_SEVT_PORT                  (GPIO_PORT_D)   /* TIM4_1_ADSM */
#define TMR4_SEVT_PIN                   (GPIO_PIN_12)

/* Timer4 SEVT channel definition */
#define TMR4_SEVT_CH                    (TMR4_SEVT_UH)

/* Timer4 SEVT output event definition */
#define TMR4_SEVT_OUTPUT_EVENT          (TMR4_SEVT_OUTPUT_EVENT0)
#define TMR4_SEVT_PORT_OUTPUT           (TMR4_SEVT_PORT_OUTPUT_EVENT0_SIGNAL)

/* ADC unit definitions */
#define ADC_UNIT                        (M4_ADC1)
#define ADC_FUNCTION_CLK_GATE           (PWC_FCG3_ADC1)

/* ADC port & channel */
#define ADC_PORT                        (GPIO_PORT_A)
#define ADC_PIN                         (GPIO_PIN_03)
#define ADC_CH                          (ADC_CH3)

/* Trigger source definitions. */
#define ADC_TRIG_SRC_TYPE               (ADC_TRIG_SRC_EVENT0)
#define ADC_TRIG_SRC_EVENT              (EVT_TMR4_1_SCMUH)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static uint32_t Tmr4PclkFreq(void);
static void AdcConfig(void);
static void AdcInitConfig(void);
static void AdcChannelConfig(void);
static void AdcTrigSrcConfig(void);

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
 * @brief  ADC configuration, including clock configuration, initial configuration
 *         and channel configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    AdcInitConfig();
    AdcChannelConfig();
    AdcTrigSrcConfig();
}

/**
 * @brief  Initializes ADC.
 * @param  None
 * @retval None
 */
static void AdcInitConfig(void)
{
    stc_adc_init_t stcInit;

    PWC_Fcg3PeriphClockCmd(ADC_FUNCTION_CLK_GATE, Enable);

    ADC_StructInit(&stcInit);
    stcInit.u16ScanMode = ADC_MODE_SA_SSHOT;
    ADC_Init(ADC_UNIT, &stcInit);
}

/**
 * @brief  Configures ADC channel(s).
 * @param  None
 * @retval None
 */
static void AdcChannelConfig(void)
{
    stc_gpio_init_t stcGpioInit;
    uint8_t au8AdcSASplTime[] = {30};

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    GPIO_Init(ADC_PORT, ADC_PIN, &stcGpioInit);

    ADC_ChannelCmd(ADC_UNIT, ADC_SEQ_A, ADC_CH, au8AdcSASplTime, Enable);
}

/**
 * @brief  Trigger source configuration. Specify trigger source for sequence A and sequence B.
 * @param  None
 * @retval None
 */
static void AdcTrigSrcConfig(void)
{
    stc_adc_trig_cfg_t stcTrigCfg;

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);

    ADC_TrigSrcStructInit(&stcTrigCfg);
    stcTrigCfg.u16TrigSrc = ADC_TRIG_SRC_TYPE;
    stcTrigCfg.enEvent0   = ADC_TRIG_SRC_EVENT;
    ADC_TrigSrcConfig(ADC_UNIT, ADC_SEQ_A, &stcTrigCfg);
    ADC_TrigSrcCmd(ADC_UNIT, ADC_SEQ_A, Enable);
}

/**
 * @brief  Main function of TIMER4 SEVT compare trigger
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    static uint16_t m_u16AdcVal = 0U;
    stc_tmr4_cnt_init_t stcTmr4CntInit;
    stc_tmr4_sevt_init_t stcTmr4SevtInit;

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize clock. */
    BSP_CLK_Init();

    /* Initialize UART for debug print function. */
    DDL_PrintfInit();

    /* Configure ADC. */
    AdcConfig();

    /* Configure Timer4 SEVT port*/
    GPIO_SetFunc(TMR4_SEVT_PORT, TMR4_SEVT_PIN, GPIO_FUNC_2, PIN_SUBFUNC_DISABLE);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Enable peripheral clock */
    PWC_Fcg2PeriphClockCmd(TMR4_FUNCTION_CLK_GATE, Enable);

    /* Initialize TIMER4 Counter. */
    TMR4_CNT_StructInit(&stcTmr4CntInit);
    stcTmr4CntInit.u16PclkDiv = TMR4_CNT_PCLK_DIV1024;
    stcTmr4CntInit.u16CycleVal = TMR4_CNT_CYCLE_VAL(stcTmr4CntInit.u16PclkDiv);
    TMR4_CNT_Init(TMR4_UNIT, &stcTmr4CntInit);

    /* Initialize TIMER4 SEVT. */
    TMR4_SEVT_StructInit(&stcTmr4SevtInit);
    stcTmr4SevtInit.u16UpMatchCmd = TMR4_SEVT_UP_ENABLE;
    stcTmr4SevtInit.u16OutpuEvent = TMR4_SEVT_OUTPUT_EVENT;
    stcTmr4SevtInit.u16Mode = TMR4_SEVT_MODE_COMPARE_TRIGGER;
    stcTmr4SevtInit.u16CompareVal = stcTmr4CntInit.u16CycleVal/2U;
    TMR4_SEVT_Init(TMR4_UNIT, TMR4_SEVT_CH, &stcTmr4SevtInit);
    TMR4_SEVT_SetPortOutputEventSig(TMR4_UNIT, TMR4_SEVT_PORT_OUTPUT);

    /* Start TIMER4 counter. */
    TMR4_CNT_Start(TMR4_UNIT);

    while (1)
    {
        if (Set == ADC_SeqGetStatus(ADC_UNIT, ADC_SEQ_FLAG_EOCA))
        {
            ADC_GetChannelData(ADC_UNIT, ADC_CH, &m_u16AdcVal, 1U);
            ADC_SeqClrStatus(ADC_UNIT, ADC_SEQ_FLAG_EOCA);

            printf("ADC sample value: %d\r\n", m_u16AdcVal);
        }
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
