/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_w25qxx.h
 * @brief This file contains all the functions prototypes of the
          ev_hc32f4a0_lqfp176_w25qxx driver library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin         First version
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
#ifndef __EV_HC32F4A0_LQFP176_W25QXX_H__
#define __EV_HC32F4A0_LQFP176_W25QXX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
//#include "ev_hc32f4a0_lqfp176.h"
#include "w25qxx.h"
#include "hc32_ddl.h"

/**
 * @addtogroup BSP
 * @{
 */

/** @addtogroup EV_HC32F4A0_LQFP176
  * @{
  */

/** @addtogroup EV_HC32F4A0_LQFP176_W25QXX
  * @{
  */

#if ((BSP_W25QXX_ENABLE == BSP_ON) && \
     (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_W25QXX_Global_Macros BSP W25QXX Global Macros
 * @{
 */
/* SPI unit define */
#define W25Q_SPI_UNIT                (M4_SPI1)
#define W25Q_SPI_UNIT_CLOCK          (PWC_FCG1_SPI1)

/* Port define */
#define W25Q_CS_PORT                 (GPIO_PORT_C)
#define W25Q_CS_PIN                  (GPIO_PIN_07)

#define W25Q_SCK_PORT                (GPIO_PORT_C)
#define W25Q_SCK_PIN                 (GPIO_PIN_06)
#define W25Q_SCK_FUNC                (GPIO_FUNC_40_SPI1_SCK)

#define W25Q_MOSI_PORT               (GPIO_PORT_B)
#define W25Q_MOSI_PIN                (GPIO_PIN_13)
#define W25Q_MOSI_FUNC               (GPIO_FUNC_41_SPI1_MOSI)

#define W25Q_MISO_PORT               (GPIO_PORT_B)
#define W25Q_MISO_PIN                (GPIO_PIN_12)
#define W25Q_MISO_FUNC               (GPIO_FUNC_42_SPI1_MISO)

/* CS pin control define */
#define W25Q_CS_ACTIVE()             GPIO_ResetPins(W25Q_CS_PORT, W25Q_CS_PIN)
#define W25Q_CS_INACTIVE()           GPIO_SetPins(W25Q_CS_PORT, W25Q_CS_PIN)

#define W25QXX_DELAY_MS(x)           DDL_DelayMS((x))
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
 * @defgroup EV_HC32F4A0_LQFP176_W25QXX_Global_Functions HC32F4A0 QSPI/SPI_NOR Global Functions
 * @{
 */
void BSP_W25Q_SPI_Init(void);

en_result_t BSP_W25Q_SPI_Transmit(const void *pvTxBuf, uint32_t u32TxLength);
en_result_t BSP_W25Q_SPI_Receive(void *pvRxBuf, uint32_t u32RxLength);

/**
 * @}
 */

#endif /* BSP_W25QXX_ENABLE && (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0) */

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

#endif /* __EV_HC32F4A0_LQFP176_W25QXX_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
