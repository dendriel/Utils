#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>

#include "alarm.h"
#include "alarm_defines.h"
#include "alarm_structs.h"


static int alarm_send_message(st_alarm *alarm)
{
	int ret;
	mqd_t dest_mq;
	struct mq_attr attr;

	memset(&attr, 0, sizeof(attr));
	attr.mq_maxmsg = ALARM_MQUEUE_MAX_MSG;
	attr.mq_msgsize = sizeof(st_alarm);

	dest_mq = mq_open(alarm->dest_mqueue, O_CREAT | O_WRONLY, ALARM_MQUEUE_PERM , &attr);
	if (dest_mq == (mqd_t)-1) {
		fprintf(stderr, "Failed to open mqueue. errno: %d; msg: %s\n", errno, strerror(errno));
		return -1;
	}

	ret = mq_send(dest_mq, alarm->data, sizeof(alarm->data), alarm->priority);
	printf("sending message!!\n");
	if (ret != 0) {
		fprintf(stderr, "Failed to send message. errno: %d; msg: %s\n", errno, strerror(errno));
		mq_close(dest_mq);
		return -1;
	}

	ret = mq_close(dest_mq);
	if (ret != 0) {
		fprintf(stderr, "Failed to close mqueue. errno: %d; msg: %s\n", errno, strerror(errno));
		return -1;
	}

	return 0;
}

static void alarm_thread(void *data)
{
	int ret;
	st_alarm *alarm = (st_alarm *)data;

	do {
		usleep(alarm->wait_time);
		ret = alarm_send_message(alarm);
		if (ret != 0) {
			fprintf(stderr, "Failed to execute trigger.\n");
		}
	} while (alarm->repeat);
}

en_alarm_ret_code alarm_set_trigger(st_alarm *alarm)
{
	if (!alarm) {
		return ALARM_RET_INVALID_PARAM;
	}

	int ret;
	pthread_t alarm_th;
	st_alarm *data;

	memset(&alarm_th, 0, sizeof(pthread_t));

	data = (st_alarm *)malloc(sizeof(st_alarm));
	if (!data) {
		return ALARM_RET_MALLOC_FAILED;
	}

	memcpy(data, alarm, sizeof(*data));

	ret = pthread_create(&alarm_th, NULL, (void *)alarm_thread, data);
	if (ret != 0) {
		return ALARM_RET_ERROR;
	}

	//TODO: store thread_id; maybe return an id;

	return ALARM_RET_SUCCESS;
}

// TODO:
int alarm_cancel_trigger(int trigger_id)
{
	return ALARM_RET_SUCCESS;
}

