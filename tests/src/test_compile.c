#include <string.h>

#include "CppUTest/TestHarness_c.h"

#include "query_builder_error_c.h"
#include "query_builder_constraint_c.h"
#include "query_builder_table_c.h"
#include "query_builder_c.h"

TEST_GROUP_C_SETUP(compile)
{
}

TEST_GROUP_C_TEARDOWN(compile)
{
}

TEST_C(compile, select)
{
	struct query_builder_table* table = Table("table",
				Column("column", INTEGER(), primary_key),
				Column("column", VARCHAR(125)),
				Column("column", VARCHAR(125))
			);

	struct query_builder* query = Query(NULL);
	struct query_builder* select = Select(table);

	char* query_string = query->compile(select);

	CHECK_EQUAL_C_STRING_TEXT(
			query_string,
			"SELECT table.column,table.column,table.column FROM table",
			query_string);

	free(query_string);
	query->free(query);
	select->free(select);
	table->free(table);
}
