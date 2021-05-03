#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_common_c.h"
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
		free(constraint->column);
	}
	free(constraint);
}
