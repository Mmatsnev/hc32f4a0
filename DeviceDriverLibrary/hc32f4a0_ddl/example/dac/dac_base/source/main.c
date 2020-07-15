/**
 *******************************************************************************
 * @file  dac/dac_base/source/main.c
 * @brief Main program of DAC base for the Device Driver Library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hexiao          First version
   2020-07-15       Hexiao          1. Replace DAC_ChannelCmd by DAC_Start and DAC_Stop
                                    2. Replace DAC_DualChannelCmd by DAC_DualChannelStart
                                       and DAC_DualChannelStop
                                    3. function rename
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
#include "hc32f4a0_dac.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup DAC_Base
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum
{
    E_KEY_NOT_PRESSED,
    E_KEY1_PRESSED,
    E_KEY2_PRESSED,
    E_KEY3_PRESSED,
    E_KEY4_PRESSED,
    E_KEY5_PRESSED,
}en_key_event;

typedef enum
{
    DAC_Unit1,
    DAC_Unit2,
    DAC_Unit_Max,
}en_dac_unit;

typedef enum
{
    E_Dac_Single,
    E_Dac_Dual,
}en_dac_cvt_t;

typedef void (*pfSingleWaveSetData)(M4_DAC_TypeDef* pstcDACx, uint16_t data);
typedef struct
{
    M4_DAC_TypeDef* pUnit;
    en_dac_cvt_t enCvtType;
    uint16_t u16Ch;
}st_dac_handle_t;
/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DAC_UNIT1_PORT                               (GPIO_PORT_A)
#define DAC_UNIT2_PORT                               (GPIO_PORT_C)
#define DAC_UNIT1_CHN1_PIN                           (GPIO_PIN_04)
#define DAC_UNIT1_CHN2_PIN                           (GPIO_PIN_05)
#define DAC_UNIT2_CHN1_PIN                           (GPIO_PIN_04)
#define DAC_UNIT2_CHN2_PIN                           (GPIO_PIN_05)
#define VREFH                                        (3.3F)
#define DAC_CHN1                                     (0U)
#define DAC_CHN2                                     (1U)
#define DAC_DATA_ALIGN_12b_R                         (0U)
#define DAC_DATA_ALIGN_12b_L                         (1U)

/* Open AMP function on  BSP_EV_HC32F4A0_LQFP176 board */
#define SUPPORT_AMP
//#define SUPPORT_ADP
#define SINGLE_WAVE_DAC_CHN                          (DAC_CHN1)
#define DAC_DATA_ALIGN                               (DAC_DATA_ALIGN_12b_L)

#define SINE_DOT_NUMBER                              (4096U)
#define SINE_NEGATIVE_TO_POSITVE                     (1.0F)

#if (SINGLE_WAVE_DAC_CHN == DAC_CHN1)
#define pfSingleWaveSetData                          (DAC_SetChannel1Data)
#define pfSingleWaveConvState                        (DAC_GetChannel1ConvState)
#else
#define pfSingleWaveSetData                          (DAC_SetChannel2Data)
#define pfSingleWaveConvState                        (DAC_GetChannel2ConvState)
#endif
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
static st_dac_handle_t gstDACHandle[DAC_Unit_Max] = {0};
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
 * @brief    Get key pressed event
 * @param    None
 * @retval   An en_key_event enumeration value
 */
