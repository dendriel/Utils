#ifndef UNITTEST_HEADER
#define UNITTEST_HEADER

#include <stdbool.h>

/**
 *	\b Declare control variables of the test.
 */
unsigned int _errorc;
unsigned int _testc;

/**
 *	\b Validate test suite.
 */
#define UT_VALIDATE_SUITE(_f)\
{\
	_errorc = 0;\
	_testc = 0;\
	printf("\nExecute: %s\n\n", #_f);\
	if (_f != 0) {\
		printf("\n\t%s has failed.\n", #_f);\
	}\
	else {\
		printf("\nExecution results:\n");\
		printf("\tTests executed: %d\n\tTests failed: %d\n\n", _testc, _errorc);\
	}\
}

/**
 *	\b Print void functions execution as reference.
 */
#define UT_DISPLAY(_f)\
	(void) _f;\
	printf("\t%s Ok.\n", #_f);

/**
 *	\b Validate function of integer type.
 */
#define UT_VALIDATE_INT(_f, _val)\
{\
	int _ret;\
	if ((UT_VALIDATE_GENERIC(_f, (_val), _ret)) != (_val)) {\
		printf("Expected: %d; Returned: %d\n\n", (_val), _ret);\
	}\
}

/**
 *	\b Validate function of bool type.
 */
#define UT_VALIDATE_BOOL(_f, _val)\
{\
	bool _ret;\
	if ((UT_VALIDATE_GENERIC(_f, (_val), _ret)) != (_val)) {\
		printf("Expected: %s; Returned: %s\n\n", (_val)?"true":"false", (_ret)?"true":"false");\
	}\
}

/**
 *	\b Validate function of char pointer.
 */
#define UT_VALIDATE_PT_CHAR(_f, _val)\
{\
	char *_ret;\
	if ((UT_VALIDATE_GENERIC(_f, (_val), _ret)) != (_val)) {\
		printf("Expected: %p; Returned: %p\n\n", (_val), _ret);\
	}\
}


/**
 *	\b Generic validation.
 */
#define UT_VALIDATE_GENERIC(_f, _val, _ret)\
{\
	_testc++;\
	if ((_ret = _f) != (_val)) {\
		printf("\t%s Failed!\n\t\t"\
		"VALIDATE(%s == %s)\n\t\t"\
		"Line: %d;\n\t\t", #_f, #_f, #_val,  __LINE__);\
		_errorc++;\
	}\
	else {\
		printf("\t%s Passed!\n", #_f);\
	}\
	_ret;\
}

#endif /* UNITTEST_HEADER */
