/**
 *******************************************************************************
 * @file  pwc/pwc_pvd/source/main.c
 * @brief Main program of PWC for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
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
 * @addtogroup PWC_Pvd
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define PVD1_INT_SRC    (INT_PVD_PVD1)
#define PVD1_IRQn       (Int003_IRQn)

#define PVD2_INT_SRC    (INT_PVD_PVD2)
#define PVD2_IRQn       (Int002_IRQn)

#define DLY_MS          (500UL)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
//    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
//    EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
//    EFM_OTP_WP_Unlock();
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
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
//    SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
//    EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
//    EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  PVD1 IRQ callback function
 * @param  None
 * @retval None
 */
void PWC_PVD1_IrqCallback(void)
{
    uint8_t u8Cnt = 10U;
    do
    {
        BSP_LED_Toggle(LED_RED);
        DDL_DelayMS(DLY_MS);
    } while(--u8Cnt);

    PWC_PVD_ClearStatus();
}

/**
 * @brief  PVD2 IRQ callback function
 * @param  None
 * @retval None
 */
void PWC_PVD2_IrqCallback(void)
{
    uint8_t u8Cnt = 10U;
    do
    {
        BSP_LED_Toggle(LED_BLUE);
        DDL_DelayMS(DLY_MS);
    } while(--u8Cnt);

    PWC_PVD_ClearStatus();
}

/**
 * @brief  PVD interrupt initial
 * @param  None
 * @retval None
 */
void PVD_IntInit(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;

    /* Clear PVD flag */
    PWC_PVD_ClearStatus();

    /* Set PVD interrupt. */
    INTC_ShareIrqCmd(INT_PVD_PVD1, Enable);

    /* PVD1 IRQ sign-in */
    stcIrqSignConfig.enIntSrc = PVD1_INT_SRC;
    stcIrqSignConfig.enIRQn   = PVD1_IRQn;
    stcIrqSignConfig.pfnCallback = &PWC_PVD1_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* PVD2 IRQ sign-in */
    stcIrqSignConfig.enIntSrc = PVD2_INT_SRC;
    stcIrqSignConfig.enIRQn   = PVD2_IRQn;
    stcIrqSignConfig.pfnCallback = &PWC_PVD2_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* Enable interrupt. */
    NVIC_ClearPendingIRQ(PVD1_IRQn);
    NVIC_SetPriority(PVD1_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(PVD1_IRQn);

    /* Enable interrupt. */
    NVIC_ClearPendingIRQ(PVD2_IRQn);
    NVIC_SetPriority(PVD2_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(PVD2_IRQn);
}

en_result_t PVD_Init(void)
{
    en_result_t enRet;
    stc_pwc_pvd_config_t  stcPwcPvdConfig;

    PWC_PVD_StructInit(&stcPwcPvdConfig);
    /* Config PVD */
    /* PVD1: 2.8V */
    stcPwcPvdConfig.u8PvdVoltage  = PWC_PVD1_2V8_PVD2_2V9;
    stcPwcPvdConfig.u8PvdCmpOutEn = PWC_PVD_CMP_ON;
    stcPwcPvdConfig.u8PvdEn       = PWC_PVD_ON;
    stcPwcPvdConfig.u8PvdIntRstEn = PWC_PVD_IR_ON;
    stcPwcPvdConfig.u8PvdIntRstSel= PWC_PVD_INT;
    stcPwcPvdConfig.u8PvdNmiEn    = PWC_PVD_INT_MASK;
    stcPwcPvdConfig.u8FilterEn    = PWC_PVD_DF_OFF;
    stcPwcPvdConfig.u8FilterClk   = PWC_PVD_DFS_DIV1;
    enRet = PWC_PVD_Init(PWC_PVD_CH1, &stcPwcPvdConfig);
    if (Ok == enRet)
    {
        /* PVD2: 2.3V */
        stcPwcPvdConfig.u8PvdVoltage  = PWC_PVD1_2V1_PVD2_2V3;
        enRet = PWC_PVD_Init(PWC_PVD_CH2, &stcPwcPvdConfig);
    }
    return enRet;
}

/**
 * @brief  Main function of PVD project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Register write enable for some required peripherals. */
    Peripheral_WE();
    /* System Clock init */
    BSP_CLK_Init();
    /* Expand IO init */
    BSP_IO_Init();
    /* LED init */
    BSP_LED_Init();
    /* PVD init */
    PVD_Init();
    /* PVD interrut init */
    PVD_IntInit();
    /* Register write protected for some required peripherals. */
    Peripheral_WP();

    while(1)
    {
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
