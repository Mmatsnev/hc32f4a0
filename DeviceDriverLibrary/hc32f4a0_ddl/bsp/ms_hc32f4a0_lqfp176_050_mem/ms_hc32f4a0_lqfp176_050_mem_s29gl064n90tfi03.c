/**
 *******************************************************************************
 * @file  ms_hc32f4a0_lqfp176_050_mem_s29gl064n90tfi03.c
 * @brief This file provides configure functions for s29gl064n90tfi03 of the 
 *        board MS_HC32F4A0_LQF176_050_MEM.
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
#include "ms_hc32f4a0_lqfp176_050_mem_s29gl064n90tfi03.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup MS_HC32F4A0_LQFP176_050_MEM
 * @{
 */

/** @defgroup MS_HC32F4A0_LQFP176_050_MEM_S29GL064N90TFI03 HC32F4A0 MS LQFP176 S29GL064N90TFI03
 * @{
 */

#if ((BSP_ON == BSP_S29GL064N90TFI03_ENABLE) && \
     (BSP_MS_HC32F4A0_LQFP176_050_MEM == BSP_EV_HC32F4A0))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup MS_HC32F4A0_LQFP176_050_MEM_S29GL064N90TFI03_Local_Macros HC32F4A0 MS LQFP176 S29GL064N90TFI03 Local Macros
 * @{
 */

/**
 * @defgroup S29GL064N90TFI03_Map_SMC_Chip S29GL064N90TFI03 Map SMC Chip
 * @{
 */
#define S29GL064N90TFI03_MAP_SMC_CHIP           (EXMC_SMC_CHIP_0)
/**
 * @}
 */

/**
 * @defgroup S29GL064N90TFI03_SMC_Address_Space S29GL064N90TFI03 DMC Address Space
 * @{
 */
#define S29GL064N90TFI03_SMC_MATCH_ADDR         (0x60UL)
#define S29GL064N90TFI03_SMC_MASK_ADDR          (0xFFUL)
/**
 * @}
 */

/**
 * @defgroup SRAM_Address_Space SRAM Address Space
 * @note SRAM address:[0x60000000, 0x607FFFFF] / size: 8M bytes
 * @{
 */
#define S29GL064N90TFI03_START_ADDR             (EXMC_SMC_ChipStartAddress(S29GL064N90TFI03_MAP_SMC_CHIP))
#define S29GL064N90TFI03_SIZE                   (8UL * 1024UL * 1024UL)     /* 8MBytes*/
#define S29GL064N90TFI03_END_ADDR               (S29GL064N90TFI03_START_ADDR + S29GL064N90TFI03_SIZE - 1UL)
/**
 * @}
 */

#define S29GL064N90TFI03_MEM_WIDTH              (EXMC_SMC_MEMORY_WIDTH_16BIT)

/**
 * @defgroup SMC_Interface_Pin EXMC SMC Interface Pin
 * @{
 */
#define SMC_IO3_PORT                            (GPIO_PORT_G)   /* PG14 - IO3(Device pin: nBYTE) */
#define SMC_IO3_PIN                             (GPIO_PIN_14)

#define SMC_CS0_PORT                            (GPIO_PORT_D)   /* PD07 - EXMC_CE0 */
#define SMC_CS0_PIN                             (GPIO_PIN_07)

#define SMC_CS1_PORT                            (GPIO_PORT_G)   /* PG09 - EXMC_CE1 */
#define SMC_CS1_PIN                             (GPIO_PIN_09)

#define SMC_RST0_PORT                           (GPIO_PORT_B)   /* PB06 - EXMC_RST0 */
#define SMC_RST0_PIN                            (GPIO_PIN_06)

#define SMC_RST1_PORT                           (GPIO_PORT_B)   /* PB07 - EXMC_RST1 */
#define SMC_RST1_PIN                            (GPIO_PIN_07)

#define SMC_RB_PORT                             (GPIO_PORT_G)   /* PG06 - EXMC_RB0 */
#define SMC_RB_PIN                              (GPIO_PIN_06)

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
#define SMC_ADD21_PORT                          (GPIO_PORT_E)   /* PE05 - EXMC_ADD21 */
#define SMC_ADD21_PIN                           (GPIO_PIN_05)
#define SMC_ADD22_PORT                          (GPIO_PORT_E)   /* PE06 - EXMC_ADD22 */
#define SMC_ADD22_PIN                           (GPIO_PIN_06)


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

/* Constants to define address to set to write a command */
#define NOR_CMD_ADDRESS_FIRST                   (0x0555U)
#define NOR_CMD_ADDRESS_FIRST_CFI               (0x0055U)
#define NOR_CMD_ADDRESS_SECOND                  (0x02AAU)
#define NOR_CMD_ADDRESS_THIRD                   (0x0555U)
#define NOR_CMD_ADDRESS_FOURTH                  (0x0555U)
#define NOR_CMD_ADDRESS_FIFTH                   (0x02AAU)
#define NOR_CMD_ADDRESS_SIXTH                   (0x0555U)

/* Constants to define data to program a command */
#define NOR_CMD_DATA_READ_RESET                 (0x00F0U)
#define NOR_CMD_DATA_FIRST                      (0x00AAU)
#define NOR_CMD_DATA_SECOND                     (0x0055U)
#define NOR_CMD_DATA_AUTO_SELECT                (0x0090U)
#define NOR_CMD_DATA_PROGRAM                    (0x00A0U)
#define NOR_CMD_DATA_CHIP_BLOCK_ERASE_THIRD     (0x0080U)
#define NOR_CMD_DATA_CHIP_BLOCK_ERASE_FOURTH    (0x00AAU)
#define NOR_CMD_DATA_CHIP_BLOCK_ERASE_FIFTH     (0x0055U)
#define NOR_CMD_DATA_CHIP_ERASE                 (0x0010U)
#define NOR_CMD_DATA_CFI                        (0x0098U)

#define NOR_CMD_DATA_WRITE_TO_BUFFER_PROGRAM            (0x25U)
#define NOR_CMD_DATA_WRITE_TO_BUFFER_PROGRAM_CONFIRM    (0x29U)

#define NOR_CMD_DATA_BLOCK_ERASE                (0x30U)

/* Mask on NOR STATUS REGISTER */
#define NOR_MASK_STATUS_DQ5                     (0x0020U)
#define NOR_MASK_STATUS_DQ6                     (0x0040U)

/* NOR device IDs addresses */
#define DEVICE_MANUFACTURER_ID_ADDRESS          (0x0000U)
#define DEVICE_ID1_ADDRESS                      (0x0001U)
#define DEVICE_ID2_ADDRESS                      (0x000EU)
#define DEVICE_ID3_ADDRESS                      (0x000FU)

/* NOR CFI IDs addresses */
#define DEVICE_CFI1_ADDRESS                     (0x0061U)
#define DEVICE_CFI2_ADDRESS                     (0x0062U)
#define DEVICE_CFI3_ADDRESS                     (0x0063U)
#define DEVICE_CFI4_ADDRESS                     (0x0064U)

#define NOR_MAX_DELAY                           (0xFFFFFFFFUL)

#define S29GL064N90TFI03_DEVICE_SECTORS             (128UL)
#define S29GL064N90TFI03_BYTES_PER_SECTOR           (64UL * 1024UL)
#define S29GL064N90TFI03_BYTES_PER_BUFFER_PROGRAM   (32UL)
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
 * @addtogroup MS_HC32F4A0_LQFP176_050_MEM_S29GL064N90TFI03_Local_Functions
 * @{
 */

static void EV_EXMC_SMC_PortInit(void);
static en_result_t BSP_SMC_S29GL064_WaitReadySignal(uint32_t u32Timeout);

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup MS_HC32F4A0_LQFP176_050_MEM_S29GL064N90TFI03_Global_Functions HC32F4A0 MS LQFP176 S29GL064N90TFI03 Global Functions
 * @{
 */

/**
 * @brief  Initialize SMC for S29GL064N90TFI03.
 * @param  None
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 */
en_result_t BSP_SMC_S29GL064_Init(void)
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
    stcSmcInit.stcChipCfg.u32ReadMode = EXMC_SMC_MEM_READ_SYNC;
    stcSmcInit.stcChipCfg.u32ReadBurstLen = EXMC_SMC_MEM_READ_BURST_4;
    stcSmcInit.stcChipCfg.u32WriteMode = EXMC_SMC_MEM_WRITE_SYNC;
    stcSmcInit.stcChipCfg.u32WriteBurstLen = EXMC_SMC_MEM_WRITE_BURST_4;
    stcSmcInit.stcChipCfg.u32SmcMemWidth = S29GL064N90TFI03_MEM_WIDTH;
    stcSmcInit.stcChipCfg.u32BAA = EXMC_SMC_BAA_PORT_DISABLE;
    stcSmcInit.stcChipCfg.u32ADV = EXMC_SMC_ADV_PORT_DISABLE;
    stcSmcInit.stcChipCfg.u32BLS = EXMC_SMC_BLS_SYNC_CS;
    stcSmcInit.stcChipCfg.u32AddressMatch = S29GL064N90TFI03_SMC_MATCH_ADDR;
    stcSmcInit.stcChipCfg.u32AddressMask = S29GL064N90TFI03_SMC_MASK_ADDR;
    stcSmcInit.stcTimingCfg.u32RC = 6UL;
    stcSmcInit.stcTimingCfg.u32WC = 6UL;
    stcSmcInit.stcTimingCfg.u32CEOE = 2UL;
    stcSmcInit.stcTimingCfg.u32WP = 3UL;
    stcSmcInit.stcTimingCfg.u32PC = 4UL;
    stcSmcInit.stcTimingCfg.u32TR = 1UL;
    EXMC_SMC_Init(S29GL064N90TFI03_MAP_SMC_CHIP, &stcSmcInit);

    /* Set command: updateregs */
    EXMC_SMC_SetCommand(S29GL064N90TFI03_MAP_SMC_CHIP, EXMC_SMC_CMD_UPDATEREGS, 0UL, 0UL);

    /* Check timing status */
    do {
        enRet = EXMC_SMC_CheckTimingStatus(S29GL064N90TFI03_MAP_SMC_CHIP, &stcSmcInit.stcTimingCfg);
    } while (Ok != enRet);

    /* Check chip status */
    do {
        enRet = EXMC_SMC_CheckChipStatus(S29GL064N90TFI03_MAP_SMC_CHIP, &stcSmcInit.stcChipCfg);
    } while (Ok != enRet);

    return Ok;
}

/**
 * @brief  Get memory information.
 * @param  [out] pu32MemStartAddr       The pointer for memory start address
 * @param  [out] pu32SectorsNumber      The pointer for device sector
 * @param  [out] pu32BytesPerSector     The pointer for bytes per sector(unit: Byte)
 * @param  [out] pu32BytesPerBufProgram The pointer for bytes per buffer program(unit: Byte)
 * @retval None
 */
void BSP_SMC_S29GL064_GetMemInfo(uint32_t *pu32MemStartAddr,
                                        uint32_t *pu32SectorsNumber,
                                        uint32_t *pu32BytesPerSector,
                                        uint32_t *pu32BytesPerBufProgram)
{
    if (NULL != pu32MemStartAddr)
    {
        *pu32MemStartAddr = S29GL064N90TFI03_START_ADDR;
    }

    if (NULL != pu32SectorsNumber)
    {
        *pu32SectorsNumber = S29GL064N90TFI03_DEVICE_SECTORS;
    }

    if (NULL != pu32BytesPerSector)
    {
        *pu32BytesPerSector = S29GL064N90TFI03_BYTES_PER_SECTOR;
    }

    if (NULL != pu32BytesPerBufProgram)
    {
        *pu32BytesPerBufProgram = S29GL064N90TFI03_BYTES_PER_BUFFER_PROGRAM;
    }
}

/**
 * @brief  Read NOR flash IDs
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [out] au16Id                 ID buffer
 * @param  [in] u32Length               Number ID to read
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer au16Id value is NULL.
 */
en_result_t BSP_SMC_S29GL064_ReadId(uint32_t u32DevicBaseAddress,
                                        uint16_t au16Id[],
                                        uint32_t u32Length)
{
    uint32_t u32TmpLen;
    uint16_t au16TmpId[4];
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != au16Id) && u32Length)
    {
        /* Send read ID command */
        RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIRST)) = NOR_CMD_DATA_FIRST;
        RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_SECOND)) = NOR_CMD_DATA_SECOND;
        RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_THIRD)) = NOR_CMD_DATA_AUTO_SELECT;

        /* Read the NOR IDs */
        au16TmpId[0] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_MANUFACTURER_ID_ADDRESS));
        au16TmpId[1] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_ID1_ADDRESS));
        au16TmpId[2] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_ID2_ADDRESS));
        au16TmpId[3] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_ID3_ADDRESS));

        u32TmpLen = (u32Length < 4UL) ? u32Length : 4UL;
        memcpy(au16Id, au16TmpId, (u32TmpLen << 1UL));

        RW_MEM16(u32DevicBaseAddress) = NOR_CMD_DATA_READ_RESET;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Read NOR flash CFI IDs
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [out] au16Id                 ID buffer
 * @param  [in] u32Length               Number ID to read
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer au16Id value is NULL.
 */
