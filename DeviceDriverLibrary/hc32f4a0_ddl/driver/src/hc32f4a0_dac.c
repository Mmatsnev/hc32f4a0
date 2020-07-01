/**
 *******************************************************************************
 * @file  hc32f4a0_dac.c
 * @brief This file provides firmware functions to manage the Digital-to-Analog
 *        Converter(DAC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao          First version
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
#include "hc32f4a0_dac.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_DAC DAC
 * @brief DAC Driver Library
 * @{
 */

#if (DDL_DAC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup DAC_Local_Macros DAC Local Macros
 * @{
 */
#define DAC_ADP_SELECT_ALL  (DAC_DAADPCR_ADPSL1 | DAC_DAADPCR_ADPSL2 | DAC_DAADPCR_ADPSL3)

/**
 * @defgroup DAC_Check_Parameters_Validity DAC Check Parameters Validity
 * @{
 */

#define IS_VALID_UNIT(x)                                                       \
(   ((x) == M4_DAC1)                               ||                          \
    ((x) == M4_DAC2))

#define IS_VALID_CH(x)                                                         \
(   ((x) == DAC_CH_1)                              ||                          \
    ((x) == DAC_CH_2))

#define IS_VALID_DATA_ALIGN(x)                                                 \
(   ((x) == DAC_DATA_ALIGN_L)                      ||                          \
    ((x) == DAC_DATA_ALIGN_R))

#define IS_VALID_DATA_SRC(x)                                                   \
(   ((x) == DAC_DATA_SRC_DATAREG)                  ||                          \
    ((x) == DAC_DATA_SRC_DCU))

#define IS_VALID_ADCPRIO_CONFIG(x)                                             \
(   (0U != (x))                                    &&                          \
    (DAC_ADP_SELECT_ALL == ((x) | DAC_ADP_SELECT_ALL)))

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
 * @defgroup DAC_Global_Functions DAC Global Functions
 * @{
 */

/**
 * @brief  Set DAC data source for specified channel
 * @param  [in] DACx       Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16Ch      Specify the DAC channel @ref DAC_CH.
 *         This parameter can be one of the following values:
 *         @arg DAC_CH_1
 *         @arg DAC_CH_2
 * @param  [in] u16Src     Specify the data source.
 *         This parameter can be a value of @ref DAC_DATA_SRC
 *         - DAC_DATA_SRC_DATAREG:   convert source is from data register
 *         - DAC_DATA_SRC_DCU:       convert source is from DCU
 * @retval None
 */
void DAC_SetDataSource(M4_DAC_TypeDef *DACx, uint16_t u16Ch, uint16_t u16Src)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_CH(u16Ch));
    DDL_ASSERT(IS_VALID_DATA_SRC(u16Src));

    SET_REG16_BIT(DACx->DACR, u16Src << (DAC_DACR_EXTDSL1_POS + u16Ch));
}

/**
 * @brief  DAC data register's data alignment pattern configuration
 * @param  [in] DACx       Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16Align   Specify the data alignment.
 *         This parameter can be a value of @ref DAC_DATAREG_ALIGN_PATTERN
 *         - DAC_DATA_ALIGN_L:  left alignment
 *         - DAC_DATA_ALIGN_R:  right alignment
 * @retval None
 */
void DAC_DataRegAlignConfig(M4_DAC_TypeDef *DACx, uint16_t u16Align)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_DATA_ALIGN(u16Align));

    SET_REG16_BIT(DACx->DACR, u16Align);
}

