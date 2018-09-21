#ifndef _MACROS_H_
#define _MACROS_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "Lib.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
#undef 	SAFE_DELETE
#define SAFE_DELETE(a)				if (a){delete a; a= 0;}
/*==============================================================================================================*/
#undef	SAFE_RELEASE
#define SAFE_RELEASE(a)				if (a){a->Release(); a= 0;}
/*==============================================================================================================*/
#undef	SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a)	if (a){delete []a; a= 0;}
/*==============================================================================================================*/
#undef	ARRAY_SIZE
#define ARRAY_SIZE(a)					(sizeof(a) / sizeof(a[0]))
/*==============================================================================================================*/
#if defined _DEBUG && !defined(DEBUG)
	#define DEBUG
#elif defined DEBUG && !defined(_DEBUG)
	#define _DEBUG
#endif
/*==============================================================================================================*/
#undef _T
#ifndef _UNICODE
  #define _T
#else
  #define _T	L
#endif
/*==============================================================================================================*/
#undef PRINTF
#undef SPRINTF
#ifdef _UNICODE
  #define PRINTF	wprintf
  #define SPRINTF	wsprintf
#else
  #define PRINTF	printf
  #define SPRINTF	sprintf
#endif
/*==============================================================================================================*/
#undef ASSERT
#ifdef _POCKET_PC
  #ifdef _DEBUG
    #define ASSERT(a, s)			if(!a){::MessageBox(NULL, s, 0, 0);exit(-1);}
  #else
    #define ASSERT(a, s)
  #endif	// DEBUG
#elif WIN32
  #ifdef _DEBUG
    #define ASSERT(a, s)			if(!a){::MessageBox(NULL, s, 0, 0);exit(-1);}
  #else
    #define ASSERT(a, s)
  #endif	// DEBUG
#else
  #ifdef _DEBUG
    #define ASSERT(a, s)			if (!a)	{9/a;}
  #else
    #define ASSERT(a, s)
  #endif	// DEBUG
#endif	// _POCKET_PC
/*==============================================================================================================*/
#undef	VERIFY
#ifdef _POCKET_PC
  #ifdef _DEBUG
    #define VERIFY(a, s)			if(!(a)){::MessageBox(NULL, s, 0, 0);exit(-1);}
  #else
    #define VERIFY(a, s)			a;
  #endif	// DEBUG
#elif WIN32
  #ifdef _DEBUG
    #define VERIFY(a, s)			if(!(a)){::MessageBox(NULL, s, 0, 0);exit(-1);}
  #else
    #define VERIFY(a, s)			a;
  #endif	// DEBUG
#else
  #ifdef _DEBUG
    #define VERIFY(a, s)			if (!(a)){9/a;}	// Will crash!
  #else
    #define VERIFY(a, s)			a;
  #endif // DEBUG
#endif	// _POCKET_PC
/*==============================================================================================================*/
#undef TRAP_MSG
#if defined(WIN32)|| defined(_POCKET_PC)
  #define TRAP_MSG(a)												\
    if(PeekMessage(&a,NULL,0,0,PM_REMOVE)){	\
      if(a.message == WM_QUIT) break;				\
        TranslateMessage(&a);								\
        DispatchMessage(&a);								\
      }
