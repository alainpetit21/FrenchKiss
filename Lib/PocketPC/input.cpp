#include "stdafx.h"
#include "gx.h"
#include "macros.h"
#include "input.h"
#include "types.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
#define WINCE_KEY_DOWN		40
#define WINCE_KEY_LEFT		37
#define WINCE_KEY_RIGHT		39
#define WINCE_KEY_UP			38
#define WINCE_KEY_FIRE		196
#define WINCE_KEY_OK			195
#define WINCE_KEY_OPTION	192
#define WINCE_KEY_HELP		194
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API unsigned char gJoyBuf;
FK_API unsigned char gJoyBuf2;
FK_API unsigned long gMouseBuf;
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API void 
INP_Trap(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message){
    case WM_KEYDOWN:
      switch(wParam){	
				case WINCE_KEY_DOWN:		gJoyBuf|= KEY_DOWN;		break;
        case WINCE_KEY_LEFT:		gJoyBuf|= KEY_LEFT;		break;
        case WINCE_KEY_RIGHT:		gJoyBuf|= KEY_RIGHT;	break;
        case WINCE_KEY_UP:			gJoyBuf|= KEY_UP;			break;
        case WINCE_KEY_FIRE:		gJoyBuf|= KEY_FIRE;		break;
        case WINCE_KEY_OK:			gJoyBuf|= KEY_OK;			break;
        case WINCE_KEY_OPTION:	gJoyBuf|= KEY_OPTION;	break;
        case WINCE_KEY_HELP:		gJoyBuf|= KEY_HELP;		break;
      }
      return;
    case WM_KEYUP:
      switch(wParam){
        case WINCE_KEY_DOWN:		gJoyBuf&= ~KEY_DOWN;	break;
        case WINCE_KEY_LEFT:		gJoyBuf&= ~KEY_LEFT;	break;
        case WINCE_KEY_RIGHT:		gJoyBuf&= ~KEY_RIGHT;	break;
        case WINCE_KEY_UP:			gJoyBuf&= ~KEY_UP;		break;
        case WINCE_KEY_FIRE:		gJoyBuf&= ~KEY_FIRE;	break;
        case WINCE_KEY_OK:			gJoyBuf&= ~KEY_OK;		break;
        case WINCE_KEY_OPTION:	gJoyBuf&= ~KEY_OPTION;break;
        case WINCE_KEY_HELP:		gJoyBuf&= ~KEY_HELP;	break;
      }
      return;
    case WM_LBUTTONDOWN:
      gMouseBuf= MSK_MOUSE_DOWN;
      gMouseBuf|= (LOWORD(lParam)<<15);
      gMouseBuf|= ((int)HIWORD(lParam))+26;
      return;
    case WM_MOUSEMOVE:
      gMouseBuf= MSK_MOUSE_DOWN;
      gMouseBuf|= (LOWORD(lParam)<<15);
      gMouseBuf|= ((int)HIWORD(lParam))+26;
      return;
    case WM_LBUTTONUP:
      gMouseBuf&= ~MSK_MOUSE_DOWN;
      return;
  }
  return;
}
/*==============================================================================================================*/
/*==============================================================================================================*/
