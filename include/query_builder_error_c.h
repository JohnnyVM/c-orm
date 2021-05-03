#if !defined(QUERY_BUILDER_ERROR_C_H)
#define QUERY_BUILDER_ERROR_C_H

enum query_builder_error_list {
	INVALID_COLUMN_DEFINITION = -1,
	IDENTIFIER_TRUNCATED = -2,
};

struct query_builder_error {
	const int code;
	const char* const code_name;
	const char* const code_string;
};

const char* query_builder_strerror(int errcode);
void* _log_malloc(size_t length, const char*, unsigned, const char*);
#define log_malloc(X) _log_malloc(X, __FILE__, __LINE__, __func__)

#endif

