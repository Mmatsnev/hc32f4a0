/**
 *******************************************************************************
 * @file  timera/timera_pwm/source/main.c
 * @brief Main program TimerA pwm for the Device Driver Library.
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
 * @addtogroup TMRA_Pwm
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Function of this example. */
#define APP_FUNC_NORMAL_SINGLE_PWM          (0U)
#define APP_FUNC_SINGLE_EDGE_ALIGNED_PWM    (1U)
#define APP_FUNC_TOW_EDGE_SYMMETRIC_PWM     (2U)

/* Specify the function of the example. */
#define APP_FUNC                            (APP_FUNC_NORMAL_SINGLE_PWM)

/*
 * Define the configurations of PWM according to the function that selected.
 * In this example:
 *   1. System clock is XTAL(8MHz).
 *   2. Clock source of TimerA is PCLK(8MHz by default) and divided by 1.
 *   3. About PWM:
 *      APP_FUNC_NORMAL_SINGLE_PWM: frequecy 200KHz, high duty 30%
 *      APP_FUNC_SINGLE_EDGE_ALIGNED_PWM: frequecy 100KH, high duty 30%, 55%
 *      APP_FUNC_TOW_EDGE_SYMMETRIC_PWM: frequecy 50KHz, high duty 50%, 40%
 *
 * Sawtooth mode:
 *   Calculate the period value according to the frequency:
 *     PeriodVal = (TimerAClockFrequency(Hz) / PWMFreq) - 1
 *   Calculate the compare value according to the duty ratio:
 *     CmpVal = ((PeriodVal + 1) * Duty) - 1
 *
 * Triangle mode:
 *   Calculate the period value according to the frequency:
 *     PeriodVal = (TimerAClockFrequency(Hz) / (PWMFreq * 2))
 *   Calculate the compare value according to the duty ratio:
 *     CmpVal = (PeriodVal * Duty)
 */
#if (APP_FUNC == APP_FUNC_NORMAL_SINGLE_PWM)
    #define APP_TMRA_UNIT                   (M4_TMRA_1)
    #define APP_TMRA_PERIP_CLK              (PWC_FCG2_TMRA_1)
    #define APP_TMRA_PWM_A_CH               (TMRA_CH_2)

    #define APP_TMRA_PWM_A_PORT             (GPIO_PORT_A)
    #define APP_TMRA_PWM_A_PIN              (GPIO_PIN_09)
    #define APP_TMRA_PWM_A_PIN_FUNC         (GPIO_FUNC_4_TIMA1_PWM2)

    #define APP_TMRA_MODE                   (TMRA_MODE_SAWTOOTH)
    #define APP_TMRA_DIR                    (TMRA_DIR_UP)
    #define APP_TMRA_PERIOD_VAL             (40UL - 1UL)
    #define APP_TMRA_PWM_A_CMP_VAL          (12UL - 1UL)

#elif (APP_FUNC == APP_FUNC_SINGLE_EDGE_ALIGNED_PWM)
    #define APP_TMRA_UNIT                   (M4_TMRA_1)
    #define APP_TMRA_PERIP_CLK              (PWC_FCG2_TMRA_1)
    #define APP_TMRA_PWM_A_CH               (TMRA_CH_1)
    #define APP_TMRA_PWM_B_CH               (TMRA_CH_2)

    #define APP_TMRA_PWM_A_PORT             (GPIO_PORT_A)
    #define APP_TMRA_PWM_A_PIN              (GPIO_PIN_08)
    #define APP_TMRA_PWM_A_PIN_FUNC         (GPIO_FUNC_4_TIMA1_PWM1)
    #define APP_TMRA_PWM_B_PORT             (GPIO_PORT_A)
    #define APP_TMRA_PWM_B_PIN              (GPIO_PIN_09)
    #define APP_TMRA_PWM_B_PIN_FUNC         (GPIO_FUNC_4_TIMA1_PWM2)

    #define APP_TMRA_MODE                   (TMRA_MODE_SAWTOOTH)
    #define APP_TMRA_DIR                    (TMRA_DIR_UP)
    #define APP_TMRA_PERIOD_VAL             (80UL - 1UL)
    #define APP_TMRA_PWM_A_CMP_VAL          (24UL - 1UL)
    #define APP_TMRA_PWM_B_CMP_VAL          (44UL - 1UL)

#elif (APP_FUNC == APP_FUNC_TOW_EDGE_SYMMETRIC_PWM)
    #define APP_TMRA_UNIT                   (M4_TMRA_1)
    #define APP_TMRA_PERIP_CLK              (PWC_FCG2_TMRA_1)
    #define APP_TMRA_PWM_A_CH               (TMRA_CH_1)
    #define APP_TMRA_PWM_B_CH               (TMRA_CH_2)

    #define APP_TMRA_PWM_A_PORT             (GPIO_PORT_A)
    #define APP_TMRA_PWM_A_PIN              (GPIO_PIN_08)
    #define APP_TMRA_PWM_A_PIN_FUNC         (GPIO_FUNC_4_TIMA1_PWM1)
    #define APP_TMRA_PWM_B_PORT             (GPIO_PORT_A)
    #define APP_TMRA_PWM_B_PIN              (GPIO_PIN_09)
    #define APP_TMRA_PWM_B_PIN_FUNC         (GPIO_FUNC_4_TIMA1_PWM2)

    #define APP_TMRA_MODE                   (TMRA_MODE_TRIANGLE)
    #define APP_TMRA_DIR                    (TMRA_DIR_UP)
    #define APP_TMRA_PERIOD_VAL             (80UL)
    #define APP_TMRA_PWM_A_CMP_VAL          (40UL)
    #define APP_TMRA_PWM_B_CMP_VAL          (48UL)

