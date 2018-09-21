#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "3DDisplay.h"
#include "mathFP.h"
#include "file.h"
#include "graphic.h"
#include "macros.h"
/*==========================================*/
/*==========================================*/
LIB_API void V3D_IPI_Nearest(MATRIX* mat, VX* tri, ui32 nb, ui32 inter);
LIB_API void V3D_IPI_Linear (MATRIX* mat, VX* tri, ui32 nb, ui32 inter);
LIB_API void V3D_IPI_Cosine (MATRIX* mat, VX* tri, ui32 nb, ui32 inter);
LIB_API void V3D_IPI_Cubic  (MATRIX* mat, VX* tri, ui32 nb, ui32 inter);
LIB_API void V3D_ID_Point   (VX* tri, TEXTURE* tex, ui32 nb);
LIB_API void V3D_ID_Lines   (VX* tri, TEXTURE* tex, ui32 nb);
LIB_API void V3D_ID_Flat    (VX* tri, TEXTURE* tex, ui32 nb);
LIB_API void V3D_ID_Text    (VX* tri, TEXTURE* tex, ui32 nb);
LIB_API void V3D_IDI_Point  (VX2* ver, TRI2* tri, TC2* p_texCoord, TEXTURE* tex, ui32 nbV, ui32 nbI);
LIB_API void V3D_IDI_Lines  (VX2* ver, TRI2* tri, TC2* p_texCoord, TEXTURE* tex, ui32 nbV, ui32 nbI);
LIB_API void V3D_IDI_Flat   (VX2* ver, TRI2* tri, TC2* p_texCoord, TEXTURE* tex, ui32 nbV, ui32 nbI);
LIB_API void V3D_IDI_Text   (VX2* ver, TRI2* tri, TC2* p_texCoord, TEXTURE* tex, ui32 nbV, ui32 nbI);
LIB_API void V3D_IPolyCalcPlane(VX* tri);
LIB_API long V3D_IEatchunk  (PTFILE file, VX** p_vertex, VB* vbToFill);
/*==========================================*/
/*==========================================*/
extern IMAGE			bb;
extern DDSURFACEDESC	ddsd;
extern IMAGE			fb;
/*==========================================*/
/*==========================================*/
ui16		m_dest[C3DW*C3DH];
DRAWER 		Drawer	= 0;
DRAWER_IB 	DrawerIB= 0;
PROCESSOR 	Process	= 0;
/*==========================================*/
//Rasterize Data
/*==========================================*/
ui16	m_ZBuffer[C3DW*C3DH];
i32		m_Left	[C3DH]
, 		m_Right	[C3DH];
i32		m_ULeft	[C3DH]
, 		m_URight[C3DH];
i32		m_VLeft	[C3DH]
, 		m_VRight[C3DH];
i32		m_ZLeft	[C3DH]
, 		m_ZRight[C3DH];
i32		m_Lines;
/*=========================*/
//Temp
/*=========================*/
i32 x[1024], y[1024], z[1024], nb;
/*=========================*/
/*=========================*/
LIB_API void 
V3D_Init()
{
  i32 i= C3DW;
  while(i--){
    m_Left[i] = IFP(C3DW);
    m_Right[i] = 0;
  }
  m_Lines = 0;
};
/*=========================*/
LIB_API void 
V3D_Quit()
{
}
/*=========================*/
LIB_API void			
V3D_Clear()
{
  memset(m_ZBuffer, 0x0, sizeof(ui16)*C3DW*C3DH);
  memset(m_dest, 0x0, sizeof(ui16)*C3DW*C3DH);
}
/*=========================*/
LIB_API void			
V3D_Flip()
{
	ui16	*p1;
	ui16	*p2	= m_dest;
	i32		i	= SCH;

	p1= VID_Lock(bb, DDLOCK_WRITEONLY);
	p2+= 360*C3DW;
	p2+= 280;
	while(i--){
		memcpy(p1, p2, ddsd.lPitch);
		p1+= ddsd.lPitch>>1;
		p2+= C3DW;
	}
	VID_Unlock(bb);
/*Trace Output
	{
		PTFILE fichier= FS_CreateFile("toto.raw", OPT_WRITE);
		FS_Write(fichier, m_dest, C3DW*C3DH*2);
		FS_CloseFile(fichier);
	}*/
}
/*=========================*/
LIB_API void 
V3D_LoadVB(VB** dst, VX** p_vertex, i32 nb)
{
  (*dst)->m_start	= (*p_vertex);
  (*dst)->m_nb		= nb;
  (*p_vertex)		+= nb;
  (*dst)++ ;
}
/*=========================*/
LIB_API void 
V3D_LoadVB2(VB2** dst, VX2** p_vertex, i32 nb)
{
  (*dst)->m_start	= (*p_vertex);
  (*dst)->m_nb		= nb;
  (*p_vertex)		+= nb;
  (*dst)++ ;
}
/*=========================*/
LIB_API void 
V3D_LoadIB2(IB2** dst, TRI2** p_index, i32 nb)
{
  (*dst)->m_start	= (*p_index);
  (*dst)->m_nb		= nb;
  (*p_index)		+= nb;
  (*dst)++;
}
/*=========================*/
LIB_API void 
V3D_LoadTB2(TB2** dst, TC2** p_texCoord, 
  i32 nb)
{
  (*dst)->m_start	= (*p_texCoord);
  (*dst)->m_nb		= nb;
  (*p_texCoord)		+= nb;
  (*dst)++;
}
/*=========================*/
LIB_API void
V3D_LoadMesh3DS(M3DS** dst, VX** p_vertex, string pInitFile)
{
  M3DS*	mesh= *dst;
  VB		vbToFill;
  PTFILE	file;

  file= FS_CreateFile(pInitFile, OPT_READ);

  vbToFill.m_start	= (*p_vertex);
  vbToFill.m_nb		= 0;
  V3D_IEatchunk(file, p_vertex, &vbToFill);
  FS_CloseFile(file);

  i32 xmax = IFP(-1000), xmin = IFP(1000);
  i32 ymax = IFP(-1000), ymin = IFP(1000);
  i32 zmax = IFP(-1000), zmin = IFP(1000);
  for(int i = 0; i < vbToFill.m_nb; i++ ){
    i32 x = vbToFill.m_start[i].orig[0];
    i32 y = vbToFill.m_start[i].orig[1];
    i32 z = vbToFill.m_start[i].orig[2];
    if (x < xmin) xmin = x;
    if (x > xmax) xmax = x;
    if (y < ymin) ymin = y;
    if (y > ymax) ymax = y;
    if (z < zmin) zmin = z;
    if (z > zmax) zmax = z;
  }
  double du = (FP_ONE * 256.0) / (double)(xmax - xmin);
  double dv = (FP_ONE * 256.0) / (double)(zmax - zmin);
  for ( i = 0; i < vbToFill.m_nb; i+=3){
    for ( i32 j = 0; j < 3; j++ ){
      vbToFill.m_start[i+j].m_U1= (int)((vbToFill.m_start[i+j].orig[0] - xmin) * du);
      vbToFill.m_start[i+j].m_V1= (int)((vbToFill.m_start[i+j].orig[2] - zmin) * dv);
    }
  }
  mesh->m_vb.m_start	= vbToFill.m_start;
  mesh->m_vb.m_nb		= vbToFill.m_nb;
  (*dst)++;
}
/*=========================*/
LIB_API void
V3D_LoadMeshMD2(MMD2** dst, VX2** p_vertex, 
  TRI2** p_index, TC2** p_texCoord, 
  string pInitFile)
{
  struct{ 
    i32	magic,version,skinWidth,skinHeight,frameSize
    ,	numSkins,numVertices,numTexCoords
    ,	numTriangles,numGlCommands
    ,	numFrames,offsetSkins,offsetTexCoords
    ,	offsetTriangles,offsetFrames
    ,	offsetGlCommands,offsetEnd;
  }md2_hdr;
  MMD2*	mesh= *dst;
  VX2*		curV;
  TC2*		curT;
  TRI2*		curI;
  PTFILE	file;
  i32		i, j;

  file= FS_CreateFile(pInitFile, OPT_READ);
  FS_ReadFile(file, &md2_hdr, sizeof(md2_hdr));

  curV=	mesh->m_vb.m_start	=	(*p_vertex);
  mesh->m_vb.m_nb		=	md2_hdr.numVertices;
  curT=	(*p_texCoord);
  curI=	mesh->m_ib.m_start	=	(*p_index);
  mesh->m_ib.m_nb		=	md2_hdr.numTriangles;
  mesh->m_nNbFrame	=	md2_hdr.numFrames;

  //Read UV
  FS_SetPos(file, md2_hdr.offsetTexCoords, FRM_BEGIN);
  for (i = 0; i < md2_hdr.numTexCoords; ++i){
    curT->m_U1= FS_ReadWord(file)<<FPP;
    curT->m_V1= FS_ReadWord(file)<<FPP;
    ++curT;++(*p_texCoord);
  }

//Read Coordonnes
  FS_SetPos(file, md2_hdr.offsetFrames, FILE_BEGIN);
  for(i = 0; i < md2_hdr.numFrames; ++i){
    float scale[3],transl[3];char name[16];
    scale[0]= (FS_ReadFloat(file));
    scale[1]= (FS_ReadFloat(file));
    scale[2]= (FS_ReadFloat(file));
    transl[0]= (FS_ReadFloat(file));
    transl[1]= (FS_ReadFloat(file));
    transl[2]= (FS_ReadFloat(file));
    FS_ReadFile(file, &name, 16);
    for(j = 0; j < md2_hdr.numVertices; ++j){
      curV->orig[0]	= FP(((float)((int)FS_ReadByte(file)) * scale[0]) + transl[0]);
      curV->orig[1]	= FP((((float)((int)FS_ReadByte(file)) * scale[1]) + transl[1]));
      curV->orig[2]	= FP(((float)((int)FS_ReadByte(file)) * scale[2]) + transl[2]);
      name[0]			= FS_ReadByte(file);
      ++curV;++(*p_vertex);
    }
  }

  //Read triangles
  FS_SetPos(file, md2_hdr.offsetTriangles, FRM_BEGIN);
  for(i = 0; i < md2_hdr.numTriangles; ++i){
    curI->idxVertex[0]	= (int)FS_ReadWord(file);
    curI->idxVertex[1]	= (int)FS_ReadWord(file);
    curI->idxVertex[2]	= (int)FS_ReadWord(file);
    curI->idxTexCoor[0]	= (int)FS_ReadWord(file);
    curI->idxTexCoor[1]	= (int)FS_ReadWord(file);
    curI->idxTexCoor[2]	= (int)FS_ReadWord(file);
    ++curI;++(*p_index);
  }

  FS_CloseFile(file);
  (*dst)++;
}
/*=========================*/
LIB_API void 
V3D_IPolyCalcPlane(VX* p_tri)
{
  i32* v1 = p_tri[0].orig;
  i32* v2 = p_tri[1].orig;
  i32* v3 = p_tri[2].orig;
  double rx1 = (double)(v1[0] - v1[0]) / FP_ONE;
  double ry1 = (double)(v2[1] - v1[1]) / FP_ONE;
  double rz1 = (double)(v2[2] - v1[2]) / FP_ONE;
  double rx2 = (double)(v3[0] - v1[0]) / FP_ONE;
  double ry2 = (double)(v3[1] - v1[1]) / FP_ONE;
  double rz2 = (double)(v3[2] - v1[2]) / FP_ONE;
  double A = (ry1 * rz2) - (ry2 * rz1 );
  double B = (rz1 * rx2) - (rz2 * rx1 );
  double C = (rx1 * ry2) - (rx2 * ry1 );
  double OneOnlen = 1/(sqrt( A * A + B * B + C * C));
  p_tri[2].nor[0]= p_tri[1].nor[0]= p_tri[0].nor[0]= (int)(A * FP_ONE * OneOnlen);
  p_tri[2].nor[1]= p_tri[1].nor[1]= p_tri[0].nor[1]= (int)(B * FP_ONE * OneOnlen);
  p_tri[2].nor[2]= p_tri[1].nor[2]= p_tri[0].nor[2]= (int)(C * FP_ONE * OneOnlen);
}
/*=========================*/
LIB_API long 
V3D_IEatchunk(PTFILE file, VX** p_vertex, VB* vbToFill)
{
  ui16 tmep;
  uchar temp2;
  VX*	cur;
  short chunkid = FS_ReadWord(file);
  long chunklength = FS_ReadDword(file);
  i32 j, i = 6;
  switch (chunkid)
  {
  case 0x4D4D:
    do{
      tmep= FS_ReadWord(file);
      i += 2;
    }while ((tmep != 0x3D3D)&&(tmep != 0xB000));
    i -= 2;
    FS_SetPos(file, -2, FILE_CURRENT);
	break;
  case 0x3D3D:
    break;
  case 0x4000:
    do{
      temp2= FS_ReadByte(file);
      ++i;
    }while(temp2);
    break;
  case 0x4100:
    break;
  case 0x4110:
    nb= FS_ReadWord(file); 	
    i+=2;
    for (j = 0; j < nb; j++ ){
      x[j]= (i32)((FP_ONE >> 3) * (FS_ReadFloat(file)/2));
      y[j]= (i32)((FP_ONE >> 3) * (FS_ReadFloat(file)/2));
      z[j]= (i32)((FP_ONE >> 3) * (FS_ReadFloat(file)/2));
      i+=12;
    }
    break;
  case 0x4120:
    {		
      vbToFill->m_nb		= FS_ReadWord(file);
      vbToFill->m_nb*= 3;
      i+=2;
      cur= vbToFill->m_start= (*p_vertex);

      for (j = 0; j < vbToFill->m_nb; j+= 3){
        i32 c1 = FS_ReadWord(file);
        i32 c2 = FS_ReadWord(file);
        i32 c3 = FS_ReadWord(file);
        i32 cS = FS_ReadWord(file);
        if ((c1 != c2) && (c2 != c3) && (c1 != c3)){
          cur->orig[0]= x[c1];
          cur->orig[1]= y[c1];
          cur->orig[2]= z[c1];
          cur->m_R= 0xF;
          cur->m_G= 0xF;
          cur->m_B= 0xF;
          (*p_vertex)++;cur++;
          cur->orig[0]= x[c2];
          cur->orig[1]= y[c2];
          cur->orig[2]= z[c2];
          cur->m_R= 0xF;
          cur->m_G= 0xF;
          cur->m_B= 0xF;
          (*p_vertex)++;cur++;
          cur->orig[0]= x[c3];
          cur->orig[1]= y[c3];
          cur->orig[2]= z[c3];
          cur->m_R= 0xF;
          cur->m_G= 0xF;
          cur->m_B= 0xF;
          (*p_vertex)++;cur++;
          V3D_IPolyCalcPlane((*p_vertex)-3);
        }
        i+=8;
      }
    }
    break;
  default:
    i = chunklength;
    FS_SetPos(file, i-6, FILE_CURRENT);
    break;
  }
  while (i < chunklength) 
    i+= V3D_IEatchunk(file, p_vertex, vbToFill);

  return chunklength;
}
/*=========================*/
int cX;
int cY;
int cZ;
LIB_API void
V3D_ProcessVB(MATRIX* p_mat, VB* p_vb)
{
  VX* 	p_tri	= p_vb->m_start;
  ui32 	p_nb	= p_vb->m_nb;
  i32 	ox
  , 	oy
  , 	oz;

  for (ui32 i = 0; i < p_nb;i++){
    ox = p_tri[i].orig[0]+ cX;
    oy = p_tri[i].orig[1]+ cY;
    oz = p_tri[i].orig[2]+ cZ;
    p_tri[i].rot[0]=	FPMUL(p_mat->l[0],	ox) + 
    					FPMUL(p_mat->l[1],	oy) + 
    					FPMUL(p_mat->l[2],	oz) + p_mat->l[3];
    p_tri[i].rot[1]=	FPMUL(p_mat->l[4],	ox) + 
    					FPMUL(p_mat->l[5],	oy) + 
    					FPMUL(p_mat->l[6],	oz) + p_mat->l[7];
    p_tri[i].rot[2]=	FPMUL(p_mat->l[8],	ox) + 
    					FPMUL(p_mat->l[9],	oy) + 
    					FPMUL(p_mat->l[10],	oz) + p_mat->l[11];

    p_tri[i].sX= IFP(600) + FPDIV(p_tri[i].rot[0], p_tri[i].rot[2] >> 9);
    p_tri[i].sY= IFP(600) + FPDIV(p_tri[i].rot[1], p_tri[i].rot[2] >> 9);
  }
}
/*=========================*/
LIB_API void
V3D_ProcessVB2(MATRIX* p_mat, VB2* p_vb)
{
  i32 	ox
  , 	oy
  , 	oz;
  VX2*	p_tri	= p_vb->m_start;
  i32	p_nb	= p_vb->m_nb;
  VX2*	p_tri2	= p_tri;

  for (int i = 0; i < p_nb;i++){
    ox = p_tri2[i].orig[0];
    oy = p_tri2[i].orig[1];
    oz = p_tri2[i].orig[2];
    p_tri[i].rot[0]=	FPMUL(p_mat->l[0],	ox) + 
						FPMUL(p_mat->l[1],	oy) + 
						FPMUL(p_mat->l[2],	oz) + p_mat->l[3];
    p_tri[i].rot[1]=	FPMUL(p_mat->l[4],	ox) + 
						FPMUL(p_mat->l[5],	oy) + 
						FPMUL(p_mat->l[6],	oz) + p_mat->l[7];
    p_tri[i].rot[2]=	FPMUL(p_mat->l[8],	ox) + 
						FPMUL(p_mat->l[9],	oy) + 
						FPMUL(p_mat->l[10],	oz) + p_mat->l[11];

    p_tri[i].sX= IFP(600) + FPDIV(p_tri[i].rot[0], p_tri[i].rot[2] >> 9);
    p_tri[i].sY= IFP(600) + FPDIV(p_tri[i].rot[1], p_tri[i].rot[2] >> 9);
  }
}
/*=========================*/
LIB_API void
V3D_ProcessMesh3DS(MATRIX* p_mat, M3DS* p_mesh)
{
  V3D_ProcessVB(p_mat, &(p_mesh->m_vb));
}
/*=========================*/
LIB_API void
V3D_ProcessMeshMD2(MATRIX* p_mat, MMD2* p_mesh)
{
/*	static	i32 previous= IFP(BMI_GetTime());
		i32 thisFrame= IFP(BMI_GetTime());
		i32 dif= thisFrame-previous;
	(this->*process)(p_mat, ((CVertexBuffer*)((CMeshMD2*)p_mesh)->GetVB())->m_start, ((CVertexBuffer*)((CMeshMD2*)p_mesh)->GetVB())->m_nb, dif);
	previous= thisFrame;
*/
  static i32 	cpt=0;
  i32 		ox
  , 		oy
  , 		oz;
  VX2*	p_tri	= p_mesh->m_vb.m_start;
  i32	p_nb	= p_mesh->m_vb.m_nb;
  VX2*	p_tri2	= p_tri+(p_nb*cpt);

  for (int i = 0; i < p_nb;i++){
    ox = p_tri2[i].orig[0];
    oy = p_tri2[i].orig[1];
    oz = p_tri2[i].orig[2];
    p_tri[i].rot[0]= FPMUL(p_mat->l[0],	ox) + 
						FPMUL(p_mat->l[1],	oy) + 
						FPMUL(p_mat->l[2],	oz) + p_mat->l[3];
    p_tri[i].rot[1]=	FPMUL(p_mat->l[4],	ox) + 
						FPMUL(p_mat->l[5],	oy) + 
						FPMUL(p_mat->l[6],	oz) + p_mat->l[7];
    p_tri[i].rot[2]=	FPMUL(p_mat->l[8],	ox) + 
						FPMUL(p_mat->l[9],	oy) + 
						FPMUL(p_mat->l[10],	oz) + p_mat->l[11];

    p_tri[i].sX= IFP(600) + FPDIV(p_tri[i].rot[0], p_tri[i].rot[2] >> 9);
    p_tri[i].sY= IFP(600) + FPDIV(p_tri[i].rot[1], p_tri[i].rot[2] >> 9);
  }
  if((++cpt)>=p_mesh->m_nNbFrame)
    cpt=0;
}
/*=========================*/
LIB_API void						
V3D_IPI_Nearest(MATRIX* p_mat, VX* p_tri, ui32 p_nb, ui32 p_inter)
{
}
/*=========================*/
LIB_API void						
V3D_IPI_Linear(MATRIX* p_mat, VX* p_tri, ui32 p_nb, ui32 p_inter)
{
}
/*=========================*/
LIB_API void						
V3D_IPI_Cosine(MATRIX* p_mat, VX* p_tri, ui32 p_nb, ui32 p_inter)
{
}
/*=========================*/
LIB_API void						
V3D_IPI_Cubic(MATRIX* p_mat, VX* p_tri, ui32 p_nb, ui32 p_inter)
{
}
/*=========================*/
LIB_API void			
V3D_DrawPrimitive(VX* p_tri, TEXTURE* p_tex, ui32 p_nb)
{
  (*Drawer)(p_tri, p_tex, p_nb);
}
/*=========================*/
LIB_API void			
V3D_DrawPrimitiveVB(VB* p_vb, TEXTURE* p_tex)
{
  V3D_DrawPrimitive((VX*)p_vb->m_start, (TEXTURE*)p_tex, (ui32)p_vb->m_nb);
}
/*=========================*/
LIB_API void			
V3D_DrawPrimitiveIB(IB2* p_ib, VB2* p_vb, TB2* p_tb, TEXTURE* p_tex)
{
  (*DrawerIB)(p_vb->m_start, p_ib->m_start, p_tb->m_start, p_tex, p_vb->m_nb, p_ib->m_nb);
}
/*=========================*/
LIB_API void			
V3D_DrawMesh3DS(M3DS* p_mesh)
{
  V3D_DrawPrimitiveVB(&(p_mesh->m_vb), p_mesh->m_tex);
}
/*=========================*/
LIB_API void			
V3D_DrawMeshMD2(MMD2* p_mesh)
{
  V3D_DrawPrimitiveIB(&(p_mesh->m_ib), &(p_mesh->m_vb), &(p_mesh->m_tb), p_mesh->m_tex);
}
/*=========================*/
//Internal
LIB_API void			
V3D_ID_Point(VX* p_tri, TEXTURE* p_tex, ui32 p_nb)
{
}
/*=========================*/
LIB_API void			
V3D_ID_Lines(VX* p_tri, TEXTURE* p_tex, ui32 p_nb)
{
}
/*=========================*/
LIB_API void			
V3D_ID_Flat(VX* p_tri, TEXTURE* p_tex, ui32 p_nb)
{
  i32 	drXMin=239, drXMax= 0
  , 	drYMin= 319, drYMax=0;
  ui32 	i;
  i32	j, k, index
  ,	maxy, miny
  ,	sxI, syI, sx1, sy1, sx2, sy2, z1, z2
  ,	sx4, sy4, sx5, sy5, sx6, sy6
  ,	fix, d, dz, dx, cz, cx, cy, y1, y2, x1, x2;
  ui16	cz2;

  for (i = 0; i < p_nb;	i+= 3, p_tri+= 3){
/*    sx4 = p_tri[0].sX >> (FPP - 6);
    sy4 = p_tri[0].sY >> (FPP - 6);
    sx5 = p_tri[1].sX >> (FPP - 6);
    sy5 = p_tri[1].sY >> (FPP - 6);
    sx6 = p_tri[2].sX >> (FPP - 6);
    sy6 = p_tri[2].sY >> (FPP - 6);
    if((((sx4 - sx5) * (sy6 - sy5)) - ((sy4 - sy5) * (sx6 - sx5))) <= 0)
      continue;
*/    
	miny = C3DH;
    maxy = 0;
    sx1 = p_tri[0].sX;
    sy1 = p_tri[0].sY;
    z1 = (0x7FFFFFF0 / p_tri[0].rot[2]) << FPP;
    for (j = 0; j < 3; j++ ){
      index= ((j+1)>2)?0:j+1;
      sx2 = p_tri[index].sX;
      sy2 = p_tri[index].sY;
      sxI= sx2>>FPP;
      syI= sy2>>FPP;
      if(sxI < drXMin)	drXMin= sxI;
      if(sxI > drXMax)	drXMax= sxI;
      if(syI < drYMin)	drYMin= syI;
      if(syI > drYMax)	drYMax= syI;
      z2 = (0x7FFFFFF0 / p_tri[index].rot[2]) << FPP;
      if (sy2 > sy1){
        cx = sx1;
        cy = sy1;
        cz = z1;
        y1 = (sy1 >> FPP) + 1;
        y2 = (sy2 >> FPP) + 1;
        if (y1 < miny) 	miny = y1;
        if (y2 > maxy) 	maxy = y2;
        d = sy2 - sy1;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx2 - sx1, d ); 
          dz = FPDIV( z2 - z1, d ); 
        }else 
          dx = 0;
        fix = FP_ONE - (sy1 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
      }else{
        cx = sx2;
        cy = sy2;
        cz = z2;
        y1 = (sy2 >> FPP) + 1;
        y2 = (sy1 >> FPP) + 1;
        if (y2 < miny) 	miny = y2;
        if (y1 > maxy) 	maxy = y1;
        d = sy1 - sy2;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx1 - sx2, d ); 
          dz = FPDIV( z1 - z2, d ); 
        }else 
          dx = 0;
        fix = FP_ONE - (sy2 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
      }
	  if(miny<0)	
		  miny= 0;
	  if(maxy>=C3DW)	
		  maxy= C3DW-1;
/*	  if(miny>maxy){	
		  int toto= maxy;
		  maxy= miny;
		  miny= maxy;
	  }*/
      for (k = y1; k < y2; k++){
        if (cx < m_Left[k]){
		  m_Left[k] = (cx<0)?0:cx;
          m_ZLeft[k] = (cz<0)?0:cz;
        }
        if (cx > m_Right[k]){
		  m_Right[k] = (cx>=IFP(C3DW))?IFP(C3DW-1):cx;
          m_ZRight[k] = (cz>=IFP(C3DW))?IFP(C3DW-1):cz;
        }
        cx += dx;
        cz += dz;
      }
      sx1 = sx2;
      sy1 = sy2;
      z1 = z2;
    }
