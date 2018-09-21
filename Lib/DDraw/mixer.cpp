//#include "Bmixer.h"
//
//CBMixer	*gObj=0;
//uchar		*startAll;
//uchar		*endAll;
//uchar		*startChk;
//uchar		*endChk;
//uchar		*cur;
//uint		curCptChk;
//uint		maxCptChk;
//uint		gBufSize;
//uint		tres;
//ulong		act=0;
//uint		arLoop[NB_PLAY_BUF];
//uchar		*arStart[NB_PLAY_BUF];
//uchar		*arCur[NB_PLAY_BUF];
//uchar		*arEnd[NB_PLAY_BUF];
//extern	uint			gbCanWrite;
//extern	HWAVEOUT	gHWO;
//
//#pragma FIXME(Mixer only work in 8 bits)
//
//uint 
//TrouverNb(culong& buf)
//{
//	uint nb=0; 
//	if(buf&0x0001)	++nb;	if(buf&0x0002)	++nb;
//	if(buf&0x0004)	++nb;	if(buf&0x0008)	++nb;
//	return nb;
//}
//
//inline uint 
//TrouverNext(culong& buf, uint bit)
//{
//	switch(bit){
//	case 0:
//		if(buf&0x0001)	return 0;
//	case 1:
//		if(buf&0x0002)	return 1;
//	case 2:
//		if(buf&0x0004)	return 2;
//	case 3:
//		if(buf&0x0008)	return 3;
//	}
///*	uint i= bit;
//	while(i < 4){
//		if(buf&(1<<i))
//			return i;
//		++i;
//	}
//	return 0;
//*/
//}
//
//void 
//MergeSound(int nb)
//{
//	uchar		*pBufD= (cur+(gBufSize-nb));
//	uint		nNbSound= TrouverNb(act);
//	int			i=nb;
//	float		amplitude = (float)((float)1 / (float)2);
//
//	if(!i)	return;
//
//	switch(nNbSound)
//	{
//		case 0:	
//			while(i--){	
//				(*pBufD++)= 0x7F;
//			}
//			break;
//		case 1:{
//			uint		first				= TrouverNext(act, 0);
//			uchar*&	pBufFrist		= arCur[first];
//			uchar*	pBufEndFrist= arEnd[first];
//			while(i--){	
//				//Mix
//				(*pBufD++)= amplitude * (*pBufFrist++);
//
//				//Restart or Reloop
//				if(pBufFrist >= pBufEndFrist){	if(arLoop[first])	pBufFrist= arStart[first];
//																				else{	act&=~(1<<first);
//																							pBufFrist= arStart[first];
//																							MergeSound(i);return;}}
//			}
//		}
//		break;
//		case 2:{
//			uint		first					= TrouverNext(act, 0);
//			uint		second				= TrouverNext(act, first+1);
//			uchar*&	pBufFrist			= arCur[first];
//			uchar*	pBufEndFrist	= arEnd[first];
//			uchar*&	pBufSecond		= arCur[second];
//			uchar*	pBufEndSecond = arEnd[second];
//			while(i--){
//				//Mix
//				(*pBufD++)= amplitude * ((*pBufFrist++)+(*pBufSecond++));
//
//				//Restart or Reloop
//				if(pBufFrist >= pBufEndFrist){	if(arLoop[first])	pBufFrist= arStart[first];
//																				else{	act&=~(1<<first);
//																							pBufFrist= arStart[first];
//																							MergeSound(i);return;}}
//				if(pBufSecond>= pBufEndSecond){	if(arLoop[second])pBufSecond= arStart[second];
//																				else{	act&=~(1<<second);
//																							pBufSecond= arStart[second];
//																							MergeSound(i);return;}}
//			}
//		}
//		break;
//		case 3:{
//			uint		first					= TrouverNext(act, 0);
//			uint		second				= TrouverNext(act, first+1);
//			uint		third					= TrouverNext(act, second+1);
//			uchar*&	pBufFrist			= arCur[first];
//			uchar*	pBufEndFrist	= arEnd[first];
//			uchar*&	pBufSecond		= arCur[second];
//			uchar*	pBufEndSecond = arEnd[second];
//			uchar*&	pBufThird			= arCur[third];
//			uchar*	pBufEndThird	= arEnd[third];
//			while(i--){
//				//Mix
//				(*pBufD++)= amplitude * ((*pBufFrist++)+(*pBufSecond++)+(*pBufThird++));
//
//				//Restart or Reloop
//				if(pBufFrist >= pBufEndFrist){	if(arLoop[first])	pBufFrist= arStart[first];
//																				else{	act&=~(1<<first);
//																							pBufFrist= arStart[first];
//																							MergeSound(i);return;}}
//				if(pBufSecond>= pBufEndSecond){	if(arLoop[second])pBufSecond= arStart[second];
//																				else{	act&=~(1<<second);
//																							pBufSecond= arStart[second];
//																							MergeSound(i);return;}}
//				if(pBufThird	>= pBufEndThird){	if(arLoop[third])	pBufThird= arStart[third];
//																				else{	act&=~(1<<third);
//																							pBufThird= arStart[third];
//																							MergeSound(i);return;}}
//			}
//		}
//		break;
//		case 4:{
//			uint		first					= TrouverNext(act, 0);
//			uint		second				= TrouverNext(act, first+1);
//			uint		third					= TrouverNext(act, second+1);
//			uint		fourth				= TrouverNext(act, third+1);
//			uchar*&	pBufFrist			= arCur[first];
//			uchar*	pBufEndFrist	= arEnd[first];
//			uchar*&	pBufSecond		= arCur[second];
//			uchar*	pBufEndSecond = arEnd[second];
//			uchar*&	pBufThird			= arCur[third];
//			uchar*	pBufEndThird	= arEnd[third];
//			uchar*&	pBufFourth		= arCur[fourth];
//			uchar*	pBufEndFouth	= arEnd[fourth];
//			while(i--){
//				//Mix
//				(*pBufD++)= amplitude * ((*pBufFrist++)+(*pBufSecond++)+(*pBufThird++)+(*pBufFourth++));
//
//				//Restart or Reloop
//				if(pBufFrist >= pBufEndFrist){	if(arLoop[first])	pBufFrist= arStart[first];
//																				else{	act&=~(1<<first);
//																							pBufFrist= arStart[first];
//																							MergeSound(i);return;}}
//				if(pBufSecond>= pBufEndSecond){	if(arLoop[second])pBufSecond= arStart[second];
//																				else{	act&=~(1<<second);
//																							pBufSecond= arStart[second];
//																							MergeSound(i);return;}}
//				if(pBufThird	>= pBufEndThird){	if(arLoop[third])	pBufThird= arStart[third];
//																				else{	act&=~(1<<third);
//																							pBufThird= arStart[third];
//																							MergeSound(i);return;}}
//				if(pBufFourth	>= pBufEndFouth){	if(arLoop[fourth])pBufFourth= arStart[fourth];
//																				else{	act&=~(1<<fourth);
//																							pBufFourth= arStart[fourth];
//																							MergeSound(i);return;}}
//			}
//		}
//		break;
//	}
//}
//
//DWORD WINAPI 
//MyThread(void* lParam)
//{
//	uint			lastlastPlayChk;
//	uint			sizeofBuffer		= ((WAVEHDR*)gObj->cntMixBuf)->dwBufferLength;
//	WAVEHDR*	pHdrCnt					= (WAVEHDR*)gObj->cntMixBuf;
//	WAVEHDR*	pHdrCur					=	&pHdrCnt[curCptChk];
//	WAVEHDR*	pFHdrCur;
//	
//	while(1){
//		lastlastPlayChk= curCptChk;
//		pFHdrCur= pHdrCur;
//
//		MergeSound(gBufSize);
//
//		pHdrCur->lpData									= (char*)cur;
//		/*mmres	= */waveOutUnprepareHeader(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//		/*mmres	= */waveOutPrepareHeader	(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//		/*mmres	= */waveOutWrite					(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		pHdrCur	= &pHdrCnt[curCptChk];
//		cur			= &startAll[curCptChk*sizeofBuffer];
//
//		//TWO
//		MergeSound(gBufSize);
//
//		pHdrCur->lpData									= (char*)cur;
//		/*mmres	= */waveOutUnprepareHeader(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//		/*mmres	= */waveOutPrepareHeader	(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//		/*mmres	= */waveOutWrite					(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		pHdrCur	= &pHdrCnt[curCptChk];
//		cur			= &startAll[curCptChk*sizeofBuffer];
//
//		//THREE
//		MergeSound(gBufSize);
//
//		pHdrCur->lpData									= (char*)cur;
//		/*mmres	= */waveOutUnprepareHeader(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//		/*mmres	= */waveOutPrepareHeader	(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//		/*mmres	= */waveOutWrite					(gHWO, pHdrCur, (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		pHdrCur	= &pHdrCnt[curCptChk];
//		cur			= &startAll[curCptChk*sizeofBuffer];
//
//		while(!(pFHdrCur->dwFlags&WHDR_DONE)){
//			Sleep(0);
//		}
//	}
//	return(0);
//}
//
//CBMixer::CBMixer():
//started(0),
//nRefCount(0),
//nowPlaying(0),
//nowWriting(0)
//{
//	gObj= this;
//	act=0;
//}
//
//CBMixer::~CBMixer()
//{
//	SAFE_DELETE_ARRAY(startAll);
//	SAFE_DELETE_ARRAY(cntMixBuf);
//}
//
//void	
//CBMixer::Start()
//{
//	hThread= CreateThread(NULL, 0, MyThread, 0, 0, &idThread);
//	SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
//}
//
//void	
//CBMixer::Stop()
//{
//	TerminateThread(hThread, 0);
//	::act= this->act= 0;
//	waveOutReset(gHWO);
//}
//
////commente le systeme Front Sound Buffer et Back Sound Buffer et Flipping
//void 
////CBMixer::Init(ulong szPlayChk, ulong szMixBuf, ulong* pMemPool)
//CBMixer::Init(ulong szPlayChk, ulong szMixBuf, char* pMemPool)
//{
//	ASSERT((szPlayChk > 0), _T"Invalid Param szPlayChk");
//	ASSERT((szMixBuf > 0), _T"Invalid Param szMixBuf");
//	VERIFY((mempool= pMemPool), _T"Invalid Param pMemPool");
//
//	nSizePlayChk= szPlayChk;
//	startAll		= new uchar[szMixBuf];					memset(startAll, 0x7F, szMixBuf);
//	endAll			= startAll + szMixBuf;
//	startChk		= startAll;
//	endChk			= startChk + nSizePlayChk;
//
//	cur					= startChk;
//	curCptChk		= 0;
//	maxCptChk		= (szMixBuf/szPlayChk);
//	cntMixBuf		= (void*)new WAVEHDR[maxCptChk];memset(cntMixBuf, 0x00, sizeof(WAVEHDR)*maxCptChk);
//
//	for(int i= 0; i< maxCptChk; ++i){
//		((WAVEHDR*)cntMixBuf)[i].lpData					= (char*)(startAll + (i * nSizePlayChk));
//		((WAVEHDR*)cntMixBuf)[i].dwBufferLength	= nSizePlayChk;
//		/*mmres	= */waveOutPrepareHeader(gHWO, &((WAVEHDR*)cntMixBuf)[i], (uint)sizeof(WAVEHDR));
//	}
//	gBufSize= szPlayChk;
//	::act= this->act;
//}
//
//void 
//CBMixer::Play(BT_WAVESOUND* wav, int p_mode)
//{
//	ASSERT(0, _T"Play : Fonction Inactive");
//	int posFree= -1;
//	if(act == 0xF) return;
//	if(act == 0x0) {posFree= 0; goto end;}
//
//	if((act&0x3)&&((act&0x3)!=0x3)){								//00000000000000??
//    if(act&0x1)			{posFree= 1;goto end;}			  //0000000000000001
//    else						{posFree= 0;goto end;}			  //0000000000000000
//	}
//	if(act&0x4)				{posFree= 3;goto end;}				//000000000000?011
//		else						{posFree= 2;goto end;}				//0000000000000011
//	return;
//end:
//	arStart[posFree]= (uchar*) wav->pData;
//	arCur[posFree]	= (uchar*) wav->pData;
//	arEnd[posFree]	= ((uchar*) wav->pData) + wav->dwDataSize;
//	arLoop[posFree]	= (p_mode==SND_PLAYLOOP)?1:0;
//	::act= this->act |= 1<<posFree;
//}
//
//void 
//CBMixer::PlayEx(int p_nWichChannel, BT_WAVESOUND* wav, int p_mode)
//{
//	::act= this->act			 &=~(1<<p_nWichChannel);
//	arStart[p_nWichChannel]	= (uchar*) wav->pData;
//	arCur[p_nWichChannel]		= (uchar*) wav->pData;
//	arEnd[p_nWichChannel]		= ((uchar*) wav->pData) + wav->dwDataSize;
//	arLoop[p_nWichChannel]	= (p_mode==SND_PLAYLOOP)?1:0;
//	::act= this->act			 |= (1<<p_nWichChannel);
//}
//
//void 
//CBMixer::PlayExFm(int p_nWichChannel, BT_WAVESOUND* wav, int p_mode)
//{
//	::act= this->act			 &=~(1<<p_nWichChannel);
//	arStart[p_nWichChannel]	= (uchar*)(mempool + (ulong)wav->pData);
//	arCur[p_nWichChannel]		= (uchar*)(mempool + (ulong)wav->pData);
//	arEnd[p_nWichChannel]		= ((uchar*)(mempool + (ulong)wav->pData) + wav->dwDataSize);
//	arLoop[p_nWichChannel]	= (p_mode==SND_PLAYLOOP)?1:0;
//	::act= this->act			 |= (1<<p_nWichChannel);
//}
//
//void
//CBMixer::StopAllChannels()
//{
//	::act= this->act= 0;
////		waveOutReset(gHWO);
//}
//
//void	
//CBMixer::StopChannel(uint p_channel)
//{
//	::act= this->act	&=~(1<<p_channel);
//}
//
///* MARCHE NO1 except slow
//DWORD WINAPI 
//MyThread(void* lParam)
//{
//	MMRESULT mmres;
//	uint lastlastPlayChk, lastPlayChk;
//	
//	lastlastPlayChk= lastPlayChk= curCptChk;
//	while(1){
//		lastlastPlayChk= curCptChk;
//		MergeSound(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength);
//
//		if(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwFlags&WHDR_PREPARED)
//			mmres	= waveOutUnprepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres		= waveOutPrepareHeader	(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres		= waveOutWrite					(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		
//		startChk= &startAll[curCptChk*((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength];
//		endChk	= startChk + ((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength;
//		cur			= (uchar*)startChk;
//
//		//TWO
//		lastPlayChk= curCptChk;
//		MergeSound(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength);
//
//		if(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwFlags&WHDR_PREPARED)
//			mmres	= waveOutUnprepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres		= waveOutPrepareHeader	(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres		= waveOutWrite					(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		
//		startChk= &startAll[curCptChk*((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength];
//		endChk	= startChk + ((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength;
//		cur			= (uchar*)startChk;
//
//		//THREE
//		MergeSound(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength);
//
//		if(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwFlags&WHDR_PREPARED)
//			mmres	= waveOutUnprepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres		= waveOutPrepareHeader	(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres		= waveOutWrite					(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		
//		startChk= &startAll[curCptChk*((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength];
//		endChk	= startChk + ((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength;
//		cur			= (uchar*)startChk;
//
//		if(!(((WAVEHDR*)gObj->cntMixBuf)[lastlastPlayChk].dwFlags&WHDR_DONE))
//			OutputDebugString(_T"Trop a lavance ");
//		while(!(((WAVEHDR*)gObj->cntMixBuf)[lastlastPlayChk].dwFlags&WHDR_DONE)){
//			OutputDebugString(_T".");
//		};
//		OutputDebugString(_T"Continue\n");
//	}
//	return(0);
//}
//*/
//
///*
//MyThread(void* lParam)
//{
//	int BonjourToto= 0;
//	while(1){
//		j= NB_PLAY_BUF;tres= BonjourToto= 0;
//		if(act&0x0001)	++BonjourToto;	if(act&0x0002)	++BonjourToto;
//		if(act&0x0004)	++BonjourToto;	if(act&0x0008)	++BonjourToto;
//		if(act&0x0010)	++BonjourToto;	if(act&0x0020)	++BonjourToto;
//		if(act&0x0040)	++BonjourToto;	if(act&0x0080)	++BonjourToto;
//		if(act&0x0100)	++BonjourToto;	if(act&0x0200)	++BonjourToto;
//		if(act&0x0400)	++BonjourToto;	if(act&0x0800)	++BonjourToto;
//		if(act&0x1000)	++BonjourToto;	if(act&0x2000)	++BonjourToto;
//		if(act&0x4000)	++BonjourToto;	if(act&0x8000)	++BonjourToto;
//
//		while(j--){
//			if(act&(1<<j)){
//				if((tres+= (*arCur[j]/BonjourToto))>0xFF){
//					tres= 0xFF;
//				}
//				arCur[j]++;
//			}
//			else continue;	
//			if(arLoop[j])		{if(arCur[j]>=(uchar*)arEnd[j])		arCur[j]=(uchar*)arStart[j];}
//			else						 if(arCur[j]>=(uchar*)arEnd[j])		act&=~(1<<j);
//		}
//		*cur= tres;
//		si(++cur>=(uchar*)endChk)
//			MMRESULT mmres= waveOutPrepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//			mmres= waveOutWrite(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//
//			if(++curCptChk>=maxCptChk)		curCptChk= 0;
//			
//			startChk= &startAll[curCptChk*((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength];
//			endChk= startChk + ((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength;
//			cur= (uchar*)startChk;
//
//			while(!(((WAVEHDR*)gObj->cntMixBuf)->dwFlags&WHDR_DONE));
//
//			mmres= waveOutUnprepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//			((WAVEHDR*)gObj->cntMixBuf)[curCptChk].lpData= (char*)startChk;
//		}
//	}
//	return(0);
//}
//*/
//
///*
//
///*
//DWORD WINAPI 
//MyThread(void* lParam)
//{
//	MMRESULT mmres;
//	uint lastlastPlayChk= lastPlayChk= curCptChk;
//	while(1){
//		MergeSound(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength);
//
//		mmres= waveOutPrepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		mmres= waveOutWrite(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//
//		if(++curCptChk>=maxCptChk)		curCptChk= 0;
//		
//		startChk= &startAll[curCptChk*((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength];
//		endChk= startChk + ((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwBufferLength;
//		cur= (uchar*)startChk;
//
//		if(!(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwFlags&WHDR_DONE))
//			OutputDebugString(_T"Trop a lavance\n");
//		while(!(((WAVEHDR*)gObj->cntMixBuf)[curCptChk].dwFlags&WHDR_DONE));
//
//		mmres= waveOutUnprepareHeader(gHWO, &((WAVEHDR*)gObj->cntMixBuf)[curCptChk], (uint)sizeof(WAVEHDR));
//		((WAVEHDR*)gObj->cntMixBuf)[curCptChk].lpData= (char*)startChk;
//	}
//	return(0);
//}
//
//*/
