#include <pspsdk.h>
#include <pspsysclib.h>
#include <stdarg.h>


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>




typedef struct
{
	char   *buf;
	const size_t *len;
	size_t cpylen;
} xprintf_ctx;



static void snprnt_cb( void *ctx, int ch )
{
	if( ch == 0x200 || ch == 0x201 ) return;
	
	xprintf_ctx *ctx_p = (xprintf_ctx*)ctx;
	
	//Buf�͈͓̔��Ȃ�R�s�[����
	if( ctx_p->cpylen  < *(ctx_p->len) )
	{
		ctx_p->buf[ctx_p->cpylen] = ch;
		ctx_p->cpylen++;
	}
}


int vsnprintf( char *buf, size_t n, const char *fmt, va_list ap )
{
	xprintf_ctx ctx = { buf, (const size_t *)&n, 0 };
	
	prnt( snprnt_cb, (void *)&ctx, fmt, ap );
	
	if( ctx.cpylen >= *(ctx.len) )
	{
		// ��������Buf�T�C�Y�𒴂���
		ctx.buf[*(ctx.len) - 1] = '\0';
	}
	else
	{
		// Buf�T�C�Y���Ɏ��܂��Ă���
		ctx.buf[ctx.cpylen] = '\0';
	}
	
	return ctx.cpylen;
}

int snprintf( char *buf, size_t n, const char *fmt, ... )
{
	va_list ap;
	int ret;
	
	va_start( ap, fmt );
	ret = vsnprintf( buf, n, fmt, ap );
	va_end( ap );
	
	return ret;
}


int strcasecmp( const char *s1, const char *s2 )
{
	register char ss1, ss2;
	
	while( ( ss1 = toupper( *s1 ) ) == ( ss2 = toupper( *s2 ) ) ){
		if( *s1 == '\0' ){
			return 0;
		} else{
			s1++;
			s2++;
		}
	}
	
	return ss1 - ss2;
}




//libini�ŕ��������l������Ȃ��̂Ń_�~�[
double strtod(const char *s, char **eptr)
{
	return 0.0;
}

