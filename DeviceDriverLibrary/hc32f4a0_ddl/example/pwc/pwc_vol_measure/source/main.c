/**
 *******************************************************************************
 * @file  pwc/pwc_vol_measure/source/main.c
 * @brief Main program of PWC voltage measure for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Yangjp          First version
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
 * @addtogroup PWC_Voltage_Measure
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* ADC unit instance for this example */
#define APP_ADC_UNIT                        (M4_ADC1)
#define APP_ADC_PERIP_CLK                   (PWC_FCG3_ADC1)
#define APP_ADC_SA_CH                       (ADC_EXT_CH)

/* ADC channel sampling time */
#define APP_ADC_SA_SAMPLE_TIME              (25U)
/* Timeout value definitions. */
#define APP_TIMEOUT_MS                      (10U)

/* ADC reference voltage. The voltage of pin VREFH. */
#define APP_ADC_VREF                        (3.2867f)
/* ADC accuracy(according to the resolution of ADC). */
#define APP_ADC_ACCURACY                    (1UL << 12U)
/* Calculate the voltage. */
#define APP_CAL_VOL(adcVal)                 ((float)(adcVal) * APP_ADC_VREF) / ((float)APP_ADC_ACCURACY)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint16_t u16AdcSaVal;

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
    // GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    // PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  ADC configuration, including clock configuration, initial configuration
 *         and channel configuration.
 * @param  None
 * @retval None
 */
static void AdcConfig(void)
{
    stc_adc_init_t stcInit;
    uint8_t u8AdcSASplTime = APP_ADC_SA_SAMPLE_TIME;

    /* Specify the clock source of ADC */
    CLK_PERI_ClkConfig(CLK_PERI_CLK_PCLK);
    /* Enable ADC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(APP_ADC_PERIP_CLK, Enable);

    ADC_StructInit(&stcInit);
    /* Initializes ADC. */
    ADC_Init(APP_ADC_UNIT, &stcInit);

    /*  Enable the ADC channels. */
    ADC_ChannelCmd(APP_ADC_UNIT, ADC_SEQ_A, APP_ADC_SA_CH, &u8AdcSASplTime, Enable);
    ADC_SetExChannelSrc(APP_ADC_UNIT, ADC_EXCH_SRC_INTERNAL);
}

/**
 * @brief  Main function of PWC voltage measure.
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Peripheral registers write unprotected */
    Peripheral_WE();
    /* Configure clock */
    BSP_CLK_Init();
    /* Reset VBAT area */
    PWC_VBAT_Reset();
    /* Configure BSP */
    BSP_IO_Init();
    BSP_LED_Init();
    /* Configure UART */
    DDL_PrintfInit();
    /* Configures ADC. */
    AdcConfig();

    /* Get the internal reference voltage. */
    PWC_VBAT_MonitorCmd(Disable);
    PWC_VBAT_MeasureVolCmd(Disable);
    PWC_AdcInternVolSel(PWC_AD_INTERN_REF);
    PWC_AdcBufCmd(Enable);
    /* Delay 50us is needed. */
    DDL_DelayMS(1U);
    ADC_PollingSA(APP_ADC_UNIT, &u16AdcSaVal, 1U, APP_TIMEOUT_MS);
    printf("Internal reference voltage: adc value is %d, voltage is %.3f.\n", u16AdcSaVal, APP_CAL_VOL(u16AdcSaVal));

    /* Get the voltage of VBAT. */
    PWC_VBAT_MeasureVolCmd(Enable);
    PWC_AdcInternVolSel(PWC_AD_VBAT_DIV2);
    PWC_VBAT_MonitorCmd(Enable);
    PWC_AdcBufCmd(Enable);
    /* Delay 50us is needed. */
    DDL_DelayMS(1U);
    ADC_PollingSA(APP_ADC_UNIT, &u16AdcSaVal, 1U, APP_TIMEOUT_MS);
    printf("VBAT: adc value is %d, voltage is %.3f.\n", u16AdcSaVal * 2U, APP_CAL_VOL(u16AdcSaVal) * 2.0f);

    /* VBAT voltage measure. */
    PWC_VBAT_MeasureVolCmd(Disable);
    PWC_VBAT_MonitorVolSel(PWC_VBAT_REF_VOL_2P0V);
    PWC_VBAT_MonitorCmd(Enable);
    /* Delay 50us is needed. */
    DDL_DelayMS(1U);
    /* Peripheral registers write protected */
    Peripheral_WP();

    while (1U)
    {
        ADC_PollingSA(APP_ADC_UNIT, &u16AdcSaVal, 1U, APP_TIMEOUT_MS);
        if (PWC_VOL_VBAT_MORE_THAN_VBATREF == PWC_VBAT_GetVolStatus())
        {
            /* Vbat > VbatRef */
            BSP_LED_On(LED_BLUE);
            BSP_LED_Off(LED_RED);
        }
        else
        {
            /* Vbat < VbatRef */
            BSP_LED_On(LED_BLUE);
            BSP_LED_Off(LED_RED);
        }
        DDL_DelayMS(100U);
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
