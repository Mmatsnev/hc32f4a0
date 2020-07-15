/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176.c
 * @brief This file provides firmware functions for EV_HC32F4A0_LQFP176 BSP
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
   2020-07-15       Zhangxl         Use XTAL 8MHz as PLL source
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
#include "ev_hc32f4a0_lqfp176.h"
#include "ev_hc32f4a0_lqfp176_tca9539.h"

/**
 * @defgroup BSP BSP
 * @{
 */

/**
 * @defgroup EV_HC32F4A0_LQFP176 HC32F4A0_LQFP176_EVB
 * @{
 */

#if (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0)

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
static uint32_t gu32GlobalKey = 0x00000000UL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup BSP_Global_Functions BSP Global Functions
 * @{
 */
/**
  * @brief  EIO delay
  * @param  [in] u32Delay Delay in ms
  * @retval none
  */
void EIO_Delay(uint32_t u32Delay)
{
    DDL_DelayMS(u32Delay);
}

/**
  * @brief  EIO initialize
  * @param  None
  * @retval None
  */
void EIO_Init(void)
{
    stc_i2c_init_t stcI2cInit;
    float32_t fErr;

    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);

    /* */
    GPIO_Init(TCA9539_SCL_PORT, TCA9539_SCL_PIN, &stcGpioInit);
    GPIO_Init(TCA9539_SDA_PORT, TCA9539_SDA_PIN, &stcGpioInit);

    GPIO_SetFunc(TCA9539_SCL_PORT, TCA9539_SCL_PIN, GPIO_FUNC_49_I2C1_SCL ,PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(TCA9539_SDA_PORT, TCA9539_SDA_PIN, GPIO_FUNC_48_I2C1_SDA ,PIN_SUBFUNC_DISABLE);

    EIO_Reset();

    /* I2C1 clock enable */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_IIC1, Enable);

    I2C_DeInit(TCA9539_I2C_CH);

    I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32Baudrate = TCA9539_BAUDRATE;
    stcI2cInit.u32SclTime = 0U;
    stcI2cInit.u32I2cClkDiv = I2C_CLK_DIV16;
    I2C_Init(TCA9539_I2C_CH, &stcI2cInit, &fErr);

    I2C_Cmd(TCA9539_I2C_CH, Enable);
}

/**
  * @brief  EIO register write
  * @param  [in] u8Reg register definition
  *   @arg  TCA9539_REG_INPUT_0
  *   @arg  TCA9539_REG_INPUT_1
  *   @arg  TCA9539_REG_OUTPUT_0
  *   @arg  TCA9539_REG_OUTPUT_1
  *   @arg  TCA9539_REG_INVERT_0
  *   @arg  TCA9539_REG_INVERT_1
  *   @arg  TCA9539_REG_CONFIG_0
  *   @arg  TCA9539_REG_CONFIG_1
  * @param  [in] u8Val register value
  * @retval en_result_t
  */
en_result_t EIO_Write(uint8_t u8Reg, uint8_t u8Val)
{
    I2C_Start(TCA9539_I2C_CH, TCA9539_TIMEOUT);
    I2C_SendAddr(TCA9539_I2C_CH, (TCA9539_ADDR | TCA9539_WRITE), TCA9539_TIMEOUT);
    I2C_SendData(TCA9539_I2C_CH, (uint8_t *)&u8Reg, 1UL, TCA9539_TIMEOUT);
    I2C_SendData(TCA9539_I2C_CH, (uint8_t *)&u8Val, 1UL, TCA9539_TIMEOUT);
    I2C_Stop(TCA9539_I2C_CH, TCA9539_TIMEOUT);
    return Ok;
}

/**
  * @brief  EIO register read
  * @param  [in] u8Reg register definition
  *   @arg  TCA9539_REG_INPUT_0
  *   @arg  TCA9539_REG_INPUT_1
  *   @arg  TCA9539_REG_OUTPUT_0
  *   @arg  TCA9539_REG_OUTPUT_1
  *   @arg  TCA9539_REG_INVERT_0
  *   @arg  TCA9539_REG_INVERT_1
  *   @arg  TCA9539_REG_CONFIG_0
  *   @arg  TCA9539_REG_CONFIG_1
  * @param  [out] *u8Val register value
  * @retval en_result_t
  */
