================================================================================
                                样例使用说明
================================================================================
版本历史
日期        版本    负责人     IAR     MDK     GCC     描述
2020-06-12  1.0     Hongjh     7.70    5.16    8.3.1   First version

================================================================================
平台说明
================================================================================
GCC工程，由Eclipse IDE外挂GNU-ARM Toolchain，再结合pyOCD GDB Server实现工程的编译、
链接和调试。在用Eclipse导入工程后，请将xxxx_PyOCDDebug中pyocd-gdbserver和SVD文件
设置为正确的路径；请将xxxx_PyOCDDownload中pyocd设置为正确的路径。注意，这些路径不
能包含非英文字符。

================================================================================
功能描述
================================================================================
本样例主要展示如何使用Timer4脉冲宽度调制模块PWM直通模式。

说明：
样例配置比较输出模块OCO高、低通道独立输出，并通过PWM直通模式，输出其波形。

================================================================================
测试环境
================================================================================
测试用板:
---------------------
EV_F4A0_LQ176_V10

辅助工具:
---------------------
示波器Tektronix DPO2024B

辅助软件:
---------------------
无

================================================================================
使用步骤
================================================================================
1）测试板J1-PE9(TIM4_1_OUH)、J1-PE8(TIM4_1_OUL)引脚与示波器相连；
2）编译、下载并运行；
3）观察示波器，J1-PE9、J1-E8通道波形极性相同，频率相同2Hz。
              _____       _____       _____
  J1-PE9    _|     |_____|     |_____|     |

              _____       _____       _____
  J1-PE8    _|     |_____|     |_____|     |

================================================================================
注意
================================================================================
无

================================================================================
