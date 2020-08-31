version     date            comment
 1.0.4
            Aug 31, 2020
                            [ADC]
                                1.  Refined code for interrupt of sample adc_02_event_trigger.
                            [BSP]
                                1.  The macro-define parameter add parentheses in s29gl064n90tfi03.c.
                                2.  The if condition statement add variable compare value in ms_hc32f4a0_lqfp176_050_mem_s29gl064n90tfi03.c.
                                3.  Modify w25qxx.c for MISRAC2012 compliance.
                                4.  Add components: nt35510 and ov5640
                                5.  Add doxygen local functions group and macro-define for ev_hc32f4a0_lqfp176
                            [CMP]
                                1.  Replace CMP_Delay300ns() with the function DDL_DelayUS().
                            [DAC]
                                1.  Refine DAC_AMPCmd
                            [DVP]
                                1.  Add DVP driver and sample
                            [EFM]
                                1.  Adjust the BSP_CLK_Init() position in efm_base and efm_irq examples.
                            [ETH]
                                1.  In the example of eth_twoboards, the ETH communication clock is increased to 25M.
                            [I2C]
                                1.  Refine I2C_Init/I2C_SendData/I2C_RcvData and remove invalid clock division value
                                2.  Modify I2C examples
                            [INTC]
                                1.  Modify for MISRAC2012 compliance.
                            [NFC]
                                1.  Modify the pointer type cast.
                            [PWC]
                                1.  Fixed LED display error in the pwc_vol_measure example.
                            [SPI]
                                1.  Modify for MISRAC2012 compliance.
                            [TMR2]
                                1.  Refined sample timer2_capture.
                            [TMRA]
                                1.  Refined samples timera_oc_position_count and timera_oc_revolution_count:
                                    Calculating the rotation speed of the quadrature encoder via position counting function and revolution counting function.
                            [USART]
                                1.  The variable f32Err un-initializes an unnecessary assignment.
                            [USB]
                                1.  Add USB host class msc.
                                2.  Add usbh_msc example.
                                3.  Modify Comment for parameter pdev.
 1.0.3
            Aug 11, 2020    [ADC]
                                1.  ADC_SeqClrStatus(): ADC_SEQ_FLAG_NESTED can be cleared.
                            [CAN]
                                1.  Refined CAN_GetStatusVal().
                            [CRC]
                                1.  Modify macro definition name IS_CRC_PROCOTOL to IS_CRC_PROTOCOL.
                            [EFM]
                                1.  Refine function EFM_SequenceProgram();
                                2.  Typo.
                            [ETH]
                                1.  Fix a known potential risk in ETH_Init function;
                                2.  Add the eth_twoboards example.
                            [OTS]
                                1.  New added.
                            [QSPI]
                                1.  Refined function W25Q64_GetUniqueID() of sample qspi_base.
                            [SDIOC]
                                1.  Fix a known potential risk in SDIOC_VerifyClockDiv function.
 1.0.2
            Jul 31, 2020    [CRC]
                                1.  Fixed a bug for CRC_Check function.
                            [DCU]
                                1.  Correct the macro define: DCU_CMP_TRIG_DATA01;
                                2.  Refine the macro define for interrupt, flag and mode;
                                3.  Rename API: from DCU_IntFuncCmd to DCU_GlobalIntCmd;
                                4.  Refine API: DCU DATA read/write and DCU_IntCmd;
                                5.  Delete API: DCU_SetCmpIntMode.
                            [PWC]
                                1.  Refine power mode switch function.
                            [TMR4]
                                1.  Modify TMR4_OCO_SetLowChCompareMode function comment from TMR4_OCO_UH/VH/WH to TMR4_OCO_UL/VL/WL.
                            [RMU]
                                1.  Add write protect check for RMU_ClrStatus function.
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
            Jun 30, 2020    1. Official release.
 0.9.0
            Jun 16, 2020    1. Alpha release for hc32f4a0 series DDL(Device Driver Library).

EOF