#include "common.h"

#include "func.h"
#include "hook.h"
#include "psf.h"
#include "plugin.h"

/*


sceChnnlsv_21BE78B4	->	sceChnnlsv_21BE78B4
sceSdSetMember		->	sceChnnlsv_850A7FA1
sceSdCreateList		->	sceChnnlsv_ABFDFC8B
sceSdGetLastIndex	->	sceChnnlsv_C4C494F8
sceSdSetIndex		->	sceChnnlsv_E7833020
sceSdRemoveValue	->	sceChnnlsv_F21A1FCA

[�Í���] ���Z�[�u�f�[�^
1. sceSdCreateList
2. sceSdSetIndex
3. sceSdRemoveValue
4. sceSdSetMember
5. sceSdRemoveValue
6. sceChnnlsv_21BE78B4
7. sceSdGetLastIndex

[������] ���Z�[�u�f�[�^
1. sceSdSetIndex
2. sceSdCreateList
3. sceSdRemoveValue
4. sceSdRemoveValue
5. sceSdSetMember
6. sceChnnlsv_21BE78B4
7. sceSdGetLastIndex

[�j���m�F] ��PARAM.SFO ���̂��̂̃n�b�V��
1. sceSdSetIndex
2. sceSdRemoveValue
3. sceSdGetLastIndex

�֐��̎��s�����𒲂ׂ邱�Ƃ�
���݂̏����󋵂�������

*/

#define	MS_GAME_NAME_MAX					(13+20)
#define	SFO_SIZE_HASH						16

#define	CLEAR_HASH(hash)					memset(hash,0,SFO_SIZE_HASH)



static SceUtilitySavedataParam* currentParams	= NULL;
static pspChnnlsvContext1 *sceSdSetIndex_ctx	= NULL;
static int sceSdSetIndex_mode;

static bool	Is_Load_SceChnnlsv_Prx				= false;
static bool	Is_Load_SceUtility_Prx				= false;
static bool Is_Raw;
static int  CurMode,LastFunc,LastMode;

//SFO�^�C�v�̃��X�g
static u8 type_list[3] = { 0x21 ,0x41 ,0x01	};
//Decrypt�^�C�v�̃��X�g
static u8 mode_list[3] = { 0x01 ,0x03 ,0x05	};


static int (*_sceIoRead)(SceUID , void *, SceSize ) 						= NULL;
static int (*_sceSdSetMember)(pspChnnlsvContext2 *, u8*, int)				= NULL;
static int (*_sceSdCreateList)(pspChnnlsvContext2 *, int, int, u8 *, u8 *)	= NULL;
static int (*_sceSdGetLastIndex)(pspChnnlsvContext1 *, u8 *, u8 *)			= NULL;
static int (*_sceSdSetIndex)(pspChnnlsvContext1 *, int ) 					= NULL;
static int (*_sceChnnlsv21BE78B4) (pspChnnlsvContext2 *) 					= NULL;
static int (*_sceSdRemoveValue)(pspChnnlsvContext1 *, u8 *, int ) 			= NULL;
static int (*_sceUtilitySavedataGetStatus)()								= NULL;
static int (*_sceUtilitySavedataShutdownStart)()							= NULL;
static int (*_sceUtilitySavedataInitStart)(SceUtilitySavedataParam* )		= NULL;

int ms_sceIoRead(SceUID fd, void *data, SceSize size);
int ms_sceSdSetMember(pspChnnlsvContext2 *ctx, unsigned char *data, int len);
int ms_sceSdCreateList(pspChnnlsvContext2 *ctx, int mode1, int mode2, unsigned char *hashkey, unsigned char *cipherkey);
int ms_sceSdGetLastIndex(pspChnnlsvContext1 *ctx, unsigned char *hash, unsigned char *cryptkey);
int ms_sceSdSetIndex(pspChnnlsvContext1 *ctx, int mode);
int ms_sceChnnlsv21BE78B4 (pspChnnlsvContext2 *ctx);
int ms_sceSdRemoveValue(pspChnnlsvContext1 *ctx, unsigned char *data, int len);
int ms_sceUtilitySavedataInitStart(SceUtilitySavedataParam* params);
int ms_sceUtilitySavedataShutdownStart(void);
int ms_sceUtilitySavedataGetStatus(void);


