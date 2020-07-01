/**
 *******************************************************************************
 * @file  spi/spi_tx_rx_polling/source/main.c
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
 * @addtogroup SPI_TX_RX_POLLING
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Configuration for Example */
/* SPI wire mode, can be SPI_WIRE_4/SPI_WIRE_3 */
#define EXAMPLE_WIRE_MODE       (SPI_WIRE_4)
/* SPI mode, can be SPI_MODE_0/SPI_MODE_1/SPI_MODE_2/SPI_MODE_3 */
#define EXAMPLE_SPI_MODE        (SPI_MODE_0)
/* Baudrate = PCLK1/DIVx */
#define EXAMPLE_BR_DIV          (SPI_BR_PCLK1_DIV256)


#if (EXAMPLE_WIRE_MODE == SPI_WIRE_3)
    #if ((EXAMPLE_SPI_MODE == SPI_MODE_0) || (EXAMPLE_SPI_MODE == SPI_MODE_2))
        #error "SPI_WIRE_3 mode can't support SPI_MODE_0 and SPI_MODE_2"
    #endif
#endif

/* Test SPI unit */
#define SPI_MASTER_UNIT         (M4_SPI1)
#define SPI_SLAVE_UNIT          (M4_SPI2)

/* SPI port definition for master */
#define SPI_M_NSS_PORT          (GPIO_PORT_A)
#define SPI_M_NSS_PIN           (GPIO_PIN_04)
#define SPI_M_SCK_PORT          (GPIO_PORT_A)
#define SPI_M_SCK_PIN           (GPIO_PIN_05)
#define SPI_M_MOSI_PORT         (GPIO_PORT_C)
#define SPI_M_MOSI_PIN          (GPIO_PIN_01)
#define SPI_M_MISO_PORT         (GPIO_PORT_F)
#define SPI_M_MISO_PIN          (GPIO_PIN_06)

/* SPI port definition for slave */
#define SPI_S_NSS_PORT          (GPIO_PORT_B)
#define SPI_S_NSS_PIN           (GPIO_PIN_12)
#define SPI_S_SCK_PORT          (GPIO_PORT_B)
#define SPI_S_SCK_PIN           (GPIO_PIN_10)
#define SPI_S_MOSI_PORT         (GPIO_PORT_C)
#define SPI_S_MOSI_PIN          (GPIO_PIN_03)
#define SPI_S_MISO_PORT         (GPIO_PORT_B)
#define SPI_S_MISO_PIN          (GPIO_PIN_02)

