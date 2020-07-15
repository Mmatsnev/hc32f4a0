/**
 *******************************************************************************
 * @file  rtc/rtc_alarm/source/main.c
 * @brief Main program of RTC Alarm for the Device Driver Library.
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

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup RTC_Alarm
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t u8SecIntFlag = 0U;
static uint8_t u8AlarmIntFlag = 0U, u8AlarmCnt = 0U;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  RTC period interrupt callback function.
 * @param  None
 * @retval None
 */
void RTC_Period_IrqHandler(void)
{
    u8SecIntFlag = 1U;
    RTC_ClearStatus(RTC_FLAG_PRDF);
}

/**
 * @brief  RTC alarm interrupt callback function.
 * @param  None
 * @retval None
 */
void RTC_Alarm_IrqHandler(void)
{
    u8AlarmCnt = 10U;
    u8AlarmIntFlag = 1U;
    RTC_ClearStatus(RTC_FLAG_ALMF);
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
 * @brief  RTC calendar configuration.
 * @param  None
 * @retval None
 */
static void RTC_CalendarConfig(void)
{
    stc_rtc_date_t stcRtcDate;
    stc_rtc_time_t stcRtcTime;

    /* Date configuration */
    stcRtcDate.u8Year    = 20U;
    stcRtcDate.u8Month   = RTC_MONTH_JANUARY;
    stcRtcDate.u8Day     = 1U;
    stcRtcDate.u8Weekday = RTC_WEEKDAY_WEDNESDAY;

    /* Time configuration */
    stcRtcTime.u8Hour   = 11U;
    stcRtcTime.u8Minute = 59U;
    stcRtcTime.u8Second = 55U;
    stcRtcTime.u8AmPm   = RTC_HOUR12_PM;

    if (Ok != RTC_SetDate(RTC_DATA_FORMAT_DEC, &stcRtcDate))
    {
        printf("Set Date failed!\r\n");
    }

    if (Ok != RTC_SetTime(RTC_DATA_FORMAT_DEC, &stcRtcTime))
    {
        printf("Set Time failed!\r\n");
    }
}

/**
 * @brief  RTC display weekday.
 * @param  [in] u8Weekday               Weekday
 *         This parameter can be one of the following values:
 *           @arg RTC_WEEKDAY_SUNDAY:     Sunday
 *           @arg RTC_WEEKDAY_MONDAY:     Monday
 *           @arg RTC_WEEKDAY_TUESDAY:    Tuesday
 *           @arg RTC_WEEKDAY_WEDNESDAY:  Wednesday
 *           @arg RTC_WEEKDAY_THURSDAY:   Thursday
 *           @arg RTC_WEEKDAY_FRIDAY:     Friday
 *           @arg RTC_WEEKDAY_SATURDAY:   Saturday
 * @retval None
 */
static void RTC_DisplayWeekday(uint8_t u8Weekday)
{
    switch (u8Weekday)
    {
        case RTC_WEEKDAY_SUNDAY:
            printf("Sunday\r\n");
            break;
        case RTC_WEEKDAY_MONDAY:
            printf("Monday\r\n");
            break;
        case RTC_WEEKDAY_TUESDAY:
            printf("Tuesday\r\n");
            break;
        case RTC_WEEKDAY_WEDNESDAY:
            printf("Wednesday\r\n");
            break;
        case RTC_WEEKDAY_THURSDAY:
            printf("Thursday\r\n");
            break;
        case RTC_WEEKDAY_FRIDAY:
            printf("Friday\r\n");
            break;
        case RTC_WEEKDAY_SATURDAY:
            printf("Saturday\r\n");
            break;
        default:
            break;
    }
}

/**
 * @brief  RTC configuration.
 * @param  None
 * @retval None
 */
static void RTC_Config(void)
{
    stc_rtc_init_t stcRtcInit;
    stc_rtc_alarm_t stcRtcAlarm;

    /* Reset RTC counter */
    if (ErrorTimeout == RTC_DeInit())
    {
        printf("Reset RTC failed!\r\n");
    }
    else
    {
        /* Configure structure initialization */
        RTC_StructInit(&stcRtcInit);

        /* Configuration RTC structure */
        stcRtcInit.u8ClockSource     = RTC_CLOCK_SOURCE_XTAL32;
        stcRtcInit.u8HourFormat      = RTC_HOUR_FORMAT_12;
        stcRtcInit.u8PeriodInterrupt = RTC_PERIOD_INT_ONE_SECOND;
        RTC_Init(&stcRtcInit);

        /* Configuration alarm clock time: Monday to Friday, PM 12:00 */
        stcRtcAlarm.u8AlarmHour    = 0x12U;
        stcRtcAlarm.u8AlarmMinute  = 0x00U;
        stcRtcAlarm.u8AlarmWeekday = RTC_ALARM_WEEKDAY_MONDAY    | RTC_ALARM_WEEKDAY_TUESDAY  |
                                     RTC_ALARM_WEEKDAY_WEDNESDAY | RTC_ALARM_WEEKDAY_THURSDAY |
                                     RTC_ALARM_WEEKDAY_FRIDAY;
        stcRtcAlarm.u8AlarmAmPm    = RTC_HOUR12_AM_HOUR24;
        RTC_SetAlarm(RTC_DATA_FORMAT_BCD, &stcRtcAlarm);
        RTC_AlarmCmd(Enable);

        /* Update date and time */
        RTC_CalendarConfig();
        /* RTC period and alarm interrupt configure */
        INTC_ShareIrqCmd(INT_RTC_ALM, Enable);
        INTC_ShareIrqCmd(INT_RTC_PRD, Enable);
        /* Clear pending */
        NVIC_ClearPendingIRQ(Int131_IRQn);
        /* Set priority */
        NVIC_SetPriority(Int131_IRQn, DDL_IRQ_PRIORITY_DEFAULT);
        /* Enable NVIC */
        NVIC_EnableIRQ(Int131_IRQn);
        /* Enable period and alarm interrupt */
        RTC_IntCmd((RTC_INT_PRDIE | RTC_INT_ALMIE), Enable);
        /* Startup RTC count */
        RTC_Cmd(Enable);
    }
}

/**
 * @brief  XTAL32 clock initialize.
 * @param  None
 * @retval None
 */
static void XTAL32_ClkInit(void)
{
    stc_clk_xtal32_init_t stcXtal32Init;

    /* Xtal32 config */
    stcXtal32Init.u8Xtal32State = CLK_XTAL32_ON;
    stcXtal32Init.u8Xtal32Drv   = CLK_XTAL32DRV_HIGH;
    stcXtal32Init.u8Xtal32NF    = CLK_XTAL32NF_PART;
    CLK_Xtal32Init(&stcXtal32Init);
    /* Waiting for XTAL32 stabilization */
    DDL_DelayMS(1000U);
}

/**
 * @brief  RTC display date and time.
 * @param  [in] u8Format                Specifies the format of the entered parameters.
 *         This parameter can be one of the following values:
 *           @arg RTC_DATA_FORMAT_DEC:  Decimal data format
 *           @arg RTC_DATA_FORMAT_BCD:  BCD data format
 * @param  [in] pcTitle                 Pointer to a title information of display
 * @param  [in] pstcRtcDate             Pointer to a @ref stc_rtc_date_t structure
 * @param  [in] pstcRtcTime             Pointer to a @ref stc_rtc_time_t structure
 * @retval None
 *
 */
static void RTC_DisplayDataTime(uint8_t u8Format, char *pcTitle, const stc_rtc_date_t *pstcRtcDate,
                                const stc_rtc_time_t *pstcRtcTime)
{
    if (RTC_DATA_FORMAT_BCD == u8Format)
    {
        printf("%s 20%02x/%02x/%02x %02x:%02x:%02x ", pcTitle, pstcRtcDate->u8Year, pstcRtcDate->u8Month,
                                                      pstcRtcDate->u8Day, pstcRtcTime->u8Hour,
                                                      pstcRtcTime->u8Minute, pstcRtcTime->u8Second);
    }
    else
    {
        printf("%s 20%02d/%02d/%02d %02d:%02d:%02d ", pcTitle, pstcRtcDate->u8Year, pstcRtcDate->u8Month,
                                                      pstcRtcDate->u8Day, pstcRtcTime->u8Hour,
                                                      pstcRtcTime->u8Minute, pstcRtcTime->u8Second);
    }

    if (RTC_HOUR12_AM_HOUR24 == pstcRtcTime->u8AmPm)
    {
        printf("Am ");
    }
    else
    {
        printf("Pm ");
    }
    RTC_DisplayWeekday(pstcRtcDate->u8Weekday);
}

/**
 * @brief  Main function of RTC Alarm.
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    stc_rtc_date_t stcCurrentDate;
    stc_rtc_time_t stcCurrentTime;

    /* Peripheral registers write unprotected */
    Peripheral_WE();
    /* Configure clock */
    BSP_CLK_Init();
    /* Reset the VBAT area */
    PWC_VBAT_Reset();
    XTAL32_ClkInit();
    /* Configure BSP */
    BSP_IO_Init();
    BSP_LED_Init();
    /* Configure UART */
    DDL_PrintfInit();
    /* Configure RTC */
    RTC_Config();
    /* Peripheral registers write protected */
    Peripheral_WP();

    while (1)
    {
        if (1U == u8SecIntFlag)
        {
            u8SecIntFlag = 0U;
            /* Print alarm information */
            if ((1U == u8AlarmIntFlag) && (u8AlarmCnt > 0U))
            {
                /* Alarm LED flicker */
                BSP_LED_Toggle(LED_RED);
                u8AlarmCnt--;
                if (0U == u8AlarmCnt)
                {
                    u8AlarmIntFlag = 0U;
                    BSP_LED_Off(LED_RED);
                }
                /* Get alarm date */
                if (Ok == RTC_GetDate(RTC_DATA_FORMAT_BCD, &stcCurrentDate))
                {
                    /* Get alarm time */
                    if (Ok == RTC_GetTime(RTC_DATA_FORMAT_BCD, &stcCurrentTime))
                    {
                        /* Print alarm date and time */
                        RTC_DisplayDataTime(RTC_DATA_FORMAT_BCD, "Alarm", &stcCurrentDate, &stcCurrentTime);
                    }
                    else
                    {
                        printf("Get alarm time failed!\r\n");
                    }
                }
                else
                {
                    printf("Get alarm date failed!\r\n");
                }
            }
            /* Print current date and time */
            else
            {
                /* Get current date */
                if (Ok == RTC_GetDate(RTC_DATA_FORMAT_DEC, &stcCurrentDate))
                {
                    /* Get current time */
                    if (Ok == RTC_GetTime(RTC_DATA_FORMAT_DEC, &stcCurrentTime))
                    {
                        /* Print current date and time */
                        RTC_DisplayDataTime(RTC_DATA_FORMAT_DEC, "Normal", &stcCurrentDate, &stcCurrentTime);
                    }
                    else
                    {
                        printf("Get time failed!\r\n");
                    }
                }
                else
                {
                    printf("Get date failed!\r\n");
                }
            }
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
