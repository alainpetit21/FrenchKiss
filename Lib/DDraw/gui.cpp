#include <windows.h>
#include "macros.h"
#include "gui.h"
#include "resman.h"
#include "file.h"
#include "graphic.h"

GUI_MSG		m_msgList[64];
uint		m_nbWindow;
uint		m_toRead;
uint		m_toAdd;
uint		m_mouseStatus;
int			m_lastX;
int			m_lastY;
GWindow*	m_curWindow;
uint		m_firstBmp;

LIB_API void 
GUI_Init()
{

}

LIB_API void 
GUI_Quit()
{

}

LIB_API void 
GUI_Open(string fileGuiCfg)
{
	PTFILE	fichier;
	char*	signature[4];
	dword	doubleMot;

	fichier= FS_CreateFile(fileGuiCfg, OPT_READ);
	doubleMot= FS_ReadDword(fichier);
	memcpy(signature, &doubleMot, 4);
	VERIFY((memcmp(signature, "PPC", 4) == 0), "Invalid GUI file");

	m_firstBmp= RM_GetBufferCurOffset(RMI_IMAGE);
	//1-Initialise Bitmap
	{
		uint	nbBitmap= FS_ReadDword(fichier);
		while(nbBitmap--){
			char filename[128];
			FS_ReadFile(fichier, filename, 128);
			VID_LoadTGA(CUR_IMAGE, CUR_PIXEL, filename);
		}
	}
	//2-Initialise Windows
	{
		uint	nbWindow= FS_ReadDword(fichier);
		RM_Allocate(RMI_WINDOW, nbWindow);
		while(nbWindow--){
			GWindow** dst= CUR_WINDOW;
			FS_ReadFile(fichier, (*dst), sizeof(GWindow));
			(*dst)++;
		}
	}
	//3-Initialise Groupes
	{
		uint	nbGroup= FS_ReadDword(fichier);
		RM_Allocate(RMI_GROUPE, nbGroup);
		while(nbGroup--){
			GGroupe** dst= CUR_GROUPE;
			FS_ReadFile(fichier, (*dst), sizeof(GGroupe));
			(*dst)++;
		}
	}
	//4-Initialise Controles
	{
		uint	nbControl= FS_ReadDword(fichier);
		RM_Allocate(RMI_CONTROL, nbControl);
		while(nbControl--){
			GControl** dst= CUR_CONTROL;
			FS_ReadFile(fichier, (*dst), sizeof(GControl));
			(*dst)++;
		}
	}
}

LIB_API void 
GUI_MapButtonPressed(uint pWnd, uint pCtl, EVENT_CALLBACK pFct)
{
	
}

LIB_API void 
GUI_Update()
{

}

LIB_API void 
GUI_PostMessage(GUI_MSG* p_msg)
{
	ASSERT((m_toAdd!=(m_toRead-1)), "MsgListFull");
	
	memcpy(m_msgList+m_toAdd, p_msg, sizeof(GUI_MSG));
	if(++m_toAdd >= 64)
		m_toAdd= 0;
}

LIB_API GUI_MSG* 
GUI_GetMessage()
{
	uint ret;

	if(m_toAdd == m_toRead)	return NULL;

	ret= m_toRead;
	if(++m_toRead >= 64)
		m_toRead= 0;

	return m_msgList+ret;
}

/*Internal*/
LIB_API void 
GUI_ITrap()
{
	
}