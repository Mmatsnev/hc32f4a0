/**
 *******************************************************************************
 * @file  can/can_normal/source/main.c
 * @brief Main program of CAN normal for the Device Driver Library.
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
 * @addtogroup CAN_Normal
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/* CAN FD function control. Non-zero to enable. */
#define APP_CAN_FD_ENABLE                   (1U)

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

    #define CAN_SHARE_IRQn                  (Int138_IRQn)
    #define APP_CAN_INT_TYPE                (CAN_INT_ALL)
    #define APP_CAN_INT_PRIO                (DDL_IRQ_PRIORITY_03)
    #define APP_CAN_INT_SRC                 (INT_CAN2_HOST)
    #define APP_CAN_IRQn                    (Int092_IRQn)
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

/*
 * External interrupt pin definition. See \example\exint\exint_key\source\main.c for details.
 * Press SW10 on the board to trigger a CAN transmission.
 */
#define KEY_PORT                            (GPIO_PORT_A)
#define KEY_PIN                             (GPIO_PIN_00)
#define KEY_EXINT_CH                        (EXINT_CH00)
#define KEY_INT_SRC                         (INT_PORT_EIRQ0)
#define KEY_IRQn                            (Int025_IRQn)
#define KEY_INT_PRIO                        (DDL_IRQ_PRIORITY_04)

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
static void CanTx(void);
static void CanTxEntity(const stc_can_tx_t *pstcTx, uint8_t u8TxBufType);
static void CanRx(void);
static void CanHandleExceptions(void);
static void CAN_IrqCallback(void);

static void Tmr2Config(void);
static void TMR2_CmpA_IrqCallback(void);

static void ExintConfig(void);
static void EXINT_IrqCallback(void);

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

    const static char *m_s8TxBufType[] = {
        "PTB",
        "STB",
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

/* External interrupt flag. */
static uint8_t m_u8ExintFlag = 0U;

/* CAN error type. */
__IO static uint8_t m_u8ErrType = 0U;

/* CAN status value. */
__IO static uint32_t m_u32StatusVal = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function of CAN normal project
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
    /* Set CAN PYH STB pin as low. */
    CanPhyEnable();
    /* Configures and starts Timer2 for 1 second timing. */
    Tmr2Config();
    /* Press SW10 on the board to trigger a CAN transmission. */
    ExintConfig();
    /* MCU Peripheral registers write protected. */
    Peripheral_WP();
    /***************** Configuration end, application start **************/

    while (1U)
    {
        CanTx();

        CanRx();

        CanHandleExceptions();
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
    stcInit.pstcAFCfg = astcAFCfg;
    stcInit.u8RBOvfOp = CAN_RB_OVF_SAVE_NEW;
    stcInit.u16AFSel  = APP_CAN_AF_SEL;

    /*stcInit.stcSBT.u32SEG1 = 1U + 15U;
    stcInit.stcSBT.u32SEG2 = 4U;
    stcInit.stcSBT.u32SJW  = 4U;*/
    stcInit.stcSBT.u32Prescaler = 8U;

#if (APP_CAN_FD_ENABLE > 0U)
    stcInit.enCANFDCmd = Enable;
    stcInit.stcFDCfg.u8TDCSSP = 16U;
    stcInit.stcFDCfg.u8CANFDMode = CAN_FD_MODE_ISO_11898;
    stcInit.stcFDCfg.stcFBT.u32SEG1 = 16U;
    stcInit.stcFDCfg.stcFBT.u32SEG2 = 4U;
    stcInit.stcFDCfg.stcFBT.u32SJW  = 4U;
    stcInit.stcFDCfg.stcFBT.u32Prescaler = 1U;
#endif

    PWC_Fcg1PeriphClockCmd(APP_CAN_PERIP_CLK, Enable);
    CAN_Init(APP_CAN_UNIT, &stcInit);

    /* Configures the interrupts if needed. */
    CanIrqConfig();
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
    stcInit.u32CmpVal = 58593U;
    TMR2_Init(M4_TMR2_1, TMR2_CH_A, &stcInit);

    stcCfg.enIntSrc    = INT_TMR2_1_CMPA;
    stcCfg.enIRQn      = Int050_IRQn;
    stcCfg.pfnCallback = &TMR2_CmpA_IrqCallback;
    INTC_IrqSignIn(&stcCfg);
    NVIC_ClearPendingIRQ(stcCfg.enIRQn);
    NVIC_SetPriority(stcCfg.enIRQn, DDL_IRQ_PRIORITY_05);
    NVIC_EnableIRQ(stcCfg.enIRQn);

    TMR2_IntCmd(M4_TMR2_1, TMR2_CH_A, TMR2_INT_CMP, Enable);
    TMR2_Start(M4_TMR2_1, TMR2_CH_A);
}

/**
 * @brief  SW10 external interrupt configuration.
 * @param  None
 * @retval None
 */
static void ExintConfig(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEY_PORT, KEY_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh = KEY_EXINT_CH;
    stcExintInit.u32ExIntLvl= EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY_IRQn;
    stcIrqSignConfig.pfnCallback = &EXINT_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEY_IRQn);
    NVIC_SetPriority(KEY_IRQn, KEY_INT_PRIO);
    NVIC_EnableIRQ(KEY_IRQn);
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

    if (m_u32StatusVal != 0U)
    {
        DBG("CAN needs handle exceptions.\n");
    }
    else
    {
        for (i=0U; i<APP_DATA_SIZE; i++)
        {
            m_au8TxPayload[i] = i + 1U;
        }

        if (m_u8TmrFlag)
        {
            stcTx.pu8Data = (uint8_t *)&m_au8TxPayload[0U];
            stcTx.u32Ctrl = 0x0U;
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
            CanTxEntity(&stcTx, CAN_BUF_STB);
            m_u8TmrFlag = 0U;
        }

        if (m_u8ExintFlag)
        {
            /* Configures the frame with ID 'APP_ID_2'. */
            stcTx.pu8Data = (uint8_t *)&m_au8TxPayload[0U];
            stcTx.u32Ctrl = 0x0U;
        #if (APP_CAN_FD_ENABLE > 0U)
            stcTx.FDF = 1U;
            stcTx.BRS = 1U;
            stcTx.DLC = CAN_DLC_32;
        #else
            stcTx.DLC = CAN_DLC_3;
        #endif
            stcTx.u32ID = APP_ID_2;
            stcTx.IDE   = APP_ID_2_IDE;
            CanTxEntity(&stcTx, CAN_BUF_PTB);
            m_u8ExintFlag = 0U;
        }
    }
}

