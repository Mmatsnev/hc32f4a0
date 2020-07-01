/**
 *******************************************************************************
 * @file  usb/usbh_mouse_kb/source/usbh_usr.c
 * @brief This file includes the user application layer.
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
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "hc32_ddl.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup USBH_MOUSE_KB
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define BUTTON_PORT         (PortD)
#define BUTTON_PIN          (Pin03)

#define GET_BUTTON_KEY()    (PORT_GetBit(PortD, Pin03))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
extern USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_cb =
{
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    NULL,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
};

/*--------------- Messages ---------------*/
const char* MSG_HOST_INIT          = "> Host Library Initialized\n";
const char* MSG_DEV_ATTACHED       = "> Device Attached\n";
const char* MSG_DEV_DISCONNECTED   = "> Device Disconnected\n";
const char* MSG_DEV_ENUMERATED     = "> Enumeration completed\n";
const char* MSG_DEV_HIGHSPEED      = "> High speed device detected\n";
const char* MSG_DEV_FULLSPEED      = "> Full speed device detected\n";
const char* MSG_DEV_LOWSPEED       = "> Low speed device detected\n";
const char* MSG_DEV_ERROR          = "> Device fault \n";

const char* MSG_MSC_CLASS          = "> Mass storage device connected\n";
const char* MSG_HID_CLASS          = "> HID device connected\n";

const char* USB_HID_MouseStatus    = "> HID Demo Device : Mouse\n";
const char* USB_HID_KeybrdStatus   = "> HID Demo Device : Keyboard\n";
const char* MSG_UNREC_ERROR        = "> UNRECOVERED ERROR STATE\n";

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
* @brief  USBH_USR_Init
*         Displays the message on terminal for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
    static uint8_t startup = 0;
    if(startup == 0 )
    {
        startup = 1;
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf("> USB Host library started.\n");
#endif
    }
}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on terminal on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf((void*)MSG_DEV_ATTACHED);
#endif
}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf((void*)MSG_UNREC_ERROR);
#endif
}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf((void *)MSG_DEV_DISCONNECTED);
#endif
}

/**
* @brief  USBH_USR_ResetUSBDevice
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  DeviceSpeed   Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
    {
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf((void *)MSG_DEV_HIGHSPEED);
#endif
    }
    else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
    {
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf((void *)MSG_DEV_FULLSPEED);
#endif
    }
    else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
    {
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf((void *)MSG_DEV_LOWSPEED);
#endif
    }
    else
    {
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf((void *)MSG_DEV_ERROR);
#endif
    }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on terminal for device descriptor
* @param  DeviceDesc    device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
    uint8_t temp[50];
    USBH_DevDesc_TypeDef *hs;
    hs = DeviceDesc;
#if (DDL_PRINT_ENABLE == DDL_ON)
    sprintf((char *)temp , "VID : %04lXh\n" , (uint32_t)(*hs).idVendor);
    printf((void *)temp);
    sprintf((char *)temp , "PID : %04lXh\n" , (uint32_t)(*hs).idProduct);
    printf((void *)temp);
#endif
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}

/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  cfgDesc   Configuration descriptor
* @param  itfDesc   Interface descriptor
* @param  epDesc    Endpoint descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
    USBH_InterfaceDesc_TypeDef *id;

    id = itfDesc;

    if((*id).bInterfaceClass  == 0x08)
    {
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf("%s", MSG_MSC_CLASS);
#endif
    }
    else if((*id).bInterfaceClass  == 0x03)
    {
#if (DDL_PRINT_ENABLE == DDL_ON)
        printf("%s", MSG_HID_CLASS);
#endif
    }
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    char temp[100];
#if (DDL_PRINT_ENABLE == DDL_ON)
    sprintf(temp, "Manufacturer : %s\n", (char *)ManufacturerString);
    printf((void *)temp);
#endif
}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  ProductString     Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
    char temp[100];
#if (DDL_PRINT_ENABLE == DDL_ON)
    sprintf((char *)temp, "Product : %s\n", (char *)ProductString);
    printf((void *)temp);
#endif
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNumString   SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    uint8_t temp[100];
#if (DDL_PRINT_ENABLE == DDL_ON)
    sprintf((char *)temp, "Serial Number : %s\n", (char *)SerialNumString);
    printf((void *)temp);
#endif
}

/**
* @brief  EnumerationDone
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    /* Enumeration complete */
    printf((void *)MSG_DEV_ENUMERATED);
#endif
}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("> Device not supported.\n");
#endif
}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status   User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

    USBH_USR_Status usbh_usr_status = USBH_USR_NO_RESP;
#if 0
    usbh_usr_status = USBH_USR_NO_RESP;

    /*Key PD3 is in polling mode to detect user action */
    if(GET_BUTTON_KEY() == Reset)
    {
        usbh_usr_status = USBH_USR_RESP_OK;
    }
#endif
    return usbh_usr_status;
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected (void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("Overcurrent detected.\n");
#endif
}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/
void USR_MOUSE_Init	(void)
{
    HID_MOUSE_ButtonReleased(0);
    HID_MOUSE_ButtonReleased(1);
    HID_MOUSE_ButtonReleased(2);
}

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data      Mouse data to be displayed
* @retval None
*/
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{

    uint8_t idx = 1U;
    static uint8_t b_state[3U] = { 0U, 0U , 0U};

    if ((data->x != 0U) && (data->y != 0U))
    {
        HID_MOUSE_UpdatePosition(data->x , data->y);
    }

    for ( idx = 0U ; idx < 3U ; idx ++)
    {

        if(data->button & (0x01U << idx))
        {
            if(b_state[idx] == 0U)
            {
                HID_MOUSE_ButtonPressed (idx);
                b_state[idx] = 1U;
            }
        }
        else
        {
            if(b_state[idx] == 1U)
            {
                HID_MOUSE_ButtonReleased (idx);
                b_state[idx] = 0U;
            }
        }
    }
}

/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void  USR_KEYBRD_Init (void)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf((void*)USB_HID_KeybrdStatus);
    printf("> Use Keyboard to tape characters: \n\n");
    printf("\n\n\n\n\n\n");
#endif
//    KeybrdCharXpos = KYBRD_FIRST_LINE;
//    KeybrdCharYpos = KYBRD_FIRST_COLUMN;
}


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data      Keyboard data to be displayed
* @retval None
*/
void  USR_KEYBRD_ProcessData (uint8_t data)
{
#if (DDL_PRINT_ENABLE == DDL_ON)
    printf("%c", (char)data);
#endif
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
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