enum
{
	FNC_sceChnnlsv_21BE78B4,
	FNC_sceSdSetMember,
	FNC_sceSdCreateList,
	FNC_sceSdGetLastIndex,
	FNC_sceSdSetIndex,
	FNC_sceSdRemoveValue,
	FNC_sceIoRead,
};



enum save_type
{
	data_none = 0,
	data_load,
	data_save,
};

typedef struct
{
	const char *mod;
	const char *lib;
	u32 nid;
	void **forg;
	void *fnew;
	
	struct
	{
		void *addr;
		u32	 data;
	}bkup;
	
} hookItem;

static hookItem hooklist[] = {
{	"sceIOFileManager"	,"IoFileMgrForKernel"	,0x6A638D83	,(void**)&_sceIoRead						,ms_sceIoRead						},
{	"sceChnnlsv"		,"sceChnnlsv"			,0xE7833020	,(void**)&_sceSdSetIndex					,ms_sceSdSetIndex					},
{	"sceChnnlsv"		,"sceChnnlsv"			,0xF21A1FCA	,(void**)&_sceSdRemoveValue					,ms_sceSdRemoveValue				},
{	"sceChnnlsv"		,"sceChnnlsv"			,0xC4C494F8	,(void**)&_sceSdGetLastIndex				,ms_sceSdGetLastIndex				},
{	"sceChnnlsv"		,"sceChnnlsv"			,0xABFDFC8B	,(void**)&_sceSdCreateList					,ms_sceSdCreateList					},
{	"sceChnnlsv"		,"sceChnnlsv"			,0x850A7FA1	,(void**)&_sceSdSetMember					,ms_sceSdSetMember					},
{	"sceChnnlsv"		,"sceChnnlsv"			,0x21BE78B4	,(void**)&_sceChnnlsv21BE78B4				,ms_sceChnnlsv21BE78B4				},
{	"sceUtility_Driver"	,"sceUtility"			,0x50C4CD57	,(void**)&_sceUtilitySavedataInitStart		,ms_sceUtilitySavedataInitStart		},
{	"sceUtility_Driver"	,"sceUtility"			,0x9790B33C	,(void**)&_sceUtilitySavedataShutdownStart	,ms_sceUtilitySavedataShutdownStart	},
{	"sceUtility_Driver"	,"sceUtility"			,0x8874DBE0	,(void**)&_sceUtilitySavedataGetStatus		,ms_sceUtilitySavedataGetStatus		},
};


bool hook_status = false;

void apiHook(bool unhook)
{
	int table_size = sizeof(hooklist) / sizeof(hookItem);
	
	int i;
	void *func;
	
	if( hook_status == !unhook ) return;
	
	for( i= 0;i < table_size ; i++)
	{
		if( unhook )
		{
			//���̃f�[�^
			func = (void*)hooklist[i].bkup.data;
		}
		else
		{
			//�㏑������֐�
			func = hooklist[i].fnew;
		}
		
		Hook2Syscall(hooklist[i].bkup.addr,func);
	}
	
	hook_status = !unhook;
	
	logs("status = %d\n",hook_status);
}

