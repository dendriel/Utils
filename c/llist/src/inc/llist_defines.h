#ifndef LLIST_DEFINES_HEADER
#define LLIST_DEFINES_HEADER

/**
 *	\b Linked list return codes.
 */
typedef enum {
	LLIST_RET_SUCCESS	= 0,			//!< Generic success return code.
	LLIST_RET_ERROR		= -1,			//!< Generic error return code.
	LLIST_RET_NOTFOUND	= -2,			//!< Requisited item was not found in the list.
} en_llist_ret_code;

#endif /* LLIST_DEFINES_HEADER */
