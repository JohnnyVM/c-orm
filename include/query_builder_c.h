#if !defined(QUERY_BUILDER_C_H)
#define QUERY_BUILDER_C_H

#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_table_c.h"

enum query_builder_type {
	query_builder_type_not_selected,
	query_builder_type_select,
	query_builder_type_update,
	query_builder_type_delete,
	query_builder_type_insert,
};

/**
 * A structure that store the information of a query
 */
struct query_builder{
	/*@{*/
	enum query_builder_type type;
	struct query_builder_table* table;
	/*@}*/
	/** List of methods for query */
	/*@{*/
	struct query_builder* (*select)(struct query_builder_table* table);
	struct query_builder* (*insert)(struct query_builder_table* table);
	struct query_builder* (*update)(struct query_builder_table* table);
	struct query_builder* (*delete)(struct query_builder_table* table);
	char* (*compile)(struct query_builder* query);
	/*@}*/
};

struct query_builder* query_builder_select(struct query_builder_table* table);
#define Select query_builder_select

struct query_builder* query_builder_update(struct query_builder_table* table);
#define Update query_builder_update

struct query_builder* query_builder_delete(struct query_builder_table* table);
#define Delete query_builder_delete

/**
 * Transform the query structure in SQL statement
 * \return SQL statement, NULL if error
 * */
char* query_builder_compile(struct query_builder* query);

#define INIT_QUERY \
	{ \
		.query_builder_type = query_builder_type_not_selected, \
		.compile = &query_builder, \
		.select = &query_builder_select, \
		.update = &query_builder_update, \
		.delete = &query_builder_delete, \
		.insert = &query_builder_insert, \
	}

struct query_builder* query_builder(struct query_builder_table* table);
#define Query query_builder

#endif
