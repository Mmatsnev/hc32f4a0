/**
 *******************************************************************************
 * @file  timer6/timer6_hw_code_cnt/source/main.c
 * @brief This example demonstrates Timer6 count for AB phase position coding
 *        function.
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
 * @addtogroup TMR6_hw_code_cnt
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define         TMR6_1_PWMA_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMA_PIN        (GPIO_PIN_09)

#define         TMR6_1_PWMB_PORT       (GPIO_PORT_B)
#define         TMR6_1_PWMB_PIN        (GPIO_PIN_08)

#define         TEST_IO_A_PORT         (GPIO_PORT_I)
#define         TEST_IO_B_PORT         (GPIO_PORT_I)

#define         TEST_IO_A_PIN          (GPIO_PIN_06)
#define         TEST_IO_B_PIN          (GPIO_PIN_05)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint32_t u32CaptureA;
__UNUSED __IO uint32_t u32Timer6Cnt0 = 0U;

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
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
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

static void GenClkIn(void)
{
    uint32_t i;

    uint8_t bAin[17U] = {0U, 1U, 1U, 0U, 0U, 1U, 1U, 0U, 0U, 0U, 0U, 1U, 1U, 0U, 0U, 1U, 1U};
    uint8_t bBin[17U] = {1U, 1U, 0U, 0U, 1U, 1U, 0U, 0U, 1U, 1U, 0U, 0U, 1U, 1U, 0U, 0U, 1U};


    for (i = 0UL; i < 17UL; i++)
    {
        u32Timer6Cnt0 = TMR6_GetCntReg(M4_TMR6_1);

        if(0U == bAin[i])
        {
            GPIO_ResetPins(TEST_IO_A_PORT, TEST_IO_A_PIN);
        }
        else
        {
            GPIO_SetPins(TEST_IO_A_PORT, TEST_IO_A_PIN);
        }

        if(0U == bBin[i])
        {
            GPIO_ResetPins(TEST_IO_B_PORT, TEST_IO_B_PIN);
        }
        else
        {
            GPIO_SetPins(TEST_IO_B_PORT, TEST_IO_B_PIN);
        }

        DDL_DelayMS(500UL);
    }

    u32Timer6Cnt0 = TMR6_GetCntReg(M4_TMR6_1);
}


/**
 * @brief  Main function of TIMER6 compare output mode project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_tmr6_basecnt_cfg_t         stcTIM6BaseCntCfg;
    stc_tmr6_port_input_cfg_t      stcTIM6PortInCfg;
    stc_gpio_init_t                stcGpioCfg;

    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    TMR6_BaseCntStructInit(&stcTIM6BaseCntCfg);
    TMR6_PortInputStructInit(&stcTIM6PortInCfg);

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR6_1, Enable);

    /* Timer6 PWM port configuration */
    GPIO_SetFunc(TMR6_1_PWMA_PORT, TMR6_1_PWMA_PIN, GPIO_FUNC_3_TIM61_PWMA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TMR6_1_PWMB_PORT, TMR6_1_PWMB_PIN, GPIO_FUNC_3_TIM61_PWMB, PIN_SUBFUNC_DISABLE);

    /* GPIO configurate */
    GPIO_StructInit(&stcGpioCfg);
    stcGpioCfg.u16PinDir = PIN_DIR_OUT;
    GPIO_Init(TEST_IO_A_PORT, TEST_IO_A_PIN, &stcGpioCfg);
    GPIO_Init(TEST_IO_B_PORT, TEST_IO_B_PIN, &stcGpioCfg);

    /* De-initialize */
    TMR6_DeInit(M4_TMR6_1);
    /* Timer6 general count function configuration */
    stcTIM6BaseCntCfg.u32CntMode = TMR6_MODE_SAWTOOTH;
    stcTIM6BaseCntCfg.u32CntDir = TMR6_CNT_INCREASE;
    stcTIM6BaseCntCfg.u32CntClkDiv = TMR6_CLK_PCLK0_DIV512;
    stcTIM6BaseCntCfg.u32CntStpAftOvf = TMR6_CNT_CONTINUOUS;
    TMR6_Init(M4_TMR6_1, &stcTIM6BaseCntCfg);

    /* Capture input port configuration */
    stcTIM6PortInCfg.u32PortMode = TMR6_PORT_CAPTURE_INPUT;
    stcTIM6PortInCfg.u32FilterSta = TMR6_PORT_INPUT_FILTER_OFF;
    stcTIM6PortInCfg.u32FltClk = TMR6_INPUT_FILTER_PCLK0_DIV1;
    TMR6_PortInputConfig(M4_TMR6_1,TMR6_IO_PWMA, &stcTIM6PortInCfg);

    TMR6_PortInputConfig(M4_TMR6_1,TMR6_IO_PWMB, &stcTIM6PortInCfg);

     while(1)
    {

        GPIO_ResetPins(TEST_IO_A_PORT, TEST_IO_A_PIN);
        GPIO_SetPins(TEST_IO_B_PORT, TEST_IO_B_PIN);

        DDL_DelayMS(1000UL);
        TMR6_HwIncreaseCondClr(M4_TMR6_1);
        TMR6_HwDecreaseCondClr(M4_TMR6_1);

        TMR6_HwIncreaseCondCmd(M4_TMR6_1, TMR6_HW_CNT_PWMBH_PWMARISING, Enable); /* PWMA Rising trigger when PWMB is high level */
        TMR6_HwDecreaseCondCmd(M4_TMR6_1, TMR6_HW_CNT_PWMBL_PWMARISING, Enable); /* PWMA Rising trigger when PWMB is low level */

        TMR6_CountCmd(M4_TMR6_1, Enable);

        GenClkIn();

        TMR6_CountCmd(M4_TMR6_1, Disable);
        TMR6_SetCntReg(M4_TMR6_1, 0UL);

        GPIO_ResetPins(TEST_IO_A_PORT, TEST_IO_A_PIN);
        GPIO_SetPins(TEST_IO_B_PORT, TEST_IO_B_PIN);

        DDL_DelayMS(1000UL);

        TMR6_HwIncreaseCondClr(M4_TMR6_1);
        TMR6_HwDecreaseCondClr(M4_TMR6_1);

        /* PWMA Rising trigger when PWMB is high level, PWMA falling trigger when PWMB is low level */
        TMR6_HwIncreaseCondCmd(M4_TMR6_1, TMR6_HW_CNT_PWMBH_PWMARISING | TMR6_HW_CNT_PWMBL_PWMAFAILLING, Enable);
        /* PWMB Rising trigger when PWMA is high level, PWMB falling trigger when PWMA is low level */
        TMR6_HwDecreaseCondCmd(M4_TMR6_1, TMR6_HW_CNT_PWMAH_PWMBRISING | TMR6_HW_CNT_PWMAL_PWMBFAILLING, Enable);

        TMR6_CountCmd(M4_TMR6_1, Enable);

        GenClkIn();

        TMR6_CountCmd(M4_TMR6_1, Disable);
        TMR6_SetCntReg(M4_TMR6_1, 0UL);
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
