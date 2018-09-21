#include "stdafx.h"
#include "file.h"
#include "macros.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
#ifdef _DEBUG
	static string_char texte[128];
#endif
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API PTFILE	
FS_CreateFile(string filename, i32 option)
{
  PTFILE ret;
#ifdef _DEBUG
  SPRINTF(texte, L"%s%s", L"Can't Open File", filename);
#endif
  VERIFY(((ret= CreateFile(filename, option, 0, 0, OPEN_EXISTING,0,0)) != INVALID_HANDLE_VALUE),
    texte);
  return ret;
}
/*==============================================================================================================*/
FK_API BOOL	
FS_ReadFile	(PTFILE file, void* buf, ulong len)
{
  ulong dwRead;
  ReadFile(file, buf, len, &dwRead, NULL);
  return dwRead == len;
}
/*==============================================================================================================*/
FK_API BOOL
FS_Write(PTFILE file, void* buf, ulong len)
{
  ulong dwWritten;
  WriteFile(file, buf, len, &dwWritten, NULL);
  return dwWritten == len;
}
/*==============================================================================================================*/
FK_API byte	
FS_ReadByte(PTFILE file)
{
  ulong read;
  uchar	data;
  ReadFile(file, &data, 1, &read, 0);
  ASSERT((read == 1), _T"Error Reading File");
  return data;
}
/*==============================================================================================================*/
FK_API float	
FS_ReadFloat(PTFILE file)
{
  ulong read;
  float	data;
  ReadFile(file, &data, 4, &read, 0);
  ASSERT((read == 4), _T"Error Reading File");
  return data;
}
/*==============================================================================================================*/
FK_API word	
FS_ReadWord	(PTFILE file)
{
  ulong		read;
  ushort	data;
  ReadFile(file, &data, 2, &read, 0);
  ASSERT((read == 2), _T"Error Reading File");
  return data;
}
/*==============================================================================================================*/
FK_API dword	
FS_ReadDword(PTFILE file)
{
  ulong read;
  ulong	data;
  ReadFile(file, &data, 4, &read, 0);
  ASSERT((read == 4), _T"Error Reading File");
  return data;
}
/*==============================================================================================================*/
FK_API BOOL	
FS_CloseFile(PTFILE file)
{
  int ret= CloseHandle(file);
  VERIFY(ret, _T"Can't Close File");
  return ret;
}
/*==============================================================================================================*/
FK_API BOOL	
FS_SetPos(PTFILE file, i32 pos, i32 flag)
{
  return SetFilePointer(file, pos, 0, flag) != 0xFFFFFFFF;
}
/*==============================================================================================================*/
/*==============================================================================================================*/
