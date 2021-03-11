#ifndef PB_COLOR_H
#define PB_COLOR_H

#define	SetAlpha(color,alpha)	((color & 0xffffff) | ((alpha & 0xff) << 24))
#define	GetAlpha(color)			((color & 0xff000000) >> 24)


#define	BLACK			0xff000000          //  ���F
#define	WHITE			0xffFFFFFF          //  ���F

#define	RED				0xff0000FF          //  �ԐF
#define	GREEN			0xff00FF00          //  �ΐF
#define	BLUE			0xffFF0000          //  �F

#define	DARKBLUE		0xff800000          //  ���F
#define	LIGHTBLUE		0xffFA0000          //  ���F
#define	BROWN			0xff404080          //  ���F
#define	PINK			0xffFF80FF          //  ���F
#define	DARKGREEN		0xff008000          //  �[�ΐF
#define	YELLOGREEN		0xff00FF80          //  ���ΐF
#define	PURPLE			0xffFF0080          //  ���F
#define	ORANGE			0xff4080FF          //  ��F
#define	LEMON			0xff80FFFF          //  �������F
#define	YELLOW			0xff00FFFF          //  ���F
#define	SKYBLUE			0xffFFFF00          //  ��F
#define	PEARLORANGE		0xff75BAFF          //  ���F
#define	GRAY			0xff808080          //  �D�F
#define	SILVER			0xffC0C0C0          //  ��F
#define	GOLD			0xff20C0E6          //  ���F


#define	Invalid			LIBM_NO_DRAW


//    32�r�b�g�J���[��16�i���R�[�h�� �q�f�a����쐬����v�Z��
//#define  RGB(r,g,b) (((long)b<<16)|((long)g<<8)|((long)r)

#endif
