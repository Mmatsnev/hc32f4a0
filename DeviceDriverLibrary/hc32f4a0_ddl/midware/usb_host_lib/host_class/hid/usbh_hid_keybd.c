/**
 *******************************************************************************
 * @file  usbh_hid_keybd.c
 * @brief This file is the application layer for USB Host HID Keyboard handling
 *        QWERTY and AZERTY Keyboard are supported as per the selection in
 *        usbh_hid_keybd.h
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       zhangxl         First version
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
#include "usbh_hid_keybd.h"

/**
 * @addtogroup MIDWARE
 * @{
 */

/**
 * @addtogroup USB_HOST_LIB
 * @{
 */

/**
 * @addtogroup USB_HOST_CLASS
 * @{
 */

/** @defgroup USBH_HID_KEYBD
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
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_KEYBD_Local_Functions USBH HID Keyboard Local Functions
 * @{
 */
static void  KEYBRD_Init (void);
static void  KEYBRD_Decode(uint8_t *data);
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_KEYBD_Global_Variables USBH HID Keyboard Global Variables
 * @{
 */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
 #if defined   (__CC_ARM) /*!< ARM Compiler */
  __align(4)
 #elif defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
 #elif defined (__GNUC__) /*!< GNU Compiler */
 #pragma pack(4)
 #endif /* __CC_ARM */
#endif

HID_cb_TypeDef HID_KEYBRD_cb=
{
    KEYBRD_Init,
    KEYBRD_Decode
};
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup USBH_HID_KEYBD_Local_Variables USBH HID Keyboard Local Variables
 * @{
 */
static  const  uint8_t  HID_KEYBRD_Codes[] =
{
    0U,      0U,      0U,      0U,      31U,     50U,     48U,     33U,
    19U,     34U,     35U,     36U,     24U,     37U,     38U,     39U,     /* 0x00 - 0x0F */
    52U,     51U,     25U,     26U,     17U,     20U,     32U,     21U,
    23U,     49U,     18U,     47U,     22U,     46U,     2U,      3U,      /* 0x10 - 0x1F */
    4U,      5U,      6U,      7U,      8U,      9U,      10U,     11U,
    43U,     110U,    15U,     16U,     61U,     12U,     13U,     27U,     /* 0x20 - 0x2F */
    28U,     29U,     42U,     40U,     41U,     1U,      53U,     54U,
    55U,     30U,     112U,    113U,    114U,    115U,    116U,    117U,    /* 0x30 - 0x3F */
    118U,    119U,    120U,    121U,    122U,    123U,    124U,    125U,
    126U,    75U,     80U,     85U,     76U,     81U,     86U,     89U,     /* 0x40 - 0x4F */
    79U,     84U,     83U,     90U,     95U,     100U,    105U,    106U,
    108U,    93U,     98U,     103U,    92U,     97U,     102U,    91U,     /* 0x50 - 0x5F */
    96U,     101U,    99U,     104U,    45U,     129U,    0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0x60 - 0x6F */
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0x70 - 0x7F */
    0U,      0U,      0U,      0U,      0U,      107U,    0U,      56U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0x80 - 0x8F */
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0x90 - 0x9F */
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0xA0 - 0xAF */
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0xB0 - 0xBF */
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0xC0 - 0xCF */
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,
    0U,      0U,      0U,      0U,      0U,      0U,      0U,      0U,      /* 0xD0 - 0xDF */
    58U,     44U,     60U,     127U,    64U,     57U,     62U,     128U     /* 0xE0 - 0xE7 */
};

#ifdef QWERTY_KEYBOARD
static  const  int8_t  HID_KEYBRD_Key[] =
{
    '\0',   '`' ,   '1' ,   '2' ,   '3' ,   '4' ,   '5' ,   '6' ,
    '7' ,   '8' ,   '9' ,   '0' ,   '-' ,   '=' ,   '\0',   '\r',
    '\t',   'q' ,   'w' ,   'e' ,   'r' ,   't' ,   'y' ,   'u' ,
    'i' ,   'o' ,   'p' ,   '[' ,   ']' ,   '\\',
    '\0',   'a' ,   's' ,   'd' ,   'f' ,   'g' ,   'h' ,   'j' ,
    'k' ,   'l' ,   ';' ,   '\'',   '\0',   '\n',
    '\0',   '\0',   'z' ,   'x' ,   'c' ,   'v' ,   'b' ,   'n' ,
    'm' ,   ',' ,   '.' ,   '/' ,   '\0',   '\0',
    '\0',   '\0',   '\0',   ' ' ,   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\r',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '7' ,   '4' ,   '1' ,
    '\0',   '/',    '8' ,   '5' ,   '2' ,
    '0' ,   '*',    '9' ,   '6' ,   '3' ,
    '.' ,   '-',    '+' ,   '\0',   '\n',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0'
};

static  const  int8_t  HID_KEYBRD_ShiftKey[] = {
    '\0',   '~' ,   '!' ,   '@' ,   '#' ,   '$' ,   '%' ,   '^' ,   '&' ,   '*' ,   '(' ,   ')' ,
    '_' ,   '+' ,   '\0',   '\0',   '\0',   'Q' ,   'W' ,   'E' ,   'R' ,   'T' ,   'Y' ,   'U' ,
    'I' ,   'O' ,   'P' ,   '{' ,   '}' ,   '|' ,   '\0',   'A' ,   'S' ,   'D' ,   'F' ,   'G' ,
    'H' ,   'J' ,   'K' ,   'L' ,   ':' ,   '"' ,   '\0',   '\n',   '\0',   '\0',   'Z' ,   'X' ,
    'C' ,   'V' ,   'B' ,   'N' ,   'M' ,   '<' ,   '>' ,   '?' ,   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0'
};

#else

static  const  int8_t  HID_KEYBRD_Key[] = {
    '\0',   '`' ,   '1' ,   '2' ,   '3' ,   '4' ,   '5' ,   '6' ,   '7' ,   '8' ,   '9' ,   '0',
    '-' ,   '=' ,   '\0',   '\r',   '\t',   'a' ,   'z' ,   'e' ,   'r' ,   't' ,   'y' ,   'u',
    'i' ,   'o' ,   'p' ,   '[' ,   ']' ,   '\\',   '\0',   'q' ,   's' ,   'd' ,   'f' ,   'g',
    'h' ,   'j' ,   'k' ,   'l' ,   'm' ,   '\0',   '\0',   '\n',   '\0',   '\0',   'w' ,   'x',
    'c' ,   'v' ,   'b' ,   'n' ,   ',' ,   ';' ,   ':' ,   '!' ,   '\0',   '\0',   '\0',   '\0',
    '\0',   ' ' ,   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\r',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '7' ,   '4' ,   '1' ,   '\0',   '/',
    '8' ,   '5' ,   '2' ,   '0' ,   '*' ,   '9' ,   '6' ,   '3' ,   '.' ,   '-' ,   '+' ,   '\0',
    '\n',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0'
};

static  const  int8_t  HID_KEYBRD_ShiftKey[] = {
    '\0',   '~' ,   '!' ,   '@' ,   '#' ,   '$' ,   '%' ,   '^' ,   '&' ,   '*' ,   '(' ,   ')' ,   '_' ,
    '+' ,   '\0',   '\0',   '\0',   'A' ,   'Z' ,   'E' ,   'R' ,   'T' ,   'Y' ,   'U' ,   'I' ,   'O' ,
    'P' ,   '{' ,   '}' ,   '*' ,   '\0',   'Q' ,   'S' ,   'D' ,   'F' ,   'G' ,   'H' ,   'J' ,   'K' ,
    'L' ,   'M' ,   '%' ,   '\0',   '\n',   '\0',   '\0',   'W' ,   'X' ,   'C' ,   'V' ,   'B' ,   'N' ,
    '?' ,   '.' ,   '/' ,   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',
    '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0',   '\0'
};
#endif

/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @addtogroup USBH_HID_KEYBD_Local_Functions USBH HID Keyboard Local Functions
 * @{
 */

/**
* @brief  KEYBRD_Init.
*         Initialize the keyboard function.
* @param  None
* @retval None
*/
static void  KEYBRD_Init (void)
{
    /* Call User Init*/
    USR_KEYBRD_Init();
}

/**
* @brief  KEYBRD_ProcessData.
*         The function is to decode the pressed keys.
* @param  pbuf : Pointer to the HID IN report data buffer
* @retval None
*/
static void KEYBRD_Decode(uint8_t *pbuf)
{
    static  uint8_t   shift;
    static  uint8_t   keys[KBR_MAX_NBR_PRESSED];
    static  uint8_t   keys_new[KBR_MAX_NBR_PRESSED];
    static  uint8_t   keys_last[KBR_MAX_NBR_PRESSED];
    static  uint8_t   key_newest;
    static  uint8_t   nbr_keys;
    static  uint8_t   nbr_keys_new;
    static  uint8_t   nbr_keys_last;
    uint8_t   ix;
    uint8_t   jx;
    uint8_t   error;
    uint8_t   output;

    nbr_keys      = 0U;
    nbr_keys_new  = 0U;
    nbr_keys_last = 0U;
    key_newest    = 0x00U;


    /* Check if Shift key is pressed */
    if ((pbuf[0U] == KBD_LEFT_SHIFT) || (pbuf[0U] == KBD_RIGHT_SHIFT))
    {
        shift = TRUE;
    }
    else
    {
        shift = FALSE;
    }

    error = FALSE;

    /* Check for the value of pressed key */
    for (ix = 2U; ix < 2U + KBR_MAX_NBR_PRESSED; ix++)
    {
        if ((pbuf[ix] == 0x01U) || (pbuf[ix] == 0x02U) || (pbuf[ix] == 0x03U))
        {
            error = TRUE;
        }
    }
    if (error == TRUE)
    {
        return;
    }
    nbr_keys     = 0U;
    nbr_keys_new = 0U;
    for (ix = 2U; ix < 2U + KBR_MAX_NBR_PRESSED; ix++)
    {
        if (pbuf[ix] != 0U)
        {
            keys[nbr_keys] = pbuf[ix];
            nbr_keys++;
            for (jx = 0U; jx < nbr_keys_last; jx++)
            {
                if (pbuf[ix] == keys_last[jx])
                {
                    break;
                }
            }
            if (jx == nbr_keys_last)
            {
                keys_new[nbr_keys_new] = pbuf[ix];
                nbr_keys_new++;
            }
        }
    }
    if (nbr_keys_new == 1U)
    {
        key_newest = keys_new[0U];

        if (shift == TRUE)
        {
            output = HID_KEYBRD_ShiftKey[HID_KEYBRD_Codes[key_newest]];
        }
        else
        {
            output = HID_KEYBRD_Key[HID_KEYBRD_Codes[key_newest]];
        }
        /* call user process handle */
        USR_KEYBRD_ProcessData(output);
    }
    else
    {
        key_newest = 0x00U;
    }
    nbr_keys_last  = nbr_keys;
    for (ix = 0U; ix < KBR_MAX_NBR_PRESSED; ix++)
    {
        keys_last[ix] = keys[ix];
    }
}

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

/**
 * @}
 */

/**
* @}
*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
