/**
 *******************************************************************************
 * @file  ms_hc32f4a0_lqfp176_050_mem_cy62167ev30ll.c
 * @brief This file provides configure functions for cy62167ev30ll of the board 
 *        MS_HC32F4A0_LQF176_050_MEM.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
   2020-07-03       Hongjh          Adjust EXMC pin drive capacity to high drive
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
#include "ms_hc32f4a0_lqfp176_050_mem_cy62167ev30ll.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup MS_HC32F4A0_LQFP176_050_MEM
 * @{
 */

/** @defgroup MS_HC32F4A0_LQFP176_050_MEM_CY62167EV30LL HC32F4A0 MS LQFP176 CY62167EV30LL
 * @{
 */

#if ((BSP_ON == BSP_CY62167EV30LL_ENABLE) && \
     (BSP_MS_HC32F4A0_LQFP176_050_MEM == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup MS_HC32F4A0_LQFP176_050_MEM_CY62167EV30LL_Local_Macros HC32F4A0 MS LQFP176 CY62167EV30LL Local Macros
 * @{
 */

/**
 * @defgroup CY62167EV30LL_Map_SMC_Chip CY62167EV30LL Map SMC Chip
 * @{
 */
#define CY62167EV30LL_MAP_SMC_CHIP                (EXMC_SMC_CHIP_0)
/**
 * @}
 */

/**
 * @defgroup CY62167EV30LL_SMC_Address_Space CY62167EV30LL DMC Address Space
 * @{
 */
#define CY62167EV30LL_SMC_MATCH_ADDR              (0x60UL)
#define CY62167EV30LL_SMC_MASK_ADDR               (0xFFUL)
/**
 * @}
 */

/**
 * @defgroup CY62167EV30LL_SRAM_Address_Space CY62167EV30LL SRAM Address Space
 * @note SRAM address:[0x60000000, 0x607FFFFF] / size: 8M bytes
 * @{
 */
#define SRAM_CY62167EV30LL_START_ADDR           (EXMC_SMC_ChipStartAddress(CY62167EV30LL_MAP_SMC_CHIP))
#define SRAM_CY62167EV30LL_SIZE                 (2UL * 1024UL * 1024UL)     /* 2MBytes*/
#define SRAM_CY62167EV30LL_END_ADDR             (SRAM_CY62167EV30LL_START_ADDR + SRAM_CY62167EV30LL_SIZE - 1UL)
/**
 * @}
 */

/**
 * @defgroup SMC_Interface_Pin EXMC SMC Interface Pin
 * @{
 */

#define SMC_IO1_PORT                            (GPIO_PORT_G)   /* PG12 - IO1(Device pin: nBYTE) */
#define SMC_IO1_PIN                             (GPIO_PIN_12)
#define SMC_IO2_PORT                            (GPIO_PORT_G)   /* PG13 - IO2(Device pin: CE2) */
#define SMC_IO2_PIN                             (GPIO_PIN_13)

#define SMC_CS_PORT                             (GPIO_PORT_D)   /* PD07 - EXMC_CE0 */
#define SMC_CS_PIN                              (GPIO_PIN_07)

#define SMC_WE_PORT                             (GPIO_PORT_D)   /* PD05 - EXMC_WE */
#define SMC_WE_PIN                              (GPIO_PIN_05)

#define SMC_BLS0_PORT                           (GPIO_PORT_E)   /* PE00 - EXMC_CE4 */
#define SMC_BLS0_PIN                            (GPIO_PIN_00)
#define SMC_BLS1_PORT                           (GPIO_PORT_E)   /* PE01 - EXMC_CE5 */
#define SMC_BLS1_PIN                            (GPIO_PIN_01)

#define SMC_OE_PORT                             (GPIO_PORT_D)   /* PD04 - EXMC_OE */
#define SMC_OE_PIN                              (GPIO_PIN_04)

#define SMC_ADD0_PORT                           (GPIO_PORT_F)   /* PF00 - EXMC_ADD0 */
#define SMC_ADD0_PIN                            (GPIO_PIN_00)
#define SMC_ADD1_PORT                           (GPIO_PORT_F)   /* PF01 - EXMC_ADD1 */
#define SMC_ADD1_PIN                            (GPIO_PIN_01)
#define SMC_ADD2_PORT                           (GPIO_PORT_F)   /* PF02 - EXMC_ADD2 */
#define SMC_ADD2_PIN                            (GPIO_PIN_02)
#define SMC_ADD3_PORT                           (GPIO_PORT_F)   /* PF03 - EXMC_ADD3 */
#define SMC_ADD3_PIN                            (GPIO_PIN_03)
#define SMC_ADD4_PORT                           (GPIO_PORT_F)   /* PF04 - EXMC_ADD4 */
#define SMC_ADD4_PIN                            (GPIO_PIN_04)
#define SMC_ADD5_PORT                           (GPIO_PORT_F)   /* PF05 - EXMC_ADD5 */
#define SMC_ADD5_PIN                            (GPIO_PIN_05)
#define SMC_ADD6_PORT                           (GPIO_PORT_F)   /* PF12 - EXMC_ADD6 */
#define SMC_ADD6_PIN                            (GPIO_PIN_12)
#define SMC_ADD7_PORT                           (GPIO_PORT_F)   /* PF13 - EXMC_ADD7 */
#define SMC_ADD7_PIN                            (GPIO_PIN_13)
#define SMC_ADD8_PORT                           (GPIO_PORT_F)   /* PF14 - EXMC_ADD8 */
#define SMC_ADD8_PIN                            (GPIO_PIN_14)
#define SMC_ADD9_PORT                           (GPIO_PORT_F)   /* PF15 - EXMC_ADD9 */
#define SMC_ADD9_PIN                            (GPIO_PIN_15)
#define SMC_ADD10_PORT                          (GPIO_PORT_G)   /* PG00 - EXMC_ADD10 */
#define SMC_ADD10_PIN                           (GPIO_PIN_00)
#define SMC_ADD11_PORT                          (GPIO_PORT_G)   /* PG01 - EXMC_ADD11 */
#define SMC_ADD11_PIN                           (GPIO_PIN_01)
#define SMC_ADD12_PORT                          (GPIO_PORT_G)   /* PG02 - EXMC_ADD12 */
#define SMC_ADD12_PIN                           (GPIO_PIN_02)
#define SMC_ADD13_PORT                          (GPIO_PORT_G)   /* PG03 - EXMC_ADD13 */
#define SMC_ADD13_PIN                           (GPIO_PIN_03)
#define SMC_ADD14_PORT                          (GPIO_PORT_G)   /* PG04 - EXMC_ADD14 */
#define SMC_ADD14_PIN                           (GPIO_PIN_04)
#define SMC_ADD15_PORT                          (GPIO_PORT_G)   /* PG05 - EXMC_ADD15 */
#define SMC_ADD15_PIN                           (GPIO_PIN_05)
#define SMC_ADD16_PORT                          (GPIO_PORT_D)   /* PD11 - EXMC_ADD16 */
#define SMC_ADD16_PIN                           (GPIO_PIN_11)
#define SMC_ADD17_PORT                          (GPIO_PORT_D)   /* PD12 - EXMC_ADD17 */
#define SMC_ADD17_PIN                           (GPIO_PIN_12)
#define SMC_ADD18_PORT                          (GPIO_PORT_D)   /* PD13 - EXMC_ADD18 */
#define SMC_ADD18_PIN                           (GPIO_PIN_13)
#define SMC_ADD19_PORT                          (GPIO_PORT_E)   /* PE02 - EXMC_ADD19 */
#define SMC_ADD19_PIN                           (GPIO_PIN_03)
#define SMC_ADD20_PORT                          (GPIO_PORT_E)   /* PE04 - EXMC_ADD20 */
#define SMC_ADD20_PIN                           (GPIO_PIN_04)


#define SMC_DATA0_PORT                          (GPIO_PORT_D)   /* PD14 - EXMC_DATA0 */
#define SMC_DATA0_PIN                           (GPIO_PIN_14)
#define SMC_DATA1_PORT                          (GPIO_PORT_D)   /* PD15 - EXMC_DATA1 */
#define SMC_DATA1_PIN                           (GPIO_PIN_15)
#define SMC_DATA2_PORT                          (GPIO_PORT_D)   /* PD00 - EXMC_DATA2 */
#define SMC_DATA2_PIN                           (GPIO_PIN_00)
#define SMC_DATA3_PORT                          (GPIO_PORT_D)   /* PD01 - EXMC_DATA3 */
#define SMC_DATA3_PIN                           (GPIO_PIN_01)
#define SMC_DATA4_PORT                          (GPIO_PORT_E)   /* PE07 - EXMC_DATA4 */
#define SMC_DATA4_PIN                           (GPIO_PIN_07)
#define SMC_DATA5_PORT                          (GPIO_PORT_E)   /* PE08 - EXMC_DATA5 */
#define SMC_DATA5_PIN                           (GPIO_PIN_08)
#define SMC_DATA6_PORT                          (GPIO_PORT_E)   /* PE09 - EXMC_DATA6 */
#define SMC_DATA6_PIN                           (GPIO_PIN_09)
#define SMC_DATA7_PORT                          (GPIO_PORT_E)   /* PE10 - EXMC_DATA7 */
#define SMC_DATA7_PIN                           (GPIO_PIN_10)
#define SMC_DATA8_PORT                          (GPIO_PORT_E)   /* PE11 - EXMC_DATA8 */
#define SMC_DATA8_PIN                           (GPIO_PIN_11)
#define SMC_DATA9_PORT                          (GPIO_PORT_E)   /* PE12 - EXMC_DATA9 */
#define SMC_DATA9_PIN                           (GPIO_PIN_12)
#define SMC_DATA10_PORT                         (GPIO_PORT_E)   /* PE13 - EXMC_DATA10 */
#define SMC_DATA10_PIN                          (GPIO_PIN_13)
#define SMC_DATA11_PORT                         (GPIO_PORT_E)   /* PE14 - EXMC_DATA11 */
#define SMC_DATA11_PIN                          (GPIO_PIN_14)
#define SMC_DATA12_PORT                         (GPIO_PORT_E)   /* PE15 - EXMC_DATA12 */
#define SMC_DATA12_PIN                          (GPIO_PIN_15)
#define SMC_DATA13_PORT                         (GPIO_PORT_D)   /* PD08 - EXMC_DATA13 */
#define SMC_DATA13_PIN                          (GPIO_PIN_08)
#define SMC_DATA14_PORT                         (GPIO_PORT_D)   /* PD09 - EXMC_DATA14 */
#define SMC_DATA14_PIN                          (GPIO_PIN_09)
#define SMC_DATA15_PORT                         (GPIO_PORT_D)   /* PD10 - EXMC_DATA15 */
#define SMC_DATA15_PIN                          (GPIO_PIN_10)
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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @addtogroup MS_HC32F4A0_LQFP176_050_MEM_CY62167EV30LL_Local_Functions
 * @{
 */

static void EV_EXMC_SMC_PortInit(void);

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup MS_HC32F4A0_LQFP176_050_MEM_CY62167EV30LL_Global_Functions HC32F4A0 MS LQFP176 CY62167EV30LL Global Functions
 * @{
 */

/**
 * @brief  Initialize SMC for CY62167EV30LL.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 */
en_result_t BSP_SMC_CY62167EV30LL_Init(void)
{
    en_result_t enRet;
    stc_exmc_smc_init_t stcSmcInit;

    /* Initialize SMC port. */
    EV_EXMC_SMC_PortInit();

    /* Enable SMC module clk */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_SMC, Enable);

    /* Enable SMC. */
    EXMC_SMC_Cmd(Enable);

    EXMC_SMC_ExitLowPower();
    while (EXMC_SMC_READY != EXMC_SMC_GetStatus())
    {}

    /* Configure SMC width && CS &chip & timing. */
    stcSmcInit.stcChipCfg.u32ReadMode = EXMC_SMC_MEM_READ_ASYNC;
    stcSmcInit.stcChipCfg.u32ReadBurstLen = EXMC_SMC_MEM_READ_BURST_1;
    stcSmcInit.stcChipCfg.u32WriteMode = EXMC_SMC_MEM_WRITE_ASYNC;
    stcSmcInit.stcChipCfg.u32WriteBurstLen = EXMC_SMC_MEM_WRITE_BURST_1;
    stcSmcInit.stcChipCfg.u32SmcMemWidth = EXMC_SMC_MEMORY_WIDTH_16BIT;
    stcSmcInit.stcChipCfg.u32BAA = EXMC_SMC_BAA_PORT_DISABLE;
    stcSmcInit.stcChipCfg.u32ADV = EXMC_SMC_ADV_PORT_DISABLE;
    stcSmcInit.stcChipCfg.u32BLS = EXMC_SMC_BLS_SYNC_CS;
    stcSmcInit.stcChipCfg.u32AddressMatch = CY62167EV30LL_SMC_MATCH_ADDR;
    stcSmcInit.stcChipCfg.u32AddressMask = CY62167EV30LL_SMC_MASK_ADDR;
    stcSmcInit.stcTimingCfg.u32RC = 5UL;
    stcSmcInit.stcTimingCfg.u32WC = 6UL;
    stcSmcInit.stcTimingCfg.u32CEOE = 2UL;
    stcSmcInit.stcTimingCfg.u32WP = 3UL;
    stcSmcInit.stcTimingCfg.u32PC = 0UL;
    stcSmcInit.stcTimingCfg.u32TR = 0UL;
    EXMC_SMC_Init(CY62167EV30LL_MAP_SMC_CHIP, &stcSmcInit);

    /* Set command: updateregs */
    EXMC_SMC_SetCommand(CY62167EV30LL_MAP_SMC_CHIP, EXMC_SMC_CMD_UPDATEREGS, 0UL, 0UL);

    /* Check timing status */
    do {
        enRet = EXMC_SMC_CheckTimingStatus(CY62167EV30LL_MAP_SMC_CHIP, &stcSmcInit.stcTimingCfg);
    } while (Ok != enRet);

    /* Check chip status */
    do {
        enRet = EXMC_SMC_CheckChipStatus(CY62167EV30LL_MAP_SMC_CHIP, &stcSmcInit.stcChipCfg);
    } while (Ok != enRet);

    return Ok;
}

/**
 * @brief  Get memory information.
 * @param  [out] pu32MemStartAddr       The pointer for memory start address
 * @param  [out] pu32MemByteSize        The pointer for memory size(unit: Byte)
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer pu32MemStartAddr and pu32MemByteSize is NULL.

 */
void BSP_SMC_CY62167EV30LL_GetMemInfo(uint32_t *pu32MemStartAddr,
                                        uint32_t *pu32MemByteSize)
{
    if (NULL != pu32MemStartAddr)
    {
        *pu32MemStartAddr = SRAM_CY62167EV30LL_START_ADDR;
    }

    if (NULL != pu32MemByteSize)
    {
        *pu32MemByteSize = SRAM_CY62167EV30LL_SIZE;
    }
}

/**
 * @}
 */

/**
 * @defgroup MS_HC32F4A0_LQFP176_050_MEM_CY62167EV30LL_Local_Functions HC32F4A0 MS LQFP176 CY62167EV30LL Local Functions
 * @{
 */

/**
 * @brief  Initialize SMC port.
 * @param  None
 * @retval None
 */
static void EV_EXMC_SMC_PortInit(void)
{
    stc_gpio_init_t stcGpioInit;

    /************************* Set pin drive capacity *************************/
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_DRV_HIGH;
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    GPIO_Init(SMC_IO1_PORT, SMC_IO1_PIN, &stcGpioInit);
    GPIO_Init(SMC_IO2_PORT, SMC_IO2_PIN, &stcGpioInit);
    GPIO_SetPins(SMC_IO1_PORT, SMC_IO1_PIN);
    GPIO_SetPins(SMC_IO2_PORT, SMC_IO2_PIN);

    /* SMC_CS */
    GPIO_Init(SMC_CS_PORT, SMC_CS_PIN, &stcGpioInit);

    /* SMC_WE */
    GPIO_Init(SMC_WE_PORT, SMC_WE_PIN, &stcGpioInit);

    /* SMC_BLS[0:1] */
    GPIO_Init(SMC_BLS0_PORT, SMC_BLS0_PIN, &stcGpioInit);
    GPIO_Init(SMC_BLS1_PORT, SMC_BLS1_PIN, &stcGpioInit);

    /* SMC_OE */
    GPIO_Init(SMC_OE_PORT, SMC_OE_PIN, &stcGpioInit);

    /* DMC_DATA[0:15] */
    GPIO_Init(SMC_DATA0_PORT, SMC_DATA0_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA1_PORT, SMC_DATA1_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA2_PORT, SMC_DATA2_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA3_PORT, SMC_DATA3_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA4_PORT, SMC_DATA4_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA5_PORT, SMC_DATA5_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA6_PORT, SMC_DATA6_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA7_PORT, SMC_DATA7_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA8_PORT, SMC_DATA8_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA9_PORT, SMC_DATA9_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA10_PORT, SMC_DATA10_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA11_PORT, SMC_DATA11_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA12_PORT, SMC_DATA12_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA13_PORT, SMC_DATA13_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA14_PORT, SMC_DATA14_PIN, &stcGpioInit);
    GPIO_Init(SMC_DATA15_PORT, SMC_DATA15_PIN, &stcGpioInit);

    /* DMC_ADD[0:18]*/
    GPIO_Init(SMC_ADD0_PORT, SMC_ADD0_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD1_PORT, SMC_ADD1_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD2_PORT, SMC_ADD2_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD3_PORT, SMC_ADD3_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD4_PORT, SMC_ADD4_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD5_PORT, SMC_ADD5_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD6_PORT, SMC_ADD6_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD7_PORT, SMC_ADD7_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD8_PORT, SMC_ADD8_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD9_PORT, SMC_ADD9_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD10_PORT, SMC_ADD10_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD11_PORT, SMC_ADD11_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD12_PORT, SMC_ADD12_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD13_PORT, SMC_ADD13_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD14_PORT, SMC_ADD14_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD15_PORT, SMC_ADD15_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD16_PORT, SMC_ADD16_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD17_PORT, SMC_ADD17_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD18_PORT, SMC_ADD18_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD19_PORT, SMC_ADD19_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD20_PORT, SMC_ADD20_PIN, &stcGpioInit);

    /************************** Set EXMC pin function *************************/
    /* SMC_CS */
    GPIO_SetFunc(SMC_CS_PORT, SMC_CS_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* SMC_WE */
    GPIO_SetFunc(SMC_WE_PORT, SMC_WE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* SMC_BLS[0:1] */
    GPIO_SetFunc(SMC_BLS0_PORT, SMC_BLS0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_BLS1_PORT, SMC_BLS1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* SMC_OE */
    GPIO_SetFunc(SMC_OE_PORT, SMC_OE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* SMC_DATA[0:15] */
    GPIO_SetFunc(SMC_DATA0_PORT, SMC_DATA0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA1_PORT, SMC_DATA1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA2_PORT, SMC_DATA2_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA3_PORT, SMC_DATA3_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA4_PORT, SMC_DATA4_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA5_PORT, SMC_DATA5_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA6_PORT, SMC_DATA6_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA7_PORT, SMC_DATA7_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA8_PORT, SMC_DATA8_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA9_PORT, SMC_DATA9_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA10_PORT, SMC_DATA10_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA11_PORT, SMC_DATA11_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA12_PORT, SMC_DATA12_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA13_PORT, SMC_DATA13_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA14_PORT, SMC_DATA14_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_DATA15_PORT, SMC_DATA15_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* SMC_ADD[0:18]*/
    GPIO_SetFunc(SMC_ADD0_PORT, SMC_ADD0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD1_PORT, SMC_ADD1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD2_PORT, SMC_ADD2_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD3_PORT, SMC_ADD3_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD4_PORT, SMC_ADD4_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD5_PORT, SMC_ADD5_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD6_PORT, SMC_ADD6_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD7_PORT, SMC_ADD7_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD8_PORT, SMC_ADD8_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD9_PORT, SMC_ADD9_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD10_PORT, SMC_ADD10_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD11_PORT, SMC_ADD11_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD12_PORT, SMC_ADD12_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD13_PORT, SMC_ADD13_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD14_PORT, SMC_ADD14_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD15_PORT, SMC_ADD15_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD16_PORT, SMC_ADD16_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD17_PORT, SMC_ADD17_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD18_PORT, SMC_ADD18_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD19_PORT, SMC_ADD19_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD20_PORT, SMC_ADD20_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
}

/**
 * @}
 */

#endif /* BSP_MS_HC32F4A0_LQFP176_050_MEM/BSP_CY62167EV30LL_ENABLE */

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
