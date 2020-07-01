/**
 *******************************************************************************
 * @file  usart/uart_dma/source/main.c
 * @brief This example demonstrates UART data receive and transfer by DMA.
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
 * @addtogroup USART_UART_DMA
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Red LED Port/Pin definition */
#define LED_R_PORT                      (GPIO_PORT_A)
#define LED_R_PIN                       (GPIO_PIN_00)
#define LED_R_ON()                      (GPIO_ResetPins(LED_R_PORT, LED_R_PIN))

/* DMA Unit/Channel/Interrupt definition */
#define DMA_UNIT                        (M4_DMA1)
#define DMA_CH                          (DMA_CH0)
#define DMA_BTC_INT                     (DMA_BTC_INT_CH0)
#define DMA_BTC_INT_SRC                 (INT_DMA1_BTC0)
#define DMA_BTC_INT_IRQn                (Int000_IRQn)
#define DMA_FUNCTION_CLK_GATE           (PWC_FCG0_DMA1)
#define DMA_TRIGGER_SOURCE              (EVT_USART1_RI)

/* Timer0 unit & channel definition */
#define TMR0_UNIT                       (M4_TMR0_1)
#define TMR0_CH                         (TMR0_CH_A)
#define TMR0_FUNCTION_CLK_GATE          (PWC_FCG2_TMR0_1)

/* UART unit definition */
#define USART_UNIT                      (M4_USART1)
#define USART_BAUDRATE                  (115200UL)
#define USART_FUNCTION_CLK_GATE         (PWC_FCG3_USART1)

/* UART unit interrupt definition */
#define USART_RXERR_INT_SRC             (INT_USART1_EI)
#define USART_RXERR_INT_IRQn            (Int001_IRQn)

#define USART_RXTO_INT_SRC              (INT_USART1_RTO)
#define USART_RXTO_INT_IRQn             (Int002_IRQn)

/* UART RX/TX Port/Pin definition */
#define USART_RX_PORT                   (GPIO_PORT_H)   /* PH13: USART1_RX */
#define USART_RX_PIN                    (GPIO_PIN_13)
#define USART_RX_GPIO_FUNC              (GPIO_FUNC_33_USART1_RX)

#define USART_TX_PORT                   (GPIO_PORT_H)   /* PH15: USART1_TX */
#define USART_TX_PIN                    (GPIO_PIN_15)
#define USART_TX_GPIO_FUNC              (GPIO_FUNC_32_USART1_TX)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static en_result_t DMA_Config(void);
static void TMR0_Config(void);
static void DMA_Btc_IrqCallback(void);
static void USART_RxErr_IrqCallback(void);
static void USART_RxTimeout_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static en_functional_state_t m_enLedOn = Disable;
static en_functional_state_t m_enLedCurrentStatus = Disable;

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
 * @brief  Initialize DMA.
 * @param  None
 * @retval en_result_t
 */
static en_result_t DMA_Config(void)
{
    en_result_t enRet = Error;
    stc_dma_init_t stcDmaInit;
    stc_irq_signin_config_t stcIrqSignConfig;

    /* DMA&AOS FCG enable */
    PWC_FCG0_Unlock();
    PWC_Fcg0PeriphClockCmd((DMA_FUNCTION_CLK_GATE | PWC_FCG0_AOS), Enable);
    PWC_FCG0_Lock();

    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCnt = 0UL;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32DestAddr = (uint32_t)(&USART_UNIT->DR);
    stcDmaInit.u32SrcAddr = ((uint32_t)(&USART_UNIT->DR) + 2UL);
    stcDmaInit.u32SrcInc = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestInc = DMA_DEST_ADDR_FIX;
    enRet = DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit);
    if (Ok == enRet)
    {
        /* Register interrupt */
        stcIrqSignConfig.enIntSrc = DMA_BTC_INT_SRC;
        stcIrqSignConfig.enIRQn  = DMA_BTC_INT_IRQn;
        stcIrqSignConfig.pfnCallback= &DMA_Btc_IrqCallback;
        INTC_IrqSignIn(&stcIrqSignConfig);
        NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
        NVIC_SetPriority(stcIrqSignConfig.enIRQn,DDL_IRQ_PRIORITY_DEFAULT);
        NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);

        /* DMA module enable */
        DMA_Cmd(DMA_UNIT, Enable);

        /* DMA channel interrupt enable */
        DMA_TransIntCmd(DMA_UNIT, DMA_BTC_INT, Enable);

        /* DMA channel enable */
        DMA_ChannelCmd(DMA_UNIT, DMA_CH, Enable);

        /* Set DMA trigger source */
        DMA_SetTriggerSrc(DMA_UNIT, DMA_CH, DMA_TRIGGER_SOURCE);
    }

    return enRet;
}

/**
 * @brief  Configure TMR0.
 * @param  None
 * @retval None
 */
