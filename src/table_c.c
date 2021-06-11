/** @file table_c.c */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "logging_c.h"
#include "query_builder_common_c.h"
#include "query_builder_error_c.h"
#include "query_builder_table_c.h"
#include "query_builder_constraint_c.h"

unsigned va_list_query_builder_table_property(struct query_builder_table_property* init, ...)
{
	unsigned i = 0;
	va_list counter_list;

	va_start(counter_list, init);
	while(init != NULL) {
		++i;
		init = va_arg(counter_list, void*);
	}
	va_end(counter_list);

	return i;
}

unsigned va_list_constraint_type(enum constraint_type (*init)(void), ...)
{
	unsigned i = 0;
	va_list counter_list;

	va_start(counter_list, init);
	while(init != NULL) {
		++i;
		init = va_arg(counter_list, void*);
	}
	va_end(counter_list);

	return i;
}

/**
 * Clean the table property that arrive from query_builder_table declaration
 */
void query_builder_table_property_free(struct query_builder_table_property* property)
{
	struct logging *log;
	if(property == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return;
	}

	struct query_builder_constraint* constraint;
	struct query_builder_column* col;
	switch(property->type) {
		case table_property_column:
			col = property->value;
			col->free(col);
			break;

		case table_property_constraint:
			constraint = property->value;
			constraint->free(constraint);
			break;
	}

	free(property);
}

static struct query_builder_table* table_copy(struct query_builder_table* orig)
{
	struct logging *log;
	if(orig == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s.", __func__, strerror(EINVAL));
		return NULL;
	}

	struct query_builder_table* dest = log_malloc(sizeof *dest);
	if(dest == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s.", __func__, strerror(EINVAL));
		return NULL;
	}

	*dest = *orig;

	return dest;
}

/**
 * Wrapper for column function
 * Must be called as Column
 * This function its a wrapper for add idiomatic form to declare Columns and constraints
 * \todo really i dont like this solution, but i dont have nothing better
 * Table('table_name', Column(...))
 */
struct query_builder_table_property* query_builder_table_column(char* name,
																struct query_builder_column* col,
																unsigned n_args,
																...)
{
	struct logging *log;
	if(name == NULL || col == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct query_builder_table_property* dest = log_malloc(sizeof *dest);
	if(dest == NULL) {
		col->free(col);
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	va_list args;
	va_start(args, n_args);
	struct query_builder_column* column_dest = query_builder_column(name, col, n_args, args);
	va_end(args);
	if(column_dest == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	dest->type = table_property_column;
	dest->value = column_dest;
	dest->free = &query_builder_table_property_free;
	return dest;
}

static void query_builder_table_free(struct query_builder_table* table)
{
	if(table == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return;
	}

	for(unsigned i = 0; i < table->n_columns; i++) {
		table->columns[i].free(&table->columns[i]);
	}

	for(unsigned i = 0; i < table->n_constraints; i++) {
		table->constraints[i].free(&table->constraints[i]);
	}

	free(table);
}


/**
 *	Return a query_builder_table
 *	The spected usage is with the macro Table
 *	\code
 *		Table("table name", ...)
 *	\endcode
 *	\param[in] name name of the table
 *	\param[in] n_args number of arguments
 *	\param ... List of table properties columnas and constraints
 */
struct query_builder_table* query_builder_table(char* name, unsigned n_args, ...)
{
	if(name == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", strerror(EINVAL));
		return NULL;
	}

	struct query_builder_table* table = log_malloc(sizeof *table);
	if(table == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s", query_builder_strerror(errno));
		return NULL;
	}

	table->columns = NULL;
	table->n_columns = 0;
	table->constraints = NULL;
	table->n_constraints = 0;

	/** iterate over la list of constraints passed to the function */
	va_list property_list;
	struct query_builder_table_property* property;
	va_start(property_list, n_args);
	for(unsigned n_properties = 0; n_properties < n_args; n_properties++) {
		property = va_arg(property_list, struct query_builder_table_property*);
		switch(property->type) {
			case table_property_column:
				///todo table column
				break;

			case table_property_constraint:
				///todo constraint
				break;
		}
	}
	va_end(property_list);

	int len = snprintf(table->name, MAX_IDENTIFIER_NAME_LENGTH, "%s", name);
	if(len < 0 || len >= MAX_IDENTIFIER_NAME_LENGTH) {
		struct logging* log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "Table name %s truncated as %s", name, table->name);
		free(table);
		return NULL;
	}

	/* Asign list of methods */
	table->copy = &table_copy;
	table->free = &query_builder_table_free;

	return table;
}

