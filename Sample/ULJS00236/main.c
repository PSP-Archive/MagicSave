#include "main.h"

// MagicSave側の機能を利用するためにインクルード
// 使える関数、使い方は includeフォルダ内にある
// ヘッダーファイル( MagicSave.h ) を参照
#include "MagicSave.h"

PSP_MODULE_INFO("MS_Plugin",PSP_MODULE_KERNEL, 1, 1);


/************************************************************
 * MagicSave用のプラグインを作る際の注意点                  *
 * どこかに不備があればプラグインは機能しない               *
 *                                                          *
 * グローバル変数 Plugin_Func  Load、Save  は               *
 * MS_GetFuncInfo_XXXX でそれぞれの情報を                   *
 * MagicSave側へ送る必要があるため                          *
 * 変数名を変更してはいけない                               *
 * ※ある程度分かってる方は自由に変更してOK                 *
 *                                                          *
 * プラグインの具体的な作成方法は下記参照 、手順4まで       *
 ************************************************************/



/**********************************************
 * 手順1、Makefileの'TARGET'へゲームIDを設定  *
 **********************************************/

/**********************************************
 * 手順2、データを処理する関数の定義          *
 **********************************************/

// MagicSaveがセーブファイルをロード・セーブした際に呼ばれる
// あまり複雑な処理はしないこと、場合によってはフリーズするので注意


// void 関数名(const char *FileName, void *Data, SceSize Size) 
// FileName	= 処理（セーブ、ロード）中のファイル名
// Data		= 処理（セーブ、ロード）中のデータ
// Size		= 処理（セーブ、ロード）中のファイルサイズ


// ※Mode2 "Raw"形式で保存した非暗号化状態のファイルは
// 　ファイル先頭に 0x10 Byte 無関係なデータが追加される
// 　つまり本来のデータはファイル上だと 0x10～ だが
// 　データを処理する関数を定義する場合に意識(Data+0x10)する必要はない
// 　（Dataは常に+0x10されたアドレスが送られてくる）
// 　また、ファイルからその余分なデータを削除してもいけない


// 関数の定義例 : ゲーム NARUTO 疾風伝



//ロード時にプレイヤー名を'NARUTO'へ変える
void Name_Change(const char *FileName, void *Data, SceSize Size)
{
	char *new_name	= "NARUTO";
	char *name		= (char*)(Data + 0x10);//Data+0x10 =>プレイヤー名のアドレス

	memcpy(name,new_name,strlen(new_name));
	name[strlen(new_name)] = '\0';
}





// セーブされた時に、NP関係（現在値、トータル）を全てMAXにする
void NP_MAX(const char *FileName, void *Data, SceSize Size)
{
	int *np = (int*)(Data + 0xA8);

	//NP 現在値を最大に
	*np = 0x0F423F;

	np++;

	//NP トータル値を最大に
	*np = 0x0F423F;
}




//ロード時にプレイヤー名をNARUTOへ、NP関係をMAXに
void Change_Name_NP(const char *FileName, void *Data, SceSize Size)
{
	//既に定義されている関数を
	//他から利用して組み合わせて使う

	Name_Change(FileName, Data, Size);
	NP_MAX(FileName,Data,Size);
}




/**********************************************
 * 手順3、定義した関数をテーブルへ登録する    *
 **********************************************/

// グローバル変数 Plugin_Table_Func Load、Save へ
//
// { "MagicSaveに表示される名前" , 登録する関数 } ,
//
// という風にして登録していく
//
// ※Load、Save 共に、最大で256個まで登録出来ます
// 　ヘッダーの MS_PLUGIN_TABLE_MAX の値を変えても
// 　MagicSave側で制限されているからそれ以上は登録不可



// MagicSaveのメニュー上で
// "Status" -> "Plugin"を有効にした時、
// 'Load' へ表示される関数リスト

static Plugin_Func Load[] =
{
	{"Name to \"NARUTO\""		,Name_Change	}, //先ほど定義した関数を登録
	{"Name Change + NP MAX"		,Change_Name_NP	}, //先ほど定義した関数を登録
};


// MagicSaveのメニュー上で
// "Status" -> "Plugin"を有効にした時、
// 'Save' へ表示される関数リスト

static Plugin_Func Save[] =
{
	{"NP Max"					,NP_MAX			},	//先ほど定義した関数を登録
};





/**********************************************
 * 手順4、PSPSDKを使いプラグインをコンパイル  *
 **********************************************/

// エラーなくコンパイルが終わったら
// MagicSave/Plugins/ フォルダへコピー
// 
// MagicSaveを有効にして対象のゲームを起動
// メニューから "Status" -> "Plugin"を有効にして
// 定義した関数を選び動作確認

// ログを有効にすればある程度のデバッグが可能です


// 正常にプラグインが起動出来て、指定した関数が呼ばれると
// Call Plugin : Mode = 状態(Load/Save) FuncName = 関数名 [ 対象ファイル ]


// 何らかのエラーでプラグインが起動出来ない場合は
// Err,Load/Save Func Table Size 0	=> Load/Save 共に関数定義数が0
// Err,Link(Exports)				=> プラグインから関数のエクスポートに失敗している


// というログが記録されます

// 関数がちゃんと呼ばれているのに動作がおかしい場合は
// MS_Logを活用してデバッグして下さい





/*************************************************
 * ここからは必須コード、変更・削除しないように  *
 *************************************************/

bool MS_GetFuncInfo_Load( Plugin_Info *Info )
{
	Info->Func		= Load;
	Info->TableSize	= sizeof(Load) / sizeof(Plugin_Func);

	return true;
}

bool MS_GetFuncInfo_Save( Plugin_Info *Info )
{
	Info->Func		= Save;
	Info->TableSize	= sizeof(Save) / sizeof(Plugin_Func);
	
	return true;
}


int module_start(SceSize args, void *argp)
{
    return 0;
}

int module_stop(SceSize args, void *argp)
{
    return 0;
}


/*************************************************
 * ここまでは必須コード、変更・削除しないように  *
 *************************************************/


