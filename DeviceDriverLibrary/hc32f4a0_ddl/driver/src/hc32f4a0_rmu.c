/**
 *******************************************************************************
 * @file  hc32f4a0_rmu.c
 * @brief This file provides firmware functions to manage the Reset Manage Unit
 *        (RMU).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb          First version
   2020-07-21       Heqb          Add write protect check for RMU_ClrStatus function
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
#include "hc32f4a0_rmu.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_RMU RMU
 * @brief RMU Driver Library
 * @{
 */

#if (DDL_RMU_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup RMU_Local_Macros RMU Local Macros
 * @{
 */
#define RMU_STATUS_MASK            (0x40007FFFUL)

/**
 * @defgroup RMU_Check_Parameters_Validity RMU Check Parameters Validity
 * @{
 */

/*! Parameter validity check for RMU reset cause. */
#define IS_VALID_RMU_RESET_STATUS(x)                                           \
(   ((x) != 0U)                              ||                                \
    ((x) | RMU_STATUS_MASK) == RMU_STATUS_MASK)

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
 * @defgroup RMU_Global_Functions RMU Global Functions
 * @{
 */

/**
 * @brief  Get the reset cause.
 * @param  [in] u32RmuResetCause    Reset flags that need to be queried
 *   @arg  RMU_RST_POWER_ON:        Power on reset
 *   @arg  RMU_RST_RESET_PIN:       Reset pin reset
 *   @arg  RMU_RST_BROWN_OUT:       Brown-out reset
 *   @arg  RMU_RST_PVD1:            Program voltage Detection 1 reset
 *   @arg  RMU_RST_PVD2:            Program voltage Detection 2 reset
 *   @arg  RMU_RST_WDT:             Watchdog timer reset
 *   @arg  RMU_RST_SWDT:            Special watchdog timer reset
 *   @arg  RMU_RST_POWER_DOWN:      Power down reset
 *   @arg  RMU_RST_SOFTWARE:        Software reset
 *   @arg  RMU_RST_MPU_ERR:         Mpu error reset
 *   @arg  RMU_RST_RAM_PARITY_ERR:  Ram parity error reset
 *   @arg  RMU_RST_RAM_ECC:         Ram ECC reset
 *   @arg  RMU_RST_CLK_ERR:         Clk frequence error reset
 *   @arg  RMU_RST_XTAL_ERR:        Xtal error reset
 *   @arg  RMU_RST_LOCKUP:          M4 Lockup reset
 *   @arg  RMU_RST_MULTI:           Multiply reset cause
 * @retval Set: Flag is Set.
 *         Reset: Flag is Reset
 */
en_flag_status_t RMU_GetStatus(uint32_t u32RmuResetCause)
{
    en_flag_status_t enRet;
    DDL_ASSERT(IS_VALID_RMU_RESET_STATUS(u32RmuResetCause));

    enRet = ((0UL == READ_REG32_BIT(M4_RMU->RSTF0, u32RmuResetCause)) ? Reset :Set);
    return enRet;
}

/**
 * @brief  Clear reset Status.
 * @param  None
 * @retval NOne
 * @note   Clear reset flag should be done after read RMU_RSTF0 register.
 *         Call PWC_Unlock(PWC_UNLOCK_CODE_1) unlock RMU_RSTF0 register first.
 */
void RMU_ClrStatus(void)
{
    DDL_ASSERT((M4_PWC->FPRC & PWC_FPRC_FPRCB1) == PWC_FPRC_FPRCB1);

    SET_REG32_BIT(M4_RMU->RSTF0, RMU_RSTF0_CLRF);
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
}

/**
 * @brief  Enable or disable LOCKUP reset.
 * @param  [in] enNewState    Enable or disable LOCKUP reset.
 * @retval None
 * @note   Call PWC_Unlock(PWC_UNLOCK_CODE_1) unlock RMU_PRSTCR0 register first.
 */
void RMU_CPULockUpCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT((M4_PWC->FPRC & PWC_FPRC_FPRCB1) == PWC_FPRC_FPRCB1);
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    WRITE_REG8(bM4_RMU->PRSTCR0_b.LKUPREN, enNewState);
}

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
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
