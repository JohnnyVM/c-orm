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

enum constraint_type {
	constraint_index,
	constraint_not_null,
	constraint_primary_key,
	constraint_default,
};

/** Count number of va_args ending in 0 or NULL */
unsigned va_list_void(void* init, ...);
extern unsigned va_list_constraint_type(enum constraint_type (*init)(void), ...);
struct query_builder_table_property;

#define va_list_count(...) va_list_count_expanded(__VA_ARGS__ __VA_OPT__(,) NULL)

#define va_list_count_expanded(X, ...) _Generic((X), \
		enum constraint_type (*)(void): va_list_constraint_type, \
		struct query_builder_table_property* : va_list_query_builder_table_property, \
		void* : va_list_void \
)(X __VA_OPT__(,) __VA_ARGS__ )

#endif
