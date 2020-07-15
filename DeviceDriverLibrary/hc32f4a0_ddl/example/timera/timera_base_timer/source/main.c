/**
 *******************************************************************************
 * @file  timera/timera_base_timer/source/main.c
 * @brief Main program TimerA base timer for the Device Driver Library.
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
 * @addtogroup TMRA_Base_Timer
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
 * This example is going to use TimerA to count 1 milliseconds.
 */

/*
 * TimerA unit and channel definitions for this example.
 * 'APP_TMRA_UNIT' can be defined as M4_TMRA_<t>(t=1 ~ 12).
 * 'APP_TMRA_CH' can be defined as TMRA_CH_x(x=1 ~ 4).
 *
 * NOTE!!! The following definitions are depend on the definitions of 'APP_TMRA_UNIT' and 'APP_TMRA_CH'.
 *   APP_TMRA_TRIG_PORT
 *   APP_TMRA_TRIG_PIN
 *   APP_TMRA_TRIG_PIN_FUNC
 *   APP_TMRA_INT_SRC
 *   APP_TMRA_IRQn
 */
#define APP_TMRA_UNIT                       (M4_TMRA_1)
#define APP_TMRA_CH                         (TMRA_CH_1)
#define APP_TMRA_PERIP_CLK                  (PWC_FCG2_TMRA_1)

/*
 * Function control of TimerA.
 * Defines the following macro as non-zero to enable the corresponding function.
 *
 * 'APP_TMRA_USE_INTERRUPT': Interrupt function control.
 * 'APP_TMRA_USE_HW_TRIG': Hardware trigger conditions control. The conditions that can start TimerA, \
 *                         stop TimerA or clear counting register of TimerA
 */
#define APP_TMRA_USE_INTERRUPT              (1U)
#define APP_TMRA_USE_HW_TRIG                (0U)

/*
 * Clock source of TimerA in this example.
 * The only valid clock source of TimerA in this example is PCLK(PCLK0 for unit1 ~ uint4. PCLK1 for unit5 ~ uint12).
 */
#define APP_TMRA_CLK                        (TMRA_CLK_PCLK)

/* The divider of the clock source. @ref TMRA_PCLK_Divider */
#define APP_TMRA_PCLK_DIV                   (TMRA_PCLK_DIV8)

/* The counting mode of TimerA. @ref TMRA_Count_Mode */
#define APP_TMRA_MODE                       (TMRA_MODE_SAWTOOTH)

/* The counting direction of TimerA. @ref TMRA_Count_Direction */
#define APP_TMRA_DIR                        (TMRA_DIR_UP)

/*
 * In this example:
 *   Timing period is 1ms.
 * A simple formula for calculating the compare value is:
 *   Sawtooth mode:
 *     TmrAPeriodVal = (TmrAPeriod(us) * [TmrAClockSource(MHz) / TmrAClockPrescaler]) - 1.
 *   Triangle mode:
 *     TmrAPeriodVal = (TmrAPeriod(us) * [TmrAClockSource(MHz) / TmrAClockPrescaler]) / 2.
 */
#if (APP_TMRA_MODE == TMRA_MODE_SAWTOOTH)
    #define APP_TMRA_PERIOD_VAL         (25000UL - 1UL)
    #define APP_TMRA_CMP_VAL            (APP_TMRA_PERIOD_VAL)
#else /* (APP_TMRA_MODE == TMRA_MODE_TRIANGLE) */
    #define APP_TMRA_PERIOD_VAL         (25000UL / 2UL)
    #define APP_TMRA_CMP_VAL            (APP_TMRA_PERIOD_VAL)
#endif /* #if (APP_TMRA_DIR == TMRA_MODE_SAWTOOTH) */

/*
 * Definitions about TimerA interrupt for the example.
 * IRQn of TimerA:
 *   M4_TMRA_x(x=1, 2): [Int000_IRQn, Int031_IRQn], [Int074_IRQn, Int079_IRQn];
 *   M4_TMRA_x(x=3, 4): [Int000_IRQn, Int031_IRQn], [Int080_IRQn, Int085_IRQn];
 *   M4_TMRA_x(x=5 ~ 8): [Int000_IRQn, Int031_IRQn], [Int092_IRQn, Int097_IRQn];
 *   M4_TMRA_x(x=9 ~ 12): [Int000_IRQn, Int031_IRQn], [Int098_IRQn, Int103_IRQn];
 *
 * 'APP_TMRA_INT_TYPE' can be defined as 'TMRA_INT_CMP_CHx'(x=1 ~ 4, depends on 'APP_TMRA_CH') \
 *   or 'TMRA_INT_OVF' in this example.
 */
