#include "common.h"

#include "color.h"
#include "menu.h"
#include "plugin.h"

extern bool hook_status;

const char *SType_List[] =
{
	MENU_SType_Auto,
	MENU_SType_Raw,
	MENU_SType_Normal,
	NULL
};


const char *Menu_Type[] =
{
	MENU_All,
	MENU_Active,
	NULL
};

const char *RID_SFO[] =
{
	MENU_Type1,
	MENU_Type2,
	MENU_Type3,
	NULL
};

const char *RID_DEC[] =
{
	MENU_Type1,
	MENU_Type2,
	MENU_Type3,
	NULL
};

const char *SYS_HOOK[] =
{
	MENU_HOOK_D,
	MENU_HOOK_E,
	NULL
};

char GameID[31];


MenuContext	Context;
libmOpt	opt;
MenuItem item_list[ITEM_COUNT];

MenuItem* MENU_CNT_SFile= NULL;

MenuItem *Item_Format	= NULL;
MenuItem *Item_DGKey 	= NULL;

MenuItem *Item_BKUP_L	= NULL;
MenuItem *Item_BKUP_S	= NULL;

MenuItem *Item_RID		= NULL;
MenuItem *Item_RID_SFO	= NULL;
MenuItem *Item_RID_DEC	= NULL;

MenuItem *Item_PLG		= NULL;
MenuItem *Item_PLG_L	= NULL;
MenuItem *Item_PLG_S 	= NULL;

MenuItem *Item_Menu_D	= NULL;
MenuItem *Item_Menu_V	= NULL;
MenuItem *Item_Menu_S	= NULL;


MenuItem *Item_Hook_S	= NULL;






//�v���O�C���ȊO�̑��X���b�h���~�E�ĊJ������
int libmExecTCmd( int cmd )
{
	static int thid_status = LIBM_TRESUME;
	static bool thid_IsDump = false;
	static bool this_IsBusy = false;
	
	static int thid_list_first[MAX_NUMBER_OF_THREADS];
	static int thid_count_first;
	
	static int thid_list_Current[MAX_NUMBER_OF_THREADS];
	static int thid_count_Current;
	
	if( cmd ==  LIBM_TCMD_DUMP )
	{
		thid_IsDump = sceKernelGetThreadmanIdList( SCE_KERNEL_TMID_Thread, thid_list_first, MAX_NUMBER_OF_THREADS, &thid_count_first ) >= 0;
		
		return thid_IsDump;
	}
	else if( (cmd == LIBM_TCMD_SUSPEND || cmd == LIBM_TCMD_RESUME) && thid_IsDump && cmd != thid_status && !this_IsBusy )
	{
		thid_status = cmd;
		this_IsBusy = true;
		
		int i, j;
		int (*func)(SceUID) =  ( cmd == LIBM_TCMD_SUSPEND ? sceKernelSuspendThread : sceKernelResumeThread );
		SceKernelThreadInfo thinfo;
		
		
		if( cmd == LIBM_TCMD_SUSPEND )
		{
			//��~
			
			int Current_id	= sceKernelGetThreadId();
			
			//���݂̃X���b�h���X�g�����
			if( sceKernelGetThreadmanIdList( SCE_KERNEL_TMID_Thread, thid_list_Current, MAX_NUMBER_OF_THREADS, &thid_count_Current ) < 0 )
			{
				//���݂̃X���b�h���X�g�擾���s
				this_IsBusy = false;
				return 0;
			}
			
			
			//���݂̃X���b�h�ꗗ�ƁA�v���O�C���N�����Ƀ_���v�����Z�[�t���X�g���r
			//�Z�[�t���X�g�ƌ��X���b�h�Ɉ�v������̂͑ΏۊO��
			for( i = 0; i < thid_count_Current; i++ )
			{
				memset(&thinfo, 0, sizeof(SceKernelThreadInfo));
				thinfo.size = sizeof(SceKernelThreadInfo);
				sceKernelReferThreadStatus(thid_list_Current[i], &thinfo);
				
				if
				(
				//	( strcasecmp( "SceImpose",thinfo.name) == 0 )
				//||
					( thinfo.attr & 0x100000 ) == 0x100000
				||
					( thid_list_Current[i] == Current_id )
				)
				{
					thid_list_Current[i] = 0;
					continue;
				}
				
				for( j = 0; j < thid_count_first; j++ )
				{
					if( thid_list_Current[i] == thid_list_first[j] )
					{
						thid_list_Current[i] = 0;
						break;
					}
				}
			}
		}
		
		
		//�ŏI�I�Ȍ��݂̃X���b�h���X�g�ɂ���X���b�h�֒�~�E�ĊJ�̑���
		for( i = 0; i < thid_count_Current; i++ )
		{
			if( thid_list_Current[i] ) ( func )( thid_list_Current[i] );
		}
		
		this_IsBusy = false;
		
		return 1;
	}
	else if( cmd == LIBM_TCMD_GET )
	{
		return thid_status;
	}
	
	return 0;
}







