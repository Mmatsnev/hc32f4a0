/**
 *******************************************************************************
 * @file  timera/timera_oc_revolution_count/source/main.c
 * @brief Main program TimerA OC revolution count for the Device Driver Library.
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
#define TMRA_SHARE_IRQn_BASE                (Int135_IRQn)

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

/* Phase-difference count X2. */
#define APP_TMRA_POS_UNIT_CLK               (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF)

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
    /* Counts 20 clocks. */
    #define APP_TMRA_POS_UNIT_PERIOD_VAL    (20UL - 1UL)
    #define APP_TMRA_Z_UNIT_CLK             (TMRA_CLK_HW_UP_SYM_OVF)
    /* Counts 10 revolutions */
    #define APP_TMRA_Z_UNIT_PERIOD_VAL      (10UL - 1UL)

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

static void TmrAConfig(void);
static void TmrAIrqConfig(void);

static void TMRA_Ovf_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
#if (APP_FUNC == APP_FUNC_PHASE_Z_COUNT)
    __IO static uint32_t u32RevolutionCnt = 0U;
    __IO static uint32_t u32ClocksPerRevolution = 0U;
#endif

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
    uint32_t u32TmrADir    = 0xFFFFFFFFU;
    int16_t s16TmrACntCurr = 0U;
    int16_t s16TmrACntLast = 0U;

    /* The default system clock is MRC(8MHz). */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures TimerA. */
    TmrAConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts TimerA. */
    TMRA_Start(APP_TMRA_Z_UNIT);
    TMRA_Start(APP_TMRA_POS_UNIT);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        s16TmrACntCurr = (int16_t)TMRA_GetCntVal(APP_TMRA_POS_UNIT);

        if (s16TmrACntCurr != s16TmrACntLast)
        {
            /* Get direction of counting. */
            u32TmrADir = TMRA_GetCntDir(APP_TMRA_POS_UNIT);
            if (u32TmrADir == TMRA_DIR_DOWN)
            {
                DBG("TimerA counts down. Steps: %d \n", (s16TmrACntCurr - s16TmrACntLast));
            }
            else if (u32TmrADir == TMRA_DIR_UP)
            {
                DBG("TimerA counts up. Steps: %d \n", (s16TmrACntCurr - s16TmrACntLast));
            }
            else
            {}
            s16TmrACntLast = s16TmrACntCurr;
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
        u32ClocksPerRevolution = TMRA_GetCmpVal(APP_TMRA_POS_UNIT, APP_TMRA_POS_CH);

        u32RevolutionCnt++;
#else
        /* The counter reaches the specified period value. */
#endif
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
