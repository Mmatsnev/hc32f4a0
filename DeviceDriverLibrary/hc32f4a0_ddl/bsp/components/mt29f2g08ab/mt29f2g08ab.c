/**
 *******************************************************************************
 * @file  mt29f2g08ab.c
 * @brief This midware file provides firmware functions to manage the nand flash
 *        component library for mt29f2g08ab.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
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
#include "hc32f4a0_clk.h"
#include "hc32f4a0_gpio.h"
#include "hc32f4a0_nfc.h"
#include "hc32f4a0_pwc.h"
#include "hc32f4a0_utility.h"

#include "mt29f2g08ab.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup MT29F2G08AB Nand Flash MT29F2G08AB
 * @{
 */

#if (BSP_MT29F2G08AB_ENABLE == BSP_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup MT29F2G08AB_Local_Macros MT29F2G08AB Local Macros
 * @{
 */

/**
 * @defgroup MT29F2G08AB_Map_NFC_Bank MT29F2G08AB Map NFC Bank
 * @{
 */
#define MT29F2G08AB_BYTES_PER_PAGE          (EXMC_NFC_PAGE_SIZE_2KBYTES)
#define MT29F2G08AB_CAPACITY_BITS           (EXMC_NFC_BANK_CAPACITY_2GBIT)
/**
 * @}
 */

/**
 * @defgroup MT29F2G08AB_Status_Register_Field MT29F2G08AB Status Register Field
 * @{
 */
/* MT29F2G08AB Status Register: bit0-FAIL */
#define MT29F2G08AB_SR_FAIL                         (1UL)

/* MT29F2G08AB Status Register: bit6-RDY */
#define MT29F2G08AB_SR_READY                        (1UL << 6)

/* MT29F2G08AB Status Register mask */
#define MT29F2G08AB_SR_MASK                         (MT29F2G08AB_SR_FAIL | MT29F2G08AB_SR_READY)
/**
 * @}
 */

/**
 * @defgroup MT29F2G08AB_Operation_Timeout MT29F2G08AB Operation Timeout
 * @{
 */
#define MT29F2G08AB_READ_TIMEOUT            (0x01000000UL)
#define MT29F2G08AB_WRITE_TIMEOUT           (0x01000000UL)
#define MT29F2G08AB_ERASE_TIMEOUT           (0x01000000UL)
#define MT29F2G08AB_MAX_TIMEOUT             (0xFFFFFFFFUL)
/**
 * @}
 */

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

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup MT29F2G08AB_Global_Functions MT29F2G08AB Global Functions
 * @{
 */

/**
 * @brief  Initialize MT29F2G08AB.
 * @param  None
 * @retval None
 */
en_result_t MT29F2G08AB_Init(void)
{
    return BSP_NFC_MT29F2G08AB_Init();
}

/**
 * @brief  Read ID.
 * @param  [in] u32IdAddr               The address
 * @param  [in] au8DevId                The id buffer
 * @param  [in] u32NumBytes             The number of bytes to read
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       au8DevId == NULL or u8NumBytes == 0
 */
en_result_t MT29F2G08AB_ReadId(uint32_t u32IdAddr,
                                    uint8_t au8DevId[],
                                    uint32_t u32NumBytes)
{
    return BSP_NFC_MT29F2G08AB_ReadId(u32IdAddr, au8DevId, u32NumBytes);
}

/**
 * @brief  Get status.
 * @param  [in] u32Timeout              Timeout value
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          Command operate successfully.
 *   @arg  Error:                       Command operate unsuccessfully.
 *   @arg  ErrorOperationInProgress:    Devcie is busy.
 */
en_result_t MT29F2G08AB_GetStatus(uint32_t u32Timeout)
{
    uint32_t u32To = 0UL;
    uint32_t u32Status = 0UL;
    en_result_t enRet = Ok;

    do
    {
        /* Block checking flag if timeout value is TIMEOUT_MAX_DELAY */
        if ((u32To++ > u32Timeout) && (u32Timeout < MT29F2G08AB_MAX_TIMEOUT))
        {
            enRet = ErrorTimeout;
            break;
        }

        u32Status = BSP_NFC_MT29F2G08AB_ReadStatus();
    } while (0UL == (u32Status & MT29F2G08AB_SR_READY));

    if (ErrorTimeout != enRet)
    {
        if (0UL != (u32Status & MT29F2G08AB_SR_FAIL))
        {
            enRet = Error;
        }
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
 *   @arg  ErrorInvalidParameter:       The paramter iv invalid.
 */
en_result_t MT29F2G08AB_EraseBlock(uint32_t u32BlockRowAddress)
{
    en_result_t enRet;

    enRet = BSP_NFC_MT29F2G08AB_EraseBlock(u32BlockRowAddress);
    if (Ok == enRet)
    {
        enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_ERASE_TIMEOUT);
    }

    return enRet;
}

/**
 * @brief  Read page.
 * @param  [in] u32Page                 The specified page
 * @param  [out] pu8Buf                 The buffer for reading
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Read timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t MT29F2G08AB_ReadPageMeta(uint32_t u32Page,
                                            uint8_t *pu8Buf,
                                            uint32_t u32NumBytes)
{
    en_result_t enRet = Error;

    if (NULL != pu8Buf)
    {
        /* Check parameters */
        DDL_ASSERT(u32Page < MT29F2G08AB_DEVICE_PAGES);
        DDL_ASSERT((u32NumBytes > 0UL) && (u32NumBytes <= MT29F2G08AB_PAGE_SIZE_WITH_SPARE));

        enRet = BSP_NFC_MT29F2G08AB_ReadPageMeta(u32Page, pu8Buf, u32NumBytes);
        if (Ok == enRet)
        {
            enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_READ_TIMEOUT);
        }
    }

    return enRet;
}

