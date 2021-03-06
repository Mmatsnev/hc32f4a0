/**
 *******************************************************************************
 * @file  spi/spi_tx_rx_dma/source/main.c
 * @brief Main program SPI master base for the Device Driver Library.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup SPI_TX_RX_DMA
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Configuration for Example */

/* Define the communication mode of application, should be COM_MASTER or COM_SLAVE*/
#define APP_TEST_MODE           (COM_MASTER)

/* SPI wire mode, can be SPI_WIRE_4/SPI_WIRE_3 */
#define EXAMPLE_WIRE_MODE       (SPI_WIRE_4)

/* SPI mode, can be SPI_MODE_0/SPI_MODE_1/SPI_MODE_2/SPI_MODE_3 */
#define EXAMPLE_SPI_MODE        (SPI_MODE_1)

/* Clock delay function configurated for master if define SCK_DELAY_FUNC_ON */
#define SCK_DELAY_FUNC_ON       (1UL)

#if (EXAMPLE_WIRE_MODE == SPI_WIRE_3)
    #if ((EXAMPLE_SPI_MODE == SPI_MODE_0) || (EXAMPLE_SPI_MODE == SPI_MODE_2))
        #error "SPI_WIRE_3 mode can't support SPI_MODE_0 and SPI_MODE_2"
    #endif
#endif

#define COM_MASTER              (1UL)
#define COM_SLAVE               (0UL)

/* Test SPI unit */
#define SPI_UNIT                (M4_SPI6)
#define SPI_PWC_FCG_DEF         (PWC_FCG1_SPI6)
#define SPI_EVT_TX_IRQ          (EVT_SPI6_SPTI)
#define SPI_EVT_RX_IRQ          (EVT_SPI6_SPRI)

/* Define for port */
#define SPI_NSS_PORT            (GPIO_PORT_E)
#define SPI_NSS_PIN             (GPIO_PIN_06)
#define SPI_SCK_PORT            (GPIO_PORT_E)
#define SPI_SCK_PIN             (GPIO_PIN_03)
#define SPI_MOSI_PORT           (GPIO_PORT_E)
#define SPI_MOSI_PIN            (GPIO_PIN_04)
#define SPI_MISO_PORT           (GPIO_PORT_E)
#define SPI_MISO_PIN            (GPIO_PIN_05)

#define SPI_NSS_GPIO_FUNC       (GPIO_FUNC_49_SPI6_NSS0)
#define SPI_SCK_GPIO_FUNC       (GPIO_FUNC_46_SPI6_SCK)
#define SPI_MOSI_GPIO_FUNC      (GPIO_FUNC_47_SPI6_MOSI)
#define SPI_MISO_GPIO_FUNC      (GPIO_FUNC_48_SPI6_MISO)

/* DMA unit define */
#define DMA_UNIT                (M4_DMA2)
/* DMA channel for data transfer */
#define DMA_CH_TX               (DMA_CH0)
/* DMA channel for data receive */
#define DMA_CH_RX               (DMA_CH1)
/* Interrupt source indicate that commnication finished */
#define DMA_UNIT_RX_SRC         (INT_DMA2_TC1)
#define DMA_IRQn_RX             (Int000_IRQn)
#define DMA_RX_TC_FLAG          (DMA_TC_INT_CH1)

/* Define for test buffer length */
#define BUF_LENGTH              (128UL)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
#if (APP_TEST_MODE == COM_MASTER)
static void Master_Init(void);
#else
static void Slave_Init(void);
#endif
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static char u8MasterTxBuf[BUF_LENGTH] = "SPI master tx buffer data: abcdefg!";
static char u8SlaveTxBuf[BUF_LENGTH] = "SPI slave tx buffer data: 1234567!";

#if (APP_TEST_MODE == COM_MASTER)
static char u8MasterRxBuf[BUF_LENGTH];
#else
static char u8SlaveRxBuf[BUF_LENGTH];
#endif

static en_flag_status_t enRevFinished = Reset;
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
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    //EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static __attribute__((unused)) void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  Main function of spi_master_base project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_gpio_init_t stcGpioCfg;
#if (APP_TEST_MODE == COM_MASTER)
#ifdef SCK_DELAY_FUNC_ON
    stc_spi_delay_t stcSpiDelay;
