/** @file select.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "logging_c.h"
#include "query_builder_select_c.h"

/**
 * Method for build select sql from query
 * @param[in] table the table in wich you do the select
 * @return Return a struct query prepared for select from the object table
 * output must be freed with Query_free
 */
struct query* Select_query(struct query* query)
{
	struct logging *log;
	if(query == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s.", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	query->type = query_type_select;

	return query;
}

/**
 * Method for build select sql from table
 * @param[in] table the table in wich you do the select
 * @return Return a struct query prepared for select from the object table
 * output must be freed with Query_free
 */
struct query* Select_table(struct table* table)
{
	struct logging *log;
	if(table == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s.", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	return Select_query(Query(table));
}

