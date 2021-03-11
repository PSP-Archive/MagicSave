#ifndef MS_PLG_H
#define MS_PLG_H


#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Is_Load_Plugin()		(MS_INI_PLG_ID_S >= 0)


enum
{
	PLG_Func_Load = 0,
	PLG_Func_Save,
};

typedef struct
{
	int Select;
	const char *NList[MS_PLUGIN_TABLE_MAX+1];
	Plugin_Info PLG;
} PLG_TBL;


void Call_PLG_Func(int type ,const char *msg,const char *fileName,void *data,SceSize len);
#define Call_PLG_Load(msg,fileName,data,len)	Call_PLG_Func(PLG_Func_Load,msg,fileName,data,len);
#define Call_PLG_Save(msg,fileName,data,len)	Call_PLG_Func(PLG_Func_Save,msg,fileName,data,len);


SceUID MS_Plugin_Load();
SceUID MS_Plugin_Unload();

int	MS_Mode_Status();



extern PLG_TBL PTBL_Load,PTBL_Save;
extern SceUID	MS_INI_PLG_ID_S;

#ifdef __cplusplus
}
#endif

#endif
