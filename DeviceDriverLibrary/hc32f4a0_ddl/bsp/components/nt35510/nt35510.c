/**
 *******************************************************************************
 * @file  nt35510.c
 * @brief This file provides firmware functions for LCD NT35510.
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
#include "hc32f4a0_utility.h"
#include "nt35510.h"

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
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup NT35510_Local_Types NT35510 Local Types
 * @{
 */

/**
 * @brief NT35510 LCD Device Structure Definition
 */
typedef struct
{
    uint16_t u16Dir;        /*!< Direction: 0, Vertical; 1, Horizontal */
    uint16_t u16ID;         /*!< LCD ID */
    uint16_t u16Width;      /*!< LCD Width */
    uint16_t u16Height;     /*!< LCD Heigth */
    uint16_t u16WRamCmd;    /*!< Start to write GRAM */
    uint16_t u16SetXCmd;    /*!< Set X axis */
    uint16_t u16SetYCmd;    /*!< Set Y axis */
} stc_nt35510_lcd_dev_t;

/**
 * @}
 */

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup NT35510_Local_Macros NT35510 Local Macros
 * @{
 */

/**
 * @defgroup LCD_ID LCD ID
 * @{
 */
#define LCD_ID                      (0x5510U)
/**
 * @}
 */

/**
 * @defgroup LCD_Size LCD Size
 * @{
 */
#define LCD_PIXEL_WIDTH             (480U)
#define LCD_PIXEL_HEIGHT            (800U)
/**
 * @}
 */

/**
 * @defgroup LCD_Resolution LCD Resolution
 * @{
 */
#define SSD_HOR_RESOLUTION          (LCD_PIXEL_WIDTH)
#define SSD_VER_RESOLUTION          (LCD_PIXEL_HEIGHT)
/**
 * @}
 */

/**
 * @defgroup LCD_Driver_Paramters LCD Driver Paramters
 * @{
 */
#define SSD_HOR_PULSE_WIDTH         (1U)
#define SSD_HOR_BACK_PORCH          (46U)
#define SSD_HOR_FRONT_PORCH         (210U)


#define SSD_VER_PULSE_WIDTH         (1U)
#define SSD_VER_BACK_PORCH          (23U)
#define SSD_VER_FRONT_PORCH         (22U)

#define SSD_HT                      (SSD_HOR_RESOLUTION + SSD_HOR_BACK_PORCH + SSD_HOR_FRONT_PORCH)
#define SSD_HPS                     (SSD_HOR_BACK_PORCH)
#define SSD_VT                      (SSD_VER_RESOLUTION + SSD_VER_BACK_PORCH + SSD_VER_FRONT_PORCH)
#define SSD_VPS                     (SSD_VER_BACK_PORCH)
/**
 * @}
 */

/* LCD Scan Direction */
#define LCD_SCAN_DIR                (LCD_SCAN_DIR_L2R_U2D)

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
 * @addtogroup NT35510_Local_Functions
 * @{
 */
static void NT35510_Delay(uint32_t u32Delay);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/**
 * @defgroup NT35510_Local_Variables NT35510 Local Variables
 * @{
 */
static stc_nt35510_lcd_dev_t m_stcLcdDevNt35510;
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup NT35510_Global_Functions NT35510 Global Functions
 * @{
 */
/**
 * @brief  LCD device initialize.
 * @param  none
 * @retval none
 */
void NT35510_Init(void)
{
    BSP_NT35510_Init();

    m_stcLcdDevNt35510.u16ID      = LCD_ID;
    m_stcLcdDevNt35510.u16Width   = LCD_PIXEL_WIDTH;
    m_stcLcdDevNt35510.u16Height  = LCD_PIXEL_HEIGHT;
    m_stcLcdDevNt35510.u16WRamCmd = 0x2C00U;
    m_stcLcdDevNt35510.u16SetXCmd = 0x2A00U;
    m_stcLcdDevNt35510.u16SetYCmd = 0x2B00U;

    /* read ID: 8000H (5510H) */
    BSP_NT35510_WriteRegData(0x0000U, 0x0001U);
    m_stcLcdDevNt35510.u16ID = BSP_NT35510_ReadRegData(0x0000U);

    BSP_NT35510_WriteReg(0xDA00);
    NT35510_Delay(10UL);
    m_stcLcdDevNt35510.u16ID = BSP_NT35510_ReadData();
    NT35510_Delay(10UL);

    BSP_NT35510_WriteReg(0xDB00U);
    NT35510_Delay(10UL);
    m_stcLcdDevNt35510.u16ID = BSP_NT35510_ReadData();
    m_stcLcdDevNt35510.u16ID <<= 8U;
    NT35510_Delay(10UL);

    BSP_NT35510_WriteReg(0xDC00U);
    NT35510_Delay(10UL);
    m_stcLcdDevNt35510.u16ID |= BSP_NT35510_ReadData();
    NT35510_Delay(10UL);

    if (0x8000U == m_stcLcdDevNt35510.u16ID)
    {
        m_stcLcdDevNt35510.u16ID = 0x5510U;
    }
    else
    {
        /* read ID: 5761H (1963H) */
        BSP_NT35510_WriteReg(0xA1U);
        NT35510_Delay(10UL);
        m_stcLcdDevNt35510.u16ID=BSP_NT35510_ReadData();
        NT35510_Delay(10UL);
        m_stcLcdDevNt35510.u16ID=BSP_NT35510_ReadData();
        NT35510_Delay(10UL);
        m_stcLcdDevNt35510.u16ID <<= 8U;
        m_stcLcdDevNt35510.u16ID |= BSP_NT35510_ReadData();
        NT35510_Delay(10UL);
        if (0x5761U == m_stcLcdDevNt35510.u16ID)
        {
            m_stcLcdDevNt35510.u16ID = 0x1963U;
        }
    }

    if (0x5510U == m_stcLcdDevNt35510.u16ID)
    {
        /* Config LCD */
        BSP_NT35510_WriteRegData(0xF000U, 0x55U);
        BSP_NT35510_WriteRegData(0xF001U, 0xAAU);
        BSP_NT35510_WriteRegData(0xF002U, 0x52U);
        BSP_NT35510_WriteRegData(0xF003U, 0x08U);
        BSP_NT35510_WriteRegData(0xF004U, 0x01U);
        /* AVDD Set AVDD 5.2V */
        BSP_NT35510_WriteRegData(0xB000U, 0x0DU);
        BSP_NT35510_WriteRegData(0xB001U, 0x0DU);
        BSP_NT35510_WriteRegData(0xB002U, 0x0DU);
        /* AVDD ratio */
        BSP_NT35510_WriteRegData(0xB600U, 0x34U);
        BSP_NT35510_WriteRegData(0xB601U, 0x34U);
        BSP_NT35510_WriteRegData(0xB602U, 0x34U);
        /* AVEE -5.2V */
        BSP_NT35510_WriteRegData(0xB100U, 0x0DU);
        BSP_NT35510_WriteRegData(0xB101U, 0x0DU);
        BSP_NT35510_WriteRegData(0xB102U, 0x0DU);
        /* AVEE ratio */
        BSP_NT35510_WriteRegData(0xB700U, 0x34U);
        BSP_NT35510_WriteRegData(0xB701U, 0x34U);
        BSP_NT35510_WriteRegData(0xB702U, 0x34U);
        /* VCL -2.5V */
        BSP_NT35510_WriteRegData(0xB200U, 0x00U);
        BSP_NT35510_WriteRegData(0xB201U, 0x00U);
        BSP_NT35510_WriteRegData(0xB202U, 0x00U);
        /* VCL ratio */
        BSP_NT35510_WriteRegData(0xB800U, 0x24U);
        BSP_NT35510_WriteRegData(0xB801U, 0x24U);
        BSP_NT35510_WriteRegData(0xB802U, 0x24U);
        /* VGH 15V (Free pump) */
        BSP_NT35510_WriteRegData(0xBF00U, 0x01U);
        BSP_NT35510_WriteRegData(0xB300U, 0x0FU);
        BSP_NT35510_WriteRegData(0xB301U, 0x0FU);
        BSP_NT35510_WriteRegData(0xB302U, 0x0FU);
        /* VGH ratio */
        BSP_NT35510_WriteRegData(0xB900U, 0x34U);
        BSP_NT35510_WriteRegData(0xB901U, 0x34U);
        BSP_NT35510_WriteRegData(0xB902U, 0x34U);
        /* VGL_REG -10V */
        BSP_NT35510_WriteRegData(0xB500U, 0x08U);
        BSP_NT35510_WriteRegData(0xB501U, 0x08U);
        BSP_NT35510_WriteRegData(0xB502U, 0x08U);
        BSP_NT35510_WriteRegData(0xC200U, 0x03U);
        /* VGLX ratio */
        BSP_NT35510_WriteRegData(0xBA00U, 0x24U);
        BSP_NT35510_WriteRegData(0xBA01U, 0x24U);
        BSP_NT35510_WriteRegData(0xBA02U, 0x24U);
        /* VGMP/VGSP 4.5V/0V */
        BSP_NT35510_WriteRegData(0xBC00U, 0x00U);
        BSP_NT35510_WriteRegData(0xBC01U, 0x78U);
        BSP_NT35510_WriteRegData(0xBC02U, 0x00U);
        /* VGMN/VGSN -4.5V/0V */
        BSP_NT35510_WriteRegData(0xBD00U, 0x00U);
        BSP_NT35510_WriteRegData(0xBD01U, 0x78U);
        BSP_NT35510_WriteRegData(0xBD02U, 0x00U);
        /* VCOM */
        BSP_NT35510_WriteRegData(0xBE00U, 0x00U);
        BSP_NT35510_WriteRegData(0xBE01U, 0x64U);
        /* Gamma Setting */
        BSP_NT35510_WriteRegData(0xD100U, 0x00U);
        BSP_NT35510_WriteRegData(0xD101U, 0x33U);
        BSP_NT35510_WriteRegData(0xD102U, 0x00U);
        BSP_NT35510_WriteRegData(0xD103U, 0x34U);
        BSP_NT35510_WriteRegData(0xD104U, 0x00U);
        BSP_NT35510_WriteRegData(0xD105U, 0x3AU);
        BSP_NT35510_WriteRegData(0xD106U, 0x00U);
        BSP_NT35510_WriteRegData(0xD107U, 0x4AU);
        BSP_NT35510_WriteRegData(0xD108U, 0x00U);
        BSP_NT35510_WriteRegData(0xD109U, 0x5CU);
        BSP_NT35510_WriteRegData(0xD10AU, 0x00U);
        BSP_NT35510_WriteRegData(0xD10BU, 0x81U);
        BSP_NT35510_WriteRegData(0xD10CU, 0x00U);
        BSP_NT35510_WriteRegData(0xD10DU, 0xA6U);
        BSP_NT35510_WriteRegData(0xD10EU, 0x00U);
        BSP_NT35510_WriteRegData(0xD10FU, 0xE5U);
        BSP_NT35510_WriteRegData(0xD110U, 0x01U);
        BSP_NT35510_WriteRegData(0xD111U, 0x13U);
        BSP_NT35510_WriteRegData(0xD112U, 0x01U);
        BSP_NT35510_WriteRegData(0xD113U, 0x54U);
        BSP_NT35510_WriteRegData(0xD114U, 0x01U);
        BSP_NT35510_WriteRegData(0xD115U, 0x82U);
        BSP_NT35510_WriteRegData(0xD116U, 0x01U);
        BSP_NT35510_WriteRegData(0xD117U, 0xCAU);
        BSP_NT35510_WriteRegData(0xD118U, 0x02U);
        BSP_NT35510_WriteRegData(0xD119U, 0x00U);
        BSP_NT35510_WriteRegData(0xD11AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD11BU, 0x01U);
        BSP_NT35510_WriteRegData(0xD11CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD11DU, 0x34U);
        BSP_NT35510_WriteRegData(0xD11EU, 0x02U);
        BSP_NT35510_WriteRegData(0xD11FU, 0x67U);
        BSP_NT35510_WriteRegData(0xD120U, 0x02U);
        BSP_NT35510_WriteRegData(0xD121U, 0x84U);
        BSP_NT35510_WriteRegData(0xD122U, 0x02U);
        BSP_NT35510_WriteRegData(0xD123U, 0xA4U);
        BSP_NT35510_WriteRegData(0xD124U, 0x02U);
        BSP_NT35510_WriteRegData(0xD125U, 0xB7U);
        BSP_NT35510_WriteRegData(0xD126U, 0x02U);
        BSP_NT35510_WriteRegData(0xD127U, 0xCFU);
        BSP_NT35510_WriteRegData(0xD128U, 0x02U);
        BSP_NT35510_WriteRegData(0xD129U, 0xDEU);
        BSP_NT35510_WriteRegData(0xD12AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD12BU, 0xF2U);
        BSP_NT35510_WriteRegData(0xD12CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD12DU, 0xFEU);
        BSP_NT35510_WriteRegData(0xD12EU, 0x03U);
        BSP_NT35510_WriteRegData(0xD12FU, 0x10U);
        BSP_NT35510_WriteRegData(0xD130U, 0x03U);
        BSP_NT35510_WriteRegData(0xD131U, 0x33U);
        BSP_NT35510_WriteRegData(0xD132U, 0x03U);
        BSP_NT35510_WriteRegData(0xD133U, 0x6DU);
        BSP_NT35510_WriteRegData(0xD200U, 0x00U);
        BSP_NT35510_WriteRegData(0xD201U, 0x33U);
        BSP_NT35510_WriteRegData(0xD202U, 0x00U);
        BSP_NT35510_WriteRegData(0xD203U, 0x34U);
        BSP_NT35510_WriteRegData(0xD204U, 0x00U);
        BSP_NT35510_WriteRegData(0xD205U, 0x3AU);
        BSP_NT35510_WriteRegData(0xD206U, 0x00U);
        BSP_NT35510_WriteRegData(0xD207U, 0x4AU);
        BSP_NT35510_WriteRegData(0xD208U, 0x00U);
        BSP_NT35510_WriteRegData(0xD209U, 0x5CU);
        BSP_NT35510_WriteRegData(0xD20AU, 0x00U);
        BSP_NT35510_WriteRegData(0xD20BU, 0x81U);
        BSP_NT35510_WriteRegData(0xD20CU, 0x00U);
        BSP_NT35510_WriteRegData(0xD20DU, 0xA6U);
        BSP_NT35510_WriteRegData(0xD20EU, 0x00U);
        BSP_NT35510_WriteRegData(0xD20FU, 0xE5U);
        BSP_NT35510_WriteRegData(0xD210U, 0x01U);
        BSP_NT35510_WriteRegData(0xD211U, 0x13U);
        BSP_NT35510_WriteRegData(0xD212U, 0x01U);
        BSP_NT35510_WriteRegData(0xD213U, 0x54U);
        BSP_NT35510_WriteRegData(0xD214U, 0x01U);
        BSP_NT35510_WriteRegData(0xD215U, 0x82U);
        BSP_NT35510_WriteRegData(0xD216U, 0x01U);
        BSP_NT35510_WriteRegData(0xD217U, 0xCAU);
        BSP_NT35510_WriteRegData(0xD218U, 0x02U);
        BSP_NT35510_WriteRegData(0xD219U, 0x00U);
        BSP_NT35510_WriteRegData(0xD21AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD21BU, 0x01U);
        BSP_NT35510_WriteRegData(0xD21CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD21DU, 0x34U);
        BSP_NT35510_WriteRegData(0xD21EU, 0x02U);
        BSP_NT35510_WriteRegData(0xD21FU, 0x67U);
        BSP_NT35510_WriteRegData(0xD220U, 0x02U);
        BSP_NT35510_WriteRegData(0xD221U, 0x84U);
        BSP_NT35510_WriteRegData(0xD222U, 0x02U);
        BSP_NT35510_WriteRegData(0xD223U, 0xA4U);
        BSP_NT35510_WriteRegData(0xD224U, 0x02U);
        BSP_NT35510_WriteRegData(0xD225U, 0xB7U);
        BSP_NT35510_WriteRegData(0xD226U, 0x02U);
        BSP_NT35510_WriteRegData(0xD227U, 0xCFU);
        BSP_NT35510_WriteRegData(0xD228U, 0x02U);
        BSP_NT35510_WriteRegData(0xD229U, 0xDEU);
        BSP_NT35510_WriteRegData(0xD22AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD22BU, 0xF2U);
        BSP_NT35510_WriteRegData(0xD22CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD22DU, 0xFEU);
        BSP_NT35510_WriteRegData(0xD22EU, 0x03U);
        BSP_NT35510_WriteRegData(0xD22FU, 0x10U);
        BSP_NT35510_WriteRegData(0xD230U, 0x03U);
        BSP_NT35510_WriteRegData(0xD231U, 0x33U);
        BSP_NT35510_WriteRegData(0xD232U, 0x03U);
        BSP_NT35510_WriteRegData(0xD233U, 0x6DU);
        BSP_NT35510_WriteRegData(0xD300U, 0x00U);
        BSP_NT35510_WriteRegData(0xD301U, 0x33U);
        BSP_NT35510_WriteRegData(0xD302U, 0x00U);
        BSP_NT35510_WriteRegData(0xD303U, 0x34U);
        BSP_NT35510_WriteRegData(0xD304U, 0x00U);
        BSP_NT35510_WriteRegData(0xD305U, 0x3AU);
        BSP_NT35510_WriteRegData(0xD306U, 0x00U);
        BSP_NT35510_WriteRegData(0xD307U, 0x4AU);
        BSP_NT35510_WriteRegData(0xD308U, 0x00U);
        BSP_NT35510_WriteRegData(0xD309U, 0x5CU);
        BSP_NT35510_WriteRegData(0xD30AU, 0x00U);
        BSP_NT35510_WriteRegData(0xD30BU, 0x81U);
        BSP_NT35510_WriteRegData(0xD30CU, 0x00U);
        BSP_NT35510_WriteRegData(0xD30DU, 0xA6U);
        BSP_NT35510_WriteRegData(0xD30EU, 0x00U);
        BSP_NT35510_WriteRegData(0xD30FU, 0xE5U);
        BSP_NT35510_WriteRegData(0xD310U, 0x01U);
        BSP_NT35510_WriteRegData(0xD311U, 0x13U);
        BSP_NT35510_WriteRegData(0xD312U, 0x01U);
        BSP_NT35510_WriteRegData(0xD313U, 0x54U);
        BSP_NT35510_WriteRegData(0xD314U, 0x01U);
        BSP_NT35510_WriteRegData(0xD315U, 0x82U);
        BSP_NT35510_WriteRegData(0xD316U, 0x01U);
        BSP_NT35510_WriteRegData(0xD317U, 0xCAU);
        BSP_NT35510_WriteRegData(0xD318U, 0x02U);
        BSP_NT35510_WriteRegData(0xD319U, 0x00U);
        BSP_NT35510_WriteRegData(0xD31AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD31BU, 0x01U);
        BSP_NT35510_WriteRegData(0xD31CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD31DU, 0x34U);
        BSP_NT35510_WriteRegData(0xD31EU, 0x02U);
        BSP_NT35510_WriteRegData(0xD31FU, 0x67U);
        BSP_NT35510_WriteRegData(0xD320U, 0x02U);
        BSP_NT35510_WriteRegData(0xD321U, 0x84U);
        BSP_NT35510_WriteRegData(0xD322U, 0x02U);
        BSP_NT35510_WriteRegData(0xD323U, 0xA4U);
        BSP_NT35510_WriteRegData(0xD324U, 0x02U);
        BSP_NT35510_WriteRegData(0xD325U, 0xB7U);
        BSP_NT35510_WriteRegData(0xD326U, 0x02U);
        BSP_NT35510_WriteRegData(0xD327U, 0xCFU);
        BSP_NT35510_WriteRegData(0xD328U, 0x02U);
        BSP_NT35510_WriteRegData(0xD329U, 0xDEU);
        BSP_NT35510_WriteRegData(0xD32AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD32BU, 0xF2U);
        BSP_NT35510_WriteRegData(0xD32CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD32DU, 0xFEU);
        BSP_NT35510_WriteRegData(0xD32EU, 0x03U);
        BSP_NT35510_WriteRegData(0xD32FU, 0x10U);
        BSP_NT35510_WriteRegData(0xD330U, 0x03U);
        BSP_NT35510_WriteRegData(0xD331U, 0x33U);
        BSP_NT35510_WriteRegData(0xD332U, 0x03U);
        BSP_NT35510_WriteRegData(0xD333U, 0x6DU);
        BSP_NT35510_WriteRegData(0xD400U, 0x00U);
        BSP_NT35510_WriteRegData(0xD401U, 0x33U);
        BSP_NT35510_WriteRegData(0xD402U, 0x00U);
        BSP_NT35510_WriteRegData(0xD403U, 0x34U);
        BSP_NT35510_WriteRegData(0xD404U, 0x00U);
        BSP_NT35510_WriteRegData(0xD405U, 0x3AU);
        BSP_NT35510_WriteRegData(0xD406U, 0x00U);
        BSP_NT35510_WriteRegData(0xD407U, 0x4AU);
        BSP_NT35510_WriteRegData(0xD408U, 0x00U);
        BSP_NT35510_WriteRegData(0xD409U, 0x5CU);
        BSP_NT35510_WriteRegData(0xD40AU, 0x00U);
        BSP_NT35510_WriteRegData(0xD40BU, 0x81U);
        BSP_NT35510_WriteRegData(0xD40CU, 0x00U);
        BSP_NT35510_WriteRegData(0xD40DU, 0xA6U);
        BSP_NT35510_WriteRegData(0xD40EU, 0x00U);
        BSP_NT35510_WriteRegData(0xD40FU, 0xE5U);
        BSP_NT35510_WriteRegData(0xD410U, 0x01U);
        BSP_NT35510_WriteRegData(0xD411U, 0x13U);
        BSP_NT35510_WriteRegData(0xD412U, 0x01U);
        BSP_NT35510_WriteRegData(0xD413U, 0x54U);
        BSP_NT35510_WriteRegData(0xD414U, 0x01U);
        BSP_NT35510_WriteRegData(0xD415U, 0x82U);
        BSP_NT35510_WriteRegData(0xD416U, 0x01U);
        BSP_NT35510_WriteRegData(0xD417U, 0xCAU);
        BSP_NT35510_WriteRegData(0xD418U, 0x02U);
        BSP_NT35510_WriteRegData(0xD419U, 0x00U);
        BSP_NT35510_WriteRegData(0xD41AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD41BU, 0x01U);
        BSP_NT35510_WriteRegData(0xD41CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD41DU, 0x34U);
        BSP_NT35510_WriteRegData(0xD41EU, 0x02U);
        BSP_NT35510_WriteRegData(0xD41FU, 0x67U);
        BSP_NT35510_WriteRegData(0xD420U, 0x02U);
        BSP_NT35510_WriteRegData(0xD421U, 0x84U);
        BSP_NT35510_WriteRegData(0xD422U, 0x02U);
        BSP_NT35510_WriteRegData(0xD423U, 0xA4U);
        BSP_NT35510_WriteRegData(0xD424U, 0x02U);
        BSP_NT35510_WriteRegData(0xD425U, 0xB7U);
        BSP_NT35510_WriteRegData(0xD426U, 0x02U);
        BSP_NT35510_WriteRegData(0xD427U, 0xCFU);
        BSP_NT35510_WriteRegData(0xD428U, 0x02U);
        BSP_NT35510_WriteRegData(0xD429U, 0xDEU);
        BSP_NT35510_WriteRegData(0xD42AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD42BU, 0xF2U);
        BSP_NT35510_WriteRegData(0xD42CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD42DU, 0xFEU);
        BSP_NT35510_WriteRegData(0xD42EU, 0x03U);
        BSP_NT35510_WriteRegData(0xD42FU, 0x10U);
        BSP_NT35510_WriteRegData(0xD430U, 0x03U);
        BSP_NT35510_WriteRegData(0xD431U, 0x33U);
        BSP_NT35510_WriteRegData(0xD432U, 0x03U);
        BSP_NT35510_WriteRegData(0xD433U, 0x6DU);
        BSP_NT35510_WriteRegData(0xD500U, 0x00U);
        BSP_NT35510_WriteRegData(0xD501U, 0x33U);
        BSP_NT35510_WriteRegData(0xD502U, 0x00U);
        BSP_NT35510_WriteRegData(0xD503U, 0x34U);
        BSP_NT35510_WriteRegData(0xD504U, 0x00U);
        BSP_NT35510_WriteRegData(0xD505U, 0x3AU);
        BSP_NT35510_WriteRegData(0xD506U, 0x00U);
        BSP_NT35510_WriteRegData(0xD507U, 0x4AU);
        BSP_NT35510_WriteRegData(0xD508U, 0x00U);
        BSP_NT35510_WriteRegData(0xD509U, 0x5CU);
        BSP_NT35510_WriteRegData(0xD50AU, 0x00U);
        BSP_NT35510_WriteRegData(0xD50BU, 0x81U);
        BSP_NT35510_WriteRegData(0xD50CU, 0x00U);
        BSP_NT35510_WriteRegData(0xD50DU, 0xA6U);
        BSP_NT35510_WriteRegData(0xD50EU, 0x00U);
        BSP_NT35510_WriteRegData(0xD50FU, 0xE5U);
        BSP_NT35510_WriteRegData(0xD510U, 0x01U);
        BSP_NT35510_WriteRegData(0xD511U, 0x13U);
        BSP_NT35510_WriteRegData(0xD512U, 0x01U);
        BSP_NT35510_WriteRegData(0xD513U, 0x54U);
        BSP_NT35510_WriteRegData(0xD514U, 0x01U);
        BSP_NT35510_WriteRegData(0xD515U, 0x82U);
        BSP_NT35510_WriteRegData(0xD516U, 0x01U);
        BSP_NT35510_WriteRegData(0xD517U, 0xCAU);
        BSP_NT35510_WriteRegData(0xD518U, 0x02U);
        BSP_NT35510_WriteRegData(0xD519U, 0x00U);
        BSP_NT35510_WriteRegData(0xD51AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD51BU, 0x01U);
        BSP_NT35510_WriteRegData(0xD51CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD51DU, 0x34U);
        BSP_NT35510_WriteRegData(0xD51EU, 0x02U);
        BSP_NT35510_WriteRegData(0xD51FU, 0x67U);
        BSP_NT35510_WriteRegData(0xD520U, 0x02U);
        BSP_NT35510_WriteRegData(0xD521U, 0x84U);
        BSP_NT35510_WriteRegData(0xD522U, 0x02U);
        BSP_NT35510_WriteRegData(0xD523U, 0xA4U);
        BSP_NT35510_WriteRegData(0xD524U, 0x02U);
        BSP_NT35510_WriteRegData(0xD525U, 0xB7U);
        BSP_NT35510_WriteRegData(0xD526U, 0x02U);
        BSP_NT35510_WriteRegData(0xD527U, 0xCFU);
        BSP_NT35510_WriteRegData(0xD528U, 0x02U);
        BSP_NT35510_WriteRegData(0xD529U, 0xDEU);
        BSP_NT35510_WriteRegData(0xD52AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD52BU, 0xF2U);
        BSP_NT35510_WriteRegData(0xD52CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD52DU, 0xFEU);
        BSP_NT35510_WriteRegData(0xD52EU, 0x03U);
        BSP_NT35510_WriteRegData(0xD52FU, 0x10U);
        BSP_NT35510_WriteRegData(0xD530U, 0x03U);
        BSP_NT35510_WriteRegData(0xD531U, 0x33U);
        BSP_NT35510_WriteRegData(0xD532U, 0x03U);
        BSP_NT35510_WriteRegData(0xD533U, 0x6DU);
        BSP_NT35510_WriteRegData(0xD600U, 0x00U);
        BSP_NT35510_WriteRegData(0xD601U, 0x33U);
        BSP_NT35510_WriteRegData(0xD602U, 0x00U);
        BSP_NT35510_WriteRegData(0xD603U, 0x34U);
        BSP_NT35510_WriteRegData(0xD604U, 0x00U);
        BSP_NT35510_WriteRegData(0xD605U, 0x3AU);
        BSP_NT35510_WriteRegData(0xD606U, 0x00U);
        BSP_NT35510_WriteRegData(0xD607U, 0x4AU);
        BSP_NT35510_WriteRegData(0xD608U, 0x00U);
        BSP_NT35510_WriteRegData(0xD609U, 0x5CU);
        BSP_NT35510_WriteRegData(0xD60AU, 0x00U);
        BSP_NT35510_WriteRegData(0xD60BU, 0x81U);
        BSP_NT35510_WriteRegData(0xD60CU, 0x00U);
        BSP_NT35510_WriteRegData(0xD60DU, 0xA6U);
        BSP_NT35510_WriteRegData(0xD60EU, 0x00U);
        BSP_NT35510_WriteRegData(0xD60FU, 0xE5U);
        BSP_NT35510_WriteRegData(0xD610U, 0x01U);
        BSP_NT35510_WriteRegData(0xD611U, 0x13U);
        BSP_NT35510_WriteRegData(0xD612U, 0x01U);
        BSP_NT35510_WriteRegData(0xD613U, 0x54U);
        BSP_NT35510_WriteRegData(0xD614U, 0x01U);
        BSP_NT35510_WriteRegData(0xD615U, 0x82U);
        BSP_NT35510_WriteRegData(0xD616U, 0x01U);
        BSP_NT35510_WriteRegData(0xD617U, 0xCAU);
        BSP_NT35510_WriteRegData(0xD618U, 0x02U);
        BSP_NT35510_WriteRegData(0xD619U, 0x00U);
        BSP_NT35510_WriteRegData(0xD61AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD61BU, 0x01U);
        BSP_NT35510_WriteRegData(0xD61CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD61DU, 0x34U);
        BSP_NT35510_WriteRegData(0xD61EU, 0x02U);
        BSP_NT35510_WriteRegData(0xD61FU, 0x67U);
        BSP_NT35510_WriteRegData(0xD620U, 0x02U);
        BSP_NT35510_WriteRegData(0xD621U, 0x84U);
        BSP_NT35510_WriteRegData(0xD622U, 0x02U);
        BSP_NT35510_WriteRegData(0xD623U, 0xA4U);
        BSP_NT35510_WriteRegData(0xD624U, 0x02U);
        BSP_NT35510_WriteRegData(0xD625U, 0xB7U);
        BSP_NT35510_WriteRegData(0xD626U, 0x02U);
        BSP_NT35510_WriteRegData(0xD627U, 0xCFU);
        BSP_NT35510_WriteRegData(0xD628U, 0x02U);
        BSP_NT35510_WriteRegData(0xD629U, 0xDEU);
        BSP_NT35510_WriteRegData(0xD62AU, 0x02U);
        BSP_NT35510_WriteRegData(0xD62BU, 0xF2U);
        BSP_NT35510_WriteRegData(0xD62CU, 0x02U);
        BSP_NT35510_WriteRegData(0xD62DU, 0xFEU);
        BSP_NT35510_WriteRegData(0xD62EU, 0x03U);
        BSP_NT35510_WriteRegData(0xD62FU, 0x10U);
        BSP_NT35510_WriteRegData(0xD630U, 0x03U);
        BSP_NT35510_WriteRegData(0xD631U, 0x33U);
        BSP_NT35510_WriteRegData(0xD632U, 0x03U);
        BSP_NT35510_WriteRegData(0xD633U, 0x6DU);
        /* LV2 Page 0 enable */
        BSP_NT35510_WriteRegData(0xF000U, 0x55U);
        BSP_NT35510_WriteRegData(0xF001U, 0xAAU);
        BSP_NT35510_WriteRegData(0xF002U, 0x52U);
        BSP_NT35510_WriteRegData(0xF003U, 0x08U);
        BSP_NT35510_WriteRegData(0xF004U, 0x00U);
        /* Display control */
        BSP_NT35510_WriteRegData(0xB100U, 0xCCU);
        BSP_NT35510_WriteRegData(0xB101U, 0x00U);
        /* Source hold time */
        BSP_NT35510_WriteRegData(0xB600U, 0x05U);
        /* Gate EQ control */
        BSP_NT35510_WriteRegData(0xB700U, 0x70U);
        BSP_NT35510_WriteRegData(0xB701U, 0x70U);
        /* Source EQ control (Mode 2) */
        BSP_NT35510_WriteRegData(0xB800U, 0x01U);
        BSP_NT35510_WriteRegData(0xB801U, 0x03U);
        BSP_NT35510_WriteRegData(0xB802U, 0x03U);
        BSP_NT35510_WriteRegData(0xB803U, 0x03U);
        /* Inversion mode (2-dot) */
        BSP_NT35510_WriteRegData(0xBC00U, 0x02U);
        BSP_NT35510_WriteRegData(0xBC01U, 0x00U);
        BSP_NT35510_WriteRegData(0xBC02U, 0x00U);
        /* Timing control 4H w/ 4-delay */
        BSP_NT35510_WriteRegData(0xC900U, 0xD0U);
        BSP_NT35510_WriteRegData(0xC901U, 0x02U);
        BSP_NT35510_WriteRegData(0xC902U, 0x50U);
        BSP_NT35510_WriteRegData(0xC903U, 0x50U);
        BSP_NT35510_WriteRegData(0xC904U, 0x50U);
        BSP_NT35510_WriteRegData(0x3500U, 0x00U);
        BSP_NT35510_WriteRegData(0x3A00U, 0x55U);  /* 16-bit/pixel */
        BSP_NT35510_WriteReg(0x1100U);
        NT35510_Delay(120UL);
        BSP_NT35510_WriteReg(0x2900U);
    }
    else if (0x1963U == m_stcLcdDevNt35510.u16ID)
    {
        BSP_NT35510_WriteReg(0xE2U);        /* Set PLL with OSC = 10MHz (hardware),
                                                Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz */
        BSP_NT35510_WriteData(0x1DU);
        BSP_NT35510_WriteData(0x02U);       /* 2 Divider M = 2, PLL = 300/(M+1) = 100MHz */
        BSP_NT35510_WriteData(0x04U);       /* 3 Validate M and N values */
        NT35510_Delay(1UL);
        BSP_NT35510_WriteReg(0xE0U);        /* Start PLL command */
        BSP_NT35510_WriteData(0x01U);       /* enable PLL */
        NT35510_Delay(10UL);
        BSP_NT35510_WriteReg(0xE0U);        /* Start PLL command again */
        BSP_NT35510_WriteData(0x03U);       /* now, use PLL output as system clock */
        NT35510_Delay(12UL);
        BSP_NT35510_WriteReg(0x01U);
        NT35510_Delay(10UL);

        BSP_NT35510_WriteReg(0xE6U);
        BSP_NT35510_WriteData(0x2FU);
        BSP_NT35510_WriteData(0xFFU);
        BSP_NT35510_WriteData(0xFFU);

        BSP_NT35510_WriteReg(0xB0U);
        BSP_NT35510_WriteData(0x20U);
        BSP_NT35510_WriteData(0x00U);

        BSP_NT35510_WriteData((SSD_HOR_RESOLUTION - 1U) >> 8U);
        BSP_NT35510_WriteData(SSD_HOR_RESOLUTION - 1U);
        BSP_NT35510_WriteData((SSD_VER_RESOLUTION - 1U) >> 8U);
        BSP_NT35510_WriteData(SSD_VER_RESOLUTION - 1U);
        BSP_NT35510_WriteData(0x00U);

        BSP_NT35510_WriteReg(0xB4U);        /* Set horizontal period */
        BSP_NT35510_WriteData((SSD_HT - 1U) >> 8U);
        BSP_NT35510_WriteData(SSD_HT - 1U);
        BSP_NT35510_WriteData(SSD_HPS >> 8U);
        BSP_NT35510_WriteData(SSD_HPS);
        BSP_NT35510_WriteData(SSD_HOR_PULSE_WIDTH - 1U);
        BSP_NT35510_WriteData(0x00U);
        BSP_NT35510_WriteData(0x00U);
        BSP_NT35510_WriteData(0x00U);
        BSP_NT35510_WriteReg(0xB6U);        /* Set vertical period */
        BSP_NT35510_WriteData((SSD_VT - 1U) >> 8U);
        BSP_NT35510_WriteData(SSD_VT - 1U);
        BSP_NT35510_WriteData(SSD_VPS >> 8U);
        BSP_NT35510_WriteData(SSD_VPS);
        BSP_NT35510_WriteData(SSD_VER_FRONT_PORCH - 1U);
        BSP_NT35510_WriteData(0x00U);
        BSP_NT35510_WriteData(0x00U);

        BSP_NT35510_WriteReg(0xF0U);
        BSP_NT35510_WriteData(0x03U);       /* 16-bit(565 format) data for 16bpp */

        BSP_NT35510_WriteReg(0x29U);
        BSP_NT35510_WriteReg(0xD0U);
        BSP_NT35510_WriteData(0x00U);

        BSP_NT35510_WriteReg(0xBEU);
        BSP_NT35510_WriteData(0x05U);
        BSP_NT35510_WriteData(0xFEU);
        BSP_NT35510_WriteData(0x01U);
        BSP_NT35510_WriteData(0x00U);
        BSP_NT35510_WriteData(0x00U);
        BSP_NT35510_WriteData(0x00U);

        BSP_NT35510_WriteReg(0xB8U);
        BSP_NT35510_WriteData(0x03U);
        BSP_NT35510_WriteData(0x01U);
        BSP_NT35510_WriteReg(0xBAU);
        BSP_NT35510_WriteData(0x01U);

        NT35510_SetBackLight(100U);
    }
    else
    {
        /* todo: add new supported LCD device */
    }

    NT35510_SetDisplayDir(LCD_DISPLAY_VERTICAL);

    /* Set cursor */
    NT35510_SetCursor(0U, 0U);

    /* Prepare to write to LCD RAM */
    NT35510_PrepareWriteRAM();
}

/**
 * @brief  Read LCD ID.
 * @param  None
 * @retval LCD Register Value.
 */
uint16_t NT35510_ReadID(void)
{
    return (LCD_ID);
}

/**
 * @brief  Enables the Display.
 * @param  None
 * @retval None
 */
void NT35510_DisplayOn(void)
{
    if ((m_stcLcdDevNt35510.u16ID == 0x9341U) || \
        (m_stcLcdDevNt35510.u16ID == 0X6804U) || \
        (m_stcLcdDevNt35510.u16ID == 0X5310U) || \
        (m_stcLcdDevNt35510.u16ID == 0X1963U))
    {
        BSP_NT35510_WriteReg(0x29U);
    }
    else if (m_stcLcdDevNt35510.u16ID == 0x5510U)
    {
        BSP_NT35510_WriteReg(0x2900U);
    }
    else
    {
        BSP_NT35510_WriteRegData(0x07U,0x0173U);
    }
}

/**
 * @brief  Disables the Display.
 * @param  None
 * @retval None
 */
void NT35510_DisplayOff(void)
{
    if ((m_stcLcdDevNt35510.u16ID == 0x9341U) || \
        (m_stcLcdDevNt35510.u16ID == 0X6804U) || \
        (m_stcLcdDevNt35510.u16ID == 0X5310U) || \
        (m_stcLcdDevNt35510.u16ID == 0X1963U))
    {
        BSP_NT35510_WriteReg(0x28U);
    }
    else if (m_stcLcdDevNt35510.u16ID == 0x5510U)
    {
        BSP_NT35510_WriteReg(0x2800U);
    }
    else
    {
        BSP_NT35510_WriteRegData(0x07U, 0x0000U);
    }
}

/**
 * @brief  Get LCD PIXEL WIDTH.
 * @param  None
 * @retval LCD PIXEL WIDTH.
 */
uint16_t NT35510_GetPixelWidth(void)
{
    /* Return LCD PIXEL WIDTH */
    return m_stcLcdDevNt35510.u16Width;
}

/**
 * @brief  Get LCD PIXEL HEIGHT.
 * @param  None
 * @retval LCD PIXEL HEIGHT.
 */
uint16_t NT35510_GetPixelHeight(void)
{
    /* Return LCD PIXEL HEIGHT */
    return m_stcLcdDevNt35510.u16Height;
}

/**
 * @brief  Set scan direction.
 * @param  [in] u32Dir:                 Scan direction
 * @param  [in] u32Dir:                 Screen direction
 *         This parameter can be one of the following values:
 *           @arg LCD_SCAN_DIR_L2R_U2D: from left to right && from up to down
 *           @arg LCD_SCAN_DIR_L2R_D2U: from left to right && from down to up
 *           @arg LCD_SCAN_DIR_R2L_U2D: from right to left && from up to down
 *           @arg LCD_SCAN_DIR_R2L_D2U: from right to left && from down to up
 *           @arg LCD_SCAN_DIR_U2D_L2R: from up to down && from left to right
 *           @arg LCD_SCAN_DIR_U2D_R2L: from up to down && from right to left
 *           @arg LCD_SCAN_DIR_D2U_L2R: from down to up && from left to right
 *           @arg LCD_SCAN_DIR_D2U_R2L: from down to up && from right to left
 * @retval None
 */
void NT35510_SetScanDir(uint32_t u32Dir)
{
    uint16_t dirreg;
    uint16_t regval = 0U;

    if((1U == m_stcLcdDevNt35510.u16Dir) || \
       ((0U == m_stcLcdDevNt35510.u16Dir) && (0x1963U == m_stcLcdDevNt35510.u16ID)))
    {
        switch(u32Dir)
        {
            case 0UL:u32Dir=6UL;break;
            case 1UL:u32Dir=7UL;break;
            case 2UL:u32Dir=4UL;break;
            case 3UL:u32Dir=5UL;break;
            case 4UL:u32Dir=1UL;break;
            case 5UL:u32Dir=0UL;break;
            case 6UL:u32Dir=3UL;break;
            case 7UL:u32Dir=2UL;break;
            default:break;
        }
    }

    if((0x5310U == m_stcLcdDevNt35510.u16ID) || \
       (0x5510U == m_stcLcdDevNt35510.u16ID) || \
       (0x1963U == m_stcLcdDevNt35510.u16ID))
    {
        switch(u32Dir)
        {
            case LCD_SCAN_DIR_L2R_U2D:
                regval |= ((0U << 7) | (0U << 6) | (0U << 5));
                break;
            case LCD_SCAN_DIR_L2R_D2U:
                regval |= ((1U << 7) | (0U <<6) | (0U << 5));
                break;
            case LCD_SCAN_DIR_R2L_U2D:
                regval |= ((0U << 7) | (1U <<6) | (0U << 5));
                break;
            case LCD_SCAN_DIR_R2L_D2U:
                regval |= ((1U << 7) | (1U << 6) | (0U << 5));
                break;
            case LCD_SCAN_DIR_U2D_L2R:
                regval |= ((0U << 7) | (0U << 6) | (1U << 5));
                break;
            case LCD_SCAN_DIR_U2D_R2L:
                regval |= ((0U << 7) | (1U << 6) | (1U << 5));
                break;
            case LCD_SCAN_DIR_D2U_L2R:
                regval |= ((1U << 7) | (0U << 6) | (1U << 5));
                break;
            case LCD_SCAN_DIR_D2U_R2L:
                regval |= ((1U << 7) | (1U << 6) | (1U << 5));
                break;
            default:
                break;
        }

        if (0x5510U == m_stcLcdDevNt35510.u16ID)
        {
            dirreg = 0x3600U;
        }
        else
        {
            dirreg = 0x36U;
        }
        BSP_NT35510_WriteRegData(dirreg,regval);

        if (0x5510U == m_stcLcdDevNt35510.u16ID)
        {
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd); BSP_NT35510_WriteData(0U);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd + 1U); BSP_NT35510_WriteData(0U);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd + 2U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Width - 1U) >> 8U);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd + 3U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Width - 1U) & 0XFFU);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetYCmd); BSP_NT35510_WriteData(0U);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetYCmd + 1U); BSP_NT35510_WriteData(0U);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetYCmd + 2U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Height - 1U) >> 8U);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetYCmd + 3U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Height - 1U) & 0XFFU);
        }
        else
        {
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd);
            BSP_NT35510_WriteData(0); BSP_NT35510_WriteData(0);
            BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Width - 1U) >> 8U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Width - 1U) & 0XFFU);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetYCmd);
            BSP_NT35510_WriteData(0); BSP_NT35510_WriteData(0);
            BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Height - 1U) >> 8U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Height - 1U) & 0XFFU);
        }
    }
}

