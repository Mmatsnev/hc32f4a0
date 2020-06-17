/**
 *******************************************************************************
 * @file  qspi/qspi_base/source/w25q64.h
 * @brief Headfile for qspi example.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12        Wuze         First version
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
#ifndef __W25Q64_H__
#define __W25Q64_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup QSPI_BASE
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup W25Q64_Standard_SPI_Instructions W25Q64 Standard SPI Instructions
 * @{
 */
#define W25Q64_WRITE_ENABLE                             (0x06U)
#define W25Q64_VOLATILE_SR_WRITE_ENABLE                 (0x50U)
#define W25Q64_WRITE_DISABLE                            (0x04U)
#define W25Q64_RELEASE_POWER_DOWN_ID                    (0xABU)
#define W25Q64_MANUFACTURER_DEVICE_ID                   (0x90U)
#define W25Q64_JEDEC_ID                                 (0x9FU)
#define W25Q64_READ_UNIQUE_ID                           (0x4BU)
#define W25Q64_READ_DATA                                (0x03U)
#define W25Q64_FAST_READ                                (0x0BU)
#define W25Q64_PAGE_PROGRAM                             (0x02U)
#define W25Q64_SECTOR_ERASE                             (0x20U)
#define W25Q64_BLOCK_ERASE_32KB                         (0x52U)
#define W25Q64_BLOCK_ERASE_64KB                         (0xD8U)
#define W25Q64_CHIP_ERASE                               (0xC7U)
#define W25Q64_READ_STATUS_REGISTER_1                   (0x05U)
#define W25Q64_WRITE_STATUS_REGISTER_1                  (0x01U)
#define W25Q64_READ_STATUS_REGISTER_2                   (0x35U)
#define W25Q64_WRITE_STATUS_REGISTER_2                  (0x31U)
#define W25Q64_READ_STATUS_REGISTER_3                   (0x15U)
#define W25Q64_WRITE_STATUS_REGISTER_3                  (0x11U)
#define W25Q64_READ_SFDP_REGISTER                       (0x5AU)
#define W25Q64_ERASE_SECURITY_REGISTER                  (0x44U)
#define W25Q64_PROGRAM_SECURITY_REGISTER                (0x42U)
#define W25Q64_READ_SECURITY_REGISTER                   (0x48U)
#define W25Q64_GLOBAL_BLOCK_LOCK                        (0x7EU)
#define W25Q64_GLOBAL_BLOCK_UNLOCK                      (0x98U)
#define W25Q64_READ_BLOCK_LOCK                          (0x3DU)
#define W25Q64_INDIVIDUAL_BLOCK_LOCK                    (0x36U)
#define W25Q64_INDIVIDUAL_BLOCK_UNLOCK                  (0x39U)
#define W25Q64_ERASE_PROGRAM_SUSPEND                    (0x75U)
#define W25Q64_ERASE_PROGRAM_RESUME                     (0x7AU)
#define W25Q64_POWER_DOWN                               (0xB9U)
#define W25Q64_ENABLE_RESET                             (0x66U)
#define W25Q64_RESET_DEVICE                             (0x99U)
/**
 * @}
 */

/**
 * @defgroup W25Q64_Dual_Quad_SPI_Instruction W25Q64 Dual Quad SPI Instruction
 * @{
 */
#define W25Q64_FAST_READ_DUAL_OUTPUT                    (0x3BU)
#define W25Q64_FAST_READ_DUAL_IO                        (0xBBU)
#define W25Q64_MFTR_DEVICE_ID_DUAL_IO                   (0x92U)
#define W25Q64_QUAD_INPUT_PAGE_PROGRAM                  (0x32U)
#define W25Q64_FAST_READ_QUAD_OUTPUT                    (0x6BU)
#define W25Q64_MFTR_DEVICE_ID_QUAD_IO                   (0x94U)
#define W25Q64_FAST_READ_QUAD_IO                        (0xEBU)
#define W25Q64_SET_BURST_WITH_WRAP                      (0x77U)
/**
 * @}
 */

/**
 * @defgroup W25Q64_Size W25Q64 Size
 * @{
 */
#define W25Q64_PAGE_SIZE                                (256UL)
#define W25Q64_SECTOR_SIZE                              (1024UL * 4UL)
#define W25Q64_BLOCK_SIZE                               (1024UL * 64UL)
#define W25Q64_PAGE_PER_SECTOR                          (W25Q64_SECTOR_SIZE / W25Q64_PAGE_SIZE)
/**
 * @}
 */

/**
 * @defgroup W25Q64_Status_Flag W25Q64 Status Flag
 * @{
 */
#define W25Q64_FLAG_BUSY                                (0x01U)
#define W25Q64_FLAG_SUSPEND                             (0x80U)
/**
 * @}
 */

/**
 * @defgroup W25Q64_Miscellaneous_Macros W25Q64 Miscellaneous Macros
 * @{
 */
#define W25Q64_UNIQUE_ID_SIZE                           (8U)
#define W25Q64_DUMMY_BYTE_VALUE                         (0xFFU)
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
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __W25Q64_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