en_result_t BSP_SMC_S29GL064_ReadCfiId(uint32_t u32DevicBaseAddress,
                                            uint16_t au16Id[],
                                            uint32_t u32Length)
{
    uint32_t u32TmpLen;
    uint16_t au16TmpId[4];
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != au16Id) && u32Length)
    {
        /* Send read CFI query command */
        RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIRST_CFI)) = NOR_CMD_DATA_CFI;

        /* read the NOR CFI information */
        au16TmpId[0] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_CFI1_ADDRESS));
        au16TmpId[1] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_CFI2_ADDRESS));
        au16TmpId[2] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_CFI3_ADDRESS));
        au16TmpId[3] = RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, DEVICE_CFI4_ADDRESS));

        u32TmpLen = (u32Length < 4UL) ? u32Length : 4UL;
        memcpy(au16Id, au16TmpId, (u32TmpLen << 1U));

        RW_MEM16(u32DevicBaseAddress) = NOR_CMD_DATA_READ_RESET;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Returns the NOR operation status
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [in] u32Timeout:             Timeout duration
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  Error:                       Errors occurred
 *   @arg  ErrorTimeout:                Timeout
 */
en_result_t BSP_SMC_S29GL064_GetStatus(uint32_t u32DevicBaseAddress,
                                            uint32_t u32Timeout)
{
    uint16_t u16TmpSr1 = 0U;
    uint16_t u16TmpSr2 = 0U;
    uint32_t u32To = 0U;
    en_result_t enStatus = ErrorOperationInProgress;

    /* Poll on NOR memory Ready/Busy signal */
    BSP_SMC_S29GL064_WaitReadySignal(u32Timeout);

    while (enStatus == ErrorOperationInProgress)
    {
        /* Read NOR status register (DQ6 and DQ5) */
        u16TmpSr1 = RW_MEM16(u32DevicBaseAddress);
        u16TmpSr2 = RW_MEM16(u32DevicBaseAddress);

        /* If DQ6 did not toggle between the two reads then return Ok  */
        if((u16TmpSr1 & NOR_MASK_STATUS_DQ6) == (u16TmpSr2 & NOR_MASK_STATUS_DQ6))
        {
            enStatus = Ok;
        }
        else
        {
            if((u16TmpSr1 & NOR_MASK_STATUS_DQ5) == NOR_MASK_STATUS_DQ5)
            {
                enStatus = ErrorOperationInProgress;
            }

            u16TmpSr1 = RW_MEM16(u32DevicBaseAddress);
            u16TmpSr2 = RW_MEM16(u32DevicBaseAddress);

            /* If DQ6 did not toggle between the two reads then return Ok */
            if((u16TmpSr1 & NOR_MASK_STATUS_DQ6) == (u16TmpSr2 & NOR_MASK_STATUS_DQ6))
            {
                enStatus = Ok;
            }
            else if((u16TmpSr1 & NOR_MASK_STATUS_DQ5) == NOR_MASK_STATUS_DQ5)
            {
                enStatus = Error;
            }
            else
            {
                /* Do nothing */
            }
        }

        /* Check for the Timeout */
        if (u32To++ > u32Timeout)
        {
            enStatus = ErrorTimeout;
        }
    }

    return enStatus;
}