/**
 * @brief  Set screen direction.
 * @param  [in] u32Dir:                 Screen direction
 *         This parameter can be one of the following values:
 *           @arg LCD_DISPLAY_VERTICAL:     LCD vertical display
 *           @arg LCD_DISPLAY_HORIZONTAL:   LCD horizontal display
 * @retval None
 */
void NT35510_SetDisplayDir(uint32_t u32Dir)
{
    if(LCD_DISPLAY_VERTICAL == u32Dir)   /* Vertical */
    {
        m_stcLcdDevNt35510.u16Dir = 0U;
        if (0x1963U == m_stcLcdDevNt35510.u16ID)
        {
            m_stcLcdDevNt35510.u16WRamCmd = 0x2CU;
            m_stcLcdDevNt35510.u16SetXCmd = 0x2BU;
            m_stcLcdDevNt35510.u16SetYCmd = 0x2AU;
        }
        else /* 5510 */
        {
            m_stcLcdDevNt35510.u16WRamCmd = 0x2C00U;
            m_stcLcdDevNt35510.u16SetXCmd = 0x2A00U;
            m_stcLcdDevNt35510.u16SetYCmd = 0x2B00U;
        }
        m_stcLcdDevNt35510.u16Width  = LCD_PIXEL_WIDTH;
        m_stcLcdDevNt35510.u16Height = LCD_PIXEL_HEIGHT;

    }
    else                            /* Horizontal */
    {
        m_stcLcdDevNt35510.u16Dir     = 1U;
        m_stcLcdDevNt35510.u16WRamCmd = 0x2C00U;
        m_stcLcdDevNt35510.u16SetXCmd = 0x2A00U;
        m_stcLcdDevNt35510.u16SetYCmd = 0x2B00U;
        m_stcLcdDevNt35510.u16Width   = LCD_PIXEL_HEIGHT;
        m_stcLcdDevNt35510.u16Height  = LCD_PIXEL_WIDTH;
    }

    NT35510_SetScanDir(LCD_SCAN_DIR);
}

