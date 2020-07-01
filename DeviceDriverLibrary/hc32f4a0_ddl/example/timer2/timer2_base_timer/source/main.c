/**
 *******************************************************************************
 * @file  timer2/timer2_base_timer/source/main.c
 * @brief Main program Timer2 base timer for the Device Driver Library.
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
 * @addtogroup TMR2_Base_Timer
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
 * This example is going to use kinds of different clock sources to count 10 milliseconds.
 */

/*
 * Timer2 unit and channel definitions for this example.
 * 'APP_TMR2_UNIT' can be defined as M4_TMR2_<t>(t=1 ~ 4).
 * 'APP_TMR2_CH' can be defined as TMR2_CH_x(x=A, B).
 *
 * NOTE!!! The following definitions are depend on the definitions of 'APP_TMR2_UNIT' and 'APP_TMR2_CH'.
 *   APP_TMR2_ASYNC_CLK_PORT
 *   APP_TMR2_ASYNC_CLK_PIN
 *   APP_TMR2_ASYNC_CLK_PIN_FUNC
 *   APP_TMR2_TRIG_PORT
 *   APP_TMR2_TRIG_PIN
 *   APP_TMR2_TRIG_PIN_FUNC
 *   APP_TMR2_INT_SRC
 */
#define APP_TMR2_UNIT                       (M4_TMR2_1)
#define APP_TMR2_CH                         (TMR2_CH_A)
#define APP_TMR2_PERIP_CLK                  (PWC_FCG2_TMR2_1)

/*
 * Function control of Timer2.
 * Defines the following macro as non-zero to enable the corresponding function.
 *
 * 'APP_TMR2_USE_INTERRUPT': Interrupt function control.
 * 'APP_TMR2_USE_HW_TRIG': Hardware trigger conditions control. The conditions that can start Timer2, \
 *                         stop Timer2 or clear counting register of Timer2
 */
#define APP_TMR2_USE_INTERRUPT              (1U)
#define APP_TMR2_USE_HW_TRIG                (0U)

/*
 * Clock source for this example.
 * In this example:
 *   1. System clock is configured as 240MHz.
 *   2. PCLK1 is 120MHz.
 *   3. Use timer2 to count 10ms.
 *
 * A simple formula for calculating the compare value is:
 *   Tmr2CompareVal = (Tmr2Period(us) * [Tmr2ClockSource(MHz) / Tmr2ClockDiv]) - 1.
 */
#define APP_TMR2_CLK                        (TMR2_CLK_SYNC_PCLK1)
#define APP_TMR2_CLK_SRC                    (TMR2_CLK_SYNC_PCLK1)
#define APP_TMR2_CLK_DIV                    (TMR2_CLK_DIV64)
#define APP_TMR2_CMP_VAL                    (18750UL - 1U)

/*
 * Definitions about Timer2 interrupt for the example.
 * Timer2 independent IRQn: [Int000_IRQn, Int031_IRQn], [Int050_IRQn, Int055_IRQn].
 *
 * NOTE:
 *   'APP_TMR2_INT_TYPE' can only be defined as 'TMR2_INT_CMP' for this example.
 */
#if (APP_TMR2_USE_INTERRUPT > 0U)
    #define APP_TMR2_INT_TYPE               (TMR2_INT_CMP)
    #define APP_TMR2_INT_PRIO               (DDL_IRQ_PRIORITY_03)
    #define APP_TMR2_INT_SRC                (INT_TMR2_1_CMPA)
    #define APP_TMR2_IRQn                   (Int050_IRQn)
#endif /* #if (APP_TMR2_USE_INTERRUPT > 0U) */

/*
 * Specify the hardware trigger conditions if enabled(APP_TMR2_USE_HW_TRIG > 0U).
 * 'APP_TMR2_START_COND' specifies the condition of starting Timer2.
 * 'APP_TMR2_STOP_COND' specifies the condition of stoping Timer2.
 * 'APP_TMR2_FILTER_ENABLE': Filter function control. Enable or disable the filter of pin TIM2_<t>_PWMAx. \
 *                           If there is a pin TIM2_<t>_PWMx is used and in input mode, the filter can be used if needed.
 * 'APP_TMR2_FILTER_CLK_DIV': The clock divider of filter of each channel depends on the signal that \
 *                            input from pin TIM2_<t>_PWMx.
 * NOTE:
 *   1. CANNOT specify a condition as both the start condition and the stop condition.
 *   2. Pin TIM2_<t>_PWMAx CANNOT be a trigger condition while it's edge is used as the synchronous clock.
 */
#if (APP_TMR2_USE_HW_TRIG > 0U)
    #define APP_TMR2_START_COND             (TMR2_START_COND_TRIGR)
    #define APP_TMR2_STOP_COND              (TMR2_STOP_COND_TRIGF)
    #define APP_TMR2_TRIG_PORT              (GPIO_PORT_A)
    #define APP_TMR2_TRIG_PIN               (GPIO_PIN_02)
    #define APP_TMR2_TRIG_PIN_FUNC          (GPIO_FUNC_16_TIM21_PWMA)
    #define APP_TMR2_FILTER_ENABLE          (1U)
    #define APP_TMR2_FILTER_CLK_DIV         (TMR2_FILTER_CLK_DIV64)
