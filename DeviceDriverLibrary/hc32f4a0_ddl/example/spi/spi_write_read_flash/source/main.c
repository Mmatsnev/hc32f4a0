/**
 *******************************************************************************
 * @file  spi/spi_write_read_flash/source/main.c
 * @brief Main program SPI write read flash for the Device Driver Library.
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
 * @addtogroup SPI_WRITE_READ_FLASH
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* SPI unit and clock definition */
#define SPI_UNIT                        (M4_SPI1)
#define SPI_UNIT_CLOCK                  (PWC_FCG1_SPI1)

/* SPI port definition for master */
#define SPI_NSS_PORT                    (GPIO_PORT_C)
#define SPI_NSS_PIN                     (GPIO_PIN_07)

#define SPI_SCK_PORT                    (GPIO_PORT_C)
#define SPI_SCK_PIN                     (GPIO_PIN_06)
#define SPI_SCK_FUNC                    (GPIO_FUNC_40_SPI1_SCK)

#define SPI_MOSI_PORT                   (GPIO_PORT_B)
#define SPI_MOSI_PIN                    (GPIO_PIN_13)
#define SPI_MOSI_FUNC                   (GPIO_FUNC_41_SPI1_MOSI)

#define SPI_MISO_PORT                   (GPIO_PORT_B)
#define SPI_MISO_PIN                    (GPIO_PIN_12)
#define SPI_MISO_FUNC                   (GPIO_FUNC_42_SPI1_MISO)

/* NSS pin control */
#define SPI_NSS_HIGH()                  (GPIO_SetPins(SPI_NSS_PORT, SPI_NSS_PIN))
#define SPI_NSS_LOW()                   (GPIO_ResetPins(SPI_NSS_PORT, SPI_NSS_PIN))

/* FLASH parameters */
#define FLASH_PAGE_SIZE                 (0x100U)
#define FLASH_SRCTOR_SIZE               (0x1000U)
#define FALSH_MAX_ADDR                  (0x800000UL)
#define FLASH_DUMMY_BYTE_VALUE          (0xffU)
#define FLASH_BUSY_BIT_MASK             (0x01U)

/* FLASH instruction */
#define FLASH_INSTR_WRITE_ENABLE        (0x06U)
#define FLASH_INSTR_PAGE_PROGRAM        (0x02U)
#define FLASH_INSTR_STANDARD_READ       (0x03U)
#define FLASH_INSTR_ERASE_4KB_SECTOR    (0x20U)
#define FLASH_INSTR_READ_SR1            (0x05U)
#define FLASH_READ_MANUFACTURER_ID      (0x90U)

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
 * @brief Configure SPI peripheral function
 *
 * @param [in] None
 *
 * @retval None
 */
static void Spi_Config(void)
{
    stc_spi_init_t stcSpiInit;
    stc_spi_delay_t stcSpiDelayCfg;

    /* Clear initialize structure */
    SPI_StructInit(&stcSpiInit);
    SPI_DelayStructInit(&stcSpiDelayCfg);

    /* Configure peripheral clock */
    PWC_Fcg1PeriphClockCmd(SPI_UNIT_CLOCK, Enable);

    /* SPI De-initialize */
    SPI_DeInit(SPI_UNIT);

    /* Configuration SPI structure */
    stcSpiInit.u32WireMode          = SPI_WIRE_3;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32Modfe             = SPI_MODFE_DISABLE;
    stcSpiInit.u32Parity            = SPI_PARITY_INVALID;
    stcSpiInit.u32SpiMode           = SPI_MODE_0;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_PCLK1_DIV256;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    SPI_Init(SPI_UNIT, &stcSpiInit);

    stcSpiDelayCfg.u32IntervalDelay = SPI_INTERVAL_TIME_8SCK_2PCLK1;
    stcSpiDelayCfg.u32ReleaseDelay = SPI_RELEASE_TIME_8SCK;
    stcSpiDelayCfg.u32SetupDelay = SPI_SETUP_TIME_1SCK;
    SPI_DelayTimeCfg(SPI_UNIT, &stcSpiDelayCfg);

    SPI_FunctionCmd(SPI_UNIT, Enable);
}

/**
 * @brief SPI flash write byte function
 *
 * @param [in] u8Data                      SPI write data to flash
 *
 * @retval uint8_t                         SPI receive data from flash
 */
static uint8_t SpiFlash_WriteReadByte(uint8_t u8Data)
{
    uint8_t u8Byte;

    /* Wait tx buffer empty */
    while (Reset == SPI_GetStatus(SPI_UNIT, SPI_FLAG_TX_BUFFER_EMPTY))
    {
    }
    /* Send data */
    SPI_WriteDataReg(SPI_UNIT, (uint32_t)u8Data);

    /* Wait rx buffer full */
    while (Reset == SPI_GetStatus(SPI_UNIT, SPI_FLAG_RX_BUFFER_FULL))
    {
    }
    /* Receive data */
    u8Byte = (uint8_t)SPI_ReadDataReg(SPI_UNIT);

    return u8Byte;
}

