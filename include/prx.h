#ifndef MS_PLUGIN_H
#define MS_PLUGIN_H

#include <pspkernel.h>
#include <pspsdk.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


#define	MS_PLUGIN_TABLE_MAX	255

typedef struct
{
	const char *Name;
	void (*Patch)(const char *,void *,SceSize);
} Plugin_Func;


typedef struct
{
	Plugin_Func *Func;
	int TableSize;
	int	Select;
} Plugin_Info;


bool MS_GetFuncInfo_Load( Plugin_Info *Info );
bool MS_GetFuncInfo_Save( Plugin_Info *Info );



#ifdef __cplusplus
}
#endif

#endif

