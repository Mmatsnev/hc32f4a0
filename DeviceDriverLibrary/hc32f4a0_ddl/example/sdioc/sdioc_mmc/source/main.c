/**
 *******************************************************************************
 * @file  sdioc/sdioc_mmc/source/main.c
 * @brief Main program of SDIOC MMC Card for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Yangjp          First version
   2020-07-15       Zhangxl         Use XTAL 8MHz as PLL source
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
#include "mmc.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup SDIOC_Mmc_Card
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* MMC transfer mode */
#define MMC_TRANS_MODE_POLLING                  (0U)
#define MMC_TRANS_MODE_INT                      (1U)
#define MMC_TRANS_MODE_DMA                      (2U)

/* Populate the following macro with an value, reference "MMC transfer mode" */
#define MMC_TRANS_MODE                          (MMC_TRANS_MODE_POLLING)

#define SDIOC_DMA_UNIT                          (M4_DMA1)
#define SDIOC_DMA_CLK                           (PWC_FCG0_DMA1)
#define SDIOC_DMA_TX_CH                         (DMA_CH0)
#define SDIOC_DMA_RX_CH                         (DMA_CH1)
#define SDIOC_DMA_TX_TRIG_SRC                   (EVT_SDIOC1_DMAW)
#define SDIOC_DMA_RX_TRIG_SRC                   (EVT_SDIOC1_DMAR)

#define SDIOC_MMC_UINT                          (M4_SDIOC1)
#define SDIOC_MMC_CLK                           (PWC_FCG1_SDIOC1)
#define SIDOC_MMC_INT_SRC                       (INT_SDIOC1_SD)
#define SIDOC_MMC_IRQ                           (Int104_IRQn)

/* CK = PC12 */
#define SDIOC_CK_PORT                           (GPIO_PORT_C)
#define SDIOC_CK_PIN                            (GPIO_PIN_12)
/* CMD = PD02 */
#define SDIOC_CMD_PORT                          (GPIO_PORT_D)
#define SDIOC_CMD_PIN                           (GPIO_PIN_02)
/* D0 = PB07 */
#define SDIOC_D0_PORT                           (GPIO_PORT_B)
#define SDIOC_D0_PIN                            (GPIO_PIN_07)
/* D1 = PA08 */
#define SDIOC_D1_PORT                           (GPIO_PORT_A)
#define SDIOC_D1_PIN                            (GPIO_PIN_08)
/* D2 = PC10 */
#define SDIOC_D2_PORT                           (GPIO_PORT_C)
#define SDIOC_D2_PIN                            (GPIO_PIN_10)
/* D3 = PB05 */
#define SDIOC_D3_PORT                           (GPIO_PORT_B)
#define SDIOC_D3_PIN                            (GPIO_PIN_05)

/* MMC card read/write/erase */
#define MMC_CARD_BLK_SIZE                       (512U)
#define MMC_CARD_BLK_NUMBER                     (10U)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
stc_mmc_handle_t MmcHandle;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__ALIGN_BEGIN uint8_t u8WriteBlocks[MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER];
__ALIGN_BEGIN uint8_t u8ReadBlocks[MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER];

#if MMC_TRANS_MODE != MMC_TRANS_MODE_POLLING
__IO uint8_t u8TxCpltFlag = 0U, u8RxCpltFlag = 0U, u8TxRxErrFlag = 0U;
#endif

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
#if MMC_TRANS_MODE != MMC_TRANS_MODE_POLLING
/**
 * @brief  SDIOC transfer complete interrupt callback function.
 * @param  None
 * @retval None
 */
static void SDIOC_TransferComplete_IrqHandler(void)
{
    MMC_IRQHandler(&MmcHandle);
}

/**
 * @brief  SDIOC tx complete callback function.
 * @param  None
 * @retval None
 */
void MMC_TxCpltCallback(stc_mmc_handle_t *handle)
{
    u8TxCpltFlag = 1U;
}

/**
 * @brief  SDIOC rx complete callback function.
 * @param  None
 * @retval None
 */
void MMC_RxCpltCallback(stc_mmc_handle_t *handle)
{
    u8RxCpltFlag = 1U;
}

/**
 * @brief  SDIOC error callback function.
 * @param  None
 * @retval None
 */
