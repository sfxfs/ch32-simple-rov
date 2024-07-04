# ch32-simple-rov

此为2024年全国大学生嵌入式芯片与系统设计竞赛参赛作品。

A simple ROV control program using the CH32V/F series MCU

Functions currently implemented:

  - JsonRPC server
  - Propellers control
  - JY901 data reading
  - MS5837 data reading
  - Online configuration modification

Functions not yet implemented:

  - Depth control
  - Direction control
  - Non-volatile configuration saving
  - Integration of RTOS

本作品的相关软件项目：

- 多平台水下机器人上位机 [ROV-Host](https://github.com/bohonghuang/rov-host)
- 基于 CH32V003 的 [串口水下深度传感器](https://github.com/sfxfs/ch32-depth-sensor)
