/**
 *******************************************************************************
 * @file  eth/eth_loopback/source/main.c
 * @brief This sample code implements a ethernet loopback function.
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
 * @addtogroup ETH_Loopback
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifdef ETH_INTERFACE_RMII
    /* RMII_INTB */
    #define ETH_RMII_INTB_PORT                  (GPIO_PORT_B)
    #define ETH_RMII_INTB_PIN                   (GPIO_PIN_00)
    #define ETH_RMII_INTB_EXINT_CH              (EXINT_CH00)
    #define ETH_RMII_INTB_INT_SRC               (INT_PORT_EIRQ0)
    #define ETH_RMII_INTB_IRQn                  (Int037_IRQn)
#endif

/* KEY10(SW10) Port/Pin definition */
#define KEY10_PORT                              (GPIO_PORT_A)
#define KEY10_PIN                               (GPIO_PIN_00)
#define KEY10_EXINT_CH                          (EXINT_CH00)
#define KEY10_INT_SRC                           (INT_PORT_EIRQ0)
#define KEY10_GroupIRQn                         (Int033_IRQn)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* eth tx buffer */
struct pbuf txPbuf;
char txBuffer[] = "This software contains source code for use with HDSC \
                   components. This software is licensed by HDSC to be adapted only \
                   for use in systems utilizing HDSC components.";

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
struct netif gnetif;
static uint8_t u8ExIntFlag = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  SysTick interrupt callback function.
 * @param  None
 * @retval None
 */
void SysTick_IrqHandler(void)
{
    SysTick_IncTick();
}

/**
 * @brief  KEY10(SW10) External interrupt Ch.0 callback function
 *         IRQ No.33 in Group IRQ entry No.32~37 is used for EXINT0
 * @param  None
 * @retval None
 */
void EXINT_KEY10_IrqCallback(void)
{
    if (Set == EXINT_GetExIntSrc(KEY10_EXINT_CH))
    {
        u8ExIntFlag = 1U;
        BSP_LED_Off(LED_YELLOW);
        EXINT_ClrExIntSrc(KEY10_EXINT_CH);
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
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1);
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
    // GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    // PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    // SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    // EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    // EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

/**
 * @brief  KEY10(SW10) initialize
 * @param  None
 * @retval None
 */
static void KEY10_Init(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt  = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(KEY10_PORT, KEY10_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh  = KEY10_EXINT_CH;
    stcExintInit.u32ExIntLvl = EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc    = KEY10_INT_SRC;
    stcIrqSignConfig.enIRQn      = KEY10_GroupIRQn;
    stcIrqSignConfig.pfnCallback = &EXINT_KEY10_IrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(KEY10_GroupIRQn);
    NVIC_SetPriority(KEY10_GroupIRQn,DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(KEY10_GroupIRQn);
}

#ifdef ETH_INTERFACE_RMII
/**
 * @brief  ETH RMII interrupt callback function.
 * @param  None
 * @retval None
 */
void ETH_RMII_LinkIrqCallback(void)
{
   if (Set == EXINT_GetExIntSrc(ETH_RMII_INTB_EXINT_CH))
   {
        EthernetIF_UpdateLink(&gnetif);
        EXINT_ClrExIntSrc(ETH_RMII_INTB_EXINT_CH);
   }
}

/**
 * @brief  ETH RMII link interrupt configuration.
 * @param  None
 * @retval None
 */
static void ETH_RMII_LinkIntConfig(void)
{
    stc_exint_init_t stcExintInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExInt  = PIN_EXINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    GPIO_Init(ETH_RMII_INTB_PORT, ETH_RMII_INTB_PIN, &stcGpioInit);

    /* Exint config */
    EXINT_StructInit(&stcExintInit);
    stcExintInit.u32ExIntCh  = ETH_RMII_INTB_EXINT_CH;
    stcExintInit.u32ExIntLvl = EXINT_TRIGGER_FALLING;
    EXINT_Init(&stcExintInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc    = ETH_RMII_INTB_INT_SRC;
    stcIrqSignConfig.enIRQn      = ETH_RMII_INTB_IRQn;
    stcIrqSignConfig.pfnCallback = &ETH_RMII_LinkIrqCallback;
    INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}
#endif /* ETH_INTERFACE_RMII */

/**
 * @brief  Configurate the BSP.
 * @param  None
 * @retval None
 */
static void BSP_Config(void)
{
    /* Configure BSP */
    BSP_IO_Init();
    BSP_LED_Init();
    /* Configure UART */
    DDL_PrintfInit();
    /* KEY10 configuration */
    KEY10_Init();
    /* SysTick configuration */
    SysTick_Init(1000U);
    /* Set Systick Interrupt to the highest priority */
    NVIC_SetPriority(SysTick_IRQn, DDL_IRQ_PRIORITY_00);
    /* Configure link interrupt IO for ETH RMII */
#ifdef ETH_INTERFACE_RMII
    ETH_RMII_LinkIntConfig();
#endif
}

/**
 * @brief  Notify link status change.
 * @param  [in] netif                   Pointer to a struct netif structure
 * @retval None
 */
void EthernetIF_NotifyLinkChange(struct netif *netif)
{
    if (Ok == EthernetIF_IsLinkUp(netif))
    {
        BSP_LED_Off(LED_RED);
        BSP_LED_On(LED_BLUE);
    }
    else
    {
        BSP_LED_Off(LED_BLUE);
        BSP_LED_On(LED_RED);
    }
}

/**
 * @brief  input data handle callback.
 * @param  netif the network interface structure for this ethernetif
 * @param  p the MAC packet to receive (e.g. IP packet including MAC addresses and type)
 * @retval None
 */
void EthernetIF_InputDataCallback(struct netif *netif, struct pbuf *p)
{
    if ((p->len == txPbuf.len) && (memcmp(p->payload, txPbuf.payload, p->len) == 0))
    {
        BSP_LED_On(LED_YELLOW);
    }
    else
    {
        printf("Receive data error! \r\n");
    }
}

/**
 * @brief  Main function of ETH Loopback.
 * @param  None
 * @retval int32_t return value, if needed
 */
int main(void)
{
    /* Peripheral registers write unprotected */
    Peripheral_WE();
    /* Configure clock */
    BSP_CLK_Init();
    /* Configure the BSP */
    BSP_Config();
    /* Configure the Ethernet */
    ethernetif_init(&gnetif);
    /* Peripheral registers write protected */
    Peripheral_WP();
    /* fill data to txPbuf */
    txPbuf.next    = NULL;
    txPbuf.payload = txBuffer;
    txPbuf.len     = strlen(txBuffer);

    while (1)
    {
        /* Send a packet */
        if (1U == u8ExIntFlag)
        {
            u8ExIntFlag = 0U;
            if (Ok != low_level_output(&gnetif, &txPbuf))
            {
                printf("eth send data error! \r\n");
            }
        }
        /* Read a received packet */
        ethernetif_input(&gnetif);
        /* Handle periodic timers */
        EthernetIF_PeriodicHandle(&gnetif);
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
