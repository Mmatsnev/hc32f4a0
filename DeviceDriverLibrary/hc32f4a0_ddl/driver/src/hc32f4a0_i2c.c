/**
 *******************************************************************************
 * @file  hc32f4a0_i2c.c
 * @brief This file provides firmware functions to manage the Inter-Integrated
 *        Circuit(I2C).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao          First version
   2020-07-15       Hexiao          Modify I2C_SmBusCmd to I2C_SetMode
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
#include "hc32f4a0_i2c.h"
#include "hc32f4a0_utility.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_I2C I2C
 * @brief I2C Driver Library
 * @{
 */

#if (DDL_I2C_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup I2C_Local_Macros I2C Local Macros
 * @{
 */

#define I2C_CLR_MASK                       (0x00F052DFUL)
#define I2C_INT_MASK                       (0x00F052DFUL)
#define I2C_SCL_HIGHT_LOW_LVL_SUM_MAX      ((float32_t)0x1F*(float32_t)2)


/**
 * @defgroup I2C_Check_Parameters_Validity I2C Check Parameters Validity
 * @{
 */
#define IS_VALID_UNIT(x)                                                       \
(   ((x) == M4_I2C1)                               ||                          \
    ((x) == M4_I2C2)                               ||                          \
    ((x) == M4_I2C3)                               ||                          \
    ((x) == M4_I2C4)                               ||                          \
    ((x) == M4_I2C5)                               ||                          \
    ((x) == M4_I2C6))

#define IS_VALID_MODE(x)                                                       \
(   ((x) == I2C_MODE_I2C)                          ||                          \
    ((x) == I2C_MODE_SMBUS))

#define IS_VALID_CLEARBIT(x)       ((0U != (x)) && (0U == ((x) & (~I2C_CLR_MASK))))

#define IS_VALID_INT(x)            ((0U != (x)) && (0U == ((x) & (~I2C_INT_MASK))))

#define IS_VALID_SPEED(speed)      ((0U != (speed)) && ((speed) <= (I2C_BAUDRATE_MAX)))

#define IS_VALID_DIGITAL_FILTER(x) ((x) <= I2C_DIG_FILTMODE_4CYCLE)

#define IS_VALID_RD_STATUS_BIT(x)                                              \
(   ((x) == I2C_SR_STARTF)                         ||                          \
    ((x) == I2C_SR_SLADDR0F)                       ||                          \
    ((x) == I2C_SR_SLADDR1F)                       ||                          \
    ((x) == I2C_SR_TENDF)                          ||                          \
    ((x) == I2C_SR_STOPF)                          ||                          \
    ((x) == I2C_SR_RFULLF)                         ||                          \
    ((x) == I2C_SR_TEMPTYF)                        ||                          \
    ((x) == I2C_SR_ARLOF)                          ||                          \
    ((x) == I2C_SR_ACKRF)                          ||                          \
    ((x) == I2C_SR_NACKF)                          ||                          \
    ((x) == I2C_SR_TMOUTF)                         ||                          \
    ((x) == I2C_SR_MSL)                            ||                          \
    ((x) == I2C_SR_BUSY)                           ||                          \
    ((x) == I2C_SR_TRA)                            ||                          \
    ((x) == I2C_SR_GENCALLF)                       ||                          \
    ((x) == I2C_SR_SMBDEFAULTF)                    ||                          \
    ((x) == I2C_SR_SMBHOSTF)                       ||                          \
    ((x) == I2C_SR_SMBALRTF))

#define IS_VALID_SMBUS_CONFIG(x)      ((0U != (x)) &&                           \
                                      (0U == ((x) & (~(uint32_t)I2C_SMBUS_CONFIG_CLEARMASK))))

#define IS_VALID_ADDR(mode,addr)      (((I2C_ADDR_MODE_7BIT == (mode)) && ((addr) <= 0x7FU)) ||\
                                      ((I2C_ADDR_MODE_10BIT == (mode)) && ((addr) <= 0x3FFU)) ||\
                                      (I2C_ADDR_MODE_DISABLE == (mode)))


#define IS_VALID_ADDR_NUM(x)                                                   \
(   ((x) == I2C_ADDR_0)                            ||                          \
    ((x) == I2C_ADDR_1))

#define IS_VALID_CLK_DIV(x)                                                    \
(   ((x) == I2C_CLK_DIV1)                          ||                          \
    ((x) == I2C_CLK_DIV2)                          ||                          \
    ((x) == I2C_CLK_DIV4)                          ||                          \
    ((x) == I2C_CLK_DIV8)                          ||                          \
    ((x) == I2C_CLK_DIV16)                         ||                          \
    ((x) == I2C_CLK_DIV32)                         ||                          \
    ((x) == I2C_CLK_DIV64)                         ||                          \
    ((x) == I2C_CLK_DIV128))

#define IS_VALID_MASTER_SLAVE_MODE(x)                                          \
(   ((x) == I2C_MASTER_SLAVE_MODE_MASTER)          ||                          \
    ((x) == I2C_MASTER_SLAVE_MODE_SLAVE))

