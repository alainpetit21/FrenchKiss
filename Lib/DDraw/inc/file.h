#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "Lib.h"
#include "types.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
typedef HANDLE			PTFILE;
#define OPT_READ		GENERIC_READ
#define OPT_WRITE		GENERIC_WRITE
#define FRM_BEGIN		FILE_BEGIN
#define FRM_CURRENT	FILE_CURRENT
#define FRM_END			FILE_END
/*==============================================================================================================*/
/*==============================================================================================================*/
LIB_API PTFILE	FS_CreateFile(string filename, i32 option);
LIB_API BOOL		FS_ReadFile	(PTFILE file, void* buf, ulong len);
LIB_API BOOL		FS_Write(PTFILE file, void* p_buf, ulong p_nb);
LIB_API byte		FS_ReadByte	(PTFILE file);
LIB_API float	FS_ReadFloat(PTFILE file);
LIB_API word		FS_ReadWord	(PTFILE file);
LIB_API dword	FS_ReadDword(PTFILE file);
LIB_API BOOL		FS_CloseFile(PTFILE file);
LIB_API BOOL		FS_SetPos(PTFILE file, i32 pos, i32 flag);
/*==============================================================================================================*/
/*==============================================================================================================*/
#endif /*_FILE_SYSTEM_H_*/