/**
 * @brief  Reset the NOR memory to Read mode
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @retval None
 */
void BSP_SMC_S29GL064_Reset(uint32_t u32DevicBaseAddress)
{
    RW_MEM16(u32DevicBaseAddress) = NOR_CMD_DATA_READ_RESET;
}

/**
  * @brief  Erase the entire NOR chip.
  * @param  [in] u32DevicBaseAddress     S29GL064 base address
  * @retval An en_result_t enumeration value:
  *   @arg  Ok:                          No errors occurred.
  */
en_result_t BSP_SMC_S29GL064_EraseChip(uint32_t u32DevicBaseAddress)
{
    /* Send NOR chip erase command sequence */
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIRST)) = NOR_CMD_DATA_FIRST;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_SECOND)) = NOR_CMD_DATA_SECOND;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_THIRD)) = NOR_CMD_DATA_CHIP_BLOCK_ERASE_THIRD;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FOURTH)) = NOR_CMD_DATA_CHIP_BLOCK_ERASE_FOURTH;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIFTH)) = NOR_CMD_DATA_CHIP_BLOCK_ERASE_FIFTH;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_SIXTH)) = NOR_CMD_DATA_CHIP_ERASE;

    return Ok;
}

/**
 * @brief  Erase the specified block of the NOR memory
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [in] u32SectorAddress        sector address
 * @retval None
 */
