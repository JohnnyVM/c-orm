#include <stdarg.h>
#include <stddef.h>

#include "query_builder_common_c.h"
#include "query_builder_table_c.h"
#include "query_builder_constraint_c.h"

unsigned va_list_void(void *init, ...)
{
	unsigned i = 0;
	va_list counter_list;

	va_start(counter_list, init);
	while(init != NULL) {
		++i;
		init = va_arg(counter_list, void*);
	}
	va_end(counter_list);

	return i;
}

