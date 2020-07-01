/**
 *******************************************************************************
 * @file  hc32f4a0_interrupts.h
 * @brief This file contains all the functions prototypes of the interrupt driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
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
#ifndef __HC32F4A0_INTERRUPTS_H__
#define __HC32F4A0_INTERRUPTS_H__

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
 * @addtogroup DDL_INTERRUPTS
 * @{
 */

#if (DDL_INTERRUPTS_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Types INTC Global Types
 * @{
 */
/**
 * @brief  Interrupt registration structure definition
 */
typedef struct
{
    en_int_src_t    enIntSrc;   /*!< Peripheral interrupt number, can be any value @ref en_int_src_t    */
    IRQn_Type       enIRQn;     /*!< Peripheral IRQ type, can be Int000_IRQn~Int127_IRQn @ref IRQn_Type */
    func_ptr_t      pfnCallback;/*!< Callback function for corresponding peripheral IRQ                 */
} stc_irq_signin_config_t;

/**
 * @brief  NMI initialize configuration structure definition
 */
typedef struct
{
    uint32_t    u32NmiSrc;      /*!< NMI trigger source, @ref NMI_TriggerSrc_Sel for details                */
    func_ptr_t  pfnNmiCallback; /*!< NMI Callback function pointers                                         */
}stc_nmi_init_t;

/**
 * @brief  EXINT initialize configuration structure definition
 */
typedef struct
{
    uint32_t    u32ExIntCh;     /*!< ExInt CH.0~15 @ref EXINT_Channel_Sel */
    uint32_t    u32ExIntFAE;    /*!< ExInt filter A function setting, @ref EXINT_FilterAClock_Sel for details */
    uint32_t    u32ExIntFAClk;  /*!< ExInt filter A clock division, @ref EXINT_FilterAClock_Div for details */
    uint32_t    u32ExIntFBE;    /*!< ExInt filter B function setting, @ref EXINT_FilterBClock_Sel for details*/
    uint32_t    u32ExIntFBTime; /*!< ExInt filter B time, @ref EXINT_FilterBTim_Sel for details*/
    uint32_t    u32ExIntLvl;    /*!< ExInt trigger edge, @ref EXINT_Trigger_Sel for details             */
}stc_exint_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Macros INTC Global Macros
 * @{
 */
/**
 * @defgroup INTC_DefaultPriority_Sel Interrupt default priority level
 * Possible values are 0 (high priority) to 15 (low priority)
 * @{
 */
#define DDL_IRQ_PRIORITY_DEFAULT    (15U)
/**
 * @}
 */

/**
 * @defgroup INTC_Priority_Sel Interrupt priority level 00 ~ 15
 * @{
 */
#define DDL_IRQ_PRIORITY_00         (0U)
#define DDL_IRQ_PRIORITY_01         (1U)
#define DDL_IRQ_PRIORITY_02         (2U)
#define DDL_IRQ_PRIORITY_03         (3U)
#define DDL_IRQ_PRIORITY_04         (4U)
#define DDL_IRQ_PRIORITY_05         (5U)
#define DDL_IRQ_PRIORITY_06         (6U)
#define DDL_IRQ_PRIORITY_07         (7U)
#define DDL_IRQ_PRIORITY_08         (8U)
#define DDL_IRQ_PRIORITY_09         (9U)
#define DDL_IRQ_PRIORITY_10         (10U)
#define DDL_IRQ_PRIORITY_11         (11U)
#define DDL_IRQ_PRIORITY_12         (12U)
#define DDL_IRQ_PRIORITY_13         (13U)
#define DDL_IRQ_PRIORITY_14         (14U)
#define DDL_IRQ_PRIORITY_15         (15U)
/**
 * @}
 */

/**
 * @defgroup INTC_Bit_mask definition Bit Mask
 * @{
 */
#define     BIT_MASK_00             (1UL << 0U)
#define     BIT_MASK_01             (1UL << 1U)
#define     BIT_MASK_02             (1UL << 2U)
#define     BIT_MASK_03             (1UL << 3U)
#define     BIT_MASK_04             (1UL << 4U)
#define     BIT_MASK_05             (1UL << 5U)
#define     BIT_MASK_06             (1UL << 6U)
#define     BIT_MASK_07             (1UL << 7U)
#define     BIT_MASK_08             (1UL << 8U)
#define     BIT_MASK_09             (1UL << 9U)
#define     BIT_MASK_10             (1UL << 10U)
#define     BIT_MASK_11             (1UL << 11U)
#define     BIT_MASK_12             (1UL << 12U)
#define     BIT_MASK_13             (1UL << 13U)
#define     BIT_MASK_14             (1UL << 14U)
#define     BIT_MASK_15             (1UL << 15U)
#define     BIT_MASK_16             (1UL << 16U)
#define     BIT_MASK_17             (1UL << 17U)
#define     BIT_MASK_18             (1UL << 18U)
#define     BIT_MASK_19             (1UL << 19U)
#define     BIT_MASK_20             (1UL << 20U)
#define     BIT_MASK_21             (1UL << 21U)
#define     BIT_MASK_22             (1UL << 22U)
#define     BIT_MASK_23             (1UL << 23U)
#define     BIT_MASK_24             (1UL << 24U)
#define     BIT_MASK_25             (1UL << 25U)
#define     BIT_MASK_26             (1UL << 26U)
#define     BIT_MASK_27             (1UL << 27U)
#define     BIT_MASK_28             (1UL << 28U)
#define     BIT_MASK_29             (1UL << 29U)
#define     BIT_MASK_30             (1UL << 30U)
#define     BIT_MASK_31             (1UL << 31U)
/**
 * @}
 */

/**
 * @defgroup NMI_TriggerSrc_Sel NMI trigger source selection
 * @{
 */
#define NMI_SRC_SWDT                (1UL << INTC_NMIFR_SWDTFR_POS)
#define NMI_SRC_PVD1                (1UL << INTC_NMIFR_PVD1FR_POS)
#define NMI_SRC_PVD2                (1UL << INTC_NMIFR_PVD2FR_POS)
#define NMI_SRC_XTAL                (1UL << INTC_NMIFR_XTALSTPFR_POS)
#define NMI_SRC_SRAM_PARITY         (1UL << INTC_NMIFR_REPFR_POS)
#define NMI_SRC_SRAM_ECC            (1UL << INTC_NMIFR_RECCFR_POS)
#define NMI_SRC_BUS_ERR             (1UL << INTC_NMIFR_BUSMFR_POS)
#define NMI_SRC_WDT                 (1UL << INTC_NMIFR_WDTFR_POS)
#define NMI_SRC_MASK                (NMI_SRC_SWDT   | NMI_SRC_PVD1          |   \
                                    NMI_SRC_PVD2    | NMI_SRC_XTAL          |   \
                                    NMI_SRC_BUS_ERR | NMI_SRC_SRAM_PARITY   |   \
                                    NMI_SRC_WDT     | NMI_SRC_SRAM_ECC)
/**
 * @}
 */

/**
 * @defgroup NOCCR_Register_Msk Noise cancel register mask
 * @{
 */
#define INTC_NOCCR_MASK     (INTC_NOCCR_NOCSEL)

/**
 * @}
 */


/**
 * @defgroup MNI_Register_Msk NMI register mask
 * @{
 */
#define INTC_NMIENR_MASK    (INTC_NMIENR_SWDTENR | INTC_NMIENR_PVD1ENR      |   \
                            INTC_NMIENR_PVD2ENR | INTC_NMIENR_XTALSTPENR    |   \
                            INTC_NMIENR_REPENR  | INTC_NMIENR_RECCENR       |   \
                            INTC_NMIENR_BUSMENR | INTC_NMIENR_WDTENR)

