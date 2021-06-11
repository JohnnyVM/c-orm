#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "logging_c.h"
#include "query_builder_common_c.h"
#include "query_builder_error_c.h"

const struct query_builder_error query_builder_error_list[] = {
	(struct query_builder_error){
		.code = INVALID_COLUMN_DEFINITION,
		.code_name = "Invalid column definition",
		.code_string = "Invalid column definition",
	},
	(struct query_builder_error){
		.code = IDENTIFIER_TRUNCATED,
		.code_name = "Identifier truncated",
		.code_string = "The identifier its bigger than " STRINGNIFY(MAX_IDENTIFIER_NAME_LENGTH) " characters",
	},
	(struct query_builder_error){
		.code = INVALID_TABLE_DEFINITION,
		.code_name = "Invalid table definition",
		.code_string = "Invalid table definition",
	},
	(struct query_builder_error){
		.code = INVALID_CONSTRAINT_DEFINITION,
		.code_name = "Invalid constraint definition",
		.code_string = "Invalid constraint definition",
	},
};

static bool query_builder_error_code(enum query_builder_error_list errcode){
	bool is_query_builder_error_code = false;
	switch(errcode) {
		case IDENTIFIER_TRUNCATED:
		case INVALID_COLUMN_DEFINITION:
		case INVALID_TABLE_DEFINITION:
		case INVALID_CONSTRAINT_DEFINITION:
			is_query_builder_error_code = true;
			break;
	}
	return is_query_builder_error_code;
}

const char* query_builder_strerror(int errcode)
{
	if(query_builder_error_code(errcode)) {
		return query_builder_error_list[-errcode - 1].code_string;
	}
	return strerror(errcode);
}

void* _log_malloc(size_t length, const char* file, unsigned line, const char* func)
{
	if(length == 0) { return NULL; }
	void* dest = malloc(length);
	if(dest == NULL && length > 0) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s:%i:%s: %s", file, line, func, strerror(ENOMEM));
		return NULL;
	}
	return dest;
}

void* _log_realloc(void* el, size_t length, const char* file, unsigned line, const char* func)
{
	void* dest = realloc(el, length);
	if(dest == NULL && length > 0) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s:%i:%s: %s", file, line, func, strerror(ENOMEM));
		return NULL;
	}
	return dest;
}