int Check_Module_Status()
{
	int table_size = sizeof(hooklist) / sizeof(hookItem);
	int i,cnt =0;
	
	if( !Is_Load_SceUtility_Prx )
	{
		// sceUtility_Driver �̃`�F�b�N
		if( sceKernelFindModuleByName("sceUtility_Driver") == NULL )
		{
			Is_Load_SceUtility_Prx = (pspSdkLoadStartModule ("flash0:/kd/utility.prx",PSP_MEMORY_PARTITION_KERNEL) >= 0 ? true : false );
		}
		else
		{
			// ���[�h����Ă���
			Is_Load_SceUtility_Prx = true;
		}
	}
	
	if( !Is_Load_SceChnnlsv_Prx )
	{
		// sceChnnlsv �̃`�F�b�N
		if( sceKernelFindModuleByName("sceChnnlsv") == NULL )
		{
			Is_Load_SceChnnlsv_Prx = (pspSdkLoadStartModule ("flash0:/kd/chnnlsv.prx",PSP_MEMORY_PARTITION_KERNEL) >= 0 ? true : false );
		}
		else
		{
			//���[�h����Ă���
			Is_Load_SceChnnlsv_Prx = true;
		}
	}
	
	if( !Is_Load_SceUtility_Prx || !Is_Load_SceChnnlsv_Prx ) return -1;
	
	//�Ώۃ��C�u��������֐���T����Syscall�e�[�u�����ł��̊֐��̎Q�ƃA�h���X������Ɍ���
	for( i= 0;i < table_size ; i++)
	{
		*(hooklist[i].forg) = (void**)Find2Func(hooklist[i].mod,hooklist[i].lib,hooklist[i].nid);
		
		if(*(hooklist[i].forg))
		{
			hooklist[i].bkup.data = (u32)*(hooklist[i].forg);
			hooklist[i].bkup.addr = Find2Syscall(*(hooklist[i].forg));
			
			cnt++;
			logs("bkup = %08X,%08X\n",hooklist[i].bkup.data,*(int*)(hooklist[i].bkup.addr));
		}
	}
		
	if( cnt != table_size )
	{
		logs("Syscall dump faild\n");
		return -2;
	}
	
	return 0;
}


/*
	�e�Z�[�u�t�H���_�ɂ���PARAM.SFO�t�@�C�����Ǎ��݂����Ɠ�����
	�e�j�����o�n�b�V�����ׂĂ𑀍삷��
*/
int ms_sceIoRead(SceUID fd, void *data, SceSize size)
{
	SFO_DATA psf;
	SFO_FILE *psf_file;
	int ret = _sceIoRead(fd, data, size);
	
	if( find_psf_key(data,ret,PSF_KEY_SAVE_FLIST,&psf) == 0 )
	{
		Is_Raw		= false;
		CurMode		= data_none;
		LastFunc	= data_none;
		LastMode	= data_none;
		
		int k1 = pspSdkSetK1(0);
		
		//�Ώۃt�@�C���̃n�b�V�����擾���ăt�@�C���̏�Ԃ��m�F�i�Í����A��Í����j
		//�����Ǎ��݂��L���Ȃ�Ώۃt�@�C���̃n�b�V�����[���N���A����
		
		if( currentParams && currentParams->fileName )
		{
			logs("SFO FILE TARGET: %s\n",currentParams->fileName );
			
			psf.curPos = psf.pos;
			
			while( (psf_file= find_psf_datafile(&psf)) )
			{
				if( strcmp( currentParams->fileName,psf_file->name ) == 0 )
				{
					int i,cnt = 0;
					
					for( i = 0 ; i < SFO_SIZE_HASH ; i++ )
					{
						if( psf_file->hash[i] == 0 ) cnt++;
					}
					
					if( cnt == SFO_SIZE_HASH ) Is_Raw = true;
					
					if( MS_INI_RID )
					{
						CLEAR_HASH(psf_file->hash);
						
						logs("CLEAR HASH [ SFO File = %s ]\n",psf_file->name);
					}
				}
			}
		}
		
		//�����Ǎ��݂��L���Ȃ� PARAMS.SFO �֘A�̃n�b�V�����[���N���A
		if( MS_INI_RID && find_psf_key(data, ret,PSF_KEY_SAVE_PARAMS, &psf) == 0 )
		{
			SFO_PARAMS *params = (SFO_PARAMS*)psf.pos;
			
			params->mode = type_list[MS_INI_RID_SFO];
			
			CLEAR_HASH(params->hash01);
			CLEAR_HASH(params->hash02);
			CLEAR_HASH(params->hash03);
			
			logs("CLEAR HASH [ SFO SAVEDATA_PARAMS ] mode = %02X\n",params->mode );
		}
		
		pspSdkSetK1(k1);
	}
	
	return ret;
}



