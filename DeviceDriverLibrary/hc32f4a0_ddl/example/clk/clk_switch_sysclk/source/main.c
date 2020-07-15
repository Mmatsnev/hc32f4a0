/**
 *******************************************************************************
 * @file  clk/clk_switch_sysclk/source/main.c
 * @brief Main program of CLK for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
   2020-07-03       Zhangxl         Delay ms before HCLK switches to XTAL32
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
 * @addtogroup CLK_Switch_sysclk
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define MCO_PORT        (GPIO_PORT_A)
#define MCO_PIN         (GPIO_PIN_08)

#define DLY_MS          (100UL)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_result_t XtalInit(void);
static en_result_t Xtal32Init(void);
static en_result_t PLLHInit(void);
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
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
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
//    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
//    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
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
 * @brief  Xtal initialize
 * @param  None
 * @retval None
 */
static en_result_t XtalInit(void)
{
    stc_clk_xtal_init_t stcXtalInit;

    /* Xtal config */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    return CLK_XtalInit(&stcXtalInit);
}

/**
 * @brief  Xtal32 initialize
 * @param  None
 * @retval None
 */
static en_result_t Xtal32Init(void)
{
    stc_clk_xtal32_init_t stcXtal32Init;

    /* Xtal32 config */
    CLK_Xtal32StrucInit(&stcXtal32Init);
    stcXtal32Init.u8Xtal32State = CLK_XTAL32_OFF;
    stcXtal32Init.u8Xtal32Drv   = CLK_XTAL32DRV_MID;
    stcXtal32Init.u8Xtal32NF    = CLK_XTAL32NF_FULL;
    return CLK_Xtal32Init(&stcXtal32Init);
}

/**
 * @brief  PLLH initialize
 * @param  None
 * @retval None
 */
static en_result_t PLLHInit(void)
{
    stc_clk_pllh_init_t stcPLLHInit;

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EX BUS Max 120MHz */
    CLK_ClkDiv(CLK_CATE_ALL,                                                    \
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 |              \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 |              \
                CLK_HCLK_DIV1));

    /* Highspeed SRAM set to 1 Read/Write wait cycle */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);

    /* SRAM1_2_3_4_backup set to 2 Read/Write wait cycle */
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);

    /* PLLH config */
    CLK_PLLHStrucInit(&stcPLLHInit);
    /*
        8MHz/M*N = 8/1*120/4 =240MHz
    */
    stcPLLHInit.u8PLLState = CLK_PLLH_OFF;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = (1UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLN = (120UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLR = (4UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLQ = (4UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLP = (4UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL;     /* Xtal = 8MHz */
    return CLK_PLLHInit(&stcPLLHInit);
}

/**
 * @brief  Main function of CLK project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Register write unprotected for some required peripherals. */
    Peripheral_WE();
    /* Expand IO init */
    BSP_IO_Init();
    /* Matrix key init */
    BSP_KEY_Init();
    /* LED init */
    BSP_LED_Init();

    /* Configure clock output system clock */
    CLK_MCO1Config(CLK_MCOSOURCCE_SYSCLK, CLK_MCO_DIV128);
    /* Configure clock output pin */
    GPIO_SetFunc(MCO_PORT, MCO_PIN, GPIO_FUNC_1_MCO, PIN_SUBFUNC_DISABLE);
    /* MCO1 output enable */
    CLK_MCO1Cmd(Enable);

    XtalInit();

    Xtal32Init();

    PLLHInit();
    /* Register write protected for some required peripherals. */
    Peripheral_WP();

    while(1)
    {
        /* MRC output */
        if (Set == BSP_KEY_GetStatus(BSP_KEY_1))
        {
            CLK_MrcCmd(Enable);
            CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_MRC);
        }
        /* HRC output */
        if (Set == BSP_KEY_GetStatus(BSP_KEY_2))
        {
            CLK_HrcCmd(Enable);
            CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_HRC);
        }
        /* LRC output */
        if (Set == BSP_KEY_GetStatus(BSP_KEY_3))
        {
            CLK_LrcCmd(Enable);
            CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_LRC);
        }
        /* Xtal output */
        if (Set == BSP_KEY_GetStatus(BSP_KEY_4))
        {
            CLK_XtalCmd(Enable);
            CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_XTAL);
        }
        /* Xtal32 output */
        if (Set == BSP_KEY_GetStatus(BSP_KEY_5))
        {
            CLK_Xtal32Cmd(Enable);
            DDL_DelayMS(DLY_MS);
            CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_XTAL32);
        }
        /* PLLH output */
        if (Set == BSP_KEY_GetStatus(BSP_KEY_6))
        {
            CLK_PLLHCmd(Enable);
            CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_7))
        {
            BSP_LED_Toggle(LED_BLUE);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_8))
        {
            BSP_LED_Toggle(LED_YELLOW);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_9))
        {
            BSP_LED_Toggle(LED_RED);
        }
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
