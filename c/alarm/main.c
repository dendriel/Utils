/*
 * brief: main program used to develop and test the alarm object.
 * author: vitor.rozsa
 * date: 25/05/2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "alarm.h"
#include "alarm_defines.h"
#include "alarm_structs.h"

#define MS 1000

int main(void)
{
	st_alarm alarm;
	en_alarm_ret_code ret;

	memset(&alarm, 0, sizeof(st_alarm));

	alarm.wait_time = 50*MS;
	alarm.wait_time = 1000*MS;
	alarm.repeat = true;
	alarm.priority = 0;
	strncpy(alarm.dest_mqueue, "/test_queue", sizeof(alarm.dest_mqueue));

	char *str = (char *)malloc(sizeof(char)*10);
	if (!str) {
		printf("failed to malloc!");
		return -1;
	}
	snprintf(str, 10, "vitor");
	alarm.data = (void *)str;

	ret = alarm_set_trigger(&alarm);
	if (ret != ALARM_RET_SUCCESS) {
		printf("Failed to set the alarm. Error %d.\n", ret);
		return -1;
	}

	while(true);

	return 0;
}
