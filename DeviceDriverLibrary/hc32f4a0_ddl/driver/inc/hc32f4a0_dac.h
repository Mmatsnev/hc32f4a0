/**
 *******************************************************************************
 * @file  hc32f4a0_dac.h
 * @brief This file contains all the functions prototypes of the DAC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao          First version
   2020-07-15       Hexiao          1. Modify DAC_ChannelCmd to DAC_Start and DAC_Stop
                                    2. Modify DAC_DualChannelCmd to DAC_DualChannelStart
                                       and DAC_DualChannelStop
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
#ifndef __HC32F4A0_DAC_H__
#define __HC32F4A0_DAC_H__

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
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_DAC
 * @{
 */

#if (DDL_DAC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup DAC_Global_Types DAC Global Types
 * @{
 */

/**
 * @brief Structure definition of DAC initialization.
 */
typedef struct
{
    uint16_t u16Src;                 /*!< Data source to be converted
                                     This parameter can be a value of @ref DAC_DATA_SRC */

    en_functional_state_t enOutput;  /*!< Enable or disable analog output
                                     This parameter can be a value of @ref en_functional_state_t */

} stc_dac_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup DAC_Global_Macros DAC Global Macros
 * @{
 */

/**
 * @defgroup DAC_CH DAC channel
 * @{
 */
#define DAC_CH_1                           (0U)
#define DAC_CH_2                           (1U)
/**
 * @}
 */

/**
 * @defgroup DAC_DATA_SRC DAC data source
 * @{
 */
#define DAC_DATA_SRC_DATAREG               (0U)
#define DAC_DATA_SRC_DCU                   (1U)
/**
 * @}
 */

/**
 * @defgroup DAC_DATAREG_ALIGN_PATTERN DAC data register alignment pattern
 * @{
 */
#define DAC_DATA_ALIGN_L                   (DAC_DACR_DPSEL)
#define DAC_DATA_ALIGN_R                   (0U)
/**
 * @}
 */

#define DAC_DATAREG_VALUE_MAX              (4096UL)

/**
 * @defgroup DAC_ADP_SELECT DAC ADCx priority select
 * @{
 */
#define DAC_ADP_SELECT_ADC1                (DAC_DAADPCR_ADPSL1)
#define DAC_ADP_SELECT_ADC2                (DAC_DAADPCR_ADPSL2)
#define DAC_ADP_SELECT_ADC3                (DAC_DAADPCR_ADPSL3)
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
 * @addtogroup DAC_Global_Functions
 * @{
 */

en_result_t DAC_StructInit(stc_dac_init_t * pstcInit);
en_result_t DAC_Init(M4_DAC_TypeDef *DACx, uint16_t u16Ch, const stc_dac_init_t *pstcInit);
void DAC_DeInit(M4_DAC_TypeDef *DACx);

void DAC_SetDataSource(M4_DAC_TypeDef *DACx, uint16_t u16Ch, uint16_t u16Src);
void DAC_DataRegAlignConfig(M4_DAC_TypeDef *DACx, uint16_t u16Align);
void DAC_OutputCmd(M4_DAC_TypeDef *DACx, uint16_t u16Ch, en_functional_state_t enNewState);
en_result_t DAC_AMPCmd(M4_DAC_TypeDef *DACx, uint16_t u16Ch, en_functional_state_t enNewState);
void DAC_ADCPrioCmd(M4_DAC_TypeDef *DACx, en_functional_state_t enNewState);
void DAC_ADCPrioConfig(M4_DAC_TypeDef *DACx, uint16_t u16ADCxPrio, en_functional_state_t enNewState);

en_result_t DAC_Start(M4_DAC_TypeDef *DACx, uint16_t u16Ch);
en_result_t DAC_Stop(M4_DAC_TypeDef *DACx, uint16_t u16Ch);
void DAC_DualChannelStart(M4_DAC_TypeDef *DACx);
void DAC_DualChannelStop(M4_DAC_TypeDef *DACx);

void DAC_SetChannel1Data(M4_DAC_TypeDef *DACx, uint16_t data);
void DAC_SetChannel2Data(M4_DAC_TypeDef *DACx, uint16_t data);
void DAC_SetDualChannelData(M4_DAC_TypeDef *DACx, uint16_t data2, uint16_t data1);
en_result_t  DAC_GetChannel1ConvState(const M4_DAC_TypeDef *DACx);
en_result_t DAC_GetChannel2ConvState(const M4_DAC_TypeDef *DACx);

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

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_DAC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