#if (APP_TMRA_USE_INTERRUPT > 0U)
    #define APP_TMRA_INT_TYPE               (TMRA_INT_CMP_CH1)
    #define APP_TMRA_INT_PRIO               (DDL_IRQ_PRIORITY_03)
    #define APP_TMRA_INT_SRC                (INT_TMRA_1_CMP)
    #define APP_TMRA_IRQn                   (Int074_IRQn)
#endif /* #if (APP_TMRA_USE_INTERRUPT > 0U) */

/* The status flag of TimerA in this example, depends on 'APP_TMRA_CH'. */
#define APP_TMRA_FLAG                       (TMRA_FLAG_CMP_CH1)

/*
 * Specify the hardware trigger conditions if enabled(APP_TMRA_USE_HW_TRIG > 0U).
 * 'APP_TMRA_START_COND' specifies the condition of starting TimerA.
 * 'APP_TMRA_STOP_COND' specifies the condition of stoping TimerA.
 * 'APP_TMRA_USE_FILTER': Filter function control. Enable or disable the filter of pin TIMA_<t>_TRIG. \
 *                        If there is a pin TIMA_<t>_TRIG is used, the filter can be used if needed.
 *                        In this example, the only pin with filter function used is TIMA_<t>_TRIG.
 * 'APP_TMRA_FILTER_CLK_DIV': The clock divider of filter of each channel depends on the signal that \
 *                            input from pin TIMA_<t>_TRIG.
 * NOTE:
 *   CANNOT specify a condition as both start condition and stop condition.
 */
#if (APP_TMRA_USE_HW_TRIG > 0U)
    #define APP_TMRA_START_COND             (TMRA_START_COND_TRIGR)
    #define APP_TMRA_STOP_COND              (TMRA_STOP_COND_TRIGF)
    #define APP_TMRA_TRIG_PORT              (GPIO_PORT_C)
    #define APP_TMRA_TRIG_PIN               (GPIO_PIN_00)
    #define APP_TMRA_TRIG_PIN_FUNC          (GPIO_FUNC_6_TIMA1_TRIG)
    #define APP_TMRA_FILTER_ENABLE          (1U)
    #define APP_TMRA_FILTER_CLK_DIV         (TMRA_FILTER_CLK_DIV16)
    #define APP_TMRA_FILTER_PIN             (TMRA_PIN_TRIG)
#endif /* #if (APP_TMRA_USE_HW_TRIG > 0U) */

/* Indicate pin definition in this example. */
#define INDICATE_PORT                       (GPIO_PORT_A)
#define INDICATE_PIN                        (GPIO_PIN_02)
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

static void TmrAConfig(void);
#if (APP_TMRA_USE_INTERRUPT > 0U)
    static void TmrAIrqConfig(void);
    static void TMRA_Cmp_IrqCallback(void);
#endif
#if (APP_TMRA_USE_HW_TRIG > 0U)
    static void TmrATrigCondConfig(void);
