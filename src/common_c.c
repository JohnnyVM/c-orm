#include <stdarg.h>

#include "query_builder_common_c.h"
#include "query_builder_constraint_c.h"

unsigned va_list_void(void *init, ...)
{
	unsigned i = 0;
	va_list counter_list;

	va_start(counter_list, init);
	while(va_arg(counter_list, void*)) { ++i; }
	va_end(counter_list);

	return i;
}

unsigned va_list_constraint_type(enum constraint_type (*init)(void), ...)
{
	unsigned i = 0;
	va_list counter_list;

	va_start(counter_list, init);
	while(va_arg(counter_list, enum constraint_type (*)(void))) { ++i; }
	va_end(counter_list);

	return i;
}

