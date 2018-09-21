#include "stdafx.h"
#include <memory.h>
#include "graphic.h"
#include "gx.h"
#include "macros.h"
#include "types.h"
#include "file.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
extern HWND	ghWnd;
/*==============================================================================================================*/
/*==============================================================================================================*/
ushort	bbData[SCREEN_PITCH*SCREEN_HEIGHT];
FK_API IMAGE		fb;
FK_API IMAGE		bb;
GXDisplayProperties	gGxDP;
/*==============================================================================================================*/
/*==============================================================================================================*/
int
VID_IApplyScisor(IMAGE* pDes, int& nDx, int& nDy, int& nDw, int& nDh, int& nSx, int& nSy, int& nSw, int& nSh) 
{
  int ret= 0;                                          

  if (nDx+nDw <= pDes->sLeft)	return ret;		//	+------			
  if (nDy+nDh <= pDes->sUp)		return ret;		//	|				Clip this part
																						//	|					
  if (nDx < pDes->sLeft){										//	|					
		nSx += pDes->sLeft;									
		nSw -= (pDes->sLeft-nDx);									
		nDw = nSw;									
		nDx = pDes->sLeft;									
  }
  if (nDy < pDes->sUp){
		nSy += pDes->sUp;									
		nSh -= (pDes->sUp-nDy);									
		nDh = nSh;									
		nDy = pDes->sUp;									
  }

  if (nDx >= pDes->sRight)	return 0;				//	|
  if (nDy >= pDes->sDown)		return 0;				//	|		Clip this part
																						//	|		
  int sortX	= (nDx + nDw) - pDes->sRight		//	------+		
		, sortY	= (nDy + nDh) - pDes->sDown;

  if (sortX > 0){	
		nSw -= sortX;
		nDw -= sortX;
  }
  if (sortY > 0){	
		nSh -= sortY;
		nDh -= sortY;
  }
  return 1;
}
/*==============================================================================================================*/
FK_API void 
VID_Init()
{
	GXOpenDisplay(ghWnd, GX_FULLSCREEN);
	gGxDP			= GXGetDisplayProperties();

	fb.pData	= (ushort*)GXBeginDraw();
	fb.w			= (ushort)gGxDP.cbyPitch/(ushort)gGxDP.cbxPitch;
	fb.h			= (ushort)gGxDP.cyHeight;
	fb.sLeft	= 0;
	fb.sUp		= 0;
	fb.sRight	= fb.w;
	fb.sDown	= fb.h;
	GXEndDraw();

	bb.pData	= bbData;
	bb.w			= (ushort)gGxDP.cbyPitch/(ushort)gGxDP.cbxPitch;
	bb.h			= (ushort)gGxDP.cyHeight;
	bb.sLeft	= 0;
	bb.sUp		= 0;
	bb.sRight	= bb.w;
	bb.sDown	= bb.h;
}
/*==============================================================================================================*/
FK_API void 
VID_Quit()
{
  GXCloseDisplay();
}
/*==============================================================================================================*/
FK_API void 
VID_FillRect_i(IMAGE* pDes, int left, int top, int right, int bottom, ushort lColor)
{
  ushort	*ptrD	= pDes->pData;
  ushort	*ptrD2;
  int	w	= right-left;
  int	w2	= w;
  int	h	= bottom-top;

  if(!(VID_IApplyScisor(pDes, left, top, w, h, left, top, w, h))) return;

  ptrD+= (bottom*SCREEN_PITCH) + left;
	
  while(h--){
    ptrD2= ptrD;
    w2= w;
    while(w2--){
      *ptrD2++= lColor;
    }
    ptrD-= SCREEN_PITCH;
  }
}
/*==============================================================================================================*/
FK_API void
VID_FillBitmap(IMAGE* pDes, ushort lColor)
{
  VID_FillRect_i(pDes, 0, 0, 
  pDes->w, pDes->h-1, lColor);
}
/*==============================================================================================================*/
FK_API ushort
VID_GetPixel(IMAGE *pDes, uint nX, uint nY)
{
  return *(pDes->pData + (nY*pDes->w) + nX);
}
/*==============================================================================================================*/
FK_API void 
VID_PutPixel(IMAGE *pDes, uint nX, uint nY, ushort lColor)
{
  *(pDes->pData+(nY*pDes->w)+nX)=	lColor;
}
/*==============================================================================================================*/
FK_API void
VID_Blt(IMAGE *pDes, int nDx, int nDy, int nDhw, int nDhh, int nMode, IMAGE *pSrc, int nSx, int nSy, int nSw, int nSh)
{
  if(!(VID_IApplyScisor(pDes, nDx, nDy, nDhw, nDhh, nSx, nSy, nSw, nSh))) return;

  ushort* ptrD=	pDes->pData+ (nDy*pDes->w)+nDx;
  ushort* ptrS=	pSrc->pData + (nSy*pSrc->w) + nSx;
  ushort* ptrD2;
  ushort* ptrS2;
  int 	i= nSh;
  int	j=0;
  int	lDPitch= pDes->w
    ,	lSPitch= pSrc->w;

  switch(nMode){
    case BLT_COPY:
      while(i--){
        memcpy(ptrD, ptrS, nSw<<1);
        ptrD+= lDPitch;
        ptrS+= lSPitch;
      }
      break;
    case BLT_MSK:
      while(i--){
        j		= nSw;
        ptrD2	= ptrD;
        ptrS2	= ptrS;
        while(j--){
          if(*ptrS2 != 0x07E0)	
            (*ptrD2)= (*ptrS2);
          ptrD2++;
          ptrS2++;
        }
        ptrD+= lDPitch;
        ptrS+= lSPitch;
      }
      break;
    case BLT_AND:
      while(i--){
        j= nSw;
        while(j--){
          ushort	pS;
          unsigned char	pSR;
          unsigned char	pSG;
          unsigned char	pSB;
          ushort	pD;
          unsigned char	pDR;
          unsigned char	pDG;
          unsigned char	pDB;
          ushort	pRes;
          unsigned char	pResR;
          unsigned char	pResG;
          unsigned char	pResB;

          pS= VID_GetPixel(pSrc, i, j);
          pD= VID_GetPixel(pDes, nDx+i, nDy+j);
          pSR= (pS&0xF800)>>11;
          pDR= (pD&0xF800)>>11;
          pSG= (pS&0x07E0)>>5;
          pDG= (pD&0x07E0)>>5;
          pSB= (pS&0x001F);
          pDB= (pD&0x001F);
          pResR= 	(((pSR*8)>>5)+(((pDR*(32-8))>>5)))&0xFF;
          pResG=	(((pSG*8)>>5)+(((pDG*(32-8))>>5)))&0xFF;
          pResB=	(((pSB*8)>>5)+(((pDB*(32-8))>>5)))&0xFF;
          pRes= 	(pResR<<11)|(pResG<<5)|pResB;
          VID_PutPixel(&bb, nDx+i, nDy+j, pRes);
        }
      }
      break;
    }
}
/*==============================================================================================================*/
FK_API void
VID_Flip()
{
  VERIFY((fb.pData= (ushort*)GXBeginDraw()), _T"Unable To Get FB");
  VID_Blt(&fb, 0, 0, 240, 320, BLT_COPY, &bb, 0, 0, 240, 320);
  GXEndDraw();
}
/*==============================================================================================================*/
FK_API void
VID_Clear(IMAGE *pDes, int mode, uint extParam)
{
  switch(mode){
    case CLR_ZERO:
      memset(pDes->pData, 0x0, 2*SCREEN_PITCH*SCREEN_HEIGHT);
      break;
    case CLR_BLUR:{
      uint	c= SCREEN_PITCH*SCREEN_HEIGHT;
      register ushort* ptr= pDes->pData;
      register ushort pixel;
      while(c--){
        pixel	=	*ptr;
        *ptr++	=	(((((pixel&0xF800)>>1)*extParam)>>20)<<11)|
									(((((pixel&0x07E0)<<4)*extParam)>>20)<<6)|
									(((((pixel&0x001F)<<10)*extParam)>>20));
      }
    }
    case CLR_IMAGE:{
    }
    break;
    case CLR_IMAGE_BLUR:{
      uint	c= SCREEN_PITCH*SCREEN_HEIGHT;
      register ushort*	ptrD= pDes->pData;
      register ushort*	ptrS= ((IMAGE*)extParam)->pData;
      register ushort	pixelS;
      register ushort	pixelD;
			register uchar	pRS;
			register uchar	pGS;
			register uchar	pBS;
			register uchar	pRD;
			register uchar	pGD;
			register uchar	pBD;
      while(c--){
        pixelS	=	*ptrS++;
        pixelD	=	*ptrD;
				pRS		=	(pixelS&0xF800)>>12;
				pGS		=	(pixelS&0x07E0)>>6;
				pBS		=	(pixelS&0x001F)>>1;
				pRD		=	(pixelD&0xF800)>>12;
				pGD		=	(pixelD&0x07E0)>>6;
				pBD		=	(pixelD&0x001F)>>1;
				pRD		+=	pRS;
				pGD		+=	pGS;
				pBD		+=	pBS;
				*ptrD++	= (pRD<<11)|(pGD<<6)|(pBD);
      }
    }
    break;
  }
}
/*==============================================================================================================*/
/*==============================================================================================================*/
#pragma pack( push, forTGA, 1)
typedef struct tagTGA_HDR
{
	unsigned char		nbCharId;
	unsigned char		cmType;
	unsigned char		imgType;
	unsigned short	idxCMFirstEntry;
	unsigned short	idxCMLenght;
	unsigned char		idxCMEntryLenght;
	unsigned short	imgXOrigin;
	unsigned short	imgYOrigin;
	unsigned short	imgWidth;
	unsigned short	imgHeight;
	unsigned char		imgPxSize;
	unsigned char		imgDesc;
}TGA_HDR;
#pragma pack( pop, forTGA )

