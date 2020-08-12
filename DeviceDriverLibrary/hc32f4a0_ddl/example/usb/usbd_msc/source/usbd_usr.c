/**
 *******************************************************************************
 * @file  usb/usbd_msc/source/usbd_usr.c
 * @brief This file includes the user application layer.
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
#include "hc32_ddl.h"
#include "usbd_usr.h"
#include <stdio.h>
#include "usb_bsp.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USBD_MSC
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/* User callback functions */
USBD_Usr_cb_TypeDef USR_cb =
{
    &USBD_USR_Init,
    &USBD_USR_DeviceReset,
    &USBD_USR_DeviceConfigured,
    &USBD_USR_DeviceSuspended,
    &USBD_USR_DeviceResumed,
    &USBD_USR_DeviceConnected,
    &USBD_USR_DeviceDisconnected,
};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* Disconnected in default */
volatile uint8_t bDeviceState = 0U;

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
 * @brief  USBD_USR_Init
 * @param  None
 * @retval None
 */
void USBD_USR_Init(void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("USBD_USR_Init\r\n");
#endif
}

/**
 * @brief  USBD_USR_DeviceReset
 * @param  [in] speed : device speed
 * @retval None
 */
void USBD_USR_DeviceReset(uint8_t speed)
{
    switch (speed)
    {
        case USB_OTG_SPEED_HIGH:
#if (DDL_PRINT_ENABLE == DDL_ON)
            printf("USB Device Library v1.1.0  [HS]\r\n");
#endif
            break;
        case USB_OTG_SPEED_FULL:
#if (DDL_PRINT_ENABLE == DDL_ON)
            printf("USB Device Library v1.1.0  [FS]\r\n");
#endif
            break;
        default:
#if (DDL_PRINT_ENABLE == DDL_ON)
            printf("USB Device Library v1.1.0  [??]\r\n");
#endif
            break;
    }
}

/**
 * @brief  USBD_USR_DeviceConfigured
 * @param  None
 * @retval None
 */
void USBD_USR_DeviceConfigured (void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("MSC Interface started.\r\n");
#endif
}

/**
 * @brief  USBD_USR_DeviceSuspended
 * @param  None
 * @retval None
 */
void USBD_USR_DeviceSuspended(void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("Device In suspend mode.\r\n");
#endif
}

/**
 * @brief  USBD_USR_DeviceResumed
 * @param  None
 * @retval None
 */
void USBD_USR_DeviceResumed(void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("Device Resumed\r\n");
#endif
}

/**
 * @brief  USBD_USR_DeviceConnected
 * @param  None
 * @retval None
 */
void USBD_USR_DeviceConnected(void)
{
    bDeviceState = 1U;
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("USB Device Connected.\r\n");
#endif
}

/**
 * @brief  USBD_USR_DeviceDisonnected
 * @param  None
 * @retval None
 */
void USBD_USR_DeviceDisconnected(void)
{
    bDeviceState = 0U;
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("USB Device Disconnected.\r\n");
#endif
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