#define IS_VALID_TX_RX_MODE(x)                                                 \
(   ((x) == I2C_TX_RX_MODE_TX)                     ||                          \
    ((x) == I2C_TX_RX_MODE_RX))
/**
 * @}
 */

/**
 * @}
 */

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
 * @defgroup I2C_Global_Functions I2C Global Functions
 * @{
 */

/**
 * @brief  Set the baudrate for I2C peripheral.
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] pstcI2C_InitStruct   Pointer to I2C configuration structure
 *                                   @ref stc_i2c_init_t
 *         @arg pstcI2C_InitStruct->u32I2cClkDiv: Division of Pclk3, reference as:
 *              step1: calculate div = (Pclk3/Baudrate/(68+2*dnfsum+SclTime)
 *                     Pclk3 -- system clock
 *                     Baudrate -- baudrate of i2c
 *                     SclTime -- =(SCL rising time + SCL falling time)/period of i2c clock
 *                                according to i2c bus hardware parameter.
 *                     dnfsum -- 0 if digital filter off;
 *                               Filter capacity if digital filter on(1 ~ 4)
 *              step2: chose a division item which is similar and bigger than div
 *                     from @ref I2C_Clock_division.
 *         @arg pstcI2C_InitStruct->u32Baudrate : Baudrate configuration
 *         @arg pstcI2C_InitStruct->u32SclTime : Indicate SCL pin rising and
 *              falling time, should be number of T(i2c clock period time)
 * @param  [out] pf32Err               Baudrate error
 * @retval en_result_t
 */
en_result_t I2C_BaudrateConfig(M4_I2C_TypeDef* I2Cx, const stc_i2c_init_t *pstcI2C_InitStruct, float32_t *pf32Err)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    en_result_t enRet = Ok;
    uint32_t u32Pclk3;
    uint32_t I2cDivClk;
    uint32_t SclCnt;
    uint32_t Baudrate;
    uint32_t dnfsum = 0UL;
    uint32_t divsum = 2UL;
    float32_t WidthTotal;
    float32_t SumTotal;
    float32_t WidthHL;
    float32_t fErr = 0.0F;

    if ((NULL == pstcI2C_InitStruct) || (NULL == pf32Err))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SPEED(pstcI2C_InitStruct->u32Baudrate));
        DDL_ASSERT(IS_VALID_CLK_DIV(pstcI2C_InitStruct->u32I2cClkDiv));

        /* Get configuration for i2c */
        u32Pclk3 = SystemCoreClock >> ((M4_CMU->SCFGR & CMU_SCFGR_PCLK3S) >> CMU_SCFGR_PCLK3S_POS);
        I2cDivClk = 1UL << pstcI2C_InitStruct->u32I2cClkDiv;
        SclCnt = pstcI2C_InitStruct->u32SclTime;
        Baudrate = pstcI2C_InitStruct->u32Baudrate;

        /* Judge digital filter status*/
        if(0U != READ_REG32_BIT(I2Cx->FLTR, I2C_FLTR_DNFEN))
        {
            dnfsum = (READ_REG32_BIT(I2Cx->FLTR, I2C_FLTR_DNF) >> I2C_FLTR_DNF_POS) + 1U;
        }

        /* Judge if clock divider on*/
        if(I2C_CLK_DIV1 == I2cDivClk)
        {
            divsum = 3UL;
        }

        WidthTotal = (float32_t)u32Pclk3 / (float32_t)Baudrate / (float32_t)I2cDivClk;
        SumTotal = 2.0F*(float32_t)divsum + 2.0F*(float32_t)dnfsum + (float32_t)SclCnt;
        WidthHL = WidthTotal - SumTotal;

        if(WidthTotal <= SumTotal)
        {
            /* Err, Should set a smaller division value for pstcI2C_InitStruct->u32I2cClkDiv */
            enRet = ErrorInvalidParameter;
        }
        else if(WidthHL > I2C_SCL_HIGHT_LOW_LVL_SUM_MAX)
        {
            /* Err, Should set a bigger division value for pstcI2C_InitStruct->u32I2cClkDiv */
            enRet = ErrorInvalidParameter;
        }
        else
        {
            fErr =(WidthHL - (float32_t)((uint32_t)WidthHL)) / WidthHL;
            WRITE_REG32(I2Cx->CCR, (pstcI2C_InitStruct->u32I2cClkDiv << I2C_CCR_FREQ_POS)     \
                           | (((uint32_t)WidthHL/2U) << I2C_CCR_SLOWW_POS)              \
                           | (((uint32_t)WidthHL - (uint32_t)WidthHL/2U) << I2C_CCR_SHIGHW_POS));
        }
    }

    if((NULL != pf32Err)&&(Ok == enRet))
    {
        *pf32Err = fErr;
    }

    return enRet;
}

/**
 * @brief  De-initialize I2C unit
 * @param  [in] I2Cx   Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @retval None
 */