/**
 * @brief  DAC output function command
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16Ch      Specify DAC channel @ref DAC_CH.
 *         This parameter can be one of the following values:
 *         @arg DAC_CH_1
 *         @arg DAC_CH_2
 * @param  [in] enNewState           New state of the DAC output function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void DAC_OutputCmd(M4_DAC_TypeDef *DACx, uint16_t u16Ch, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_CH(u16Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    const uint16_t u16Cmd = (uint16_t)1U << (DAC_DAOCR_DAODIS1_POS + u16Ch);

    if(Enable == enNewState)
    {
        CLEAR_REG16_BIT(DACx->DAOCR, u16Cmd);
    }
    else
    {
        SET_REG16_BIT(DACx->DAOCR, u16Cmd);
    }
}

/**
 * @brief  DAC AMP function command
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16Ch      Specify DAC channel @ref DAC_CH.
 *         This parameter can be one of the following values:
 *         @arg DAC_CH_1
 *         @arg DAC_CH_2
 * @param  [in] enNewState           New state of the AMP function,
 *                                   @ref en_functional_state_t
 * @retval An en_result_t enumeration value:
 *         - Ok: No errors occurred
 *         - ErrorInvalidMode: cannot enable AMP when data source is from DCU
 */
en_result_t DAC_AMPCmd(M4_DAC_TypeDef *DACx, uint16_t u16Ch, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_CH(u16Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    en_result_t ret = Ok;
    uint16_t u16Cmd = (uint16_t)1U << (DAC_DACR_EXTDSL1_POS + u16Ch);

    if(0U != (READ_REG16_BIT(DACx->DACR, u16Cmd)))
    {
        ret = ErrorInvalidMode;
    }
    else
    {
        u16Cmd = (uint16_t)1U << (DAC_DACR_DAAMP1_POS + u16Ch);

        if(Enable == enNewState)
        {
            SET_REG16_BIT(DACx->DACR, u16Cmd);
        }
        else
        {
            CLEAR_REG16_BIT(DACx->DACR, u16Cmd);
        }
    }

    return ret;
}

/**
 * @brief  DAC ADC priority function command
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] enNewState           New state of the ADC priority function,
 *                                   @ref en_functional_state_t
 * @retval None
 * @note   please make sure ADC is in stoped status before calling DAC_ADCPrioCmd
 */
void DAC_ADCPrioCmd(M4_DAC_TypeDef *DACx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG16_BIT(DACx->DAADPCR, DAC_DAADPCR_ADPEN);
    }
    else
    {
        CLEAR_REG16_BIT(DACx->DAADPCR, DAC_DAADPCR_ADPEN);
    }
}

/**
 * @brief  Enable or Disable the ADP priority for the selected ADCx
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16ADCxPrio  ADCx priority to be enabled or disabled.
 *         This parameter can be one or any combination of the following values:
 *         @arg DAC_ADP_SELECT_ADC1
 *         @arg DAC_ADP_SELECT_ADC2
 *         @arg DAC_ADP_SELECT_ADC3
 * @param  [in] enNewState    New state of ADCx priority
 *                            @ref en_functional_state_t
 * @retval None
 * @note   please make sure ADC is in stoped status before calling DAC_ADCPrioConfig
 */
void DAC_ADCPrioConfig(M4_DAC_TypeDef *DACx, uint16_t u16ADCxPrio, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_ADCPRIO_CONFIG(u16ADCxPrio));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG16_BIT(DACx->DAADPCR, u16ADCxPrio);
    }
    else
    {
        CLEAR_REG16_BIT(DACx->DAADPCR, u16ADCxPrio);
    }
}

/**
 * @brief  DAC channel 1 or channel 2 function command
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16Ch      Specify DAC channel @ref DAC_CH.
 *         This parameter can be one of the following values:
 *         @arg DAC_CH_1
 *         @arg DAC_CH_2
 * @param  [in] enNewState    New state of DAC channel 1 or channel 2 function,
 *                            @ref en_functional_state_t
 * @retval An en_result_t enumeration value:
 *         - Ok: No errors occurred
 *         - ErrorInvalidMode: cannot enable or disable single channel when \n
 *                           these two channels have already been enabled by \n
 *                           @ref DAC_DualChannelCmd
 */
