#if !defined(QUERY_BUILDER_COMMON_C_H)
#define QUERY_BUILDER_COMMON_C_H

#define QUERY_BUILDER_LOGGER_NAME "query_builder"

/** Max name length for identify names of tables, columns, or other database objects */
#define MAX_IDENTIFIER_NAME_LENGTH 64

/**
 * enum with the list of driver supported
 */
enum driver_type {
	none,
};

#endif
