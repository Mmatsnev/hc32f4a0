/**
 *******************************************************************************
 * @file  usbh_def.h
 * @brief Definitions used in the USB host library.
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
#ifndef __USBH_DEF_H__
#define __USBH_DEF_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
 * Include files
 ******************************************************************************/

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_HOST_LIB
 * @{
 */

/**
 * @addtogroup USB_HOST_CORE
 * @{
 */

/** @defgroup USBH_DEF USB host define
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup USBH_DEF_Global_Types USBH Define Global Types
 * @{
 */
/*  This Union is copied from usb_core.h  */
typedef union
{
    uint16_t w;
    struct BW
    {
        uint8_t msb;
        uint8_t lsb;
    }
    bw;
}uint16_t_uint8_t;

typedef union _USB_Setup
{
    uint8_t d8[8];

    struct _SetupPkt_Struc
    {
        uint8_t           bmRequestType;
        uint8_t           bRequest;
        uint16_t_uint8_t  wValue;
        uint16_t_uint8_t  wIndex;
        uint16_t_uint8_t  wLength;
    } b;
} USB_Setup_TypeDef;

typedef  struct  _DescHeader
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
} USBH_DescHeader_t;

typedef struct
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  bcdUSB;        /* USB Specification Number which device complies too */
    uint8_t   bDeviceClass;
    uint8_t   bDeviceSubClass;
    uint8_t   bDeviceProtocol;
    /* If equal to Zero, each interface specifies its own class
    code if equal to 0xFF, the class code is vendor specified.
    Otherwise field is valid Class Code.*/
    uint8_t   bMaxPacketSize;
    uint16_t  idVendor;      /* Vendor ID (Assigned by USB Org) */
    uint16_t  idProduct;     /* Product ID (Assigned by Manufacturer) */
    uint16_t  bcdDevice;     /* Device Release Number */
    uint8_t   iManufacturer;  /* Index of Manufacturer String Descriptor */
    uint8_t   iProduct;       /* Index of Product String Descriptor */
    uint8_t   iSerialNumber;  /* Index of Serial Number String Descriptor */
    uint8_t   bNumConfigurations; /* Number of Possible Configurations */
}USBH_DevDesc_TypeDef;


typedef struct
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  wTotalLength;        /* Total Length of Data Returned */
    uint8_t   bNumInterfaces;       /* Number of Interfaces */
    uint8_t   bConfigurationValue;  /* Value to use as an argument to select this configuration*/
    uint8_t   iConfiguration;       /*Index of String Descriptor Describing this configuration */
    uint8_t   bmAttributes;         /* D7 Bus Powered , D6 Self Powered, D5 Remote Wakeup , D4..0 Reserved (0)*/
    uint8_t   bMaxPower;            /*Maximum Power Consumption */
}USBH_CfgDesc_TypeDef;


typedef struct
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  bcdHID;               /* indicates what endpoint this descriptor is describing */
    uint8_t   bCountryCode;        /* specifies the transfer type. */
    uint8_t   bNumDescriptors;     /* specifies the transfer type. */
    uint8_t   bReportDescriptorType;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
    uint16_t  wItemLength;          /* is used to specify the polling interval of certain transfers. */
}USBH_HIDDesc_TypeDef;


typedef struct
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;    /* Value used to select alternative setting */
    uint8_t bNumEndpoints;        /* Number of Endpoints used for this interface */
    uint8_t bInterfaceClass;      /* Class Code (Assigned by USB Org) */
    uint8_t bInterfaceSubClass;   /* Subclass Code (Assigned by USB Org) */
    uint8_t bInterfaceProtocol;   /* Protocol Code */
    uint8_t iInterface;           /* Index of String Descriptor Describing this interface */

}USBH_InterfaceDesc_TypeDef;


