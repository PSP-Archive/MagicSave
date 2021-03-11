#include "main.h"

// MagicSave���̋@�\�𗘗p���邽�߂ɃC���N���[�h
// �g����֐��A�g������ include�t�H���_���ɂ���
// �w�b�_�[�t�@�C��( MagicSave.h ) ���Q��
#include "MagicSave.h"

PSP_MODULE_INFO("MS_Plugin",PSP_MODULE_KERNEL, 1, 1);


/************************************************************
 * MagicSave�p�̃v���O�C�������ۂ̒��ӓ_                  *
 * �ǂ����ɕs��������΃v���O�C���͋@�\���Ȃ�               *
 *                                                          *
 * �O���[�o���ϐ� Plugin_Func  Load�ASave  ��               *
 * MS_GetFuncInfo_XXXX �ł��ꂼ��̏���                   *
 * MagicSave���֑���K�v�����邽��                          *
 * �ϐ�����ύX���Ă͂����Ȃ�                               *
 * ��������x�������Ă���͎��R�ɕύX����OK                 *
 *                                                          *
 * �v���O�C���̋�̓I�ȍ쐬���@�͉��L�Q�� �A�菇4�܂�       *
 ************************************************************/



/**********************************************
 * �菇1�AMakefile��'TARGET'�փQ�[��ID��ݒ�  *
 **********************************************/

/**********************************************
 * �菇2�A�f�[�^����������֐��̒�`          *
 **********************************************/

// MagicSave���Z�[�u�t�@�C�������[�h�E�Z�[�u�����ۂɌĂ΂��
// ���܂蕡�G�ȏ����͂��Ȃ����ƁA�ꍇ�ɂ���Ă̓t���[�Y����̂Œ���


// void �֐���(const char *FileName, void *Data, SceSize Size) 
// FileName	= �����i�Z�[�u�A���[�h�j���̃t�@�C����
// Data		= �����i�Z�[�u�A���[�h�j���̃f�[�^
// Size		= �����i�Z�[�u�A���[�h�j���̃t�@�C���T�C�Y


// ��Mode2 "Raw"�`���ŕۑ�������Í�����Ԃ̃t�@�C����
// �@�t�@�C���擪�� 0x10 Byte ���֌W�ȃf�[�^���ǉ������
// �@�܂�{���̃f�[�^�̓t�@�C���ゾ�� 0x10�` ����
// �@�f�[�^����������֐����`����ꍇ�Ɉӎ�(Data+0x10)����K�v�͂Ȃ�
// �@�iData�͏��+0x10���ꂽ�A�h���X�������Ă���j
// �@�܂��A�t�@�C�����炻�̗]���ȃf�[�^���폜���Ă������Ȃ�


// �֐��̒�`�� : �Q�[�� NARUTO �����`



//���[�h���Ƀv���C���[����'NARUTO'�֕ς���
void Name_Change(const char *FileName, void *Data, SceSize Size)
{
	char *new_name	= "NARUTO";
	char *name		= (char*)(Data + 0x10);//Data+0x10 =>�v���C���[���̃A�h���X

	memcpy(name,new_name,strlen(new_name));
	name[strlen(new_name)] = '\0';
}





// �Z�[�u���ꂽ���ɁANP�֌W�i���ݒl�A�g�[�^���j��S��MAX�ɂ���
void NP_MAX(const char *FileName, void *Data, SceSize Size)
{
	int *np = (int*)(Data + 0xA8);

	//NP ���ݒl���ő��
	*np = 0x0F423F;

	np++;

	//NP �g�[�^���l���ő��
	*np = 0x0F423F;
}




//���[�h���Ƀv���C���[����NARUTO�ցANP�֌W��MAX��
void Change_Name_NP(const char *FileName, void *Data, SceSize Size)
{
	//���ɒ�`����Ă���֐���
	//�����痘�p���đg�ݍ��킹�Ďg��

	Name_Change(FileName, Data, Size);
	NP_MAX(FileName,Data,Size);
}




/**********************************************
 * �菇3�A��`�����֐����e�[�u���֓o�^����    *
 **********************************************/

// �O���[�o���ϐ� Plugin_Table_Func Load�ASave ��
//
// { "MagicSave�ɕ\������閼�O" , �o�^����֐� } ,
//
// �Ƃ������ɂ��ēo�^���Ă���
//
// ��Load�ASave ���ɁA�ő��256�܂œo�^�o���܂�
// �@�w�b�_�[�� MS_PLUGIN_TABLE_MAX �̒l��ς��Ă�
// �@MagicSave���Ő�������Ă��邩�炻��ȏ�͓o�^�s��



// MagicSave�̃��j���[���
// "Status" -> "Plugin"��L���ɂ������A
// 'Load' �֕\�������֐����X�g

static Plugin_Func Load[] =
{
	{"Name to \"NARUTO\""		,Name_Change	}, //��قǒ�`�����֐���o�^
	{"Name Change + NP MAX"		,Change_Name_NP	}, //��قǒ�`�����֐���o�^
};


// MagicSave�̃��j���[���
// "Status" -> "Plugin"��L���ɂ������A
// 'Save' �֕\�������֐����X�g

static Plugin_Func Save[] =
{
	{"NP Max"					,NP_MAX			},	//��قǒ�`�����֐���o�^
};





/**********************************************
 * �菇4�APSPSDK���g���v���O�C�����R���p�C��  *
 **********************************************/

// �G���[�Ȃ��R���p�C�����I�������
// MagicSave/Plugins/ �t�H���_�փR�s�[
// 
// MagicSave��L���ɂ��đΏۂ̃Q�[�����N��
// ���j���[���� "Status" -> "Plugin"��L���ɂ���
// ��`�����֐���I�ѓ���m�F

// ���O��L���ɂ���΂�����x�̃f�o�b�O���\�ł�


// ����Ƀv���O�C�����N���o���āA�w�肵���֐����Ă΂���
// Call Plugin : Mode = ���(Load/Save) FuncName = �֐��� [ �Ώۃt�@�C�� ]


// ���炩�̃G���[�Ńv���O�C�����N���o���Ȃ��ꍇ��
// Err,Load/Save Func Table Size 0	=> Load/Save ���Ɋ֐���`����0
// Err,Link(Exports)				=> �v���O�C������֐��̃G�N�X�|�[�g�Ɏ��s���Ă���


// �Ƃ������O���L�^����܂�

// �֐��������ƌĂ΂�Ă���̂ɓ��삪���������ꍇ��
// MS_Log�����p���ăf�o�b�O���ĉ�����





/*************************************************
 * ��������͕K�{�R�[�h�A�ύX�E�폜���Ȃ��悤��  *
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
 * �����܂ł͕K�{�R�[�h�A�ύX�E�폜���Ȃ��悤��  *
 *************************************************/


