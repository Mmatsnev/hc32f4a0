/**
 *******************************************************************************
 * @file  exmc/exmc_smc_nor_s29gl064n90tfi03/source/main.c
 * @brief This example demonstrates SMC NOR Flash function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
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
#include <string.h>
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup EXMC_SMC_NOR_S29GL064N90TFI03
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DATA_BUFFER_LEN                         (16UL)
#define S29GL064_BUF_PROGRAM_HALFWORD_SIZE      (m_u32BytesPerBufProgram >> 1UL)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint32_t m_u32MemStartAddr = 0UL;
static uint32_t m_u32MemSectors = 0UL;
static uint32_t m_u32MemBytePerSector = 0UL;
static uint32_t m_u32BytesPerBufProgram = 0UL;

static uint16_t m_au16ReadData[DATA_BUFFER_LEN];
static uint16_t m_au16WriteData[DATA_BUFFER_LEN];

static uint16_t m_au16Id[4];
static __IO uint8_t m_u8TestError __UNUSED = 1U;

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
 * @brief  Main function of EXMC SMC NOR Flash project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t i;

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock: */
    BSP_CLK_Init();

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EXCLK 60MHz */
    CLK_ClkDiv(CLK_CATE_ALL, (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                              CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | \
                              CLK_PCLK4_DIV2 | CLK_EXCLK_DIV4 | CLK_HCLK_DIV1));

    /* Initialize test data. */
    for (i = 0UL; i < DATA_BUFFER_LEN; i++)
    {
        m_au16ReadData[i] = 0U;
        m_au16WriteData[i] = 0x1234U;
    }

    /* Configure NOR Flash: S29GL064. */
    S29GL064_Init();
    S29GL064_GetMemInfo(&m_u32MemStartAddr,
                        &m_u32MemSectors,
                        &m_u32MemBytePerSector,
                        &m_u32BytesPerBufProgram);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Read NOR Flash device ID */
    S29GL064_ReadId(m_au16Id, 4UL);
    if ((S29GL064N90TFI03_MANUFACTURER_ID == m_au16Id[0]) || \
        (S29GL064N90TFI03_DEVICE_ID1 == m_au16Id[1]) || \
        (S29GL064N90TFI03_DEVICE_ID2 == m_au16Id[2]) || \
        (S29GL064N90TFI03_DEVICE_ID3 == m_au16Id[3]))
    {
        S29GL064_EraseSector(m_u32MemStartAddr);
        m_au16ReadData[0] = S29GL064_Read(m_u32MemStartAddr);
        if (0xFFFFU == m_au16ReadData[0])
        {
            S29GL064_ProgramBuffer(m_u32MemStartAddr, m_au16WriteData, S29GL064_BUF_PROGRAM_HALFWORD_SIZE);
            S29GL064_ReadBuffer(m_u32MemStartAddr, m_au16ReadData, S29GL064_BUF_PROGRAM_HALFWORD_SIZE);

            if (0 == memcmp (m_au16ReadData, m_au16WriteData, m_u32BytesPerBufProgram))
            {
                memset (m_au16ReadData, 0, m_u32BytesPerBufProgram);

                S29GL064_ProgramBuffer(m_u32MemStartAddr + m_u32BytesPerBufProgram, m_au16WriteData, S29GL064_BUF_PROGRAM_HALFWORD_SIZE);
                S29GL064_ReadBuffer(m_u32MemStartAddr + m_u32BytesPerBufProgram, m_au16ReadData, S29GL064_BUF_PROGRAM_HALFWORD_SIZE);

                if (0 == memcmp (m_au16ReadData, m_au16WriteData, m_u32BytesPerBufProgram))
                {
                    m_u8TestError = 0U;
                }
            }
        }
    }

    while (1)
    {}
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
