#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "FrenchKISS.h"
#include "types.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
#ifdef i486
#define SCREEN_PITCH	240
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	320
#elif defined(_X86_)
#define SCREEN_PITCH	240
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	320
#else
#define SCREEN_PITCH	256
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	320
#endif
#define SCP	SCREEN_PITCH
#define SCW	SCREEN_WIDTH
#define SCH	SCREEN_HEIGHT
/*==============================================================================================================*/
#define BLT_COPY	0x0
#define BLT_MSK		0x1
#define BLT_AND		0x2
/*==============================================================================================================*/
#define CLR_ZERO				0x0
#define CLR_BLUR				0x1
#define CLR_IMAGE				0x2
#define CLR_IMAGE_BLUR	0x3
/*==============================================================================================================*/
/*==============================================================================================================*/
typedef struct tagIMAGE{
	unsigned short	w;
	unsigned short	h;
	unsigned short	sLeft, sRight, sUp, sDown;
	unsigned short	*pData;
}IMAGE;
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API void VID_Init(void);
FK_API void VID_Quit(void);
FK_API void VID_FillRect_i(IMAGE* p_imageDst, int left, int top, int right, int bottom, unsigned short lColor);
FK_API void VID_FillBitmap(IMAGE* p_imageDst, unsigned short lColor);
FK_API void VID_Flip(void);
FK_API void VID_Clear(IMAGE *pDestBitmap, int mode, unsigned int extParam);
FK_API void VID_PutPixel(IMAGE *pDes, uint nX, uint nY, ushort lColor);
FK_API void VID_LoadTGA(IMAGE **ppBitmap, ushort **bufPixel, cstring szBitmap);
FK_API void VID_LoadRAW565(IMAGE **ppBitmap, ushort **bufPixel, cstring szBitmap);
FK_API void VID_Blt(IMAGE *pDes, int nDx, int nDy, int nDhw, int nDhh, int nMode, IMAGE *pSrc, int nSx, int nSy, int nSw, int nSh);
/*==============================================================================================================*/
/*==============================================================================================================*/
#endif /*_GRAPHIC_H_*/
