version     date            comment
 1.0.1
            Jul 15, 2020    [CLK]
                                1.  Refine driver and sample;
                                2.  Support 16MHz & 20MHz HRC;
                                3.  Use XTAL 8MHz as PLL source for BSP and all examples. 
                            [DAC]
                                1.  Modify DAC_ChannelCmd to Dac_Start and Dac_Stop;
                                2.  Modify DAC_DualChannelCmd to DAC_DualChannelStart and DAC_DualChannelStop;
                                3.  Modify DAC related examples.
                            [EFM]
                                1.  Add flag judgement for EFM_SetOperateMode function;
                                2.  Add flag judgement when SWAP operation.
                            [EMB]
                                1.  Modify structure member comments for stc_emb_monitor_port_t/stc_emb_monitor_tmr_pwm_t;
                                2.  Replace the word Timer with TMR abbreviation.
                            [ETH]
                                1.  Optimize stc_eth_mac_init_t structure.
                            [EXMC]
                                1.  Merge API from EXMC_XXX_Enable/Disable to EXMC_XXX_Cmd(XXX = DMC/NFC/SMC);
                                2.  Merge API from EXMC_NFC_Enable/DisableEcc to EXMC_NFC_EccCmd;
                                3.  Merge API from EXMC_NFC_Enable/DisableWriteProtect to EXMC_NFC_WriteProtectCmd;
                                4.  Adjust EXMC pin drive capacity to high drive.
                            [I2C]
                                1.  Modify I2C_SmBusCmd to I2C_SetMode.
                            [MAU]
                                1.  Modify MAU_SqrtStartCmd to MAU_SqrtStart.
                            [QSPI]
                                1.  Refine driver and sample.
                            [TMR4]
                                1.  Modify parameter assert condition for the function TMR4_PWM_SetPclkDiv;
                                2.  Modify macro define for TMR4 SEVT match mask.
                            [TMR6]
                                1.  Refine macro definition.
                            [MISC]
                                1.  Add doxygen group:USART_Local_Functions;
                                2.  Call the PWC_FCG0_Unlock function in the examples of CLK, CAN,
                                    ETH, ICG, PWC, QSPI, RTC, SRAM, SWDT, TMRA, WDT, SYSTICK;
 1.0.0
            Jun 30, 2020    1. Official release
 0.9.0
            Jun 16, 2020    1. Alpha release for hc32f4a0 series DDL(Device Driver Library)

EOF