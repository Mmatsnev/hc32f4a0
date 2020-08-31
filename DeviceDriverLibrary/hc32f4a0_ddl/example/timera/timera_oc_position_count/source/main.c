/**
 *******************************************************************************
 * @file  timera/timera_oc_position_count/source/main.c
 * @brief Main program TimerA OC position count for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
   2020-08-20       Wuze            Calculating the rotation speed of the quadrature
                                    encoder via postion counting function.
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
 * @addtogroup TMRA_OC_Position_Count
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Phase-difference count. */
#define APP_PHASE_DIFF_COUNT_X1             (1U)
#define APP_PHASE_DIFF_COUNT_X2             (2U)
#define APP_PHASE_DIFF_COUNT_X4             (4U)

/* Select phase-difference count. */
#define APP_PHASE_DIFF_COUNT                (APP_PHASE_DIFF_COUNT_X1)

/* OC cycles per round. Different OC different parameter. */
#define OC_CYCLE_PER_ROUND                  (1000UL)
#define APP_OC_CYCLE_PER_ROUND              (OC_CYCLE_PER_ROUND * APP_PHASE_DIFF_COUNT)

/*
 * TimerA unit and channel definitions for this example.
 * 'APP_TMRA_UNIT' can be defined as M4_TMRA_<t>(t=1 ~ 12).
 */
#define APP_TMRA_UNIT                       (M4_TMRA_9)
#define APP_TMRA_PERIP_CLK                  (PWC_FCG2_TMRA_9)

/* Select the pins for phase A and phase B inputing according to 'APP_TMRA_UNIT'. */
#define APP_A_PORT                          (GPIO_PORT_G)
#define APP_A_PIN                           (GPIO_PIN_04)
#define APP_A_PIN_FUNC                      (GPIO_FUNC_4_TIMA9_PWM1)
#define APP_B_PORT                          (GPIO_PORT_G)
#define APP_B_PIN                           (GPIO_PIN_05)
#define APP_B_PIN_FUNC                      (GPIO_FUNC_4_TIMA9_PWM2)

/* Define the configuration values according to the function that selected. */
#if (APP_PHASE_DIFF_COUNT == APP_PHASE_DIFF_COUNT_X1)
    #define APP_TMRA_CLK                    (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR)

#elif (APP_PHASE_DIFF_COUNT == APP_PHASE_DIFF_COUNT_X2)
    #define APP_TMRA_CLK                    (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF)

#elif (APP_PHASE_DIFF_COUNT == APP_PHASE_DIFF_COUNT_X4)
    #define APP_TMRA_CLK                    (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF | \
                                             TMRA_CLK_HW_UP_CLKAH_CLKBF | TMRA_CLK_HW_DOWN_CLKBH_CLKAF | \
                                             TMRA_CLK_HW_UP_CLKAL_CLKBR | TMRA_CLK_HW_DOWN_CLKBL_CLKAR)
#else
    #error "Phase-difference count is NOT supported!!!"
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

/* 1 second timer, for calculating rotation speed. */
static void Tmr2Config(void);
static void Tmr2IrqConfig(void);
static void TMR2_Cmp_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8SpeedUpd  = 0UL;
static uint32_t m_u32OCSpeed = 0UL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timera_oc_position_count project
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

    /* Starts Timer2. */
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);
    /* Starts TimerA. */
    TMRA_Start(APP_TMRA_UNIT);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        if (m_u8SpeedUpd != 0U)
        {
            DBG("OC speed: %d RPM\n", m_u32OCSpeed);
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
static void TmrAConfig(void)
{
    stc_tmra_init_t stcInit;

    /* 1. Enable TimerA peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMRA_PERIP_CLK, Enable);

    /* 2. Set a default initialization value for stcInit. */
    TMRA_StructInit(&stcInit);

    /* 3. Modifies the initialization values depends on the application. */
    stcInit.u32ClkSrc = APP_TMRA_CLK;
    TMRA_Init(APP_TMRA_UNIT, &stcInit);

    /* 4. Configures the function of phase A and phase B's input pin. */
    GPIO_SetFunc(APP_A_PORT, APP_A_PIN, APP_A_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(APP_B_PORT, APP_B_PIN, APP_B_PIN_FUNC, PIN_SUBFUNC_DISABLE);
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
    NVIC_SetPriority(stcCfg.enIRQn, DDL_IRQ_PRIORITY_03);
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
    uint32_t u32Dir;
    uint32_t u32CurrCycleCnt;
    uint32_t u32CycleCnt;
    static uint32_t u32LastCycleCnt = 0U;
    static uint32_t u32TmrCnt = 0U;

    u32TmrCnt++;
    /* 100ms * 10 = 1s */
    if (u32TmrCnt >= 10U)
    {
        u32CurrCycleCnt = TMRA_GetCntVal(APP_TMRA_UNIT);
        u32Dir = TMRA_GetCntDir(APP_TMRA_UNIT);

        if (u32Dir == TMRA_DIR_DOWN)
        {
            if (u32CurrCycleCnt > u32LastCycleCnt)
            {
                u32CycleCnt = (65536UL + u32LastCycleCnt) - u32CurrCycleCnt;
            }
            else
            {
                u32CycleCnt = u32LastCycleCnt - u32CurrCycleCnt;
            }
        }
        else /* (u32TmrADir == TMRA_DIR_UP) */
        {
            if (u32CurrCycleCnt > u32LastCycleCnt)
            {
                u32CycleCnt = u32CurrCycleCnt - u32LastCycleCnt;
            }
            else
            {
                u32CycleCnt = (65536UL + u32CurrCycleCnt) - u32LastCycleCnt;
            }
        }

        m_u32OCSpeed    = (u32CycleCnt * 60UL) / APP_OC_CYCLE_PER_ROUND;
        u32LastCycleCnt = u32CurrCycleCnt;
        m_u8SpeedUpd    = 1U;
        u32TmrCnt       = 0U;
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
