#ifndef ALARM_STRUCTS_HEADER
#define ALARM_STRUCTS_HEADER

#include <linux/limits.h>
#include <stdbool.h>

/**
 * \b Structure used to set up alarms.
 */
typedef struct {
	unsigned int wait_time;			//!< Time to wait in micro seconds before triggering.
	bool repeat;					//!< Repeat the alarm unitl forever.
	char dest_mqueue[NAME_MAX];		//!< Destination mqueue.
	unsigned int priority;			//!< Mqueue priority of the message.
	void *data;						//!< Storage pointer (used to inform an action or maintain some data).
} st_alarm;

#endif /* ALARM_STRUCTS_HEADER */
