/**
 *******************************************************************************
 * @file  eth/eth_twoboards/source/ethernetif.c
 * @brief This file implements Ethernet network interface drivers.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Yangjp          First version
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
#include "ethernetif.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup ETH_Two_Boards
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* Global Ethernet handle*/
stc_eth_handle_t EthHandle;

/* Ethernet Tx DMA Descriptor */
__ALIGN_BEGIN stc_eth_dma_desc_t EthDmaTxDscrTab[ETH_TXBUF_NUMBER];
/* Ethernet Rx DMA Descriptor */
__ALIGN_BEGIN stc_eth_dma_desc_t EthDmaRxDscrTab[ETH_RXBUF_NUMBER];
/* Ethernet Transmit Buffer */
__ALIGN_BEGIN uint8_t EthTxBuff[ETH_TXBUF_NUMBER][ETH_TXBUF_SIZE];
/* Ethernet Receive Buffer */
__ALIGN_BEGIN uint8_t EthRxBuff[ETH_RXBUF_NUMBER][ETH_RXBUF_SIZE];

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup ETH_IF_Global_Functions Ethernet Interface Global Functions
 * @{
 */

/**
 * @brief  Initializes the Ethernet GPIO.
 * @param  None
 * @retval None
 */
static void Ethernet_GpioInit(void)
{
    /* Holding ETH_RST */
    BSP_IO_ConfigPortPin(EIO_PORT1, EIO_ETH_RST, EIO_DIR_OUT);
    BSP_IO_WritePortPin(EIO_PORT1, EIO_ETH_RST, (uint8_t)Disable);
    SysTick_Delay(PHY_HW_RESET_DELAY);

   /* Configure MII/RMII selection IO for ETH */
#ifdef ETH_INTERFACE_RMII
    /* Ethernet RMII pins configuration */
    /*
        ETH_RMII_TX_EN --------------> PG11
        ETH_RMII_TXD0 ---------------> PG13
        ETH_RMII_TXD1 ---------------> PG14
        ETH_RMII_REF_CLK ------------> PA1
        ETH_RMII_CRS_DV -------------> PA7
        ETH_RMII_RXD0 ---------------> PC4
        ETH_RMII_RXD1 ---------------> PC5
    */
    /* Configure PA1 and PA7 */
    GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_07), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
    /* Configure PC4 and PC5 */
    GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_04 | GPIO_PIN_05), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
    /* Configure PG11, PG13 and PG14 */
    GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
#else
    /* Ethernet MII pins configuration */
    /*
        ETH_MII_TX_CLK --------------> PB6
        ETH_MII_TX_EN ---------------> PG11
        ETH_MII_TXD0 ----------------> PG13
        ETH_MII_TXD1 ----------------> PG14
        ETH_MII_TXD2 ----------------> PB9
        ETH_MII_TXD3 ----------------> PB8
        ETH_MII_RX_CLK --------------> PA1
        ETH_MII_RX_DV ---------------> PA7
        ETH_MII_RXD0 ----------------> PC4
        ETH_MII_RXD1 ----------------> PC5
        ETH_MII_RXD2 ----------------> PB0
        ETH_MII_RXD3 ----------------> PB1
    */
    /* Configure PA1, PA2 and PA7 */
    GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_07), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
    /* Configure PB0, PB1, PB6, PB8 and PB9 */
    GPIO_SetFunc(GPIO_PORT_B, (GPIO_PIN_00 | GPIO_PIN_01 | GPIO_PIN_06 | GPIO_PIN_08 | GPIO_PIN_09), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
    /* Configure PC1, PC4 and PC5 */
    GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_04 | GPIO_PIN_05), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
    /* Configure PG11, PG13 and PG14 */
    GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14), GPIO_FUNC_11_ETH ,PIN_SUBFUNC_DISABLE);
#endif
}

/**
 * @brief  In this function, the hardware should be initialized.
 *         Called from EthernetIF_Init().
 * @param  None
 * @return An en_result_t enumeration value:
 *           - Ok: The IF is initialize succeeded
 *           - Error: The IF is initialize failed
 */