#endif /* #if (APP_TMR2_USE_HW_TRIG > 0U) */

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

#if (APP_TMR2_USE_INTERRUPT > 0U)
    static void Tmr2IrqConfig(void);
    static void TMR2_Cmp_IrqCallback(void);
#endif

#if (APP_TMR2_USE_HW_TRIG > 0U)
    static void Tmr2TrigCondConfig(void);
#endif

static void Tmr2Start(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_base_timer project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the system clock. */
    SystemClockConfig();
    /* Configures indicator. */
    IndicateConfig();
    /* Configures Timer2. */
    Tmr2Config();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts Timer2. */
    Tmr2Start();

    /***************** Configuration end, application start **************/
    while (1U)
    {
#if (APP_TMR2_USE_INTERRUPT > 0U)
        /* See TMR2_Cmp_IrqCallback in this file. */
#else
        /* Call TMR2_GetStatus to check the flag state. */
        if (TMR2_GetStatus(APP_TMR2_UNIT, APP_TMR2_CH, TMR2_FLAG_CMP) == Set)
        {
            TMR2_ClrStatus(APP_TMR2_UNIT, APP_TMR2_CH, TMR2_FLAG_CMP);
            INDICATE_OUT_TOGGLE();
        }
#endif /* #if (APP_TMR2_USE_INTERRUPT > 0U) */
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

    /* 1. Enable Timer2 peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMR2_PERIP_CLK, Enable);

    /* 2. Set a default initialization value for stcInit. */
    TMR2_StructInit(&stcInit);

    /* 3. Modifies the initialization values depends on the application. */
    stcInit.u32ClkSrc = APP_TMR2_CLK_SRC;
    stcInit.u32ClkDiv = APP_TMR2_CLK_DIV;
    stcInit.u32CmpVal = APP_TMR2_CMP_VAL;
    TMR2_Init(APP_TMR2_UNIT, APP_TMR2_CH, &stcInit);

#if (APP_TMR2_USE_INTERRUPT > 0U)
    /* 4. Configures IRQ if needed. */
    Tmr2IrqConfig();
#endif /* #if (APP_TMR2_USE_INTERRUPT > 0U) */

#if (APP_TMR2_USE_HW_TRIG > 0U)
    /* 5. Configures hardware trigger condition if needed. */
    Tmr2TrigCondConfig();
#endif /* #if (APP_TMR2_USE_HW_TRIG > 0U) */
}

#if (APP_TMR2_USE_INTERRUPT > 0U)
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
#endif /* #if (APP_TMR2_USE_INTERRUPT > 0U) */

#if (APP_TMR2_USE_HW_TRIG > 0U)
/**
 * @brief
 * @param  [in]
 * @param  [out]
 * @retval None
 */
static void Tmr2TrigCondConfig(void)
{
    stc_tmr2_trig_cond_t stcTrigCond;

    /*
     * If a peripheral is used to generate the event which is used as a hardware trigger condition of Timer2, \
     *     call the API of the peripheral to configure the peripheral.
     * The following operations are only used in this example.
     */

#if (defined APP_TMR2_FILTER_ENABLE && APP_TMR2_FILTER_ENABLE > 0U)
    TMR2_FilterConfig(APP_TMR2_UNIT, APP_TMR2_CH, APP_TMR2_FILTER_CLK_DIV);
    TMR2_FilterCmd(APP_TMR2_UNIT, APP_TMR2_CH, Enable);
#endif
    GPIO_SetFunc(APP_TMR2_TRIG_PORT, APP_TMR2_TRIG_PIN, APP_TMR2_TRIG_PIN_FUNC, PIN_SUBFUNC_DISABLE);

    TMR2_TrigCondStructInit(&stcTrigCond);
    stcTrigCond.u32StartCond = APP_TMR2_START_COND;
    stcTrigCond.u32StopCond  = APP_TMR2_STOP_COND;
    TMR2_SetTrigCond(APP_TMR2_UNIT, APP_TMR2_CH, &stcTrigCond);
}
#endif

/**
 * @brief  Start Timer2.
 * @param  None
 * @retval None
 */
static void Tmr2Start(void)
{
    /*
     * If a peripheral is used to generate the event which is used as a hardware trigger condition of Timer2, \
     *     call the API of the peripheral to start the peripheral here or anywhere else you need.
     * The following operations are only used in this example.
     */

#if ((APP_TMR2_USE_HW_TRIG == 0U) || \
     ((APP_TMR2_USE_HW_TRIG > 0U) && (APP_TMR2_START_COND == TMR2_START_COND_INVALID)))
    TMR2_Start(APP_TMR2_UNIT, APP_TMR2_CH);
#else
    /* Make an rising edge on pin TIM2_<t>_PWMx to start Timer2. */
#endif
}

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