void I2C_DeInit(M4_I2C_TypeDef* I2Cx)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    /* Reset peripheral register and internal status*/
    CLEAR_REG32_BIT(I2Cx->CR1,I2C_CR1_PE);
    SET_REG32_BIT(I2Cx->CR1,I2C_CR1_SWRST);
}

/**
 * @brief  Initialize structure stc_i2c_init_t variable with default value.
 * @param  [out] pstcI2C_InitStruct     Pointer to a stc_i2c_init_t structure variable
 *                                      which will be initialized.
 *   @arg  See the struct @ref stc_i2c_init_t.
 * @retval en_result_t
 */
en_result_t I2C_StructInit(stc_i2c_init_t* pstcI2C_InitStruct)
{
    en_result_t enRet = Ok;

    if (pstcI2C_InitStruct == NULL)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        pstcI2C_InitStruct->u32Baudrate = 50000UL;
        pstcI2C_InitStruct->u32SclTime = 0UL;
    }

    return enRet;
}

/**
 * @brief  Initialize I2C peripheral according to the structure
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] pstcI2C_InitStruct   Pointer to I2C configuration structure
 *                                   @ref stc_i2c_init_t
 *         @arg pstcI2C_InitStruct->u32I2cClkDiv: Division of Pclk3, reference as:
 *              step1: calculate div = (Pclk3/Baudrate/(68+2*dnfsum+SclTime)
 *                     Pclk3 -- system clock
 *                     Baudrate -- baudrate of i2c
 *                     SclTime -- =(SCL rising time + SCL falling time)/period of i2c clock
 *                                according to i2c bus hardware parameter.
 *                     dnfsum -- 0 if digital filter off;
 *                               Filter capacity if digital filter on(1 ~ 4)
 *              step2: chose a division item which is similar and bigger than div
 *                     from @ref I2C_Clock_division.
 *         @arg pstcI2C_InitStruct->u32Baudrate : Baudrate configuration
 *         @arg pstcI2C_InitStruct->u32SclTime : Indicate SCL pin rising and
 *              falling time, should be number of T(i2c clock period time)
 * @param  [out] pf32Err               Baudrate error
 * @retval en_result_t
 */
en_result_t I2C_Init(M4_I2C_TypeDef* I2Cx, const stc_i2c_init_t *pstcI2C_InitStruct, float32_t *pf32Err)
{
    en_result_t enRet = Ok;
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    if (NULL == pstcI2C_InitStruct )
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        DDL_ASSERT(IS_VALID_SPEED(pstcI2C_InitStruct->u32Baudrate));
        DDL_ASSERT(IS_VALID_CLK_DIV(pstcI2C_InitStruct->u32I2cClkDiv));

        /* Register and internal status reset */
        CLEAR_REG32_BIT(I2Cx->CR1,I2C_CR1_PE);
        SET_REG32_BIT(I2Cx->CR1,I2C_CR1_SWRST);
        SET_REG32_BIT(I2Cx->CR1,I2C_CR1_PE);

        /* I2C baudrate config */
        I2C_BaudrateConfig(I2Cx, pstcI2C_InitStruct, pf32Err);

        /* Disable global broadcast address function */
        CLEAR_REG32_BIT(I2Cx->CR1,I2C_CR1_ENGC);

        /* Release software reset */
        CLEAR_REG32_BIT(I2Cx->CR1,I2C_CR1_SWRST);
        /* Disable I2C peripheral */
        CLEAR_REG32_BIT(I2Cx->CR1,I2C_CR1_PE);
    }

    return enRet;
}

/**
 * @brief  I2C slave address config
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32AddrNum            I2C address 0 or address 1 @ref I2C_Address_Num
 * @param  [in] u32AddrMode           Address mode configuration,@ref  I2C_Addr_Config
 *                                    This parameter can be one of the following values:
 *                                    @arg I2C_ADDR_MODE_DISABLE
 *                                    @arg I2C_ADDR_MODE_7BIT
 *                                    @arg I2C_ADDR_MODE_10BIT
 * @param  [in] u32Addr               The slave address
 * @retval None
 */
void I2C_SlaveAddrConfig(M4_I2C_TypeDef* I2Cx, uint32_t u32AddrNum, uint32_t u32AddrMode, uint32_t u32Addr)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_ADDR_NUM(u32AddrNum));
    DDL_ASSERT(IS_VALID_ADDR(u32AddrMode, u32Addr));

    __IO uint32_t* pu32SLRx = (__IO uint32_t*)((uint32_t)&I2Cx->SLR0 + u32AddrNum * 4UL);

    if(I2C_ADDR_MODE_DISABLE == u32AddrMode)
    {
        CLEAR_REG32_BIT(*pu32SLRx, I2C_SLR0_SLADDR0EN);
    }
    else
    {
        if(I2C_ADDR_MODE_10BIT == u32AddrMode)
        {
            WRITE_REG16(*pu32SLRx, u32AddrMode + u32Addr);
        }
        else
        {
            WRITE_REG16(*pu32SLRx, u32AddrMode + (u32Addr << 1));
        }
    }
}