static en_result_t low_level_init(void)
{
    en_result_t enRet = Ok;
    stc_eth_init_t stcEthInit;

    /* Init Ethernet GPIO */
    Ethernet_GpioInit();
    /* Enable ETH clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_ETHER, Enable);

    /* Reset ETHERNET */
    ETH_DeInit();
    /* Configure structure initialization */
    ETH_CommStructInit(&EthHandle.stcCommInit);
#ifdef ETH_INTERFACE_RMII
    EthHandle.stcCommInit.u32MediaInterface  = ETH_MAC_MEDIA_INTERFACE_RMII;
#else
    EthHandle.stcCommInit.u32MediaInterface  = ETH_MAC_MEDIA_INTERFACE_MII;
#endif
    EthHandle.stcCommInit.u32ChecksumMode    = ETH_MAC_CHECKSUM_MODE_SOFTWARE;

    ETH_StructInit(&stcEthInit);
    /* DMA */
    stcEthInit.stcDmaInit.u32RxDMABurstLength          = ETH_DMA_RX_BURST_LENGTH_1BEAT;
    stcEthInit.stcDmaInit.u32TxDMABurstLength          = ETH_DMA_TX_BURST_LENGTH_1BEAT;
    stcEthInit.stcDmaInit.u32DropChecksumErrorFrame    = ETH_DMA_DROP_CHECKSUM_ERRORFRAME_DISABLE;
    stcEthInit.stcDmaInit.u32ForwardErrorFrame         = ETH_DMA_FORWARD_ERRORFRAME_ENABLE;
    stcEthInit.stcDmaInit.u32ForwardUndersizeGoodFrame = ETH_DMA_FORWARD_UNDERSIZE_GOODFRAME_ENABLE;
    /* MAC */
    stcEthInit.stcMacInit.u32CarrierSense         = ETH_MAC_CARRIER_SENCE_DISABLE;
    stcEthInit.stcMacInit.u32AutoStripPadFCS      = ETH_MAC_AUTO_STRIP_PAD_FCS_ENABLE;
    stcEthInit.stcMacInit.u32ReceiveOwn           = ETH_MAC_RECEIVE_OWN_DISABLE;
    stcEthInit.stcMacInit.u32ReceiveAll           = ETH_MAC_RECEIVE_ALL_ENABLE;
    stcEthInit.stcMacInit.u32PassControlFrame     = ETH_MAC_PASS_CTRLFRAME_FORWARD_ALL;
    stcEthInit.stcMacInit.u32MulticastFrameFilter = ETH_MAC_MULTICASTFRAME_FILTER_NONE;

    /* Select MII or RMII Mode*/
    MODIFY_REG32(M4_ETH->MAC_IFCONFR, ETH_MAC_IFCONFR_IFSEL, EthHandle.stcCommInit.u32MediaInterface);
    /* Set communication mode */
    ETH_MAC_SetDuplexSpeed(EthHandle.stcCommInit.u32DuplexMode, EthHandle.stcCommInit.u32Speed);
    /* Configure MAC and DMA */
    ETH_MAC_Init(&EthHandle, &stcEthInit.stcMacInit);
    ETH_DMA_Init(&stcEthInit.stcDmaInit);

    /* Initialize Tx Descriptors list: Chain Mode */
    ETH_DMA_TxDescListInit(&EthHandle, EthDmaTxDscrTab, &EthTxBuff[0][0], ETH_TXBUF_NUMBER);
    /* Initialize Rx Descriptors list: Chain Mode  */
    ETH_DMA_RxDescListInit(&EthHandle, EthDmaRxDscrTab, &EthRxBuff[0][0], ETH_RXBUF_NUMBER);
    /* Enable MAC and DMA transmission and reception */
    ETH_Start();

    return enRet;
}

/**
 * @brief  This function should do the actual transmission of the packet.
 * @param  p                            The data packet to send
 * @return An en_result_t enumeration value:
 *           - Ok: The packet could be sent
 *           - Error: The packet couldn't be sent
 */
