#include <assert.h>
#include <string.h>
#include <errno.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_c.h"

static unsigned chunk_size;

char* query_builder_compile_select(struct query_builder* query) {
	char* query_string;
	if(!query || !query->table || query->type != query_builder_type_select) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	return query_string;
}

char* query_builder_compile_update(struct query_builder* query) {
	char* query_string;
	if(!query || !query->table || query->type != query_builder_type_update) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	return query_string;
}

char* query_builder_compile_delete(struct query_builder* query) {
	char* query_string;
	if(!query || !query->table || query->type != query_builder_type_delete) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	return query_string;
}

char* query_builder_compile_insert(struct query_builder* query) {
	char* query_string;
	if(!query || !query->table || query->type != query_builder_type_insert) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	return query_string;
}

char* query_builder_compile(struct query_builder* query)
{
	if(!query || !query->table || query->type == query_builder_type_not_selected) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	char* query_string;
	switch(query->type){
		case query_builder_type_not_selected:
			assert(0); // This case cannot happen
			break;
		case query_builder_type_select:
			query_string = query_builder_compile_select(query);
			break;
		case query_builder_type_update:
			query_string = query_builder_compile_update(query);
			break;
		case query_builder_type_delete:
			query_string = query_builder_compile_delete(query);
			break;
		case query_builder_type_insert:
			query_string = query_builder_compile_insert(query);
			break;
	}

	if(!query_string) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
	}
	return query_string;
}
