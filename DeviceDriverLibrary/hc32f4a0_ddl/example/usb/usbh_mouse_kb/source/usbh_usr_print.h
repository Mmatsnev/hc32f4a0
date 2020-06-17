/**
 *******************************************************************************
 * @file  usb/usbh_mouse_kb/source/usbh_usr_print.h
 * @brief Headfile for usbh_usr_print.c.
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
#ifndef __USBH_USR_PRINT_H__
#define __USBH_USR_PRINT_H__


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbh_hid_core.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USBH_MOUSE_KB
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*Left Button : Report data :0x01*/
#define HID_MOUSE_BUTTON1                (0x01U)
/*Right Button : Report data :0x02*/
#define HID_MOUSE_BUTTON2                (0x02U )
/*Middle Button : Report data : 0x04*/
#define HID_MOUSE_BUTTON3                (0x04U)

/* Mouse directions */
#define MOUSE_TOP_DIR                   (0x80U)
#define MOUSE_BOTTOM_DIR                (0x00U)
#define MOUSE_LEFT_DIR                  (0x80U)
#define MOUSE_RIGHT_DIR                 (0x00U)

#define MOUSE_WINDOW_X                  (100U)
#define MOUSE_WINDOW_Y                  (220U)
#define MOUSE_WINDOW_X_MAX              (181U)
#define MOUSE_WINDOW_Y_MIN              (101U)
#define MOUSE_WINDOW_HEIGHT             (90U)
#define MOUSE_WINDOW_WIDTH              (128U)

#define HID_MOUSE_BUTTON_HEIGHT         (10U)
#define HID_MOUSE_BUTTON_WIDTH          (24U)
#define HID_MOUSE_BUTTON_XCHORD         (201U)
#define HID_MOUSE_BUTTON1_YCHORD        (220U)
#define HID_MOUSE_BUTTON3_YCHORD        (166U)
#define HID_MOUSE_BUTTON2_YCHORD        (116U)

#define MOUSE_LEFT_MOVE                  (1U)
#define MOUSE_RIGHT_MOVE                 (2U)
#define MOUSE_UP_MOVE                    (3U)
#define MOUSE_DOWN_MOVE                  (4U)

#define HID_MOUSE_HEIGHTLSB              (2U)
#define HID_MOUSE_WIDTHLSB               (2U)
#define HID_MOUSE_RES_X                  (4U)
#define HID_MOUSE_RES_Y                  (4U)

#define SMALL_FONT_COLUMN_WIDTH          (8U)
#define SMALL_FONT_LINE_WIDTH            (12U)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void HID_MOUSE_UpdatePosition(int8_t x ,int8_t y);
void HID_MOUSE_ButtonReleased(uint8_t button_idx);
void HID_MOUSE_ButtonPressed (uint8_t button_idx);

/**
 * @}
 */

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* __USBH_USR_PRINT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