void MenuCreate()
{
	memset(&opt,0,sizeof(libmOpt));
	
	opt.type		= LIBM_OPT_LIST;
	opt.list.val	= item_list;
	opt.list.size	= sizeof(item_list);
	
	libmSetOpt(&Context,&opt);
	
	//���j���[�^�C�v�́u�S�̕\���v
	Context.MenuInfo.Type			= MS_INI_Menu_Type;
	
	Context.MenuInfo.AutoReturn		= true;
	
	//�A�N�e�B�u�ȍ��ڂ̃t�H���g�F�A�w�i�F
	Context.Color.Active_Font		= PINK;
	Context.Color.Active_Back		= BLUE;
	
	//��A�N�e�B�u�ȍ��ڂ̃t�H���g�F�A�w�i�F
	Context.Color.Normal_Font		= WHITE;
	Context.Color.Normal_Back		= GRAY;
	
	//Context.Color.Disable_Font	= WHITE;
	
	//���������₷������ׂɍs�Ԃ��L����
	Context.MenuInfo.Lines			= 1;
	
	//���j���[Info�p�̕�������
	snprintf(GameID,30," %s:%s", MENU_Game,game_id  );
	
	libmAddItem(&Context	, NULL		, libmCreateSpacer(&opt,"MagicSave Ver" MS_STR_VER)		, YELLOGREEN, DARKGREEN		, NoAction			, 0	);
	libmAddItem(&Context	, NULL		, libmCreateSpacer(&opt,(char*)GameID)				, WHITE		, BLACK			, NoAction			, 0	);
	libmAddItem(&Context	, NULL		, libmCreateSpacer(&opt,NULL)						, Invalid	, Invalid		, NoAction			, 0	);
	
	
	MENU_CNT_SFile					=	libmAddItem(&Context	, NULL					, libmCreateContainer(&opt,MENU_SAVEFILE)								, WHITE		, 0xFF000088	,NoAction			, 0	);
	
										libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateSpacer(&opt,MENU_SType_Info)								, WHITE		, BLACK			, NoAction			, 0	);
	Item_Format						=	libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateSelectBox(&opt,NULL,SType_List,MS_INI_SFormat)				, Invalid	, Invalid		, SFormat_Change	, 0	);
										libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateSpacer(&opt,NULL)											, Invalid	, Invalid		, NoAction			, 0	);
	Item_DGKey						=	libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateCheckBox(&opt,MENU_Dump_Gamekey,MS_INI_DumpGameKey)			, WHITE		, BLACK			, GameKey_Change	, 0	);
										libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateSpacer(&opt,NULL)											, Invalid	, Invalid		, NoAction			, 0	);
	
	
	
	MenuItem* MENU_CNT_RID 			= 	libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateContainer(&opt,MENU_RID)									, Invalid	, ORANGE		, NoAction			, 0	);
	Item_RID						=	libmAddItem(&Context	, MENU_CNT_RID 			, libmCreateCheckBox(&opt,MENU_Enable,MS_INI_RID)						, WHITE		, BLACK			, RID_Change		, 0	);
										libmAddItem(&Context	, MENU_CNT_RID 			, libmCreateSpacer(&opt,NULL)											, WHITE		, BLACK			, NoAction			, 0	);
	Item_RID_SFO					=	libmAddItem(&Context	, MENU_CNT_RID			, libmCreateSelectBox(&opt,MENU_RID_SFO,RID_SFO,0)						, Invalid	, Invalid		, RID_SFO_Change	, 0	);
	Item_RID_DEC					=	libmAddItem(&Context	, MENU_CNT_RID			, libmCreateSelectBox(&opt,MENU_RID_DEC,RID_DEC,0)						, Invalid	, Invalid		, RID_DEC_Change	, 0	);
	
	MENU_CNT_RID->Ctrl.CNT.Disp = Item_RID;
	//Item_RID->Color.Line	= YELLOW;
	
	
	SceUID fd = sceIoOpen(path_prx, PSP_O_RDONLY, 0777);
	
	if( fd >= 0 )
	{
		MenuItem* MENU_CNT_PLG 			=	libmAddItem(&Context	, MENU_CNT_SFile 		, libmCreateContainer(&opt,MENU_Plugin)							, Invalid	, ORANGE		, NoAction			, 0	);
		Item_PLG						= 	libmAddItem(&Context	, MENU_CNT_PLG 			, libmCreateCheckBox(&opt,MENU_Enable,false)					, WHITE		, BLACK			, PLG_Use_Change	, 0	);
											libmAddItem(&Context	, MENU_CNT_PLG 			, libmCreateSpacer(&opt,NULL)									, Invalid	, Invalid		, NoAction			, 0	);
											libmAddItem(&Context	, MENU_CNT_PLG 			, libmCreateSpacer(&opt,MENU_Plugin_Info)						, WHITE		, BLACK			, NoAction			, 0	);
		Item_PLG_L						=	libmAddItem(&Context	, MENU_CNT_PLG 			, libmCreateSelectBox(&opt,MENU_PLOAD,PTBL_Load.NList,0)		, Invalid	, Invalid		, PLG_RSel_Change	, 0	);
		Item_PLG_S						=	libmAddItem(&Context	, MENU_CNT_PLG 			, libmCreateSelectBox(&opt,MENU_PSAVE,PTBL_Save.NList,0)		, Invalid	, Invalid		, PLG_WSel_Change	, 0	);
		
		MENU_CNT_PLG->Ctrl.CNT.Disp = Item_PLG;
		//Item_PLG->Color.Line		= YELLOW;
		
		sceIoClose(fd);
	}
	
	
	MenuItem* MENU_CNT_BKUP			=	libmAddItem(&Context	, MENU_CNT_SFile		, libmCreateContainer(&opt,MENU_BackUp)								, Invalid	, ORANGE		, NoAction			, 0	);
	Item_BKUP_L						=	libmAddItem(&Context	, MENU_CNT_BKUP			, libmCreateCheckBox(&opt,MENU_LMS,MS_INI_RMS)						, Invalid	, Invalid		, BKUP_RMS_Change	, 0	);
	Item_BKUP_S						=	libmAddItem(&Context	, MENU_CNT_BKUP			, libmCreateCheckBox(&opt,MENU_SMS,MS_INI_WMS)						, Invalid	, Invalid		, BKUP_WMS_Change	, 0	);
	
	
	MenuItem* MENU_CNT_Menu			=	libmAddItem(&Context	, NULL					, libmCreateContainer(&opt,MENU_MenuOption)							, WHITE		, 0xff008888	, NoAction			, 0	);
	
										libmAddItem(&Context	, MENU_CNT_Menu 		, libmCreateSpacer(&opt,MENU_S_SYSTEM)								, WHITE		, BLACK			, NoAction			, 0	);
	Item_Hook_S 					=	libmAddItem(&Context	, MENU_CNT_Menu 		, libmCreateSelectBox(&opt,MENU_HOOK,SYS_HOOK,MS_INI_Hook)			, Invalid	, Invalid		, Hook_Change	, 0	);
	
										libmAddItem(&Context	, MENU_CNT_Menu 		, libmCreateSpacer(&opt,NULL)										, Invalid	, Invalid		, NoAction			, 0	);
	
	
										libmAddItem(&Context	, MENU_CNT_Menu 		, libmCreateSpacer(&opt,MENU_S_MENU)								, WHITE		, BLACK			, NoAction			, 0	);
	Item_Menu_D						=	libmAddItem(&Context	, MENU_CNT_Menu			, libmCreateUpDownCtrl(&opt,MENU_Delay,MS_INI_Menu_Delay,MS_INIT_MDSTEP,50,MS_INIT_MDMAX,true)	,  Invalid	, Invalid			, Delay_Change		, 0	);
	Item_Menu_V						=	libmAddItem(&Context	, MENU_CNT_Menu			, libmCreateSelectBox(&opt,MENU_View,Menu_Type,( MS_INI_Menu_Type == LIBM_VIEW_ALL ? 0 : 1 ))	,  Invalid	, Invalid		, MenuType_Change	, 0	);
										libmAddItem(&Context	, MENU_CNT_Menu			, libmCreateSpacer(&opt,NULL)																	, Invalid	, Invalid		, NoAction			, 0	);
	Item_Menu_S						= 	libmAddItem(&Context	, MENU_CNT_Menu			, libmCreateCheckBox(&opt,MENU_TH_Stop,MS_INI_Menu_STH) 								,  Invalid	, Invalid			, TH_Stop_Change	, 0	);
	
	libmSetActive( &Context , hook_status ? MENU_CNT_SFile : MENU_CNT_Menu);
	
	Sync_Item();
}




