/*
 * @Author: ischen.x ischen.x@foxmail.com
 * @Date: 2025-08-06 17:10:15
 * @LastEditors: ischen.x ischen.x@foxmail.com
 * @LastEditTime: 2025-08-08 11:08:38
 * 
 * Copyright (c) 2025 by fhchengz, All Rights Reserved. 
 */
#include "fh_log.h"
#include <string.h>

static void (*log_output)(const char* str) = NULL;
static log_time_fn log_get_time = NULL;

void log_init(void (*output_func)(const char *str), log_time_fn time_func)
{
    if (output_func != NULL && time_func != NULL) {
        log_output = output_func;
        log_get_time = time_func;
        LOGI("Log system initialized with output function: %p and time function: %p", (void *)log_output, (void *)log_get_time);
    } else {
        LOGE("Log initialization failed: output_func or time_func is NULL");
    }
}

void log_printf(log_level_t level, const char *fmt, ...) 
{
    if (level > LOG_LEVEL || log_output == NULL) return;

    char buf[512] = {0};
    int offset = 0;

    /* 根据 level 选择颜色和标签 */
    const char *tag_color;
    const char *tag;
    switch (level) {
        case LOG_ERROR: tag_color = LOG_COLOR_RED;    tag = "[E]"; break;
        case LOG_WARN:  tag_color = LOG_COLOR_YELLOW; tag = "[W]"; break;
        case LOG_INFO:  tag_color = LOG_COLOR_GREEN;  tag = "[I]"; break;
        case LOG_DEBUG: tag_color = LOG_COLOR_CYAN;   tag = "[D]"; break;
        default:        tag_color = LOG_COLOR_RESET;  tag = "[ ]"; break;
    }

    /* 颜色 + 标签 */
    offset += snprintf(buf + offset, sizeof(buf) - offset, "%s%s ", tag_color, tag);

    /* 时间戳（和标签同颜色） */
    if (log_get_time) {
        uint32_t ms = log_get_time();
        offset += snprintf(buf + offset, sizeof(buf) - offset,
                           "[%lu ms] ", (unsigned long)ms);
    }

    /* 拼接用户日志 */
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf + offset, sizeof(buf) - offset, fmt, args);
    va_end(args);

    /* 颜色复位 */
    strncat(buf, LOG_COLOR_RESET, sizeof(buf) - strlen(buf) - 1);

    /* 输出 */
    log_output(buf);
}
