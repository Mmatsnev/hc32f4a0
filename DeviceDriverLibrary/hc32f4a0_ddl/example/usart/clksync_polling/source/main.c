/**
 *******************************************************************************
 * @file  usart/clksync_polling/source/main.c
 * @brief This example demonstrates CLKSYNC data receive and transfer by polling.
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
 * @addtogroup USART_Clocksync_Polling
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Key port&pin definition */
#define KEY_PORT                        (GPIO_PORT_A)
#define KEY_PIN                         (GPIO_PIN_00)

/* CLKSYNC CK/RX/TX Port/Pin definition */
#define CLKSYNC_CK_PORT                 (GPIO_PORT_E)   /* PE3: USART6_CK */
#define CLKSYNC_CK_PIN                  (GPIO_PIN_03)
#define CLKSYNC_CK_GPIO_FUNC            (GPIO_FUNC_7)

#define CLKSYNC_RX_PORT                 (GPIO_PORT_H)   /* PH6: USART6_RX */
#define CLKSYNC_RX_PIN                  (GPIO_PIN_06)
#define CLKSYNC_RX_GPIO_FUNC            (GPIO_FUNC_37_USART6_RX)

#define CLKSYNC_TX_PORT                 (GPIO_PORT_E)   /* PE6: USART6_TX */
#define CLKSYNC_TX_PIN                  (GPIO_PIN_06)
#define CLKSYNC_TX_GPIO_FUNC            (GPIO_FUNC_36_USART6_TX)

/* USART unit definition */
#define CLKSYNC_UNIT                    (M4_USART6)
#define USART_FUNCTION_CLK_GATE         (PWC_FCG3_USART6)

/* Timeout max definition  */
#define TIMEOUT_MAX                     (0xFFFFFFFFUL)

/* CLKSYNC device mode definition */
#define CLKSYNC_MASTER_MODE             (0U)
#define CLKSYNC_SLAVE_MODE              (1U)

/* USART master or slave mode selection */
#define CLKSYNC_DEVICE_MODE             (CLKSYNC_MASTER_MODE)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static en_flag_status_t KeyState(void);
static en_result_t USART_WaitOnFlagUntilTimeout(M4_USART_TypeDef *USARTx,
                                                uint32_t u32Flag,
                                                en_flag_status_t enStatus,
                                                uint32_t u32TickStart,
                                                uint32_t u32Timeout);
static en_result_t CLKSYNC_TransmitReceive(M4_USART_TypeDef *USARTx,
                                           const uint8_t au8TxData[],
                                           uint8_t au8RxData[],
                                           uint32_t u32Size,
                                           uint32_t u32Timeout);

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
 * @brief  This function handles USART Communication Timeout.
 * @param  [in] USARTx                  pointer to a USART instance.
 * @param  [in] u32Flag                 specifies the USART flag to check.
 * @param  [in] enStatus                The new Flag status (Set or Reset).
 * @param  [in] u32TickStart            Tick start value.
 * @param  [in] u32Timeout              Timeout duration.
 * @retval An en_result_t enumeration value:
 *           - Ok: success
 *           - ErrorTimeout: timeout
 */
static en_result_t USART_WaitOnFlagUntilTimeout(M4_USART_TypeDef *USARTx,
                                                uint32_t u32Flag,
                                                en_flag_status_t enStatus,
                                                uint32_t u32TickStart,
                                                uint32_t u32Timeout)
{
    en_result_t enRet = Ok;

    /* Wait until flag is set */
    while((USART_GetStatus(USARTx, u32Flag) ? Set : Reset) == enStatus)
    {
        /* Check for the Timeout */
        if (TIMEOUT_MAX - u32Timeout)
        {
            if (((SysTick_GetTick() - u32TickStart) > u32Timeout) || (!u32Timeout))
            {
                enRet = ErrorTimeout;
                break;
            }
        }
    }

    return enRet;
}

/**
 * @brief  Send && receive an amount of data in full-duplex mode (blocking mode).
 * @param  [in] USARTx                  Pointer to a USART instance.
 * @param  [in] au8TxData               Data transmitted buffer
 * @param  [out] au8RxData              Data received buffer
 * @param  [in] u32Size                 Amount of data to be sent
 * @param  [in] u32Timeout              Timeout duration
 * @retval An en_result_t enumeration value:
 *           - Ok: success
 *           - ErrorInvalidParameter: USARTx/pu8TxData/pu8RxData/u16Size is invalid
 */
