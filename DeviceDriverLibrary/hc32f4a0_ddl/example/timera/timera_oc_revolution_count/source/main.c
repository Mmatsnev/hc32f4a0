/**
 *******************************************************************************
 * @file  timera/timera_oc_revolution_count/source/main.c
 * @brief Main program TimerA OC revolution count for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
   2020-08-20       Wuze            Calculating the rotation speed of the quadrature
                                    encoder via revolution counting function.
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
 * @addtogroup TMRA_OC_Revolution_Count
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Function of this example. */
#define APP_FUNC_PHASE_Z_COUNT              (1U)
#define APP_FUNC_POS_OVF_COUNT              (2U)

/* Select a function for this example. */
#define APP_FUNC                            (APP_FUNC_PHASE_Z_COUNT)

/* Position unit phase-difference count */
#define APP_POS_PHASE_DIFF_COUNT_X1         (1U)
#define APP_POS_PHASE_DIFF_COUNT_X2         (2U)
#define APP_POS_PHASE_DIFF_COUNT_X4         (4U)

/* Select position unit phase-difference count. */
#define APP_POS_PHASE_DIFF_COUNT            (APP_POS_PHASE_DIFF_COUNT_X1)

/*
 * TimerA unit definitions for this example.
 *
 * APP_TMRA_POS_UNIT         APP_TMRA_Z_UNIT
 *    M4_TMRA_1                 M4_TMRA_2
 *    M4_TMRA_3                 M4_TMRA_4
 *    M4_TMRA_5                 M4_TMRA_6
 *    M4_TMRA_7                 M4_TMRA_8
 *    M4_TMRA_9                 M4_TMRA_10
 *    M4_TMRA_11                M4_TMRA_12
 */
#define APP_TMRA_POS_UNIT                   (M4_TMRA_9)
#define APP_TMRA_Z_UNIT                     (M4_TMRA_10)
#define APP_TMRA_PERIP_CLK                  (PWC_FCG2_TMRA_9 | PWC_FCG2_TMRA_10)

#define APP_TMRA_POS_CH                     (TMRA_CH_1)

#define APP_TMRA_Z_OVF_EVENT                (EVT_TMRA_10_OVF)

/* Definitions about interrupt. */
#define APP_TMRA_Z_UNIT_INT_TYPE            (TMRA_INT_OVF)
#define APP_TMRA_Z_UNIT_INT_PRIO            (DDL_IRQ_PRIORITY_03)
#define APP_TMRA_Z_UNIT_INT_SRC             (INT_TMRA_10_OVF)
#define APP_TMRA_Z_UNIT_IRQn                (Int024_IRQn)
#define APP_TMRA_FLAG                       (TMRA_FLAG_OVF)

/* Select the pins for phase A and phase B inputing according to 'APP_TMRA_POS_UNIT' and 'APP_TMRA_Z_UNIT'. */
#define APP_A_PORT                          (GPIO_PORT_G)
#define APP_A_PIN                           (GPIO_PIN_04)
#define APP_A_PIN_FUNC                      (GPIO_FUNC_4_TIMA9_PWM1)
#define APP_B_PORT                          (GPIO_PORT_G)
#define APP_B_PIN                           (GPIO_PIN_05)
#define APP_B_PIN_FUNC                      (GPIO_FUNC_4_TIMA9_PWM2)

#define APP_Z_PORT                          (GPIO_PORT_F)
#define APP_Z_PIN                           (GPIO_PIN_05)
#define APP_Z_PIN_FUNC                      (GPIO_FUNC_6_TIMA10_TRIG)

/* Phase-difference count. */
#if (APP_POS_PHASE_DIFF_COUNT == APP_POS_PHASE_DIFF_COUNT_X1)
    #define APP_TMRA_POS_UNIT_CLK           (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR)

#elif (APP_POS_PHASE_DIFF_COUNT == APP_POS_PHASE_DIFF_COUNT_X2)
    #define APP_TMRA_POS_UNIT_CLK           (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF)

#elif (APP_POS_PHASE_DIFF_COUNT == APP_POS_PHASE_DIFF_COUNT_X4)
    #define APP_TMRA_POS_UNIT_CLK           (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF | \
                                             TMRA_CLK_HW_UP_CLKAH_CLKBF | TMRA_CLK_HW_DOWN_CLKBH_CLKAF | \
                                             TMRA_CLK_HW_UP_CLKAL_CLKBR | TMRA_CLK_HW_DOWN_CLKBL_CLKAR)
#endif

/* Position unit cycles per round. */
#define APP_POS_CYCLE_PER_ROUND             (1000UL * APP_POS_PHASE_DIFF_COUNT)

