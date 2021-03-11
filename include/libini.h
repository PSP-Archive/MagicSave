#ifndef _LIB_INI_H_
#define _LIB_INI_H_


#include <stdbool.h>
#include <pspiofilemgr.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
	iniLoad/iniSave/iniSplit�ɂ���
	�����Ă���Key/Value�͓ǂݍ��񂾂܂܂̏�ԂȂ̂�
	�R�[���o�b�N�֐��ŏ�������ۂ͑啶��/�������̋�ʂ�
	�ǂ����邩�ӎ�����̂�Y��Ȃ��ŉ�����
	
	�啶��/��������ʂ����F������悤�ɂ���̂���ʓI
*/



//iniAddKey�Ŏw�肷��Key�̃f�[�^�^
typedef enum
{
	INI_TYPE_NONE = 0,	//����
	INI_TYPE_DEC,		//10�i��
	INI_TYPE_HEX,		//16�i��
	INI_TYPE_OCT,		//8�i��
	INI_TYPE_BOOL,		//BOOL
	INI_TYPE_STR,		//������
	INI_TYPE_DOUBLE,	//DOUBLE ( ���������l )

}ini_type;


//Key,Value�̃y�A
typedef struct
{
	ini_type	type;	//�f�[�^�^
	const char	*key;	//Key��
	
	void *val_load;
	void *val_save;

	bool flag;
	int id;
} ini_pair;


//iniParseLine
typedef struct
{
	char *key;			//key
	char *key_end;		//key�̍Ō�
	char key_end_cd;	//key_end��\0�Œu��������O�̕���
	
	char *val;			//value
	char *val_end;		//value�̍Ō�
	char val_end_cd;	//value_end��\0�Œu��������O�̕���
	
} ini_line;



typedef struct
{
	ini_pair *pair;		//Key/Value��ǉ�/�ݒ�/�폜����ۂɎg�p����ini_pair�z��
	int size;			//pair�Ŏw�肵���z��T�C�Y( sizeof(pair) )
	
	char *buf;			//iniReadLine�Ŏg����o�b�t�@�[�w��
	int bufLen;			//buf�̃T�C�Y�i�����Ŏw�肵���l��iniReadLine�œǂݍ��߂�1�s�̍ő啶�����j
} ini_data;


// iniSplitVal�̃R�[���o�b�N�֐� void (*)(const char *val,void *callback_opt)
typedef void (*libini_cbsp)(const char *val,void *callback_opt);

// iniSave �̃R�[���o�b�N�֐� const char* (*)(const char *key,void *opt,ini_pair *pair)
typedef const char* (*libini_cbs)(const char *key,void *opt,ini_pair *pair);

// iniLoad �̃R�[���o�b�N�֐� bool (*)(const char *key, char *val,void *opt,ini_pair *pair)
typedef bool (*libini_cbl)(const char *key, char *val,void *opt,ini_pair *pair);


enum
{
	INIREADLINE_END		= 0,		//�t�@�C���̏I���A�܂��ُ͈�ȏ��
	INIREADLINE_CR		= -1,		//���s�R�[�hCR�̂�
	INIREADLINE_LF		= -2,		//���s�R�[�hLF�̂�
	INIREADLINE_CRLF	= -3,		//���s�R�[�hCRLF
};



/*	iniReadLine
    �t�@�C������1�s��ǂݍ���
    
    @param: SceUID fd
    SceIoOpen�ŊJ�����t�@�C��
    
    @param: char *buf
    �ǂݍ��񂾕�������i�[����o�b�t�@�[
    
    @param: int bufLen
    �o�b�t�@�[�T�C�Y
	�T�C�Y�������������1�s�ۂ��Ǝ擾�o���Ȃ��̂ŕK�v�ɉ����Ďw��
	�Ō��\0�������ǉ������d�l����Œ� 2 �͕K�v
    
    @return: �ȉ��̒ʂ�
	
	INIREADLINE_END		= �t�@�C���I�[		��buf -> '\0'�̂�
    INIREADLINE_CR		= ���s�R�[�h(CR) 	��buf -> '\0'�̂�
    INIREADLINE_LF		= ���s�R�[�h(LF)	��buf -> '\0'�̂�
    INIREADLINE_CRLF	= ���s�R�[�h(CRLF)	��buf -> '\0'�̂�
	����ȊO			= �ǂݍ��񂾕�����	��buf -> �ǂݍ��񂾕�����
 */

int iniReadLine( SceUID fd, char *buf, int bufLen );


/*	iniParseLine
    iniReadLine�ɂ���ēǂݍ��񂾕������Key�AValue�֕�����
    
    @param: ini_line *line
    ���ʂ��󂯎�� ini_line�ϐ�
    
    @param: void *buf
    �Ώە�����
	
    @return: true = �����Afalse = ���s(Key = Value�̌`�ł͂Ȃ�)
    
    ��Key/Value�̎n�܂�/�I����
    �@�^�u(\t)�A���p/�S�p�X�y�[�X�����O�����ʒu�ɂȂ�܂�
    �@���O�Ώە��������Ȃ��ꍇ�͖߂�l false �ƂȂ�܂�
 */
bool iniParseLine( ini_line *line ,char *buf );

/*	iniRestoreLine
    iniParseLine �� Key�AVal �ւƕ������ꂽ�����������1�s�ւƕ�������
    
    @param: ini_line *line
    iniParseLine�Ŏw�肵�� ini_line �ϐ�
	
 */
#define	iniRestoreLine( line )	if( (line)->key_end ) *(line)->key_end = (line)->key_end_cd;\
								if( (line)->val_end ) *(line)->val_end = (line)->val_end_cd;


/*	iniSplitVal
    iniParseLine���瓾��Value�i������j����ؕ����ŕ�������
    
    @param: char *val
    �Ώە�����
    
    @param: const char *delim
    ��؂镶���Q�i�����ꂩ����v����Ƃ��̕����ŋ�؂���j
    
    @param: libini_cbsp callback_sp
    ��؂�ꂽ��������󂯎��R�[���o�b�N�֐�
	
	[�R�[���o�b�N�֐��֑����Ă���l]
    val 			= ��؂�ꂽ������
    callback_opt	= iniSplitVal�Ŏw�肵���f�[�^�i�C�Ӂj
    
    @param: void *callback_opt
    �R�[���o�b�N�֐��֓n���f�[�^
    
    ����؂�ꂽ�����͑O�ォ�疳������( ���p/�S�p �󔒁Atab )����菜�����
    �@�R�[���o�b�N�֐��֑����Ă��܂�
 */
void iniSplitVal( char *val ,const char *delim ,libini_cbsp callback_sp ,void *callback_opt );



/*	iniSearchKey
  	data(ini_data*) ����w�肵��Key���܂܂��f�[�^(ini_pair*)���擾
    
    @param: ini_data *data
    �����Ώۂ�ini_data�ϐ�
    
    @param: const char *key
    �����Ώۂ�Key
    NULL���w�肷��Ƌ�(Key���Z�b�g����Ă��Ȃ��ꏊ�j��T��
	
    @return: �Ώ�Key���܂܂�Ă����f�[�^(ini_pair*)
    NULL = ���݂��Ȃ�����
 */
ini_pair* iniSearchKey( ini_data *data ,const char *key );

/*	iniAddKey
  	data(ini_data*)��Key(Value)��ǉ�����
    
    @param: ini_data *data
    �Ώۂ�ini_data�ϐ�
    
    @param: const char *key
    �ǉ��Ώۂ�Key
	
    @param: ini_type type
    �f�[�^�^���w��iINI_TYPE_NONE�ȊO)
    
    @param: void *val_load
    iniLoad�����s����ۂɁA�擾�����l��ۑ�����ϐ��̃|�C���^�[
    (�Ώ�Key�̌^�� INI_TYPE_STR �̏ꍇ�� NULL ���w��j
    
    @param: void *val_save
    iniSave�����s����ۂɁA�ۑ�����l���Q�Ƃ���ϐ��ւ̃|�C���^�[
    (�Ώ�Key�̌^�� INI_TYPE_STR �̏ꍇ�� NULL ���w��j

    @param: int id
	Key�Ɋ֘A�t����ID(�C��)
	
    @return: true = �����Afalse = ���s�i�z��̃T�C�Y������Ȃ��A���ɑ��݂���Key�A�����Ȍ^)
    
    ��void *val_load ,void *val_save �͂��ꂼ��
	�@�w�肷�� int_type �ɂ���ĕϐ��̌^�����܂��Ă��܂�
    
    *bool	= INI_TYPE_BOOL
    *int  	= INI_TYPE_DEC / INI_TYPE_OCT / INI_TPYE_HEX
    *double	= INI_TYPE_DOUBLE
 */
bool iniAddKey( ini_data *data ,const char *key ,ini_type type ,void *val_load ,void *val_save, int id );

/*	iniSetVal
	���Ƀ��X�g�֒ǉ��ς݂�(iniAddKey����) Key�ɑ΂���
	iniLoad / iniSave �ŎQ�Ƃ���� void *val_load ,void *val_save ���Đݒ肷��
    
    @param: ini_data *data
    �Ώۂ�ini_data�ϐ�
    
    @param: const char *key
    �ǉ��ς݂�Key
    
    @param: void *val_load
    iniLoad�����s����ۂɁA�擾�����l��ۑ�����ϐ��̃|�C���^�[
    (�Ώ�Key�̌^�� INI_TYPE_STR �̏ꍇ�� NULL ���w��j
    
    @param: void *val_save
    iniSave�����s����ۂɁA�ۑ�����l���Q�Ƃ���ϐ��ւ̃|�C���^�[
    (�Ώ�Key�̌^�� INI_TYPE_STR �̏ꍇ�� NULL ���w��j

    @param: int id
	Key�Ɋ֘A�t����ID(�C��)
	
    @return: true = �����Afalse = ���s�iKey�����݂��Ȃ�)
    
    ��void *val_load ,void *val_save �͂��ꂼ��
	�@�w�肵�� int_type �ɂ���ĕϐ��̌^�����܂��Ă��܂�
    
    *bool	= INI_TYPE_BOOL
    *int  	= INI_TYPE_DEC / INI_TYPE_OCT / INI_TPYE_HEX
    *double	= INI_TYPE_DOUBLE
 */
bool iniSetVal( ini_data *data ,const char *key ,void *val_load ,void *val_save, int id );


/*	iniRmvKey
  	data(ini_data*)����Key���폜�i�����Ɂj����
    
    @param: ini_data *data
    �Ώۂ�ini_data�ϐ�
    
    @param: const char *key
    �폜�Ώۂ�Key
	
    @return: true = �����Afalse = ���s�iKey�����݂��Ȃ�)
 */
bool iniRmvKey( ini_data *data ,const char *key );



/*	iniLoad
  	�t�@�C������ݒ���擾����
  	
  	data(ini_data*)�Ŏw�肳��Ă���eKey�̃f�[�^�^�ɉ����Ēl���擾
    
    @param: const char *path
    �ǂݍ��ރt�@�C����
    
    @param: ini_data *data
    �Ώۂ�ini_data�ϐ�
    
    @param: libini_cbl callback_load
    �l��������^��Key����������R�[���o�b�N�֐�
    �����Ă���Key�ɉ�����Value��ϊ�����
    ���t�@�C���ɑΏ�Key�̋L�q���Ȃ���ΌĂяo����Ȃ�
    
    [�R�[���o�b�N�֐��֑����Ă���l]
    const char *key		= ��������Key��
    char *val			= ��������Value
    void *opt			= �R�[���o�b�N�֓n���ꂽ�f�[�^�iiniLoad�Ŏw�肵��callback_opt)
	ini_pair *pair		= ��������Key���(�ڍ�)

    ��������𐳂��������o�������ǂ������Abool�^�Ŗ߂�l�Ƃ��ĕԋp
	
    @param: void *callback_opt
    �R�[���o�b�N�֐��֓n���f�[�^
    �R�[���o�b�N���w��Ȃ�NULL
	
    @return: iniAddKey�Œǉ�����Key���Ɣ�ׂāA���ۂɃt�@�C������擾�o�����ݒ萔
	
    0 = �t�@�C�������݂��Ȃ��A�����̐ݒ�s���A����ȊO = ���炩�̐ݒ肪�擾�o���Ă���
   	
  	��Key/Value�̎n�܂�E�I���͑O��̃^�u(\t)�A���p/�S�p�X�y�[�X�����O�����ʒu�Ŏ���
  	�@�啶���������͋�ʂ����ɔ�r���܂��iINI_TYPE_STR�̂ݎg�p�҂Ŕ��f�j
  	�@
  	�@�l�������l��Key��iniAddKey�Ō^(8�i��/10�i��/16�i��)���w�肵�Ȃ���
  	�@�������l���擾����Ȃ��̂Œ���
 */
int iniLoad( const char *path ,ini_data *data ,libini_cbl callback_load ,void *callback_opt );

/*	iniSave
	�t�@�C���֐ݒ��ۑ�����
	
  	data(ini_data*)�Ŏw�肳��Ă���Key/Value��
  	���ꂼ��̃f�[�^�^�ɉ����ăt�@�C���֕ۑ�����
    
    @param: const char *path
    �ۑ�����t�@�C����
    
    @param: ini_data *data
    �Ώۂ�ini_data�ϐ�
    
    @param: libini_cbs callback_save
    �l��������^��Key����������R�[���o�b�N�֐�
    �����Ă���Key�ɉ����ăt�@�C���֏������ޕ������ԋp
    ���t�@�C���ɑΏ�Key�̋L�q���Ȃ���ΌĂяo����Ȃ�
	
	[�R�[���o�b�N�֐��֑����Ă���l]
    const char *key		= ��������Key��
    void *opt			= �R�[���o�b�N�֓n���ꂽ�f�[�^�iiniSave�Ŏw�肵��callback_opt)
	ini_pair *pair		= ��������Key���(�ڍ�)

   ���C�ӂ̕�����ԋp�����Key���ɑ΂��Ă��̓��e���������܂��ANULL��ԋp����Ə������܂�܂���
	
    @param: void *callback_opt
    �R�[���o�b�N�֐��֓n���f�[�^
    �R�[���o�b�N���w��Ȃ�NULL
	
    @return: true = �����Afalse = ���s�i�󂫗e�ʕs�����Ńt�@�C�����������߂Ȃ��A�����̐ݒ�s���j
   	
   	���ۑ���t�@�C�������ɑ��݂���ꍇ��
   	�@�ΏۊO�̕������ۂ����܂܏㏑�������̂Ŗ��Ȃ�
   	�@
  	�@�l�������l��Key��iniAddKey�Ō^(8�i��/10�i��/16�i��)���w�肵�Ȃ���
  	�@��]�����Ԃŕۑ�����Ȃ��̂Œ���
*/
bool iniSave( const char *path ,ini_data *data ,libini_cbs callback_save ,void *callback_opt );


/*
	iniCounterPbrk
	������(src)�̐擪���當���Q(search)���܂܂�Ȃ��ŏ��̈ʒu��Ԃ�
	
    @param: const char * src
    �������̕�����
    
    @param: const char *search
    �����Ώۂ̕�����
	
	@return: �����Q���܂܂�Ȃ��ŏ��̈ʒu�i�擪����j
	NULL = ���݂��Ȃ�
*/
char* iniCounterPbrk( const char *src, const char *search );

/*
	iniCounterReversePbrk
	������(src)�̖������當���Q(search)���܂܂�Ȃ��ŏ��̈ʒu��Ԃ�
	
    @param: const char * src
    �������̕�����
    
    @param: const char *search
    �����Ώۂ̕�����
	
	@return: �����Q���܂܂�Ȃ��ŏ��̈ʒu�i��������j
	NULL = ���݂��Ȃ�
*/
char* iniCounterReversePbrk( const char *src, const char *search );

/*
	iniIsLoad
	iniLoad�Œl��ǂݍ��߂����ݒ育�ƂɊm�F
	(iniLoad�̖߂�l�͓ǂݍ��߂��ݒ萔�����Ȃ̂ŁA�����⏕������́j
	
    @param: ini_data *data
    �Ώۂ�ini_data�ϐ�
    
    @param: const char *key
    �Ώۂ�Key
	
	@return: true = �����Afalse = ���s
	
	��iniLoad������Ɏ��s
*/
int	iniIsLoad( ini_data *data, const char *key );



/*
	iniGetArraySize
	data->size����data->pair�z��̗v�f�����擾����
	(data->pair�z��̐ݒ�󋵂��`�F�b�N�j
	
	@param: ini_data *data
    �Ώۂ�ini_data�ϐ�
	
	@return: data->pair�z��̗v�f��
	��data->size�̒l���ُ�ȏꍇ��0
*/
int iniGetArraySize( ini_data *data );




/*	�J�[�l�����[�h�̃v���O�C���֓��������
	�⊮���K�v��3�̕W�����C�u�����֐�
*/

extern int snprintf( char *buf, size_t n, const char *fmt, ... );
extern int strcasecmp( const char *s1, const char *s2 );
extern double strtod(const char *s, char **eptr);


#ifdef __cplusplus
}
#endif

#endif

