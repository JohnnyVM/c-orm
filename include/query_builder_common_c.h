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

/**
 * The errors in the library are reported in errno with values <0
 */
enum library_errno {
	NO_ENOUGH_SPACE = -1
};

#endif