void BSP_SMC_S29GL064_EraseSector(uint32_t u32DevicBaseAddress,
                                        uint32_t u32SectorAddress)
{
    /* Send block erase command sequence */
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIRST)) = NOR_CMD_DATA_FIRST;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_SECOND)) = NOR_CMD_DATA_SECOND;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_THIRD)) = NOR_CMD_DATA_CHIP_BLOCK_ERASE_THIRD;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FOURTH)) = NOR_CMD_DATA_CHIP_BLOCK_ERASE_FOURTH;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIFTH)) = NOR_CMD_DATA_CHIP_BLOCK_ERASE_FIFTH;

    RW_MEM16(u32SectorAddress) = NOR_CMD_DATA_BLOCK_ERASE;
}

/**
 * @brief  Write memory for half-word.
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [in] u32ProgramAddress       Memory address to write
 * @param  [in] u16Data                 Data to write
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 */
en_result_t BSP_SMC_S29GL064_Program(uint32_t u32DevicBaseAddress,
                                            uint32_t u32ProgramAddress,
                                            uint16_t u16Data)
{
    /* Send program data command */
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIRST)) = NOR_CMD_DATA_FIRST;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_SECOND)) = NOR_CMD_DATA_SECOND;
    RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_THIRD)) = NOR_CMD_DATA_PROGRAM;

    /* Write the data */
    RW_MEM16(u32ProgramAddress) = u16Data;

    return Ok;
}

