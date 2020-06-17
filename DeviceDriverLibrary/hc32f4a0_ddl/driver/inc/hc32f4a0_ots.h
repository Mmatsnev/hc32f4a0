/**
 *******************************************************************************
 * @file  hc32f4a0_ots.h
 * @brief This file contains all the functions prototypes of the OTS driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
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
#ifndef __HC32F4A0_OTS_H__
#define __HC32F4A0_OTS_H__

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
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_OTS
 * @{
 */

#if (DDL_OTS_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup OTS_Global_Types OTS Global Types
 * @{
 */
typedef struct
{
    uint16_t u16ClkSel;
    uint16_t u16AutOffEn;
    float32_t f32ParaK;
    float32_t f32ParaM;
    uint32_t u32ClkMHz;
    en_functional_state_t enUseDefaultPara;
} stc_ots_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup OTS_Global_Macros OTS Global Macros
 * @{
 */

/**
 * @defgroup OTS_Clock_Selection OTS Clock Selection
 * @{
 */
#define OTS_CLK_XTAL                    (0x0U)              /*!< Select XTAL as OTS clock. */
#define OTS_CLK_HRC                     (OTS_CTL_OTSCK)     /*!< Select HRC as OTS clock */
/**
 * @}
 */

/**
 * @defgroup OTS_Automatic_Off_Command OTS Automatic Off Command
 * @{
 */
#define OTS_AUTO_OFF_DISABLE            (0x0U)
#define OTS_AUTO_OFF_ENABLE             (OTS_CTL_TSSTP)
/**
 * @}
 */

/**
 * @defgroup OTS_Common_Trigger_Sel OTS Common Trigger Source Select
 * @{
 */
#define OTS_TRIG_COM1_COM2_OFF          (0x00UL)
#define OTS_TRIG_COM1_ON_COM2_OFF       (0x01UL << 31U)
#define OTS_TRIG_COM1_OFF_COM2_ON       (0x01UL << 30U)
#define OTS_TRIG_COM1_COM2_ON           (0x03UL << 30U)
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
 * @addtogroup OTS_Global_Functions
 * @{
 */

/**
 * @brief  Start OTS.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void OTS_Start(void)
{
    bM4_OTS->CTL_b.OTSST = (uint32_t)1U;
}

/**
 * @brief  Stop OTS.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void OTS_Stop(void)
{
    bM4_OTS->CTL_b.OTSST = (uint32_t)0U;
}

en_result_t OTS_Init(const stc_ots_init_t *pstcInit);
en_result_t OTS_StructInit(stc_ots_init_t *pstcInit);
void OTS_DeInit(void);

en_result_t OTS_Polling(float32_t *pf32Temp, uint32_t u32Timeout);

void OTS_IntCmd(en_functional_state_t enNewState);
void OTS_SetTriggerSrc(en_event_src_t enEvent);
void OTS_ComTrigCmd(uint32_t u32ComTrigEn);

en_result_t OTS_ScalingExperiment(uint16_t *pu16Dr1, uint16_t *pu16Dr2, \
                                  uint16_t *pu16Ecr, float32_t *pf32A,  \
                                  uint32_t u32Timeout);

float OTS_CalculateTemp(void);

/**
 * @}
 */

#endif /* DDL_OTS_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_OTS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
