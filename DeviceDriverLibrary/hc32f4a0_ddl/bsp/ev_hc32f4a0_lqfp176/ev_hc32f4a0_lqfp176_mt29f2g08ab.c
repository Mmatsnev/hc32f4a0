/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_mt29f2g08ab.c
 * @brief This file provides configure functions for mt29f2g08ab of the board 
 *        EV_F4A0_LQ176_V10.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
   2020-07-03       Hongjh          Adjust EXMC pin drive capacity to high drive
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
#include "ev_hc32f4a0_lqfp176_mt29f2g08ab.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */

/** @defgroup EV_HC32F4A0_LQFP176_MT29F2G08AB HC32F4A0 EVB LQFP176 MT29F2G08AB
 * @{
 */

#if ((BSP_ON == BSP_MT29F2G08AB_ENABLE) && \
     (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_MT29F2G08AB_Local_Macros HC32F4A0 EVB LQFP176 MT29F2G08AB Local Macros
 * @{
 */

/**
 * @defgroup NFC_Interface_Pin NF Interface Pin
 * @{
 */
#define NFC_CE_PORT                             (GPIO_PORT_C)   /* PC02 - EXMC_CE0 */
#define NFC_CE_PIN                              (GPIO_PIN_02)

#define NFC_RE_PORT                             (GPIO_PORT_F)   /* PF11 - EXMC_OE */
#define NFC_RE_PIN                              (GPIO_PIN_11)

#define NFC_WE_PORT                             (GPIO_PORT_C)   /* PC00 - EXMC_WE */
#define NFC_WE_PIN                              (GPIO_PIN_00)

#define NFC_CLE_PORT                            (GPIO_PORT_I)   /* PI12 - EXMC_CLE */
#define NFC_CLE_PIN                             (GPIO_PIN_12)

#define NFC_ALE_PORT                            (GPIO_PORT_C)   /* PC03 - EXMC_ALE */
#define NFC_ALE_PIN                             (GPIO_PIN_03)

#define NFC_WP_PORT                             (GPIO_PORT_G)   /* PG15 - EXMC_BAA */
#define NFC_WP_PIN                              (GPIO_PIN_15)

#define NFC_RB_PORT                             (GPIO_PORT_G)   /* PG06 - EXMC_RB0 */
#define NFC_RB_PIN                              (GPIO_PIN_06)

#define NFC_DATA0_PORT                          (GPIO_PORT_D)   /* PD14 - EXMC_DATA0 */
#define NFC_DATA0_PIN                           (GPIO_PIN_14)
#define NFC_DATA1_PORT                          (GPIO_PORT_D)   /* PD15 - EXMC_DATA1 */
#define NFC_DATA1_PIN                           (GPIO_PIN_15)
#define NFC_DATA2_PORT                          (GPIO_PORT_D)   /* PD0 - EXMC_DATA2 */
#define NFC_DATA2_PIN                           (GPIO_PIN_00)
#define NFC_DATA3_PORT                          (GPIO_PORT_D)   /* PD1 - EXMC_DATA3 */
#define NFC_DATA3_PIN                           (GPIO_PIN_01)
#define NFC_DATA4_PORT                          (GPIO_PORT_E)   /* PE7 - EXMC_DATA4 */
#define NFC_DATA4_PIN                           (GPIO_PIN_07)
#define NFC_DATA5_PORT                          (GPIO_PORT_E)   /* PE8 - EXMC_DATA5 */
#define NFC_DATA5_PIN                           (GPIO_PIN_08)
#define NFC_DATA6_PORT                          (GPIO_PORT_E)   /* PE9 - EXMC_DATA6 */
#define NFC_DATA6_PIN                           (GPIO_PIN_09)
#define NFC_DATA7_PORT                          (GPIO_PORT_E)   /* PE10 - EXMC_DATA7 */
#define NFC_DATA7_PIN                           (GPIO_PIN_10)

/**
 * @defgroup EV_EXMC_NFC_Operation_Timeout EXMC NFC Operation Timeout
 * @{
 */
#define BSP_NFC_ERASE_TIMEOUT                    (2000000UL)
#define BSP_NFC_READ_TIMEOUT                     (2000000UL)
#define BSP_NFC_READ_HWECC_TIMEOUT               (9000000UL)
#define BSP_NFC_WRITE_TIMEOUT                    (2000000UL)
#define BSP_NFC_WRITE_HWECC_TIMEOUT              (2000000UL)
#define BSP_NFC_RESET_TIMEOUT                    (2000000UL)
#define BSP_NFC_MAX_TIMEOUT                      (0xFFFFFFFFUL)
/**
 * @}
 */

/**
 * @}
 */

#define BSP_EV_HC32F4A0_MT29F2G08AB_BANK        (EXMC_NFC_BANK_0)
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @addtogroup EV_HC32F4A0_LQFP176_IS62WV51216_Local_Functions
 * @{
 */

static void EV_EXMC_NFC_PortInit(void);
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_MT29G08AB_Global_Functions HC32F4A0 LQFP176 EVB MT29G08AB Global Functions
 * @{
 */

/**
 * @brief  Initialize Flash.
 * @param  None
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          Initialize successfully.
 *   @arg  Error:                       Initialize unsuccessfully.
 */
en_result_t BSP_NFC_MT29F2G08AB_Init(void)
{
    en_result_t enRet = Error;
    stc_exmc_nfc_init_t stcInit;

    /* Initialize NFC port.*/
    EV_EXMC_NFC_PortInit();

    /* Enable NFC module clk */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_NFC, Enable);

    /* Enable NFC. */
    EXMC_NFC_Cmd(Enable);

    /* Configure NFC width && refresh period & chip & timing. */
    stcInit.u32OpenPage = EXMC_NFC_OPEN_PAGE_DISABLE;
    stcInit.stcBaseCfg.u32CapacitySize = EXMC_NFC_BANK_CAPACITY_2GBIT;
    stcInit.stcBaseCfg.u32MemWidth = EXMC_NFC_MEMORY_WIDTH_8BIT;
    stcInit.stcBaseCfg.u32BankNum = EXMC_NFC_1_BANK;
    stcInit.stcBaseCfg.u32PageSize = EXMC_NFC_PAGE_SIZE_2KBYTES;
    stcInit.stcBaseCfg.u32WrProtect = EXMC_NFC_WR_PROTECT_DISABLE;
    stcInit.stcBaseCfg.u32EccMode = EXMC_NFC_ECC_1BIT;
    stcInit.stcBaseCfg.u32RowAddrCycle = EXMC_NFC_3_ROW_ADDRESS_CYCLES;
    stcInit.stcBaseCfg.u8SpareSizeForUserData = 0U;

    stcInit.stcTimingReg0.u32TS = 4UL;         /* CLOCK frequency @60MHz: 3.3V */
    stcInit.stcTimingReg0.u32TWP = 3UL;
    stcInit.stcTimingReg0.u32TRP = 6UL;
    stcInit.stcTimingReg0.u32TH = 2UL;

    stcInit.stcTimingReg1.u32TWH = 2UL;
    stcInit.stcTimingReg1.u32TRH = 3UL;
    stcInit.stcTimingReg1.u32TRR = 8UL;
    stcInit.stcTimingReg1.u32TWB = 8UL;

    stcInit.stcTimingReg2.u32TCCS = 10UL;
    stcInit.stcTimingReg2.u32TWTR = 0xdUL;
    stcInit.stcTimingReg2.u32TRTW = 8UL;
    stcInit.stcTimingReg2.u32TADL = 10UL;
    if (Ok == EXMC_NFC_Init(&stcInit))
    {
        /* Reset NFC device. */
        if(Ok == EXMC_NFC_Reset(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, BSP_NFC_RESET_TIMEOUT))
        {
            enRet = Ok;
        }
    }

    return enRet;
}

/**
 * @brief  Read status.
 * @param  None
 * @retval NAND Flash status
 */
uint32_t BSP_NFC_MT29F2G08AB_ReadStatus(void)
{
    return EXMC_NFC_ReadStatus(BSP_EV_HC32F4A0_MT29F2G08AB_BANK);
}

/**
 * @brief  Read ID.
 * @param  [in] u32IdAddr               The address
 * @param  [in] au8DevId                The id buffer
 * @param  [in] u32NumBytes             The number of bytes to read
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       au8DevId == NULL or u8NumWords == 0
 */
en_result_t BSP_NFC_MT29F2G08AB_ReadId(uint32_t u32IdAddr,
                                        uint8_t au8DevId[],
                                        uint32_t u32NumBytes)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != au8DevId) && (u32NumBytes > 0UL))
    {
        enRet = EXMC_NFC_ReadId(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, \
                                u32IdAddr, \
                                au8DevId, \
                                u32NumBytes);
    }

    return enRet;
}

