#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"

/* Motto: all problems in programming science can be solved adding a level of indirection*/
enum query_builder_table_property_type {
	query_builder_table_property_column,
	query_builder_table_property_constraint,
	query_builder_table_property_schema,
};

/** Generic table struct for hold table properties */
struct query_builder_table_property {
	enum query_builder_table_property_type type;
	void* value;
	void (*free)(struct query_builder_table_property* property); /**< free method, mostly a helper */
};

/**
 * A structure that store the information of a table of the database
 */
struct query_builder_table {
	/*@{*/
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< Name of the table represented */
	struct query_builder_column** columns; /**< column list */
	unsigned n_columns; /**< number of columns */
	struct query_builder_constraint** constraints; /**< constraint list */
	unsigned n_constraints; /**< number of constraints */
	char schema[MAX_IDENTIFIER_NAME_LENGTH];
	/*@}*/
	/** List of methods for table */
	/*@{*/
	struct query_builder_table* (*copy)(struct query_builder_table*); /**< \todo methods copy */
	void (*free)(struct query_builder_table*); /**< method free */
	/** add constraint to table */
	struct query_builder_table* (*add)(struct query_builder_table*, struct query_builder_table_property*);
	/*@}*/
};

/**
 * Prepare a table of \p name with the properties added as args
 * as in the version of common the ending null is optional but the diference with Column
 * is that column had been evaluated twice if would be use the macro for expand as in Column
 * \todo probably i should add sqlalchemy metadata argument but i still dont find the use case
 * @param[in] name name of the table
 * @param[in] n_args number of arguments
 * @param[in] ... origin list of table properties
 * @return table
 */
struct query_builder_table* query_builder_table(char* name, ...);
#define Table(ARG_NAME, ...) \
	query_builder_table(ARG_NAME, __VA_ARGS__ __VA_OPT__(,) NULL)

struct query_builder_table_property* query_builder_table_column(char* name, struct query_builder_column* col, unsigned n_args, ...);
#define Column(ARG_NAME, ARG_COL, ...) \
	query_builder_table_column(ARG_NAME, ARG_COL, va_list_query_builder_constraint_type(__VA_ARGS__ __VA_OPT__(,) NULL), __VA_ARGS__ __VA_OPT__(,) NULL)

#endif
