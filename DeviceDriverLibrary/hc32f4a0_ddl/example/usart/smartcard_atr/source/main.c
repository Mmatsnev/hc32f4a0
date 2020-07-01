/**
 *******************************************************************************
 * @file  usart/smartcard_atr/source/main.c
 * @brief This example demonstrates USART receive smart-card ATR.
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

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USART_Smartcard_ATR
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
    uint8_t  au8Buf[33];
} stc_buffer_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* Smart-card CD port/pin definition */
#define SMARTCARD_CD_PORT               (EIO_PORT0)
#define SMARTCARD_CD_PIN                (EIO_PIN3)

/* Smart-card reset port/pin definition */
#define SMARTCARD_RESET_PORT            (GPIO_PORT_E)
#define SMARTCARD_RESET_PIN             (GPIO_PIN_04)

/* Smart-card power on port/pin definition */
#define SMARTCARD_PWREN_PORT            (GPIO_PORT_E)
#define SMARTCARD_PWREN_PIN             (GPIO_PIN_05)

/* Smart-card CD pin operation */
#define IS_CARD_REMOVED()               (BSP_IO_ReadPortPin(SMARTCARD_CD_PORT, SMARTCARD_CD_PIN))

/* Smart-card reset pin operation */
#define SMARTCARD_RESET_LOW()           (GPIO_ResetPins(SMARTCARD_RESET_PORT, SMARTCARD_RESET_PIN))
#define SMARTCARD_RESET_HIGH()          (GPIO_SetPins(SMARTCARD_RESET_PORT, SMARTCARD_RESET_PIN))

/* Smart-card power pin operation */
#define SMARTCARD_POWER_ON()            (GPIO_ResetPins(SMARTCARD_PWREN_PORT, SMARTCARD_PWREN_PIN))
#define SMARTCARD_POWER_OFF()           (GPIO_SetPins(SMARTCARD_PWREN_PORT, SMARTCARD_PWREN_PIN))

/* Smart-card USART RX Port/Pin definition */
#define USART_RX_PORT                   (GPIO_PORT_H)
#define USART_RX_PIN                    (GPIO_PIN_06)
#define USART_RX_GPIO_FUNC              (GPIO_FUNC_37_USART6_RX)

/* Smart-card USART CK Port/Pin definition */
#define USART_CK_PORT                   (GPIO_PORT_E)
#define USART_CK_PIN                    (GPIO_PIN_03)
#define USART_CK_GPIO_FUNC              (GPIO_FUNC_7)

/* Smart-card USART uit definition */
#define USART_UNIT                      (M4_USART6)
#define USART_FUNCTION_CLK_GATE         (PWC_FCG3_USART6)

/* UART unit interrupt definition */
#define USART_UNIT_RXERR_INT_SRC        (INT_USART6_EI)
#define USART_UNIT_RXERR_INT_IRQn       (Int000_IRQn)

#define USART_UNIT_RX_INT_SRC           (INT_USART6_RI)
#define USART_UNIT_RX_INT_IRQn          (Int001_IRQn)

/* Buffer size */
#define IS_BUFFER_EMPYT(x)              (0U == ((x)->u16UsedSize))

