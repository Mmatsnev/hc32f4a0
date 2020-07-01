/**
 *******************************************************************************
 * @file  timer2/timer2_base_counter/source/main.c
 * @brief Main program Timer2 base counter for the Device Driver Library.
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
 * @addtogroup TMR2_Base_Counter
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
 * 'APP_FUNC_CNT_PERIP_EVENT': Use Timer2 to count the number of times that the specified event has occurred.
 * 'APP_FUNC_CNT_TIMER6_OVF': Use Timer2 to count the number of times of TIMER6 overflow(or underflow).
 * 'APP_FUNC_CNT_PIN_EDGE': Use Timer2 to count the number of rising edge(or falling edge) that occurred on pin TIM2_<t>_PWMx(t=1 ~ 4, x=A, B).
 */
#define APP_FUNC_CNT_PERIP_EVENT            (0U)
#define APP_FUNC_CNT_TIMER6_OVF             (1U)
#define APP_FUNC_CNT_PIN_EDGE               (2U)

/* Select a function of this example. */
#define APP_TMR2_FUNC                       (APP_FUNC_CNT_PERIP_EVENT)

/*
 * Timer2 unit and channel definitions for this example.
 * 'APP_TMR2_UNIT' can be defined as M4_TMR2_<t>(t=1 ~ 4).
 * 'APP_TMR2_CH' can be defined as TMR2_CH_x(x=A, B).
 *
 * NOTE!!! The following definitions are depend on the definitions of 'APP_TMR2_UNIT' and 'APP_TMR2_CH'.
 *   APP_TMR6_UNIT
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
 *                         stop Timer2 or clear counting register of Timer2.
 *                         Define it as zero when APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT in this example.
 */
#define APP_TMR2_USE_INTERRUPT              (0U)
#define APP_TMR2_USE_HW_TRIG                (0U)

/*
 * Specifies count times for this example.
 * The compare match flag is set when counting register reached the specified value, the interrupt occurred if enabled.
 */
#define APP_TMR2_COUNT_TIMES                (10U)

/*
 * Define configuration values according to the function of this example just selected.
 * In this example:
 *   1. PCLK1 is set to 120MHz.
 *   2. The maximum frequency input from pin TRIGm(m=A, B) is PCLK1/3(typical value). \
 *      100Hz will be used in this example.
 *
 * NOTE:
 *   1. Clock divier is NOT valid when the clock source of Timer2 is specified as TMR2_CLK_SYNC_EVENT,
 *      TMR2_CLK_SYNC_TIMER6_OVF, TMR2_CLK_SYNC_TIMER6_UDF, TMR2_CLK_SYNC_TRIGR or TMR2_CLK_SYNC_TRIGF.
 */
#if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT)
    #define APP_TMR2_CLK_SRC                (TMR2_CLK_SYNC_EVENT)
    #define APP_TMR2_CMP_VAL                (APP_TMR2_COUNT_TIMES - 1U)
    #define APP_TMR2_CNT_EVENT              (EVT_PORT_EIRQ0)

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF)
    /* APP_TMR2_CLK_SRC can also be defined as TMR2_CLK_SYNC_TIMER6_UDF */
    #define APP_TMR2_CLK_SRC                (TMR2_CLK_SYNC_TIMER6_OVF)
    #define APP_TMR2_CMP_VAL                (APP_TMR2_COUNT_TIMES - 1U)
    #define APP_TMR6_UNIT                   (M4_TMR6_1)

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE)
    /*
     * APP_TMR2_CLK_SRC can also be defined as TMR2_CLK_SYNC_TRIGF.
     * 'APP_TMR2_FILTER_ENABLE': Filter function control. Enable or disable the filter of pin TIM2_<t>_PWMAx. \
     *                           If there is a pin TIM2_<t>_PWMx is used and in input mode, the filter can be used if needed.
     * 'APP_TMR2_FILTER_CLK_DIV': The clock divider of filter of each channel depends on the signal that \
     *                            input from pin TIM2_<t>_PWMx.
     */
    #define APP_TMR2_CLK_SRC                (TMR2_CLK_SYNC_TRIGR)
    #define APP_TMR2_CMP_VAL                (APP_TMR2_COUNT_TIMES - 1U)
    #define APP_TMR2_CNT_PORT               (GPIO_PORT_E)
    #define APP_TMR2_CNT_PIN                (GPIO_PIN_05)
    #define APP_TMR2_CNT_PIN_FUNC           (GPIO_FUNC_16_TIM21_PWMA)
    #define APP_TMR2_FILTER_ENABLE          (1U)
    #define APP_TMR2_FILTER_CLK_DIV         (TMR2_FILTER_CLK_DIV16)

#else
    #error "The function you selected is NOT supported in this example!"
#endif /* #if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT) */