/* Define the configuration values according to the function that selected. */
#if (APP_FUNC == APP_FUNC_PHASE_Z_COUNT)
    #define APP_TMRA_POS_UNIT_CLR_COND      (TMRA_CLR_COND_EVENT)
    #define APP_TMRA_POS_UNIT_CLR_EVENT     (APP_TMRA_Z_OVF_EVENT)
    #define APP_TMRA_POS_UNIT_CAPT_COND     (TMRA_CAPT_COND_EVENT)
    #define APP_TMRA_POS_UNIT_CAPT_EVENT    (APP_TMRA_Z_OVF_EVENT)

    #define APP_TMRA_Z_UNIT_CLK             (TMRA_CLK_HW_UP_TRIGR)
    /* Counts 1 revolution */
    #define APP_TMRA_Z_UNIT_PERIOD_VAL      (1UL - 1UL)

#elif (APP_FUNC == APP_FUNC_POS_OVF_COUNT)
    /* Counts 100 cycles. */
    #define APP_TMRA_POS_BASE_CNT           (100UL)
    #define APP_TMRA_POS_UNIT_PERIOD_VAL    (APP_TMRA_POS_BASE_CNT - 1UL)
    /* Different direction, different clock for Z unit.
       APP_TMRA_Z_UNIT_CLK can be defined as 'TMRA_CLK_HW_UP_SYM_UNF' or 'TMRA_CLK_HW_UP_SYM_OVF',
       according to the real application. */
    #define APP_TMRA_Z_UNIT_CLK             (TMRA_CLK_HW_UP_SYM_UNF)
    /* Counts 10 revolutions */
    #define APP_TMRA_Z_UNIT_PERIOD_VAL      ((APP_POS_CYCLE_PER_ROUND / APP_TMRA_POS_BASE_CNT)  - 1UL)

#else
    #error "This function is NOT supported!!!"
#endif

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

static void TmrAConfig(void);
static void TmrAIrqConfig(void);
static void TMRA_Ovf_IrqCallback(void);

/* 1 second timer, for calculating rotation speed. */
static void Tmr2Config(void);
static void Tmr2IrqConfig(void);
static void TMR2_Cmp_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8SpeedUpd  = 0UL;
static uint32_t m_u32RevolutionCnt = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timera_oc_revolution_count project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the PLLHP(240MHz) as the system clock. */
    SystemClockConfig();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures TimerA. */
    TmrAConfig();
    /* Configures Timer2. */
    Tmr2Config();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts TimerA. */
    TMRA_Start(APP_TMRA_Z_UNIT);
    TMRA_Start(APP_TMRA_POS_UNIT);
    /* Starts Timer2. */
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        if (m_u8SpeedUpd != 0U)
        {
            DBG("OC speed: %d RPM\n", m_u32RevolutionCnt * 60UL);
            m_u32RevolutionCnt = 0UL;
            m_u8SpeedUpd = 0U;
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
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV8 | CLK_PCLK2_DIV4 | \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV4 | CLK_EXCLK_DIV2 | \
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
    PWC_Lock(PWC_UNLOCK_CODE_0);
}

/**
 * @brief  TimerA configuration.
 * @param  None
 * @retval None
 */
