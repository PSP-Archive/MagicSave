#ifndef MS_FUNC_H
#define MS_FUNC_H



#ifdef __cplusplus
extern "C" {
#endif

enum
{
	SFORMAT_AUTO = 0,
	SFORMAT_RAW,
	SFORMAT_NORMAL,
};


// �Z�[�u�f�[�^�̃Z�[�u�t�H�[�}�b�g���擾
// 0 = Auto , 1 = Raw , 2= Normal
int	MS_Mode_Status();


#ifdef __cplusplus
}
#endif

#endif
