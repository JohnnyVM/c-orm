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
		log->error(log, "%s", strerror(EINVAL));
		return;
	}

	if(orig->n_constraints > 0 ) { free(orig->constraints); }

	switch(orig->type) {
		case query_builder_VARCHAR:
		case query_builder_INTEGER:
			column_free_tailed(orig);
		break;
	}
}

static struct column* column_copy_tailed(struct column* orig)
{
	struct column* dest = log_malloc(sizeof *orig + (size_t)orig->octet_length);
	if(dest == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
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
		log->error(log, "%s", strerror(EINVAL));
		errno = EINVAL;
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
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	if(orig->n_constraints > 0) {
		enum constraint_type (**constraint_list)(void) = \
				log_malloc(orig->n_constraints * sizeof *constraint_list);
		if(orig->n_constraints > 0 && constraint_list == NULL) {
			log = get_logger(QUERY_BUILDER_LOGGER_NAME);
			log->error(log, "%s", query_builder_strerror(errno));
			return NULL;
		}
		memcpy(constraint_list, orig->constraints, orig->n_constraints * sizeof *constraint_list);
		dest->constraints = constraint_list;
	}

	return dest;
}

/**
 * This function should not be called directly, but this is C, private methods doesnt exists
 */
struct column* query_builder_column(char* name, struct column* column, unsigned n_args, va_list args)
{
	int check = snprintf(column->name, MAX_IDENTIFIER_NAME_LENGTH, "%s", name);
	if(check < 0 || check >= MAX_IDENTIFIER_NAME_LENGTH) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		column_free(column);
		log->error(log, "Invalid column name %s. %s",
				name, query_builder_strerror(IDENTIFIER_TRUNCATED));
		errno = IDENTIFIER_TRUNCATED;
		return NULL;
	}

	column->constraints = log_malloc(n_args * sizeof *column->constraints);
	if(column->constraints == NULL && n_args > 0) {
		column->free(column);
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(ENOMEM));
		return NULL;
	}

	/* list of default values */
	column->indicator = 0;
	column->nullable = 1;

	/* list of properties */
	// Really the properties as NOT NULL, PRIMARY KEY, etc
	// are table constraints are added here and will be transfered to table
	va_list ap;
	enum constraint_type (*ptr_func)(void);
	column->n_constraints = 0;
	va_copy(ap, args);
	for(column->n_constraints = 0; column->n_constraints < n_args; ++column->n_constraints) {
		ptr_func = va_arg(ap, enum constraint_type (*)(void));
		column->constraints[column->n_constraints] = ptr_func;
	}
	va_end(ap);

	return column;
}

struct column* VARCHAR(unsigned length)
{
	if(length == 0) { length = 1; }
	struct column* column = log_malloc(sizeof *column + length);
	if(column == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	column->type = query_builder_VARCHAR;
	column->octet_length = (int)length;
	column->data = (void*)((uintptr_t)column + sizeof *column);

	/* list of methods */
	column->copy = &column_copy;
	column->free = &column_free;

	return column;
}

struct column* INTEGER(void)
{
	struct column* column = log_malloc(sizeof *column + sizeof(intmax_t));
	if(column == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	column->type = query_builder_INTEGER;
	column->octet_length = sizeof(intmax_t);
	column->data = (void*)((uintptr_t)column + sizeof *column);

	/* list of methods */
	column->copy = &column_copy;
	column->free = &column_free;

	return column;
}
