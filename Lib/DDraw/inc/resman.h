#ifndef _RES_MANAGER_H_
#define _RES_MANAGER_H_

enum{
	RMI_IMAGE	= 0	,
	RMI_TEXTURE		,
	RMI_PIXEL		,
	RMI_TPIXEL		,
	RMI_VERTEX		,
	RMI_VERTEX2		,
	RMI_TEXCOORD2	,
	RMI_TRIANGLES2	,
	RMI_MESH_MD2	,
	RMI_MESH_3DS	,
	RMI_WAVE_ST		,
	RMI_WAVE_DATA_8	,
	RMI_WAVE_DATA_16,
	RMI_WINDOW		,
	RMI_GROUPE		,
	RMI_CONTROL		,
	RMI_ENTITIES	,
};

LIB_API void	RM_Init(void);
LIB_API void	RM_Quit(void);
LIB_API void	RM_Allocate(int wichBuffer, int size);
LIB_API void*	RM_GetBufferStart(int wichBuffer);
LIB_API void**	RM_GetBufferCur(int wichBuffer);
LIB_API uint	RM_GetBufferCurOffset(int wichBuffer);

#define CUR_IMAGE			(IMAGE**)RM_GetBufferCur(RMI_IMAGE)
#define CUR_TEXTURE			(TEXTURE**)RM_GetBufferCur(RMI_TEXTURE)
#define CUR_PIXEL			(ushort**)RM_GetBufferCur(RMI_PIXEL)
#define CUR_TPIXEL			(ushort**)RM_GetBufferCur(RMI_TPIXEL)
#define CUR_VERTEX			(VX**)RM_GetBufferCur(RMI_VERTEX)
#define CUR_VERTEX2			(VX2**)RM_GetBufferCur(RMI_VERTEX2)
#define CUR_TEXCOORD2		(TC2**)RM_GetBufferCur(RMI_TEXCOORD2)
#define CUR_TRIANGLES2		(TRI2**)RM_GetBufferCur(RMI_TRIANGLES2)
#define CUR_MESH_MD2		(MMD2**)RM_GetBufferCur(RMI_MESH_MD2)
#define CUR_MESH_3DS		(M3DS**)RM_GetBufferCur(RMI_MESH_3DS)
#define CUR_WAVESOUND		(WAVESOUND**)RM_GetBufferCur(RMI_WAVE_ST)
#define CUR_WAVE_DATA_8		(ui8**)RM_GetBufferCur(RMI_WAVE_DATA_8)
#define CUR_WAVE_DATA_16	(ui16**)RM_GetBufferCur(RMI_WAVE_DATA_16)
#define CUR_WINDOW			(GWindow**)RM_GetBufferCur(RMI_WINDOW)
#define CUR_GROUPE			(GGroupe**)RM_GetBufferCur(RMI_GROUPE)
#define CUR_CONTROL			(GControl**)RM_GetBufferCur(RMI_CONTROL)
#define CUR_ENTITY			(ENTITY**)RM_GetBufferCur(RMI_ENTITIES)

#define AR_IMAGE			((IMAGE*)RM_GetBufferStart(RMI_IMAGE))
#define AR_TEXTURE			((TEXTURE*)RM_GetBufferStart(RMI_TEXTURE))
#define AR_PIXEL			((ushort*)RM_GetBufferStart(RMI_PIXEL))
#define AR_TPIXEL			((ushort*)RM_GetBufferStart(RMI_TPIXEL))
#define AR_VERTEX			((VX*)RM_GetBufferStart(RMI_VERTEX))
#define AR_VERTEX2			((VX2*)RM_GetBufferStart(RMI_VERTEX2))
#define AR_TEXCOORD2		((TC2*)RM_GetBufferStart(RMI_TEXCOORD2))
#define AR_TRIANGLES2		((TRI2*)RM_GetBufferStart(RMI_TRIANGLES2))
#define AR_MESH_MD2			((MMD2*)RM_GetBufferStart(RMI_MESH_MD2))
#define AR_MESH_3DS			((M3DS*)RM_GetBufferStart(RMI_MESH_3DS))
#define AR_WAVESOUND		((WAVESOUND*)RM_GetBufferStart(RMI_WAVE_ST))
#define AR_WAVE_DATA_8		((ui8*)RM_GetBufferStart(RMI_WAVE_DATA_8))
#define AR_WAVE_DATA_16		((ui16*)RM_GetBufferStart(RMI_WAVE_DATA_16))
#define AR_WINDOW			(GWindow*)RM_GetBufferStart(RMI_WINDOW)
#define AR_GROUPE			(GGroupe*)RM_GetBufferStart(RMI_GROUPE)
#define AR_CONTROL			(GControl*)RM_GetBufferStart(RMI_CONTROL)
#define AR_ENTITY			(ENTITY*)RM_GetBufferStart(RMI_ENTITIES)
#endif /*_RES_MANAGER_H_*/