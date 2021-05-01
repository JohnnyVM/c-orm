#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"

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
 * @param[in] ... origin list of columns
 * @return table
 */
struct table* Table(char* name, ...);

#endif
