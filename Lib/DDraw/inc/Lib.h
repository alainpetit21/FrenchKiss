#ifndef _LIB_DLL_H_
#define _LIB_DLL_H_

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef EXPORTS
	#define LIB_API __declspec(dllexport)
#else
	#define LIB_API __declspec(dllimport)
#endif

#endif //_LIB_DLL_H_