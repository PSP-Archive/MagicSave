#include "common.h"

#include "cfg.h"
#include "plugin.h"

#include <libini.h>


enum KEY_ID
{
	KEY_NONE	,
	KEY_FORMAT	,
	KEY_RIDS	,
	KEY_RIDD	,
	KEY_PAD		,
	KEY_MENUT	,
};


int		MS_INI_SFormat				= MS_INIT_SFORMAT;
bool	MS_INI_RID					= MS_INIT_RID;
int		MS_INI_RID_SFO				= MS_INIT_RID_SFO;
int		MS_INI_RID_DEC				= MS_INIT_RID_DEC;
bool	MS_INI_DumpGameKey			= MS_INIT_GAMEKEY;
bool	MS_INI_WMS					= MS_INIT_WMS;
bool	MS_INI_RMS					= MS_INIT_RMS;
int		MS_INI_Menu_Delay			= MS_INIT_MDELAY;
bool	MS_INI_Menu_Type			= MS_INIT_MTYPE;
bool	MS_INI_Menu_STH				= MS_INIT_MSTH;
int		MS_INI_Menu_Pos_X			= MS_INIT_POSX;
int		MS_INI_Menu_Pos_Y			= MS_INIT_POSY;
bool	MS_INI_Menu					= MS_INIT_MENU;
bool 	MS_INI_Plg_Use 				= false;
bool	MS_INI_Hook					= true;


static char buf[256];
static ini_pair list[30];
static ini_data data;


static struct key_list
{
	char *key;
	ini_type type;
	void *data;
	bool ronly;
	int id;
} const setting[] = 
{
	{ INI_KEY_FORMAT		,INI_TYPE_STR		, NULL						, false	,KEY_FORMAT	},//
	{ INI_KEY_DKEY 			,INI_TYPE_BOOL		, &MS_INI_DumpGameKey		, false	,KEY_NONE	},
	
	{ INI_KEY_RID 			,INI_TYPE_BOOL		, &MS_INI_RID				, true	,KEY_NONE	},
	{ INI_KEY_RIDS		 	,INI_TYPE_STR		, NULL						, true	,KEY_RIDS	},//
	{ INI_KEY_RIDD 			,INI_TYPE_STR		, NULL						, true	,KEY_RIDD	},//
	
	{ INI_KEY_BKUPL 		,INI_TYPE_BOOL		, &MS_INI_RMS				, false	,KEY_NONE	},
	{ INI_KEY_BKUPS 		,INI_TYPE_BOOL		, &MS_INI_WMS				, false	,KEY_NONE	},
	
	{ INI_KEY_MENU 			,INI_TYPE_BOOL		, &MS_INI_Menu				, true	,KEY_NONE	},
	{ INI_KEY_DELAY 		,INI_TYPE_DEC		, &MS_INI_Menu_Delay		, false	,KEY_NONE	},
	{ INI_KEY_MENU_T 		,INI_TYPE_STR		, NULL						, false	,KEY_MENUT	},//
	{ INI_KEY_MENU_STH 		,INI_TYPE_BOOL		, &MS_INI_Menu_STH			, false	,KEY_NONE	},
	{ INI_KEY_MENU_PX 		,INI_TYPE_DEC		, &MS_INI_Menu_Pos_X		, true	,KEY_NONE	},
	{ INI_KEY_MENU_PY 		,INI_TYPE_DEC		, &MS_INI_Menu_Pos_Y		, true	,KEY_NONE	},
	
	{ INI_KEY_HOTK_SHOW 	,INI_TYPE_STR		, &Context.HotKey.Show		, true	,KEY_PAD	},
	{ INI_KEY_HOTK_BACK 	,INI_TYPE_STR		, &Context.HotKey.Back		, true	,KEY_PAD	},
	{ INI_KEY_HOTK_SEL 		,INI_TYPE_STR		, &Context.HotKey.Select	, true	,KEY_PAD	},
	{ INI_KEY_HOTK_UP 		,INI_TYPE_STR		, &Context.HotKey.Up		, true	,KEY_PAD	},
	{ INI_KEY_HOTK_DOWN 	,INI_TYPE_STR		, &Context.HotKey.Down		, true	,KEY_PAD	},
	{ INI_KEY_HOTK_LEFT 	,INI_TYPE_STR		, &Context.HotKey.Left		, true	,KEY_PAD	},
	{ INI_KEY_HOTK_RIGHT 	,INI_TYPE_STR		, &Context.HotKey.Right		, true	,KEY_PAD	},
	{ INI_KEY_PLG_USE		,INI_TYPE_BOOL		, &MS_INI_Plg_Use			, false	,KEY_NONE	},
	{ INI_KEY_PLG_LFNC		,INI_TYPE_DEC		, &PTBL_Load.Select			, false	,KEY_NONE 	},
	{ INI_KEY_PLG_SFNC		,INI_TYPE_DEC		, &PTBL_Save.Select			, false	,KEY_NONE 	},
	{ INI_KEY_HOOK 			,INI_TYPE_BOOL		, &MS_INI_Hook				, true	,KEY_NONE	},
};


