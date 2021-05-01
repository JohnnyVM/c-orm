#if !defined(QUERY_BUILDER_SELECT_C_H)
#define QUERY_BUILDER_SELECT_C_H

#include "query_builder_query_c.h"
#include "query_builder_table_c.h"

struct query* Select_table(struct table* table);
struct query* Select_query(struct query* query);
#endif