#endif
#endif
    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    /* Clear receive buffer */
    for(uint32_t i = 0UL; i < BUF_LENGTH; i++)
    {
#if (APP_TEST_MODE == COM_MASTER)
        u8MasterRxBuf[i] = 0x00U;
#else
        u8SlaveRxBuf[i] = 0x00U;
#endif
    }

    /* Configure peripheral clock */
    PWC_Fcg1PeriphClockCmd(SPI_PWC_FCG_DEF, Enable);
    PWC_Fcg0PeriphClockCmd((PWC_FCG0_DMA2 | PWC_FCG0_AOS), Enable);

    /* Port configurate, High driving capacity for output pin. */
    GPIO_StructInit(&stcGpioCfg);

#if (APP_TEST_MODE == COM_MASTER)
    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_Init(SPI_NSS_PORT,  SPI_NSS_PIN, &stcGpioCfg);
#endif
    GPIO_Init(SPI_SCK_PORT,  SPI_SCK_PIN, &stcGpioCfg);
    GPIO_Init(SPI_MOSI_PORT, SPI_MOSI_PIN, &stcGpioCfg);

    /* CMOS input for input pin */
    stcGpioCfg.u16PinIType = PIN_ITYPE_CMOS;
    GPIO_Init(SPI_MISO_PORT, SPI_MISO_PIN, &stcGpioCfg);
#else

    stcGpioCfg.u16PinIType = PIN_ITYPE_CMOS;
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_Init(SPI_NSS_PORT,  SPI_NSS_PIN, &stcGpioCfg);
#endif
    GPIO_Init(SPI_SCK_PORT,  SPI_SCK_PIN, &stcGpioCfg);
    GPIO_Init(SPI_MOSI_PORT, SPI_MOSI_PIN, &stcGpioCfg);

    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
    GPIO_Init(SPI_MISO_PORT, SPI_MISO_PIN, &stcGpioCfg);

#endif
    /* Configure SPI Port function for master */
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_SetFunc(SPI_NSS_PORT,  SPI_NSS_PIN,  SPI_NSS_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
#endif
    GPIO_SetFunc(SPI_SCK_PORT,  SPI_SCK_PIN,  SPI_SCK_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_GPIO_FUNC, PIN_SUBFUNC_DISABLE);

    SPI_DeInit(SPI_UNIT);
    /* Configuration SPI function for master*/
#if (APP_TEST_MODE == COM_MASTER)
    Master_Init();
#ifdef SCK_DELAY_FUNC_ON
    /* SPI Singnal delay configurate, for wait slave */
    SPI_DelayStructInit(&stcSpiDelay);
    stcSpiDelay.u32IntervalDelay = SPI_INTERVAL_TIME_8SCK_2PCLK1;
    stcSpiDelay.u32ReleaseDelay = SPI_RELEASE_TIME_8SCK;
    stcSpiDelay.u32SetupDelay = SPI_SETUP_TIME_8SCK;
    SPI_DelayTimeCfg(SPI_UNIT, &stcSpiDelay);
#endif
#else
    /* Configuration SPI function for slave*/
    Slave_Init();
#endif

#if (APP_TEST_MODE == COM_MASTER)
    /* Wait for key */
    while(Pin_Set == GPIO_ReadInputPins(GPIO_PORT_A, GPIO_PIN_00));
#endif
    /* Command SPI function */
    SPI_FunctionCmd(SPI_UNIT, Enable);

    /* Wait transfer finish */
    while(Reset == enRevFinished)
    {
        ;
    }

    SPI_DeInit(SPI_UNIT);

#if (APP_TEST_MODE == COM_MASTER)
    /* Compare u8TxBuffer and u8RxBuffer */
    if(0U == memcmp(u8SlaveTxBuf, u8MasterRxBuf, BUF_LENGTH))
#else
    if(0U == memcmp(u8MasterTxBuf, u8SlaveRxBuf, BUF_LENGTH))
#endif
    {
        /* Communication success */
        while(1)
        {
            BSP_LED_Toggle(LED_BLUE);
            DDL_DelayMS(500UL);
        }
    }
    else
    {
        /* Communication data error */
        while(1)
        {
            BSP_LED_Toggle(LED_RED);
            DDL_DelayMS(500UL);
        }
    }
}

/**
 * @brief  DMA transfer end interrupt
 * @param  None
 * @retval None
 */
static void DmaRxEnd_IrqCallback(void)
{
    enRevFinished = Set;
}

#if (APP_TEST_MODE == COM_MASTER)
/**
 * @brief  Initialization for master
 * @param  None
 * @retval None
 */
