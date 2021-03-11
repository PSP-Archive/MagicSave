#ifndef _PSP_PSF_H_
#define	_PSP_PSF_H_

#define	OFFSET_TABLE_INFO				0x14

#define	PSF_KEY_BOOTABLE				"BOOTABLE"				//INT
#define	PSF_KEY_CATEGORY				"CATEGORY"				//TXT
#define	PSF_KEY_DISC_ID					"DISC_ID"				//TXT
#define	PSF_KEY_DISC_NUMBER				"DISC_NUMBER"			//INT
#define	PSF_KEY_DISC_TOTAL				"DISC_TOTAL"			//INT
#define	PSF_KEY_DISC_VERSION			"DISC_VERSION"			//TXT
#define	PSF_KEY_DRIVER_PATH				"DRIVER_PATH"			//TXT
#define	PSF_KEY_LANGUAGE				"LANGUAGE"				//TXT
#define	PSF_KEY_PLEVEL					"PARENTAL_LEVEL"		//INT
#define	PSF_KEY_PSP_SYSTEM_VER			"PSP_SYSTEM_VER"		//TXT
#define	PSF_KEY_REGION					"REGION"				//INT
#define	PSF_KEY_SAVE_DETAIL				"SAVEDATA_DETAIL"		//TXT
#define	PSF_KEY_SAVE_DIRECTORY			"SAVEDATA_DIRECTORY"	//TXT
#define	PSF_KEY_SAVE_FLIST				"SAVEDATA_FILE_LIST"	//BIN
#define	PSF_KEY_SAVE_PARAMS				"SAVEDATA_PARAMS"		//BIN
#define	PSF_KEY_SAVE_TITLE				"SAVEDATA_TITLE"		//TXT
#define	PSF_KEY_TITLE					"TITLE"					//TXT
#define	PSF_KEY_TITLE_0					"TITLE_0"				//TXT
#define	PSF_KEY_TITLE_2					"TITLE_2"				//TXT
#define	PSF_KEY_TITLE_3					"TITLE_3"				//TXT
#define	PSF_KEY_TITLE_4					"TITLE_4"				//TXT
#define	PSF_KEY_TITLE_5					"TITLE_5"				//TXT
#define	PSF_KEY_TITLE_6					"TITLE_6"				//TXT
#define	PSF_KEY_TITLE_7					"TITLE_7"				//TXT
#define	PSF_KEY_TITLE_8					"TITLE_8"				//TXT
#define	PSF_KEY_UPDATER_VER				"UPDATER_VER"			//TXT



#define	PSF_CATEGORY_WG					"WG"	//WLAN Game 	a game runable via Gamesharing
#define	PSF_CATEGORY_MS					"MS"	//MemoryStick Save 	a savegame
#define	PSF_CATEGORY_MG					"MG"	//MemoryStick Game 	a game runnable from MemoryStick
#define	PSF_CATEGORY_UG					"UG"	//UMD Game 	a game runnable from UMD
#define	PSF_CATEGORY_UV					"UV"	//UMD Video
#define	PSF_CATEGORY_UA					"UA"	//UMD Audio
#define	PSF_CATEGORY_UC					"UC"	//UMD Cleaning Disc

enum
{
	PSF_DATA_BIN	= 0x00,
	PSF_DATA_TXT	= 0x02,
	PSF_DATA_INT	= 0x04,
};

enum
{
	PSF_FLAG_WG		= 0x00,
	PSF_FLAG_MS		= 0x01,
	PSF_FLAG_MG		= 0x02,
	PSF_FLAG_UG		= 0x04,
};

#define	PSF_FLAG_XG		(PSF_FLAG_MG | PSF_FLAG_UG)
#define	PSF_FLAG_MX		(PSF_FLAG_MS | PSF_FLAG_MG)
#define	PSF_FLAG_ALL	(PSF_FLAG_MS | PSF_FLAG_MG | PSF_FLAG_UG)



typedef struct
{
	u32		signature;		//���".PSF"
	
	u32		version;		//PSF�̃o�[�W����

	u32		offset_key;		//�I�t�Z�b�g�A�e�[�u�����̊J�n�ʒu
	u32		offset_val;		//�I�t�Z�b�g�A�e�[�u���l�̊J�n�ʒu
	u32		number_ent;		//�e�[�u�����̃A�C�e����
} SFO_HEADER;



typedef struct
{
	u16		offset_key;		//�e�[�u�����ւ̃I�t�Z�b�g
	u8		alignment;		//��� 04
	u8		type;			//�f�[�^�̃^�C�v (0:�o�C�i���[ 2:�e�L�X�g 4:32bit����)
	u32		size_01;		//�f�[�^�̂�
	u32		size_02;		//�S�́i�f�[�^�{�󂫁j
	u32		offset_val;		//�I�t�Z�b�g�A�f�[�^�����̊J�n�ʒu
} SFO_TABLE;


typedef struct
{
	char *key;
	u8 type;
	u8 *pos;
	int size_01;
	int size_02;
	u8 *curPos;
} SFO_DATA;

typedef struct
{
	char name[13];			//�t�@�C����
	u8 hash[16];			//�n�b�V��
	u8 unk[3];				//�s�v�ȃf�[�^
} SFO_FILE;

typedef struct
{
	u8	mode;		//1
	u8	unk01[15];	//2-16
	u8	hash01[16];	//17-
	u8	hash02[16];
	u8	unk02[64];
	u8	hash03[16];
}SFO_PARAMS;


int find_psf_key(u8 *buf, int bufLen ,const char *key ,SFO_DATA *data_p);
SFO_FILE* find_psf_datafile(SFO_DATA *data);




#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               