void MMC_ErrorCallback(stc_mmc_handle_t *handle)
{
    u8TxRxErrFlag = 1U;
}
#endif

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
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    // SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    // EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    // EFM_OTP_WP_Unlock();
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
    // GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    // PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

#if MMC_TRANS_MODE == MMC_TRANS_MODE_DMA
/**
 * @brief  Initializes the MMC DMA.
 * @param  None
 * @retval None
 */
static void MMC_DMAInit(void)
{
    stc_dma_init_t stcDmaInit;

    /* Enable DMA and PTDIS(AOS) clock. */
    PWC_Fcg0PeriphClockCmd((SDIOC_DMA_CLK | PWC_FCG0_AOS), Enable);

    DMA_StructInit(&stcDmaInit);
    /* Configure MMC DMA Transfer */
    stcDmaInit.u32IntEn     = DMA_INT_DISABLE;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_32BIT;
    /* Set source & destination address mode. */
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_FIX;
    if (Ok != DMA_Init(SDIOC_DMA_UNIT, SDIOC_DMA_TX_CH, &stcDmaInit))
    {
        while (1)
        {}
    }

    DMA_StructInit(&stcDmaInit);
    /* Configure MMC DMA Receive */
    stcDmaInit.u32IntEn     = DMA_INT_DISABLE;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_32BIT;
    /* Set source & destination address mode. */
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;
    if (Ok != DMA_Init(SDIOC_DMA_UNIT, SDIOC_DMA_RX_CH, &stcDmaInit))
    {
        while (1)
        {}
    }

    DMA_SetTriggerSrc(SDIOC_DMA_UNIT, SDIOC_DMA_TX_CH, SDIOC_DMA_TX_TRIG_SRC);
    DMA_SetTriggerSrc(SDIOC_DMA_UNIT, SDIOC_DMA_RX_CH, SDIOC_DMA_RX_TRIG_SRC);
    /* Enable DMA. */
    DMA_Cmd(SDIOC_DMA_UNIT, Enable);
}
#endif

/**
 * @brief  Initializes the MMC GPIO.
 * @param  None
 * @retval None
 */
static void MMC_GpioInit(void)
{
    /* SDIO1_CD */
    BSP_IO_ConfigPortPin(EIO_PORT0, EIO_SDIC1_CD, EIO_DIR_IN);

    /* SDIOC pins configuration */
    GPIO_SetFunc(SDIOC_CK_PORT,  SDIOC_CK_PIN,  GPIO_FUNC_9_SDIO1_CK,   PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SDIOC_CMD_PORT, SDIOC_CMD_PIN, GPIO_FUNC_9_SDIO1_CMD,  PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SDIOC_D0_PORT,  SDIOC_D0_PIN,  GPIO_FUNC_9_SDIO1_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SDIOC_D1_PORT,  SDIOC_D1_PIN,  GPIO_FUNC_9_SDIO1_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SDIOC_D2_PORT,  SDIOC_D2_PIN,  GPIO_FUNC_9_SDIO1_DATA, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SDIOC_D3_PORT,  SDIOC_D3_PIN,  GPIO_FUNC_9_SDIO1_DATA, PIN_SUBFUNC_DISABLE);
}

/**
 * @brief  Get MMC card insert status.
 * @param  None
 * @retval An en_flag_status_t enumeration value:
 *           - Set: MMC card inserted
 *           - Reset: No MMC card insert
 */
static en_flag_status_t MMCCard_GetInsertStatus(void)
{
    en_flag_status_t enFlagSta = Set;

    /* Check MMC card detect pin */
    if (0U != BSP_IO_ReadPortPin(EIO_PORT0, EIO_SDIC1_CD))
    {
        enFlagSta = Reset;
    }

    return enFlagSta;
}

/**
 * @brief  MMC card configuration.
 * @param  None
 * @retval None
 */
