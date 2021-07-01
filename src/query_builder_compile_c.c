#include <assert.h>
#include <string.h>
#include <errno.h>
#include <alloca.h>
#include <stdio.h>

#include "logging_c.h"
#include "query_builder_error_c.h"
#include "query_builder_c.h"

#if defined(__clang__)
#if __has_warning("-Wformat-nonliteral")
#  pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
#endif

#if defined(__GNUC__) && (__GNUC__ >= 7)
#  pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif


static size_t chunk_size = 1024;

static int get_columns(struct query_builder_table* table, char** column_string, int *len) {

	if(!table || !column_string || !len) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return EINVAL;
	}

	*column_string = NULL;
	*len = 0;

	size_t buffer_len = chunk_size;
	int output_len;
	char* buffer = log_malloc(chunk_size), *tmp_ptr;
	if(!buffer) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return ENOMEM;
	}

	// lets assume this not fail
	output_len = snprintf(buffer, buffer_len, "%s.%s", table->name, table->columns[0]->name);
	if(output_len < 0 || output_len > (long int)buffer_len) {
		free(buffer);
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return ENOMEM;
	}

	tmp_ptr = buffer;
	for (unsigned i = 1; i < table->n_columns; i++) {
		// get the expected size of the output
		output_len = snprintf(NULL, 0, "%s,%s.%s", buffer, table->name, table->columns[i]->name);
		if(output_len > (long int)buffer_len) {
			buffer_len += chunk_size;
			tmp_ptr = log_realloc(buffer, buffer_len);
			if(!tmp_ptr) {
				free(buffer);
				struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
				log->error(log, "%s", strerror(ENOMEM));
				return ENOMEM;
			}
			buffer = tmp_ptr;
			i--;
			continue;
		}

		tmp_ptr = alloca(buffer_len);
		output_len = snprintf(tmp_ptr, buffer_len, "%s,%s.%s", buffer, table->name, table->columns[i]->name);
		if(output_len < 0 || output_len > (long int)buffer_len) {
			free(buffer);
			struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
			log->error(log, "%s", strerror(ENOMEM));
			return ENOMEM;
		}
		strncpy(buffer, tmp_ptr, buffer_len);
	}

	*column_string = buffer;
	*len = output_len;

	return 0;
}

static char* query_builder_compile_select(struct query_builder* query) {
	int output_len, columns_len;
	char *query_string, *query_columns;
	if(!query || !query->table || query->type != query_builder_type_select) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	int err;
	if( (err = get_columns(query->table, &query_columns, &columns_len)) ) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(err));
		return NULL;
	}

	const char* format = "SELECT %.*s FROM %s";

	output_len = snprintf(NULL, 0, format,
			columns_len,
			query_columns,
			query->table->name);
	output_len++;

	query_string = log_malloc((size_t)output_len);
	if(!query_string) {
		free(query_columns);
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}

	output_len = snprintf(query_string, (size_t)output_len, format,
			columns_len,
			query_columns,
			query->table->name);

	free(query_columns);

	return query_string;
}

static char* query_builder_compile_update(struct query_builder* query) {
	int output_len;
	char *query_string;
	if(!query || !query->table || query->type != query_builder_type_update) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	const char* format = "UPDATE %s SET";

	output_len = snprintf(NULL, 0, format,
			query->table->name);
	query_string = log_malloc((size_t)output_len+1);
	if(!query_string) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}
	output_len = snprintf(query_string, (size_t)output_len, format,
			query->table->name);

	return query_string;
}

static char* query_builder_compile_delete(struct query_builder* query) {
	int output_len;
	char* query_string;
	if(!query || !query->table || query->type != query_builder_type_delete) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	const char* format = "DELETE FROM %s";

	output_len = snprintf(NULL, 0, format,
			query->table->name);
	query_string = log_malloc((size_t)output_len+1);
	if(!query_string) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}
	output_len = snprintf(query_string, (size_t)output_len, format,
			query->table->name);

	return query_string;
}

static char* query_builder_compile_insert(struct query_builder* query) {
	int output_len;
	char* query_string;
	if(!query || !query->table || query->type != query_builder_type_delete) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	const char* format = "INSERT INTO %s VALUES";

	output_len = snprintf(NULL, 0, format,
			query->table->name);
	query_string = log_malloc((size_t)output_len+1);
	if(!query_string) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(ENOMEM));
		return NULL;
	}
	output_len = snprintf(query_string, (size_t)output_len, format,
			query->table->name);

	return query_string;
}

char* query_builder_compile(struct query_builder* query)
{
	struct logging *log;
	if(!query || !query->table || query->type == query_builder_type_not_selected) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	char* query_string = NULL;
	switch(query->type){
		case query_builder_type_not_selected:
			assert(0); // This case cannot happen
			log = get_logger(QUERY_BUILDER_LOGGER_NAME);
			log->error(log, "%s", strerror(EINVAL));
			return NULL;

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
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
	}

	return query_string;
}
