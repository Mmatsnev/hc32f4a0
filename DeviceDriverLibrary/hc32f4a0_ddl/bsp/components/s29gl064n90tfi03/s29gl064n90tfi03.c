/**
 *******************************************************************************
 * @file  s29gl064n90tfi03.c
 * @brief This midware file provides firmware functions to manage the NOR Flash 
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "s29gl064n90tfi03.h"
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
 * @defgroup S29GL064N90TFI03 NOR Flash S29GL064N90TFI03
 * @{
 */

#if (BSP_S29GL064N90TFI03_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup S29GL064N90TFI03_Local_Macros S29GL064N90TFI03 Local Macros
 * @{
 */

/**
 * @defgroup EXMC_SMC_Check_Parameters_Validity EXMC SMC Check Parameters Validity
 * @{
 */

#define IS_S29GL064_BUFPROMGRAM_SIZE(halfwords)                                \
(   (0UL < halfwords)             &&                                           \
    ((halfwords << 1UL) <= S29GL064_BYTES_PER_BUFPROMGRAM))

#define IS_S29GL064_ADDRESS(address, halfwords)                                \
(   (S29GL064_START_ADDRESS <= address) &&                                     \
    (address + (halfwords << 1UL)) <= S29GL064_END_ADDRESS)

/**
 * @}
 */

/**
 * @defgroup S29GL064_Operation_Timeout S29GL064 Operation Timeout
 * @{
 */

/* NOR operations Timeout definitions */
#define PROGRAM_TIMEOUT                     (0x01000000UL)  /* Program NOR  timeout */
#define ERASECHIP_TIMEOUT                   (0x30000000UL)  /* Erase NOR chip timeout */
#define ERASESECTOR_TIMEOUT                 (0x01000000UL)  /* Erase NOR block timeout */
/**
 * @}
 */

/**
 * @defgroup S29GL064_Address_Information S29GL064 Address Information
 * @{
 */
#define S29GL064_START_ADDRESS              (m_u32MemStartAddr)
#define S29GL064_END_ADDRESS                (S29GL064_START_ADDRESS + S29GL064_SIZE - 1UL)
#define S29GL064_SECTOR_NUMBER              (m_u32SectorsNumber)
#define S29GL064_BYTES_PER_SECTOR           (m_u32BytesPerSector)
#define S29GL064_SIZE                       (S29GL064_SECTOR_NUMBER * S29GL064_BYTES_PER_SECTOR)
#define S29GL064_BYTES_PER_BUFPROMGRAM      (m_u32BytesPerBufProgram)
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
 * @defgroup S29GL064N90TFI03_Local_Variables S29GL064N90TFI03 Local Variables
 * @{
 */

static uint32_t m_u32MemStartAddr = 0UL;
static uint32_t m_u32SectorsNumber = 0UL;
static uint32_t m_u32BytesPerSector = 0UL;
static uint32_t m_u32BytesPerBufProgram = 0UL;

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup S29GL064N90TFI03_Global_Functions S29GL064N90TFI03 Global Functions
 * @{
 */

/**
 * @brief  Initialize SMC for S29GL064N90TFI03.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error                        Initialize unsuccessfully.
 */
en_result_t S29GL064_Init(void)
{
    BSP_SMC_S29GL064_Init();
    BSP_SMC_S29GL064_GetMemInfo(&m_u32MemStartAddr, \
                                &m_u32SectorsNumber, \
                                &m_u32BytesPerSector, \
                                &m_u32BytesPerBufProgram);

    return Ok;
}

/**
 * @brief  Get memory information.
 * @param  [out] pu32MemStartAddr       The pointer for memory start address
 * @param  [out] pu32SectorsNumber      The pointer for device sector
 * @param  [out] pu32BytesPerSector     The pointer for bytes per sector(unit: Byte)
 * @param  [out] pu32BytesPerBufProgram The pointer for bytes per buffer program(unit: Byte)
 * @retval None
 */
void S29GL064_GetMemInfo(uint32_t *pu32MemStartAddr,
                                        uint32_t *pu32SectorsNumber,
                                        uint32_t *pu32BytesPerSector,
                                        uint32_t *pu32BytesPerBufProgram)
{
    BSP_SMC_S29GL064_GetMemInfo(pu32MemStartAddr,
                                pu32SectorsNumber,
                                pu32BytesPerSector,
                                pu32BytesPerBufProgram);
}

/**
 * @brief  Reset the NOR memory to Read mode
 * @param  None
 * @retval None
 */
void S29GL064_Reset(void)
{
    BSP_SMC_S29GL064_Reset(S29GL064_START_ADDRESS);
}

/**
 * @brief  Read NOR flash IDs
 * @param  [out] au16Id                 ID buffer
 * @param  [in] u32Length               Number ID to read
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred
 *   @arg  ErrorInvalidParameter:       The pointer au16Id value is NULL
 */
en_result_t S29GL064_ReadId(uint16_t au16Id[], uint32_t u32Length)
{
    return BSP_SMC_S29GL064_ReadId(S29GL064_START_ADDRESS, au16Id, u32Length);
}

/**
 * @brief  Read NOR flash CFI IDs
 * @param  [out] au16Id                 ID buffer
 * @param  [in] u32Length               Number ID to read
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred
 *   @arg  ErrorInvalidParameter:       The pointer au16Id value is NULL
 */
en_result_t S29GL064_ReadCfiId(uint16_t au16Id[], uint32_t u32Length)
{
    return BSP_SMC_S29GL064_ReadCfiId(S29GL064_START_ADDRESS, au16Id, u32Length);
}

/**
 * @brief  Erase the entire NOR chip.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred
 *   @arg  Error:                       Errors occurred
 */
en_result_t S29GL064_EraseChip(void)
{
    en_result_t enRet = Ok;

    BSP_SMC_S29GL064_EraseChip(S29GL064_START_ADDRESS);

   /* Return the NOR memory status */
    if (BSP_SMC_S29GL064_GetStatus(S29GL064_START_ADDRESS, ERASECHIP_TIMEOUT) != Ok)
    {
        enRet = Error;
    }

    return enRet;
}

/**
 * @brief  Erase the specified block of the NOR memory
 * @param  [in] u32SectorAddress        sector address
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred
 *   @arg  Error:                       Errors occurred
 */
en_result_t S29GL064_EraseSector(uint32_t u32SectorAddress)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(u32SectorAddress >= S29GL064_START_ADDRESS);

    BSP_SMC_S29GL064_EraseSector(S29GL064_START_ADDRESS, u32SectorAddress);

    /* Return the NOR memory status */
    if (BSP_SMC_S29GL064_GetStatus(S29GL064_START_ADDRESS, ERASESECTOR_TIMEOUT) != Ok)
    {
        enRet = Error;
    }

    return enRet;
}

/**
 * @brief  Write memory for half-word.
 * @param  [in] u32ProgramAddress       Memory address to write
 * @param  [in] u16Data                 Data buffer to write
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred
 *   @arg  Error:                       Errors occurred
 */
en_result_t S29GL064_Program(uint32_t u32ProgramAddress, uint16_t u16Data)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_ADDRESS_ALIGN_HALFWORD(u32ProgramAddress));
    DDL_ASSERT(u32ProgramAddress >= S29GL064_START_ADDRESS);

    BSP_SMC_S29GL064_Program(S29GL064_START_ADDRESS, u32ProgramAddress, u16Data);

    /* Return the NOR memory status */
    if (BSP_SMC_S29GL064_GetStatus(S29GL064_START_ADDRESS, ERASESECTOR_TIMEOUT) != Ok)
    {
        enRet = Error;
    }

    return enRet;
}

