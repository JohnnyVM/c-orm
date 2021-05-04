#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_constraint_c.h"

[[maybe_unused]]static struct constraint* constraint_copy(struct constraint* source)
{
	if(source == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct constraint* dest = log_malloc(sizeof *source);
	if(dest == NULL) {
		return NULL;
	}

	*dest = *source;

	if(source->type == constraint_default) {
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

void constraint_free(struct constraint* constraint)
{
	if(constraint == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->warning(log, "%s: %s. Probably this shouldn't happen", __func__, strerror(EINVAL));
		return;
	}

	if(constraint->type == constraint_default) {
		free(constraint->string_literal);
	}

	if(constraint->n_columns) {
		free(constraint->columns);
	}

	free(constraint);
}

[[maybe_unused]]static struct constraint* constraint_add_column(struct constraint* dest, struct column* column)
{
	if(column == NULL || dest == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s. Probably this shouldn't happen", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	++dest->n_columns;
	void* tmp = log_realloc(dest->columns, dest->n_columns * sizeof(struct column*));
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
enum constraint_type primary_key(void)
{
	return constraint_primary_key;
}

/**
 * Return contraint not null, syntax sugar
 */
enum constraint_type not_null(void)
{
	return constraint_not_null;
}

/**
 * index is already taken by string
 */
enum constraint_type is_index(void)
{
	return constraint_index;
}
