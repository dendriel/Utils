/***************************************************************************************************
 * Description: alarm module - functions header.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 25/05/2013
 **************************************************************************************************/
#ifndef ALARM_HEADER
#define ALARM_HEADER

#include "alarm_defines.h"
#include "alarm_structs.h"

/**
 *	\b Create a trigger.
 *	\p alarm The data to fill the trigger.
 *	\p entry Reference for the created alarm. Used to remove the alarm when it's not needed anymore.
 *	\r ALARM_RET_SUCCESS for successfully operation; ALARM_RET_INVALID_PARAM for null parameter;
 *	ALARM_RET_MALLOC_FAILED (see en_alarm_ret_code); ALARM_RET_ERROR if could not launch the thread.
 */
en_alarm_ret_code alarm_set_trigger(st_alarm *alarm, int *entry);

/**
 *	\b Remove a trigger.
 *	\p entry Reference to the alarm that must be removed.
 *	\r ALARM_RET_SUCCESS if the trigger was removed; ALARM_RET_ERROR if the operation failed.
 */
en_alarm_ret_code alarm_remove_trigger(int entry);

#endif /* ALARM_HEADER */
