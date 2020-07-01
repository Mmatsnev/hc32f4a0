/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_tca9539.c
 * @brief This file provides firmware functions for IO expand IC TCA9539.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Zhangxl         First version
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
#include "ev_hc32f4a0_lqfp176_tca9539.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */

/** @defgroup EV_HC32F4A0_LQFP176_TCA9539 HC32F4A0 EVB LQFP176 TCA9539
  * @{
  */

#if ((BSP_TCA9539_ENABLE == BSP_ON) && (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0))

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
 * @defgroup EV_HC32F4A0_LQFP176_TCA9539_Global_Functions HC32F4A0 LQFP176 EVB TCA9539 Global Functions
 * @{
 */

/**
 * @brief  Expand IO initialize.
 * @param  None
 * @retval None
 */
void BSP_IO_Init(void)
{
    /* I2C channel init */
    TCA9539_Init();
    /* all Pins are input as default */
    TCA9539_WriteReg(TCA9539_REG_CONFIG_0, 0xFFU);
    TCA9539_WriteReg(TCA9539_REG_CONFIG_1, 0xFFU);
}

/**
 * @brief  Expand IO interrupt initialize.
 * @param  None
 * @retval None
 */
void BSP_IO_IntInit(void)
{
    TCA9539_IntInit();
}

/**
  * @brief  Set EIO port pin output value
  * @param  [in] u8Port Port
  *   @arg  EIO_PORT0
  *   @arg  EIO_PORT1
  * @param  [in] u8Pin Pin
  *   @arg  EIO_PIN0
  *   @arg  EIO_PIN1
  *   @arg  EIO_PIN2
  *   @arg  EIO_PIN3
  *   @arg  EIO_PIN4
  *   @arg  EIO_PIN5
  *   @arg  EIO_PIN6
  *   @arg  EIO_PIN7
  *   @arg  EIO_PIN_ALL
  * @param  [in] u8PinState Pin state to be written
  * @retval None
  */
void BSP_IO_WritePortPin(uint8_t u8Port, uint8_t u8Pin, uint8_t u8PinState)
{
    uint8_t u8PortVal;
    const uint8_t u8POReg = u8Port + TCA9539_REG_OUTPUT_0;

    TCA9539_ReadReg(u8POReg, &u8PortVal);

    if (0U == u8PinState)
    {
        u8PortVal &= (uint8_t)(~u8Pin);
    }
    else
    {
        u8PortVal |= u8Pin;
    }

    TCA9539_WriteReg(u8POReg, u8PortVal);
}

/**
  * @brief  Get EIO port pin input value
  * @param  [in] u8Port Port
  *   @arg  EIO_PORT0
  *   @arg  EIO_PORT1
  * @param  [in] u8Pin Pin
  *   @arg  EIO_PIN0
  *   @arg  EIO_PIN1
  *   @arg  EIO_PIN2
  *   @arg  EIO_PIN3
  *   @arg  EIO_PIN4
  *   @arg  EIO_PIN5
  *   @arg  EIO_PIN6
  *   @arg  EIO_PIN7
  * @retval Pin state
  */
uint8_t BSP_IO_ReadPortPin(uint8_t u8Port, uint8_t u8Pin)
{
    uint8_t u8PortVal;
    uint8_t u8PinVal;
    const uint8_t u8PIReg = u8Port + TCA9539_REG_INPUT_0;

    TCA9539_ReadReg(u8PIReg, &u8PortVal);

    if (0U != (u8Pin & u8PortVal))
    {
        u8PinVal = 1U;
    }
    else
    {
        u8PinVal = 0U;
    }
    return u8PinVal;
}

/**
  * @brief  Toggle EIO port pin
  * @param  [in] u8Port Port
  *   @arg  EIO_PORT0
  *   @arg  EIO_PORT1
  * @param  [in] u8Pin Pin
  *   @arg  EIO_PIN0
  *   @arg  EIO_PIN1
  *   @arg  EIO_PIN2
  *   @arg  EIO_PIN3
  *   @arg  EIO_PIN4
  *   @arg  EIO_PIN5
  *   @arg  EIO_PIN6
  *   @arg  EIO_PIN7
  *   @arg  EIO_PIN_ALL
  * @retval None
  */
void BSP_IO_TogglePortPin(uint8_t u8Port, uint8_t u8Pin)
{
    uint8_t u8PortVal;
    const uint8_t u8POReg = u8Port + TCA9539_REG_OUTPUT_0;

    TCA9539_ReadReg(u8POReg, &u8PortVal);

    u8PortVal ^= u8Pin;

    TCA9539_WriteReg(u8POReg, u8PortVal);
}

/**
  * @brief  Config EIO port pin direction
  * @param  [in] u8Port Port
  *   @arg  EIO_PORT0
  *   @arg  EIO_PORT1
  * @param  [in] u8Pin Pin
  *   @arg  EIO_PIN0
  *   @arg  EIO_PIN1
  *   @arg  EIO_PIN2
  *   @arg  EIO_PIN3
  *   @arg  EIO_PIN4
  *   @arg  EIO_PIN5
  *   @arg  EIO_PIN6
  *   @arg  EIO_PIN7
  *   @arg  EIO_PIN_ALL
  * @param  [in] u8Dir Direction
  *   @arg  EIO_DIR_IN
  *   @arg  EIO_DIR_OUT
  * @retval None
  */
void BSP_IO_ConfigPortPin(uint8_t u8Port, uint8_t u8Pin, uint8_t u8Dir)
{
    uint8_t u8ConfigVal;
    const uint8_t u8ConfigReg = u8Port + TCA9539_REG_CONFIG_0;

    TCA9539_ReadReg(u8ConfigReg, &u8ConfigVal);

    if (EIO_DIR_OUT == u8Dir)
    {
        u8ConfigVal &= (uint8_t)(~u8Pin);
    }
    else
    {
        u8ConfigVal |= u8Pin;
    }
    TCA9539_WriteReg(u8ConfigReg, u8ConfigVal);

}

/**
 * @}
 */

#endif /* BSP_EV_HC32F4A0_LQFP176_TCA9539_ENABLE */

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
