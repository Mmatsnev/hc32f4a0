/**
 *******************************************************************************
 * @file  sram/sram_error_check/source/main.c
 * @brief Main program of SRAM for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
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
 * @addtogroup SRAM_Error_Check
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Function control. Only for test. */
#define APP_MAKE_ERROR_FOR_TEST                 (1U)

/* Function of SRAM checking definitions. */
#define APP_FUNC_SRAM_PARITY_CHECK              (1U)
#define APP_FUNC_SRAM_ECC_CHECK                 (2U)

/* Select a function of SRAM checking */
#define APP_FUNC                                (APP_FUNC_SRAM_ECC_CHECK)

/* Definitions according to the function that just specified. */
#if (APP_FUNC == APP_FUNC_SRAM_PARITY_CHECK)
    #define APP_SRAM_SELECT                     (SRAM_SRAM123 | SRAM_SRAMH)
    #define APP_SRAM_CHECK_ADDR                 (0x20020000UL)
    #define APP_SRAM_NMI_SRC                    (NMI_SRC_SRAM_PARITY)
#else
    #define APP_SRAM_SELECT                     (SRAM_SRAM4 | SRAM_SRAMB)
    #define APP_SRAM_CHECK_ADDR                 (0x20058000UL)
    #define APP_SRAM_ECC_MODE                   (SRAM_ECC_MODE_3)
    #define APP_SRAM_NMI_SRC                    (NMI_SRC_SRAM_ECC)
#endif

/*
 * Operation after error occurs.
 * 'SRAM_ERR_OP_NMI' and 'SRAM_ERR_OP_RESET' can be used.
 */
#define APP_SRAM_ERR_OP                         (SRAM_ERR_OP_NMI)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void SramConfig(void);

#if (APP_MAKE_ERROR_FOR_TEST > 0U)
    static void SramMakeError(void);
#endif

#if (APP_SRAM_ERR_OP == SRAM_ERR_OP_NMI)
    static void NMI_IrqCallback(void);
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of SRAM error check project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* SRAM configuration. */
    SramConfig();
#if (APP_MAKE_ERROR_FOR_TEST > 0U)
    SramMakeError();
#endif
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    while (1U)
    {
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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    // GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    // PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    // PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    // SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    SRAM_CKCR_Unlock();
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
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    // GPIO_Lock();
    /* Lock PWC register: FCG0 */
    // PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    // PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    // SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    SRAM_CKCR_Lock();
    /* Lock all EFM registers */
    // EFM_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
}

/**
 * @brief  Configures SRAM.
 * @param  None
 * @retval None
 */
static void SramConfig(void)
{
    stc_nmi_init_t stcInit;

    SRAM_Init();

    SRAM_SetErrOperation(APP_SRAM_SELECT, APP_SRAM_ERR_OP);

#if (APP_FUNC == APP_FUNC_SRAM_ECC_CHECK)
    SRAM_SetEccMode(APP_SRAM_SELECT, APP_SRAM_ECC_MODE);
#endif

    /* NMI interrupt configuration. */
    stcInit.u32NmiSrc = APP_SRAM_NMI_SRC;
    stcInit.pfnNmiCallback = &NMI_IrqCallback;
    NMI_Init(&stcInit);
}

#if (APP_MAKE_ERROR_FOR_TEST > 0U)
/**
 * @brief  Make an error of SRAM.
 * @param  None
 * @retval None
 */
static void SramMakeError(void)
{
    __UNUSED uint32_t u32Temp;

#if (APP_FUNC == APP_FUNC_SRAM_PARITY_CHECK)
    /* Read a SRAM address that uninitialized and the parity check error will occur after the reading operation. */
    u32Temp = RW_MEM32(APP_SRAM_CHECK_ADDR);
#else
    /* 1. Specifies an ECC mode. */
    SRAM_SetEccMode(APP_SRAM_SELECT, APP_SRAM_ECC_MODE);

    /* 2. Write and read while an ECC is enabled. */
    RW_MEM32(APP_SRAM_CHECK_ADDR) = 0x12345678UL;
    u32Temp = RW_MEM32(APP_SRAM_CHECK_ADDR);

    /* 3. Disable ECC mode and write a different value to the same address. */
    SRAM_SetEccMode(APP_SRAM_SELECT, SRAM_ECC_MODE_INVALID);
    RW_MEM32(APP_SRAM_CHECK_ADDR) = 0x11223344UL;

    /* 4. Enable the ECC mode again. */
    SRAM_SetEccMode(APP_SRAM_SELECT, APP_SRAM_ECC_MODE);

    /* 5. Read the address that was just written and the ECC check error will occur after the reading operation. */
    u32Temp = RW_MEM32(APP_SRAM_CHECK_ADDR);
#endif
}
#endif /* #if (APP_MAKE_ERROR_FOR_TEST > 0U) */

/**
 * @brief  NMI handler.
 * @param  None
 * @retval None
 */
static void NMI_IrqCallback(void)
{
    SRAM_ClrStatus(SRAM_FLAG_ALL);
    NMI_ClrNmiSrc(NMI_SRC_SRAM_PARITY | NMI_SRC_SRAM_ECC);
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