static void MMCCard_Config(void)
{
#if MMC_TRANS_MODE != MMC_TRANS_MODE_POLLING
    stc_irq_signin_config_t stcIrqSignConfig;

    stcIrqSignConfig.enIntSrc    = SIDOC_MMC_INT_SRC;
    stcIrqSignConfig.enIRQn      = SIDOC_MMC_IRQ;
    stcIrqSignConfig.pfnCallback = &SDIOC_TransferComplete_IrqHandler;
    INTC_IrqSignOut(stcIrqSignConfig.enIRQn);
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC setting */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
#endif

    /* Init MMC GPIO */
    MMC_GpioInit();
    /* Enable SDIOC clock */
    PWC_Fcg1PeriphClockCmd(SDIOC_MMC_CLK, Enable);

    /* Configure structure initialization */
    MmcHandle.SDIOCx                          = SDIOC_MMC_UINT;
    MmcHandle.stcSdiocInit.u32Mode            = SDIOC_MODE_MMC;
    MmcHandle.stcSdiocInit.u8CardDetectSelect = SDIOC_CARD_DETECT_CD_PIN_LEVEL;
    MmcHandle.stcSdiocInit.u8SpeedMode        = SDIOC_SPEED_MODE_HIGH;
    MmcHandle.stcSdiocInit.u8BusWidth         = SDIOC_BUS_WIDTH_4BIT;
    MmcHandle.stcSdiocInit.u16ClockDiv        = SDIOC_CLOCK_DIV_2;
#if MMC_TRANS_MODE == MMC_TRANS_MODE_DMA
    MMC_DMAInit();
    MmcHandle.DMAx      = SDIOC_DMA_UNIT;
    MmcHandle.u8DmaTxCh = SDIOC_DMA_TX_CH;
    MmcHandle.u8DmaRxCh = SDIOC_DMA_RX_CH;
#else
    MmcHandle.DMAx    = NULL;
#endif

    /* Reset SDIOC */
    if (Ok != SDIOC_SoftwareReset(MmcHandle.SDIOCx, SDIOC_SW_RESET_ALL))
    {
        printf("Reset SDIOC failed!\r\n");
    }
    else
    {
        if (Set != MMCCard_GetInsertStatus())
        {
            printf("No MMC card insertion!\r\n");
        }

        if (Ok != MMC_Init(&MmcHandle))
        {
            printf("MMC card initialize failed!\r\n");
        }
    }
}

/**
 * @brief  MMC card erase.
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Ok: MMC card erase success
 *           - Error: MMC card erase error
 */
static en_result_t MMCCard_Erase(void)
{
    __IO uint32_t i;
    en_result_t enRet = Ok;

    /* Initialize read/write blocks */
    memset(u8ReadBlocks, 0x20, (MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER));
    /* Erase MMC card */
    if (Ok != MMC_Erase(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER))
    {
        enRet = Error;
    }

    /* Read MMC card */
#if MMC_TRANS_MODE == MMC_TRANS_MODE_POLLING
    if (Ok != MMC_ReadBlocks(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8ReadBlocks, 2000UL))
    {
        enRet = Error;
    }
#elif MMC_TRANS_MODE == MMC_TRANS_MODE_INT
    u8RxCpltFlag = 0U;
    if (Ok != MMC_ReadBlocks_INT(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8ReadBlocks))
    {
        enRet = Error;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCpltFlag) && (0U == u8TxRxErrFlag))
    {
    }
#else
    u8RxCpltFlag = 0U;
    if (Ok != MMC_ReadBlocks_DMA(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8ReadBlocks))
    {
        enRet = Error;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCpltFlag) && (0U == u8TxRxErrFlag))
    {
    }
#endif

    /* Check whether data value is OxFF or 0x00 after erase MMC card */
    for (i = 0UL; i < (MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER); i++)
    {
        if (0x00U != u8ReadBlocks[i])
        {
            if (0xFFU != u8ReadBlocks[i])
            {
                enRet = Error;
                break;
            }
        }
    }

#if MMC_TRANS_MODE != MMC_TRANS_MODE_POLLING
    if (0U != u8TxRxErrFlag)
    {
        enRet = Error;
    }
#endif

    if (Ok != enRet)
    {
        printf("MMC card erase failed!\r\n");
    }

    return enRet;
}

/**
 * @brief  MMC card multi-block read/write.
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Ok: MMC card multi-block read/write success
 *           - Error: MMC card multi-block read/write error
 */
