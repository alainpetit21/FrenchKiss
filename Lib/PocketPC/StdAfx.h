#ifndef _STDAFX_H_
#define _STDAFX_H_

#ifdef DEBUG
	#ifndef _DEBUG
		#define _DEBUG
	#endif
#endif
#ifdef _DEBUG
	#ifndef DEBUG
		#define DEBUG
	#endif
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


//{{AFX_INSERT_LOCATION}}
#endif // _STDAFX_H_