/*
 * Definitions about Timer2 interrupt for the example.
 * Timer2 independent IRQn: [Int000_IRQn, Int031_IRQn], [Int050_IRQn, Int055_IRQn].
 *
 * NOTE!!! 'APP_TMR2_INT_TYPE' can only be defined as 'TMR2_INT_CMP' for this example.
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
 *   1. CANNOT specify a condition as the start condition and the stop condition at the same time.
 *   2. Pin TIM2_<t>_PWMAx CANNOT be a trigger condition while it's edge is used as the synchronous clock.
 */
#if (APP_TMR2_USE_HW_TRIG > 0U)
    #if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT)
    /*
     * Neither start condition nor stop condition can be defined as a peripheral event.
     * Start condition and stop condition can only be defined as the edge of pin TIM2_<t>_PWMAx.
     */
        #define APP_TMR2_START_COND         (TMR2_START_COND_TRIGR)
        #define APP_TMR2_STOP_COND          (TMR2_STOP_COND_TRIGF)
        #define APP_TMR2_TRIG_PORT          (GPIO_PORT_A)
        #define APP_TMR2_TRIG_PIN           (GPIO_PIN_02)
        #define APP_TMR2_TRIG_PIN_FUNC      (GPIO_FUNC_16_TIM21_PWMA)
        #define APP_TMR2_FILTER_ENABLE      (1U)
        #define APP_TMR2_FILTER_CLK_DIV     (TMR2_FILTER_CLK_DIV64)

    #elif (APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF)
        #define APP_TMR2_START_COND         (TMR2_START_COND_EVENT)
        #define APP_TMR2_STOP_COND          (TMR2_STOP_COND_TRIGF)
        #define APP_TMR2_TRIG_PORT          (GPIO_PORT_A)
        #define APP_TMR2_TRIG_PIN           (GPIO_PIN_02)
        #define APP_TMR2_TRIG_PIN_FUNC      (GPIO_FUNC_16_TIM21_PWMA)
        #define APP_TMR2_TRIG_EVENT         (EVT_PORT_EIRQ0)
        #define APP_TMR2_FILTER_ENABLE      (1U)
        #define APP_TMR2_FILTER_CLK_DIV     (TMR2_FILTER_CLK_DIV64)

    #else /* (APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE) */
    /*
     * Start condition and stop condition, one of which can only be defined as a peripheral event.
     * The edge of pin TIM2_<t>_PWMAx CANNOT be used as start condition and stop condition .
     */
        #define APP_TMR2_START_COND         (TMR2_START_COND_EVENT)
        #define APP_TMR2_STOP_COND          (TMR2_STOP_COND_INVALID)
        #define APP_TMR2_TRIG_EVENT         (EVT_PORT_EIRQ0)
    #endif
#endif

/* Key definition. SW10 on the board. */
#define KEY_PORT                            (GPIO_PORT_A)
#define KEY_PIN                             (GPIO_PIN_00)
#define KEY_EXINT_CH                        (EXINT_CH00)
#define KEY_INT_SRC                         (INT_PORT_EIRQ0)
#define KEY_IRQn                            (Int025_IRQn)
#define KEY_INT_PRIO                        (DDL_IRQ_PRIORITY_04)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void Tmr2CntPeripConfig(void);
static void Tmr2CntPeripStart(void);

static void Tmr2Config(void);
#if (APP_TMR2_USE_INTERRUPT > 0U)
    static void Tmr2IrqConfig(void);
    static void TMR2_Cmp_IrqCallback(void);
#endif
#if (APP_TMR2_USE_HW_TRIG > 0U)
    static void Tmr2TrigCondConfig(void);
#endif

static void Tmr2Start(void);

/*
 * The event generated by the key has the following functions in this example:
 *   1. The counting condition of Timer2 when APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT.
 *   2. The start condition of Timer2 when APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF or \
 *      APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE.
 */
void EventKeyConfig(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_base_counter project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The default system clock is MRC(8MHz). */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the peripheral for Timer2 counting. */
    Tmr2CntPeripConfig();
    /* Configures Timer2. */
    Tmr2Config();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts Timer2. */
    Tmr2Start();
    /* Starts the peripheral for counting. */
    Tmr2CntPeripStart();

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
    // PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
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
    // PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
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
 * @brief  The event generated by the key has the following functions in this example:
 *           1. The count condition of Timer2 when APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT.
 *           2. The start condition of Timer2 when APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF or \
 *              APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE.
 * @param  None
 * @retval None
 */
void EventKeyConfig(void)
{
    stc_exint_init_t stcExintInit;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEY_PORT, KEY_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEY_EXINT_CH;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);
}

/**
 * @brief  Configure the peripheral which is used to generate the event that for Timer2 couting.
 * @param  None
 * @retval None
 */
