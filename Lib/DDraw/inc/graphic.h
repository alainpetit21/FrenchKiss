#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_
/*=========================*/
/*=========================*/
#include "types.h"
#include "Lib.h"
#include <ddraw.h>
/*=========================*/
/*=========================*/
#define SCREEN_PITCH	640
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SCP	SCREEN_PITCH
#define SCW	SCREEN_WIDTH
#define SCH	SCREEN_HEIGHT
/*=========================*/
#define BLT_COPY	0x0
#define BLT_MSK	0x1
#define BLT_AND	0x2
/*=========================*/
#define CLR_ZERO		0x0
#define CLR_BLUR		0x1
#define CLR_IMAGE		0x2
#define CLR_IMAGE_BLUR	0x3
/*=========================*/
/*=========================*/
typedef IDirectDrawSurface*	IMAGE;
/*=========================*/
typedef struct tagTEXTURE{
	unsigned short	w;
	unsigned short	h;
	unsigned short	*pData;
}TEXTURE;
/*=========================*/
/*=========================*/
LIB_API void	VID_Init(int w, int h, int bbp, int fs);
LIB_API void	VID_Quit(void);
LIB_API void	VID_FillRect_i(IDirectDrawSurface* p_imageDst, int left, int top, int right, int bottom, unsigned short lColor);
LIB_API void	VID_FillBitmap(IDirectDrawSurface* p_imageDst, unsigned short lColor);
LIB_API void	VID_Flip(void);
LIB_API void	VID_Clear(IDirectDrawSurface* pDestBitmap, int mode, unsigned int extParam);
LIB_API void	VID_PutPixel(IDirectDrawSurface* pDes, uint nX, uint nY, ushort lColor);
LIB_API void	VID_LoadTGA(IDirectDrawSurface*** ppBitmap, ushort **bufPixel, cstring szBitmap);
LIB_API void	VID_LoadTGATexture(TEXTURE** ppBitmap, ushort **bufPixel, cstring szBitmap);
LIB_API void	VID_Blt(IDirectDrawSurface* pDes, int nDx, int nDy, int nDhw, int nDhh, int nMode, IDirectDrawSurface* pSrc, int nSx, int nSy, int nSw, int nSh);
LIB_API ushort* VID_Lock(IDirectDrawSurface* ppBitmap, int mode);
LIB_API void	VID_Unlock(IDirectDrawSurface* ppBitmap);
/*=========================*/
/*=========================*/
#endif /*_GRAPHIC_H_*/