/* Smart-card initial character TS: 0x3B or 0x3F */
#define SMARTCARD_INITIAL_CHARACTER_TS_DIRECT_CONVENTION    (0x3BU)
#define SMARTCARD_INITIAL_CHARACTER_TS_INVERSE_CONVENTION   (0x3FU)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static void USART_Rx_IrqCallback(void);
static void USART_RxErr_IrqCallback(void);
static en_result_t BufWrite(stc_buffer_t *pstcBuffer, uint8_t u8Data);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_buffer_t m_stcRxBuf = {
    .u16InIdx = 0,
    .u16UsedSize = 0,
    .u16Capacity = sizeof (m_stcRxBuf.au8Buf),
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
 * @brief  USART RX IRQ callback
 * @param  None
 * @retval None
 */
static void USART_Rx_IrqCallback(void)
{
    uint16_t u16Data = USART_RecData(USART_UNIT);

    BufWrite(&m_stcRxBuf, (uint8_t)u16Data);
}

/**
 * @brief  USART error IRQ callback.
 * @param  None
 * @retval None
 */
static void USART_RxErr_IrqCallback(void)
{
    if (Set == USART_GetStatus(USART_UNIT, (USART_FLAG_PE | USART_FLAG_FE)))
    {
        (void)USART_RecData(USART_UNIT);
    }

    USART_ClearStatus(USART_UNIT, (USART_CLEAR_FLAG_PE | \
                                 USART_CLEAR_FLAG_FE | \
                                 USART_CLEAR_FLAG_ORE));
}

/**
 * @brief  Write buffer.
 * @param  [in] pstcBuffer              Pointer to a @ref stc_buffer_t structure
 * @param  [in] u8Data                  Data to write
 * @retval An en_result_t enumeration value:
 *           - Ok: Write success.
 *           - ErrorBufferFull: Buffer is full.
 */
static en_result_t BufWrite(stc_buffer_t *pstcBuffer, uint8_t u8Data)
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
 * @brief  Main function of UART interrupt project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_gpio_init_t stcGpioInit;
    stc_irq_signin_config_t stcIrqSigninCfg;
    const stc_usart_smartcard_init_t stcSmartcardInit = {
        .u32Baudrate = 9600UL,
        .u32ClkMode = USART_INTERNCLK_OUTPUT,
        .u32PclkDiv = USART_PCLK_DIV1,
        .u32StopBit = USART_STOPBIT_2BIT,
        .u32BitDirection = USART_LSB,
    };

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize IO. */
    BSP_IO_Init();

    /* Initialize LED. */
    BSP_LED_Init();

    /* Initialize smart card pin. */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    GPIO_Init(SMARTCARD_RESET_PORT, SMARTCARD_RESET_PIN, &stcGpioInit);
    GPIO_Init(SMARTCARD_PWREN_PORT, SMARTCARD_PWREN_PIN, &stcGpioInit);

    GPIO_SetFunc(USART_RX_PORT, USART_RX_PIN, USART_RX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USART_CK_PORT, USART_CK_PIN, USART_CK_GPIO_FUNC, PIN_SUBFUNC_DISABLE);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    while (IS_CARD_REMOVED())
    {
        DDL_DelayMS(200UL);
        BSP_LED_Toggle(LED_RED);
    }

    BSP_LED_Off(LED_RED);

    /* Enable peripheral clock */
    PWC_Fcg3PeriphClockCmd(USART_FUNCTION_CLK_GATE, Enable);

    /* Smart card : active */
    SMARTCARD_RESET_LOW();
    SMARTCARD_POWER_ON();
    USART_DeInit(USART_UNIT);

    /* Initialize UART function. */
    if (Ok == USART_SmartcardInit(USART_UNIT, &stcSmartcardInit))
    {
        /* Register error IRQ handler && configure NVIC. */
        stcIrqSigninCfg.enIRQn = USART_UNIT_RXERR_INT_IRQn;
        stcIrqSigninCfg.enIntSrc = USART_UNIT_RXERR_INT_SRC;
        stcIrqSigninCfg.pfnCallback = &USART_RxErr_IrqCallback;
        INTC_IrqSignIn(&stcIrqSigninCfg);
        NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
        NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
        NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

        /* Register RX IRQ handler && configure NVIC. */
        stcIrqSigninCfg.enIRQn = USART_UNIT_RX_INT_IRQn;
        stcIrqSigninCfg.enIntSrc = USART_UNIT_RX_INT_SRC;
        stcIrqSigninCfg.pfnCallback = &USART_Rx_IrqCallback;
        INTC_IrqSignIn(&stcIrqSigninCfg);
        NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
        NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
        NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

        /* Enable RX function */
        USART_FuncCmd(USART_UNIT, (USART_RX | USART_INT_RX), Enable);

        /* Smart card : cold reset*/
        SMARTCARD_RESET_HIGH();
        DDL_DelayMS(200UL);  /* Delay for receving Smart-card ATR */

        /* Smart card : release */
        SMARTCARD_RESET_LOW();
        USART_DeInit(USART_UNIT);
        SMARTCARD_POWER_OFF();
    }

    if ((SMARTCARD_INITIAL_CHARACTER_TS_DIRECT_CONVENTION == m_stcRxBuf.au8Buf[0]) || \
        (SMARTCARD_INITIAL_CHARACTER_TS_INVERSE_CONVENTION == m_stcRxBuf.au8Buf[0]))
    {
        BSP_LED_On(LED_BLUE);
    }
    else
    {
        BSP_LED_On(LED_RED);
    }

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
