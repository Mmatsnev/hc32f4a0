/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_tca9539.h
 * @brief This file contains all the functions prototypes of the 
 *        ev_hc32f4a0_lqfp176_tca9539 driver library.
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
#ifndef __EV_HC32F4A0_LQFP176_TCA9539_H__
#define __EV_HC32F4A0_LQFP176_TCA9539_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ev_hc32f4a0_lqfp176.h"
#include "tca9539.h"

/**
 * @addtogroup BSP
 * @{
 */

/** @addtogroup EV_HC32F4A0_LQFP176
  * @{
  */

/** @addtogroup EV_HC32F4A0_LQFP176_TCA9539
  * @{
  */

#if ((BSP_TCA9539_ENABLE == BSP_ON) && (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/** @defgroup HC32F4A0_EV_IO_Exported_Constants IO Exported Constants
  * @{
  */
#define EIO_PORT0               ((uint8_t)0x00U)
#define EIO_PORT1               ((uint8_t)0x01U)

#define EIO_PIN0                ((uint8_t)0x01U)
#define EIO_PIN1                ((uint8_t)0x02U)
#define EIO_PIN2                ((uint8_t)0x04U)
#define EIO_PIN3                ((uint8_t)0x08U)
#define EIO_PIN4                ((uint8_t)0x10U)
#define EIO_PIN5                ((uint8_t)0x20U)
#define EIO_PIN6                ((uint8_t)0x40U)
#define EIO_PIN7                ((uint8_t)0x80U)
#define EIO_PIN_ALL             ((uint8_t)0xFFU)

#define EIO_DIR_OUT             ((uint8_t)0x00U)
#define EIO_DIR_IN              ((uint8_t)0x01U)

#define EIO_PIN_RESET            ((uint8_t)0x00U)
#define EIO_PIN_SET              ((uint8_t)0x01U)

/**
 * @}
 */

/** @defgroup HC32F4A0_EV_IO_Function_Sel Expand IO function definition
 * @{
 */
#define EIO_USBFS_OC            (EIO_PIN0)      /* USBFS over-current, input */
#define EIO_USBHS_OC            (EIO_PIN1)      /* USBHS over-current, input */
#define EIO_SDIC1_CD            (EIO_PIN2)      /* SDIC1 card detect, input */
#define EIO_SCI_CD              (EIO_PIN3)      /* Smart card detect, input */
#define EIO_TOUCH_INT           (EIO_PIN4)      /* Touch screen interrupt, input */
#define EIO_LIN_SLEEP           (EIO_PIN5)      /* LIN PHY sleep, output */
#define EIO_RTCS_CTRST          (EIO_PIN6)      /* 'CS' for Resistor touch panel or 'Reset' for Cap touch panel, output */
#define EIO_LCD_RST             (EIO_PIN7)      /* LCD panel reset, output */

#define EIO_CAM_RST             (EIO_PIN0)      /* Camera module reset, output */
#define EIO_CAM_STB             (EIO_PIN1)      /* Camera module standby, output */
#define EIO_USB3300_RST         (EIO_PIN2)      /* USBHS PHY USB3300 reset, output */
#define EIO_ETH_RST             (EIO_PIN3)      /* ETH PHY reset, output */
#define EIO_CAN_STB             (EIO_PIN4)      /* CAN PHY standby, output */
#define EIO_LED_RED             (EIO_PIN5)      /* Red LED, output */
#define EIO_LED_YELLOW          (EIO_PIN6)      /* Yellow LED, output */
#define EIO_LED_BLUE            (EIO_PIN7)      /* Blue LED, output */
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
 * @defgroup EV_HC32F4A0_LQFP176_TCA9539_Exported_Functions HC32F4A0 LQFP176 EVB TCA9539 Exported Functions
 * @{
 */
void BSP_IO_Init(void);
void BSP_IO_IntInit(void);
void BSP_IO_WritePortPin(uint8_t u8Port, uint8_t u8Pin, uint8_t u8PinState);
void BSP_IO_ConfigPortPin(uint8_t u8Port, uint8_t u8Pin, uint8_t u8Dir);
uint8_t BSP_IO_ReadPortPin(uint8_t u8Port, uint8_t u8Pin);
void BSP_IO_TogglePortPin(uint8_t u8Port, uint8_t u8Pin);
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

#ifdef __cplusplus
}
#endif

#endif /* __EV_HC32F4A0_LQFP176_TCA9539__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
