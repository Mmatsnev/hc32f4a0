/**
 *******************************************************************************
 * @file  hash/hash_dma/source/main.c
 * @brief Main program HASH for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Heqb          First version
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
 * @addtogroup HASH_Dma
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DMA_BLKSIZE         (16U)
#define DMA_UNIT            (M4_DMA1)
#define DMA_CH              (DMA_CH0)
#define HASH_DATA_CHAR      (1U)
#define HASH_DATA_ARRAY     (2U)

#define HASH_DATA_TYPE      (HASH_DATA_ARRAY)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
static void FillData(void);
static void DMA_Reconfig(void);
#endif
static void Peripheral_WE(void);
static void Peripheral_WP(void);
static void DMA_Config(void);
static void HASH_Config(void);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t  u8HashMsgDigest[32U];
static uint8_t u8ExpectDigest[32] = \
{0x20,0x93,0x00,0x6c,0x2a,0x8a,0x79,0x8a,0x12,0xab,0x00,0xf0,0x08,0xab,\
 0xb1,0xe3,0x5c,0xdd,0x7c,0x74,0x85,0xb1,0xff,0xc7,0xc0,0x95,0x0c,0xf9,\
 0x7e,0x33,0xa0,0x98};

static uint32_t u32DataSize = 0U;
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)

static uint32_t u32Temp;
static uint32_t u32FillData = 0U;
static uint32_t u32Pos = 0U;
static uint8_t  u8FillFlag = 0U;
static uint32_t u32TsfCnt = 0U;
static uint8_t  u8tempFlag = 0U;
static uint32_t  u32FillBuffer[16U];

static char *SrcData = "HDSC-HC32F4A0.hash/hash_dma/source/main.c\
Main program HASH base for the Device Driver Library";
#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)

static uint32_t SrcData[] = \
{0x48445343,0x2D484333,0x32463441,0x302E6861,0x73682F68,0x6173685F,\
 0x646D612F,0x736F7572,0x63652F6D,0x61696E2E,0x634D6169,0x6E207072,\
 0x6F677261,0x6D204841,0x53482062,0x61736520,0x666F7220,0x74686520,\
 0x44657669,0x63652044,0x72697665,0x72204C69,0x62726172,0x79800000,\
 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,\
 0x00000000,0x000002E8};

#endif 

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 *********************************************** *******************************/
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
    /* Unlock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    //PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Unlock SRAM register: WTCR */
    //SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    //SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    //EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OPT write protect registers */
    //EFM_OTP_WP_Unlock();
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
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK registers, @ref PWC_REG_Write_Unlock_Code for details */
    //PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
    /* Lock SRAM register: WTCR */
    //SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    //SRAM_CKCR_Lock();
    /* Lock EFM OPT write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    //EFM_Lock();
}

/**
 * @brief  Main function of hash_base project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Unlock peripherals or registers */
    Peripheral_WE();
    /* Config HASH */
    HASH_Config();
    /* Congif DMA */
    DMA_Config();
    /* Config UART for printing. Baud rate 115200. */
    DDL_PrintfInit();
    /* Set the first group. */
    HASH_MsgGrpConfig(HASH_MSG_GRP_FIRST);
    /* Start HASH */
    HASH_Start();
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)

    do{
        /* clear the DAM transfer completed flag */
        DMA_ClearTransIntStatus(DMA_UNIT, DMA_BTC_INT_CH0 | DMA_TC_INT_CH0);
        if(strlen((char *)SrcData) <= LAST_GROUP_MAX_LEN)
        {
            /* Set the last group. */
            HASH_MsgGrpConfig(HASH_MSG_GRP_END);
        }
        u32FillData = strlen((char *)SrcData) - HASH_GROUP_LEN * u32TsfCnt;
        if (u32FillData >= HASH_GROUP_LEN)
        {
            for(uint8_t i = 0U; i < DMA_BLKSIZE; i++)
            {
                u32Temp = *((uint32_t *)&SrcData[u32Pos]);
                u32FillBuffer[i] = (uint32_t)__REV(u32Temp);
                u32Pos += 4U;
            }
            u32TsfCnt ++;
        }
        else
        {
            FillData();
            
            if (u8FillFlag == 1U)
            {
                /* Select the DMA1 channal_0 transfer complete as trigger source */
                HASH_SetTriggerSrc(HASH_TRG_SRC_DMA1_TC0);
            }
        }
        /* DMA Reconfigration */
        DMA_Reconfig();
        /* AOS trigger for DMA */
        AOS_SW_Trigger();
    }while(u8FillFlag == 0U);

