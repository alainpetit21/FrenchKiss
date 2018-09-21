#ifndef _3D_DISPLAY_H_
#define _3D_DISPLAY_H_
/*=========================*/
/*=========================*/
#include "types.h"
#include "3D.h"
#include "graphic.h"
#include "mathFP.h"
/*=========================*/
/*=========================*/
enum{
  REN_SOFTWARE	= 0x0,
  REN_D3D3		= 0x1,
  REN_D3D5		= 0x2,
  REN_D3D6		= 0x3,
  REN_D3D7		= 0x4,
  REN_D3D8		= 0x5,
  REN_OPENGL	= 0x6,
};
enum{
  RST_POINT		= 0x0,
  RST_LINES		= 0x1,
  RST_FLAT		= 0x2,
  RST_TEXT		= 0x3,
};
enum{
  ANI_NEAREST	= 0x0,
  ANI_LINEAR	= 0x1,
  ANI_COSINE	= 0x2,
  ANI_CUBIC		= 0x3,
};
/*=========================*/
/*=========================*/
#define VB_SIZE   	10
#define VB2_SIZE	100000
#define IB_SIZE		512
#define TB_SIZE 	1024
#define CANVAS_3D_W	800
#define CANVAS_3D_H	800
#define C3DW		CANVAS_3D_W
#define C3DH		CANVAS_3D_H
/*=========================*/
/*=========================*/
typedef void (*DRAWER)(VX*, TEXTURE*, ui32);
typedef void (*DRAWER_IB)(VX2*, TRI2*, TC2*, TEXTURE*, ui32, ui32);
typedef void (*PROCESSOR)(MATRIX*, VX*, ui32, ui32);
/*=========================*/
typedef struct tagVB{
  VX*	m_start;
  i32	m_nb;
}VB;
/*=========================*/
typedef struct tagVB2{
  VX2*	m_start;
  i32	m_nb;
}VB2;
/*=========================*/
typedef struct tagIB2{
  TRI2*		m_start;
  i32		m_nb;
}IB2;
typedef struct tagTB2{
  TC2*		m_start;
  i32		m_nb;
}TB2;
/*=========================*/
typedef struct tagM3DS{
  VB		m_vb;
  TEXTURE*	m_tex;	
}M3DS;
/*=========================*/
typedef struct tagMMD2{
  VB2		m_vb;
  IB2		m_ib;
  TB2		m_tb;
  TEXTURE*	m_tex;
  i32		m_cptFrame;
  i32		m_nNbFrame;
  i32		m_nFps;
}MMD2;
/*=========================*/
/*=========================*/
#define V3D_MeshSetVB(mesh, vb)			\
  (mesh)->m_vb.m_start	= (vb)->m_start;	\
  (mesh)->m_vb.m_nb		= (vb)->m_nb;
#define V3D_MeshSetTexture(mesh, tex)		\
  (mesh)->m_tex		= tex;
#define V3D_MeshMD2SetIB(mesh, ib)		\
  (mesh)->m_ib.m_start	= (ib)->m_start;	\
  (mesh)->m_ib.m_nb		= (ib)->m_nb;
#define V3D_MeshMD2SetTB(mesh, tb, nb)\
  (mesh)->m_tb.m_start	= tb;			\
  (mesh)->m_tb.m_nb		= nb;
#define V3D_MeshMD2SetFPS(nFps)		\
  m_nFps= nFps;
LIB_API void V3D_LoadVB(VB** dst, VX** p_vertex, i32 nb);
LIB_API void V3D_LoadVB2(VB2** dst, VX2** p_vertex, i32 nb);
LIB_API void V3D_LoadIB2(IB2** dst, TRI2** p_index, i32 nb);
LIB_API void V3D_LoadTB2(TB2** dst, TC2** p_texCoord, i32 nb);
LIB_API void V3D_LoadMesh3DS(M3DS** dst, VX** p_vertex, string pInitFile);
LIB_API void V3D_LoadMeshMD2(MMD2** dst, VX2** p_vertex, TRI2** p_index, TC2** p_texCoord, string pInitFile);
LIB_API void V3D_Init(void);
LIB_API void V3D_Quit(void);
LIB_API void V3D_SetRenderer(i32 wichOne);
LIB_API void V3D_SetRendererStage(i32 wichOne);
LIB_API void V3D_SetAnimatorStage(i32 wichOne);
LIB_API void V3D_Clear(void);
LIB_API void V3D_ProcessVB(MATRIX* mat, VB* vb);
LIB_API void V3D_ProcessVB2(MATRIX* p_mat, VB2* p_vb);
LIB_API void V3D_ProcessMesh3DS(MATRIX* mat, M3DS* mesh);
LIB_API void V3D_ProcessMeshMD2(MATRIX* mat, MMD2* mesh);
LIB_API void V3D_DrawPrimitive(VX* tri, TEXTURE* tex, ui32 nb);
LIB_API void V3D_DrawPrimitiveIB(IB2* ib, VB2* vb, TB2* p_tb, TEXTURE* tex);				
LIB_API void V3D_DrawPrimitiveVB(VB* vb, TEXTURE* tex);				
LIB_API void V3D_DrawMesh3DS(M3DS* mesh);
LIB_API void V3D_DrawMeshMD2(MMD2* mesh);
LIB_API void V3D_Flip(void);
/*=========================*/
/*=========================*/
#endif // _3D_DISPLAY_H_






