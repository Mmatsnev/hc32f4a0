/**
 *******************************************************************************
 * @file  exmc/exmc_dmc_sdram_is42s16400j7tli/source/main.c
 * @brief This example demonstrates SDRAM function.
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
 * @addtogroup EXMC_DMC_SDRAM_IS42S16400J7TLI
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DATA_BUFFER_LEN                         (0x8000UL)

/* Toggle LED when conition is true */
#define LED_TOGGLE(condition)                                                  \
do {                                                                           \
    if (condition)                                                             \
    {                                                                          \
        BSP_LED_Toggle(LED_BLUE);                                              \
    }                                                                          \
} while (0)                                                                    \

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
static uint32_t m_u32MemByteSize = 0UL;
static uint32_t m_u32MemHalfwordSize = 0UL;
static uint32_t m_u32MemWordSize = 0UL;

static uint8_t m_au8ReadData[DATA_BUFFER_LEN];
static uint8_t m_au8WriteData[DATA_BUFFER_LEN];

static uint16_t m_au16ReadData[DATA_BUFFER_LEN];
static uint16_t m_au16WriteData[DATA_BUFFER_LEN];

static uint32_t m_au32ReadData[DATA_BUFFER_LEN];
static uint32_t m_au32WriteData[DATA_BUFFER_LEN];

static uint32_t m_u32TestCnt = 0UL;
static uint32_t m_u32ByteTestErrorCnt = 0UL;
static uint32_t m_u32HalfwordTestErrorCnt = 0UL;
static uint32_t m_u32WordTestErrorCnt = 0UL;

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
 * @brief  Main function of EXMC SDRAM project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    uint32_t i;
    uint32_t j;
    uint32_t u32MemAddr;
    uint32_t u32ToggleCnt = 0UL;

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

    /* Initialize UART print */
    DDL_PrintfInit();

    /* Initialize LED */
    BSP_IO_Init();
    BSP_LED_Init();

    /* Initialize test data. */
    for (i = 0UL; i < DATA_BUFFER_LEN; i++)
    {
        m_au8ReadData[i] = 0U;
        m_au8WriteData[i] = 0x12U;

        m_au16ReadData[i] = 0U;
        m_au16WriteData[i] = 0x5678U;

        m_au32ReadData[i] = 0UL;
        m_au32WriteData[i] = 0xAABBCCDDUL;
    }

    /* Configure SRAM. */
    IS42S16400J7TLI_Init();
    IS42S16400J7TLI_GetMemInfo(&m_u32MemStartAddr, &m_u32MemByteSize);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    m_u32MemHalfwordSize = m_u32MemByteSize/2UL;
    m_u32MemWordSize = m_u32MemByteSize/4UL;

    printf("Memory start address: 0x%08lx \r\n", m_u32MemStartAddr);
    printf("Memory end   address: 0x%08lx \r\n", (m_u32MemStartAddr + m_u32MemByteSize - 1UL));
    printf("Memory size  (Bytes): 0x%08lx \r\n\r\n", m_u32MemByteSize);

    while (1)
    {
        m_u32TestCnt++;
        printf("******** Write/read test times: %ld ********\r\n", m_u32TestCnt);

        /****************** Byte read/write *****************/
        m_u32ByteTestErrorCnt = 0UL;
        u32MemAddr = m_u32MemStartAddr;
        for (i = 0UL; i < m_u32MemByteSize; i += DATA_BUFFER_LEN)
        {
            IS42S16400J7TLI_WriteMem8(u32MemAddr, m_au8WriteData, DATA_BUFFER_LEN);
            IS42S16400J7TLI_ReadMem8(u32MemAddr, m_au8ReadData, DATA_BUFFER_LEN);

            /* Verify write/read data. */
            for (j = 0UL; j < DATA_BUFFER_LEN; j++)
            {
                if (m_au8WriteData[j] != m_au8ReadData[j])
                {
                    m_u32ByteTestErrorCnt++;
                    printf("Byte read/write error: address = 0x%lx; write data = 0x%x; read data = 0x%x\r\n", 
                           (u32MemAddr+j), m_au8WriteData[j], m_au8ReadData[j]);
                }
            }

            u32MemAddr += (DATA_BUFFER_LEN * sizeof(m_au8ReadData[0]));
            memset(m_au8ReadData, 0, (DATA_BUFFER_LEN * sizeof(m_au8ReadData[0])));
            LED_TOGGLE(!(u32ToggleCnt++ % 5UL));
        }

        printf("     Byte read/write error data count: %ld \r\n", m_u32ByteTestErrorCnt);

        /**************** Halfword read/write ****************/
        m_u32HalfwordTestErrorCnt = 0UL;
        u32MemAddr = m_u32MemStartAddr;
        for (i = 0UL; i < m_u32MemHalfwordSize; i += DATA_BUFFER_LEN)
        {
            IS42S16400J7TLI_WriteMem16(u32MemAddr, m_au16WriteData, DATA_BUFFER_LEN);
            IS42S16400J7TLI_ReadMem16(u32MemAddr, m_au16ReadData, DATA_BUFFER_LEN);

            /* Verify write/read data. */
            for (j = 0UL; j < DATA_BUFFER_LEN; j++)
            {
                if (m_au16WriteData[j] != m_au16ReadData[j])
                {
                    m_u32HalfwordTestErrorCnt++;
                    printf("Halfword read/write error: address = 0x%lx; write data = 0x%x; read data = 0x%x\r\n", 
                           (u32MemAddr+j), m_au16WriteData[j], m_au16ReadData[j]);
                }
            }

            u32MemAddr += (DATA_BUFFER_LEN * sizeof(m_au16ReadData[0]));
            memset(m_au16ReadData, 0, (DATA_BUFFER_LEN * sizeof(m_au16ReadData[0])));
            LED_TOGGLE(!(u32ToggleCnt++ % 5UL));
        }

        printf(" Halfword read/write error data count: %ld \r\n", m_u32HalfwordTestErrorCnt);

        /****************** Word read/write ******************/
        m_u32WordTestErrorCnt = 0UL;
        u32MemAddr = m_u32MemStartAddr;
        for (i = 0UL; i < m_u32MemWordSize; i += DATA_BUFFER_LEN)
        {
            IS42S16400J7TLI_WriteMem32(u32MemAddr, m_au32WriteData, DATA_BUFFER_LEN);
            IS42S16400J7TLI_ReadMem32(u32MemAddr, m_au32ReadData, DATA_BUFFER_LEN);

            /* Verify write/read data. */
            for (j = 0UL; j < DATA_BUFFER_LEN; j++)
            {
                if (m_au32WriteData[j] != m_au32ReadData[j])
                {
                    m_u32WordTestErrorCnt++;
                    printf("Word read/write error: address = 0x%lx; write data = 0x%lx; read data = 0x%lx\r\n", 
                           (u32MemAddr+j), m_au32WriteData[j], m_au32ReadData[j]);
                }
            }

            u32MemAddr += (DATA_BUFFER_LEN * sizeof(m_au32ReadData[0]));
            memset(m_au32ReadData, 0, (DATA_BUFFER_LEN * sizeof(m_au32ReadData[0])));
            LED_TOGGLE(!(u32ToggleCnt++ % 5UL));
        }

        printf("     Word read/write error data count: %ld \r\n\r\n", m_u32WordTestErrorCnt);

        /****************** Error check ******************/
        if (m_u32ByteTestErrorCnt || m_u32HalfwordTestErrorCnt || m_u32WordTestErrorCnt)
        {
            BSP_LED_On(LED_RED);
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
