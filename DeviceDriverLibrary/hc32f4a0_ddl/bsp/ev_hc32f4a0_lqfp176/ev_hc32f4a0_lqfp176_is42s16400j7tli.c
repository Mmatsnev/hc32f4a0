/**
 *******************************************************************************
 * @file  ev_hc32f4a0_lqfp176_is42s16400j7tli.c
 * @brief This file provides configure functions for is42s16400j7tli of the 
 *        board EV_F4A0_LQ176_V10.
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
#include "ev_hc32f4a0_lqfp176_is42s16400j7tli.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A0_LQFP176
 * @{
 */

/** @defgroup EV_HC32F4A0_LQFP176_IS42S16400J7TLI HC32F4A0 EVB LQFP176 IS42S16400J7TLI
 * @{
 */

#if ((BSP_ON == BSP_IS42S16400J7TLI_ENABLE) && \
     (BSP_EV_HC32F4A0_LQFP176 == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_IS42S16400J7TLI_Local_Macros HC32F4A0 EVB LQFP176 IS42S16400J7TLI Local Macros
 * @{
 */

/**
 * @defgroup IS42S16400J7TLI_Map_DMC_Chip IS42S16400J7TLI Map DMC Chip
 * @{
 */
#define IS42S16400J7TLI_MAP_DMC_CHIP            (EXMC_DMC_CHIP_1)
/**
 * @}
 */

/**
 * @defgroup SDRAM_Address_Space SDRAM Address Space
 * @note SRAM address:[0x80000000, 0x807FFFFF] / size: 8M bytes
 * @{
 */
#define SDRAM_IS42S16400J7TLI_START_ADDR        (EXMC_DMC_ChipStartAddress(IS42S16400J7TLI_MAP_DMC_CHIP))
#define SDRAM_IS42S16400J7TLI_SIZE              (8UL * 1024UL * 1024UL)     /* 8MBytes*/
#define SDRAM_IS42S16400J7TLI_END_ADDR          (SDRAM_IS42S16400J7TLI_START_ADDR + SDRAM_IS42S16400J7TLI_SIZE - 1UL)
/**
 * @}
 */

/**
 * @defgroup DMC_Interface_Pin DMC Interface Pin
 * @{
 */
#define DMC_CKE_PORT                            (GPIO_PORT_C)   /* PC03 - EXMC_ALE */
#define DMC_CKE_PIN                             (GPIO_PIN_03)

#define DMC_CLK_PORT                            (GPIO_PORT_G)   /* PD03 - EXMC_CLK */
#define DMC_CLK_PIN                             (GPIO_PIN_08)

#define DMC_DQM0_PORT                           (GPIO_PORT_E)   /* PE00 - EXMC_CE4 */
#define DMC_DQM0_PIN                            (GPIO_PIN_00)
#define DMC_DQM1_PORT                           (GPIO_PORT_E)   /* PE01 - EXMC_CE5 */
#define DMC_DQM1_PIN                            (GPIO_PIN_01)

#define DMC_BA0_PORT                            (GPIO_PORT_D)   /* PD11 - EXMC_ADD16 */
#define DMC_BA0_PIN                             (GPIO_PIN_11)
#define DMC_BA1_PORT                            (GPIO_PORT_D)   /* PD12 - EXMC_ADD17 */
#define DMC_BA1_PIN                             (GPIO_PIN_12)

#define DMC_CS1_PORT                            (GPIO_PORT_G)   /* PG09 - EXMC_CE1 */
#define DMC_CS1_PIN                             (GPIO_PIN_09)

#define DMC_RAS_PORT                            (GPIO_PORT_F)   /* PF11 - EXMC_OE */
#define DMC_RAS_PIN                             (GPIO_PIN_11)

#define DMC_CAS_PORT                            (GPIO_PORT_G)   /* PG15 - EXMC_BAA */
#define DMC_CAS_PIN                             (GPIO_PIN_15)

#define DMC_WE_PORT                             (GPIO_PORT_C)   /* PC00 - EXMC_WE */
#define DMC_WE_PIN                              (GPIO_PIN_00)

#define DMC_ADD0_PORT                           (GPIO_PORT_F)   /* PF00 - EXMC_ADD0 */
#define DMC_ADD0_PIN                            (GPIO_PIN_00)
#define DMC_ADD1_PORT                           (GPIO_PORT_F)   /* PF01 - EXMC_ADD1 */
#define DMC_ADD1_PIN                            (GPIO_PIN_01)
#define DMC_ADD2_PORT                           (GPIO_PORT_F)   /* PF02 - EXMC_ADD2 */
#define DMC_ADD2_PIN                            (GPIO_PIN_02)
#define DMC_ADD3_PORT                           (GPIO_PORT_F)   /* PF03 - EXMC_ADD3 */
#define DMC_ADD3_PIN                            (GPIO_PIN_03)
#define DMC_ADD4_PORT                           (GPIO_PORT_F)   /* PF04 - EXMC_ADD4 */
#define DMC_ADD4_PIN                            (GPIO_PIN_04)
#define DMC_ADD5_PORT                           (GPIO_PORT_F)   /* PF05 - EXMC_ADD5 */
#define DMC_ADD5_PIN                            (GPIO_PIN_05)
#define DMC_ADD6_PORT                           (GPIO_PORT_F)   /* PF12 - EXMC_ADD6 */
#define DMC_ADD6_PIN                            (GPIO_PIN_12)
#define DMC_ADD7_PORT                           (GPIO_PORT_F)   /* PF13 - EXMC_ADD7 */
#define DMC_ADD7_PIN                            (GPIO_PIN_13)
#define DMC_ADD8_PORT                           (GPIO_PORT_F)   /* PF14 - EXMC_ADD8 */
#define DMC_ADD8_PIN                            (GPIO_PIN_14)
#define DMC_ADD9_PORT                           (GPIO_PORT_F)   /* PF15 - EXMC_ADD9 */
#define DMC_ADD9_PIN                            (GPIO_PIN_15)
#define DMC_ADD10_PORT                          (GPIO_PORT_G)   /* PG00 - EXMC_ADD10 */
#define DMC_ADD10_PIN                           (GPIO_PIN_00)
#define DMC_ADD11_PORT                          (GPIO_PORT_G)   /* PG01 - EXMC_ADD11 */
#define DMC_ADD11_PIN                           (GPIO_PIN_01)

#define DMC_DATA0_PORT                          (GPIO_PORT_D)   /* PD14 - EXMC_DATA0 */
#define DMC_DATA0_PIN                           (GPIO_PIN_14)
#define DMC_DATA1_PORT                          (GPIO_PORT_D)   /* PD15 - EXMC_DATA1 */
#define DMC_DATA1_PIN                           (GPIO_PIN_15)
#define DMC_DATA2_PORT                          (GPIO_PORT_D)   /* PD00 - EXMC_DATA2 */
#define DMC_DATA2_PIN                           (GPIO_PIN_00)
#define DMC_DATA3_PORT                          (GPIO_PORT_D)   /* PD01 - EXMC_DATA3 */
#define DMC_DATA3_PIN                           (GPIO_PIN_01)
#define DMC_DATA4_PORT                          (GPIO_PORT_E)   /* PE07 - EXMC_DATA4 */
#define DMC_DATA4_PIN                           (GPIO_PIN_07)
#define DMC_DATA5_PORT                          (GPIO_PORT_E)   /* PE08 - EXMC_DATA5 */
#define DMC_DATA5_PIN                           (GPIO_PIN_08)
#define DMC_DATA6_PORT                          (GPIO_PORT_E)   /* PE09 - EXMC_DATA6 */
#define DMC_DATA6_PIN                           (GPIO_PIN_09)
#define DMC_DATA7_PORT                          (GPIO_PORT_E)   /* PE10 - EXMC_DATA7 */
#define DMC_DATA7_PIN                           (GPIO_PIN_10)
#define DMC_DATA8_PORT                          (GPIO_PORT_E)   /* PE11 - EXMC_DATA8 */
#define DMC_DATA8_PIN                           (GPIO_PIN_11)
#define DMC_DATA9_PORT                          (GPIO_PORT_E)   /* PE12 - EXMC_DATA9 */
#define DMC_DATA9_PIN                           (GPIO_PIN_12)
#define DMC_DATA10_PORT                         (GPIO_PORT_E)   /* PE13 - EXMC_DATA10 */
#define DMC_DATA10_PIN                          (GPIO_PIN_13)
#define DMC_DATA11_PORT                         (GPIO_PORT_E)   /* PE14 - EXMC_DATA11 */
#define DMC_DATA11_PIN                          (GPIO_PIN_14)
#define DMC_DATA12_PORT                         (GPIO_PORT_E)   /* PE15 - EXMC_DATA12 */
#define DMC_DATA12_PIN                          (GPIO_PIN_15)
#define DMC_DATA13_PORT                         (GPIO_PORT_D)   /* PD08 - EXMC_DATA13 */
#define DMC_DATA13_PIN                          (GPIO_PIN_08)
#define DMC_DATA14_PORT                         (GPIO_PORT_D)   /* PD09 - EXMC_DATA14 */
#define DMC_DATA14_PIN                          (GPIO_PIN_09)
#define DMC_DATA15_PORT                         (GPIO_PORT_D)   /* PD10 - EXMC_DATA15 */
#define DMC_DATA15_PIN                          (GPIO_PIN_10)
/**
 * @}
 */

/**
 * @defgroup IS42S16400J7TLI_Mode_Register_Field IS42S16400J7TLI Mode Register Field
 * @{
 */

/* IS42S16400J7TLI burst length definition */
#define IS42S16400J7TLI_MR_BURST_LEN_1                  (0UL)
#define IS42S16400J7TLI_MR_BURST_LEN_2                  (1UL)
#define IS42S16400J7TLI_MR_BURST_LEN_4                  (2UL)
#define IS42S16400J7TLI_MR_BURST_LEN_8                  (3UL)
#define IS42S16400J7TLI_MR_BURST_LEN_FULLPAGE           (7UL)

/* IS42S16400J7TLI burst type definition */
#define IS42S16400J7TLI_MR_BURST_TYPE_SEQUENTIAL        (0UL)
#define IS42S16400J7TLI_MR_BURST_TYPE_INTERLEAVED       (1UL << 3)

/* IS42S16400J7TLI CAS latency definition */
#define IS42S16400J7TLI_MR_CAS_LATENCY_2                (2UL << 4)
#define IS42S16400J7TLI_MR_CAS_LATENCY_3                (3UL << 4)

/* IS42S16400J7TLI write burst mode definition */
#define IS42S16400J7TLI_MR_WRITEBURST_MODE_PROGRAMMED   (0UL)
#define IS42S16400J7TLI_MR_WRITEBURST_MODE_SINGLE       (1UL << 9)

/**
 * @}
 */

/**
 * @defgroup IS42S16400J7TLI_Mode_Register_Value IS42S16400J7TLI Mode Register Value
 * @{
 */
#define IS42S16400J7TLI_MR_VALUE                                               \
(   IS42S16400J7TLI_MR_BURST_TYPE_SEQUENTIAL    |                              \
    IS42S16400J7TLI_MR_WRITEBURST_MODE_PROGRAMMED)
/**
 * @}
 */

/**
 * @defgroup IS42S16400J7TLI_Map_DMC_Address_Space IS42S16400J7TLI Map DMC Address Space
 * @{
 */
#define IS42S16400J7TLI_MAP_DMC_ADDR_MATCH      (EXMC_DMC_ADDR_MATCH_0X80000000)
#define IS42S16400J7TLI_MAP_DMC_ADDR_MASK       (EXMC_DMC_ADDR_MASK_16MB)
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
 * @addtogroup EV_HC32F4A0_LQFP176_IS42S16400J7TLI_Local_Functions
 * @{
 */

static void EV_EXMC_DMC_PortInit(void);
static void EV_EXMC_DMC_InitSequence(uint32_t u32Chip,
                                     uint32_t u32Bank,
                                     uint32_t u32MdRegVal);

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A0_LQFP176_IS42S16400J7TLI_Global_Functions EVB HC32F4A0 LQFP176 IS42S16400J7TLI Global Functions
 * @{
 */

/**
 * @brief  Initialize DMC for IS42S16400J7TLI.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 */
en_result_t BSP_DMC_IS42S16400J7TLI_Init(void)
{
    uint32_t u32MdRegVal;
    stc_exmc_dmc_init_t stcDmcInit;
    stc_exmc_dmc_cs_cfg_t stcCsCfg;

    /* Initialization DMC port.*/
    EV_EXMC_DMC_PortInit();

    /* Enable DMC module clk */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_DMC, Enable);

    /* Enable DMC. */
    EXMC_DMC_Cmd(Enable);

    /* Configure DMC width && refresh period & chip & timing. */
    EXMC_DMC_StructInit(&stcDmcInit);
    stcDmcInit.u32RefreshPeriod = 900UL;
    stcDmcInit.stcChipCfg.u32ColumnBitsNumber = EXMC_DMC_COLUMN_BITS_NUM_8;
    stcDmcInit.stcChipCfg.u32RowBitsNumber = EXMC_DMC_ROW_BITS_NUM_12;
    stcDmcInit.stcChipCfg.u32MemBurst = EXMC_DMC_MEM_BURST_1;
    stcDmcInit.stcChipCfg.u32AutoRefreshChips = EXMC_DMC_AUTO_REFRESH_CHIPS_01;

    stcDmcInit.stcTimingCfg.u32CASL = 2UL;
    stcDmcInit.stcTimingCfg.u32DQSS = 0UL;
    stcDmcInit.stcTimingCfg.u32MRD = 2UL;
    stcDmcInit.stcTimingCfg.u32RAS = 3UL;
    stcDmcInit.stcTimingCfg.u32RC = 4UL;
    stcDmcInit.stcTimingCfg.u32RCD = 1UL;
    stcDmcInit.stcTimingCfg.u32RFC = 4UL;
    stcDmcInit.stcTimingCfg.u32RP = 1UL;
    stcDmcInit.stcTimingCfg.u32RRD = 1UL;
    stcDmcInit.stcTimingCfg.u32WR = 2UL;
    stcDmcInit.stcTimingCfg.u32WTR = 1UL;
    stcDmcInit.stcTimingCfg.u32XP = 1UL;
    stcDmcInit.stcTimingCfg.u32XSR = 5UL;
    stcDmcInit.stcTimingCfg.u32ESR = 5UL;
    EXMC_DMC_Init(&stcDmcInit);

    /* Configure DMC address space. */
    stcCsCfg.u32AddrMask = IS42S16400J7TLI_MAP_DMC_ADDR_MASK;
    stcCsCfg.u32AddrMatch = IS42S16400J7TLI_MAP_DMC_ADDR_MATCH;
    stcCsCfg.u32AddrDecodeMode = EXMC_DMC_CS_DECODE_ROWBANKCOL;
    EXMC_DMC_CsConfig(IS42S16400J7TLI_MAP_DMC_CHIP, &stcCsCfg);

    /* SDRAM initialization sequence. */
    u32MdRegVal = IS42S16400J7TLI_MR_VALUE;

    if (2UL == stcDmcInit.stcTimingCfg.u32CASL)
    {
        u32MdRegVal |= IS42S16400J7TLI_MR_CAS_LATENCY_2;
    }
    else
    {
        u32MdRegVal |= IS42S16400J7TLI_MR_CAS_LATENCY_3;
    }

    if (EXMC_DMC_MEM_BURST_1 == stcDmcInit.stcChipCfg.u32MemBurst)
    {
        u32MdRegVal |= IS42S16400J7TLI_MR_BURST_LEN_1;
    }
    else if (EXMC_DMC_MEM_BURST_2 == stcDmcInit.stcChipCfg.u32MemBurst)
    {
        u32MdRegVal |= IS42S16400J7TLI_MR_BURST_LEN_2;
    }
    else if (EXMC_DMC_MEM_BURST_4 == stcDmcInit.stcChipCfg.u32MemBurst)
    {
        u32MdRegVal |= IS42S16400J7TLI_MR_BURST_LEN_4;
    }
    else
    {
        u32MdRegVal |= IS42S16400J7TLI_MR_BURST_LEN_8;
    }

    EV_EXMC_DMC_InitSequence(IS42S16400J7TLI_MAP_DMC_CHIP, EXMC_DMC_BANK_0, u32MdRegVal);

    /* Switch state from configure to ready */
    EXMC_DMC_SetState(EXMC_DMC_CTL_STATE_GO);
    while (EXMC_DMC_CURR_STATUS_LOWPOWER != EXMC_DMC_GetStatus())
    {}

    EXMC_DMC_SetState(EXMC_DMC_CTL_STATE_WAKEUP);
    while (EXMC_DMC_CURR_STATUS_PAUSED != EXMC_DMC_GetStatus())
    {}

    EXMC_DMC_SetState(EXMC_DMC_CTL_STATE_GO);
    while (EXMC_DMC_CURR_STATUS_READY != EXMC_DMC_GetStatus())
    {}

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
void BSP_DMC_IS42S16400J7TLI_GetMemInfo(uint32_t *pu32MemStartAddr,
                                                uint32_t *pu32MemByteSize)
{
    if (NULL != pu32MemStartAddr)
    {
        *pu32MemStartAddr = SDRAM_IS42S16400J7TLI_START_ADDR;
    }

    if (NULL != pu32MemByteSize)
    {
        *pu32MemByteSize = SDRAM_IS42S16400J7TLI_SIZE;
    }
}

/**
 * @}
 */

/**
 * @defgroup EV_HC32F4A0_LQFP176_IS42S16400J7TLI_Local_Functions HC32F4A0 LQFP176 EVB IS42S16400J7TLI Local Functions
 * @{
 */

/**
 * @brief  Initialize DMC port.
 * @param  None
 * @retval None
 */
static void EV_EXMC_DMC_PortInit(void)
{
    stc_gpio_init_t stcGpioInit;

    /************************* Set pin drive capacity *************************/
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_DRV_HIGH;

    /* DMC_CKE */
    GPIO_Init(DMC_CKE_PORT, DMC_CKE_PIN, &stcGpioInit);

    /* DMC_CLK */
    GPIO_Init(DMC_CLK_PORT, DMC_CLK_PIN, &stcGpioInit);

    /* DMC_LDQM && DMC_UDQM */
    GPIO_Init(DMC_DQM0_PORT, DMC_DQM0_PIN, &stcGpioInit);
    GPIO_Init(DMC_DQM1_PORT, DMC_DQM1_PIN, &stcGpioInit);

    /* DMC_BA[0:1] */
    GPIO_Init(DMC_BA0_PORT, DMC_BA0_PIN, &stcGpioInit);
    GPIO_Init(DMC_BA1_PORT, DMC_BA1_PIN, &stcGpioInit);

    /* DMC_CAS && DMC_RAS */
    GPIO_Init(DMC_CAS_PORT, DMC_CAS_PIN, &stcGpioInit);
    GPIO_Init(DMC_RAS_PORT, DMC_RAS_PIN, &stcGpioInit);

    /* DMC_WE */
    GPIO_Init(DMC_WE_PORT, DMC_WE_PIN, &stcGpioInit);

    /* DMC_DATA[0:15] */
    GPIO_Init(DMC_DATA0_PORT, DMC_DATA0_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA1_PORT, DMC_DATA1_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA2_PORT, DMC_DATA2_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA3_PORT, DMC_DATA3_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA4_PORT, DMC_DATA4_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA5_PORT, DMC_DATA5_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA6_PORT, DMC_DATA6_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA7_PORT, DMC_DATA7_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA8_PORT, DMC_DATA8_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA9_PORT, DMC_DATA9_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA10_PORT, DMC_DATA10_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA11_PORT, DMC_DATA11_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA12_PORT, DMC_DATA12_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA13_PORT, DMC_DATA13_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA14_PORT, DMC_DATA14_PIN, &stcGpioInit);
    GPIO_Init(DMC_DATA15_PORT, DMC_DATA15_PIN, &stcGpioInit);

    /* DMC_ADD[0:11]*/
    GPIO_Init(DMC_ADD0_PORT, DMC_ADD0_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD1_PORT, DMC_ADD1_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD2_PORT, DMC_ADD2_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD3_PORT, DMC_ADD3_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD4_PORT, DMC_ADD4_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD5_PORT, DMC_ADD5_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD6_PORT, DMC_ADD6_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD7_PORT, DMC_ADD7_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD8_PORT, DMC_ADD8_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD9_PORT, DMC_ADD9_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD10_PORT, DMC_ADD10_PIN, &stcGpioInit);
    GPIO_Init(DMC_ADD11_PORT, DMC_ADD11_PIN, &stcGpioInit);

    /************************** Set EXMC pin function *************************/
    /* DMC_CKE */
    GPIO_SetFunc(DMC_CKE_PORT, DMC_CKE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_CLK */
    GPIO_SetFunc(DMC_CLK_PORT, DMC_CLK_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_LDQM && DMC_UDQM */
    GPIO_SetFunc(DMC_DQM0_PORT, DMC_DQM0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DQM1_PORT, DMC_DQM1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_BA[0:1] */
    GPIO_SetFunc(DMC_BA0_PORT, DMC_BA0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_BA1_PORT, DMC_BA1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_CS */
    GPIO_SetFunc(DMC_CS1_PORT, DMC_CS1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_CAS && DMC_RAS */
    GPIO_SetFunc(DMC_CAS_PORT, DMC_CAS_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_RAS_PORT, DMC_RAS_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_WE */
    GPIO_SetFunc(DMC_WE_PORT, DMC_WE_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_DATA[0:15] */
    GPIO_SetFunc(DMC_DATA0_PORT, DMC_DATA0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA1_PORT, DMC_DATA1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA2_PORT, DMC_DATA2_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA3_PORT, DMC_DATA3_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA4_PORT, DMC_DATA4_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA5_PORT, DMC_DATA5_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA6_PORT, DMC_DATA6_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA7_PORT, DMC_DATA7_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA8_PORT, DMC_DATA8_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA9_PORT, DMC_DATA9_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA10_PORT, DMC_DATA10_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA11_PORT, DMC_DATA11_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA12_PORT, DMC_DATA12_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA13_PORT, DMC_DATA13_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA14_PORT, DMC_DATA14_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_DATA15_PORT, DMC_DATA15_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* DMC_ADD[0:11]*/
    GPIO_SetFunc(DMC_ADD0_PORT, DMC_ADD0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD1_PORT, DMC_ADD1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD2_PORT, DMC_ADD2_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD3_PORT, DMC_ADD3_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD4_PORT, DMC_ADD4_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD5_PORT, DMC_ADD5_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD6_PORT, DMC_ADD6_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD7_PORT, DMC_ADD7_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD8_PORT, DMC_ADD8_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD9_PORT, DMC_ADD9_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD10_PORT, DMC_ADD10_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(DMC_ADD11_PORT, DMC_ADD11_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
}

/**
 * @brief  SDRAM IS42S16400J7TLI initialization sequence.
 * @param  [in] u32Chip                     The command chip number.
 *         This parameter can be one of the following values:
 *           @arg EXMC_DMC_CHIP_0:          Chip 0
 *           @arg EXMC_DMC_CHIP_1:          Chip 1
 *           @arg EXMC_DMC_CHIP_2:          Chip 2
 *           @arg EXMC_DMC_CHIP_3:          Chip 3
 * @param  [in] u32Bank                     The command bank.
 *         This parameter can be one of the following values:
 *           @arg EXMC_DMC_BANK_0:          Bank 0
 *           @arg EXMC_DMC_BANK_1:          Bank 1
 *           @arg EXMC_DMC_BANK_2:          Bank 2
 *           @arg EXMC_DMC_BANK_3:          Bank 3
 * @param  [in] u32MdRegVal                 The SDRAM mode register value
 * @retval None
 */
static void EV_EXMC_DMC_InitSequence(uint32_t u32Chip,
                                     uint32_t u32Bank,
                                     uint32_t u32MdRegVal)
{
    /* SDRAM initialization sequence:
       CMD NOP->PrechargeAll->AutoRefresh->AutoRefresh->MdRegConfig->NOP */
    EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_NOP, 0UL);
    EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_PRECHARGEALL, 0UL);
    EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_AUTOREFRESH, 0UL);
    EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_AUTOREFRESH, 0UL);
    EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_MDREGCONFIG, u32MdRegVal);
    EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_NOP, 0UL);
}

/**
 * @}
 */

#endif /* BSP_EV_HC32F4A0_LQFP176/BSP_IS42S16400J7TLI_ENABLE */

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
