/**
 *******************************************************************************
 * @file  i2c/i2c_master_irq/source/ddl_config.h
 * @brief This file contains HC32 Series Device Driver Library usage management.
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
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
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
#ifndef __DDL_CONFIG_H__
#define __DDL_CONFIG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Chip module on-off define */
#define DDL_ON                                      (1U)
#define DDL_OFF                                     (0U)

/**
 * @brief This is the list of modules to be used in the Device Driver Library.
 * Select the modules you need to use to DDL_ON.
 * @note DDL_ICG_ENABLE must be turned on(DDL_ON) to ensure that the chip works
 * properly.
 * @note DDL_UTILITY_ENABLE must be turned on(DDL_ON) if using Device Driver
 * Library.
 * @note DDL_PRINT_ENABLE must be turned on(DDL_ON) if using printf function.
 */
#define DDL_ICG_ENABLE                              (DDL_ON)
#define DDL_UTILITY_ENABLE                          (DDL_ON)
#define DDL_PRINT_ENABLE                            (DDL_OFF)

#define DDL_ADC_ENABLE                              (DDL_OFF)
#define DDL_AES_ENABLE                              (DDL_OFF)
#define DDL_CAN_ENABLE                              (DDL_OFF)
#define DDL_CLK_ENABLE                              (DDL_ON)
#define DDL_CMP_ENABLE                              (DDL_OFF)
#define DDL_CRC_ENABLE                              (DDL_OFF)
#define DDL_CTC_ENABLE                              (DDL_OFF)
#define DDL_DAC_ENABLE                              (DDL_OFF)
#define DDL_DCU_ENABLE                              (DDL_OFF)
#define DDL_DMA_ENABLE                              (DDL_OFF)
#define DDL_DMC_ENABLE                              (DDL_OFF)
#define DDL_DVP_ENABLE                              (DDL_OFF)
#define DDL_EFM_ENABLE                              (DDL_ON)
#define DDL_EMB_ENABLE                              (DDL_OFF)
#define DDL_ETH_ENABLE                              (DDL_OFF)
#define DDL_EVENT_PORT_ENABLE                       (DDL_OFF)
#define DDL_FCM_ENABLE                              (DDL_OFF)
#define DDL_FMAC_ENABLE                             (DDL_OFF)
#define DDL_GPIO_ENABLE                             (DDL_ON)
#define DDL_HASH_ENABLE                             (DDL_OFF)
#define DDL_HRPWM_ENABLE                            (DDL_OFF)
#define DDL_I2C_ENABLE                              (DDL_ON)
#define DDL_I2S_ENABLE                              (DDL_OFF)
#define DDL_INTERRUPTS_ENABLE                       (DDL_ON)
#define DDL_KEYSCAN_ENABLE                          (DDL_ON)
#define DDL_MAU_ENABLE                              (DDL_OFF)
#define DDL_MPU_ENABLE                              (DDL_OFF)
#define DDL_NFC_ENABLE                              (DDL_OFF)
#define DDL_OTS_ENABLE                              (DDL_OFF)
#define DDL_PWC_ENABLE                              (DDL_ON)
#define DDL_QSPI_ENABLE                             (DDL_OFF)
#define DDL_RMU_ENABLE                              (DDL_OFF)
#define DDL_RTC_ENABLE                              (DDL_OFF)
#define DDL_SDIOC_ENABLE                            (DDL_OFF)
#define DDL_SMC_ENABLE                              (DDL_OFF)
#define DDL_SPI_ENABLE                              (DDL_OFF)
#define DDL_SRAM_ENABLE                             (DDL_ON)
#define DDL_SWDT_ENABLE                             (DDL_OFF)
#define DDL_TMR0_ENABLE                             (DDL_OFF)
#define DDL_TMR2_ENABLE                             (DDL_OFF)
#define DDL_TMR4_ENABLE                             (DDL_OFF)
#define DDL_TMR6_ENABLE                             (DDL_OFF)
#define DDL_TMRA_ENABLE                             (DDL_OFF)
#define DDL_TRNG_ENABLE                             (DDL_OFF)
#define DDL_USART_ENABLE                            (DDL_OFF)
#define DDL_USBFS_ENABLE                            (DDL_OFF)
#define DDL_USBHS_ENABLE                            (DDL_OFF)
#define DDL_VBAT_ENABLE                             (DDL_OFF)
#define DDL_WDT_ENABLE                              (DDL_OFF)

/* BSP on-off define */
#define BSP_ON                                      (1U)
#define BSP_OFF                                     (0U)

/**
 * @brief The following is a list of currently supported BSP boards.
 */
#define BSP_EV_HC32F4A0_LQFP176                     (1U)
#define BSP_MS_HC32F4A0_LQFP176_050_MEM             (2U)

