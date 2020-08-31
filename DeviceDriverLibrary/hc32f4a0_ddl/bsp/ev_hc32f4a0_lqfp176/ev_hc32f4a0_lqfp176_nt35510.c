/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_nt35510.c
 * @brief This file provides firmware functions for LCD.
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
#include "ev_hc32f4a0_lqfp176.h"
#include "ev_hc32f4a0_lqfp176_nt35510.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */

/** @defgroup EV_HC32F4A0_LQFP176_NT35510 HC32F4A0 EVB NT35510
  * @{
  */

#if (BSP_NT35510_ENABLE == BSP_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup ADC_Global_Types ADC Global Types
 * @{
 */

/**
 * @defgroup EV_HC32F4A0_LQFP176_NT35510_Local_Types HC32F4A0 EVB LQFP176 NT35510 Local Types
 * @{
 */

/**
 * @brief LCD Device Controller Structure Definition
 */
typedef struct
{
    __IO uint16_t REG;
    __IO uint16_t RAM;
} LCD_Controller_Typedef;

/**
 * @}
 */

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_NT35510_Local_Macros HC32F4A0 EVB LQFP176 NT35510 Local Macros
 * @{
 */

/**
 * @defgroup LCD_Interface_Pin LCD Interface Pin
 * @{
 */
#define LCD_CS                          (EXMC_SMC_CHIP_3)

#define LCD_CS_PORT                     (GPIO_PORT_G)   /* PG10 - EXMC_CE2 */
#define LCD_CS_PIN                      (GPIO_PIN_12)

#define LCD_WE_PORT                     (GPIO_PORT_C)   /* PC00 - EXMC_WE */
#define LCD_WE_PIN                      (GPIO_PIN_00)

#define LCD_OE_PORT                     (GPIO_PORT_F)   /* PF11 - EXMC_OE */
#define LCD_OE_PIN                      (GPIO_PIN_11)

#define LCD_RS_PORT                     (GPIO_PORT_G)   /* PG02 - EXMC_ADD12 for LCD_RS */
#define LCD_RS_PIN                      (GPIO_PIN_02)

#define LCD_DATA0_PORT                  (GPIO_PORT_D)   /* PD14 - EXMC_DATA0 */
#define LCD_DATA0_PIN                   (GPIO_PIN_14)
#define LCD_DATA1_PORT                  (GPIO_PORT_D)   /* PD15 - EXMC_DATA1 */
#define LCD_DATA1_PIN                   (GPIO_PIN_15)
#define LCD_DATA2_PORT                  (GPIO_PORT_D)   /* PD00 - EXMC_DATA2 */
#define LCD_DATA2_PIN                   (GPIO_PIN_00)
#define LCD_DATA3_PORT                  (GPIO_PORT_D)   /* PD01 - EXMC_DATA3 */
#define LCD_DATA3_PIN                   (GPIO_PIN_01)
#define LCD_DATA4_PORT                  (GPIO_PORT_E)   /* PE07 - EXMC_DATA4 */
#define LCD_DATA4_PIN                   (GPIO_PIN_07)
#define LCD_DATA5_PORT                  (GPIO_PORT_E)   /* PE08 - EXMC_DATA5 */
#define LCD_DATA5_PIN                   (GPIO_PIN_08)
#define LCD_DATA6_PORT                  (GPIO_PORT_E)   /* PE09 - EXMC_DATA6 */
#define LCD_DATA6_PIN                   (GPIO_PIN_09)
#define LCD_DATA7_PORT                  (GPIO_PORT_E)   /* PE10 - EXMC_DATA7 */
#define LCD_DATA7_PIN                   (GPIO_PIN_10)
#define LCD_DATA8_PORT                  (GPIO_PORT_E)   /* PE11 - EXMC_DATA8 */
#define LCD_DATA8_PIN                   (GPIO_PIN_11)
#define LCD_DATA9_PORT                  (GPIO_PORT_E)   /* PE12 - EXMC_DATA9 */
#define LCD_DATA9_PIN                   (GPIO_PIN_12)
#define LCD_DATA10_PORT                 (GPIO_PORT_E)   /* PE13 - EXMC_DATA10 */
#define LCD_DATA10_PIN                  (GPIO_PIN_13)
#define LCD_DATA11_PORT                 (GPIO_PORT_E)   /* PE14 - EXMC_DATA11 */
#define LCD_DATA11_PIN                  (GPIO_PIN_14)
#define LCD_DATA12_PORT                 (GPIO_PORT_E)   /* PE15 - EXMC_DATA12 */
#define LCD_DATA12_PIN                  (GPIO_PIN_15)
#define LCD_DATA13_PORT                 (GPIO_PORT_D)   /* PD08 - EXMC_DATA13 */
#define LCD_DATA13_PIN                  (GPIO_PIN_08)
#define LCD_DATA14_PORT                 (GPIO_PORT_D)   /* PD09 - EXMC_DATA14 */
#define LCD_DATA14_PIN                  (GPIO_PIN_09)
#define LCD_DATA15_PORT                 (GPIO_PORT_D)   /* PD10 - EXMC_DATA15 */
#define LCD_DATA15_PIN                  (GPIO_PIN_10)
/**
 * @}
 */

/* Use EXMC CS3, A12 as the RS signal */
#define LCD_BASE                        (0x60000000UL | ((1UL << 13U) - 2UL))

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
 * @addtogroup EV_HC32F4A0_LQFP176_NT35510_Local_Functions
 * @{
 */
static void LCD_Port_Init(void);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_NT35510_Local_Variables HC32F4A0 LQFP176 EVB NT35510 Local Variables
 * @{
 */
static LCD_Controller_Typedef *LCD = ((LCD_Controller_Typedef *)LCD_BASE);
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_NT35510_Global_Functions HC32F4A0 LQFP176 EVB NT35510 Global Functions
 * @{
 */

/**
 * @brief  Initializes LCD low level.
 * @param  None
 * @retval An en_result_t enumeration value:
 *           - Ok: Initialize successfully.
 *           - Error: Initialize unsuccessfully.
 */
en_result_t BSP_NT35510_Init(void)
{
    en_result_t enRet;
    stc_exmc_smc_init_t stcSmcInit;

    LCD_Port_Init();

    /* Enable SMC module clk */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_SMC, Enable);

    /* Enable SMC. */
    EXMC_SMC_Cmd(Enable);

    EXMC_SMC_ExitLowPower();
    while (EXMC_SMC_READY != EXMC_SMC_GetStatus())
    {}

    EXMC_SMC_StructInit(&stcSmcInit);

    /*0x6000_0000 ~ 0x60FFFFFF, 16Mb area */
    stcSmcInit.stcChipCfg.u32AddressMatch = 0x60UL;
    stcSmcInit.stcChipCfg.u32AddressMask  = 0xFFUL;
    stcSmcInit.stcChipCfg.u32SmcMemWidth  = EXMC_SMC_MEMORY_WIDTH_16BIT;
    stcSmcInit.stcChipCfg.u32BAA  = EXMC_SMC_BAA_PORT_DISABLE;
    stcSmcInit.stcChipCfg.u32ADV  = EXMC_SMC_ADV_PORT_DISABLE;
    stcSmcInit.stcChipCfg.u32BLS  = EXMC_SMC_BLS_SYNC_CS;
    stcSmcInit.stcChipCfg.u32ReadBurstLen  = EXMC_SMC_MEM_READ_BURST_4;
    stcSmcInit.stcChipCfg.u32WriteBurstLen  = EXMC_SMC_MEM_WRITE_BURST_4;
    stcSmcInit.stcChipCfg.u32ReadMode  = EXMC_SMC_MEM_READ_SYNC;
    stcSmcInit.stcChipCfg.u32WriteMode  = EXMC_SMC_MEM_WRITE_SYNC;

    stcSmcInit.stcTimingCfg.u32RC = 6UL;
    stcSmcInit.stcTimingCfg.u32WC = 4UL;
    stcSmcInit.stcTimingCfg.u32CEOE = 1UL;
    stcSmcInit.stcTimingCfg.u32WP = 2UL;
    stcSmcInit.stcTimingCfg.u32PC = 4UL;
    stcSmcInit.stcTimingCfg.u32TR = 1UL;

    EXMC_SMC_Init(LCD_CS, &stcSmcInit);

    /* Set command: updateregs */
    EXMC_SMC_SetCommand(LCD_CS, EXMC_SMC_CMD_UPDATEREGS, 0UL, 0UL);

    /* Check timing status */
    do {
        enRet = EXMC_SMC_CheckTimingStatus(LCD_CS, &stcSmcInit.stcTimingCfg);
    } while (Ok != enRet);

    /* Check chip status */
    do {
        enRet = EXMC_SMC_CheckChipStatus(LCD_CS, &stcSmcInit.stcChipCfg);
    } while (Ok != enRet);

    return enRet;
}

/**
 * @brief  Writes data on LCD data register.
 * @param  [in] u16Data:                Data to be written
 * @retval None
 */
void BSP_NT35510_WriteData(uint16_t u16Data)
{
    /* Write 16-bit Reg */
    LCD->RAM = u16Data;
}

/**
 * @brief  Writes several data on LCD data register.
 * @param  [in] au16Data:               Pointer on data to be written
 * @param  [in] u32Size:                Data amount in 16bits short unit
 * @retval None
 */
en_result_t BSP_NT35510_WriteMultipleData(const uint16_t au16Data[],
                                            uint32_t u32Size)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if ((au16Data != NULL) && (u32Size > 0UL))
    {
        for (i = 0UL; i < u32Size; i++)
        {
            LCD->RAM = au16Data[i];
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Writes register on LCD register.
 * @param  [in] u16Reg:                 Address of the selected register.
 * @retval None
 */
void BSP_NT35510_WriteReg(uint16_t u16Reg)
{
    /* Write 16-bit Index, then Write Reg */
    LCD->REG = u16Reg;
}

/**
 * @brief  Reads data from LCD data register.
 * @retval Read data.
 */
uint16_t BSP_NT35510_ReadData(void)
{
    return LCD->RAM;
}

/**
 * @brief  Writes to the selected LCD register.
 * @param  [in] u16Reg:                 Address of the selected register.
 * @param  [in] u16Data:                Data to be written
 * @retval None
 */
void BSP_NT35510_WriteRegData(uint16_t u16Reg, uint16_t u16Data)
{
    /* Write 16-bit index */
    LCD->REG = u16Reg;

    /* Write 16-bit Reg */
    LCD->RAM = u16Data;
}

/**
 * @brief  Read the selected LCD register.
 * @param  [in] u16Reg:                 Address of the selected register.
 * @retval Register value
 */
uint16_t BSP_NT35510_ReadRegData(uint16_t u16Reg)
{
    /* Write 16-bit index*/
    LCD->REG = u16Reg;

    return LCD->RAM;;
}

/**
 * @}
 */

/**
 * @defgroup EV_HC32F4A0_LQFP176_NT35510_Local_Functions HC32F4A0 LQFP176 EVB NT35510 Local Functions
 * @{
 */

/**
 * @brief  Initializes LCD gpio.
 */
static void LCD_Port_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_DRV_HIGH;

    /* LCD_CS */
    GPIO_Init(LCD_CS_PORT, LCD_CS_PIN, &stcGpioInit);

    /* LCD_WE */
    GPIO_Init(LCD_WE_PORT, LCD_WE_PIN, &stcGpioInit);

    /* LCD_OE */
    GPIO_Init(LCD_OE_PORT, LCD_OE_PIN, &stcGpioInit);

    /* SMC_DATA[0:15] */
    GPIO_Init(LCD_DATA0_PORT,  LCD_DATA0_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA1_PORT,  LCD_DATA1_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA2_PORT,  LCD_DATA2_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA3_PORT,  LCD_DATA3_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA4_PORT,  LCD_DATA4_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA5_PORT,  LCD_DATA5_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA6_PORT,  LCD_DATA6_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA7_PORT,  LCD_DATA7_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA8_PORT,  LCD_DATA8_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA9_PORT,  LCD_DATA9_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA10_PORT, LCD_DATA10_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA11_PORT, LCD_DATA11_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA12_PORT, LCD_DATA12_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA13_PORT, LCD_DATA13_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA14_PORT, LCD_DATA14_PIN, &stcGpioInit);
    GPIO_Init(LCD_DATA15_PORT, LCD_DATA15_PIN, &stcGpioInit);

    GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, &stcGpioInit);

    /* LCD_DATA[0:15] */
    GPIO_SetFunc(LCD_DATA0_PORT,  LCD_DATA0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA1_PORT,  LCD_DATA1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA2_PORT,  LCD_DATA2_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA3_PORT,  LCD_DATA3_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA4_PORT,  LCD_DATA4_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA5_PORT,  LCD_DATA5_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA6_PORT,  LCD_DATA6_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA7_PORT,  LCD_DATA7_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA8_PORT,  LCD_DATA8_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA9_PORT,  LCD_DATA9_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA10_PORT, LCD_DATA10_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA11_PORT, LCD_DATA11_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA12_PORT, LCD_DATA12_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA13_PORT, LCD_DATA13_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA14_PORT, LCD_DATA14_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_DATA15_PORT, LCD_DATA15_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    GPIO_SetFunc(LCD_CS_PORT, LCD_CS_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_RS_PORT, LCD_RS_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_WE_PORT, LCD_WE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(LCD_OE_PORT, LCD_OE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
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
