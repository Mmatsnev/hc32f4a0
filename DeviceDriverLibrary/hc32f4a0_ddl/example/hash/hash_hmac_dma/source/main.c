/**
 *******************************************************************************
 * @file  hash/hash_hmac_dma/source/main.c
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
 * @addtogroup HASH_Hmac_Dma
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DMA_BLKSIZE         (16U)
#define HASH_IRQn           (Int010_IRQn)
#define HASH_SOURCE         (INT_HASH)
#define DMA_UNIT            (M4_DMA1)
#define DMA_CH              (DMA_CH0)

#define u8Type_Key          (1U)
#define u8Type_Data         (2U)

#define HASH_DATA_CHAR      (1U)
#define HASH_DATA_ARRAY     (2U)

#define HASH_DATA_TYPE      (HASH_DATA_ARRAY)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void HASH_Config(void);
static void DMA_Config(const uint32_t au32Data[], uint32_t u32DataLength);
static void IRQ_Config(void);
static void Peripheral_WE(void);
static void Peripheral_WP(void);

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
static void FillData(uint8_t *u8Data);
static void DMA_Reconfig(void);
static void DataConversion(uint8_t u8Type);
#endif
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t u8HashMsgDigest[32U];
static uint8_t u8ExpectDigest[32] = \
{0xca,0x33,0x45,0x1a,0xca,0x33,0xc8,0xca,0xef,0x9f,0x9d,0x1f,\
 0xb4,0x5e,0x92,0x57,0x04,0xe2,0xba,0xbf,0x4e,0x5d,0x7c,0xa2,\
 0x8b,0x8c,0x88,0x31,0x3d,0x5b,0x1b,0xea};

static uint8_t u8Count = 0U;
static uint32_t u32DataSize = 0U;

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
static uint32_t u32Temp;
static uint32_t u32TsfCnt = 0U;
static uint32_t u32Pos = 0U;
static uint32_t u32FillData = 0U;
static uint8_t  u8FillFlag = 0U;
static uint8_t  u8TempFlag = 0U;
static uint32_t u32FillBuffer[16U] = {0U};
static uint8_t  u8Tempbuffer[64U] = {0U};

static char *SrcData = \
"abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\
01234567890123456789";

static char *Key = "HDSC-HC32F4A0.hash/hash_hmac/source/main.c\
Main program HASH for the Device Driver Library";

#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)
static uint32_t SrcData[] = \
{0x61626364U,0x65666768U,0x696A6B6CU,0x6D6E6F70U,0x71727374U,0x75767778U,\
 0x797A3031U,0x32333435U,0x36373839U,0x41424344U,0x45464748U,0x494A4B4CU,\
 0x4D4E4F50U,0x51525354U,0x55565758U,0x595A3031U,0x32333435U,0x36373839U,\
 0x61626364U,0x65666768U,0x696A6B6CU,0x6D6E6F70U,0x71727374U,0x75767778U,\
 0x797A3031U,0x32333435U,0x36373839U,0x41424344U,0x45464748U,0x494A4B4CU,\
 0x4D4E4F50U,0x51525354U,0x55565758U,0x595A3031U,0x32333435U,0x36373839U,\
 0x61626364U,0x65666768U,0x696A6B6CU,0x6D6E6F70U,0x71727374U,0x75767778U,\
 0x797A3031U,0x32333435U,0x36373839U,0x41424344U,0x45464748U,0x494A4B4CU,\
 0x4D4E4F50U,0x51525354U,0x55565758U,0x595A3031U,0x32333435U,0x36373839U,\
 0x30313233U,0x34353637U,0x38393031U,0x32333435U,0x36373839U,0x80000000U,\
 0x00000000U,0x00000000U,0x00000000U,0x00000760U};

static uint32_t Key[] = \
{0x48445343U,0x2D484333U,0x32463441U,0x302E6861U,0x73682F68U,0x6173685FU,\
 0x686D6163U,0x2F736F75U,0x7263652FU,0x6D61696EU,0x2E634D61U,0x696E2070U,\
 0x726F6772U,0x616D2048U,0x41534820U,0x666F7220U,0x74686520U,0x44657669U,\
 0x63652044U,0x72697665U,0x72204C69U,0x62726172U,0x79800000U,0x00000000U,\
 0x00000000U,0x00000000U,0x00000000U,0x00000000U,0x00000000U,0x00000000U,\
 0x00000000U,0x000002C8U};
#endif


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
 * @brief  HASH operations complete IRQ callback
 * @param  None
 * @retval None
 */
