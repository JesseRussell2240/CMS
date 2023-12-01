#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include "sound.h"

#define SAMPLES_SEC 44100  // Define your sample rate

static int g_nBitsPerSample = 16;
static HWAVEOUT HWaveOut;
static HWAVEIN HWaveIn;
static WAVEFORMATEX WaveFormat;
static WAVEHDR WaveHeader[NFREQUENCIES];

/* PLAYBACK FUNCTIONS */
/* ********************************************************************************************* */

int InitializePlayback(int samplesPerSec)
{
    int rc;

    // set up the format structure, needed for playback (and recording)
    SetupFormat(&WaveFormat, samplesPerSec);

    // open the playback device
    rc = waveOutOpen(&HWaveOut, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL);

    if (rc)
    {
        printf("Unable to open Output sound Device! Error %x.", rc);
        return 0;
    }

    return 1;
}

int PlayBuffer(short* piBuf, long lSamples)
{
    WAVEHDR WaveHeader;
    MMRESULT mmErr;
    int rc;

    // stop previous note (just in case)
    waveOutReset(HWaveOut);

    // get the header ready for playback
    WaveHeader.lpData = (char*)piBuf;
    WaveHeader.dwBufferLength = lSamples * sizeof(short);
    rc = waveOutPrepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));

    if (rc)
    {
        printf("Failed preparing WAVEHDR, error 0x%x.", rc);
        return 0;
    }

    WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP | WHDR_ENDLOOP);

    // play the buffer. This is NON-blocking.
    mmErr = waveOutWrite(HWaveOut, &WaveHeader, sizeof(WAVEHDR));

    // wait for completion
    rc = WaitOnHeader(&WaveHeader, 0);

    // give back resources
    waveOutUnprepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
    return rc;
}

void ClosePlayback(void)
{
    int i;

    // Loop through all audio buffers defined by "NFREQUENCIES"
    for (i = 0; i < NFREQUENCIES; ++i)
    {
        waveOutUnprepareHeader(HWaveOut, &WaveHeader[i], sizeof(WAVEHDR));
    }

    // close the playback device
    waveOutClose(HWaveOut);
}

/* SUPPORT FUNCTIONS USED BY PLAYBACK FUNCTIONS - Updated 2021 */
/* ********************************************************************************************* */
static void SetupFormat(WAVEFORMATEX* wf, int samplesPerSec)
{
    wf->wFormatTag = WAVE_FORMAT_PCM;
    wf->nChannels = 1;
    wf->nSamplesPerSec = samplesPerSec;
    wf->wBitsPerSample = g_nBitsPerSample;
    wf->nBlockAlign = wf->nChannels * (wf->wBitsPerSample / 8);
    wf->nAvgBytesPerSec = wf->nSamplesPerSec * wf->nBlockAlign;
    wf->cbSize = 0;
}

static int WaitOnHeader(WAVEHDR* wh, char cDit)
{
    long lTime = 0;

    // wait for whatever is being played, to finish. Quit after 10 seconds.
    for (;;)
    {
        if (wh->dwFlags & WHDR_DONE) return 1;

        // idle for a bit so as to free CPU
        Sleep(100L);
        lTime += 100;

        if (lTime >= 10000) return 0;  // timeout period
        if (cDit) printf("%c", cDit);
    }
}

int InitializeRecording(int samplesPerSec, short iBigBuf, long lBigBufSize)
{
    MMRESULT rc;

    // set up the format structure, needed for recording.
    SetupFormat(&WaveFormat, samplesPerSec);

    // open the recording device
    rc = waveInOpen(&HWaveIn, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL);
    if (rc)
    {  // If statement error flag if the input device cannot be opened
        printf("Unable to open Input sound Device! Error %x.", rc);
        return 0;
    }

    // prepare the buffer header for use later on
    WAVEHDR WaveHeaderIn;
    WaveHeaderIn.lpData = (char*) iBigBuf;
    WaveHeaderIn.dwBufferLength = lBigBufSize * sizeof(short);
    rc = waveInPrepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
    if (rc)
    {
        printf("Failed preparing input WAVEHDR, error 0x%x.", rc);
        return 0;
    }

    return 1;
}



int RecordBuffer(short* piBuf, long lBufSize)
{
    MMRESULT mmErr;
    int rc;

    printf("Recording now.....");

    // stop previous recording (just in case)
    waveInReset(HWaveIn);

    static WAVEHDR WaveHeaderIn;
    // get the header ready for recording. This should not be needed here AND in init.
    WaveHeaderIn.lpData = (char*)piBuf;                             // Define audio buffer
    WaveHeaderIn.dwBufferLength = lBufSize * sizeof(short);           // Define byte size length for recording
    rc = waveInPrepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));  // Prepare the header
    if (rc)
    {  // If statement error flag for preparing audio recording
        printf("Failed preparing WAVEHDR, error 0x%x.", rc);
        return 0;
    }
    WaveHeaderIn.dwFlags &= ~(WHDR_BEGINLOOP | WHDR_ENDLOOP);  // Clear loop flags

    // Give the buffer to the recording device to fill.
    mmErr = waveInAddBuffer(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
    // Record the buffer. This is NON-blocking.
    mmErr = waveInStart(HWaveIn);
    // wait for completion
    rc = WaitOnHeader(&WaveHeaderIn, '.');
    // stop input
    waveInStop(HWaveIn);
    return rc;
}

void CloseRecording(void)
{
    static WAVEHDR WaveHeaderIn;
    waveInUnprepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
    // close the playback device
    waveInClose(HWaveIn);
    return;
}