static void TMR0_Config(void)
{
    uint32_t u32CmpVal;
    stc_tmr0_init_t stcTmr0Init;

    PWC_Fcg2PeriphClockCmd(TMR0_FUNCTION_CLK_GATE, Enable);

    /* Clear CNTAR register for channel A */
    TMR0_SetCntVal(TMR0_UNIT, TMR0_CH, 0U);

    /* TIMER0 basetimer function initialize */
    TMR0_StructInit(&stcTmr0Init);
    stcTmr0Init.u32ClockDivision = TMR0_CLK_DIV8;
    stcTmr0Init.u32ClockSource = TMR0_CLK_SRC_XTAL32;
    stcTmr0Init.u32HwTrigFunc = (TMR0_BT_HWTRG_FUNC_START | TMR0_BT_HWTRG_FUNC_CLEAR);
    if (TMR0_CLK_DIV1 == stcTmr0Init.u32ClockDivision)
    {
        u32CmpVal = (USART_BAUDRATE - 4UL);
    }
    else if (TMR0_CLK_DIV2 == stcTmr0Init.u32ClockDivision)
    {
        u32CmpVal = (USART_BAUDRATE/2UL - 2UL);
    }
    else
    {
        u32CmpVal = (USART_BAUDRATE / (1UL << (stcTmr0Init.u32ClockDivision >> TMR0_BCONR_CKDIVA_POS)) - 1UL);
    }
    DDL_ASSERT(u32CmpVal <= 0xFFFFUL);
    stcTmr0Init.u16CmpValue =  (uint16_t)(u32CmpVal);
    TMR0_Init(TMR0_UNIT, TMR0_CH, &stcTmr0Init);

    /* Clear compare flag */
    TMR0_ClearStatus(TMR0_UNIT, TMR0_CH);
}

/**
 * @brief  DMA block transfer complete IRQ callback function.
 * @param  None
 * @retval None
 */
static void DMA_Btc_IrqCallback(void)
{
    m_enLedOn = Disable;
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_BTC_INT);
}

/**
 * @brief  USART RX timeout IRQ callback.
 * @param  None
 * @retval None
 */
static void USART_RxTimeout_IrqCallback(void)
{
    m_enLedOn = Enable;
    TMR0_Cmd(TMR0_UNIT, TMR0_CH, Disable);
    USART_ClearStatus(USART_UNIT, USART_CLEAR_FLAG_RTOF);
}

/**
 * @brief  USART error IRQ callback function.
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
 * @brief  Main function of UART DMA project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_irq_signin_config_t stcIrqSigninCfg;
    const stc_usart_uart_init_t stcUartInit = {
        .u32Baudrate = USART_BAUDRATE,
        .u32BitDirection = USART_LSB,
        .u32StopBit = USART_STOPBIT_1BIT,
        .u32Parity = USART_PARITY_NONE,
        .u32DataWidth = USART_DATA_LENGTH_8BIT,
        .u32ClkMode = USART_INTERNCLK_OUTPUT,
        .u32PclkDiv = USART_PCLK_DIV64,
        .u32OversamplingBits = USART_OVERSAMPLING_8BIT,
        .u32NoiseFilterState = USART_NOISE_FILTER_DISABLE,
        .u32SbDetectPolarity = USART_SB_DETECT_FALLING,
    };

    /* MCU Peripheral registers write unprotected */
    Peripheral_WE();

    /* Initialize system clock. */
    BSP_CLK_Init();

    /* Initialize LED. */
    BSP_IO_Init();
    BSP_LED_Init();

    /* Initialize DMA. */
    DMA_Config();

    /* Initialize TMR0. */
    TMR0_Config();

    /* Configure USART RX/TX pin. */
    GPIO_SetFunc(USART_RX_PORT, USART_RX_PIN, USART_RX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(USART_TX_PORT, USART_TX_PIN, USART_TX_GPIO_FUNC, PIN_SUBFUNC_DISABLE);

    /* MCU Peripheral registers write protected */
    Peripheral_WP();

    /* Enable peripheral clock */
    PWC_Fcg3PeriphClockCmd(USART_FUNCTION_CLK_GATE, Enable);

    /* Initialize UART function. */
    if (Ok != USART_UartInit(USART_UNIT, &stcUartInit))
    {
        BSP_LED_On(LED_RED);
        while(1)
        {
        }
    }

    /* Register RX error IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = USART_RXERR_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = USART_RXERR_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &USART_RxErr_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    /* Register RX timeout IRQ handler && configure NVIC. */
    stcIrqSigninCfg.enIRQn = USART_RXTO_INT_IRQn;
    stcIrqSigninCfg.enIntSrc = USART_RXTO_INT_SRC;
    stcIrqSigninCfg.pfnCallback = &USART_RxTimeout_IrqCallback;
    INTC_IrqSignIn(&stcIrqSigninCfg);
    NVIC_ClearPendingIRQ(stcIrqSigninCfg.enIRQn);
    NVIC_SetPriority(stcIrqSigninCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSigninCfg.enIRQn);

    /* Enable TX && RX && RX interrupt function */
    USART_FuncCmd(USART_UNIT, (USART_RX | USART_INT_RX | USART_TX | \
                               USART_RTO | USART_INT_RTO), Enable);

    while (1)
    {
        if (m_enLedCurrentStatus != m_enLedOn)
        {
            if (m_enLedOn)
            {
                BSP_LED_On(LED_RED);
            }
            else
            {
                BSP_LED_Off(LED_RED);
            }
            m_enLedCurrentStatus = m_enLedOn;
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