/**
 * @brief  Manually set I2C master or slave mode
 * @param  [in] I2Cx                  Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32MSMode             Specify I2C in master or slave mode.
 *         This parameter can be one of the following values:
 *         @arg I2C_MASTER_SLAVE_MODE_MASTER
 *         @arg I2C_MASTER_SLAVE_MODE_SLAVE
 * @retval None
 */
void I2C_SetMasterSlaveMode(M4_I2C_TypeDef* I2Cx, uint32_t u32MSMode)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_MASTER_SLAVE_MODE(u32MSMode));

    if(I2C_MASTER_SLAVE_MODE_MASTER == u32MSMode)
    {
        SET_REG32_BIT(I2Cx->SR, I2C_MASTER_SLAVE_MODE_MASTER);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->SR, I2C_MASTER_SLAVE_MODE_MASTER);
    }
}

/**
 * @brief  Manually set I2C tx or rx mode
 * @param  [in] I2Cx                  Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32TxRxMode       Specifies I2C in tx or rx mode.
 *         This parameter can be one of the following values:
 *         @arg  I2C_TX_RX_MODE_TX
 *         @arg  I2C_TX_RX_MODE_RX
 * @retval None
 */
void I2C_SetTxRxMode(M4_I2C_TypeDef* I2Cx, uint32_t u32TxRxMode)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_TX_RX_MODE(u32TxRxMode));

    if(I2C_TX_RX_MODE_TX == u32TxRxMode)
    {
        SET_REG32_BIT(I2Cx->SR, I2C_TX_RX_MODE_TX);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->SR, I2C_TX_RX_MODE_TX);
    }
}

/**
 * @brief  Configure peripheral mode
 * @param  [in] I2Cx   Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32Mode This parameter can be one of the following values:
 *         @arg I2C_MODE_I2C
 *         @arg I2C_MODE_SMBUS
 * @retval None
 */
void I2C_SetMode(M4_I2C_TypeDef* I2Cx, uint32_t u32Mode)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_MODE(u32Mode));

    MODIFY_REG32(I2Cx->CR1, I2C_CR1_SMBUS, u32Mode);
}

/**
 * @brief  Enables or disables the specified I2C peripheral
 * @param  [in] I2Cx                Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_Cmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    MODIFY_REG32(I2Cx->CR1, I2C_CR1_PE, (uint32_t)enNewState << I2C_CR1_PE_POS);
}

/**
 * @brief I2C fast ACK config
 * @param  [in] I2Cx           Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param [in] enNewState      New state of the fast ACK function, can be
 *                             Disable or Enable the function
 * @retval None
 */
void I2C_FastAckCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        CLEAR_REG32_BIT(I2Cx->CR3, I2C_CR3_FACKEN);
    }
    else
    {
        SET_REG32_BIT(I2Cx->CR3, I2C_CR3_FACKEN);
    }
}

/**
 * @brief  I2C SCL high level timeout configuration
 * @param  [in] I2Cx       Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param [in] u16TimeoutH  clock timeout period for high level
 * @retval None
 */
void I2C_ClkHighTimeoutConfig(M4_I2C_TypeDef* I2Cx, uint16_t u16TimeoutH)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    CLEAR_REG32_BIT(I2Cx->SLTR, I2C_SLTR_TOUTHIGH);
    SET_REG32_BIT(I2Cx->SLTR, ((uint32_t)u16TimeoutH << 16U));
}

/**
 * @brief  I2C SCL low level timeout configuration
 * @param  [in] I2Cx       Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param [in] u16TimeoutL  clock timeout period for low level
 * @retval None
 */
void I2C_ClkLowTimeoutConfig(M4_I2C_TypeDef* I2Cx, uint16_t u16TimeoutL)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    CLEAR_REG32_BIT(I2Cx->SLTR, I2C_SLTR_TOUTLOW);
    SET_REG32_BIT(I2Cx->SLTR, u16TimeoutL);
}

/**
 * @brief  I2C SCL high level timeout function
 * @param  [in] I2Cx       Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param [in] enNewState  New state of the I2C SCL high level timeout function,
 *                         can be Disable or Enable the function
 * @retval None
 */
void I2C_ClkHighTimeoutCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(I2Cx->CR3, I2C_CR3_HTMOUT);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->CR3, I2C_CR3_HTMOUT);
    }
}

/**
 * @brief  I2C SCL low level timeout function
 * @param  [in] I2Cx       Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param [in] enNewState  New state of the I2C SCL low level timeout function
 *                         can be Disable or Enable the function
 * @retval None
 */
void I2C_ClkLowTimeoutCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(I2Cx->CR3, I2C_CR3_LTMOUT);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->CR3, I2C_CR3_LTMOUT);
    }
}

/**
 * @brief  I2C SCL timeout function command
 * @param  [in] I2Cx      Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param [in] enNewState New state of the I2C SCL timeout function, can be
 *                        Disable or Enable the function
 * @retval None
 */
void I2C_ClkTimeoutCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(I2Cx->CR3, I2C_CR3_TMOUTEN);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->CR3, I2C_CR3_TMOUTEN);
    }
}

