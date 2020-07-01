/**
 *******************************************************************************
 * @file  rmu/rmu_reset/source/main.c
 * @brief Main program of RMU for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb         First version
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
 * @addtogroup RMU_Reset
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Test reset mode enum*/
#define RESET_WDT                       (0U)
#define RESET_SWDT                      (1U)
#define RESET_XTALERR                   (2U)
#define RESET_SOFTWARERST               (3U)
#define RESET_PVD                       (4U)
#define RESET_CLKFREQERR                (5U)

/* Modify for test reset mode */
#define TEST_RESET_MODE                 (RESET_PVD)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void PrintResetMode(void);
static void MakeReset(void);
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
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
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
    //GPIO_Lock();
    /* Lock PWC register: FCG0 */
    //PWC_FCG0_Lock();
    /* Lock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    //PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
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
 * @brief  Main function of RMU project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Unlock peripherals or registers */
    Peripheral_WE();
    /* Configure system clock. HClK = 240MHZ */
    BSP_CLK_Init();
    /* configuration uart for debug information */
    DDL_PrintfInit();
    /* KEY initialize*/
    BSP_IO_Init();
    BSP_KEY_Init();
    /* Enable CPU lockup reset */
    RMU_CPULockUpCmd(Enable);

    /* Printf reset cause */
    PrintResetMode();

    printf("\nPress SW1 to config reset condition.\n");
    /* Wait short press KEY_1(SW1) */
    while(Reset == BSP_KEY_GetStatus(BSP_KEY_1))
    {
        ;
    }
    /* Clear reset cause */
    RMU_ClrStatus();
    MakeReset();
    /* Lock peripherals or registers */
    Peripheral_WP();
    /* Reset condition configuration finished */
    while(1U)
    {
        ;
    }
}


/**
 * @brief  Print reset information.
 * @param  None
 * @retval None
 */
