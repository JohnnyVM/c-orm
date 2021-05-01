/** @file query.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "logging_c.h"
#include "query_builder_query_c.h"
#include "query_builder_table_c.h"
#include "query_builder_select_c.h"

/**
 * Method for build query sql
 * @param[in] table the table in wich you do the select
 * @return Return a struct query prepared for select from the object table
 * output must be freed quth Query_free
 */
struct query* Query(struct table* table)
{
	struct logging *log;
	if(table == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct query* query = malloc(sizeof *query);
	if(query == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(ENOMEM));
		errno = ENOMEM;
		return NULL;
	}

	// from y name have same size
	snprintf(query->from, sizeof query->from, "%s", table->name);

	query->select = &Select_query;

	return query;
}

/**
 * Clean query memory allocated
 * @param[in] query Previously allocated query
 */
void Query_free(struct query* query)
{
	free(query);
}
