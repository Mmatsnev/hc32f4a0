 /*******************************************************************************
 * @file  dmac/dmac_base/source/main.c
 * @brief This example demonstrates DMA basic transfer function.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup DMAC_Base
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DMAC */
#define DMA_UNIT        (M4_DMA2)
#define DMA_CH          (DMA_CH4)
#define DMA_MX_CH       (DMA_MX_CH0)
#define DMA_TC          (4UL)
#define DMA_BC          (5UL)
#define DMA_DW          (DMA_DATAWIDTH_32BIT)
#define DMA_INT_SRC     (INT_DMA2_TC4)
#define DMA_IRQn        (Int000_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
en_flag_status_t u8DmaTcEnd = Reset;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const uint32_t u32SrcBuf[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                       11, 12, 13, 14, 15, 16, 17, 18,
                                       19, 20};
static uint32_t u32DestBuf[20] = {0};
static uint32_t u32ExpectDestBufData[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                          13, 14, 15, 16, 17, 18, 19, 20};

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  MCU Peripheral registers write unprotected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WE(void)
{
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
//    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
//    EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
//    EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
//    SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
//    EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
//    EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  DMA basic function init
 * @param  None
 * @retval en_result_t
 */
en_result_t DmaInit(void)
{
    stc_dma_init_t stcDmaInit;

    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH, EVT_AOS_STRG);

    DMA_StructInit(&stcDmaInit);

    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = DMA_BC;
    stcDmaInit.u32TransCnt  = DMA_TC;
    stcDmaInit.u32DataWidth = DMA_DW;
    stcDmaInit.u32DestAddr  = (uint32_t)(&u32DestBuf[0]);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&u32SrcBuf[0]);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;

    return DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit);
}

/**
 * @brief  DMA ch.4 transfer complete IRQ callback
 * @param  None
 * @retval None
 */
void DMA2_CH4_TransEnd_IrqCallback(void)
{
    u8DmaTcEnd = Set;
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_TC_INT_CH4);
}

/**
 * @brief  DMA basic function interrupt init
 * @param  None
 * @retval None
 */
void DmaIntInit(void)
{
    stc_irq_signin_config_t stcIrqSignConfig;

    stcIrqSignConfig.enIntSrc   = DMA_INT_SRC;
    stcIrqSignConfig.enIRQn     = DMA_IRQn;
    stcIrqSignConfig.pfnCallback= &DMA2_CH4_TransEnd_IrqCallback;

    INTC_IrqSignIn(&stcIrqSignConfig);
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_TC_INT_CH4);

    /* NVIC setting */
    NVIC_ClearPendingIRQ(DMA_IRQn);
    NVIC_SetPriority(DMA_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(DMA_IRQn);
}

/**
 * @brief  Main function of DMAC project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Register write enable for some required peripherals. */
    Peripheral_WE();
    /* System clock init */
    BSP_CLK_Init();
    /* Expand IO init */
    BSP_IO_Init();
    /* LED init */
    BSP_LED_Init();
    /* DMA/AOS FCG enable */
    PWC_Fcg0PeriphClockCmd((PWC_FCG0_DMA2 | PWC_FCG0_AOS), Enable);
    /* Register write protected for some required peripherals. */
    Peripheral_WP();
    /* Config DMA */
    if (Ok != DmaInit()) {
        while (1)
        {}
    }

    /* DMA interrupt config */
    DmaIntInit();

    /* DMA module enable */
    DMA_Cmd(DMA_UNIT, Enable);

    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH, Enable);
    //DMA_MxChannelCmd(DMA_UNIT, DMA_MX_CH, Enable);

    /* 1st trigger for DMA */
    AOS_SW_Trigger();

    while (Reset == u8DmaTcEnd)
    {
        AOS_SW_Trigger();
    }
    if (memcmp(u32DestBuf, u32ExpectDestBufData, sizeof(u32DestBuf)))
    {
        /* LED red */
        BSP_LED_On(LED_RED);
    }
    else
    {
        /* LED blue, as expected */
        BSP_LED_On(LED_BLUE);
    }

    while(1)
    {
        ;
    }
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