/**
 * @brief  Read memory for half-word.
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [in] u32ReadAddress          Memory address to read
 * @retval Data of the specified address
 */
uint16_t BSP_SMC_S29GL064_Read(uint32_t u32DevicBaseAddress,
                                    uint32_t u32ReadAddress)
{
    return RW_MEM16(u32ReadAddress);
}

/**
 * @brief  Write memory(using Program Buffer to Flash command) for half-word.
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [in] u32ProgramAddress       Memory address to write
 * @param  [in] au16Data                Data buffer to write
 * @param  [in] u32NumHalfwords         Number half-word to write
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer au16Data value is NULL.
 */
en_result_t BSP_SMC_S29GL064_ProgramBuffer(uint32_t u32DevicBaseAddress,
                                                  uint32_t u32ProgramAddress,
                                                  const uint16_t au16Data[],
                                                  uint32_t u32NumHalfwords)
{
    uint32_t i = 0UL;
    uint32_t u32CurAddr;
    uint32_t u32EndAddr;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != au16Data)
    {
        DDL_ASSERT(IS_ADDRESS_ALIGN(u32ProgramAddress, 16UL));

        /* Initialize variables */
        u32EndAddr = u32ProgramAddress + (u32NumHalfwords << 1UL) - 1UL;

        /* Issue unlock command sequence */
        RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_FIRST)) = NOR_CMD_DATA_FIRST;
        RW_MEM16(SMC_ADDR_SHIFT(u32DevicBaseAddress, S29GL064N90TFI03_MEM_WIDTH, NOR_CMD_ADDRESS_SECOND)) = NOR_CMD_DATA_SECOND;

        /* Write Buffer Load Command */
        RW_MEM16(u32ProgramAddress) =  NOR_CMD_DATA_WRITE_TO_BUFFER_PROGRAM;
        RW_MEM16(u32ProgramAddress) = (uint16_t)(u32NumHalfwords - 1UL);

        /* Load Data into NOR Buffer */
        for (u32CurAddr = u32ProgramAddress; u32CurAddr <= u32EndAddr; u32CurAddr += 2UL)
        {
            RW_MEM16(u32CurAddr) = au16Data[i++];
        }

        RW_MEM16(u32ProgramAddress) = NOR_CMD_DATA_WRITE_TO_BUFFER_PROGRAM_CONFIRM;
        enRet = Ok;
    }

    return enRet;
}

