/**
 *******************************************************************************
 * @file  hc32f4a0_sram.h
 * @brief This file contains all the functions prototypes of the SRAM driver
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
#ifndef __HC32F4A0_SRAM_H__
#define __HC32F4A0_SRAM_H__

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
 * @addtogroup DDL_SRAM
 * @{
 */

#if (DDL_SRAM_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SRAM_Global_Macros SRAM Global Macros
 * @{
 */

/**
 * @defgroup SRAM_Index_Bit_Mask SRAM Index Bit Mask
 * @{
 */
#define SRAM_SRAMH              (1UL << 2U)                 /*!< 0x1FFE0000~0x1FFFFFFF, 128KB */
#define SRAM_SRAM123            (1UL << 0U)                 /*!< SRAM1: 0x20000000~0x2001FFFF, 128KB \
                                                                 SRAM2: 0x20020000~0x2003FFFF, 128KB \
                                                                 SRAM3: 0x20040000~0x20057FFF, 96KB */
#define SRAM_SRAM4              (1UL << 1U)                 /*!< 0x20058000~0x2005FFFF, 32KB */
#define SRAM_SRAMB              (1UL << 3U)                 /*!< 0x200F0000~0x200F0FFF, 4KB */
#define SRAM_SRAM_ALL           (SRAM_SRAMH | SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB)
/**
 * @}
 */

/**
 * @defgroup SRAM_Access_Wait_Cycle SRAM Access Wait Cycle
 * @{
 */
#define SRAM_WAIT_CYCLE_0       (0U)                        /*!< Wait 1 CPU cycle. */
#define SRAM_WAIT_CYCLE_1       (1U)                        /*!< Wait 1 CPU cycle. */
#define SRAM_WAIT_CYCLE_2       (2U)                        /*!< Wait 2 CPU cycles. */
#define SRAM_WAIT_CYCLE_3       (3U)                        /*!< Wait 3 CPU cycles. */
#define SRAM_WAIT_CYCLE_4       (4U)                        /*!< Wait 4 CPU cycles. */
#define SRAM_WAIT_CYCLE_5       (5U)                        /*!< Wait 5 CPU cycles. */
#define SRAM_WAIT_CYCLE_6       (6U)                        /*!< Wait 6 CPU cycles. */
#define SRAM_WAIT_CYCLE_7       (7U)                        /*!< Wait 7 CPU cycles. */
/**
 * @}
 */

/**
 * @defgroup SRAM_Operation_After_Check_Error SRAM Operation After Check Error
 * @note For: Even-parity check error of SRAM1, SRAM2, SRAM3 and SRAMH. ECC check error of SRAM4 and SRAMB.
 * @{
 */
#define SRAM_ERR_OP_NMI         (0U)                        /*!< Non-maskable interrupt occurres while check error occurres. */
#define SRAM_ERR_OP_RESET       (SRAMC_CKCR_PYOAD)          /*!< System reset occurres while check error occurres. */
/**
 * @}
 */

/**
 * @defgroup SRAM_ECC_Mode SRAM ECC Mode
 * @note For: SRAM4 and SRAMB.
 * @{
 */
#define SRAM_ECC_MODE_INVALID   (0U)                        /*!< The ECC mode is invalid. */
#define SRAM_ECC_MODE_1         (SRAMC_CKCR_ECCMOD_0)       /*!< When 1-bit error occurres: \
                                                                 ECC error corrects. \
                                                                 No 1-bit-error status flag setting, no interrupt or reset. \
                                                                 When 2-bit error occurres: \
                                                                 ECC error detects. \
                                                                 2-bit-error status flag sets and interrupt or reset occurres. */
#define SRAM_ECC_MODE_2         (SRAMC_CKCR_ECCMOD_1)       /*!< When 1-bit error occurres: \
                                                                 ECC error corrects. \
                                                                 1-bit-error status flag sets, no interrupt or reset. \
                                                                 When 2-bit error occurres: \
                                                                 ECC error detects. \
                                                                 2-bit-error status flag sets and interrupt or reset occurres. */
#define SRAM_ECC_MODE_3         (SRAMC_CKCR_ECCMOD_1 | \
                                 SRAMC_CKCR_ECCMOD_0)       /*!< When 1-bit error occurres: \
                                                                 ECC error corrects. \
                                                                 1-bit-error status flag sets and interrupt or reset occurres. \
                                                                 When 2-bit error occurres: \
                                                                 ECC error detects. \
                                                                 2-bit-error status flag sets and interrupt or reset occurres. */
