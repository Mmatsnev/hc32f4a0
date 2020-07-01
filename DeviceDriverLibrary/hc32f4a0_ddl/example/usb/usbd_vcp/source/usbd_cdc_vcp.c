/**
 *******************************************************************************
 * @file  usb/usbd_vcp/source/usbd_cdc_vcp.c
 * @brief Generic media access Layer.
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
#include "usbd_cdc_vcp.h"
#include "usb_conf.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USBD_VCP
 * @{
 */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#pragma     data_alignment = 4
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CDC_COMM                        (M4_USART6)
/* UART RX/TX Port/Pin definition */
#define USART_RX_PORT                   (GPIO_PORT_H)   /* PH6: USART6_RX */
#define USART_RX_PIN                    (GPIO_PIN_06)
#define USART_RX_GPIO_FUNC              (GPIO_FUNC_37_USART6_RX)

#define USART_TX_PORT                   (GPIO_PORT_E)   /* PE6: USART6_TX */
#define USART_TX_PIN                    (GPIO_PIN_06)
#define USART_TX_GPIO_FUNC              (GPIO_FUNC_36_USART6_TX)

/* UART unit definition */
#define USART_FUNCTION_CLK_GATE         (PWC_FCG3_USART6)

/* UART unit interrupt definition */
#define USART_UNIT_ERR_INT              (INT_USART6_EI)
#define USART_UNIT_ERR_IRQn             (Int000_IRQn)

#define USART_UNIT_RX_INT               (INT_USART6_RI)
#define USART_UNIT_RX_IRQn              (Int001_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* These are external variables imported from CDC core to be used for IN
   transfer management. */
extern uint8_t  APP_Rx_Buffer [APP_RX_DATA_SIZE]; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx   (uint32_t Len);
static uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_COMConfig(void);  /* MISRAC 2004*/

static void UsartErrIrqCallback(void);
static void UsartRxIrqCallback(void);

static void InstalIrqHandler(const stc_irq_signin_config_t *pstcConfig,
                                    uint32_t u32Priority);

CDC_IF_Prop_TypeDef VCP_fops =
{
    &VCP_Init,
    &VCP_DeInit,
    &VCP_Ctrl,
    &VCP_DataTx,
    &VCP_DataRx
};

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
  * @brief  VCP_Init
  *         Initializes the Media
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Init(void)
{
    stc_irq_signin_config_t stcIrqSigninCfg;
    const stc_usart_uart_init_t stcUartInit = {
        .u32Baudrate = 500000UL,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOPBIT_1BIT,
        .u32Parity = USART_PARITY_NONE,
        .u32DataWidth = USART_DATA_LENGTH_8BIT,
        .u32ClkMode = USART_INTERNCLK_NONE_OUTPUT,
        .u32OversamplingBits = USART_OVERSAMPLING_8BIT,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(USART_RX_PORT, USART_RX_PIN, USART_RX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USART_TX_PORT, USART_TX_PIN, USART_TX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);

    /* Enable peripheral clock */
    PWC_Fcg3PeriphClockCmd(USART_FUNCTION_CLK_GATE, Enable);

    /* Initialize UART function. */
    if (Ok == USART_UartInit(CDC_COMM, &stcUartInit))
    {
        /* Register error IRQ handler && configure NVIC. */
        stcIrqSigninCfg.enIRQn = USART_UNIT_ERR_IRQn;
        stcIrqSigninCfg.enIntSrc = USART_UNIT_ERR_INT;
        stcIrqSigninCfg.pfnCallback = &UsartErrIrqCallback;
        InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_01);

        /* Register RX IRQ handler && configure NVIC. */
        stcIrqSigninCfg.enIRQn = USART_UNIT_RX_IRQn;
        stcIrqSigninCfg.enIntSrc = USART_UNIT_RX_INT;
        stcIrqSigninCfg.pfnCallback = &UsartRxIrqCallback;
        InstalIrqHandler(&stcIrqSigninCfg, DDL_IRQ_PRIORITY_01);

        USART_FuncCmd(CDC_COMM, (USART_RX | USART_TX), Enable);
        USART_FuncCmd(CDC_COMM, USART_INT_RX, Enable);

    }
    else
    {
        return USBD_FAIL;
    }

    return USBD_OK;
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the Media
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_DeInit(void)
{
    return USBD_OK;
}


/**
  * @brief  VCP_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
    switch (Cmd)
    {
        case SEND_ENCAPSULATED_COMMAND:
        /* Not  needed for this driver */
        break;

        case GET_ENCAPSULATED_RESPONSE:
        /* Not  needed for this driver */
        break;

        case SET_COMM_FEATURE:
        /* Not  needed for this driver */
        break;

        case GET_COMM_FEATURE:
        /* Not  needed for this driver */
        break;

        case CLEAR_COMM_FEATURE:
        /* Not  needed for this driver */
        break;

        case SET_LINE_CODING:
            linecoding.bitrate = ((uint32_t)Buf[0U] | ((uint32_t)Buf[1U] << 8U) | ((uint32_t)Buf[2U] << 16U) | ((uint32_t)Buf[3U] << 24U));
            linecoding.format = Buf[4];
            linecoding.paritytype = Buf[5U];
            linecoding.datatype = Buf[6U];
            /* Set the new configuration */
            VCP_COMConfig();  /* MISRAC 2004*/
        break;
        case GET_LINE_CODING:
            Buf[0U] = (uint8_t)(linecoding.bitrate);
            Buf[1U] = (uint8_t)(linecoding.bitrate >> 8U);
            Buf[2U] = (uint8_t)(linecoding.bitrate >> 16U);
            Buf[3U] = (uint8_t)(linecoding.bitrate >> 24U);
            Buf[4U] = linecoding.format;
            Buf[5U] = linecoding.paritytype;
            Buf[6U] = linecoding.datatype;
        break;
        case SET_CONTROL_LINE_STATE:
        /* Not  needed for this driver */
        break;
        case SEND_BREAK:
        /* Not  needed for this driver */
        break;
        default:
        break;
    }
    return USBD_OK;
}