/**
 * @brief  Erase block.
 * @param  [in] u32BlockRowAddress      The specified block row address
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Erase timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_NFC_MT29F2G08AB_EraseBlock(uint32_t u32BlockRowAddress)
{
    en_result_t enRet;

    enRet = EXMC_NFC_EraseBlock(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, \
                                u32BlockRowAddress, \
                                BSP_NFC_ERASE_TIMEOUT);
    return enRet;
}

/**
 * @brief  Read page.
 * @param  [in] u32Page                 The specified page
 * @param  [out] pu8Data                The buffer for reading
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Read timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_NFC_MT29F2G08AB_ReadPageMeta(uint32_t u32Page,
                                    uint8_t *pu8Data,
                                    uint32_t u32NumBytes)
{
    en_result_t enRet;

    enRet = EXMC_NFC_ReadPageMeta(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, \
                                  u32Page, \
                                  pu8Data, \
                                  u32NumBytes, \
                                  BSP_NFC_READ_TIMEOUT);
    return enRet;
}

/**
 * @brief  Write page.
 * @param  [in] u32Page                 The specified page
 * @param  [in] pu8Data                 The buffer for writing
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Write timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_NFC_MT29F2G08AB_WritePageMeta(uint32_t u32Page,
                                    const uint8_t *pu8Data,
                                    uint32_t u32NumBytes)
{
    en_result_t enRet;

    enRet = EXMC_NFC_WritePageMeta(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, \
                                    u32Page, \
                                    pu8Data, \
                                    u32NumBytes, \
                                    BSP_NFC_WRITE_TIMEOUT);
    return enRet;
}

/**
 * @brief  Read page with hardware ecc.
 * @param  [in] u32Page                 The specified page
 * @param  [out] pu8Data                The buffer for reading
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Read timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_NFC_MT29F2G08AB_ReadPageHwEcc(uint32_t u32Page,
                                    uint8_t *pu8Data,
                                    uint32_t u32NumBytes)
{
    en_result_t enRet;

    enRet = EXMC_NFC_ReadPageHwEcc(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, \
                                    u32Page, \
                                    pu8Data, \
                                    u32NumBytes, \
                                    BSP_NFC_READ_HWECC_TIMEOUT);
    return enRet;
}

/**
 * @brief  Write page.
 * @param  [in] u32Page                 The specified page
 * @param  [in] pu8Data                 The buffer for writing
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Write timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_NFC_MT29F2G08AB_WritePageHwEcc(uint32_t u32Page,
                                    const uint8_t *pu8Data,
                                    uint32_t u32NumBytes)
{
    en_result_t enRet;

    enRet = EXMC_NFC_WritePageHwEcc(BSP_EV_HC32F4A0_MT29F2G08AB_BANK, \
                                    u32Page, \
                                    pu8Data, \
                                    u32NumBytes, \
                                    BSP_NFC_WRITE_HWECC_TIMEOUT);
    return enRet;
}

/**
 * @}
 */

