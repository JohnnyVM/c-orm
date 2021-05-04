#include <string.h>
#include <stdint.h>

#include "CppUTest/TestHarness_c.h"

#include "query_builder_error_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"
#include "query_builder_table_c.h"

TEST_GROUP_C_SETUP(table)
{
}

TEST_GROUP_C_TEARDOWN(table)
{
}

TEST_C(table, create_clean)
{
	struct table* table = \
	Table("create_clean",
		Column("column1", INTEGER(), primary_key, COLUMN_END),
		Column("column2", VARCHAR(1), COLUMN_END),
		TABLE_END
	);
	table->free(table);
}
