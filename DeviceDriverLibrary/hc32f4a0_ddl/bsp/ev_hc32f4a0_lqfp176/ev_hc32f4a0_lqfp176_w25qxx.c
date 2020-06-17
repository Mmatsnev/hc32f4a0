/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_w25qxx.c
 * @brief This file provides firmware functions for QSPI/SPI NOR W25QXX.
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
#include "ev_hc32f4a0_lqfp176_w25qxx.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */

/** @defgroup EV_HC32F4A0_LQFP176_W25QXX HC32F4A0 EVB W25QXX
  * @{
  */

#if ((BSP_W25QXX_ENABLE == BSP_ON) && \
     (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

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
 * @defgroup W25QXX_Global_Functions W25QXX Global Functions
 * @{
 */

/**
 * @brief  Initializes SPI for W25QXX.
 * @param  None
 * @retval None
 */
void BSP_W25Q_SPI_Init(void)
{
    stc_gpio_init_t stcGpioCfg;
    stc_spi_init_t stcSpiInit;
    stc_spi_delay_t stcSpiDelayCfg;

    /* Clear initialize structure */
    SPI_StructInit(&stcSpiInit);
    SPI_DelayStructInit(&stcSpiDelayCfg);

    /* Configure peripheral clock */
    PWC_Fcg1PeriphClockCmd(W25Q_SPI_UNIT_CLOCK, Enable);

    /* Port configurate */
    GPIO_StructInit(&stcGpioCfg);

    /* High driving capacity for output pin. */
    stcGpioCfg.u16PinDir = PIN_DIR_OUT;
    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
    stcGpioCfg.u16PinState = PIN_STATE_SET;
    GPIO_Init(W25Q_CS_PORT,  W25Q_CS_PIN, &stcGpioCfg);

    GPIO_StructInit(&stcGpioCfg);
    stcGpioCfg.u16PinDrv = PIN_DRV_HIGH;
    GPIO_Init(W25Q_SCK_PORT,  W25Q_SCK_PIN, &stcGpioCfg);
    GPIO_Init(W25Q_MOSI_PORT, W25Q_MOSI_PIN, &stcGpioCfg);

    /* CMOS input for input pin */
    stcGpioCfg.u16PinDrv = PIN_DRV_LOW;
    stcGpioCfg.u16PinIType = PIN_ITYPE_CMOS;
    GPIO_Init(W25Q_MISO_PORT, W25Q_MISO_PIN, &stcGpioCfg);

    /* Configure SPI Port function for master */
    GPIO_SetFunc(W25Q_SCK_PORT,  W25Q_SCK_PIN,  W25Q_SCK_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(W25Q_MOSI_PORT, W25Q_MOSI_PIN, W25Q_MOSI_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(W25Q_MISO_PORT, W25Q_MISO_PIN, W25Q_MISO_FUNC, PIN_SUBFUNC_DISABLE);

    /* SPI De-initialize */
    SPI_DeInit(W25Q_SPI_UNIT);

    /* Configuration SPI structure */
    stcSpiInit.u32WireMode          = SPI_WIRE_3;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32SuspMode          = SPI_COM_SUSP_FUNC_OFF;
    stcSpiInit.u32Modfe             = SPI_MODFE_DISABLE;
    stcSpiInit.u32Parity            = SPI_PARITY_INVALID;
    stcSpiInit.u32SpiMode           = SPI_MODE_3;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_PCLK1_DIV2;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    SPI_Init(W25Q_SPI_UNIT, &stcSpiInit);

    stcSpiDelayCfg.u32IntervalDelay = SPI_INTERVAL_TIME_1SCK_2PCLK1;
    stcSpiDelayCfg.u32ReleaseDelay = SPI_RELEASE_TIME_1SCK;
    stcSpiDelayCfg.u32SetupDelay = SPI_SETUP_TIME_1SCK;
    SPI_DelayTimeCfg(W25Q_SPI_UNIT, &stcSpiDelayCfg);

    SPI_FunctionCmd(W25Q_SPI_UNIT, Enable);
}

/**
 * @brief  BSP W25QXX SPI transmit and receive data.
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 *                                  If this pointer is NULL and the pvRxBuf is NOT NULL, the MOSI output high
 *                                  and the the received data will be stored in the buffer pointed by pvRxBuf.
 * @param  [in]  u32TxLength        The length of the data(in byte or half word) to be sent and received.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorTimeout:            SPI transmit and receive timeout.
 *   @arg  ErrorInvalidParameter:   pvRxBuf == NULL or pvRxBuf == NULL or u32Length == 0U
 * @note   SPI receives data while sending data. Only works in full duplex master mode.
 */
en_result_t BSP_W25Q_SPI_Transmit(const void *pvTxBuf, uint32_t u32TxLength)
{
    return SPI_Transmit(W25Q_SPI_UNIT, pvTxBuf, u32TxLength);
}

/**
 * @brief  BSP W25QXX SPI receive data.
 * @param  [in]  pvRxBuf            The pointer to the buffer which the received data to be stored.
 * @param  [in]  u32RxLength        The length of the data to be received.
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                      No errors occurred
 *   @arg  ErrorTimeout:            SPI receive timeout.
 *   @arg  ErrorInvalidParameter:   pvRxBuf == NULL or u32RxLength == 0U
 * @note   -No NSS pin active and inactive operation in 3-wire mode. Add operations of NSS pin depending on your application.
 *         -This function only works in full duplex master mode.
 */
en_result_t BSP_W25Q_SPI_Receive(void *pvRxBuf, uint32_t u32RxLength)
{
    return SPI_Receive(W25Q_SPI_UNIT, pvRxBuf, u32RxLength);
}

/**
 * @}
 */

/**
 * @addtogroup W25QXX_Local_Functions W25QXX Local Functions
 * @{
 */

/**
 * @}
 */

#endif /* BSP_W25QXX_ENABLE && (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0) */

/**
 * @}
 */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