en_result_t EIO_Read(uint8_t u8Reg, uint8_t *u8Val)
{
    I2C_Start(TCA9539_I2C_CH, TCA9539_TIMEOUT);
    I2C_SendAddr(TCA9539_I2C_CH, (TCA9539_ADDR | TCA9539_WRITE), TCA9539_TIMEOUT);
    I2C_SendData(TCA9539_I2C_CH, (uint8_t *)&u8Reg, 1UL, TCA9539_TIMEOUT);

    I2C_Restart(TCA9539_I2C_CH, TCA9539_TIMEOUT);
    I2C_SendAddr(TCA9539_I2C_CH, (TCA9539_ADDR | TCA9539_READ), TCA9539_TIMEOUT);
    I2C_RcvData(TCA9539_I2C_CH, u8Val, 1UL, TCA9539_TIMEOUT);
    I2C_Stop(TCA9539_I2C_CH, TCA9539_TIMEOUT);
    return Ok;
}

/**
  * @brief  EIO reset
  * @retval none
  */
void EIO_Reset(void)
{
    GPIO_OE(GPIO_PORT_C, GPIO_PIN_13, Enable);
    GPIO_ResetPins(GPIO_PORT_C, GPIO_PIN_13);
    DDL_DelayMS(3UL);
    GPIO_SetPins(GPIO_PORT_C, GPIO_PIN_13);
}

/**
  * @brief  EIO interrupt initialize
  * @retval none
  */
void EIO_IntInit(void)
{
    /* interrupt config*/
}

/**
 * @brief  CAM initialize.
 * @param  None
 * @retval none
 */
void BSP_CAM_Init(void)
{
    /* Set initial state before output */
    BSP_IO_WritePortPin(CAM_PORT, (CAM_RST_PIN | CAM_STB_PIN), EIO_PIN_SET);
    /* CAM pins set to output */
    BSP_IO_ConfigPortPin(CAM_PORT, (CAM_RST_PIN | CAM_STB_PIN), EIO_DIR_OUT);
}

/**
 * @brief  CAM reset pin config.
 * @param  [in] Cmd
 *   @arg  EIO_PIN_SET
 *   @arg  EIO_PIN_RESET
 * @retval none
 */
void BSP_CAM_RSTCmd(uint8_t Cmd)
{
    BSP_IO_WritePortPin(CAM_PORT, CAM_RST_PIN, Cmd);
}

/**
 * @brief  CAM standby pin config.
 * @param  [in] Cmd
 *   @arg  EIO_PIN_SET
 *   @arg  EIO_PIN_RESET
 * @retval none
 */
void BSP_CAM_STBCmd(uint8_t Cmd)
{
    BSP_IO_WritePortPin(CAM_PORT, CAM_STB_PIN, Cmd);
}

/**
 * @brief  LCD ctrl IO initialize.
 * @param  None
 * @retval none
 */
void BSP_LCD_IO_Init(void)
{
    /* Set initial state before output */
    BSP_IO_WritePortPin(LCD_RST_PORT, LCD_RST_PIN, EIO_PIN_SET);
    BSP_IO_WritePortPin(LCD_CTRST_PORT, LCD_CTRST_PIN, EIO_PIN_SET);
    /* LCD control IO pins set to output */
    BSP_IO_ConfigPortPin(LCD_RST_PORT, LCD_RST_PIN, EIO_DIR_OUT);
    BSP_IO_ConfigPortPin(LCD_CTRST_PORT, LCD_CTRST_PIN, EIO_DIR_OUT);
}

/**
 * @brief  CAN PYH STB pin initialization.
 * @param  None
 * @retval none
 */