/**
 * @brief  I2C SMBUS function configuration
 * @param  [in] I2Cx                    Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32SmbusConfig          Indicate the SMBUS address match function configuration.
 *         This parameter can be one or any combination of the following values:
 *         @ref I2C_Smbus_Match_Cfg
 *         @arg I2C_SMBUS_MATCH_ALRT    : Smbus alarm address
 *         @arg I2C_SMBUS_MATCH_DEFAULT : Smbus default address
 *         @arg I2C_SMBUS_MATCH_HOST    : Smbus host address address
 * @param  [in] enNewState              New state of the I2Cx SMBUS match config,
 *                                      @ref en_functional_state_t
 * @retval None
 */
void I2C_SmbusConfig(M4_I2C_TypeDef* I2Cx, uint32_t u32SmbusConfig, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_SMBUS_CONFIG(u32SmbusConfig));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(I2Cx->CR1, u32SmbusConfig);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->CR1, u32SmbusConfig);
    }
}

/**
 * @brief  I2C digital filter function configuration
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32DigFilterMode     Chose the digital filter mode,
 *         @ref I2C_Digital_Filter_mode
 *         This parameter can be one of the following values:
 *         @arg I2C_DIG_FILTMODE_1CYCLE
 *         @arg I2C_DIG_FILTMODE_2CYCLE
 *         @arg I2C_DIG_FILTMODE_3CYCLE
 *         @arg I2C_DIG_FILTMODE_4CYCLE
 * @retval None
 */
void I2C_DigitalFilterConfig(M4_I2C_TypeDef* I2Cx, uint32_t u32DigFilterMode)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_DIGITAL_FILTER(u32DigFilterMode));

    MODIFY_REG32(I2Cx->FLTR, I2C_FLTR_DNF, u32DigFilterMode);
}

/**
 * @brief  I2C digital filter function command
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_DigitalFilterCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    MODIFY_REG32(I2Cx->FLTR, I2C_FLTR_DNFEN, (uint32_t)enNewState << I2C_FLTR_DNFEN_POS);
}

/**
 * @brief  I2C analog filter function command
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_AnalogFilterCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    MODIFY_REG32(I2Cx->FLTR, I2C_FLTR_ANFEN, (uint32_t)enNewState << I2C_FLTR_ANFEN_POS);
}

/**
 * @brief  I2C general call function command
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_GeneralCallCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    MODIFY_REG32(I2Cx->CR1, I2C_CR1_ENGC, (uint32_t)enNewState << I2C_CR1_ENGC_POS);
}

/**
 * @brief  I2C generate start condition
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @retval None
 */
void I2C_GenerateStart(M4_I2C_TypeDef* I2Cx)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    SET_REG32_BIT(I2Cx->CR1, I2C_CR1_START);
}

/**
 * @brief  I2C generate restart condition
 * @param  [in] I2Cx                  Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @retval None
 */
void I2C_GenerateReStart(M4_I2C_TypeDef* I2Cx)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    SET_REG32_BIT(I2Cx->CR1, I2C_CR1_RESTART);
}

/**
 * @brief  I2C generate stop condition
 * @param  [in] I2Cx                  Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @retval None
 */
void I2C_GenerateStop(M4_I2C_TypeDef* I2Cx)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    SET_REG32_BIT(I2Cx->CR1, I2C_CR1_STOP);
}

/**
 * @brief  I2C status bit get
 * @param  [in] I2Cx                  Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32StatusBit         specify the flag to check,
 *         This parameter can be one of the following values:
 *         @arg   I2C_SR_STARTF     : Start condition detected flag
 *         @arg   I2C_SR_SLADDR0F   : Address 0 detected flag
 *         @arg   I2C_SR_SLADDR1F   : Address 1 detected flag
 *         @arg   I2C_SR_TENDF      : Transfer end flag
 *         @arg   I2C_SR_STOPF      : Stop condition detected flag
 *         @arg   I2C_SR_RFULLF     : Receive buffer full flag
 *         @arg   I2C_SR_TEMPTYF    : Transfer buffer empty flag
 *         @arg   I2C_SR_ARLOF      : Arbitration fails flag
 *         @arg   I2C_SR_ACKRF      : ACK detected flag
 *         @arg   I2C_SR_NACKF      : NACK detected flag
 *         @arg   I2C_SR_TMOUTF     : Time out detected flag
 *         @arg   I2C_SR_MSL        : Master mode flag
 *         @arg   I2C_SR_BUSY       : Bus busy status flag
 *         @arg   I2C_SR_TRA        : Transfer mode flag
 *         @arg   I2C_SR_GENCALLF   : General call detected flag
 *         @arg   I2C_SR_SMBDEFAULTF: Smbus default address detected flag
 *         @arg   I2C_SR_SMBHOSTF   : Smbus host address detected flag
 *         @arg   I2C_SR_SMBALRTF   : Smbus alarm address detected flag
 * @retval The status of the I2C status flag, may be Set or Reset.
 */
