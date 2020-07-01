 /*******************************************************************************
 * @file  dmac/dmac_link_list_pointer/source/main.c
 * @brief This example demonstrates DMA linked list pointer function.
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
 * @addtogroup DMAC_LLP
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* DMAC */
#define DMA_UNIT        (M4_DMA1)
#define DMA_CH          (DMA_CH5)
#define DMA_MX_CH       (DMA_MX_CH5)
#define DMA_TC          (1UL)
#define DMA_BC          (10UL)
#define DMA_DW          (DMA_DATAWIDTH_32BIT)
#define DMA_INT_SRC     (INT_DMA1_TC5)
#define DMA_IRQn        (Int000_IRQn)

#define DMA_LLP0_TC     (5UL << DMA_DTCTL_CNT_POS)
#define DMA_LLP0_BC     (1UL << DMA_DTCTL_BLKSIZE_POS)
#define DMA_LLP0_DW     (DMA_DATAWIDTH_16BIT)

#define DMA_LLP1_TC     (2UL << DMA_DTCTL_CNT_POS)
#define DMA_LLP1_BC     (5UL << DMA_DTCTL_BLKSIZE_POS)
#define DMA_LLP1_DW     (DMA_DATAWIDTH_8BIT)

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
static const uint32_t u32SrcBuf0[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static uint32_t u32DestBuf0[10] = {0};

static const uint16_t u16SrcBuf1[5] = {21, 22, 23, 24, 25};
static uint16_t u16DestBuf1[5] = {0};

static const uint8_t u8SrcBuf2[10] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
static uint8_t u8DestBuf2[10] = {0};

static stc_dma_llp_descriptor_t stcLlpDesc[2] = {0};

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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
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
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
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
 * @brief  DMA basic and llp function init
 * @param  None
 * @retval en_result_t
 */
en_result_t DmaInit(void)
{
    stc_dma_init_t stcDmaInit;
    stc_dma_llp_init_t stcDmaLlpInit;

    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH, EVT_AOS_STRG);

    DMA_StructInit(&stcDmaInit);

    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = DMA_BC;
    stcDmaInit.u32TransCnt  = DMA_TC;
    stcDmaInit.u32DataWidth = DMA_DW;
    stcDmaInit.u32DestAddr  = (uint32_t)(&u32DestBuf0[0]);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&u32SrcBuf0[0]);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;

    if (Ok != DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit))
    {
        while (1)
        {}
    }

    stcLlpDesc[0].SARx  = (uint32_t)&u16SrcBuf1[0];
    stcLlpDesc[0].DARx  = (uint32_t)&u16DestBuf1[0];
    stcLlpDesc[0].DTCTLx= DMA_LLP0_TC | DMA_LLP0_BC;
    stcLlpDesc[0].LLPx  = (uint32_t)&stcLlpDesc[1];
    stcLlpDesc[0].CHCTLx= DMA_SRC_ADDR_INC | DMA_DEST_ADDR_INC | DMA_LLP0_DW |  \
                          DMA_LLP_ENABLE   | DMA_INT_DISABLE   | DMA_LLP_RUN;

    stcLlpDesc[1].SARx  = (uint32_t)&u8SrcBuf2[0];
    stcLlpDesc[1].DARx  = (uint32_t)&u8DestBuf2[0];
    stcLlpDesc[1].DTCTLx= DMA_LLP1_TC | DMA_LLP1_BC;
    stcLlpDesc[1].CHCTLx= DMA_SRC_ADDR_INC | DMA_DEST_ADDR_INC | DMA_LLP1_DW |  \
                          DMA_LLP_DISABLE  | DMA_INT_ENABLE;

    DMA_LlpStructInit(&stcDmaLlpInit);

    stcDmaLlpInit.u32LlpEn  = DMA_LLP_ENABLE;
    stcDmaLlpInit.u32LlpRun = DMA_LLP_RUN;
    stcDmaLlpInit.u32LlpAddr= (uint32_t)&stcLlpDesc[0];

    return (DMA_LlpInit(DMA_UNIT, DMA_CH, &stcDmaLlpInit));
}

/**
 * @brief  DMA ch.5 transfer complete IRQ callback
 * @param  None
 * @retval None
 */
void DMA1_CH5_TransEnd_IrqCallback(void)
{
    u8DmaTcEnd = Set;
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_TC_INT_CH5);
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
    stcIrqSignConfig.pfnCallback= &DMA1_CH5_TransEnd_IrqCallback;

    INTC_IrqSignIn(&stcIrqSignConfig);

    DMA_ClearTransIntStatus(DMA_UNIT, DMA_TC_INT_CH5);

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
    uint8_t u8Ret1, u8Ret2, u8Ret3;

    /* Register write enable for some required peripherals. */
    Peripheral_WE();
    /* System clock init */
    BSP_CLK_Init();
    /* Expand IO init */
    BSP_IO_Init();
    /* LED init */
    BSP_LED_Init();

    /* DMA/AOS FCG enable */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_DMA1 | PWC_FCG0_AOS, Enable);
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

    u8Ret1 = memcmp(u32DestBuf0, u32SrcBuf0, sizeof(u32SrcBuf0));

    u8Ret2 = memcmp(u16DestBuf1, u16SrcBuf1, sizeof(u16SrcBuf1));

    u8Ret3 = memcmp(u8DestBuf2, u8SrcBuf2, sizeof(u8SrcBuf2));

    if (0U == (u8Ret1 | u8Ret2 | u8Ret3))
    {
        /* LED blue, as expected */
        BSP_LED_On(LED_BLUE);
    }
    else
    {
        /* LED red */
        BSP_LED_On(LED_RED);
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
