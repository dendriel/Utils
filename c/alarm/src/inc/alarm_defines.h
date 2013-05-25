/***************************************************************************************************
 * Description: alarm module - constants definition.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 25/05/2013
 **************************************************************************************************/

#ifndef ALARM_DEFINES_HEADER
#define ALARM_DEFINES_HEADER

#define ALARM_MAX_ENTRY			256		//!< Maximum alarm creation quantity.
#define ALARM_ENTRY_USED		-1		//!< Used to allocate an entry in the alarms list.
#define ALARM_ENTRY_FREE		0		//!< Indicate an free entry in the list to be used.
#define ALARM_FIRST_ENTRY		0		//!< Identifies the first entry of the list.
#define ALARM_MQUEUE_PERM		0644	//!< Permission mask to open the mqueue.
#define ALARM_MQUEUE_MAX_MSG	10		//!< Message queue maximum enqueue of messages.

/**
 *	\b Alarm module return codes.
 */
typedef enum {
	ALARM_RET_SUCCESS		= 0,	//!< Generic success return code.
	ALARM_RET_ERROR			= -1,	//!< Generic error return code.
	ALARM_RET_INVALID_PARAM	= -2,	//!< Invalid parameter return code.
	ALARM_RET_MALLOC_FAILED	= -3,	//!< Try to malloc failed.
	ALARM_RET_FULL			= -4,	//!< There is no more space to set another trigger.
} en_alarm_ret_code;

/**
 *	\b Alarm triggering actions.
 */
typedef enum {
	ALARM_TRIGGER_SET		= 0,	//!< Set a trigger.
	ALARM_TRIGGER_RELEASE	= 1,	//!< Release an existing trigger.
} en_alarm_action;

#endif /* ALARM_DEFINES_HEADER */
