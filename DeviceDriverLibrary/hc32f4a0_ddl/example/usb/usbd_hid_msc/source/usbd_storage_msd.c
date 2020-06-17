/**
 *******************************************************************************
 * @file  usb/usbd_hid_msc/source/usbd_storage_msd.c
 * @brief This file includes the user MSC application layer.
 *   
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
#include "usbd_storage_msd.h"
#include "usbd_msc_mem.h"
#include "usb_conf.h"
#include "w25qxx.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USBD_HID_MSC
 * @{
 */


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/* User callback functions */
USBD_STORAGE_cb_TypeDef USBD_MEM_SPI_fops =
{
    &STORAGE_Init,
    &STORAGE_GetCapacity,
    &STORAGE_IsReady,
    &STORAGE_IsWriteProtected,
    &STORAGE_Read,
    &STORAGE_Write,
    &STORAGE_GetMaxLun,
    (int8_t *)STORAGE_Inquirydata,
};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Max. supported device number */
#define STORAGE_LUN_NBR         2U

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* Variable for Storage operation status */
volatile uint8_t USB_STATUS_REG = 0U;

/* USB Mass storage querty data (36 bytes for each lun) */
const int8_t STORAGE_Inquirydata[] =
{
    /* LUN 0 */
    0x00U,
    0x80U,
    0x02U,
    0x02U,
    (USBD_STD_INQUIRY_LENGTH - 4U),
    0x00U,
    0x00U,
    0x00U,
    /* Vendor Identification */
    'H', 'D', 'S', 'C', ' ', 'M', 'C', 'U', ' ',    //9 bytes
    /* Product Identification */
    'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',    //15 bytes
    ' ', 'D', 'i', 's', 'k', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                             //4 bytes

    /* LUN 1 */
    0x00U,
    0x80U,
    0x02U,
    0x02U,
    (USBD_STD_INQUIRY_LENGTH - 4U),
    0x00U,
    0x00U,
    0x00U,
    /* Vendor Identification */
    'H', 'D', 'S', 'C', ' ', 'M', 'C', 'U', ' ',    //9 bytes
    /* Product Identification */
    'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',    //15 bytes
    'D', 'i', 's', 'k', ' ', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                             //4 bytes
};
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/* Pointer to USBD_MEM_SPI_fops */
USBD_STORAGE_cb_TypeDef *USBD_STORAGE_fops = &USBD_MEM_SPI_fops;
stc_w25qxx_t stcW25qxx;
/**
 * @brief Storage initialize
 * @param lun: logic number
 *   \@arg: 0, SD card
 *   \@arg: 1, SPI flash
 * @retval Ok
 */
int8_t STORAGE_Init(uint8_t lun)
{
    W25QXX_Init(&stcW25qxx);
    return Ok;
}

/**
 * @brief Get Storage capacity
 * @param lun: logic number
 *   \@arg: 0, SD card
 *   \@arg: 1, SPI flash
 * @param block_num: sector number
 * @param block_size: sector size
 * @retval Ok
 */
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
    if (lun == 1U)
    {
        *block_size = 512U;
//        *block_num  = SDCardInfo.CardCapacity / 512;
    }else
    {
        *block_size = 512U;
        *block_num  = stcW25qxx.u32CapacityInBytes / 512U;
    }
    return Ok;
}

/**
 * @brief Check storage if ready
 * @param lun: logic number
 *   \@arg: 0, SD card
 *   \@arg: 1, SPI flash
 * @retval Ok
 */
int8_t  STORAGE_IsReady(uint8_t lun)
{
    USB_STATUS_REG |= (uint8_t)0X10;
    return Ok;
}

/**
 * @brief Check storage if write protected
 * @param lun: logic number
 *   \@arg: 0, SD card
 *   \@arg: 1, SPI flash
 * @retval Ok
 *
 */
int8_t  STORAGE_IsWriteProtected(uint8_t lun)
{
    return Ok;
}

/**
 * @brief Read data from storage devices
 *
 * @param [in] lun: logic number
 *   \@arg: 0, SD card
 *   \@arg: 1, SPI flash
 * @param [in] blk_addr: sector address
 * @param [in] blk_len: sector count
 * @param [out] buf: data buffer be read
 *
 * @retval Ok: read successful
 *         Other: read fail
 */
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t res = (int8_t)0;
    USB_STATUS_REG |= 0X02U;
    if (lun == 1U)
    {
//        res = SD_ReadDisk(buf, blk_addr, blk_len);
        if (res)
        {
            USB_STATUS_REG |= 0X08U;
        }
    }else
    {
        W25QXX_ReadData(blk_addr * 512U, buf, blk_len * 512U);
    }
    return res;
}

/**
 * @brief Write data to storage devices
 * @param [in] lun: logic number
 *   \@arg: 0, SD card
 *   \@arg: 1, SPI flash
 * @param [in] blk_addr: sector address
 * @param [in] blk_len: sector count
 * @param [out] buf: data buffer be written
 *
 * @retval Ok: write successful
 *         Other: write fail
 */
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t res = (int8_t)0;
    USB_STATUS_REG |= 0X01U;
    if (lun == 1U)
    {
//        res = SD_WriteDisk(buf, blk_addr, blk_len);
        if (res)
        {
            USB_STATUS_REG |= 0X04U;
        }
    }else
    {
        W25QXX_WriteData(blk_addr * 512U, buf, blk_len * 512U);
    }
    return res;
}

/**
 * @brief Get supported logic number
 * @param None
 * @retval STORAGE_LUN_NBR-1
 */
int8_t STORAGE_GetMaxLun(void)
{
    return (int8_t)(STORAGE_LUN_NBR - 1U);
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
