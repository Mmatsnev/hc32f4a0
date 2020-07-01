/**
 *******************************************************************************
 * @file  qspi/qspi_base/source/main.c
 * @brief Main program of QSPI base for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
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
#include "w25q64.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup QSPI_BASE
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*
 * QSPI read mode and W25Q64 instruction.
 * 
 *           QSPI_Read_Mode                  W25Q64_Instruction                 W25Q64_Instruction_Dummy_Cycles
 *     QSPI_READ_STANDARD_READ          |     W25Q64_READ_DATA              |          (Ignore)
 *     QSPI_READ_FAST_READ              |     W25Q64_FAST_READ              |             8
 *     QSPI_READ_FAST_READ_DUAL_OUTPUT  |     W25Q64_FAST_READ_DUAL_OUTPUT  |             8
 *     QSPI_READ_FAST_READ_DUAL_IO      |     W25Q64_FAST_READ_DUAL_IO      |             4
 *     QSPI_READ_FAST_READ_QUAD_OUTPUT  |     W25Q64_FAST_READ_QUAD_OUTPUT  |             8
 *     QSPI_READ_FAST_READ_QUAD_IO      |     W25Q64_FAST_READ_QUAD_IO      |             6
 */
#define APP_QSPI_READ_MODE                  (QSPI_READ_FAST_READ_DUAL_IO)
#define APP_W25Q64_READ_INSTR               (W25Q64_FAST_READ_DUAL_IO)
#define APP_W25Q64_READ_INSTR_DUMMY_CYCLES  (4U)

/* Pin definitions. */
#define QSPI_CS_PORT                (GPIO_PORT_C)
#define QSPI_CS_PIN                 (GPIO_PIN_07)
#define QSPI_SCK_PORT               (GPIO_PORT_C)
#define QSPI_SCK_PIN                (GPIO_PIN_06)
#define QSPI_IO0_PORT               (GPIO_PORT_B)
#define QSPI_IO0_PIN                (GPIO_PIN_13)
#define QSPI_IO1_PORT               (GPIO_PORT_B)
#define QSPI_IO1_PIN                (GPIO_PIN_12)
#define QSPI_IO2_PORT               (GPIO_PORT_B)
#define QSPI_IO2_PIN                (GPIO_PIN_10)
#define QSPI_IO3_PORT               (GPIO_PORT_B)
#define QSPI_IO3_PIN                (GPIO_PIN_02)
#define QSPI_PIN_FUNC               (GPIO_FUNC_18_QSPI)

/* Macro of word-to-byte. */
#define WORD_TO_BYTE(__word__, __au8__)                       \
        do {                                                  \
                (__au8__)[0U] = (uint8_t)(__word__);          \
                (__au8__)[1U] = (uint8_t)((__word__) >> 8U);  \
                (__au8__)[2U] = (uint8_t)((__word__) >> 16U); \
                (__au8__)[3U] = (uint8_t)((__word__) >> 24U); \
        } while (0U)

/* Data size definition for this example. */
#define APP_TEST_DATA_SIZE                  (1024UL * 4UL)

/* External QSPI flash address definition. */
#define APP_TEST_ADDRESS                    (4096UL * 1UL)

/* Debug printing definition. */
#if (DDL_PRINT_ENABLE == DDL_ON)
#define DBG         printf
#else
#define DBG(...)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void SystemClockConfig(void);

static void QspiConfig(void);
static void QspiPinConfig(void);
static void QspiInitConfig(void);

/* Some API for W25Q64 */
void W25Q64_GetUniqueID(uint8_t pu8ID[], uint32_t u32IDSize);
void W25Q64_WriteEnable(void);
void W25Q64_WriteDisable(void);

en_result_t W25Q64_EraseChip(void);
en_result_t W25Q64_EraseSector(uint32_t u32SectorAddress);
en_result_t W25Q64_EraseBlock(uint8_t u8EraseBlockInstr, uint32_t u32BlockAddress);

en_result_t W25Q64_WriteData(uint32_t u32Address, const uint8_t pu8WriteBuf[], uint32_t u32NumByteToWrite);
void W25Q64_ReadData(uint32_t u32Address, uint8_t pu8ReadBuf[], uint32_t u32NumByteToRead);

void W25Q64_WriteCommand(uint8_t u8Instr, uint8_t pu8InstrData[], uint32_t u32InstrDataSize);
en_result_t W25Q64_CheckProcessDone(void);

