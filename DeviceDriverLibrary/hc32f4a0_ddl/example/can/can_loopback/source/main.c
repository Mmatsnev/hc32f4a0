/**
 *******************************************************************************
 * @file  can/can_loopback/source/main.c
 * @brief Main program of CAN loopback for the Device Driver Library.
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
 * @addtogroup CAN_Loopback
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*
 * CAN mode definitions.
 *
 * CAN_MODE_ELB: External loopback mode. Self transmits self receives, or communicates
 *               with other CAN modules.
 * CAN_MODE_NORMAL: Normal communication mode. Can only communicates with other CAN modules.
 */
#define APP_CAN_MODE                        (CAN_MODE_ELB)

/* Specify the function of the target. */
#if (APP_CAN_MODE != CAN_MODE_ELB) && (APP_CAN_MODE != CAN_MODE_NORMAL)
    #error "The mode is NOT supported in this example."
#endif

/* CAN FD function control. Non-zero to enable. */
#define APP_CAN_FD_ENABLE                   (0U)

/* Unit definition of CAN in this example. */
#define APP_CAN_SEL_U1                      (0U)
#define APP_CAN_SEL_U2                      (1U)

/* Select a CAN unit. */
#define APP_CAN_SEL                         (APP_CAN_SEL_U1)

/* Definitions according to 'APP_CAN'. */
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
#else
    #error "The unit is NOT supported!!!"
#endif

/* Acceptance filter. */
#define APP_CAN_AF_SEL                      (CAN_AF1 | CAN_AF2 | CAN_AF3)

#define APP_CAN_AF1_ID                      (0xA1)
#define APP_CAN_AF1_ID_MSK                  (0x00)                  /* Only accept messages with ID '1010 0001'. */
#define APP_CAN_AF1_MSK_TYPE                (CAN_AF_MSK_STD)        /* Only accept standard ID. */

#define APP_CAN_AF2_ID                      (0xB2)
#define APP_CAN_AF2_ID_MSK                  (0xB0)                  /* Accept messages with ID 'x0xx 0010'. */
#define APP_CAN_AF2_MSK_TYPE                (CAN_AF_MSK_EXT)        /* Accept extended ID. */

#define APP_CAN_AF3_ID                      (0xC3)
#define APP_CAN_AF3_ID_MSK                  (0x03)                  /* Accept messages with ID '1100 00xx'. */
#define APP_CAN_AF3_MSK_TYPE                (CAN_AF_MSK_STD_EXT)    /* Accept standard ID and extended ID. */

/* Message ID definitions. */
#define APP_ID_1                            (0x9AC01)
#define APP_ID_1_IDE                        (1U)

#define APP_ID_2                            (0x52)
#define APP_ID_2_IDE                        (0U)

#define APP_ID_3                            (APP_CAN_AF3_ID)
#define APP_ID_3_IDE_ENABLE                 (1U)
#define APP_ID_3_IDE_DISABLE                (0U)

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
static void Tmr2Config(void);

static void CanTx(void);
static void CanTxEntity(const stc_can_tx_t *pstcTx);

static void CanRx(void);

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
        "Value of error type is invalid.",
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

/* One second timing flag. */
static uint8_t m_u8TmrFlag = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of CAN loopback project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* MCU Peripheral registers write unprotected. */
    Peripheral_WE();
    /* Configures the system clock as 120MHz. */
    SystemClockConfig();
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Initializes UART for debug printing. Baudrate is 115200. */
    DDL_PrintfInit();
