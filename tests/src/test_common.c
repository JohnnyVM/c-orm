#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "CppUTest/TestHarness_c.h"

#include "query_builder_common_c.h"
#include "query_builder_constraint_c.h"

TEST_GROUP_C_SETUP(common)
{
}

TEST_GROUP_C_TEARDOWN(common)
{
}

TEST_C(common, va_list_count)
{
	assert(va_list_count() == 0);
	assert(va_list_count(NULL) == 0);
	assert(va_list_count(primary_key) == 1);
	assert(va_list_count(primary_key, NULL) == 1);
	assert(va_list_count(primary_key, not_null) == 2);
	assert(va_list_count(primary_key, not_null, NULL) == 2);
	CHECK_C(1);
}

