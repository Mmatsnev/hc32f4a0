/**
 *******************************************************************************
 * @file  can/can_ttc/source/main.c
 * @brief Main program of CAN ttc for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wuze            First version
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
 * @addtogroup CAN_Ttc
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Unit definition of CAN in this example. */
#define APP_CAN_SEL_U1                      (0U)
#define APP_CAN_SEL_U2                      (1U)

/* Select a CAN unit. */
#define APP_CAN_SEL                         (APP_CAN_SEL_U1)

/*
 * Definitions according to 'APP_CAN'.
 * CAN independent IRQn: [Int000_IRQn, Int031_IRQn], [Int092_IRQn, Int097_IRQn].
 */
#if (APP_CAN_SEL == APP_CAN_SEL_U1)
    #define APP_CAN_UNIT                    (M4_CAN1)
    #define APP_CAN_PERIP_CLK               (PWC_FCG1_CAN1)

    #define APP_CAN_TX_PORT                 (GPIO_PORT_D)
    #define APP_CAN_TX_PIN                  (GPIO_PIN_05)
    #define APP_CAN_TX_PIN_FUNC             (GPIO_FUNC_60_CAN1_TX)

    #define APP_CAN_RX_PORT                 (GPIO_PORT_D)
    #define APP_CAN_RX_PIN                  (GPIO_PIN_04)
    #define APP_CAN_RX_PIN_FUNC             (GPIO_FUNC_61_CAN1_RX)

    #define APP_CAN_CLK_CH                  (CLK_CAN_CH1)
    #define APP_CAN_CLK_DIV                 (CLK_CAN1_CLK_MCLK_DIV3)

    #define APP_CAN_INT_TYPE                (CAN_INT_ALL)
    #define APP_CAN_INT_PRIO                (DDL_IRQ_PRIORITY_03)
    #define APP_CAN_INT_SRC                 (INT_CAN1_HOST)
    #define APP_CAN_IRQn                    (Int092_IRQn)
	#define APP_CAN_TTC_INT_TYPE			(CAN_TTC_INT_ALL)

#elif (APP_CAN_SEL == APP_CAN_SEL_U2)
    #define APP_CAN_UNIT                    (M4_CAN2)
    #define APP_CAN_PERIP_CLK               (PWC_FCG1_CAN2)

    #define APP_CAN_TX_PORT                 (GPIO_PORT_D)
    #define APP_CAN_TX_PIN                  (GPIO_PIN_07)
    #define APP_CAN_TX_PIN_FUNC             (GPIO_FUNC_62_CAN2_TX)

    #define APP_CAN_RX_PORT                 (GPIO_PORT_D)
    #define APP_CAN_RX_PIN                  (GPIO_PIN_06)
    #define APP_CAN_RX_PIN_FUNC             (GPIO_FUNC_63_CAN2_RX)

    #define APP_CAN_CLK_CH                  (CLK_CAN_CH2)
    #define APP_CAN_CLK_DIV                 (CLK_CAN2_CLK_MCLK_DIV3)

    #define APP_CAN_INT_TYPE                (CAN_INT_ALL)
    #define APP_CAN_INT_PRIO                (DDL_IRQ_PRIORITY_03)
    #define APP_CAN_INT_SRC                 (INT_CAN2_HOST)
    #define APP_CAN_IRQn                    (Int092_IRQn)
	#define APP_CAN_TTC_INT_TYPE			(CAN_TTC_INT_ALL)
#else
    #error "The unit is NOT supported!!!"
#endif

/* Acceptance filter. */
#define APP_CAN_AF_SEL                      (CAN_AF1 | CAN_AF2 | CAN_AF3 | CAN_AF4)

#define APP_CAN_AF1_ID                      (0xA1)
#define APP_CAN_AF1_ID_MSK                  (0x00)                  /* Only accept messages with ID '1010 0001'. */
#define APP_CAN_AF1_MSK_TYPE                (CAN_AF_MSK_STD)        /* Only accept standard ID. */

#define APP_CAN_AF2_ID                      (0xB2)
#define APP_CAN_AF2_ID_MSK                  (0xB0)                  /* Accept messages with ID 'x0xx 0010'. */
#define APP_CAN_AF2_MSK_TYPE                (CAN_AF_MSK_EXT)        /* Accept extended ID. */

