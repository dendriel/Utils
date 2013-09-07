#ifndef MEMORY_CONTROL_DEFINES_HEADER
#define MEMORY_CONTROL_DEFINES_HEADER

#define SUCCESS 0
#define ERROR 1

#define FREEZE_DEFAULT_VALUE false		//!< freeze flag default value.
#define HALT_DEFAULT_VALUE false		//!< halt flag default value.

/**
 *	\b Create default get and set functions with mutex.
 */
#define CREATE_MUTEX_FUNCTIONS(_name, _param, _mutex)\
__typeof__(_param) memctrl_get_##_name(void) {\
	__typeof__(_param) _value;\
	pthread_mutex_lock(&_mutex);\
	_value = _param;\
	pthread_mutex_unlock(&_mutex);\
	return _value;\
}\
void memctrl_set_##_name(__typeof__(_param) _value) {\
	pthread_mutex_lock(&_mutex);\
	_param = _value;\
	pthread_mutex_unlock(&_mutex);\
}

/**
 *	\b Declare default get and set functions.
 */
#define DECLARE_MUTEX_FUNCTIONS(_name, _type) \
_type memctrl_get_##_name(void);\
void memctrl_set_##_name(_type _name);


#endif /* MEMORY_CONTROL_DEFINES_HEADER */
