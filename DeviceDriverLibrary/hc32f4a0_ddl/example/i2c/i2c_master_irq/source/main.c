/**
 *******************************************************************************
 * @file  i2c/i2c_master_irq/source/main.c
 * @brief Main program of I2C for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao          First version
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
 * @addtogroup I2C_Master_Irq
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief I2c communication mode enum
 */
typedef enum stc_i2c_com_mode
{
    Mode_Send = 0U,
    Mode_Rev = 1U,
}stc_i2c_com_mode_t;


/**
 * @brief I2c communication structure
 */
typedef struct
{
    stc_i2c_com_mode_t enMode;    /*!< I2C communication mode*/
    uint32_t u32Length;           /*!< I2C communication data length*/
    uint8_t* pBuf;                /*!< I2C communication data buffer pointer*/
    uint32_t u32DataIndex;        /*!< I2C communication data transfer index*/
    __IO uint8_t u8FinishFlag;    /*!< I2C communication status*/
}stc_i2c_communication_t;


/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Define slave device address for example */
#define DEVICE_ADDRESS                  (0x06U)

/* Define port and pin for SDA and SCL */
#define I2C_SCL_PORT                    (GPIO_PORT_D)
#define I2C_SCL_PIN                     (GPIO_PIN_03)
#define I2C_SDA_PORT                    (GPIO_PORT_F)
#define I2C_SDA_PIN                     (GPIO_PIN_10)


#define TIMEOUT                         (0x24000U)

#define I2C_RET_OK                      (0U)
#define I2C_RET_ERROR                   (1U)

#define GENERATE_START                  (0x00U)
#define GENERATE_RESTART                (0x01U)

#define ADDRESS_W                       (0x00U)
#define ADDRESS_R                       (0x01U)

/* Define Write and read data length for the example */
#define TEST_DATA_LEN                   (128U)
/* Define i2c baudrate */
#define I2C_BAUDRATE                    (400000UL)

/* I2C interrupt source and number define */
#define I2C_EEI_IRQn                    (Int008_IRQn)
#define I2C_EEI_SOURCE                  (INT_I2C1_EEI)
#define I2C_TXI_IRQn                    (Int020_IRQn)
#define I2C_TXI_SOURCE                  (INT_I2C1_TXI)
#define I2C_RXI_IRQn                    (Int018_IRQn)
#define I2C_RXI_SOURCE                  (INT_I2C1_RXI)
#define I2C_TEI_IRQn                    (Int016_IRQn)
#define I2C_TEI_SOURCE                  (INT_I2C1_TEI)

#define LED_GREEN_PORT                  (GPIO_PORT_C)
#define LED_GREEN_PIN                   (GPIO_PIN_09)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_i2c_communication_t stcI2cCom;

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
    /* Unlock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Unlock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Unlock SRAM register: WTCR */
    SRAM_WTCR_Unlock();
    /* Unlock SRAM register: CKCR */
    SRAM_CKCR_Unlock();
    /* Unlock all EFM registers */
    EFM_Unlock();
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Unlock();
    /* Unlock EFM OTP write protect registers */
    //EFM_OTP_WP_Unlock();
}

/**
 * @brief  MCU Peripheral registers write protected.
 * @param  None
 * @retval None
 * @note Comment/uncomment each API depending on APP requires.
 */