/**
 * @brief SPI flash write enable function
 *
 * @param [in] None
 *
 * @retval None
 */
void SpiFlash_WriteEnable(void)
{
    SPI_NSS_LOW();
    SpiFlash_WriteReadByte(FLASH_INSTR_WRITE_ENABLE);
    SPI_NSS_HIGH();
}

/**
 * @brief SPI flash wait for write operation end function
 *
 * @param [in] None
 *
 * @retval Ok                              Flash internal operation finish
 * @retval ErrorTimeout                    Flash internal operation timeout
 */
en_result_t SpiFlash_WaitForWriteEnd(void)
{
    en_result_t enRet = Ok;
    uint8_t u8Status = 0U;
    uint32_t u32Timeout;
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    u32Timeout = stcClkFreq.sysclkFreq / 1000U;
    SPI_NSS_LOW();
    SpiFlash_WriteReadByte(FLASH_INSTR_READ_SR1);
    do
    {
        u8Status = SpiFlash_WriteReadByte(FLASH_DUMMY_BYTE_VALUE);
        u32Timeout--;
    } while ((u32Timeout != 0UL) &&
             ((u8Status & FLASH_BUSY_BIT_MASK) == FLASH_BUSY_BIT_MASK));

    if (FLASH_BUSY_BIT_MASK == u8Status)
    {
        enRet = ErrorTimeout;
    }
    SPI_NSS_HIGH();

    return enRet;
}

/**
 * @brief SPI flash page write program function
 *
 * @param [in] u32Addr                     Valid flash address
 * @param [in] pData                       Pointer to send data buffer
 * @param [in] len                         Send data length
 *
 * @retval Error                           Page write program failed
 * @retval Ok                              Page write program success
 */
en_result_t SpiFlash_WritePage(uint32_t u32Addr, const uint8_t pData[], uint16_t len)
{
    en_result_t enRet = Ok;
    uint16_t u16Index = 0U;

    if ((u32Addr > FALSH_MAX_ADDR) || (NULL == pData) || (len > FLASH_PAGE_SIZE))
    {
        enRet = Error;
    }
    else
    {
        SpiFlash_WriteEnable();
        /* Send data to flash */
        SPI_NSS_LOW();
        SpiFlash_WriteReadByte(FLASH_INSTR_PAGE_PROGRAM);
        SpiFlash_WriteReadByte((uint8_t)((u32Addr & 0xFF0000UL) >> 16U));
        SpiFlash_WriteReadByte((uint8_t)((u32Addr & 0xFF00U) >> 8U));
        SpiFlash_WriteReadByte((uint8_t)(u32Addr & 0xFFU));
        while (len--)
        {
            SpiFlash_WriteReadByte(pData[u16Index]);
            u16Index++;
        }
        SPI_NSS_HIGH();
        /* Wait for flash idle */
        enRet = SpiFlash_WaitForWriteEnd();
    }

    return enRet;
}

/**
 * @brief SPI flash read data function
 *
 * @param [in] u32Addr                     Valid flash address
 * @param [out] pData                      Pointer to receive data buffer
 *
 * @param [in] len                         Read data length
 *
 * @retval Error                           Read data program failed
 * @retval Ok                              Read data program success
 */
en_result_t SpiFlash_ReadData(uint32_t u32Addr, uint8_t pData[], uint16_t len)
{
    en_result_t enRet = Ok;
    uint16_t u16Index = 0U;

    if ((u32Addr > FALSH_MAX_ADDR) || (NULL == pData))
    {
        enRet = Error;
    }
    else
    {
        SpiFlash_WriteEnable();
        /* Send data to flash */
        SPI_NSS_LOW();
        SpiFlash_WriteReadByte(FLASH_INSTR_STANDARD_READ);
        SpiFlash_WriteReadByte((uint8_t)((u32Addr & 0xFF0000UL) >> 16U));
        SpiFlash_WriteReadByte((uint8_t)((u32Addr & 0xFF00U) >> 8U));
        SpiFlash_WriteReadByte((uint8_t)(u32Addr & 0xFFU));
        while (len--)
        {
            pData[u16Index] = SpiFlash_WriteReadByte(FLASH_DUMMY_BYTE_VALUE);
            u16Index++;
        }
        SPI_NSS_HIGH();
    }

    return enRet;
}

/**
 * @brief SPI flash read ID for test
 *
 * @param [in] None
 *
 * @retval uint8_t                         Flash ID
 */
