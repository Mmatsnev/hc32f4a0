/**
 *******************************************************************************
 * @file  1bit_per_512byte_ecc.c
 * @brief This file contains all the functions prototypes of the 1bit/512bytes 
 *        ECC
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
#include "hc32_common.h"

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
/**
 * @addtogroup EXMC_NFC_Nandflash_MT29F2G08AB_Local_Macros
 * @{
 */
#define BIT0(x)                 (((x) & 0x01U) >> 0U)
#define BIT1(x)                 (((x) & 0x02U) >> 1U)
#define BIT2(x)                 (((x) & 0x04U) >> 2U)
#define BIT3(x)                 (((x) & 0x08U) >> 3U)
#define BIT4(x)                 (((x) & 0x10U) >> 4U)
#define BIT5(x)                 (((x) & 0x20U) >> 5U)
#define BIT6(x)                 (((x) & 0x40U) >> 6U)
#define BIT7(x)                 (((x) & 0x80U) >> 7U)

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
 * @addtogroup EXMC_NFC_Nandflash_MT29F2G08AB_Local_Variable
 * @{
 */

/*
 * Pre-calculated 256-way 1 byte column parity
 */
static const uint8_t nand_ecc_precalc_table[256] = {
    0x00, 0xD5, 0xD6, 0x03, 0xD9, 0x0C, 0x0F, 0xDA, 0xDA, 0x0F, 0x0C, 0xD9, 0x03, 0xD6, 0xD5, 0x00,
    0xE5, 0x30, 0x33, 0xE6, 0x3C, 0xE9, 0xEA, 0x3F, 0x3F, 0xEA, 0xE9, 0x3C, 0xE6, 0x33, 0x30, 0xE5,
    0xE6, 0x33, 0x30, 0xE5, 0x3F, 0xEA, 0xE9, 0x3C, 0x3C, 0xE9, 0xEA, 0x3F, 0xE5, 0x30, 0x33, 0xE6,
    0x03, 0xD6, 0xD5, 0x00, 0xDA, 0x0F, 0x0C, 0xD9, 0xD9, 0x0C, 0x0F, 0xDA, 0x00, 0xD5, 0xD6, 0x03,
    0xE9, 0x3C, 0x3F, 0xEA, 0x30, 0xE5, 0xE6, 0x33, 0x33, 0xE6, 0xE5, 0x30, 0xEA, 0x3F, 0x3C, 0xE9,
    0x0C, 0xD9, 0xDA, 0x0F, 0xD5, 0x00, 0x03, 0xD6, 0xD6, 0x03, 0x00, 0xD5, 0x0F, 0xDA, 0xD9, 0x0C,
    0x0F, 0xDA, 0xD9, 0x0C, 0xD6, 0x03, 0x00, 0xD5, 0xD5, 0x00, 0x03, 0xD6, 0x0C, 0xD9, 0xDA, 0x0F,
    0xEA, 0x3F, 0x3C, 0xE9, 0x33, 0xE6, 0xE5, 0x30, 0x30, 0xE5, 0xE6, 0x33, 0xE9, 0x3C, 0x3F, 0xEA,
    0xEA, 0x3F, 0x3C, 0xE9, 0x33, 0xE6, 0xE5, 0x30, 0x30, 0xE5, 0xE6, 0x33, 0xE9, 0x3C, 0x3F, 0xEA,
    0x0F, 0xDA, 0xD9, 0x0C, 0xD6, 0x03, 0x00, 0xD5, 0xD5, 0x00, 0x03, 0xD6, 0x0C, 0xD9, 0xDA, 0x0F,
    0x0C, 0xD9, 0xDA, 0x0F, 0xD5, 0x00, 0x03, 0xD6, 0xD6, 0x03, 0x00, 0xD5, 0x0F, 0xDA, 0xD9, 0x0C,
    0xE9, 0x3C, 0x3F, 0xEA, 0x30, 0xE5, 0xE6, 0x33, 0x33, 0xE6, 0xE5, 0x30, 0xEA, 0x3F, 0x3C, 0xE9,
    0x03, 0xD6, 0xD5, 0x00, 0xDA, 0x0F, 0x0C, 0xD9, 0xD9, 0x0C, 0x0F, 0xDA, 0x00, 0xD5, 0xD6, 0x03,
    0xE6, 0x33, 0x30, 0xE5, 0x3F, 0xEA, 0xE9, 0x3C, 0x3C, 0xE9, 0xEA, 0x3F, 0xE5, 0x30, 0x33, 0xE6,
    0xE5, 0x30, 0x33, 0xE6, 0x3C, 0xE9, 0xEA, 0x3F, 0x3F, 0xEA, 0xE9, 0x3C, 0xE6, 0x33, 0x30, 0xE5,
    0x00, 0xD5, 0xD6, 0x03, 0xD9, 0x0C, 0x0F, 0xDA, 0xDA, 0x0F, 0x0C, 0xD9, 0x03, 0xD6, 0xD5, 0x00,
};

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @addtogroup EXMC_NFC_Nandflash_MT29F2G08AB_Global_Functions
 * @{
 */

/**
 * @brief  Calculate 3-byte ECC for 512-bytes
 * @param  [in] au8Data                 The flash data
 * @param  [out] au8Ecc                 ECC value
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       au8Buf == NULL or au8Ecc == NULL
 * @note   The paramter size of array au8Ecc[] must be larger than 2 and paramter
 *         size of array au8Data[] must be equal 512
 */
en_result_t NFC_SwCalculateEcc1Bit(const uint8_t au8Data[],
                                            uint8_t au8Ecc[])
{
    uint16_t i, u16BufData;
    uint32_t u32BufAddr;
    en_result_t enRet = ErrorInvalidParameter;
    uint8_t u8Idx, u8Reg1, u8Reg2, u8Reg3, u8Tmp1, u8Tmp2;

    if ((NULL != au8Data) && (NULL != au8Ecc))
    {
        u32BufAddr = (uint32_t)au8Data;
        u8Reg1 = u8Reg2 = u8Reg3 = 0xFFU;

        for(i = 0U; i < (512U / 2U); i++)
        {
            u16BufData = RW_MEM16(u32BufAddr);
            u8Idx = nand_ecc_precalc_table[u16BufData & 0xFFU];
            u8Reg1 ^= (u8Idx & 0x7FU);
            u8Idx = nand_ecc_precalc_table[(u16BufData >> 8U) & 0xFFU];
            u8Reg1 ^= (u8Idx & 0xBFU);

            if((nand_ecc_precalc_table[u16BufData & 0xFFU] ^ nand_ecc_precalc_table[(u16BufData >> 8U) & 0xFFU]) & 0xC0U)
            {
                u8Reg3 ^= (uint8_t) i;
                u8Reg2 ^= (uint8_t)(~((uint8_t) i));
            }
            u32BufAddr += 2UL;
        }

        u8Tmp1  = (u8Reg3 & 0x80U) >> 0U; /* B7 -> B7 */
        u8Tmp1 |= (u8Reg2 & 0x80U) >> 1U; /* B7 -> B6 */
        u8Tmp1 |= (u8Reg3 & 0x40U) >> 1U; /* B6 -> B5 */
        u8Tmp1 |= (u8Reg2 & 0x40U) >> 2U; /* B6 -> B4 */
        u8Tmp1 |= (u8Reg3 & 0x20U) >> 2U; /* B5 -> B3 */
        u8Tmp1 |= (u8Reg2 & 0x20U) >> 3U; /* B5 -> B2 */
        u8Tmp1 |= (u8Reg3 & 0x10U) >> 3U; /* B4 -> B1 */
        u8Tmp1 |= (u8Reg2 & 0x10U) >> 4U; /* B4 -> B0 */

        u8Tmp2  = (uint8_t)((u8Reg3 & 0x08U) << 4U); /* B3 -> B7 */
        u8Tmp2 |= (uint8_t)((u8Reg2 & 0x08U) << 3U); /* B3 -> B6 */
        u8Tmp2 |= (uint8_t)((u8Reg3 & 0x04U) << 3U); /* B2 -> B5 */
        u8Tmp2 |= (uint8_t)((u8Reg2 & 0x04U) << 2U); /* B2 -> B4 */
        u8Tmp2 |= (uint8_t)((u8Reg3 & 0x02U) << 2U); /* B1 -> B3 */
        u8Tmp2 |= (uint8_t)((u8Reg2 & 0x02U) << 1U); /* B1 -> B2 */
        u8Tmp2 |= (uint8_t)((u8Reg3 & 0x01U) << 1U); /* B0 -> B1 */
        u8Tmp2 |= (uint8_t)((u8Reg2 & 0x01U) << 0U); /* B7 -> B0 */

        au8Ecc[0] = u8Reg1;
        au8Ecc[1] = u8Tmp2;
        au8Ecc[2] = u8Tmp1;
    }

    return enRet;
}

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