#else
    #error "The function is NOT supported."
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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timera_pwm project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the system clock. */
    SystemClockConfig();
    /* Configures TimerA. */
    TmrAConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts TimerA. */
    TMRA_Start(APP_TMRA_UNIT);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        /*
         * Stop PWM output:
         *   TMRA_Stop(APP_TMRA_UNIT);
         *   or
         *   TMRA_PWM_Cmd(APP_TMRA_UNIT, APP_TMRA_PWM_x_CH, Disable);
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
    // PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
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
    // PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
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
 * @brief  Configures XTAL(8MHz) as the system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_xtal_init_t stcXtalInit;

    /* Xtal config */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);
    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_XTAL);
}

/**
 * @brief  TimerA configuration.
 * @param  None
 * @retval None
 */
static void TmrAConfig(void)
{
    stc_tmra_init_t stcInit;
    stc_tmra_pwm_cfg_t stcCfg;

    /* 1. Enable TimerA peripheral clock. */
    PWC_Fcg2PeriphClockCmd(APP_TMRA_PERIP_CLK, Enable);

    /* 2. Set a default initialization value for stcInit. */
    TMRA_StructInit(&stcInit);

    /* 3. Modifies the initialization values depends on the application. */
    stcInit.u32CntDir    = APP_TMRA_DIR;
    stcInit.u32CntMode   = APP_TMRA_MODE;
    stcInit.u32PeriodVal = APP_TMRA_PERIOD_VAL;
    TMRA_Init(APP_TMRA_UNIT, &stcInit);

    /* 4. Set the comparison reference value. */
#if (APP_FUNC == APP_FUNC_NORMAL_SINGLE_PWM)
    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, APP_TMRA_PWM_A_CMP_VAL);
    GPIO_SetFunc(APP_TMRA_PWM_A_PORT, APP_TMRA_PWM_A_PIN, APP_TMRA_PWM_A_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    TMRA_PWM_StructInit(&stcCfg);
    TMRA_PWM_Config(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, &stcCfg);
    TMRA_PWM_Cmd(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, Enable);

#elif (APP_FUNC == APP_FUNC_SINGLE_EDGE_ALIGNED_PWM)
    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, APP_TMRA_PWM_A_CMP_VAL);
    GPIO_SetFunc(APP_TMRA_PWM_A_PORT, APP_TMRA_PWM_A_PIN, APP_TMRA_PWM_A_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    TMRA_PWM_StructInit(&stcCfg);
    TMRA_PWM_Config(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, &stcCfg);
    TMRA_PWM_Cmd(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, Enable);

    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_PWM_B_CH, APP_TMRA_PWM_B_CMP_VAL);
    GPIO_SetFunc(APP_TMRA_PWM_B_PORT, APP_TMRA_PWM_B_PIN, APP_TMRA_PWM_B_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    TMRA_PWM_StructInit(&stcCfg);
    TMRA_PWM_Config(APP_TMRA_UNIT, APP_TMRA_PWM_B_CH, &stcCfg);
    TMRA_PWM_Cmd(APP_TMRA_UNIT, APP_TMRA_PWM_B_CH, Enable);

#else
    TMRA_PWM_StructInit(&stcCfg);
    stcCfg.u32StartPolarity  = TMRA_PWM_START_HIGH;
    stcCfg.u32StopPolarity   = TMRA_PWM_STOP_HIGH;
    stcCfg.u32PeriodPolarity = TMRA_PWM_PERIOD_KEEP;
    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, APP_TMRA_PWM_A_CMP_VAL);
    GPIO_SetFunc(APP_TMRA_PWM_A_PORT, APP_TMRA_PWM_A_PIN, APP_TMRA_PWM_A_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    TMRA_PWM_Config(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, &stcCfg);
    TMRA_PWM_Cmd(APP_TMRA_UNIT, APP_TMRA_PWM_A_CH, Enable);

    stcCfg.u32StartPolarity = TMRA_PWM_START_LOW;
    stcCfg.u32StopPolarity  = TMRA_PWM_STOP_LOW;
    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_PWM_B_CH, APP_TMRA_PWM_B_CMP_VAL);
    GPIO_SetFunc(APP_TMRA_PWM_B_PORT, APP_TMRA_PWM_B_PIN, APP_TMRA_PWM_B_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    TMRA_PWM_Config(APP_TMRA_UNIT, APP_TMRA_PWM_B_CH, &stcCfg);
    TMRA_PWM_Cmd(APP_TMRA_UNIT, APP_TMRA_PWM_B_CH, Enable);
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
