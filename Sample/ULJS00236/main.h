#ifndef MS_PLUGIN_H
#define MS_PLUGIN_H

#include <pspkernel.h>
#include <pspsdk.h>


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>




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
} Plugin_Info;


bool MS_GetFuncInfo_Load( Plugin_Info *Info );
bool MS_GetFuncInfo_Save( Plugin_Info *Info );





#endif