//    ui16 src = (ui16)	((p_tri->m_R<<11)| 
//			(p_tri->m_G<<5)|
//			(p_tri->m_B));
    ui16 src = 0xFFFF;
    ui16* dst = m_dest + (miny * C3DW);
    ui16* zbf = m_ZBuffer + (miny * C3DW);
    for ( j = miny; j < maxy; j++ ){
      x1 = m_Left[j] >> FPP; 
      x2 = m_Right[j] >> FPP;
      if (x2 > x1)	{
        dz = ((m_ZRight[j] - m_ZLeft[j]) / (m_Right[j] - m_Left[j])) >> (13 - FPP);
        cz2 = (ui16)(m_ZLeft[j] >> 13);
        for ( i32 k = x1; k < x2; k++ ){
          if (*(zbf + k) < cz2){
            *(dst + k) = src;
            *(zbf + k) = cz2;
           }
          cz2 += (ui16)dz;
        }
      }
      dst += 1200;
      zbf += 1200;
      m_Left[j] = IFP(1200);
      m_Right[j] = 0;
    }
  }
}
/*=========================*/
LIB_API void			
V3D_ID_Text(VX* p_tri, TEXTURE* p_tex, ui32 p_nb)
{
  i32 	drXMin=239, drXMax= 0
  , 	drYMin= 319, drYMax=0;
  ui32 i;
  i32	j, k, index
  ,	maxy, miny
  ,	sxI, syI, sx1, sy1, sx2, sy2, z1, z2
  ,	sx4, sy4, sx5, sy5, sx6, sy6
  ,	u1, v1, u2, v2
  ,	fix, d, du, dv, dz, dx
  , 	cu, cv, cz, cx, cy, y1, y2, x1, x2;
  ulong	cuv, duv;
  ui16 cz2;

  for (i = 0; i < p_nb;	i+= 3, p_tri+= 3){
/*    sx4 = p_tri[0].sX >> (FPP - 6);
    sy4 = p_tri[0].sY >> (FPP - 6);
    sx5 = p_tri[1].sX >> (FPP - 6);
    sy5 = p_tri[1].sY >> (FPP - 6);
    sx6 = p_tri[2].sX >> (FPP - 6);
    sy6 = p_tri[2].sY >> (FPP - 6);
    if((((sx4 - sx5) * (sy6 - sy5)) - ((sy4 - sy5) * (sx6 - sx5))) <= 0)
      continue;
*/    
	miny = 1200;
    maxy = 0;
    sx1 = p_tri[0].sX;
    sy1 = p_tri[0].sY;
    if(sx1 <= 0)		sx1=0;
    if(sx1 >= IFP(1200))sx1=IFP(1199);	
    if(sy1 <= 0)		sy1=0;
    if(sy1 >= IFP(1200))sy1=IFP(1199);	
    u1 = p_tri[0].m_U1;
    v1 = p_tri[0].m_V1;
    z1 = (0x7FFFFFF0 / p_tri[0].rot[2]) << FPP;
    for (j = 0; j < 3; j++ ){
      index= ((j+1)>2)?0:j+1;
      sx2 = p_tri[index].sX;
      sy2 = p_tri[index].sY;
      if(sx1 <= 0)			sx1=0;
      if(sx1 >= IFP(1200))	sx1=IFP(1199);	
      if(sy1 <= 0)			sy1=0;
      if(sy1 >= IFP(1200))	sy1=IFP(1199);	
      sxI= sx2>>FPP;
      syI= sy2>>FPP;
      if(sxI < drXMin)	drXMin= sxI;
      if(sxI > drXMax)	drXMax= sxI;
      if(syI < drYMin)	drYMin= syI;
      if(syI > drYMax)	drYMax= syI;
      u2 = p_tri[index].m_U1;
      v2 = p_tri[index].m_V1;
      z2 = (0x7FFFFFF0 / p_tri[index].rot[2]) << FPP;
      if (sy2 > sy1){
        cx = sx1;
        cy = sy1;
        cu = u1;
        cv = v1;
        cz = z1;
        y1 = (sy1 >> FPP) + 1;
        y2 = (sy2 >> FPP) + 1;
        if (y1 < miny) 	miny = y1;
        if (y2 > maxy) 	maxy = y2;
        d = sy2 - sy1;
        if ((d > 4) || (d < -4)) {
          dx = FPDIV( sx2 - sx1, d ); 
          du = FPDIV( u2 - u1, d ); 
          dv = FPDIV( v2 - v1, d ); 
          dz = FPDIV( z2 - z1, d ); 
        }else 
          du = dv = dx = 0;
        fix = FP_ONE - (sy1 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
        cu += FPMUL( fix, du );
        cv += FPMUL( fix, dv );
      }else{
        cx = sx2;
        cy = sy2;
        cu = u2;
        cv = v2;
        cz = z2;
        y1 = (sy2 >> FPP) + 1;
        y2 = (sy1 >> FPP) + 1;
        if (y2 < miny) 	miny = y2;
        if (y1 > maxy) 	maxy = y1;
        d = sy1 - sy2;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx1 - sx2, d ); 
          du = FPDIV( u1 - u2, d ); 
          dv = FPDIV( v1 - v2, d ); 
          dz = FPDIV( z1 - z2, d ); 
        }else 
          du = dv = dx = 0;
        fix = FP_ONE - (sy2 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
        cu += FPMUL( fix, du );
        cv += FPMUL( fix, dv );
      }
      for (k = y1; k < y2; k++){
        if (cx < m_Left[k]){
		  m_Left[k] = cx;
          m_ULeft[k] = cu;
          m_VLeft[k] = cv;
          m_ZLeft[k] = cz;
        }
        if (cx > m_Right[k]){
		  m_Right[k] = cx;
          m_URight[k] = cu;
          m_VRight[k] = cv;
          m_ZRight[k] = cz;
        }
        cx += dx;
        cu += du;
        cv += dv;
        cz += dz;
      }
      sx1 = sx2;
      sy1 = sy2;
      u1 = u2;
      v1 = v2;
      z1 = z2;
    }
    ui16* dst = m_dest + (miny * 1200);
    ui16* zbf = m_ZBuffer + (miny * 1200);
    ui16* src = p_tex->pData;

    for ( j = miny; j < maxy; j++ ){
      x1 = m_Left[j] >> FPP, x2 = m_Right[j] >> FPP;
      if (x2 > x1){
        du = FPDIV(m_URight[j] - m_ULeft[j], m_Right[j] - m_Left[j]);
        dv = FPDIV(m_VRight[j] - m_VLeft[j], m_Right[j] - m_Left[j]);
        dz = ((m_ZRight[j] - m_ZLeft[j]) / (m_Right[j] - m_Left[j])) >> (13 - FPP);
        cuv = ((m_ULeft[j] >> (FPP - 8)) << 16) + (m_VLeft[j] >> (FPP - 8));
        duv = ((du >> (FPP - 8)) << 16) + (dv >> (FPP - 8));
        cz2 = (ui16)(m_ZLeft[j] >> 13);
        for ( i32 k = x1; k < x2; k++ ){
          if(*(zbf + k) < cz2){
            *(dst + k) = *(src + (cuv >> 24) + (cuv & 0xFF00));
            *(zbf + k) = cz2;
          }
          cuv += duv;
          cz2 += (ui16)dz;
        }
      }
      dst += 1200;
      zbf += 1200;
      m_Left[j] = IFP(1200);
      m_Right[j] = 0;
    }
  }
}
/*=========================*/
LIB_API void			
V3D_IDI_Point(VX2* p_ver, TRI2* p_tri, TC2* p_texCoord, TEXTURE* p_tex, ui32 p_nbV, ui32 p_nbI)
{
/*  i32 	drXMin=239, drXMax= 0
  , 	drYMin= 319, drYMax=0
  ,	sx1, sy1, sx2, sy2, sx3, sy3;
	
  for (ui32 k = 0; k < p_nbI;++k){
    sx1= (p_ver[p_tri[k].idxVertex[0]].sX)>>FPP;
    sy1= (p_ver[p_tri[k].idxVertex[0]].sY)>>FPP;
    sx2= (p_ver[p_tri[k].idxVertex[1]].sX)>>FPP;
    sy2= (p_ver[p_tri[k].idxVertex[1]].sY)>>FPP;
    sx3= (p_ver[p_tri[k].idxVertex[2]].sX)>>FPP;
    sy3= (p_ver[p_tri[k].idxVertex[2]].sY)>>FPP;
    if(sx1 < drXMin)	drXMin= sx1;
    if(sx1 > drXMax)	drXMax= sx1;
    if(sy1 < drYMin)	drYMin= sy1;
    if(sy1 > drYMax)	drYMax= sy1;
    VID_PutPixel(m_dest, sx1, sy1, 0x9999);
    if(sx2 < drXMin)	drXMin= sx2;
    if(sx2 > drXMax)	drXMax= sx2;
    if(sy2 < drYMin)	drYMin= sy2;
    if(sy2 > drYMax)	drYMax= sy2;
    VID_PutPixel(m_dest, sx2, sy2, 0x9999);
    if(sx3 < drXMin)	drXMin= sx3;
    if(sx3 > drXMax)	drXMax= sx3;
    if(sy3 < drYMin)	drYMin= sy3;
    if(sy3 > drYMax)	drYMax= sy3;
    VID_PutPixel(m_dest, sx3, sy3, 0x9999);
  }
*/}
/*=========================*/
LIB_API void			
V3D_IDI_Lines(VX2* p_ver, TRI2* p_tri, TC2* p_texCoord, TEXTURE* p_tex, ui32 p_nbV, ui32 p_nbI)
{
}
/*=========================*/
LIB_API void			
V3D_IDI_Flat(VX2* p_ver, TRI2* p_tri, TC2* p_texCoord, TEXTURE* p_tex, ui32 p_nbV, ui32 p_nbI)
{
  i32 	drXMin=239, drXMax= 0
  , 	drYMin= 319, drYMax=0;
  ui32 i;
  i32	j, k, index
  ,	maxy, miny
  ,	sxI, syI, sx1, sy1, sx2, sy2, z1, z2
  ,	sx4, sy4, sx5, sy5, sx6, sy6
  ,	fix, d, dz, dx, cz, cx, cy, y1, y2, x1, x2;
  ui16 	cz2;
  for (i = 0; i < p_nbI;++i){
  //todo incrementer ptri a place de [i]
    sx4 = p_ver[p_tri[i].idxVertex[0]].sX >> (FPP - 6);
    sy4 = p_ver[p_tri[i].idxVertex[0]].sY >> (FPP - 6);
    sx5 = p_ver[p_tri[i].idxVertex[1]].sX >> (FPP - 6);
    sy5 = p_ver[p_tri[i].idxVertex[1]].sY >> (FPP - 6);
    sx6 = p_ver[p_tri[i].idxVertex[2]].sX >> (FPP - 6);
    sy6 = p_ver[p_tri[i].idxVertex[2]].sY >> (FPP - 6);
    if((((sx4 - sx5) * (sy6 - sy5)) - ((sy4 - sy5) * (sx6 - sx5))) <= 0)
      continue;
    miny = 1200;
    maxy = 0;
    sx1 = p_ver[p_tri[i].idxVertex[0]].sX;
    sy1 = p_ver[p_tri[i].idxVertex[0]].sY;
    if(sx1 <= 0)		sx1=0;
    if(sx1 >= IFP(1200))	sx1=IFP(1199);	
    if(sy1 <= 0)		sy1=0;
    if(sy1 >= IFP(1200))	sy1=IFP(1199);	
    z1 = (0x7FFFFFF0 / p_ver[p_tri[i].idxVertex[0]].rot[2]) << FPP;
    for (j = 0; j < 3; j++ ){
      index= ((j+1)>2)?0:j+1;
      sx2 = p_ver[p_tri[i].idxVertex[index]].sX;
      sy2 = p_ver[p_tri[i].idxVertex[index]].sY;
      if(sx1 <= 0)			sx1=0;
      if(sx1 >= IFP(1200))	sx1=IFP(1199);	
      if(sy1 <= 0)			sy1=0;
      if(sy1 >= IFP(1200))	sy1=IFP(1199);	
      sxI= sx2>>FPP;
      syI= sy2>>FPP;
      if(sxI < drXMin)	drXMin= sxI;
      if(sxI > drXMax)	drXMax= sxI;
      if(syI < drYMin)	drYMin= syI;
      if(syI > drYMax)	drYMax= syI;
      z2 = (0x7FFFFFF0 / p_ver[p_tri[i].idxVertex[index]].rot[2]) << FPP;
      if (sy2 > sy1){
        cx = sx1;
        cy = sy1;
        cz = z1;
        y1 = (sy1 >> FPP) + 1;
        y2 = (sy2 >> FPP) + 1;
        if (y1 < miny) 	miny = y1;
        if (y2 > maxy) 	maxy = y2;
        d = sy2 - sy1;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx2 - sx1, d ); 
          dz = FPDIV( z2 - z1, d ); 
        }else 
          dx = 0;
        fix = FP_ONE - (sy1 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
      }else{
        cx = sx2;
        cy = sy2;
        cz = z2;
        y1 = (sy2 >> FPP) + 1;
        y2 = (sy1 >> FPP) + 1;
        if (y2 < miny) 	miny = y2;
        if (y1 > maxy) 	maxy = y1;
        d = sy1 - sy2;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx1 - sx2, d ); 
          dz = FPDIV( z1 - z2, d ); 
        }else 
          dx = 0;
        fix = FP_ONE - (sy2 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
      }
      for (k = y1; k < y2; k++){
        if (cx < m_Left[k]){
          m_Left[k] = cx;
          m_ZLeft[k] = cz;
        }
        if (cx > m_Right[k]){
          m_Right[k] = cx;
          m_ZRight[k] = cz;
        }
        cx += dx;
        cz += dz;
      }
      sx1 = sx2;
      sy1 = sy2;
      z1 = z2;
    }
    ui16 src = (ui16) 
	((p_ver[p_tri[i].idxVertex[0]].m_R<<11)|
	(p_ver[p_tri[i].idxVertex[0]].m_G<<5)| 
	(p_ver[p_tri[i].idxVertex[0]].m_B));
    ui16* dst = m_dest + (miny * 1200);
    ui16* zbf = m_ZBuffer + (miny * 1200);

    for ( j = miny; j < maxy; j++ ){
      x1 = m_Left[j] >> FPP; 
      x2 = m_Right[j] >> FPP;
      if (x2 > x1){
        dz = ((m_ZRight[j] - m_ZLeft[j]) / (m_Right[j] - m_Left[j])) >> (13 - FPP);
        cz2 = (ui16)(m_ZLeft[j] >> 13);
        for ( i32 k = x1; k < x2; k++ ){
          if (*(zbf + k) < cz2){
            *(dst + k) = src;
            *(zbf + k) = cz2;
          }
          cz2 += (ui16)dz;
        }
      }
      dst += 1200;
      zbf += 1200;
      m_Left[j] = IFP(1200);
      m_Right[j] = 0;
    }
  } 
}
/*=========================*/
LIB_API void			
V3D_IDI_Text(VX2* p_ver, TRI2* p_tri, TC2* p_texCoord, TEXTURE* p_tex, ui32 p_nbV, ui32 p_nbI)
{
  i32 	drXMin=239, drXMax= 0
  , 	drYMin= 319, drYMax=0;
  ui32 	i, cpt=0;
  i32	j, k, index
  ,	maxy, miny
  ,	sxI, syI, sx1, sy1, sx2, sy2, z1, z2
  ,	sx4, sy4, sx5, sy5, sx6, sy6
  ,	u1, v1, u2, v2
  ,	fix, d, du, dv, dz, dx
  , 	cu, cv, cz, cx, cy, y1, y2, x1, x2;
  ulong	cuv, duv;
  ui16 	cz2;

  ui16* src = p_tex->pData;
  for (i = 0; i < p_nbI;++i){
  //todo incrementer ptri a place de [i]
    sx4 = p_ver[p_tri[i].idxVertex[0]].sX >> (FPP - 6);
    sy4 = p_ver[p_tri[i].idxVertex[0]].sY >> (FPP - 6);
    sx5 = p_ver[p_tri[i].idxVertex[1]].sX >> (FPP - 6);
    sy5 = p_ver[p_tri[i].idxVertex[1]].sY >> (FPP - 6);
    sx6 = p_ver[p_tri[i].idxVertex[2]].sX >> (FPP - 6);
    sy6 = p_ver[p_tri[i].idxVertex[2]].sY >> (FPP - 6);
    if((((sy4 - sy5) * (sx6 - sx5)) - ((sx4 - sx5) * (sy6 - sy5))) <= 0)
      {++cpt;continue;}
    miny = 1200;
    maxy = 0;
    sx1 = p_ver[p_tri[i].idxVertex[0]].sX;
    sy1 = p_ver[p_tri[i].idxVertex[0]].sY;
    if(sx1 <= 0)		sx1=0;
    if(sx1 >= IFP(1200))	sx1=IFP(1199);	
    if(sy1 <= 0)		sy1=0;
    if(sy1 >= IFP(1200))	sy1=IFP(1199);	
    u1 = p_texCoord[p_tri[i].idxTexCoor[0]].m_U1;
    v1 = p_texCoord[p_tri[i].idxTexCoor[0]].m_V1;
    z1 = (0x7FFFFFF0 / p_ver[p_tri[i].idxVertex[0]].rot[2]) << FPP;
    for (j = 0; j < 3; j++ ){
      index= ((j+1)>2)?0:j+1;
      sx2 = p_ver[p_tri[i].idxVertex[index]].sX;
      sy2 = p_ver[p_tri[i].idxVertex[index]].sY;
      if(sx1 <= 0)			sx1=0;
      if(sx1 >= IFP(1200))	sx1=IFP(1199);	
      if(sy1 <= 0)			sy1=0;
      if(sy1 >= IFP(1200))	sy1=IFP(1199);	
      sxI= sx2>>FPP;
      syI= sy2>>FPP;
      if(sxI < drXMin)	drXMin= sxI;
      if(sxI > drXMax)	drXMax= sxI;
      if(syI < drYMin)	drYMin= syI;
      if(syI > drYMax)	drYMax= syI;
      u2 = p_texCoord[p_tri[i].idxTexCoor[index]].m_U1;
      v2 = p_texCoord[p_tri[i].idxTexCoor[index]].m_V1;
      z2 = (0x7FFFFFF0 / p_ver[p_tri[i].idxVertex[index]].rot[2]) << FPP;
      if (sy2 > sy1){
        cx = sx1;
        cy = sy1;
        cu = u1;
        cv = v1;
        cz = z1;
        y1 = (sy1 >> FPP) + 1;
        y2 = (sy2 >> FPP) + 1;
        if (y1 < miny) 	miny = y1;
        if (y2 > maxy) 	maxy = y2;
        d = sy2 - sy1;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx2 - sx1, d ); 
          du = FPDIV( u2 - u1, d ); 
          dv = FPDIV( v2 - v1, d ); 
          dz = FPDIV( z2 - z1, d ); 
        }else 
          du = dv = dx = 0;
        fix = FP_ONE - (sy1 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
        cu += FPMUL( fix, du );
        cv += FPMUL( fix, dv );
      }else{
        cx = sx2;
        cy = sy2;
        cu = u2;
        cv = v2;
        cz = z2;
        y1 = (sy2 >> FPP) + 1;
        y2 = (sy1 >> FPP) + 1;
        if (y2 < miny) 	miny = y2;
        if (y1 > maxy) 	maxy = y1;
        d = sy1 - sy2;
        if ((d > 4) || (d < -4)){
          dx = FPDIV( sx1 - sx2, d ); 
          du = FPDIV( u1 - u2, d ); 
          dv = FPDIV( v1 - v2, d ); 
          dz = FPDIV( z1 - z2, d ); 
        }else 
           du = dv = dx = 0;
        fix = FP_ONE - (sy2 & (FP_ONE - 1));
        cx += FPMUL( fix, dx );
        cu += FPMUL( fix, du );
        cv += FPMUL( fix, dv );
      }
      for (k = y1; k < y2; k++){
        if (cx < m_Left[k]){
          m_Left[k] = cx;
          m_ULeft[k] = cu;
          m_VLeft[k] = cv;
          m_ZLeft[k] = cz;
        }
        if (cx > m_Right[k]){
          m_Right[k] = cx;
          m_URight[k] = cu;
          m_VRight[k] = cv;
          m_ZRight[k] = cz;
        }
        cx += dx;
        cu += du;
        cv += dv;
        cz += dz;
      }
      sx1 = sx2;
      sy1 = sy2;
      u1 = u2;
      v1 = v2;
      z1 = z2;
    }
    ui16* dst = m_dest + (miny * 1200);
    ui16* zbf = m_ZBuffer + (miny * 1200);

    for ( j = miny; j < maxy; j++ ){
      x1 = m_Left[j] >> FPP; 
      x2 = m_Right[j] >> FPP;
      if (x2 > x1){
        du = FPDIV(m_URight[j] - m_ULeft[j], m_Right[j] - m_Left[j]);
        dv = FPDIV(m_VRight[j] - m_VLeft[j], m_Right[j] - m_Left[j]);
        dz = ((m_ZRight[j] - m_ZLeft[j]) / (m_Right[j] - m_Left[j])) >> (13 - FPP);
        cuv = ((m_ULeft[j] >> (FPP - 8)) << 16) + (m_VLeft[j] >> (FPP - 8));
        duv = ((du >> (FPP - 8)) << 16) + (dv >> (FPP - 8));
        cz2 = (ui16)(m_ZLeft[j] >> 13);
        for ( i32 k = x1; k < x2; k++ ){
          if(*(zbf + k) < cz2){
//color-key
//          if((*(zbf + k) < cz2) && (*(src + (cuv >> 24) + (cuv & 0xFF00)))){
            *(dst + k) = *(src + (cuv >> 24) + (cuv & 0xFF00));
            *(zbf + k) = cz2;
          }
          cuv += duv;
          cz2 += (ui16)dz;
        }
      }
      dst += 1200;
      zbf += 1200;
      m_Left[j] = IFP(1200);
      m_Right[j] = 0;
    }
  }
  string_char chaine[64];
  sprintf(chaine, "skip : %d on %d\n", cpt, p_nbI);
  OutputDebugString(chaine);	
}