en_result_t low_level_output(struct pbuf *p)
{
    en_result_t enRet;
    struct pbuf *q;
    uint8_t *txBuffer;
    __IO stc_eth_dma_desc_t *DmaTxDesc;
    uint32_t byteCnt = 0UL;
    uint32_t frameLength = 0UL;
    uint32_t bufferOffset;
    uint32_t payloadOffset;

    DmaTxDesc = EthHandle.stcTxDesc;
    txBuffer = (uint8_t *)((EthHandle.stcTxDesc)->u32Buffer1Addr);
    bufferOffset = 0UL;

    /* Copy frame from pbufs to driver buffers */
    for (q = p; q != NULL; q = q->next)
    {
        /* If this buffer isn't available, goto error */
        if (Reset != (DmaTxDesc->u32ControlStatus & ETH_DMATXDESC_OWN))
        {
            enRet = Error;
            goto error;
        }

        /* Get bytes in current buffer */
        byteCnt = q->len;
        payloadOffset = 0UL;
        /* Check if the length of data to copy is bigger than Tx buffer size */
        while ((byteCnt + bufferOffset) > ETH_TXBUF_SIZE)
        {
            /* Copy data to Tx buffer*/
            memcpy((uint8_t *)((uint8_t *)txBuffer + bufferOffset), (uint8_t *)((uint8_t *)q->payload + payloadOffset), (ETH_TXBUF_SIZE - bufferOffset));
            /* Point to next descriptor */
            DmaTxDesc = (stc_eth_dma_desc_t *)(DmaTxDesc->u32Buffer2NextDescAddr);
            /* Check if the buffer is available */
            if (Reset != (DmaTxDesc->u32ControlStatus & ETH_DMATXDESC_OWN))
            {
                enRet = Error;
                goto error;
            }

            txBuffer = (uint8_t *)(DmaTxDesc->u32Buffer1Addr);
            byteCnt = byteCnt - (ETH_TXBUF_SIZE - bufferOffset);
            payloadOffset = payloadOffset + (ETH_TXBUF_SIZE - bufferOffset);
            frameLength = frameLength + (ETH_TXBUF_SIZE - bufferOffset);
            bufferOffset = 0UL;
        }

        /* Copy the remaining bytes */
        memcpy((uint8_t *)((uint8_t *)txBuffer + bufferOffset), (uint8_t *)((uint8_t *)q->payload + payloadOffset), byteCnt);
        bufferOffset = bufferOffset + byteCnt;
        frameLength = frameLength + byteCnt;
    }
    /* Prepare transmit descriptors to give to DMA */
    ETH_DMA_SetTransmitFrame(&EthHandle, frameLength);
    enRet = Ok;

error:
    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if (Reset != (READ_REG32_BIT(M4_ETH->DMA_DMASTSR, ETH_DMA_DMASTSR_TUS)))
    {
        /* Clear DMA TUS flag */
        WRITE_REG32(M4_ETH->DMA_DMASTSR, ETH_DMA_DMASTSR_TUS);
        /* Resume DMA transmission */
        WRITE_REG32(M4_ETH->DMA_TXPOLLR, 0UL);
    }

    return enRet;
}

/**
 * @brief  Should allocate a pbuf and transfer the bytes of the incoming
 *         packet from the interface into the pbuf.
 * @param  None
 * @return a pbuf filled with the received packet (NULL on memory error)
 */
static struct pbuf *low_level_input(void)
{
    uint8_t *pbuf = NULL;
    struct pbuf *p = NULL;
    struct pbuf *q;
    uint16_t len;
    uint8_t *rxBuffer;
    __IO stc_eth_dma_desc_t *DmaRxDesc;
    uint32_t byteCnt = 0UL;
    uint32_t bufferOffset;
    uint32_t payloadOffset;
    uint32_t i = 0UL;

    /* Get received frame */
    if (Ok != ETH_DMA_GetReceiveFrame(&EthHandle))
    {
        return NULL;
    }

    /* Obtain the size of the packet */
    len = (EthHandle.stcRxFrame).u32Length;
    rxBuffer = (uint8_t *)(EthHandle.stcRxFrame).u32Buffer;
    if (len > 0)
    {
        /* Allocate a pbuf chain of pbufs from the buffer */
        pbuf = (uint8_t *)malloc(len + sizeof(struct pbuf));
        if (NULL != pbuf)
        {
            p = (struct pbuf *)pbuf;
            p->next = NULL;
            p->payload = pbuf + sizeof(struct pbuf);
            p->len = len;
            memset(p->payload, 0, p->len);
        }
    }

