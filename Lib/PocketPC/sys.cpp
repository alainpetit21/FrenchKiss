#include "stdafx.h"
#include "sys.h"
#include "audio.h"
#include "graphic.h"
#include "input.h"
#include "macros.h"
#include "types.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
WNDCLASS	wc;
__int64		gFreq;
HINSTANCE	ghInstance	=0;
HWND			ghWnd				=0;
string		ghAppName		=0;
string		ghClassName	=0;
/*==============================================================================================================*/
/*==============================================================================================================*/
long FAR PASCAL 
WindowProc(HWND hWnd, UINT message,	WPARAM wParam, LPARAM lParam)
{
  INP_Trap(hWnd, message, wParam, lParam);
	AUD_Trap(hWnd, message, wParam, lParam);
	
  if(message == WM_DESTROY){
      PostQuitMessage(0);
      return 0;
	}else{
      return ::DefWindowProc(hWnd, message, wParam, lParam);
  }
}
/*==============================================================================================================*/
FK_API void SYS_Init(string p_appName, string p_className, HINSTANCE hInstance)
{
  if(ghWnd= FindWindow(ghAppName= p_appName, ghClassName= p_className)){
    SetForegroundWindow(ghWnd);
    return;
  }
  ghInstance= hInstance;

  wc.style				= CS_HREDRAW | CS_VREDRAW ;
  wc.lpfnWndProc	= (WNDPROC)WindowProc;
  wc.cbClsExtra		= 0;
  wc.cbWndExtra		= 0;
  wc.hIcon				= 0;
  wc.hInstance		= ghInstance;
  wc.hCursor			= NULL;
  wc.hbrBackground= (HBRUSH)  GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName= ghClassName;

  VERIFY(RegisterClass(&wc), _T"Can't Register Class");
  VERIFY(ghWnd= CreateWindow(
									ghClassName, ghAppName, 
									WS_VISIBLE, CW_USEDEFAULT,	
									CW_USEDEFAULT, 
									CW_USEDEFAULT, 
									CW_USEDEFAULT,	NULL, NULL, 
									ghInstance, NULL ),
									_T"Can't Create Window");

  VERIFY(QueryPerformanceFrequency((LARGE_INTEGER*)&gFreq), _T"No HW clock");

	ShowWindow((HWND)ghWnd, SW_SHOWNORMAL);
	UpdateWindow((HWND)ghWnd);
}
/*==============================================================================================================*/
FK_API void SYS_Quit()
{
}
/*==============================================================================================================*/
/*==============================================================================================================*/