typedef struct
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint8_t   bEndpointAddress;   /* indicates what endpoint this descriptor is describing */
    uint8_t   bmAttributes;       /* specifies the transfer type. */
    uint16_t  wMaxPacketSize;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
    uint8_t   bInterval;          /* is used to specify the polling interval of certain transfers. */
}USBH_EpDesc_TypeDef;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup USBH_DEF_Global_Macros USBH Define Global Macros
 * @{
 */
#ifndef USBH_NULL
#define USBH_NULL ((void *)0U)
#endif

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef TRUE
#define TRUE 1U
#endif

#define ValBit(VAR,POS)                               ((VAR) & (1UL << (POS)))
#define SetBit(VAR,POS)                               ((VAR) |= (1UL << (POS)))
#define ClrBit(VAR,POS)                               ((VAR) &= ((1UL << (POS))^255U))

#define  LE16(addr)   (((uint16_t)(*((uint8_t *)(addr)))) + (((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))

#define  USB_LEN_DESC_HDR                               (0x02U)
#define  USB_LEN_DEV_DESC                               (0x12U)
#define  USB_LEN_CFG_DESC                               (0x09U)
#define  USB_LEN_IF_DESC                                (0x09U)
#define  USB_LEN_EP_DESC                                (0x07U)
#define  USB_LEN_OTG_DESC                               (0x03U)
#define  USB_LEN_SETUP_PKT                              (0x08U)

/* bmRequestType :D7 Data Phase Transfer Direction  */
#define  USB_REQ_DIR_MASK                               (0x80U)
#define  USB_H2D                                        (0x00U)
#define  USB_D2H                                        (0x80U)

/* bmRequestType D6..5 Type */
#define  USB_REQ_TYPE_STANDARD                          (0x00U)
#define  USB_REQ_TYPE_CLASS                             (0x20U)
#define  USB_REQ_TYPE_VENDOR                            (0x40U)
#define  USB_REQ_TYPE_RESERVED                          (0x60U)

/* bmRequestType D4..0 Recipient */
#define  USB_REQ_RECIPIENT_DEVICE                       (0x00U)
#define  USB_REQ_RECIPIENT_INTERFACE                    (0x01U)
#define  USB_REQ_RECIPIENT_ENDPOINT                     (0x02U)
#define  USB_REQ_RECIPIENT_OTHER                        (0x03U)

/* Table 9-4. Standard Request Codes  */
/* bRequest , Value */
#define  USB_REQ_GET_STATUS                             (0x00U)
#define  USB_REQ_CLEAR_FEATURE                          (0x01U)
#define  USB_REQ_SET_FEATURE                            (0x03U)
#define  USB_REQ_SET_ADDRESS                            (0x05U)
#define  USB_REQ_GET_DESCRIPTOR                         (0x06U)
#define  USB_REQ_SET_DESCRIPTOR                         (0x07U)
#define  USB_REQ_GET_CONFIGURATION                      (0x08U)
#define  USB_REQ_SET_CONFIGURATION                      (0x09U)
#define  USB_REQ_GET_INTERFACE                          (0x0AU)
#define  USB_REQ_SET_INTERFACE                          (0x0BU)
#define  USB_REQ_SYNCH_FRAME                            (0x0CU)

/* Table 9-5. Descriptor Types of USB Specifications */
#define  USB_DESC_TYPE_DEVICE                              (1U)
#define  USB_DESC_TYPE_CONFIGURATION                       (2U)
#define  USB_DESC_TYPE_STRING                              (3U)
#define  USB_DESC_TYPE_INTERFACE                           (4U)
#define  USB_DESC_TYPE_ENDPOINT                            (5U)
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    (6U)
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           (7U)
#define  USB_DESC_TYPE_INTERFACE_POWER                     (8U)
#define  USB_DESC_TYPE_HID                                 (0x21U)
#define  USB_DESC_TYPE_HID_REPORT                          (0x22U)

#define USB_DEVICE_DESC_SIZE                               (18U)
#define USB_CONFIGURATION_DESC_SIZE                        (9U)
#define USB_HID_DESC_SIZE                                  (9U)
#define USB_INTERFACE_DESC_SIZE                            (9U)
#define USB_ENDPOINT_DESC_SIZE                             (7U)

/* Descriptor Type and Descriptor Index  */
/* Use the following values when calling the function USBH_GetDescriptor  */
#define  USB_DESC_DEVICE                    (((uint16_t)USB_DESC_TYPE_DEVICE << 8U) & 0xFF00U)
#define  USB_DESC_CONFIGURATION             (((uint16_t)USB_DESC_TYPE_CONFIGURATION << 8U) & 0xFF00U)
#define  USB_DESC_STRING                    (((uint16_t)USB_DESC_TYPE_STRING << 8U) & 0xFF00U)
#define  USB_DESC_INTERFACE                 (((uint16_t)USB_DESC_TYPE_INTERFACE << 8U) & 0xFF00U)
#define  USB_DESC_ENDPOINT                  (((uint16_t)USB_DESC_TYPE_INTERFACE << 8U) & 0xFF00U)
#define  USB_DESC_DEVICE_QUALIFIER          (((uint16_t)USB_DESC_TYPE_DEVICE_QUALIFIER << 8U) & 0xFF00U)
#define  USB_DESC_OTHER_SPEED_CONFIGURATION (((uint16_t)USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION << 8U) & 0xFF00U)
#define  USB_DESC_INTERFACE_POWER           (((uint16_t)USB_DESC_TYPE_INTERFACE_POWER << 8U) & 0xFF00U)
#define  USB_DESC_HID_REPORT                (((uint16_t)USB_DESC_TYPE_HID_REPORT << 8U) & 0xFF00U)
#define  USB_DESC_HID                       (((uint16_t)USB_DESC_TYPE_HID << 8U) & 0xFF00U)


#define  USB_EP_TYPE_CTRL                               (0x00U)
#define  USB_EP_TYPE_ISOC                               (0x01U)
#define  USB_EP_TYPE_BULK                               (0x02U)
#define  USB_EP_TYPE_INTR                               (0x03U)

#define  USB_EP_DIR_OUT                                 (0x00U)
#define  USB_EP_DIR_IN                                  (0x80U)
#define  USB_EP_DIR_MSK                                 (0x80U)

/* supported classes */
#define USB_MSC_CLASS                                   (0x08U)
#define USB_HID_CLASS                                   (0x03U)

/* Interface Descriptor field values for HID Boot Protocol */
#define HID_BOOT_CODE                                  (0x01U)
#define HID_KEYBRD_BOOT_CODE                           (0x01U)
#define HID_MOUSE_BOOT_CODE                            (0x02U)

/* As per USB specs 9.2.6.4 :Standard request with data request timeout: 5sec
   Standard request with no data stage timeout : 50ms */
#define DATA_STAGE_TIMEOUT                              (5000U)
#define NODATA_STAGE_TIMEOUT                            (50U)

#define USBH_CONFIGURATION_DESCRIPTOR_SIZE (USB_CONFIGURATION_DESC_SIZE \
                                           + USB_INTERFACE_DESC_SIZE\
                                           + (USBH_MAX_NUM_ENDPOINTS * USB_ENDPOINT_DESC_SIZE))


#define CONFIG_DESC_wTOTAL_LENGTH (ConfigurationDescriptorData.ConfigDescfield.\
                                          ConfigurationDescriptor.wTotalLength)
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

#endif /* __USBH_DEF_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
