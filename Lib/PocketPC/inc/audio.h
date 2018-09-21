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
FK_API void	AUD_Init(i32 bps, i32 channels, i32 khz);
FK_API long	AUD_Trap(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
FK_API void	AUD_Quit(void);
FK_API void	AUD_InitWaveSound(WAVESOUND **pSnd, uchar** p_mem, PTFILE pWaveFile);
FK_API void	AUD_WavePlay(WAVESOUND *pSnd, i32 nMode);
FK_API void	AUD_WavePause(WAVESOUND *pWaveSnd, ulong* lpdwPos);
FK_API void	AUD_WaveResume(WAVESOUND *pWaveSnd, ulong* lpdwPos, short nMode);
FK_API void	AUD_WaveStop(WAVESOUND *pWaveSnd);
FK_API void	AUD_WaveGetStatus(WAVESOUND *pWaveSnd, long *plStatus);
FK_API void	AUD_WaveGetVolume(WAVESOUND *pWaveSnd, long *lVolume);
FK_API void	AUD_WaveSetVolume(WAVESOUND *pWaveSnd, long lVolume);
FK_API void	AUD_UnInitWaveSound(WAVESOUND *pWaveSnd);
FK_API void	AUD_SetMasterVolume(long lVolume);
FK_API void	AUD_GetMasterVolume(long* lVolume);
/*==============================================================================================================*/
#endif /*_AUDIO_H_*/
