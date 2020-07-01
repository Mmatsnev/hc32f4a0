/**
 *******************************************************************************
 * @file  fcm/fcm_freq_measure/source/main.c
 * @brief FCM main program example for the Device Driver Library.
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
 * @addtogroup FCM
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define REF_FREQ            (XTAL_VALUE)
#define REF_DIV             (8192U)
#define TAR_FREQ            (MRC_VALUE)
#define TAR_DIV             (1U)

#define FCM_ERR_IRQn        (Int000_IRQn)
#define FCM_OVF_IRQn        (Int001_IRQn)
#define FCM_END_IRQn        (Int002_IRQn)

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
char *TarClk = 0;

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
//    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
//    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
//    EFM_Unlock();
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
//    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
//    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
//    SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
//    EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
//    EFM_FWMC_Lock();
    /* Lock all EFM registers */
//    EFM_Lock();
}

/**
 * @brief  FCM frequency error IRQ callback
 * @param  None
 * @retval None
 */
void FCM_ERR_IrqCallback(void)
{
    FCM_Cmd(Disable);
    FCM_ClearStatus(FCM_FLAG_ERR);
}

/**
 * @brief  FCM measure counter overflow IRQ callback
 * @param  None
 * @retval None
 */
void FCM_OVF_IrqCallback(void)
{
    FCM_Cmd(Disable);
    FCM_ClearStatus(FCM_FLAG_OVF);
}

/**
 * @brief  FCM measure end IRQ callback
 * @param  None
 * @retval None
 */
void FCM_END_IrqCallback(void)
{
    uint16_t u16FcmCnt = 0U;
    u16FcmCnt = FCM_GetCounter();
    FCM_Cmd(Disable);
    if (FCM_REF_CLK_XTAL32 == READ_REG32_BIT(M4_FCM->RCCR, (FCM_RCCR_INEXS | FCM_RCCR_RCKS)))
    {
        printf ("%s freq. is %ld Hz\n" , TarClk, ((XTAL32_VALUE * u16FcmCnt / REF_DIV)*TAR_DIV));
    }
    else
    {
        printf ("%s freq. is %ld KHz\n" , TarClk, ((REF_FREQ /1000 * u16FcmCnt / REF_DIV)*TAR_DIV));
    }

    FCM_ClearStatus(FCM_FLAG_END);
}

/**
 * @brief  FCM frequency error interrupt init
 * @param  None
 * @retval en_result_t
 */