/**
 * @brief  Prepare to write LCD RAM.
 * @param  None
 */
void NT35510_PrepareWriteRAM(void)
{
    BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16WRamCmd);
}

/**
 * @brief  Set screen backlight.
 * @param  [in] u8PWM:          PWM level
           This parameter can be a value between Min_Data = 0 and Max_Data = 100
 * @retval None
 */
void NT35510_SetBackLight(uint8_t u8PWM)
{
    float f32PWM = ((float)u8PWM * 2.55F);

    BSP_NT35510_WriteReg(0xBEU);
    BSP_NT35510_WriteData(0x05U);
    BSP_NT35510_WriteData((uint16_t)f32PWM);
    BSP_NT35510_WriteData(0x01U);
    BSP_NT35510_WriteData(0xFFU);
    BSP_NT35510_WriteData(0x00U);
    BSP_NT35510_WriteData(0x00U);
}


/**
 * @brief  Set Cursor position.
 * @param  u16Xpos:                     specifies the X position.
 * @param  u16Ypos:                     specifies the Y position.
 * @retval None
 */
void NT35510_SetCursor(uint16_t u16Xpos, uint16_t u16Ypos)
{
    if (0x5510U == m_stcLcdDevNt35510.u16ID)
    {
        BSP_NT35510_WriteRegData(m_stcLcdDevNt35510.u16SetXCmd, (u16Xpos >> 8U));
        BSP_NT35510_WriteRegData((m_stcLcdDevNt35510.u16SetXCmd + 1U), (u16Xpos & 0xFFU));
        BSP_NT35510_WriteRegData(m_stcLcdDevNt35510.u16SetYCmd, (u16Ypos >> 8U));
        BSP_NT35510_WriteRegData((m_stcLcdDevNt35510.u16SetYCmd + 1U), (u16Ypos & 0xFFU));
    }
    else if (0x1963U == m_stcLcdDevNt35510.u16ID)
    {
        if (0U == m_stcLcdDevNt35510.u16Dir)
        {
            u16Xpos = (m_stcLcdDevNt35510.u16Width - 1U - u16Xpos);
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd);
            BSP_NT35510_WriteData(0U); BSP_NT35510_WriteData(0U);
            BSP_NT35510_WriteData(u16Xpos >> 8U); BSP_NT35510_WriteData(u16Xpos & 0xFFU);
        }
        else
        {
            BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetXCmd);
            BSP_NT35510_WriteData(u16Xpos >> 8U); BSP_NT35510_WriteData(u16Xpos & 0xFFU);
            BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Width - 1U) >> 8U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Width - 1U) & 0xFFU);
        }

        BSP_NT35510_WriteReg(m_stcLcdDevNt35510.u16SetYCmd);
        BSP_NT35510_WriteData(u16Ypos >> 8U); BSP_NT35510_WriteData(u16Ypos & 0xFFU);
        BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Height - 1U) >> 8U); BSP_NT35510_WriteData((m_stcLcdDevNt35510.u16Height - 1U) & 0xFFU);
    }
    else
    {
        /* todo */
    }
}

