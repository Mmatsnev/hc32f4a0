/**
 *******************************************************************************
 * @file  w25qxx.h
 * @brief This file provides firmware functions to W25QXX group spi flash.
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
#ifndef __W25QXX_H__
#define __W25QXX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/** @addtogroup W25QXX
  * @{
  */

#if (BSP_W25QXX_ENABLE == BSP_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Global_Types W25QXX Global Types
 * @{
 */

/**
 * @brief Structure definition of W25QXX information.
 */
typedef struct
{
    uint16_t u16ManId;              /*!< Manufacturer device ID. */
    uint8_t  au8UniqueId[8U];       /*!< 64 bit unique ID number. */
    uint32_t u32PageCount;
    uint32_t u32SectorCount;
    uint32_t u32BlockCount32k;
    uint32_t u32BlockCount64k;
    uint32_t u32CapacityInBytes;
    uint32_t u32CapacityInKB;
} stc_w25qxx_t;


/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup W25QXX_Global_Macros W25QXX Global Macros
 * @{
 */

/**
 * @defgroup W25QXX_ID W25QXX ID
 * @{
 */
#define W25Q80                  (0xEF13U)
#define W25Q16                  (0xEF14U)
#define W25Q32                  (0xEF15U)
#define W25Q64                  (0xEF16U)
#define W25Q128                 (0xEF17U)
/**
 * @}
 */

/**
 * @defgroup W25QXX_Command W25QXX Command
 * @{
 */
#define W25Q_WRITE_ENABLE                   ((uint8_t)0x06U)
#define W25Q_VOLATILE_SR_WRITE_ENABLE       ((uint8_t)0x50U)
#define W25Q_WRITE_DISABLE                  ((uint8_t)0x04U)
#define W25Q_READ_STATUS_REG_1              ((uint8_t)0x05U)
#define W25Q_READ_STATUS_REG_2              ((uint8_t)0x35U)
#define W25Q_WRITE_STATUS_REG               ((uint8_t)0x01U)
#define W25Q_PAGE_PROGRAM                   ((uint8_t)0x02U)
#define W25Q_SECTOR_ERASE                   ((uint8_t)0x20U)
#define W25Q_BLOCK_ERASE_32K                ((uint8_t)0x52U)
#define W25Q_BLOCK_ERASE_64K                ((uint8_t)0xD8U)
#define W25Q_CHIP_ERASE                     ((uint8_t)0xC7U)
#define W25Q_ERASE_PROGRAM_SUSPEND          ((uint8_t)0x75U)
#define W25Q_ERASE_PROGRAM_RESUME           ((uint8_t)0x7AU)
#define W25Q_POWER_DOWN                     ((uint8_t)0xB9U)
#define W25Q_READ_DATA                      ((uint8_t)0x03U)
#define W25Q_FAST_READ                      ((uint8_t)0x0BU)
#define W25Q_DEVICE_ID                      ((uint8_t)0xABU)
#define W25Q_RELEASE_POWER_DOWN             (W25Q_DEVICE_ID)
#define W25Q_MANUFACTURER_DEVICE_ID         ((uint8_t)0x90U)
#define W25Q_JEDEC_ID                       ((uint8_t)0x9FU)
#define W25Q_READ_UNIQUE_ID                 ((uint8_t)0x4BU)
#define W25Q_READ_SFDP_REG                  ((uint8_t)0x5AU)
#define W25Q_REASE_SECURITY_REG             ((uint8_t)0x44U)
#define W25Q_PROGRAM_SECURITY_REG           ((uint8_t)0x42U)
#define W25Q_READ_SECURITY_REG              ((uint8_t)0x48U)
#define W25Q_ENABLE_QPI                     ((uint8_t)0x38U)
#define W25Q_ENABLE_RESET                   ((uint8_t)0x66U)
#define W25Q_RESET                          ((uint8_t)0x99U)
/**
 * @}
 */

#define W25Q_SIZE_1K                (1024U)                 /*!< 1KB */
#define W25Q_SIZE_PAGE              (256U)                  /*!< 256B/page */
#define W25Q_SIZE_SECTOR            (W25Q_SIZE_1K * 4U)     /*!< 4KB/sector */
#define W25Q_SIZE_BLOCK             (W25Q_SIZE_1K * 64U)    /*!< 64KB/block */

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
 * @addtogroup W25QXX_Global_Functions W25QXX Global Functions
 * @{
 */
void W25QXX_Init(stc_w25qxx_t *pstcW25qxx);
uint16_t W25QXX_ReadManDeviceId(void);
void W25QXX_ReadUniqueId(uint8_t *pu8UniqueId);

uint16_t W25QXX_ReadStatus(void);
void W25QXX_WriteStatus(uint16_t u16Status);
void W25QXX_PowerDown(void);
void W25QXX_ReleasePowerDown(void);

void W25QXX_EraseChip(void);
void W25QXX_EraseSector(uint32_t u32SectorAddress);
void W25QXX_EraseBlock(uint32_t u32BlockAddress);

void W25QXX_WriteData(uint32_t u32Address, const uint8_t *pu8WriteBuf, uint32_t u32NumByteToWrite);
void W25QXX_ReadData(uint32_t u32Address, uint8_t *pu8ReadBuf, uint32_t u32NumByteToRead);
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

#ifdef __cplusplus
}
#endif

#endif /* __W25QXX_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
