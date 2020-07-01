/**
 *******************************************************************************
 * @file  sdioc/sdioc_mmc/source/mmc.c
 * @brief This file provides firmware functions to manage the Secure Digital(MMC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Yangjp          First version
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
#include "mmc.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup SDIOC_Mmc_Card
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup MMC_Local_Macros MMC Local Macros
 * @{
 */

/* Block size is 512 bytes */
#define MMC_CARD_BLOCK_SIZE                     (512U)

/* Switch to high speed mode */
#define MMC_SET_FUNC_HIGH_SPEED                 (0x03B90100UL)

/* Log Block number for 2 G bytes Cards */
#define MMC_CARD_CAPACITY                       (0x400000UL)

/* Switch the bus width */
#define MMC_CMD6_BUS_WIDTH_1BIT                 (0x03B70000UL)
#define MMC_CMD6_BUS_WIDTH_4BIT                 (0x03B70100UL)
#define MMC_CMD6_BUS_WIDTH_8BIT                 (0x03B70200UL)

/* MMC switching function maximum attempts */
#define MMC_MAX_SWITCH_FUNC_TRIAL               (0x0000FFFFUL)

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
static void MMC_DmaTransferConfig(const stc_mmc_handle_t *handle, uint8_t u8Ch, uint32_t u32SrcAddr,
                                  uint32_t u32DestAddr, uint16_t u16BlkSize, uint16_t u16TransCnt);
static en_result_t MMC_SetSpeedMode(stc_mmc_handle_t *handle);
static en_result_t MMC_SetBusWidth(stc_mmc_handle_t *handle);
static en_result_t MMC_InitCard(stc_mmc_handle_t *handle);
static en_result_t MMC_PowerCmd(stc_mmc_handle_t *handle, en_functional_state_t enNewSta);
static en_result_t MMC_GetCurrCardStatus(stc_mmc_handle_t *handle, uint32_t *pu32CardStatus);
static en_result_t MMC_ReadWriteFifo(stc_mmc_handle_t *handle, const stc_sdioc_data_init_t *pstcDataCfg, uint8_t pu8Data[], uint32_t u32Timeout);
static void MMC_ExtractCardCSD(stc_mmc_handle_t *handle);

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup MMC_Global_Functions MMC Global Functions
 * @{
 */

/**
 * @brief  De-Initialize MMC.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: MMC De-Initialize success
 *           - ErrorInvalidParameter: handle == NULL
 */
en_result_t MMC_DeInit (stc_mmc_handle_t *handle)
{
    en_result_t enRet = Ok;

    if (NULL == handle)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Set MMC power state to off */
        MMC_PowerCmd(handle, Disable);
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
    }

    return enRet;
}

/**
 * @brief  Initialize MMC.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: MMC Initialize success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or An invalid parameter was
 *                                    write to the send command
 *           - ErrorTimeout: Send command timeout
 *           - ErrorInvalidMode: The Bus clock frequency is too high
 */
en_result_t MMC_Init(stc_mmc_handle_t *handle)
{
    en_result_t enRet = Ok;
    stc_sdioc_init_t stcSdInit;
    uint16_t u16ClkDiv = 0U;

    if (NULL == handle)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check the SDIOC clock is over 26Mhz or 52Mhz */
        enRet = SDIOC_VerifyClockDiv(SDIOC_MODE_MMC, handle->stcSdiocInit.u8SpeedMode, handle->stcSdiocInit.u16ClockDiv);
        if (Ok != enRet)
        {
            return ErrorInvalidMode;
        }

        /* Default SDIOC configuration for SD card initialization */
        SDIOC_StructInit(&stcSdInit);
        enRet = SDIOC_GetOptimumClockDiv(SDIOC_OUTPUT_CLK_FREQ_400K, &u16ClkDiv);
        if (Ok != enRet)
        {
            return ErrorInvalidMode;
        }
        stcSdInit.u16ClockDiv = u16ClkDiv;
        stcSdInit.u32Mode = handle->stcSdiocInit.u32Mode;
        stcSdInit.u8CardDetectSelect = handle->stcSdiocInit.u8CardDetectSelect;

        /* Set Power State to ON */
        SDIOC_PowerCmd(handle->SDIOCx, Enable);
        /* Initialize SDIOC with default configure */
        SDIOC_Init(handle->SDIOCx, &stcSdInit);
        /* Wait for the SDIOC to initialization */
        DDL_DelayMS(2U);

        /* Identify card operating voltage */
        enRet = MMC_PowerCmd(handle, Enable);
        if (Ok != enRet)
        {
            return enRet;
        }

        /* Card initialization */
        enRet = MMC_InitCard(handle);
        if (Ok != enRet)
        {
            return enRet;
        }

        /* Configure MMC Bus width */
        enRet = MMC_SetBusWidth(handle);
        if (Ok != enRet)
        {
            return enRet;
        }

        /* Configure MMC High speed mode */
        enRet = MMC_SetSpeedMode(handle);

        /* Initialize the error code */
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        /* Initialize the MMC operation */
        handle->u32Context = MMC_CONTEXT_NONE;
    }

    return enRet;
}

/**
 * @brief  Get the current MMC card state.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [out] puCardState            Pointer to a @ref en_mmc_card_state_t enumeration
 * @retval An en_result_t enumeration value:
 *           - Ok: Get MMC card state success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or puCardState == NULL or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_GetCardState(stc_mmc_handle_t *handle, en_mmc_card_state_t *puCardState)
{
    en_result_t enRet = Ok;
    uint32_t u32Response = 0UL;

    if ((NULL == puCardState) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        enRet = MMC_GetCurrCardStatus(handle, &u32Response);
        *puCardState = (en_mmc_card_state_t)(uint32_t)((u32Response >> SDMMC_STATUS_CURRENT_STATE_POS) & 0x0FU);
    }

    return enRet;
}

/**
 * @brief  Get information of the card which CID register.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [out] pstcCID                Pointer to a @ref stc_mmc_card_cid_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Get MMC card CID register success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pstcCID == NULL
 */
en_result_t MMC_GetCardCID(const stc_mmc_handle_t *handle, stc_mmc_card_cid_t *pstcCID)
{
    en_result_t enRet = Ok;

    if ((NULL == pstcCID) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcCID->u8ManufacturerID   = (uint8_t)((handle->u32CID[3] & 0xFF000000U) >> 24U);
        pstcCID->u8Reserved1        = (uint8_t)((handle->u32CID[3] & 0x00FC0000U) >> 18U);
        pstcCID->u8DeviceBGA        = (uint8_t)((handle->u32CID[3] & 0x00030000U) >> 16U);
        pstcCID->u16OemAppID        = (uint8_t)((handle->u32CID[3] & 0x0000FF00U) >> 8U);
        pstcCID->u32ProductName1    = (uint32_t)(((handle->u32CID[3] & 0x000000FFU) << 24U) | ((handle->u32CID[2] & 0xFFFFFF00U) >> 8U));
        pstcCID->u16ProductName2    = (uint16_t)(((handle->u32CID[2] & 0x000000FFU) << 8U) | ((handle->u32CID[1] & 0xFF000000U) >> 24U));
        pstcCID->u8ProductRevision  = (uint8_t)((handle->u32CID[1] & 0x00FF0000U) >> 16U);
        pstcCID->u32ProductSN       = (uint32_t)(((handle->u32CID[1] & 0x0000FFFFU) << 16U) | ((handle->u32CID[0] & 0xFFFF0000U) >> 16U));
        pstcCID->u8ManufactDate     = (uint8_t)((handle->u32CID[0] & 0x0000FF00U) >> 8U);
        pstcCID->u8CRCChecksum      = (uint8_t)((handle->u32CID[0] & 0x000000FEU) >> 1U);
        pstcCID->u8Reserved2        = 1U;
    }

    return enRet;
}

