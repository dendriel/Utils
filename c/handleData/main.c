/***************************************************************************************************
 * Brief: Main program used to develop and test the handleData module.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Date: 27/05/2013
 **************************************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "handleData.h"

typedef struct {
	char a;
	char b;
	char c;
	char d;
	char label[10];
} st_scenery;


int main(void)
{
	int ret;
	char *file_name = "scenery.dat";
	st_scenery sc_data;

	memset(&sc_data, 'h', sizeof(sc_data));
	sc_data.a = 'a';
	sc_data.b = 'b';
	sc_data.c = 'c';
	sc_data.d = 'd';

	/* Save data. */
	ret = handleData_write(file_name, &sc_data, sizeof(st_scenery));
	if (ret != 0) {
		printf("Failed to write down the data.");
		return -1;
	}

	st_scenery *scenery = NULL;
	size_t size;

	/* Load data. */
	ret = handleData_load(file_name, (void *)&scenery, &size);
	if (ret != 0) {
		printf("Failed to load the data.");
		return -1;
	}

	if (size == sizeof(st_scenery)) {
		fwrite(scenery, sizeof(st_scenery), 1, stdout);
		printf("\n");
	}
	else {
		printf("Invalid data read from the file. size: %d\n", (int)size);
	}

	free(scenery);

	return 0;
}

