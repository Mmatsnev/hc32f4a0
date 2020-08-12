/**
 *******************************************************************************
 * @file  ots/ots_base/source/main.c
 * @brief Main program OTS base for the Device Driver Library.
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
 * @addtogroup OTS_Base
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
 * This example is going to get the temperature of the chip inside.
 */

/*
 * Specifies a clock source for OTS in this example.
 * 'APP_OTS_CLK' can be defined as 'OTS_CLK_XTAL'(recommended) or 'OTS_CLK_HRC'. */
#define APP_OTS_CLK                         (OTS_CLK_XTAL)

/*
 * Function control of OTS.
 * Defines the following macro as non-zero to enable the corresponding function.
 *
 * 'APP_OTS_USE_DEFAULT_PARA': Use default parameters, not need scaling experiment.
 * 'APP_OTS_USE_INTERRUPT': Interrupt function control.
 * 'APP_OTS_USE_TRIG': Hardware trigger conditions control. The condition that used to start OTS.
 */
#define APP_OTS_USE_DEFAULT_PARA            (1U)
#define APP_OTS_USE_TRIG                    (1U)

#if (APP_OTS_USE_TRIG > 0U)
    #define APP_OTS_USE_INTERRUPT           (APP_OTS_USE_TRIG)
#else
    #define APP_OTS_USE_INTERRUPT           (1U)
#endif

/*
 * Definitions about OTS interrupt for the example.
 * OTS independent IRQn: [Int000_IRQn, Int031_IRQn], [Int116_IRQn, Int121_IRQn].
 */
#if (APP_OTS_USE_INTERRUPT > 0U)
    #define APP_OTS_INT_PRIO                (DDL_IRQ_PRIORITY_03)
    #define APP_OTS_INT_SRC                 (INT_OTS)
    #define APP_OTS_IRQn                    (Int116_IRQn)
#endif /* #if (APP_OTS_USE_INTERRUPT > 0U) */

/* OTS parameters, slope K and offset M. Different chip, different parameters. */
#if (APP_OTS_USE_DEFAULT_PARA == 0U)
    #define OTS_XTAL_K                      (527152.3178f)
    #define OTS_XTAL_M                      (-41.6212f)
    #define OTS_HRC_K                       (2159.5225f)
    #define OTS_HRC_M                       (-41.5189f)
#endif /* #if (APP_OTS_USE_DEFAULT_PARA == 0U) */

/* Timeout value. */
#define TIMEOUT_VAL                         (10000U)

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

static void OtsConfig(void);
static void OtsInitConfig(void);
static void OtsClkConfig(void);

#if (APP_OTS_USE_INTERRUPT > 0U)
    static void OtsIrqConfig(void);
    static void OTS_IrqCallback(void);
#endif

#if (APP_OTS_USE_TRIG > 0U)
    static void OtsTrigConfig(void);
#endif

#if ((APP_OTS_USE_INTERRUPT > 0U) || (APP_OTS_USE_TRIG > 0U))
    static void OtsStart(void);
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__UNUSED static float32_t m_f32Temperature;

#if (APP_OTS_USE_INTERRUPT > 0U)
static uint8_t m_u8OtsIntFlag = 0U;
#endif

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
    /* Configures OTS. */
    OtsConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

#if ((APP_OTS_USE_INTERRUPT > 0U) || (APP_OTS_USE_TRIG > 0U))
    /* Starts OTS. */
    OtsStart();