/**
 * @brief  CAN transmission entity.
 * @param  [in]  pstcTx                 Pointer to a frame which is going to be transmitted.
 * @param  [in]  u8TxBufType            Transmit buffer type.
 *                                      This parameter can be a value of @ref CAN_Transmit_Buffer_Type
 *   @arg  CAN_BUF_PTB:                 Primary transmit buffer.
 *   @arg  CAN_BUF_STB:                 Secondary transmit buffer.
 * @retval None
 */
static void CanTxEntity(const stc_can_tx_t *pstcTx, uint8_t u8TxBufType)
{
    en_result_t enTxResult;

    enTxResult = CAN_TransData(APP_CAN_UNIT, pstcTx,
                               u8TxBufType, CAN_STB_TRANS_ONE, 0U);
    if (enTxResult == Ok)
    {
        /* Transmission OK. */
        DBG("CAN loaded %s frame with %s ID %.8lx into %s\n", \
             m_s8FrameTypeStr[pstcTx->FDF], m_s8IDTypeStr[pstcTx->IDE], pstcTx->u32ID, m_s8TxBufType[u8TxBufType]);
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
 * @brief  CAN exceptions handler.
 * @param  None
 * @retval None
 */
static void CanHandleExceptions(void)
{
    uint8_t u8ErrType;
    if (m_u32StatusVal != 0U)
    {
        u8ErrType = (m_u8ErrType >> 5U);
        DBG("---> CAN error type: %d, %s\n", u8ErrType, m_s8ErrorType[u8ErrType]);
        DBG("CAN status flags %.8lx\n", m_u32StatusVal);

        if (m_u32StatusVal & CAN_FLAG_PTB_TRANS_OK)
        {
            /* Transmission OK. */
            DBG("PTB has been transmitted.\n");
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_PTB_TRANS_OK);
        }
        if (m_u32StatusVal & CAN_FLAG_STB_TRANS_OK)
        {
            DBG("STB has been transmitted.\n");
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_STB_TRANS_OK);
        }
        if (m_u32StatusVal & CAN_FLAG_BUS_OFF)
        {
            /* BUS OFF. Needs power on reset or 128 occurrence of 11 consecutive 'recessive' bits to resume. */
            DBG("BUS OFF!!!\n");
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_BUS_OFF);
        }
        if (m_u32StatusVal & CAN_FLAG_BUS_TX)
        {
            /* The BUS is transmitting. Can abort the transmission if needed. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_BUS_TX);
        }
        if (m_u32StatusVal & CAN_FLAG_BUS_RX)
        {
            /* The BUS is receiving. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_BUS_RX);
        }
        if (m_u32StatusVal & CAN_FLAG_TB_FULL)
        {
            /* TX buffers are full. */
            DBG("Transmit buffers are full.\n");
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_TB_FULL);
        }
        if (m_u32StatusVal & CAN_FLAG_RB_OVF)
        {
            /* RX buffer overflow. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_RB_OVF);
        }
        if (m_u32StatusVal & CAN_FLAG_TRANS_ABORTED)
        {
            /* Transmission is aborted. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_TRANS_ABORTED);
        }
        if (m_u32StatusVal & CAN_FLAG_ERR_INT)
        {
            /* Error interrupt. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_ERR_INT);
        }
        if (m_u32StatusVal & CAN_FLAG_RB_ALMOST_FULL)
        {
            /* Receive buffers are almost full. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_RB_ALMOST_FULL);
        }
        if (m_u32StatusVal & CAN_FLAG_RB_FIFO_FULL)
        {
            /* Receive buffers are full. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_RB_FIFO_FULL);
        }
        if (m_u32StatusVal & CAN_FLAG_RX_OVERRUN)
        {
            /* Receive buffer overrun. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_RX_OVERRUN);
        }
        if (m_u32StatusVal & CAN_FLAG_RX_OK)
        {
            /* Received a frame. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_RX_OK);
        }
        if (m_u32StatusVal & CAN_FLAG_BUS_ERR)
        {
            /* Bus error. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_BUS_ERR);
        }
        if (m_u32StatusVal & CAN_FLAG_ARB_LOST)
        {
            /* Arbitration lost. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_ARB_LOST);
        }
        if (m_u32StatusVal & CAN_FLAG_ERR_PASSIVE)
        {
            /* Error passive. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_ERR_PASSIVE);
        }
        if (m_u32StatusVal & CAN_FLAG_ERR_PASSIVE_NODE)
        {
            /* The node is an error passive node. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_ERR_PASSIVE_NODE);
        }
        if (m_u32StatusVal & CAN_FLAG_REACH_WARN_LIMIT)
        {
            /* TEC or REC reached warning limit. */
            m_u32StatusVal &= (uint32_t)(~CAN_FLAG_REACH_WARN_LIMIT);
        }
    }
}

