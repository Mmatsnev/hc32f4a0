/**
 *******************************************************************************
 * @file  i2c/i2c_at24c02_disable_fack/source/main.c
 * @brief Main program of I2C at24c02 disable fack for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao         First version
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
 * @addtogroup I2c_At24c02_Disable_Fack
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Define I2C unit used for the example */
#define I2C_CH                          (M4_I2C1)
/* Define E2PROM device address */
#define E2_ADDRESS                      0x50U
/* AT24C02 page length is 8byte*/
#define PAGE_LEN                        8U
/* Define test address for read and write */
#define DATA_TEST_ADDR                  (0x00U)
/* Define port and pin for SDA and SCL */
#define I2C1_SCL_PORT                   (GPIO_PORT_D)
#define I2C1_SCL_PIN                    (GPIO_PIN_03)
#define I2C1_SDA_PORT                   (GPIO_PORT_F)
#define I2C1_SDA_PIN                    (GPIO_PIN_10)
/* Define i2c baudrate */
#define I2C_BAUDRATE                    400000UL

#define TIMEOUT                         0x15000U

#define I2C_RET_OK                      0U
#define I2C_RET_ERROR                   1U

#define GENERATE_START                  0x00U
#define GENERATE_RESTART                0x01U

#define E2_ADDRESS_W                    0x00U
#define E2_ADDRESS_R                    0x01U

/**
 * Wait time for eeprom.
 * specification is 5ms, but you may enlarg it according to compilation options
 */
#define APP_WAIT_EEPROM                 (5UL)

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
    /* Unlock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

static void Led_Init(void)
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

static void Led_On(void)
{
    GPIO_SetPins(LED_GREEN_PORT, LED_GREEN_PIN);
}

/**
 * @brief  Send start or restart condition.
 * @param  [in] u8Start specify start or restart flag to be generated
 *         This parameter can be one of the following values:
 *         @arg GENERATE_START
 *         @arg GENERATE_RESTART
 * @retval Process result
 *         - Error  Send failed
 *         - Ok     Send success
 */
static uint8_t E2_StartOrRestart(uint8_t u8Start)
{
    en_result_t enRet = Ok;

    /* generate start or restart signal */
    if(GENERATE_START == u8Start)
    {
        enRet = I2C_Start(M4_I2C1,TIMEOUT);
    }
    else
    {
        /* Clear start status flag */
        enRet = I2C_Restart(M4_I2C1,TIMEOUT);

    }

    return enRet;
}

/**
 * @brief  Send e2prom device address.
 * @param  [in] u8Adr The slave address
 * @retval Process result
 *         - ErrorTimeout  Send failed
 *         - Ok            Send success
 */
static uint8_t E2_SendAdr(uint8_t u8Adr)
{
    return I2C_SendAddr(M4_I2C1,u8Adr,TIMEOUT);
}

/**
 * @brief  Send data address to e2prom
 * @param  [in] u8DataAdr Data address to be send
 * @retval Process result
 *         - ErrorTimeout  Send failed
 *         - Ok            Send success
 */
static uint8_t E2_SendDataAdr(uint8_t u8DataAdr)
{
    return I2C_SendData(M4_I2C1, &u8DataAdr, 1U, TIMEOUT);
}

/**
 * @brief  Send data to e2prom
 * @param  [in] pTxData Pointer to the data buffer
 * @param  [in] u32Size Data size
 * @retval Process result
 *         - ErrorTimeout  Send failed
 *         - Ok            Send success
 */
static uint8_t E2_WriteData(uint8_t *pTxData, uint32_t u32Size)
{
    return I2C_SendData(M4_I2C1, pTxData, u32Size, TIMEOUT);
}

/**
 * @brief  Receive data from e2prom
 * @param  [in] pRxData Pointer to the data buffer
 * @param  [in] u32Size Data size
 * @retval Process result
 *         - ErrorTimeout  Receive failed
 *         - Ok            Receive success
 */
static uint8_t E2_RevData(uint8_t *pRxData, uint32_t u32Size)
{
    return I2C_RcvData(M4_I2C1,pRxData, u32Size,TIMEOUT);
}

/**
 * @brief  General stop condition to e2prom
 * @param  None
 * @retval Process result
 *         - ErrorTimeout  stop failed
 *         - Ok            stop success
 */
uint8_t E2_Stop(void)
{
    return I2C_Stop(M4_I2C1,TIMEOUT);
}

/**
 * @brief  Initialize the I2C peripheral for e2prom
 * @param  None
 * @retval Process result
 *         - I2C_RET_ERROR   failed
 *         - I2C_RET_OK   success
 */
uint8_t E2_Initialize(void)
{
    stc_i2c_init_t stcI2cInit;
    float32_t fErr;

    I2C_DeInit(M4_I2C1);

    I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32Baudrate = I2C_BAUDRATE;
    stcI2cInit.u32SclTime = 5U;
    stcI2cInit.u32I2cClkDiv = I2C_CLK_DIV1;
    I2C_Init(M4_I2C1, &stcI2cInit, &fErr);

    I2C_Cmd(M4_I2C1, Enable);
    I2C_FastAckCmd(M4_I2C1, Disable);
    return I2C_RET_OK;
}

/**
 * @brief  Judge the result.
 * @param  [in] u8Result The result value to be judge
 * @retval None
 */
static void JudgeResult(uint8_t u8Result)
{
    if(Ok != u8Result)
    {
        while(1)
        {
            DDL_DelayMS(500UL);
        }
    }
}

