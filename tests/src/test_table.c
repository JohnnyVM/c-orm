#include <string.h>
#include <stdint.h>

#include "CppUTest/TestHarness_c.h"

#include "query_builder_error_c.h"
#include "query_builder_constraint_c.h"
#include "query_builder_table_c.h"

TEST_GROUP_C_SETUP(table)
{
}

TEST_GROUP_C_TEARDOWN(table)
{
}

TEST_C(table, create_free)
{
	struct query_builder_table *table= Table("table", Column("column", VARCHAR(1), primary_key));
	table->free(table);
}
