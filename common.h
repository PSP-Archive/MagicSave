#ifndef MS_COMMON_H
#define MS_COMMON_H

#include <psptypes.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <pspctrl.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <libmenu.h>

#include "prx.h"
#include "log.h"

enum
{
	ERR_FILE_SUCCESS = 0,
	ERR_FILE_WRITE,
	ERR_FILE_OPEN,
	ERR_FILE_MS_FREE,
	ERR_FILE_MS_GET,
};


enum
{
	SFORMAT_AUTO = 0,
	SFORMAT_RAW,
	SFORMAT_NORMAL,
};

enum
{
	RID_SFO1 = 0,
	RID_SFO2,
	RID_SFO3,
};

enum
{
	RID_DEC1 = 0,
	RID_DEC2,
	RID_DEC3,
};




typedef struct
{
	unsigned long clusters_max;
	unsigned long clusters_free;
	int sector_max;
	int sector_size;
	int sector_count;
}sDev_Disk_Info;




#define	MS_STR_TITLE			"MagicSave"


#define	MS_STR_VER				"3.63"

#define	MS_STR_SUCCESS			"Success"
#define	MS_STR_FAILED			"Failed"


#define MS_STR_RAW				"Raw"
#define	MS_STR_NORMAL			"Normal"
#define	MS_STR_AUTO				"Auto"

#define	MS_STR_TRUE				"True"
#define	MS_STR_FALSE			"False"

#define	MS_STR_ON				"On"
#define	MS_STR_OFF				"Off"
#define	MS_STR_ALL				"All"
#define	MS_STR_ACTIVE			"Active"


#define	MS_STR_TYPE1			"Type1"
#define	MS_STR_TYPE2			"Type2"
#define	MS_STR_TYPE3			"Type3"

#define	MS_DRIVE_DEF			"ms"
#define	MS_DRIVE_PGO			"ef"

#define	MS_PATH_ROOT			"ms0:/MagicSave"
#define	MS_PATH_LOG				"Log"
#define	MS_PATH_SAVE			"Save"
#define	MS_PATH_CONFIG			"Config"
#define	MS_PATH_GAMEKEY			"Gamekey"
#define	MS_PATH_PLUGINS			"Plugins"


#define	MS_INIT_STR_MAX			256

#define	MS_INIT_SFORMAT			0

#define	MS_INIT_RID				false
#define	MS_INIT_RID_SFO			RID_SFO1		//0x21
#define	MS_INIT_RID_DEC			RID_DEC3		//5

#define	MS_INIT_WMS				false
#define	MS_INIT_RMS				false

#define	MS_INIT_GAMEKEY			false

#define	MS_INIT_LOG				false


#define	MS_INIT_MDELAY			1000
#define	MS_INIT_MTYPE			LIBM_VIEW_ACTIVE
#define	MS_INIT_MSTH			false
#define	MS_INIT_MDSTEP			50
#define	MS_INIT_MDMAX			2500

#define	MS_INIT_POSX			96
#define	MS_INIT_POSY			24

#define	MS_INIT_MENU			true



#define	PAD_KEY_SELECT			"SELECT"
#define	PAD_KEY_START			"START"
#define	PAD_KEY_UP				"Up"
#define	PAD_KEY_RIGHT			"Right"
#define	PAD_KEY_DOWN			"Down"
#define	PAD_KEY_LEFT			"Left"
#define	PAD_KEY_LT				"LTrigger"
#define	PAD_KEY_RT				"RTrigger"
#define	PAD_KEY_TRIANGLE		"Triangle"
#define	PAD_KEY_CIRCLE			"Circle"
#define	PAD_KEY_CROSS			"Cross"
#define	PAD_KEY_SQUARE			"Square"
#define	PAD_KEY_HOME			"HOME"
#define	PAD_KEY_HOLD			"HOLD"
#define	PAD_KEY_NOTE			"NOTE"
#define	PAD_KEY_SCREEN			"SCREEN"
#define	PAD_KEY_VUP				"VolUp"
#define	PAD_KEY_VDOWN			"VolDown"
#define	PAD_KEY_WLAN			"WLAN"
#define	PAD_KEY_REMOTE			"REMOTE"
#define	PAD_KEY_DISC			"DISC"
#define	PAD_KEY_MS				"MS"


#define	INI_KEY_FORMAT			"Save_Format"
#define	INI_KEY_LOG				"Log"
#define	INI_KEY_DKEY			"Dump_GameKey"
#define	INI_KEY_RID				"RID"
#define	INI_KEY_RIDS			"RID_SFO"
#define	INI_KEY_RIDD			"RID_DEC"

#define	INI_KEY_BKUPL			"BackUp_Load"
#define	INI_KEY_BKUPS			"BackUp_Save"

#define	INI_KEY_MENU			"Menu"
#define	INI_KEY_DELAY			"Menu_Delay"
#define	INI_KEY_MENU_T			"Menu_Type"
#define	INI_KEY_MENU_STH		"Menu_Stop_Other"
#define	INI_KEY_MENU_PX			"Menu_Pos_X"
#define	INI_KEY_MENU_PY			"Menu_Pos_Y"
#define	INI_KEY_HOTK_SHOW		"Menu_Key_Show"
#define	INI_KEY_HOTK_BACK		"Menu_Key_Back"
#define	INI_KEY_HOTK_SEL		"Menu_Key_Select"
#define	INI_KEY_HOTK_UP			"Menu_Key_Up"
#define	INI_KEY_HOTK_DOWN		"Menu_Key_Down"
#define	INI_KEY_HOTK_LEFT		"Menu_Key_Left"
#define	INI_KEY_HOTK_RIGHT		"Menu_Key_Right"


