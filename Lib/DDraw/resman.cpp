#include <windows.h>
#include "3D.h"
#include "3DDisplay.h"
#include "audio.h"
#include "resman.h"
#include "macros.h"
#include "gui.h"
#include "objMan.h"

IMAGE		*gArImage			;
TEXTURE		*gArTexture			;
ushort		*gArPixel			;
ushort		*gArTPixel			;
VX			*gArVertex			;
VX2 		*gArVertex2			;
TC2 		*gArTexCoord		;
TRI2 		*gArTriangle		;
MMD2		*gArMeshMD2			;
M3DS		*gArMesh3DS			;
WAVESOUND	*gArWaveSt			;
ui8			*gArWaveData8bit	;
ui16		*gArWaveData16bit	;
GWindow		*gArWindow			;
GGroupe		*gArGroupe			;
GControl	*gArControl			;
ENTITY		*gArEntities		;
IMAGE		*gcurImage			= gArImage			;
TEXTURE		*gcurTexture		= gArTexture		;
ushort		*gcurPixel			= gArPixel			;
ushort		*gcurTPixel			= gArTPixel			;
VX			*gcurVertex			= gArVertex			;
VX2 		*gcurVertex2		= gArVertex2		;
TC2 		*gcurTexCoord		= gArTexCoord		;
TRI2 		*gcurTriangle		= gArTriangle		;
MMD2		*gcurMeshMD2		= gArMeshMD2		;
M3DS		*gcurMesh3DS		= gArMesh3DS		;
WAVESOUND	*gcurWaveSt			= gArWaveSt			;
ui8			*gcurWaveData8bit	= gArWaveData8bit	;
ui16		*gcurWaveData16bit	= gArWaveData16bit	;
GWindow		*gcurWindow			= gArWindow			;
GGroupe		*gcurGroupe			= gArGroupe			;
GControl	*gcurControl		= gArControl		;
ENTITY		*gcurEntities		= gArEntities		;

void**		gAllBuffer[]={
	(void**)&gArImage			,
	(void**)&gArTexture			,
	(void**)&gArPixel			,
	(void**)&gArTPixel			,
	(void**)&gArVertex			,
	(void**)&gArVertex2			,
	(void**)&gArTexCoord		,
	(void**)&gArTriangle		,
	(void**)&gArMeshMD2			,
	(void**)&gArMesh3DS			,
	(void**)&gArWaveSt			,
	(void**)&gArWaveData8bit	,
	(void**)&gArWaveData16bit	,
	(void**)&gArWindow			,
	(void**)&gArGroupe			,
	(void**)&gArControl			,
	(void**)&gArEntities		,
};

void**		gcurPts[]={
	(void**)&gcurImage			,
	(void**)&gcurTexture		,
	(void**)&gcurPixel			,
	(void**)&gcurTPixel			,
	(void**)&gcurVertex			,
	(void**)&gcurVertex2		,
	(void**)&gcurTexCoord		,
	(void**)&gcurTriangle		,
	(void**)&gcurMeshMD2		,
	(void**)&gcurMesh3DS		,
	(void**)&gcurWaveSt			,	
	(void**)&gcurWaveData8bit	,
	(void**)&gcurWaveData16bit	,
	(void**)&gcurWindow			,
	(void**)&gcurGroupe			,
	(void**)&gcurControl		,
	(void**)&gcurEntities		,
};

int	gsizeof[]={
	sizeof(IMAGE)		,
	sizeof(TEXTURE)		,
	sizeof(ushort)		,
	sizeof(ushort)		,
	sizeof(VX)			,
	sizeof(VX2)			,
	sizeof(TC2)			,
	sizeof(TRI2)		,
	sizeof(MMD2)		,
	sizeof(M3DS)		,
	sizeof(WAVESOUND)	,
	sizeof(ui8)			,
	sizeof(ui16)		,
	sizeof(GWindow)		,
	sizeof(GGroupe)		,
	sizeof(GControl)	,
	sizeof(ENTITY)		,
};

LIB_API void 
RM_Init()
{
	gcurImage			=gArImage			= 0;
	gcurTexture			=gArTexture			= 0;
	gcurPixel			=gArPixel			= 0;
	gcurTPixel			=gArTPixel			= 0;
	gcurVertex			=gArVertex			= 0;
	gcurVertex2			=gArVertex2			= 0;
	gcurTexCoord		=gArTexCoord		= 0;
	gcurTriangle		=gArTriangle		= 0;
	gcurMeshMD2			=gArMeshMD2			= 0;
	gcurMesh3DS			=gArMesh3DS			= 0;
	gcurWaveSt			=gArWaveSt			= 0;
	gcurWaveData8bit	=gArWaveData8bit	= 0;
	gcurWaveData16bit	=gArWaveData16bit	= 0;
	gcurWindow			=gArWindow			= 0;
	gcurGroupe			=gArGroupe			= 0;
	gcurControl			=gArControl			= 0;
	gcurEntities		=gArEntities		= 0;
}

LIB_API void 
RM_Quit()
{
	if(gArImage			) {free(gArImage		);gArImage			= 0;}
	if(gArTexture		) {free(gArTexture		);gArTexture		= 0;}
	if(gArPixel			) {free(gArPixel		);gArPixel			= 0;}
	if(gArTPixel		) {free(gArTPixel		);gArTPixel			= 0;}
	if(gArVertex		) {free(gArVertex		);gArVertex			= 0;}
	if(gArVertex2		) {free(gArVertex2		);gArVertex2		= 0;}
	if(gArTexCoord		) {free(gArTexCoord		);gArTexCoord		= 0;}
	if(gArTriangle		) {free(gArTriangle		);gArTriangle		= 0;}
	if(gArWaveSt		) {free(gArWaveSt		);gArWaveSt			= 0;}
	if(gArWaveData8bit	) {free(gArWaveData8bit	);gArWaveData8bit	= 0;}
	if(gArWaveData16bit	) {free(gArWaveData16bit);gArWaveData16bit	= 0;}
	if(gArWindow		) {free(gArWindow		);gArWindow			= 0;}
	if(gArGroupe		) {free(gArGroupe		);gArGroupe			= 0;}
	if(gArControl		) {free(gArControl		);gArControl		= 0;}
	if(gArEntities		) {free(gArEntities		);gArEntities		= 0;}
}

LIB_API void 
RM_Allocate(int wichBuffer, int size)
{
  if(wichBuffer == RMI_PIXEL){
    OutputDebugString("Allocating pixels bypassed. Used by DDraw\r\n");
	return;
  }
  (*gcurPts[wichBuffer])= (*gAllBuffer[wichBuffer])= malloc(size*gsizeof[wichBuffer]);
  VERIFY(((*gcurPts[wichBuffer])), "Not EnoughMemory");
	
}

LIB_API void*
RM_GetBufferStart(int wichBuffer)
{
	return (*gAllBuffer[wichBuffer]);
}

LIB_API void**
RM_GetBufferCur(int wichBuffer)
{
  return gcurPts[wichBuffer];
}

LIB_API uint
RM_GetBufferCurOffset(int wichBuffer)
{
  return (gcurPts[wichBuffer]-gAllBuffer[wichBuffer])>>2;
}
