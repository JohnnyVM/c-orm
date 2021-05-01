#if !defined(QUERY_BUILDER_QUERY_C_H)
#define QUERY_BUILDER_QUERY_C_H

#include "query_builder_common_c.h"

/**
 * A enum with the posible type of querys
 */
enum query_type {
	query_type_select, /**< select */
	query_type_insert, /**< insert */
	query_type_update, /**< update */
	query_type_delete, /**< delete */
};

/**
 * A structure that store the information necesary for build a query
 */
struct query {
	enum query_type type; /**< type of operation */
	char from[MAX_IDENTIFIER_NAME_LENGTH]; /**< table_name i wich the operation will happen*/
	/**
	 * List of methos from query
	 */
	/*@{*/
	struct query* (*select)(struct query*); /**< select method */
	struct query* (*insert)(); /**< insert method */
	struct query* (*update)(); /**< update method */
	struct query* (*delete)(); /**< delete method */
	/*@}*/
};

#endif
