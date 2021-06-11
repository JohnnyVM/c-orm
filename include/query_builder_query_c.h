#if !defined(QUERY_BUILDER_QUERY_C_H)
#define QUERY_BUILDER_QUERY_C_H

#include "query_builder_common_c.h"
#include "query_builder_table_c.h"

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
	/*@{*/
	enum query_type type; /**< type of operation */
	struct query_builder_table *from; /**< principal table */
	/*@}*/
	/**
	 * List of methos from query
	 */
	/*@{*/
	struct query* (*select)(struct query*); /**< select method */
	struct query* (*insert)(struct query*); /**< insert method */
	struct query* (*update)(struct query*); /**< update method */
	struct query* (*delete)(struct query*); /**< delete method */
	void (*free)(struct query*); /**< free method */
	/*@}*/
};

/**
 * base function for sql builder
 * @param[in] table table to be referenced
 * @return a struct for build sql querys
 */
struct query* Query(struct query_builder_table* table);

#endif
