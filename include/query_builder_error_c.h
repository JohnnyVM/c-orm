#if !defined(QUERY_BUILDER_ERROR_C_H)
#define QUERY_BUILDER_ERROR_C_H

#include <stdlib.h>

enum query_builder_error_list {
	IDENTIFIER_TRUNCATED = -1,
	INVALID_COLUMN_DEFINITION = -2,
	INVALID_TABLE_DEFINITION = -3,
	INVALID_CONSTRAINT_DEFINITION = -4,
};

struct query_builder_error {
	const int code;
	const char* const code_name;
	const char* const code_string;
};

const char* query_builder_strerror(int errcode);
void* _log_malloc(size_t length, const char*, unsigned, const char*);
#define log_malloc(X) _log_malloc(X, __FILE__, __LINE__, __func__)
void* _log_realloc(void* el, size_t length, const char*, unsigned, const char*);
#define log_realloc(X, L) _log_realloc(X, L, __FILE__, __LINE__, __func__)

#endif