static void Tmr2CntPeripConfig(void)
{
#if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT)
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    TMR2_SetTriggerSrc(APP_TMR2_CNT_EVENT);
    EventKeyConfig();

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF)
    stc_tmr6_basecnt_cfg_t stcTIM6BaseCntCfg;

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);

    TMR6_BaseCntStructInit(&stcTIM6BaseCntCfg);
    TMR6_CountCmd(M4_TMR6_1, Disable);
    /* Timer6 general count function configuration */
    stcTIM6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV8;
    TMR6_Init(M4_TMR6_1, &stcTIM6BaseCntCfg);
    /* Period register set. Clock of TIMER6 is PCLK0/8, 1MHz. */
    TMR6_SetPeriodReg(M4_TMR6_1, TMR6_PERIOD_REG_A, 10000UL - 1UL);
    /* Set General Compare RegisterA Value */
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, 10000UL - 1UL);

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE)
    /* If filter function of pin TIM2_<t>_PWMx is enabled. */
    #if (defined APP_TMR2_FILTER_ENABLE && APP_TMR2_FILTER_ENABLE > 0U)
        TMR2_FilterConfig(APP_TMR2_UNIT, APP_TMR2_CH, APP_TMR2_FILTER_CLK_DIV);
        TMR2_FilterCmd(APP_TMR2_UNIT, APP_TMR2_CH, Enable);
    #endif
    GPIO_SetFunc(APP_TMR2_CNT_PORT, APP_TMR2_CNT_PIN, \
                 APP_TMR2_CNT_PIN_FUNC, PIN_SUBFUNC_DISABLE);
#endif
}

/**
 * @brief  Start the peripheral which is used to generate the event that for Timer2 couting.
 * @param  None
 * @retval None
 */
static void Tmr2CntPeripStart(void)
{
#if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT)
    /* Start event peripheral. Press key SW10 on the board. */

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF)
    /* Start timer6 */
    TMR6_CountCmd(M4_TMR6_1, Enable);

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE)
    /* Make falling/rising edge on pin TIM2_<t>_PWMx */

#endif
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
    }
}
#endif /* #if (APP_TMR2_USE_INTERRUPT > 0U) */

#if (APP_TMR2_USE_HW_TRIG > 0U)
/**
 * @brief  Configure the conditions which are used to start and stop Timer2.
 * @param  None
 * @retval None
 */
static void Tmr2TrigCondConfig(void)
{
    stc_tmr2_trig_cond_t stcTrigCond;

    /*
     * If a peripheral is used to generate the event which is used as a hardware trigger condition of Timer2, \
     *   call the API of the peripheral to configure the peripheral.
     * The following operations are only used in this example.
     */
#if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT)
    #if (defined APP_TMR2_FILTER_ENABLE && APP_TMR2_FILTER_ENABLE > 0U)
        TMR2_FilterConfig(APP_TMR2_UNIT, APP_TMR2_CH, APP_TMR2_FILTER_CLK_DIV);
        TMR2_FilterCmd(APP_TMR2_UNIT, APP_TMR2_CH, Enable);
    #endif
    GPIO_SetFunc(APP_TMR2_TRIG_PORT, APP_TMR2_TRIG_PIN, APP_TMR2_TRIG_PIN_FUNC, PIN_SUBFUNC_DISABLE);

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF)
    #if (defined APP_TMR2_FILTER_ENABLE && APP_TMR2_FILTER_ENABLE > 0U)
        TMR2_FilterConfig(APP_TMR2_UNIT, APP_TMR2_CH, APP_TMR2_FILTER_CLK_DIV);
        TMR2_FilterCmd(APP_TMR2_UNIT, APP_TMR2_CH, Enable);
    #endif
    GPIO_SetFunc(APP_TMR2_TRIG_PORT, APP_TMR2_TRIG_PIN, APP_TMR2_TRIG_PIN_FUNC, PIN_SUBFUNC_DISABLE);

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    TMR2_SetTriggerSrc(APP_TMR2_TRIG_EVENT);
    EventKeyConfig();

#elif (APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE)
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    TMR2_SetTriggerSrc(APP_TMR2_TRIG_EVENT);
    EventKeyConfig();

#endif
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
     *   call the API of the peripheral to start the peripheral here or anywhere else you need.
     * The following operations are only used in this example.
     */

#if ((APP_TMR2_USE_HW_TRIG == 0U) || \
     ((APP_TMR2_USE_HW_TRIG > 0U) && (APP_TMR2_START_COND == TMR2_START_COND_INVALID)))
    /* Software start. */
    TMR2_Start(APP_TMR2_UNIT, APP_TMR2_CH);
#else
    /* Hardware start. */
    #if (APP_TMR2_FUNC == APP_FUNC_CNT_PERIP_EVENT)
        /* Make an rising edge on pin TIM2_<t>_PWMx(PA2 for this example) to start Timer2. */
    #elif (APP_TMR2_FUNC == APP_FUNC_CNT_TIMER6_OVF)
        /* Press key SW10 to start Timer2. */
    #else /* (APP_TMR2_FUNC == APP_FUNC_CNT_PIN_EDGE) */
        /* Press key SW10 to start Timer2. */
    #endif
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