static int key_list_size = sizeof(setting) / sizeof(struct key_list);

// save_cfg(iniSave callback)
static const char* scb_main(const char *key,void *opt,ini_pair *pair)
{
	const char *val = NULL;
	
	logs("scb_main(%d,%s) = %p\n",opt,key,val);
	
	
	if( pair->id == KEY_FORMAT )
	{
		//"Save_Format"
		
		switch( MS_INI_SFormat )
		{
			case SFORMAT_AUTO	:val = MS_STR_AUTO		;break;
			case SFORMAT_RAW	:val = MS_STR_RAW		;break;
			case SFORMAT_NORMAL	:val = MS_STR_NORMAL	;break;
		}
	}
	else if( pair->id == KEY_MENUT )
	{
		//"Menu_Type"
		
		val = ( MS_INI_Menu_Type == LIBM_VIEW_ALL ? MS_STR_ALL : MS_STR_ACTIVE );
	}
	
	return val;
}

// lcb_main(iniSplitVal callback)
static void cb_split(const char *val,void *opt)
{
	struct Pad_Data_Table
	{
		char *name;
		unsigned int code;
	} const pad_data[] = 
	{
		{ PAD_KEY_SELECT	,PSP_CTRL_SELECT	},
		{ PAD_KEY_START		,PSP_CTRL_START		},
		{ PAD_KEY_UP		,PSP_CTRL_UP		},
		{ PAD_KEY_RIGHT		,PSP_CTRL_RIGHT		},
		{ PAD_KEY_DOWN		,PSP_CTRL_DOWN		},
		{ PAD_KEY_LEFT		,PSP_CTRL_LEFT		},
		{ PAD_KEY_LT		,PSP_CTRL_LTRIGGER	},
		{ PAD_KEY_RT		,PSP_CTRL_RTRIGGER	},
		{ PAD_KEY_TRIANGLE	,PSP_CTRL_TRIANGLE	},
		{ PAD_KEY_CIRCLE	,PSP_CTRL_CIRCLE	},
		{ PAD_KEY_CROSS		,PSP_CTRL_CROSS		},
		{ PAD_KEY_SQUARE	,PSP_CTRL_SQUARE	},
		{ PAD_KEY_HOME		,PSP_CTRL_HOME		},
		{ PAD_KEY_HOLD		,PSP_CTRL_HOLD		},
		{ PAD_KEY_NOTE		,PSP_CTRL_NOTE		},
		{ PAD_KEY_SCREEN	,PSP_CTRL_SCREEN	},
		{ PAD_KEY_VUP		,PSP_CTRL_VOLUP		},
		{ PAD_KEY_VDOWN		,PSP_CTRL_VOLDOWN	},
		{ PAD_KEY_WLAN		,PSP_CTRL_WLAN_UP	},
		{ PAD_KEY_REMOTE	,PSP_CTRL_REMOTE	},
		{ PAD_KEY_DISC		,PSP_CTRL_DISC		},
		{ PAD_KEY_MS		,PSP_CTRL_MS		},
	};
	
	int i;
	
	for( i = 0 ; i < (sizeof(pad_data) / sizeof(struct Pad_Data_Table)) ; i++ )
	{
		if( !strcasecmp(val,pad_data[i].name) )
		{
			*(int*)opt |= pad_data[i].code;
			break;
		}
	}
	
	logs("cb_split(%s,%p) = 0x%08X\n",val,opt,*(int*)opt);
}



