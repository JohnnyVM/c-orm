#if !defined(QUERY_BUILDER_CONSTRAINT_C_H)
#define QUERY_BUILDER_CONSTRAINT_C_H

#include "query_builder_common_c.h"

enum constraint_type {
	constraint_index,
	constraint_not_null,
	constraint_primary_key,
	constraint_default,
};

struct constraint {
	/*@{*/
	enum constraint_type type;
	char name[MAX_IDENTIFIER_NAME_LENGTH];
	struct column** column; /**< list of pointers to affected columns, readonly */
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

#endif