en_flag_status_t I2C_GetStatus(const M4_I2C_TypeDef *I2Cx, uint32_t u32StatusBit)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_RD_STATUS_BIT(u32StatusBit));

    return ((0UL != READ_REG32_BIT(I2Cx->SR, u32StatusBit)) ? Set : Reset);
}

/**
 * @brief  Clear I2C status flag
 * @param  [in] I2Cx                  Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32StatusBit         Specifies the flag to clear,
 *         This parameter can be any combination of the following values:
 *         @arg  I2C_CLR_STARTFCLR     : Start flag clear
 *         @arg  I2C_CLR_SLADDR0FCLR   : Address 0 detected flag clear
 *         @arg  I2C_CLR_SLADDR1FCLR   : Address 1 detected flag clear
 *         @arg  I2C_CLR_TENDFCLR      : Transfer end flag clear
 *         @arg  I2C_CLR_STOPFCLR      : Stop flag clear
 *         @arg  I2C_CLR_RFULLFCLR     : Receive buffer full flag clear
 *         @arg  I2C_CLR_TEMPTYFCLR    : Transfer buffer empty flag clear
 *         @arg  I2C_CLR_ARLOFCLR      : Arbitration fails flag clear
 *         @arg  I2C_CLR_NACKFCLR      : Nack detected flag clear
 *         @arg  I2C_CLR_TMOUTFCLR     : Time out detected flag clear
 *         @arg  I2C_CLR_GENCALLFCLR   : General call address detected flag clear
 *         @arg  I2C_CLR_SMBDEFAULTFCLR: Smbus default address detected flag clear
 *         @arg  I2C_CLR_SMBHOSTFCLR   : Smbus host address detected flag clear
 *         @arg  I2C_CLR_SMBALRTFCLR   : Smbus alarm address detected flag clear
 * @retval None
 */
void I2C_ClearStatus(M4_I2C_TypeDef* I2Cx, uint32_t u32StatusBit)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_CLEARBIT(u32StatusBit));

    WRITE_REG32(I2Cx->CLR,u32StatusBit);
}

/**
 * @brief  I2C software reset function command
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] enNewState           New state of the I2Cx function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_SoftwareResetCmd(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    MODIFY_REG32(I2Cx->CR1, I2C_CR1_SWRST, (uint32_t)enNewState << I2C_CR1_SWRST_POS);
}

/**
 * @brief  I2C interrupt function command
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32IntEn   Specifies the I2C interrupts sources to be configuration.
 *         This parameter can be any combination of the following values:
 *         @arg    I2C_CR2_STARTIE      : Start flag interrupt
 *         @arg    I2C_CR2_SLADDR0IE    : Address 0 detected interrupt
 *         @arg    I2C_CR2_SLADDR1IE    : Address 1 detected interrupt
 *         @arg    I2C_CR2_TENDIE       : Transfer end interrupt
 *         @arg    I2C_CR2_STOPIE       : Stop flag interrupt
 *         @arg    I2C_CR2_RFULLIE      : Receive buffer full interrupt
 *         @arg    I2C_CR2_TEMPTYIE     : Transfer buffer empty interrupt
 *         @arg    I2C_CR2_ARLOIE       : Arbitration fails interrupt
 *         @arg    I2C_CR2_NACKIE       : NACK flag detected interrupt
 *         @arg    I2C_CR2_TMOUTIE      : Time out detected interrupt
 *         @arg    I2C_CR2_GENCALLIE    : General call address detected interrupt
 *         @arg    I2C_CR2_SMBDEFAULTIE : Smbus default address detected interrupt
 *         @arg    I2C_CR2_SMBHOSTIE    : Smbus host address detected interrupt
 *         @arg    I2C_CR2_SMBALRTIE    : Smbus alarm address detected interrupt
 * @param  [in] enNewState           New state of the I2Cx interrupt function,
 *                                   @ref en_functional_state_t
 * @retval None
 */
void I2C_IntCmd(M4_I2C_TypeDef* I2Cx, uint32_t u32IntEn, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_VALID_INT(u32IntEn));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if(Enable == enNewState)
    {
        SET_REG32_BIT(I2Cx->CR2, u32IntEn);
    }
    else
    {
        CLEAR_REG32_BIT(I2Cx->CR2, u32IntEn);
    }
}

/**
 * @brief  I2C send data or address
 * @param  [in] I2Cx                 Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u8Data               The data to be send
 * @retval None
 */
void I2C_WriteDataReg(M4_I2C_TypeDef* I2Cx, uint8_t u8Data)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    WRITE_REG8(I2Cx->DTR, u8Data);
}

/**
 * @brief  I2C read data from register
 * @param  [in] I2Cx   Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @retval The value of the received data
 */
uint8_t I2C_ReadDataReg(const M4_I2C_TypeDef *I2Cx)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    return READ_REG8(I2Cx->DRR);
}

/**
 * @brief  I2C ACK status configuration
 * @param  [in] I2Cx                Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] enNewState           New state of the I2Cx function, can be
 *                                   Disable or Enable the function
 * @retval None
 */