/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in
  *         this function.
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataTx (uint32_t Len)
{
    if (linecoding.datatype == 7U)
    {
        APP_Rx_Buffer[APP_Rx_ptr_in] = (uint8_t)USART_RecData(CDC_COMM) & 0x7FU;
    }
    else if (linecoding.datatype == 8U)
    {
        APP_Rx_Buffer[APP_Rx_ptr_in] = (uint8_t)USART_RecData(CDC_COMM);
    }
    else
    {
        //
    }

    APP_Rx_ptr_in++;

    /* To avoid buffer overflow */
    if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
    {
        APP_Rx_ptr_in = 0U;
    }

    return USBD_OK;
}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
    uint32_t i;

    for (i = 0UL; i < Len; i++)
    {
        while(Set != USART_GetStatus(CDC_COMM, USART_FLAG_TXE))
        {
            ;
        }
        USART_SendData(CDC_COMM, (uint16_t)*(Buf + i));
    }
    return USBD_OK;
}

uint16_t VCP_COMConfigDefault(void)
{

    uint16_t u16Res = USBD_OK;
    uint8_t u8Cnt;
    float32_t fErr;
#if 0
    const stc_usart_uart_init_t stcUartInit = {
        .u32Baudrate = 500000UL,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOPBIT_1BIT,
        .u32Parity = USART_PARITY_NONE,
        .u32DataWidth = USART_DATA_LENGTH_8BIT,
        .u32ClkMode = USART_INTERNCLK_NONE_OUTPUT,
        .u32OversamplingBits = USART_OVERSAMPLING_8BIT,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* Initialize UART function. */
    if (Ok == USART_UartInit(CDC_COMM, &stcUartInit))
    {

    }
#endif
    for (u8Cnt=0U; u8Cnt < 4U; u8Cnt++)
    {
        if(Ok == USART_SetBaudrate(CDC_COMM, 500000UL, &fErr))
        {
            USART_FuncCmd(CDC_COMM, (USART_RX | USART_TX), Enable);
            USART_FuncCmd(CDC_COMM, USART_INT_RX, Enable);
            break;
        }
        else
        {
            USART_SetPclkDiv(CDC_COMM, u8Cnt);
        }
    }
    if (u8Cnt == 4U)
    {
        u16Res = USBD_FAIL;
    }
    return u16Res;
}
/**
  * @brief  VCP_COMConfig
  *         Configure the COM Port with default values or values received from host.
  * @param  None  MISRAC 2004
  *
  * @retval None.
  */
static uint16_t VCP_COMConfig(void)
{

    uint8_t u8Cnt;
    uint16_t u16Res = USBD_OK;
    uint8_t u8ReturnFlag = 0U;
    float fErr;

    stc_usart_uart_init_t stcUartInit = {
        .u32Baudrate = 500000UL,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOPBIT_1BIT,
        .u32Parity = USART_PARITY_NONE,
        .u32DataWidth = USART_DATA_LENGTH_8BIT,
        .u32ClkMode = USART_INTERNCLK_NONE_OUTPUT,
        .u32OversamplingBits = USART_OVERSAMPLING_8BIT,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* set the Stop bit*/
    switch (linecoding.format)
    {
        case 0U:
            stcUartInit.u32StopBit = USART_STOPBIT_1BIT;
            break;
        case 1U:
            stcUartInit.u32StopBit = USART_STOPBIT_1BIT;
            break;
        case 2U:
            stcUartInit.u32StopBit = USART_STOPBIT_2BIT;
            break;
        default :
            VCP_COMConfigDefault();
            u16Res = USBD_FAIL;
            u8ReturnFlag = 1U;
            break;
    }

    if(1U != u8ReturnFlag)
    {
        /* set the parity bit*/
        switch (linecoding.paritytype)
        {
            case 0U:
                stcUartInit.u32Parity = USART_PARITY_NONE;
                break;
            case 1U:
                stcUartInit.u32Parity = USART_PARITY_EVEN;
                break;
            case 2U:
                stcUartInit.u32Parity = USART_PARITY_ODD;
                break;
            default :
                VCP_COMConfigDefault();
                u16Res = USBD_FAIL;
                u8ReturnFlag = 1U;
                break;
        }

        if(1U != u8ReturnFlag)
        {
            /*set the data type : only 8bits and 9bits is supported */
            switch (linecoding.datatype)
            {
                case 0x07U:
                    /* With this configuration a parity (Even or Odd) should be set */
                    stcUartInit.u32DataWidth = USART_DATA_LENGTH_8BIT;
                    break;
                case 0x08U:
                    if (stcUartInit.u32Parity == USART_PARITY_NONE)
                    {
                        stcUartInit.u32DataWidth = USART_DATA_LENGTH_8BIT;
                    }
                    else
                    {
                        stcUartInit.u32DataWidth = USART_DATA_LENGTH_9BIT;
                    }
                    break;
                default :
                    VCP_COMConfigDefault();
                    u16Res = USBD_FAIL;
                    u8ReturnFlag = 1U;
                    break;
            }

            if(1U != u8ReturnFlag)
            {
                stcUartInit.u32Baudrate = linecoding.bitrate;
                /* Configure and enable the USART */
                USART_UartInit(CDC_COMM, &stcUartInit);

                for (u8Cnt=0U; u8Cnt < 4U; u8Cnt++)
                {
                    if(Ok == USART_SetBaudrate(CDC_COMM, stcUartInit.u32Baudrate, &fErr))
                    {
                        USART_FuncCmd(CDC_COMM, (USART_RX | USART_TX), Enable);
                        USART_FuncCmd(CDC_COMM, USART_INT_RX, Enable);
                        break;
                    }
                    else
                    {
                        USART_SetPclkDiv(CDC_COMM, u8Cnt);
                    }
                }
                if (u8Cnt == 4U)
                {
                    u16Res = USBD_FAIL;
                }
            }
        }
    }
    return u16Res;
}

/**
 * @brief USART RX irq callback function.
 * @param [in] None
 * @retval None
 */
static void UsartRxIrqCallback(void)
{
    if (Set == USART_GetStatus(CDC_COMM, USART_FLAG_RXNE))
    {
        /* Send the received data to the PC Host*/
        VCP_DataTx (0U);
    }
}

/**
 * @brief USART RX error irq callback function.
 * @param [in] None
 * @retval None
 */
static void UsartErrIrqCallback(void)
{
    if (Set == USART_GetStatus(CDC_COMM, USART_FLAG_FE))
    {
        USART_ClearStatus(CDC_COMM, USART_CLEAR_FLAG_FE);
    }
    else
    {
    }

    if (Set == USART_GetStatus(CDC_COMM, USART_FLAG_PE))
    {
        USART_ClearStatus(CDC_COMM, USART_CLEAR_FLAG_PE);
    }
    else
    {
    }

    if (Set == USART_GetStatus(CDC_COMM, USART_FLAG_ORE))
    {
        USART_ClearStatus(CDC_COMM, USART_CLEAR_FLAG_ORE);
    }
    else
    {
    }
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
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
