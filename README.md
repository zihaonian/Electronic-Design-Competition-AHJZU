## 摘要

​		随着时代的进步和发展，单片机技术已经普及到我们生活，工作，科研，各个领域，已经成为一种比较成熟的技术。本文介绍了 DS18B20 数字温度传感器，DS1302 时钟模块，LCD1602 以及矩阵键盘在主控 STC89C52 控制下的硬件连接及 软件编程，并给出了软件流程图以及硬件原理图。

​		该系统由上位机和下位机两大部分组成。下位机中包含四种模式（学号显示 模式-温度及时间显示模式-时间修改模式-数据回查模式），此四种模式分别对应4 个交互界面（由显示终端 LCD1602 完成）。利用矩阵键盘中 S15 和 S16 来完成 界面切换（S16 用于学号显示模式-温度及时间显示模式切换，S15 用于时间修改 模式-数据回查模式切换）。配置定时器 0，50ms 产生一次中断，在中断服务函数 中定义计时计数变量 Count_i,计数为 200 时（即 10s）通过串口向上位机发送一 次温度时间数据。计数为 400 时（即 20s）记录一次时间与温度。在时间修改模 式中，通过矩阵键盘 S1 S5 S2 S6 S3 S7 S4 S8 S9 S13 S10 S14 分别完成对年， 月，日，时，分，秒的增减，实现时间修改。在数据回查模式中，通过矩阵键盘S8 与 S12 完成对数据的翻页查询。此外按下 S11 将会把查询界面的数据通过串 口发送给上位机，实现数据的回显功能。上位机部分使用了通用 PC。

​		该系统实现了温度测量，时间显示，时间修改，数据记录，数据回显等功能。

#### 关键词：温度测量   STC89C52   DS18B20   DS1302   单总线系统

## 文件结构

```
NUEDC/
│
├── Project/                    # 存放电子设计大赛比赛题目
├── res/                        # 存放readme.md图表文件
├── file_output/             	# 存放STC51工程文件				
├── SCH/                        # 电路原理图.pdf
│   ├── 简易温度测量系统SCH.svg
│   └── 简易温度测量系统SCH.pdf
├── result/                     # 存放测试结果.pdf
│   └── 上位机接收数据.txt
│
├── 设计报告/                     # 设计报告文件 
│   └── 设计报告.pdf
│
└── README.md                   # 本文档，提供测试方案与结果、系统总体设计以及文件结构等信息
```

##  前言

​		了解到该系统的需求，为了将数据信息显示出来，采用了一块 LCD1602 液晶 显示屏，并通过编写函数完成初始化，能够完成显示目的。其次考虑的是完成温 度的测量，本系统采用 DS18B20 温度传感器采集温度数据，并配置实现单总线通 信读取温度数据，并通过 LCD1602 将温度数据显示出来。计时模块采用 DS1302

 实时时钟，该时钟内部带有备用电源，可以使时间数据掉电不丢失，并通过软件 编写程序，实现了通过按键设置显示时间的功能。为了完成与上位机的通信，配置了 UART 串口函数，最终，通过串口可以实现向上位机发送历史温度以及时间 数据，达到数据保存目的。至此该系统总体需求均已实现。

<div align=center><img  src ="https://github.com/zihaonian/Electronic-Design-Competition-AHJZU/blob/main/res/DS1302 real-time clock wiring diagram.jpg"/></div>

##  系统方案设计

###  方案一

​		由于本设计是测温电路，可以使用热敏电阻之类的器件利用其感温效应，在将随被测温度变化的电压或电流采集过来，进行 A/D 转换后，就可以用单片机进 行数据的处理，在显示电路上，就可以将被测温度显示出来，这种设计需要用到A/D 转换电路，感温电路比较麻烦。入库过程图

### 倒车入库

<div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Reversing into storage.jpg"/></div>

### 侧方位停车

<div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Side parking.jpg"/></div>

## 系统总体设计

### OpenMV检测车库方法