/**
 * @brief  Write memory for half-word.
 * @param  [in] u32ProgramAddress       Memory address to write
 * @param  [in] au16Data                Data buffer to write
 * @param  [in] u32NumHalfwords         Number half-word to write
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer au16Data value is NULL.
 */
en_result_t S29GL064_ProgramBuffer(uint32_t u32ProgramAddress,
                                            uint16_t au16Data[],
                                            uint32_t u32NumHalfwords)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != au16Data)
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN_HALFWORD(u32ProgramAddress));
        DDL_ASSERT(IS_S29GL064_BUFPROMGRAM_SIZE(u32NumHalfwords));
        DDL_ASSERT(IS_S29GL064_ADDRESS(u32ProgramAddress, u32NumHalfwords));

        BSP_SMC_S29GL064_ProgramBuffer(S29GL064_START_ADDRESS,
                                u32ProgramAddress,
                                au16Data,
                                u32NumHalfwords);

        /* Return the NOR memory status */
        if (BSP_SMC_S29GL064_GetStatus(S29GL064_START_ADDRESS, ERASESECTOR_TIMEOUT) == Ok)
        {
            enRet = Ok;
        }
        else
        {
            enRet = Error;
        }
    }

    return enRet;
}

/**
 * @brief  Read memory for half-word.
 * @param  [in] u32ReadAddress          Memory address to read
 * @retval Data of the specified address
 */
uint16_t S29GL064_Read(uint32_t u32ReadAddress)
{
    DDL_ASSERT(IS_ADDRESS_ALIGN_HALFWORD(u32ReadAddress));
    DDL_ASSERT(IS_S29GL064_ADDRESS(u32ReadAddress, 1UL));

    return BSP_SMC_S29GL064_Read(S29GL064_START_ADDRESS, u32ReadAddress);
}

/**
 * @brief  Read memory for half-word.
 * @param  [in] u32ReadAddress          Memory address to read
 * @param  [out] au16Data               Data buffer to read
 * @param  [in] u32NumHalfwords         Number half-word to read
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer au16Data value is NULL.
 */
en_result_t S29GL064_ReadBuffer(uint32_t u32ReadAddress,
                                    uint16_t au16Data[],
                                    uint32_t u32NumHalfwords)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != au16Data) && (u32NumHalfwords > 0UL))
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN_HALFWORD(u32ReadAddress));
        DDL_ASSERT(IS_S29GL064_ADDRESS(u32ReadAddress, u32NumHalfwords));

        enRet = BSP_SMC_S29GL064_ReadBuffer(S29GL064_START_ADDRESS,
                                            u32ReadAddress,
                                            au16Data,
                                            u32NumHalfwords);
    }

    return enRet;
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
