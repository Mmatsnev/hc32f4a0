/**
 *******************************************************************************
 * @file  mau/mau_base/source/main.c
 * @brief Main program of MAU base for the Device Driver Library.
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
#include "math.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup MAU
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define MAU_SQRT_IRQn                   (Int008_IRQn)
#define MAU_SQRT_INTSRC                 (INT_MAU_SQRT)

#define PI                              (3.1415926F)
#define SIN_DELTA                       (1U)
#define SQRT_DELTA                      (1U)
#define SIN_DOT_CNT                     (4096U)
#define SQRT_DATA_CNT                   (4096U)

#define LED_DLY_MS                      (200U)
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

/**
 * @brief   MAU Initialization
 * @param   None
 * @retval  None
 */
static void MAU_Init(void)
{
    /* Enable MAU peripheral clock. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_CORDIC, Enable);
}

/**
 * @brief  Check whether the delta between two values is below threshold
 * @param  [in] u32VarA       First input variable
 * @param  [in] u32VarB       Second input variable
 * @param  [in] u32MaxError   threshold
 * @retval Status
 *           0: Delta is below threshold
 *           1: Delta is above threshold
 */
static uint8_t MAU_CheckResidualErr(uint32_t u32VarA, uint32_t u32VarB, uint32_t u32MaxError)
{
    uint8_t status = 0U;
    if (u32VarA >= u32VarB)
    {
        if ((u32VarA - u32VarB) > u32MaxError)
        {
            status = 1U;
        }
    }
    else
    {
        if ((u32VarB - u32VarA) > u32MaxError)
        {
            status = 1U;
        }
    }
    return status;
}

/**
 * @brief  Error Handler, toggle led red if u32IsError is 1, else turn led blue on
 * @param  [in] u32IsError
 * @retval  None
 */
static void ErrHandler(uint32_t u32IsError)
{
    if (u32IsError)
    {
        while(1)
        {
            BSP_LED_Toggle(LED_RED);
            DDL_DelayMS(LED_DLY_MS);
        }
    }
    else
    {
        BSP_LED_On(LED_BLUE);
        while(1)
        {
            /* do nothing */
        }
    }
}

/**
 * @brief   Interrupt callback function of sqrt Done
 * @param   None
 * @retval  None
 */
static void MAU_SqrtIrqCallback(void)
{
    MAU_SqrtReadDataReg(M4_MAU);
}

/**
 * @brief   Sqrt interrupt init
 * @param   None
 * @retval  None
 */
static void Sqrt_IntCfg(void)
{
    stc_irq_signin_config_t stcIrqSignCfg;
    MAU_SqrtIntCmd(M4_MAU, Enable);

    /* Register IRQ handler && configure NVIC. */
    stcIrqSignCfg.enIRQn = MAU_SQRT_IRQn;
    stcIrqSignCfg.enIntSrc = MAU_SQRT_INTSRC;
    stcIrqSignCfg.pfnCallback = &MAU_SqrtIrqCallback;
    INTC_IrqSignIn(&stcIrqSignCfg);
    NVIC_ClearPendingIRQ(stcIrqSignCfg.enIRQn);
    NVIC_SetPriority(stcIrqSignCfg.enIRQn, DDL_IRQ_PRIORITY_03);
    NVIC_EnableIRQ(stcIrqSignCfg.enIRQn);
}

/**
 * @brief  Generate random data as radicand parameters for sqrt example
 * @param  [in] u32TrngArr  array to hold random data
 * @param  [in] count       number of random data that will be generated
 * @retval None
 */
static void  Trng_Gen(uint32_t u32TrngArr[],uint32_t count)
{
    uint32_t i = 0U,trngloop = count / 2U;

    /* Enable TRNG. */
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_TRNG, Enable);
    /* Shift 64 times */
    TRNG_SetShiftCnt(TRNG_SHIFT_COUNT_64);
    /* Disable the Load function */
    TRNG_ReloadCmd(TRNG_RELOAD_DISABLE);
    for(i = 0U; i < trngloop; i++)
    {
        TRNG_Generate(&u32TrngArr[i * 2U]);
    }
}