#define INTC_NMIFR_MASK     (INTC_NMIFR_SWDTFR | INTC_NMIFR_PVD1FR          |   \
                            INTC_NMIFR_PVD2FR   | INTC_NMIFR_XTALSTPFR      |   \
                            INTC_NMIFR_REPFR    | INTC_NMIFR_RECCFR         |   \
                            INTC_NMIFR_BUSMFR   | INTC_NMIFR_WDTFR)

#define INTC_NMICLR_MASK    (INTC_NMICFR_SWDTCFR  | INTC_NMICFR_PVD1CFR     |   \
                            INTC_NMICFR_PVD2CFR | INTC_NMICFR_XTALSTPCFR    |   \
                            INTC_NMICFR_REPCFR  | INTC_NMICFR_RECCCFR       |   \
                            INTC_NMICFR_BUSMCFR | INTC_NMICFR_WDTCFR)
/**
 * @}
 */

/**
 * @defgroup EXINT_Channel_Sel External interrupt channel selection
 * @{
 */
#define EXINT_CH00                  (1UL << 0U)
#define EXINT_CH01                  (1UL << 1U)
#define EXINT_CH02                  (1UL << 2U)
#define EXINT_CH03                  (1UL << 3U)
#define EXINT_CH04                  (1UL << 4U)
#define EXINT_CH05                  (1UL << 5U)
#define EXINT_CH06                  (1UL << 6U)
#define EXINT_CH07                  (1UL << 7U)
#define EXINT_CH08                  (1UL << 8U)
#define EXINT_CH09                  (1UL << 9U)
#define EXINT_CH10                  (1UL <<10U)
#define EXINT_CH11                  (1UL <<11U)
#define EXINT_CH12                  (1UL <<12U)
#define EXINT_CH13                  (1UL <<13U)
#define EXINT_CH14                  (1UL <<14U)
#define EXINT_CH15                  (1UL <<15U)
#define EXINT_CH_MASK   (EXINT_CH00 | EXINT_CH01 | EXINT_CH02 | EXINT_CH03 |    \
                         EXINT_CH04 | EXINT_CH05 | EXINT_CH06 | EXINT_CH07 |    \
                         EXINT_CH08 | EXINT_CH09 | EXINT_CH10 | EXINT_CH11 |    \
                         EXINT_CH12 | EXINT_CH13 | EXINT_CH14 | EXINT_CH15)
