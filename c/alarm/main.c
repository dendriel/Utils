/*
 * Brief: Main program used to develop and test the alarm object.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Date: 25/05/2013
 * Note: For memory leak (not freed memory) use: 
 * "valgrind --leak-check=yes  --leak-check=full --show-reachable=yes"
 * There is a 136 bytes per thread that is supposed lost, but there is something wrong 
 * with the verification.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "alarm.h"
#include "alarm_defines.h"
#include "alarm_structs.h"

#define SUCCESS 0
#define ERROR -1
#define MS 1000
#define MAX 100
#define MQUEUE_NAME "/test_queue"


int launch_ala(int i, int *entry)
{
	st_alarm alarm;
	en_alarm_ret_code ret;
	char *str;

	memset(&alarm, 0, sizeof(st_alarm));

	alarm.wait_time = 50*MS;
	alarm.wait_time = 1000*MS;
	alarm.repeat = true;
	alarm.priority = 0;
	strncpy(alarm.dest_mqueue, MQUEUE_NAME, sizeof(alarm.dest_mqueue));

	str = (char *)malloc(sizeof(char)*10);
	if (!str) {
		printf("Failed to malloc!");
		return ERROR;
	}
	snprintf(str, 10, "vitor");
	alarm.data = (void *)str;

	ret = alarm_set_trigger(&alarm, entry);
	if (ret != ALARM_RET_SUCCESS) {
		printf("Failed to set the alarm. Error %d.\n", ret);
		return ERROR;
	}

	return SUCCESS;
}

/**
 * \b Main function.
 */
int main(void)
{
	int i;
	int ret;
	int entry[MAX];
	memset(&entry, 0, MAX);

	for (i=0; i < MAX; i++) {
		ret = launch_ala(i, &entry[i]);
		if (ret != 0) {
			printf("Failed to set alarm %d.\n", i);
		}
		printf("Alarm %d set.\n", i);
	}

	sleep(1);

	for (i=0; i < MAX; i++) {
		ret = alarm_remove_trigger(entry[i]);
		if (ret != ALARM_RET_SUCCESS) {
			printf("Failed to remove the alarm. Error %d.\n", ret);
			return ERROR;
		}
		printf("Alarm %d removed\n", i);
	}

	sleep(3);

	/* Try re-using the past allocated positions. */
	for (i=0; i < MAX; i++) {
		ret = launch_ala(i, &entry[i]);
		if (ret != 0) {
			printf("Failed to set alarm %d.\n", i);
		}
		printf("Alarm %d set.\n", i);
	}

	sleep(1);

	for (i=0; i < MAX; i++) {
		ret = alarm_remove_trigger(entry[i]);
		if (ret != ALARM_RET_SUCCESS) {
			printf("Failed to remove the alarm. Error %d.\n", ret);
			return ERROR;
		}
		printf("Alarm %d removed\n", i);
	}

	return SUCCESS;
}

