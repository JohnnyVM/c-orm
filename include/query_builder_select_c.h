#if !defined(QUERY_BUILDER_SELECT_C_H)
#define QUERY_BUILDER_SELECT_C_H

#include "query_builder_query_c.h"
#include "query_builder_table_c.h"

/**
 * Prepare a query of type select for the table \p table
 * @param[out] table origin table
 * @return query
 */
struct query* Select_table(struct query_builder_table* table);

/**
 * Prepare one query of type select
 * @param[out] query origin query
 * @return new query
 */
struct query* Select_query(struct query* query);
#define Select(X) _Generic((X), \
	struct table*: Select_table \
	struct query*: Select_query \
)(X)

#endif
