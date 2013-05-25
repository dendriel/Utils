/***************************************************************************************************
 * Description: alarm module - Functions that permit the setup of alarms that send data through 
 * message queues.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 25/05/2013
 **************************************************************************************************/

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


/**
 *	\b Alarms list that permits the cancellation and liberation of stored data.
 *	\b Certainly there is a better way to do this. Maybe an linked list, or even 
 *	an initialization function that tells the N alarms entries needed in the list.
 *	For multiple threading [intense] use, mutex would be necessary.
 */
static st_alarm_entry alarms_list[ALARM_MAX_ENTRY];

/**
 *	\b Just a flag to signal the need of initialize the alarms_list.
 */
static bool alarms_list_initialized = false;


/**************************************************************************************************/
/**
 *	\b Send a message to the requesting process.
 *	\p alarm Alarm data to identify the process.
 *	\r ALARM_RET_SUCCESS for successfully operation; ALARM_RET_ERROR otherwise.
 */
static en_alarm_ret_code alarm_send_message(st_alarm *alarm);

/**************************************************************************************************/
/**
 *	\b Main alarm thread loop.
 */
static void alarm_thread(void *data);

/**************************************************************************************************/
/**
 *	\b Save an alarm in the alarms list.
 *	\p index The position of the alarm in the list.
 *	\p tid The running thread that trigger the alarm.
 *	\p alarm The alarm structure with allocated data.
 *	\r ALARM_RET_SUCCESS if could insert the alarm in the list; ALARM_RET_INVALID_PARAM if one or 
 *	more parameters are invalid.
 */
static en_alarm_ret_code alarm_list_add_entry(int index, pthread_t tid, st_alarm *alarm);

/**************************************************************************************************/
/*
 *	\b Search and allocate an entry in the list, if there is any free.
 *	\r Return An index for the entry in the list (>= 0) or ALARM_RET_FULL if there is no more space 
 *	in the list.
 */
static int alarm_list_alloc_entry(void);

/**************************************************************************************************/
/**
 *	\b Clean the allocation flag of the entry.
 *	\p entry The index of the entry.
 *	\r ALARM_RET_SUCCESS if could free the entry; ALARM_RET_INVALID_PARAM if the given index is 
 *	invalid.
 */
static en_alarm_ret_code alarm_list_free_entry(int entry);

/**************************************************************************************************/
/**
 *	\b Free the trigger allocated memory.
 *	\p entry The index of the entry.
 */
static void alarm_remove_trigger_data(int entry);


/**************************************************************************************************/

static en_alarm_ret_code alarm_send_message(st_alarm *alarm)
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
		return ALARM_RET_ERROR;
	}

	ret = mq_send(dest_mq, alarm->data, sizeof(alarm->data), alarm->priority);
	printf("sending message!!\n");
	if (ret != 0) {
		fprintf(stderr, "Failed to send message. errno: %d; msg: %s\n", errno, strerror(errno));
		mq_close(dest_mq);
		return ALARM_RET_ERROR;
	}

	ret = mq_close(dest_mq);
	if (ret != 0) {
		fprintf(stderr, "Failed to close mqueue. errno: %d; msg: %s\n", errno, strerror(errno));
		return ALARM_RET_ERROR;
	}

	return ALARM_RET_SUCCESS;
}

/**************************************************************************************************/

static void alarm_thread(void *data)
{
	int ret;
	st_alarm *alarm = (st_alarm *)data;

	do {
		usleep(alarm->wait_time);
		ret = alarm_send_message(alarm);
		if (ret != ALARM_RET_SUCCESS) {
			fprintf(stderr, "Failed to execute the trigger.\n");
		}
	} while (alarm->repeat);
}

/**************************************************************************************************/

static en_alarm_ret_code alarm_list_add_entry(int entry, pthread_t tid, st_alarm *alarm)
{
	if ((0 > entry) || (entry > ALARM_MAX_ENTRY)) {
		return ALARM_RET_INVALID_PARAM;
	}

	alarms_list[entry].tid = tid;
	alarms_list[entry].alarm = alarm;
	
	return ALARM_RET_SUCCESS;
}