/**
 * @}
 */

/**
 * @defgroup SRAM_Check_Status_Flag SRAM Check Status Flag
 * @{
 */
#define SRAM_FLAG_SRAM1_PYERR   (SRAMC_CKSR_SRAM1_PYERR)    /*!< SRAM1 parity error. */
#define SRAM_FLAG_SRAM2_PYERR   (SRAMC_CKSR_SRAM2_PYERR)    /*!< SRAM2 parity error. */
#define SRAM_FLAG_SRAM3_PYERR   (SRAMC_CKSR_SRAM3_PYERR)    /*!< SRAM3 parity error. */
#define SRAM_FLAG_SRAMH_PYERR   (SRAMC_CKSR_SRAMH_PYERR)    /*!< SRAMH parity error. */
#define SRAM_FLAG_SRAM4_1ERR    (SRAMC_CKSR_SRAM4_1ERR)     /*!< SRAM4 ECC 1-bit error. */
#define SRAM_FLAG_SRAM4_2ERR    (SRAMC_CKSR_SRAM4_2ERR)     /*!< SRAM4 ECC 2-bit error. */
#define SRAM_FLAG_SRAMB_1ERR    (SRAMC_CKSR_SRAMB_1ERR)     /*!< SRAMB ECC 1-bit error. */
#define SRAM_FLAG_SRAMB_2ERR    (SRAMC_CKSR_SRAMB_2ERR)     /*!< SRAMB ECC 2-bit error. */
#define SRAM_FLAG_CACHE_PYERR   (SRAMC_CKSR_CACHE_PYERR)    /*!< Cache RAM parity error. */
#define SRAM_FLAG_ALL           (0x1FFUL)
/**
 * @}
 */

/**
 * @defgroup SRAM_Register_Protect_Command SRAM Register Protect Command
 * @{
 */
#define SRAM_LOCK_CMD           (0x76U)
#define SRAM_UNLOCK_CMD         (0x77U)
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
 * @addtogroup SRAM_Global_Functions
 * @{
 */

/**
 * @brief  Lock access wait cycle control register.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void SRAM_WTCR_Lock(void)
{
    WRITE_REG32(M4_SRAMC->WTPR, SRAM_LOCK_CMD);
}

/**
 * @brief  Unlock access wait cycle control register.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void SRAM_WTCR_Unlock(void)
{
    WRITE_REG32(M4_SRAMC->WTPR, SRAM_UNLOCK_CMD);
}

/**
 * @brief  Lock check control register.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void SRAM_CKCR_Lock(void)
{
    WRITE_REG32(M4_SRAMC->CKPR, SRAM_LOCK_CMD);
}

/**
 * @brief  Unlock check control register.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void SRAM_CKCR_Unlock(void)
{
    WRITE_REG32(M4_SRAMC->CKPR, SRAM_UNLOCK_CMD);
}

void SRAM_Init(void);
void SRAM_DeInit(void);

void SRAM_WTCR_Lock(void);
void SRAM_WTCR_Unlock(void);
void SRAM_CKCR_Lock(void);
void SRAM_CKCR_Unlock(void);

void SRAM_SetWaitCycle(uint32_t u32SramIndex, uint32_t u32WriteCycle, uint32_t u32ReadCycle);
void SRAM_SetEccMode(uint32_t u32SramIndex, uint32_t u32EccMode);
void SRAM_SetErrOperation(uint32_t u32SramIndex, uint32_t u32OpAfterError);

en_flag_status_t SRAM_GetStatus(uint32_t u32Flag);
void SRAM_ClrStatus(uint32_t u32Flag);

/**
 * @}
 */

#endif /* DDL_SRAM_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_SRAM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