/**
 * @brief  Write page.
 * @param  [in] u32Page                 The specified page
 * @param  [in] pu8Buf                  The buffer for writing
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Write timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t MT29F2G08AB_WritePageMeta(uint32_t u32Page,
                                            const uint8_t *pu8Buf,
                                            uint32_t u32NumBytes)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pu8Buf)
    {
        /* Check parameters */
        DDL_ASSERT(u32Page < MT29F2G08AB_DEVICE_PAGES);
        DDL_ASSERT((u32NumBytes > 0UL) && (u32NumBytes <= MT29F2G08AB_PAGE_SIZE_WITH_SPARE));

        enRet = BSP_NFC_MT29F2G08AB_WritePageMeta(u32Page, pu8Buf, u32NumBytes);
        if (Ok == enRet)
        {
            enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_WRITE_TIMEOUT);
        }
    }

    return enRet;
}

/**
 * @brief  Read page with hardware ecc.
 * @param  [in] u32Page                 The specified page
 * @param  [out] pu8Buf                 The buffer for reading
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Read timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t MT29F2G08AB_ReadPageHwEcc1Bit(uint32_t u32Page,
                                                    uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pu8Buf)
    {
        /* Check parameters */
        DDL_ASSERT(u32Page < MT29F2G08AB_DEVICE_PAGES);
        DDL_ASSERT((u32NumBytes > 0UL) && (u32NumBytes <= MT29F2G08AB_PAGE_SIZE_WITH_SPARE));

        enRet = BSP_NFC_MT29F2G08AB_ReadPageHwEcc(u32Page, pu8Buf, u32NumBytes);
        if (Ok == enRet)
        {
            enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_READ_TIMEOUT);
        }
    }

    return enRet;
}

/**
 * @brief  Write page with hardware ecc.
 * @param  [in] u32Page                 The specified page
 * @param  [in] pu8Buf                  The buffer for writing
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Write timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t MT29F2G08AB_WritePageHwEcc1Bit(uint32_t u32Page,
                                                    const uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pu8Buf)
    {
        /* Check parameters */
        DDL_ASSERT(u32Page < MT29F2G08AB_DEVICE_PAGES);
        DDL_ASSERT((u32NumBytes > 0UL) && (u32NumBytes <= MT29F2G08AB_PAGE_SIZE_WITH_SPARE));

        enRet = BSP_NFC_MT29F2G08AB_WritePageHwEcc(u32Page, pu8Buf, u32NumBytes);
        if (Ok == enRet)
        {
            enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_WRITE_TIMEOUT);
        }
    }

    return enRet;
}

/**
 * @brief  Write page with hardware ecc.
 * @param  [in] u32Page                 The specified page
 * @param  [in] pu8Buf                  The buffer for writing
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Write timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t MT29F2G08AB_WritePageHwEcc4Bits(uint32_t u32Page,
                                                    const uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pu8Buf)
    {
        /* Check parameters */
        DDL_ASSERT(u32Page < MT29F2G08AB_DEVICE_PAGES);
        DDL_ASSERT((u32NumBytes > 0UL) && (u32NumBytes <= MT29F2G08AB_PAGE_SIZE_WITH_SPARE));

        enRet = BSP_NFC_MT29F2G08AB_WritePageHwEcc(u32Page, pu8Buf, u32NumBytes);
        if (Ok == enRet)
        {
            enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_WRITE_TIMEOUT);
        }
    }

    return enRet;
}

/**
 * @brief  Read page with hardware ecc.
 * @param  [in] u32Page                 The specified page
 * @param  [out] pu8Buf                 The buffer for reading
 * @param  [in] u32NumBytes             The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorTimeout:                Read timeout.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t MT29F2G08AB_ReadPageHwEcc4Bits(uint32_t u32Page,
                                                    uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pu8Buf)
    {
        /* Check parameters */
        DDL_ASSERT(u32Page < MT29F2G08AB_DEVICE_PAGES);
        DDL_ASSERT((u32NumBytes > 0UL) && (u32NumBytes <= MT29F2G08AB_PAGE_SIZE_WITH_SPARE));

        enRet = BSP_NFC_MT29F2G08AB_ReadPageHwEcc(u32Page, pu8Buf, u32NumBytes);
        if (Ok == enRet)
        {
            enRet = MT29F2G08AB_GetStatus(MT29F2G08AB_READ_TIMEOUT);
        }
    }

    return enRet;
}

/**
 * @}
 */

#endif /* BSP_MT29F2G08AB_ENABLE */

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