#endif /* #if (DDL_PRINT_ENABLE == DDL_ON) */
    /* Configures CAN. */
    CanConfig();
    /* Set CAN PYH STB pin as low. */
    CanPhyEnable();
    /* Configures and starts Timer2 for 1 second timing. */
    Tmr2Config();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();

    /***************** Configuration end, application start **************/

    while (1U)
    {
        if (m_u8TmrFlag)
        {
            CanTx();
            m_u8TmrFlag = 0U;
        }

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
    // SRAM_WTCR_Unlock();
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
    // SRAM_WTCR_Lock();
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
 * @brief Configures PLLHP(120MHz) as the system clock.
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
    stcPLLHInit.PLLCFGR_f.PLLP = 8UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLQ = 4UL - 1UL;
    stcPLLHInit.PLLCFGR_f.PLLR = 4UL - 1UL;

    /* stcPLLHInit.PLLCFGR_f.PLLSRC = CLK_PLLSRC_XTAL; */
    CLK_PLLHInit(&stcPLLHInit);

    /* Set EFM wait cycle. 2 wait cycles needed when system clock is 120MHz */
    EFM_SetWaitCycle(EFM_WAIT_CYCLE_2);

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
    stc_can_af_cfg_t astcAFCfg[] = { \
        {APP_CAN_AF1_ID, APP_CAN_AF1_ID_MSK, APP_CAN_AF1_MSK_TYPE}, \
        {APP_CAN_AF2_ID, APP_CAN_AF2_ID_MSK, APP_CAN_AF2_MSK_TYPE}, \
        {APP_CAN_AF3_ID, APP_CAN_AF3_ID_MSK, APP_CAN_AF3_MSK_TYPE}, \
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
    stcInit.u8WorkMode   = APP_CAN_MODE;
    stcInit.pstcAFCfg    = astcAFCfg;
    stcInit.u8RBOvfOp    = CAN_RB_OVF_SAVE_NEW;
    stcInit.u16AFSel     = APP_CAN_AF_SEL;
#if (APP_CAN_MODE == CAN_MODE_ELB)
    stcInit.u8SelfACKCmd = CAN_SELF_ACK_ENABLE;
#else
    stcInit.u8SelfACKCmd = CAN_SELF_ACK_DISABLE;
#endif
    stcInit.stcSBT.u32SEG1 = 1U + 14U;
    stcInit.stcSBT.u32SEG2 = 5U;
    stcInit.stcSBT.u32SJW  = 5U;
    stcInit.stcSBT.u32Prescaler = 4U;

#if (APP_CAN_FD_ENABLE > 0U)
    stcInit.enCANFDCmd = Enable;
    stcInit.stcFDCfg.u8TDCSSP = 4U;
    stcInit.stcFDCfg.u8CANFDMode = CAN_FD_MODE_BOSCH;
    stcInit.stcFDCfg.stcFBT.u32SEG1 = 8U;
    stcInit.stcFDCfg.stcFBT.u32SEG2 = 2U;
    stcInit.stcFDCfg.stcFBT.u32SJW  = 2U;
    stcInit.stcFDCfg.stcFBT.u32Prescaler = 1U;
#endif

    PWC_Fcg1PeriphClockCmd(APP_CAN_PERIP_CLK, Enable);
    CAN_Init(APP_CAN_UNIT, &stcInit);
}

/**
 * @brief  Timer2 configuration.
 * @param  None
 * @retval None
 */
static void Tmr2Config(void)
{
    stc_tmr2_init_t stcInit;
    stc_irq_signin_config_t stcCfg;

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_TMR2_1, Enable);

    TMR2_StructInit(&stcInit);

    stcInit.u32ClkSrc = TMR2_CLK_SYNC_PCLK1;
    stcInit.u32ClkDiv = TMR2_CLK_DIV256;
    stcInit.u32CmpVal = 29296U;
    TMR2_Init(M4_TMR2_1, TMR2_CH_A, &stcInit);

    stcCfg.enIntSrc    = INT_TMR2_1_CMPA;
    stcCfg.enIRQn      = Int050_IRQn;
    stcCfg.pfnCallback = &TMR2_1_CmpA_IrqHandler;
    INTC_IrqSignIn(&stcCfg);
    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    TMR2_IntCmd(M4_TMR2_1, TMR2_CH_A, TMR2_INT_CMP, Enable);
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);
}

/**
 * @brief  CAN transmits data.
 * @param  None
 * @retval None
 */
static void CanTx(void)
{
    uint8_t i;
    stc_can_tx_t stcTx;

    for (i=0U; i<APP_DATA_SIZE; i++)
    {
        m_au8TxPayload[i] = i + 1U;
    }

    stcTx.u32Ctrl = 0x0U;
    stcTx.pu8Data = (uint8_t *)&m_au8TxPayload[0U];

    /* Configures the frame with ID 'APP_ID_1'. */
#if (APP_CAN_FD_ENABLE > 0U)
    stcTx.FDF = 1U;
    stcTx.BRS = 1U;
    stcTx.DLC = CAN_DLC_64;
#else
    stcTx.DLC = CAN_DLC_8;
#endif
    stcTx.u32ID = APP_ID_1;
    stcTx.IDE   = APP_ID_1_IDE;
    CanTxEntity(&stcTx);

    /* Configures the frame with ID 'APP_ID_2'. */
    stcTx.u32Ctrl = 0x0U;
    stcTx.u32ID   = APP_ID_2;
    stcTx.IDE     = APP_ID_2_IDE;
    stcTx.DLC     = CAN_DLC_7;
    CanTxEntity(&stcTx);

    /* Configures the frame with ID 'APP_ID_3'. */
    stcTx.u32Ctrl = 0x0U;
    stcTx.u32ID   = APP_ID_3;
    stcTx.IDE     = APP_ID_3_IDE_ENABLE;
    stcTx.DLC     = CAN_DLC_3;
    CanTxEntity(&stcTx);

    stcTx.u32Ctrl = 0x0U;
#if (APP_CAN_FD_ENABLE > 0U)
    stcTx.FDF = 1U;
    stcTx.BRS = 1U;
    stcTx.DLC = CAN_DLC_32;
#else
    stcTx.DLC = CAN_DLC_3;
#endif
    stcTx.u32ID = APP_ID_3;
    stcTx.IDE   = APP_ID_3_IDE_DISABLE;
    CanTxEntity(&stcTx);
}

