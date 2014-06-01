#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "memory_control.h"
#include "memory_control_defines.h"
#include "memory_control_structs.h"


/**************************************************************************************************/
/**
 *	\b Declare shared memory structure and initialize with default values.
 */
static moth_data_st moth_data = {
	.freeze = FREEZE_DEFAULT_VALUE,
	.halt = HALT_DEFAULT_VALUE,
	.name = NULL,
	.initialized = false,
};

/**************************************************************************************************/
/**
 *	\b Declare mutexes.
 */
static pthread_mutex_t general_var_lock;
static pthread_mutex_t visual_list_lock;


/**************************************************************************************************/
/**
 *	\b Free the shared memory resources and restore default values.
 */
static void memctrl_finalize_data(void);

/**************************************************************************************************/

static void memctrl_finalize_data(void)
{
	moth_data.freeze = FREEZE_DEFAULT_VALUE;
	moth_data.halt = HALT_DEFAULT_VALUE;
	free(moth_data.name);
	moth_data.name = NULL;
}

/**************************************************************************************************/

int memctrl_initialize(void)
{
	if (moth_data.initialized == true) {
		return 0;
	}

	int ret;

	/* Initialize mutexes. */
	ret = pthread_mutex_init(&general_var_lock, NULL);
	if (ret != 0) {
		return ret;
	}

	ret = pthread_mutex_init(&visual_list_lock, NULL);
	if (ret != 0) {
		(void ) pthread_mutex_destroy(&general_var_lock);
		return ret;
	}

	/* Initialize Moth data. */
	moth_data.initialized = true;

	return SUCCESS;
}

/**************************************************************************************************/

void memctrl_finalize(void)
{
	if (moth_data.initialized == false ) {
		return;
	}

	/* Unitialize the mutexes. */
	(void) pthread_mutex_destroy(&general_var_lock);
	(void) pthread_mutex_destroy(&visual_list_lock);


	/* Finalize the Moth data. */
	memctrl_finalize_data();
	moth_data.initialized = false;
}

/**************************************************************************************************/

CREATE_MUTEX_FUNCTIONS(freeze, moth_data.halt, general_var_lock);

/**************************************************************************************************/

CREATE_MUTEX_FUNCTIONS(halt, moth_data.halt, general_var_lock);

/**************************************************************************************************/

CREATE_MUTEX_FUNCTIONS(name, moth_data.name, general_var_lock);

/**************************************************************************************************/

