/**
 *******************************************************************************
 * @file  i2c/i2c_slave_polling/source/main.c
 * @brief Main program of I2C slave polling for the Device Driver Library.
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
 * @addtogroup I2C_Slave_Polling
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

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

#define TIMEOUT                         (0x14000U)

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
    /* Lock EFM register: FWMC */
    //EFM_FWMC_Lock();
    /* Lock all EFM registers */
    EFM_Lock();
}

static void Slave_LedInit(void)
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

static void Slave_LedOn(void)
{
    GPIO_SetPins(LED_GREEN_PORT, LED_GREEN_PIN);
}

/**
 * @brief   Slave send data to master
 * @param   [in]  pTxData    The data buffer to be send.
 * @param   [in]  u32Size    The data length to be send.
 * @retval  Process result
 *          - I2C_RET_ERROR  Write data failed
 *          - I2C_RET_OK     Write data success
 */
static uint8_t Slave_WriteData(uint8_t *pTxData, uint32_t u32Size)
{
    uint8_t Ret = I2C_RET_OK;
    if(I2C_SendData(M4_I2C1, pTxData, u32Size, TIMEOUT) != Ok)
    {
        Ret = I2C_RET_ERROR;
    }
    /* Wait stop condition */
    uint32_t u32TimeOut = TIMEOUT;
    while(Reset == I2C_GetStatus(M4_I2C1, I2C_SR_STOPF))
    {
        /* Release SCL pin */
        I2C_ReadDataReg(M4_I2C1);
        if(0U == (u32TimeOut--))
        {
            Ret = I2C_RET_ERROR;
            break;
        }
    }
    return Ret;
}

/**
 * @brief   Receive the data until stop condition received
 * @param   [in]  u8RxData   The receive buffer pointer.
 * @retval  Process result
 *          - I2C_RET_ERROR  Receive failed
 *          - I2C_RET_OK     Receive success
 */
static uint8_t Slave_RevData(uint8_t u8RxData[])
{
    uint8_t i = 0U;

    while(1)
    {
        /* Wait for the Rx full flag set */
        if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_RFULLF))
        {
            /* Read the data from buffer */
            u8RxData[i++] = I2C_ReadDataReg(M4_I2C1);
        }

        /* Detect the stop signal on the bus */
        if(Set == I2C_GetStatus(M4_I2C1, I2C_SR_STOPF))
        {
            I2C_ClearStatus(M4_I2C1, I2C_SR_STOPF);
            break;
        }
    }
    return I2C_RET_OK;
}

/**
 * @brief   Initialize the I2C peripheral for slave
 * @param   None
 * @retval  Process result
 *          - I2C_RET_ERROR  Initialize failed
 *          - I2C_RET_OK     Initialize success
 */
static uint8_t Slave_Initialize(void)
{
    stc_i2c_init_t stcI2cInit;
    float32_t fErr;

    I2C_DeInit(M4_I2C1);

    I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32Baudrate = I2C_BAUDRATE;
    stcI2cInit.u32I2cClkDiv = I2C_CLK_DIV1;
    stcI2cInit.u32SclTime = 5U;
    I2C_Init(M4_I2C1, &stcI2cInit, &fErr);

    I2C_Cmd(M4_I2C1, Enable);

    /* Set slave address*/
#ifdef I2C_10BITS_ADDRESS
    I2C_SlaveAddrConfig(M4_I2C1, I2C_ADDR_0, I2C_ADDR_MODE_10BIT, DEVICE_ADDRESS);
#else
    I2C_SlaveAddrConfig(M4_I2C1, I2C_ADDR_0, I2C_ADDR_MODE_7BIT, DEVICE_ADDRESS);
#endif

    return I2C_RET_OK;
}

/**
 * @brief  Main function of i2c_slave_polling project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    Peripheral_WE();

    /* Configure system clock. */
    BSP_CLK_Init();
    Slave_LedInit();

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
    Slave_Initialize();
    uint8_t u8RxBuf[TEST_DATA_LEN] = {0};
    while(1)
    {
        /* Wait slave address matched*/
        while(Reset == I2C_GetStatus(M4_I2C1, I2C_SR_SLADDR0F))
        {
            ;
        }
        I2C_ClearStatus(M4_I2C1, I2C_CLR_SLADDR0FCLR);

        if(Reset == I2C_GetStatus(M4_I2C1, I2C_SR_TRA))
        {
            /* Slave receive data*/
            Slave_RevData(u8RxBuf);
        }
        else
        {
            /* Slave send data*/
            Slave_WriteData(u8RxBuf, TEST_DATA_LEN);
            break;
        }
    }

    /* Communication finished */
    Slave_LedOn();
    while(1)
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