/**
 * @brief  Get information of the card which CSD register.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [out] pstcCSD                Pointer to a @ref stc_mmc_card_csd_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Get MMC card CSD register success
 *           - ErrorInvalidParameter: handle == NULL or pstcCSD == NULL
 */
en_result_t MMC_GetCardCSD(stc_mmc_handle_t *handle, stc_mmc_card_csd_t *pstcCSD)
{
    en_result_t enRet = Ok;
    uint32_t u32Temp = 0UL;

    if ((NULL == handle) || (NULL == pstcCSD))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        u32Temp = (handle->u32CSD[3] & 0xFF000000U) >> 24U;
        pstcCSD->u8CSDStruct          = (uint8_t)((u32Temp & 0xC0U) >> 6U);
        pstcCSD->u8SysSpecVersion     = (uint8_t)((u32Temp & 0x3CU) >> 2U);
        pstcCSD->u8Reserved1          = 0U;
        pstcCSD->u8TAAC               = (uint8_t)((handle->u32CSD[3] & 0x00FF0000U) >> 16U);
        pstcCSD->u8NSAC               = (uint8_t)((handle->u32CSD[3] & 0x0000FF00U) >> 8U);
        pstcCSD->u8MaxBusClkFreq      = (uint8_t)(handle->u32CSD[3] & 0x000000FFU);
        pstcCSD->u16CardCmdClass      = (uint16_t)((handle->u32CSD[2] & 0xFFF00000U) >> 20U);
        pstcCSD->u8ReadBlockLen       = (uint8_t)((handle->u32CSD[2] & 0x000F0000U) >> 16U);

        u32Temp = (handle->u32CSD[2] & 0x0000F000U) >> 12U;
        pstcCSD->u8BlockReadPartial   = (uint8_t)(u32Temp & 0x08U);
        pstcCSD->u8WriteBlockMisalign = (uint8_t)(u32Temp & 0x04U);
        pstcCSD->u8ReadBlockMisalign  = (uint8_t)(u32Temp & 0x02U);
        pstcCSD->u8DSRImplement       = (uint8_t)(u32Temp & 0x01U);
        pstcCSD->u8Reserved2          = 0U;

        u32Temp = (uint8_t)((handle->u32CSD[1] & 0xFF000000U) >> 24U);
        pstcCSD->u32DeviceSize        = (uint32_t)(((handle->u32CSD[2] & 0x000003FFU) << 2U) | ((u32Temp & 0xC0U) >> 6U));
        pstcCSD->u8MaxReadCurrVDDMin  = (uint8_t)((u32Temp & 0x38U) >> 3U);
        pstcCSD->u8MaxReadCurrVDDMax  = (uint8_t)(u32Temp & 0x07U);

        u32Temp = (uint8_t)((handle->u32CSD[1] & 0x00FF0000U) >> 16U);
        pstcCSD->u8MaxWriteCurrVDDMin = (uint8_t)((u32Temp & 0xE0U) >> 5U);
        pstcCSD->u8MaxWriteCurrVDDMax = (uint8_t)((u32Temp & 0x1CU) >> 2U);
        pstcCSD->u8DeviceSizeMul      = (uint8_t)((u32Temp & 0x03U) << 1U);

        u32Temp = (uint8_t)((handle->u32CSD[1] & 0x0000FF00U) >> 8U);
        pstcCSD->u8DeviceSizeMul     |= (uint8_t)((u32Temp & 0x80U) >> 7U);

        handle->stcMmcCardInfo.u32BlockNbr     = (pstcCSD->u32DeviceSize + 1U) ;
        handle->stcMmcCardInfo.u32BlockNbr    *= (1UL << (pstcCSD->u8DeviceSizeMul + 2U));
        handle->stcMmcCardInfo.u32BlockSize    = 1UL << (pstcCSD->u8ReadBlockLen);
        if (handle->stcMmcCardInfo.u32BlockSize >= 512U)
        {
            handle->stcMmcCardInfo.u32LogBlockNbr = handle->stcMmcCardInfo.u32BlockNbr * (handle->stcMmcCardInfo.u32BlockSize / 512U);
        }
        else
        {
            handle->stcMmcCardInfo.u32LogBlockNbr = (handle->stcMmcCardInfo.u32BlockNbr / 512U) * handle->stcMmcCardInfo.u32BlockSize;
        }
        handle->stcMmcCardInfo.u32LogBlockSize = 512UL;

        pstcCSD->u8EraseGroupSize     = (uint8_t)((u32Temp & 0x7CU) >> 2U);
        pstcCSD->u8EraseGroupSizeMul  = (uint8_t)((u32Temp & 0x03U) << 3U);

        u32Temp = (uint8_t)(handle->u32CSD[1] & 0x000000FFU);
        pstcCSD->u8EraseGroupSizeMul  |= (uint8_t)((u32Temp & 0xE0U) >> 5U);
        pstcCSD->u8WriteProtectGrpSize = (uint8_t)(u32Temp & 0x1FU);

        u32Temp = (uint8_t)((handle->u32CSD[0] & 0xFF000000U) >> 24U);
        pstcCSD->u8WriteProtectGrpEn = (uint8_t)((u32Temp & 0x80U) >> 7U);
        pstcCSD->u8ManuDefaultECC    = (uint8_t)((u32Temp & 0x60U) >> 5U);
        pstcCSD->u8WriteSpeedFactor  = (uint8_t)((u32Temp & 0x1CU) >> 2U);
        pstcCSD->u8MaxWriteBlockLen  = (uint8_t)((u32Temp & 0x03U) << 2U);

        u32Temp = (uint8_t)((handle->u32CSD[0] & 0x00FF0000U) >> 16U);
        pstcCSD->u8MaxWriteBlockLen |= (uint8_t)((u32Temp & 0xC0U) >> 6U);
        pstcCSD->u8WriteBlockPartial = (uint8_t)((u32Temp & 0x20U) >> 5U);
        pstcCSD->u8Reserved3         = 0U;
        pstcCSD->u8ContProtectApp    = (uint8_t)(u32Temp & 0x01U);

        u32Temp = (uint8_t)((handle->u32CSD[0] & 0x0000FF00U) >> 8U);
        pstcCSD->u8FileFormatGroup  = (uint8_t)((u32Temp & 0x80U) >> 7U);
        pstcCSD->u8CopyFlag         = (uint8_t)((u32Temp & 0x40U) >> 6U);
        pstcCSD->u8PermWriteProtect = (uint8_t)((u32Temp & 0x20U) >> 5U);
        pstcCSD->u8TempWriteProtect = (uint8_t)((u32Temp & 0x10U) >> 4U);
        pstcCSD->u8FileFormat       = (uint8_t)((u32Temp & 0x0CU) >> 2U);
        pstcCSD->u8ECCCode          = (uint8_t)(u32Temp & 0x03U);

        u32Temp = (uint8_t)(handle->u32CSD[0] & 0x000000FFU);
        pstcCSD->u8CRCChecksum   = (uint8_t)((u32Temp & 0xFEU) >> 1U);
        pstcCSD->u8Reserved4 = 1U;
    }

    return enRet;
}

/**
 * @brief  Get the MMC card information.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [out] pstcCardInfo           Pointer to a @ref stc_mmc_card_info_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Get MMC card information success
 *           - ErrorInvalidParameter: handle == NULL or pstcCardInfo == NULL
 */
