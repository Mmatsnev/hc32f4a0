/**
 *******************************************************************************
 * @file  timera/timera_compare_value_cache/source/main.c
 * @brief Main program TimerA compare value cache for the Device Driver Library.
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
 * @addtogroup TMRA_Compare_Value_Cache
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
 * This example is going to show how to cache the compare value of even channel.
 */

/*
 * TimerA unit and channel definitions for this example.
 * Symmetric channel: channel 1 and 2; channel 3 and 4.
 *
 *    APP_TMRA_SRC_CH       APP_TMRA_DEST_CH
 *      TMRA_CH_2               TMRA_CH_1
 *      TMRA_CH_4               TMRA_CH_3
 */
#define APP_TMRA_UNIT                       (M4_TMRA_1)
#define APP_TMRA_SRC_CH                     (TMRA_CH_4)
#define APP_TMRA_DEST_CH                    (TMRA_CH_3)
#define APP_TMRA_PERIP_CLK                  (PWC_FCG2_TMRA_1)

/* The counting mode of TimerA. @ref TMRA_Count_Mode */
#define APP_TMRA_MODE                       (TMRA_MODE_SAWTOOTH)

/* The counting direction of TimerA. @ref TMRA_Count_Direction */
#define APP_TMRA_DIR                        (TMRA_DIR_UP)

/*
 * Clock source of TimerA in this example.
 * The only valid clock source of TimerA in this example is PCLK(PCLK0 for unit1 ~ uint4. PCLK1 for unit5 ~ uint12).
 */
#define APP_TMRA_CLK                        (TMRA_CLK_PCLK)

/* The divider of the clock source. @ref TMRA_PCLK_Divider */
#define APP_TMRA_PCLK_DIV                   (TMRA_PCLK_DIV8)

/* Period value and compare value. */
#define APP_TMRA_PERIOD_VAL                 (0x2000UL)
#define APP_TMRA_SRC_CMP_VAL                (0x1234UL)

/*
 * Cache-condition.
 * 'APP_TMRA_CACHE_CONDITION' can be defined as a value of @ref TMRA_Cmp_Value_Cache_Condition
 */

#if (APP_TMRA_MODE == TMRA_MODE_TRIANGLE)
    /* 'APP_TMRA_CACHE_CONDITION' can be defined as 'TMRA_CACHE_COND_TW_PEAK' and 'TMRA_CACHE_COND_TW_VALLEY' */
    #define APP_TMRA_CACHE_CONDITION        (TMRA_CACHE_COND_TW_PEAK)
#else
    /* Condition: 'APP_TMRA_MODE' is 'TMRA_MODE_SAWTOOTH' or 'APP_TMRA_CLK' is not 'TMRA_CLK_PCLK' */
    #define APP_TMRA_CACHE_CONDITION        (TMRA_CACHE_COND_OVF_CLR)
#endif

/*
 * Definitions about TimerA interrupt for the example.
 * IRQn of TimerA:
 *   M4_TMRA_x(x=1, 2): [Int000_IRQn, Int031_IRQn], [Int074_IRQn, Int079_IRQn];
 *   M4_TMRA_x(x=3, 4): [Int000_IRQn, Int031_IRQn], [Int080_IRQn, Int085_IRQn];
 *   M4_TMRA_x(x=5 ~ 8): [Int000_IRQn, Int031_IRQn], [Int092_IRQn, Int097_IRQn];
 *   M4_TMRA_x(x=9 ~ 12): [Int000_IRQn, Int031_IRQn], [Int098_IRQn, Int103_IRQn];
 *
 * NOTE!!! 'APP_TMRA_INT_TYPE' can only be defined as 'TMRA_INT_OVF'(x=1 ~ 4, depends on 'APP_TMRA_CH') for this example.
 */
#define TMRA_SHARE_IRQn_BASE                (Int135_IRQn)

#define APP_TMRA_INT_TYPE                   (TMRA_INT_OVF)
#define APP_TMRA_INT_FLAG                   (TMRA_FLAG_OVF)
#define APP_TMRA_INT_PRIO                   (DDL_IRQ_PRIORITY_03)
#define APP_TMRA_INT_SRC                    (INT_TMRA_1_OVF)
#define APP_TMRA_IRQn                       (Int074_IRQn)
#define APP_TMRA_FLAG                       (APP_TMRA_INT_FLAG)

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
    /* The default system clock is MRC(8MHz). */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures TimerA. */
    TmrAConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /* Starts TimerA. High unit first. */
    TMRA_Start(APP_TMRA_UNIT);

    /***************** Configuration end, application start **************/
    while (1U)
    {
        /* See TMRA_Ovf_IrqCallback in this file. */
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
    // GPIO_Unlock();
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
    // GPIO_Lock();
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

    /* 2. Initialize TimerA low unit . */
    TMRA_StructInit(&stcInit);
    stcInit.u32ClkSrc    = APP_TMRA_CLK;
    stcInit.u32PCLKDiv   = APP_TMRA_PCLK_DIV;
    stcInit.u32CntDir    = APP_TMRA_DIR;
    stcInit.u32CntMode   = APP_TMRA_MODE;
    stcInit.u32PeriodVal = APP_TMRA_PERIOD_VAL;
    TMRA_Init(APP_TMRA_UNIT, &stcInit);
    TMRA_SetCmpVal(APP_TMRA_UNIT, APP_TMRA_SRC_CH, APP_TMRA_SRC_CMP_VAL);

    /* 3. Cache-condition. */
    TMRA_CmpValCacheConfig(APP_TMRA_UNIT, APP_TMRA_DEST_CH, APP_TMRA_CACHE_CONDITION);
    TMRA_CmpValCacheCmd(APP_TMRA_UNIT, APP_TMRA_DEST_CH, Enable);

    /* 4. Configures IRQ if needed. */
    TmrAIrqConfig();
}

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
    stcCfg.pfnCallback = &TMRA_Ovf_IrqCallback;
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
static void TMRA_Ovf_IrqCallback(void)
{
    if (TMRA_GetStatus(APP_TMRA_UNIT, APP_TMRA_FLAG) == Set)
    {
        /*
         * Breakpoint here, you will see the compare value of the source channel was \
         *   cached to the destination channel and the destination channel can \
         *   use it.
         */

        TMRA_ClrStatus(APP_TMRA_UNIT, APP_TMRA_FLAG);
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