#endif

    /***************** Configuration end, application start **************/

    while (1U)
    {
#if (APP_OTS_USE_INTERRUPT > 0U)
        if (m_u8OtsIntFlag != 0U)
        {
            m_u8OtsIntFlag = 0U;
            DBG("Temperature: %.2f\n", m_f32Temperature);
#if (APP_OTS_USE_TRIG == 0U)
            DDL_DelayMS(1000U);
            OTS_Start();
#endif
        }
#else
        OTS_Polling(&m_f32Temperature, TIMEOUT_VAL);
        DBG("Temperature: %.2f\n", m_f32Temperature);
        m_f32Temperature = 0.0f;
        DDL_DelayMS(1000U);
#endif
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
    PWC_FCG0_Unlock();
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
    PWC_FCG0_Lock();
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

/**
 * @brief  OTS configuration.
 * @param  None
 * @retval None
 */
static void OtsConfig(void)
{
    OtsInitConfig();
    OtsClkConfig();
}

/**
 * @brief  OTS initialization configuration.
 * @param  None
 * @retval None
 */
static void OtsInitConfig(void)
{
    stc_ots_init_t stcInit;

    OTS_StructInit(&stcInit);
    stcInit.u16ClkSrc = APP_OTS_CLK;
#if (APP_OTS_USE_DEFAULT_PARA == 0U)
    #if (APP_OTS_CLK == OTS_CLK_XTAL)
        stcInit.f32SlopeK  = OTS_XTAL_K;
        stcInit.f32OffsetM = OTS_XTAL_M;
    #else
        stcInit.f32SlopeK  = OTS_HRC_K;
        stcInit.f32OffsetM = OTS_HRC_M;
    #endif /* #if (APP_OTS_CLK == OTS_CLK_XTAL) */
#else
	#if (APP_OTS_CLK == OTS_CLK_XTAL)
        /* Real XTAL frequency(MHz). */
        stcInit.stcParaCond.u16ClkFreq = 8U;
    #else
        /* Real HRC frequency(MHz). */
        stcInit.stcParaCond.u16ClkFreq = 16U;
    #endif /* #if (APP_OTS_CLK == OTS_CLK_XTAL) */
#endif /* #if (APP_OTS_USE_DEFAULT_PARA == 0U) */

    /* 1. Enable OTS peripheral clock. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_OTS, Enable);

    /* 2. Initialize OTS. */
    OTS_Init(&stcInit);

#if (APP_OTS_USE_INTERRUPT > 0U)
    OtsIrqConfig();
#endif

#if (APP_OTS_USE_TRIG > 0U)
    OtsTrigConfig();
#endif
}

/**
 * @brief  OTS clock configuration.
 * @param  None
 * @retval None
 */
static void OtsClkConfig(void)
{
#if (APP_OTS_CLK == OTS_CLK_HRC)
    /* 1. Enable HRC for OTS. */
    CLK_HrcCmd(Enable);
    /* 2. Enable XTAL32 when clock source of OTS is HRC. */
    CLK_Xtal32Cmd(Enable);
#else
    /* Enable XTAL for OTS. */
    CLK_XtalCmd(Enable);
#endif

    /* Enable LRC for OTS. */
    CLK_LrcCmd(Enable);
}

#if (APP_OTS_USE_INTERRUPT > 0U)
/**
 * @brief  OTS interrupt configuration.
 * @param  None
 * @retval None
 */
static void OtsIrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = APP_OTS_INT_SRC;
    stcCfg.enIRQn      = APP_OTS_IRQn;
    stcCfg.pfnCallback = &OTS_IrqCallback;

    /* Independent interrupt. */
    INTC_IrqSignIn(&stcCfg);
    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_OTS_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of OTS. */
    OTS_IntCmd(Enable);
}

/**
 * @brief  OTS interrupt callback function.
 * @param  None
 * @retval None
 */
void OTS_IrqCallback(void)
{
    m_f32Temperature = OTS_CalculateTemp();
    m_u8OtsIntFlag = 1U;
}
#endif /* #if (APP_OTS_USE_INTERRUPT > 0U) */

#if (APP_OTS_USE_TRIG > 0U)
/**
 * @brief  Specifies event 'EVT_TMR2_1_CMPA' of TIMER2 uint 1 channel A as the trigger source event of OTS. \
 *         Event 'EVT_TMR2_1_CMPA' occurs each second.
 * @param  None
 * @retval None
 */
static void OtsTrigConfig(void)
{
    /*
     * If a peripheral is used to generate the event which is used as a start trigger condition of OTS, \
     *   call the API of the peripheral to configure the peripheral.
     * The following operations are only used in this example.
     */

    stc_tmr2_init_t stcInit;

    /* Initials TIMER2. */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR2_1, Enable);
    TMR2_StructInit(&stcInit);
    stcInit.u32ClkSrc = TMR2_CLK_SYNC_PCLK1;
    stcInit.u32ClkDiv = TMR2_CLK_DIV256;
    stcInit.u32CmpVal = 31250UL;
    TMR2_Init(M4_TMR2_1, TMR2_CH_A, &stcInit);

    /* Specifies event 'EVT_TMR2_1_CMPA' as the trigger source event of OTS. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    OTS_SetTriggerSrc(EVT_TMR2_1_CMPA);
}

#endif /* #if (APP_OTS_USE_TRIG > 0U) */

#if ((APP_OTS_USE_INTERRUPT > 0U) || (APP_OTS_USE_TRIG > 0U))
/**
 * @brief  Start OTS.
 * @param  None
 * @retval None
 */
static void OtsStart(void)
{
    /*
     * If a peripheral is used to generate the event which is used as a start trigger condition of OTS, \
     *   call the API of the peripheral to start the peripheral here or anywhere else you need.
     * The following operations are only used in this example.
     */

#if (APP_OTS_USE_TRIG > 0U)
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);
#elif (APP_OTS_USE_INTERRUPT > 0U)
    OTS_Start();
#endif
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
