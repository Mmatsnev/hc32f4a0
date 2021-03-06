================================================================================
                                样例使用说明
================================================================================
版本历史
日期        版本    负责人         IAR     MDK     GCC     描述
2020-06-12  1.0     Zhangxl        7.70    5.16    8.3.1   first version
================================================================================
平台说明
================================================================================
GCC工程，由Eclipse IDE外挂GNU-ARM Toolchain，再结合pyOCD GDB Server实现工程的编译、
链接和调试。在用Eclipse导入工程后，请将xxxx_PyOCDDebug中pyocd-gdbserver和SVD文件
设置为正确的路径；请将xxxx_PyOCDDownload中pyocd设置为正确的路径。注意，这些路径不
能包含非英文字符。


功能描述
================================================================================
本样例为时钟频率测量，用XTAL(8MHz) 8192分频作为参考基准，测量：
XTAL32  (32.768KHz)
HRC     (16MHz)
LRC     (32K)
SWDTLRC (10K)
PCLK1   (4MHz,系统时钟为MRC_8MHz，div2)
PLLAP   (60MHz，VCO_480MHz div8)
MRC     (8MHz)
PLLHP   (60MHz，VCO_960MHz div16)
RTCLRC  (32KHz)

================================================================================
测试环境
================================================================================
测试用板:
---------------------
EV_F4A0_LQ176_V10

辅助工具:
---------------------
无

辅助软件:
---------------------
串口调试助手（115200bps，8N1）

================================================================================
使用步骤
================================================================================
1）打开工程并重新编译；
2）启动IDE的下载和调试功能，全速运行；
3）打开串口调试助手，选择识别的COM口，设置波特率115200bps，8N1；
4）依次按下按键SW1~SW9，被测时钟频率将被打印至串口终端。


================================================================================
注意
================================================================================


================================================================================