uint8_t SpiFlash_ReadID(void)
{
    uint8_t u8IdRead;
    SPI_NSS_LOW();
    SpiFlash_WriteReadByte(FLASH_READ_MANUFACTURER_ID);
    SpiFlash_WriteReadByte((uint8_t)0x00U);
    SpiFlash_WriteReadByte((uint8_t)0x00U);
    SpiFlash_WriteReadByte((uint8_t)0x00U);
    u8IdRead = SpiFlash_WriteReadByte(FLASH_DUMMY_BYTE_VALUE);
    SPI_NSS_HIGH();
    return u8IdRead;
}

/**
 * @brief SPI flash erase 4Kb sector function
 *
 * @param [in] u32Addr                     Valid flash address
 *
 * @retval Error                           Sector erase failed
 * @retval Ok                              Sector erase success
 */
en_result_t SpiFlash_Erase4KbSector(uint32_t u32Addr)
{
    en_result_t enRet = Ok;

    if (u32Addr >= FALSH_MAX_ADDR)
    {
        enRet =  Error;
    }
    else
    {
        SpiFlash_WriteEnable();
        /* Send instruction to flash */
        SPI_NSS_LOW();
        SpiFlash_WriteReadByte(FLASH_INSTR_ERASE_4KB_SECTOR);
        SpiFlash_WriteReadByte((uint8_t)((u32Addr & 0xFF0000UL) >> 16U));
        SpiFlash_WriteReadByte((uint8_t)((u32Addr & 0xFF00U) >> 8U));
        SpiFlash_WriteReadByte((uint8_t)(u32Addr & 0xFFU));
        //SPI_GetStatus(const M4_SPI_TypeDef *SPIx, uint32_t u32Flag) //todo
        SPI_NSS_HIGH();
        /* Wait for flash idle */
        enRet = SpiFlash_WaitForWriteEnd();
    }

    return enRet;
}

/**
 * @brief  Main function of spi_master_base project
 * @param  None
 * @retval int32_t return value, if needed
 */

int32_t main(void)
{
    uint32_t flashAddr = 0UL;
    uint16_t bufferLen = 0U;
    char txBuffer[] = "SPI read and write flash example: Welcome to use HDSC micro chip";
    char rxBuffer[128];
    stc_gpio_init_t stcGpioCfg;

    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_KEY_Init();
    BSP_LED_Init();

    /* Port configurate */
    GPIO_StructInit(&stcGpioCfg);

    /* High driving capacity for output pin. */
    stcGpioCfg.u16PinDir = PIN_DIR_OUT;
    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
    stcGpioCfg.u16PinState = PIN_STATE_SET;
    GPIO_Init(SPI_NSS_PORT,  SPI_NSS_PIN, &stcGpioCfg);

    GPIO_StructInit(&stcGpioCfg);
    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
    GPIO_Init(SPI_SCK_PORT,  SPI_SCK_PIN, &stcGpioCfg);
    GPIO_Init(SPI_MOSI_PORT, SPI_MOSI_PIN, &stcGpioCfg);

    /* CMOS input for input pin */
    stcGpioCfg.u16PinDrv = PIN_DRV_LOW;
    stcGpioCfg.u16PinIType = PIN_ITYPE_CMOS;
    GPIO_Init(SPI_MISO_PORT, SPI_MISO_PIN, &stcGpioCfg);

    /* Configure SPI Port function for master */
    GPIO_SetFunc(SPI_SCK_PORT,  SPI_SCK_PIN,  SPI_SCK_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_FUNC, PIN_SUBFUNC_DISABLE);

    /* Configure SPI for SPI flash */
    Spi_Config();

    /* Get tx buffer length */
    bufferLen = (uint16_t)sizeof(txBuffer);

    while (1)
    {
        while(Pin_Set == GPIO_ReadInputPins(GPIO_PORT_A, GPIO_PIN_00))
        {
            /* Wait */
        }
        DDL_DelayMS(10UL);
        while(Pin_Reset == GPIO_ReadInputPins(GPIO_PORT_A, GPIO_PIN_00))
        {
            /* Wait */
        }

        BSP_LED_Off(LED_RED);
        BSP_LED_Off(LED_BLUE);
        memset(rxBuffer, 0L, sizeof(rxBuffer));
        /* Erase sector */
        SpiFlash_Erase4KbSector(flashAddr);
        /* Write data to flash */
        SpiFlash_WritePage(flashAddr, (uint8_t*)&txBuffer[0], bufferLen);
        /* Read data from flash */
        SpiFlash_ReadData(flashAddr, (uint8_t*)&rxBuffer[0], bufferLen);

        /* Compare txBuffer and rxBuffer */
        if (memcmp(txBuffer, rxBuffer, (uint32_t)bufferLen) != 0)
        {
            BSP_LED_On(LED_RED);
        }
        else
        {
            BSP_LED_On(LED_BLUE);
        }

        /* Flash address offset */
        flashAddr += FLASH_SRCTOR_SIZE;
        if (flashAddr >= FALSH_MAX_ADDR)
        {
            flashAddr = 0U;
        }

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