en_result_t MMC_GetCardInfo(stc_mmc_handle_t *handle, stc_mmc_card_info_t *pstcCardInfo)
{
    en_result_t enRet = Ok;

    if ((NULL == pstcCardInfo) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcCardInfo->u32CardType     = handle->stcMmcCardInfo.u32CardType;
        pstcCardInfo->u32Class        = handle->stcMmcCardInfo.u32Class;
        pstcCardInfo->u32RelCardAddr  = handle->stcMmcCardInfo.u32RelCardAddr;
        pstcCardInfo->u32BlockNbr     = handle->stcMmcCardInfo.u32BlockNbr;
        pstcCardInfo->u32BlockSize    = handle->stcMmcCardInfo.u32BlockSize;
        pstcCardInfo->u32LogBlockNbr  = handle->stcMmcCardInfo.u32LogBlockNbr;
        pstcCardInfo->u32LogBlockSize = handle->stcMmcCardInfo.u32LogBlockSize;
    }

    return enRet;
}

/**
 * @brief  Get the MMC error code.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [out] u32ErrCode             Pointer to the value of error code
 * @retval An en_result_t enumeration value:
 *           - Ok: Get MMC error code success
 *           - ErrorInvalidParameter: handle == NULL or u32ErrCode == NULL
 */
en_result_t MMC_GetErrorCode(const stc_mmc_handle_t *handle, uint32_t *u32ErrCode)
{
    en_result_t enRet = Ok;

    if ((NULL == u32ErrCode) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        *u32ErrCode = handle->u32ErrorCode;
    }

    return enRet;
}

/**
 * @brief  This function handles MMC card interrupt request.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval None
 */
void MMC_IRQHandler(stc_mmc_handle_t *handle)
{
    en_result_t enCmdRet = Ok;
    en_mmc_card_state_t enCardState = MMCCardStateIdle;

    /* Check for SDIO interrupt flags */
    if (Reset != SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_NORMAL_INT_FLAG_TC))
    {
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_NORMAL_INT_FLAG_TC);
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN  | SDIOC_ERROR_INT_DTOESEN |
                                       SDIOC_NORMAL_INT_TCSEN | SDIOC_NORMAL_INT_BRRSEN | SDIOC_NORMAL_INT_BWRSEN), Disable);

        if ((0UL != (handle->u32Context & MMC_CONTEXT_INT)) || (0UL != (handle->u32Context & MMC_CONTEXT_DMA)))
        {
            if ((0UL != (handle->u32Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK)) || (0UL != (handle->u32Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK)))
            {
                /* Send stop transmission command */
                enCmdRet = SDMMC_CMD12_StopTransmission(handle->SDIOCx, &handle->u32ErrorCode);
                if (Ok != enCmdRet)
                {
                    MMC_ErrorCallback(handle);
                }
            }

            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            if ((0UL != (handle->u32Context & MMC_CONTEXT_WRITE_SINGLE_BLOCK)) || (0UL != (handle->u32Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK)))
            {
                MMC_TxCpltCallback(handle);
            }
            else
            {
                MMC_RxCpltCallback(handle);
            }
        }
    }
    else if ((Disable != SDIOC_GetIntEnableState(handle->SDIOCx, SDIOC_NORMAL_INT_BWRSEN)) &&
             (Reset != SDIOC_GetHostStatus(handle->SDIOCx, SDIOC_HOST_FLAG_BWE)))
    {
        SDIOC_WriteBuffer(handle->SDIOCx, handle->pu8Buffer, 4UL);
        handle->pu8Buffer += 4U;
        handle->u32Length -= 4U;
        if (0UL == handle->u32Length)
        {
            SDIOC_IntCmd(handle->SDIOCx, SDIOC_NORMAL_INT_BWRSEN, Disable);
        }
    }
    else if ((Disable != SDIOC_GetIntEnableState(handle->SDIOCx, SDIOC_NORMAL_INT_BRRSEN)) &&
             (Reset != SDIOC_GetHostStatus(handle->SDIOCx, SDIOC_HOST_FLAG_BRE)))
    {
        SDIOC_ReadBuffer(handle->SDIOCx, handle->pu8Buffer, 4UL);
        handle->pu8Buffer += 4U;
        handle->u32Length -= 4U;
        if (0UL == handle->u32Length)
        {
            SDIOC_IntCmd(handle->SDIOCx, SDIOC_NORMAL_INT_BRRSEN, Disable);
        }
    }
    else if (Reset != SDIOC_GetIntStatus(handle->SDIOCx, (SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN | SDIOC_ERROR_INT_DTOESEN)))
    {
        /* Set Error code */
        if (Reset != SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_DEBESEN))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_DATA_STOP_BIT;
        }
        if (Reset != SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_DCESEN))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_DATA_CRC_FAIL;
        }
        if (Reset != SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_DTOESEN))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_DATA_TIMEOUT;
        }

        /* Clear All flags */
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
        /* Disable all interrupts */
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN  | SDIOC_ERROR_INT_DTOESEN |
                                       SDIOC_NORMAL_INT_TCSEN | SDIOC_NORMAL_INT_BRRSEN | SDIOC_NORMAL_INT_BWRSEN), Disable);

        if (0UL != (handle->u32Context & MMC_CONTEXT_INT))
        {
            MMC_ErrorCallback(handle);
        }
        else if (0UL != (handle->u32Context & MMC_CONTEXT_DMA))
        {
            if (NULL != handle->DMAx)
            {
                /* Disable the DMA Channel */
                if ((0UL != (handle->u32Context & MMC_CONTEXT_WRITE_SINGLE_BLOCK)) || (0UL != (handle->u32Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK)))
                {
                    DMA_ChannelCmd(handle->DMAx, handle->u8DmaTxCh, Disable);
                }
                else
                {
                    DMA_ChannelCmd(handle->DMAx, handle->u8DmaRxCh, Disable);
                }
                /* Stop MMC transfer */
                MMC_GetCardState(handle, &enCardState);
                handle->u32ErrorCode = SDMMC_ERROR_NONE;
                if ((MMCCardStateSendingData == enCardState) || (MMCCardStateReceiveData == enCardState))
                {
                    /* Send stop transmission command */
                    SDMMC_CMD12_StopTransmission(handle->SDIOCx, &handle->u32ErrorCode);
                }
                MMC_ErrorCallback(handle);
            }
        }
        else
        {
        }
    }
    else
    {
    }
}

/**
 * @brief  MMC Tx completed callbacks
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval None
 */
__WEAKDEF void MMC_TxCpltCallback(stc_mmc_handle_t *handle)
{
    (void)handle;
    /* NOTE: This function MMC_TxCpltCallback can be implemented in the user file */
}

/**
 * @brief  MMC Rx completed callbacks
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval None
 */
__WEAKDEF void MMC_RxCpltCallback(stc_mmc_handle_t *handle)
{
    (void)handle;
    /* NOTE: This function MMC_TxCpltCallback can be implemented in the user file */
}

/**
 * @brief  MMC error callbacks
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval None
 */
__WEAKDEF void MMC_ErrorCallback(stc_mmc_handle_t *handle)
{
    (void)handle;
    /* NOTE: This function MMC_TxCpltCallback can be implemented in the user file */
}