#endif
static void TmrAStart(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timera_base_timer project
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
    /* Configures TimerA. */
    TmrAConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts TimerA. */
    TmrAStart();

    /***************** Configuration end, application start **************/
    while (1U)
    {
#if (APP_TMRA_USE_INTERRUPT > 0U)
        /* See TMRA_Cmp_IrqCallback in this file. */
#else
        /* Call TMRA_GetStatus to check the flag state. */
        if (TMRA_GetStatus(APP_TMRA_UNIT, APP_TMRA_FLAG) == Set)
        {
            TMRA_ClrStatus(APP_TMRA_UNIT, APP_TMRA_FLAG);
            INDICATE_OUT_TOGGLE();
        }
#endif /* #if (APP_TMRA_USE_INTERRUPT > 0U) */
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
    PWC_Unlock(PWC_UNLOCK_CODE_0);
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
    PWC_Lock(PWC_UNLOCK_CODE_0);
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
 * @brief  Configures a new system clock(200MHz).
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
    PWC_Lock(PWC_UNLOCK_CODE_0);
}

/**
 * @brief  TimerA configuration.
 * @param  None
 * @retval None
 */
static void TmrAConfig(void)
{
    stc_tmra_init_t stcInit;

    /* 1. Enable TimerA peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMRA_PERIP_CLK, Enable);

    /* 2. Set a default initialization value for stcInit. */
    TMRA_StructInit(&stcInit);

    /* 3. Modifies the initialization values depends on the application. */
    stcInit.u32ClkSrc    = APP_TMRA_CLK;
    stcInit.u32PCLKDiv   = APP_TMRA_PCLK_DIV;
    stcInit.u32CntDir    = APP_TMRA_DIR;
    stcInit.u32CntMode   = APP_TMRA_MODE;
    stcInit.u32PeriodVal = APP_TMRA_PERIOD_VAL;
    TMRA_Init(APP_TMRA_UNIT, &stcInit);

    /* 4. Set the comparison reference value. */
    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_CH, APP_TMRA_CMP_VAL);

#if (APP_TMRA_USE_INTERRUPT > 0U)
    /* 5. Configures IRQ if needed. */
    TmrAIrqConfig();
#endif /* #if (APP_TMRA_USE_INTERRUPT > 0U) */

#if (APP_TMRA_USE_HW_TRIG > 0U)
    /* 6. Configures hardware trigger condition if needed. */
    TmrATrigCondConfig();
#endif /* #if (APP_TMRA_USE_HW_TRIG > 0U) */
}

#if (APP_TMRA_USE_INTERRUPT > 0U)
/**
 * @brief  TimerA interrupt configuration.
 * @param  None
 * @retval None
 */
static void TmrAIrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = APP_TMRA_INT_SRC;
    stcCfg.enIRQn      = APP_TMRA_IRQn;
    stcCfg.pfnCallback = &TMRA_Cmp_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_TMRA_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of TimerA. */
    TMRA_IntCmd(APP_TMRA_UNIT, APP_TMRA_INT_TYPE, Enable);
}

/**
 * @brief  TimerA counter compare match interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMRA_Cmp_IrqCallback(void)
{
    if (TMRA_GetStatus(APP_TMRA_UNIT, APP_TMRA_FLAG) == Set)
    {
        TMRA_ClrStatus(APP_TMRA_UNIT, APP_TMRA_FLAG);
        INDICATE_OUT_TOGGLE();
    }
}
#endif /* #if (APP_TMRA_USE_INTERRUPT > 0U) */

#if (APP_TMRA_USE_HW_TRIG > 0U)
/**
 * @brief  Trigger condition configuration.
 * @param  None
 * @retval None
 */
static void TmrATrigCondConfig(void)
{
    stc_tmra_trig_cond_t stcTrigCond;

    /*
     * If a peripheral is used to generate the event which is used as a hardware trigger condition of TimerA, \
     *   call the API of the peripheral to configure it.
     * The following operations are only used in this example.
     */

#if (defined APP_TMRA_FILTER_ENABLE &&  APP_TMRA_FILTER_ENABLE > 0U)
    TMRA_FilterConfig(APP_TMRA_UNIT, APP_TMRA_FILTER_PIN, APP_TMRA_FILTER_CLK_DIV);
    TMRA_FilterCmd(APP_TMRA_UNIT, APP_TMRA_FILTER_PIN, Enable);
#endif
    GPIO_SetFunc(APP_TMRA_TRIG_PORT, APP_TMRA_TRIG_PIN, APP_TMRA_TRIG_PIN_FUNC, PIN_SUBFUNC_DISABLE);

    TMRA_TrigCondStructInit(&stcTrigCond);
    stcTrigCond.u32StartCond = APP_TMRA_START_COND;
    stcTrigCond.u32StopCond  = APP_TMRA_STOP_COND;
    TMRA_SetTrigCond(APP_TMRA_UNIT, &stcTrigCond);
}
#endif

/**
 * @brief  Start TimerA.
 * @param  None
 * @retval None
 */
static void TmrAStart(void)
{
    /*
     * If a peripheral is used to generate the event which is used as a hardware trigger condition of TimerA, \
     *   call the API of the peripheral to start the peripheral here or anywhere else you need.
     * The following operations are only used in this example.
     */

#if ((APP_TMRA_USE_HW_TRIG == 0U) || \
     ((APP_TMRA_USE_HW_TRIG > 0U) && (APP_TMRA_START_COND == TMRA_START_COND_INVALID)))
    TMRA_Start(APP_TMRA_UNIT);
#else
    /* Make an rising edge on pin TIMA_<t>_TRIG to start TimerA. */
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
