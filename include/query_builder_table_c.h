#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"

/** its necesary a signal of ho many columns are passed */
#define TABLE_END NULL

/* Motto: all problems in programming science can be solved adding a level of indirection*/
enum table_property_type {
	table_property_column,
	table_property_constrain,
};

union table_property_option {
	struct column *column;
	struct constraint *constraint;
};

struct table_property {
	enum table_property_type type;
	union table_property_option property;
};

/**
 * A structure that store the information of a table of the database
 */
struct table {
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< Name of the table represented */
	struct column* column; /**< column list */
	unsigned n_columns;
	struct constraint* constraint; /**< constraint list */
	unsigned n_constraints;
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

struct table_property* Column(char* name, struct column* col, ...);

#endif