为了精确检测到车库位置，对摄像头采集到的视频进行实时的色块检测。对视频图像进行二值化处理并设定一个30×140的ROI区域，小车前进过程中不断检测并统计该区域中黑色像素点个数，当像素数达到设定阈值即表示检测到了车库交接处，通过IO口发送高电平信号给单片机，完成车库检测。车库检测方法示意图，如图所示。

<div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Schematic diagram of garage inspection.jpg"/></div>

### STM32c语言程序设计		

​		本系统以STM32F407作为核心处理器，主要由OpenMV模块、循迹检测模块、蜂鸣器模块、直流电机、舵机等部分构成。该系统总体设计图以及系统设计清单如下图所示：                       



<div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Schematic diagram of module composition.png"/></div>

| 元件         | 数量 | 元件           | 数量 | 元件             | 数量 |
| ------------ | ---- | -------------- | ---- | ---------------- | ---- |
| 四位独立按键 | 1    | 星瞳OpenMV     | 1    | 有源蜂鸣器       | 1    |
| 电源         | 1    | 红外发射传感器 | 2    | STM32F4芯片      | 1    |
| 稳压模块     | 1    | 编码器         | 1    | 单片机最小系统板 | 1    |
| 直流电机     | 1    | 舵机           | 1    |                  |      |

## 单元电路设计

### 单片机最小系统设计

微处理器选择STM32F407单片机，其运算速度非常快，具有丰富的外设接口，适合电动车的运行，其主要性能指标如下：

- 工作频率：168MHZ

- 时钟配置： STM32F407的SYSCLK最高频率是168MHz；HCLK最高频率为168MHz；内部RC振荡器，可产生16MHz的时钟信号；

- 14个定时器，包含：高级定时器 TIM1、TIM8 ；通用定时器 TIM2、TIM5、TIM3、TIM4，TIM9-TIM14；基本定时器 TIM6、TIM7 。

- 6个UART（串口），可灵活地与外部设备全双工数据交换。

- 丰富的IO口，方便连接外设。


STM32F407单片机核心板电路图如附录所示，其中包括MCU、晶振和复位电路以及外接设备模块连接情况。

### 电机驱动模块设计

该驱动模块采用DRV8701E芯片，该芯片具有驱动力强，响应频率高，体积较小等优良性能，同时带有使能控制端。同时控制电机正反转无需两路PWM，只需一路PWM加高低电平即可实现正反转，节省单片机PWM资源。驱动模块电路原理图如图所示。

 <div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Schematic diagram of the drive module.png"/></div>                              

### 独立按键模块设计

四位独立按键一端接地，另一端接单片机IO口，将单片机IO输入上拉，当按键按下时IO口被接地，IO口电平反翻，用于按键扫描检测。该模块原理图如图所示。

  <div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Schematic diagram of a stand-alone button module.png"/></div>   

### 巡线避障模块设计

通过该模块检测车库边缘库线。使用TCRT5000传感器，当检测到黑线时反射回的红外线强度不够大，红外接收管关闭，输出端为高电平，指示灯熄灭。该模块电路原理图如图所示。

  <div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Schematic diagram of the obstacle avoidance module.png"/></div>   

## 系统程序设计

 <div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Overall flow chart of the system software.jpg"/></div>  

# 测试方案与测试结果

## 测试仪器

电动车、秒表、比赛赛道。

## 测试方案

### “邻库有车”情况停车测试

将库1、库3、库4、库6分别放入车辆，分三次将电动车分别置于距起点5cm、15cm、30cm处，启动电动车，小车蜂鸣器第一次响起（即开始倒车）时按下秒表，待小车蜂鸣器第二次响起（即倒车完成）停止计时，记录时间。分别观察小车倒车入库以及侧方位入库的完成情况，记录小车停止后与车库边的夹角。

### “邻库无车”情况停车测试

