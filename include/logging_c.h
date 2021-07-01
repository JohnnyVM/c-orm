#if !defined(LOGGING_H)
#define LOGGING_H
#include <stdarg.h>

#if !defined(DEFAULT_LOG_LEVEL)
#define DEFAULT_LOG_LEVEL 0
#endif

enum LOG_LEVEL {
	EMERGENCY = 80, // system is unusable, i hope you will never need that
	ALERT = 70, // action must be taken inmmediately
	CRITICAL = 60, // critical conditions, usually at this point the program day
	ERROR = 50, // error conditions
	WARNING = 40, // warning conditions
	NOTICE = 30, // normal, but significant, condition
	INFO = 20, // informational message
	DEBUG = 10, // debug-level message
};

struct logging {
	char name[64];
	int log_level;
	char* format; //reserved
	struct logging* (*getLogger)(const char*);
	void (*setLogLevel)(struct logging*, const int);
	void (*debug)(const struct logging*, const char*, ...);
	void (*info)(const struct logging*, const char*, ...);
	void (*notice)(const struct logging*, const char*, ...);
	void (*warning)(const struct logging*, const char*, ...);
	void (*error)(const struct logging*, const char*, ...);
	void (*critical)(const struct logging*, const char*, ...);
	void (*alert)(const struct logging*, const char*, ...);
	void (*emergency)(const struct logging*, const char*, ...);
	void (*closeLogger)(const char*);
};

// warning: this cannot be used in windows
#define INIT_LOGGING { \
	.log_level = DEFAULT_LOG_LEVEL, \
	.getLogger = &get_logger, \
	.setLogLevel = &set_loglevel, \
	.debug = &log_debug, \
	.info = &log_info, \
	.notice = &log_notice, \
	.warning = &log_warning, \
	.error = &log_error, \
	.critical = &log_critical, \
	.alert = &log_alert, \
	.emergency = &log_emergency, \
	.closeLogger = &close_logger \
}

/**
 * \brief initialice the logger
 *
 * why getLogger and not "setLogger"? becouse normal implementation have to allow multiple loggers
 * that normally its done with a "global list of file descriptors" (see python implementation)
 * becouse no one want add the log as parameter, or reset the log multiple times.
 * \param name if null, the name of the binary will be used
 */
struct logging* get_logger(const char*);

/**
 * \brief set log level
 *
 * the signature its int for compatibility for allow multiple implementations, a enum should be too restrictive
 * the log level its spected that work as threshold. Example its set as warning,
 * level warning and bigger are logged
 * messages the low priority (debug, notice, info) are ignored
 * \param level level to set
 */
void set_loglevel(struct logging*, const int);

/**
 * \brief close the fd of the log
 */
void close_logger(const char*);

/**
 * \brief log the message with priority indicated
 */
void log_debug(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_notice(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_info(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_warning(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_error(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_critical(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_alert(const struct logging*, const char*, ...);
/**
 * \brief log the message with priority indicated
 */
void log_emergency(const struct logging*, const char*, ...);

#endif
