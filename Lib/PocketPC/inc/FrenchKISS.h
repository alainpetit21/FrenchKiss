#ifndef _FRENCHKISS_DLL_H_
#define _FRENCHKISS_DLL_H_

#ifdef EXPORTS
	#define FK_API __declspec(dllexport)
#else
	#define FK_API __declspec(dllimport)
#endif

#endif //_FRENCHKISS_DLL_H_