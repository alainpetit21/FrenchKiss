#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "FrenchKISS.h"
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
FK_API PTFILE	FS_CreateFile(string filename, i32 option);
FK_API BOOL		FS_ReadFile	(PTFILE file, void* buf, ulong len);
FK_API BOOL		FS_Write(PTFILE file, void* p_buf, ulong p_nb);
FK_API byte		FS_ReadByte	(PTFILE file);
FK_API float	FS_ReadFloat(PTFILE file);
FK_API word		FS_ReadWord	(PTFILE file);
FK_API dword	FS_ReadDword(PTFILE file);
FK_API BOOL		FS_CloseFile(PTFILE file);
FK_API BOOL		FS_SetPos(PTFILE file, i32 pos, i32 flag);
/*==============================================================================================================*/
/*==============================================================================================================*/
#endif /*_FILE_SYSTEM_H_*/
