/***************************************************************************************************
 * Description: handleData module - Functions that permit manipulate data with files.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 27/05/2013
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "handleData.h"
#include "handleData_defines.h"


/**************************************************************************************************/

en_handleData_ret_code handleData_load(const char *file_name, void **data, size_t *data_size)
{
	if (!file_name || !data || !data_size) {
		fprintf(stderr, "Invalid parameter received.");
		return HANDLEDATA_RET_ERROR;
	}

	FILE *file;
	long file_len;
	char *ret_buffer = NULL;

	file = fopen(file_name, HANDLE_DATA_RDBIN_MODE);
	if (!file) {
		fprintf(stderr, "Failed to open the file %s.", file_name);
		return HANDLEDATA_RET_ERROR;
	}
	
	/* Find out the file length. */
	fseek(file, 0, SEEK_END);
	file_len = ftell(file);
	fseek(file, 0, SEEK_SET);

	/* Allocate memory space to the return buffer. */
	ret_buffer = (char *)malloc(sizeof(char)*file_len);
	if (!ret_buffer) {
		fprintf(stderr, "Failed to allocate memory.");
		return HANDLEDATA_RET_ERROR;
	}

	fread(ret_buffer, file_len, 1, file);
	fclose(file);

	*data = (void *)ret_buffer;
	*data_size = file_len;

	return HANDLEDATA_RET_SUCCESS;
}

/**************************************************************************************************/

en_handleData_ret_code handleData_write(const char *file_name, const void *data, const size_t data_size)
{
	if (!file_name || !data) {
		fprintf(stderr, "Invalid parameter received.");
		return HANDLEDATA_RET_ERROR;
	}

	FILE *file;

	file = fopen(file_name, HANDLE_DATA_WRBIN_MODE);
	if (!file) {
		fprintf(stderr, "Failed to open file %s.", file_name);
		return HANDLEDATA_RET_ERROR;
	}

	fwrite(data, data_size, 1, file);
	fclose(file);

	return HANDLEDATA_RET_SUCCESS;
}

