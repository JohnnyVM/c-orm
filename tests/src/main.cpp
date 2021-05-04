#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

/** For each C test group */
TEST_GROUP_C_WRAPPER(column)
{
    TEST_GROUP_C_SETUP_WRAPPER(column); /** optional */
    TEST_GROUP_C_TEARDOWN_WRAPPER(column); /** optional */
};
TEST_C_WRAPPER(column, create_varchar);
TEST_C_WRAPPER(column, create_integer);

//TEST_GROUP_C_WRAPPER(table)
//{
//    TEST_GROUP_C_SETUP_WRAPPER(table); /** optional */
//    TEST_GROUP_C_TEARDOWN_WRAPPER(table); /** optional */
//};
//TEST_C_WRAPPER(table, table_clean);

TEST_GROUP_C_WRAPPER(common)
{
    TEST_GROUP_C_SETUP_WRAPPER(common); /** optional */
    TEST_GROUP_C_TEARDOWN_WRAPPER(common); /** optional */
};
TEST_C_WRAPPER(common, va_list_count);

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
