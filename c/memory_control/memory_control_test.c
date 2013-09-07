#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "memory_control.h"
#include "memory_control_defines.h"
#include "memory_control_structs.h"

#include "unittest.h"

#define TEST_STR "moth processor"

/**
 *	\b Test basic utilization (initialization, default values, get, set, finalization).
 *	\r 0 for success; -1 for test execution error.
 */
int unittest_memctrl_basic(void)
{
	/* Test initialization. */
	UT_VALIDATE_INT(memctrl_initialize(), SUCCESS);

	/* Test default values. get() operations. */
	UT_VALIDATE_BOOL(memctrl_get_freeze(), FREEZE_DEFAULT_VALUE);
	UT_VALIDATE_BOOL(memctrl_get_halt(), HALT_DEFAULT_VALUE);
	UT_VALIDATE_PT_CHAR(memctrl_get_name(), NULL);

	/* Test set functions. set() and get() operations. */
	/* freeze. */
	UT_DISPLAY(memctrl_set_freeze(!(FREEZE_DEFAULT_VALUE)));
	UT_VALIDATE_BOOL(memctrl_get_freeze(), !(FREEZE_DEFAULT_VALUE));
	/* halt. */
	UT_DISPLAY(memctrl_set_halt(!(HALT_DEFAULT_VALUE)));
	UT_VALIDATE_BOOL(memctrl_get_halt(), !(HALT_DEFAULT_VALUE));
	/* name. */
	char *test_data = (char *) malloc(strlen(TEST_STR) + 1);
	if (test_data == NULL) {
		printf("\nmalloc() failed. Will stop the test.\n");
		return -1;
	}

	strncpy(test_data, TEST_STR, strlen(TEST_STR)+1);
	UT_DISPLAY(memctrl_set_name(test_data));
	UT_VALIDATE_PT_CHAR(memctrl_get_name(), test_data);

	/* Test memctrl_finalize.
	 * The default values should be restored.
	 */
	UT_DISPLAY(memctrl_finalize());
	UT_VALIDATE_BOOL(memctrl_get_freeze(), FREEZE_DEFAULT_VALUE);
	UT_VALIDATE_BOOL(memctrl_get_halt(), HALT_DEFAULT_VALUE);
	UT_VALIDATE_PT_CHAR(memctrl_get_name(), NULL);

	return 0;
}


int main(void)
{
	UT_VALIDATE_SUITE(unittest_memctrl_basic());

	return 0;
}
