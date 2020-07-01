/**
 *******************************************************************************
 * @file  timer2/timer2_capture/source/main.c
 * @brief Main program Timer2 capture for the Device Driver Library.
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
 * @addtogroup TMR2_Capture
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*
 * Functions of this example.
 * 'APP_FUNC_CAPTURE_EVENT': Measure the time that between two occurrences of a same event.
 * 'APP_FUNC_MEASURE_PULSE_WIDTH': Measure the pulse width of the square wave input from pin TIM2_<t>_PWMx(t=1 ~ 4, x=A, B).
 * 'APP_FUNC_MEASURE_PERIOD': Measure the period of the square wave input from pin TIM2_<t>_PWMx.
 */
#define APP_FUNC_CAPTURE_EVENT              (0U)
#define APP_FUNC_MEASURE_PULSE_WIDTH        (1U)
#define APP_FUNC_MEASURE_PERIOD             (2U)

/* Select the function of this example. */
#define APP_FUNC                            (APP_FUNC_CAPTURE_EVENT)

/*
 * Timer2 unit and channel definitions for this example.
 * 'APP_TMR2_UNIT' can be defined as M4_TMR2_<t>(t=1 ~ 4).
 * 'APP_TMR2_CH' can de defined as TMR2_CH_x(x=A, B).
 *
 * The following definitions are depend on the definitions of 'APP_TMR2_UNIT' and 'APP_TMR2_CH'.
 *   APP_TMR2_CAPT_PORT
 *   APP_TMR2_CAPT_PIN
 *   APP_TMR2_CAPT_PIN_FUNC
 *   APP_TMR2_INT_SRC_CMP
 *   APP_TMR2_INT_SRC_OVF
 */
#define APP_TMR2_UNIT                       (M4_TMR2_1)
#define APP_TMR2_CH                         (TMR2_CH_A)
#define APP_TMR2_PERIP_CLK                  (PWC_FCG2_TMR2_1)

/*
 * Define configuration values according to the function of this example just selected.
 * In this example:
 *     Set PLLHP(200MHz) as the system clock.
 *     Set PCLK1(100MHz) as the clock source of Timer2.
 *     Timer2 clock frequency is 100/2 = 50MHz, clock cycle is 1/50 = 0.02us.
 *
 *    The maximum frequency input from pin TIM2_<t>_PWMx is PCLK1/3(typical value). \
 *        100Hz will be used in this example.
 */
#define APP_TMR2_CLK_SRC                    (TMR2_CLK_SYNC_PCLK1)
#define APP_TMR2_CLK_DIV                    (TMR2_CLK_DIV1)
#define APP_TMR2_CLK_FREQ                   (100000000UL)

#if (APP_FUNC == APP_FUNC_CAPTURE_EVENT)
    #define APP_TMR2_START_COND             (TMR2_START_COND_EVENT)
    #define APP_TMR2_STOP_COND              (TMR2_STOP_COND_INVALID)
    #define APP_TMR2_CLR_COND               (TMR2_CLR_COND_INVALID)
    #define APP_TMR2_CAPT_COND              (TMR2_CAPT_COND_EVENT)
    #define APP_TMR2_CAPT_PERIP_EVENT       (EVT_PORT_EIRQ0)
    #define APP_CAPT_COUNT                  (2U)

    /* Key definitions. SW10 on the board. */
    #define KEY_PORT                        (GPIO_PORT_A)
    #define KEY_PIN                         (GPIO_PIN_00)
    #define KEY_EXINT_CH                    (EXINT_CH00)
    #define KEY_INT_SRC                     (INT_PORT_EIRQ0)
    #define KEY_IRQn                        (Int025_IRQn)
    #define KEY_INT_PRIO                    (DDL_IRQ_PRIORITY_04)

