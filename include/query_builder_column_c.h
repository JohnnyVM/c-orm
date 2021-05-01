#if !defined(QUERY_BUILDER_COLUMN_C_H)
#define QUERY_BUILDER_COLUMN_C_H

#include "query_builder_common_c.h"

enum column_type  {
	query_builder_VARCHAR,
	query_builder_INTEGER,
};

/**
 * A structure that store the column information
 */
struct column {
	/*@{*/
	enum column_type type; /**< numeric code of the data type of the column */
	char name[MAX_IDENTIFIER_NAME_LENGTH]; /**< name of the column */
	int octet_length; /**< length of the character representation of the datum in bytes */
	int indicator; /**< the indicator (indicating a null value or a value truncation) */
	int nullable; /**< nullable column */
	/*@}*/
	/**
	 * List of methos from query
	 */
	/*@{*/
	struct column* (*copy)(struct column*); /** copy method */
	void (*free)(struct column*); /**< free method */
	/*@}*/
	/*@{*/
	void* data; /**< actual data item, has to be in the last position */
	/*@}*/
};


/**
 * create a column of type varchar
 * @param length varchar length
 * @return created column
 */
struct column* Column(char* name, struct column* description);

/**
 * create a column of type varchar
 * @param length varchar length
 * @return anonymous column setted as integer use
 */
struct column* VARCHAR(unsigned length);

/**
 * create a column of type integer
 * @return anonymous column setted as integer use
 */
struct column* INTEGER(void);

#endif
