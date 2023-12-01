#pragma once

#include <windows.h>  // Contains WAVEFORMATEX structure

// CONSTANTS - SET RECORDING TIME AND RATE
#define DEFAULT_NSAMPLES 4000
#define MIN_BUFSIZE 1000
#define RECORD_TIME 6  // seconds to record from the mic
#define SAMPLES_SEC 8000  // number of samples per second
#define NFREQUENCIES 96  // number of frequencies used - leave this alone


// Playback
int InitializePlayback(int samplesPerSec);
int PlayBuffer(short* piBuf, long lSamples);
void ClosePlayback(void);

// Recording
int InitializeRecording(int samplesPerSec, short iBigBuf, long lBigBufSize);
int RecordBuffer(short* piBuf, long lBufSize);
void CloseRecording(void);

// Support functions for Playback functions (updated 2021)
void SetupFormat(WAVEFORMATEX* wf, int samplesPerSec);  // Used by InitializePlayback()
int WaitOnHeader(WAVEHDR* wh, char cDit);