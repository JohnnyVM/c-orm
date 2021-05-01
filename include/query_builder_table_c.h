#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"

/**
 * A structure that store the information of a table of the database
 */
struct table {
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< Name of the table represented */
};

#endif
