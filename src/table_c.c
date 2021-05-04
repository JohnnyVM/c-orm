/** @file table_c.c */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "logging_c.h"
#include "query_builder_common_c.h"
#include "query_builder_error_c.h"
#include "query_builder_table_c.h"

static struct table* table_copy(struct table* orig)
{
	struct logging *log;
	if(orig == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s.", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct table* dest = log_malloc(sizeof *dest);
	if(dest == NULL) {
		return NULL;
	}

	*dest = *orig;
	// todo

	return dest;
}

/**
 * Wrapper for column function
 */
struct table_property* Column(char* name, struct column* col, ...)
{
	struct logging *log;
	if(name == NULL || col == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct table_property* dest = log_malloc(sizeof *dest);
	if(dest == NULL) {
		return NULL;
	}

	va_list args;
	va_start(args, col);
	struct column* column_dest = column(name, col, args);
	va_end(args);
	if(column_dest == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	dest->type = table_property_column;
	dest->property.column = column_dest;
	return dest;
}

struct table* Table(char* name, struct table_property* property, ...)
{
	struct logging *log;
	if(name == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s.", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct table* table = log_malloc(sizeof *table);
	if(table == NULL) {
		return NULL;
	}

	table->n_constraints = 0;
	table->n_columns = 0;

	int i = snprintf(table->name, MAX_IDENTIFIER_NAME_LENGTH, "%s", name);
	if(i < 0 || i >= MAX_IDENTIFIER_NAME_LENGTH) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->warning(log, "Column name %s truncated as %s", name, table->name);
	}

	/* Asign list of methods */
	table->copy = &table_copy;

	va_list counter, ap;
	va_start(counter, property);
	va_copy(ap, counter);
	while(property != TABLE_END) {
		// todo
		property = va_arg(counter, struct table_property*);
	}
	va_end(counter);

	return table;
}

