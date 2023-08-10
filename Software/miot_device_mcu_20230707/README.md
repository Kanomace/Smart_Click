# 概述

本文将介绍如何使用小米通用`MCU DEMO`接入小米`Wi-Fi`模组，并开发用户应用程序。在开始前，请先参阅[小米IoT开发者平台](https://iot.mi.com/new/index.html)中的开发引导了解小米模组串口开发相关概念

# 搭建开发环境

## 硬件环境

- 小米`Wi-Fi`模组采用串口方式与`MCU`进行通信，且`MCU DEMO`采用串口实时打印`MCU`日志，平台建议具备2个及以上开发串口

- `MCU`通信串口硬件配置为：波特率115200、8位有效数据、无奇偶校验、1位停止位

## 软件环境

### 获取`MCU DEMO`

首先从小米[Github仓库](http://v9.git.n.xiaomi.com/miot_embedded/mcu_demo)获取`MCU DEMO`，获取到的`DEMO`目录结构如下：

```
.
├── arch
│   ├── linux
│   │   └── Makefile
│   └── stm32
│       ├── core
│       ├── device
│       ├── lib
│       ├── main
│       ├── MDK-ARM
│       └── system
├── miio
├── user
│   ├── app
│   ├── handler
│   ├── ota
│   └── user_config.h
├── tools
└── README.md
```

其中：
- README.md：`MCU DEMO`开发相关说明
- arch：需要用户适配硬件平台的相关接口函数
- arch/linux：`linux`开发环境下实现的`MCU DEMO`工程
- arch/linux/Makefile：`linux`开发环境下的工程编译脚本
- arch/stm32：`stm32`开发环境下实现的`MCU DEMO`工程
- arch/stm32/core：`stm32`核心启动代码
- arch/stm32/device：软件适配层，需要在`stm32`平台上适配对应的接口
- arch/stm32/lib：`stm32`库函数
- arch/stm32/main：`stm32`主函数
- arch/stm32/system：`stm32`系统配置，uart，delay等
- arch/stm32/MDK-ARM：`stm32`工程编译配置相关目录
- miio：小米相关代码存放目录
- user：用户实现SPEC定义的产品功能相关目录
- user/app：用户自定义函数存放目录
- user/handler：由小米代码自动生成平台生成的函数文件
- user/ota：用户硬件平台`ota`相关函数
- user/user_config.h：用户配置文件
- tools：固件crc校验工具，MCU固件上传到云端前需要使用工具添加crc校验

# 用户开发说明

以下针对用户需要关心的**arch和user目录文件**做出相关说明

## 1.arch目录

arch目录主要存放两个demo工程，linux和stm32开发环境下的demo工程，其主要实现了相关接口的适配。下面以stm32为例进行说明：

​	在stm32的工程中，arch/stm32/device目录下为软件适配的接口函数。需要**用户适配的相关接口函数**，主要包括`MCU`串口通信接口、`debug`调试接口（已经由小米给出例程）、`MCU`硬件初始化接口、`RTOS`相关接口、`MCU OTA`升级相关接口。用户只需根据定义好的相关接口格式，做好适配工作即可；其中需要注意的是：用户应将使用到的头文件包含在`arch_define.h`中，使用时使用`miio_define.h`统一调用

### `MCU`串口通信适配

下面以开发过程中比较重要的串口为例，说明具体的接口适配流程。串口部分需要用户适配的有以下几个接口：

```c
uart_error_t _uart_init(miio_uart_t *uart);
int _uart_send_str(miio_uart_t *uart, const char* str);
int _uart_send_byte(miio_uart_t *uart, const char c);
int _uart_send_str_wait_ack(miio_uart_t *uart, const char* str);
int _uart_recv_str(miio_uart_t *uart, uint8_t* buf, int buf_len, uint32_t timeout_ms);
int _uart_recv_byte(miio_uart_t *uart, uint8_t* c, uint32_t timeout_ms);
int _Xmodem_recv_str(miio_uart_t *uart, uint8_t* buf, uint32_t timeout_ms);
```

其中:
- _uart_init：主要在`DEMO`启动前对`MCU`串口硬件进行初始化操作
- _uart_send_str：主要将用户字符串数据发送到通信串口缓存
- _uart_send_byte：主要将用户字节数据发送到通信串口缓存
- _uart_send_str_wait_ack：主要将用户字符串数据发送到通信串口缓存，并确认接收到`Wi-Fi`模组的"ok\r"回复
- _uart_recv_str：主要将`MCU`通信串口缓存中的字符串数据提取到用户数组中
- _Xmodem_recv_str：主要用于`MCU`OTA升级时，串口缓存中的字符串数据提取到用户数组中
- _uart_recv_byte：主要将`MCU`通信串口缓存中的字节数据提取到用户数组中
- _uart_deinit：主要销毁打开的`MCU`串口硬件配置

用户需要根据采用的`MCU`串口硬件的具体情况进行适配操作，`DEMO`中保留了`linux`环境下的具体实现函数

## user/app目录

user/app目录主要存放**用户自定义的功能函数**

- 用户自定义的功能函数，建议**统一放在user/app/user_app_func.c文件中，并统一在user/app/user_app_main.c中的user_app_main()接口中调用**
- 该目录下已经为用户提供了相关例程可直接调试使用，更具体的功能函数开发由用户进行

## user/handler目录

user/handler目录主要存放，**用户在代码自动生成平台生成的函数**，具体内容以用户在平台上定义的功能为准（`DEMO`中以小米`model：miot.plug.plugv1`为例）

- 用户需要在代码自动生成平台上根据产品`model`，生成相应代码并复制到该文件夹下。该部分代码屏蔽了`MCU`与`Wi-Fi`模组通信的具体细节，并预留了用户接口

### 功能函数适配说明

- ***开始适配前，用户需要了解小米`model和`SPEC`的[相关说明文档](https://iot.mi.com/new/doc/04-%E5%B5%8C%E5%85%A5%E5%BC%8F%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97/03-WiFi%E2%80%94%E9%80%9A%E7%94%A8%E6%A8%A1%E7%BB%84%E6%8E%A5%E5%85%A5/01-%E9%80%9A%E7%94%A8%E6%A8%A1%E7%BB%84%E6%A0%87%E5%87%86%E5%8D%8F%E8%AE%AE%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97.html)***

- 下面以通用模型`miot.plug.plugv1`中的`P_2_1_On_doSet`函数为例，说明用户在代码自动生成平台上获取代码后，如何进行适配操作：

```c
static void P_2_1_On_doSet(property_operation_t *o)
{
    /* judge value format */
    if (o->value->format != PROPERTY_FORMAT_BOOLEAN)
    {
        o->code = OPERATION_ERROR_VALUE;
        return;
    }

    /* TODO : execute operation */

    /* return execution result */
    o->code = OPERATION_OK;

    return;
}
```

- 该部分代码放置在`user/handler/S_2_Switch_doSet.c`文件中，`P_2_1_On_doSet`函数主要表示对于`MCU`控制的开关进行通断操作

- 用户**只需要对于函数中标记的`TODO`部分进行具体实现**：在接收到小米后台下发的控制指令后，`DEMO`会自动解析`Wi-Fi`模组的串口指令`down set_properties 2 1 [true]/[false]`并进入到该函数内，用户只需做出`打开/关闭`开关的动作，并将返回结果赋值给结构体指针`property_operation_t *o`的`code`成员即可（code成员为枚举类型，具体定义可在`property_operation_t`结构体中查看）

## user/ota目录

user/ota目录主要存放**MCU OTA升级相关代码**

- 小米已经为用户做好标准的`Xmodem`通信流程，让`MCU`能够顺利从小米`Wi-Fi`模组处获取`MCU`升级固件

- 在用户收到`Wi-Fi`模组传输的`MCU`升级固件后，**更进一步的固件升级操作**，由用户在`user/arch/arch_ota.c`文件中定义的接口函数完成

- ***关于用户如何上传MCU固件到小米开发者平台，和如何通过后台指令进行MCU OTA升级，可参阅：[小米开发者平台OTA文档](https://iot.mi.com/new/doc/04-%E5%B5%8C%E5%85%A5%E5%BC%8F%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97/03-WiFi%E2%80%94%E9%80%9A%E7%94%A8%E6%A8%A1%E7%BB%84%E6%8E%A5%E5%85%A5/03-%E4%B8%B2%E5%8F%A3MCU%20OTA%E5%8D%8F%E8%AE%AE.html)***

## user/user_config.h文件

- user/user_config.h文件存放用户配置选项，其中为用户准备了`USER_OS_ENABLE`、`USER_OTA_ENABLE`等宏定义开关进行代码的适配，定义了`USER_MODEL`、`USER_MCU_VERSION`等宏定义需要用户修改为开发中采用的`model`和`MCU`版本号