LIB_API void 
V3D_SetRenderer(i32 wichOne)
  {ASSERT((wichOne == REN_SOFTWARE), 
    "Only Software Work for PPC version")};

LIB_API void 
V3D_SetRendererStage(i32 wichOne)
{							
    switch(wichOne){
      case RST_POINT:	
        Drawer	= V3D_ID_Point;
        DrawerIB= V3D_IDI_Point;	
        break;
      case RST_LINES:
        Drawer	= V3D_ID_Lines;
        DrawerIB= V3D_IDI_Lines;	
        break;
      case RST_FLAT:	
        Drawer	= V3D_ID_Flat;	
        DrawerIB= V3D_IDI_Flat;	
        break;
      case RST_TEXT:	
        Drawer	= V3D_ID_Text;	
        DrawerIB= V3D_IDI_Text;	
        break;
    }
};

LIB_API void 
V3D_SetAnimatorStage(int wichOne)
{							
    switch(wichOne){
      case ANI_NEAREST:	
        Process= V3D_IPI_Nearest;	
        break;
      case ANI_LINEAR:	
        Process= V3D_IPI_Linear;	
        break;
      case ANI_COSINE:	
        Process= V3D_IPI_Cosine;	
        break;
      case ANI_CUBIC:		
        Process= V3D_IPI_Cubic;	
        break;
    }
};




