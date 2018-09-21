#ifndef _AUDIO_H_
#define _AUDIO_H_
/*==============================================================================================================*/
/*==============================================================================================================*/
#include "types.h"
#include "file.h"
/*==============================================================================================================*/
/*==============================================================================================================*/
#define	SND_PLAYONCE	0
#define SND_PLAYLOOP	1
/*==============================================================================================================*/
/*==============================================================================================================*/
typedef struct tagWAVESOUND{
  ui32	dwDataSize;
  ui32	dwDataLeft;
  ui32	nPlayingMode;
  ui32	lPosition;
  void*	pData;
}WAVESOUND;
/*==============================================================================================================*/
LIB_API void	AUD_Init(i32 bps, i32 channels, i32 khz);
LIB_API long	AUD_Trap(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LIB_API void	AUD_Quit(void);
LIB_API void	AUD_InitWaveSound(WAVESOUND **pSnd, uchar** p_mem, PTFILE pWaveFile);
LIB_API void	AUD_WavePlay(WAVESOUND *pSnd, i32 nMode);
LIB_API void	AUD_WavePause(WAVESOUND *pWaveSnd, ulong* lpdwPos);
LIB_API void	AUD_WaveResume(WAVESOUND *pWaveSnd, ulong* lpdwPos, short nMode);
LIB_API void	AUD_WaveStop(WAVESOUND *pWaveSnd);
LIB_API void	AUD_WaveGetStatus(WAVESOUND *pWaveSnd, long *plStatus);
LIB_API void	AUD_WaveGetVolume(WAVESOUND *pWaveSnd, long *lVolume);
LIB_API void	AUD_WaveSetVolume(WAVESOUND *pWaveSnd, long lVolume);
LIB_API void	AUD_UnInitWaveSound(WAVESOUND *pWaveSnd);
LIB_API void	AUD_SetMasterVolume(long lVolume);
LIB_API void	AUD_GetMasterVolume(long* lVolume);
/*==============================================================================================================*/
#endif /*_AUDIO_H_*/