/**
 * @brief  CAN interrupt callback.
 * @param  None
 * @retval None
 */
static void CAN_IrqCallback(void)
{
    m_u32StatusVal = CAN_GetStatusVal(APP_CAN_UNIT);
    m_u8ErrType    = CAN_GetErrType(APP_CAN_UNIT);

    CAN_ClrStatus(APP_CAN_UNIT, m_u32StatusVal);
}

/**
 * @brief  Timer2 interrupt callback.
 * @param  None
 * @retval None
 */
static void TMR2_CmpA_IrqCallback(void)
{
    if (TMR2_GetStatus(M4_TMR2_1, TMR2_CH_A, TMR2_FLAG_CMP) == Set)
    {
        m_u8TmrFlag = 1U;
        TMR2_ClrStatus(M4_TMR2_1, TMR2_CH_A, TMR2_FLAG_CMP);
    }
}

/**
 * @brief  External interrupt callback.
 * @param  None
 * @retval None
 */
static void EXINT_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEY_EXINT_CH))
    {
        /* Wait for key release. */
        while (Pin_Reset == GPIO_ReadInputPins(KEY_PORT, KEY_PIN))
        {
            ;
        }
        m_u8ExintFlag = 1U;
        EXINT_ClrExIntSrc(KEY_EXINT_CH);
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