/**
 * @brief  Write pixel.
 * @param  u16Xpos:                     specifies the X position.
 * @param  u16Ypos:                     specifies the Y position.
 * @param  u16RGBCode:                  the RGB pixel color in RGB565 format
 * @retval None
 */
void NT35510_WritePixel(uint16_t u16Xpos, uint16_t u16Ypos, uint16_t u16RGBCode)
{
    /* Set cursor */
    NT35510_SetCursor(u16Xpos, u16Ypos);

    /* Prepare to write to LCD RAM */
    NT35510_PrepareWriteRAM();

    BSP_NT35510_WriteData(u16RGBCode);
}

/**
 * @brief  Write line.
 * @param  u16X1:                       specifies the X position 1.
 * @param  u16X2:                       specifies the X position 2.
 * @param  u16Y1:                       specifies the Y position 1.
 * @param  u16Y2:                       specifies the Y position 2.
 * @param  u16RGBCode:                  the RGB pixel color in RGB565 format
 * @retval None
 */
void NT35510_DrawLine(uint16_t u16X1,
                    uint16_t u16Y1,
                    uint16_t u16X2,
                    uint16_t u16Y2,
                    uint16_t u16RGBCode)
{
    int16_t t;
    int16_t xerr = 0;
    int16_t yerr = 0;
    int16_t delta_x;
    int16_t delta_y;
    int16_t distance;
    int16_t incx;
    int16_t incy;
    int16_t Row;
    int16_t Col;

    Row = (int16_t)u16X1;
    Col = (int16_t)u16Y1;
    delta_x = ((int16_t)u16X2 - (int16_t)u16X1);      /* calc delta X, Y*/
    delta_y = ((int16_t)u16Y2 - (int16_t)u16Y1);

    if (delta_x > 0)
    {
        incx = 1;           /* forward u8Direction */
    }
    else if (delta_x == 0)
    {
        incx = 0;           /* vertical line */
    }
    else
    {
        incx = -1;          /* reverse direction */
        delta_x = -delta_x;
    }

    if (delta_y > 0)        /* downward direction */
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;             /* horizontal line */
    }
    else
    {
        incy = -1;            /* upward direction */
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x; /* set axis */
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t <= (distance + 1); t++)
    {
        NT35510_WritePixel((uint16_t)Row, (uint16_t)Col, u16RGBCode);   /* draw pixel */

        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            Row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            Col += incy;
        }
    }
}

