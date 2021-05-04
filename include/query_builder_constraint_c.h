#if !defined(QUERY_BUILDER_CONSTRAINT_C_H)
#define QUERY_BUILDER_CONSTRAINT_C_H

#include "query_builder_common_c.h"

struct constraint {
	/*@{*/
	enum constraint_type type;
	char name[MAX_IDENTIFIER_NAME_LENGTH];
	struct column** columns; /**< list of pointers to affected columns, readonly */
	unsigned n_columns; /**< number of columns affected, readonly */
	char *string_literal; /**< this value will be added to DEFAULT clause */
	/*@}*/
	/**
	 * List of methos for contraints
	 */
	/*@{*/
	struct constraint* (*copy)(struct constraint*);
	void (*free)(struct constraint*);
	/*@}*/
};

/**
 * List of functions
 */
enum constraint_type primary_key(void);
enum constraint_type not_null(void);
enum constraint_type is_index(void);
#endif
