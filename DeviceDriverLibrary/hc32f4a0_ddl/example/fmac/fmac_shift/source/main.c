/**
 *******************************************************************************
 * @file  fmac/fmac_shift/source/main.c
 * @brief Main program FMAC for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb          First version
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
 * @addtogroup FMAC_Shift
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define FMACx              (M4_FMAC3)
#define ADC_UNIT           (M4_ADC1)
#define ADC_CH             (ADC_CH2)
#define ADC1_CH2_PORT      (GPIO_PORT_A)
#define ADC1_CH2_PIN       (GPIO_PIN_02)
#define FMAC_IRQn          (Int014_IRQn)
#define FMAC_SOURCE        (INT_FMAC_3)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SystemClockConfig(void);
static void FMACInitConfig(void);
static void ADCInitConfig(void);
static void IRQ_Config(void);
static void Peripheral_WE(void);
static void Peripheral_WP(void);
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
    /* Unlock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    EFM_FWMC_Unlock();
    /* Unlock EFM OPT write protect registers */
    //EFM_OTP_WP_Unlock();
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
    /* Lock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    //PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Lock SRAM register: WTCR */
    //SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OPT write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    //EFM_Lock();
}

/**
 * @brief  FMAC complete IRQ callback
 * @param  None
 * @retval None
 */
static void Fmac_IrqCallback(void)
{
    stc_fmac_result_t stcResult;

    /* Get result and output it*/
    FMAC_GetResult(FMACx, &stcResult);
    printf("\nThe result is %lu,%lu", stcResult.u32ResultHigh, stcResult.u32ResultLow);
    /* clear the interrupt flag */
    FMAC_GetStatus(FMACx);
}

/**
 * @brief  Main function of fmac_shift project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint16_t u16ADCVal = 0U;
    /* Unlock peripherals or registers */
    Peripheral_WE();
    /* Configures the PLLHP(240MHz) as the system clock. */
    SystemClockConfig();
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
    /* Configures FMAC. */
    FMACInitConfig();
    /* Configures FMAC. */
    ADCInitConfig();
    /*Configure IRQ handler && NVIC*/
    IRQ_Config();
    /* Start ADC */
    ADC_Start(ADC_UNIT);
    /* Lock peripherals or registers */
    Peripheral_WP();
    /* Wait end of conversion of sequence A */
    while(ADC_SeqGetStatus(ADC_UNIT, ADC_SEQ_FLAG_EOCA) == Reset)
    {
        ;
    }

    while (1)
    {
        u16ADCVal = ADC_GetValue(ADC_UNIT, ADC_CH_NUM_2);
        printf("\nThe input is %d", u16ADCVal);
        /* Input data to be filtered */
        FMAC_FIRInput(FMACx, (int16_t)u16ADCVal);
        DDL_DelayMS(200U);
    }
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
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV8 | CLK_PCLK2_DIV8 | \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV8 | CLK_EXCLK_DIV2 | \
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

    /* Highspeed SRAM set to 1 Read/Write wait cycle */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);

    /* SRAM1_2_3_4_backup set to 2 Read/Write wait cycle */
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);
    /* EFM wait cycle */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);   /* 5-wait @ 240MHz */


    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
}

/**
 * @brief  Initializes FMAC.
 * @param  None
 * @retval None
 */
static void FMACInitConfig(void)
{
    stc_fmac_init_t stcFmacInit;
    int16_t FiltFactor = 0x1234;

    /* Enable FMAC peripheral clock. */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_FMAC3, Enable);
    /* Enable FMAC3 */
    FMAC_Cmd(FMACx, Enable);
    /* FMAC function initialize */
    FMAC_StructInit(&stcFmacInit);
    FMAC_StructInit(&stcFmacInit);
    stcFmacInit.i16FiltFactor = FiltFactor;
    stcFmacInit.u32FiltStage  = FMAC_FILTER_STAGE_10;
    stcFmacInit.u32FiltShift  = FMAC_FILTER_SHIFT_6BIT;
    stcFmacInit.u32IntCmd     = FMAC_INT_ENABLE;
    FMAC_Init(FMACx, &stcFmacInit);
}

/**
 * @brief  Initializes ADC.
 * @param  None
 * @retval None
 */
static void ADCInitConfig(void)
{
    stc_adc_init_t stcAdcInit;
    stc_gpio_init_t stcGpioInit;
    uint8_t u8AdcSplTime[] = {30U};
    /* Enable ADC1 peripheral clock. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_ADC1, Enable);
    /* ADC function Initialize */
    ADC_StructInit(&stcAdcInit);
    stcAdcInit.u16ScanMode = ADC_MODE_SA_CONT;
    ADC_Init(ADC_UNIT, &stcAdcInit);
    /* Set the specified Pin to analog mode */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    GPIO_Init(ADC1_CH2_PORT, ADC1_CH2_PIN, &stcGpioInit);
    /* Enable the ADC channels. */
    ADC_ChannelCmd(ADC_UNIT, ADC_SEQ_A,  \
                   ADC_CH, u8AdcSplTime,\
                   Enable);

}

/**
 * @brief  Configure IRQ handler && NVIC.
 * @param  None
 * @retval None
 */
static void IRQ_Config(void)
{
    stc_irq_signin_config_t stcIrqRegCfg;

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegCfg.enIRQn = FMAC_IRQn;
    stcIrqRegCfg.enIntSrc = FMAC_SOURCE;
    stcIrqRegCfg.pfnCallback = &Fmac_IrqCallback;
    INTC_IrqSignIn(&stcIrqRegCfg);
    NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIORITY_15);
    NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);
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
