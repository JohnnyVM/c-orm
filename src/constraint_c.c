/** \file query_builder_constraint_c.c */
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"

unsigned va_list_query_builder_constraint_type(enum query_builder_constraint_type (*init)(void), ...)
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

[[maybe_unused]]static struct query_builder_constraint* query_builder_constraint_copy(struct query_builder_constraint* source)
{
	if(source == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct query_builder_constraint* dest = log_malloc(sizeof *source);
	if(dest == NULL) {
		return NULL;
	}

	*dest = *source;

	if(source->type == query_builder_constraint_default_value) {
		dest->string_literal = log_malloc(strlen(source->string_literal) + 1);
		if(dest->string_literal == NULL) {
			free(dest);
			return NULL;
		}
		snprintf(dest->string_literal,
				strlen(source->string_literal) + 1, "%s", source->string_literal);
	}

	return dest;
}

/**
 * static function
 */
[[maybe_unused]] static void query_builder_constraint_free(struct query_builder_constraint* query_builder_constraint)
{
	if(query_builder_constraint == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->warning(log, "%s: %s. Probably this shouldn't happen", __func__, strerror(EINVAL));
		return;
	}

	if(query_builder_constraint->type == query_builder_constraint_default_value) {
		free(query_builder_constraint->string_literal);
	}

	if(query_builder_constraint->n_columns) {
		free(query_builder_constraint->columns);
	}

	free(query_builder_constraint);
}

[[maybe_unused]] static struct query_builder_constraint* query_builder_constraint_add_column(struct query_builder_constraint* dest, struct query_builder_column* column)
{
	if(column == NULL || dest == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s. Probably this shouldn't happen", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	++dest->n_columns;
	void* tmp = log_realloc(dest->columns, dest->n_columns * sizeof(struct query_builder_column*));
	if(dest->columns == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		--dest->n_columns;
		free(dest);
		return NULL;
	}
	dest->columns = tmp;
	dest->columns[dest->n_columns - 1] = column;

	return dest;
}

/**
 * Return contraint type primary key, syntax sugar
 */
enum query_builder_constraint_type primary_key(void)
{
	return query_builder_constraint_primary_key;
}

/**
 * Return contraint not null, syntax sugar
 */
enum query_builder_constraint_type not_null(void)
{
	return query_builder_constraint_not_null;
}

/**
 * index is already taken by string
 */
enum query_builder_constraint_type is_index(void)
{
	return query_builder_constraint_index;
}
