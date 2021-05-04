#if !defined(QUERY_BUILDER_COMMON_C_H)
#define QUERY_BUILDER_COMMON_C_H

#include <stdbool.h>

#define QUERY_BUILDER_LOGGER_NAME "query_builder"

/** Max name length for identify names of tables, columns, or other database objects */
#define MAX_IDENTIFIER_NAME_LENGTH 64

#define STRINGNIFY(a) str(a)
#define str(a) #a

/**
 * enum with the list of driver supported
 */
enum driver_type {
	ANSI,
};

/**
 * Wrapper for va_args lists
 * this allow distinc NULL fom error
 */
struct va_arg_wrap {
	bool error;
	void* payload;
};

#endif