/**
 * @}
 */

/**
 * @defgroup EXINT_FilterAClock_Sel External interrupt filter A function selection
 * @{
 */
#define EXINT_FILTER_A_OFF          (0UL)
#define EXINT_FILTER_A_ON           (INTC_EIRQCR_EFEN)
/**
 * @}
 */

/**
 * @defgroup EXINT_FilterBClock_Sel External interrupt filter B function selection
 * @{
 */
#define EXINT_FILTER_B_OFF          (0UL)
#define EXINT_FILTER_B_ON           (INTC_EIRQCR_NOCEN)
/**
 * @}
 */

/**
 * @defgroup EXINT_FilterAClock_Div External interrupt filter A sampling clock division selection
 * @{
 */
#define EXINT_FACLK_HCLK_DIV1        (0UL)
#define EXINT_FACLK_HCLK_DIV8        (INTC_EIRQCR_EISMPCLK_0)
#define EXINT_FACLK_HCLK_DIV32       (INTC_EIRQCR_EISMPCLK_1)
#define EXINT_FACLK_HCLK_DIV64       (INTC_EIRQCR_EISMPCLK)
/**
 * @}
 */

/**
 * @defgroup EXINT_FilterBTim_Sel External interrupt filter B time selection
 * @{
 */
#define EXINT_FBTIM_500NS           (0UL << INTC_NOCCR_NOCSEL_POS)
#define EXINT_FBTIM_1US             (1UL << INTC_NOCCR_NOCSEL_POS)
#define EXINT_FBTIM_2US             (2UL << INTC_NOCCR_NOCSEL_POS)
#define EXINT_FBTIM_4US             (3UL << INTC_NOCCR_NOCSEL_POS)
/**
 * @}
 */

/**
 * @defgroup EXINT_Trigger_Sel External interrupt trigger method selection
 * @{
 */