#elif (APP_FUNC == APP_FUNC_MEASURE_PULSE_WIDTH)
    #define APP_TMR2_START_COND             (TMR2_START_COND_TRIGR)
    #define APP_TMR2_STOP_COND              (TMR2_STOP_COND_TRIGF)
    #define APP_TMR2_CLR_COND               (TMR2_CLR_COND_TRIGF)
    #define APP_TMR2_CAPT_COND              (TMR2_CAPT_COND_TRIGF)
    #define APP_TMR2_CAPT_PORT              (GPIO_PORT_A)
    #define APP_TMR2_CAPT_PIN               (GPIO_PIN_02)
    #define APP_TMR2_CAPT_PIN_FUNC          (GPIO_FUNC_16_TIM21_PWMA)
    #define APP_CAPT_COUNT                  (11U)

#elif (APP_FUNC == APP_FUNC_MEASURE_PERIOD)
    #define APP_TMR2_START_COND             (TMR2_START_COND_TRIGR)
    #define APP_TMR2_STOP_COND              (TMR2_STOP_COND_INVALID)
    #define APP_TMR2_CLR_COND               (TMR2_CLR_COND_TRIGR)
    #define APP_TMR2_CAPT_COND              (TMR2_CAPT_COND_TRIGR)
    #define APP_TMR2_CAPT_PORT              (GPIO_PORT_A)
    #define APP_TMR2_CAPT_PIN               (GPIO_PIN_02)
    #define APP_TMR2_CAPT_PIN_FUNC          (GPIO_FUNC_16_TIM21_PWMA)
    #define APP_CAPT_COUNT                  (21U)
#else
    #error "Function is NOT supported!!!"
#endif

/*
 * Definitions about Timer2 interrupt for the example.
 * Timer2 independent IRQn: [Int000_IRQn, Int031_IRQn], [Int050_IRQn, Int055_IRQn].
 */
#define APP_TMR2_INT_TYPE_CMP               (TMR2_INT_CMP)
#define APP_TMR2_INT_PRIO_CMP               (DDL_IRQ_PRIORITY_05)
#define APP_TMR2_INT_SRC_CMP                (INT_TMR2_1_CMPA)
#define APP_TMR2_IRQn_CMP                   (Int053_IRQn)

#define APP_TMR2_INT_TYPE_OVF               (TMR2_INT_OVF)
#define APP_TMR2_INT_PRIO_OVF               (DDL_IRQ_PRIORITY_04)
#define APP_TMR2_INT_SRC_OVF                (INT_TMR2_1_OVFA)
#define APP_TMR2_IRQn_OVF                   (Int054_IRQn)

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

static void Tmr2Config(void);
static void Tmr2IrqConfig(void);
static void Tmr2CaptCondConfig(void);
static void Tmr2CaptCondStart(void);

