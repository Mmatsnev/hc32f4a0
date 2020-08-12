/**
 *******************************************************************************
 * @file  hc32f4a0_usb_otg.c
 * @brief USB Core Layer.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "hc32f4a0_usb_defines.h"
#include "hc32f4a0_usb_otg_regs.h"
#include "hc32f4a0_usb_core.h"
#include "hc32f4a0_usb_otg.h"

/**
 * @addtogroup HC32F4A0_DDL_Driver
 * @{
 */

/**
 * @defgroup DDL_USB_OTG USB OTG
 * @brief USB Core Layer.
 * @{
 */

#if (DDL_USBFS_ENABLE == DDL_ON)

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
/**
 * @defgroup USB_OTG_Local_Functions USB OTG Local Functions
 * @{
 */
#ifdef USE_OTG_MODE
static uint32_t USB_OTG_HandleConnectorIDStatusChange_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t USB_OTG_HandleSessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev);
static uint32_t USB_OTG_Read_itr(USB_OTG_CORE_HANDLE *pdev);
#endif
/**
 * @}
 */
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @defgroup USB_OTG_Global_Functions USB OTG Global Functions
 * @{
 */

#ifdef USE_OTG_MODE

/**
 * @brief  USBO_OTG_ISR_Handler
 *
 * @param  [in] pdev     Selected device
 * @retval uint32_t
 */
uint32_t USBO_OTG_ISR_Handler(USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t retval = 0UL;
    USB_OTG_GINTSTS_TypeDef  gintsts ;
    gintsts.d32 = 0UL;

    gintsts.d32 = USB_OTG_Read_itr(pdev);
    if (gintsts.d32 == 0UL)
    {
        //return 0UL;   /* MISRAC 2004*/
        retval = 0UL;
    }

#ifdef USE_OTG_MODE
    else if (gintsts.b.conidstschng)
    {
        retval |= USB_OTG_HandleConnectorIDStatusChange_ISR(pdev);
    }
    else if (gintsts.b.vbusvint)
    {
        retval |= USB_OTG_HandleSessionRequest_ISR(pdev);
    }
#endif
    return retval;
}

/**
 * @brief  USB_OTG_InitiateSRP
 *         Initiate an srp session
 * @param  [in] pdev     Selected device
 * @retval None
 */
void USB_OTG_InitiateSRP(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GOTGCTL_TypeDef  otgctl;

    otgctl.d32 = 0UL;

    otgctl.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GOTGCTL );
    if (otgctl.b.sesreq)
    {
        return; /* SRP in progress */
    }
    otgctl.b.sesreq = 1U;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GOTGCTL, otgctl.d32);
}


/**
 * @brief  USB_OTG_InitiateHNP
 *         Initiate HNP
 * @param  [in] pdev     Selected device
 * @param  [in] mode     Host or device mode
 * @retval None
 */
void USB_OTG_InitiateHNP(USB_OTG_CORE_HANDLE *pdev , uint8_t state, uint8_t mode)
{
    USB_OTG_GOTGCTL_TypeDef   otgctl;
    USB_OTG_HPRT0_TypeDef    hprt0;

    otgctl.d32 = 0UL;
    hprt0.d32  = 0UL;

    otgctl.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GOTGCTL );
    if (mode)
    {
        /* Device mode */
        if (state)
        {
            otgctl.b.devhnpen = 1U; /* B-Dev has been enabled to perform HNP         */
            otgctl.b.hnpreq   = 1U; /* Initiate an HNP req. to the connected USB host*/
            USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GOTGCTL, otgctl.d32);
        }
    }
    else
    {
        /* Host mode */
        if (state)
        {
            otgctl.b.hstsethnpen = 1U; /* A-Dev has enabled B-device for HNP       */
            USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GOTGCTL, otgctl.d32);
            /* Suspend the bus so that B-dev will disconnect indicating the initial condition for HNP to DWC_Core */
            hprt0.d32  = USB_OTG_ReadHPRT0(pdev);
            hprt0.b.prtsusp = 1U; /* The core clear this bit when disconnect interrupt generated (GINTSTS.DisconnInt = '1') */
            USB_OTG_WRITE_REG32(pdev->regs.HPRT0, hprt0.d32);
        }
    }
}