#define	INI_KEY_PLG_USE			"PLG_Use"
#define	INI_KEY_PLG_LFNC		"PLG_LFunc"
#define	INI_KEY_PLG_SFNC		"PLG_SFunc"

#define	INI_KEY_HOOK			"Hook"



#if LNGJPN

#define	MENU_SAVEFILE			"セーブデータ"
#define	MENU_SType_Info			"セーブ フォーマット"
#define	MENU_SType_Auto			"自動"
#define	MENU_SType_Raw			"暗号化なし"
#define	MENU_SType_Normal		"暗号化あり"

#define	MENU_Dump_Gamekey		"ゲームキーをダンプする"
#define	MENU_RID				"破損データ読込"
#define	MENU_Plugin				"プラグイン"
#define	MENU_Plugin_Info		"(セーブファイルへパッチ処理)"
#define	MENU_PLOAD				"ロード:"
#define	MENU_PSAVE				"セーブ:"
#define	MENU_BackUp				"バックアップ"
#define	MENU_Log				"ログ"
#define	MENU_LMS				"MagicSaveから読み込む"
#define	MENU_SMS				"MagicSaveへ　書き込む"
#define	MENU_FRS				"読み込みサイズエラーを修正"
#define	MENU_Mode2_SType		"保存形式を指定"
#define	MENU_MenuOption			"オプション"
#define	MENU_Delay				"ディレイ:"
#define	MENU_View				"　\x95\x5C\x8E\xA6　:"
#define	MENU_Priority			"優先度　:"
#define MENU_TH_Stop			"他スレッドを停止させる"

#define	MENU_Enable				"有効にする"

#define	MENU_Off				"オフ"

#define	MENU_All				"全て"
#define	MENU_Active				"アクティブ"



#define	MENU_Game				"ゲームID "
#define MENU_Info				"ステータス"

#define	MENU_Type1				"タイプ1"
#define	MENU_Type2				"タイプ2"
#define	MENU_Type3				"タイプ3"

#define	MENU_RID_SFO			"SFOファイル:"
#define	MENU_RID_DEC			"復号化 方法 :"

#define	MENU_HOOK				"フック:"

#define	MENU_S_SYSTEM			"システム"
#define	MENU_S_MENU				"メニュー"

#define MENU_HOOK_E				"有効"
#define MENU_HOOK_D				"無効"

#else


#define	MENU_SAVEFILE			"Save Data"
#define	MENU_SType_Info			"Save Format"
#define	MENU_SType_Auto			MS_STR_AUTO
#define	MENU_SType_Raw			MS_STR_RAW
#define	MENU_SType_Normal		MS_STR_NORMAL

#define	MENU_Dump_Gamekey		"Dump Gamekey"
#define	MENU_RID				"Read Invalid Data"
#define	MENU_Plugin				"Plugin"
#define	MENU_Plugin_Info		"(Patch to \"Save File\")"
#define	MENU_PLOAD				"Load:"
#define	MENU_PSAVE				"Save:"
#define	MENU_BackUp				"BackUp"
#define	MENU_Log				"Log"
#define	MENU_LMS				"Load from MagicSave"
#define	MENU_SMS				"Save  to  MagicSave"
#define	MENU_FRS				"Fix Read Size"
#define	MENU_Mode2_SType		"Select Write Type"

#define	MENU_MenuOption			"Option"
#define	MENU_Delay				"Delay:"
#define	MENU_View				"View :"
#define	MENU_Priority			"Priority:"
#define MENU_TH_Stop			"Stop Other Threads"

#define	MENU_Enable				"Enable"

#define	MENU_Off				MS_STR_OFF

#define	MENU_All				MS_STR_ALL
#define	MENU_Active				MS_STR_ACTIVE


#define	MENU_Game				"GameID"
#define MENU_Info				"Status"

#define	MENU_Type1				MS_STR_TYPE1
#define	MENU_Type2				MS_STR_TYPE2
#define	MENU_Type3				MS_STR_TYPE3

#define	MENU_RID_SFO			"SFO File:"
#define	MENU_RID_DEC			"Decrypt :"

#define	MENU_HOOK				"Hook:"

#define	MENU_S_SYSTEM			"Syetem"
#define	MENU_S_MENU				"Menu"

#define MENU_HOOK_E				"Enable"
#define MENU_HOOK_D				"Disable"

#endif




extern char game_id[11];
extern MenuContext Context;
extern int	MS_INI_SFormat;
extern bool	MS_INI_DumpGameKey;
extern int	MS_INI_Menu_Delay;
extern bool	MS_INI_Menu_Type;
extern bool	MS_INI_Menu_STH;
extern bool	MS_INI_WMS;
extern bool	MS_INI_RMS;
extern bool	MS_INI_RID;
extern int	MS_INI_RID_SFO;
extern int	MS_INI_RID_DEC;
extern int	MS_INI_Menu_Pos_X;
extern int	MS_INI_Menu_Pos_Y;
extern bool	MS_INI_Menu;
extern bool MS_INI_Plg_Use;
extern bool MS_INI_Hook;

extern char path_prx[MS_INIT_STR_MAX];


void CreateDir(char* path);

void strutilRemoveChar( char *__restrict__ str, const char *__restrict__ search );

int WriteFile(const char *path,void *data,SceSize size);
bool ReadFile(const char *path ,void *data ,SceSize size);



#endif

