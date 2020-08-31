/**
 *******************************************************************************
 * @file  nt35510.h
 * @brief This file contains all the functions prototypes of the NT35510 driver
 *        library.
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
#ifndef __NT35510_H__
#define __NT35510_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/** @defgroup NT35510
  * @{
  */

#if (BSP_NT35510_ENABLE == BSP_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup NT35510_Global_Macros NT35510 Global Macros
 * @{
 */

/**
 * @defgroup LCD_Scan_Direction LCD Scan Direction
 * @{
 */
#define LCD_SCAN_DIR_L2R_U2D            (0U)    /* from left to right && from up to down */
#define LCD_SCAN_DIR_L2R_D2U            (1U)    /* from left to right && from down to up */
#define LCD_SCAN_DIR_R2L_U2D            (2U)    /* from right to left && from up to down */
#define LCD_SCAN_DIR_R2L_D2U            (3U)    /* from right to left && from down to up */
#define LCD_SCAN_DIR_U2D_L2R            (4U)    /* from up to down && from left to right */
#define LCD_SCAN_DIR_U2D_R2L            (5U)    /* from up to down && from right to left */
#define LCD_SCAN_DIR_D2U_L2R            (6U)    /* from down to up && from left to right */
#define LCD_SCAN_DIR_D2U_R2L            (7U)    /* from down to up && from right to left */
/**
 * @}
 */

/**
 * @defgroup LCD_Display_Direction LCD Display Direction
 */
#define LCD_DISPLAY_VERTICAL            (0x0000U)
#define LCD_DISPLAY_HORIZONTAL          (0x0001U)
/**
 * @}
 */

/**
 * @defgroup LCD_Color LCD Color
 * @{
 */
#define LCD_COLOR_WHITE                 (0xFFFFU)
#define LCD_COLOR_BLACK                 (0x0000U)
#define LCD_COLOR_BLUE                  (0x001FU)
#define LCD_COLOR_BRED                  (0XF81FU)
#define LCD_COLOR_GRED                  (0XFFE0U)
#define LCD_COLOR_GBLUE                 (0X07FFU)
#define LCD_COLOR_RED                   (0xF800U)
#define LCD_COLOR_MAGENTA               (0xF81FU)
#define LCD_COLOR_GREEN                 (0x07E0U)
#define LCD_COLOR_CYAN                  (0x7FFFU)
#define LCD_COLOR_YELLOW                (0xFFE0U)
#define LCD_COLOR_BROWN                 (0XBC40U)
#define LCD_COLOR_BRRED                 (0XFC07U)
#define LCD_COLOR_GRAY                  (0X8430U)

#define LCD_COLOR_DARKBLUE              (0X01CFU)
#define LCD_COLOR_LIGHTBLUE             (0X7D7CU)
#define LCD_COLOR_GRAYBLUE              (0X5458U)

#define LCD_COLOR_LIGHTGREEN            (0X841FU)
#define LCD_COLOR_LIGHTGRAY             (0XEF5BU)
#define LCD_COLOR_LGRAY                 (0XC618U)
#define LCD_COLOR_LGRAYBLUE             (0XA651U)
#define LCD_COLOR_LBBLUE                (0X2B12U)
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
 * @defgroup NT35510_Global_Functions NT35510 Global Functions
 * @{
 */
void NT35510_Init(void);
uint16_t NT35510_ReadID(void);
void NT35510_DisplayOn(void);
void NT35510_DisplayOff(void);
uint16_t NT35510_GetPixelWidth(void);
uint16_t NT35510_GetPixelHeight(void);
void NT35510_SetScanDir(uint32_t u32Dir);
void NT35510_SetDisplayDir(uint32_t u32Dir);
void NT35510_PrepareWriteRAM(void);
void NT35510_SetBackLight(uint8_t u8PWM);
void NT35510_SetCursor(uint16_t u16Xpos, uint16_t u16Ypos);
void NT35510_WritePixel(uint16_t u16Xpos, uint16_t u16Ypos, uint16_t u16RGBCode);
void NT35510_DrawLine(uint16_t u16X1,
                    uint16_t u16Y1,
                    uint16_t u16X2,
                    uint16_t u16Y2,
                    uint16_t u16RGBCode);
void NT35510_DrawCircle(uint16_t u16Xpos,
                    uint16_t u16Ypos,
                    uint16_t u16Radius,
                    uint16_t u16RGBCode);
void NT35510_FillTriangle(uint16_t u16X1,
                        uint16_t u16X2,
                        uint16_t u16X3,
                        uint16_t u16Y1,
                        uint16_t u16Y2,
                        uint16_t u16Y3,
                        uint16_t u16RGBCode);
void NT35510_DrawRectangle(uint16_t u16X1,
                            uint16_t u16Y1,
                            uint16_t u16X2,
                            uint16_t u16Y2,
                            uint16_t u16RGBCode);
void NT35510_Clear(uint16_t u16RGBCode);

/* Implement the below functions for the specified BSP board */
en_result_t BSP_NT35510_Init(void);
void BSP_NT35510_WriteData(uint16_t u16Data);
en_result_t BSP_NT35510_WriteMultipleData(const uint16_t au16Data[],
                                            uint32_t u32Size);
void BSP_NT35510_WriteReg(uint16_t u16Reg);
uint16_t BSP_NT35510_ReadData(void);
void BSP_NT35510_WriteRegData(uint16_t u16Reg, uint16_t u16Data);
uint16_t BSP_NT35510_ReadRegData(uint16_t u16Reg);

/**
 * @}
 */

#endif /* BSP_NT35510_ENABLE */

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

#endif /* __NT35510_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
