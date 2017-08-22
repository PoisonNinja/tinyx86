#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define TINYX86_VERSION_NAME "tinyx86"

#define TINYX86_VERSION_MAJOR 0
#define TINYX86_VERSION_MINOR 0
#define TINYX86_VERSION_PATCH 1

typedef size_t addr_t;

#define TINYX86_MINIMUM_MEMORY 128
#define TINYX86_MAXIMUM_MEMORY 1024

typedef void (*log_LockFn)(void *udata, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __VA_ARGS__)
#define log_info(...) log_log(LOG_INFO, __VA_ARGS__)
#define log_warn(...) log_log(LOG_WARN, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __VA_ARGS__)

void log_set_udata(void *udata);
void log_set_lock(log_LockFn fn);
void log_set_fp(FILE *fp);
void log_set_level(int level);
void log_set_quiet(int enable);

void log_log(int level, const char *fmt, ...);