/**
 * @defgroup EV_HC32F4A0_LQFP176_MT29G08AB_Local_Functions HC32F4A0 LQFP176 EVB MT29G08AB Local Functions
 * @{
 */

/**
 * @brief  Initialize NFC port.
 * @param  None
 * @retval None
 */
static void EV_EXMC_NFC_PortInit(void)
{
    stc_gpio_init_t stcGpioInit;

    /************************* Set pin drive capacity *************************/
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_DRV_HIGH;

    /* NFC_CE */
    GPIO_Init(NFC_CE_PORT, NFC_CE_PIN, &stcGpioInit);

    /* NFC_RE */
    GPIO_Init(NFC_RE_PORT, NFC_RE_PIN, &stcGpioInit);

    /* NFC_WE */
    GPIO_Init(NFC_WE_PORT, NFC_WE_PIN, &stcGpioInit);

    /* NFC_CLE */
    GPIO_Init(NFC_CLE_PORT, NFC_CLE_PIN, &stcGpioInit);

    /* NFC_ALE */
    GPIO_Init(NFC_ALE_PORT, NFC_ALE_PIN, &stcGpioInit);

    /* NFC_WP */
    GPIO_Init(NFC_WP_PORT, NFC_WP_PIN, &stcGpioInit);
    GPIO_SetPins(NFC_WP_PORT, NFC_WP_PIN);

    /* NFC_DATA[0:7] */
    GPIO_Init(NFC_DATA0_PORT, NFC_DATA0_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA1_PORT, NFC_DATA1_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA2_PORT, NFC_DATA2_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA3_PORT, NFC_DATA3_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA4_PORT, NFC_DATA4_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA5_PORT, NFC_DATA5_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA6_PORT, NFC_DATA6_PIN, &stcGpioInit);
    GPIO_Init(NFC_DATA7_PORT, NFC_DATA7_PIN, &stcGpioInit);

    /* NFC_RB */
    GPIO_Init(NFC_RB_PORT, NFC_RB_PIN, &stcGpioInit);

    /************************** Set EXMC pin function *************************/
    /* NFC_CE */
    GPIO_SetFunc(NFC_CE_PORT, NFC_CE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_RE */
    GPIO_SetFunc(NFC_RE_PORT, NFC_RE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_WE */
    GPIO_SetFunc(NFC_WE_PORT, NFC_WE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_CLE */
    GPIO_SetFunc(NFC_CLE_PORT, NFC_CLE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_ALE */
    GPIO_SetFunc(NFC_ALE_PORT, NFC_ALE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_WP */
    GPIO_SetFunc(NFC_WP_PORT, NFC_WP_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_RB */
    GPIO_SetFunc(NFC_RB_PORT, NFC_RB_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* NFC_DATA[0:7] */
    GPIO_SetFunc(NFC_DATA0_PORT, NFC_DATA0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA1_PORT, NFC_DATA1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA2_PORT, NFC_DATA2_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA3_PORT, NFC_DATA3_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA4_PORT, NFC_DATA4_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA5_PORT, NFC_DATA5_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA6_PORT, NFC_DATA6_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(NFC_DATA7_PORT, NFC_DATA7_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
}

/**
 * @}
 */

#endif /* BSP_EV_HC32F4A0_LQFP176/BSP_MT29F2G08AB_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
