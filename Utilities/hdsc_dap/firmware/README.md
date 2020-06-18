#DAPLink固件升级：
  1. 通过跳帽连接EV_F4A0_LQ176_V02开发板***J29:GND<——>J29:BOOT***
  2. 通过micro-USB数据线连接电脑和开发板
  3. 插入USB线后，LED1红色灯点亮，弹出U盘（盘符：***MAINTENANCE***）
  4. 将DAPLink固件***hc32f460_if_crc.hex***拖入U盘
  5. 等待文件拷贝完成
  6. 拷贝完成后，U盘自动退出，LED1绿色LED为呼吸灯状态
  7. 重新复位开发板