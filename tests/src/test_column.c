#include <string.h>
#include <stdint.h>

#include "CppUTest/TestHarness_c.h"

#include "query_builder_error_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"
#include "query_builder_table_c.h"

TEST_GROUP_C_SETUP(column)
{
}

TEST_GROUP_C_TEARDOWN(column)
{
}

TEST_C(column, create_varchar)
{
	struct query_builder_table_property* property = Column("column", VARCHAR(1), primary_key);
	struct query_builder_column *col = property->value;
	strncpy(col->data, "a", (size_t)col->octet_length);
	struct query_builder_column *col2 = col->copy(col);
	CHECK_C(*(char*)col2->data == 'a');
	CHECK_C(col2->n_constraints == 1);
	property->free(property);
	col2->free(col2);
}

TEST_C(column, create_integer)
{
	struct query_builder_table_property *property = Column("column", INTEGER(), primary_key, not_null);
	struct query_builder_column* col = property->value;
	*(intmax_t*)col->data = INTMAX_MAX;
	struct query_builder_column *col2 = col->copy(col);
	CHECK_C(*(intmax_t*)col2->data == INTMAX_MAX);
	CHECK_C(col2->n_constraints == 2);
	property->free(property);
	col2->free(col2);
}