static __attribute__((unused)) void Peripheral_WP(void)
{
    /* Lock GPIO register: PSPCR, PCCR, PINAER, PCRxy, PFSRxy */
    GPIO_Lock();
    /* Lock PWC register: FCG0 */
    PWC_FCG0_Lock();
    /* Lock PWC, CLK, PVD registers, @ref PWC_REG_Write_Unlock_Code for details */
    PWC_Lock(PWC_UNLOCK_CODE_0 | PWC_UNLOCK_CODE_1 | PWC_UNLOCK_CODE_2);
    /* Lock SRAM register: WTCR */
    SRAM_WTCR_Lock();
    /* Lock SRAM register: CKCR */
    SRAM_CKCR_Lock();
    /* Lock EFM OTP write protect registers */
    //EFM_OTP_WP_Lock();
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

static void Master_LedInit(void)
{
    stc_gpio_init_t stcGpioInit;

    /* RGB LED initialize */
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(LED_GREEN_PORT, LED_GREEN_PIN, &stcGpioInit);

    /* "Turn off" LED before set to output */
    GPIO_ResetPins(LED_GREEN_PORT, LED_GREEN_PIN);

    /* Output enable */
    GPIO_OE(LED_GREEN_PORT, LED_GREEN_PIN, Enable);
}

static void Master_LedOn(void)
{
    GPIO_SetPins(LED_GREEN_PORT, LED_GREEN_PIN);
}

/**
 * @brief  Master communication status read.
 * @param  None
 * @retval uint8_t  If return 1 indicate communication finished.
 */
static uint8_t GetComStatus(void)
{
    return stcI2cCom.u8FinishFlag;
}

/**
 * @brief   Send start or restart condition
 * @param   [in]  u8Start  Indicate the start mode, start or restart
 * @retval  Process result
 *          - I2C_RET_ERROR  Send start or restart failed
 *          - I2C_RET_OK     Send start or restart success
 */
static uint8_t Master_StartOrRestart(uint8_t u8Start)
{
    /* generate start or restart signal */
    if(GENERATE_START == u8Start)
    {
        I2C_GenerateStart(M4_I2C1);
    }
    else
    {
        /* Clear start status flag */
        I2C_ClearStatus(M4_I2C1, I2C_CLR_STARTFCLR);
        /* Send restart condition */
        I2C_GenerateReStart(M4_I2C1);
    }

    return I2C_RET_OK;
}

/**
 * @brief   Send slave address
 * @param   [in]  u8Adr  The slave address
 * @retval  Process result
 *          - I2C_RET_ERROR  Send failed
 *          - I2C_RET_OK     Send success
 */
static en_result_t Master_SendAdr(uint8_t u8Adr)
{
    return I2C_SendAddr(M4_I2C1,u8Adr,TIMEOUT);
}

/**
 * @brief  Master send data kick start via interrupt .
 * @param  [in] u32Len  Data length
 * @param  [in] pbuf    Data buffer pointer
 * @retval None
 */
static void MasterSendData(uint32_t u32Len, uint8_t* pbuf)
{
    stcI2cCom.u32DataIndex = 0U;
    stcI2cCom.u8FinishFlag = 0U;
    stcI2cCom.enMode = Mode_Send;
    stcI2cCom.u32Length = u32Len;
    stcI2cCom.pBuf = pbuf;

    /* General start condition */
    Master_StartOrRestart(GENERATE_START);
}

/**
 * @brief  Master Rev data kick start via interrupt .
 * @param  [in] u32Len  Data length
 * @param  [in] pbuf    Data buffer pointer
 * @retval None
 */
static void MasterRevData(uint32_t u32Len, uint8_t* pbuf)
{
    stcI2cCom.u32DataIndex = 0U;
    stcI2cCom.u8FinishFlag = 0U;
    stcI2cCom.enMode = Mode_Rev;
    stcI2cCom.u32Length = u32Len;
    stcI2cCom.pBuf = pbuf;

    /* General start condition */
    Master_StartOrRestart(GENERATE_START);
}

/**
 * @brief   static function for buffer write.
 * @param   [in]   pbuf   Buffer pointer
 * @param   [in]   u8Data the data to be write.
 * @retval  None
 */
static void BufWrite(uint8_t pbuf[], uint8_t u8Data)
{
    if(stcI2cCom.u32DataIndex >= TEST_DATA_LEN)
    {
        //error
        while(1U)
        {
            ;
        }
    }
    pbuf[stcI2cCom.u32DataIndex] = u8Data;
    stcI2cCom.u32DataIndex++;
}

/**
 * @brief   Static function for buffer read.
 * @param   [in]   pbuf   Buffer pointer
 * @retval  uint8_t  The data read out from buffer.
 */
static uint8_t BufRead(uint8_t const pbuf[])
{
    uint8_t temp;
    if(stcI2cCom.u32DataIndex >= TEST_DATA_LEN)
    {
        //error
        while(1U)
        {
            ;
        }
    }
    temp = pbuf[stcI2cCom.u32DataIndex];
    stcI2cCom.u32DataIndex++;

    return temp;
}

/**
 * @brief   I2C EEI(communication error or event) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_EEI_Callback(void)
{
    /* If starf flag valid */
    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_STARTF))
    {
        I2C_ClearStatus(M4_I2C1, I2C_CLR_STARTFCLR);
        if(Mode_Send == stcI2cCom.enMode)
        {
            Master_SendAdr(((uint8_t)DEVICE_ADDRESS<<1U) | ADDRESS_W);
        }
        else
        {
            Master_SendAdr(((uint8_t)DEVICE_ADDRESS<<1U) | ADDRESS_R);
        }
    }

    /* If address flag valid */
    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_SLADDR0F))
    {
        I2C_ClearStatus(M4_I2C1, I2C_CLR_SLADDR0FCLR);
        /* Enable Tx or Rx function*/
        if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_TRA))
        {
            /* Config tx buffer empty interrupt function*/
            I2C_IntCmd(M4_I2C1, I2C_CR2_TEMPTYIE, Enable);
            /* Write the first data to DTR immediately */
            I2C_WriteDataReg(M4_I2C1, BufRead(stcI2cCom.pBuf));
        }
        else
        {
            /* Config rx buffer full interrupt function*/
            I2C_IntCmd(M4_I2C1, I2C_CR2_RFULLIE, Enable);
        }

        /* Enable NACK interrupt */
        I2C_IntCmd(M4_I2C1, I2C_CR2_STOPIE | I2C_CR2_NACKIE, Enable);
    }

    /* If NACK interrupt occurred */
    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_NACKF))
    {
        /* clear NACK flag*/
        I2C_ClearStatus(M4_I2C1, I2C_CLR_NACKFCLR);
        /* Stop tx or rx process*/
        if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_TRA))
        {
            /* Config tx buffer empty interrupt function disable*/
            I2C_IntCmd(M4_I2C1, I2C_CR2_TEMPTYIE, Disable);
            /* Read DRR register to release SCL*/
            I2C_ReadDataReg(M4_I2C1);
        }
        else
        {
            /* Config rx buffer full interrupt function disable */
            I2C_IntCmd(M4_I2C1, I2C_CR2_RFULLIE, Disable);
        }
    }

    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_STOPF))
    {
        /* Communication finished */
        stcI2cCom.u8FinishFlag = 1U;
        /* Disable Stop flag interrupt */
        I2C_IntCmd(M4_I2C1, I2C_CR2_STOPIE, Disable);
        I2C_ClearStatus(M4_I2C1, I2C_CLR_STOPFCLR);
    }
}

