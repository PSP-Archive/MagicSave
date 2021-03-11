#ifndef MS_LOG_H
#define MS_LOG_H


#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif



#if _DEBUG >= 1
void logs(const char *format, ...);
#else
#define logs( format, ... )		;
#endif




#ifdef __cplusplus
}
#endif

#endif

