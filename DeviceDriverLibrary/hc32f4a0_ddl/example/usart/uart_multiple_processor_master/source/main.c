/**
 *******************************************************************************
 * @file  usart/uart_multiple_processor_master/source/main.c
 * @brief This example demonstrates UART multi-processor receive and transfer.
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
#include <stdio.h>

#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USART_UART_Multiple_Processor_Master
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/**
 * @brief  Ring buffer structure definition
 */
typedef struct
{
    uint16_t u16Capacity;
    __IO uint16_t u16UsedSize;
    uint16_t u16InIdx;
    uint16_t u16OutIdx;
    uint8_t  au8Buf[50];
} stc_ring_buffer_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Key port&pin definition */
#define KEY_PORT                        (GPIO_PORT_A)
#define KEY_PIN                         (GPIO_PIN_00)

/* UART RX/TX Port/Pin definition */
#define USART_RX_PORT                   (GPIO_PORT_H)   /* PH6: USART6_RX */
#define USART_RX_PIN                    (GPIO_PIN_06)
#define USART_RX_GPIO_FUNC              (GPIO_FUNC_37_USART6_RX)

#define USART_TX_PORT                   (GPIO_PORT_E)   /* PE6: USART6_TX */
#define USART_TX_PIN                    (GPIO_PIN_06)
#define USART_TX_GPIO_FUNC              (GPIO_FUNC_36_USART6_TX)

/* UART unit definition */
#define USART_UNIT                      (M4_USART6)
#define USART_FUNCTION_CLK_GATE         (PWC_FCG3_USART6)

/* UART unit interrupt definition */
#define USART_UNIT_ERR_INT_SRC          (INT_USART6_EI)
#define USART_UNIT_ERR_INT_IRQn         (Int000_IRQn)

#define USART_UNIT_RX_INT_SRC           (INT_USART6_RI)
#define USART_UNIT_RX_INT_IRQn          (Int001_IRQn)

#define USART_UNIT_TX_INT_SRC           (INT_USART6_TI)
#define USART_UNIT_TX_INT_IRQn          (Int002_IRQn)

#define USART_UNIT_TCI_INT_SRC          (INT_USART6_TCI)
#define USART_UNIT_TCI_INT_IRQn         (Int003_IRQn)

/* UART multiple processor ID definition */
#define UART_MASTER_STATION_ID          (0x20U)
#define UART_SLAVE_STATION_ID           (0x21U)

/* Ring buffer size */
#define IS_RING_BUFFER_EMPYT(x)         (0U == ((x)->u16UsedSize))

/* Multi-processor silence mode */
#define USART_UART_NORMAL_MODE          (0U)
#define USART_UART_SILENCE_MODE         (1U)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static en_flag_status_t KeyState(void);
static void USART_TxEmpty_IrqCallback(void);
static void USART_TxComplete_IrqCallback(void);
static void USART_Rx_IrqCallback(void);
static void USART_RxErr_IrqCallback(void);
static en_result_t RingBufWrite(stc_ring_buffer_t *pstcBuffer, uint8_t u8Data);
static en_result_t RingBufRead(stc_ring_buffer_t *pstcBuffer, uint8_t *pu8Data);
static void UsartSetSilenceMode(uint8_t u8Mode);
static uint8_t UsartGetSilenceMode(void);
static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                    uint32_t u32Priority);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8UartSilenceMode = USART_UART_NORMAL_MODE;

static stc_ring_buffer_t m_stcRingBuf = {
    .u16InIdx = 0U,
    .u16OutIdx = 0U,
    .u16UsedSize = 0U,
    .u16Capacity = sizeof (m_stcRingBuf.au8Buf),
};

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
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
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
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy */
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
 * @brief  Get key state
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Set: Released after key is pressed
 *           - Reset: Key isn't pressed
 */
