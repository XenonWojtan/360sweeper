#include "StdAfx.h"
#include "Globals.h"
#include "Audio.h"

// -------------------------------------------------------
// PlayPCM
// Loads and plays a PCM wave file at the given volume.
// -------------------------------------------------------
VOID PlayPCM(IXAudio2* pXaudio2, const char* szFilename, float fVolume)
{
    if (g_IsMuted) return;
    XAUDIO2_PERFORMANCE_DATA perfData;
    pXaudio2->GetPerformanceData(&perfData);
    if (perfData.ActiveSourceVoiceCount > 24) return;
    HRESULT hr = S_OK;

    ATG::WaveFile WaveFile;
    if (FAILED(hr = WaveFile.Open(szFilename)))
        return;

    WAVEFORMATEXTENSIBLE wfx = {0};
    WaveFile.GetFormat(&wfx);

    DWORD cbWaveSize = 0;
    WaveFile.GetDuration(&cbWaveSize);

    BYTE* pbWaveData = new BYTE[cbWaveSize];
    if (FAILED(hr = WaveFile.ReadSample(0, pbWaveData, cbWaveSize, &cbWaveSize))) {
        delete[] pbWaveData;
        return;
    }

    VoiceContext* pContext = new VoiceContext(pbWaveData);

    IXAudio2SourceVoice* pSourceVoice;
    if (FAILED(hr = pXaudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx, 0, 2.0f, pContext))) {
        delete pContext;
        delete[] pbWaveData;
        return;
    }
	pContext->pVoice = pSourceVoice;

    pSourceVoice->SetVolume(fVolume);

    XAUDIO2_BUFFER buffer = {0};
    buffer.pAudioData  = pbWaveData;
    buffer.Flags       = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes  = cbWaveSize;
    buffer.pContext    = pContext;

    pSourceVoice->SubmitSourceBuffer(&buffer);
    pSourceVoice->Start(0);
}