#define BUF_LENGTH              (128UL)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Master_Init(void);
static void Slave_Init(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static char u8MasterTxBuf[BUF_LENGTH] = "SPI master tx buffer data: abcdefg!";
static char u8MasterRxBuf[BUF_LENGTH];

static char u8SlaveTxBuf[BUF_LENGTH] = "SPI slave tx buffer data: 1234567!";
static char u8SlaveRxBuf[BUF_LENGTH];

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

    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    /* Configure peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_SPI1, Enable);
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_SPI2, Enable);

#if (APP_TEST_MODE == COM_MASTER)
    /* Wait for key */
    while(Pin_Set == GPIO_ReadInputPins(GPIO_PORT_A, GPIO_PIN_00));
#endif

    /* Port configurate, High driving capacity for output pin. */
    GPIO_StructInit(&stcGpioCfg);

    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_Init(SPI_M_NSS_PORT,  SPI_M_NSS_PIN, &stcGpioCfg);
#endif
    GPIO_Init(SPI_M_SCK_PORT,  SPI_M_SCK_PIN, &stcGpioCfg);
    GPIO_Init(SPI_M_MOSI_PORT, SPI_M_MOSI_PIN, &stcGpioCfg);
    GPIO_Init(SPI_S_MISO_PORT, SPI_S_MISO_PIN, &stcGpioCfg);

    /* CMOS input for input pin */
    stcGpioCfg.u16PinIType = PIN_ITYPE_CMOS;
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_Init(SPI_S_NSS_PORT,  SPI_S_NSS_PIN, &stcGpioCfg);
#endif
    GPIO_Init(SPI_S_SCK_PORT,  SPI_S_SCK_PIN, &stcGpioCfg);
    GPIO_Init(SPI_S_MOSI_PORT, SPI_S_MOSI_PIN, &stcGpioCfg);
    GPIO_Init(SPI_M_MISO_PORT, SPI_M_MISO_PIN, &stcGpioCfg);

    /* Configure SPI Port function for master */
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_SetFunc(SPI_M_NSS_PORT,  SPI_M_NSS_PIN,  GPIO_FUNC_19_SPI1_NSS0, PIN_SUBFUNC_DISABLE);
#endif
    GPIO_SetFunc(SPI_M_SCK_PORT,  SPI_M_SCK_PIN,  GPIO_FUNC_40_SPI1_SCK, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_M_MOSI_PORT, SPI_M_MOSI_PIN, GPIO_FUNC_41_SPI1_MOSI, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_M_MISO_PORT, SPI_M_MISO_PIN, GPIO_FUNC_42_SPI1_MISO, PIN_SUBFUNC_DISABLE);

    /* Configure SPI Port function for slave */
#if (EXAMPLE_WIRE_MODE == SPI_WIRE_4)
    GPIO_SetFunc(SPI_S_NSS_PORT,  SPI_S_NSS_PIN,  GPIO_FUNC_19_SPI2_NSS0, PIN_SUBFUNC_DISABLE);
#endif
    GPIO_SetFunc(SPI_S_SCK_PORT,  SPI_S_SCK_PIN,  GPIO_FUNC_19_SPI2_SCK, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_S_MOSI_PORT, SPI_S_MOSI_PIN, GPIO_FUNC_19_SPI2_MOSI, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_S_MISO_PORT, SPI_S_MISO_PIN, GPIO_FUNC_45_SPI2_MISO, PIN_SUBFUNC_DISABLE);

    /* Configuration SPI function for master*/
    Master_Init();

    /* Configuration SPI function for slave*/
    Slave_Init();

    /* Command SPI function */
    SPI_FunctionCmd(SPI_SLAVE_UNIT, Enable);
    SPI_FunctionCmd(SPI_MASTER_UNIT, Enable);

    /* Clear receive buffer */
    for(uint32_t i = 0UL; i < BUF_LENGTH; i++)
    {
        u8MasterRxBuf[i] = 0x00U;
        u8SlaveRxBuf[i] = 0x00U;
    }

    /* Full duplex data transfer  */
    for(uint32_t i = 0UL; i < BUF_LENGTH; i++)
    {
        /* Wait tx buffer empty for slave */
        while(Reset == SPI_GetStatus(SPI_SLAVE_UNIT, SPI_FLAG_TX_BUFFER_EMPTY))
        {
            ;
        }
        SPI_WriteDataReg(SPI_SLAVE_UNIT, u8SlaveTxBuf[i]);

        /* Wait tx buffer empty for master */
        while(Reset == SPI_GetStatus(SPI_MASTER_UNIT, SPI_FLAG_TX_BUFFER_EMPTY))
        {
            ;
        }
        SPI_WriteDataReg(SPI_MASTER_UNIT, u8MasterTxBuf[i]);

        /* Wait rx buffer full for slave */
        while(Reset == SPI_GetStatus(SPI_SLAVE_UNIT, SPI_FLAG_RX_BUFFER_FULL))
        {
            ;
        }
        u8SlaveRxBuf[i] = (uint8_t)SPI_ReadDataReg(SPI_SLAVE_UNIT);

        /* Wait rx buffer full for master */
        while(Reset == SPI_GetStatus(SPI_MASTER_UNIT, SPI_FLAG_RX_BUFFER_FULL))
        {
            ;
        }
        u8MasterRxBuf[i] = (uint8_t)SPI_ReadDataReg(SPI_MASTER_UNIT);
    }

    /* Compare u8TxBuffer and u8RxBuffer */
    if ((0U == memcmp(u8MasterTxBuf, u8SlaveRxBuf, BUF_LENGTH))
        && (0U == memcmp(u8SlaveTxBuf, u8MasterRxBuf, BUF_LENGTH)))
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
 * @brief  SPI unit configurate for master
 * @param  [in]  None
 * @retval None.
 */
static void Master_Init(void)
{
    stc_spi_init_t stcSpiInit;

    /* SPI De-initialize */
    SPI_DeInit(SPI_MASTER_UNIT);

    SPI_StructInit(&stcSpiInit);   /* Clear initialize structure */

    stcSpiInit.u32WireMode          = EXAMPLE_WIRE_MODE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32Modfe             = SPI_MODFE_DISABLE;
    stcSpiInit.u32Parity            = SPI_PARITY_INVALID;
    stcSpiInit.u32SpiMode           = EXAMPLE_SPI_MODE;
    stcSpiInit.u32BaudRatePrescaler = EXAMPLE_BR_DIV;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    SPI_Init(SPI_MASTER_UNIT, &stcSpiInit);
}

/**
 * @brief  SPI unit configurate for slave
 * @param  [in]  None
 * @retval None.
 */
static void Slave_Init(void)
{
    stc_spi_init_t stcSpiInit;

    SPI_DeInit(SPI_SLAVE_UNIT);

    SPI_StructInit(&stcSpiInit);   /* Clear initialize structure */
    stcSpiInit.u32WireMode          = EXAMPLE_WIRE_MODE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_SLAVE;
    stcSpiInit.u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32Modfe             = SPI_MODFE_DISABLE;
    stcSpiInit.u32Parity            = SPI_PARITY_INVALID;
    stcSpiInit.u32SpiMode           = EXAMPLE_SPI_MODE;
    stcSpiInit.u32BaudRatePrescaler = EXAMPLE_BR_DIV;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    SPI_Init(SPI_SLAVE_UNIT, &stcSpiInit);
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
