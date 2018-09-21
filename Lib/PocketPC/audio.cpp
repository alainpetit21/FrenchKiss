#include "stdafx.h"
#include "audio.h"
#include "file.h"
#include "Macros.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
HWAVEOUT			gHWO;
WAVEFORMATEX	gWF;
WAVEHDR				gHW;
ui32					gbCanWrite=0;
/*==============================================================================================================*/
/*==============================================================================================================*/
FK_API void
AUD_Init(i32 bps, i32 channels, i32 khz)
{
  VERIFY((waveOutGetNumDevs() != 0), _T"No Sound Devices");

  memset(&gHWO,	0, sizeof(gHWO));
  memset(&gWF,	0, sizeof(gWF));
  memset(&gHW,	0, sizeof(gHW));
	
  gWF.cbSize					= 0;
  gWF.wFormatTag			= WAVE_FORMAT_PCM;
  gWF.wBitsPerSample	= (ui16)bps;
  gWF.nChannels				= (ui16)channels;
  gWF.nSamplesPerSec	= khz;
  gWF.nBlockAlign			= (channels * khz / 8);
  gWF.nAvgBytesPerSec	= (gWF.nBlockAlign * khz);

  VERIFY((waveOutOpen(&gHWO, 0, (WAVEFORMATEX*)&gWF, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR),
    _T"Cant Open Wave Device");

  gbCanWrite	= 1;
}
/*==============================================================================================================*/
FK_API long
AUD_Trap(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if(message == WOM_DONE){
      waveOutUnprepareHeader(gHWO, &gHW, (ui32)sizeof(WAVEHDR));
      gbCanWrite= 1;
      return 1;
  }
  return 0;
}
/*==============================================================================================================*/
FK_API void
AUD_Quit(void)
{
  VERIFY((waveOutReset(gHWO) == MMSYSERR_NOERROR), 
    _T"Can't Reset Device");
  VERIFY((waveOutClose(gHWO) == MMSYSERR_NOERROR), 
    _T"Can't Close Device");
}
/*==============================================================================================================*/
FK_API void
AUD_IPreWavAllocation(WAVESOUND **pWSnd, PTFILE pWaveFile)
{
  typedef struct tagWAVE_CHUNK{
    ulong ckid;
    ulong cksize;
  } WAVE_CHUNK;
	
  WAVE_CHUNK waveChunk = {0,0};
  ulong		buf32;
  uchar		bFinished = 0;
  uchar		miniBuffer= 0;
    
  FS_ReadFile(pWaveFile, &waveChunk, sizeof(WAVE_CHUNK));
  ASSERT(((waveChunk.ckid) || (waveChunk.cksize)), 
	  _T"Wrong Format File because of the chunk ID or the chunk size is invalid");

  FS_ReadFile(pWaveFile, &buf32, sizeof(ulong));
  ASSERT(((waveChunk.ckid == mmioFOURCC('R','I','F','F')) && (buf32 == mmioFOURCC('W','A','V','E'))), 
    _T"Wrong Format File because the chunk ID is not the magic word or wave signature is not present");

  do{
    FS_ReadFile(pWaveFile, &buf32, sizeof(ulong));
  }while(buf32 != mmioFOURCC('d','a','t','a'));
	
  FS_ReadFile(pWaveFile, &buf32, sizeof(ulong));
  (*pWSnd)->dwDataSize= buf32;
  (*pWSnd)->dwDataLeft= buf32;
}
/*==============================================================================================================*/
FK_API void
AUD_IPostWavAllocation(WAVESOUND **pWSnd, PTFILE pWaveFile)
{
  FS_ReadFile(pWaveFile, (*pWSnd)->pData, (*pWSnd)->dwDataSize);
}
/*==============================================================================================================*/
FK_API void
AUD_InitWaveSound(WAVESOUND **pSnd, uchar** p_mem, PTFILE pWaveFile)
{
  AUD_IPreWavAllocation(pSnd, pWaveFile);
  (*pSnd)->pData= (*p_mem);
  AUD_IPostWavAllocation(pSnd, pWaveFile);
  (*p_mem)+= (*pSnd)->dwDataSize;
  (*pSnd)++;
}
/*==============================================================================================================*/
FK_API void
AUD_WavePlay(WAVESOUND *pSnd, i32 nMode)
{
  ui32 nPlayFlags		= SND_ASYNC | SND_MEMORY | SND_NODEFAULT; 
	
  pSnd->nPlayingMode= nMode;

  gHW.lpData				= (char*)pSnd->pData;
  gHW.dwBufferLength= pSnd->dwDataSize;
  gHW.dwUser				= 0;
	
  if(nMode == SND_PLAYLOOP){
      gHW.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
      gHW.dwLoops = 0xFFFFFFFF;
  }else{
      gHW.dwFlags = 0;
  }

  waveOutPrepareHeader(gHWO, &gHW, (ui32)sizeof(WAVEHDR));
			
  VERIFY((waveOutReset(gHWO) == MMSYSERR_NOERROR), 
    _T"Can't Reset Device");

  VERIFY((waveOutWrite(gHWO, &gHW, (ui32)sizeof(WAVEHDR)) == MMSYSERR_NOERROR), 
	  _T"Cant Write to Device");

	waveOutUnprepareHeader(gHWO, &gHW, (ui32)sizeof(WAVEHDR));

  gbCanWrite= 0;
}
/*==============================================================================================================*/
FK_API void
AUD_WavePause(WAVESOUND *pWaveSnd, ulong* lpdwPos)
{
  MMTIME mmtime;
  ASSERT(pWaveSnd, _T"invalid wave")

  mmtime.wType = TIME_BYTES;
	
  VERIFY((waveOutGetPosition(gHWO, &mmtime, sizeof(mmtime)) == MMSYSERR_NOERROR), 
    _T"Cant Get Wave Position");

  *lpdwPos= (ulong)mmtime.u.cb;
  pWaveSnd->lPosition= *lpdwPos;

  VERIFY((waveOutPause(gHWO) == MMSYSERR_NOERROR), 
    _T"Cant Pause Wave");
}
/*==============================================================================================================*/
FK_API void
AUD_WaveResume(WAVESOUND *pWaveSnd, ulong* lpdwPos, short nMode)
{
  ASSERT(pWaveSnd, _T"invalid wave")

	VERIFY((waveOutReset(gHWO) == MMSYSERR_NOERROR), 
    _T"Can't Reset Device");

  AUD_WavePlay(pWaveSnd, nMode);
}
/*==============================================================================================================*/
FK_API void
AUD_WaveStop(WAVESOUND *pWaveSnd)
{
  VERIFY((waveOutReset(gHWO) == MMSYSERR_NOERROR), 
    _T"Can't Reset Device");

	if(pWaveSnd)	pWaveSnd->nPlayingMode= NULL;
}
/*==============================================================================================================*/
FK_API void
AUD_WaveGetStatus(WAVESOUND *pWaveSnd, long *plStatus)
{
}
/*==============================================================================================================*/
FK_API void
AUD_WaveGetVolume(WAVESOUND *pWaveSnd, long *lVolume)
{
  ASSERT(pWaveSnd, _T"invalid wave")

	VERIFY((waveOutGetVolume(gHWO, (ulong*)lVolume) == MMSYSERR_NOERROR), 
    _T"Cant Get Volume");

  *lVolume = (long)((*lVolume)&0xFF);
}
/*==============================================================================================================*/
FK_API void
AUD_WaveSetVolume(WAVESOUND *pWaveSnd, long lVolume)
{
  i32 	x;
  uchar 	octet= lVolume & 0xFF;
  ulong dwVol=(ulong)MAKELPARAM(((octet)|(octet<<8)),((octet)|(octet<<8)));	

  VERIFY(((x= waveOutSetVolume(gHWO, dwVol)) == MMSYSERR_NOERROR), 
    _T"Can't Set Volume");
  if(x){
    VERIFY((waveOutReset(gHWO) == MMSYSERR_NOERROR), 
      _T"Can't Reset Device");
  }
}
/*==============================================================================================================*/
FK_API void
AUD_UnInitWaveSound(WAVESOUND *pWaveSnd)
{
	ASSERT(pWaveSnd, _T"invalid wave")
	ASSERT(pWaveSnd->pData, _T"invalid wave Data")

	AUD_WaveStop(pWaveSnd);
	pWaveSnd->pData= 0;
}
/*==============================================================================================================*/
FK_API void
AUD_SetMasterVolume(long lVolume)
{
	AUD_WaveSetVolume(0, lVolume);
}
/*==============================================================================================================*/
FK_API void
AUD_GetMasterVolume(long* lVolume)
{
}
/*==============================================================================================================*/
/*==============================================================================================================*/
