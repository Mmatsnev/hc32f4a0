/**
 *******************************************************************************
 * @file  timer6/timer6_cmp_triangular_buf/source/main.c
 * @brief This example demonstrates Timer6 compare output with single buffer.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin          First version
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
 * @addtogroup TMR6_Cmp_Triangular_Buf
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define         TMR6_1_PWMA_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMB_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMA_PIN        (GPIO_PIN_09)
#define         TMR6_1_PWMB_PIN        (GPIO_PIN_08)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    //EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static __attribute__((unused)) void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  TIMER6 underflow interrupt handler callback.
 * @param  None
 * @retval None
 */
void Tmr6_UnderFlow_CallBack(void)
{
    static uint8_t i = 1U;

    if( 0U == i)
    {
        TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, 0x3000U);
        TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_D, 0x6000U);
        i = 1U;
    }
    else
    {
        TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, 0x6000U);
        TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_D, 0x3000U);
        i = 0U;
    }
}

/**
 * @brief  Main function of TIMER6 compare output mode project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t                       u32Period;
    uint32_t                       u32Compare;
    stc_tmr6_basecnt_cfg_t         stcTIM6BaseCntCfg;
    stc_tmr6_port_output_cfg_t     stcTIM6PortOutCfg;
    stc_gpio_init_t                stcGpioInit;
    stc_irq_signin_config_t        stcIrqRegiConf;
    stc_tmr6_buf_func_cfg_t        stcBufCfg;

    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    TMR6_BaseCntStructInit(&stcTIM6BaseCntCfg);
    TMR6_PortOutputStructInit(&stcTIM6PortOutCfg);
    GPIO_StructInit(&stcGpioInit);
    TMR6_BufFuncStructInit(&stcBufCfg);

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);

    /* Timer6 PWM output port configuration */
    GPIO_SetFunc(TMR6_1_PWMA_PORT, TMR6_1_PWMA_PIN, GPIO_FUNC_3_TIM61_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_1_PWMB_PORT, TMR6_1_PWMB_PIN, GPIO_FUNC_3_TIM61_PWMB, PIN_SUBFUNC_DISABLE);

    TMR6_DeInit(M4_TMR6_1);
    /* Timer6 general count function configuration */
    stcTIM6BaseCntCfg.u32CntMode = TMR6_MODE_TRIANGLE;
    stcTIM6BaseCntCfg.u32CntDir = TMR6_CNT_INCREASE;
    stcTIM6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV512;
    stcTIM6BaseCntCfg.u32CntStpAftOvf = TMR6_CNT_CONTINUOUS;
    TMR6_Init(M4_TMR6_1, &stcTIM6BaseCntCfg);

    /* Period register set */
    u32Period = 0x8340U;
    TMR6_SetPeriodReg(M4_TMR6_1, TMR6_PERIOD_REG_A, u32Period);

    /* Set General Compare RegisterA Value */
    u32Compare = 0x3000U;
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_A, u32Compare); /* General comprare register A */
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_C, u32Compare); /* General comprare register C, as buffer register for GCMAR */

    /* Set General Compare RegisterB Value */
    u32Compare = 0x6000U;
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_B, u32Compare); /* General comprare register B */
    TMR6_SetGenCmpReg(M4_TMR6_1, TMR6_CMP_REG_D, u32Compare); /* General comprare register D, as buffer register for GCMBR */

    /* General compare buffer function configurate */
    stcBufCfg.u32BufFunCmd = TMR6_BUF_FUNC_ON;
    stcBufCfg.u32BufNum = TMR6_BUF_FUNC_SINGLE;
    stcBufCfg.u32BufTransTim = TMR6_BUF_TRANS_TIM_OVERFLOW;
    TMR6_GenCmpBufCfg(M4_TMR6_1, TMR6_CH_A, &stcBufCfg);
    TMR6_GenCmpBufCfg(M4_TMR6_1, TMR6_CH_B, &stcBufCfg);

     /* Configurate PWM output */
    stcTIM6PortOutCfg.u32PortMode = TMR6_PORT_COMPARE_OUTPUT;
    stcTIM6PortOutCfg.u32NextPeriodForceStd = TMR6_FORCE_PORT_OUTPUT_INVALID;
    stcTIM6PortOutCfg.u32DownCntMatchAnotherCmpRegStd = TMR6_PORT_OUTPUT_STD_HOLD;
    stcTIM6PortOutCfg.u32UpCntMatchAnotherCmpRegStd = TMR6_PORT_OUTPUT_STD_HOLD;
    stcTIM6PortOutCfg.u32DownCntMatchCmpRegStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTIM6PortOutCfg.u32UpCntMatchCmpRegStd = TMR6_PORT_OUTPUT_STD_HIGH;
    stcTIM6PortOutCfg.u32UnderflowStd = TMR6_PORT_OUTPUT_STD_HOLD;
    stcTIM6PortOutCfg.u32OverflowStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTIM6PortOutCfg.u32StopStd = TMR6_PORT_OUTPUT_STD_LOW;
    stcTIM6PortOutCfg.u32StartStd = TMR6_PORT_OUTPUT_STD_LOW;
    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMA, &stcTIM6PortOutCfg);

    TMR6_PortOutputConfig(M4_TMR6_1, TMR6_IO_PWMB, &stcTIM6PortOutCfg);

    /* Enable interrupt */
    TMR6_IntCmd(M4_TMR6_1, TMR6_IRQ_EN_UNDERFLOW, Enable);

    stcIrqRegiConf.enIRQn = Int002_IRQn;                    /* Register INT_TMR6_1_GUDF Int to Vect.No.002 */
    stcIrqRegiConf.enIntSrc = INT_TMR6_1_GUDF;               /* Select Event interrupt of M4_TMR6_1 */
    stcIrqRegiConf.pfnCallback = &Tmr6_UnderFlow_CallBack;  /* Callback function */
    INTC_IrqSignIn(&stcIrqRegiConf);                        /* Registration IRQ */

    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);            /* Clear Pending */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);/* Set priority */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);                  /* Enable NVIC */

    /* Start timer6 */
    TMR6_CountCmd(M4_TMR6_1, Enable);

    while(1)
    {
        ;
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
