#ifndef _GUI_H_
#define _GUI_H_
/*=========================*/
/*=========================*/
#include "types.h"
/*=========================*/
/*=========================*/
typedef uint (*EVENT_CALLBACK)(uint lParam, uint rParam);
/*=========================*/
typedef enum tagCONTROL_STATE{
	CS_UP		=0x00,
	CS_DOWN		=0x01,
	CS_SELECTED	=0x02,
	CS_FOCUS	=0x04,
	CS_CHANGED	=0x08,
}CONTROL_STATE;
/*=========================*/
typedef enum tagGUI_EVENTS{
	E_Nil				= 0x0	,
	EW_Enter					,
	EW_Exit						,
	EW_Goto						,
	EC_Enter					,
	EC_Exit						,
	EC_Unselect					,
	EC_Select					,
	EC_Repeat					,
	EL_Enter					,
	EL_Select					,
	EL_Exit						,	
	EF_Enter					,
	EF_Changed					,
	EGUI_SetDirty				,
	EGUI_SetAllDirty			,
	FORCEDWORD_EVENT	= 0x7FFFFFFF,
}GUI_EVENTS;
/*=========================*/
/*=========================*/
typedef struct tagGUI_MSG{
	GUI_EVENTS	eType;
	uint		penDown;
	uint		screenX;
	uint		screenY;
	union E_Data{
		struct E_Generic{
			uint	data1;
			uint	data2;
			uint	data3;
			uint	data4;
			uint	data5;
			uint	data6;
			uint	data7;
			uint	data8;
			} eGeneric;
		struct W_Enter{
			uint	enterWindow; 
			uint	exitWindow;
			} wEnter;
		struct W_Exit{
			uint	enterWindow; 
			uint	exitWindow;
			} wExit;
		struct W_Goto{
			uint	formID;
			} wGoto;
		struct C_Enter{
			uint	controlID;
			void* 	pControl;
			} cEnter;
		struct C_Exit{
			uint	controlID;
			void*	pControl;
			uint	on;
			} cExit;
		struct C_Repeat{
			uint	controlID;
			void*	pControl;
			ulong	time;
			} cRepeat;
		struct F_Enter{
			uint	fieldID;
			void*   pField;
			} fEnter;
		struct F_Changed{
			uint	fieldID;
			void*	pField;
			} fChanged;
		struct F_Exit{
			uint	fieldID;
			void*	pField;
			} fExit;
		struct L_Enter{
			uint	listID;
			void*	pList;
			uint	selection;
			} lEnter;
		struct L_Exit{
			uint	listID;
			void*	pList;
			} lEXIT;
		struct L_Selection{
			uint	listID;
			void*	pList;
			uint	selection;
			} lSelection;
		} eData;
}GUI_MSG;
/*=========================*/
typedef struct tagGControl{
	uint			m_nId;
	uint			m_nState;
	int				m_rLeft, m_rTop, m_rRight, m_rBottom;
	uint			m_nBitmap[2];
	void*			m_groupe;
	EVENT_CALLBACK	m_callback;
}GControl;
/*=========================*/
typedef struct tagGGroupe{
	void*		m_idxControle;
	void*		m_selected;
}GGroupe;
/*=========================*/
typedef struct tagGWindow{
	uint	m_ctlMax;
	uint	m_backGround;
	void*	m_arControle;
	void*	m_arGroupes;
	void*	m_curControl;
}GWindow;
/*=========================*/
/*=========================*/
LIB_API void		GUI_Init();
LIB_API void		GUI_Quit();
LIB_API void		GUI_Open(string fileGuiCfg);
LIB_API void		GUI_MapButtonPressed(uint pWnd, uint pCtl, EVENT_CALLBACK pFct);
LIB_API void		GUI_Update();
LIB_API void		GUI_PostMessage(GUI_MSG* p_msg);
LIB_API GUI_MSG*	GUI_GetMessage();
LIB_API void		GUI_ITrap();
/*=========================*/
/*=========================*/
#endif /*_GUI_H_*/