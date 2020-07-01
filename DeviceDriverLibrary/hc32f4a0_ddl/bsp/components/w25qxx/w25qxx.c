/**
 *******************************************************************************
 * @file  w25qxx.c
 * @brief This midware file provides firmware functions to W25QXX group spi flash.
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
#include "w25qxx.h"
#include "ev_hc32f4a0_lqfp176_w25qxx.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/** @defgroup W25QXX Flash Driver for W25QXX
  * @{
  */

#if (BSP_W25QXX_ENABLE == BSP_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Local_Macros W25QXX Local Macros
 * @{
 */
#define W25Q_BIT_0                  (1UL << 0U)
#define W25Q_BIT_1                  (1UL << 1U)
#define W25Q_BIT_2                  (1UL << 2U)
#define W25Q_BIT_3                  (1UL << 3U)
#define W25Q_BIT_4                  (1UL << 4U)
#define W25Q_BIT_5                  (1UL << 5U)
#define W25Q_BIT_6                  (1UL << 6U)
#define W25Q_BIT_7                  (1UL << 7U)
#define W25Q_BIT_8                  (1UL << 8U)
#define W25Q_BIT_9                  (1UL << 9U)
#define W25Q_BIT_10                 (1UL << 10U)
#define W25Q_BIT_11                 (1UL << 11U)
#define W25Q_BIT_12                 (1UL << 12U)
#define W25Q_BIT_13                 (1UL << 13U)
#define W25Q_BIT_14                 (1UL << 14U)
#define W25Q_BIT_15                 (1UL << 15U)

#define W25Q_ST_BUSY                ((uint16_t)W25Q_BIT_0)
#define W25Q_ST_WEL                 ((uint16_t)W25Q_BIT_1)  /*<! Write enable latch. */

#define LOAD_CMD(a, cmd, addr)  do  {                                       \
                                        (a)[0U] = (cmd);                    \
                                        (a)[1U] = (uint8_t)((addr) >> 16U); \
                                        (a)[2U] = (uint8_t)((addr) >> 8U);  \
                                        (a)[3U] = (uint8_t)(addr);          \
                                    } while (0U)

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Local_Functions W25QXX Local Functions
 * @{
 */
static void W25QXX_WriteCmd(uint8_t u8Cmd, const uint8_t *pu8CmdData, uint32_t u32CmdDataLength);
static void W25QXX_ReadCmd(uint8_t u8Cmd, uint8_t *pu8CmdData, uint32_t u32CmdDataLength,
                         uint8_t *pu8Info, uint8_t u8InfoLength);

static void W25QXX_Wt(uint8_t u8Cmd, uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength);
static void W25QXX_Rd(uint8_t u8Cmd, uint32_t u32Address, uint8_t *pu8Data, uint32_t u32DataLength);

static void W25QXX_WaitBusy(void);

static void W25QXX_WriteEnable(void);
static void W25QXX_WriteDisable(void);

static void W25QXX_WritePage(uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength);

static void W25QXX_Write_NoCheck(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
static void W25QXX_WriteCmd(uint8_t u8Cmd, const uint8_t *pu8CmdData, uint32_t u32CmdDataLength);
static void W25QXX_ReadCmd(uint8_t u8Cmd, uint8_t *pu8CmdData, uint32_t u32CmdDataLength,
                         uint8_t *pu8Info, uint8_t u8InfoLength);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t W25QXX_BUFFER[4096U];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Global_Functions W25QXX Global Functions
 * @{
 */

/**
 * @brief  Initializes W25QXX.
 * @param  [out] pstcW25qxx         Pointer to a stc_w25qxx_t structure which contains the information of the SPI flash.
 * @retval None
 */
void W25QXX_Init(stc_w25qxx_t *pstcW25qxx)
{
    BSP_W25Q_SPI_Init();

    if (pstcW25qxx != NULL)
    {
        /* Read Flash ID */
        pstcW25qxx->u16ManId = W25QXX_ReadManDeviceId();

        switch (pstcW25qxx->u16ManId)
        {
            case W25Q64:
                pstcW25qxx->u32PageCount       = 32768UL;    /* W25Q64 contains 32768 pages. */
                pstcW25qxx->u32SectorCount     = 2048U;     /* W25Q64 contains 2048 sectors. */
                pstcW25qxx->u32BlockCount32k   = 0U;        /* DO NOT support 32K block. */
                pstcW25qxx->u32BlockCount64k   = 128U;
                pstcW25qxx->u32CapacityInBytes = pstcW25qxx->u32PageCount * W25Q_SIZE_PAGE;
                pstcW25qxx->u32CapacityInKB    = pstcW25qxx->u32CapacityInBytes * W25Q_SIZE_1K;
                break;

            default:
                break;
        }
    }
}

/**
 * @brief  Read manufacturer device ID.
 * @param  None
 * @retval 16 bit manufacturer device ID.
 */
uint16_t W25QXX_ReadManDeviceId(void)
{
    uint8_t au8TempId[2U];
    uint8_t au8Dummy[3U] = {0U};
    uint16_t u16ManID;

    W25QXX_ReadCmd(W25Q_MANUFACTURER_DEVICE_ID, au8Dummy, 3U, au8TempId, 2U);

    u16ManID = (uint16_t)au8TempId[0U] << 8U;
    u16ManID |= au8TempId[1U];

    return u16ManID;
}

/**
 * @brief  Read unique ID.
 * @param  [out] pu8UniqueId        Pointer to a buffer the 64 bit unique ID to be stored.
 * @retval None
 */
void W25QXX_ReadUniqueId(uint8_t *pu8UniqueId)
{
    uint8_t au8Dummy[4U] = {0U};

    W25QXX_ReadCmd(W25Q_READ_UNIQUE_ID, au8Dummy, 4U, pu8UniqueId, 8U);
}

/**
 * @brief  W25QXX read status register.
 * @param  None
 * @retval 16 bit W25QXX status.
 */
uint16_t W25QXX_ReadStatus(void)
{
    uint8_t  u8TempStatus;
    uint16_t u16RetStatus;

    W25QXX_ReadCmd(W25Q_READ_STATUS_REG_2, NULL, 0U, &u8TempStatus, 1U);

    u16RetStatus = u8TempStatus;

    W25QXX_ReadCmd(W25Q_READ_STATUS_REG_1, NULL, 0U, &u8TempStatus, 1U);

    u16RetStatus <<= 8U;
    u16RetStatus |= u8TempStatus;

    return u16RetStatus;
}

/**
 * @brief  W25QXX write status register
 * @param  [in]  u16Status          Specified status.
 * @retval None
 */
void W25QXX_WriteStatus(uint16_t u16Status)
{
    uint8_t au8Data[2U];

    au8Data[0U] = (uint8_t)u16Status;
    au8Data[1U] = (uint8_t)(u16Status >> 8U);

    W25QXX_WriteCmd(W25Q_WRITE_STATUS_REG, au8Data, 2U);
}

/**
 * @brief  W25QXX power down.
 * @param  None
 * @retval None
 */
void W25QXX_PowerDown(void)
{
    W25QXX_WriteCmd(W25Q_POWER_DOWN, NULL, 0U);

    W25QXX_DELAY_MS(1U);
}

/**
 * @brief  W25QXX release power down.
 * @param  None
 * @retval None
 */
void W25QXX_ReleasePowerDown(void)
{
    W25QXX_WriteCmd(W25Q_RELEASE_POWER_DOWN, NULL, 0U);

    W25QXX_DELAY_MS(1U);
}

/**
 * @brief  W25QXX chip ease.
 * @param  None
 * @retval None
 */
void W25QXX_EraseChip(void)
{
    W25QXX_WriteEnable();
    W25QXX_WaitBusy();
    W25QXX_WriteCmd(W25Q_CHIP_ERASE, NULL, 0U);
    W25QXX_WaitBusy();
}

/**
 * @brief  W25QXX sector ease.
 * @param  [in] u32SectorAddress   The address of the specified sector.
 * @retval None
 */
void W25QXX_EraseSector(uint32_t u32SectorAddress)
{
    u32SectorAddress *= W25Q_SIZE_SECTOR;

    W25QXX_WriteEnable();
    W25QXX_WaitBusy();

    W25QXX_Wt(W25Q_SECTOR_ERASE, u32SectorAddress, NULL, 0U);

    W25QXX_WaitBusy();
    W25QXX_WriteDisable();
}

/**
 * @brief  W25QXX block ease.
 * @param  [in] u32BlockAddress    The address of the specified block.
 * @retval None
 */
void W25QXX_EraseBlock(uint32_t u32BlockAddress)
{
    W25QXX_Wt(W25Q_BLOCK_ERASE_64K, u32BlockAddress, NULL, 0U);
}


/**
 * @brief W25QXX flash write
 * @param [in] pBuffer          Data buffer to be written
 * @param [in] WriteAddr        Address to be written
 * @param [in] NumByteToWrite   Number to be written, (MAX. 65535)
 * @retval None
 */
static void W25QXX_Write_NoCheck(const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t pageremain;
    uint32_t u32BufAdrTmp = (uint32_t)pBuffer;
    pageremain = 256U - WriteAddr % 256U;
    if (NumByteToWrite <= pageremain)
    {
        pageremain = NumByteToWrite;
    }
    while (1)
    {
        W25QXX_WritePage(WriteAddr, (uint8_t *)u32BufAdrTmp, pageremain);
        if (NumByteToWrite == (uint16_t)pageremain)
        {
            break;
        }
        else //NumByteToWrite>pageremain
        {
            u32BufAdrTmp += pageremain;
            WriteAddr      += pageremain;

            NumByteToWrite -= (uint16_t)pageremain;
            if (NumByteToWrite > 256U)
            {
                pageremain = 256U;
            }
            else
            {
                pageremain = (uint32_t)NumByteToWrite;
            }
        }
    }
}


/**
 * @brief  W25QXX write data.
 * @param  [in]  u32Address         The start address of the data to be written.
 * @param  [in]  pu8WriteBuf        The pointer to the buffer contains the data to be written.
 * @param  [in]  u32NumByteToWrite  Buffer size in bytes.
 * @retval None
 */
void W25QXX_WriteData(uint32_t u32Address, const uint8_t *pu8WriteBuf, uint32_t u32NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t  *pW25QXX_BUF;
    pW25QXX_BUF = W25QXX_BUFFER;

    secpos     = u32Address / 4096U;
    secoff     = (uint16_t)(u32Address % 4096U);
    secremain  = 4096U - secoff;

    if (u32NumByteToWrite <= secremain)
    {
        secremain = (uint16_t)u32NumByteToWrite;                                //��less than 4K
    }
    while (1)
    {
        W25QXX_ReadData(secpos * 4096U, pW25QXX_BUF, 4096U);         // read one sector content
        for (i = 0U; i < secremain; i++)                            // check if blank sector
        {
            if (pW25QXX_BUF[secoff + i] != (uint8_t)0xFFU)
            {
                break;
            }
        }
        if (i < secremain)
        {
            W25QXX_EraseSector(secpos);                           // not blank, need erase
            for (i = 0U; i < secremain; i++)                        // backup first
            {
                pW25QXX_BUF[i + secoff] = pu8WriteBuf[i];
            }
            W25QXX_Write_NoCheck(pW25QXX_BUF, secpos * 4096U, 4096U); // write back after erase

        }
        else
        {
            W25QXX_Write_NoCheck(pu8WriteBuf, u32Address, secremain);
        }
        if (u32NumByteToWrite == secremain)
        {
            break;
        }
        else
        {
            secpos++;                                              // next sector
            secoff          = 0U;

            pu8WriteBuf        += secremain;
            u32Address         += secremain;
            u32NumByteToWrite  -= secremain;
            if (u32NumByteToWrite > 4096U)
            {
                secremain = 4096U;
            }
            else
            {
                secremain = (uint16_t)u32NumByteToWrite;
            }
        }
    }
}

/**
 * @brief  W25QXX read data.
 * @param  [in]  u32Address         The start address of the data to be read.
 * @param  [in]  pu8ReadBuf         The pointer to the buffer contains the data to be stored.
 * @param  [in]  u32NumByteToRead   Buffer size in bytes.
 * @retval None
 */
void W25QXX_ReadData(uint32_t u32Address, uint8_t *pu8ReadBuf, uint32_t u32NumByteToRead)
{
    W25QXX_Rd(W25Q_READ_DATA, u32Address, pu8ReadBuf, u32NumByteToRead);
}

/**
 * @}
 */

/**
 * @addtogroup W25QXX_Local_Functions W25QXX Local Functions
 * @{
 */

/**
 * @brief  W25QXX write command.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  pu8CmdData         Pointer to a buffer that contains the data following the command.
 * @param  [in]  u32CmdDataLength   The length of the command data in bytes.
 * @retval None
 */
static void W25QXX_WriteCmd(uint8_t u8Cmd, const uint8_t *pu8CmdData, uint32_t u32CmdDataLength)
{
    W25Q_CS_ACTIVE();
    BSP_W25Q_SPI_Transmit(&u8Cmd, 1U);
    BSP_W25Q_SPI_Transmit(pu8CmdData, u32CmdDataLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX read command.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  pu8CmdData         Pointer to a buffer that contains the data following the command.
 * @param  [in]  u32CmdDataLength   The length of the command data in bytes.
 * @param  [in]  pu8Info            The information of the command.
 * @param  [in]  u8InfoLength       The length of the information.
 * @retval None
 */
static void W25QXX_ReadCmd(uint8_t u8Cmd, uint8_t *pu8CmdData, uint32_t u32CmdDataLength,
                         uint8_t *pu8Info, uint8_t u8InfoLength)
{
    W25Q_CS_ACTIVE();
    BSP_W25Q_SPI_Transmit(&u8Cmd, 1U);
    BSP_W25Q_SPI_Transmit(pu8CmdData, u32CmdDataLength);
    BSP_W25Q_SPI_Receive(pu8Info, (uint32_t)u8InfoLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX write data.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  u32Address         The start address of the data to be written.
 * @param  [in]  pu8Data            The data to be written.
 * @param  [in]  u32DataLength      The length of the data in bytes.
 * @retval None
 */
static void W25QXX_Wt(uint8_t u8Cmd, uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength)
{
    uint8_t au8Cmd[4U];

    LOAD_CMD(au8Cmd, u8Cmd, u32Address);

    W25Q_CS_ACTIVE();
    BSP_W25Q_SPI_Transmit(au8Cmd, 4U);
    BSP_W25Q_SPI_Transmit(pu8Data, u32DataLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX read data.
 * @param  [in]  u8Cmd              Command of W25QXX.
 * @param  [in]  u32Address         The start address of the data to be written.
 * @param  [in]  pu8Data            The data to be stored.
 * @param  [in]  u32DataLength      The length of the data in bytes.
 * @retval None
 */
static void W25QXX_Rd(uint8_t u8Cmd, uint32_t u32Address, uint8_t *pu8Data, uint32_t u32DataLength)
{
    uint8_t au8Cmd[4U];

    LOAD_CMD(au8Cmd, u8Cmd, u32Address);

    W25Q_CS_ACTIVE();
    BSP_W25Q_SPI_Transmit(au8Cmd, 4U);
    BSP_W25Q_SPI_Receive(pu8Data, u32DataLength);
    W25Q_CS_INACTIVE();
}

/**
 * @brief  W25QXX Write enable.
 * @param  None
 * @retval None
 */
static void W25QXX_WriteEnable(void)
{
    W25QXX_WriteCmd(W25Q_WRITE_ENABLE, NULL, 0U);
}

/**
 * @brief  W25QXX Write disable.
 * @param  None
 * @retval None
 */
static void W25QXX_WriteDisable(void)
{
    W25QXX_WriteCmd(W25Q_WRITE_DISABLE, NULL, 0U);
}

/**
 * @brief  Wait while W25QXX is busy.
 * @param  None
 * @retval None
 */
static void W25QXX_WaitBusy(void)
{
    while ((W25QXX_ReadStatus() & W25Q_ST_BUSY) == W25Q_ST_BUSY)
    {
        ;
    }
}

/**
 * @brief  W25QXX page program.
 * @param  [in]  u32Address         Start address of the page.
 * @param  [in]  pu8Data            Pointer to a buffer that contains the data to be written.
 * @param  [in]  u32DataLength      Size of the buffer in bytes.
 * @retval None
 */
static void W25QXX_WritePage(uint32_t u32Address, const uint8_t *pu8Data, uint32_t u32DataLength)
{
    W25QXX_WriteEnable();
    W25QXX_Wt(W25Q_PAGE_PROGRAM, u32Address, pu8Data, u32DataLength);
    W25QXX_WaitBusy();
}

/**
 * @}
 */

#endif /* BSP_W25QXX_ENABLE */

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