static void TMR2_Ovf_IrqCallback(void);
static void TMR2_Cmp_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t m_u32CaptCnt = 0U;
static uint32_t m_u32OvfCnt  = 0U;
static uint32_t m_au32CaptTime[APP_CAPT_COUNT];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_capture project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t i;
    uint32_t u32Temp;
    float32_t f32Temp;

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* System clock is configured as 200MHz. */
    SystemClockConfig();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures Timer2. */
    Tmr2Config();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Start the peripheral to generate the condition for Timer2 capturing. */
    Tmr2CaptCondStart();

    /***************** Configuration end, application start **************/

    while (1U)
    {
        if (m_u32CaptCnt >= APP_CAPT_COUNT)
        {
#if (APP_FUNC == APP_FUNC_CAPTURE_EVENT)
            (void)i;
            TMR2_Stop(APP_TMR2_UNIT, APP_TMR2_CH);

            if (m_u32OvfCnt > 0U)
            {
                u32Temp = m_u32OvfCnt * 65536UL + m_au32CaptTime[1U] - m_au32CaptTime[0U];
            }
            else
            {
                if (m_au32CaptTime[0U] == 0U)
                {
                    u32Temp = m_au32CaptTime[1U] + 2U;
                }
                else
                {
                    if (m_au32CaptTime[1U] > m_au32CaptTime[0U])
                    {
                        u32Temp = m_au32CaptTime[1U] - m_au32CaptTime[0U];
                    }
                    else
                    {
                        DBG("Capture error!!!! m_u32OvfCnt should bigger than 0.");
                        while (1U);
                    }
                }
            }
            f32Temp = (float32_t)(u32Temp) / ((float)APP_TMR2_CLK_FREQ);
            DBG("Capture event completed: %ld overflow, %ld timer2 cycles, %f seconds.\n", m_u32OvfCnt, u32Temp, f32Temp);
            TMR2_SetCntVal(APP_TMR2_UNIT, APP_TMR2_CH, 0U);
            m_u32OvfCnt  = 0U;
            m_u32CaptCnt = 0U;

#elif (APP_FUNC == APP_FUNC_MEASURE_PULSE_WIDTH)
            u32Temp = 0U;
            /* The first capturing value is invalid. */
            for (i=1UL; i<APP_CAPT_COUNT; i++)
            {
                u32Temp += (m_au32CaptTime[i] + 2U);
            }
            f32Temp = (float32_t)u32Temp / ((float32_t)(APP_CAPT_COUNT-1U));
            DBG("Calculate pulse width completed: %.2f timer2 cycles, %f microseconds.\n", \
                 f32Temp, f32Temp/((float)(APP_TMR2_CLK_FREQ/1000000U)));
            m_u32CaptCnt = 0U;
            DDL_DelayMS(500U);

#elif (APP_FUNC == APP_FUNC_MEASURE_PERIOD)
            u32Temp = 0U;
            /* The first capturing value is invalid. */
            for (i=1UL; i<APP_CAPT_COUNT; i++)
            {
                u32Temp += (m_au32CaptTime[i] + 2U);
            }
            f32Temp = (float32_t)u32Temp / ((float32_t)(APP_CAPT_COUNT-1U));
            DBG("Calculate period completed: %.2f timer2 cycles, %f microseconds.\n", \
                 f32Temp, f32Temp/((float)(APP_TMR2_CLK_FREQ/1000000U)));
            m_u32CaptCnt = 0U;
            DDL_DelayMS(500U);

#endif
            TMR2_TrigCondCmd(APP_TMR2_UNIT, APP_TMR2_CH, \
                            (APP_TMR2_START_COND | APP_TMR2_CAPT_COND | APP_TMR2_CLR_COND), \
                             Enable);
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
 * @brief  Configures a new system clock. 200MHz.
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
    stcInit.u32FuncMode = TMR2_FUNC_CAPTURE;
    stcInit.u32ClkSrc   = APP_TMR2_CLK_SRC;
    stcInit.u32ClkDiv   = APP_TMR2_CLK_DIV;
    TMR2_Init(APP_TMR2_UNIT, APP_TMR2_CH, &stcInit);

    /* 4. Configures IRQ. */
    Tmr2IrqConfig();

    /* 5. Configures capture condition. */
    Tmr2CaptCondConfig();
}

/**
 * @brief  Timer2 interrupt configuration.
 * @param  None
 * @retval None
 */
static void Tmr2IrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = APP_TMR2_INT_SRC_CMP;
    stcCfg.enIRQn      = APP_TMR2_IRQn_CMP;
    stcCfg.pfnCallback = &TMR2_Cmp_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_TMR2_INT_PRIO_CMP);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Overflow interrupt. */
    stcCfg.enIntSrc    = APP_TMR2_INT_SRC_OVF;
    stcCfg.enIRQn      = APP_TMR2_IRQn_OVF;
    stcCfg.pfnCallback = &TMR2_Ovf_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_TMR2_INT_PRIO_OVF);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of Timer2. */
    TMR2_IntCmd(APP_TMR2_UNIT, APP_TMR2_CH, APP_TMR2_INT_TYPE_CMP | APP_TMR2_INT_TYPE_OVF, Enable);
}

/**
 * @brief  Specifies the hardware trigger conditions for capturing and \
 *         configures the peripheral which will generate the condition that to be captured by Timer2.
 * @param  None
 * @retval None
 */
static void Tmr2CaptCondConfig(void)
{
    stc_tmr2_trig_cond_t stcTrigCond;

#if (APP_FUNC == APP_FUNC_CAPTURE_EVENT)
    stc_exint_init_t stcExintInit;
    stc_gpio_init_t stcGpioInit;

    /* 1. Configure the peripheral. */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEY_PORT, KEY_PIN, &stcGpioInit);
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEY_EXINT_CH;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* 2. Enable AOS function. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    /* 3. Set the event for Timer2 capturing. */
    TMR2_SetTriggerSrc(APP_TMR2_CAPT_PERIP_EVENT);

