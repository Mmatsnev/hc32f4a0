/**
 *******************************************************************************
 * @file  timera/timera_oc_position_count/source/main.c
 * @brief Main program TimerA OC position count for the Device Driver Library.
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
 * @addtogroup TMRA_OC_Position_Count
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Function of this example. */
#define APP_FUNC_PHASE_DIFF_COUNT_X1        (1U)
#define APP_FUNC_PHASE_DIFF_COUNT_X2        (2U)
#define APP_FUNC_PHASE_DIFF_COUNT_X4        (3U)

/* Select a function for this example. */
#define APP_FUNC                            (APP_FUNC_PHASE_DIFF_COUNT_X2)

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
#if (APP_FUNC == APP_FUNC_PHASE_DIFF_COUNT_X1)
    #define APP_TMRA_CLK                    (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR)

#elif (APP_FUNC == APP_FUNC_PHASE_DIFF_COUNT_X2)
    #define APP_TMRA_CLK                    (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF)

#elif (APP_FUNC == APP_FUNC_PHASE_DIFF_COUNT_X4)
    #define APP_TMRA_CLK                    (TMRA_CLK_HW_UP_CLKBH_CLKAR | TMRA_CLK_HW_DOWN_CLKAH_CLKBR | \
                                             TMRA_CLK_HW_UP_CLKBL_CLKAF | TMRA_CLK_HW_DOWN_CLKAL_CLKBF | \
                                             TMRA_CLK_HW_UP_CLKAH_CLKBF | TMRA_CLK_HW_DOWN_CLKBH_CLKAF | \
                                             TMRA_CLK_HW_UP_CLKAL_CLKBR | TMRA_CLK_HW_DOWN_CLKBL_CLKAR)

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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

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
    TMRA_Start(APP_TMRA_UNIT);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        s16TmrACntCurr = (int16_t)TMRA_GetCntVal(APP_TMRA_UNIT);

        if (s16TmrACntCurr != s16TmrACntLast)
        {
            /* Get direction of counting. */
            u32TmrADir = TMRA_GetCntDir(APP_TMRA_UNIT);
            if (u32TmrADir == TMRA_DIR_DOWN)
            {
                DBG("TimerA counts down. Number of clocks: %d \n", (s16TmrACntCurr - s16TmrACntLast));
            }
            else if (u32TmrADir == TMRA_DIR_UP)
            {
                DBG("TimerA counts up. Number of clocks: %d \n", (s16TmrACntCurr - s16TmrACntLast));
            }
            else
            {
                /* rsvd */
            }
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
    // PWC_FCG0_Unlock();
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
    // PWC_FCG0_Lock();
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
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
