/**
 *******************************************************************************
 * @file  cy62167ev30ll.c
 * @brief This midware file provides firmware functions to manage the SRAM 
 *        component library for cy62167ev30ll.
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
#include "cy62167ev30ll.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup CY62167EV30LL SRAM CY62167EV30LL
 * @{
 */

#if (BSP_CY62167EV30LL_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CY62167EV30LL_Local_Macros CY62167EV30LL Local Macros
 * @{
 */

/**
 * @defgroup CY62167EV30LL_Address_Information CY62167EV30LL Address Information
 * @{
 */
#define CY62167EV30LL_START_ADDRESS         (m_u32MemStartAddr)
#define CY62167EV30LL_END_ADDRESS           (m_u32MemStartAddr + m_u32MemByteSize)
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
/**
 * @defgroup CY62167EV30LL_Local_Variables CY62167EV30LL Local Variables
 * @{
 */

static uint32_t m_u32MemStartAddr = 0UL;
static uint32_t m_u32MemByteSize = 0UL;

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup CY62167EV30LL_Global_Functions CY62167EV30LL Global Functions
 * @{
 */

/**
 * @brief  Initialize SMC for CY62167EV30LL.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error                        Initialize unsuccessfully.
 */
en_result_t CY62167EV30LL_Init(void)
{
    BSP_SMC_CY62167EV30LL_Init();
    BSP_SMC_CY62167EV30LL_GetMemInfo(&m_u32MemStartAddr, &m_u32MemByteSize);

    return Ok;
}

/**
 * @brief  Get memory information.
 * @param  [out] pu32MemStartAddr       The pointer for memory start address
 * @param  [out] pu32MemByteSize        The pointer for memory size(unit: Byte)
 * @retval None
 */
void CY62167EV30LL_GetMemInfo(uint32_t *pu32MemStartAddr,
                                    uint32_t *pu32MemByteSize)
{
    BSP_SMC_CY62167EV30LL_GetMemInfo(pu32MemStartAddr, pu32MemByteSize);
}

/**
 * @brief  Write memory for byte.
 * @param  [in] u32Address              CY62167EV30LL memory address to write
 * @param  [in] au8SrcBuffer            Pointer to source buffer
 * @param  [in] u32BufferSize           Size of the buffer to write to memory
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The variable u32BufferSize value is 0.
 */
en_result_t CY62167EV30LL_WriteMem8(uint32_t u32Address,
                                    const uint8_t au8SrcBuffer[],
                                    uint32_t u32BufferSize)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if (0UL != u32BufferSize)
    {
        DDL_ASSERT(u32Address >= CY62167EV30LL_START_ADDRESS);
        DDL_ASSERT((u32Address + u32BufferSize) <= CY62167EV30LL_END_ADDRESS);

        for (i = 0UL; i < u32BufferSize; i++)
        {
            *(uint8_t *)(u32Address + i) = au8SrcBuffer[i];
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Read memory for byte.
 * @param  [in] u32Address              CY62167EV30LL memory address to read
 * @param  [out] au8DstBuffer           Pointer to destination buffer to write
 * @param  [in] u32BufferSize           Size of the buffer to read from memory
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       -The pointer au8DstBuffer value is NULL.
 *                                      -The variable u32BufferSize value is 0.
 */
en_result_t CY62167EV30LL_ReadMem8(uint32_t u32Address,
                                    uint8_t au8DstBuffer[],
                                    uint32_t u32BufferSize)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != au8DstBuffer) && (0UL != u32BufferSize))
    {
        DDL_ASSERT(u32Address >= CY62167EV30LL_START_ADDRESS);
        DDL_ASSERT((u32Address + u32BufferSize) <= CY62167EV30LL_END_ADDRESS);

        for (i = 0UL; i < u32BufferSize; i++)
        {
            au8DstBuffer[i] = *(uint8_t *)(u32Address + i);
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Write memory for half-word.
 * @param  [in] u32Address              CY62167EV30LL memory address to write
 * @param  [in] au16SrcBuffer           Pointer to source buffer
 * @param  [in] u32BufferSize           Size of the buffer to write to memory
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The variable u32BufferSize value is 0.
 */
en_result_t CY62167EV30LL_WriteMem16(uint32_t u32Address,
                                    const uint16_t au16SrcBuffer[],
                                    uint32_t u32BufferSize)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if (0UL != u32BufferSize)
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN_HALFWORD(u32Address));
        DDL_ASSERT(u32Address >= CY62167EV30LL_START_ADDRESS);
        DDL_ASSERT((u32Address + (u32BufferSize << 1UL)) <= CY62167EV30LL_END_ADDRESS);

        for (i = 0UL; i < u32BufferSize; i++)
        {
            *((uint16_t *)u32Address) = au16SrcBuffer[i];
            u32Address += 2UL;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Read memory for half-word.
 * @param  [in] u32Address              CY62167EV30LL memory address to read
 * @param  [out] au16DstBuffer          Pointer to destination buffer to write
 * @param  [in] u32BufferSize           Size of the buffer to read from memory
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       -The pointer au16DstBuffer value is NULL.
 *                                      -The variable u32BufferSize value is 0.
 */
en_result_t CY62167EV30LL_ReadMem16(uint32_t u32Address,
                                    uint16_t au16DstBuffer[],
                                    uint32_t u32BufferSize)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != au16DstBuffer) && (0UL != u32BufferSize))
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN_HALFWORD(u32Address));
        DDL_ASSERT(u32Address >= CY62167EV30LL_START_ADDRESS);
        DDL_ASSERT((u32Address + (u32BufferSize << 1UL)) <= CY62167EV30LL_END_ADDRESS);

        for (i = 0UL; i < u32BufferSize; i++)
        {
            au16DstBuffer[i] = *((uint16_t *)u32Address);
            u32Address += 2UL;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Write memory for word.
 * @param  [in] u32Address              CY62167EV30LL memory address to write
 * @param  [in] au32SrcBuffer           Pointer to source buffer
 * @param  [in] u32BufferSize           Size of the buffer to write to memory
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The variable u32BufferSize value is 0.
 */
en_result_t CY62167EV30LL_WriteMem32(uint32_t u32Address,
                                    const uint32_t au32SrcBuffer[],
                                    uint32_t u32BufferSize)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if (0UL != u32BufferSize)
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN_WORD(u32Address));
        DDL_ASSERT(u32Address >= CY62167EV30LL_START_ADDRESS);
        DDL_ASSERT((u32Address + (u32BufferSize << 2UL)) <= CY62167EV30LL_END_ADDRESS);

        for (i = 0UL; i < u32BufferSize; i++)
        {
            *((uint32_t *)u32Address) = au32SrcBuffer[i];
            u32Address += 4UL;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Read memory for word.
 * @param  [in] u32Address              CY62167EV30LL memory address to read
 * @param  [out] au32DstBufferBuf       Pointer to destination buffer to write
 * @param  [in] u32BufferSize           Size of the buffer to read from memory
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       -The pointer au32DstBufferBuf value is NULL.
 *                                      -The variable u32BufferSize value is 0.
 */
en_result_t CY62167EV30LL_ReadMem32(uint32_t u32Address,
                                    uint32_t au32DstBufferBuf[],
                                    uint32_t u32BufferSize)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != au32DstBufferBuf)
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN_WORD(u32Address));
        DDL_ASSERT(u32Address >= CY62167EV30LL_START_ADDRESS);
        DDL_ASSERT((u32Address + (u32BufferSize << 2UL)) <= CY62167EV30LL_END_ADDRESS);

        for (i = 0UL; i < u32BufferSize; i++)
        {
            au32DstBufferBuf[i] = *((uint32_t *)u32Address);
            u32Address += 4UL;
        }
        enRet = Ok;
    }

    return enRet;
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