/**
 * @brief  Draws a circle.
 * @param  [in]                         u16Xpos: X position
 * @param  [in]                         u16Ypos: Y position
 * @param  [in]                         u16Radius: Circle radius
 * @param  [in]                         u16RGBCode: RGB code
 * @retval None
 */
void NT35510_DrawCircle(uint16_t u16Xpos,
                    uint16_t u16Ypos,
                    uint16_t u16Radius,
                    uint16_t u16RGBCode)
{
    int32_t  decision;       /* Decision Variable */
    uint32_t current_x;      /* Current X Value */
    uint32_t current_y;      /* Current Y Value */

    decision = 3 - ((int32_t)u16Radius * 2);
    current_x = 0U;
    current_y = u16Radius;

    while (current_x <= current_y)
    {
        NT35510_WritePixel((u16Xpos + (uint16_t)current_x), (u16Ypos - (uint16_t)current_y), u16RGBCode);
        NT35510_WritePixel((u16Xpos + (uint16_t)current_y), (u16Ypos - (uint16_t)current_x), u16RGBCode);
        NT35510_WritePixel((u16Xpos + (uint16_t)current_y), (u16Ypos + (uint16_t)current_x), u16RGBCode);
        NT35510_WritePixel((u16Xpos + (uint16_t)current_x), (u16Ypos + (uint16_t)current_y), u16RGBCode);
        NT35510_WritePixel((u16Xpos - (uint16_t)current_x), (u16Ypos + (uint16_t)current_y), u16RGBCode);
        NT35510_WritePixel((u16Xpos - (uint16_t)current_y), (u16Ypos + (uint16_t)current_x), u16RGBCode);
        NT35510_WritePixel((u16Xpos - (uint16_t)current_x), (u16Ypos - (uint16_t)current_y), u16RGBCode);
        NT35510_WritePixel((u16Xpos - (uint16_t)current_y), (u16Ypos - (uint16_t)current_x), u16RGBCode);
        current_x++;
        /* Bresenham algorithm */
        if(decision < 0)
        {
          decision += ((4 * (int32_t)current_x) + 6);
        }
        else
        {
            decision += (10 + (4 * ((int32_t)current_x - (int32_t)current_y)));
            current_y--;
        }
    }
}