/**************************************************************************************************/

static int alarm_list_alloc_entry(void)
{
	int i;

	if (alarms_list_initialized == false) {
		memset(alarms_list, 0, sizeof(alarms_list));
		alarms_list_initialized = true;
		alarms_list[ALARM_FIRST_ENTRY].tid = ALARM_ENTRY_USED;
		return ALARM_FIRST_ENTRY;
	}

	for (i = 0; i < ALARM_MAX_ENTRY; i++) {
		if (alarms_list[i].tid == ALARM_ENTRY_FREE) {
			alarms_list[i].tid = ALARM_ENTRY_USED;
			return i;
		}
	}

	return ALARM_RET_FULL;
}

/**************************************************************************************************/

static en_alarm_ret_code alarm_list_free_entry(int entry)
{
	/* if (++++++0-------------256++++++) */
	if ((0 > entry) || (entry > ALARM_MAX_ENTRY)) {
		return ALARM_RET_INVALID_PARAM;
	}

	alarms_list[entry].tid = ALARM_ENTRY_FREE;

	return ALARM_RET_SUCCESS;
}

/**************************************************************************************************/

static void alarm_remove_trigger_data(int entry)
{
	if ((0 > entry) || (entry > ALARM_MAX_ENTRY)) {
		return;
	}

	free(alarms_list[entry].alarm->data);
	free(alarms_list[entry].alarm);
}

/**************************************************************************************************/

en_alarm_ret_code alarm_set_trigger(st_alarm *alarm, int *entry)
{
	en_alarm_ret_code mret;
	int ret;
	int entry_index;
	pthread_t alarm_th;
	st_alarm *al_data;

	if (!alarm) {
		return ALARM_RET_INVALID_PARAM;
	}

	/* Verify and allocate space in the list to add a new alarm. */
	entry_index = alarm_list_alloc_entry();
	if (entry_index == ALARM_RET_FULL) {
		return ALARM_RET_FULL;
	}

	void free_entry() {
		mret = alarm_list_free_entry(entry_index);
		if (mret != ALARM_RET_SUCCESS) {
			fprintf(stderr, "Failed to free the list element (internal error).\n");
		}
		return;
	}

	/* Allocate memory and copy the alarm data to local reference. */
	al_data = (st_alarm *)malloc(sizeof(st_alarm));
	if (!al_data) {
		free_entry();
		return ALARM_RET_MALLOC_FAILED;
	}
	memcpy(al_data, alarm, sizeof(*al_data));

	/* Launch the thread. */
	ret = pthread_create(&alarm_th, NULL, (void *)alarm_thread, al_data);
	if (ret != 0) {
		free_entry();
		free(al_data->data);
		free(al_data);
		return ALARM_RET_ERROR;
	}

	/* Save the reference of the thread for later resources release. */
	mret = alarm_list_add_entry(entry_index, alarm_th, al_data);
	if (mret != ALARM_RET_SUCCESS) {
		fprintf(stderr, "Failed to add entry in the alarms list.\n");
		return ALARM_RET_ERROR;
	}

	/* Save reference. */
	*entry = entry_index;

	return ALARM_RET_SUCCESS;
}

/**************************************************************************************************/

en_alarm_ret_code alarm_remove_trigger(int entry)
{
	int ret;

	if ((0 > entry) || (entry > ALARM_MAX_ENTRY)) {
		return ALARM_RET_INVALID_PARAM;
	}

	/* Stop the thread. (TODO: ensure that the thread is not in middle of an mqueue operation). */
	ret = pthread_cancel(alarms_list[entry].tid);
	if (ret != 0) {
		fprintf(stderr, "Failed to cancel the thread. errno: %d; msg: %s\n", errno, strerror(errno));
		return ALARM_RET_ERROR;
	}

	/* Remove the alarm stored data. */
	alarm_remove_trigger_data(entry);

	return ALARM_RET_SUCCESS;
}

