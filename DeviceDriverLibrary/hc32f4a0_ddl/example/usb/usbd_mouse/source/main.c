/**
 *******************************************************************************
 * @file  usb/usbd_mouse/source/main.c
 * @brief Main program of USBD mouse example.
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
#include "usbd_desc.h"
#include "usb_bsp.h"


/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USBD_MOUSE
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  The button typedef.
 **
 ******************************************************************************/
typedef enum
{
    BUTTON_NULL = 1U,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_UP,
    BUTTON_DOWN,
}Button_TypeDef;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CURSOR_STEP     10U

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
USB_OTG_CORE_HANDLE  USB_OTG_dev;
__IO uint32_t test = 0UL;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief Key status read function
 * @param  None
 * @retval Button id
 */
Button_TypeDef Key_ReadIOPin_continuous(void)
{

    Button_TypeDef enKey = BUTTON_NULL;

    if (Set == BSP_KEY_GetStatus(BSP_KEY_4))
    {
        enKey = BUTTON_LEFT;
    }
    else if (Set == BSP_KEY_GetStatus(BSP_KEY_6))
    {
        enKey = BUTTON_RIGHT;
    }
    else if (Set == BSP_KEY_GetStatus(BSP_KEY_8))
    {
        enKey = BUTTON_DOWN;
    }
    else if (Set == BSP_KEY_GetStatus(BSP_KEY_2))
    {
        enKey = BUTTON_UP;
    }
    else
    {
        enKey = BUTTON_NULL;
    }

    return enKey;
}

/**
 * @brief USB HID device get position function
 * @param [in]  None
 * @retval Pointer to report
 */
static uint8_t* USBD_HID_GetPos (void)
{
    int8_t  x = (int8_t)0, y = (int8_t)0;
    static uint8_t HID_Buffer [4U];

    switch (Key_ReadIOPin_continuous())
    {
        case BUTTON_LEFT:
            x -= (int8_t)CURSOR_STEP;
            break;
        case BUTTON_RIGHT:
            x += (int8_t)CURSOR_STEP;
            break;
        case BUTTON_DOWN:
            y += (int8_t)CURSOR_STEP;
            break;
        case BUTTON_UP:
            y -= (int8_t)CURSOR_STEP;
            break;
        default:
            break;
    }
    HID_Buffer[0U] = (uint8_t)0;
    HID_Buffer[1U] = (uint8_t)x;
    HID_Buffer[2U] = (uint8_t)y;
    HID_Buffer[3U] = (uint8_t)0;

    return HID_Buffer;
}
/**
 * @brief SysTick IRQ function that get mouse position and report it
 * @param [in]  None
 * @retval None
 */
void SysTick_IrqHandler(void)
{
    uint8_t *buf;

    buf = USBD_HID_GetPos();
    if((buf[1U] != 0U) ||(buf[2U] != 0U))
    {
        USBD_HID_SendReport (&USB_OTG_dev, buf, 4U);
    }
}
/**
 * @brief  main function
 * @param [in]  None
 * @retval int32_t Return value, if needed
 */
int32_t main (void)
{
    USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_FS
              USB_OTG_FS_CORE_ID,
#else
              USB_OTG_HS_CORE_ID,
#endif
              &USR_desc,
              &USBD_HID_cb,
              &USR_cb);

    while (1)
    {
        /* remote wakeup test */
        if(test == 0x1UL)
        {
            USB_OTG_ActiveRemoteWakeup(&USB_OTG_dev);
            test  = 0UL;
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

