#if !defined(QUERY_BUILDER_H)
#define QUERY_BUILDER_H

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

/**
 * enum with the list of driver supported
 */
enum driver_type {
	none,
};

/**
 * A enum with the posible type of querys
 */
enum query_type {
	query_type_select, /**< select */
	query_type_insert, /**< insert */
	query_type_update, /**< update */
	query_type_delete, /**< delete */
};

/**
 * A structure that store the column information
 */
struct column {
	int type; /**< numeric code of the data type of the column */
	char* name; /**< name of the column */
	int octet_length; /**< length of the character representation of the datum in bytes */
	int indicator; /**< the indicator (indicating a null value or a value truncation) */
	void* data; /**< actual data item (therefore, the data type of this field depends on the query) */
};

/**
 * A structure that store the information necesary for build a query
 */
struct query {
	enum query_type type;
	struct query* (*values)(struct query*, char*, void*); // not used
};

/**
 * A structure that store the information of a table of the database
 */
struct table {
	char* table_name; /**< Name of the table represented */
};

/**
 * for a input of type char add the values
 * @param[out] query query builded
 * @param[in] key name of the column
 * @param[in] value od the column
 * @return error code if error else 0
 */
int insert_values_char(struct query* query, char* key, char* value);

/**
 * for a input of type integer add the values
 * @param[out] query query builded
 * @param[in] key name of the column
 * @param[in] value od the column
 * @return error code if error else 0
 */
int insert_values_int(struct query* query, char* key, intmax_t value);

/** More idiomatic insert for values */
#define insert_values(Q, K, X) _Generic((X), \
	char* : insert_values_char \
	const char* : insert_values_char \
	int : insert_values_int \
	const int : insert_values_int \
	unsigned : insert_values_int \
	const unsigned : insert_values_int \
	long : insert_values_int \
	const long : insert_values_int \
)(Q, K, X)

/**
 * add columns to the result
 * @param[out] query query builded
 * @param[in] ... list of column names
 * @return error code if error else 0
 */
int select(struct query* query, ...);

/**
 * add columns to the result casted as int
 * @param[out] query query builded
 * @param[in] ... list of column names
 * @return error code if error else 0
 */
int select_int(struct query* query, ...);

/**
 * add columns to the result casted as char
 * @param[out] query query builded
 * @param[in] ... list of column names
 * @return error code if error else 0
 */
int select_char(struct query* query, ...);

#endif
