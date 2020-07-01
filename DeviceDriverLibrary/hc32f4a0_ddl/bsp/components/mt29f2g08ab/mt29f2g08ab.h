/**
 *******************************************************************************
 * @file  mt29f2g08ab.h
 * @brief This file contains all the functions prototypes of the nand flash 
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
#ifndef __MT29F2G08AB_H__
#define __MT29F2G08AB_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @addtogroup MT29F2G08AB
 * @{
 */

#if (BSP_MT29F2G08AB_ENABLE == BSP_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup MT29F2G08AB_Global_Macros MT29F2G08AB Global Macros
 * @{
 */

/**
 * @defgroup MT29F2G08AB_Device_Size MT29F2G08AB Device Size
 * @{
 */
#define MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE (2048UL)
#define MT29F2G08AB_SPARE_AREA_SIZE         (64UL)
#define MT29F2G08AB_PAGE_SIZE_WITH_SPARE    (MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE + \
                                             MT29F2G08AB_SPARE_AREA_SIZE)

#define MT29F2G08AB_PAGES_PER_BLOCK         (64UL)
#define MT29F2G08AB_BLOCKS_PER_PLANE        (1024UL)
#define MT29F2G08AB_PLANE_PER_DEVICE        (2UL)
#define MT29F2G08AB_DEVICE_PAGES            (MT29F2G08AB_PLANE_PER_DEVICE * \
                                             MT29F2G08AB_BLOCKS_PER_PLANE * \
                                             MT29F2G08AB_PAGES_PER_BLOCK)

#define MT29F2G08AB_PAGE_1BIT_ECC_VALUE_SIZE                                   \
((MT29F2G08AB_PAGE_SIZE_WITHOUT_SPARE / EXMC_NFC_ECC_CALCULATE_BLOCK_BYTES) * EXMC_NFC_1BIT_ECC_VALUE_BYTES)
/**
 * @}
 */

/**
 * @defgroup MT29F2G08AB_ID_Information MT29F2G08AB ID Information
 * @{
 */
#define MT29F2G08ABAEA_MANUFACTURER_ID      (0x2CU)
#define MT29F2G08ABAEA_DEVICE_ID1           (0xDAU)
#define MT29F2G08ABAEA_DEVICE_ID2           (0x90U)
#define MT29F2G08ABAEA_DEVICE_ID3           (0x95U)
/**
 * @}
 */

/**
 * @}
 */


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup MT29F2G08AB_Global_Functions
 * @{
 */
en_result_t MT29F2G08AB_Init(void);
en_result_t MT29F2G08AB_ReadId(uint32_t u32IdAddr,
                                    uint8_t au8DevId[],
                                    uint32_t u32NumBytes);
en_result_t MT29F2G08AB_GetStatus(uint32_t u32Timeout);
en_result_t MT29F2G08AB_EraseBlock(uint32_t u32BlockRowAddress);
en_result_t MT29F2G08AB_ReadPageMeta(uint32_t u32Page,
                                            uint8_t *pu8Buf,
                                            uint32_t u32NumBytes);
en_result_t MT29F2G08AB_WritePageMeta(uint32_t u32Page,
                                            const uint8_t *pu8Buf,
                                            uint32_t u32NumBytes);
en_result_t MT29F2G08AB_ReadPageHwEcc1Bit(uint32_t u32Page,
                                                    uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes);
en_result_t MT29F2G08AB_WritePageHwEcc1Bit(uint32_t u32Page,
                                                    const uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes);
en_result_t MT29F2G08AB_WritePageHwEcc4Bits(uint32_t u32Page,
                                                    const uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes);
en_result_t MT29F2G08AB_ReadPageHwEcc4Bits(uint32_t u32Page,
                                                    uint8_t *pu8Buf,
                                                    uint32_t u32NumBytes);

/* Implement the below functions for the specified BSP board */
en_result_t BSP_NFC_MT29F2G08AB_Init(void);
en_result_t BSP_NFC_MT29F2G08AB_ReadId(uint32_t u32IdAddr,
                                        uint8_t au8DevId[],
                                        uint32_t u32NumBytes);
uint32_t BSP_NFC_MT29F2G08AB_ReadStatus(void);
en_result_t BSP_NFC_MT29F2G08AB_EraseBlock(uint32_t u32BlockRowAddress);
en_result_t BSP_NFC_MT29F2G08AB_ReadPageMeta(uint32_t u32Page,
                                    uint8_t *pu8Data,
                                    uint32_t u32NumBytes);
en_result_t BSP_NFC_MT29F2G08AB_WritePageMeta(uint32_t u32Page,
                                    const uint8_t *pu8Data,
                                    uint32_t u32NumBytes);
en_result_t BSP_NFC_MT29F2G08AB_ReadPageHwEcc(uint32_t u32Page,
                                    uint8_t *pu8Data,
                                    uint32_t u32NumBytes);
en_result_t BSP_NFC_MT29F2G08AB_WritePageHwEcc(uint32_t u32Page,
                                    const uint8_t *pu8Data,
                                    uint32_t u32NumBytes);
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

#ifdef __cplusplus
}
#endif

#endif /* __MT29F2G08AB_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
