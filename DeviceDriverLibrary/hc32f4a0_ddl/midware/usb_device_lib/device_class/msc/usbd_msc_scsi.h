/**
 *******************************************************************************
 * @file  usbd_msc_scsi.h
 * @brief Header for the usbd_msc_scsi.c file
 *        
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12      Zhangxl         First version
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
#ifndef __USBD_MSC_SCSI_H__
#define __USBD_MSC_SCSI_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_def.h"

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_DEVICE_LIB
 * @{
 */

/**
 * @addtogroup USB_DEVICE_CLASS
 * @{
 */

/** @addtogroup USBD_MSC_SCSI
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 * @defgroup USBD_MSC_SCSI_Global_Types USBD MSC SCSI Global Types
 * @{
 */

typedef struct
{
    char Skey;
    union
    {
        struct _ASCs
        {
            char ASC;
            char ASCQ;
        }b;
        unsigned int ASC;
        char *       pData;
    } w;
} SCSI_Sense_TypeDef;
/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USBD_MSC_SCSI_Global_Macros USBD MSC SCSI Global Macros
 * @{
 */

#define SENSE_LIST_DEEPTH                           (4U)

/* SCSI Commands */
#define SCSI_FORMAT_UNIT                            (0x04U)
#define SCSI_INQUIRY                                (0x12U)
#define SCSI_MODE_SELECT6                           (0x15U)
#define SCSI_MODE_SELECT10                          (0x55U)
#define SCSI_MODE_SENSE6                            (0x1AU)
#define SCSI_MODE_SENSE10                           (0x5AU)
#define SCSI_ALLOW_MEDIUM_REMOVAL                   (0x1EU)
#define SCSI_READ6                                  (0x08U)
#define SCSI_READ10                                 (0x28U)
#define SCSI_READ12                                 (0xA8U)
#define SCSI_READ16                                 (0x88U)

#define SCSI_READ_CAPACITY10                        (0x25U)
#define SCSI_READ_CAPACITY16                        (0x9EU)

#define SCSI_REQUEST_SENSE                          (0x03U)
#define SCSI_START_STOP_UNIT                        (0x1BU)
#define SCSI_TEST_UNIT_READY                        (0x00U)
#define SCSI_WRITE6                                 (0x0AU)
#define SCSI_WRITE10                                (0x2AU)
#define SCSI_WRITE12                                (0xAAU)
#define SCSI_WRITE16                                (0x8AU)

#define SCSI_VERIFY10                               (0x2FU)
#define SCSI_VERIFY12                               (0xAFU)
#define SCSI_VERIFY16                               (0x8FU)

#define SCSI_SEND_DIAGNOSTIC                        (0x1DU)
#define SCSI_READ_FORMAT_CAPACITIES                 (0x23U)

#define NO_SENSE                                    (0U)
#define RECOVERED_ERROR                             (1U)
#define NOT_READY                                   (2U)
#define MEDIUM_ERROR                                (3U)
#define HARDWARE_ERROR                              (4U)
#define ILLEGAL_REQUEST                             (5U)
#define UNIT_ATTENTION                              (6U)
#define DATA_PROTECT                                (7U)
#define BLANK_CHECK                                 (8U)
#define VENDOR_SPECIFIC                             (9U)
#define COPY_ABORTED                                (10U)
#define ABORTED_COMMAND                             (11U)
#define VOLUME_OVERFLOW                             (13U)
#define MISCOMPARE                                  (14U)

#define INVALID_CDB                                 (0x20U)
#define INVALID_FIELED_IN_COMMAND                   (0x24U)
#define PARAMETER_LIST_LENGTH_ERROR                 (0x1AU)
#define INVALID_FIELD_IN_PARAMETER_LIST             (0x26U)
#define ADDRESS_OUT_OF_RANGE                        (0x21U)
#define MEDIUM_NOT_PRESENT                          (0x3AU)
#define MEDIUM_HAVE_CHANGED                         (0x28U)
#define WRITE_PROTECTED                             (0x27U)
#define UNRECOVERED_READ_ERROR                      (0x11U)
#define WRITE_FAULT                                 (0x03U)

#define READ_FORMAT_CAPACITY_DATA_LEN               (0x0CU)
#define READ_CAPACITY10_DATA_LEN                    (0x08U)
#define MODE_SENSE10_DATA_LEN                       (0x08U)
#define MODE_SENSE6_DATA_LEN                        (0x04U)
#define REQUEST_SENSE_DATA_LEN                      (0x12U)
#define STANDARD_INQUIRY_DATA_LEN                   (0x24U)
#define BLKVFY                                      (0x04U)
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
/**
 * @addtogroup USBD_MSC_SCSI_Global_Variables
 * @{
 */

extern SCSI_Sense_TypeDef SCSI_Sense [SENSE_LIST_DEEPTH];
extern uint8_t SCSI_Sense_Head;
extern uint8_t SCSI_Sense_Tail;
/**
 * @}
 */

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup USBD_MSC_SCSI_Global_Functions
 * @{
 */
int8_t SCSI_ProcessCmd(USB_OTG_CORE_HANDLE  *pdev,
                       uint8_t lun,
                       uint8_t *cmd);

void SCSI_SenseCode(uint8_t lun,
                      uint8_t sKey,
                      uint8_t ASC);
/**
 * @}
 */

/**
 * @}
 */

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

#endif /* __USBD_MSC_SCSI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
