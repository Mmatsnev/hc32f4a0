/**
 *******************************************************************************
 * @file  usart/lin_master/source/lin.h
 * @brief This file contains all the functions prototypes of the LIN midware
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
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
#ifndef __LIN_H__
#define __LIN_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USART_LIN_Master
 * @{
 */

#if (DDL_CLK_ENABLE == DDL_OFF)
#error "please configure macro definition DDL_CLK_ENABLE to DDL_ON in ddl_config.h"
#endif

#if (DDL_GPIO_ENABLE == DDL_OFF)
#error "please configure macro definition DDL_GPIO_ENABLE to DDL_ON in ddl_config.h"
#endif

#if (DDL_INTERRUPTS_ENABLE == DDL_OFF)
#error "please configure macro definition DDL_INTERRUPTS_ENABLE to DDL_ON in ddl_config.h"
#endif

#if (DDL_USART_ENABLE == DDL_OFF)
#error "please configure macro definition DDL_USART_ENABLE to DDL_ON in ddl_config.h"
#endif

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup USART_LIN_Master_Global_Types USART LIN Master Global Types
 * @{
 */

/**
 * @brief LIN state enumeration definition
 */
typedef enum
{
    LinStateSleep  = 0U,                /*!< Idle */
    LinStateWakeup = 1U,                /*!< Frame Break field */
} en_lin_state_t;

/**
 * @brief LIN frame state enumeration definition
 */
typedef enum
{
    LinFrameStateIdle     = 0U,         /*!< Frame idle */
    LinFrameStateBreak    = 1U,         /*!< Frame Break field */
    LinFrameStateSync     = 2U,         /*!< Frame Sync field  */
    LinFrameStatePID      = 3U,         /*!< Frame PID field  */
    LinFrameStateData     = 4U,         /*!< Frame Data  */
    LinFrameStateChecksum = 5U,         /*!< Frame Checksum */
} en_lin_frame_state_t;

/**
 * @brief LIN error code enumeration definition
 */
typedef enum
{
    LinOk          = 0U,                /*!< No error */
    LinErrBreak    = 1U,                /*!< Error break */
    LinErrPID      = 2U,                /*!< Error PID */
    LinErrChecksum = 3U,                /*!< Error checksum */
} lin_err_t;

/**
 * @brief LIN farme data structure definition
 */
typedef struct
{
    __IO en_lin_frame_state_t enState;  /*!< LIN frame state
                                             This parameter can be a value of @ref en_lin_frame_state_t */

    uint8_t u8ID;                       /*!< ID-Number for Frame */
    uint8_t u8PID;                      /*!< Protected ID-Number for Frame */
    uint8_t u8Length;                   /*!< Data lenght */
    uint8_t au8Data[8];                 /*!< Data array */
    uint8_t u8Checksum;                 /*!< Data checksum */

    uint8_t u8XferCnt;                  /*!< Transfer data count */
    uint8_t u8Error;                    /*!< Error code
                                             This parameter can be a value of @ref lin_err_t */
} stc_lin_frame_t;

/**
 * @brief LIN interrupt number configure structure definition
 */
typedef struct
{

    IRQn_Type   RxIntIRQn;          /*!< USART RX interrupt interrupt IRQ number
                                         This parameter can be a value of @ref IRQn_Type */

    IRQn_Type   RxErrIntIRQn;       /*!< USART RX error interrupt IRQ number
                                         This parameter can be a value of @ref IRQn_Type */

    IRQn_Type   BreakWkupIntIRQn;   /*!< USART RX interrupt interrupt IRQ number
                                         This parameter can be a value of @ref IRQn_Type */
} stc_lin_int_cfg_t;

/**
 * @brief LIN pin configure structure definition
 */
typedef struct
{
    uint8_t  u8RxPort;              /*!< GPIO_PORT_x, x can be (A~I) to select the GPIO peripheral */

    uint16_t u16RxPin;              /*!< GPIO_PIN_x, x can be (00~15) to select the PIN index */

    uint8_t  u8RxPinFunc;           /*!< GPIO_FUNC_x, x can be selected from GPIO function definitions */

    uint8_t  u8TxPort;              /*!< GPIO_PORT_x, x can be (A~I) to select the GPIO peripheral */

    uint16_t u16TxPin;              /*!< GPIO_PIN_x, x can be (00~15) to select the PIN index */

    uint8_t  u8TxPinFunc;           /*!< GPIO_FUNC_x, x can be selected from GPIO function definitions */
} stc_lin_pin_cfg_t;

/**
 * @brief LIN mode initialization structure definition
 */
typedef struct
{
    M4_USART_TypeDef     *USARTx;       /*!< USART LIN instance
                                             This parameter can be M4_USARTx(x = 5,10) */

    uint32_t             u32LinMode;    /*!< LIN mode
                                             This parameter can be a value of @ref LIN_Mode */

    stc_usart_lin_init_t stcLinInit;    /*!< LIN function initialization structure
                                             This structure details refer @ref stc_usart_lin_init_t */

    stc_lin_pin_cfg_t    stcPinCfg;     /*!< LIN pin configure structure
                                             This structure details refer @ref stc_lin_pin_cfg_t */

    stc_lin_int_cfg_t    stcIrqnCfg;    /*!< LIN function IRQ number configure structure
                                             This structure details refer @ref stc_lin_int_cfg_t */

    en_lin_state_t       enLinState;    /*!< LIN state
                                             This parameter can be a value of @ref en_lin_state_t */

    stc_lin_frame_t      *pstcFrame;    /*!< LIN frame
                                             This structure details refer @ref stc_lin_frame_t */
} stc_lin_hanlde_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USART_LIN_Master_Global_Macros USART LIN Master Global Macros
 * @{
 */

/**
 * @defgroup LIN_Mode LIN Mode
 * @{
 */
#define LIN_MASTER                      (0UL)       /*!< LIN master */
#define LIN_SLAVE                       (1UL)       /*!< LIN slave */
/**
 * @}
 */

/**
 * @defgroup LIN_Fram_Data_Length LIN Fram Data Length
 * @{
 */
#define LIN_FRAME_DATA_BYTES_2          (1U << 4)   /*!< Frame data length:2 bytes */
#define LIN_FRAME_DATA_BYTES_4          (2U << 4)   /*!< Frame data length:4 bytes */
#define LIN_FRAME_DATA_BYTES_8          (3U << 4)   /*!< Frame data length:8 bytes */
/**
 * @}
 */

/**
 * @defgroup LIN_Receive_Waiting_Time LIN Receive Waiting Time
 * @{
 */
#define LIN_REC_WAITING_FOREVER         (-1)
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup USART_LIN_Master_Global_Functions
 * @{
 */

en_result_t LIN_Init(stc_lin_hanlde_t *pstcLinHandle);

/* LIN master mode */
en_result_t LIN_MASTER_SendFrame(stc_lin_hanlde_t *pstcLinHandle,
                                        stc_lin_frame_t *pstcFrame);
en_result_t LIN_MASTER_RecFrame(stc_lin_hanlde_t *pstcLinHandle,
                                    stc_lin_frame_t *pstcFrame,
                                    int32_t i32Timeout);

/* LIN slave mode */
en_result_t LIN_SLAVE_RecFrameHeader(stc_lin_hanlde_t *pstcLinHandle,
                                            stc_lin_frame_t *pstcFrame,
                                            int32_t i32Timeout);
en_result_t LIN_SLAVE_RecFrameResponse(const stc_lin_hanlde_t *pstcLinHandle,
                                                int32_t i32Timeout);
en_result_t LIN_SLAVE_SendFrameResponse(stc_lin_hanlde_t *pstcLinHandle);

en_result_t LIN_SetState(stc_lin_hanlde_t *pstcLinHandle,
                            en_lin_state_t enState);
en_lin_state_t LIN_GetState(const stc_lin_hanlde_t *pstcLinHandle);
en_result_t LIN_Sleep(stc_lin_hanlde_t *pstcLinHandle);
en_result_t LIN_SendWakeupSignal(const stc_lin_hanlde_t *pstcLinHandle);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __LIN_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