en_result_t DAC_ChannelCmd(M4_DAC_TypeDef *DACx, uint16_t u16Ch, en_functional_state_t enNewState)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_CH(u16Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if((DACx->DACR & DAC_DACR_DAE) != 0U)
    {
        enRet = ErrorInvalidMode;
    }
    else
    {
        const uint16_t u16Cmd = (uint16_t)1U << (DAC_DACR_DA1E_POS + u16Ch);

        if(Enable == enNewState)
        {
            SET_REG16_BIT(DACx->DACR, u16Cmd);
        }
        else
        {
            CLEAR_REG16_BIT(DACx->DACR, u16Cmd);
        }
    }

    return enRet;
}

/**
 * @brief  DAC channel 1 and channel 2 function command
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] enNewState    New state of DAC channel 1 and channel 2 function,
 *                            @ref en_functional_state_t
 * @retval None
 */
void DAC_DualChannelCmd(M4_DAC_TypeDef *DACx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG16_BIT(DACx->DACR, DAC_DACR_DAE);
    }
    else
    {
        CLEAR_REG16_BIT(DACx->DACR, DAC_DACR_DAE);
    }
}

/**
 * @brief  Set the specified data holding register value for DAC channel 1
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] data   Data to be loaded into data holding register of channel 1
 * @retval None
 */
void DAC_SetChannel1Data(M4_DAC_TypeDef *DACx, uint16_t data)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));

    if(READ_REG16_BIT(DACx->DACR, DAC_DACR_DPSEL) == DAC_DATA_ALIGN_L)
    {
        DDL_ASSERT(0U == (data & 0xFU));
    }
    else
    {
        DDL_ASSERT(0U == (data & 0xF000U));
    }

    WRITE_REG16(DACx->DADR1,data);
}

/**
 * @brief  Set the specified data holding register value for DAC channel 2
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] data   Data to be loaded into data holding register of channel 2
 * @retval None
 */
void DAC_SetChannel2Data(M4_DAC_TypeDef *DACx, uint16_t data)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));

    if(READ_REG16_BIT(DACx->DACR, DAC_DACR_DPSEL) == DAC_DATA_ALIGN_L)
    {
        DDL_ASSERT(0U == (data & 0xFU));
    }
    else
    {
        DDL_ASSERT(0U == (data & 0xF000U));
    }

    WRITE_REG16(DACx->DADR2,data);
}

/**
 * @brief  Set the specified data holding register value for channel 1 and channel 2
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  data2:    Data to be loaded into data holding register of channel 2
 * @param  data1:    Data to be loaded into data holding register of channel 1
 * @retval None
 */
void DAC_SetDualChannelData(M4_DAC_TypeDef *DACx, uint16_t data2, uint16_t data1)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));

    if(READ_REG16_BIT(DACx->DACR, DAC_DACR_DPSEL) == DAC_DATA_ALIGN_L)
    {
        DDL_ASSERT(0U == (data1 & 0xFU));
        DDL_ASSERT(0U == (data2 & 0xFU));
    }
    else
    {
        DDL_ASSERT(0U == (data1 & 0xF000U));
        DDL_ASSERT(0U == (data2 & 0xF000U));
    }

    WRITE_REG16(DACx->DADR1,data1);
    WRITE_REG16(DACx->DADR2,data2);
}

/**
 * @brief  Get convert status of channel 1 in ADC priority mode
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @retval An en_dac_conv_sate_t enumeration value:
 *         - ErrorInvalidMode: Could not get convert status when adc priority is not enabled
 *         - Ok: Data convert completed
 *         - OperationInProgress: Data convert is ongoing
 */
en_result_t  DAC_GetChannel1ConvState(const M4_DAC_TypeDef *DACx)
{
    en_result_t enStat = ErrorInvalidMode;

    DDL_ASSERT(IS_VALID_UNIT(DACx));

    if(0U != READ_REG16_BIT(DACx->DAADPCR, DAC_DAADPCR_ADPEN))
    {
        enStat = OperationInProgress;

        if(READ_REG16_BIT(DACx->DAADPCR, DAC_DAADPCR_DA1SF) == 0U)
        {
            enStat = Ok;
        }
    }

    return enStat;
}

