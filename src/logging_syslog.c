#include <unistd.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "logging_c.h"

static const unsigned int buff_size = 200;
static struct logging *LOGGER_LIST = NULL;
static unsigned LOGGING_LENGTH = 0;
static bool syslog_open = false;

static struct logging* search_logger(const char* name)
{
	unsigned i;
	struct logging* log, *result = NULL;

	for(log = (struct logging*)LOGGER_LIST, i = 0; i < LOGGING_LENGTH && result == NULL; i++) {
		if(!strncmp(name, log[i].name, sizeof(log->name) + 1)) {
			result = log + i;
		}
	}

	return result;
}

/**
 * \brief initialice the logger
 *
 * \param name if null, empty string will be used
 * \return logging direction
 */
struct logging* get_logger(const char* name)
{
	int log_option = 0;
	char ttyname_buff[64], sentinel[] = "";

	if(name == NULL ) { name = sentinel; }

	struct logging* log = search_logger(name);
	if(log == NULL) {
		LOGGING_LENGTH += 1;
		void* tmp = realloc(LOGGER_LIST, LOGGING_LENGTH * sizeof *LOGGER_LIST);
		if(tmp == NULL) { return NULL; }

		log = (struct logging*)tmp + LOGGING_LENGTH - 1;
		*log = (struct logging)INIT_LOGGING;
		strncpy(log->name, name, sizeof log->name);
		log->name[sizeof log->name - 1] = '\0';
	}

	if(!syslog_open) {
		int status = ttyname_r(STDERR_FILENO, ttyname_buff, sizeof ttyname_buff);
		if(!status && ttyname_buff[0] != '\0') {
			log_option |= LOG_PERROR;
		}
		openlog(NULL, log_option, LOG_USER);
		if(status) {
			log->error(log, "ttyname_r: %s, disabling console log.", strerror(status));
		}
		syslog_open = true;
	}

	return log;
}

/**
 * \brief set the log level
 */
void set_loglevel(struct logging* log, const int level)
{
	if(log != NULL) {
		log->log_level = level;
	}
}

#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wformat-nonliteral"

/**
 * \brief close the logger, optional
 */
void close_logger([[maybe_unused]] const char* name)
{
	struct logging* log_list = LOGGER_LIST;
	free(log_list);
	closelog();
}

static inline void log_format(const struct logging* log, char* buff, unsigned size, const char* fmt, va_list va)
{
	char tbuff[size];
	struct logging sentinel = INIT_LOGGING;

	assert(size > 0);
	assert(buff);

	if(log == NULL) {
		log = &sentinel;
	}
	vsnprintf(tbuff, size, fmt, va);
	snprintf(buff, size, "[%s] %s", log->name, tbuff);
}

/**
 * \brief log message with priority debug
 * \param fmt format string
 */
void log_debug(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || DEBUG >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_DEBUG, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority info
 * \param fmt format string
 */
void log_info(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || INFO >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_INFO, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority notice
 * \param fmt format string
 */
void log_notice(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || NOTICE >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_NOTICE, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority warning
 * \param fmt format string
 */
void log_warning(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || WARNING >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_WARNING, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority error
 * \param fmt format string
 */
void log_error(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || ERROR >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_ERR, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority critical
 * \param fmt format string
 */
void log_critical(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || CRITICAL >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_CRIT, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority alert
 * \param fmt format string
 */
void log_alert(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || ALERT >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_ALERT, buff);
		va_end(args);
	}
}

/**
 * \brief log message with priority emergency
 * \param fmt format string
 */
void log_emergency(const struct logging* log, const char* fmt, ...)
{
	if(log == NULL || EMERGENCY >= log->log_level) {
		va_list args;
		va_start(args, fmt);
		char buff[buff_size];
		log_format(log, buff, buff_size, fmt, args);
		syslog(LOG_EMERG, buff);
		va_end(args);
	}
}
