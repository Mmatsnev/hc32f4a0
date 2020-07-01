/**
 *******************************************************************************
 * @file  timer0/timer0_capture/source/main.c
 * @brief Main program of TIMER0 for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb          First version
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
 * @addtogroup TMR0_Capture
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* TMR0 interrupt source and number define */
#define TMR0_IRQn                      (Int014_IRQn)
#define TMR0_SOURCE                    (INT_TMR0_2_CMPB)
/* EIRQ0 Port and Pin define */
#define KEY10_PORT                     (GPIO_PORT_A)
#define KEY10_PIN                      (GPIO_PIN_00)
#define KEY10_EXINT_CH                 (EXINT_CH00)

#define TMR0x                          (M4_TMR0_2)
#define TMR0_CH_x                      (TMR0_CH_B)
/* TMR0 Clock source define*/
/* If select XTAL32 or LRC, Please make sure they are on  */
#define TMR0_CLKSRC_PCLK               (0U)
#define TMR0_CLKSRC_XTAL32             (1U)
#define TMR0_CLKSRC_LRC                (2U)
#define TMR0_CLKSRC                    (TMR0_CLKSRC_PCLK)

#if (TMR0_CLKSRC == TMR0_CLKSRC_PCLK)
    #define TMR0_CLK_SRC               (TMR0_CLK_SRC_PCLK1)
    #define TMR0_CLK_DIV               (TMR0_CLK_DIV1024)
    #define TMR0_CMP_VAL               (0xFFFFU)

#elif (TMR0_CLKSRC == TMR0_CLKSRC_LRC)
    #define TMR0_CLK_SRC               (TMR0_CLK_SRC_LRC)
    #define TMR0_CLK_DIV               (TMR0_CLK_DIV16)
    #define TMR0_CMP_VAL               (0xFFFFU)

#elif (TMR0_CLKSRC == TMR0_CLKSRC_XTAL32)
    #define TMR0_CLK_SRC               (TMR0_CLK_SRC_XTAL32)
    #define TMR0_CLK_DIV               (TMR0_CLK_DIV16)
    #define TMR0_CMP_VAL               (0xFFFFU)
#endif

#define CAPTURE_CNT_MAX                (2U)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void TMR0_Config(void);
static void EXINT_IRQ_Config(void);
static void Peripheral_WE(void);
static void Peripheral_WP(void);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__IO static uint16_t CaptureData[2] = {0U};
__IO static uint8_t CaptureCnt = 0U;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
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
    /* Unlock PWC registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0);
    /* Unlock SRAM register: WTCR */
    //SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    //EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OPT write protect registers */
    //EFM_OTP_WP_Unlock();
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
    /* Lock PWC registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
    //SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OPT write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    //EFM_Lock();
}
/**
 * @brief  TMR0_2 channelB capture IRQ callback
 * @param  None
 * @retval None
 */
static void TMR0_2_ChBCap_IrqCallback(void)
{       
    if(CaptureCnt != CAPTURE_CNT_MAX)
    {
        CaptureData[CaptureCnt] = TMR0_GetCmpVal(TMR0x, TMR0_CH_x);
        CaptureCnt++;
    }
    else
    {
        /* Finish timer0 capture */
        TMR0_Cmd(TMR0x, TMR0_CH_x, Disable);
        /* Timer0 interrupt function Disable */
        TMR0_IntCmd(TMR0x, TMR0_CH_x, Disable);
    }
    TMR0_ClearStatus(TMR0x, TMR0_CH_x);
}

/**
 * @brief  Main function of example project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Unlock peripherals or registers */
    Peripheral_WE();
    /* Expand IO init */
    BSP_IO_Init();
    /* LED init */
    BSP_LED_Init();
    /* Enable AOS function */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    /*Configure EXINT and IRQ handler && NVIC*/
    EXINT_IRQ_Config();

    /*Configure TMR0*/
    TMR0_Config();
    /* Timer0 ch1 start counting */
    TMR0_Cmd(TMR0x, TMR0_CH_x, Enable);

    while(CAPTURE_CNT_MAX > CaptureCnt)
    {
        ;
    }
    /* Turn on LED_B */
    BSP_LED_On(LED_BLUE);
    /* Lock peripherals or registers */
    Peripheral_WP();
    while(1)
    {
        ;
    }
}


/**
 * @brief  Configure TMR0.
 * @param  None
 * @retval None
 */
static void TMR0_Config(void)
{
    stc_tmr0_init_t stcTmr0Init;

    /* Enable timer0 peripheral clock */
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR0_2, Enable);

    /* TIMER0 basetimer function initialize */
    TMR0_StructInit(&stcTmr0Init);
    stcTmr0Init.u32ClockDivision = TMR0_CLK_DIV;        /* Config clock division */
    stcTmr0Init.u32ClockSource = TMR0_CLK_SRC;          /* Chose clock source */
    stcTmr0Init.u32Tmr0Func = TMR0_FUNC_CAP;            /* Timer0 compare mode */
    stcTmr0Init.u32HwTrigFunc = TMR0_BT_HWTRG_FUNC_NONE;/* Config Hardware trigger function */
    stcTmr0Init.u16CmpValue = TMR0_CMP_VAL;             /* Set compara register data */
    TMR0_Init(TMR0x, TMR0_CH_x, &stcTmr0Init);
    /* In asynchronous clock, If you want to write a TMR0 register, you need to wait for at 
       least 3 asynchronous clock cycles after the last write operation! */
    DDL_DelayMS(1U); /* Wait at least 3 asynchronous clock cycles.*/
    /* Timer0 interrupt function Enable */
    TMR0_IntCmd(TMR0x, TMR0_CH_x, Enable);
    DDL_DelayMS(1U); /* Wait at least 3 asynchronous clock cycles.*/
    /* Set internal hardware capture source */
    TMR0_SetTriggerSrc(EVT_PORT_EIRQ0);

}
/**
 * @brief  Configure EXINT and IRQ handler && NVIC.
 * @param  None
 * @retval None
 */
static void EXINT_IRQ_Config(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEY10_PORT, KEY10_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEY10_EXINT_CH;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_BOTH;
    EXINT_Init(&stcExintInit);

    /* Register IRQ handler && configure NVIC. */
    stcIrqSignConfig.enIRQn = TMR0_IRQn;
    stcIrqSignConfig.enIntSrc = TMR0_SOURCE;
    stcIrqSignConfig.pfnCallback = &TMR0_2_ChBCap_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIORITY_15);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
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