static en_result_t CLKSYNC_TransmitReceive(M4_USART_TypeDef *USARTx,
                                           const uint8_t au8TxData[],
                                           uint8_t au8RxData[],
                                           uint32_t u32Size,
                                           uint32_t u32Timeout)
{
    uint32_t i = 0UL;
    uint32_t u32Flag;
    uint32_t u32TickStart = 0UL;
    uint32_t u32XferCount = u32Size;
    en_result_t enRet = ErrorInvalidParameter;

    if(USARTx && au8TxData && au8RxData && u32Size)
    {
        enRet = Ok;

        /* Init tickstart for timeout managment */
        u32TickStart = SysTick_GetTick();

        /* Check the remain data to be received */
        while (u32XferCount > 0UL)
        {
            u32XferCount--;

            /* Wait for TX empty or TX complete flag in order to write data in DR */
            u32Flag = (USART_INTERNCLK_OUTPUT == USART_GetClockMode(USARTx)) ? USART_FLAG_TC : USART_FLAG_TXE;
            if (USART_WaitOnFlagUntilTimeout(USARTx, u32Flag, Reset, u32TickStart, u32Timeout) != Ok)
            {
                enRet = ErrorTimeout;
            }
            else
            {
                USART_SendData(USARTx, (uint16_t)(au8TxData[i]));

                /* Wait for RXNE Flag */
                if (USART_WaitOnFlagUntilTimeout(USARTx, USART_FLAG_RXNE, Reset, u32TickStart, u32Timeout) != Ok)
                {
                    enRet = ErrorTimeout;
                }
                else
                {
                    /* Receive data */
                    au8RxData[i] = (uint8_t)USART_RecData(USARTx);
                }
                i++;
            }

            if (enRet != Ok)
            {
                break;
            }
        }
    }

    return enRet;
}

/**
 * @brief  Main function of CLKSYNC polling project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_usart_clksync_init_t stcClksyncInit;
    /* Buffer used for transmission */
    char au8TxBuffer[] = "CLKSYNC TX/RX example: Communication between two boards using usart interface!";
    /* Buffer used for reception */
    uint8_t au8RxBuffer[(ARRAY_SZ(au8TxBuffer))];

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Configure system tick. */
    SysTick_Init(100UL);

    /* Initialize IO. */
    BSP_IO_Init();

    /* Initialize LED. */
    BSP_LED_Init();

    /* Configure USART CK/RX/TX pin. */
    GPIO_SetFunc(CLKSYNC_CK_PORT, CLKSYNC_CK_PIN, CLKSYNC_CK_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(CLKSYNC_RX_PORT, CLKSYNC_RX_PIN, CLKSYNC_RX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(CLKSYNC_TX_PORT, CLKSYNC_TX_PIN, CLKSYNC_TX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Enable peripheral clock */
    PWC_Fcg3PeriphClockCmd(USART_FUNCTION_CLK_GATE, Enable);

    /* Initialize CLKSYNC function. */
    USART_ClkSyncStructInit(&stcClksyncInit);
#if (CLKSYNC_DEVICE_MODE == CLKSYNC_MASTER_MODE)
    stcClksyncInit.u32Baudrate = 115200UL;
    stcClksyncInit.u32PclkDiv = USART_PCLK_DIV4,
    stcClksyncInit.u32ClkMode = USART_INTERNCLK_OUTPUT;
#else
    stcClksyncInit.u32ClkMode = USART_EXTCLK;
#endif
    USART_ClkSyncInit(CLKSYNC_UNIT, &stcClksyncInit);

    /* Enable RX/TX function */
    USART_FuncCmd(CLKSYNC_UNIT, (USART_RX | USART_TX), Enable);

    /* User key */
    while (Reset == KeyState())
    {
    }

    /* Start the transmission process*/
    CLKSYNC_TransmitReceive(CLKSYNC_UNIT, (uint8_t *)au8TxBuffer, au8RxBuffer, (ARRAY_SZ(au8TxBuffer)), TIMEOUT_MAX);

    /* Compare m_u8TxBuffer and m_u8RxBuffer data */
    if (memcmp(au8TxBuffer, au8RxBuffer, (ARRAY_SZ(au8TxBuffer))) == 0)
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
 * @brief  systick callback function
 * @param  [in]  None
 * @retval None
 *
 */
void SysTick_IrqHandler(void)
{
    SysTick_IncTick();
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