/* Some function for testing. */
static void AppLoadData(void);
static void AppClearData(void);
static uint8_t AppCheckPageProgram(void);
static uint8_t AppCheckErase(uint32_t u32Szie);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint8_t m_au8WriteData[APP_TEST_DATA_SIZE];
uint8_t m_au8ReadData[APP_TEST_DATA_SIZE];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of qspi_base project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t i;

    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the PLLHP(240MHz) as the system clock. */
    SystemClockConfig();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    QspiConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    AppLoadData();

    while (1)
    {
        DBG("W25Q64 read unique ID......\n");
        W25Q64_GetUniqueID(m_au8ReadData, W25Q64_UNIQUE_ID_SIZE);
        DBG("W25Q64 unique ID:\n");
        for (i = 0UL; i < W25Q64_UNIQUE_ID_SIZE; i++)
        {
            DBG("0x%.2x ", m_au8ReadData[i]);
        }
        DBG("\n");


        DBG("W25Q64 erase sector test......\n");
        AppClearData();

        if (W25Q64_EraseSector(APP_TEST_ADDRESS) == Ok)
        {
            W25Q64_ReadData(APP_TEST_ADDRESS, m_au8ReadData, APP_TEST_DATA_SIZE);
        }
        else
        {
            DBG("W25Q64 erase sector timeout. Extend timeout value in function W25Q64_CheckProcessDone and try again.\n");
            while (1U);
        }
        if (AppCheckErase(APP_TEST_DATA_SIZE) == 0U)
        {
            DBG("W25Q64 erase sector test successfully.\n");
        }
        else
        {
            DBG("--->F W25Q64 erase sector test failed.\n");
            while (1U);
        }


        DBG("W25Q64 page program test......\n");
        AppClearData();
        /* Erase sector before write data. The erase address is the start address of the sector. */
        if (W25Q64_EraseSector(APP_TEST_ADDRESS) == Ok)
        {
            W25Q64_WriteData(APP_TEST_ADDRESS, m_au8WriteData, APP_TEST_DATA_SIZE);
        }
        else
        {
            DBG("W25Q64 write timeout. Extend timeout value in function W25Q64_CheckProcessDone and try again.\n");
            while (1U);
        }
        W25Q64_ReadData(APP_TEST_ADDRESS, m_au8ReadData, APP_TEST_DATA_SIZE);
        if (AppCheckPageProgram() == 0U)
        {
            DBG("W25Q64 page program test successfully.\n");
        }
        else
        {
            DBG("--->F W25Q64 page program test failed.\n");
            while (1U);
        }


        /* Change read mode. */
        QSPI_SetReadMode(QSPI_READ_FAST_READ_QUAD_IO, W25Q64_FAST_READ_QUAD_IO, 6UL);
        QSPI_ReadData(APP_TEST_ADDRESS, m_au8ReadData, APP_TEST_DATA_SIZE);
        if (AppCheckPageProgram() == 0U)
        {
            DBG("++++W25Q64 page program test successfully.\n");
        }
        else
        {
            DBG("++++--->F W25Q64 page program test failed.\n");
            while (1U);
        }


        /* Resume to initial mode */
        QSPI_SetReadMode(APP_QSPI_READ_MODE, APP_W25Q64_READ_INSTR, APP_W25Q64_READ_INSTR_DUMMY_CYCLES);
    }
}

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
    // PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0);
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
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    // PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
}

