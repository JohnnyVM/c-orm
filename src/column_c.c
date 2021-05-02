/** @file column_c.c */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "logging_c.h"
#include "query_builder_common_c.h"
#include "query_builder_column_c.h"
#include "query_builder_table_c.h"

static void column_free_tailed(struct column* orig)
{
	free(orig);
}

static void column_free(struct column* orig)
{
	if(orig == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
	}

	switch(orig->type) {
		case query_builder_VARCHAR:
		case query_builder_INTEGER:
			return column_free_tailed(orig);
		break;
	}
}

static struct column* column_copy_tailed(struct column* orig)
{
	struct column* dest = malloc(sizeof *orig + (size_t)orig->octet_length);
	if(dest == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(ENOMEM));
		errno = ENOMEM;
		return NULL;
	}

	memcpy(dest, orig, sizeof *orig + (size_t)orig->octet_length);
	if(orig->octet_length) {
		dest->data = (void*)((uintptr_t)dest + sizeof *dest);
	}

	return dest;
}

static struct column* column_copy(struct column* orig)
{
	if(orig == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	switch(orig->type) {
		case query_builder_VARCHAR:
		case query_builder_INTEGER:
			return column_copy_tailed(orig);
			break;
	}

	return NULL;
}

struct table_property* Column(char* name, struct column* column)
{
	struct logging *log;
	if(name == NULL || column == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(EINVAL));
		errno = EINVAL;
		return NULL;
	}

	struct table_property* property = malloc(sizeof *property);
	if(property == NULL) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(ENOMEM));
		errno = ENOMEM;
		return NULL;
	}

	int i = snprintf(column->name, MAX_IDENTIFIER_NAME_LENGTH, "%s", name);
	if(i < 0 || i >= MAX_IDENTIFIER_NAME_LENGTH) {
		log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->warning(log, "Column name %s truncated as %s", name, column->name);
	}

	/* list of methods */
	column->copy = &column_copy;
	column->free = &column_free;

	/* list of dependent valiables */
	column->indicator = 0;
	column->nullable = 0;

	property->type = table_property_column;
	property->property.column = column;
	return property;
}

struct column* VARCHAR(unsigned length)
{
	if(length == 0) { length = 1; }
	struct column* column = malloc(sizeof *column + length);
	if(column == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(ENOMEM));
		errno = ENOMEM;
		return NULL;
	}

	column->type = query_builder_VARCHAR;
	column->octet_length = (int)length;
	column->data = (void*)((uintptr_t)column + sizeof *column);

	return column;
}

struct column* INTEGER(void)
{
	struct column* column = malloc(sizeof *column + sizeof(intmax_t));
	if(column == NULL) {
		struct logging *log = get_logger(QUERY_BUILDER_LOGGER_NAME);
		log->error(log, "%s: %s", __func__, strerror(ENOMEM));
		errno = ENOMEM;
		return NULL;
	}

	column->type = query_builder_INTEGER;
	column->octet_length = sizeof(intmax_t);
	column->data = (void*)((uintptr_t)column + sizeof *column);

	return column;
}
