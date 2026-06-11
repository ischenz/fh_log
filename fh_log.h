/*
 * @Author: ischen.x ischen.x@foxmail.com
 * @Date: 2026-04-28 15:10:42
 * @LastEditors: ischen.x ischen.x@foxmail.com
 * @LastEditTime: 2026-05-21 17:32:54
 *
 * MIT License
 * Copyright (c) 2026 ischen.x
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
#ifndef FH_LOG_H
#define FH_LOG_H

#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_ENABLE

#ifndef LOG_BUF_SIZE
#define LOG_BUF_SIZE 512
#endif

/* 日志等级 */
typedef enum {
    LOG_ERROR = 0,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} log_level_t;

/* ANSI 颜色码 */
#define LOG_COLOR_RED     "\033[0;31m"
#define LOG_COLOR_YELLOW  "\033[0;33m"
#define LOG_COLOR_GREEN   "\033[0;32m"
#define LOG_COLOR_CYAN    "\033[0;36m"
#define LOG_COLOR_RESET   "\033[0m"

#ifndef LOG_DEFAULT_LEVEL
#define LOG_DEFAULT_LEVEL LOG_DEBUG
#endif

/* 获取毫秒时间戳的函数指针（由用户提供） */
typedef uint32_t (*log_time_fn)(void);
typedef void (*output_fn)(const char *str);

int log_init(output_fn output, log_time_fn time_func);
void log_set_level(log_level_t level);
log_level_t log_get_level(void);
void log_printf(log_level_t level, const char *fmt, ...);
void log_color_enable(int enable);
int  log_color_is_enabled(void);

#define __FILENAME__ ( \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : \
    (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)) \
)

#ifdef LOG_ENABLE
#define LOGE(fmt, ...) log_printf(LOG_ERROR, "%s:%d %s(): " fmt "\r\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_printf(LOG_WARN,  "%s:%d %s(): " fmt "\r\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_printf(LOG_INFO,  "%s:%d %s(): " fmt "\r\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_printf(LOG_DEBUG, "%s:%d %s(): " fmt "\r\n", __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

#else
#define LOGE(fmt, ...) ((void)0)
#define LOGW(fmt, ...) ((void)0)
#define LOGI(fmt, ...) ((void)0)
#define LOGD(fmt, ...) ((void)0) 
#endif

#ifdef __cplusplus
}
#endif

#endif /* FH_LOG_H */
