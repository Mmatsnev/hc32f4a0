/**
 *******************************************************************************
 * @file  s29gl064n90tfi03.h
 * @brief This file contains all the functions prototypes of the NOR Flash 
 *        component library for s29gl064n90tfi03.
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
#ifndef __S29GL064N90TFI03_H__
#define __S29GL064N90TFI03_H__

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
 * @addtogroup S29GL064N90TFI03
 * @{
 */

#if (BSP_S29GL064N90TFI03_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup S29GL064N90TFI03_Global_Macros S29GL064N90TFI03 Global Macros
 * @{
 */

/**
 * @defgroup S29GL064N90TFI03_ID_Information S29GL064N90TFI03 ID Information
 * @{
 */
#define S29GL064N90TFI03_MANUFACTURER_ID    (0x0001U)
#define S29GL064N90TFI03_DEVICE_ID1         (0x227EU)
#define S29GL064N90TFI03_DEVICE_ID2         (0x2210U)
#define S29GL064N90TFI03_DEVICE_ID3         (0x2201U)
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
 * @addtogroup S29GL064N90TFI03_Global_Functions S29GL064N90TFI03 Global Functions
 * @{
 */
en_result_t S29GL064_Init(void);
void S29GL064_GetMemInfo(uint32_t *pu32MemStartAddr,
                                        uint32_t *pu32SectorsNumber,
                                        uint32_t *pu32BytesPerSector,
                                        uint32_t *pu32BytesPerBufProgram);
void S29GL064_Reset(void);
en_result_t S29GL064_ReadId(uint16_t au16Id[], uint32_t u32Length);
en_result_t S29GL064_ReadCfiId(uint16_t au16Id[], uint32_t u32Length);
en_result_t S29GL064_EraseChip(void);
en_result_t S29GL064_EraseSector(uint32_t u32SectorAddress);
en_result_t S29GL064_Program(uint32_t u32ProgramAddress, uint16_t u16Data);
en_result_t S29GL064_ProgramBuffer(uint32_t u32ProgramAddress,
                                            uint16_t au16Data[],
                                            uint32_t u32NumHalfWords);
uint16_t S29GL064_Read(uint32_t u32ReadAddress);
en_result_t S29GL064_ReadBuffer(uint32_t u32ReadAddress,
                                    uint16_t au16DataBuf[],
                                    uint32_t u32NumHalfwords);

/* Implement the below functions for the specified BSP board */
en_result_t BSP_SMC_S29GL064_Init(void);
void BSP_SMC_S29GL064_GetMemInfo(uint32_t *pu32MemStartAddr,
                                        uint32_t *pu32SectorsNumber,
                                        uint32_t *pu32BytesPerSector,
                                        uint32_t *pu32BytesPerBufProgram);
en_result_t BSP_SMC_S29GL064_ReadId(uint32_t u32DevicBaseAddress,
                                        uint16_t au16Id[],
                                        uint32_t u32Length);
en_result_t BSP_SMC_S29GL064_ReadCfiId(uint32_t u32DevicBaseAddress,
                                            uint16_t au16Id[],
                                            uint32_t u32Length);
en_result_t BSP_SMC_S29GL064_GetStatus(uint32_t u32DevicBaseAddress,
                                            uint32_t Timeout);
void BSP_SMC_S29GL064_Reset(uint32_t u32DevicBaseAddress);
en_result_t BSP_SMC_S29GL064_EraseChip(uint32_t u32DevicBaseAddress);
void BSP_SMC_S29GL064_EraseSector(uint32_t u32DevicBaseAddress,
                                        uint32_t u32SectorAddress);
en_result_t BSP_SMC_S29GL064_Program(uint32_t u32DevicBaseAddress,
                                            uint32_t u32ProgramAddress,
                                            uint16_t u16Data);
uint16_t BSP_SMC_S29GL064_Read(uint32_t u32DevicBaseAddress,
                                    uint32_t u32ReadAddress);
en_result_t BSP_SMC_S29GL064_ProgramBuffer(uint32_t u32DevicBaseAddress,
                                                  uint32_t u32ProgramAddress,
                                                  const uint16_t au16Data[],
                                                  uint32_t u32NumHalfWords);
en_result_t BSP_SMC_S29GL064_ReadBuffer(uint32_t u32DevicBaseAddress,
                                            uint32_t u32ReadAddress,
                                            uint16_t au16DataBuf[],
                                            uint32_t u32NumHalfwords);

/**
 * @}
 */

#endif /* BSP_S29GL064N90TFI03_ENABLE */

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

#endif /* __S29GL064N90TFI03_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