void I2C_NackConfig(M4_I2C_TypeDef* I2Cx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    MODIFY_REG32(I2Cx->CR1, I2C_CR1_ACK, (uint32_t)enNewState << I2C_CR1_ACK_POS);
}

/**
 * @brief  Try to wait a status of specified flags
 * @param  [in] I2Cx                Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32Flags             specify the flags to check
 * @param  [in] enStatus             expected status,
 *         This parameter can be any combination of the following values:
 *         @arg   I2C_SR_STARTF     : Start condition detected flag
 *         @arg   I2C_SR_SLADDR0F   : Address 0 detected flag
 *         @arg   I2C_SR_SLADDR1F   : Address 1 detected flag
 *         @arg   I2C_SR_TENDF      : Transfer end flag
 *         @arg   I2C_SR_STOPF      : Stop condition detected flag
 *         @arg   I2C_SR_RFULLF     : Receive buffer full flag
 *         @arg   I2C_SR_TEMPTYF    : Transfer buffer empty flag
 *         @arg   I2C_SR_ARLOF      : Arbitration fails flag
 *         @arg   I2C_SR_ACKRF      : ACK detected flag
 *         @arg   I2C_SR_NACKF      : NACK detected flag
 *         @arg   I2C_SR_TMOUTF     : Time out detected flag
 *         @arg   I2C_SR_MSL        : Master mode flag
 *         @arg   I2C_SR_BUSY       : Bus busy status flag
 *         @arg   I2C_SR_TRA        : Transfer mode flag
 *         @arg   I2C_SR_GENCALLF   : General call detected flag
 *         @arg   I2C_SR_SMBDEFAULTF: Smbus default address detected flag
 *         @arg   I2C_SR_SMBHOSTF   : Smbus host address detected flag
 *         @arg   I2C_SR_SMBALRTF   : Smbus alarm address detected flag
 * @param  [in]  u32Timeout  Maximum count of trying to get a status of a
 *              flag in status register
 * @retval  Process result
 *          - Error         Failed to get expected status of specified flags
 *          - Ok            successfully gotten the expected status of the specified flags
 */
static en_result_t I2C_WaitStatus(const M4_I2C_TypeDef *I2Cx, uint32_t u32Flags, en_flag_status_t enStatus, uint32_t u32Timeout)
{
    en_result_t enRet = Error;
    uint32_t u32RegStatusBit;

    while(1U)
    {
        u32RegStatusBit = (READ_REG32_BIT(I2Cx->SR, u32Flags));
        if(((enStatus == Set) && (u32Flags == u32RegStatusBit))
           || ((enStatus == Reset) && (0UL == u32RegStatusBit)))
        {
            enRet = Ok;
        }

        if((Ok == enRet) || (0UL == u32Timeout))
        {
            break;
        }
        else
        {
            u32Timeout--;
        }
    }

    return enRet;
}

/**
 * @brief  I2Cx Start
 * @param  [in] I2Cx   Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in]  u32Timeout  Maximum count of trying to get a status of a
 *              flag in status register
 * @retval An en_result_t enumeration value:
 *            - Ok: Start successfully
 *            - Error: Start unsuccessfully
 */
en_result_t I2C_Start(M4_I2C_TypeDef* I2Cx, uint32_t u32Timeout)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    en_result_t enRet;

    enRet = I2C_WaitStatus(I2Cx, I2C_SR_BUSY, Reset, u32Timeout);

    if(Ok == enRet)
    {
        /* generate start signal */
        I2C_GenerateStart(I2Cx);
        /* Judge if start success*/
        enRet = I2C_WaitStatus(I2Cx, (I2C_SR_BUSY | I2C_SR_STARTF), Set, u32Timeout);
    }

    return enRet;
}

/**
 * @brief  I2Cx Restart
 * @param  [in] I2Cx         Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u32Timeout  Maximum count of trying to get a status of a
 *              flag in status register
 * @retval An en_result_t enumeration value:
 *            - Ok: Restart successfully
 *            - Error: Restart unsuccessfully
 */
en_result_t I2C_Restart(M4_I2C_TypeDef* I2Cx, uint32_t u32Timeout)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    en_result_t enRet;

    /* Clear start status flag */
    I2C_ClearStatus(I2Cx, I2C_CLR_STARTFCLR);
    /* Send restart condition */
    I2C_GenerateReStart(I2Cx);
    /* Judge if start success*/
    enRet = I2C_WaitStatus(I2Cx, (I2C_SR_BUSY | I2C_SR_STARTF), Set, u32Timeout);

    return enRet;
}

/**
 * @brief  I2Cx Send Address
 * @param  [in] I2Cx         Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] u8Addr       The address to be sent
 * @param  [in] u32Timeout   Maximum count of trying to get a status of a
 *              flag in status register
 * @retval An en_result_t enumeration value:
 *            - Ok: Send successfully
 *            - Error: Send unsuccessfully
 */
