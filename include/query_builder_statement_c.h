#if !defined(QUERY_BUILDER_STATEMENT_C_H)
#define QUERY_BUILDER_TABLE_STATEMENT_H

#include "query_builder_common_c.h"

/**
 * A structure that store the statement information
 */
struct statement {
	char* sql;
	char* sql_len;
};

#endif

