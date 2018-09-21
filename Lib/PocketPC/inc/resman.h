#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "FrenchKISS.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
enum{
	RMI_IMAGE	= 0	,
	RMI_PIXEL		,
	RMI_VERTEX		,
	RMI_VERTEX2		,
	RMI_TEXCOORD2	,
	RMI_TRIANGLES2	,
	RMI_MESH_MD2	,
	RMI_MESH_3DS	,
	RMI_WAVE_ST		,
	RMI_WAVE_DATA_8	,
	RMI_WAVE_DATA_16,
	RMI_NUMBER,
};
/*==============================================================================================================*/
/*==============================================================================================================*/
typedef struct tagIMG_HDR
{
	unsigned int	w;
	unsigned int	h;
	unsigned int	bps;
}IMG_HDR;
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API extern void**		gAllBuffer[RMI_NUMBER];
FK_API extern void**		gcurPts[RMI_NUMBER];
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API void	RM_Init(void);
FK_API void	RM_Quit(void);
FK_API void	RM_Allocate(int wichBuffer, int size);
/*==============================================================================================================*/
FK_API void RM_LoadImage(IMAGE **ppBitmap, ushort **bufPixel, cstring szBitmap);
/*==============================================================================================================*/
#define RM_GetBufferStart(wichBuffer)	(void*)(*gAllBuffer[wichBuffer])
#define RM_GetBufferCur(wichBuffer)		(void**)gcurPts[wichBuffer]
/*==============================================================================================================*/
/*==============================================================================================================*/
#define CUR_IMAGE			(IMAGE**)RM_GetBufferCur(RMI_IMAGE)
#define CUR_PIXEL			(ushort**)RM_GetBufferCur(RMI_PIXEL)
#define CUR_VERTEX			(VX**)RM_GetBufferCur(RMI_VERTEX)
#define CUR_VERTEX2			(VX2**)RM_GetBufferCur(RMI_VERTEX2)
#define CUR_TEXCOORD2		(TC2**)RM_GetBufferCur(RMI_TEXCOORD2)
#define CUR_TRIANGLES2		(TRI2**)RM_GetBufferCur(RMI_TRIANGLES2)
#define CUR_MESH_MD2		(MMD2**)RM_GetBufferCur(RMI_MESH_MD2)
#define CUR_MESH_3DS		(M3DS**)RM_GetBufferCur(RMI_MESH_3DS)
#define CUR_WAVESOUND		(WAVESOUND**)RM_GetBufferCur(RMI_WAVE_ST)
#define CUR_WAVE_DATA_8		(ui8**)RM_GetBufferCur(RMI_WAVE_DATA_8)
#define CUR_WAVE_DATA_16	(ui16**)RM_GetBufferCur(RMI_WAVE_DATA_16)
/*==============================================================================================================*/
#define AR_IMAGE		((IMAGE*)RM_GetBufferStart(RMI_IMAGE))
#define AR_PIXEL		((ushort*)RM_GetBufferStart(RMI_PIXEL))
#define AR_VERTEX		((VX*)RM_GetBufferStart(RMI_VERTEX))
#define AR_VERTEX2		((VX2*)RM_GetBufferStart(RMI_VERTEX2))
#define AR_TEXCOORD2	((TC2*)RM_GetBufferStart(RMI_TEXCOORD2))
#define AR_TRIANGLES2	((TRI2*)RM_GetBufferStart(RMI_TRIANGLES2))
#define AR_MESH_MD2		((MMD2*)RM_GetBufferStart(RMI_MESH_MD2))
#define AR_MESH_3DS		((M3DS*)RM_GetBufferStart(RMI_MESH_3DS))
#define AR_WAVESOUND	((WAVESOUND*)RM_GetBufferStart(RMI_WAVE_ST))
#define AR_WAVE_DATA_8	((ui8*)RM_GetBufferStart(RMI_WAVE_DATA_8))
#define AR_WAVE_DATA_16	((ui16*)RM_GetBufferStart(RMI_WAVE_DATA_16))
/*==============================================================================================================*/
/*==============================================================================================================*/
#endif /*_RES_MANAGER_H_*/