/**
 * @brief  Erases the specified memory area of the given MMC card.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlkStartAddr         Start Block address
 * @param  [in] u32BlkEndAddr           End Block address
 * @retval An en_result_t enumeration value:
 *           - Ok: Erases the MMC card success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_Erase(stc_mmc_handle_t *handle, uint32_t u32BlkStartAddr, uint32_t u32BlkEndAddr)
{
    en_result_t enRet = Ok;
    uint32_t u32CardSta = 0UL;

    if ((u32BlkEndAddr < u32BlkStartAddr) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        if (u32BlkEndAddr > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check if the card command class supports erase command */
        if (0UL == ((handle->stcMmcCardInfo.u32Class) & SDMMC_CSD_SUPPORT_CLASS5_ERASE))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_REQUEST_NOT_APPLICABLE;
            return Error;
        }

        /* Check the lock status */
        enRet = MMC_GetCurrCardStatus(handle, &u32CardSta);
        if (Ok != enRet)
        {
            return enRet;
        }
        if (SDMMC_STATUS_CARD_IS_LOCKED == (u32CardSta & SDMMC_STATUS_CARD_IS_LOCKED))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_LOCK_UNLOCK_FAILED;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlkStartAddr *= 512U;
            u32BlkEndAddr   *= 512U;
        }

        /* Send CMD35 ERASE_GRP_START with argument as addr  */
        enRet = SDMMC_CMD35_EraseGroupStartAddr(handle->SDIOCx, u32BlkStartAddr, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Send CMD36 ERASE_GRP_END with argument as addr  */
        enRet = SDMMC_CMD36_EraseGroupEndAddr(handle->SDIOCx, u32BlkEndAddr, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Send CMD38 ERASE */
        enRet = SDMMC_CMD38_Erase(handle->SDIOCx, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }
    }

    return enRet;
}

/**
 * @brief  Reads block(s) from a specified address in a card.
 * @note   The Data transfer is managed by polling mode.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlockAddr            Block Address
 * @param  [in] u16BlockCnt             Block Count
 * @param  [out] pu8Data                Pointer to the buffer that will contain the received data
 * @param  [in] u32Timeout              Read timeout value
 * @retval An en_result_t enumeration value:
 *           - Ok: Read block(s) success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pu8Data == NULL or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_ReadBlocks(stc_mmc_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCnt, uint8_t *pu8Data, uint32_t u32Timeout)
{
    en_result_t enRet = Ok;
    stc_sdioc_data_init_t stcDataCfg;

    if ((NULL == pu8Data) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((u32BlockAddr + u16BlockCnt) > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlockAddr *= 512U;
        }

        /* Set Block Size for Card */
        enRet = SDMMC_CMD16_SetBlockLength(handle->SDIOCx, MMC_CARD_BLOCK_SIZE, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Configure the MMC data transfer */
        stcDataCfg.u16BlockSize    = MMC_CARD_BLOCK_SIZE;
        stcDataCfg.u16BlockCount   = u16BlockCnt;
        stcDataCfg.u16TransferDir  = SDIOC_TRANSFER_DIR_TO_HOST;
        stcDataCfg.u16AutoCMD12En  = SDIOC_AUTO_SEND_CMD12_DISABLE;
        stcDataCfg.u16TransferMode = (u16BlockCnt > 1U) ? (uint16_t)SDIOC_TRANSFER_MODE_MULTIPLE : (uint16_t)SDIOC_TRANSFER_MODE_SINGLE;
        stcDataCfg.u16DataTimeout  = SDIOC_DATA_TIMEOUT_CLK_2_27;
        SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);

        /* Read block(s) in polling mode */
        if (u16BlockCnt > 1U)
        {
            handle->u32Context = MMC_CONTEXT_READ_MULTIPLE_BLOCK;
            /* Read Multi Block command */
            enRet = SDMMC_CMD18_ReadMultipleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }
        else
        {
            handle->u32Context = MMC_CONTEXT_READ_SINGLE_BLOCK;
            /* Read Single Block command */
            enRet = SDMMC_CMD17_ReadSingleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }

        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Get data */
        enRet = MMC_ReadWriteFifo(handle, &stcDataCfg, (uint8_t *)pu8Data, u32Timeout);
        if (Ok != enRet)
        {
            return enRet;
        }
    }

    return enRet;
}

/**
 * @brief  Write block(s) to a specified address in a card.
 * @note   The Data transfer is managed by polling mode.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlockAddr            Block Address
 * @param  [in] u16BlockCnt             Block Count
 * @param  [in] pu8Data                 Pointer to the buffer that will contain the data to transmit
 * @param  [in] u32Timeout              Write timeout value
 * @retval An en_result_t enumeration value:
 *           - Ok: Write block(s) success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pu8Data == NULL or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_WriteBlocks(stc_mmc_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCnt, uint8_t *pu8Data, uint32_t u32Timeout)
{
    en_result_t enRet = Ok;
    stc_sdioc_data_init_t stcDataCfg;

    if ((NULL == pu8Data) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((u32BlockAddr + u16BlockCnt) > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlockAddr *= 512U;
        }

        /* Set Block Size for Card */
        enRet = SDMMC_CMD16_SetBlockLength(handle->SDIOCx, MMC_CARD_BLOCK_SIZE, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Configure the MMC data transfer */
        stcDataCfg.u16BlockSize    = MMC_CARD_BLOCK_SIZE;
        stcDataCfg.u16BlockCount   = u16BlockCnt;
        stcDataCfg.u16TransferDir  = SDIOC_TRANSFER_DIR_TO_CARD;
        stcDataCfg.u16AutoCMD12En  = SDIOC_AUTO_SEND_CMD12_DISABLE;
        stcDataCfg.u16TransferMode = (u16BlockCnt > 1U) ? (uint16_t)SDIOC_TRANSFER_MODE_MULTIPLE : (uint16_t)SDIOC_TRANSFER_MODE_SINGLE;
        stcDataCfg.u16DataTimeout  = SDIOC_DATA_TIMEOUT_CLK_2_27;
        SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);

        /* Write block(s) in polling mode */
        if (u16BlockCnt > 1U)
        {
            handle->u32Context = MMC_CONTEXT_WRITE_MULTIPLE_BLOCK;
            /* Write Multi Block command */
            enRet = SDMMC_CMD25_WriteMultipleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }
        else
        {
            handle->u32Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK;
            /* Write Single Block command */
            enRet = SDMMC_CMD24_WriteSingleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }

        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Get data */
        enRet = MMC_ReadWriteFifo(handle, &stcDataCfg, (uint8_t *)pu8Data, u32Timeout);
        if (Ok != enRet)
        {
            return enRet;
        }
    }

    return enRet;
}



