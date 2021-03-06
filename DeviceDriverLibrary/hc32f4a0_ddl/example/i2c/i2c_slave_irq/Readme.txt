================================================================================
                                样例使用说明
================================================================================
版本历史
日期        版本    负责人     IAR     MDK     GCC     描述
2020-06-12  1.0     Hexiao     7.70    5.16    8.3.1   First version

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
本样例为I2C外设以从机模式进行中断方式的数据收发样例
说明：
无

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
1）准备另一个测试板作为主机；
2）连线：主机SDA->从机SDA；主机SCL->从机SCL；主机GND->从机GND；
3）打开从机板子IDE工程i2c_slave_irq并重新编译；
4）启动从机板子IDE的下载和调试功能，全速运行；
5）打开主机板子IDE工程i2c_master_irq并重新编译；
6）启动主机板子IDE的下载和调试功能，全速运行；
7）观察从机板子LED（D26）被点亮，从机成功完成数据的收发操作。

================================================================================
注意
================================================================================
1）本样例需要两个测试板并且与样例i2c_master_irq配合使用；
2）从机板子LED（D26）没有被点亮，则从机发送或接收操作失败。

================================================================================
