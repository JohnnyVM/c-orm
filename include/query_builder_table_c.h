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
	struct column* columns; /**< column list */
	unsigned n_columns;
	struct constraint* constraints; /**< constraint list */
	unsigned n_constraints;
	/**
	 * List of methods for table
	 */
	/*@{*/
	struct table* (*copy)(struct table*); /**< \todo methods copy */
	void (*free)(struct table*); /**< method free */
	/*@}*/
};

/**
 * Prepare a table of \p name with the columns added as args
 * @param[in] name name of the table
 * @param[in] ... origin list of table properties
 * @return table
 */
struct table* Table(char* name, struct table_property*, ...);

struct table_property* query_builder_table_column(char* name, struct column* col, unsigned n_args, ...);
#define Column(ARG_NAME, ARG_COL, ...) \
	query_builder_table_column(ARG_NAME, ARG_COL, va_list_count(__VA_ARGS__), __VA_ARGS__)

#endif