static en_result_t MMCCard_RWMultiBlock(void)
{
    en_result_t enRet = Ok;

    /* Initialize read/write blocks */
    memset(u8WriteBlocks, 0x20, (MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER));
    memset(u8ReadBlocks,  0, (MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER));

    /* Write MMC card */
#if MMC_TRANS_MODE == MMC_TRANS_MODE_POLLING
    if (Ok != MMC_WriteBlocks(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8WriteBlocks, 2000U))
    {
        enRet = Error;
    }
#elif MMC_TRANS_MODE == MMC_TRANS_MODE_INT
    u8TxCpltFlag = 0U;
    if (Ok != MMC_WriteBlocks_INT(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8WriteBlocks))
    {
        enRet = Error;
    }
    /* Wait for transfer completed */
    while ((0U == u8TxCpltFlag) && (0U == u8TxRxErrFlag))
    {
    }
#else
    u8TxCpltFlag = 0U;
    if (Ok != MMC_WriteBlocks_DMA(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8WriteBlocks))
    {
        enRet = Error;
    }
    /* Wait for transfer completed */
    while ((0U == u8TxCpltFlag) && (0U == u8TxRxErrFlag))
    {
    }
#endif

    /* Read MMC card */
#if MMC_TRANS_MODE == MMC_TRANS_MODE_POLLING
    if (Ok != MMC_ReadBlocks(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8ReadBlocks, 2000UL))
    {
        enRet = Error;
    }
#elif MMC_TRANS_MODE == MMC_TRANS_MODE_INT
    u8RxCpltFlag = 0U;
    if (Ok != MMC_ReadBlocks_INT(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8ReadBlocks))
    {
        enRet = Error;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCpltFlag) && (0U == u8TxRxErrFlag))
    {
    }
#else
    u8RxCpltFlag = 0U;
    if (Ok != MMC_ReadBlocks_DMA(&MmcHandle, 0UL, MMC_CARD_BLK_NUMBER, (uint8_t *)u8ReadBlocks))
    {
        enRet = Error;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCpltFlag) && (0U == u8TxRxErrFlag))
    {
    }
#endif

    /* Check data value */
    if (0 != memcmp(u8WriteBlocks, u8ReadBlocks, (MMC_CARD_BLK_SIZE * MMC_CARD_BLK_NUMBER)))
    {
        enRet = Error;
    }

#if MMC_TRANS_MODE != MMC_TRANS_MODE_POLLING
    if (0U != u8TxRxErrFlag)
    {
        enRet = Error;
    }
#endif

    if (Ok != enRet)
    {
        printf("MMC card multi-block read/write failed!\r\n");
    }

    return enRet;
}

/**
 * @brief  SYS clock initialize.
 * @param  None
 * @retval None
 */
void SYS_CLK_Init(void)
{
    stc_clk_pllh_init_t stcPLLHInit;

    /* PCLK0, HCLK Max 200MHz */
    CLK_ClkDiv(CLK_CATE_ALL, (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 | \
                              CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 | \
                              CLK_HCLK_DIV1));

    CLK_PLLHStrucInit(&stcPLLHInit);
    /* VCO = (8/1)*100 = 800MHz*/
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = 1UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLN = 100UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLP = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLQ = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLR = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL;
    CLK_PLLHInit(&stcPLLHInit);

    /* Highspeed SRAM set to 1 Read/Write wait cycle */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);

    /* SRAM1_2_3_4_backup set to 2 Read/Write wait cycle */
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    /* 0-wait @ 40MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);   

    /* 4 cycles for 200 ~ 250MHz */
    GPIO_SetReadWaitCycle(GPIO_READ_WAIT_4);

    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
}

/**
 * @brief  Main function of SDIOC MMC card.
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    en_result_t enEraseRet = Ok, enMulBlkRet = Ok;

    /* Peripheral registers write unprotected */
    Peripheral_WE();
    /* Configure clock */
    SYS_CLK_Init();
    /* Configure BSP */
    BSP_IO_Init();
    BSP_LED_Init();
    /* Configure UART */
    DDL_PrintfInit();
    /* Configure MMC Card */
    MMCCard_Config();

    /* Erase/single-block/multi-block test */
    enEraseRet  = MMCCard_Erase();
    enMulBlkRet = MMCCard_RWMultiBlock();
    if ((Ok != enEraseRet) || (Ok != enMulBlkRet))
    {
        /* Test failed */
        BSP_LED_On(LED_RED);
        BSP_LED_Off(LED_BLUE);
    }
    else
    {
        /* Test success */
        BSP_LED_On(LED_BLUE);
        BSP_LED_Off(LED_RED);
    }
    /* Peripheral registers write protected */
    Peripheral_WP();

    while (1)
    {
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