// load_cfg(iniLoad callback)
static bool lcb_main(const char *key, char *val,void *opt,ini_pair *pair)
{
	bool ret = true;
	
	logs("icb_main(%s,%s,%p)\n",key,val,opt);
	
	if( pair->id == KEY_FORMAT )
	{
		//Save_Format
		
		if( !strcasecmp(val,MS_STR_AUTO) )
		{
			MS_INI_SFormat = SFORMAT_AUTO;
		}
		else if( !strcasecmp(val,MS_STR_RAW) )
		{
			MS_INI_SFormat = SFORMAT_RAW;
		}
		else if( !strcasecmp(val,MS_STR_NORMAL) )
		{
			MS_INI_SFormat = SFORMAT_NORMAL;
		}
		else
		{
			ret = false;
		}
	}
	else if( pair->id == KEY_RIDS )
	{
		//RID_SFO"
		
		if( !strcasecmp(val,MS_STR_TYPE1) )
		{
			MS_INI_RID_SFO = RID_SFO1;
		}
		else if( !strcasecmp(val,MS_STR_TYPE2) )
		{
			MS_INI_RID_SFO = RID_SFO2;
		}
		else if( !strcasecmp(val,MS_STR_TYPE3) )
		{
			MS_INI_RID_SFO = RID_SFO3;
		}
		else
		{
			ret = false;
		}
	}
	else if( pair->id == KEY_RIDD )
	{
		//"RID_DEC"
		
		if( !strcasecmp(val,MS_STR_TYPE1) )
		{
			MS_INI_RID_DEC = RID_DEC1;
		}
		else if( !strcasecmp(val,MS_STR_TYPE2) )
		{
			MS_INI_RID_DEC = RID_DEC2;
		}
		else if( !strcasecmp(val,MS_STR_TYPE3) )
		{
			MS_INI_RID_DEC = RID_DEC3;
		}
		else
		{
			ret = false;
		}
	}
	else if( pair->id == KEY_PAD )
	{
		//Menu_Key_XXXX
		
		u32 pad = 0;
		
		//Valを分解・解析してPadコードへ変換
		iniSplitVal(val,"+",cb_split,&pad);
		
		if( pad != 0 && pair->val_load )
		{
			*(u32*)pair->val_load = pad;
		}
		else
		{
			ret = false;
		}
	}
	else if( pair->id == KEY_MENUT )
	{
		//Menu_Type
		
		if( !strcasecmp(val,MS_STR_ALL) )
		{
			MS_INI_Menu_Type = LIBM_VIEW_ALL;
		}
		else if( !strcasecmp(val,MS_STR_ACTIVE) )
		{
			MS_INI_Menu_Type = LIBM_VIEW_ACTIVE;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}
	
	return ret;
}



int load_cfg(const char *file)
{
	int i;
	
	memset(&data,0,sizeof(ini_data));
	memset(list,0,sizeof(list));
	
	data.pair		= list;
	data.size		= sizeof(list);
	data.buf		= buf;
	data.bufLen		= sizeof(buf);
	
	for( i = 0 ; i < key_list_size ; i++ )
	{
		iniAddKey( &data ,setting[i].key ,setting[i].type ,setting[i].data ,NULL, setting[i].id);
	}
	
	logs("Load Config : %s\n", file );
	
	return iniLoad(file ,&data,lcb_main,NULL);
}



void save_cfg(const char *file)
{
	int i;
	
	memset(&data,0,sizeof(ini_data));
	memset(list,0,sizeof(list));
	
	data.pair		= list;
	data.size		= sizeof(list);
	data.buf		= buf;
	data.bufLen		= sizeof(buf);
	
	MS_INI_Plg_Use = Is_Load_Plugin();
	
	for( i = 0 ; i < key_list_size ; i++ )
	{
		//読込み専用の設定は上書きしない
		if( !setting[i].ronly )
		{
			iniAddKey( &data ,setting[i].key ,setting[i].type ,NULL ,setting[i].data,setting[i].id );
		}
	}
	
	logs("Save Config : %s\n", file );
	
	iniSave(file ,&data,scb_main,NULL);
}