#define APP_CAN_AF3_ID                      (0xC3)
#define APP_CAN_AF3_ID_MSK                  (0x03)                  /* Accept messages with ID '1100 00xx'. */
#define APP_CAN_AF3_MSK_TYPE                (CAN_AF_MSK_STD_EXT)    /* Accept standard ID and extended ID. */

#define APP_CAN_AF4_ID                      (0x701)
#define APP_CAN_AF4_ID_MSK                  (0x00)                  /* Only accept messages with ID '1101 0000'. */
#define APP_CAN_AF4_MSK_TYPE                (CAN_AF_MSK_STD)        /* Accept standard ID. */

/* TTCAN reference ID. */
#define APP_CAN_TTC_REF_ID                  (APP_CAN_AF4_ID)
#define APP_CAN_TTC_REF_IDE                 (Disable)               /* Enable or Disable */

/* Message ID definitions. */
#define APP_ID_1                            (0x601)
#define APP_ID_1_IDE                        (1U)

#define APP_ID_2                            (0x602)
#define APP_ID_2_IDE                        (0U)

/* Max data size of the frame. */
#if (APP_CAN_FD_ENABLE > 0U)
    #define APP_DATA_SIZE                   (64U)
#else
    #define APP_DATA_SIZE                   (8U)
#endif

/* Debug printing definition. */
#if (DDL_PRINT_ENABLE == DDL_ON)
#define DBG         printf
#else
#define DBG(...)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void Peripheral_WE(void);
static void Peripheral_WP(void);

static void SystemClockConfig(void);

static void CanPhyEnable(void);

static void CanConfig(void);
static void CanIrqConfig(void);

static void CanTTCTx(void);
static void CanRx(void);

static void CAN_IrqCallback(void);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
#if (DDL_PRINT_ENABLE == DDL_ON)
    const static char *m_s8IDTypeStr[] = {
        "standard",
        "extended",
    };

    const static char *m_s8FrameTypeStr[] = {
        "normal",
        "CAN-FD",
    };

    const static char *m_s8ErrorType[] = {
        "no error.",
        "Bit Error.",
        "Form Error.",
        "Stuff Error.",
        "ACK Error.",
        "CRC Error.",
        "Other Error.",
    };
#endif

/*
 * DLC to data size.
 * Row: 0 is normal frame, 1 is CAN-FD frame.
 * Column: data length code(0 ~ 15). 
 */
const static uint8_t m_au8DLC2Size[2U][16U] =
{
    {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 8U, 8U, 8U, 8U, 8U, 8U, 8U},
    {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U},
};

/* Data buffers. */
static uint8_t m_au8TxPayload[APP_DATA_SIZE];
static uint8_t m_au8RxPayload1[APP_DATA_SIZE];
static uint8_t m_au8RxPayload2[APP_DATA_SIZE];
static uint8_t m_au8RxPayload3[APP_DATA_SIZE];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of CAN ttc project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the system clock as 240MHz. */
    SystemClockConfig();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures CAN. */
    CanConfig();
    /* Set CAN PHY STB pin as low. */
    CanPhyEnable();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /***************** Configuration end, application start **************/

    DBG("TTCAN start\n");

    while (1U)
    {
        CanRx();
    }
}

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
    // SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    // EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    // EFM_OTP_WP_Unlock();
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
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
}

/**
 * @brief Configures PLLHP(240MHz) as the system clock.
 * @param  None
 * @retval None
 */
