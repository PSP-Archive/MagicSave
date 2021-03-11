#include "common.h"

#if _DEBUG >= 1

void logs(const char *format, ...)
{
	SceUID file;
	va_list ap;
	char va_str[MS_INIT_STR_MAX];
	
	va_start( ap, format );
	vsnprintf( va_str, MS_INIT_STR_MAX,format, ap );
	va_end( ap );
	
	if( (file = sceIoOpen (MS_PATH_ROOT "/log.txt", PSP_O_WRONLY|PSP_O_APPEND|PSP_O_CREAT, 0777)) < 0 ) return;//ファイルオープン失敗
	
	sceIoWrite(file, va_str, strlen(va_str));
	
	sceIoClose(file);
}

#endif
