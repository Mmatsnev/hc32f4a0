/**
 *******************************************************************************
 * @file  timer2/timer2_32bit_counter/source/main.c
 * @brief Main program Timer2 32-bit counter for the Device Driver Library.
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
 * @addtogroup TMR2_32bit_Counter
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
 * This example uses two Timer2 units to count 1 minute.
 */

/*
 * Timer2 units and channels definitions for this example.
 * NOTE!!! Timer2 low unit and high uint must be different.
 */
#define APP_L_TMR2_UNIT                     (M4_TMR2_1)
#define APP_L_TMR2_CH                       (TMR2_CH_A)
#define APP_H_TMR2_UNIT                     (M4_TMR2_2)
#define APP_H_TMR2_CH                       (TMR2_CH_A)
#define APP_TMR2_PERIP_CLK                  (PWC_FCG2_TMR2_1 | PWC_FCG2_TMR2_2)

/* The event of Timer2 low unit that is used as the synchronous clock of Timer2 high unit. */
#define APP_L_TMR2_CMP_EVENT                (EVT_TMR2_1_CMPA)

/* Definitions of interrupt of Timer2 high unit. */
#define APP_H_TMR2_INT_TYPE                 (TMR2_INT_CMP)
#define APP_H_TMR2_INT_FLAG                 (TMR2_FLAG_CMP)
#define APP_H_TMR2_INT_PRIO                 (DDL_IRQ_PRIORITY_03)
#define APP_H_TMR2_INT_SRC                  (INT_TMR2_2_CMPA)
#define APP_H_TMR2_IRQn                     (Int050_IRQn)

/*
 * In this example:
 *     Timing period is 1 second.
 *     Set PCLK1(8MHz) as the clock source of Timer2.
 *     Timer2 clock frequency is 8/1 = 8MHz, clock cycle is 1/8 = 0.125us.
 */
#define APP_L_TMR2_CLK_SRC                  (TMR2_CLK_SYNC_PCLK1)
#define APP_L_TMR2_CLK_DIV                  (TMR2_CLK_DIV1)

#define APP_H_TMR2_CLK_SRC                  (TMR2_CLK_SYNC_EVENT)
#define APP_H_TMR2_CLK_SRC_EVENT            (APP_L_TMR2_CMP_EVENT)

/*
 * Number of counting cycles: (TimingPeriod(second) / Timer2ClockCycle) - 1
 *
 * TimingPeriod: 1 second.
 * Number of counting cycles: (1s * 1000000us/s) / 0.125us = 0x7A1200 - 1 = 0x7A11FF.
 * Low Timer2 Initial value of the count register: 0x10000 - 0x11FF = 0xEE01
 * Low Timer2 compare value: 0xFFFF
 * High Timer2 compare value: 0x7A
 */
#define APP_H_TMR2_CMP_VAL                  (0x7AUL)
#define APP_L_TMR2_CMP_VAL                  (0xFFFFUL)
#define APP_L_TMR2_INIT_CNT_VAL             (0xEE01UL)

/* Definitions of interrupt. */
#define APP_TMR2_INT_UNIT                   (APP_H_TMR2_UNIT)
#define APP_TMR2_INT_FLAG                   (APP_H_TMR2_INT_FLAG)
#define APP_TMR2_INT_CH                     (APP_H_TMR2_CH)
#define APP_TMR2_INT_TYPE                   (APP_H_TMR2_INT_TYPE)
#define APP_TMR2_INT_PRIO                   (APP_H_TMR2_INT_PRIO)
#define APP_TMR2_INT_SRC                    (APP_H_TMR2_INT_SRC)
#define APP_TMR2_IRQn                       (APP_H_TMR2_IRQn)

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

static void IndicateConfig(void);

static void Tmr2Config(void);
static void Tmr2IrqConfig(void);
static void Tmr2Start(void);

static void TMR2_Cmp_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of timer2_pwm project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The system clock is MRC(8MHz) by default. */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
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

    /*
     * 3. Timer2 high unit initialization.
     *    Modifies the initialization values depends on the application.
     */
    stcInit.u32ClkSrc = APP_H_TMR2_CLK_SRC;
    stcInit.u32CmpVal = APP_H_TMR2_CMP_VAL;
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    TMR2_SetTriggerSrc(APP_H_TMR2_CLK_SRC_EVENT);
    TMR2_Init(APP_H_TMR2_UNIT, APP_H_TMR2_CH, &stcInit);

    /*
     * 4. Timer2 low unit initialization.
     *    Modifies the initialization values depends on the application.
     */
    stcInit.u32ClkSrc = APP_L_TMR2_CLK_SRC;
    stcInit.u32ClkDiv = APP_L_TMR2_CLK_DIV;
    stcInit.u32CmpVal = APP_L_TMR2_CMP_VAL;
    stcInit.u32CntVal = APP_L_TMR2_INIT_CNT_VAL;
    TMR2_Init(APP_L_TMR2_UNIT, APP_L_TMR2_CH, &stcInit);

    /* 5. Configures the interrupt. */
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

    stcCfg.enIntSrc    = APP_TMR2_INT_SRC;
    stcCfg.enIRQn      = APP_TMR2_IRQn;
    stcCfg.pfnCallback = &TMR2_Cmp_IrqCallback;

    /* Independent interrupt. */
    INTC_IrqSignIn(&stcCfg);
    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_TMR2_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of Timer2. */
    TMR2_IntCmd(APP_TMR2_INT_UNIT, APP_TMR2_INT_CH, APP_TMR2_INT_TYPE, Enable);
}

/**
 * @brief  Starts timer2 for 32-bit counting.
 * @param  None
 * @retval None
 */
static void Tmr2Start(void)
{
    TMR2_Start(APP_H_TMR2_UNIT, APP_H_TMR2_CH);
    TMR2_Start(APP_L_TMR2_UNIT, APP_L_TMR2_CH);
}

/**
 * @brief  Timer2 counter compare match interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR2_Cmp_IrqCallback(void)
{
    if (TMR2_GetStatus(APP_TMR2_INT_UNIT, APP_TMR2_INT_CH, APP_TMR2_INT_FLAG) == Set)
    {
        TMR2_ClrStatus(APP_TMR2_INT_UNIT, APP_TMR2_INT_CH, APP_TMR2_INT_FLAG);

        /* Reload initial value. */
        TMR2_Stop(APP_L_TMR2_UNIT, APP_L_TMR2_CH);
        TMR2_SetCntVal(APP_L_TMR2_UNIT, APP_L_TMR2_CH, APP_L_TMR2_INIT_CNT_VAL);
        TMR2_Start(APP_L_TMR2_UNIT, APP_L_TMR2_CH);

        INDICATE_OUT_TOGGLE();
    }
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