#endif
/*==============================================================================================================*/
/*==============================================================================================================*/
#undef PROFILER_INC
#undef PROFILER_DEC
#undef PROFILER_INIT
#undef PROFILER_BEFORE
#undef PROFILER_AFTER
#ifdef _PROFILE
  #if defined(_POCKET_PC)
    #define PROFILER_INC
    #define PROFILER_DEC	\
      __int64 freq;				\
      __int64 timePrev;		\
      __int64 time;				\
      wchar_t  buf[64];														      
   #define PROFILER_INIT					\
    ::QueryPerformanceFrequency(	\
      (LARGE_INTEGER*)&freq);			\
    buf[0]='\0';
  #define PROFILER_BEFORE				\
    ::QueryPerformanceCounter(	\
      (LARGE_INTEGER*)&time);		\
    timePrev= time;
  #define PROFILER_AFTER(string, returnLine)\
    ::QueryPerformanceCounter(							\
      (LARGE_INTEGER*)&time);								\
    swprintf(buf,_T"%s%d%s", string,				\
      (uint)(time-timePrev), returnLine);		\
    OutputDebugString(buf);
  #elif defined(WIN32)
    #define PROFILER_INC
    #define PROFILER_DEC	\
      __int64 freq;				\
      __int64 timePrev;		\
      __int64 time;				\
      char  buf[64];														      
   #define PROFILER_INIT					\
    ::QueryPerformanceFrequency(	\
      (LARGE_INTEGER*)&freq);			\
    buf[0]='\0';
  #define PROFILER_BEFORE				\
    ::QueryPerformanceCounter(	\
      (LARGE_INTEGER*)&time);		\
    timePrev= time;
  #define PROFILER_AFTER(string, returnLine)\
    ::QueryPerformanceCounter(							\
      (LARGE_INTEGER*)&time);								\
    sprintf(buf,"%s%d%s", string,				\
      (uint)(time-timePrev), returnLine);		\
    OutputDebugString(buf);
  #endif	//	_POCKET_PC
#else
  #define PROFILER_INC
  #define PROFILER_DEC
  #define PROFILER_INIT
  #define PROFILER_BEFORE
  #define PROFILER_AFTER(string, returnLine)
#endif	//	_PROFILE
/*==============================================================================================================*/
/*==============================================================================================================*/
#undef CHAR_TO_DIGIT
#define CHAR_TO_DIGIT(a)	((a) - '0')
/*==============================================================================================================*/
#undef DIGIT_TO_CHAR
#define DIGIT_TO_CHAR(a)	((a) + '0')
/*==============================================================================================================*/
#undef	SWAP32
#define SWAP32(a)									\
((((long)a) & 0xff000000)	>> 24)|	\
((((long)a) & 0x00ff0000)	>> 8)	|	\
((((long)a) & 0x0000ff00)	<< 8)	|	\
((((long)a) & 0x000000ff)	<< 24)
/*==============================================================================================================*/
#undef	SWAP16
#define SWAP16(a)									\
((((short)a) & 0xff00) >> 8)		|	\
((((short)a) & 0x00ff) << 8) 
/*==============================================================================================================*/
/*==============================================================================================================*/
#undef	PI
#define PI	3.1415926535f
/*==============================================================================================================*/
#undef	IS_ODD
#define IS_ODD(a)	(((uchar)a)&1)
/*==============================================================================================================*/
#undef	IS_EVEN
#define IS_EVEN(a)	(!IS_ODD(a))
/*==============================================================================================================*/
#undef	SIGN
#define SIGN(a)	((p<0)?-1:1)
/*==============================================================================================================*/
#undef	DEG_TO_RAD
#define DEG_TO_RAD(a)	(p*PI/180.0f)
/*==============================================================================================================*/
#undef	RAD_TO_DEG
#define RAD_TO_DEG(a)	(p*180.0f/PI)
/*==============================================================================================================*/
#undef	RAND_RANGE
#define RAND_RANGE(a,b)	(a + (rand()%(b-a+1)))
/*==============================================================================================================*/
#undef	MIN
#define MIN(a,b)	((a<b)?a:b)
/*==============================================================================================================*/
#undef	MAX
#define MAX(a,b)	((a>b)?a:b)
/*==============================================================================================================*/
#undef	ABS
#define ABS(a)		((a>0)?a:-a)
/*==============================================================================================================*/
#undef	SWAP
#define SWAP(a,b,t)	t=a;a=b;a=t;
/*==============================================================================================================*/
#define KB		<<10
#define MB		<<20
/*==============================================================================================================*/
/*==============================================================================================================*/
#endif	// _MACROS_H_