static en_key_event Key_Event(void)
{
    en_key_event enEvent = E_KEY_NOT_PRESSED;

    if(Set == BSP_KEY_GetStatus(BSP_KEY_1))
    {
        enEvent = E_KEY1_PRESSED;
    }
    else if(Set == BSP_KEY_GetStatus(BSP_KEY_2))
    {
        enEvent = E_KEY2_PRESSED;
    }
    else if(Set == BSP_KEY_GetStatus(BSP_KEY_3))
    {
        enEvent = E_KEY3_PRESSED;
    }
    else if(Set == BSP_KEY_GetStatus(BSP_KEY_4))
    {
        enEvent = E_KEY4_PRESSED;
    }
    else if(Set == BSP_KEY_GetStatus(BSP_KEY_5))
    {
        enEvent = E_KEY5_PRESSED;
    }
    else
    {
        enEvent = E_KEY_NOT_PRESSED;
    }
    return enEvent;
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
 * @brief    Sin table Initialization
 * @param    [in] pSinTable  sin table
 * @param    [in] u32count   number of pSinTable items
 * @retval   None
 */
static void SinTable_Init(uint32_t pSinTable[],uint32_t u32count)
{
    uint32_t i = 0U;
    uint32_t u32AngAvg = (uint32_t)(float32_t)((float32_t)((float32_t)MAU_SIN_ANGIDX_TOTAL / (float32_t)u32count) + 0.5);
    float32_t fSin = 0.0F;
    for(i = 0U; i < u32count; i++)
    {
        fSin = (((float32_t)MAU_Sin(M4_MAU, (uint16_t)(u32AngAvg * i))
                 / (float32_t)MAU_SIN_Q15_SCALAR + SINE_NEGATIVE_TO_POSITVE) / VREFH) *
            (float32_t)DAC_DATAREG_VALUE_MAX + 0.5F;

        #if (DAC_DATA_ALIGN == DAC_DATA_ALIGN_12b_L)
        {
            pSinTable[i] = (uint32_t)fSin << 4;
        }
        #else
        {
            pSinTable[i] = (uint32_t)fSin;
        }
        #endif
    }
}

/**
 * @brief    Enable DAC peripheral clock
 * @param    [in] enUnit  The selected DAC unit
 * @retval   None
 */
static void DAC_PClkEnable(en_dac_unit enUnit)
{
    uint32_t u32PClk = 0U;
    switch(enUnit)
    {
    case DAC_Unit1:
        u32PClk = PWC_FCG3_DAC1;
        break;
    case DAC_Unit2:
        u32PClk = PWC_FCG3_DAC2;
        break;
    default:
        u32PClk = PWC_FCG3_DAC1 | PWC_FCG3_DAC2;
        break;
    }
    /* Enable DAC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(u32PClk, Enable);
}

/**
 * @brief    Init DAC single channel
 * @param    [in] enUnit  The selected DAC unit
 * @retval   A pointer of DAC handler
 */
st_dac_handle_t* DAC_SingleConversionInit(en_dac_unit enUnit)
{
    uint8_t u8Port;
    uint16_t u16Pin;
    st_dac_handle_t* pDac = NULL;

    if(enUnit == DAC_Unit1)
    {
        pDac = &gstDACHandle[DAC_Unit1];
        pDac->pUnit = M4_DAC1;
    }
    else
    {
        pDac = &gstDACHandle[DAC_Unit2];
        pDac->pUnit = M4_DAC2;
    }
    DAC_PClkEnable(enUnit);

    pDac->enCvtType = E_Dac_Single;
    #if (SINGLE_WAVE_DAC_CHN == DAC_CHN1)
    pDac->u16Ch = DAC_CH_1;
    #else
    pDac->u16Ch = DAC_CH_2;
    #endif

    /* Init DAC by default value: source from data register and output enabled*/
    DAC_DeInit(pDac->pUnit);
    stc_dac_init_t stInit;
    DAC_StructInit(&stInit);
    DAC_Init(pDac->pUnit, pDac->u16Ch, &stInit);
    #if (DAC_DATA_ALIGN == DAC_DATA_ALIGN_12b_L)
    DAC_DataRegAlignConfig(pDac->pUnit,DAC_DATA_ALIGN_L);
    #else
    DAC_DataRegAlignConfig(pDac->pUnit,DAC_DATA_ALIGN_R);
    #endif

    /* Set DAC pin attribute to analog */
    if(enUnit == DAC_Unit1)
    {
        u8Port = DAC_UNIT1_PORT;
        #if (SINGLE_WAVE_DAC_CHN == DAC_CHN1)
        u16Pin = DAC_UNIT1_CHN1_PIN;
        #else
        u16Pin = DAC_UNIT1_CHN2_PIN;
        #endif
    }
    else
    {
        u8Port = DAC_UNIT2_PORT;
        #if (SINGLE_WAVE_DAC_CHN == DAC_CHN1)
        u16Pin = DAC_UNIT2_CHN1_PIN;
        #else
        u16Pin = DAC_UNIT2_CHN2_PIN;
        #endif
    }
    stc_gpio_init_t stcGpioInit;
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    GPIO_Init(u8Port, u16Pin, &stcGpioInit);

    #ifdef SUPPORT_ADP
    /* Set ADC first */
    /* Enable DAC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_ADC1|PWC_FCG3_ADC2|PWC_FCG3_ADC3, Enable);
    if(M4_ADC1->STR == 0U)
    {
        if(M4_ADC2->STR == 0U)
        {
            if(M4_ADC3->STR == 0U)
            {
                DAC_ADCPrioConfig(pDac->pUnit, DAC_ADP_SELECT_ALL);
                DAC_ADCPrioCmd(pDac->pUnit, Enable);
            }
        }
    }
    #endif
    return pDac;
}

/**
 * @brief    Init DAC Dual channel
 * @param    [in] enUnit  The selected DAC unit
 * @retval   A pointer of DAC handler
 */
st_dac_handle_t* DAC_DualConversionInit(en_dac_unit enUnit)
{
    uint8_t u8Port;
    st_dac_handle_t* pDac = NULL;

    if(enUnit == DAC_Unit1)
    {
        pDac = &gstDACHandle[DAC_Unit1];
        pDac->pUnit = M4_DAC1;
    }
    else
    {
        pDac = &gstDACHandle[DAC_Unit2];
        pDac->pUnit = M4_DAC2;
    }
    DAC_PClkEnable(enUnit);
    pDac->enCvtType = E_Dac_Dual;

    /* Init DAC by default value: source from data register and output enabled*/
    DAC_DeInit(pDac->pUnit);
    stc_dac_init_t stInit;
    DAC_StructInit(&stInit);
    DAC_Init(pDac->pUnit, DAC_CH_1, &stInit);
    DAC_Init(pDac->pUnit, DAC_CH_2, &stInit);
    #if (DAC_DATA_ALIGN == DAC_DATA_ALIGN_12b_L)
    DAC_DataRegAlignConfig(pDac->pUnit,DAC_DATA_ALIGN_L);
    #else
    DAC_DataRegAlignConfig(pDac->pUnit,DAC_DATA_ALIGN_R);
    #endif

    /* Set DAC pin attribute to analog */
    stc_gpio_init_t stcGpioInit;
    GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;

    if(enUnit == DAC_Unit1)
    {
        u8Port = DAC_UNIT1_PORT;
        GPIO_Init(u8Port,DAC_UNIT1_CHN1_PIN, &stcGpioInit);
        GPIO_Init(u8Port,DAC_UNIT1_CHN2_PIN, &stcGpioInit);
    }
    else
    {
        u8Port = DAC_UNIT2_PORT;
        GPIO_Init(u8Port,DAC_UNIT2_CHN1_PIN, &stcGpioInit);
        GPIO_Init(u8Port,DAC_UNIT2_CHN2_PIN, &stcGpioInit);
    }

    #ifdef SUPPORT_ADP
    /* Set ADC first */
    /* Enable DAC peripheral clock. */
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_ADC1|PWC_FCG3_ADC2|PWC_FCG3_ADC3, Enable);
    if(M4_ADC1->STR == 0U)
    {
        if(M4_ADC2->STR == 0U)
        {
            if(M4_ADC3->STR == 0U)
            {
                DAC_ADCPrioConfig(pDac->pUnit, DAC_ADP_SELECT_ALL);
                DAC_ADCPrioCmd(pDac->pUnit, Enable);
            }
        }
    }
    #endif
    return pDac;
}

/**
 * @brief    Start  Dual DAC conversions
 * @param    [in] pDac       A pointer of DAC handler
 * @retval   None
 */
static void DAC_StartDualConversion(const st_dac_handle_t* pDac)
{
    #ifdef SUPPORT_AMP
    /* Enalbe AMP */
    DAC_AMPCmd(pDac->pUnit,DAC_CH_1,Enable);
    DAC_AMPCmd(pDac->pUnit,DAC_CH_2,Enable);
    #endif

    /* Start Dual Channel Conversion */
    DAC_DualChannelStart(pDac->pUnit);

    #ifdef SUPPORT_AMP
    /* delay 3us before setting data*/
    DDL_DelayMS(1U);
    #endif
}

/**
 * @brief    Start single DAC conversions
 * @param    [in] pDac       A pointer of DAC handler
 * @retval   None
 */
static void DAC_StartSingleConversion(const st_dac_handle_t* pDac)
{
    /* Enalbe AMP */
    #ifdef SUPPORT_AMP
    DAC_AMPCmd(pDac->pUnit,pDac->u16Ch,Enable);
    #endif

    DAC_Start(pDac->pUnit,pDac->u16Ch);

    #ifdef SUPPORT_AMP
    /* delay 3us before setting data*/
    DDL_DelayMS(1U);
    #endif
}

/**
 * @brief    Convert data by single DAC channel
 * @param    [in] pDac       A pointer of DAC handler
 * @param    [in] pDataTable The data table to be converted
 * @param    [in] u32count   Number of data table items
 * @retval   None
 */
__STATIC_INLINE void DAC_SetSingleConversionData(const st_dac_handle_t* pDac, uint32_t const pDataTable[],uint32_t u32count)
{
    for(uint32_t i = 0U; i < u32count; i++)
    {
        #ifdef SUPPORT_ADP
        uint32_t u32TryCount = 100U;
        while(u32TryCount != 0U)
        {
            u32TryCount--;
            if(OperationInProgress != pfSingleWaveConvState(pDac->pUnit))
            {
                break;
            }
        }
        #endif
        pfSingleWaveSetData(pDac->pUnit,(uint16_t)pDataTable[i]);
    }
}

/**
 * @brief    Convert data by dual DAC channel
 * @param    [in] pDac       A pointer of DAC handler
 * @param    [in] pDataTable The data table to be converted
 * @param    [in] u32count   Number of data table items
 * @retval   None
 */
__STATIC_INLINE void DAC_SetDualConversionData(const st_dac_handle_t* pDac, uint32_t const pDataTable[],uint32_t u32count)
{
    for(uint32_t i = 0U; i < u32count; i++)
    {
        #ifdef SUPPORT_ADP
        en_dac_conv_sate_t enSt;
        uint32_t u32TryCount = 100U;
        while(u32TryCount != 0U)
        {
            enSt = DAC_GetChannel1ConvState(pDac->pUnit);
            u32TryCount--;
            if(OperationInProgress != enSt)
            {
                enSt = DAC_GetChannel2ConvState(pDac->pUnit);
                if(OperationInProgress != enSt)
                {
                    break;
                }
            }
        }
        #endif
        DAC_SetDualChannelData(pDac->pUnit,(uint16_t)pDataTable[i],(uint16_t)pDataTable[i]);
    }
}

/**
 * @brief    stop DAC conversion
 * @param    [in] pDac A pointer of DAC handler
 * @retval   None
 */
static void DAC_StopConversion(const st_dac_handle_t* pDac)
{
    if(NULL == pDac)
    {
        DAC_DeInit(M4_DAC1);
        DAC_DeInit(M4_DAC2);
    }
    else if(pDac->enCvtType != E_Dac_Dual)
    {
        DAC_Stop(pDac->pUnit,pDac->u16Ch);
    }
    else
    {
        DAC_DualChannelStop(pDac->pUnit);
    }
}

/**
 * @brief  Main function of DAC base project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    Peripheral_WE();

    BSP_CLK_Init();
    BSP_IO_Init();
    BSP_LED_Init();
    BSP_KEY_Init();

    /*DAC_UNIT2_CHN1_PIN(PC04) and DAC_UNIT2_CHN2_PIN(PC05) are also used by
    * Ethernet PHY module on BSP_EV_HC32F4A0_LQFP176 board. Pull down reset pin
    * of Ethernet to eliminate the influence from Ethernet
    */
    BSP_IO_ConfigPortPin(EIO_PORT1, EIO_ETH_RST, EIO_DIR_OUT);
    BSP_IO_WritePortPin(EIO_PORT1, EIO_ETH_RST, (uint8_t)Disable);

    /* Init MAU for generating sine data*/
    MAU_Init();
    /* Init sine data table */
    static uint32_t gu32SinTable[SINE_DOT_NUMBER];
    SinTable_Init(gu32SinTable, SINE_DOT_NUMBER);

    /* Init DAC */
    st_dac_handle_t* pSingleDac = DAC_SingleConversionInit(DAC_Unit1);
    st_dac_handle_t* pDualDac = DAC_DualConversionInit(DAC_Unit2);
    /**
    * Output sine waves after system restart,
    * Press KEY2(SW2) to stop the single sine wave ,and Press KEY1(SW1) to restart
    * Press KEY4(SW4) to stop the dual sine waves ,and Press KEY3(SW3) to restart
    * Press KEY5(SW5) to stop all sine waves and end this sample
    */
    en_key_event enEvent = E_KEY_NOT_PRESSED;
    uint8_t u8EnableSingle = 1U,u8EnableDual = 1U;
    DAC_StartSingleConversion(pSingleDac);
    BSP_LED_On(LED_BLUE);
    DAC_StartDualConversion(pDualDac);
    BSP_LED_On(LED_RED);

    while (1U)
    {
        enEvent = Key_Event();
        switch(enEvent)
        {
            case E_KEY1_PRESSED:
                u8EnableSingle = 1U;
                BSP_LED_On(LED_BLUE);
                DAC_StartSingleConversion(pSingleDac);
                break;
            case E_KEY2_PRESSED:
                u8EnableSingle = 0U;
                DAC_StopConversion(pSingleDac);
                BSP_LED_Off(LED_BLUE);
                break;
            case E_KEY3_PRESSED:
                u8EnableDual = 1U;
                BSP_LED_On(LED_RED);
                DAC_StartDualConversion(pDualDac);
                break;
            case E_KEY4_PRESSED:
                u8EnableDual = 0U;
                DAC_StopConversion(pDualDac);
                BSP_LED_Off(LED_RED);
                break;
            case E_KEY5_PRESSED:
                DAC_StopConversion(NULL);
                u8EnableSingle = 0U;
                u8EnableDual = 0U;
                BSP_LED_Off(LED_BLUE);
                BSP_LED_Off(LED_RED);
                break;
            default:
                break;
        }
        if(u8EnableSingle)
        {
            DAC_SetSingleConversionData(pSingleDac,gu32SinTable, SINE_DOT_NUMBER);
        }
        if(u8EnableDual)
        {
            DAC_SetDualConversionData(pDualDac,gu32SinTable, SINE_DOT_NUMBER);
        }
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