#endif

    /* Wait for the message operation to complete */
    while(HASH_GetStatus(HASH_FLAG_CYC_END) == Reset)
    {
        ;
    }
    /* Get the digest result */
    HASH_GetResult(u8HashMsgDigest);
    /* Clear the flag */
    HASH_ClearStatus(HASH_FLAG_CYC_END);
    if ((uint8_t)memcmp(u8HashMsgDigest, u8ExpectDigest, sizeof(u8HashMsgDigest)) == 0U)
    {
        printf("message digest:\n");
        for (uint8_t i = 0U; i < sizeof(u8HashMsgDigest); i++)
        {
            printf("%.2x ", u8HashMsgDigest[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Computation Errors\n");
    }
    /* Lock peripherals or registers */
    Peripheral_WP();
    while (1)
    {

    }

}

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
/**
 * @brief  HASH Filling data.
 * @param  None
 * @retval None
 */
static void FillData(void)
{
    uint32_t u32BitLenHig;
    uint32_t u32BitLenLow;
    uint8_t u8buffer[64U] = {0U};

    u32BitLenHig = (u32DataSize >> 29U) & 0x7U;
    u32BitLenLow = (u32DataSize << 3U);
    if((u32FillData >= LAST_GROUP_MAX_LEN) && (u8tempFlag == 0U))
    {
        memcpy(u8buffer, &SrcData[HASH_GROUP_LEN * u32TsfCnt], u32FillData);
        u8buffer[u32FillData] = 0x80U;
        for(uint8_t i = 0U; i < HASH_GROUP_LEN; i+=4U)
        {
            u32Temp = *((uint32_t *)&u8buffer[i]);
            u32FillBuffer[i/4U] = (uint32_t)__REV(u32Temp);
        }
        u8FillFlag = 0U;
        u32FillData = 0U;
        u8tempFlag = 1U;
    }
    else
    {
        if(u8tempFlag == 1U)
        {
            ;
        }
        else
        {
            memcpy(u8buffer, &SrcData[HASH_GROUP_LEN * u32TsfCnt], u32FillData);
            u8buffer[u32FillData] = 0x80U;
            
        }
        u8buffer[63U] = (uint8_t)(u32BitLenLow);
        u8buffer[62U] = (uint8_t)(u32BitLenLow >> 8U);
        u8buffer[61U] = (uint8_t)(u32BitLenLow >> 16U);
        u8buffer[60U] = (uint8_t)(u32BitLenLow >> 24U);
        u8buffer[59U] = (uint8_t)(u32BitLenHig);
        u8buffer[58U] = (uint8_t)(u32BitLenHig >> 8U);
        u8buffer[57U] = (uint8_t)(u32BitLenHig >> 16U);
        u8buffer[56U] = (uint8_t)(u32BitLenHig >> 24U);
        u8FillFlag = 1U;
        for(uint8_t i = 0U; i < HASH_GROUP_LEN; i+=4U)
        {
            u32Temp = *((uint32_t *)&u8buffer[i]);
            u32FillBuffer[i/4U] = (uint32_t)__REV(u32Temp);
        }
    }
}
#endif

/**
 * @brief  HASH Configration.
 * @param  None
 * @retval None
 */
static void HASH_Config(void)
{
    /* Enable HASH. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_HASH, Enable);
    /* Config HASH */
    HASH_SetMode(HASH_MODE_SHA_256);            /* SHA_256 Operating mode */
    HASH_IntCmd(HASH_INT_GROUP, Disable);       /* Disable interrupt function */
    /* Enable AOS. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    HASH_SetTriggerSrc(HASH_TRG_SRC_DMA1_BTC0); /* Select the DMA1 channal_0 block transfer complete as trigger source */
#if (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    HASH_SetTriggerSrc(HASH_TRG_SRC_DMA1_TC0);  /* Select the DMA1 channal_0transfer complete as trigger source */
#endif
}

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
/**
 * @brief  DMA Reconfigration.
 * @param  None
 * @retval None
 */
static void DMA_Reconfig(void)
{
    DMA_SetTransCnt(DMA_UNIT, DMA_CH, 1U);
    DMA_SetSrcAddr(DMA_UNIT, DMA_CH, (uint32_t)(&u32FillBuffer[0]));
    DMA_SetDestAddr(DMA_UNIT, DMA_CH, (uint32_t)(&M4_HASH->DR15));
    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH, Enable);
}
#endif

/**
 * @brief  DMA Configration.
 * @param  None
 * @retval None
 */
static void DMA_Config(void)
{
    stc_dma_init_t stcDmaInit;

    /* Enable DMA1. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_DMA1, Enable);

    DMA_StructInit(&stcDmaInit);
    /* Config DMA */
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_32BIT;
    stcDmaInit.u32BlockSize = DMA_BLKSIZE;
    stcDmaInit.u32IntEn     = DMA_INT_DISABLE;
    /* Set transfer count */
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    u32DataSize = strlen((char *)SrcData);
    stcDmaInit.u32TransCnt = 1U;
    stcDmaInit.u32SrcAddr = (uint32_t)(&u32FillBuffer[0]);

#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    u32DataSize = sizeof(SrcData);
    stcDmaInit.u32TransCnt = u32DataSize / HASH_GROUP_LEN;
    stcDmaInit.u32SrcAddr = (uint32_t)(&SrcData[0]);
#endif 


    stcDmaInit.u32DestAddr = (uint32_t)(&M4_HASH->DR15);
    stcDmaInit.u32SrcInc = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc = DMA_DEST_ADDR_INC;
    if (Ok != DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit))
    {    
        while (1)
        {}
    }
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    /* Set DMA trigger source */
    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH, EVT_AOS_STRG);

#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    /* Set DMA trigger source */
    DMA_SetTriggerSrc(DMA_UNIT, DMA_CH, EVT_HASH);

    stc_dma_rpt_init_t stcDmaRptInit;
    DMA_RepeatStructInit(&stcDmaRptInit);
    /* DMA destination repeat enable  */
    stcDmaRptInit.u32DestRptEn = DMA_DEST_RPT_ENABLE;
    /* DMA source repeat size = 16 */
    stcDmaRptInit.u32DestRptSize = DMA_BLKSIZE;
    DMA_RepeatInit(DMA_UNIT, DMA_CH, &stcDmaRptInit);
#endif

    /* DMA module enable */
    DMA_Cmd(DMA_UNIT, Enable);
    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH, Enable);
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
