#include <string.h>
#include <errno.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_c.h"

struct query_builder* query_builder_select(struct query_builder_table* table)
{
	if(table == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	struct query_builder* query = log_malloc(sizeof *query);
	if(query == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}

	query->type = query_builder_type_select;

	return query;
}

struct query_builder* query_builder_update(struct query_builder_table* table)
{
	if(table == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	struct query_builder* query = log_malloc(sizeof *query);
	if(query == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}

	query->type = query_builder_type_update;

	return query;
}

struct query_builder* query_builder_delete(struct query_builder_table* table)
{
	if(table == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	struct query_builder* query = log_malloc(sizeof *query);
	if(query == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}

	query->type = query_builder_type_delete;

	return query;
}

struct query_builder* query_builder_insert(struct query_builder_table* table)
{
	if(table == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	struct query_builder* query = log_malloc(sizeof *query);
	if(query == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}

	query->type = query_builder_type_insert;

	return query;
}
