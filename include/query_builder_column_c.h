#if !defined(QUERY_BUILDER_TABLE_C_H)
#define QUERY_BUILDER_TABLE_C_H

#include "query_builder_common_c.h"

/**
 * A structure that store the column information
 */
struct column {
	int type; /**< numeric code of the data type of the column */
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< name of the column */
	int octet_length; /**< length of the character representation of the datum in bytes */
	int indicator; /**< the indicator (indicating a null value or a value truncation) */
	void* data; /**< actual data item (therefore, the data type of this field depends on the query) */
};

#endif