static void Master_Init(void)
{
    stc_spi_init_t stcSpiInit;
    stc_dma_init_t stcDmaInit;
    stc_irq_signin_config_t stcIrqSignConfig;

    /* SPI configurate for master */
    SPI_StructInit(&stcSpiInit);   /* Clear initialize structure */
    stcSpiInit.u32WireMode          = EXAMPLE_WIRE_MODE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32Modfe             = SPI_MODFE_DISABLE;
    stcSpiInit.u32Parity            = SPI_PARITY_INVALID;
    stcSpiInit.u32SpiMode           = EXAMPLE_SPI_MODE;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_PCLK1_DIV16;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    SPI_Init(SPI_UNIT, &stcSpiInit);

    /* DMA configurate for master tx*/
    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH_TX, SPI_EVT_TX_IRQ);
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCnt  = BUF_LENGTH;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32DestAddr  = (uint32_t)(&SPI_UNIT->DR);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&u8MasterTxBuf[0]);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_FIX;

    if(Ok != DMA_Init(DMA_UNIT, DMA_CH_TX, &stcDmaInit))
    {
        while (1)
        {}
    }

    /* DMA configurate for master rx*/
    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH_RX, SPI_EVT_RX_IRQ);
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCnt  = BUF_LENGTH;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32DestAddr  = (uint32_t)(&u8MasterRxBuf[0]);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&SPI_UNIT->DR);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;

    if(Ok != DMA_Init(DMA_UNIT, DMA_CH_RX, &stcDmaInit))
    {
        while (1)
        {}
    }

    /* DMA interrupt configurate */
    stcIrqSignConfig.enIntSrc   = DMA_UNIT_RX_SRC;
    stcIrqSignConfig.enIRQn     = DMA_IRQn_RX;
    stcIrqSignConfig.pfnCallback= &DmaRxEnd_IrqCallback;

    INTC_IrqSignIn(&stcIrqSignConfig);
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_RX_TC_FLAG);

    /* NVIC setting */
    NVIC_ClearPendingIRQ(DMA_IRQn_RX);
    NVIC_SetPriority(DMA_IRQn_RX,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(DMA_IRQn_RX);

    /* DMA module enable */
    DMA_Cmd(DMA_UNIT, Enable);

    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH_TX, Enable);
    DMA_ChannelCmd(DMA_UNIT, DMA_CH_RX, Enable);
}

#else

/**
 * @brief  Initialization for slave
 * @param  None
 * @retval None
 */
static void Slave_Init(void)
{
    stc_spi_init_t stcSpiInit;
    stc_dma_init_t stcDmaInit;
    stc_irq_signin_config_t stcIrqSignConfig;

    SPI_StructInit(&stcSpiInit);   /* Clear initialize structure */
    stcSpiInit.u32WireMode          = EXAMPLE_WIRE_MODE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_SLAVE;
    stcSpiInit.u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32Modfe             = SPI_MODFE_DISABLE;
    stcSpiInit.u32Parity            = SPI_PARITY_INVALID;
    stcSpiInit.u32SpiMode           = EXAMPLE_SPI_MODE;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_PCLK1_DIV2;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    SPI_Init(SPI_UNIT, &stcSpiInit);

    /* DMA configurate for slave tx*/
    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH_TX, SPI_EVT_TX_IRQ);
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCnt  = BUF_LENGTH;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32DestAddr  = (uint32_t)(&SPI_UNIT->DR);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&u8SlaveTxBuf[0]);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_FIX;

    if(Ok != DMA_Init(DMA_UNIT, DMA_CH_TX, &stcDmaInit))
    {
        while (1)
        {}
    }

    /* DMA configurate for slave rx*/
    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH_RX, SPI_EVT_RX_IRQ);
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn     = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCnt  = BUF_LENGTH;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32DestAddr  = (uint32_t)(&u8SlaveRxBuf[0]);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&SPI_UNIT->DR);
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_INC;

    if(Ok != DMA_Init(DMA_UNIT, DMA_CH_RX, &stcDmaInit))
    {
        while (1)
        {}
    }

    /* DMA interrupt configurate */
    stcIrqSignConfig.enIntSrc   = DMA_UNIT_RX_SRC;
    stcIrqSignConfig.enIRQn     = DMA_IRQn_RX;
    stcIrqSignConfig.pfnCallback= &DmaRxEnd_IrqCallback;

    INTC_IrqSignIn(&stcIrqSignConfig);
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_RX_TC_FLAG);

    /* NVIC setting */
    NVIC_ClearPendingIRQ(DMA_IRQn_RX);
    NVIC_SetPriority(DMA_IRQn_RX,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(DMA_IRQn_RX);

    /* DMA module enable */
    DMA_Cmd(DMA_UNIT, Enable);

    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH_TX, Enable);
    DMA_ChannelCmd(DMA_UNIT, DMA_CH_RX, Enable);
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
