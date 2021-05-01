#include <string.h>
#include <stdint.h>

#include "CppUTest/TestHarness_c.h"

#include "query_builder_column_c.h"

TEST_GROUP_C_SETUP(column)
{
}

TEST_GROUP_C_TEARDOWN(column)
{
}

TEST_C(column, create_varchar)
{
	struct column *col = Column("column", VARCHAR(1));
	strncpy(col->data, "a", (size_t)col->octet_length);
	struct column *col2 = col->copy(col);
	CHECK_C(*(char*)col2->data == 'a');
	col->free(col);
	col2->free(col2);
}

TEST_C(column, create_integer)
{
	struct column *col = Column("column", INTEGER());
	*(intmax_t*)col->data = INTMAX_MAX;
	struct column *col2 = col->copy(col);
	CHECK_C(*(intmax_t*)col2->data == INTMAX_MAX);
	col->free(col);
	col2->free(col2);
}