/**
 * @brief Compare the sqrt result generated by mau with the result generated by C library math.h
 * @param  None
 * @retval u32Err
 *           0: Delta is below threshold
 *           1: Delta is above threshold
 */
static en_result_t Sqrt_Example(void)
{
    static uint32_t u32Radicands[SQRT_DATA_CNT];
    static uint32_t u32SplGrp[SQRT_DATA_CNT];
    static uint32_t u32CtrlGrp[SQRT_DATA_CNT];
    uint32_t u32CtrlScale = 1U;
    en_result_t enRet = Ok;

    /* Generate random data as radicand parameters for sqrt example */
    Trng_Gen(u32Radicands,SQRT_DATA_CNT);
    /* Sqrt configuration */
    Sqrt_IntCfg();
    MAU_SqrtResultLShiftCfg(M4_MAU,0U);
    /* Input random radicands, generate sqrt and compare results */
    for(uint32_t i = 0U; i < SQRT_DATA_CNT; i++)
    {
        /* Generate sample group by mau */
        enRet = MAU_Sqrt(M4_MAU, u32Radicands[i], &u32SplGrp[i]);
        if(Ok == enRet)
        {
            u32SplGrp[i] = (uint32_t)((float32_t)((float32_t)u32SplGrp[i] / (float32_t)u32CtrlScale) + 0.5F);
            /* Generate control group by math.h */
            u32CtrlGrp[i] = (uint32_t)((float32_t)(sqrt((float64_t)u32Radicands[i]) + 0.5F));
            /* Compare result to check error*/
            if(MAU_CheckResidualErr(u32SplGrp[i]
                                    ,(u32CtrlGrp[i])
                                        ,SQRT_DELTA))
            {
                enRet = Error;
            }
        }

        if(Error == enRet)
        {
            break;
        }
    }
    return enRet;
}

/**
 * @brief  Compare the sine result generated by mau with the result generated by C library math.h
 * @param  None
 * @retval u32Err
 *           0: Delta is below threshold
 *           1: Delta is above threshold
 */
static uint32_t Sin_Example(void)
{
    static uint32_t u32SplGrp[SIN_DOT_CNT];
    static uint32_t u32CtrlGrp[SIN_DOT_CNT];
    volatile uint16_t u16AvgAngIdx = (uint16_t)(float32_t)((float32_t)MAU_SIN_ANGIDX_TOTAL / (float32_t)SIN_DOT_CNT + (float32_t)0.5);
    uint32_t u32Err = 0U;
    float32_t fAvgRadian;

    if(u16AvgAngIdx == 0U)
    {
        u16AvgAngIdx = 1U;
    }
    fAvgRadian = (float32_t)u16AvgAngIdx / (float32_t)MAU_SIN_ANGIDX_TOTAL * (float32_t)2 * PI;
    for(uint32_t i = 0U; i < SIN_DOT_CNT; i++)
    {
        /* Generate sqrt sample group by mau */
        u32SplGrp[i] = (uint32_t)MAU_Sin(M4_MAU, (uint16_t)(u16AvgAngIdx * i));
        /* Generate sqrt control group by math.h */
        u32CtrlGrp[i] = (uint32_t)(int32_t)(float32_t)(sin(fAvgRadian * (float64_t)i) * (float32_t)MAU_SIN_Q15_SCALAR);
        /* Compare result to check error*/
        if(MAU_CheckResidualErr(u32SplGrp[i]
                                ,(u32CtrlGrp[i])
                                    ,SIN_DELTA))
        {
            u32Err = 1U;
            break;
        }
    }
    return u32Err;
}

/**
 * @brief  Main function of MAU project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_LED_Init();

    uint32_t u32ErrSqrt = 0U,u32ErrSin = 0U;
    /* 0.MAU initialization*/
    MAU_Init();

    /* 1.Compare the sqrt result generated by mau with the result generated by C library math.h*/
    u32ErrSqrt = Sqrt_Example();
    /* 2.Compare the sine result generated by mau with the result generated by C library math.h*/
    u32ErrSin = Sin_Example();

    /* 3.Error handler, turn led blue on if u32ErrSqrt and u32ErrSin are both 0, else toggle led red*/
    ErrHandler((u32ErrSqrt || u32ErrSin));
    return 0;
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