/**
 * @brief  Reads block(s) from a specified address in a card.
 * @note   The Data transfer is managed by interrupt mode.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlockAddr            Block Address
 * @param  [in] u16BlockCnt             Block Count
 * @param  [out] pu8Data                Pointer to the buffer that will contain the received data
 * @retval An en_result_t enumeration value:
 *           - Ok: Read block(s) success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pu8Data == NULL or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_ReadBlocks_INT(stc_mmc_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCnt, uint8_t *pu8Data)
{
    en_result_t enRet = Ok;
    stc_sdioc_data_init_t stcDataCfg;

    if ((NULL == pu8Data) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((u32BlockAddr + u16BlockCnt) > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlockAddr *= 512U;
        }

        /* Set Block Size for Card */
        enRet = SDMMC_CMD16_SetBlockLength(handle->SDIOCx, MMC_CARD_BLOCK_SIZE, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Configure interrupt transfer parameters */
        handle->pu8Buffer = pu8Data;
        handle->u32Length = (uint32_t)u16BlockCnt * MMC_CARD_BLOCK_SIZE;
        SDIOC_ClearIntStatus(handle->SDIOCx, (SDIOC_NORMAL_INT_FLAG_BWR | SDIOC_NORMAL_INT_FLAG_BRR));
        /* Enable SDIOC interrupt */
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN  | SDIOC_ERROR_INT_DTOESEN |
                                      SDIOC_NORMAL_INT_TCSEN  | SDIOC_NORMAL_INT_BRRSEN), Enable);

        /* Configure the MMC data transfer */
        stcDataCfg.u16BlockSize    = MMC_CARD_BLOCK_SIZE;
        stcDataCfg.u16BlockCount   = u16BlockCnt;
        stcDataCfg.u16TransferDir  = SDIOC_TRANSFER_DIR_TO_HOST;
        stcDataCfg.u16AutoCMD12En  = SDIOC_AUTO_SEND_CMD12_DISABLE;
        stcDataCfg.u16TransferMode = (u16BlockCnt > 1U) ? (uint16_t)SDIOC_TRANSFER_MODE_MULTIPLE : (uint16_t)SDIOC_TRANSFER_MODE_SINGLE;
        stcDataCfg.u16DataTimeout  = SDIOC_DATA_TIMEOUT_CLK_2_27;
        SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);

        /* Read block(s) in interrupt mode */
        if (u16BlockCnt > 1U)
        {
            handle->u32Context = MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_INT;
            /* Read Multi Block command */
            enRet = SDMMC_CMD18_ReadMultipleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }
        else
        {
            handle->u32Context = MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_INT;
            /* Read Single Block command */
            enRet = SDMMC_CMD17_ReadSingleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }

        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }
    }

    return enRet;
}

/**
 * @brief  Write block(s) to a specified address in a card.
 * @note   The Data transfer is managed by interrupt mode.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlockAddr            Block Address
 * @param  [in] u16BlockCnt             Block Count
 * @param  [in] pu8Data                 Pointer to the buffer that will contain the data to transmit
 * @retval An en_result_t enumeration value:
 *           - Ok: Write block(s) success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pu8Data == NULL or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_WriteBlocks_INT(stc_mmc_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCnt, uint8_t *pu8Data)
{
    en_result_t enRet = Ok;
    stc_sdioc_data_init_t stcDataCfg;

    if ((NULL == pu8Data) || (NULL == handle))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((u32BlockAddr + u16BlockCnt) > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlockAddr *= 512U;
        }

        /* Set Block Size for Card */
        enRet = SDMMC_CMD16_SetBlockLength(handle->SDIOCx, MMC_CARD_BLOCK_SIZE, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Configure interrupt transfer parameters */
        handle->pu8Buffer = pu8Data;
        handle->u32Length = (uint32_t)u16BlockCnt * MMC_CARD_BLOCK_SIZE;
        SDIOC_ClearIntStatus(handle->SDIOCx, (SDIOC_NORMAL_INT_FLAG_BWR | SDIOC_NORMAL_INT_FLAG_BRR));
        /* Enable SDIOC interrupt */
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN  | SDIOC_ERROR_INT_DTOESEN |
                                      SDIOC_NORMAL_INT_TCSEN  | SDIOC_NORMAL_INT_BWRSEN), Enable);

        /* Configure the MMC data transfer */
        stcDataCfg.u16BlockSize    = MMC_CARD_BLOCK_SIZE;
        stcDataCfg.u16BlockCount   = u16BlockCnt;
        stcDataCfg.u16TransferDir  = SDIOC_TRANSFER_DIR_TO_CARD;
        stcDataCfg.u16AutoCMD12En  = SDIOC_AUTO_SEND_CMD12_DISABLE;
        stcDataCfg.u16TransferMode = (u16BlockCnt > 1U) ? (uint16_t)SDIOC_TRANSFER_MODE_MULTIPLE : (uint16_t)SDIOC_TRANSFER_MODE_SINGLE;
        stcDataCfg.u16DataTimeout  = SDIOC_DATA_TIMEOUT_CLK_2_27;
        SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);

        /* Write block(s) in interrupt mode */
        if (u16BlockCnt > 1U)
        {
            handle->u32Context = MMC_CONTEXT_WRITE_MULTIPLE_BLOCK | MMC_CONTEXT_INT;
            /* Write Multi Block command */
            enRet = SDMMC_CMD25_WriteMultipleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }
        else
        {
            handle->u32Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_INT;
            /* Write Single Block command */
            enRet = SDMMC_CMD24_WriteSingleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }

        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }
    }

    return enRet;
}

/**
 * @brief  Reads block(s) from a specified address in a card.
 * @note   The Data transfer is managed by DMA mode.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlockAddr            Block Address
 * @param  [in] u16BlockCnt             Block Count
 * @param  [out] pu8Data                Pointer to the buffer that will contain the received data
 * @retval An en_result_t enumeration value:
 *           - Ok: Read block(s) success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pu8Data == NULL or NULL == handle->DMAx or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_ReadBlocks_DMA(stc_mmc_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCnt, uint8_t *pu8Data)
{
    en_result_t enRet = Ok;
    stc_sdioc_data_init_t stcDataCfg;

    if ((NULL == pu8Data) || (NULL == handle) || (NULL == handle->DMAx) || (0U != ((uint32_t)pu8Data % 4U)))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((u32BlockAddr + u16BlockCnt) > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlockAddr *= 512U;
        }

        /* Set Block Size for Card */
        enRet = SDMMC_CMD16_SetBlockLength(handle->SDIOCx, MMC_CARD_BLOCK_SIZE, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Enable SDIOC transfer complete and errors interrupt */
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_NORMAL_INT_TCSEN | SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN | SDIOC_ERROR_INT_DTOESEN), Enable);
        /* Configure DMA parameters */
        MMC_DmaTransferConfig(handle, handle->u8DmaRxCh, (uint32_t)(&handle->SDIOCx->BUF0), (uint32_t)pu8Data, (MMC_CARD_BLOCK_SIZE / 4U), u16BlockCnt);
        /* Enable the DMA Channel */
        DMA_ChannelCmd(handle->DMAx, handle->u8DmaRxCh, Enable);

        /* Configure the MMC data transfer */
        stcDataCfg.u16BlockSize    = MMC_CARD_BLOCK_SIZE;
        stcDataCfg.u16BlockCount   = u16BlockCnt;
        stcDataCfg.u16TransferDir  = SDIOC_TRANSFER_DIR_TO_HOST;
        stcDataCfg.u16AutoCMD12En  = SDIOC_AUTO_SEND_CMD12_DISABLE;
        stcDataCfg.u16TransferMode = (u16BlockCnt > 1U) ? (uint16_t)SDIOC_TRANSFER_MODE_MULTIPLE : (uint16_t)SDIOC_TRANSFER_MODE_SINGLE;
        stcDataCfg.u16DataTimeout  = SDIOC_DATA_TIMEOUT_CLK_2_27;
        SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);

        /* Read block(s) in DMA mode */
        if (u16BlockCnt > 1U)
        {
            handle->u32Context = MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA;
            /* Read Multi Block command */
            enRet = SDMMC_CMD18_ReadMultipleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }
        else
        {
            handle->u32Context = MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_DMA;
            /* Read Single Block command */
            enRet = SDMMC_CMD17_ReadSingleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }

        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }
    }

    return enRet;
}

