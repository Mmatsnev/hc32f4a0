/**
 *******************************************************************************
 * @file  hc32f4a0_crc.h
 * @brief This file contains all the functions prototypes of the CRC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb            First version
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
#ifndef __HC32F4A0_CRC_H__
#define __HC32F4A0_CRC_H__

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
 * @addtogroup DDL_CRC
 * @{
 */

#if (DDL_CRC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Bits definitions of CRC control register(CRC_CR). */
/**
 * @defgroup CRC_Global_Macros CRC Global Macros
 * @{
 */

/**
 * @defgroup CRC_Protocol_Control_Bit CRC Protocol Control Bit
 * @note: - CRC16 polynomial is X16 + X12 + X5 + 1
 *        - CRC32 polynomial is X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 + \
 *                              X8 + X7 + X5 + X4 + X2 + X + 1
 * @{
 */
#define CRC_CRC16           (0x0UL)
#define CRC_CRC32           (CRC_CR_CR)
/**
 * @}
 */

/**
 * @defgroup CRC_Flag_Bit_Mask CRC Flag Bit Mask
 * @{
 */
#define CRC_FLAG_MASK       (CRC_CR_FLAG)
/**
 * @}
 */

/**
 * @defgroup CRC_Bit_Width CRC Bit Width
 * @{
 */
#define CRC_BW_8            (8U)
#define CRC_BW_16           (16U)
#define CRC_BW_32           (32U)
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
 * @addtogroup CRC_Global_Functions
 * @{
 */
uint32_t CRC_Calculate(uint32_t u32CrcProtocol,
                       const void *pvData,
                       uint32_t u32InitVal,
                       uint32_t u32Length,
                       uint8_t u8BitWidth);

en_flag_status_t CRC_Check(uint32_t u32CrcProtocol,
                           uint32_t u32CheckSum,
                           const void *pvData,
                           uint32_t u32InitVal,
                           uint32_t u32Length,
                           uint8_t u8BitWidth);
/**
 * @}
 */

#endif /* DDL_CRC_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_CRC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