static void PrintResetMode(void)
{
    if(Set == RMU_GetStatus(RMU_RST_XTAL_ERR))
    {
        printf("XTAL error reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_LOCKUP))
    {
        printf("M4 CPU lock reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_CLK_ERR))
    {
        printf("Clock freqence error reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_RAM_ECC))
    {
        printf("Ram ECC reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_RAM_PARITY_ERR))
    {
        printf("RAM parity error reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_MPU_ERR))
    {
        printf("Mpu error reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_SOFTWARE))
    {
        printf("Software reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_POWER_DOWN))
    {
        printf("Power down reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_SWDT))
    {
        printf("Special watchdog timer reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_WDT))
    {
        printf("Watchdog timer reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_PVD2))
    {
        printf("Program voltage Dectection 2 reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_PVD1))
    {
        printf("Program voltage Dectection 1 reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_BROWN_OUT))
    {
        printf("Brown-out reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_RESET_PIN))
    {
        printf("Reset pin reset.\r\n");
    }
    if(Set == RMU_GetStatus(RMU_RST_POWER_ON))
    {
        printf("Power on reset.\r\n");
    }
}

/**
 * @brief  Make reset condition for test
 * @param  [in]  None
 * @retval None
 */
static void MakeReset(void)
{
#if (TEST_RESET_MODE == RESET_XTALERR)
    /* Config XTAL status detect error reset on*/
    stc_clk_xtalstd_init_t stcXtalStdCfg;
    stc_clk_xtal_init_t    stcXtalInit;

    /* Config XTAL and Enable */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);

    stcXtalStdCfg.u8XtalStdState = CLK_XTALSTD_ON;
    stcXtalStdCfg.u8XtalStdMode  = CLK_XTALSTD_MODE_RST;
    stcXtalStdCfg.u8XtalStdInt   = CLK_XTALSTD_INT_OFF;
    stcXtalStdCfg.u8XtalStdRst   = CLK_XTALSTD_RST_ON;
    CLK_XtalStdInit(&stcXtalStdCfg);

#elif (TEST_RESET_MODE == RESET_SOFTWARERST)
    /* Software reset MCU */
    NVIC_SystemReset();

#elif (TEST_RESET_MODE == RESET_SWDT)
    /* Configuration  Special Watchdog function */
    stc_swdt_init_t stcSwdtInit;
    /* SWDT configure */
    stcSwdtInit.u32CountCycle    = SWDT_COUNTER_CYCLE_256;
    stcSwdtInit.u32ClockDivision = SWDT_CLOCK_DIV64;
    stcSwdtInit.u32RefreshRange  = SWDT_RANGE_0TO25PCT;
    stcSwdtInit.u32LPModeCountEn = SWDT_LPM_COUNT_STOP;
    stcSwdtInit.u32TrigType      = SWDT_TRIG_EVENT_RESET;
    SWDT_Init(&stcSwdtInit);
    /* Start SWDT */
    SWDT_Feed();

#elif (TEST_RESET_MODE == RESET_WDT)
    /* Configuration  Special Watchdog function */
    stc_wdt_init_t stcWdtInit;
    /* WDT configure */
    stcWdtInit.u32CountCycle    = WDT_COUNTER_CYCLE_65536;
    stcWdtInit.u32ClockDivision = WDT_CLOCK_DIV512;
    stcWdtInit.u32RefreshRange  = WDT_RANGE_0TO25PCT;
    stcWdtInit.u32LPModeCountEn = WDT_LPM_COUNT_STOP;
    stcWdtInit.u32TrigType      = WDT_TRIG_EVENT_RESET;
    WDT_Init(&stcWdtInit);
    /* Start SWDT */
    WDT_Feed();

#elif (TEST_RESET_MODE == RESET_PVD)
    /* Configuration Program voltage Dectection */
    stc_pwc_pvd_config_t  stcPwcPvdConfig;
    PWC_PVD_StructInit(&stcPwcPvdConfig);
    /* Config PVD1 */
    /* PVD1: 2.8V; PVD2: 2.7V */
    stcPwcPvdConfig.u8PvdVoltage  = PWC_PVD1_2V7_PVD2_2V8;
    stcPwcPvdConfig.u8PvdCmpOutEn = PWC_PVD_CMP_ON;
    stcPwcPvdConfig.u8PvdEn       = PWC_PVD_ON;
    stcPwcPvdConfig.u8PvdIntRstEn = PWC_PVD_IR_ON;
    stcPwcPvdConfig.u8PvdIntRstSel= PWC_PVD_RST;
    stcPwcPvdConfig.u8PvdNmiEn    = PWC_PVD_INT_MASK;
    PWC_PVD_Init(PWC_PVD_CH1, &stcPwcPvdConfig);

    /* Config PVD2 */
    /* PVD1: 2.8V; PVD2: 2.7V */
    stcPwcPvdConfig.u8PvdVoltage  = PWC_PVD1_2V7_PVD2_2V8;
    stcPwcPvdConfig.u8PvdCmpOutEn = PWC_PVD_CMP_ON;
    stcPwcPvdConfig.u8PvdEn       = PWC_PVD_ON;
    stcPwcPvdConfig.u8PvdIntRstEn = PWC_PVD_IR_ON;
    stcPwcPvdConfig.u8PvdIntRstSel= PWC_PVD_RST;
    stcPwcPvdConfig.u8PvdNmiEn    = PWC_PVD_INT_MASK;
    PWC_PVD_Init(PWC_PVD_CH2, &stcPwcPvdConfig);
#elif (TEST_RESET_MODE == RESET_CLKFREQERR)
    /* Configuration Clk freqence  measurement function */
    stc_fcm_init_t stcFcmInit;
    stc_clk_xtal_init_t    stcXtalInit;

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_FCM, Enable);

    /* Config XTAL and Enable */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);
    /* Config FCM  Reference Clock: XTAL (8MHZ),  Target Clock: MRC (8MHZ)*/
    FCM_StructInit(&stcFcmInit);
    stcFcmInit.u32RefClk     = FCM_REF_CLK_XTAL;
    stcFcmInit.u32RefClkDiv  = FCM_REF_CLK_DIV8192;
    stcFcmInit.u32RefClkEdge = FCM_REF_CLK_RISING;
    stcFcmInit.u32TarClk     = FCM_TAR_CLK_MRC;
    stcFcmInit.u32TarClkDiv  = FCM_TAR_CLK_DIV1;
    stcFcmInit.u32RstEn      = FCM_RST_ON;
    stcFcmInit.u32IntRstSel  = FCM_ERR_RESET;
    /* idea count value = (tar_freq/tar_div)/(ref_freq/ref_div) */
    stcFcmInit.u16LowerLimit = (uint16_t)((((MRC_VALUE/1U)/(8000000U/8192U))* 80U)/100U);
    stcFcmInit.u16UpperLimit = (uint16_t)((((MRC_VALUE/1U)/(8000000U/8192U))* 85U)/100U);
    FCM_Init(&stcFcmInit);

    /* Enable FMC */
    FCM_Cmd(Enable);
#endif
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
