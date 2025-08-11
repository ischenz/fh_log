#ifndef __LOG_H__
#define __LOG_H__

#include <stdarg.h>
#include <stdint.h>

#define LOG_ENABLE

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

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_DEBUG
#endif

/* 获取毫秒时间戳的函数指针（由用户提供） */
typedef uint32_t (*log_time_fn)(void);
typedef void (*output_fn)(const char *str);

void log_init(output_fn output ,log_time_fn time_func);
void log_printf(log_level_t level, const char *fmt, ...);

#ifdef LOG_ENABLE
#define LOGE(fmt, ...) log_printf(LOG_ERROR, "%s:%d %s(): " fmt "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGW(fmt, ...) log_printf(LOG_WARN,  "%s:%d %s(): " fmt "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGI(fmt, ...) log_printf(LOG_INFO,  "%s:%d %s(): " fmt "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGD(fmt, ...) log_printf(LOG_DEBUG, "%s:%d %s(): " fmt "\r\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#else
#define LOGE(fmt, ...) 
#define LOGW(fmt, ...) 
#define LOGI(fmt, ...) 
#define LOGD(fmt, ...) 
#endif

#endif
