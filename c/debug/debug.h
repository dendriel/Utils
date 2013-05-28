/***************************************************************************************************
 * Description: Debug facility.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 05/2013
 **************************************************************************************************/
#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <stdio.h>

#define PREFIX "[debug]"

#if defined(DEBUG)
#define debug(fmt,args...) (void)(printf("%s %s:%3d - "fmt"\n",PREFIX, __FILE__, __LINE__, ##args))
#else
#define debug(...)
#endif

#endif /* DEBUG_HEADER */
