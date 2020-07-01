/**
 *******************************************************************************
 * @file  timer2/timer2_async_clock/source/main.c
 * @brief Main program Timer2 async clock for the Device Driver Library.
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
 * @addtogroup TMR2_Async_Clock
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*
 * Function of this example.
 * This example is going to use asynchronous clock sources to count 10 milliseconds.
 */

/*
 * Timer2 unit and channel definitions for this example.
 * 'APP_TMR2_UNIT' can be defined as M4_TMR2_<t>(t=1 ~ 4).
 * 'APP_TMR2_CH' can be defined as TMR2_CH_x(x=A, B).
 */
#define APP_TMR2_UNIT                       (M4_TMR2_1)
#define APP_TMR2_CH                         (TMR2_CH_A)
#define APP_TMR2_PERIP_CLK                  (PWC_FCG2_TMR2_1)

/*
 * Clock source of Timer2 in this example.
 * 'APP_CLK_ASYNC_LRC': LRC, 32.768KHz.
 * 'APP_CLK_ASYNC_XTAL32': XTAL32, 32.768KHz.
 * 'APP_CLK_ASYNC_PIN_CLK': Clock from pin TIM2_<t>_CLKx(t=1 ~ 4, x=A, B). The frequency is up to 25MHz. \
 *                          2MHz is used in this example.
 */
#define APP_CLK_ASYNC_LRC                   (1U)
#define APP_CLK_ASYNC_XTAL32                (2U)
#define APP_CLK_ASYNC_PIN_CLK               (3U)

/* Select a clock source for this example. */
#define APP_TMR2_CLK                        (APP_CLK_ASYNC_LRC)

/*
 * Define configuration values according to the clock source just selected.
 * A simple formula for calculating the compare value is:
 *   Tmr2CompareVal = (Tmr2Period(us) * [Tmr2ClockSource(MHz) / Tmr2ClockDiv]) - 1.
 */
#if (APP_TMR2_CLK == APP_CLK_ASYNC_LRC)
    #define APP_TMR2_CLK_SRC                (TMR2_CLK_ASYNC_LRC)
    #define APP_TMR2_CLK_DIV                (TMR2_CLK_DIV1)
    #define APP_TMR2_CMP_VAL                (327UL - 1U)

#elif (APP_TMR2_CLK == APP_CLK_ASYNC_XTAL32)
    #define APP_TMR2_CLK_SRC                (TMR2_CLK_ASYNC_XTAL32)
    #define APP_TMR2_CLK_DIV                (TMR2_CLK_DIV1)
    #define APP_TMR2_CMP_VAL                (327UL - 1U)

#elif (APP_TMR2_CLK == APP_CLK_ASYNC_PIN_CLK)
    #define APP_TMR2_CLK_SRC                (TMR2_CLK_ASYNC_PIN_CLK)
    #define APP_TMR2_CLK_DIV                (TMR2_CLK_DIV1)
    #define APP_TMR2_CMP_VAL                (20000UL - 1U)
    #define APP_TMR2_PIN_CLK_FREQ           (2000000UL)
    #define APP_TMR2_ASYNC_CLK_PORT         (GPIO_PORT_E)
    #define APP_TMR2_ASYNC_CLK_PIN          (GPIO_PIN_03)
    #define APP_TMR2_ASYNC_CLK_PIN_FUNC     (GPIO_FUNC_16_TIM21_CLKA)
#else
    #error "The clock source you selected is NOT supported in this example!"
#endif /* #if (APP_TMR2_CLK == APP_CLK_ASYNC_LRC) */

/*
 * Definitions about Timer2 interrupt for the example.
 * Timer2 independent IRQn: [Int000_IRQn, Int031_IRQn], [Int050_IRQn, Int055_IRQn].
 *
 * NOTE:
 *   'APP_TMR2_INT_TYPE' can only be defined as 'TMR2_INT_CMP' for this example.
 */
