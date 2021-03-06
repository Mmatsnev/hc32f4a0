================================================================================
                                样例使用说明
================================================================================
版本历史
日期        版本    负责人     IAR     MDK     GCC     描述
2020-06-12  1.0     Wuze     7.70    5.16    8.3.1   First version

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
本例程以ADC单元3（M4_ADC3）为例，将模拟输入引脚ADC123_IN3（PA3）映射到ADC3的通道0和通
道1，将模拟输入引脚ADC123_IN12（PC2）映射到ADC3通道2，重新映射引脚后的通道进行扫描，并
用串口将ADC值输出。

说明：
将ADC123_IN3映射到通道0和通道1，旨在表明同一个模拟输入引脚可以映射到多个通道，但必须注意，
不能在多个通道或ADC单元对同一个引脚进行采样。

================================================================================
测试环境
================================================================================
测试用板:
---------------------
EV_F4A0_LQ176_V10

辅助工具:
---------------------
USB转串口工具

辅助软件:
---------------------
串口调试助手软件（115200bps，8N1）

================================================================================
使用步骤
================================================================================
1）如果需要用在上位机观察程序运行情况，请将USB转串口工具正确地与开发板上的J28端口（靠近
   MicroUSB接口）连接；如果不需要，请忽略此步骤；
2）打开工程并重新编译；
3）启动IDE的下载和调试功能；
4）全速运行程序或在需要的位置设置断点；
5）可断点观察或用串口调试助手软件观察ADC值，可看到通道0和通道1的值大致相同（由于有误差，
   相等的概率较小）。

================================================================================
注意
================================================================================
无

================================================================================
