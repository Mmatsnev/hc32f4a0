/**
 *******************************************************************************
 * @file  keyscan/keyscan_matrix/source/main.c
 * @brief Main program keyscan for the Device Driver Library.
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
 * @addtogroup Keyscan
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
#define KEYSCAN_ROW0_EXINT      (EXINT_CH08)
#define KEYSCAN_ROW1_EXINT      (EXINT_CH03)
#define KEYSCAN_ROW2_EXINT      (EXINT_CH07)

#define KEYSCAN_ROW0_IRQn       (Int035_IRQn)
#define KEYSCAN_ROW1_IRQn       (Int036_IRQn)
#define KEYSCAN_ROW2_IRQn       (Int037_IRQn)

#define KEYSCAN_ROW0_INT_SRC    (INT_PORT_EIRQ8)
#define KEYSCAN_ROW1_INT_SRC    (INT_PORT_EIRQ3)
#define KEYSCAN_ROW2_INT_SRC    (INT_PORT_EIRQ7)

#define KEYOUT0_PORT            (GPIO_PORT_C)
#define KEYOUT0_PIN             (GPIO_PIN_11)
#define KEYOUT1_PORT            (GPIO_PORT_C)
#define KEYOUT1_PIN             (GPIO_PIN_08)
#define KEYOUT2_PORT            (GPIO_PORT_A)
#define KEYOUT2_PIN             (GPIO_PIN_06)

#define KEYIN0_PORT             (GPIO_PORT_I)
#define KEYIN0_PIN              (GPIO_PIN_08)
#define KEYIN1_PORT             (GPIO_PORT_I)
#define KEYIN1_PIN              (GPIO_PIN_03)
#define KEYIN2_PORT             (GPIO_PORT_H)
#define KEYIN2_PIN              (GPIO_PIN_07)

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

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
    PWC_Unlock(PWC_UNLOCK_CODE_0);
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
 * @brief  EXINT Ch.8 as KYESCAN row 0 callback function
 * @param  None
 * @retval None
 */
void KEYSCAN_ROW0_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEYSCAN_ROW0_EXINT))
    {
        printf("Key row 0, col %ld is pressed.\n", KEYSCAN_GetKeyoutIdx());
        /* clear int request flag */
        EXINT_ClrExIntSrc(KEYSCAN_ROW0_EXINT);
    }
}

/**
 * @brief  EXINT Ch.3 as KYESCAN row 1 callback function
 * @param  None
 * @retval None
 */
void KEYSCAN_ROW1_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEYSCAN_ROW1_EXINT))
    {
        printf("Key row 1, col %ld is pressed.\n", KEYSCAN_GetKeyoutIdx());
        /* clear int request flag */
        EXINT_ClrExIntSrc(KEYSCAN_ROW1_EXINT);
    }
}

/**
 * @brief  EXINT Ch.7 as KYESCAN row 2 callback function
 * @param  None
 * @retval None
 */
void KEYSCAN_ROW2_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEYSCAN_ROW2_EXINT))
    {
        printf("Key row 2, col %ld is pressed.\n", KEYSCAN_GetKeyoutIdx());
        /* clear int request flag */
        EXINT_ClrExIntSrc(KEYSCAN_ROW2_EXINT);
    }
}

/**
 * @brief  KEYSCAN row 0 initialize
 * @param  None
 * @retval None
 */
void KEYSCAN_ROW0_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEYIN0_PORT, KEYIN0_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEYSCAN_ROW0_EXINT;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEYSCAN_ROW0_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEYSCAN_ROW0_IRQn;
    stcIrqSignConfig.pfnCallback = &KEYSCAN_ROW0_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEYSCAN_ROW0_IRQn);
    NVIC_SetPriority(KEYSCAN_ROW0_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(KEYSCAN_ROW0_IRQn);
}

/**
 * @brief  KEYSCAN row 1 initialize
 * @param  None
 * @retval None
 */
void KEYSCAN_ROW1_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEYIN1_PORT, KEYIN1_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEYSCAN_ROW1_EXINT;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEYSCAN_ROW1_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEYSCAN_ROW1_IRQn;
    stcIrqSignConfig.pfnCallback = &KEYSCAN_ROW1_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEYSCAN_ROW1_IRQn);
    NVIC_SetPriority(KEYSCAN_ROW1_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(KEYSCAN_ROW1_IRQn);
}

/**
 * @brief  KEYSCAN row 2 initialize
 * @param  None
 * @retval None
 */
void KEYSCAN_ROW2_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEYIN2_PORT, KEYIN2_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEYSCAN_ROW2_EXINT;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEYSCAN_ROW2_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEYSCAN_ROW2_IRQn;
    stcIrqSignConfig.pfnCallback = &KEYSCAN_ROW2_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEYSCAN_ROW2_IRQn);
    NVIC_SetPriority(KEYSCAN_ROW2_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(KEYSCAN_ROW2_IRQn);
}

/**
 * @brief  KEYSCAN column initialize
 * @param  None
 * @retval None
 */
void KEYSCAN_COL_Init(void)
{
    stc_gpio_init_t stcGpioInit;
    stc_keyscan_init_t stcKeyscanInit;

    GPIO_StructInit(&stcGpioInit);
    KEYSCAN_StructInit(&stcKeyscanInit);

    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEYOUT0_PORT, KEYOUT0_PIN, &stcGpioInit);
    GPIO_SetFunc(KEYOUT0_PORT, KEYOUT0_PIN, GPIO_FUNC_8_KEYSCAN, PIN_SUBFUNC_DISABLE);

    GPIO_Init(KEYOUT1_PORT, KEYOUT1_PIN, &stcGpioInit);
    GPIO_SetFunc(KEYOUT1_PORT, KEYOUT1_PIN, GPIO_FUNC_8_KEYSCAN, PIN_SUBFUNC_DISABLE);

    GPIO_Init(KEYOUT2_PORT, KEYOUT2_PIN, &stcGpioInit);
    GPIO_SetFunc(KEYOUT2_PORT, KEYOUT2_PIN, GPIO_FUNC_8_KEYSCAN, PIN_SUBFUNC_DISABLE);

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_KEY, Enable);

    stcKeyscanInit.u32HizCycle = KEYSCAN_HIZ_CLC_512;
    stcKeyscanInit.u32LowCycle = KEYSCAN_LOW_CLC_512;
    stcKeyscanInit.u32KeyClk   = KEYSCAN_CLK_HCLK;
    stcKeyscanInit.u32KeyOut   = KEYSCAN_OUT_0T2;
    stcKeyscanInit.u32KeyIn    = (KEYSCAN_IN_8 | KEYSCAN_IN_3 | KEYSCAN_IN_7);

    KEYSCAN_Init(&stcKeyscanInit);

}

/**
 * @brief  Main function of keyscan project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Register write enable for some required peripherals. */
    Peripheral_WE();
    /* System clock init */
    BSP_CLK_Init();

    DDL_PrintfInit();

    KEYSCAN_ROW0_Init();
    KEYSCAN_ROW1_Init();
    KEYSCAN_ROW2_Init();

    KEYSCAN_COL_Init();
    /* Register write protected for some required peripherals. */
    Peripheral_WP();

    /* Clear all KEYIN interrupt flag before enable */
    EXINT_ClrExIntSrc(KEYSCAN_ROW0_EXINT);
    EXINT_ClrExIntSrc(KEYSCAN_ROW1_EXINT);
    EXINT_ClrExIntSrc(KEYSCAN_ROW2_EXINT);

    KEYSCAN_Cmd(Enable);
    /* Add your code here */
    while (1)
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
