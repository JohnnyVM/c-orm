#if !defined(QUERY_BUILDER_COLUMN_C_H)
#define QUERY_BUILDER_COLUMN_C_H

#include <stdarg.h>

#include "query_builder_common_c.h"
#include "query_builder_constraint_c.h"

enum column_type  {
	query_builder_VARCHAR,
	query_builder_INTEGER,
};

/**
 * A structure that store the column information
 */
struct query_builder_column {
	/*@{*/
	enum column_type type; /**< numeric code of the data type of the column */
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< name of the column */
	int octet_length; /**< length of the character representation of the datum in bytes */
	int indicator; /**< the indicator (indicating a null value or a value truncation) */
	int nullable; /**< nullable column */
	enum constraint_type (**constraints)(void); /**< pointer to funtions */
	unsigned n_constraints;
	/*@}*/
	/**
	 * List of methos from query
	 */
	/*@{*/
	struct query_builder_column* (*copy)(struct query_builder_column*); /** copy method */
	void (*free)(struct query_builder_column*); /**< free method */
	/*@}*/
	/*@{*/
	void* data; /**< actual data item, has to be in the last position */
	/*@}*/
};

/**
 * create a column of type varchar
 * @param length varchar length
 * @param ... list of column properties/modificators (primary_key, not null etc)
 * @return created column
 */
struct query_builder_column* query_builder_column(char* name, struct query_builder_column* column, unsigned n_args, va_list constraints);

/**
 * create a column of type varchar
 * @param length varchar length
 * @return anonymous column setted as integer use
 */
struct query_builder_column* VARCHAR(unsigned length);

/**
 * create a column of type integer
 * @return anonymous column setted as integer use
 */
struct query_builder_column* INTEGER(void);

#endif