/**
 * @brief  Read memory for half-word.
 * @param  [in] u32DevicBaseAddress     S29GL064 base address
 * @param  [in] u32ReadAddress          Memory address to read
 * @param  [in] au16Data                Data buffer for reading
 * @param  [in] u32NumHalfWords         Number half-word to write
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                          No errors occurred.
 *   @arg  ErrorInvalidParameter:       The pointer au16Data value is NULL.
 */
en_result_t BSP_SMC_S29GL064_ReadBuffer(uint32_t u32DevicBaseAddress,
                                            uint32_t u32ReadAddress,
                                            uint16_t au16Data[],
                                            uint32_t u32NumHalfWords)
{
    uint32_t i;
    en_result_t enRet = ErrorInvalidParameter;

    if (au16Data != NULL)
    {
        for (i = 0UL; i < u32NumHalfWords; i++)
        {
            au16Data[i] = RW_MEM16(u32ReadAddress);
            u32ReadAddress += 2UL;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 * @}
 */

/**
 * @defgroup MS_HC32F4A0_LQFP176_050_MEM_S29GL064N90TFI03_Local_Functions HC32F4A0 MS LQFP176 S29GL064N90TFI03 Local Functions
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
    GPIO_Init(SMC_IO3_PORT, SMC_IO3_PIN, &stcGpioInit);

    /* SMC_CS */
    GPIO_Init(SMC_CS0_PORT, SMC_CS0_PIN, &stcGpioInit);
    GPIO_Init(SMC_CS1_PORT, SMC_CS1_PIN, &stcGpioInit);

    /* SMC_RST */
    GPIO_Init(SMC_RST0_PORT, SMC_RST0_PIN, &stcGpioInit);
    GPIO_Init(SMC_RST1_PORT, SMC_RST1_PIN, &stcGpioInit);

    /* SMC_RB */
    GPIO_Init(SMC_RB_PORT, SMC_RB_PIN, &stcGpioInit);

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
    GPIO_Init(SMC_ADD21_PORT, SMC_ADD21_PIN, &stcGpioInit);
    GPIO_Init(SMC_ADD22_PORT, SMC_ADD22_PIN, &stcGpioInit);

    /************************** Set EXMC pin function *************************/
    /* SMC_CS */
    GPIO_SetFunc(SMC_CS0_PORT, SMC_CS0_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_CS1_PORT, SMC_CS1_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    /* SMC_RB */
    GPIO_SetFunc(SMC_RB_PORT, SMC_RB_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

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
    GPIO_SetFunc(SMC_ADD21_PORT, SMC_ADD21_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(SMC_ADD22_PORT, SMC_ADD22_PIN, GPIO_FUNC_12_EXMC, PIN_SUBFUNC_DISABLE);

    GPIO_SetPins(SMC_IO3_PORT, SMC_IO3_PIN);

    /* Reset */
    GPIO_ResetPins(SMC_RST0_PORT, SMC_RST0_PIN);
    GPIO_ResetPins(SMC_RST1_PORT, SMC_RST1_PIN);
    DDL_DelayMS(10UL);
    GPIO_SetPins(SMC_RST0_PORT, SMC_RST0_PIN);
    GPIO_SetPins(SMC_RST1_PORT, SMC_RST1_PIN);
}

/**
 * @brief  Wait NOR Ready/Busy signal.
 * @param  [in] u32Timeout:          Timeout duration
 * @retval An en_result_t enumeration value:
 *   @arg  Ok:                  Ready.
 *   @arg  ErrorTimeout:        Wait ready signal timeout.
 */
static en_result_t BSP_SMC_S29GL064_WaitReadySignal(uint32_t u32Timeout)
{
    en_pin_state_t enPinState;
    __IO uint32_t u32To = u32Timeout;

    /* Polling on Ready/nBusy signal */
    do {
        u32To--;
        enPinState = GPIO_ReadInputPins(SMC_RB_PORT, SMC_RB_PIN);
    } while ((u32To > 0UL) && (Pin_Reset != enPinState));

    if (u32To)
    {
        u32To = u32Timeout;

        /* Polling on Ready/nBusy signal */
        do {
            u32To--;
            enPinState = GPIO_ReadInputPins(SMC_RB_PORT, SMC_RB_PIN);
        } while ((u32To > 0UL) && (Pin_Set != enPinState));
    }

    return u32To ? Ok : ErrorTimeout;
}

/**
 * @}
 */

#endif /* BSP_MS_HC32F4A0_LQFP176/BSP_S29GL064N90TFI03_ENABLE */

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