en_result_t FcmErrIntInit(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;
    en_result_t enRet = Ok;

    stcIrqSignConfig.enIntSrc = INT_FCMFERRI;
    stcIrqSignConfig.enIRQn   = FCM_ERR_IRQn;
    stcIrqSignConfig.pfnCallback = &FCM_ERR_IrqCallback;
    enRet = INTC_IrqSignIn(&stcIrqSignConfig);

    if (Ok == enRet)
    {
        /* NVIC config */
        NVIC_ClearPendingIRQ(FCM_ERR_IRQn);
        NVIC_SetPriority(FCM_ERR_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
        NVIC_EnableIRQ(FCM_ERR_IRQn);
    }
    return enRet;
}

/**
 * @brief  FCM measure end interrupt init
 * @param  None
 * @retval en_result_t
 */
en_result_t FcmEndIntInit(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;
    en_result_t enRet = Ok;

    stcIrqSignConfig.enIntSrc = INT_FCMMENDI;
    stcIrqSignConfig.enIRQn   = FCM_END_IRQn;
    stcIrqSignConfig.pfnCallback = &FCM_END_IrqCallback;
    enRet = INTC_IrqSignIn(&stcIrqSignConfig);

    if (Ok == enRet)
    {
        /* NVIC config */
        NVIC_ClearPendingIRQ(FCM_END_IRQn);
        NVIC_SetPriority(FCM_END_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
        NVIC_EnableIRQ(FCM_END_IRQn);
    }
    return enRet;
}

/**
 * @brief  FCM counter overflow interrupt init
 * @param  None
 * @retval en_result_t
 */
en_result_t FcmOvfIntInit(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;
    en_result_t enRet = Ok;

    stcIrqSignConfig.enIntSrc = INT_FCMCOVFI;
    stcIrqSignConfig.enIRQn   = FCM_OVF_IRQn;
    stcIrqSignConfig.pfnCallback = &FCM_OVF_IrqCallback;
    enRet = INTC_IrqSignIn(&stcIrqSignConfig);

    if (Ok == enRet)
    {
        /* NVIC config */
        NVIC_ClearPendingIRQ(FCM_OVF_IRQn);
        NVIC_SetPriority(FCM_OVF_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
        NVIC_EnableIRQ(FCM_OVF_IRQn);
    }
    return enRet;
}

/**
 * @brief  FCM init
 * @param  [in] u32TraClk Target clock type selection.
 *   @arg  FCM_REF_CLK_EXINPUT
 *   @arg  FCM_TAR_CLK_XTAL
 *   @arg  FCM_TAR_CLK_XTAL32
 *   @arg  FCM_TAR_CLK_HRC
 *   @arg  FCM_TAR_CLK_LRC
 *   @arg  FCM_TAR_CLK_SWDTLRC
 *   @arg  FCM_TAR_CLK_PCLK1
 *   @arg  FCM_TAR_CLK_PLLAP
 *   @arg  FCM_TAR_CLK_MRC
 *   @arg  FCM_TAR_CLK_PLLHP
 *   @arg  FCM_TAR_CLK_RTCLRC
 * @param  [in] u32TarClkFreq Target clock frequency.
 * @retval en_result_t
 */
en_result_t FcmInit(uint32_t u32TraClk, uint32_t u32TarClkFreq)
{
    stc_fcm_init_t stcFcmInit;

    FCM_StructInit(&stcFcmInit);
    stcFcmInit.u32RefClk     = FCM_REF_CLK_XTAL;
    stcFcmInit.u32RefClkDiv  = FCM_REF_CLK_DIV8192;
    stcFcmInit.u32RefClkEdge = FCM_REF_CLK_RISING;
    stcFcmInit.u32TarClk     = u32TraClk;
    stcFcmInit.u32TarClkDiv  = FCM_TAR_CLK_DIV1;
    stcFcmInit.u32IntType    = FCM_END_INT_ON | FCM_ERR_INT_ON | FCM_OVF_INT_ON;
    /* idea count value = (tar_freq/tar_div)/(ref_freq/ref_div) */
    stcFcmInit.u16LowerLimit = (uint16_t)((((u32TarClkFreq/TAR_DIV)/(REF_FREQ/REF_DIV))* 97UL)/100UL);
    stcFcmInit.u16UpperLimit = (uint16_t)((((u32TarClkFreq/TAR_DIV)/(REF_FREQ/REF_DIV))* 103UL)/100UL);

    return FCM_Init(&stcFcmInit);
}

/**
 * @brief  FCM reference clock init
 * @param  None
 * @retval en_result_t
 */
en_result_t RefClkInit(void)
{
    stc_clk_xtal_init_t stcXtalInit;

    CLK_XtalStrucInit(&stcXtalInit);

    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_HIGH;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_2MS;
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    return CLK_XtalInit(&stcXtalInit);
}

/**
 * @brief  FCM target clock init, including HRC, LRC, RTCLRC, PLLH, PLLA,
 *         XTAL, XTAL32
 * @param  None
 * @retval en_result_t
 */
en_result_t TarClkInit(void)
{
    stc_clk_xtal32_init_t stcXtal32Init;

    stc_clk_pllh_init_t stcPLLHInit;

    stc_clk_plla_init_t stcPLLAInit;

    /* Xtal32 config */
    CLK_Xtal32StrucInit(&stcXtal32Init);
    stcXtal32Init.u8Xtal32State = CLK_XTAL32_ON;
    stcXtal32Init.u8Xtal32Drv   = CLK_XTAL32DRV_MID;
    stcXtal32Init.u8Xtal32NF    = CLK_XTAL32NF_FULL;
    CLK_Xtal32Init(&stcXtal32Init);

    CLK_HrcCmd(Enable);
    CLK_MrcCmd(Enable);
    CLK_LrcCmd(Enable);
    RTC_LrcCmd(Enable);

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EX BUS Max 120MHz */
    CLK_ClkDiv(CLK_CATE_ALL,                                                    \
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 |              \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 |              \
                CLK_HCLK_DIV1));

    /* PLLH config */
    CLK_PLLHStrucInit(&stcPLLHInit);
    /*
        VCO = 8/1*120 = 960MHz
        8MHz/M*N = 8/1*120/4 =240MHz
    */
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = (1UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLN = (120UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLR = (4UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLQ = (4UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLP = (16UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL;     /* Xtal = 8MHz */
    CLK_PLLHInit(&stcPLLHInit);

    /* PLLA config */
    CLK_PLLAStrucInit(&stcPLLAInit);
    /*
        VCO = 8/1*60 = 480MHz
        8MHz/M*N = 8/1*60/2 =240MHz
    */
    stcPLLAInit.u8PLLState = CLK_PLLA_ON;
    stcPLLAInit.PLLCFGR = 0UL;
    stcPLLAInit.PLLCFGR_f.PLLM = (1UL  - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLN = (60UL - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLR = (2UL  - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLQ = (2UL  - 1UL);
    stcPLLAInit.PLLCFGR_f.PLLP = (8UL  - 1UL);

    CLK_PLLAInit(&stcPLLAInit);

    return Ok;
}

/**
 * @brief  SWDT initialize, SWDTLRC will work after SWDT works.
 * @param  None
 * @retval None
 */
void SwdtInit(void)
{
    stc_swdt_init_t stcSwdtInit;

    /* SWDT configure */
    stcSwdtInit.u32CountCycle    = SWDT_COUNTER_CYCLE_256;
    stcSwdtInit.u32ClockDivision = SWDT_CLOCK_DIV32;
    stcSwdtInit.u32RefreshRange  = SWDT_RANGE_0TO100PCT;
    stcSwdtInit.u32LPModeCountEn = SWDT_LPM_COUNT_CONTINUE;
    stcSwdtInit.u32TrigType      = SWDT_TRIG_EVENT_INT;
    SWDT_Init(&stcSwdtInit);
}

/**
 * @brief  Main function of FCM project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Register write enable for some required peripherals. */
    Peripheral_WE();
    /* Matrix key init */
    BSP_KEY_Init();

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_FCM, Enable);

    TarClkInit();
    RefClkInit();
    FcmErrIntInit();
    FcmEndIntInit();
    FcmOvfIntInit();
    SwdtInit();

    DDL_PrintfInit();

    /* Register write protected for some required peripherals. */
    Peripheral_WP();

    printf("XTAL=8MHz divided by 8192 is used as the reference clock for this demo.\n");


    while (1)
    {
        SWDT_Feed();
        if (Set == BSP_KEY_GetStatus(BSP_KEY_1))
        {
            TarClk = "XTAL32";
            FcmInit(FCM_TAR_CLK_XTAL32, XTAL32_VALUE);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_2))
        {
            TarClk = "HRC";
            FcmInit(FCM_TAR_CLK_HRC, HRC_VALUE);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_3))
        {
            TarClk = "LRC";
            FcmInit(FCM_TAR_CLK_LRC, LRC_VALUE);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_4))
        {
            TarClk = "SWDTLRC";
            FcmInit(FCM_TAR_CLK_SWDTLRC, SWDTLRC_VALUE);
            FCM_SetRefClk(FCM_REF_CLK_XTAL32, FCM_REF_CLK_DIV8192);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_5))
        {
            TarClk = "PCLK1(SYSCLK=8MHz div2)";
            FcmInit(FCM_TAR_CLK_PCLK1, 4UL*1000UL*1000UL);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_6))
        {
            TarClk = "PLLAP(VCO=480MHz div8)";
            FcmInit(FCM_TAR_CLK_PLLAP, 60UL*1000UL*1000UL);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_7))
        {
            TarClk = "MRC";
            FcmInit(FCM_TAR_CLK_MRC, MRC_VALUE);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_8))
        {
            TarClk = "PLLHP(VCO=960MHz div16)";
            FcmInit(FCM_TAR_CLK_PLLHP, 60UL*1000UL*1000UL);
            FCM_Cmd(Enable);
        }
        if (Set == BSP_KEY_GetStatus(BSP_KEY_9))
        {
            TarClk = "RTCLRC";
            FcmInit(FCM_TAR_CLK_RTCLRC, RTCLRC_VALUE);
            FCM_Cmd(Enable);
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