static void SystemClockConfig(void)
{
    stc_clk_pllh_init_t stcPLLHInit;
    stc_clk_xtal_init_t stcXtalInit;

    /* Configures XTAL. PLLH input source is XTAL. */
    CLK_XtalStrucInit(&stcXtalInit);
    stcXtalInit.u8XtalState = CLK_XTAL_ON;
    stcXtalInit.u8XtalDrv   = CLK_XTALDRV_LOW;
    stcXtalInit.u8XtalMode  = CLK_XTALMODE_OSC;
    stcXtalInit.u8XtalStb   = CLK_XTALSTB_499US;
    CLK_XtalInit(&stcXtalInit);

    /* PCLK0, HCLK  Max 240MHz */
    /* PCLK1, PCLK4 Max 120MHz */
    /* PCLK2, PCLK3 Max 60MHz  */
    /* EX BUS Max 120MHz */
    CLK_ClkDiv(CLK_CATE_ALL,                                        \
               (CLK_PCLK0_DIV1 | CLK_PCLK1_DIV16 | CLK_PCLK2_DIV4 | \
                CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2 | CLK_EXCLK_DIV2 |  \
                CLK_HCLK_DIV1));

    CLK_PLLHStrucInit(&stcPLLHInit);
    /* VCO = 8/1*120 = 960MHz*/
    stcPLLHInit.u8PLLState = CLK_PLLH_ON;
    stcPLLHInit.PLLCFGR    = 0UL;
    stcPLLHInit.PLLCFGR_f.PLLM = 1UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLN = 120UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLP = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLQ = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLR = 4UL - 1UL;

    /* stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL; */
    CLK_PLLHInit(&stcPLLHInit);

    /* Set SRAM wait cycles. */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE_1, SRAM_WAIT_CYCLE_1);
    SRAM_SetWaitCycle((SRAM_SRAM123 | SRAM_SRAM4 | SRAM_SRAMB), SRAM_WAIT_CYCLE_2, SRAM_WAIT_CYCLE_2);

    /* Set EFM wait cycle. 5 wait cycles needed when system clock is 240MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_5);

    CLK_PLLHCmd(Enable);
    CLK_SetSysClkSrc(CLK_SYSCLKSOURCE_PLLH);
}

/**
 * @brief  Set CAN PHY STB pin as low.
 * @param  None
 * @retval None
 */
static void CanPhyEnable(void)
{
    BSP_IO_Init();
    BSP_CAN_STB_IO_Init();

    /* Set PYH STB pin as low. */
    BSP_CAN_STBCmd(EIO_PIN_RESET);
}

/**
 * @brief  CAN configuration.
 * @param  None
 * @retval None
 */
static void CanConfig(void)
{
    stc_can_init_t stcInit;
    stc_can_ttc_cfg_t stcTTCCfg;
    stc_can_af_cfg_t astcAFCfg[] = { \
        {APP_CAN_AF1_ID, APP_CAN_AF1_ID_MSK, APP_CAN_AF1_MSK_TYPE}, \
        {APP_CAN_AF2_ID, APP_CAN_AF2_ID_MSK, APP_CAN_AF2_MSK_TYPE}, \
        {APP_CAN_AF3_ID, APP_CAN_AF3_ID_MSK, APP_CAN_AF3_MSK_TYPE}, \
        {APP_CAN_AF4_ID, APP_CAN_AF4_ID_MSK, APP_CAN_AF4_MSK_TYPE}, \
    };

    /* Set the function of CAN pins. */
    GPIO_SetFunc(APP_CAN_TX_PORT, APP_CAN_TX_PIN, \
                 APP_CAN_TX_PIN_FUNC, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(APP_CAN_RX_PORT, APP_CAN_RX_PIN, \
                 APP_CAN_RX_PIN_FUNC, PIN_SUBFUNC_DISABLE);

    /* Configures the clock of CAN. */
    CLK_CAN_ClkConfig(APP_CAN_CLK_CH, APP_CAN_CLK_DIV);

    /* Initializes CAN. */
    CAN_StructInit(&stcInit);
    stcInit.u8WorkMode = CAN_MODE_NORMAL;
    stcInit.pstcAFCfg  = astcAFCfg;
    stcInit.u8RBOvfOp  = CAN_RB_OVF_SAVE_NEW;
    stcInit.u16AFSel   = APP_CAN_AF_SEL;

    /* Bit rate 500kbps. bit time is 2us. */
    stcInit.stcSBT.u32Prescaler = 8U;
    PWC_Fcg1PeriphClockCmd(APP_CAN_PERIP_CLK, Enable);
    CAN_Init(APP_CAN_UNIT, &stcInit);

    /* Confiures TTCAN. */
    CAN_TTC_StructInit(&stcTTCCfg);
    /* NTU is slow bit time * 8, is 16us. */
    stcTTCCfg.u8NTUPrescaler   = CAN_TTC_NTU_PRESC_8;
    stcTTCCfg.u32RefMsgIDE     = APP_CAN_TTC_REF_IDE;
    stcTTCCfg.u32RefMsgID      = APP_CAN_TTC_REF_ID;
    stcTTCCfg.u16TrigType      = CAN_TTC_TRIG_SSHOT_TRANS_TRIG;
    stcTTCCfg.u16TxTrigTime    = 31250U;
    stcTTCCfg.u16WatchTrigTime = 65000U;
    CAN_TTC_ClrStatus(APP_CAN_UNIT, CAN_TTC_FLAG_ALL);
    CAN_TTC_Config(APP_CAN_UNIT, &stcTTCCfg);

    /* Configures the interrupts if needed. */
    CanIrqConfig();
    CAN_TTC_Cmd(APP_CAN_UNIT, Enable);
}

/**
 * @brief  CAN interrupt configuration.
 * @param  None
 * @retval None
 */
static void CanIrqConfig(void)
{
    stc_irq_signin_config_t stcCfg;

    stcCfg.enIntSrc    = APP_CAN_INT_SRC;
    stcCfg.enIRQn      = APP_CAN_IRQn;
    stcCfg.pfnCallback = &CAN_IrqCallback;
    INTC_IrqSignIn(&stcCfg);

    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, APP_CAN_INT_PRIO);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    /* Enable the specified interrupts of CAN. */
    CAN_IntCmd(APP_CAN_UNIT, APP_CAN_INT_TYPE, Enable);
    CAN_TTC_IntCmd(APP_CAN_UNIT, APP_CAN_TTC_INT_TYPE, Enable);
}

