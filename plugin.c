#include "common.h"

#include "plugin.h"


// プラグイン関係
SceUID	MS_INI_PLG_ID_S				= -1;
PLG_TBL PTBL_Load,PTBL_Save;


SceUID MS_Plugin_Load()
{
	int i;
	
	// 既にロードされたら何もしない
	if( MS_INI_PLG_ID_S >= 0 ) return MS_INI_PLG_ID_S;
	
	#if _DEBUG >= 1
	const char *err	= MS_STR_SUCCESS;
	#endif
	
	//プラグインをロードしてみる
	MS_INI_PLG_ID_S = pspSdkLoadStartModule (path_prx,PSP_MEMORY_PARTITION_KERNEL);
		
	//プラグインがロード成功したかどうか
	if( MS_INI_PLG_ID_S >= 0 )
	{
		int Is_Link_Load = -1;
		int Is_Link_Save = -1;
			
		//変数の初期化
		memset(&(PTBL_Load.PLG),0,sizeof(Plugin_Info));
		memset(&(PTBL_Save.PLG),0,sizeof(Plugin_Info));
		
		
		//それぞれの情報取得
		//プラグインへリンク成功しているかどうか
		Is_Link_Load = MS_GetFuncInfo_Load( &(PTBL_Load.PLG) );
		Is_Link_Save = MS_GetFuncInfo_Save( &(PTBL_Save.PLG) );
		
		
		//テーブルサイズが両方とも0
		//このままローﾄﾞしていても意味が無い
		if( PTBL_Load.PLG.TableSize == 0 && PTBL_Save.PLG.TableSize == 0 )
		{
			//失敗した
			
			#if _DEBUG >= 1
			if( (Is_Link_Load & 0x8002013A) == 0x8002013A || (Is_Link_Save & 0x8002013A) == 0x8002013A )
			{
				err = "Err,Link(Exports)";
			}
			else
			{
				err = "Err,Load/Save Func Table Size 0";
			}
			#endif
			
			MS_Plugin_Unload();
		}
		else
		{
			//成功した
			
			//テーブルサイズが範囲外、制限する
			if( PTBL_Load.PLG.TableSize > MS_PLUGIN_TABLE_MAX ) PTBL_Load.PLG.TableSize	= MS_PLUGIN_TABLE_MAX;
			if( PTBL_Save.PLG.TableSize > MS_PLUGIN_TABLE_MAX ) PTBL_Save.PLG.TableSize	= MS_PLUGIN_TABLE_MAX;
				
				
			//Load 定義されている関数分、表示用テーブルへ文字列追加
			for( i = 1 ; i <= PTBL_Load.PLG.TableSize ; i++ )
			{
				PTBL_Load.NList[i] = PTBL_Load.PLG.Func[i-1].Name;
			}
			
			//Save 定義されている関数分、表示用テーブルへ文字列追加
			for( i = 1 ; i <= PTBL_Save.PLG.TableSize ; i++ )
			{
				PTBL_Save.NList[i] = PTBL_Save.PLG.Func[i-1].Name;
			}
			
			//Load、Save 表示用テーブルの最終へNULL追加
			//追加位置はそれぞれのテーブルサイズ+1
			//(最初に本来ないOffが追加されているため)
			PTBL_Load.NList[PTBL_Load.PLG.TableSize+1]	= NULL;
			PTBL_Save.NList[PTBL_Save.PLG.TableSize+1]	= NULL;
			
			//設定ファイルが存在していたら、Read/WriteのSelは指定値になっている
			//プラグインから得た各テーブルサイズを超えていたら修正
			if( PTBL_Load.Select > PTBL_Load.PLG.TableSize )	PTBL_Load.Select	= 0;
			if( PTBL_Save.Select > PTBL_Save.PLG.TableSize )	PTBL_Save.Select	= 0;
		}
	}
	else
	{
		#if _DEBUG >= 1
		// プラグインのロードに失敗
		// mod < 0 になっている
		err = MS_STR_FAILED;
		#endif
	}
	
	logs("Load Plugin : %s [ Ret:%s , ErrCode:0x%08X ]\n",path_prx,err,MS_INI_PLG_ID_S);
	
	return MS_INI_PLG_ID_S;
}


SceUID MS_Plugin_Unload()
{
	// まだロードされいなければ何もしない
	if( MS_INI_PLG_ID_S < 0 ) return MS_INI_PLG_ID_S;
	
	int ret = sceKernelStopModule( MS_INI_PLG_ID_S, 0, NULL, NULL, NULL );
	
	if( ret >= 0 )
	{
		sceKernelDelayThread( 1000000 );
		ret = sceKernelUnloadModule( MS_INI_PLG_ID_S );
		
		if( ret >= 0 )
		{
			memset(&(PTBL_Load.PLG)	,0,sizeof(Plugin_Info));
			memset(&(PTBL_Save.PLG)	,0,sizeof(Plugin_Info));
			
			MS_INI_PLG_ID_S = -1;
		}
	}
	
	logs("UnLoad Plugin : %s\n",MS_INI_PLG_ID_S == -1 ? MS_STR_SUCCESS : MS_STR_FAILED );
	
	return MS_INI_PLG_ID_S;
}


void Call_PLG_Func(int type ,const char *msg,const char *fileName,void *data,SceSize len)
{
	PLG_TBL *TBL = ( type == PLG_Func_Load ? &PTBL_Load : &PTBL_Save);
	
	if( Is_Load_Plugin() && TBL->Select > 0 )
	{
		logs("Call Plugin : Mode = %s , FuncName = %s [ %s ]\n",type == PLG_Func_Load ? "Load" : "Save",TBL->PLG.Func[TBL->Select-1].Name,msg);
		TBL->PLG.Func[TBL->Select-1].Patch(fileName,data,len);
	}
}

int	MS_Mode_Status()
{
	return MS_INI_SFormat;
}


