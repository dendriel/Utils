#ifndef MEMORY_CONTROL_HEADER
#define MEMORY_CONTROL_HEADER

#include <stdbool.h>
#include "memory_control_structs.h"
#include "memory_control_defines.h"

/**
 *	\b Initialize shared memory control module. Needed to access some data types (the types that is
 *	allocated).
 *	\r SUCCESS if the module was initialized; ERROR otherwise.
 */
int memctrl_initialize(void);

/**
 *	\b Finalize the control module. Free aloccated resources and return defaults values. Also, set
 *	the "initialized" flag to false.
 */
void memctrl_finalize(void);

/**
 *	\b Set and Get functions for freeze variable.
 */
DECLARE_MUTEX_FUNCTIONS(freeze, bool);

/**
 *	\b Set and Get functions for halt variable.
 */
DECLARE_MUTEX_FUNCTIONS(halt, bool);

/**
 *	\b Set and Get functions for name variable.
 */
DECLARE_MUTEX_FUNCTIONS(name, char*);

#endif /* MEMORY_CONTROL_HEADER */
