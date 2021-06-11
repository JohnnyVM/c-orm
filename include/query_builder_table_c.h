#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"

/* Motto: all problems in programming science can be solved adding a level of indirection*/
enum table_property_type {
	table_property_column,
	table_property_constraint,
};

/** Generic table struct for hold table properties */
struct query_builder_table_property {
	enum table_property_type type;
	void* value;
	void (*free)(struct query_builder_table_property* property); /**< free method, mostly a helper */
};

/**
 * Count the list of argument for query_builder_table
 */
unsigned va_list_query_builder_table_property(struct query_builder_table_property* init, ...);

/**
 * A structure that store the information of a table of the database
 */
struct query_builder_table {
	/*@{*/
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< Name of the table represented */
	struct query_builder_column* columns; /**< column list */
	unsigned n_columns; /**< number of columns */
	struct query_builder_constraint* constraints; /**< constraint list */
	unsigned n_constraints; /**< number of constraints */
	/*@}*/
	/** List of methods for table */
	/*@{*/
	struct query_builder_table* (*copy)(struct query_builder_table*); /**< \todo methods copy */
	void (*free)(struct query_builder_table*); /**< method free */
	void (*add_constraint)(struct query_builder_table*); /**< method add constraint */
	void (*add_column)(struct query_builder_table*); /**< method add column */
	/*@}*/
};

/**
 * Add column to table
 */

/**
 * Prepare a table of \p name with the columns added as args
 *
 * sqlalchemy add a metadata arguments. probably i should too
 * @param[in] name name of the table
 * @param[in] n_args number of arguments
 * @param[in] ... origin list of table properties
 * @return table
 */
struct query_builder_table* query_builder_table(char* name, unsigned n_args, ...);
#define Table(ARG_NAME, ...) \
	query_builder_table(ARG_NAME, va_list_count(__VA_ARGS__), __VA_ARGS__)

struct query_builder_table_property* query_builder_table_column(char* name, struct query_builder_column* col, unsigned n_args, ...);
#define Column(ARG_NAME, ARG_COL, ...) \
	query_builder_table_column(ARG_NAME, ARG_COL, va_list_count(__VA_ARGS__), __VA_ARGS__)

#endif