void Hash_IrqCallback(void)
{
    if(++u8Count >= 2U)
    {
        /* Get the message digest result */
        HASH_GetResult(u8HashMsgDigest);
        u8Count = 0U;
        if ((uint8_t)memcmp(u8HashMsgDigest, u8ExpectDigest,sizeof(u8HashMsgDigest)) == 0U)
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
        /* Clear the CYC_END flag */
        HASH_ClearStatus(HASH_FLAG_CYC_END);
        /* Clear the HMAC_END flag */
        HASH_ClearStatus(HASH_FLAG_HMAC_END);
    }
    /* Clear the CYC_END flag */
    HASH_ClearStatus(HASH_FLAG_CYC_END);
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    /* Disable DMA1_Ch1 trigger HASH */
    HASH_SetTriggerSrc(HASH_TRG_SRC_DMA1_TC2);
#endif
}

/**
 * @brief  Main function of template project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* Unlock peripherals or registers */
    Peripheral_WE();
    /* Config IRQ */
    IRQ_Config();
    /* Config HASH */
    HASH_Config();
    /* Config UART for printing. Baud rate 115200. */
    DDL_PrintfInit();
/******************* Secret key to calculate *********************/

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    /* Congif DMA */
    DMA_Config((uint32_t *)Key, strlen((char *)Key));
    /* Is the length of the secret key greater than 64 bytes */
    if(strlen((char *)Key) > HASH_KEY_LEN_LONG)
    {
        HASH_SetKeyLength(HASH_KEY_LEN_LONG);
    }
#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    /* Congif DMA */
    DMA_Config((uint32_t *)Key, sizeof(Key));
    /* Is the length of the secret key greater than 64 bytes */
    if(sizeof(Key) > HASH_KEY_LEN_LONG)
    {
        HASH_SetKeyLength(HASH_KEY_LEN_LONG);
    }
#endif


    /* Set the first group. */
    HASH_MsgGrpConfig(HASH_MSG_GRP_FIRST);
    /* Start HASH */
    HASH_Start();

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    DataConversion(u8Type_Key);
#endif

/*********************  Message to calculate *********************/
    while(u8Count == 0U)
    {
        ;
    }
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    /* Congif DMA */
    DMA_Config((uint32_t *)SrcData, strlen((char *)SrcData));
#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    /* clear the DAM transfer completed flag */
    DMA_ClearTransIntStatus(DMA_UNIT, DMA_BTC_INT_CH0 | DMA_TC_INT_CH0);
    /* Congif DMA */
    DMA_Config((uint32_t *)SrcData, sizeof(SrcData));
#endif

    /* Set the first group. */
    HASH_MsgGrpConfig(HASH_MSG_GRP_FIRST);
    /* Start HASH */
    HASH_Start();

#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    DataConversion(u8Type_Data);
#endif
    /* Lock peripherals or registers */
    Peripheral_WP();
    while (1)
    {
        ;
    }
}

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
    HASH_SetMode(HASH_MODE_HMAC);              /* HMAC Operating mode */
    HASH_IntCmd(HASH_INT_ALL, Enable);         /* Enable the All data operations complete interrupt */
    /* Enable AOS. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_AOS, Enable);
    HASH_SetTriggerSrc(HASH_TRG_SRC_DMA1_BTC0);/* Select the DMA1 channal_0 block transfer complete as trigger source */
#if (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    HASH_SetTriggerSrc(HASH_TRG_SRC_DMA1_TC0); /* Select the DMA1 channal_0 transfer complete as trigger source */
#endif
}

/**
 * @brief  DMA Configration.
 * @param  [in] au32Data        Buffer of the source data
 * @param  [in] u32DataLength   Length of the source data
 * @retval None
 */