/**
 * @brief  Configures the PLLHP(240MHz) as the system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_pllh_init_t stcPLLHInit;
    stc_clk_xtal_init_t stcXtalInit;

    /* Configures XTAL. PLLH input source is XTAL. */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EX BUS Max 120MHz */
    CLK_ClkDiv(CLK_CATE_ALL,                                       \
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 | \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV4 | CLK_EXCLK_DIV2 | \
                CLK_HCLK_DIV1));

    CLK_PLLHStrucInit(&stcPLLHInit);
    /*
     * PLLP_freq = ((PLL_source / PLLM) * PLLN) / PLLP
     *           = (8 / 1) * 120 / 4
     *           = 240
     */
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = (1UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLN = (120UL - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLP = (4UL   - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLQ = (16UL  - 1UL);
    stcPLLHInit.PLLCFGR_f.PLLR = (16UL  - 1UL);

    /* stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL; */
    CLK_PLLHInit(&stcPLLHInit);

    /* Set SRAM wait cycles. */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    /* Set EFM wait cycle. 5 wait cycles needed when system clock is 240MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);

    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
}

/**
 * @brief  QSPI configuration.
 * @param  None 
 * @retval None
 */
static void QspiConfig(void)
{
    QspiPinConfig();
    QspiInitConfig();
}

/**
 * @brief  QSPI pin configuration.
 * @param  None
 * @retval None
 */
static void QspiPinConfig(void)
{
    GPIO_SetFunc(QSPI_CS_PORT, QSPI_CS_PIN, \
                 QSPI_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(QSPI_SCK_PORT, QSPI_SCK_PIN, \
                 QSPI_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(QSPI_IO0_PORT, QSPI_IO0_PIN, \
                 QSPI_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(QSPI_IO1_PORT, QSPI_IO1_PIN, \
                 QSPI_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(QSPI_IO2_PORT, QSPI_IO2_PIN, \
                 QSPI_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(QSPI_IO3_PORT, QSPI_IO3_PIN, \
                 QSPI_PIN_FUNC, PIN_SUBFUNC_DISABLE);
}

/**
 * @brief  QSPI initialization configuration.
 * @param  None
 * @retval None
 */
static void QspiInitConfig(void)
{
    stc_qspi_init_t stcInit;

    PWC_Fcg1PeriphClockCmd(PWC_FCG1_QSPI, Enable);

    QSPI_StructInit(&stcInit);
    stcInit.u32ClkDiv        = 3UL;
    stcInit.u32ReadMode      = APP_QSPI_READ_MODE;
    stcInit.u8RomAccessInstr = APP_W25Q64_READ_INSTR;
    stcInit.u32DummyCycles   = APP_W25Q64_READ_INSTR_DUMMY_CYCLES;
    QSPI_Init(&stcInit);
}

/**
 * @brief  W25Q64 write command.
 * @param  [in]  u8Instr                W25Q64 instruction.
 * @param  [in]  pu8InstrData           Pointer to an array which stores the data to be following behind u8Instr.
 * @param  [in]  u32InstrDataSize       Size of pu8InstrData.
 * @retval None
 */
void W25Q64_WriteCommand(uint8_t u8Instr, uint8_t pu8InstrData[], uint32_t u32InstrDataSize)
{
    uint32_t i;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(u8Instr);
    if ((pu8InstrData != NULL) && (u32InstrDataSize > 0U))
    {
        for (i=0UL; i<u32InstrDataSize; i++)
        {
            QSPI_WriteDirectCommValue(pu8InstrData[i]);
        }
    }
    QSPI_ExitDirectCommMode();
}

/**
 * @brief  W25Q64 check process done.
 * @param  None
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorTimeout:                W25Q64 works timeout.
 */
en_result_t W25Q64_CheckProcessDone(void)
{
    uint8_t u8Status = 0U;
    uint32_t u32TimeCnt;
    stc_clk_freq_t stcClkFreq;
    en_result_t enRet = ErrorTimeout;

    CLK_GetClockFreq(&stcClkFreq);
    u32TimeCnt = stcClkFreq.sysclkFreq / 100U;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25Q64_READ_STATUS_REGISTER_1);
    while (u32TimeCnt--)
    {
        u8Status = QSPI_ReadDirectCommValue();
        if ((u8Status & W25Q64_FLAG_BUSY) == 0U)
        {
            enRet = Ok;
            break;
        }
    }
    QSPI_ExitDirectCommMode();

    return enRet;
}

/**
 * @brief  Get the UID of W25Q64.
 * @param  [in]  pu8ID                  Pointer to an array which is used to store the UID.
 * @param  [in]  u32IDSize              Size of pu8ID.
 * @retval None
 */
void W25Q64_GetUniqueID(uint8_t pu8ID[], uint32_t u32IDSize)
{
    uint32_t i;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(W25Q64_READ_UNIQUE_ID);

    /* Four dummy bytes. */
    for (i=0U; i<4U; i++)
    {
        QSPI_WriteDirectCommValue(0xFFU);
    }

    for (i=0U; i<u32IDSize; i++)
    {
        pu8ID[i] = QSPI_ReadDirectCommValue();
    }

    QSPI_ExitDirectCommMode();
}

/**
 * @brief  W25Q64 write enable.
 * @param  None
 * @retval None
 */
void W25Q64_WriteEnable(void)
{
    W25Q64_WriteCommand(W25Q64_WRITE_ENABLE, NULL, 0U);
}

/**
 * @brief  W25Q64 write disable.
 * @param  None
 * @retval None
 */
void W25Q64_WriteDisable(void)
{
    W25Q64_WriteCommand(W25Q64_WRITE_DISABLE, NULL, 0U);
}

/**
 * @brief  W25Q64 erase chip.
 * @param  None
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorTimeout:                W25Q64 works timeout.
 */
en_result_t W25Q64_EraseChip(void)
{
    W25Q64_WriteEnable();
    W25Q64_WriteCommand(W25Q64_CHIP_ERASE, NULL, 0U);
    return W25Q64_CheckProcessDone();
}

/**
 * @brief  W25Q64 erase sector.
 * @param  [in]  u32SectorAddress       The start address of the target sector.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorTimeout:                W25Q64 works timeout.
 */
en_result_t W25Q64_EraseSector(uint32_t u32SectorAddress)
{
    uint8_t au8Address[4U];
    WORD_TO_BYTE(u32SectorAddress, au8Address);
    W25Q64_WriteEnable();
    W25Q64_WriteCommand(W25Q64_SECTOR_ERASE, au8Address, 3U);
    return W25Q64_CheckProcessDone();
}

/**
 * @brief  W25Q64 erase sector.
 * @param  [in]  u8EraseBlockInstr      Instrcution of erase block.
 * @param  [in]  u32BlockAddress        The start address of the target block.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorTimeout:                W25Q64 works timeout.
 */
en_result_t W25Q64_EraseBlock(uint8_t u8EraseBlockInstr, uint32_t u32BlockAddress)
{
    uint8_t au8Address[4U];

    WORD_TO_BYTE(u32BlockAddress, au8Address);
    W25Q64_WriteEnable();
    W25Q64_WriteCommand(u8EraseBlockInstr, au8Address, 3U);
    return W25Q64_CheckProcessDone();
}

/**
 * @brief  W25Q64 write data.
 * @param  [in]  u32Address             Target address.
 * @param  [in]  pu8WriteBuf            Source data buffer.
 * @param  [in]  u32NumByteToWrite      Number of byte to be written.
 * @retval An en_result_t enumeration value.
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       pu8WriteBuf == NULL or u32NumByteToWrite == 0U
 *   @arg  ErrorTimeout:                W25Q64 works timeout.
 */
en_result_t W25Q64_WriteData(uint32_t u32Address, const uint8_t pu8WriteBuf[], uint32_t u32NumByteToWrite)
{
    uint32_t u32TempSize;
    uint32_t u32AddrOfst = 0U;
    en_result_t enRet = ErrorInvalidParameter;

    if (pu8WriteBuf != NULL)
    {
        while (u32NumByteToWrite)
        {
            if (u32NumByteToWrite >= W25Q64_PAGE_SIZE)
            {
                u32TempSize = W25Q64_PAGE_SIZE;
            }
            else
            {
                u32TempSize = u32NumByteToWrite;
            }

            W25Q64_WriteEnable();
            QSPI_WriteData(W25Q64_PAGE_PROGRAM, u32Address, (uint8_t *)&pu8WriteBuf[u32AddrOfst], u32TempSize);
            enRet = W25Q64_CheckProcessDone();
            if (enRet != Ok)
            {
                break;
            }
            u32NumByteToWrite -= u32TempSize;
            u32AddrOfst += u32TempSize;
            u32Address += u32TempSize;
        }
    }

    return enRet;
}

/**
 * @brief  W25Q64 read data.
 * @param  [in]  u32Address             Target address.
 * @param  [in]  pu8ReadBuf             Destination data buffer.
 * @param  [in]  u32NumByteToRead       Number of byte to be read.
 * @retval None
 */
void W25Q64_ReadData(uint32_t u32Address, uint8_t pu8ReadBuf[], uint32_t u32NumByteToRead)
{
    QSPI_ReadData(u32Address, pu8ReadBuf, u32NumByteToRead);
}

/**
 * @brief  The following functions just for testing.
 * @param  None
 * @retval None
 */
static void AppLoadData(void)
{
    uint32_t i;
    for (i=0UL; i<APP_TEST_DATA_SIZE; i++)
    {
        m_au8WriteData[i] = (uint8_t)i;
    }
}

static void AppClearData(void)
{
    uint32_t i;
    for (i=0UL; i<APP_TEST_DATA_SIZE; i++)
    {
        m_au8ReadData[i] = 0U;
    }
}

static uint8_t AppCheckPageProgram(void)
{
    uint32_t i;
    for (i=0UL; i<APP_TEST_DATA_SIZE; i++)
    {
        if (m_au8ReadData[i] != m_au8WriteData[i])
        {
            return 1U;
        }
    }

    return 0U;
}

static uint8_t AppCheckErase(uint32_t u32Szie)
{
    uint32_t i;
    for (i=0UL; i<u32Szie; i++)
    {
        if (m_au8ReadData[i] != 0xFFU)
        {
            return 1U;
        }
    }

    return 0U;
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