#define EXINT_TRIGGER_FALLING       (0UL)
#define EXINT_TRIGGER_RISING        (INTC_EIRQCR_EIRQTRG_0)
#define EXINT_TRIGGER_BOTH          (INTC_EIRQCR_EIRQTRG_1)
#define EXINT_TRIGGER_LOW           (INTC_EIRQCR_EIRQTRG)

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
 * @addtogroup INTC_Global_Functions
 * @{
 */

/**
 * @brief  AOS software trigger.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void AOS_SW_Trigger(void)
{
    WRITE_REG32(bM4_AOS->INT_SFTTRG_b.STRG, Set);
}

/**
 * @brief  AOS common trigger source 1 config.
 * @param  [in] enTrig can be any value @ref en_event_src_t
 * @retval None
 */
__STATIC_INLINE void AOS_COM_Trigger1(en_event_src_t enTrig)
{
    WRITE_REG32(M4_AOS->COMTRG1, enTrig);
}

/**
 * @brief  AOS common trigger source 2 config.
 * @param  [in] enTrig can be any value @ref en_event_src_t
 * @retval None
 */
__STATIC_INLINE void AOS_COM_Trigger2(en_event_src_t enTrig)
{
    WRITE_REG32(M4_AOS->COMTRG2, enTrig);
}

en_result_t INTC_IrqSignIn(const stc_irq_signin_config_t *pstcIrqSignConfig);
en_result_t INTC_IrqSignOut(IRQn_Type enIRQn);
en_result_t INTC_ShareIrqCmd(en_int_src_t enIntSrc, en_functional_state_t enNewState);
void INTC_WakeupSrcCmd(uint32_t u32WakeupSrc, en_functional_state_t enNewState);
void INTC_EventCmd(uint32_t u32Event, en_functional_state_t enNewState);
void INTC_IntCmd(uint32_t u32Int, en_functional_state_t enNewState);
void INTC_SWICmd(uint32_t u32SWI, en_functional_state_t enNewState);

en_result_t NMI_Init(const stc_nmi_init_t *pstcNmiInit);
en_result_t NMI_StructInit(stc_nmi_init_t *pstcNmiInit);
en_flag_status_t NMI_GetNmiSrc(uint32_t u32NmiSrc);
void NMI_SetNmiSrc(uint32_t u32NmiSrc);
void NMI_ClrNmiSrc(uint32_t u32NmiSrc);

en_result_t EXINT_Init(const stc_exint_init_t *pstcExIntInit);
en_result_t EXINT_StructInit(stc_exint_init_t *pstcExIntInit);
en_flag_status_t EXINT_GetExIntSrc(uint32_t u32ExIntCh);
void EXINT_ClrExIntSrc(uint32_t u32ExIntCh);

__WEAKDEF void NMI_IrqHandler(void);
__WEAKDEF void HardFault_IrqHandler(void);
__WEAKDEF void SVC_IrqHandler(void);
__WEAKDEF void PendSV_IrqHandler(void);
__WEAKDEF void SysTick_IrqHandler(void);

__WEAKDEF void EXTINT_00_IrqHandler(void);
__WEAKDEF void EXTINT_01_IrqHandler(void);
__WEAKDEF void EXTINT_02_IrqHandler(void);
__WEAKDEF void EXTINT_03_IrqHandler(void);
__WEAKDEF void EXTINT_04_IrqHandler(void);
__WEAKDEF void EXTINT_05_IrqHandler(void);
__WEAKDEF void EXTINT_06_IrqHandler(void);
__WEAKDEF void EXTINT_07_IrqHandler(void);
__WEAKDEF void EXTINT_08_IrqHandler(void);
__WEAKDEF void EXTINT_09_IrqHandler(void);
__WEAKDEF void EXTINT_10_IrqHandler(void);
__WEAKDEF void EXTINT_11_IrqHandler(void);
__WEAKDEF void EXTINT_12_IrqHandler(void);
__WEAKDEF void EXTINT_13_IrqHandler(void);
__WEAKDEF void EXTINT_14_IrqHandler(void);
__WEAKDEF void EXTINT_15_IrqHandler(void);

__WEAKDEF void DMA_1_Tc0_IrqHandler(void);
__WEAKDEF void DMA_1_Tc1_IrqHandler(void);
__WEAKDEF void DMA_1_Tc2_IrqHandler(void);
__WEAKDEF void DMA_1_Tc3_IrqHandler(void);
__WEAKDEF void DMA_1_Tc4_IrqHandler(void);
__WEAKDEF void DMA_1_Tc5_IrqHandler(void);
__WEAKDEF void DMA_1_Tc6_IrqHandler(void);
__WEAKDEF void DMA_1_Tc7_IrqHandler(void);
__WEAKDEF void DMA_1_Btc0_IrqHandler(void);
__WEAKDEF void DMA_1_Btc1_IrqHandler(void);
__WEAKDEF void DMA_1_Btc2_IrqHandler(void);
__WEAKDEF void DMA_1_Btc3_IrqHandler(void);
__WEAKDEF void DMA_1_Btc4_IrqHandler(void);
__WEAKDEF void DMA_1_Btc5_IrqHandler(void);
__WEAKDEF void DMA_1_Btc6_IrqHandler(void);
__WEAKDEF void DMA_1_Btc7_IrqHandler(void);
__WEAKDEF void DMA_1_Err0_IrqHandler(void);
__WEAKDEF void DMA_1_Err1_IrqHandler(void);
__WEAKDEF void DMA_1_Err2_IrqHandler(void);
__WEAKDEF void DMA_1_Err3_IrqHandler(void);
__WEAKDEF void DMA_1_Err4_IrqHandler(void);
__WEAKDEF void DMA_1_Err5_IrqHandler(void);
__WEAKDEF void DMA_1_Err6_IrqHandler(void);
__WEAKDEF void DMA_1_Err7_IrqHandler(void);

__WEAKDEF void DMA_2_Tc0_IrqHandler(void);
__WEAKDEF void DMA_2_Tc1_IrqHandler(void);
__WEAKDEF void DMA_2_Tc2_IrqHandler(void);
__WEAKDEF void DMA_2_Tc3_IrqHandler(void);
__WEAKDEF void DMA_2_Tc4_IrqHandler(void);
__WEAKDEF void DMA_2_Tc5_IrqHandler(void);
__WEAKDEF void DMA_2_Tc6_IrqHandler(void);
__WEAKDEF void DMA_2_Tc7_IrqHandler(void);
__WEAKDEF void DMA_2_Btc0_IrqHandler(void);
__WEAKDEF void DMA_2_Btc1_IrqHandler(void);
__WEAKDEF void DMA_2_Btc2_IrqHandler(void);
__WEAKDEF void DMA_2_Btc3_IrqHandler(void);
__WEAKDEF void DMA_2_Btc4_IrqHandler(void);
__WEAKDEF void DMA_2_Btc5_IrqHandler(void);
__WEAKDEF void DMA_2_Btc6_IrqHandler(void);
__WEAKDEF void DMA_2_Btc7_IrqHandler(void);
__WEAKDEF void DMA_2_Err0_IrqHandler(void);
__WEAKDEF void DMA_2_Err1_IrqHandler(void);
__WEAKDEF void DMA_2_Err2_IrqHandler(void);
__WEAKDEF void DMA_2_Err3_IrqHandler(void);
__WEAKDEF void DMA_2_Err4_IrqHandler(void);
__WEAKDEF void DMA_2_Err5_IrqHandler(void);
__WEAKDEF void DMA_2_Err6_IrqHandler(void);
__WEAKDEF void DMA_2_Err7_IrqHandler(void);

__WEAKDEF void EFM_0_PgmEraseErr_IrqHandler(void);
__WEAKDEF void EFM_0_ColErr_IrqHandler(void);
__WEAKDEF void EFM_0_OpEnd_IrqHandler(void);
__WEAKDEF void EFM_1_PgmEraseErr_IrqHandler(void);
__WEAKDEF void EFM_1_ColErr_IrqHandler(void);
__WEAKDEF void EFM_1_OpEnd_IrqHandler(void);

__WEAKDEF void QSPI_Err_IrqHandler(void);

__WEAKDEF void MAU_Sqrt_IrqHandler(void);

__WEAKDEF void DVP_FrameStart_IrqHandler(void);
__WEAKDEF void DVP_FrameEnd_IrqHandler(void);
__WEAKDEF void DVP_LineStart_IrqHandler(void);
__WEAKDEF void DVP_LineEnd_IrqHandler(void);
__WEAKDEF void DVP_SwSyncErr_IrqHandler(void);
__WEAKDEF void DVP_FifoErr_IrqHandler(void);

__WEAKDEF void FMAC_1_IrqHandler(void);
__WEAKDEF void FMAC_2_IrqHandler(void);
__WEAKDEF void FMAC_3_IrqHandler(void);
__WEAKDEF void FMAC_4_IrqHandler(void);

__WEAKDEF void DCU_1_IrqHandler(void);
__WEAKDEF void DCU_2_IrqHandler(void);
__WEAKDEF void DCU_3_IrqHandler(void);
__WEAKDEF void DCU_4_IrqHandler(void);
__WEAKDEF void DCU_5_IrqHandler(void);
__WEAKDEF void DCU_6_IrqHandler(void);
__WEAKDEF void DCU_7_IrqHandler(void);
__WEAKDEF void DCU_8_IrqHandler(void);

__WEAKDEF void TMR0_1_CmpA_IrqHandler(void);
__WEAKDEF void TMR0_1_CmpB_IrqHandler(void);
__WEAKDEF void TMR0_2_CmpA_IrqHandler(void);
__WEAKDEF void TMR0_2_CmpB_IrqHandler(void);

__WEAKDEF void TMR2_1_CmpA_IrqHandler(void);
__WEAKDEF void TMR2_1_CmpB_IrqHandler(void);
__WEAKDEF void TMR2_1_OvfA_IrqHandler(void);
__WEAKDEF void TMR2_1_OvfB_IrqHandler(void);
__WEAKDEF void TMR2_2_CmpA_IrqHandler(void);
__WEAKDEF void TMR2_2_CmpB_IrqHandler(void);
__WEAKDEF void TMR2_2_OvfA_IrqHandler(void);
__WEAKDEF void TMR2_2_OvfB_IrqHandler(void);
__WEAKDEF void TMR2_3_CmpA_IrqHandler(void);
__WEAKDEF void TMR2_3_CmpB_IrqHandler(void);
__WEAKDEF void TMR2_3_OvfA_IrqHandler(void);
__WEAKDEF void TMR2_3_OvfB_IrqHandler(void);
__WEAKDEF void TMR2_4_CmpA_IrqHandler(void);
__WEAKDEF void TMR2_4_CmpB_IrqHandler(void);
__WEAKDEF void TMR2_4_OvfA_IrqHandler(void);
__WEAKDEF void TMR2_4_OvfB_IrqHandler(void);

__WEAKDEF void RTC_TimeStamp0_IrqHandler(void);
__WEAKDEF void RTC_TimeStamp1_IrqHandler(void);
__WEAKDEF void RTC_Alarm_IrqHandler(void);
__WEAKDEF void RTC_Period_IrqHandler(void);

__WEAKDEF void CLK_XtalStop_IrqHandler(void);

__WEAKDEF void SWDT_IrqHandler(void);

__WEAKDEF void WDT_IrqHandler(void);

__WEAKDEF void PWC_WakeupTimer_IrqHandler(void);

__WEAKDEF void TMR6_1_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_1_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_1_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_1_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_1_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_1_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_1_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_1_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_1_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_1_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_1_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_1_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_1_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_2_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_2_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_2_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_2_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_2_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_2_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_2_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_2_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_2_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_2_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_2_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_2_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_2_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_3_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_3_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_3_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_3_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_3_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_3_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_3_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_3_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_3_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_3_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_3_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_3_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_3_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_4_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_4_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_4_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_4_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_4_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_4_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_4_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_4_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_4_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_4_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_4_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_4_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_4_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_5_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_5_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_5_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_5_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_5_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_5_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_5_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_5_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_5_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_5_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_5_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_5_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_5_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_6_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_6_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_6_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_6_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_6_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_6_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_6_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_6_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_6_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_6_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_6_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_6_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_6_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_7_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_7_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_7_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_7_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_7_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_7_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_7_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_7_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_7_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_7_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_7_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_7_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_7_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR6_8_GCmpA_IrqHandler(void);
__WEAKDEF void TMR6_8_GCmpB_IrqHandler(void);
__WEAKDEF void TMR6_8_GCmpC_IrqHandler(void);
__WEAKDEF void TMR6_8_GCmpD_IrqHandler(void);
__WEAKDEF void TMR6_8_GCmpE_IrqHandler(void);
__WEAKDEF void TMR6_8_GCmpF_IrqHandler(void);
__WEAKDEF void TMR6_8_GOvf_IrqHandler(void);
__WEAKDEF void TMR6_8_GUdf_IrqHandler(void);
__WEAKDEF void TMR6_8_Gdte_IrqHandler(void);
__WEAKDEF void TMR6_8_SCmpUpA_IrqHandler(void);
__WEAKDEF void TMR6_8_SCmpDownA_IrqHandler(void);
__WEAKDEF void TMR6_8_SCmpUpB_IrqHandler(void);
__WEAKDEF void TMR6_8_SCmpDownB_IrqHandler(void);

__WEAKDEF void TMR4_1_GCmpUH_IrqHandler(void);
__WEAKDEF void TMR4_1_GCmpUL_IrqHandler(void);
__WEAKDEF void TMR4_1_GCmpVH_IrqHandler(void);
__WEAKDEF void TMR4_1_GCmpVL_IrqHandler(void);
__WEAKDEF void TMR4_1_GCmpWH_IrqHandler(void);
__WEAKDEF void TMR4_1_GCmpWL_IrqHandler(void);
__WEAKDEF void TMR4_1_Ovf_IrqHandler(void);
__WEAKDEF void TMR4_1_Udf_IrqHandler(void);
__WEAKDEF void TMR4_1_ReloadU_IrqHandler(void);
__WEAKDEF void TMR4_1_ReloadV_IrqHandler(void);
__WEAKDEF void TMR4_1_ReloadW_IrqHandler(void);

__WEAKDEF void TMR4_2_GCmpUH_IrqHandler(void);
__WEAKDEF void TMR4_2_GCmpUL_IrqHandler(void);
__WEAKDEF void TMR4_2_GCmpVH_IrqHandler(void);
__WEAKDEF void TMR4_2_GCmpVL_IrqHandler(void);
__WEAKDEF void TMR4_2_GCmpWH_IrqHandler(void);
__WEAKDEF void TMR4_2_GCmpWL_IrqHandler(void);
__WEAKDEF void TMR4_2_Ovf_IrqHandler(void);
__WEAKDEF void TMR4_2_Udf_IrqHandler(void);
__WEAKDEF void TMR4_2_ReloadU_IrqHandler(void);
__WEAKDEF void TMR4_2_ReloadV_IrqHandler(void);
__WEAKDEF void TMR4_2_ReloadW_IrqHandler(void);

__WEAKDEF void TMR4_3_GCmpUH_IrqHandler(void);
__WEAKDEF void TMR4_3_GCmpUL_IrqHandler(void);
__WEAKDEF void TMR4_3_GCmpVH_IrqHandler(void);
__WEAKDEF void TMR4_3_GCmpVL_IrqHandler(void);
__WEAKDEF void TMR4_3_GCmpWH_IrqHandler(void);
__WEAKDEF void TMR4_3_GCmpWL_IrqHandler(void);
__WEAKDEF void TMR4_3_Ovf_IrqHandler(void);
__WEAKDEF void TMR4_3_Udf_IrqHandler(void);
__WEAKDEF void TMR4_3_ReloadU_IrqHandler(void);
__WEAKDEF void TMR4_3_ReloadV_IrqHandler(void);
__WEAKDEF void TMR4_3_ReloadW_IrqHandler(void);

__WEAKDEF void TMRA_1_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_1_Udf_IrqHandler(void);
__WEAKDEF void TMRA_1_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_1_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_1_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_1_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_2_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_2_Udf_IrqHandler(void);
__WEAKDEF void TMRA_2_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_2_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_2_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_2_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_3_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_3_Udf_IrqHandler(void);
__WEAKDEF void TMRA_3_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_3_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_3_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_3_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_4_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_4_Udf_IrqHandler(void);
__WEAKDEF void TMRA_4_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_4_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_4_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_4_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_5_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_5_Udf_IrqHandler(void);
__WEAKDEF void TMRA_5_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_5_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_5_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_5_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_6_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_6_Udf_IrqHandler(void);
__WEAKDEF void TMRA_6_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_6_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_6_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_6_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_7_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_7_Udf_IrqHandler(void);
__WEAKDEF void TMRA_7_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_7_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_7_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_7_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_8_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_8_Udf_IrqHandler(void);
__WEAKDEF void TMRA_8_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_8_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_8_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_8_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_9_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_9_Udf_IrqHandler(void);
__WEAKDEF void TMRA_9_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_9_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_9_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_9_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_10_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_10_Udf_IrqHandler(void);
__WEAKDEF void TMRA_10_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_10_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_10_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_10_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_11_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_11_Udf_IrqHandler(void);
__WEAKDEF void TMRA_11_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_11_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_11_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_11_Cmp4_IrqHandler(void);

__WEAKDEF void TMRA_12_Ovf_IrqHandler(void);
__WEAKDEF void TMRA_12_Udf_IrqHandler(void);
__WEAKDEF void TMRA_12_Cmp1_IrqHandler(void);
__WEAKDEF void TMRA_12_Cmp2_IrqHandler(void);
__WEAKDEF void TMRA_12_Cmp3_IrqHandler(void);
__WEAKDEF void TMRA_12_Cmp4_IrqHandler(void);

__WEAKDEF void EMB_GR0_IrqHandler(void);
__WEAKDEF void EMB_GR1_IrqHandler(void);
__WEAKDEF void EMB_GR2_IrqHandler(void);
__WEAKDEF void EMB_GR3_IrqHandler(void);
__WEAKDEF void EMB_GR4_IrqHandler(void);
__WEAKDEF void EMB_GR5_IrqHandler(void);
__WEAKDEF void EMB_GR6_IrqHandler(void);

__WEAKDEF void USART_1_RxErr_IrqHandler(void);
__WEAKDEF void USART_1_RxEnd_IrqHandler(void);
__WEAKDEF void USART_1_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_1_TxEnd_IrqHandler(void);
__WEAKDEF void USART_1_RxTO_IrqHandler(void);

__WEAKDEF void USART_2_RxErr_IrqHandler(void);
__WEAKDEF void USART_2_RxEnd_IrqHandler(void);
__WEAKDEF void USART_2_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_2_TxEnd_IrqHandler(void);
__WEAKDEF void USART_2_RxTO_IrqHandler(void);

__WEAKDEF void USART_3_RxErr_IrqHandler(void);
__WEAKDEF void USART_3_RxEnd_IrqHandler(void);
__WEAKDEF void USART_3_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_3_TxEnd_IrqHandler(void);

__WEAKDEF void USART_4_RxErr_IrqHandler(void);
__WEAKDEF void USART_4_RxEnd_IrqHandler(void);
__WEAKDEF void USART_4_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_4_TxEnd_IrqHandler(void);

__WEAKDEF void USART_5_LinBreakField_IrqHandler(void);
__WEAKDEF void USART_5_LinWakeup_IrqHandler(void);
__WEAKDEF void USART_5_RxErr_IrqHandler(void);
__WEAKDEF void USART_5_RxEnd_IrqHandler(void);
__WEAKDEF void USART_5_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_5_TxEnd_IrqHandler(void);

__WEAKDEF void USART_6_RxErr_IrqHandler(void);
__WEAKDEF void USART_6_RxEnd_IrqHandler(void);
__WEAKDEF void USART_6_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_6_TxEnd_IrqHandler(void);
__WEAKDEF void USART_6_RxTO_IrqHandler(void);

__WEAKDEF void USART_7_RxErr_IrqHandler(void);
__WEAKDEF void USART_7_RxEnd_IrqHandler(void);
__WEAKDEF void USART_7_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_7_TxEnd_IrqHandler(void);
__WEAKDEF void USART_7_RxTO_IrqHandler(void);

__WEAKDEF void USART_8_RxErr_IrqHandler(void);
__WEAKDEF void USART_8_RxEnd_IrqHandler(void);
__WEAKDEF void USART_8_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_8_TxEnd_IrqHandler(void);

__WEAKDEF void USART_9_RxErr_IrqHandler(void);
__WEAKDEF void USART_9_RxEnd_IrqHandler(void);
__WEAKDEF void USART_9_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_9_TxEnd_IrqHandler(void);

__WEAKDEF void USART_10_LinBreakField_IrqHandler(void);
__WEAKDEF void USART_10_LinWakeup_IrqHandler(void);
__WEAKDEF void USART_10_RxErr_IrqHandler(void);
__WEAKDEF void USART_10_RxEnd_IrqHandler(void);
__WEAKDEF void USART_10_TxEmpty_IrqHandler(void);
__WEAKDEF void USART_10_TxEnd_IrqHandler(void);

__WEAKDEF void SPI_1_RxEnd_IrqHandler(void);
__WEAKDEF void SPI_1_TxEmpty_IrqHandler(void);
__WEAKDEF void SPI_1_Err_IrqHandler(void);
__WEAKDEF void SPI_1_Idle_IrqHandler(void);

__WEAKDEF void SPI_2_RxEnd_IrqHandler(void);
__WEAKDEF void SPI_2_TxEmpty_IrqHandler(void);
__WEAKDEF void SPI_2_Err_IrqHandler(void);
__WEAKDEF void SPI_2_Idle_IrqHandler(void);

__WEAKDEF void SPI_3_RxEnd_IrqHandler(void);
__WEAKDEF void SPI_3_TxEmpty_IrqHandler(void);
__WEAKDEF void SPI_3_Err_IrqHandler(void);
__WEAKDEF void SPI_3_Idle_IrqHandler(void);

__WEAKDEF void SPI_4_RxEnd_IrqHandler(void);
__WEAKDEF void SPI_4_TxEmpty_IrqHandler(void);
__WEAKDEF void SPI_4_Err_IrqHandler(void);
__WEAKDEF void SPI_4_Idle_IrqHandler(void);

__WEAKDEF void SPI_5_RxEnd_IrqHandler(void);
__WEAKDEF void SPI_5_TxEmpty_IrqHandler(void);
__WEAKDEF void SPI_5_Err_IrqHandler(void);
__WEAKDEF void SPI_5_Idle_IrqHandler(void);

__WEAKDEF void SPI_6_RxEnd_IrqHandler(void);
__WEAKDEF void SPI_6_TxEmpty_IrqHandler(void);
__WEAKDEF void SPI_6_Err_IrqHandler(void);
__WEAKDEF void SPI_6_Idle_IrqHandler(void);

__WEAKDEF void CAN_1_IrqHandler(void);
__WEAKDEF void CAN_2_IrqHandler(void);

__WEAKDEF void I2S_1_Tx_IrqHandler(void);
__WEAKDEF void I2S_1_Rx_IrqHandler(void);
__WEAKDEF void I2S_1_Err_IrqHandler(void);

__WEAKDEF void I2S_2_Tx_IrqHandler(void);
__WEAKDEF void I2S_2_Rx_IrqHandler(void);
__WEAKDEF void I2S_2_Err_IrqHandler(void);

__WEAKDEF void I2S_3_Tx_IrqHandler(void);
__WEAKDEF void I2S_3_Rx_IrqHandler(void);
__WEAKDEF void I2S_3_Err_IrqHandler(void);

__WEAKDEF void I2S_4_Tx_IrqHandler(void);
__WEAKDEF void I2S_4_Rx_IrqHandler(void);
__WEAKDEF void I2S_4_Err_IrqHandler(void);

__WEAKDEF void USBFS_Global_IrqHandler(void);

__WEAKDEF void SDIOC_1_Normal_IrqHandler(void);
__WEAKDEF void SDIOC_1_Error_IrqHandler(void);

__WEAKDEF void SDIOC_2_Normal_IrqHandler(void);
__WEAKDEF void SDIOC_2_Error_IrqHandler(void);

__WEAKDEF void ETH_Global_IrqHandler(void);
__WEAKDEF void ETH_Wakeup_IrqHandler(void);

__WEAKDEF void I2C_1_RxEnd_IrqHandler(void);
__WEAKDEF void I2C_1_TxEnd_IrqHandler(void);
__WEAKDEF void I2C_1_TxEmpty_IrqHandler(void);
__WEAKDEF void I2C_1_Err_IrqHandler(void);

__WEAKDEF void I2C_2_RxEnd_IrqHandler(void);
__WEAKDEF void I2C_2_TxEnd_IrqHandler(void);
__WEAKDEF void I2C_2_TxEmpty_IrqHandler(void);
__WEAKDEF void I2C_2_Err_IrqHandler(void);

__WEAKDEF void I2C_3_RxEnd_IrqHandler(void);
__WEAKDEF void I2C_3_TxEnd_IrqHandler(void);
__WEAKDEF void I2C_3_TxEmpty_IrqHandler(void);
__WEAKDEF void I2C_3_Err_IrqHandler(void);

__WEAKDEF void I2C_4_RxEnd_IrqHandler(void);
__WEAKDEF void I2C_4_TxEnd_IrqHandler(void);
__WEAKDEF void I2C_4_TxEmpty_IrqHandler(void);
__WEAKDEF void I2C_4_Err_IrqHandler(void);

__WEAKDEF void I2C_5_RxEnd_IrqHandler(void);
__WEAKDEF void I2C_5_TxEnd_IrqHandler(void);
__WEAKDEF void I2C_5_TxEmpty_IrqHandler(void);
__WEAKDEF void I2C_5_Err_IrqHandler(void);

__WEAKDEF void I2C_6_RxEnd_IrqHandler(void);
__WEAKDEF void I2C_6_TxEnd_IrqHandler(void);
__WEAKDEF void I2C_6_TxEmpty_IrqHandler(void);
__WEAKDEF void I2C_6_Err_IrqHandler(void);

__WEAKDEF void PWC_Pvd1_IrqHandler(void);
__WEAKDEF void PWC_Pvd2_IrqHandler(void);

__WEAKDEF void FCM_Err_IrqHandler(void);
__WEAKDEF void FCM_End_IrqHandler(void);
__WEAKDEF void FCM_Ovf_IrqHandler(void);

__WEAKDEF void CTC_Udf_IrqHandler(void);
__WEAKDEF void CTC_Ovf_IrqHandler(void);

__WEAKDEF void ADC_1_SeqA_IrqHandler(void);
__WEAKDEF void ADC_1_SeqB_IrqHandler(void);
__WEAKDEF void ADC_1_Cmp0_IrqHandler(void);
__WEAKDEF void ADC_1_Cmp1Ind_IrqHandler(void);
__WEAKDEF void ADC_1_Cmp1Comb_IrqHandler(void);

__WEAKDEF void ADC_2_SeqA_IrqHandler(void);
__WEAKDEF void ADC_2_SeqB_IrqHandler(void);
__WEAKDEF void ADC_2_Cmp0_IrqHandler(void);
__WEAKDEF void ADC_2_Cmp1Ind_IrqHandler(void);
__WEAKDEF void ADC_2_Cmp1Comb_IrqHandler(void);

__WEAKDEF void ADC_3_SeqA_IrqHandler(void);
__WEAKDEF void ADC_3_SeqB_IrqHandler(void);
__WEAKDEF void ADC_3_Cmp0_IrqHandler(void);
__WEAKDEF void ADC_3_Cmp1Ind_IrqHandler(void);
__WEAKDEF void ADC_3_Cmp1Comb_IrqHandler(void);

__WEAKDEF void NFC_IrqHandler(void);

/**
 * @}
 */

#endif /* DDL_INTERRUPTS_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_INTERRUPTS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