/**
 * @brief  Write block(s) to a specified address in a card.
 * @note   The Data transfer is managed by DMA mode.
 * @note   This API should be followed by a check on the card state through MMC_GetCardState().
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u32BlockAddr            Block Address
 * @param  [in] u16BlockCnt             Block Count
 * @param  [in] pu8Data                 Pointer to the buffer that will contain the data to transmit
 * @retval An en_result_t enumeration value:
 *           - Ok: Write block(s) success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or pu8Data == NULL or NULL == handle->DMAx or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_WriteBlocks_DMA(stc_mmc_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCnt, uint8_t *pu8Data)
{
    en_result_t enRet = Ok;
    stc_sdioc_data_init_t stcDataCfg;

    if ((NULL == pu8Data) || (NULL == handle) || (NULL == handle->DMAx) || (0U != ((uint32_t)pu8Data % 4U)))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((u32BlockAddr + u16BlockCnt) > (handle->stcMmcCardInfo.u32LogBlockNbr))
        {
            handle->u32ErrorCode |= SDMMC_ERROR_ADDR_OUT_OF_RANGE;
            return Error;
        }

        /* Check the Card capacity in term of Logical number of blocks */
        if (handle->stcMmcCardInfo.u32LogBlockNbr < MMC_CARD_CAPACITY)
        {
            u32BlockAddr *= 512U;
        }

        /* Set Block Size for Card */
        enRet = SDMMC_CMD16_SetBlockLength(handle->SDIOCx, MMC_CARD_BLOCK_SIZE, &handle->u32ErrorCode);
        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }

        /* Enable SDIOC transfer complete and errors interrupt */
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_NORMAL_INT_TCSEN | SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN | SDIOC_ERROR_INT_DTOESEN), Enable);
        /* Configure DMA parameters */
        MMC_DmaTransferConfig(handle, handle->u8DmaTxCh, (uint32_t)pu8Data, (uint32_t)(&handle->SDIOCx->BUF0), (MMC_CARD_BLOCK_SIZE / 4U), u16BlockCnt);
        /* Enable the DMA Channel */
        DMA_ChannelCmd(handle->DMAx, handle->u8DmaTxCh, Enable);

        /* Configure the MMC data transfer */
        stcDataCfg.u16BlockSize    = MMC_CARD_BLOCK_SIZE;
        stcDataCfg.u16BlockCount   = u16BlockCnt;
        stcDataCfg.u16TransferDir  = SDIOC_TRANSFER_DIR_TO_CARD;
        stcDataCfg.u16AutoCMD12En  = SDIOC_AUTO_SEND_CMD12_DISABLE;
        stcDataCfg.u16TransferMode = (u16BlockCnt > 1U) ? (uint16_t)SDIOC_TRANSFER_MODE_MULTIPLE : (uint16_t)SDIOC_TRANSFER_MODE_SINGLE;
        stcDataCfg.u16DataTimeout  = SDIOC_DATA_TIMEOUT_CLK_2_27;
        SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);

        /* Write block(s) in DMA mode */
        if (u16BlockCnt > 1U)
        {
            handle->u32Context = MMC_CONTEXT_WRITE_MULTIPLE_BLOCK | MMC_CONTEXT_DMA;
            /* Write Multi Block command */
            enRet = SDMMC_CMD25_WriteMultipleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }
        else
        {
            handle->u32Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_DMA;
            /* Write Single Block command */
            enRet = SDMMC_CMD24_WriteSingleBlock(handle->SDIOCx, u32BlockAddr, &handle->u32ErrorCode);
        }

        if (Ok != enRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enRet;
        }
    }

    return enRet;
}

/**
 * @brief  Abort the current transfer and disable the MMC.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Abort transfer success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: handle == NULL or
 *                                    An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
en_result_t MMC_Abort(stc_mmc_handle_t *handle)
{
    en_result_t enRet = Ok;
    en_mmc_card_state_t enCardState = MMCCardStateIdle;

    if (NULL == handle)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Disable All interrupts */
        SDIOC_IntCmd(handle->SDIOCx, (SDIOC_ERROR_INT_DEBESEN | SDIOC_ERROR_INT_DCESEN  | SDIOC_ERROR_INT_DTOESEN |
                                       SDIOC_NORMAL_INT_TCSEN | SDIOC_NORMAL_INT_BRRSEN | SDIOC_NORMAL_INT_BWRSEN), Disable);
        /* Clear All flags */
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);

        if (0UL != (handle->u32Context & MMC_CONTEXT_DMA))
        {
            if (NULL != handle->DMAx)
            {
                /* Disable the DMA Channel */
                if ((0UL != (handle->u32Context & MMC_CONTEXT_WRITE_SINGLE_BLOCK)) || (0UL != (handle->u32Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK)))
                {
                    DMA_ChannelCmd(handle->DMAx, handle->u8DmaTxCh, Disable);
                }
                else
                {
                    DMA_ChannelCmd(handle->DMAx, handle->u8DmaRxCh, Disable);
                }
            }
        }

        /* Stop MMC transfer */
        MMC_GetCardState(handle, &enCardState);
        handle->u32ErrorCode = SDMMC_ERROR_NONE;
        if ((MMCCardStateSendingData == enCardState) || (MMCCardStateReceiveData == enCardState))
        {
            /* Send stop transmission command */
            SDMMC_CMD12_StopTransmission(handle->SDIOCx, &handle->u32ErrorCode);
        }
    }

    return enRet;
}

/**
 * @brief  Configure the Dma transfer parameters.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] u8Ch                    DMA transfer channel
 * @param  [in] u32SrcAddr              Source Address
 * @param  [in] u32DestAddr             Destination Address
 * @param  [in] u16BlkSize              Block Size
 * @param  [in] u16TransCnt             Transfer Count
 * @retval None
 */
static void MMC_DmaTransferConfig(const stc_mmc_handle_t *handle, uint8_t u8Ch, uint32_t u32SrcAddr, uint32_t u32DestAddr, uint16_t u16BlkSize, uint16_t u16TransCnt)
{
    /* Stop Configure channel */
    DMA_ChannelCmd(handle->DMAx, u8Ch, Disable);
    DMA_ClearTransIntStatus(handle->DMAx, (uint32_t)(0x1UL << u8Ch));

    /* Config DMA source and destination address */
    DMA_SetSrcAddr(handle->DMAx, u8Ch, u32SrcAddr);
    DMA_SetDestAddr(handle->DMAx, u8Ch, u32DestAddr);
    /* Config DMA block size and transfer count */
    DMA_SetBlockSize(handle->DMAx, u8Ch, u16BlkSize);
    DMA_SetTransCnt(handle->DMAx, u8Ch, u16TransCnt);
}

/**
 * @brief  Set the speed mode of the MMC card.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Set speed mode success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
static en_result_t MMC_SetSpeedMode(stc_mmc_handle_t *handle)
{
    en_result_t enCmdRet = Ok;

    if (SDIOC_SPEED_MODE_HIGH == handle->stcSdiocInit.u8SpeedMode)
    {
        /* Send CMD6 SWITCH_FUNC with argument */
        enCmdRet = SDMMC_CMD6_SwitchFunc(handle->SDIOCx, MMC_SET_FUNC_HIGH_SPEED, &handle->u32ErrorCode);
        if (Ok != enCmdRet)
        {
            return enCmdRet;
        }
    }

    /* Set the clock division and speed mode of SDIOC */
    SDIOC_SetSpeedMode(handle->SDIOCx, handle->stcSdiocInit.u8SpeedMode);
    SDIOC_SetClockDiv(handle->SDIOCx, handle->stcSdiocInit.u16ClockDiv);

    return enCmdRet;
}

