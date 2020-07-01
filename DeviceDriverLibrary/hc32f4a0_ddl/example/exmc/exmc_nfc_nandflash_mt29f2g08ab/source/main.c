/**
 *******************************************************************************
 * @file  exmc/exmc_nfc_nandflash_mt29f2g08ab/source/main.c
 * @brief This example demonstrates NADN Flash function.
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
#include <string.h>

#include "hc32_ddl.h"
#include "1bit_per_512byte_ecc.h"
#include "decode_4bit_ecc_err_loc.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup EXMC_NFC_Nandflash_MT29F2G08AB
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

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
static en_result_t MT29F2G08AB_MetaWithoutSpareTest(uint32_t u32Page);
static en_result_t MT29F2G08AB_MetaWithSpareTest(uint32_t u32Page);
static en_result_t MT29F2G08AB_HwEcc1BitTest(uint32_t u32Page);
static en_result_t MT29F2G08AB_HwEcc4BitsTest(uint32_t u32Page);

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
 * @brief  nandflash program test without ECC and spare
 * @param  [in] u32Page                 Program page
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 */
static en_result_t MT29F2G08AB_MetaWithoutSpareTest(uint32_t u32Page)
{
    en_result_t enRet = Error;

    __ALIGN_BEGIN static uint8_t m_au8ReadDataMeta[MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE];
    __ALIGN_BEGIN static uint8_t m_au8WriteDataMeta[MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE];

    /* Initialize data. */
    for (uint32_t i = 0U; i < MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE; i++)
    {
        m_au8WriteDataMeta[i] = (uint8_t)i;
    }

    /* Write page: 2048Bytes */
    if (Ok == MT29F2G08AB_WritePageMeta(u32Page, m_au8WriteDataMeta, MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE))
    {
        /* Read page: 2048Bytes */
        if (Ok == MT29F2G08AB_ReadPageMeta(u32Page, m_au8ReadDataMeta, MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE))
        {
            /* Verify write/read data. */
            if (0 == memcmp (m_au8WriteDataMeta, m_au8ReadDataMeta, MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE))
            {
                enRet = Ok;
            }
        }
    }

    return enRet;
}

/**
 * @brief  nandflash program test without ECC
 * @param  [in] u32Page                 Program page
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 */
static en_result_t MT29F2G08AB_MetaWithSpareTest(uint32_t u32Page)
{
    en_result_t enRet = Error;

    __ALIGN_BEGIN static uint8_t m_au8ReadDataMetaWithSpare[MT29F2G08AB_PAGE_SIZE_WITH_SPARE];
    __ALIGN_BEGIN static uint8_t m_au8WriteDataMetaWithSpare[MT29F2G08AB_PAGE_SIZE_WITH_SPARE];

    /* Initialize data. */
    for (uint32_t i = 0U; i < MT29F2G08AB_PAGE_SIZE_WITH_SPARE; i++)
    {
        m_au8WriteDataMetaWithSpare[i] = (uint8_t)i;
    }

    /* Write page: 2048 + 64Bytes */
    if (Ok == MT29F2G08AB_WritePageMeta(u32Page, m_au8WriteDataMetaWithSpare, MT29F2G08AB_PAGE_SIZE_WITH_SPARE))
    {
        /* Read page: 2048 + 64Bytes */
        if (Ok == MT29F2G08AB_ReadPageMeta(u32Page, m_au8ReadDataMetaWithSpare, MT29F2G08AB_PAGE_SIZE_WITH_SPARE))
        {
            /* Verify write/read data. */
            if (0 == memcmp (m_au8WriteDataMetaWithSpare, m_au8ReadDataMetaWithSpare, MT29F2G08AB_PAGE_SIZE_WITH_SPARE))
            {
                enRet = Ok;
            }
        }
    }

    return enRet;
}