/*
	���̊֐������s�����ƑΏۃf�[�^�ɑ΂���
	���[�h���� ������
	�Z�[�u���� �Í���
	�̏����������̂ŁA�f�[�^�̏�Ԃɉ����Đ��䂷��
*/

int ms_sceSdSetMember(pspChnnlsvContext2 *ctx, unsigned char *data, int len)
{
	int k1	= pspSdkSetK1(0);
	int ret	= 0;
	
	bool EnDecrypt = false;
	
	//�Z�[�u�A�Í��������܂���
	if( CurMode == data_save )
	{
		Call_PLG_Save("System File",currentParams->fileName,data,len);
	}
	
	
	if
	(
		( CurMode == data_load && !Is_Raw )
	|| 
		(	
			CurMode == data_save &&
			(
				( MS_INI_SFormat == SFORMAT_AUTO && !Is_Raw )
			||
				( MS_INI_SFormat == SFORMAT_NORMAL )
			)
		)
	)
	{
		//���[�h���͕������A�Z�[�u���͈Í��������s�����
		
		//�f�[�^�̏�ԁA�ݒ�ɉ����Đ��䂷��
		
		//���[�h(������)
		//�f�[�^���Í�������Ă���ꍇ�̂�
		
		//�Z�[�u(�Í���)
		//�t�H�[�}�b�g�Ŏ����w��A���X�f�[�^���Í�������Ă���
		//�t�H�[�}�b�g�ňÍ����w��
		
		ret = _sceSdSetMember(ctx, data, len);
		
		EnDecrypt = true;
	}
	
	//���[�h�A�������������
	if( CurMode == data_load )
	{
		Call_PLG_Load("System File",currentParams->fileName,data,len);
	}
	
	
	if( CurMode != data_none )
	{
		logs("%s File : %s [ System File (%s) ]\n",( CurMode == data_load ? "Load" : "Save" ),currentParams->fileName,( EnDecrypt ? "Normal" : "Raw" ));
	}
	
	logs("sceSdSetMember(%p,%p,%x) = %x\n",ctx,data,len,ret);
	
	LastFunc = FNC_sceSdSetMember;
	
	pspSdkSetK1(k1);
	
	return ret;
}



int ms_sceSdCreateList(pspChnnlsvContext2 *ctx, int mode1, int mode2, unsigned char *hashkey, unsigned char *cipherkey)
{
	int k1	= pspSdkSetK1(0);
	int ret;
	
	
	if( LastFunc == data_none )
	{
		CurMode = data_save;
	}
	else if( LastFunc == FNC_sceSdSetIndex )
	{
		CurMode = data_load;
	}
	
	
	//���[�h�A�����ǂݍ��݁ADecrypt���[�h���C��
	if( MS_INI_RID && CurMode == data_load )
	{
		mode1 = mode_list[MS_INI_RID_DEC];
	}
	
	ret = _sceSdCreateList(ctx, mode1, mode2, hashkey, cipherkey);
	
	if( cipherkey && MS_INI_DumpGameKey )
	{
		char path[MS_INIT_STR_MAX];
		snprintf(path,MS_INIT_STR_MAX, MS_PATH_ROOT "/" MS_PATH_GAMEKEY "/%s.bin",game_id);
		u8 gkey[16];
		
		if( !ReadFile(path,gkey,16) )
		{
			CreateDir(path);
			WriteFile(path,cipherkey, 16);
		}
	}
	
	logs("sceSdCreateList(%p,%x,%x,%p,%p) = %x\n",ctx,mode1,mode2,hashkey,cipherkey,ret);
	
	LastFunc = FNC_sceSdCreateList;
	LastMode = CurMode;
	
	pspSdkSetK1(k1);
	
	return ret;
}



