#include "Lib.h"
//#ifndef _B_MIXER_H_
//#define _B_MIXER_H_
//
//#define NB_PLAY_BUF 4
//#include "interfaces.h"
//
//class CBMixer : public IBMixerManager  
//{
//public:
//	class CBPlayBuf
//	{
//	public:
//		uint	loop;
//		uchar *start;
//		uchar *cur;
//		uchar *end;
//	};
//	explicit			CBMixer();
//	virtual				~CBMixer();
//					void	Init(ulong szPlayChk, ulong szMixBuf, char* pMemPool);
//					void	Play(BT_WAVESOUND* wav, int p_mode);
//					void	PlayEx(int p_nWichChannel, BT_WAVESOUND* wav, int p_mode);
//					void	PlayExFm(int p_nWichChannel, BT_WAVESOUND* wav, int p_mode);
//					void	StopChannel(uint p_channel);
//					void	StopAllChannels();
//					void	Start();
//					void	Stop();
//					void	AddRef(){nRefCount++;};
//					void	Release()
//					{
//						if(!(--nRefCount))
//							delete this;
//					};
//
//	uint					nRefCount;
//	uint					act;
//	void					*cntMixBuf;
//	char					*mempool;
//	uint					started;
//	ulong					nSizePlayChk;
//	ulong					nowPlaying;
//	ulong					nowWriting;
//	ulong					idThread;
//	HANDLE				hThread;
//};
//#endif  // _B_MIXER_H_