/**
 * @brief  USB_OTG_GetCurrentState
 *         Return current OTG State
  * @param pdev     Selected device
 * @retval None
 */
uint32_t USB_OTG_GetCurrentState (USB_OTG_CORE_HANDLE *pdev)
{
    return pdev->otg.OTG_State;
}

#endif

/**
 * @}
 */


/**
 * @addtogroup USB_OTG_Local_Functions USB OTG Local Functions
 * @{
 */


#ifdef  USE_OTG_MODE

/**
 * @brief  USB_OTG_HandleConnectorIDStatusChange_ISR
 *         handles the Connector ID Status Change Interrupt
 * @param  [in] pdev     Selected device
 * @retval Status
 */
static uint32_t USB_OTG_HandleConnectorIDStatusChange_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTMSK_TypeDef  gintmsk;
    USB_OTG_GOTGCTL_TypeDef   gotgctl;
    USB_OTG_GINTSTS_TypeDef  gintsts;

    gintsts.d32 = 0UL ;
    gintmsk.d32 = 0UL ;
    gotgctl.d32 = 0UL ;
    gintmsk.b.sofintr = 1U;

    USB_OTG_MODIFY_REG32(&pdev->regs.GREGS->GINTMSK, gintmsk.d32, 0U);
    gotgctl.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GOTGCTL);

    /* B-Device connector (Device Mode) */
    if (gotgctl.b.conidsts)
    {
        USB_OTG_DisableGlobalInt(pdev);
        USB_OTG_CoreInitDev(pdev);
        USB_OTG_EnableGlobalInt(pdev);
        pdev->otg.OTG_State = B_PERIPHERAL;
    }
    else
    {
        USB_OTG_DisableGlobalInt(pdev);
        USB_OTG_CoreInitHost(pdev);
        USB_OTG_EnableGlobalInt(pdev);
        pdev->otg.OTG_State = A_HOST;
        USB_OTG_DriveVbus(pdev, 1U);
    }
    /* Set flag and clear interrupt */
    gintsts.b.conidstschng = 1U;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1UL;
}

/**
 * @brief  USB_OTG_HandleSessionRequest_ISR
 *           Initiating the Session Request Protocol
  * @param  [in] pdev     Selected device
 * @retval Status
 */
static uint32_t USB_OTG_HandleSessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_GOTGCTL_TypeDef   gotgctl;

#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("SRP IRQ\n");
#endif
    gotgctl.d32 = 0UL;
    gintsts.d32 = 0UL;

    gotgctl.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GOTGCTL );
    if (USB_OTG_IsDeviceMode(pdev) && (gotgctl.b.bsesvld))
    {
    }
    else if (gotgctl.b.asesvld)
    {
    }
    USB_OTG_DriveVbus(pdev, 1U);
    /* Clear interrupt */
    gintsts.d32 = 0UL;
    gintsts.b.vbusvint = 1U;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1UL;
}

/**
 * @brief  USB_OTG_Read_itr
 *         returns the Core Interrupt register
 * @param  [in] pdev     Selected device
 * @retval Status
 */
static uint32_t USB_OTG_Read_itr(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_GINTMSK_TypeDef  gintmsk;
    USB_OTG_GINTMSK_TypeDef  gintmsk_common;

    gintsts.d32 = 0UL;
    gintmsk.d32 = 0UL;
    gintmsk_common.d32 = 0UL;

    /* OTG interrupts */
    gintmsk_common.b.vbusvint = 1U;
    gintmsk_common.b.conidstschng = 1U;

    gintsts.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS);
    gintmsk.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTMSK);
    return ((gintsts.d32 & gintmsk.d32 ) & gintmsk_common.d32);
}

#endif

/**
 * @}
 */

#endif /* DDL_USBFS_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
