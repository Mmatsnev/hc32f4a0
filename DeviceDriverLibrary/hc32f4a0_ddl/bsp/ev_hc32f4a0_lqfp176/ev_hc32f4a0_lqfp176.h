/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176.h
 * @brief This file contains all the functions prototypes of the
 *        EV_HC32F4A0_LQFP176 BSP driver library.
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
#ifndef __EV_HC32F4A0_LQFP176_H__
#define __EV_HC32F4A0_LQFP176_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "hc32f4a0_clk.h"
#include "hc32f4a0_efm.h"
#include "hc32f4a0_gpio.h"
#include "hc32f4a0_i2c.h"
#include "hc32f4a0_icg.h"
#include "hc32f4a0_interrupts.h"
#include "hc32f4a0_pwc.h"
#include "hc32f4a0_sram.h"
#include "hc32f4a0_utility.h"

#include "hc32f4a0_keyscan.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */


#if (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup BSP_Global_Macros BSP Global Macros
 * @{
 */
/** @defgroup BSP_KEY_config BSP Key config definition
 * @{
 */
#define BSP_KEY_ROW0_EXINT      (EXINT_CH08)
#define BSP_KEY_ROW1_EXINT      (EXINT_CH03)
#define BSP_KEY_ROW2_EXINT      (EXINT_CH07)

#define BSP_KEY_ROW0_IRQn       (Int029_IRQn)
#define BSP_KEY_ROW1_IRQn       (Int030_IRQn)
#define BSP_KEY_ROW2_IRQn       (Int031_IRQn)

#define BSP_KEY_ROW0_INT_SRC    (INT_PORT_EIRQ8)
#define BSP_KEY_ROW1_INT_SRC    (INT_PORT_EIRQ3)
#define BSP_KEY_ROW2_INT_SRC    (INT_PORT_EIRQ7)

#define BSP_KEYOUT0_PORT        (GPIO_PORT_C)
#define BSP_KEYOUT0_PIN         (GPIO_PIN_11)
#define BSP_KEYOUT1_PORT        (GPIO_PORT_C)
#define BSP_KEYOUT1_PIN         (GPIO_PIN_08)
#define BSP_KEYOUT2_PORT        (GPIO_PORT_A)
#define BSP_KEYOUT2_PIN         (GPIO_PIN_06)

#define BSP_KEYIN0_PORT         (GPIO_PORT_I)
#define BSP_KEYIN0_PIN          (GPIO_PIN_08)
#define BSP_KEYIN1_PORT         (GPIO_PORT_I)
#define BSP_KEYIN1_PIN          (GPIO_PIN_03)
#define BSP_KEYIN2_PORT         (GPIO_PORT_H)
#define BSP_KEYIN2_PIN          (GPIO_PIN_07)
/**
 * @}
 */

/** @defgroup BSP_KEY_Sel BSP Key definition
 * @{
 */
#define BSP_KEY_1               ((uint32_t)0x0001UL)    /*!< BSP KEY 1 */
#define BSP_KEY_2               ((uint32_t)0x0002UL)    /*!< BSP KEY 2 */
#define BSP_KEY_3               ((uint32_t)0x0004UL)    /*!< BSP KEY 3 */
#define BSP_KEY_4               ((uint32_t)0x0010UL)    /*!< BSP KEY 4 */
#define BSP_KEY_5               ((uint32_t)0x0020UL)    /*!< BSP KEY 5 */
#define BSP_KEY_6               ((uint32_t)0x0040UL)    /*!< BSP KEY 6 */
#define BSP_KEY_7               ((uint32_t)0x0100UL)    /*!< BSP KEY 7 */
#define BSP_KEY_8               ((uint32_t)0x0200UL)    /*!< BSP KEY 8 */
#define BSP_KEY_9               ((uint32_t)0x0400UL)    /*!< BSP KEY 9 */
/**
 * @}
 */

/** @defgroup BSP_CAM_PortPin_Sel BSP Camera port/pin definition
 * @{
 */
#define CAM_PORT                (EIO_PORT1)
#define CAM_RST_PORT            (EIO_PORT1)
#define CAM_RST_PIN             (EIO_CAM_RST)
#define CAM_STB_PORT            (EIO_PORT1)
#define CAM_STB_PIN             (EIO_CAM_STB)
/**
 * @}
 */

/** @defgroup BSP_CAN_PortPin_Sel BSP CAN PHY STB port/pin definition
 * @{
 */
#define CAN_STB_PORT            (EIO_PORT1)
#define CAN_STB_PIN             (EIO_CAN_STB)
/**
 * @}
 */

/** @defgroup BSP_LCD_PortPin_Sel BSP LCD panel port/pin definition
 * @{
 */
#define LCD_RST_PORT            (EIO_PORT1)
#define LCD_RST_PIN             (EIO_LCD_RST)
#define LCD_RTCS_PORT           (EIO_PORT0)
#define LCD_RTCS_PIN            (EIO_RTCS_CTRST)
#define LCD_CTRST_PORT          (EIO_PORT0)
#define LCD_CTRST_PIN           (EIO_RTCS_CTRST)

/**
 * @}
 */

/** @defgroup BSP_LED_PortPin_Sel BSP LED port/pin definition
 * @{
 */
#define LED_PORT                (EIO_PORT1)
#define LED_RED_PORT            (EIO_PORT1)
#define LED_RED_PIN             (EIO_LED_RED)
#define LED_YELLOW_PORT         (EIO_PORT1)
#define LED_YELLOW_PIN          (EIO_LED_YELLOW)
#define LED_BLUE_PORT           (EIO_PORT1)
#define LED_BLUE_PIN            (EIO_LED_BLUE)
/**
 * @}
 */

/** @defgroup BSP_LED_Sel BSP LED definition
 * @{
 */
#define LED_RED                 (EIO_LED_RED)
#define LED_YELLOW              (EIO_LED_YELLOW)
#define LED_BLUE                (EIO_LED_BLUE)
/**
 * @}
 */

/** @defgroup BSP_LED_OnOff_Sel BSP LED ON/OFF definition
 * @{
 */
#define LED_OFF                 (EIO_PIN_RESET)
#define LED_ON                  (EIO_PIN_SET)
/**
 * @}
 */

/** @defgroup BSP_TCA9539_Config BSP TCA9539 expand IC I2C config
 * @{
 */
#define TCA9539_ADDR            ((uint8_t)0xE8U)
#define TCA9539_WRITE           ((uint8_t)0x00U)
#define TCA9539_READ            ((uint8_t)0x01U)

#define TCA9539_SCL_PORT        (GPIO_PORT_D)
#define TCA9539_SCL_PIN         (GPIO_PIN_03)
#define TCA9539_SDA_PORT        (GPIO_PORT_F)
#define TCA9539_SDA_PIN         (GPIO_PIN_10)

#define TCA9539_I2C_CH          (M4_I2C1)
#define TCA9539_BAUDRATE        (200000UL)
#define TCA9539_TIMEOUT         (10000UL)
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup BSP_Global_Functions
 * @{
 */
void BSP_LED_Init(void);
void BSP_LED_On(uint8_t u8Led);
void BSP_LED_Off(uint8_t u8Led);
void BSP_LED_Toggle(uint8_t u8Led);
void BSP_KEY_Init(void);
en_flag_status_t BSP_KEY_GetStatus(uint32_t u32Key);
void BSP_CLK_Init(void);
void BSP_CAM_Init(void);
void BSP_CAM_RSTCmd(uint8_t Cmd);
void BSP_CAM_STBCmd(uint8_t Cmd);

void BSP_CAN_STB_IO_Init(void);
void BSP_CAN_STBCmd(uint8_t Cmd);

void BSP_LCD_IO_Init(void);
void BSP_LCD_RSTCmd(uint8_t Cmd);
void BSP_LCD_BKLCmd(uint8_t Cmd);

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

#ifdef __cplusplus
}
#endif

#endif /* __EV_HC32F4A0_LQFP176_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
