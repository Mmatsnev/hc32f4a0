/**
 *******************************************************************************
 * @file  cy62167ev30ll.h
 * @brief This file contains all the functions prototypes of the SRAM component
 *        library for cy62167ev30ll.
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
#ifndef __CY62167EV30LL_H__
#define __CY62167EV30LL_H__

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
 * @addtogroup CY62167EV30LL
 * @{
 */

#if (BSP_CY62167EV30LL_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup CY62167EV30LL_Global_Functions CY62167EV30LL Global Functions
 * @{
 */

en_result_t CY62167EV30LL_Init(void);

void CY62167EV30LL_GetMemInfo(uint32_t *pu32MemStartAddr,
                                    uint32_t *pu32MemByteSize);

en_result_t CY62167EV30LL_WriteMem8(uint32_t u32Address,
                                    const uint8_t au8SrcBuffer[],
                                    uint32_t u32BufferSize);
en_result_t CY62167EV30LL_ReadMem8(uint32_t u32Address,
                                    uint8_t au8DstBuffer[],
                                    uint32_t u32BufferSize);
en_result_t CY62167EV30LL_WriteMem16(uint32_t u32Address,
                                    const uint16_t au16SrcBuffer[],
                                    uint32_t u32BufferSize);
en_result_t CY62167EV30LL_ReadMem16(uint32_t u32Address,
                                    uint16_t au16DstBuffer[],
                                    uint32_t u32BufferSize);
en_result_t CY62167EV30LL_WriteMem32(uint32_t u32Address,
                                    const uint32_t au32SrcBuffer[],
                                    uint32_t u32BufferSize);
en_result_t CY62167EV30LL_ReadMem32(uint32_t u32Address,
                                    uint32_t au32DstBufferBuf[],
                                    uint32_t u32BufferSize);

/* Implement the below functions for the specified BSP board */
en_result_t BSP_SMC_CY62167EV30LL_Init(void);

void BSP_SMC_CY62167EV30LL_GetMemInfo(uint32_t *pu32MemStartAddr,
                                            uint32_t *pu32MemByteSize);

/**
 * @}
 */

#endif /* BSP_CY62167EV30LL_ENABLE */

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

#endif /* __CY62167EV30LL_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