/**
 * @brief   I2C TXI(transfer buffer empty) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_TXI_Callback(void)
{
    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_TEMPTYF))
    {
        if(stcI2cCom.u32DataIndex < TEST_DATA_LEN)
        {
            I2C_WriteDataReg(M4_I2C1, BufRead(stcI2cCom.pBuf));
        }
        else
        {
            I2C_IntCmd(M4_I2C1, I2C_CR2_TEMPTYIE, Disable);
            I2C_IntCmd(M4_I2C1, I2C_CR2_TENDIE, Enable);
        }
    }
}

/**
 * @brief   I2C RXI(receive buffer full) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_RXI_Callback(void)
{
    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_RFULLF))
    {
        BufWrite(stcI2cCom.pBuf, I2C_ReadDataReg(M4_I2C1));
    }

    if(stcI2cCom.u32DataIndex >= TEST_DATA_LEN)
    {
        /* Generate stop condition */
        I2C_ClearStatus(M4_I2C1, I2C_CLR_STOPFCLR);
        I2C_GenerateStop(M4_I2C1);
        /* Enable Stop flag interrupt */
        I2C_IntCmd(M4_I2C1, I2C_CR2_STOPIE, Enable);
        /* Disable TXI interrupt */
        I2C_IntCmd(M4_I2C1, I2C_CR2_RFULLIE, Disable);
    }
}

/**
 * @brief   I2C TEI(Transfer end) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_TEI_Callback(void)
{
    if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_TENDF))
    {
        /* Generate stop condition */
        I2C_ClearStatus(M4_I2C1, I2C_CLR_STOPFCLR);
        I2C_GenerateStop(M4_I2C1);
        /* Enable Stop flag interrupt */
        I2C_IntCmd(M4_I2C1, I2C_CR2_STOPIE, Enable);
        /* Disable TXI interrupt */
        I2C_IntCmd(M4_I2C1, I2C_CR2_TEMPTYIE|I2C_CR2_TENDIE, Disable);
    }
}

/**
 * @brief   Initialize the I2C peripheral for master
 * @param   None
 * @retval  Process result
 *          - I2C_RET_ERROR  Initialize failed
 *          - I2C_RET_OK     Initialize success
 */