#if (APP_FUNC == APP_FUNC_PHASE_Z_COUNT)
static void TmrAConfig(void)
{
    stc_tmra_init_t stcInit;
    stc_tmra_trig_cond_t stcCfg;

    /* 1. Configures the function of phase A, phase B and phase Z's input pin. */
    GPIO_SetFunc(APP_A_PORT, APP_A_PIN, APP_A_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(APP_B_PORT, APP_B_PIN, APP_B_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(APP_Z_PORT, APP_Z_PIN, APP_Z_PIN_FUNC, PIN_SUBFUNC_DISABLE);

    /* 2. Enable TimerA peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMRA_PERIP_CLK, Enable);

    TMRA_StructInit(&stcInit);
    /* 3.1 Initializes position-count unit. */
    stcInit.u32ClkSrc = APP_TMRA_POS_UNIT_CLK;
    TMRA_Init(APP_TMRA_POS_UNIT, &stcInit);

    /* 3.2 Configures the clear-condition of position-count unit. */
    TMRA_TrigCondStructInit(&stcCfg);
    stcCfg.u32ClrCond = APP_TMRA_POS_UNIT_CLR_COND;
    TMRA_SetTrigCond(APP_TMRA_POS_UNIT, &stcCfg);
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    TMRA_SetTriggerSrc(APP_TMRA_POS_UNIT, TMRA_EVENT_USAGE_CNT, APP_TMRA_POS_UNIT_CLR_EVENT);

    /* 3.3 Configures the capture-condition of position-count unit if needed. */
    /* PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable); */
    TMRA_SetTriggerSrc(APP_TMRA_POS_UNIT, TMRA_EVENT_USAGE_CAPT, APP_TMRA_POS_UNIT_CAPT_EVENT);
    TMRA_SetCaptCond(APP_TMRA_POS_UNIT, APP_TMRA_POS_CH, APP_TMRA_POS_UNIT_CAPT_COND);
    TMRA_SetFuncMode(APP_TMRA_POS_UNIT, APP_TMRA_POS_CH, TMRA_FUNC_CAPTURE);

    /* 4. Initializes z-count unit. */
    stcInit.u32ClkSrc    = APP_TMRA_Z_UNIT_CLK;
    stcInit.u32PeriodVal = APP_TMRA_Z_UNIT_PERIOD_VAL;
    TMRA_Init(APP_TMRA_Z_UNIT, &stcInit);

    /* 5. Configures interrupt if needed. */
    TmrAIrqConfig();
}
#else /*(APP_FUNC == APP_FUNC_POS_OVF_COUNT) */
static void TmrAConfig(void)
{
    stc_tmra_init_t stcInit;

    /* 1. Configures the function of phase A, phase B and phase Z's input pin. */
    GPIO_SetFunc(APP_A_PORT, APP_A_PIN, APP_A_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(APP_B_PORT, APP_B_PIN, APP_B_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(APP_Z_PORT, APP_Z_PIN, APP_Z_PIN_FUNC, PIN_SUBFUNC_DISABLE);

    /* 2. Enable TimerA peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMRA_PERIP_CLK, Enable);

    TMRA_StructInit(&stcInit);
    /* 3. Initializes position-count unit. */
    stcInit.u32ClkSrc    = APP_TMRA_POS_UNIT_CLK;
    stcInit.u32PeriodVal = APP_TMRA_POS_UNIT_PERIOD_VAL;
    TMRA_Init(APP_TMRA_POS_UNIT, &stcInit);

    /* 4. Initializes z-count unit. */
    stcInit.u32ClkSrc    = APP_TMRA_Z_UNIT_CLK;
    stcInit.u32PeriodVal = APP_TMRA_Z_UNIT_PERIOD_VAL;
    TMRA_Init(APP_TMRA_Z_UNIT, &stcInit);

    /* 5. Configures interrupt if needed. */
    TmrAIrqConfig();
}
#endif /* #if (APP_FUNC == APP_FUNC_PHASE_Z_COUNT) */

/**
 * @brief  TimerA interrupt configuration.
 * @param  None
 * @retval None
 */
static void TmrAIrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = APP_TMRA_Z_UNIT_INT_SRC;
    stcCfg.enIRQn      = APP_TMRA_Z_UNIT_IRQn;
    stcCfg.pfnCallback = &TMRA_Ovf_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_TMRA_Z_UNIT_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of TimerA. */
    TMRA_IntCmd(APP_TMRA_Z_UNIT, APP_TMRA_Z_UNIT_INT_TYPE, Enable);
}

/**
 * @brief  TimerA z-count unit overflow interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMRA_Ovf_IrqCallback(void)
{
    if (TMRA_GetStatus(APP_TMRA_Z_UNIT, APP_TMRA_FLAG) == Set)
    {
        TMRA_ClrStatus(APP_TMRA_Z_UNIT, APP_TMRA_FLAG);
#if (APP_FUNC == APP_FUNC_PHASE_Z_COUNT)
        /* Quadrature encoder generates an interrupt every revolution. */

        /* Read the number of clocks generated by the quadrature encoder per revolution. */
        /* xxxx = TMRA_GetCmpVal(APP_TMRA_POS_UNIT, APP_TMRA_POS_CH); */
#endif
        m_u32RevolutionCnt++;
    }
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
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR2_1, Enable);

    /* 2. Set a default initialization value for stcInit. */
    TMR2_StructInit(&stcInit);

    /* 3. Modifies the initialization values depends on the application. */
    stcInit.u32ClkSrc = TMR2_CLK_SYNC_PCLK1;
    stcInit.u32ClkDiv = TMR2_CLK_DIV64;
    stcInit.u32CmpVal = 46875UL - 1UL;
    TMR2_Init(M4_TMR2_1, TMR2_CH_A, &stcInit);

    /* 4. Configures IRQ if needed. */
    Tmr2IrqConfig();
}

/**
 * @brief  Timer2 interrupt configuration.
 * @param  None
 * @retval None
 */
static void Tmr2IrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = INT_TMR2_1_CMPA;
    stcCfg.enIRQn      = Int050_IRQn;
    stcCfg.pfnCallback = &TMR2_Cmp_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, DDL_IRQ_PRIORITY_06);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of Timer2. */
    TMR2_IntCmd(M4_TMR2_1, TMR2_CH_A, TMR2_INT_CMP, Enable);
}

/**
 * @brief  Timer2 counter comparison match interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR2_Cmp_IrqCallback(void)
{
    static uint32_t u32TmrCnt = 0U;

    u32TmrCnt++;
    /* 100ms * 10 = 1s */
    if (u32TmrCnt >= 10U)
    {
        m_u8SpeedUpd = 1U;
        u32TmrCnt    = 0U;
    }
    TMR2_ClrStatus(M4_TMR2_1, TMR2_CH_A, TMR2_FLAG_CMP);
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
