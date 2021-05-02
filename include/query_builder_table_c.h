#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"
#include "query_builder_column_c.h"

/** its necesary a signal of ho many columns are passed */
#define TABLE_END NULL

enum table_property_type {
	table_property_column,
	table_property_constrain,
};

union table_property_option {
	struct column* column;
};

struct table_property {
	enum table_property_type type;
	union table_property_option property;
};

struct columns {
	struct column* column;
	unsigned column_count;
};

/**
 * A structure that store the information of a table of the database
 */
struct table {
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< Name of the table represented */
	struct columns columns; /**< column list */
	/**
	 * List of methods for table
	 */
	/*@{*/
	struct table* (*copy)(struct table*); /**< \todo methods copy */
	/*@}*/
};

/**
 * Prepare a table of \p name with the columns added as args
 * @param[in] name name of the table
 * @param[in] ... origin list of table properties
 * @return table
 */
struct table* Table(char* name, struct table_property*, ...);

#endif
