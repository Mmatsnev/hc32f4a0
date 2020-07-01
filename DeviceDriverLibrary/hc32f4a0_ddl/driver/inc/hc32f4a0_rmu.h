/**
 *******************************************************************************
 * @file  hc32f4a0_rmu.h
 * @brief This file contains all the functions prototypes of the RMU driver
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
#ifndef __HC32F4A0_RMU_H__
#define __HC32F4A0_RMU_H__

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
 * @addtogroup DDL_RMU
 * @{
 */
#if (DDL_RMU_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup RMU_Global_Macros RMU Global Macros
 * @{
 */

/**
 * @defgroup RMU_ResetCause Rmu reset cause
 * @{
 */
#define RMU_RST_POWER_ON              (RMU_RSTF0_PORF)        /*!< Power on reset */
#define RMU_RST_RESET_PIN             (RMU_RSTF0_PINRF)       /*!< Reset pin reset */
#define RMU_RST_BROWN_OUT             (RMU_RSTF0_BORF)        /*!< Brown-out reset */
#define RMU_RST_PVD1                  (RMU_RSTF0_PVD1RF)      /*!< Program voltage Detection 1 reset */
#define RMU_RST_PVD2                  (RMU_RSTF0_PVD2RF)      /*!< Program voltage Detection 2 reset */
#define RMU_RST_WDT                   (RMU_RSTF0_WDRF)        /*!< Watchdog timer reset */
#define RMU_RST_SWDT                  (RMU_RSTF0_SWDRF)       /*!< Special watchdog timer reset */
#define RMU_RST_POWER_DOWN            (RMU_RSTF0_PDRF)        /*!< Power down reset */
#define RMU_RST_SOFTWARE              (RMU_RSTF0_SWRF)        /*!< Software reset */
#define RMU_RST_MPU_ERR               (RMU_RSTF0_MPUERF)      /*!< Mpu error reset */
#define RMU_RST_RAM_PARITY_ERR        (RMU_RSTF0_RAPERF)      /*!< Ram parity error reset */
#define RMU_RST_RAM_ECC               (RMU_RSTF0_RAECRF)      /*!< Ram ECC reset */
#define RMU_RST_CLK_ERR               (RMU_RSTF0_CKFERF)      /*!< Clk frequence error reset */
#define RMU_RST_XTAL_ERR              (RMU_RSTF0_XTALERF)     /*!< Xtal error reset */
#define RMU_RST_LOCKUP                (RMU_RSTF0_LKUPRF)      /*!< M4 Lockup reset */
#define RMU_RST_MULTI                 (RMU_RSTF0_MULTIRF)     /*!< Multiply reset cause */
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
 * @addtogroup RMU_Global_Functions
 * @{
 */

en_flag_status_t RMU_GetStatus(uint32_t u32RmuResetCause);
void RMU_ClrStatus(void);
void RMU_CPULockUpCmd(en_functional_state_t enNewState);

/**
 * @}
 */

#endif /* DDL_RMU_ENABLE */

/**
 * @}
 */

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_RMU_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

