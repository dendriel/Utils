/***************************************************************************************************
 * Description: handleData module - Define functions prototypes.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 27/05/2013
 **************************************************************************************************/
#ifndef HANDLEDATA_HEADER
#define HANDLEDATA_HEADER

#include <stdio.h>

/**************************************************************************************************/
/**
 *	\b Write data into a file.
 *	\p file_name The name (or path) for the file that will receive the data.
 *	\p data The data to be written.
 *	\p data_size The amount of data that will be written in the file.
 *	\r HANDLEDATA_RET_SUCCESS if could correctly write the file; 
 *	HANDLEDATA_RET_FILE_ERROR if any file operation has failed;
 *	HANDLEDATA_RET_INVALID_PARAM if one or more invalid parameters were received;
 *	HANDLEDATA_RET_ERROR for operation error.
 */
int handleData_write(const char *file_name, const void *data, const size_t data_size);


/**************************************************************************************************/
/**
 *	\b Load data from a file.
 *	\p file_name The file name that will be read.
 *	\p data The pointer that will receive the reference for the data readed from the file (must be
 *	freed).
 *	\p data_size The amount of data read from the file.
 *	\r HANDLEDATA_RET_SUCCESS if could correctly write the file; 
 *	HANDLEDATA_RET_INVALID_PARAM if one or more invalid parameters were received;
 *	HANDLEDATA_RET_ERROR for operation error.
 */
int handleData_load(const char *file_name, void **data, size_t *data_size);

#endif /* HANDLEDATA_HEADER */
