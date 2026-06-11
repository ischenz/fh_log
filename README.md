<!--
 * @Author: ischen.x ischen.x@foxmail.com
 * @Date: 2025-08-08 11:11:04
 * @LastEditors: ischen.x ischen.x@foxmail.com
 * @LastEditTime: 2026-06-11 14:30:00
 * 
 * Copyright (c) 2025 by fhchengz, All Rights Reserved. 
-->
# fh_log

一个轻量级、可移植的 C 语言日志模块，支持彩色输出、日志等级、时间戳自定义，适用于嵌入式和桌面开发。

![效果演示](./pic01.png)

## 特性

- 支持日志等级（ERROR / WARN / INFO / DEBUG）
- 支持 **运行时** 动态切换日志等级（`log_set_level`）
- 支持 ANSI 彩色输出，可 **运行时** 开关（`log_color_enable`）
- 支持自定义输出函数（如串口、文件、网络等）
- 支持自定义时间戳函数
- 日志宏开关（`LOG_ENABLE`），可条件编译裁剪
- Buffer 大小可配置（`LOG_BUF_SIZE`）
- `extern "C"` 兼容 C++ 编译

## 快速开始

### 1. 添加文件

将 `fh_log.c`、`fh_log.h` 添加到你的工程。

### 2. 实现输出函数和时间戳函数

```c
// 例：串口输出
void uart_send_str(const char *str) {
    // 用户实现：通过串口发送字符串
}

// 例：获取毫秒时间戳
uint32_t log_time(void) {
    return HAL_GetTick(); // 或其他获取 ms 时间戳的函数
}
```

### 3. 初始化日志系统

```c
#include "fh_log.h"

if (log_init(uart_send_str, log_time) != 0) {
    // 初始化失败：output 或 time_func 为 NULL
}
```

### 4. 使用日志宏

```c
LOGE("错误信息: %d", err_code);
LOGW("警告信息");
LOGI("普通信息");
LOGD("调试信息: var=%d", var);
```

## 运行时 API

### 日志等级

```c
// 获取当前日志等级
log_level_t level = log_get_level();

// 设置日志等级（高于当前等级的日志不会被输出）
log_set_level(LOG_WARN);  // 只输出 ERROR 和 WARN
log_set_level(LOG_DEBUG); // 输出所有等级
```

### 颜色开关

```c
// 查询颜色是否启用
int enabled = log_color_is_enabled();  // 返回 0 或 1

// 开关 ANSI 颜色（关闭后输出纯文本，适合不支持 ANSI 的终端或记录文件）
log_color_enable(0);  // 关闭颜色
log_color_enable(1);  // 开启颜色
```

## 配置

编译时可通过宏定义配置：

| 宏 | 默认值 | 说明 |
|----|--------|------|
| `LOG_DEFAULT_LEVEL` | `LOG_DEBUG` | 初始日志等级（可运行时覆盖） |
| `LOG_BUF_SIZE` | `512` | 单条日志 buffer 大小（字节） |
| `LOG_ENABLE` | 定义（启用） | 定义则启用日志宏，否则所有宏展开为 `((void)0)` |

```bash
# 编译时修改默认等级
-DLOG_DEFAULT_LEVEL=LOG_WARN
# 修改 buffer 大小
-DLOG_BUF_SIZE=256
# 完全禁用日志
-ULOG_ENABLE
```