#elif ((APP_FUNC == APP_FUNC_MEASURE_PULSE_WIDTH) || (APP_FUNC == APP_FUNC_MEASURE_PERIOD))
    /* Use TimerA to output PWM with a frequency of 1MHz and a duty cycle of 50%. */
    stc_tmra_init_t stcTmrAInit;
    stc_tmra_pwm_cfg_t stcPwmCfg;

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMRA_1, Enable);

    /* TimerA unit 1: clock source PCLK0(200MHZ). Divider: 2. Final count clock frequency 100MHz. */
    TMRA_StructInit(&stcTmrAInit);
    stcTmrAInit.u32ClkSrc    = TMRA_CLK_PCLK;
    stcTmrAInit.u32PCLKDiv   = TMRA_PCLK_DIV2;
    stcTmrAInit.u32PeriodVal = 100U - 1U;
    TMRA_Init(M4_TMRA_1, &stcTmrAInit);

    TMRA_SetFuncMode(M4_TMRA_1, TMRA_CH_2, TMRA_FUNC_COMPARE);
    TMRA_SetCmpVal(M4_TMRA_1, TMRA_CH_2, 50U - 1U);

    TMRA_PWM_StructInit(&stcPwmCfg);
    TMRA_PWM_Config(M4_TMRA_1, TMRA_CH_2, &stcPwmCfg);

    TMRA_PWM_Cmd(M4_TMRA_1, TMRA_CH_2, Enable);

    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_11, \
                 GPIO_FUNC_4_TIMA1_PWM2, PIN_SUBFUNC_DISABLE);

    /* Configure the wave input pin. */
    GPIO_SetFunc(APP_TMR2_CAPT_PORT, APP_TMR2_CAPT_PIN, APP_TMR2_CAPT_PIN_FUNC, PIN_SUBFUNC_DISABLE);
#endif

    TMR2_TrigCondStructInit(&stcTrigCond);
    stcTrigCond.u32StartCond = APP_TMR2_START_COND;
    stcTrigCond.u32StopCond  = APP_TMR2_STOP_COND;
    stcTrigCond.u32ClrCond   = APP_TMR2_CLR_COND;
    stcTrigCond.u32CaptCond  = APP_TMR2_CAPT_COND;
    TMR2_SetTrigCond(APP_TMR2_UNIT, APP_TMR2_CH, &stcTrigCond);
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
        if (m_u32CaptCnt < APP_CAPT_COUNT)
        {
            m_au32CaptTime[m_u32CaptCnt] = TMR2_GetCmpVal(APP_TMR2_UNIT, APP_TMR2_CH);
            m_u32CaptCnt++;
            if (m_u32CaptCnt >= APP_CAPT_COUNT)
            {
                /* Disable the conditions for calculating the result, if needed. */
                TMR2_TrigCondCmd(APP_TMR2_UNIT, APP_TMR2_CH, \
                                (APP_TMR2_START_COND | APP_TMR2_CAPT_COND | APP_TMR2_CLR_COND), \
                                 Disable);
            }
        }
    }
}

/**
 * @brief  Timer2 counter overflow interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR2_Ovf_IrqCallback(void)
{
    if (TMR2_GetStatus(APP_TMR2_UNIT, APP_TMR2_CH, TMR2_FLAG_OVF) == Set)
    {
        TMR2_ClrStatus(APP_TMR2_UNIT, APP_TMR2_CH, TMR2_FLAG_OVF);
        if (m_u32CaptCnt < APP_CAPT_COUNT)
        {
            m_u32OvfCnt++;
        }
    }
}

/**
 * @brief  Start the peripheral that was configured to generate the condition that to be captured by Timer2.
 * @param  None
 * @retval None
 */
static void Tmr2CaptCondStart(void)
{
#if (APP_FUNC == APP_FUNC_CAPTURE_EVENT)
    /* For this example: press key SW10 to generate the event for TIEMR2 capturing. */
#else
    /* Make falling/rising edge on pin TIM2_<t>_PWMx. */
    TMRA_Start(M4_TMRA_1);
#endif
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
