#ifndef AUDIO_H
#define AUDIO_H
#include "StdAfx.h"
#include "Globals.h"

// -------------------------------------------------------
// VoiceContext
// Handles cleanup of audio buffer when playback ends.
// -------------------------------------------------------
class VoiceContext : public IXAudio2VoiceCallback {
public:
    BYTE* pBuffer;
    IXAudio2SourceVoice* pVoice;  // add this
    
    VoiceContext(BYTE* b) : pBuffer(b), pVoice(NULL) {}

    STDMETHOD_(void, OnBufferEnd)(void* pBufferContext) {
		delete[] pBuffer;
		pBuffer = NULL;
		if (pVoice) {
			g_deadVoices.push_back(pVoice);
			pVoice = NULL;
		}
	}

    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) {}
    STDMETHOD_(void, OnVoiceProcessingPassEnd)()        {}
    STDMETHOD_(void, OnStreamEnd)()                     {}
    STDMETHOD_(void, OnBufferStart)(void*)              {}
    STDMETHOD_(void, OnLoopEnd)(void*)                  {}
    STDMETHOD_(void, OnVoiceError)(void*, HRESULT)      {}
};

// -------------------------------------------------------
// AUDIO FUNCTION DECLARATIONS
// -------------------------------------------------------
VOID PlayPCM(IXAudio2* pXaudio2, const char* szFilename, float fVolume);

#endif // AUDIO_H