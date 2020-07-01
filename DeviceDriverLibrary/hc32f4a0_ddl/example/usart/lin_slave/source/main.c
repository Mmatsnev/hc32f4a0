/**
 *******************************************************************************
 * @file  usart/lin_slave/source/main.c
 * @brief This example demonstrates LIN data receive and transfer.
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
#include "hc32_ddl.h"
#include "lin.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USART_LIN_Slave
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* USART Channel && RX/TX Port/Pin definition */
#define LIN_CH                          (M4_USART10)

#define LIN_RX_PORT                     (GPIO_PORT_I)   /* PI1: USART10_RX */
#define LIN_RX_PIN                      (GPIO_PIN_11)
#define LIN_RX_GPIO_FUNC                (GPIO_FUNC_39_USART10_RX)

#define LIN_TX_PORT                     (GPIO_PORT_I)   /* PI09: USART10_TX */
#define LIN_TX_PIN                      (GPIO_PIN_09)
#define LIN_TX_GPIO_FUNC                (GPIO_FUNC_38_USART10_TX)

/* LIN sleep port/pin definition */
#define LIN_SLEEP_PORT                  (EIO_PORT0)
#define LIN_SLEEP_PIN                   (EIO_PIN5)

/* Interrupt number definition */
#define LIN_RX_IRQn                     (Int000_IRQn)
#define LIN_RXERR_IRQn                  (Int001_IRQn)
#define LIN_BREAKWKUP_IRQn              (Int002_IRQn)

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
 * @brief  Main function of LIN slave project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    en_result_t enRet;
    static stc_lin_frame_t stcFrame;
    static stc_lin_hanlde_t stcLinHandle = {
        .USARTx = LIN_CH,
        .u32LinMode = LIN_SLAVE,
        .stcLinInit = {
            .u32Baudrate = 8000UL,
            .u32ClkMode = USART_INTERNCLK_NONE_OUTPUT,
            .u32PclkDiv = USART_PCLK_DIV16,
            .u32BmcPclkDiv = USART_LIN_BMC_PCLK_DIV8,
            .u32OversamplingBits = USART_OVERSAMPLING_8BIT,
            .u32DetectBreakLen = USART_LIN_DETECT_BREAK_10BIT,
        },
        .stcPinCfg = {
            .u8RxPort = LIN_RX_PORT,
            .u16RxPin = LIN_RX_PIN,
            .u8RxPinFunc = LIN_RX_GPIO_FUNC,
            .u8TxPort = LIN_TX_PORT,
            .u16TxPin = LIN_TX_PIN,
            .u8TxPinFunc = LIN_TX_GPIO_FUNC,
        },
        .stcIrqnCfg = {
            .RxIntIRQn = LIN_RX_IRQn,
            .RxErrIntIRQn = LIN_RXERR_IRQn,
            .BreakWkupIntIRQn = LIN_BREAKWKUP_IRQn,
        },
        .enLinState = LinStateSleep,
    };

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO. */
    BSP_IO_Init();

    /* Initialize LED. */
    BSP_LED_Init();

    /* Configure LIN transceiver chip sleep pin. */
    BSP_IO_WritePortPin(LIN_SLEEP_PORT, LIN_SLEEP_PIN, 1U);
    BSP_IO_ConfigPortPin(LIN_SLEEP_PORT, LIN_SLEEP_PIN, EIO_DIR_OUT);

    /* Initialize LIN slave function. */
    LIN_Init(&stcLinHandle);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    while (1)
    {
        /* Wait wakeup. */
        while (LinStateSleep == LIN_GetState(&stcLinHandle))
        {
        }

        /* Start LIN slave receive frame header function(blocking mode). */
        enRet = LIN_SLAVE_RecFrameHeader(&stcLinHandle, &stcFrame , LIN_REC_WAITING_FOREVER);
        if (Ok == enRet)
        {
            /* Start LIN slave receive frame data function. */
            enRet = LIN_SLAVE_RecFrameResponse(&stcLinHandle, LIN_REC_WAITING_FOREVER);
        }

        if (Ok == enRet)
        {
            /* Start LIN slave receive frame header function(blocking mode). */
            enRet = LIN_SLAVE_RecFrameHeader(&stcLinHandle, &stcFrame, LIN_REC_WAITING_FOREVER);
            if (Ok == enRet)
            {
                /* LIN slave send frame response. */
                enRet = LIN_SLAVE_SendFrameResponse(&stcLinHandle);
            }
        }

        if (Ok != enRet)
        {
            BSP_LED_On(LED_RED);
        }
        else
        {
            BSP_LED_Off(LED_RED);
        }

        /* Enter sleep. */
        LIN_Sleep(&stcLinHandle);
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
