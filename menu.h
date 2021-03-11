#ifndef MS_MENU_H
#define MS_MENU_H


#ifdef __cplusplus
extern "C" {
#endif




#define	LIBM_TSUSPEND				LIBM_TCMD_SUSPEND
#define	LIBM_TRESUME				LIBM_TCMD_RESUME
#define MAX_NUMBER_OF_THREADS 		64


#define	ITEM_COUNT	32


// libmExecTCmd �Ŏw��\�ȃR�}���h�ꗗ
enum
{
	LIBM_TCMD_SUSPEND	,	//���X���b�h���~
	LIBM_TCMD_RESUME	,	//���X���b�h���ĊJ
	LIBM_TCMD_DUMP		,	//���X���b�h���~�E�ĊJ������ׂ̃Z�[�t���X�g�����
	LIBM_TCMD_GET		,	//���X���b�h�̒�~�E�ĊJ��Ԃ��擾
};


enum
{
	NoAction,
	GameKey_Change,
	RID_Change,
	RID_SFO_Change,
	RID_DEC_Change,
	BKUP_WMS_Change,
	BKUP_RMS_Change,
	BKUP_FRS_Change,
	Log_Change,
	SFormat_Change,
	Delay_Change,
	MenuType_Change,
	PLG_Use_Change,
	PLG_RSel_Change,
	PLG_WSel_Change,
	TH_Stop_Change,
	Hook_Change,
};


void MenuCreate();
void Sync_Item();

int libmExecTCmd( int cmd );

extern MenuItem* MENU_CNT_SFile;
#ifdef __cplusplus
}
#endif

#endif