    if (p != NULL)
    {
        DmaRxDesc = (EthHandle.stcRxFrame).pstcFSDesc;
        bufferOffset = 0UL;
        for (q = p; q != NULL; q = q->next)
        {
            byteCnt = q->len;
            payloadOffset = 0UL;

            /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
            while ((byteCnt + bufferOffset) > ETH_RXBUF_SIZE)
            {
                /* Copy data to pbuf */
                memcpy((uint8_t *)((uint8_t *)q->payload + payloadOffset), (uint8_t *)((uint8_t *)rxBuffer + bufferOffset), (ETH_RXBUF_SIZE - bufferOffset));
                /* Point to next descriptor */
                DmaRxDesc = (stc_eth_dma_desc_t *)(DmaRxDesc->u32Buffer2NextDescAddr);
                rxBuffer = (uint8_t *)(DmaRxDesc->u32Buffer1Addr);
                byteCnt = byteCnt - (ETH_RXBUF_SIZE - bufferOffset);
                payloadOffset = payloadOffset + (ETH_RXBUF_SIZE - bufferOffset);
                bufferOffset = 0UL;
            }

            /* Copy remaining data in pbuf */
            memcpy((uint8_t *)((uint8_t *)q->payload + payloadOffset), (uint8_t *)((uint8_t *)rxBuffer + bufferOffset), byteCnt);
            bufferOffset = bufferOffset + byteCnt;
        }
    }

    /* Release descriptors to DMA */
    /* Point to first descriptor */
    DmaRxDesc = (EthHandle.stcRxFrame).pstcFSDesc;
    for (i = 0UL; i < (EthHandle.stcRxFrame).u32SegCount; i++)
    {
        DmaRxDesc->u32ControlStatus |= ETH_DMARXDESC_OWN;
        DmaRxDesc = (stc_eth_dma_desc_t *)(DmaRxDesc->u32Buffer2NextDescAddr);
    }
    /* Clear Segment_Count */
    (EthHandle.stcRxFrame).u32SegCount = 0UL;

    /* When Rx Buffer unavailable flag is set, clear it and resume reception */
    if (Reset != (READ_REG32_BIT(M4_ETH->DMA_DMASTSR, ETH_DMA_DMASTSR_RUS)))
    {
        /* Clear DMA RUS flag */
        WRITE_REG32(M4_ETH->DMA_DMASTSR, ETH_DMA_DMASTSR_RUS);
        /* Resume DMA reception */
        WRITE_REG32(M4_ETH->DMA_RXPOLLR, 0UL);
    }

    return p;
}

/**
 * @brief  Should be called at the beginning of the program to set up the 
           network interface.
 * @param  None
 * @return An en_result_t enumeration value:
 *           - Ok: The IF is initialized
 *           - Error: The IF is uninitialized
 */
en_result_t EthernetIF_Init(void)
{
    /* initialize the hardware */
    return low_level_init();
}

/**
 * @brief  This function should be called when a packet is ready to be read
 *         from the interface.
 * @param  None
 * @return An en_result_t enumeration value:
 *           - Ok: A packet was received
 *           - Error: No data packet received
 */
en_result_t EthernetIF_Input(void)
{
    en_result_t enRet = Ok;
    struct pbuf *p;

    /* Move received packet into a new pbuf */
    p = low_level_input();
    /* No packet could be read, silently ignore this */
    if (p == NULL)
    {
        enRet = Error;
    }
    else
    {
        /* Input callback */
        EthernetIF_InputCallback(p);
        free(p);
        p = NULL;
    }

    return enRet;
}

/**
 * @brief  This function should be called when you are ready to write packets to
 *         the interface.
 * @param  p                            The data packet to send
 * @return An en_result_t enumeration value:
 *           - Ok: The packet could be sent
 *           - Error: The packet couldn't be sent
 */
en_result_t EthernetIF_Output(struct pbuf *p)
{
    return low_level_output(p);
}

/**
 * @brief  Input data handle callback.
 * @param  p                            Packets received.
 * @retval None
 */
__WEAKDEF void EthernetIF_InputCallback(struct pbuf *p)
{
    /* This is function could be implemented in user file
       when the callback is needed */
}

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
