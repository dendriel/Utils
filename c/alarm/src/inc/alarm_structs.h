/***************************************************************************************************
 * Description: alarm module - declaration of structures.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 25/05/2013
 **************************************************************************************************/

#ifndef ALARM_STRUCTS_HEADER
#define ALARM_STRUCTS_HEADER

#include <linux/limits.h>
#include <stdbool.h>
#include <pthread.h>

/**
 * \b Structure used to set up alarms.
 */
typedef struct {
	unsigned int wait_time;			//!< Time to wait in micro seconds before triggering.
	bool repeat;					//!< Repeat the alarm unitl forever.
	char dest_mqueue[NAME_MAX];		//!< Destination mqueue.
	unsigned int priority;			//!< Mqueue priority of the message.
	void *data;						//!< Storage pointer (used to inform an action or maintain some data).
	size_t data_size;				//!< Storage the data size.
} st_alarm;

/**
 * \b Structure that identifies an entry of alarm in the global list.
 */
typedef struct {
	pthread_t tid;						//!< Id of the thread that holds the trigger.
	st_alarm *alarm;				//!< Reference to the alarm that (used to free at alarm cancelation).
} st_alarm_entry;

#endif /* ALARM_STRUCTS_HEADER */
