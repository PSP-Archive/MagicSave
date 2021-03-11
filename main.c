#include "common.h"

#include "main.h"
#include "menu.h"
#include "func.h"
#include "cfg.h"
#include "plugin.h"

#include <pspsysevent.h>


PSP_MODULE_INFO("MagicSave", PSP_MODULE_KERNEL, 3, 6);




char path_prx[MS_INIT_STR_MAX];
char game_id[11];


int Callback_Suspend(int ev_id, char *ev_name, void *param, int *result)
{
	// 返り値に 0 ではなく -1 を返すと、スリープを無効化できる
	
	// スリープ時
	if( ev_id == PSP_SYSEVENT_SUSPEND_QUERY && libmExecTCmd(LIBM_TCMD_GET) == LIBM_TSUSPEND )
	{
		libmExecTCmd( LIBM_TCMD_RESUME );
		Context.IsOpen = false;
	}
	
	return 0;
}


int threadMain(SceSize args, void *argp)
{
    bool flag = false;
    char path[MS_INIT_STR_MAX];
    
	PspSysEventHandler events;
	
	SceCtrlData pad;
	MenuParams* Params;
	char mstr[256];
	libm_vram_info vram_tmp;
	int ret;
	
	//メニュー操作用の各Pad設定
	Context.HotKey.Show		= PSP_CTRL_VOLUP | PSP_CTRL_VOLDOWN;
	Context.HotKey.Back		= PSP_CTRL_TRIANGLE;
	Context.HotKey.Select	= PSP_CTRL_CROSS;
	Context.HotKey.Up		= PSP_CTRL_UP;
	Context.HotKey.Down		= PSP_CTRL_DOWN;
	Context.HotKey.Left		= PSP_CTRL_LEFT;
	Context.HotKey.Right	= PSP_CTRL_RIGHT;
	
	PTBL_Load.NList[0]		= MENU_Off;
	PTBL_Save.NList[0]		= MENU_Off;
	
	
	while( 1 )
	{
			if( ReadFile("disc0:/UMD_DATA.BIN" ,game_id, 10) )
			{
				ret = Check_Module_Status();
				
				//Syscall Dump 失敗
				if( ret != 0 ) break;
				
				strutilRemoveChar( game_id, "-" );
				game_id[9] = '\0';
				
				//標準ディレクトリのチェック(iniを読み込むためにも)
				CreateDir(MS_PATH_ROOT "/" MS_PATH_SAVE		"/" );
				CreateDir(MS_PATH_ROOT "/" MS_PATH_CONFIG	"/" );
				CreateDir(MS_PATH_ROOT "/" MS_PATH_GAMEKEY	"/" );
				CreateDir(MS_PATH_ROOT "/" MS_PATH_PLUGINS	"/" );
				
				libmExecTCmd(LIBM_TCMD_DUMP);
				
				load_cfg(MS_PATH_ROOT "/" MS_PATH_CONFIG "/default.ini" );
				
				snprintf(path,MS_INIT_STR_MAX,MS_PATH_ROOT "/" MS_PATH_CONFIG "/%s.ini",game_id);
				load_cfg(path);
				
				if( MS_INI_Hook ) apiHook(false);
				
				events.size			= 0x40;
				events.name			= "MS_Suspend";
				events.type_mask	= 0x0000FF00;
				events.handler		= Callback_Suspend;
				
				sceKernelRegisterSysEventHandler(&events);
				
				break;
			}
		
		sceKernelDelayThread(5000);
	}
	
	logs("---- " MS_STR_TITLE " Ver " MS_STR_VER " ----\n");
	logs("Initialize : %d\n",( ret ) );
	
	//Hook失敗
	if( ret != 0 )	return sceKernelExitDeleteThread(0);
	
	//プラグイン用のパスを作る
	snprintf(path_prx,MS_INIT_STR_MAX,MS_PATH_ROOT "/" MS_PATH_PLUGINS "/%s.prx",game_id);
	
	//メニューの有効・無効 状態
	logs("Create Menu : %s\n", MS_INI_Menu ? MS_STR_SUCCESS : MS_STR_FAILED );
	
	if( !MS_INI_Menu )		return 0;
	if(	MS_INI_Plg_Use )	MS_Plugin_Load();
	
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    
	MenuCreate();
	
	while( 1 )
	{
	   	sceCtrlPeekBufferPositive(&pad, 1);
		
		Params = libmGetHandle(&Context, &pad);
		
		if( !Params ) continue;
		
		if (Params->Action != Menu_None)
		{
			if(Params->Action == Menu_Show )
			{
				if( MS_INI_Menu_STH )
				{
					libmGetCurVInfo(&vram_tmp);
					
					//他スレッドをサスペンド
					while(!libmExecTCmd( LIBM_TCMD_SUSPEND ));
					
					while(!libmInitBuffers(false,PSP_DISPLAY_SETBUF_NEXTFRAME));
				}
			}
			else if(Params->Action == Menu_Close)
			{
				if( libmExecTCmd(LIBM_TCMD_GET) == LIBM_TSUSPEND )
				{
					sceDisplaySetFrameBuf( vram_tmp.buffer , vram_tmp.lineWidth , vram_tmp.format , PSP_DISPLAY_SETBUF_IMMEDIATE);
					sceDisplayWaitVblankStart();
					sceDisplayWaitVblankStart();
					
					//サスペンドしていた他スレッドを再開
					while(!libmExecTCmd( LIBM_TCMD_RESUME ));
				}
				
				if( flag )
				{
					snprintf(path,MS_INIT_STR_MAX,MS_PATH_ROOT "/" MS_PATH_CONFIG "/%s.ini",game_id);
					save_cfg(path);
				}
				
				flag = false;
			}
			
			
			if(Params->Item)
			{
				switch (Params->Item->actionID)
				{
					case SFormat_Change:
					{
						MS_INI_SFormat = Params->Item->Ctrl.SB.Index;
						
						flag = true;
						break;
					}
					
					case MenuType_Change:
					{
						MS_INI_Menu_Type		= ( Params->Item->Ctrl.SB.Index == LIBM_VIEW_ALL ? LIBM_VIEW_ALL : LIBM_VIEW_ACTIVE );
						Context.MenuInfo.Type	= MS_INI_Menu_Type;
						
						flag = true;
						break;
					}
					
					case GameKey_Change:
					{
						MS_INI_DumpGameKey = Params->Item->Ctrl.CB_Checked;
						
						flag = true;
						break;
					}
					
					case Delay_Change:
					{
						MS_INI_Menu_Delay = (int)Params->Item->Ctrl.UD.Now;
						
						flag = true;
						break;
					}
					
					case TH_Stop_Change:
					{
						MS_INI_Menu_STH	= Params->Item->Ctrl.CB_Checked;
						
						flag = true;
						break;
					}
					
					case PLG_RSel_Change:
					{
						PTBL_Load.Select = Params->Item->Ctrl.SB.Index;
						
						flag = true;
						break;
					}
					
					case PLG_WSel_Change:
					{
						PTBL_Save.Select = Params->Item->Ctrl.SB.Index;
						
						flag = true;
						break;
					}
					
					case PLG_Use_Change:
					{
						if( Params->Item->Ctrl.CB_Checked )
						{
							MS_Plugin_Load();
						}
						else
						{
							MS_Plugin_Unload();
						}
						
						flag = true;
						break;
					}
					
					case BKUP_WMS_Change:
					{
						MS_INI_WMS = Params->Item->Ctrl.CB_Checked;
						
						flag = true;
						break;
					}
					
					case BKUP_RMS_Change:
					{
						MS_INI_RMS = Params->Item->Ctrl.CB_Checked;
						
						flag = true;
						break;
					}
					
					
					case RID_Change:
					{
						MS_INI_RID	= Params->Item->Ctrl.CB_Checked;
						break;
					}
					
					
					
					case RID_SFO_Change:
					{
						MS_INI_RID_SFO = Params->Item->Ctrl.SB.Index;
						break;
					}
					
					case RID_DEC_Change:
					{
						MS_INI_RID_DEC = Params->Item->Ctrl.SB.Index;
						break;
					}
					
					case Hook_Change:
					{
						apiHook(!Params->Item->Ctrl.SB.Index);
						break;
					}
					
					default:
						break;
				}
			}
			
			Sync_Item();
		}
		
		if( (pad.Buttons & PSP_CTRL_HOME) == PSP_CTRL_HOME  )
		{
			if( libmExecTCmd(LIBM_TCMD_GET) == LIBM_TSUSPEND )
			{
				while(!libmExecTCmd( LIBM_TCMD_RESUME ));
			}
			
			Context.IsOpen = false;
		}
		
		if( Context.IsOpen )
		{
			if( libmExecTCmd(LIBM_TCMD_GET) == LIBM_TSUSPEND )
			{
				libmClearBuffers();
				libmRender(&Context,MS_INI_Menu_Pos_X,MS_INI_Menu_Pos_Y,mstr,256);
				libmSwapBuffers();
			}
			else if( libmInitBuffers(false,PSP_DISPLAY_SETBUF_NEXTFRAME) )
			{
				libmRender(&Context,MS_INI_Menu_Pos_X,MS_INI_Menu_Pos_Y,mstr,256);
				sceDisplayWaitVblankStart();
			}
		}
		
		sceKernelDelayThread( MS_INI_Menu_Delay );
	}
	
	logs("--- Pugin End ---- [ Error ]\n");
	
	return sceKernelExitDeleteThread( 0 );
}

int module_start(SceSize args, void *argp)
{
    SceUID main_thid = sceKernelCreateThread("MagicSave_Main", threadMain, 16,0x1500, 0, NULL);
	
    if(main_thid >= 0) sceKernelStartThread(main_thid, args, argp);
	
    return 0;
}

int module_stop(SceSize args, void *argp)
{
    return 0;
}