/**
 * @brief  CAN transmission entity.
 * @param  [in]  pstcTx                 Pointer to a frame which is going to be transmitted.
 * @retval None
 */
static void CanTxEntity(const stc_can_tx_t *pstcTx)
{
    uint8_t u8Tmp;
    uint8_t u8ErrorType;
    uint32_t u32StatusVal;
    en_result_t enTxResult;

    enTxResult = CAN_TransData(APP_CAN_UNIT, pstcTx,
                               CAN_BUF_STB, CAN_STB_TRANS_ONE, 1000U);
    u32StatusVal = CAN_GetStatusVal(APP_CAN_UNIT);
    if (enTxResult == Ok)
    {
        DBG("CAN transmitted %s frame with %s ID: %.8lx   Status value: %.8lx\n", \
             m_s8FrameTypeStr[pstcTx->FDF], m_s8IDTypeStr[pstcTx->IDE], pstcTx->u32ID, u32StatusVal);
    }
    else
    {
        u8ErrorType  = CAN_GetErrType(APP_CAN_UNIT);
        u8ErrorType >>= 5U;
        DBG("---> CAN error type: %d, %s\n", u8ErrorType, m_s8ErrorType[u8ErrorType]);
        DBG("CAN transmission error. Error status flags %.8lx\n", u32StatusVal);

        if (u32StatusVal & CAN_FLAG_BUS_OFF)
        {
            DBG("BUS OFF!!!");
        }
        if (u32StatusVal & CAN_FLAG_BUS_TX)
        {
            DBG("The transmission is still executing.\n");
            u8Tmp = CAN_GetTBType(APP_CAN_UNIT);
            if ((u8Tmp == CAN_BUF_PTB) || (u8Tmp == CAN_BUF_STB))
            {
                CAN_AbortTrans(APP_CAN_UNIT, u8Tmp);
            }
        }
        if (u32StatusVal & CAN_FLAG_TB_FULL)
        {
            DBG("TX buffers are full.\n");
            u8Tmp = CAN_GetTBType(APP_CAN_UNIT);
            if ((u8Tmp == CAN_BUF_PTB) || (u8Tmp == CAN_BUF_STB))
            {
                CAN_AbortTrans(APP_CAN_UNIT, u8Tmp);
            }
        }
        if (u32StatusVal & CAN_FLAG_ERR_INT)
        {
            DBG("Error interrupt.\n");
        }
        if (u32StatusVal & CAN_FLAG_BUS_ERR)
        {
            DBG("Bus error.\n");
        }
        if (u32StatusVal & CAN_FLAG_ARB_LOST)
        {
            DBG("Arbitration lost.\n");
        }
        if (u32StatusVal & CAN_FLAG_ERR_PASSIVE)
        {
            DBG("Error passive.\n");
        }
        if (u32StatusVal & CAN_FLAG_ERR_PASSIVE_NODE)
        {
            DBG("The node is an error passive node.\n");
        }
        if (u32StatusVal & CAN_FLAG_REACH_WARN_LIMIT)
        {
            DBG("TEC or REC reached warning limit.\n");
        }

        CAN_ClrStatus(APP_CAN_UNIT, u32StatusVal);
    }
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
    uint8_t u8RxFrameCnt;
    stc_can_rx_t stcRx[3U];

    stcRx[0U].pu8Data = (uint8_t *)&m_au8RxPayload1[0U];
    stcRx[1U].pu8Data = (uint8_t *)&m_au8RxPayload2[0U];
    stcRx[2U].pu8Data = (uint8_t *)&m_au8RxPayload3[0U];

    CAN_ReceiveData(APP_CAN_UNIT, &stcRx[0U], &u8RxFrameCnt, 3U);
    for (u8RxCnt=0U; u8RxCnt<u8RxFrameCnt; u8RxCnt++)
    {
        DBG("CAN received %s frame with %s ID %.8lx:\n", \
            m_s8FrameTypeStr[stcRx[u8RxCnt].FDF], m_s8IDTypeStr[stcRx[u8RxCnt].IDE], stcRx[u8RxCnt].u32ID);
        u8DataSize = m_au8DLC2Size[stcRx[u8RxCnt].FDF][stcRx[u8RxCnt].DLC];
        for (u8DataIdx=0; u8DataIdx<u8DataSize; u8DataIdx++)
        {
            DBG(" %.2x.", stcRx[u8RxCnt].pu8Data[u8DataIdx]);
            stcRx[u8RxCnt].pu8Data[u8DataIdx] = 0U;
        }
        DBG("\n");
    }
}

/**
 * @brief  Timer2 interrupt callback.
 * @param  None
 * @retval None
 */
void TMR2_1_CmpA_IrqHandler(void)
{
    if (TMR2_GetStatus(M4_TMR2_1, TMR2_CH_A, TMR2_FLAG_CMP) == Set)
    {
        m_u8TmrFlag = 1U;
        TMR2_ClrStatus(M4_TMR2_1, TMR2_CH_A, TMR2_FLAG_CMP);
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
