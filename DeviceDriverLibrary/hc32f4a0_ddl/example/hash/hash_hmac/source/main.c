/**
 *******************************************************************************
 * @file  hash/hash_hmac/source/main.c
 * @brief Main program HASH for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb          First version
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
 * @addtogroup HASH_Hmac
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define HASH_IRQn           (Int010_IRQn)
#define HASH_SOURCE         (INT_HASH)
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
static uint8_t u8HashMsgDigest[32U];
static uint8_t u8ExpectDigest[32] = \
{0xca,0x33,0x45,0x1a,0xca,0x33,0xc8,0xca,0xef,0x9f,0x9d,0x1f,\
 0xb4,0x5e,0x92,0x57,0x04,0xe2,0xba,0xbf,0x4e,0x5d,0x7c,0xa2,\
 0x8b,0x8c,0x88,0x31,0x3d,0x5b,0x1b,0xea};

static char *c8SrcData = \
"abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
01234567890123456789";

static char *c8Key = "HDSC-HC32F4A0.hash/hash_hmac/source/main.c\
Main program HASH for the Device Driver Library";

static uint8_t u8Count = 0U;
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
    /* Unlock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    //PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Unlock SRAM register: WTCR */
    //SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    //EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OPT write protect registers */
    //EFM_OTP_WP_Unlock();
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
    /* Lock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    //PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Lock SRAM register: WTCR */
    //SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OPT write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    //EFM_Lock();
}

/**
 * @brief  HASH operations complete IRQ callback
 * @param  None
 * @retval None
 */
void Hash_IrqCallback(void)
{
    if(++u8Count == 2U)
    {
        /* Get the message digest result */
        HASH_GetResult(u8HashMsgDigest);
        u8Count = 0U;
        if ((uint8_t)memcmp(u8HashMsgDigest, u8ExpectDigest,sizeof(u8HashMsgDigest)) == 0U)
        {
            printf("message digest:\n");
            for (uint8_t i = 0U; i < sizeof(u8HashMsgDigest); i++)
            {
                printf("%.2x ", u8HashMsgDigest[i]);
            }
            printf("\n");
        }
        else
        {
            printf("Computation Errors\n");
        }
        /* Clear the CYC_END flag */
        HASH_ClearStatus(HASH_FLAG_CYC_END);
        /* Clear the HMAC_END flag */
        HASH_ClearStatus(HASH_FLAG_HMAC_END);
    }
    /* Clear the CYC_END flag */
    HASH_ClearStatus(HASH_FLAG_CYC_END);
}

/**
 * @brief  Main function of template project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_signin_config_t stcIrqRegCfg;
    /* Unlock peripherals or registers */
    Peripheral_WE();
    /* Enable HASH. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_HASH, Enable);

    /* Enable the All data operations complete interrupt */
    HASH_IntCmd(HASH_INT_ALL, Enable);

    /* Config UART for printing. Baud rate 115200. */
    DDL_PrintfInit();

    /* Register IRQ handler && configure NVIC. */
    stcIrqRegCfg.enIRQn = HASH_IRQn;
    stcIrqRegCfg.enIntSrc = HASH_SOURCE;
    stcIrqRegCfg.pfnCallback = &Hash_IrqCallback;
    INTC_IrqSignIn(&stcIrqRegCfg);
    NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIORITY_15);
    NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);

    /* Use HASH. */
    HMAC_Calculate((uint8_t *)c8SrcData, strlen((char *)c8SrcData),\
                   (uint8_t *)c8Key, strlen((char *)(c8Key)),      \
                    u8HashMsgDigest);
    /* Lock peripherals or registers */
    Peripheral_WP();
    while (1)
    {

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