static en_flag_status_t KeyState(void)
{
    en_flag_status_t enKeyState = Reset;

    if (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
    {
        DDL_DelayMS(50UL);

        if (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
        {
            while (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
            {
            }
            enKeyState = Set;
        }
    }

    return enKeyState;
}

/**
 * @brief  UART TX Empty IRQ callback.
 * @param  None
 * @retval None
 */
static void USART_TxEmpty_IrqCallback(void)
{
    uint8_t u8Data = 0U;
    en_flag_status_t enFlag = USART_GetFlag(USART_UNIT, USART_FLAG_TXE);
    en_functional_state_t enState = USART_GetFuncState(USART_UNIT, USART_INT_TXE);

    if ((Set == enFlag) && (Enable == enState))
    {
        USART_SendId(USART_UNIT, UART_SLAVE_STATION_ID);

        while (Reset == USART_GetFlag(USART_UNIT, USART_FLAG_TC))   /* Wait Tx data register empty */
        {
        }

        if (Ok == RingBufRead(&m_stcRingBuf, &u8Data))
        {
            USART_SendData(USART_UNIT, (uint16_t)u8Data);
        }

        if (IS_RING_BUFFER_EMPYT(&m_stcRingBuf))
        {
            USART_FuncCmd(USART_UNIT, USART_INT_TXE, Disable);
            USART_FuncCmd(USART_UNIT, USART_INT_TC, Enable);
        }
    }
}

/**
 * @brief  UART TX Complete IRQ callback.
 * @param  None
 * @retval None
 */
static void USART_TxComplete_IrqCallback(void)
{
    en_flag_status_t enFlag = USART_GetFlag(USART_UNIT, USART_FLAG_TC);
    en_functional_state_t enState = USART_GetFuncState(USART_UNIT, USART_INT_TC);

    if ((Set == enFlag) && (Enable == enState))
    {
        /* Disable TX function */
        USART_FuncCmd(USART_UNIT, (USART_TX | USART_RX | USART_INT_TC), Disable);

        /* Enable RX function */
        USART_FuncCmd(USART_UNIT, (USART_RX | USART_INT_RX), Enable);
    }
}

/**
 * @brief  UART RX IRQ callback.
 * @param  None
 * @retval None
 */
static void USART_Rx_IrqCallback(void)
{
    uint8_t u8RxData = 0U;
    en_flag_status_t enFlag = USART_GetFlag(USART_UNIT, USART_FLAG_RXNE);
    en_functional_state_t enState = USART_GetFuncState(USART_UNIT, USART_INT_RX);

    if ((Set == enFlag) && (Enable == enState))
    {
        u8RxData = (uint8_t)USART_RecData(USART_UNIT);

        if ((Reset == USART_GetFlag(USART_UNIT, USART_FLAG_MPB)) &&
            (USART_UART_NORMAL_MODE == UsartGetSilenceMode()))
        {
            RingBufWrite(&m_stcRingBuf, u8RxData);
        }
        else
        {
            if (UART_MASTER_STATION_ID != u8RxData)
            {
                USART_SilenceCmd(USART_UNIT, Enable);
                UsartSetSilenceMode(USART_UART_SILENCE_MODE);
            }
            else
            {
                UsartSetSilenceMode(USART_UART_NORMAL_MODE);
            }
        }
    }
}

/**
 * @brief  UART RX Error IRQ callback.
 * @param  None
 * @retval None
 */
static void USART_RxErr_IrqCallback(void)
{
    USART_ClearFlag(USART_UNIT, (USART_CLEAR_FLAG_FE | USART_CLEAR_FLAG_PE | USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  Write ring buffer.
 * @param  [in] pstcBuffer              Pointer to a @ref stc_ring_buffer_t structure
 * @param  [in] u8Data                  Data to write
 * @retval An en_result_t enumeration value:
 *           - Ok: Write success.
 *           - ErrorBufferFull: Buffer is full.
 */
static en_result_t RingBufWrite(stc_ring_buffer_t *pstcBuffer, uint8_t u8Data)
{
    en_result_t enRet = Ok;

    if (pstcBuffer->u16UsedSize >= pstcBuffer->u16Capacity)
    {
        enRet = ErrorBufferFull;
    }
    else
    {
        pstcBuffer->au8Buf[pstcBuffer->u16InIdx++] = u8Data;
        pstcBuffer->u16InIdx %= pstcBuffer->u16Capacity;
        pstcBuffer->u16UsedSize++;
    }

    return enRet;
}

/**
 * @brief  Write ring buffer.
 * @param  [in] pstcBuffer              Pointer to a @ref stc_ring_buffer_t structure
 * @param  [in] pu8Data                 Pointer to data buffer to read
 * @retval An en_result_t enumeration value:
 *           - Ok: Write success.
 *           - ErrorNotReady: Buffer is empty.
 */
static en_result_t RingBufRead(stc_ring_buffer_t *pstcBuffer, uint8_t *pu8Data)
{
    en_result_t enRet = Ok;

    if (!pstcBuffer->u16UsedSize)
    {
        enRet = ErrorNotReady;
    }
    else
    {
        *pu8Data = pstcBuffer->au8Buf[pstcBuffer->u16OutIdx++];
        pstcBuffer->u16OutIdx %= pstcBuffer->u16Capacity;
        pstcBuffer->u16UsedSize--;
    }

    return enRet;
}

/**
 * @brief  Set silence mode.
 * @param  [in] u8Mode                      Silence mode
 *         This parameter can be one of the following values:
 *           @arg USART_UART_SILENCE_MODE:  UART silence mode
 *           @arg USART_UART_NORMAL_MODE:   UART normal mode
 * @retval None
 */
static void UsartSetSilenceMode(uint8_t u8Mode)
{
    m_u8UartSilenceMode = u8Mode;
}

/**
 * @brief  Get silence mode.
 * @param  [in] None
 * @retval Returned value can be one of the following values:
 *           @arg USART_UART_SILENCE_MODE:  UART silence mode
 *           @arg USART_UART_NORMAL_MODE:   UART normal mode
 */
static uint8_t UsartGetSilenceMode(void)
{
    return m_u8UartSilenceMode;
}

/**
 * @brief  Instal IRQ handler.
 * @param  [in] pstcConfig      Pointer to struct @ref stc_irq_signin_config_t
 * @param  [in] u32Priority     Interrupt priority
 * @retval None
 */
static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                    uint32_t u32Priority)
{
    if (NULL != pstcConfig)
    {
        INTC_IrqSignIn(pstcConfig);
        NVIC_ClearPendingIRQ(pstcConfig->enIRQn);
        NVIC_SetPriority(pstcConfig->enIRQn, u32Priority);
        NVIC_EnableIRQ(pstcConfig->enIRQn);
    }
}

/**
 * @brief  Main function of UART multiple processor master project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    static uint8_t u8TxData = 0U;
    static uint8_t u8RxData = 0U;
    stc_irq_signin_config_t stcIrqSigninCfg;
    const stc_usart_multiprocessor_init_t stcUartMultiProcessorInit = {
        .u32Baudrate = 115200UL,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOPBIT_1BIT,
        .u32DataWidth = USART_DATA_WIDTH_8BIT,
        .u32ClkMode = USART_INTCLK_NONE_OUTPUT,
        .u32PclkDiv = USART_PCLK_DIV64,
        .u32OversamplingBits = USART_OVERSAMPLING_8BIT,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO. */
    BSP_IO_Init();

    /* Initialize LED. */
    BSP_LED_Init();

    /* Initialize UART for debug print function. */
    DDL_PrintfInit();

    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(USART_RX_PORT, USART_RX_PIN, USART_RX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USART_TX_PORT, USART_TX_PIN, USART_TX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Enable peripheral clock */
    PWC_Fcg3PeriphClockCmd(USART_FUNCTION_CLK_GATE, Enable);

    /* Set silence mode */
    UsartSetSilenceMode(USART_UART_SILENCE_MODE);

    /* Initialize UART function. */
    USART_MultiProcessorInit(USART_UNIT, &stcUartMultiProcessorInit);

    /* Register error IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = USART_UNIT_ERR_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = USART_UNIT_ERR_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &USART_RxErr_IrqCallback;
    InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

    /* Register RX IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = USART_UNIT_RX_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = USART_UNIT_RX_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &USART_Rx_IrqCallback;
    InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_00);

    /* Register TX IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = USART_UNIT_TX_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = USART_UNIT_TX_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &USART_TxEmpty_IrqCallback;
    InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

    /* Register TC IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = USART_UNIT_TCI_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = USART_UNIT_TCI_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &USART_TxComplete_IrqCallback;
    InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_DEFAULT);

    while (1)
    {
        /* Wait key release */
        while (Reset == KeyState())
        {
        }

        RingBufWrite(&m_stcRingBuf, u8TxData);

        USART_FuncCmd(USART_UNIT, (USART_TX | USART_INT_TXE), Enable);

        while (IS_RING_BUFFER_EMPYT(&m_stcRingBuf))
        {
        }

        RingBufRead(&m_stcRingBuf, &u8RxData);

        printf("Master send:%d; master receive:%d \r\n", u8TxData, u8RxData);

        if (u8RxData == u8TxData)
        {
            BSP_LED_Off(LED_RED);
            BSP_LED_Toggle(LED_BLUE);
        }
        else
        {
            BSP_LED_On(LED_RED);
            BSP_LED_Off(LED_BLUE);
        }

        u8TxData++;
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