/**
 * @brief  Fills a triangle (between 3 points).
 * @param  [in] u16X1:                  Point 1 X position
 * @param  [in] u16Y1:                  Point 1 Y position
 * @param  [in] u16X2:                  Point 2 X position
 * @param  [in] u16Y2:                  Point 2 Y position
 * @param  [in] u16X3:                  Point 3 X position
 * @param  [in] u16Y3:                  Point 3 Y position
 * @param  [in] u16RGBCode:             RGB code
 * @retval None
 */
void NT35510_FillTriangle(uint16_t u16X1,
                        uint16_t u16X2,
                        uint16_t u16X3,
                        uint16_t u16Y1,
                        uint16_t u16Y2,
                        uint16_t u16Y3,
                        uint16_t u16RGBCode)
{
    uint16_t deltax;
    uint16_t deltay;
    int16_t xoff;
    int16_t yoff;
    int16_t xinc1;
    int16_t xinc2;
    int16_t yinc1;
    int16_t yinc2;
    uint16_t den;
    uint16_t num;
    uint16_t numadd;
    uint16_t numpixels;
    uint16_t curpixel;

    xoff = (int16_t)u16X1;                      /* Start x off at the first pixel */
    yoff = (int16_t)u16Y1;                      /* Start y off at the first pixel */

    if (u16X2 > u16X1)              /* The difference between the x's */
    {
        deltax = (u16X2 - u16X1);
    }
    else
    {
        deltax = (u16X1 - u16X2);
    }

    if (u16Y2 > u16Y1)              /* The difference between the y's */
    {
        deltay = (u16Y2 - u16Y1);
    }
    else
    {
        deltay = (u16Y1 - u16Y2);
    }

    if (u16X2 >= u16X1)             /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                            /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (u16Y2 >= u16Y1)             /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                            /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)           /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = (deltax / 2U);
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    }
    else                            /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = (deltay / 2U);
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0U; curpixel <= numpixels; curpixel++)
    {
        NT35510_DrawLine((uint16_t)xoff, (uint16_t)yoff, u16X3, u16Y3, u16RGBCode);

        num += numadd;              /* Increase the numerator by the top of the fraction */
        if (num >= den)             /* Check if numerator >= denominator */
        {
            num -= den;             /* Calculate the new numerator value */
            xoff += xinc1;             /* Change the x as appropriate */
            yoff += yinc1;             /* Change the y as appropriate */
        }
        xoff += xinc2;                 /* Change the x as appropriate */
        yoff += yinc2;                 /* Change the y as appropriate */
    }
}