/**
 * @brief  Main function of i2c_at24c02_disable_fack project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    Peripheral_WE();

    uint8_t u8TxBuf[PAGE_LEN];
    uint8_t u8RxBuf[PAGE_LEN] = {0U};
    uint32_t i;
    uint8_t u8Ret = I2C_RET_OK;

    /* Configure system clock. */
    BSP_CLK_Init();
    Led_Init();

    for(i=0UL; i<PAGE_LEN; i++)
    {
        u8TxBuf[i] = (uint8_t)i+1U;
    }

    /* Initialize I2C port*/
    stc_gpio_init_t stcGpioInit;
    GPIO_StructInit(&stcGpioInit);
    GPIO_Init(I2C1_SCL_PORT, I2C1_SCL_PIN, &stcGpioInit);
    GPIO_Init(I2C1_SDA_PORT, I2C1_SDA_PIN, &stcGpioInit);
    GPIO_SetFunc(I2C1_SCL_PORT, I2C1_SCL_PIN, GPIO_FUNC_49_I2C1_SCL, PIN_SUBFUNC_DISABLE);
    GPIO_SetFunc(I2C1_SDA_PORT, I2C1_SDA_PIN, GPIO_FUNC_48_I2C1_SDA, PIN_SUBFUNC_DISABLE);

    /* Enable peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_IIC1, Enable);
    /* Initialize I2C peripheral and enable function*/
    E2_Initialize();

    /* E2prom byte write*/
    u8Ret = E2_StartOrRestart(GENERATE_START);
    JudgeResult(u8Ret);
    u8Ret = E2_SendAdr((uint8_t)(E2_ADDRESS<<1U)|E2_ADDRESS_W);
    JudgeResult(u8Ret);
    u8Ret = E2_SendDataAdr(DATA_TEST_ADDR);
    JudgeResult(u8Ret);
    u8Ret = E2_WriteData(u8TxBuf, 1U);
    JudgeResult(u8Ret);
    u8Ret = E2_Stop();
    JudgeResult(u8Ret);

    /* 5mS delay for e2prom*/
    DDL_DelayMS(APP_WAIT_EEPROM);

    /* E2prom random read*/
    u8Ret = E2_StartOrRestart(GENERATE_START);
    JudgeResult(u8Ret);
    u8Ret = E2_SendAdr((uint8_t)(E2_ADDRESS<<1U)|E2_ADDRESS_W);
    JudgeResult(u8Ret);
    u8Ret = E2_SendDataAdr(DATA_TEST_ADDR);
    JudgeResult(u8Ret);

    u8Ret = E2_StartOrRestart(GENERATE_RESTART);
    JudgeResult(u8Ret);
    u8Ret = E2_SendAdr((uint8_t)(E2_ADDRESS<<1U)|E2_ADDRESS_R);
    JudgeResult(u8Ret);
    u8Ret = E2_RevData(u8RxBuf, 1U);
    JudgeResult(u8Ret);
    u8Ret = E2_Stop();
    JudgeResult(u8Ret);

    /* Compare the data */
    if(0x01U != u8RxBuf[0])
    {
        /* e2prom byte write error*/
        while(1)
        {
            DDL_DelayMS(500UL);
        }
    }

    /* 5mS delay for e2prom*/
    DDL_DelayMS(APP_WAIT_EEPROM);
    /* E2prom page write*/
    u8Ret = E2_StartOrRestart(GENERATE_START);
    JudgeResult(u8Ret);
    u8Ret = E2_SendAdr((uint8_t)(E2_ADDRESS<<1U)|E2_ADDRESS_W);
    JudgeResult(u8Ret);
    u8Ret = E2_SendDataAdr(DATA_TEST_ADDR);
    JudgeResult(u8Ret);
    u8Ret = E2_WriteData(u8TxBuf, PAGE_LEN);
    JudgeResult(u8Ret);
    u8Ret = E2_Stop();
    JudgeResult(u8Ret);

    /* 5mS delay for e2prom*/
    DDL_DelayMS(APP_WAIT_EEPROM);

    /* E2prom sequential read*/
    u8Ret = E2_StartOrRestart(GENERATE_START);
    JudgeResult(u8Ret);
    u8Ret = E2_SendAdr((uint8_t)(E2_ADDRESS<<1U)|E2_ADDRESS_W);
    JudgeResult(u8Ret);
    u8Ret = E2_SendDataAdr(DATA_TEST_ADDR);
    JudgeResult(u8Ret);

    u8Ret = E2_StartOrRestart(GENERATE_RESTART);
    JudgeResult(u8Ret);
    u8Ret = E2_SendAdr((uint8_t)(E2_ADDRESS<<1U)|E2_ADDRESS_R);
    JudgeResult(u8Ret);
    u8Ret = E2_RevData(u8RxBuf, PAGE_LEN);
    JudgeResult(u8Ret);
    u8Ret = E2_Stop();
    JudgeResult(u8Ret);

    /* Compare the data */
    for(i=0UL; i<PAGE_LEN; i++)
    {
        if(u8TxBuf[i] != u8RxBuf[i])
        {
            /* e2prom page write error*/
            while(1)
            {
                DDL_DelayMS(500UL);
            }
        }
    }

    /* e2prom sample success*/
    Led_On();
    while(1)
    {
        DDL_DelayMS(500UL);
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
