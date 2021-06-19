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
	struct query_builder_table_property* property = Column("column", VARCHAR(1), primary_key);
	struct query_builder_table* table = Table("table", property);
	table->free(table);
	table = Table("table", Column("column", INTEGER(), primary_key));
	table->free(table);
	table = Table("table",
				Column("column", INTEGER(), primary_key),
				Column("column", VARCHAR(125)),
				Column("column", VARCHAR(125))
			);
	table->free(table);
}