typedef struct _BIT888
{
	unsigned char b,g,r;
}BIT888;
/*==============================================================================================================*/
/*==============================================================================================================*/
#define RED565(x)		(x<<11)
#define GREEN565(x)	(x<<5)
#define BLUE565(x)	(x)
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API void
VID_LoadTGA(IMAGE **ppBitmap, ushort **bufPixel, cstring szBitmap)
{
	PTFILE fichierR;
	TGA_HDR	tgaHeader;
	int w,h;
	unsigned short	buf565;
	BIT888 buf888;

	fichierR= FS_CreateFile((string)szBitmap, OPT_READ);
	FS_ReadFile(fichierR, &tgaHeader, sizeof(TGA_HDR));
	w= tgaHeader.imgWidth;
	h= tgaHeader.imgHeight;

	(*ppBitmap)->w= w;
	(*ppBitmap)->h= h;
	(*ppBitmap)->pData= (*bufPixel);

	for(int i= 0; i<h; ++i){
		for(int j= 0; j<w; ++j){
//			FS_ReadFile(fichierR, &buf888, sizeof(BIT888));
			buf888.r= i;
			buf888.g= j;
			buf888.b= 0;

			buf565= 0;
			buf565|= RED565((buf888.r<<5)>>8);
			buf565|= GREEN565((buf888.g<<6)>>8);
			buf565|= BLUE565((buf888.b<<5)>>8);
			VID_PutPixel(*ppBitmap, j, i, buf565);
		}
	}
	FS_CloseFile(fichierR);
	(*ppBitmap)++;
	(*bufPixel)+= w*h;
}
/*==============================================================================================================*/
/*==============================================================================================================*/
