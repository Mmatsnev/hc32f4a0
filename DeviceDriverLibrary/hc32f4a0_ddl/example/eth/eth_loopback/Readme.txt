================================================================================
                                样例使用说明
================================================================================
版本历史
日期        版本    负责人     IAR     MDK     GCC     描述
2020-06-12  1.0     Yangjp     7.70    5.16    8.3.1   First version

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
本样例展示ETH的Loop back通信功能。

说明：
本样例配置ETH工作在PHY的loop back模式，目标板LED_B亮起后，点击SW10触发数据发送，
loop back将发送的数据回传回来，对接收到的数据和发送的数据进行比较，内容一致则
LED_Y亮起，反之，则熄灭。

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
无

================================================================================
使用步骤
================================================================================
1）打开工程并重新编译；
2）启动IDE的下载和调试功能，关闭IDE下载界面；
3）等待目标板LED_B亮起；
4）点击SW10触发数据发送，并观察LED_Y亮灭状态；
5）LED_Y亮起则收发数据正常，反之，则通信失败。

================================================================================
注意
================================================================================
无

================================================================================
