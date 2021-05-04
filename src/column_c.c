/** @file column_c.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_table_c.h"
#include "query_builder_column_c.h"

static void column_free_tailed(struct column* orig)
{
	free(orig);
}

static void column_free(struct column* orig)
{
	if(orig == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
	}

	if(orig->n_constraints > 0 ) { free(orig->constraints); }

	switch(orig->type) {
		case query_builder_VARCHAR:
		case query_builder_INTEGER:
			return column_free_tailed(orig);
		break;
	}
}

static struct column* column_copy_tailed(struct column* orig)
{
	struct column* dest = log_malloc(sizeof *orig + (size_t)orig->octet_length);
	if(dest == NULL) {
		return NULL;
	}

	memcpy(dest, orig, sizeof *orig + (size_t)orig->octet_length);
	if(orig->octet_length) {
		dest->data = (void*)((uintptr_t)dest + sizeof *dest);
	}

	return dest;
}

static struct column* column_copy(struct column* orig)
{
	struct logging *log;
	if(orig == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	enum constraint_type (**constraint_list)(void) = \
			log_malloc(orig->n_constraints * sizeof(enum constraint_type (*)(void)));
	if(orig->n_constraints != 0 && constraint_list == NULL) {
		return NULL;
	}

	struct column* dest = NULL;
	switch(orig->type) {
		case query_builder_VARCHAR:
		case query_builder_INTEGER:
			dest = column_copy_tailed(orig);
			break;
	}
	if(dest == NULL) {
		free(constraint_list);
		return NULL;
	}

	dest->constraints = constraint_list;
	dest->n_constraints = orig->n_constraints;
	memcpy(dest->constraints, orig->constraints, orig->n_constraints * sizeof(enum constraint_type (*)(void)));

	return dest;
}

/**
 * This function should not be called directly, but this is C, private methods doesnt exists
 */
struct column* column(char* name, struct column* column, va_list args)
{
	struct logging *log;
	int check = snprintf(column->name, MAX_IDENTIFIER_NAME_LENGTH, "%s", name);
	if(check < 0 || check >= MAX_IDENTIFIER_NAME_LENGTH) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		column_free(column);
		log->error(log, "Invalid column name %s. %s",
				name, query_builder_strerror(IDENTIFIER_TRUNCATED));
		errno = IDENTIFIER_TRUNCATED;
		return NULL;
	}

	/* list of methods */
	column->copy = &column_copy;
	column->free = &column_free;

	/* list of default values */
	column->indicator = 0;
	column->nullable = 1;

	/* list of properties */
	// Really the properties as NOT NULL, PRIMARY KEY, etc
	// are table constraints are added here and will be transfered to table
	{
		unsigned i = 0;
		va_list counter_list;
		va_copy(counter_list, args);
		while(va_arg(counter_list, struct constraint*)) {
			i++;
		}
		va_end(counter_list);
		column->constraints = log_malloc(i * sizeof column->constraints);
		if(i > 0 && column->constraints == NULL) {
			log = get_logger(QUERY_BUILDER_LOGGER_NAME);
			column_free(column);
			return NULL;
		}

		va_list ap;
		enum constraint_type (*ptr_func)(void);
		column->n_constraints = 0;
		va_copy(ap, args);
		while((ptr_func = va_arg(ap, enum constraint_type (*)(void))) != NULL) {
			column->constraints[column->n_constraints] = ptr_func;
			++column->n_constraints;
		}
		va_end(ap);

		// todo list of pointer from contraint to column
	}

	return column;
}

struct column* VARCHAR(unsigned length)
{
	if(length == 0) { length = 1; }
	struct column* column = log_malloc(sizeof *column + length);
	if(column == NULL) {
		return NULL;
	}

	column->type = query_builder_VARCHAR;
	column->octet_length = (int)length;
	column->data = (void*)((uintptr_t)column + sizeof *column);

	return column;
}

struct column* INTEGER(void)
{
	struct column* column = log_malloc(sizeof *column + sizeof(intmax_t));
	if(column == NULL) {
		return NULL;
	}

	column->type = query_builder_INTEGER;
	column->octet_length = sizeof(intmax_t);
	column->data = (void*)((uintptr_t)column + sizeof *column);

	return column;
}