void BSP_CAN_STB_IO_Init(void)
{
    /* Set STB pin high before output */
    BSP_IO_WritePortPin(CAN_STB_PORT, CAN_STB_PIN, EIO_PIN_SET);
    /* STB pin set to output */
    BSP_IO_ConfigPortPin(CAN_STB_PORT, CAN_STB_PIN, EIO_DIR_OUT);
}

/**
 * @brief  CAN PYH STB pin control
 * @param  [in] Cmd
 *   @arg  EIO_PIN_SET
 *   @arg  EIO_PIN_RESET
 * @retval none
 */
void BSP_CAN_STBCmd(uint8_t Cmd)
{
    BSP_IO_WritePortPin(CAN_STB_PORT, CAN_STB_PIN, Cmd);
}

/**
 * @brief  LCD reset pin config.
 * @param  [in] Cmd
 *   @arg  EIO_PIN_SET
 *   @arg  EIO_PIN_RESET
 * @retval none
 */
void BSP_LCD_RSTCmd(uint8_t Cmd)
{
    BSP_IO_WritePortPin(LCD_RST_PORT, LCD_RST_PIN, Cmd);
}

/**
 * @brief  LCD backlight pin config.
 * @param  [in] Cmd
 *   @arg  EIO_PIN_SET
 *   @arg  EIO_PIN_RESET
 * @retval none
 */
void BSP_LCD_BKLCmd(uint8_t Cmd)
{
    // todo
}

/**
 * @brief  LED initialize.
 * @param  None
 * @retval none
 */
void BSP_LED_Init(void)
{
    /* Turn off LED before output */
    BSP_IO_WritePortPin(LED_PORT, (LED_RED_PIN | LED_YELLOW_PIN | LED_BLUE_PIN), LED_OFF);
    /* LED pins set to output */
    BSP_IO_ConfigPortPin(LED_PORT, (LED_RED_PIN | LED_YELLOW_PIN |LED_BLUE_PIN), EIO_DIR_OUT);
}

/**
 * @brief  Turn on LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval none
 */
void BSP_LED_On(uint8_t u8Led)
{
    BSP_IO_WritePortPin(LED_PORT, u8Led, LED_ON);
}

/**
 * @brief  Turn off LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval none
 */
void BSP_LED_Off(uint8_t u8Led)
{
    BSP_IO_WritePortPin(LED_PORT, u8Led, LED_OFF);
}

/**
 * @brief  Toggle LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval none
 */
void BSP_LED_Toggle(uint8_t u8Led)
{
    BSP_IO_TogglePortPin(LED_PORT, u8Led);
}

/**
 * @brief  EXINT Ch.8 as BSP Key row 0 callback function
 * @param  None
 * @retval None
 */
static void BSP_KEY_ROW0_IrqCallback(void)
{
    const uint32_t u32Idx = KEYSCAN_GetKeyoutIdx();
    if (Set == EXINT_GetExIntSrc(BSP_KEY_ROW0_EXINT))
    {
        while (1)
        {
            if (Pin_Reset == GPIO_ReadInputPins(BSP_KEYIN0_PORT, BSP_KEYIN0_PIN))
            {
                gu32GlobalKey |= (0x01UL) << u32Idx;
            }
            else
            {
                /* clear int request flag  after KEY released */
                EXINT_ClrExIntSrc(BSP_KEY_ROW0_EXINT);
                break;
            }
        }
    }
}

/**
 * @brief  EXINT Ch.3 as BSP Key row 1 callback function
 * @param  None
 * @retval None
 */
static void BSP_KEY_ROW1_IrqCallback(void)
{
    const uint32_t u32Idx = KEYSCAN_GetKeyoutIdx();
    if (Set == EXINT_GetExIntSrc(BSP_KEY_ROW1_EXINT))
    {
        while (1)
        {
            if (Pin_Reset == GPIO_ReadInputPins(BSP_KEYIN1_PORT, BSP_KEYIN1_PIN))
            {
                gu32GlobalKey |= (0x10UL) << u32Idx;
            }
            else
            {
                /* clear int request flag after KEY released */
                EXINT_ClrExIntSrc(BSP_KEY_ROW1_EXINT);
                break;
            }
        }
    }
}