/**
 * @brief  Get convert status of channel 2 in ADC priority mode
 * @param  [in] DACx   Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @retval An en_dac_conv_sate_t enumeration value:
 *         - ErrorInvalidMode: Could not get convert status when adc priority is not enabled
 *         - Ok: Data convert completed
 *         - OperationInProgress: Data convert is ongoing
 */
en_result_t DAC_GetChannel2ConvState(const M4_DAC_TypeDef *DACx)
{
    en_result_t enStat = ErrorInvalidMode;

    DDL_ASSERT(IS_VALID_UNIT(DACx));

    if(0U != READ_REG16_BIT(DACx->DAADPCR, DAC_DAADPCR_ADPEN))
    {
        enStat = OperationInProgress;

        if(READ_REG16_BIT(DACx->DAADPCR, DAC_DAADPCR_DA2SF) == 0U)
        {
            enStat = Ok;
        }
    }

    return enStat;
}

/**
 * @brief  Fills each pstcInit member with its default value
 * @param  [in] pstcInit   pointer to a stc_dac_init_t structure which will
 *         be initialized.
 * @retval An en_result_t enumeration value.
 *         - Ok: No errors occurred.
 *         - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t DAC_StructInit(stc_dac_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if(pstcInit != NULL)
    {
        pstcInit->u16Src = DAC_DATA_SRC_DATAREG;
        pstcInit->enOutput = Enable;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Initialize the DAC peripheral according to the specified parameters
 *         in the stc_dac_init_t
 * @param  [in] DACx       Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @param  [in] u16Ch      Specify the DAC channel @ref DAC_CH.
 *         This parameter can be one of the following values:
 *         @arg DAC_CH_1
 *         @arg DAC_CH_2
 * @param  [in] pstcInit   pointer to a stc_dac_init_t structure that contains
 *         the configuration information for the  specified DAC channel.
 * @retval An en_result_t enumeration value:
 *         - Ok: Initialize successfully
 *         - ErrorInvalidParameter: pstcInit = NULL
 */
en_result_t DAC_Init(M4_DAC_TypeDef *DACx, uint16_t u16Ch, const stc_dac_init_t *pstcInit)
{
    DDL_ASSERT(IS_VALID_UNIT(DACx));
    DDL_ASSERT(IS_VALID_CH(u16Ch));

    en_result_t enRet = ErrorInvalidParameter;

    if(pstcInit != NULL)
    {
        DDL_ASSERT(IS_VALID_DATA_SRC(pstcInit->u16Src));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInit->enOutput));

        DAC_SetDataSource(DACx, u16Ch, pstcInit->u16Src);
        DAC_OutputCmd(DACx, u16Ch, pstcInit->enOutput);
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Deinitialize the DAC peripheral registers to their default reset values.
 * @param  [in] DACx       Pointer to the DAC peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_DAC1
 *         @arg M4_DAC2
 * @retval None
 */
void DAC_DeInit(M4_DAC_TypeDef *DACx)
{
    DAC_DualChannelCmd(DACx, Disable);

    DAC_SetDataSource(DACx, DAC_CH_1, DAC_DATA_SRC_DATAREG);
    DAC_SetDataSource(DACx, DAC_CH_2, DAC_DATA_SRC_DATAREG);

    DAC_DataRegAlignConfig(DACx, DAC_DATA_ALIGN_R);

    DAC_AMPCmd(DACx, DAC_CH_1,Disable);
    DAC_AMPCmd(DACx, DAC_CH_2,Disable);

    DAC_OutputCmd(DACx, DAC_CH_1, Enable);
    DAC_OutputCmd(DACx, DAC_CH_2, Enable);

    DAC_ADCPrioConfig(DACx, DAC_ADP_SELECT_ALL, Disable);
    DAC_ADCPrioCmd(DACx, Disable);
}

/**
 * @}
 */

#endif /* DDL_DAC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