int ms_sceSdGetLastIndex(pspChnnlsvContext1 *ctx, unsigned char *hash, unsigned char *cryptkey)
{
	int k1	= pspSdkSetK1(0);
	int ret	= _sceSdGetLastIndex(ctx, hash, cryptkey);
	
	if( hash )
	{
		/*	[���[�h��]
			�j���t�@�C���Ǎ��ݗL���Ȃ�SFO�t�@�C�����ɂ���
			�Ώۃt�@�C���̔j�����o�n�b�V���̓[���N���A����Ă���̂�
			�e�Z�[�u�t�@�C���̍�����f�[�^�����ɏo���ꂽ�n�b�V����
			�[���N���A���č��킹��
		*/
		
		
		/*	[�Z�[�u��]
			�t�H�[�}�b�g�Ŕ�Í�����I�� �܂��� 
			������I��ł��đΏۃt�@�C���������_�Ŕ�Í�����ԂȂ�
			
			SFO�t�@�C�����֋L�^�����e�t�@�C���ւ̔j�����o�n�b�V����
			��Í�����Ԃ��ƋL�����邽�߃[���N���A����
		*/
		
		
		if
		(
			(	//[�Z�[�u]
				//�t�@�C���Ɋւ���n�b�V���������N���A(SFO�֘A�̂��N���A���Ȃ��悤�ɑ΍�
				CurMode == data_save && LastFunc == FNC_sceSdSetMember && 
				(
					( MS_INI_SFormat == SFORMAT_AUTO && Is_Raw )	//�t�H�[�}�b�g(����)�A��Í���
				||
					( MS_INI_SFormat == SFORMAT_RAW )				//�t�H�[�}�b�g(��Í���)
				)
			)
		|| 
			//�����ǂݍ��݁ASFO+FILE�̃n�b�V���N���A�i�Z�[�u�ł͖����Ƃ��Ɂj
			( CurMode != data_save && MS_INI_RID )
		||	//���[�h�A���E��ԁiFILE�̃n�b�V���N���A�j
			( CurMode == data_load && Is_Raw ) 
		)
		{
			CLEAR_HASH(hash);
			
			logs("CLEAR HASH [ sceSdGetLastIndex ] Is_Raw = %d , mode = %d\n",Is_Raw,CurMode);
		}
	}
	
	logs("sceSdGetLastIndex(%p,%p,%p) = %x [ dmode = %d ]\n",ctx,hash,cryptkey,ret,CurMode);
	
	if( currentParams )
	{
		logs("sceSdGetLastIndex TARGET: %s\n",currentParams->fileName );
	}
	
	LastFunc = data_none;
	
	pspSdkSetK1(k1);
	
	return ret;
}


int ms_sceSdSetIndex(pspChnnlsvContext1 *ctx, int mode)
{
	int k1	= pspSdkSetK1(0);
	
	//�Z�[�u�ȊO�̃n�b�V���A�����ǂݍ���
	if( CurMode == data_none && MS_INI_RID )
	{
		sceSdSetIndex_ctx = ctx;
		sceSdSetIndex_mode = mode;
		
		mode = mode_list[MS_INI_RID_DEC];
	}
	
	int ret	= _sceSdSetIndex(ctx, mode);
	
	logs("sceSdSetIndex(%p,%x) = %x\n",ctx,mode,ret);
	
	LastFunc = FNC_sceSdSetIndex;
	
	pspSdkSetK1(k1);
	
	return ret;
}


int ms_sceChnnlsv21BE78B4 (pspChnnlsvContext2 *ctx)
{
	int k1	= pspSdkSetK1(0);
	int ret	= _sceChnnlsv21BE78B4(ctx);
	
	logs("sceChnnlsv_21BE78B4(%p) = %x\n",ctx,ret);
	
	pspSdkSetK1(k1);
	
	return ret;
}

