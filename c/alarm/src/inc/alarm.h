#ifndef ALARM_HEADER
#define ALARM_HEADER

#include "alarm_defines.h"
#include "alarm_structs.h"

/**
 * \b Create a trigger.
 * \p alarm The data to fill the trigger;
 * \r ALARM_RET_INVALID_PARAM for null parameter; ALARM_RET_MALLOC_FAILED (see en_alarm_ret_code);
 *	ALARM_RET_ERROR if could not launch the thread; ALARM_RET_SUCCESS for successfully operation.
 */
en_alarm_ret_code alarm_set_trigger(st_alarm *alarm);

#endif /* ALARM_HEADER */
