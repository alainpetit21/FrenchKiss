#include "stdafx.h"
#include "3D.h"
#include "3DDisplay.h"
#include "audio.h"
#include "resman.h"
#include "macros.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
IMAGE			*gArImage			;
u8				*gArPixel			;
VX				*gArVertex		;
VX2 			*gArVertex2		;
TC2 			*gArTexCoord	;
TRI2 			*gArTriangle	;
MMD2			*gArMeshMD2		;
M3DS			*gArMesh3DS		;
WAVESOUND	*gArWaveSt		;
u8				*gArWaveData	;
IMAGE			*gcurImage		= gArImage		;
uchar			*gcurPixel		= gArPixel		;
VX				*gcurVertex		= gArVertex		;
VX2 			*gcurVertex2	= gArVertex2	;
TC2 			*gcurTexCoord	= gArTexCoord	;
TRI2 			*gcurTriangle	= gArTriangle	;
MMD2			*gcurMeshMD2	= gArMeshMD2	;
M3DS			*gcurMesh3DS	= gArMesh3DS	;
WAVESOUND	*gcurWaveSt		= gArWaveSt		;
u8				*gcurWaveData	= gArWaveData	;
/*==============================================================================================================*/
FK_API void**		gAllBuffer[RMI_NUMBER]={
	(void**)&gArImage			,
	(void**)&gArPixel			,
	(void**)&gArVertex		,
	(void**)&gArVertex2		,
	(void**)&gArTexCoord	,
	(void**)&gArTriangle	,
	(void**)&gArMeshMD2		,
	(void**)&gArMesh3DS		,
	(void**)&gArWaveSt		,
	(void**)&gArWaveData	,
};
/*==============================================================================================================*/
FK_API void**		gcurPts[RMI_NUMBER]={
	(void**)&gcurImage		,
	(void**)&gcurPixel		,
	(void**)&gcurVertex		,
	(void**)&gcurVertex2	,
	(void**)&gcurTexCoord	,
	(void**)&gcurTriangle	,
	(void**)&gcurMeshMD2	,
	(void**)&gcurMesh3DS	,
	(void**)&gcurWaveSt		,	
	(void**)&gcurWaveData	,
};
/*==============================================================================================================*/
FK_API int	gsizeof[]={
	sizeof(IMAGE)			,
	sizeof(u8)				,
	sizeof(VX)				,
	sizeof(VX2)				,
	sizeof(TC2)				,
	sizeof(TRI2)			,
	sizeof(MMD2)			,
	sizeof(M3DS)			,
	sizeof(WAVESOUND)	,
	sizeof(u8)				,
};
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API void 
RM_Init()
{
	gcurImage			=gArImage			= 0;
	gcurPixel			=gArPixel			= 0;
	gcurVertex		=gArVertex		= 0;
	gcurVertex2		=gArVertex2		= 0;
	gcurTexCoord	=gArTexCoord	= 0;
	gcurTriangle	=gArTriangle	= 0;
	gcurMeshMD2		=gArMeshMD2		= 0;
	gcurMesh3DS		=gArMesh3DS		= 0;
	gcurWaveSt		=gArWaveSt		= 0;
	gcurWaveData	=gArWaveData	= 0;
}
/*==============================================================================================================*/
FK_API void 
RM_Quit()
{
	if(gArImage)		{free(gArImage);		gArImage		= 0;}
	if(gArPixel)		{free(gArPixel);		gArPixel		= 0;}
	if(gArVertex)		{free(gArVertex);		gArVertex		= 0;}
	if(gArVertex2)	{free(gArVertex2);	gArVertex2	= 0;}
	if(gArMeshMD2)	{free(gArMeshMD2);	gArMeshMD2	= 0;}
	if(gArMesh3DS)	{free(gArMesh3DS);	gArMesh3DS	= 0;}
	if(gArTexCoord)	{free(gArTexCoord);	gArTexCoord	= 0;}
	if(gArTriangle)	{free(gArTriangle);	gArTriangle	= 0;}
	if(gArWaveSt)		{free(gArWaveSt);		gArWaveSt		= 0;}
	if(gArWaveData) {free(gArWaveData);	gArWaveData	= 0;}
}
/*==============================================================================================================*/
FK_API void 
RM_Allocate(int wichBuffer, int size)
{
  (*gcurPts[wichBuffer])= (*gAllBuffer[wichBuffer])= malloc(size*gsizeof[wichBuffer]);
  VERIFY(((*gcurPts[wichBuffer])), _T"Not EnoughMemory");
	
}
/*
	In the near future we will plan to remove every common format (WAV, MD2, 3DS, TGA) to replace by a single 
	HDR+RAW format. The runtime version only use these HDR+RAM version while the tool convert HI-RES graph probably 
	located on a server or PC, to the run-time version. For model geometry, if it could not be converted we will 
	have to choose among different prog mesh.
*/
/*==============================================================================================================*/
FK_API void
RM_LoadImage(IMAGE **ppBitmap, ushort **bufPixel, cstring szBitmap)
{
	PTFILE	fichierR;
	IMG_HDR	header;
	int			w,h,size;

	fichierR= FS_CreateFile((string)szBitmap, OPT_READ);
	FS_ReadFile(fichierR, &header, sizeof(IMG_HDR));

	w= (*ppBitmap)->w= header.w;
	h= (*ppBitmap)->h= header.h;
	(*ppBitmap)->pData= (*bufPixel);
	
	switch(header.bps){
		case 1:		size= (w*h)>>3; break;
		case 2:		size= (w*h)>>2; break;
		case 4:		size= (w*h)>>1; break;
		case 8:		size= (w*h);		break;
		case 15:	
		case 16:	size= (w*h)<<1; break;
		case 24:	size= (w*h)*3;	break;
		case 32:	size= (w*h)<<2; break;
	}

	FS_ReadFile(fichierR, (*ppBitmap)->pData, size);	(*bufPixel)+= size;
	FS_CloseFile(fichierR);														(*ppBitmap)++;
}
/*==============================================================================================================*/
/*==============================================================================================================*/