en_result_t I2C_SendAddr(M4_I2C_TypeDef* I2Cx, uint8_t u8Addr, uint32_t u32Timeout)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    en_result_t enRet;

    enRet = I2C_WaitStatus(I2Cx, I2C_SR_TEMPTYF, Set, u32Timeout);

    if(Ok == enRet)
    {
        /* Send I2C address */
        I2C_WriteDataReg(I2Cx, u8Addr);

        if(0U == (u8Addr & 0x01U))
        {
            /* If in master transfer process, Need wait transfer end */
            enRet = I2C_WaitStatus(I2Cx, I2C_SR_TENDF, Set, u32Timeout);

            if(enRet == Ok)
            {
                enRet = I2C_WaitStatus(I2Cx, I2C_SR_ACKRF, Reset, u32Timeout);
            }
        }
    }

    return enRet;
}

/**
 * @brief  I2Cx Send Data
 * @param  [in] I2Cx           Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in] pau8TxData     The data array to be sent
 * @param  [in] u32Size        Number of data in array pau8TxData
 * @param  [in] u32Timeout     Maximum count of trying to get a status of a
 *              flag in status register
 * @retval An en_result_t enumeration value:
 *            - Ok: Send successfully
 *            - Error: Send unsuccessfully
 *            - ErrorInvalidParameter: pau8TxData is NULL
 */
en_result_t I2C_SendData(M4_I2C_TypeDef* I2Cx, uint8_t const pau8TxData[], uint32_t u32Size, uint32_t u32Timeout)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    en_result_t enRet = Ok;
    uint32_t u32Cnt = 0UL;

    if(pau8TxData != NULL)
    {
        while((u32Cnt != u32Size) && (enRet == Ok))
        {
            /* Wait tx buffer empty */
            enRet = I2C_WaitStatus(I2Cx, I2C_SR_TEMPTYF, Set, u32Timeout);

            if(enRet == Ok)
            {
                /* Send one byte data */
                I2C_WriteDataReg(I2Cx, pau8TxData[u32Cnt++]);
                /* Wait transfer end*/
                enRet = I2C_WaitStatus(I2Cx, I2C_SR_TENDF, Set, u32Timeout);

                if(enRet == Ok)
                {
                    enRet = I2C_WaitStatus(I2Cx, I2C_SR_ACKRF, Reset, u32Timeout);
                }
            }
        }
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  I2Cx Receive Data
 * @param  [in] I2Cx          Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [out] pau8RxData    Array to hold the received data
 * @param  [in]  u32Size       Number of data to be received
 * @param  [in]  u32Timeout    Maximum count of trying to get a status of a
 *              flag in status register
 * @retval An en_result_t enumeration value:
 *            - Ok: Receive successfully
 *            - Error: Send unsuccessfully
 *            - ErrorInvalidParameter: pau8TxData is NULL
 */
en_result_t I2C_RcvData(M4_I2C_TypeDef* I2Cx, uint8_t pau8RxData[], uint32_t u32Size, uint32_t u32Timeout)
{
    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    en_result_t enRet = Ok;

    if(pau8RxData != NULL)
    {
        for(uint32_t i=0UL; i<u32Size; i++)
        {
            if(i == (u32Size - 1UL))
            {
                I2C_NackConfig(I2Cx, Enable);
            }

            enRet = I2C_WaitStatus(I2Cx, I2C_SR_RFULLF, Set, u32Timeout);

            if(enRet == Ok)
            {
                 /* read data from register */
                pau8RxData[i] = I2C_ReadDataReg(I2Cx);
                /* manually send ack if FACKEN is set to 1(1:manually ack;0:fast ack) */
                if((0UL != READ_REG32_BIT(I2Cx->CR3, I2C_CR3_FACKEN)) && (i != (u32Size - 1UL)))
                {
                    I2C_NackConfig(I2Cx, Disable);
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

/**
 * @brief  I2Cx Stop
 * @param  [in] I2Cx          Pointer to the I2C peripheral register.
 *         This parameter can be one of the following values:
 *         @arg M4_I2C1
 *         @arg M4_I2C2
 *         @arg M4_I2C3
 *         @arg M4_I2C4
 *         @arg M4_I2C5
 *         @arg M4_I2C6
 * @param  [in]  u32Timeout   Maximum count of trying to get a status of a
 *              flag in status register
 * @retval An en_result_t enumeration value:
 *            - Ok: Stop successfully
 *            - Error: Stop unsuccessfully
 */
en_result_t I2C_Stop(M4_I2C_TypeDef* I2Cx, uint32_t u32Timeout)
{
    en_result_t enRet;

    DDL_ASSERT(IS_VALID_UNIT(I2Cx));

    /* Clear stop flag */
    while((Set == I2C_GetStatus(I2Cx, I2C_SR_STOPF)) && (u32Timeout > 0UL))
    {
        I2C_ClearStatus(I2Cx, I2C_CLR_STOPFCLR);
        u32Timeout--;
    }
    I2C_GenerateStop(I2Cx);
    /* Wait stop flag */
    enRet = I2C_WaitStatus(I2Cx, I2C_SR_STOPF, Set, u32Timeout);

    return enRet;
}
/**
 * @}
 */

#endif /* DDL_I2C_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
