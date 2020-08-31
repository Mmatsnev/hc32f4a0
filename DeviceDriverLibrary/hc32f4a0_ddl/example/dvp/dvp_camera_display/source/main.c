/**
 *******************************************************************************
 * @file  dvp/dvp_camera_display/main.c
 * @brief This example demonstrates DVP capture function.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-08-25       Hongjh          First version
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

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup DVP_Camera_Display
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* LCD RAM address definition */
#define LCD_RAM_ADDR                (0x60002000UL)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static void CAM_Init(void);
static void LCD_Init(void);
static void DVP_DMA_Init(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  MCU Peripheral registers write unprotected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WE(void)
{
    /* Unlock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Unlock();
    /* Unlock PWC register: FCG0 */
    PWC_FCG0_Unlock();
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
//    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
//    EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
//    EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Lock();
    /* Unlock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
//    SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
//    EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
//    EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  Initialize camera.
 * @param  None
 * @retval None
 */
static void CAM_Init(void)
{
    /* Initialize OV5640 */
    OV5640_Init();

    /* Set OV5640 mode: RGB565 */
    OV5640_RGB565_Mode();

    /* Set OV5640 out size */
    OV5640_SetOutSize(0x0190U, 0U, 480U, 800U);
}

/**
 * @brief  Initialize LCD.
 * @param  None
 * @retval None
 */
static void LCD_Init(void)
{
    /* Initialize NT35510 LCD */
    NT35510_Init();

    /* Clear LCD screen */
    NT35510_Clear(LCD_COLOR_RED);

    /* Turn on LCD backlight */
    BSP_LCD_BKLCmd(EIO_PIN_SET);

    NT35510_DrawRectangle(100,
                            100,
                            400,
                            700,
                            LCD_COLOR_GREEN);
    /* Set LCD cursor */
    NT35510_SetCursor(0U, 0U);

    /* Prepare to write to LCD RAM */
    NT35510_PrepareWriteRAM();
}

/**
 * @brief  Initialize DMA for DVP.
 * @param  None
 * @retval None
 */
static void DVP_DMA_Init(void)
{
    stc_dma_init_t stcDmaInit;
    stc_dma_rpt_init_t stcDmaRptInit;

    static uint32_t DVPData;

    /*********************** DMA1_0 for DVP DMA request ***********************/
    /* Initialize DMA1_0 */
    DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32SrcAddr   = (uint32_t)(&M4_DVP->DMR);
    stcDmaInit.u32DestAddr  = (uint32_t)(&DVPData);
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_32BIT;
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_FIX;
    stcDmaInit.u32IntEn     = DMA_INT_DISABLE;
    stcDmaInit.u32TransCnt  = 0UL;
    stcDmaInit.u32BlockSize = 1UL;
    DMA_Init(M4_DMA1, DMA_CH0, &stcDmaInit);

    /* DMA1_0 triggger source for DVP DMA request */
    DMA_SetTriggerSrc(M4_DMA1, DMA_CH0, EVT_DVP_DMAREQ);

    /*********************** DMA2_0 for LCD ***********************************/
    /* Initialize DMA2_0 */
    stcDmaInit.u32SrcAddr   = (uint32_t)(&DVPData);
    stcDmaInit.u32DestAddr  = LCD_RAM_ADDR;
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_16BIT;
    stcDmaInit.u32SrcInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc   = DMA_DEST_ADDR_FIX;
    stcDmaInit.u32IntEn     = DMA_INT_DISABLE;
    stcDmaInit.u32TransCnt  = 0UL;
    stcDmaInit.u32BlockSize = 2UL;
    DMA_Init(M4_DMA2, DMA_CH0, &stcDmaInit);

    /* Set DMA2_0 repeat function */
    DMA_RepeatStructInit(&stcDmaRptInit);
    stcDmaRptInit.u32SrcRptEn   = DMA_SRC_RPT_ENABLE;
    stcDmaRptInit.u32SrcRptSize = stcDmaInit.u32BlockSize;
    DMA_RepeatInit(M4_DMA2, DMA_CH0, &stcDmaRptInit);

    /* DMA2_0 triggger source for DMA1_BTC0 */
    DMA_SetTriggerSrc(M4_DMA2, DMA_CH0, EVT_DMA1_BTC0);

    /*********************** Enable DMA ***************************************/
    DMA_Cmd(M4_DMA1, Enable);
    DMA_Cmd(M4_DMA2, Enable);

    DMA_ChannelCmd(M4_DMA1, DMA_CH0, Enable);
    DMA_ChannelCmd(M4_DMA2, DMA_CH0, Enable);
}

/**
 * @brief  Main function of DVP camera display project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_dvp_init_t stcDvpInit;

    /* Unlock peripherals or registers */
    Peripheral_WE();

    /* Enable DMA&AOS module clk */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_DMA1 | PWC_FCG0_DMA2 | PWC_FCG0_AOS, Enable);

    /* Enable DVP module clk */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_DVP, Enable);

    /* Configure clock */
    BSP_CLK_Init();

    /* EXBUS CLK 120MHz */
    CLK_ClkDiv(CLK_CATE_EXCLK, CLK_EXCLK_DIV2);

    /* Initialize CAM/LCD IO */
    BSP_IO_Init();
    BSP_LED_Init();
    BSP_CAM_IO_Init();
    BSP_LCD_IO_Init();
    BSP_KEY_Init();

    /* HW Reset LCD */
    BSP_LCD_RSTCmd(EIO_PIN_RESET);
    BSP_CAM_RSTCmd(EIO_PIN_SET);    /* RST# to low */
    DDL_DelayMS(50UL);
    BSP_LCD_RSTCmd(EIO_PIN_SET);
    BSP_CAM_RSTCmd(EIO_PIN_RESET);  /* RST# to high */
    BSP_CAM_STBCmd(EIO_PIN_SET);    /* STB# to low */
    DDL_DelayMS(50UL);

    /* Initialize CAM */
    CAM_Init();

    /* Initialize LCD */
    LCD_Init();

    /* Lock peripherals or registers */
    Peripheral_WP();

    /* Initialize DMA for DVP */
    DVP_DMA_Init();

    /* Initiliaze DVP */
    DVP_StructInit(&stcDvpInit);
    stcDvpInit.u32PIXCLKPolarity = DVP_PIXCLK_RISING;
    stcDvpInit.u32CaptureMode  = DVP_CAPT_MD_CONTINUOS_FRAME;
    DVP_Init(&stcDvpInit);

    /* Enable DVP */
    DVP_Cmd(Enable);

    /* Enable DVP Capture*/
    DVP_CaptrueCmd(Enable);

    for (;;)
    {
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