/**
 * @brief  nandflash program test with hardware ECC 1 bit
 * @param  [in] u32Page                 Program page
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 */
static en_result_t MT29F2G08AB_HwEcc1BitTest(uint32_t u32Page)
{
    en_result_t enRet = Error;
    uint32_t u32EccTestResult;
    uint32_t u32EccExpectedResult;

    __ALIGN_BEGIN static uint8_t m_au8ReadDataHwEcc[MT29F2G08AB_PAGE_SIZE_WITH_SPARE];
    __ALIGN_BEGIN static uint8_t m_au8WriteDataHwEcc[MT29F2G08AB_PAGE_SIZE_WITH_SPARE];
    __ALIGN_BEGIN static uint8_t m_au8SwEcc1Bit[MT29F2G08AB_PAGE_1BIT_ECC_VALUE_SIZE];

    /* Initialize data. */
    for (uint32_t i = 0UL; i < MT29F2G08AB_PAGE_SIZE_WITH_SPARE; i++)
    {
        m_au8WriteDataHwEcc[i] = (uint8_t)(i);
    }

    /* Enable ECC 1bit: write 2048Bytes */
    if (Ok == MT29F2G08AB_WritePageHwEcc1Bit(u32Page, \
                                        m_au8WriteDataHwEcc, \
                                        MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE))
    {
        /* Disable ECC 1bit: read 2048 + 64Bytes */
        MT29F2G08AB_ReadPageMeta(u32Page, m_au8ReadDataHwEcc, MT29F2G08AB_PAGE_SIZE_WITH_SPARE);

        /* Software calculate ECC 1bit */
        NFC_SwCalculateEcc1Bit(&m_au8WriteDataHwEcc[0], &m_au8SwEcc1Bit[0]);
        NFC_SwCalculateEcc1Bit(&m_au8WriteDataHwEcc[512], &m_au8SwEcc1Bit[3]);
        NFC_SwCalculateEcc1Bit(&m_au8WriteDataHwEcc[1024], &m_au8SwEcc1Bit[6]);
        NFC_SwCalculateEcc1Bit(&m_au8WriteDataHwEcc[1536], &m_au8SwEcc1Bit[9]);

        /* Compare software & hardware calculating ECC result */
        if (0 == memcmp (m_au8SwEcc1Bit, \
                    &m_au8ReadDataHwEcc[MT29F2G08AB_PAGE_SIZE_WITH_SPARE - MT29F2G08AB_PAGE_1BIT_ECC_VALUE_SIZE], \
                    MT29F2G08AB_PAGE_1BIT_ECC_VALUE_SIZE))
        {
            /* Modify the 2nd byte value from 0x01 to 0x00 */
            m_au8ReadDataHwEcc[1] = 0x00U;

            /* Single bit error: the 2nd byte - bit 0 */
            u32EccExpectedResult = ((0UL << EXMC_NFC_1BIT_ECC_ERR_BIT_POS) | \
                                    (1UL << EXMC_NFC_1BIT_ECC_ERR_BYTE_POS) | \
                                    EXMC_NFC_1BIT_ECC_SINGLE_BIT_ERR);

            /* Disable ECC 1bit: write 2048 + 64Bytes */
            MT29F2G08AB_WritePageMeta(u32Page, m_au8ReadDataHwEcc, MT29F2G08AB_PAGE_SIZE_WITH_SPARE);

            /* Enable ECC 1bit: read 2048Bytes */
            MT29F2G08AB_ReadPageHwEcc1Bit(u32Page, \
                                      m_au8ReadDataHwEcc, \
                                      MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE);

            /* Get ECC result */
            u32EccTestResult = EXMC_NFC_GetEcc1BitResult(EXMC_NFC_ECC_SECTION0);
            if (u32EccExpectedResult == u32EccTestResult)
            {
                enRet = Ok; /* The result meets the expected */
            }
        }
    }

    return enRet;
}