static void DMA_Config(const uint32_t au32Data[], uint32_t u32DataLength)
{
    stc_dma_init_t stcDmaInit;
    /* Enable DMA1. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_DMA1, Enable);
    u32DataSize = u32DataLength;

    DMA_StructInit(&stcDmaInit);
    /* Config DMA */
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_32BIT;
    stcDmaInit.u32BlockSize = DMA_BLKSIZE;
    
#if (HASH_DATA_TYPE == HASH_DATA_CHAR)
    stcDmaInit.u32TransCnt = 1U;
    stcDmaInit.u32SrcAddr = (uint32_t)(&u32FillBuffer[0U]);
#elif (HASH_DATA_TYPE == HASH_DATA_ARRAY)
    stcDmaInit.u32TransCnt = u32DataSize / HASH_GROUP_LEN;
    stcDmaInit.u32SrcAddr = (uint32_t)(&au32Data[0U]);
#endif /* #if (HASH_DATA_TYPE == HASH_DATA_CHAR) */

    stcDmaInit.u32DestAddr = (uint32_t)(&M4_HASH->DR15);
    stcDmaInit.u32SrcInc = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestInc = DMA_DEST_ADDR_INC;
    if (Ok != DMA_Init(DMA_UNIT, DMA_CH, &stcDmaInit))
    {    
        while (1)
        {
            ;
        }
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
#endif /* #if (HASH_DATA_TYPE == HASH_DATA_CHAR) */

    /* DMA module enable */
    DMA_Cmd(DMA_UNIT, Enable);
    /* DMA channel enable */
    DMA_ChannelCmd(DMA_UNIT, DMA_CH, Enable);
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

/**
 * @brief  DATA conversion.
 * @param  [in] u8Type  Key or data
 * @retval None
 */
static void DataConversion(uint8_t u8Type)
{
    if(u8Type == u8Type_Key)
    {
        u8FillFlag = 0U;
        u32TsfCnt = 0U;
        u32Pos = 0U;
        if(strlen((char *)Key) > HASH_GROUP_LEN)
        {
            do{
                /* clear the DAM transfer completed flag */
                DMA_ClearTransIntStatus(DMA_UNIT, DMA_BTC_INT_CH0 | DMA_TC_INT_CH0);
                u32FillData = strlen((char *)Key) - HASH_GROUP_LEN * u32TsfCnt;
                if (u32FillData >= HASH_GROUP_LEN)
                {  
                    for(uint8_t i = 0U; i < DMA_BLKSIZE; i++)
                    {
                        u32Temp = *((uint32_t *)&Key[u32Pos]);
                        u32FillBuffer[i] = (uint32_t)__REV(u32Temp);
                        u32Pos += 4U;
                    }
                    u32TsfCnt ++;
                }
                else
                {
                    FillData((uint8_t *)Key);
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
        }
        else
        {
            memcpy(u8Tempbuffer, &Key[0], strlen((char *)Key));
            for(uint8_t i = 0U; i < HASH_GROUP_LEN; i+=4U)
            {
                u32Temp = *((uint32_t *)&u8Tempbuffer[i]);
                u32FillBuffer[i/4U] = (uint32_t)__REV(u32Temp);
            }
            /* Set the last group. */
            HASH_MsgGrpConfig(HASH_MSG_GRP_END);
            /* AOS trigger for DMA */
            AOS_SW_Trigger();
        }
    }
    else
    {
        u8FillFlag = 0U;
        u32TsfCnt = 0U;
        u32Pos = 0U;
        do{
            /* clear the DAM transfer completed flag */
            DMA_ClearTransIntStatus(DMA_UNIT, DMA_BTC_INT_CH0 | DMA_TC_INT_CH0);
            if (strlen((char *)SrcData) <= LAST_GROUP_MAX_LEN)
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
                FillData((uint8_t *)SrcData);
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
    }
}

/**
 * @brief  HASH Filling data.
 * @param  None
 * @retval None
 */
static void FillData(uint8_t *u8Data)
{
    uint32_t u32BitLenHig;
    uint32_t u32BitLenLow;
    uint8_t u8buffer[64] = {0U};
    u32BitLenHig = (u32DataSize >> 29U) & 0x7U;
    u32BitLenLow = (u32DataSize << 3U);
    if ((u32FillData >= LAST_GROUP_MAX_LEN) && (u8TempFlag == 0U))
    {
        memcpy(u8buffer, &u8Data[DMA_BLKSIZE * u32TsfCnt], u32FillData);
        u8buffer[u32FillData] = 0x80U;
        for(uint8_t i = 0U; i < HASH_GROUP_LEN; i+=4U)
        {
            u32Temp = *((uint32_t *)&u8buffer[i]);
            u32FillBuffer[i/4U] = (uint32_t)__REV(u32Temp);
        }
        u8FillFlag = 0U;
        u32FillData = 0U;
        u8TempFlag = 1U;
    }
    else
    {
        if (u8TempFlag == 1U)
        {
            ;
        }
        else
        {
            memcpy(u8buffer, &u8Data[HASH_GROUP_LEN * u32TsfCnt], u32FillData);
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

#endif /* if (HASH_DATA_TYPE == HASH_DATA_CHAR) */

/**
 * @brief  IRQ configration.
 * @param  None
 * @retval None
 */
static void IRQ_Config(void)
{
    stc_irq_signin_config_t stcIrqRegCfg;
    /* Register IRQ handler && configure NVIC. */
    stcIrqRegCfg.enIRQn = HASH_IRQn;
    stcIrqRegCfg.enIntSrc = HASH_SOURCE;
    stcIrqRegCfg.pfnCallback = &Hash_IrqCallback;
    INTC_IrqSignIn(&stcIrqRegCfg);
    NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
    NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIORITY_15);
    NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);
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