/**
 * @brief  EXINT Ch.7 as KYESCAN row 2 callback function
 * @param  None
 * @retval None
 */
static void BSP_KEY_ROW2_IrqCallback(void)
{
    const uint32_t u32Idx = KEYSCAN_GetKeyoutIdx();
    if (Set == EXINT_GetExIntSrc(BSP_KEY_ROW2_EXINT))
    {
        while (1)
        {
            if (Pin_Reset == GPIO_ReadInputPins(BSP_KEYIN2_PORT, BSP_KEYIN2_PIN))
            {
                gu32GlobalKey |= (0x100UL) << u32Idx;
            }
            else
            {
                /* clear int request flag after KEY released */
                EXINT_ClrExIntSrc(BSP_KEY_ROW2_EXINT);
                break;
            }
        }
    }
}

/**
 * @brief  BSP key row 0 initialize
 * @param  None
 * @retval None
 */
static void BSP_KEY_ROW0_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(BSP_KEYIN0_PORT, BSP_KEYIN0_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = BSP_KEY_ROW0_EXINT;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = BSP_KEY_ROW0_INT_SRC;
    stcIrqSignConfig.enIRQn   = BSP_KEY_ROW0_IRQn;
    stcIrqSignConfig.pfnCallback = &BSP_KEY_ROW0_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(BSP_KEY_ROW0_IRQn);
    NVIC_SetPriority(BSP_KEY_ROW0_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(BSP_KEY_ROW0_IRQn);
}

/**
 * @brief  BSP key row 1 initialize
 * @param  None
 * @retval None
 */
static void BSP_KEY_ROW1_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(BSP_KEYIN1_PORT, BSP_KEYIN1_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = BSP_KEY_ROW1_EXINT;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = BSP_KEY_ROW1_INT_SRC;
    stcIrqSignConfig.enIRQn   = BSP_KEY_ROW1_IRQn;
    stcIrqSignConfig.pfnCallback = &BSP_KEY_ROW1_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(BSP_KEY_ROW1_IRQn);
    NVIC_SetPriority(BSP_KEY_ROW1_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(BSP_KEY_ROW1_IRQn);
}

/**
 * @brief  BSP key row 2 initialize
 * @param  None
 * @retval None
 */
static void BSP_KEY_ROW2_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(BSP_KEYIN2_PORT, BSP_KEYIN2_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = BSP_KEY_ROW2_EXINT;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = BSP_KEY_ROW2_INT_SRC;
    stcIrqSignConfig.enIRQn   = BSP_KEY_ROW2_IRQn;
    stcIrqSignConfig.pfnCallback = &BSP_KEY_ROW2_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(BSP_KEY_ROW2_IRQn);
    NVIC_SetPriority(BSP_KEY_ROW2_IRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(BSP_KEY_ROW2_IRQn);
}

/**
 * @brief  BSP key column initialize
 * @param  None
 * @retval None
 */
static void BSP_KEY_COL_Init(void)
{
    stc_gpio_init_t stcGpioInit;
    stc_keyscan_init_t stcKeyscanInit;

    GPIO_StructInit(&stcGpioInit);
    KEYSCAN_StructInit(&stcKeyscanInit);

//    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(BSP_KEYOUT0_PORT, BSP_KEYOUT0_PIN, &stcGpioInit);
    GPIO_SetFunc(BSP_KEYOUT0_PORT, BSP_KEYOUT0_PIN, GPIO_FUNC_8_KEYSCAN, PIN_SUBFUNC_DISABLE);

    GPIO_Init(BSP_KEYOUT1_PORT, BSP_KEYOUT1_PIN, &stcGpioInit);
    GPIO_SetFunc(BSP_KEYOUT1_PORT, BSP_KEYOUT1_PIN, GPIO_FUNC_8_KEYSCAN, PIN_SUBFUNC_DISABLE);

    GPIO_Init(BSP_KEYOUT2_PORT, BSP_KEYOUT2_PIN, &stcGpioInit);
    GPIO_SetFunc(BSP_KEYOUT2_PORT, BSP_KEYOUT2_PIN, GPIO_FUNC_8_KEYSCAN, PIN_SUBFUNC_DISABLE);

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_KEY, Enable);

    /* Enable LRC for scan clock */
    CLK_LrcCmd(Enable);

    stcKeyscanInit.u32HizCycle = KEYSCAN_HIZ_CLC_4;
    stcKeyscanInit.u32LowCycle = KEYSCAN_LOW_CLC_512;
    stcKeyscanInit.u32KeyClk   = KEYSCAN_CLK_LRC;
    stcKeyscanInit.u32KeyOut   = KEYSCAN_OUT_0T2;
    stcKeyscanInit.u32KeyIn    = (KEYSCAN_IN_8 | KEYSCAN_IN_3 | KEYSCAN_IN_7);

    KEYSCAN_Init(&stcKeyscanInit);
}

/**
 * @brief  BSP key initialize
 * @param  None
 * @retval None
 */
void BSP_KEY_Init(void)
{
    BSP_KEY_ROW0_Init();
    BSP_KEY_ROW1_Init();
    BSP_KEY_ROW2_Init();

    BSP_KEY_COL_Init();

    /* Clear all KEYIN interrupt flag before enable */
    EXINT_ClrExIntSrc(BSP_KEY_ROW0_EXINT);
    EXINT_ClrExIntSrc(BSP_KEY_ROW1_EXINT);
    EXINT_ClrExIntSrc(BSP_KEY_ROW2_EXINT);

    KEYSCAN_Cmd(Enable);
}

/**
 * @brief  Get BSP key status
 * @param  [in] u32Key chose one macro from below
 *   @arg  BSP_KEY_1
 *   @arg  BSP_KEY_2
 *   @arg  BSP_KEY_3
 *   @arg  BSP_KEY_4
 *   @arg  BSP_KEY_5
 *   @arg  BSP_KEY_6
 *   @arg  BSP_KEY_7
 *   @arg  BSP_KEY_8
 *   @arg  BSP_KEY_9
 * @retval en_flag_status_t
 *   @arg  Set, Key pressed.
 *   @arg  Reset, Key released.
 */
en_flag_status_t BSP_KEY_GetStatus(uint32_t u32Key)
{
    en_flag_status_t enRet = Reset;
    if (0UL != (gu32GlobalKey & u32Key))
    {
        enRet = Set;
        gu32GlobalKey &= ~u32Key;
    }
    else
    {
    }
    return enRet;
}

/**
 * @brief  BSP clock initialize.
 *         Set board system clock to PLLH@240MHz
 *         Flash: 5 wait
 *         SRAM_HS: 1 wait
 *         SRAM1_2_3_4_B: 2 wait
 *         PCLK0: 240MHz
 *         PCLK1: 120MHz
 *         PCLK2: 60MHz
 *         PCLK3: 60MHz
 *         PCLK4: 120MHz
 *         EXCLK: 120MHz
 *         HCLK:  240MHz
 * @param  None
 * @retval None
 */
void BSP_CLK_Init(void)
{
    stc_clk_pllh_init_t stcPLLHInit;

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EX BUS Max 120MHz */
    CLK_ClkDiv(CLK_CATE_ALL,                                                    \
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 |              \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 |              \
                CLK_HCLK_DIV1));

    CLK_PLLHStrucInit(&stcPLLHInit);
    /* VCO = (8/1)*120 = 960MHz*/
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = 1UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLN = 120UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLP = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLQ = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLR = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL;
    CLK_PLLHInit(&stcPLLHInit);

    /* Highspeed SRAM set to 1 Read/Write wait cycle */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);

    /* SRAM1_2_3_4_backup set to 2 Read/Write wait cycle */
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    /* 0-wait @ 40MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);

    /* 4 cycles for 200 ~ 250MHz */
    GPIO_SetReadWaitCycle(GPIO_READ_WAIT_4);

    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
}

/**
 * @}
 */

#endif /* BSP_EV_HC32F4A0_LQFP176 */

/**
 * @}
 */

/**
 * @}
 */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