#define APP_TMR2_INT_TYPE                   (TMR2_INT_CMP)
#define APP_TMR2_INT_PRIO                   (DDL_IRQ_PRIORITY_03)
#define APP_TMR2_INT_SRC                    (INT_TMR2_1_CMPA)
#define APP_TMR2_IRQn                       (Int050_IRQn)

/* Indicate pin definition in this example. */
#define INDICATE_PORT                       (GPIO_PORT_A)
#define INDICATE_PIN                        (GPIO_PIN_10)
#define INDICATE_OUT_HIGH()                 (GPIO_SetPins(INDICATE_PORT, INDICATE_PIN))
#define INDICATE_OUT_LOW()                  (GPIO_ResetPins(INDICATE_PORT, INDICATE_PIN))
#define INDICATE_OUT_TOGGLE()               (GPIO_TogglePins(INDICATE_PORT, INDICATE_PIN))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void SystemClockConfig(void);
static void IndicateConfig(void);

static void Tmr2Config(void);
static void Tmr2ClkSrcEnable(void);
static void Tmr2IrqConfig(void);
static void TMR2_Cmp_IrqCallback(void);

#if (APP_TMR2_CLK == APP_CLK_ASYNC_PIN_CLK)
    static void ExternalClockConfig(void);
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_async_clock project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    SystemClockConfig();
    /* Configures indicator. */
    IndicateConfig();
    /* Configures Timer2. */
    Tmr2Config();
#if (APP_TMR2_CLK == APP_CLK_ASYNC_PIN_CLK)
    ExternalClockConfig();
#endif
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts Timer2. Timer2 will start after 3 asynchronous clocks. */
    TMR2_Start(APP_TMR2_UNIT, APP_TMR2_CH);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        /* See TMR2_Cmp_IrqCallback in this file. */

        /*
         * Pay attention to the operation of these registers: CNTAR/CNTBR, CMPAR/CMPBR, BCONR.CSTA/B, STFLR.CMFA/B
         *   1. If the asynchronous clock is valid:
         *      If you want to write a Timer2 register, you need to wait for at least 3 asynchronous clock cycles
         *      after the last write operation! e.g.:
         *        DDL_DelayUS(xxx);    // Wait at least 3 asynchronous clock cycles.
         *        TMR2_SetCmpVal(111);
         *        TMR2_SetCntVal(222);
         *        DDL_DelayUS(xxx);    // Wait at least 3 asynchronous clock cycles.
         *        TMR2_SetCmpVal(333);
         *        TMR2_SetCntVal(444);
         *   2. If the asynchronous clock is valid, the data will not be written to these registers.
         *   3. Read these registers when the timer stops counting. e.g.:
         *        TMR2_Stop(APP_TMR2_UNIT, APP_TMR2_CH);
         *        u16ReadVal = TMR2_GetCntVal(APP_TMR2_UNIT, APP_TMR2_CH)
         */
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
 * @brief  Configures a new system clock.
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
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 | \
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
}

/**
 * @brief  Timer2 configuration.
 * @param  None
 * @retval None
 */
static void Tmr2Config(void)
{
    stc_tmr2_init_t stcInit;

    /* 1. If selected asynchronous clock source, enable it firstly is recommended. */
    Tmr2ClkSrcEnable();

    /* 2. Enable Timer2 peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMR2_PERIP_CLK, Enable);

    /* 3. Set a default initialization value for stcInit. */
    TMR2_StructInit(&stcInit);

    /* 4. Modifies the initialization values depends on the application. */
    stcInit.u32ClkSrc = APP_TMR2_CLK_SRC;
    stcInit.u32ClkDiv = APP_TMR2_CLK_DIV;
    stcInit.u32CmpVal = APP_TMR2_CMP_VAL;
    TMR2_Init(APP_TMR2_UNIT, APP_TMR2_CH, &stcInit);

    /* 5. Configures IRQ if needed. */
    Tmr2IrqConfig();
}

