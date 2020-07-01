/**
 *******************************************************************************
 * @file  hc32f4a0_usb_defines.h
 * @brief A detailed description is available at  Header of the Core Layer
 *        
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Wangmin         First version
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

#ifndef __HC32F4A0_USB_DEFINES_H__
#define __HC32F4A0_USB_DEFINES_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_conf.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @addtogroup DDL_USB_OTG
 * @{
 */

#if (DDL_USBFS_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup USB_defines_Global_Types USB Defines Global Types
 * @{
 */

/**
 * @brief USB_DEFINES_Exported_Types
 */
typedef enum
{
    USB_OTG_HS_CORE_ID = 0U,
    USB_OTG_FS_CORE_ID = 1U
}USB_OTG_CORE_ID_TypeDef;

/**
 * @brief ENUMERATION TYPE
 */
enum USB_OTG_SPEED {
    USB_SPEED_UNKNOWN = 0U,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH
};

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 * @defgroup USB_Defines_Global_Macros USB Defines Global Macros
 * @{
 */

/**
 * @defgroup __CORE_DEFINES_ Defines for USB CORE
 * @{
 */
#define USB_OTG_SPEED_PARAM_HIGH            (0U)
#define USB_OTG_SPEED_PARAM_HIGH_IN_FULL    (1U)
#define USB_OTG_SPEED_PARAM_FULL            (3U)

#define USB_OTG_SPEED_HIGH                  (0U)
#define USB_OTG_SPEED_FULL                  (1U)

#define USB_OTG_ULPI_PHY                    (1U)
#define USB_OTG_EMBEDDED_PHY                (2U)
/**
 * @}
 */

/**
 * @defgroup __GLOBAL_DEFINES_ Defines for USB Global Register Function
 * @{
 */
#define GAHBCFG_GLBINT_ENABLE                  (1U)
#define GAHBCFG_INT_DMA_BURST_SINGLE           (0U)
#define GAHBCFG_INT_DMA_BURST_INCR             (1U)
#define GAHBCFG_INT_DMA_BURST_INCR4            (3U)
#define GAHBCFG_INT_DMA_BURST_INCR8            (5U)
#define GAHBCFG_INT_DMA_BURST_INCR16           (7U)
#define GAHBCFG_DMAENABLE                      (1U)
#define GAHBCFG_TXFEMPTYLVL_EMPTY              (1U)
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          (0U)
#define GRXSTS_PKTSTS_IN                       (2U)
#define GRXSTS_PKTSTS_IN_XFER_COMP             (3U)
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          (5U)
#define GRXSTS_PKTSTS_CH_HALTED                (7U)

#define SELF_POWERED                      (1UL << 6U)
#define REMOTE_WAKEUP                     (1UL << 5U)
/**
 * @}
 */

/**
 * @defgroup __OnTheGo_DEFINES_ Defines for USB OTG Register Function
 * @{
 */
#define MODE_HNP_SRP_CAPABLE                   (0U)
#define MODE_SRP_ONLY_CAPABLE                  (1U)
#define MODE_NO_HNP_SRP_CAPABLE                (2U)
#define MODE_SRP_CAPABLE_DEVICE                (3U)
#define MODE_NO_SRP_CAPABLE_DEVICE             (4U)
#define MODE_SRP_CAPABLE_HOST                  (5U)
#define MODE_NO_SRP_CAPABLE_HOST               (6U)
#define A_HOST                                 (1U)
#define A_SUSPEND                              (2U)
#define A_PERIPHERAL                           (3U)
#define B_PERIPHERAL                           (4U)
#define B_HOST                                 (5U)
#define DEVICE_MODE                            (0U)
#define HOST_MODE                              (1U)
#define OTG_MODE                               (2U)
/**
 * @}
 */

/**
 * @defgroup __DEVICE_DEFINES_ Defines for USB Device Register Function
 * @{
 */
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     (0U)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     (1U)
#define DSTS_ENUMSPD_LS_PHY_6MHZ               (2U)
#define DSTS_ENUMSPD_FS_PHY_48MHZ              (3U)

#define DCFG_FRAME_INTERVAL_80                 (0U)
#define DCFG_FRAME_INTERVAL_85                 (1U)
#define DCFG_FRAME_INTERVAL_90                 (2U)
#define DCFG_FRAME_INTERVAL_95                 (3U)

#define DEP0CTL_MPS_64                         (0U)
#define DEP0CTL_MPS_32                         (1U)
#define DEP0CTL_MPS_16                         (2U)
#define DEP0CTL_MPS_8                          (3U)

#define EP_SPEED_LOW                           (0U)
#define EP_SPEED_FULL                          (1U)
#define EP_SPEED_HIGH                          (2U)

#define EP_TYPE_CTRL                           (0U)
#define EP_TYPE_ISOC                           (1U)
#define EP_TYPE_BULK                           (2U)
#define EP_TYPE_INTR                           (3U)
#define EP_TYPE_MSK                            (3U)

#define STS_GOUT_NAK                           (1U)
#define STS_DATA_UPDT                          (2U)
#define STS_XFER_COMP                          (3U)
#define STS_SETUP_COMP                         (4U)
#define STS_SETUP_UPDT                         (6U)
/**
 * @}
 */

/**
 * @defgroup __HOST_DEFINES_ Defines for USB Host Register Function
 * @{
 */
#define HC_PID_DATA0                           (0U)
#define HC_PID_DATA2                           (1U)
#define HC_PID_DATA1                           (2U)
#define HC_PID_SETUP                           (3U)

#define HPRT0_PRTSPD_HIGH_SPEED                (0U)
#define HPRT0_PRTSPD_FULL_SPEED                (1U)
#define HPRT0_PRTSPD_LOW_SPEED                 (2U)

#define HCFG_30_60_MHZ                         (0U)
#define HCFG_48_MHZ                            (1U)
#define HCFG_6_MHZ                             (2U)

#define HCCHAR_CTRL                            (0U)
#define HCCHAR_ISOC                            (1U)
#define HCCHAR_BULK                            (2U)
#define HCCHAR_INTR                            (3U)
/**
 * @}
 */

#ifndef __MIN
#define  __MIN(a, b)      (((a) < (b)) ? (a) : (b))
#endif
/**
 * @}
 */

/**
 * @defgroup Internal_Macro USB internal Macro
 * @{
 */
#define USB_OTG_READ_REG32(reg)  (*(__IO uint32_t *)(reg))
#define USB_OTG_WRITE_REG32(reg,value) (*(__IO uint32_t *)(reg) = (value))
#define USB_OTG_MODIFY_REG32(reg,clear_mask,set_mask) (USB_OTG_WRITE_REG32((reg), (((USB_OTG_READ_REG32((reg))) & ~(clear_mask)) | (set_mask))))

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/


#endif /* DDL_USBFS_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4A0_USB_DEFINES_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
