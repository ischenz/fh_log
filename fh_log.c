/*
 * @Author: ischen.x ischen.x@foxmail.com
 * @Date: 2025-08-06 17:10:15
 * @LastEditors: ischen.x ischen.x@foxmail.com
 * @LastEditTime: 2026-06-11 14:10:45
 *
 * MIT License
 * Copyright (c) 2025 ischen.x
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "fh_log.h"
#include <stdio.h>
#include <string.h>

static output_fn log_output = NULL;
static log_time_fn log_get_time = NULL;
static log_level_t log_runtime_level = LOG_DEFAULT_LEVEL;
static int log_color_enabled = 1;

int log_init(output_fn output, log_time_fn time_func)
{
    if (output != NULL && time_func != NULL) {
        log_output = output;
        log_get_time = time_func;
        LOGI("Log system initialized with output function: %p and time function: %p", (void *)log_output, (void *)log_get_time);
        return 0;
    }
    return -1;
}

void log_set_level(log_level_t level)
{
    log_runtime_level = level;
}

log_level_t log_get_level(void)
{
    return log_runtime_level;
}

void log_color_enable(int enable)
{
    log_color_enabled = enable ? 1 : 0;
}

int log_color_is_enabled(void)
{
    return log_color_enabled;
}

void log_printf(log_level_t level, const char *fmt, ...) 
{
    if (level > log_runtime_level || log_output == NULL) return;

    char buf[LOG_BUF_SIZE] = {0};
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
    if (log_color_enabled) {
        offset += snprintf(buf + offset, sizeof(buf) - offset, "%s%s ", tag_color, tag);
    } else {
        offset += snprintf(buf + offset, sizeof(buf) - offset, "%s ", tag);
    }

    /* 时间戳 */
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
    if (log_color_enabled) {
        strncat(buf, LOG_COLOR_RESET, sizeof(buf) - strlen(buf) - 1);
    }

    /* 输出 */
    log_output(buf);
}
