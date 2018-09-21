#include <windows.h>
#include <memory.h>
#include "graphic.h"
#include "macros.h"
#include "types.h"
#include "file.h"

IDirectDrawSurface			*fb				;
LIB_API IDirectDrawSurface	*bb				;
IDirectDrawSurface			*tb				;
IDirectDraw					*lpDD			; 
IDirectDrawClipper			*Clipper		; 
IDirectDrawClipper			*WindowClipper	; 
DDBLTFX						BlitFX			;
DDSURFACEDESC				ddsd			;
DDSCAPS						ddscaps			;
DDPIXELFORMAT				ddpf			;
int							PixelFormat		;
int							nWidth			;
int							nHeight			;
int							bFullScreen		;
int							WindowClientY0	;
int							WindowClientX0	;
							
extern HWND					ghWnd			;

LIB_API IDirectDrawClipper*	VID_AttachClipper(IDirectDrawSurface *surface, int RectCount, RECT* ClipList);

LIB_API int
VID_BitmapDoClipping(int& nDx, int& nDy, int& nDw, int& nDh, int& nSx, int& nSy, int& nSw, int& nSh) 
{  
  int ret= 0;                                          

//	+------			
//	|				Clip this part
//	|					
//	|					
  if (nDx+nDw <= 0)	return ret;
  if (nDy+nDh <= 0)	return ret;
	
  if (nDx < 0){
    nSx -= nDx;
    nSw += nDx;
    nDx = 0;
    ret|= 1;
  }
  if (nDy < 0){
    nSy -= nDy;
    nSh += nDy;
    nDy = 0;
    ret|= 1;
  }

//	 	  |
//	 	  |		Clip this part
//	 	  |		
//	------+		
  if (nDx >= nWidth)	return 0;
  if (nDy >= nHeight)	return 0;

  int sortX= (nDx + nDw) - nWidth
  , sortY= (nDy + nDh) - nHeight;

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

LIB_API void 
VID_NewBitmap(IDirectDrawSurface **ppBitmap, uint nWidth, uint nHeight)
{
	DDSURFACEDESC TempDescription;
    DDCOLORKEY    ddck;

	TempDescription.dwSize = sizeof(TempDescription);
	TempDescription.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	TempDescription.dwWidth = nWidth;
	TempDescription.dwHeight = nHeight;

	TempDescription.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	lpDD->CreateSurface(&TempDescription, ppBitmap,NULL);

    ddck.dwColorSpaceLowValue  = 0;
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    (*ppBitmap)->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

LIB_API void 
VID_Init(int w, int h, int bbp, int fs)
{
  RECT ScreenRect;
  bFullScreen	= fs;
  nWidth		= w;
  nHeight		= h;

  memset(&ddsd,0,sizeof(ddsd));
  memset(&ddpf,0,sizeof(ddpf));
  memset(&BlitFX, 0, sizeof(BlitFX));
  ddsd.dwSize = sizeof(ddsd);
  ddpf.dwSize = sizeof(ddpf);
  BlitFX.dwSize = sizeof(BlitFX);

  DirectDrawCreate(NULL, &lpDD, NULL);
  if(!bFullScreen){
    lpDD->SetCooperativeLevel(ghWnd, DDSCL_NORMAL);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddsd.dwBackBufferCount = 0;
  }else{
		lpDD->SetCooperativeLevel(ghWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
		lpDD->SetDisplayMode(nWidth, nHeight, bbp);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE  | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
		ddsd.dwBackBufferCount = 1;
	}
	lpDD->CreateSurface(&ddsd,&fb,NULL);
	fb->GetPixelFormat(&ddpf);
	PixelFormat = ddpf.dwRGBBitCount;

	if (bFullScreen){
		ddsd.dwFlags			= DDSD_CAPS;
		ddsd.ddsCaps.dwCaps		= DDSCAPS_BACKBUFFER;
		ddsd.dwBackBufferCount	= 1;
 		fb->GetAttachedSurface(&ddsd.ddsCaps,&bb);
		VID_FillBitmap(bb,0x00FF);
		VID_FillBitmap(fb,0xFFFF);
	}else{
		VID_NewBitmap(&bb, nWidth, nHeight);
		VID_FillBitmap(bb,0x00FF);
	}
	VID_NewBitmap(&tb, nWidth, nHeight);
	VID_FillBitmap(tb,0xFFFF);

	ScreenRect.left	= 0;
	ScreenRect.top	= 0;
	ScreenRect.right= nWidth;
	ScreenRect.bottom=nHeight;

	Clipper = VID_AttachClipper(bb,1,&ScreenRect);
	lpDD->CreateClipper(0,&WindowClipper,NULL);
	WindowClipper->SetHWnd(0, ghWnd);
	if (!bFullScreen){
		WindowClientX0= -ScreenRect.left;
		WindowClientY0= -ScreenRect.top;
		fb->SetClipper(WindowClipper);
	}
}

LIB_API void 
VID_Quit()
{
	if (Clipper)		{Clipper->Release();		Clipper			=0;}
	if (WindowClipper)	{WindowClipper->Release();	WindowClipper	=0;}
	if (tb)				{tb->Release();				tb				=0;}
	if (bb)				{bb->Release();				bb				=0;}
	if (fb)				{fb->Release();				fb				=0;}
	if (lpDD)			{lpDD->Release();			lpDD			=0;}
}

LIB_API void 
VID_FillRect_i(IDirectDrawSurface* pDes, int left, int top, int right, int bottom, ushort lColor)
{
	RECT rec= {left, top, right, bottom};
	BlitFX.dwFillColor = lColor;
	int ret= pDes->Blt(&rec,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&BlitFX);
}

LIB_API void
VID_FillBitmap(IDirectDrawSurface* pDes, ushort lColor)
{
	BlitFX.dwFillColor = lColor;
	pDes->Blt(NULL,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&BlitFX);
}

LIB_API ushort
VID_GetPixel_locked(void* pDes, uint nX, uint nY)
{
  return 0;
}

LIB_API void 
VID_PutPixel_locked(void* pDes, uint nX, uint nY, ushort lColor)
{

}

LIB_API void
VID_Blt(IDirectDrawSurface* pDes, int nDx, int nDy, int nDhw, int nDhh, int nMode, IDirectDrawSurface* pSrc, int nSx, int nSy, int nSw, int nSh)
{
  if(!(VID_BitmapDoClipping(nDx, nDy, nDhw, nDhh, nSx, nSy, nSw, nSh))) return;

  RECT dstRect={nDx, nDy, nDx+nDhw, nDy+nDhh};
  RECT srcRect={nSx, nSy, nSx+nSw, nSy+nSh};


  switch(nMode){
    case BLT_COPY:
		pDes->Blt(&dstRect, pSrc, &srcRect, DDBLT_WAIT, NULL);
      break;
    case BLT_MSK:
		pDes->Blt(&dstRect, pSrc, &srcRect, DDBLT_WAIT|DDBLT_KEYSRC, NULL);
      break;
    case BLT_AND:
      break;
    }
}

LIB_API void
VID_Flip()
{
RECT dest= {0, 0, nWidth, nHeight};
int ret;
	if (bFullScreen){
		fb->Blt(&dest, bb, NULL, DDBLT_WAIT,NULL);
	}
	else{
		GetWindowRect(ghWnd, &dest);
		dest.left	+= WindowClientX0;
		dest.top	+= WindowClientY0;
		dest.right	= dest.left+ nWidth;
		dest.bottom = dest.top + nHeight;
		ret= fb->Blt(&dest, bb, NULL, DDBLT_WAIT,NULL);
	}
}

LIB_API void
VID_Clear(IDirectDrawSurface* pDes, int mode, uint extParam)
{
  switch(mode){
    case CLR_ZERO:
	  VID_FillBitmap(bb, 0);
      break;
    case CLR_BLUR:{
    }
    case CLR_IMAGE:{
    }
    break;
    case CLR_IMAGE_BLUR:{
    }
    break;
  }
}

#pragma pack( push, forTGA, 1)
typedef struct tagTGA_HDR
{
	unsigned char	nbCharId;
	unsigned char	cmType;
	unsigned char	imgType;
	unsigned short	idxCMFirstEntry;
	unsigned short	idxCMLenght;
	unsigned char	idxCMEntryLenght;
	unsigned short	imgXOrigin;
	unsigned short	imgYOrigin;
	unsigned short	imgWidth;
	unsigned short	imgHeight;
	unsigned char	imgPxSize;
	unsigned char	imgDesc;
}TGA_HDR;
typedef struct _BIT888
{
	unsigned char b,g,r;
}BIT888;
#pragma pack( pop, forTGA )

#define RED565(x)	(x<<11)
#define GREEN565(x)	(x<<5)
#define BLUE565(x)	(x)

LIB_API ushort* 
VID_Lock(IDirectDrawSurface* ppBitmap, int mode)
{
	ppBitmap->Lock(0, &ddsd, mode|DDLOCK_WAIT, 0);
	return(ushort*)ddsd.lpSurface;
}

LIB_API void 
VID_Unlock(IDirectDrawSurface* ppBitmap)
{
	ppBitmap->Unlock(0);
}

LIB_API void
VID_LoadTGA(IDirectDrawSurface*** ppBitmap, ushort **bufPixel, cstring szBitmap)
{
	PTFILE fichierR;
	TGA_HDR	tgaHeader;
	int w,h;
	unsigned short	buf565;
	ushort	*ptrDst, *ptrDst2;
	BIT888 buf888;

	fichierR= FS_CreateFile((string)szBitmap, OPT_READ);
	FS_ReadFile(fichierR, &tgaHeader, sizeof(TGA_HDR));
	w= tgaHeader.imgWidth;
	h= tgaHeader.imgHeight;

	VID_NewBitmap(*ppBitmap, w, h);
	VID_FillBitmap(**ppBitmap, 0);
	ptrDst= VID_Lock(**ppBitmap, DDLOCK_WRITEONLY);
	for(int i= 0; i<h; ++i){
		ptrDst2= ptrDst;
		for(int j= 0; j<w; ++j){
			FS_ReadFile(fichierR, &buf888, sizeof(BIT888));

			buf565= 0;
			buf565|= RED565((buf888.r<<5)>>8);
			buf565|= GREEN565((buf888.g<<6)>>8);
			buf565|= BLUE565((buf888.b<<5)>>8);
			*ptrDst2++= buf565;
		}
		ptrDst+= ddsd.lPitch>>1;
	}
	FS_CloseFile(fichierR);
	VID_Unlock(**ppBitmap);
	(*ppBitmap)++;
}

LIB_API void
VID_LoadTGATexture(TEXTURE** ppBitmap, ushort **bufPixel, cstring szBitmap)
{
	PTFILE fichierR;
	TGA_HDR	tgaHeader;
	int w,h;
	unsigned short	buf565;
	ushort	*ptrDst, *ptrDst2;
	BIT888 buf888;

	fichierR= FS_CreateFile((string)szBitmap, OPT_READ);
	FS_ReadFile(fichierR, &tgaHeader, sizeof(TGA_HDR));
	w= tgaHeader.imgWidth;
	h= tgaHeader.imgHeight;

	(*ppBitmap)->pData= (*bufPixel);
	ptrDst= (*ppBitmap)->pData;
	for(int i= 0; i<h; ++i){
		ptrDst2= ptrDst;
		for(int j= 0; j<w; ++j){
			FS_ReadFile(fichierR, &buf888, sizeof(BIT888));

			buf565= 0;
			buf565|= RED565((buf888.r<<5)>>8);
			buf565|= GREEN565((buf888.g<<6)>>8);
			buf565|= BLUE565((buf888.b<<5)>>8);
			*ptrDst2++= buf565;
		}
		ptrDst+= w;
	}
	FS_CloseFile(fichierR);
	(*ppBitmap)++;
	(*bufPixel)+= w*h;
}

LIB_API IDirectDrawClipper*
VID_AttachClipper(IDirectDrawSurface *surface, int RectCount, RECT* ClipList)
{
	int Count;
	LPDIRECTDRAWCLIPPER Clipper;
	LPRGNDATA RegionData;

	lpDD->CreateClipper(0,&Clipper,NULL); 

	RegionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+(RectCount*sizeof(RECT)));
	memcpy(RegionData->Buffer, ClipList, sizeof(RECT)*RectCount);

	RegionData->rdh.dwSize          = sizeof(RGNDATAHEADER);
	RegionData->rdh.iType           = RDH_RECTANGLES;
	RegionData->rdh.nCount          = RectCount;
	RegionData->rdh.nRgnSize        = RectCount*sizeof(RECT);

	RegionData->rdh.rcBound.left    =  64000;
	RegionData->rdh.rcBound.top     =  64000;
	RegionData->rdh.rcBound.right   = -64000;
	RegionData->rdh.rcBound.bottom  = -64000;

	for (Count = 0; Count < RectCount; Count++) 
	{
		if (ClipList[Count].left	< RegionData->rdh.rcBound.left)		RegionData->rdh.rcBound.left	= ClipList[Count].left;
		if (ClipList[Count].right	> RegionData->rdh.rcBound.right)	RegionData->rdh.rcBound.right = ClipList[Count].right;
		if (ClipList[Count].top		< RegionData->rdh.rcBound.top)		RegionData->rdh.rcBound.top		= ClipList[Count].top;
		if (ClipList[Count].bottom> RegionData->rdh.rcBound.bottom)	RegionData->rdh.rcBound.bottom= ClipList[Count].bottom;
	};

	if (FAILED(Clipper->SetClipList(RegionData,0))) 
	{
		free(RegionData);
		return(NULL);
	};

	if (FAILED(surface->SetClipper(Clipper))) 
	{
		free(RegionData);
		return(NULL);
	};

	free(RegionData);
	return Clipper;
};