void Sync_Item()
{
	//�t�H�[�}�b�g�I����ԂƓ���
	MS_INI_SFormat = Item_Format->Ctrl.SB.Index;
	
	Item_DGKey->Ctrl.CB_Checked	= MS_INI_DumpGameKey;
	
	//�o�b�N�A�b�v
	Item_BKUP_L->Ctrl.CB_Checked	= MS_INI_RMS;
	Item_BKUP_S->Ctrl.CB_Checked	= MS_INI_WMS;
	
	//�v���O�C��
	if( Item_PLG )
	{
		Item_PLG->Ctrl.CB_Checked = Is_Load_Plugin();
		
		if( !Is_Load_Plugin() )
		{
			PTBL_Load.Select	= 0;
			PTBL_Load.NList[1]	= NULL;
			
			PTBL_Save.Select	= 0;
			PTBL_Save.NList[1]	= NULL;
		}
		
		Item_PLG_L->Ctrl.SB.Index		= PTBL_Load.Select;
		Item_PLG_S->Ctrl.SB.Index		= PTBL_Save.Select;
	}
	
	//�j���t�@�C��
	Item_RID->Ctrl.CB_Checked		= MS_INI_RID;
	Item_RID_SFO->Ctrl.SB.Index		= MS_INI_RID_SFO;
	Item_RID_DEC->Ctrl.SB.Index		= MS_INI_RID_DEC;
	
	//���j���[�A�f�B���C �\������
	if(
		(Item_Menu_D->Ctrl.UD.Now < Item_Menu_D->Ctrl.UD.Min)
	||
		(Item_Menu_D->Ctrl.UD.Now > Item_Menu_D->Ctrl.UD.Max)
	)
	{
		Item_Menu_D->Ctrl.UD.Now = Item_Menu_D->Ctrl.UD.Min;
	}
	else
	{
		float tmp =  (int)Item_Menu_D->Ctrl.UD.Now % (int)Item_Menu_D->Ctrl.UD.Step;
		Item_Menu_D->Ctrl.UD.Now -= tmp;
	}
	
	MS_INI_Menu_Delay = (int)Item_Menu_D->Ctrl.UD.Now;
	
	Item_Menu_V->Ctrl.SB.Index		= MS_INI_Menu_Type;
	
	//���j���[�A���X���b�h��~ �\������(���킹�邾��)
	Item_Menu_S->Ctrl.CB_Checked	= MS_INI_Menu_STH;
	
	MENU_CNT_SFile->Visible = hook_status;
}