/**
 * @brief  CAN transmits data.
 * @param  None
 * @retval None
 */
static void CanTTCTx(void)
{
    uint8_t i;
    stc_can_tx_t stcTx;

    for (i=0U; i<APP_DATA_SIZE; i++)
    {
        m_au8TxPayload[i] = 0xC0 + i;
    }

    stcTx.u32Ctrl = 0x0U;
    stcTx.pu8Data = (uint8_t *)&m_au8TxPayload[0U];
    stcTx.u32ID   = APP_ID_1;
    stcTx.IDE     = APP_ID_1_IDE;
    stcTx.DLC     = CAN_DLC_5;
    CAN_TTC_TransData(APP_CAN_UNIT, &stcTx, CAN_TTC_TBS_STB2);
}

/**
 * @brief  CAN receives data.
 * @param  None
 * @retval None
 */
static void CanRx(void)
{
    uint8_t u8RxCnt;
    uint8_t u8DataIdx;
    uint8_t u8DataSize;
    uint8_t u8RxFrameCnt = 0;
    uint8_t u8RefMsgFlag = 0U;
    stc_can_rx_t stcRx[3U];

    stcRx[0U].pu8Data = (uint8_t *)&m_au8RxPayload1[0U];
    stcRx[1U].pu8Data = (uint8_t *)&m_au8RxPayload2[0U];
    stcRx[2U].pu8Data = (uint8_t *)&m_au8RxPayload3[0U];

    CAN_ReceiveData(APP_CAN_UNIT, &stcRx[0U], &u8RxFrameCnt, 3U);
    for (u8RxCnt=0U; u8RxCnt<u8RxFrameCnt; u8RxCnt++)
    {
        DBG("CAN received %s frame with %s ID %.8lx, cycle time:%d\n", \
            m_s8FrameTypeStr[stcRx[u8RxCnt].FDF], \
            m_s8IDTypeStr[stcRx[u8RxCnt].IDE], \
            stcRx[u8RxCnt].u32ID, \
            stcRx[u8RxCnt].CYCLE_TIME);
        u8DataSize = m_au8DLC2Size[stcRx[u8RxCnt].FDF][stcRx[u8RxCnt].DLC];
        for (u8DataIdx=0; u8DataIdx<u8DataSize; u8DataIdx++)
        {
            DBG(" %.2x.", stcRx[u8RxCnt].pu8Data[u8DataIdx]);
            stcRx[u8RxCnt].pu8Data[u8DataIdx] = 0U;
        }
        DBG("\n");

        if (stcRx[u8RxCnt].u32ID == APP_CAN_TTC_REF_ID)
        {
            DBG("\n\n");
            DBG("Received a reference message ID %.8x\n", APP_CAN_TTC_REF_ID);
            u8RefMsgFlag = 1U;
        }
    }

    if (u8RefMsgFlag)
    {
        CanTTCTx();
        DBG("TTCAN loaded data.\n");
    }
}

/**
 * @brief  CAN interrupt callback. Debug print is only for test.
 * @param  None
 * @retval None
 */