移除库1、库3、库4、库6中的车辆，分三次将电动车分别置于距起点5cm、15cm、30cm处，启动电动车，小车蜂鸣器第一次响起（即开始倒车）时按下秒表，待小车蜂鸣器第二次响起（即倒车完成）停止计时，记录时间。分别观察小车倒车入库以及侧方位入库的完成情况，记录小车停止后与车库边的夹角。

## 测试结果

​															电动车“邻库有车”倒车入库情况测试结果

| 小车距起点距离（cm） | 是否完成倒车 | 是否压线 | 倒车所用时间（s） | 小车停止后与库边夹角（**°**） |
| -------------------- | ------------ | -------- | ----------------- | ----------------------------- |
| 5                    | 是           | 否       | 2.87              | 7                             |
| 15                   | 是           | 否       | 2.45              | 9                             |
| 30                   | 是           | 否       | 2.73              | 10                            |

​															电动车“邻库有车”侧方位入库情况测试结果

| 小车距起点距离（cm） | 是否完成倒车 | 是否压线 | 倒车所用时间（s） | 小车停止后与库边夹角（**°**） |
| -------------------- | ------------ | -------- | ----------------- | ----------------------------- |
| 5                    | 是           | 否       | 2.25              | 9                             |
| 15                   | 是           | 否       | 2.46              | 11                            |
| 30                   | 是           | 否       | 2.15              | 10                            |

​															电动车“邻库无车”倒车入库情况测试结果

| 小车距起点距离（cm） | 是否完成倒车 | 是否压线 | 倒车所用时间（s） | 小车停止后与库边夹角（**°**） |
| -------------------- | ------------ | -------- | ----------------- | ----------------------------- |
| 5                    | 是           | 否       | 2.45              | 9                             |
| 15                   | 是           | 否       | 2.96              | 9                             |
| 30                   | 是           | 否       | 2.28              | 10                            |

​															电动车“邻库无车”侧方位入库情况测试结果

| 小车距起点距离（cm） | 是否完成倒车 | 是否压线 | 倒车所用时间（s） | 小车停止后与库边夹角（**°**） |
| -------------------- | ------------ | -------- | ----------------- | ----------------------------- |
| 5                    | 是           | 否       | 2.64              | 8                             |
| 15                   | 是           | 否       | 2.27              | 10                            |
| 30                   | 是           | 否       | 2.84              | 11                            |

## 测试结果分析

根据“邻库有车”情况停车测试结果，可以看出电动车在任意位置发车，能够识别到空车库，并完成倒车入库和侧方位入库，车身位置与库边夹角在10°左右；倒车所用时间均在30s以内。

根据“邻库无车”情况停车测试结果，可以看出电动车在任意位置发车，能够识别到目标车库，并完成倒车入库和侧方位入库，车身位置与库边夹角在10°左右；倒车所用时间均在30s以内。

综上，该系统能够完成题目所给任务。

# 结论

该系统以正点原子STM32F407VET6作为核心板，由OpenMV模块、循迹检测模块、蜂鸣器模块和前轮转向式四轮车构成。该自动泊车系统的设计采用了模块化思想，完成了色块识别模块及自动泊车模块的设计，达到集中检测车库、自动泊车于一体的目的，实现了自动泊车功能，较好的完成了设计要求。

通过测试方案检验，本系统在邻库有车和邻库无车的情况下，均能完成自动泊车功能。

# 实验结果

​															邻库有车倒车入库测试结果图

 <div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/There is a diagram of the test results of the reversing of the car in the adjacent warehouse.jpg"/></div> 

​															邻库有车侧方位停车测试结果图

 <div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/There is a map of the parking test results on the side of the car in the adjacent warehouse.jpg"/></div> 

​															邻库无车倒车入库测试结果图

<div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Diagram of the test results of reversing and warehousing without vehicles in adjacent warehouses.jpg"/></div> 

​															邻库无车侧方位停车测试结果图

<div align=center><img  src ="https://github.com/zihaonian/Ti_Cup_NUEDC-2022-10-B/blob/main/res/Diagram of the test results of parking on the side of the adjacent garage without a car.jpg"/></div> 
