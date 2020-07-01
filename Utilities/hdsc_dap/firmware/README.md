# DAPLink固件升级：
  1. 连接EV_F4A0_LQ176_V02开发板***J29:GND<——>J29:BOOT***
  2. 通过USB数据线连接PC和开发板***J25:DAP***
  3. 插入USB线后，LED1红色灯点亮，弹出U盘（盘符：***MAINTENANCE***）
  4. 断开连接***J29:GND  J29:BOOT***
  5. 将DAPLink固件***hc32f460_if_crc.hex***拖入U盘
  6. LED1红色灯闪烁，表示正在烧录DAPLink固件
  7. 观察文件拷贝进度条，等待文件拷贝完成
  8. 文件拷贝完成后，U盘自动退出，LED1红色灯熄灭，LED1绿色为呼吸灯状态
  9. 复位开发板