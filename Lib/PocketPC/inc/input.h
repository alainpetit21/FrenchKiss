#ifndef _INPUT_H_
#define _INPUT_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "FrenchKISS.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
#define KEY_DOWN		0x01
#define KEY_LEFT		0x02
#define KEY_RIGHT		0x04
#define KEY_UP			0x08
#define KEY_FIRE		0x10
#define KEY_OK			0x20
#define KEY_OPTION	0x40
#define KEY_HELP		0x80
#define KEY_ANY_KEY	0xFF
/*==============================================================================================================*/
/*==============================================================================================================*/
#define MSK_MOUSE_DOWN	0x40000000
#define MSK_MOUSE_X			0x3FFF8000
#define MSK_MOUSE_Y			0x00007FFF
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API extern unsigned char gJoyBuf;
FK_API extern unsigned char gJoyBuf2;
FK_API extern unsigned long gMouseBuf;
/*==============================================================================================================*/
/*==============================================================================================================*/
#define INP_Init()	\
  gJoyBuf= 0;				\
  VERIFY(GXOpenInput(), L"Can't Open Input");
#define INP_Quit()								VERIFY(GXCloseInput(), L"Can't Close Input");
#define INP_Update								gJoyBuf2= gJoyBuf;
#define INP_IsKeyDown(p_vkKey)		(gJoyBuf&p_vkKey)
#define INP_IsKeyDownAB(p_vkKey)	\
  ((gJoyBuf&p_vkKey)&&(!(gJoyBuf2&p_vkKey)))
#define INP_ReadMouseStatus(pnX, pnY, pnB)		\
  pnB=	(int)((gMouseBuf&MSK_MOUSE_DOWN)>>30);\
  pnX=	(int)((gMouseBuf&MSK_MOUSE_X)>>15);		\
  pnY=	(int)(gMouseBuf&MSK_MOUSE_Y);			
FK_API void INP_Trap(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
/*==============================================================================================================*/
/*==============================================================================================================*/
#endif /*_INPUT_H_*/
