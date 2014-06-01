#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <iomanip>
#include <assert.h>

/**************************************************************************************************
 * Debug utilities.
 */
//!< Checks and print error if the operation fails.
#define CHK_NULL(_f)\
{\
	if ((_f) == NULL) {\
		std::cout << "Error occurred at " << #_f << endl;\
		return -1;\
	}\
}

//!< Checks and print error if the operation fails.
#define CHK(_f, _r)\
{\
	if ((_f) == (_r)) {\
		std::cout << "Error occurred at " << #_f << "; Returned: " << _r << endl;\
		return -1;\
	}\
}

#define PREFIX "[debug]"

#if defined(DEBUG)
#define debug(_str) std::cout << PREFIX << " " << setw(25) << __FILE__ << ":" << setw(4) << __LINE__ << ": " << _str << std::endl
#define assert_exit(_str) debug(_str << " (Asserting...)"); assert(0)
#else
#define debug(...)
#define assert_exit(...) assert(0)
#endif



#endif /* DEBUG_H_ */
