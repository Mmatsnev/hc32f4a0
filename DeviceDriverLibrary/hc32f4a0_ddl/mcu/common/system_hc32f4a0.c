/**
 *******************************************************************************
 * @file  system_hc32f4a0.c
 * @brief This file provides two functions and one global variable to be called
 *        from  user application
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
   2020-07-03       Zhangxl         Modify for 16MHz & 20MHz HRC
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
#include "hc32_common.h"

/**
 * @addtogroup CMSIS
 * @{
 */

/**
 * @addtogroup HC32F4A0_System
 * @{
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @addtogroup HC32F4A0_System_Global_Variable
 * @{
 */

/*!< System clock frequency (Core clock) */
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /*!< GNU Compiler */
__NO_INIT uint32_t SystemCoreClock;
__NO_INIT uint32_t HRC_VALUE;
#elif defined (__ICCARM__)                      /*!< IAR Compiler */
__NO_INIT uint32_t SystemCoreClock;
__NO_INIT uint32_t HRC_VALUE;
#else
uint32_t HRC_VALUE = HRC_16MHz_VALUE;
uint32_t SystemCoreClock = MRC_VALUE;
#endif

/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @addtogroup HC32F4A0_System_Global_Functions
 * @{
 */

/**
 * @brief  Setup the microcontroller system. Initialize the System and update
 *         the SystemCoreClock variable.
 * @param  None
 * @retval None
 */
void SystemInit(void)
{
    (*((volatile unsigned short*)(0x400543FEUL)))=0xA50BU;
    (*((volatile unsigned int*)(0x4004CCE8UL)))=0x00040000UL;
    (*((volatile unsigned short*)(0x400543FEUL)))=0xA500U;
    /* FPU settings */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif
    SystemCoreClockUpdate();
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 * @param  None
 * @retval None
 */
void SystemCoreClockUpdate(void)
{
    uint8_t tmp;
    uint32_t plln;
    uint32_t pllp;
    uint32_t pllm;

    /* Select proper HRC_VALUE according to ICG1.HRCFREQSEL bit */
    /* ICG1.HRCFREQSEL = '0' represent HRC_VALUE = 20000000UL   */
    /* ICG1.HRCFREQSEL = '1' represent HRC_VALUE = 16000000UL   */
    if (1UL == (HRC_FREQ_MON() & 1UL))
    {
        HRC_VALUE = HRC_16MHz_VALUE;
    }
    else
    {
        HRC_VALUE = HRC_20MHz_VALUE;
    }

    tmp = M4_CMU->CKSWR & CMU_CKSWR_CKSW;
    switch(tmp)
    {
        case 0x00U:  /* use internal high speed RC */
            SystemCoreClock = HRC_VALUE;
            break;
        case 0x01U:  /* use internal middle speed RC */
            SystemCoreClock = MRC_VALUE;
            break;
        case 0x02U:  /* use internal low speed RC */
            SystemCoreClock = LRC_VALUE;
            break;
        case 0x03U:  /* use external high speed OSC */
            SystemCoreClock = XTAL_VALUE;
            break;
        case 0x04U:  /* use external low speed OSC */
            SystemCoreClock = XTAL32_VALUE;
            break;
        case 0x05U:  /* use PLLH */
            /* PLLCLK = ((pllsrc / pllm) * plln) / pllp */
            pllp = (uint32_t)((M4_CMU->PLLHCFGR >> CMU_PLLHCFGR_PLLHP_POS) & 0x0FUL);
            plln = (uint32_t)((M4_CMU->PLLHCFGR >> CMU_PLLHCFGR_PLLHN_POS) & 0xFFUL);
            pllm = (uint32_t)((M4_CMU->PLLHCFGR >> CMU_PLLHCFGR_PLLHM_POS) & 0x03UL);

            /* use external high speed OSC as PLL source */
            if (0UL == bM4_CMU->PLLHCFGR_b.PLLSRC)
            {
                SystemCoreClock = (XTAL_VALUE) / (pllm + 1UL) * (plln + 1UL) / (pllp + 1UL);
            }
            /* use internal high RC as PLL source */
            else
            {
                SystemCoreClock = (HRC_VALUE) / (pllm + 1UL) * (plln + 1UL) / (pllp + 1UL);
            }
            break;
        default:
            break;
    }
}

#if defined (__CC_ARM) || defined (__CLANG_ARM)
extern int32_t $Super$$main(void);
/* re-define main function */
int $Sub$$main(void)
{
    SystemCoreClockUpdate();
    $Super$$main();
    return 0;
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