/**
 * @brief The macro BSP_EV_HC32F4A0 is used to specify the BSP board currently
 * in use.
 * The value should be set to one of the list of currently supported BSP boards.
 * @note  If there is no supported BSP board or the BSP function is not used,
 * the value needs to be set to BSP_EV_HC32F4A0.
 */
#define BSP_EV_HC32F4A0                             (BSP_EV_HC32F4A0_LQFP176)

/**
 * @brief This is the list of BSP components to be used.
 * Select the components you need to use to BSP_ON.
 */
#define BSP_CY62167EV30LL_ENABLE                    (BSP_OFF)
#define BSP_IS42S16400J7TLI_ENABLE                  (BSP_OFF)
#define BSP_IS62WV51216_ENABLE                      (BSP_OFF)
#define BSP_MT29F2G08AB_ENABLE                      (BSP_OFF)
#define BSP_S29GL064N90TFI03_ENABLE                 (BSP_OFF)
#define BSP_TCA9539_ENABLE                          (BSP_ON)
#define BSP_W25QXX_ENABLE                           (BSP_OFF)
#define BSP_WM8731_ENABLE                           (BSP_OFF)

/**
 * @brief Ethernet and PHY Configuration.
 * @note  PHY delay these values are based on a 1 ms Systick interrupt.
 */
/* MAC ADDRESS */
#define ETH_MAC_ADDR0                   (2U)
#define ETH_MAC_ADDR1                   (0U)
#define ETH_MAC_ADDR2                   (0U)
#define ETH_MAC_ADDR3                   (0U)
#define ETH_MAC_ADDR4                   (0U)
#define ETH_MAC_ADDR5                   (0U)

/* Ethernet driver buffers size and count */
#define ETH_TXBUF_SIZE                  (ETH_PACKET_MAX_SIZE) /* Buffer size for receive              */
#define ETH_RXBUF_SIZE                  (ETH_PACKET_MAX_SIZE) /* Buffer size for transmit             */
#define ETH_TXBUF_NUMBER                (4UL)                 /* 4 Rx buffers of size ETH_RX_BUF_SIZE */
#define ETH_RXBUF_NUMBER                (4UL)                 /* 4 Tx buffers of size ETH_TX_BUF_SIZE */

/* PHY Address*/
#define PHY_ADDRESS                     (0x00U)               /* RTL8201F                             */

/* PHY Configuration delay */
#define PHY_HW_RESET_DELAY              (0x0000003FUL)
#define PHY_RESET_DELAY                 (0x0000007FUL)
#define PHY_CONFIG_DELAY                (0x0000003FUL)
#define PHY_READ_TIMEOUT                (0x00000005UL)
#define PHY_WRITE_TIMEOUT               (0x00000005UL)

/* Common PHY Registers */
#define PHY_BCR                         (0x00U)     /*!< Basic Control Register               */
#define PHY_BSR                         (0x01U)     /*!< Basic Status Register                */

#define PHY_SOFT_RESET                  (0x8000U)   /*!< PHY Soft Reset                       */
#define PHY_LOOPBACK                    (0x4000U)   /*!< Select loop-back mode                */
#define PHY_FULLDUPLEX_100M             (0x2100U)   /*!< Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M             (0x2000U)   /*!< Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M              (0x0100U)   /*!< Set the full-duplex mode at 10 Mb/s  */
#define PHY_HALFDUPLEX_10M              (0x0000U)   /*!< Set the half-duplex mode at 10 Mb/s  */
#define PHY_AUTONEGOTIATION             (0x1000U)   /*!< Enable auto-negotiation function     */
#define PHY_POWERDOWN                   (0x0800U)   /*!< Select the power down mode           */
#define PHY_ISOLATE                     (0x0400U)   /*!< Isolate PHY from MII                 */
#define PHY_RESTART_AUTONEGOTIATION     (0x0200U)   /*!< Restart auto-negotiation function    */

#define PHY_100BASE_TX_FD               (0x4000U)   /*!< 100Base-TX full duplex support       */
#define PHY_100BASE_TX_HD               (0x2000U)   /*!< 100Base-TX half duplex support       */
#define PHY_10BASE_T_FD                 (0x1000U)   /*!< 10Base-T full duplex support         */
#define PHY_10BASE_T_HD                 (0x0800U)   /*!< 10Base-T half duplex support         */
#define PHY_AUTONEGO_COMPLETE           (0x0020U)   /*!< Auto-Negotiation process completed   */
#define PHY_LINK_STATUS                 (0x0004U)   /*!< Valid link established               */
#define PHY_JABBER_DETECTION            (0x0002U)   /*!< Jabber condition detected            */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __DDL_CONFIG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