int ms_sceSdRemoveValue(pspChnnlsvContext1 *ctx, unsigned char *data, int len)
{
	int k1	= pspSdkSetK1(0);
	int ret	= 0;
	
	//�Z�[�u�ł͂Ȃ��ASFO�n�b�V���������A�����ǂݍ���
	if( CurMode == data_none && LastFunc == FNC_sceSdSetIndex && MS_INI_RID )
	{
		//sceSdSetIndex�����Ȃ���
		ret = _sceSdSetIndex(sceSdSetIndex_ctx, sceSdSetIndex_mode);
		
		logs("new_sceSdSetIndex(%p,%x) = %x\n",sceSdSetIndex_ctx,sceSdSetIndex_mode,ret);
	}
	
	if( ret >= 0 )
	{
		ret	= _sceSdRemoveValue(ctx,data,len);
		
		logs("sceSdRemoveValue(%p,%p,%d) = %x\n",ctx,data,len,ret);
	}
	
	pspSdkSetK1(k1);
	
	return ret;
}


/*
	SceUtilitySavedataParam �̍\���̂���ł�
	���ݏ�������Ă���Z�[�u�f�[�^�̃t�@�C���ꏊ���킩��Ȃ��̂�
*/

int ms_sceUtilitySavedataInitStart(SceUtilitySavedataParam* params)
{
	int k1	= pspSdkSetK1(0);
	int ret	= _sceUtilitySavedataInitStart(params);
	
    currentParams	= params;
	
	logs("sceUtilitySavedataInitStart\n");
	
	pspSdkSetK1(k1);
    
    return ret;
}


int ms_sceUtilitySavedataShutdownStart(void)
{
	int k1	= pspSdkSetK1(0);
	int ret	= _sceUtilitySavedataShutdownStart();
	
    currentParams	= NULL;
    
	logs("sceUtilitySavedataShutdownStart\n");
	
	pspSdkSetK1(k1);
	
    return ret;
}


int ms_sceUtilitySavedataGetStatus(void)
{
    int k1	= pspSdkSetK1(0);
	int ret	= _sceUtilitySavedataGetStatus();
	
	if( ret == 3 && currentParams && currentParams->base.result == 0 && currentParams->dataBufSize > 0 && currentParams->gameName && currentParams->saveName && currentParams->fileName)
	{
		char dpath[MS_INIT_STR_MAX]	;
		char sname[MS_GAME_NAME_MAX];
		
		snprintf(sname,MS_GAME_NAME_MAX,"%s%s",currentParams->gameName,currentParams->saveName);
		strutilRemoveChar( sname, "\\/:*?\"<>|" );
		snprintf(dpath,MS_INIT_STR_MAX, MS_PATH_ROOT "/"  MS_PATH_SAVE  "/%s/%s", sname,currentParams->fileName);
		
		if( strlen(sname) > 0 && strlen(currentParams->fileName) > 0 )
		{
			SceIoStat stat;
			
			//���[�h
			if( LastMode == data_load && MS_INI_RMS && sceIoGetstat(dpath, &stat) >= 0 )
			{
				if( stat.st_size <= currentParams->dataBufSize && ReadFile(dpath,currentParams->dataBuf, stat.st_size) )
				{
					Call_PLG_Load("BackUp File",currentParams->fileName,currentParams->dataBuf,stat.st_size);
					
					logs("Load BackUp File : %s\n",dpath);
				}
			}
			else if( LastMode == data_save && MS_INI_WMS )
			{
				CreateDir(dpath);
				
				Call_PLG_Save("BackUp File",currentParams->fileName,currentParams->dataBuf,currentParams->dataBufSize);
				
				WriteFile(dpath,currentParams->dataBuf, currentParams->dataBufSize);
			}
		}
	}
	
    pspSdkSetK1(k1);
	
    return ret;
}