/**
 * @brief  Draw rectangle.
 * @param  [in] u16X1:                  Point 1 X position
 * @param  [in] u16Y1:                  Point 1 Y position
 * @param  [in] u16X2:                  Point 2 X position
 * @param  [in] u16Y2:                  Point 2 Y position
 * @param  [in] u16RGBCode:             RGB code
 * @retval None
 */
void NT35510_DrawRectangle(uint16_t u16X1,
                            uint16_t u16Y1,
                            uint16_t u16X2,
                            uint16_t u16Y2,
                            uint16_t u16RGBCode)
{
    NT35510_DrawLine(u16X1, u16Y1, u16X2, u16Y1, u16RGBCode);
    NT35510_DrawLine(u16X1, u16Y1, u16X1, u16Y2, u16RGBCode);
    NT35510_DrawLine(u16X1, u16Y2, u16X2, u16Y2, u16RGBCode);
    NT35510_DrawLine(u16X2, u16Y1, u16X2, u16Y2, u16RGBCode);
}

/**
 * @brief  Clear screen.
 * @param  [in] u16RGBCode:             RGB code
 * @retval None
 */
void NT35510_Clear(uint16_t u16RGBCode)
{
    uint32_t i;

    /* Set cursor */
    NT35510_SetCursor(0U, 0U);

    /* Prepare to write to LCD RAM */
    NT35510_PrepareWriteRAM();

    for(i = 0UL; i < (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); i++)
    {
        BSP_NT35510_WriteData(u16RGBCode);
    }
}

/**
 * @}
 */

/**
 * @defgroup NT35510_Local_Functions NT35510 Local Functions
 * @{
 */

/**
 * @brief  LCD delay
 * @param  delay: Delay in ms
 * @retval None
 */
static void NT35510_Delay(uint32_t u32Delay)
{
    DDL_DelayMS(u32Delay);
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
