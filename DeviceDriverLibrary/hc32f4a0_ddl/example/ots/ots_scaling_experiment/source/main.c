/**
 *******************************************************************************
 * @file  ots/ots_scaling_experiment/source/main.c
 * @brief Main program OTS scaling experiment for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-08-11       Wuze            First version
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
 * @addtogroup OTS_Scaling_Experiment
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* This example will show how to do scaling experiment */

/* Average count. */
#define OTS_AVG_COUNT                       (10U)

/* Timeout value. */
#define OTS_TIMEOUT                         (1000UL)

/*
 * External interrupt pin definition. See \example\exint\exint_key\source\main.c for details.
 * Press SW10 on the board to trigger scaling experiment start.
 */
#define KEY_PORT                            (GPIO_PORT_A)
#define KEY_PIN                             (GPIO_PIN_00)
#define KEY_EXINT_CH                        (EXINT_CH00)
#define KEY_INT_SRC                         (INT_PORT_EIRQ0)
#define KEY_IRQn                            (Int025_IRQn)
#define KEY_INT_PRIO                        (DDL_IRQ_PRIORITY_04)

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
/* static void Peripheral_WP(void); */

static void KeyConfig(void);
static void EXINT_IrqCallback(void);

static void OtsHrcConfig(void);
static void OtsXtalConfig(void);
static void OtsScalingExperiment(char *strClkSrc);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__IO static uint8_t m_u8KeyPressed = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Main function of ots_base project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* The system clock is MRC(8MHz) by default. */

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Press SW10 on the board to trigger scaling experiment start. */
    KeyConfig();

    /***************** Configuration end, application start **************/

    while (1U)
    {
        DBG("---> Press SW10 to start.\n");
        while (m_u8KeyPressed == 0U)
        {
            ;
        }
        m_u8KeyPressed = 0U;

        /* 1. HRC 16MHz. */
        OTS_DeInit();
        OtsHrcConfig();
        OtsScalingExperiment("HRC 16MHz");

        /* 2. Change clock to XTAL 8MHz. */
        OTS_DeInit();
        OtsXtalConfig();
        OtsScalingExperiment("XTAL 8MHz");
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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    // PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, LVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0);
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

#if 0
/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    // PWC_FCG0_Lock();
    /* Lock PWC, CLK, LVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
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
#endif

/**
 * @brief  Control key configuration.
 * @param  None
 * @retval None
 */
static void KeyConfig(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
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

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY_IRQn;
    stcIrqSignConfig.pfnCallback = &EXINT_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEY_IRQn);
    NVIC_SetPriority(KEY_IRQn, KEY_INT_PRIO);
    NVIC_EnableIRQ(KEY_IRQn);
}

/**
 * @brief  Set HRC as the clock source for OTS.
 * @param  None
 * @retval None
 */
static void OtsHrcConfig(void)
{
    stc_ots_init_t stcInit;

    OTS_StructInit(&stcInit);
    stcInit.u16ClkSrc = OTS_CLK_HRC;
    /* Initials OTS. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_OTS, Enable);
    OTS_Init(&stcInit);

    /* Clock configuration. */
    CLK_HrcCmd(Enable);
    CLK_Xtal32Cmd(Enable);
    CLK_LrcCmd(Enable);
}

/**
 * @brief  Set XTAL as the clock source for OTS.
 * @param  None
 * @retval None
 */
static void OtsXtalConfig(void)
{
    stc_ots_init_t stcInit;

    OTS_StructInit(&stcInit);
    stcInit.u16ClkSrc = OTS_CLK_XTAL;
    /* Initials OTS. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_OTS, Enable);
    OTS_Init(&stcInit);

    /* Clock configuration. */
    CLK_XtalCmd(Enable);
    CLK_LrcCmd(Enable);
}

/**
 * @brief  OTS scaling experiment entity.
 * @param  [in]  strClkSrc              String of OTS clock source information.
 * @retval None
 */
static void OtsScalingExperiment(char *strClkSrc)
{
    uint32_t i;
    uint16_t u16Dr1;
    uint16_t u16Dr2;
    uint16_t u16Ecr;

    float32_t f32A;
    float32_t f32SumA = 0.0f;
    DBG("---> Clock source is %s.\n", strClkSrc);
    for (i=0U; i<OTS_AVG_COUNT; i++)
    {
        DDL_DelayMS(100U);

        if (OTS_ScalingExperiment(&u16Dr1, &u16Dr2, &u16Ecr, &f32A, OTS_TIMEOUT) == Ok)
        {
            DBG("DR1 = %d, DR2 = %d, ECR = %d, A = %f\n",
                u16Dr1, u16Dr2, u16Ecr, f32A);

            f32SumA += f32A;
        }
        else
        {
            DBG("OTS fault -- timeout.\n");
            while (1U);
        }
    }

    DBG("%s: SUM A = %f, AVG A = %f\n", strClkSrc, f32SumA, f32SumA/(float32_t)OTS_AVG_COUNT);
}

/**
 * @brief  External interrupt callback.
 * @param  None
 * @retval None
 */
static void EXINT_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEY_EXINT_CH))
    {
        /* Wait for key release. */
        while (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
        {
            ;
        }
        m_u8KeyPressed = 1U;
        EXINT_ClrExIntSrc(KEY_EXINT_CH);
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
