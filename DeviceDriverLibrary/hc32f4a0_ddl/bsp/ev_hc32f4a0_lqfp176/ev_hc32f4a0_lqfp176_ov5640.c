/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_ov5640.c
 * @brief This file provides firmware functions for Camera.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-08-20       Zhangxl         First version
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
#include "ev_hc32f4a0_lqfp176_ov5640.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */

/** @defgroup EV_HC32F4A0_LQFP176_OV5640 HC32F4A0 EVB OV5640
 * @{
 */

#if (BSP_OV5640_ENABLE == BSP_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_OV5640_Local_Macros HC32F4A0 EVB LQFP176 OV5640 Local Macros
 * @{
 */

/**
 * @defgroup DVP_Interface_Pin DVP Interface Pin
 * @{
 */
#define DVP_PIXCLK_PORT                         (GPIO_PORT_F)   /* PF09 - DVP_PIXCLK */
#define DVP_PIXCLK_PIN                          (GPIO_PIN_09)

#define DVP_HSYNC_PORT                          (GPIO_PORT_H)   /* PH08 - DVP_HSYNC */
#define DVP_HSYNC_PIN                           (GPIO_PIN_08)   /* Line sync */

#define DVP_VSYNC_PORT                          (GPIO_PORT_I)   /* PI05 - DVP_VSYNC */
#define DVP_VSYNC_PIN                           (GPIO_PIN_05)   /* Frame sync */

#define DVP_DATA0_PORT                          (GPIO_PORT_H)   /* PH09 - DVP_DATA0 */
#define DVP_DATA0_PIN                           (GPIO_PIN_09)
#define DVP_DATA1_PORT                          (GPIO_PORT_H)   /* PH10 - DVP_DATA1 */
#define DVP_DATA1_PIN                           (GPIO_PIN_10)
#define DVP_DATA2_PORT                          (GPIO_PORT_H)   /* PH11 - DVP_DATA2 */
#define DVP_DATA2_PIN                           (GPIO_PIN_11)
#define DVP_DATA3_PORT                          (GPIO_PORT_H)   /* PH12 - DVP_DATA3 */
#define DVP_DATA3_PIN                           (GPIO_PIN_12)
#define DVP_DATA4_PORT                          (GPIO_PORT_H)   /* PH14 - DVP_DATA4 */
#define DVP_DATA4_PIN                           (GPIO_PIN_14)
#define DVP_DATA5_PORT                          (GPIO_PORT_I)   /* PI04 - DVP_DATA5 */
#define DVP_DATA5_PIN                           (GPIO_PIN_04)
#define DVP_DATA6_PORT                          (GPIO_PORT_I)   /* PI06 - DVP_DATA6 */
#define DVP_DATA6_PIN                           (GPIO_PIN_06)
#define DVP_DATA7_PORT                          (GPIO_PORT_I)   /* PI07 - DVP_DATA7 */
#define DVP_DATA7_PIN                           (GPIO_PIN_07)
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @addtogroup EV_HC32F4A0_LQFP176_OV5640_Local_Functions
 * @{
 */
static void CAM_Port_Init(void);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_OV5640_Global_Functions HC32F4A0 LQFP176 EVB OV5640 Global Functions
 * @{
 */

/**
 * @brief  Initializes Camera low level.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 */
en_result_t BSP_OV5640_Init(void)
{
    /* Initialize DVP port */
    CAM_Port_Init();

    return Ok;
}

/**
 * @brief  Read register on Camera register.
 * @param  [in]  u16Reg:                Register to be read
 * @param  [out] pu8RxBuf               The buffer for reading
 * @param  [in]  u32Len                 The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_OV5640_ReadReg(uint16_t u16Reg,
                                uint8_t *pu8RxBuf,
                                uint32_t u32Len)
{
    uint16_t u16Temp;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pu8RxBuf != NULL) && (u32Len > 0UL))
    {
        I2C_Start(OV5640_I2C_CH, OV5640_TIMEOUT);

        I2C_SendAddr(OV5640_I2C_CH, (OV5640_ADDR | BSP_I2C_WR), OV5640_TIMEOUT);

        u16Temp = ((u16Reg & 0xFFU) << 8U) | ((u16Reg & 0xFF00U) >> 8U);
        I2C_SendData(OV5640_I2C_CH, (uint8_t*)((uint32_t)&u16Temp), 2UL, OV5640_TIMEOUT);

        I2C_Restart(OV5640_I2C_CH, OV5640_TIMEOUT);

        I2C_SendAddr(OV5640_I2C_CH, (OV5640_ADDR | BSP_I2C_RD), OV5640_TIMEOUT);

        I2C_RcvData(OV5640_I2C_CH, pu8RxBuf, u32Len, OV5640_TIMEOUT);

        I2C_Stop(OV5640_I2C_CH, OV5640_TIMEOUT);
    }

    return enRet;
}

/**
 * @brief  Writes register on Camera register.
 * @param  [in]  u16Reg:                Register to be written
 * @param  [out] pu8RxBuf               The buffer for reading
 * @param  [in]  u32Len                 The buffer size for bytes
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The paramter is invalid.
 */
en_result_t BSP_OV5640_WriteReg(uint16_t u16Reg,
                                const uint8_t *pu8TxBuf,
                                uint32_t u32Len)
{
    uint16_t u16Temp;
    en_result_t enRet = ErrorInvalidParameter;

    if ((pu8TxBuf != NULL) && (u32Len > 0UL))
    {
        I2C_Start(OV5640_I2C_CH, OV5640_TIMEOUT);

        I2C_SendAddr(OV5640_I2C_CH, (OV5640_ADDR | BSP_I2C_WR), OV5640_TIMEOUT);

        u16Temp = ((u16Reg & 0xFFU) << 8U) | ((u16Reg & 0xFF00U) >> 8U);
        I2C_SendData(OV5640_I2C_CH, (uint8_t *)((uint32_t)&u16Temp), 2UL, OV5640_TIMEOUT);

        I2C_SendData(OV5640_I2C_CH, pu8TxBuf, u32Len, OV5640_TIMEOUT);

        I2C_Stop(OV5640_I2C_CH, OV5640_TIMEOUT);
    }

    return enRet;
}

/**
 * @}
 */

/**
 * @defgroup EV_HC32F4A0_LQFP176_OV5640_Local_Functions HC32F4A0 LQFP176 EVB OV5640 Local Functions
 * @{
 */

/**
 * @brief  Initializes Camera gpio.
 * @param  None
 * @retval None
 */
static void CAM_Port_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinIType = PIN_ITYPE_CMOS;

    /* DVP_PIXCLK */
    GPIO_Init(DVP_PIXCLK_PORT, DVP_PIXCLK_PIN, &stcGpioInit);

    /* DVP_HSYNC */
    GPIO_Init(DVP_HSYNC_PORT, DVP_HSYNC_PIN, &stcGpioInit);

    /* DVP_VSYNC */
    GPIO_Init(DVP_VSYNC_PORT, DVP_VSYNC_PIN, &stcGpioInit);

    /* DVP_DATA[0:7] */
    GPIO_Init(DVP_DATA0_PORT,  DVP_DATA0_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA1_PORT,  DVP_DATA1_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA2_PORT,  DVP_DATA2_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA3_PORT,  DVP_DATA3_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA4_PORT,  DVP_DATA4_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA5_PORT,  DVP_DATA5_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA6_PORT,  DVP_DATA6_PIN, &stcGpioInit);
    GPIO_Init(DVP_DATA7_PORT,  DVP_DATA7_PIN, &stcGpioInit);

    /* DVP_DATA[0:7] */
    GPIO_SetFunc(DVP_DATA0_PORT,  DVP_DATA0_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA1_PORT,  DVP_DATA1_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA2_PORT,  DVP_DATA2_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA3_PORT,  DVP_DATA3_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA4_PORT,  DVP_DATA4_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA5_PORT,  DVP_DATA5_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA6_PORT,  DVP_DATA6_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_DATA7_PORT,  DVP_DATA7_PIN,  GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(DVP_PIXCLK_PORT, DVP_PIXCLK_PIN, GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_HSYNC_PORT, DVP_HSYNC_PIN, GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DVP_VSYNC_PORT, DVP_VSYNC_PIN, GPIO_FUNC_13_DVP, PIN_SUBFUNC_DISABLE);
}

/**
 * @}
 */

#endif /* BSP_OV5640_ENABLE */

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