static void CAN_IrqCallback(void)
{
    uint32_t u32StatusVal;
    uint8_t u8ErrType;
    uint8_t u8TtcStatusVal;

    u8ErrType      = CAN_GetErrType(APP_CAN_UNIT);
    u8TtcStatusVal = CAN_TTC_GetStatusVal(APP_CAN_UNIT);
    u32StatusVal   = CAN_GetStatusVal(APP_CAN_UNIT);

    if (u32StatusVal != 0U)
    {
        u8ErrType = (u8ErrType >> 5U);
        DBG("---> CAN error type: %d, %s\n", u8ErrType, m_s8ErrorType[u8ErrType]);
        DBG("CAN status flags %.8lx\n", u32StatusVal);

        if (u32StatusVal & CAN_FLAG_PTB_TRANS_OK)
        {
            /* Transmission OK. */
            DBG("PTB has been transmitted.\n");
        }
        if (u32StatusVal & CAN_FLAG_STB_TRANS_OK)
        {
            DBG("STB has been transmitted.\n");
        }
        if (u32StatusVal & CAN_FLAG_BUS_OFF)
        {
            /* BUS OFF. Needs power on reset or 128 occurrence of 11 consecutive 'recessive' bits to resume. */
            DBG("BUS OFF!!!\n");
        }
        if (u32StatusVal & CAN_FLAG_BUS_TX)
        {
            /* The BUS is transmitting. Can abort the transmission if needed. */
            DBG("The BUS is transmitting. Can abort the transmission if needed.\n");
        }

        if (u32StatusVal & CAN_FLAG_BUS_RX)
        {
            /* The BUS is receiving. */
            DBG("The BUS is receiving.\n");
        }
        if (u32StatusVal & CAN_FLAG_TB_FULL)
        {
            /* TX buffers are full. */
            DBG("Transmit buffers are full.\n");
        }
        if (u32StatusVal & CAN_FLAG_RB_OVF)
        {
            /* RX buffer overflow. */
            DBG("RX buffer overflow.\n");
        }
        if (u32StatusVal & CAN_FLAG_TRANS_ABORTED)
        {
            /* Transmission is aborted. */
            DBG("Transmission is aborted.\n");
        }
        if (u32StatusVal & CAN_FLAG_ERR_INT)
        {
            /* Error interrupt. */
            DBG("Error interrupt.\n");
        }
        if (u32StatusVal & CAN_FLAG_RB_ALMOST_FULL)
        {
            /* Receive buffers are almost full. */
            DBG("Receive buffers are almost full.\n");
        }
        if (u32StatusVal & CAN_FLAG_RB_FIFO_FULL)
        {
            /* Receive buffers are full. */
            DBG("Receive buffers are full.\n");
        }
        if (u32StatusVal & CAN_FLAG_RX_OVERRUN)
        {
            /* Receive buffer overrun. */
            DBG("Receive buffer overrun.\n");
        }
        if (u32StatusVal & CAN_FLAG_RX_OK)
        {
            /* Received a frame. */
            DBG("Received a frame.\n");
        }
        if (u32StatusVal & CAN_FLAG_BUS_ERR)
        {
            /* Bus error. */
            DBG("Bus error.\n");
        }
        if (u32StatusVal & CAN_FLAG_ARB_LOST)
        {
            /* Arbitration lost. */
            DBG("Arbitration lost.\n");
        }
        if (u32StatusVal & CAN_FLAG_ERR_PASSIVE)
        {
            /* Error passive. */
            DBG("Error passive.\n");
        }
        if (u32StatusVal & CAN_FLAG_ERR_PASSIVE_NODE)
        {
            /* The node is an error passive node. */
            DBG("The node is an error passive node.\n");
        }
        if (u32StatusVal & CAN_FLAG_REACH_WARN_LIMIT)
        {
            /* TEC or REC reached warning limit. */
            DBG("TEC or REC reached warning limit.\n");
        }

        CAN_ClrStatus(APP_CAN_UNIT, u32StatusVal);
    }

    if (u8TtcStatusVal != 0U)
    {
        if (u8TtcStatusVal & CAN_TTC_FLAG_TTI)
        {
            DBG("TTC: Time Trigger Interrupt Flag\n");
        }

        if (u8TtcStatusVal & CAN_TTC_FLAG_TEI)
        {
            DBG("TTC: Trigger Error Interrupt Flag\n");
        }

        if (u8TtcStatusVal & CAN_TTC_FLAG_WTI)
        {
            DBG("TTC: Watch Trigger Interrupt Flag\n");
        }

        CAN_TTC_ClrStatus(APP_CAN_UNIT, u8TtcStatusVal);
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