/**
 * @brief  nandflash program test with hardware ECC 4bits
 * @param  [in] u32Page                 Program page
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 */
static en_result_t MT29F2G08AB_HwEcc4BitsTest(uint32_t u32Page)
{
    en_result_t enRet = Error;
    uint16_t au16SynVal[8];
    int16_t ai16EccTestErrByteNumber[4];
    int16_t ai16EccTestErrByteBit[4];
    int16_t ai16EccExpectedErrByteNumber[4];
    int16_t ai16EccExpectedErrByteBit[4];

    __ALIGN_BEGIN static uint8_t m_au8ReadDataHwEcc[MT29F2G08AB_PAGE_SIZE_WITH_SPARE];
    __ALIGN_BEGIN static uint8_t m_au8WriteDataHwEcc[MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE];

    /* Initialize data. */
    for (uint32_t i = 0UL; i < MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE; i++)
    {
        m_au8WriteDataHwEcc[i] = (uint8_t)(i);
    }

    EXMC_NFC_SetEccMode(EXMC_NFC_ECC_4BITS);

    /* Enable ECC 4bit: write 2048Bytes */
    if (Ok == MT29F2G08AB_WritePageHwEcc4Bits(u32Page, \
                                        m_au8WriteDataHwEcc, \
                                        MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE))
    {
        /* Enable ECC 4bit: read 2048Bytes */
        MT29F2G08AB_ReadPageHwEcc4Bits(u32Page, \
                                        m_au8ReadDataHwEcc, \
                                        MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE);

        /* Check whether ECC errors occur */
        if (EXMC_NFC_GetStatus(EXMC_NFC_FLAG_ECC_ERROR) == Reset)
        {
            /* Disable ECC 4bit: read 2048 + 64Bytes */
            MT29F2G08AB_ReadPageMeta(u32Page, m_au8ReadDataHwEcc, MT29F2G08AB_PAGE_SIZE_WITH_SPARE);

            /* Modify data to create error */
            m_au8ReadDataHwEcc[254] = 0xAEU;  /* Modify the 254th byte value from 0xFE to 0xAE */
            m_au8ReadDataHwEcc[255] = 0xF5U;  /* Modify the 255th byte value from 0xFF to 0xF5 */

            /* Error bit: the 254th byte - 2 bit */
            ai16EccExpectedErrByteNumber[0] = 254;
            ai16EccExpectedErrByteBit[0] = 4;

            /* Error bit: the 254th byte - 3 bit */
            ai16EccExpectedErrByteNumber[1] = 254;
            ai16EccExpectedErrByteBit[1] = 6;

            /* Error bit: the 255th byte - 1 bit */
            ai16EccExpectedErrByteNumber[2] = 255;
            ai16EccExpectedErrByteBit[2] = 1;

            /* Error bit: the 255th byte - 3 bit */
            ai16EccExpectedErrByteNumber[3] = 255;
            ai16EccExpectedErrByteBit[3] = 3;

            /* Disable ECC 4bit: write 2048 + 64Bytes */
            MT29F2G08AB_WritePageMeta(u32Page, m_au8ReadDataHwEcc, MT29F2G08AB_PAGE_SIZE_WITH_SPARE);

            /* Enable ECC 4bit: read 2048Bytes */
            MT29F2G08AB_ReadPageHwEcc4Bits(u32Page, \
                                        m_au8ReadDataHwEcc, \
                                        MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE);

            /* Get ECC syndrome */
            EXMC_NFC_GetSyndrome(EXMC_NFC_ECC_SECTION0, au16SynVal, 8U);

            /* Decode ECC errors location */
            if (4 == NFC_SwDecodeEcc4BitsErrLocation((const int16_t *)au16SynVal, ai16EccTestErrByteNumber, ai16EccTestErrByteBit, 4))
            {
                if ((ai16EccExpectedErrByteBit[0] == ai16EccTestErrByteBit[0]) && \
                    (ai16EccExpectedErrByteBit[1] == ai16EccTestErrByteBit[1]) && \
                    (ai16EccExpectedErrByteBit[2] == ai16EccTestErrByteBit[2]) && \
                    (ai16EccExpectedErrByteBit[3] == ai16EccTestErrByteBit[3]) && \
                    (ai16EccExpectedErrByteNumber[0] == ai16EccTestErrByteNumber[0]) && \
                    (ai16EccExpectedErrByteNumber[1] == ai16EccTestErrByteNumber[1]) && \
                    (ai16EccExpectedErrByteNumber[2] == ai16EccTestErrByteNumber[2]) && \
                    (ai16EccExpectedErrByteNumber[3] == ai16EccTestErrByteNumber[3]))
                {
                    enRet = Ok; /* The result meets the expected */
                }
            }
        }
    }

    return enRet;
}

/**
 * @brief  Main function of EXMC NFC project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint8_t au8DevId[4];
    uint8_t u8TestErrCnt = 0U;

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock: */
    BSP_CLK_Init();

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EXCLK 60MHz */
    CLK_ClkDiv(CLK_CATE_ALL, (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                              CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | \
                              CLK_PCLK4_DIV2 | CLK_EXCLK_DIV4 | CLK_HCLK_DIV1));

    /* Initialize LED */
    BSP_IO_Init();
    BSP_LED_Init();

    /* Configure nandflash */
    MT29F2G08AB_Init();

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Read ID */
    MT29F2G08AB_ReadId(0UL, au8DevId, sizeof(au8DevId));
    if ((au8DevId[0] == MT29F2G08ABAEA_MANUFACTURER_ID) || \
        (au8DevId[1] == MT29F2G08ABAEA_DEVICE_ID1) || \
        (au8DevId[2] == MT29F2G08ABAEA_DEVICE_ID2) || \
        (au8DevId[3] == MT29F2G08ABAEA_DEVICE_ID3))
    {
        /* Erase nandflash. */
        if (Ok == MT29F2G08AB_EraseBlock(0UL))
        {
            if (Ok != MT29F2G08AB_MetaWithoutSpareTest(0UL))
            {
                u8TestErrCnt++;
            }

            if (Ok != MT29F2G08AB_MetaWithSpareTest(1UL))
            {
                u8TestErrCnt++;
            }

            if (Ok != MT29F2G08AB_HwEcc1BitTest(2UL))
            {
                u8TestErrCnt++;
            }

            if (Ok != MT29F2G08AB_HwEcc4BitsTest(3UL))
            {
                u8TestErrCnt++;
            }
        }
        else
        {
            u8TestErrCnt++;
        }
    }
    else
    {
        u8TestErrCnt++;
    }

    if (u8TestErrCnt > 0U)
    {
        BSP_LED_On(LED_RED);
    }
    else
    {
        BSP_LED_On(LED_BLUE);
    }

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
