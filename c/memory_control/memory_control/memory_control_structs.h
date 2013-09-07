#ifndef MEMORY_CONTROL_STRUCTS_HEADER
#define MEMORY_CONTROL_STRUCTS_HEADER

#include <stdbool.h>

/**
 *	\b Shared memory template structure.
 */
typedef struct {
	bool freeze;
	bool halt;
	char *name;
	bool initialized; /* Used only for some types of attributes. */
} moth_data_st;


#endif /* MEMORY_CONTROL_STRUCTS_HEADER */