static uint8_t Master_Initialize(void)
{
    stc_i2c_init_t stcI2cInit;
    stc_irq_signin_config_t stcIrqSignCfg;
    float32_t fErr;

    I2C_DeInit(M4_I2C1);

    I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32Baudrate = I2C_BAUDRATE;
    stcI2cInit.u32SclTime = 5U;
    stcI2cInit.u32I2cClkDiv = I2C_CLK_DIV1;
    I2C_Init(M4_I2C1, &stcI2cInit, &fErr);

    /* Set slave address*/
#ifdef I2C_10BITS_ADDRESS
    I2C_SlaveAddrConfig(M4_I2C1, I2C_ADDR_0, I2C_ADDR_MODE_10BIT, DEVICE_ADDRESS);
#else
    I2C_SlaveAddrConfig(M4_I2C1, I2C_ADDR_0, I2C_ADDR_MODE_7BIT, DEVICE_ADDRESS);
#endif

    /* Register IRQ handler && configure NVIC. */
    stcIrqSignCfg.enIRQn = I2C_EEI_IRQn;
    stcIrqSignCfg.enIntSrc = I2C_EEI_SOURCE;
    stcIrqSignCfg.pfnCallback = &I2C_EEI_Callback;
    INTC_IrqSignIn(&stcIrqSignCfg);
    NVIC_ClearPendingIRQ(stcIrqSignCfg.enIRQn);
    NVIC_SetPriority(stcIrqSignCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqSignCfg.enIRQn);

    stcIrqSignCfg.enIRQn = I2C_RXI_IRQn;
    stcIrqSignCfg.enIntSrc = I2C_RXI_SOURCE;
    stcIrqSignCfg.pfnCallback = &I2C_RXI_Callback;
    INTC_IrqSignIn(&stcIrqSignCfg);
    NVIC_ClearPendingIRQ(stcIrqSignCfg.enIRQn);
    NVIC_SetPriority(stcIrqSignCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqSignCfg.enIRQn);

    stcIrqSignCfg.enIRQn = I2C_TXI_IRQn;
    stcIrqSignCfg.enIntSrc = I2C_TXI_SOURCE;
    stcIrqSignCfg.pfnCallback = &I2C_TXI_Callback;
    INTC_IrqSignIn(&stcIrqSignCfg);
    NVIC_ClearPendingIRQ(stcIrqSignCfg.enIRQn);
    NVIC_SetPriority(stcIrqSignCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqSignCfg.enIRQn);

    stcIrqSignCfg.enIRQn = I2C_TEI_IRQn;
    stcIrqSignCfg.enIntSrc = I2C_TEI_SOURCE;
    stcIrqSignCfg.pfnCallback = &I2C_TEI_Callback;
    INTC_IrqSignIn(&stcIrqSignCfg);
    NVIC_ClearPendingIRQ(stcIrqSignCfg.enIRQn);
    NVIC_SetPriority(stcIrqSignCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqSignCfg.enIRQn);

    /* I2C function command */
    I2C_Cmd(M4_I2C1, Enable);

    /* Config startf and slave address match interrupt function*/
    I2C_IntCmd(M4_I2C1, I2C_CR2_STARTIE | I2C_CR2_SLADDR0IE, Enable);

    return I2C_RET_OK;
}

/**
 * @brief  Main function of i2c_master_irq project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    Peripheral_WE();

    /* Configure system clock. */
    BSP_CLK_Init();

    Master_LedInit();

    static uint8_t u8TxBuf[TEST_DATA_LEN];
    static uint8_t u8RxBuf[TEST_DATA_LEN] = {0U};
    uint32_t i;
    /* Test buffer initialize */
    for(i=0U; i<TEST_DATA_LEN; i++)
    {
        u8TxBuf[i] = (uint8_t)i+1U;
    }

    /* Initialize I2C port*/
    stc_gpio_init_t stcGpioInit;
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(I2C_SCL_PORT, I2C_SCL_PIN, &stcGpioInit);
    GPIO_Init(I2C_SDA_PORT, I2C_SDA_PIN, &stcGpioInit);
    GPIO_SetFunc(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_FUNC_49_I2C1_SCL, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_FUNC_48_I2C1_SDA, PIN_SUBFUNC_DISABLE);

    /* Enable peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_IIC1, Enable);
    /* Initialize I2C peripheral and enable function*/
    Master_Initialize();
    /* I2C master data write */
    MasterSendData(TEST_DATA_LEN, u8TxBuf);

    i = 0UL;
    /* Wait communication finished*/
    while(0U == GetComStatus())
    {
        if(TIMEOUT == i++)
        {
            /* Communication time out*/
            while(1U)
            {
                DDL_DelayMS(500U);
            }
        }
    }
    /* 5mS delay for device*/
    DDL_DelayMS(5U);
    /* I2C master data Read */
    MasterRevData(TEST_DATA_LEN, u8RxBuf);

    i = 0UL;
    /* Wait communication finished*/
    while(0U == GetComStatus())
    {
        if(TIMEOUT == i++)
        {
            /* Communication time out*/
            while(1U)
            {
                DDL_DelayMS(500U);
            }
        }
    }

    /* Compare the data */
    for(i=0U; i<TEST_DATA_LEN; i++)
    {
        if(u8TxBuf[i] != u8RxBuf[i])
        {
            /* Data write error*/
            while(1U)
            {
                DDL_DelayMS(500U);
            }
        }
    }

    /* Communication finished */
    Master_LedOn();
    while(1U)
    {
        DDL_DelayMS(500U);
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
