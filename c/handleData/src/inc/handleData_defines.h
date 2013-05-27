/***************************************************************************************************
 * Description: handleData module - Define constants.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 27/05/2013
 **************************************************************************************************/
#ifndef HANDLEDATA_DEFINES_HEADER
#define HANDLEDATA_DEFINES_HEADER


#define HANDLE_DATA_RDBIN_MODE "rb"		//!< Read file mode.
#define HANDLE_DATA_WRBIN_MODE "wb"		//!< Write file mode.

/**
 *	\b Handle Data module returne codes.
 */
typedef enum {
	HANDLEDATA_RET_SUCCESS			= 0,		//!< Generic success return code.
	HANDLEDATA_RET_ERROR			= -1,		//!< Generic error return code.
} en_handleData_ret_code;

#endif /* HANDLEDATA_DEFINES_HEADER */