/**
 * @brief  Enabled the clock source of Timer2.
 * @param  None
 * @retval None
 */
static void Tmr2ClkSrcEnable(void)
{
#if (APP_TMR2_CLK == APP_CLK_ASYNC_LRC)
    CLK_LrcCmd(Enable);

#elif (APP_TMR2_CLK == APP_CLK_ASYNC_XTAL32)
    CLK_Xtal32Cmd(Enable);

#elif (APP_TMR2_CLK == APP_CLK_ASYNC_PIN_CLK)
    GPIO_SetFunc(APP_TMR2_ASYNC_CLK_PORT, APP_TMR2_ASYNC_CLK_PIN, \
                 APP_TMR2_ASYNC_CLK_PIN_FUNC, PIN_SUBFUNC_DISABLE);
#endif
}

/**
 * @brief  Timer2 interrupt configuration.
 * @param  None
 * @retval None
 */
static void Tmr2IrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = APP_TMR2_INT_SRC;
    stcCfg.enIRQn      = APP_TMR2_IRQn;
    stcCfg.pfnCallback = &TMR2_Cmp_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_TMR2_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of Timer2. */
    TMR2_IntCmd(APP_TMR2_UNIT, APP_TMR2_CH, APP_TMR2_INT_TYPE, Enable);
}

/**
 * @brief  Timer2 counter comparison match interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR2_Cmp_IrqCallback(void)
{
    if (TMR2_GetStatus(APP_TMR2_UNIT, APP_TMR2_CH, TMR2_FLAG_CMP) == Set)
    {
        TMR2_ClrStatus(APP_TMR2_UNIT, APP_TMR2_CH, TMR2_FLAG_CMP);
        INDICATE_OUT_TOGGLE();
    }
}

#if (APP_TMR2_CLK == APP_CLK_ASYNC_PIN_CLK)
/**
 * @brief  Configures the external clock source for the specified 'APP_TMR2_UNIT'.
 *         The specified clock source is 2MHz.
 * @param  None
 * @retval None
 */
static void ExternalClockConfig(void)
{
    stc_tmra_init_t stcTmrAInit;
    stc_tmra_pwm_cfg_t stcPwmCfg;

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMRA_1, Enable);

    /* TimerA unit 1: clock source PCLK0(240MHZ). Divider: 2. Final count clock frequency 120MHz. */
    TMRA_StructInit(&stcTmrAInit);
    stcTmrAInit.u32ClkSrc    = TMRA_CLK_PCLK;
    stcTmrAInit.u32PCLKDiv   = TMRA_PCLK_DIV2;
    stcTmrAInit.u32PeriodVal = 60U - 1U;
    TMRA_Init(M4_TMRA_1, &stcTmrAInit);

    TMRA_SetFuncMode(M4_TMRA_1, TMRA_CH_2, TMRA_FUNC_COMPARE);
    TMRA_SetCmpVal(M4_TMRA_1, TMRA_CH_2, 30U - 1U);

    TMRA_PWM_StructInit(&stcPwmCfg);
    TMRA_PWM_Config(M4_TMRA_1, TMRA_CH_2, &stcPwmCfg);

    TMRA_PWM_Cmd(M4_TMRA_1, TMRA_CH_2, Enable);

    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_11, \
                 GPIO_FUNC_4_TIMA1_PWM2, PIN_SUBFUNC_DISABLE);

    TMRA_Start(M4_TMRA_1);
}
#endif /* #if (APP_TMR2_CLK == APP_CLK_ASYNC_PIN_CLK) */

/**
 * @brief  Indicator configuration.
 * @param  None
 * @retval None
 */
static void IndicateConfig(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(INDICATE_PORT, INDICATE_PIN, &stcGpioInit);
    /* Output enable */
    GPIO_OE(INDICATE_PORT, INDICATE_PIN, Enable);
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
