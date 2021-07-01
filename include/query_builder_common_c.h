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

enum query_builder_constraint_type {
	query_builder_constraint_index,
	query_builder_constraint_not_null,
	query_builder_constraint_primary_key,
	query_builder_constraint_default_value,
};

/** Count number of va_args ending in 0 or NULL */
unsigned va_list_void(void* init, ...);

#endif