/**
 * @brief  Set the bus width of the MMC card.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: Set bus width success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
static en_result_t MMC_SetBusWidth(stc_mmc_handle_t *handle)
{
    en_result_t enCmdRet = Ok;
    uint32_t u32BusWidth = 0UL, u32Response = 0UL;
    uint32_t u32BusySta = 0UL;
    __IO uint32_t u32Count = 0UL;

    if (SDIOC_BUS_WIDTH_1BIT == handle->stcSdiocInit.u8BusWidth)
    {
        u32BusWidth = MMC_CMD6_BUS_WIDTH_1BIT;
    }
    else if (SDIOC_BUS_WIDTH_4BIT == handle->stcSdiocInit.u8BusWidth)
    {
        u32BusWidth = MMC_CMD6_BUS_WIDTH_4BIT;
    }
    else if (SDIOC_BUS_WIDTH_8BIT == handle->stcSdiocInit.u8BusWidth)
    {
        u32BusWidth = MMC_CMD6_BUS_WIDTH_8BIT;
    }
    else
    {
        return ErrorInvalidParameter;
    }

    /* Send CMD6 SWITCH_FUNC with argument */
    enCmdRet = SDMMC_CMD6_SwitchFunc(handle->SDIOCx, u32BusWidth, &handle->u32ErrorCode);
    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }

    /* Check for switch error and violation of the trial number of sending CMD 13 */
    while (0UL == u32BusySta)
    {
        if (u32Count++ >= MMC_MAX_SWITCH_FUNC_TRIAL)
        {
            handle->u32ErrorCode |= SDMMC_ERROR_REQUEST_NOT_APPLICABLE;
            return Error;
        }

        enCmdRet = MMC_GetCurrCardStatus(handle, &u32Response);
        /* Get operating result */
        u32BusySta = (((u32Response >> 7U) == 1UL) ? 0UL : 1UL);
    }

    /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
    u32Count = 0UL;
    while (0UL == u32BusySta)
    {
        if (u32Count++ >= SDMMC_DATA_TIMEOUT)
        {
            handle->u32ErrorCode |= SDMMC_ERROR_REQUEST_NOT_APPLICABLE;
            return Error;
        }

        enCmdRet = MMC_GetCurrCardStatus(handle, &u32Response);
        /* Get operating result */
        u32BusySta = (((u32Response >> 8U) == 1UL) ? 1UL : 0UL);
    }

    if (SDMMC_ERROR_NONE != handle->u32ErrorCode)
    {
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
        return enCmdRet;
    }

    /* Set the bus width of SDIOC */
    SDIOC_SetBusWidth(handle->SDIOCx, handle->stcSdiocInit.u8BusWidth);

    return enCmdRet;
}

/**
 * @brief  Initializes the MMC card.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval An en_result_t enumeration value:
 *           - Ok: MMC card Initialize success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
static en_result_t MMC_InitCard(stc_mmc_handle_t *handle)
{
    en_result_t enCmdRet = Ok;
    uint16_t u16RcaVal = 0U;

    /* Check the power State */
    if (Disable == SDIOC_GetPowerState(handle->SDIOCx))
    {
        handle->u32ErrorCode |= SDMMC_ERROR_REQUEST_NOT_APPLICABLE;
        return Error;
    }

    /* Send CMD2 ALL_SEND_CID */
    enCmdRet = SDMMC_CMD2_AllSendCID(handle->SDIOCx, &handle->u32ErrorCode);
    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }
    /* Get Card identification number data */
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT0_31, &handle->u32CID[0]);
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT32_63, &handle->u32CID[1]);
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT64_95, &handle->u32CID[2]);
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT96_127, &handle->u32CID[3]);

    /* Send CMD3 SET_REL_ADDR with argument 0 */
    /* MMC Card publishes its RCA */
    enCmdRet = SDMMC_CMD3_SendRelativeAddr(handle->SDIOCx, &u16RcaVal, &handle->u32ErrorCode);
    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }
    /* Get the MMC card RCA */
    handle->stcMmcCardInfo.u32RelCardAddr = u16RcaVal;

    /* Send CMD9 SEND_CSD with argument as card's RCA */
    enCmdRet = SDMMC_CMD9_SendCSD(handle->SDIOCx, (uint32_t)(handle->stcMmcCardInfo.u32RelCardAddr << 16U), &handle->u32ErrorCode);
    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }
    /* Get Card Specific Data */
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT0_31, &handle->u32CSD[0]);
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT32_63, &handle->u32CSD[1]);
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT64_95, &handle->u32CSD[2]);
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT96_127, &handle->u32CSD[3]);

    /* Get the Card Class */
    handle->stcMmcCardInfo.u32Class = handle->u32CSD[2] >> 20U;
    /* Get CSD parameters */
    MMC_ExtractCardCSD(handle);

    /* Select the Card */
    enCmdRet = SDMMC_CMD7_SelectDeselectCard(handle->SDIOCx, (uint32_t)(handle->stcMmcCardInfo.u32RelCardAddr << 16U), &handle->u32ErrorCode);
    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }

    return enCmdRet;
}

/**
 * @brief  Enable or disable MMC power.
 * @note   At the power-on, Enquires cards about their operating voltage and configures
 *         clock controls and stores MMC information.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] enNewSta                The function new state.
 *           @arg This parameter can be: Enable or Disable.
 * @retval An en_result_t enumeration value:
 *           - Ok: Enable or disable MMC power success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
static en_result_t MMC_PowerCmd(stc_mmc_handle_t *handle, en_functional_state_t enNewSta)
{
    uint32_t u32Response = 0UL, u32PowerSta = 0UL;
    __IO uint32_t u32Count = 0UL;
    en_result_t enCmdRet = Ok;

    if (Disable != enNewSta)
    {
        /* CMD0: GO_IDLE_STATE */
        enCmdRet = SDMMC_CMD0_GoIdleState(handle->SDIOCx, &handle->u32ErrorCode);
        if (Ok != enCmdRet)
        {
            return enCmdRet;
        }

        /* Wait for reset to completed */
        DDL_DelayMS(1U);
        while (0UL == u32PowerSta)
        {
            if (u32Count++ >= SDMMC_MAX_VOLTAGE_TRIAL)
            {
                handle->u32ErrorCode |= SDMMC_ERROR_INVALID_VOLTRANGE;
                return Error;
            }

            /* Send CMD1 SEND_OP_COND with Argument EMMC_VOL_RANGE_HIGH_VOLTAGE(0xC0FF8000) */
            enCmdRet = SDMMC_CMD1_SendOperatCond(handle->SDIOCx, EMMC_VOL_RANGE_HIGH_VOLTAGE, &handle->u32ErrorCode);
            if (Ok != enCmdRet)
            {
                handle->u32ErrorCode |= SDMMC_ERROR_UNSUPPORTED_FEATURE;
                return Error;
            }

            /* Get command response */
            SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT0_31, &u32Response);
            /* Get Card power up status bit (busy) */
            u32PowerSta = (((u32Response >> 31U) == 1UL) ? 1UL : 0UL);
        }

        /* When power routine is finished and command returns valid voltage */
        if (MMC_VOL_RANGE_HIGH_VOLTAGE == (u32Response & EMMC_VOL_RANGE_HIGH_VOLTAGE))
        {
            /* When voltage range of the card is within 2.7V and 3.6V */
            handle->stcMmcCardInfo.u32CardType = MMC_HIGH_VOLTAGE_CARD;
        }
        else
        {
            /* When voltage range of the card is within 1.65V and 1.95V or 2.7V and 3.6V */
            handle->stcMmcCardInfo.u32CardType = MMC_DUAL_VOLTAGE_CARD;
        }
    }
    else
    {
        /* Set Power State to OFF */
        SDIOC_ClockCmd(handle->SDIOCx, Disable);
        SDIOC_PowerCmd(handle->SDIOCx, Disable);
    }

    return enCmdRet;
}

/**
 * @brief  Get the current card status.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [out] pu32CardStatus         Pointer to the value of current card status
 * @retval An en_result_t enumeration value:
 *           - Ok: Get card status success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
static en_result_t MMC_GetCurrCardStatus(stc_mmc_handle_t *handle, uint32_t *pu32CardStatus)
{
    en_result_t enCmdRet = Ok;

    /* Send Status command */
    enCmdRet = SDMMC_CMD13_SendStatus(handle->SDIOCx, (uint32_t)(handle->stcMmcCardInfo.u32RelCardAddr << 16U), &handle->u32ErrorCode);
    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }
    /* Get MMC card status */
    SDIOC_GetResponse(handle->SDIOCx, SDIOC_RESPONSE_REG_BIT0_31, pu32CardStatus);

    return enCmdRet;
}

/**
 * @brief  Read or Write the MMC Card FIFO.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @param  [in] pstcDataCfg             Pointer to a @ref stc_sdioc_data_init_t structure
 * @param  [out] pu8Data                Pointer to the value of read/write fifo
 * @param  [in] u32Timeout              The timeout time
 * @retval An en_result_t enumeration value:
 *           - Ok: Read or Write the FIFO success
 *           - Error: Refer to u32ErrorCode for the reason of error
 *           - ErrorInvalidParameter: An invalid parameter was write to the send command
 *           - ErrorTimeout: Send command timeout
 */
static en_result_t MMC_ReadWriteFifo(stc_mmc_handle_t *handle, const stc_sdioc_data_init_t *pstcDataCfg, uint8_t pu8Data[], uint32_t u32Timeout)
{
    __IO uint32_t u32Count;
    en_result_t enCmdRet = Ok;
    uint32_t u32Index = 0UL;

    /* The u32Timeout is expressed in ms */
    u32Count = u32Timeout * (HCLK_VALUE / 20000UL);
    while (Reset == SDIOC_GetIntStatus(handle->SDIOCx, (SDIOC_ERROR_INT_FLAG_DEBE | SDIOC_ERROR_INT_FLAG_DCE |
                                                        SDIOC_ERROR_INT_FLAG_DTOE | SDIOC_NORMAL_INT_FLAG_TC)))
    {
        if (SDIOC_TRANSFER_DIR_TO_CARD != pstcDataCfg->u16TransferDir)
        {
            // Read buffer data
            if (Set == SDIOC_GetHostStatus(handle->SDIOCx, SDIOC_HOST_FLAG_BRE))
            {
                SDIOC_ReadBuffer(handle->SDIOCx, (uint8_t *)&pu8Data[u32Index], (uint32_t)(pstcDataCfg->u16BlockSize));
                u32Index += pstcDataCfg->u16BlockSize;
            }
        }
        else
        {
            // Write buffer data
            if (Set == SDIOC_GetHostStatus(handle->SDIOCx, SDIOC_HOST_FLAG_BWE))
            {
                SDIOC_WriteBuffer(handle->SDIOCx, (uint8_t *)&pu8Data[u32Index], (uint32_t)(pstcDataCfg->u16BlockSize));
                u32Index += pstcDataCfg->u16BlockSize;
            }
        }

        if (u32Count-- == 0UL)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return ErrorTimeout;
        }
    }

    /* Send stop transmission command in case of multiblock read/write */
    if ((Set == SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_NORMAL_INT_FLAG_TC)) && (pstcDataCfg->u16BlockCount > 1U))
    {
        /* Send stop transmission command */
        enCmdRet = SDMMC_CMD12_StopTransmission(handle->SDIOCx, &handle->u32ErrorCode);
        if (Ok != enCmdRet)
        {
            SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
            return enCmdRet;
        }
    }

    /* Get error state */
    if (Set == SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_FLAG_DEBE))
    {
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
        handle->u32ErrorCode |= SDMMC_ERROR_DATA_STOP_BIT;
        return Error;
    }
    else if (Set == SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_FLAG_DCE))
    {
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
        handle->u32ErrorCode |= SDMMC_ERROR_DATA_CRC_FAIL;
        return Error;
    }
    else if (Set == SDIOC_GetIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_FLAG_DTOE))
    {
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
        handle->u32ErrorCode |= SDMMC_ERROR_DATA_TIMEOUT;
        return Error;
    }
    else
    {
        /* Empty FIFO if there is still any data */
        if (SDIOC_TRANSFER_DIR_TO_CARD != pstcDataCfg->u16TransferDir)
        {
            u32Count = u32Timeout * (HCLK_VALUE / 20000UL);
            while (Set == SDIOC_GetHostStatus(handle->SDIOCx, SDIOC_HOST_FLAG_BRE))
            {
                SDIOC_ReadBuffer(handle->SDIOCx, (uint8_t *)&pu8Data[u32Index], (uint32_t)(pstcDataCfg->u16BlockSize));
                u32Index += pstcDataCfg->u16BlockSize;
                if (u32Count-- == 0UL)
                {
                    SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
                    return ErrorTimeout;
                }
            }
        }

        /* Clear all the error and completed flags */
        SDIOC_ClearIntStatus(handle->SDIOCx, SDIOC_ERROR_INT_STATIC_FLAGS);
    }

    return enCmdRet;
}

/**
 * @brief  Extract information of the card which CSD register.
 * @param  [in] handle                  Pointer to a @ref stc_mmc_handle_t structure
 * @retval None
 */
static void MMC_ExtractCardCSD(stc_mmc_handle_t *handle)
{
    uint32_t u32Temp = 0UL;
    stc_mmc_card_csd_t stcCardCSD;

    stcCardCSD.u8ReadBlockLen   = (uint8_t)((handle->u32CSD[2] & 0x000F0000U) >> 16U);
    u32Temp = (uint8_t)((handle->u32CSD[1] & 0xFF000000U) >> 24U);
    stcCardCSD.u32DeviceSize    = (uint32_t)(((handle->u32CSD[2] & 0x000003FFU) << 2U) | ((u32Temp & 0xC0U) >> 6U));
    u32Temp = (uint8_t)((handle->u32CSD[1] & 0x00FF0000U) >> 16U);
    stcCardCSD.u8DeviceSizeMul  = (uint8_t)((u32Temp & 0x03U) << 1U);
    u32Temp = (uint8_t)((handle->u32CSD[1] & 0x0000FF00U) >> 8U);
    stcCardCSD.u8DeviceSizeMul |= (uint8_t)((u32Temp & 0x80U) >> 7U);

    handle->stcMmcCardInfo.u32BlockNbr     = (stcCardCSD.u32DeviceSize + 1U);
    handle->stcMmcCardInfo.u32BlockNbr    *= (1UL << (stcCardCSD.u8DeviceSizeMul + 2U));
    handle->stcMmcCardInfo.u32BlockSize    = 1UL << (stcCardCSD.u8ReadBlockLen);
    if (handle->stcMmcCardInfo.u32BlockSize >= 512U)
    {
        handle->stcMmcCardInfo.u32LogBlockNbr = handle->stcMmcCardInfo.u32BlockNbr * (handle->stcMmcCardInfo.u32BlockSize / 512U);
    }
    else
    {
        handle->stcMmcCardInfo.u32LogBlockNbr = (handle->stcMmcCardInfo.u32BlockNbr / 512U) * handle->stcMmcCardInfo.u32BlockSize;
    }
    handle->stcMmcCardInfo.u32LogBlockSize = 512UL;
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
