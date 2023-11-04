/*	Filename: sound.cpp
Author: Michael Galle
Date: Updated 2022
Details: Implementation - Contains functions for Windows sound API (sound recording & playback)
*/

#pragma comment(lib, "Ws2_32.lib")	   // Make sure we are linking against the Ws2_32.lib library
#pragma comment(lib, "Winmm.lib")      // Make sure we are linking against the Winmm.lib library - some functions/symbols from this library (Windows sound API) are used
#include <stdio.h>
#include <windows.h>					// Contains WAVEFORMATEX structure
#include <mmsystem.h>
#include <math.h>
#include "sound.h"

// BUFFERS
short iBigBuf[SAMPLES_SEC * RECORD_TIME];
long  lBigBufSize = SAMPLES_SEC * RECORD_TIME;	// total number of samples

// output and input channel parameters
static	int			g_nSamplesPerSec = SAMPLES_SEC;		//Define sample seconds
static	int			g_nBitsPerSample = 16;				//Define bits per second
static	HWAVEOUT	HWaveOut;				// Handle of opened WAVE Out and In device
static  HWAVEIN		HWaveIn;				// Handle of opened WAVE in and Out device
static	WAVEFORMATEX WaveFormat;			// WAVEFORMATEX structure for reading in the WAVE fmt chunk
static  WAVEHDR	WaveHeader[NFREQUENCIES];	// WAVEHDR structures - 1 per buffer
static  WAVEHDR	WaveHeaderSilence;			// Define space for empty sound bytes
static  WAVEHDR WaveHeaderIn;				// Define buffer for audio in

/* PLAYBACK FUNCTIONS */
/* ********************************************************************************************* */

int	InitializePlayback(void)
{
    int		rc;
    // set up the format structure, needed for playback (and recording)
    SetupFormat(&WaveFormat);
    // open the playback device
    rc = waveOutOpen(&HWaveOut, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL);
    //If statment error flag
    if (rc) {
        printf("Unable to open Output sound Device! Error %x.", rc);
        return(0);
    }
    return(1);	//return 1 if initialization worked
}

int PlayBuffer(short* piBuf, long lSamples)
{
    //Define local variables
    static	WAVEFORMATEX WaveFormat;	// WAVEFORMATEX structure for reading in the WAVE fmt chunk
    static  WAVEHDR	WaveHeader;			// WAVEHDR structure for this buffer
    MMRESULT mmErr;						// Multi meadia result
    int	rc;								//

    // stop previous note (just in case)
    waveOutReset(HWaveOut);   // is this good?

    // get the header ready for playback
    WaveHeader.lpData = (char*)piBuf;									// set the data to audio buffer
    WaveHeader.dwBufferLength = lSamples * sizeof(short);				// set buffer length in bytes
    rc = waveOutPrepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));	// Prepare the header
    //if statment error flag
    if (rc) {
        printf("Failed preparing WAVEHDR, error 0x%x.", rc);
        return(0);
    }
    WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP | WHDR_ENDLOOP);				// Clear loop flags

    // play the buffer. This is NON-blocking.
    mmErr = waveOutWrite(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
    // wait for completion
    rc = WaitOnHeader(&WaveHeader, 0);
    // give back resources
    waveOutUnprepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
    return(rc);
}

void ClosePlayback(void)
{
    int	i;

    // Loop through all audio buuffers defined by "NFREQUNCIES"
    for (i = 0; i < NFREQUENCIES; ++i) {
        waveOutUnprepareHeader(HWaveOut, &WaveHeader[i], sizeof(WAVEHDR));	// Free memory associated with playback buffer
    }
    // close the playback device
    waveOutClose(HWaveOut);
    return;
}

/* RECORDING FUNCTIONS */
/* ********************************************************************************************* */
int InitializeRecording(void)
{
    MMRESULT rc;

    // set up the format structure, needed for recording.
    SetupFormat(&WaveFormat);

    // open the recording device
    rc = waveInOpen(&HWaveIn, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0, CALLBACK_NULL);
    if (rc) {				// If statment error flag if input device cannot be opened
        printf("Unable to open Input sound Device! Error %x.", rc);
        return(0);
    }

    // prepare the buffer header for use later on
    WaveHeaderIn.lpData = (char*)iBigBuf;
    WaveHeaderIn.dwBufferLength = lBigBufSize * sizeof(short);
    rc = waveInPrepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
    if (rc) {
        printf("Failed preparing input WAVEHDR, error 0x%x.", rc);
        return(0);
    }

    return(1);

}

int	RecordBuffer(short* piBuf, long lBufSize)
{
    static	WAVEFORMATEX WaveFormat;	/* WAVEFORMATEX structure for reading in the WAVE fmt chunk */
    static  WAVEHDR	WaveHeader;			/* WAVEHDR structure for this buffer */
    MMRESULT	mmErr;
    int		rc;

    printf("Recording now.....");

    // stop previous recording (just in case)
    waveInReset(HWaveIn);   // is this good?

    // get the header ready for recording.  This should not be needed here AND in init.
    WaveHeader.lpData = (char*)piBuf;									//Define audio buffer
    WaveHeader.dwBufferLength = lBufSize * sizeof(short);				//Define byte size length for recoding
    rc = waveInPrepareHeader(HWaveIn, &WaveHeader, sizeof(WAVEHDR));	//Prepare the header
    if (rc) {		//If statment error flag for preparing audio recording
        printf("Failed preparing WAVEHDR, error 0x%x.", rc);
        return(0);
    }
    WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP | WHDR_ENDLOOP);				// Clear loop flags

    // Give the buffer to the recording device to fill.
    mmErr = waveInAddBuffer(HWaveIn, &WaveHeader, sizeof(WAVEHDR));
    // Record the buffer. This is NON-blocking.
    mmErr = waveInStart(HWaveIn);
    // wait for completion
    rc = WaitOnHeader(&WaveHeader, '.');
    // stop input
    waveInStop(HWaveIn);
    return(rc);
}

void CloseRecording(void)
{
    waveInUnprepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
    // close the playback device
    waveInClose(HWaveIn);
    return;
}


/* SUPPORT FUNCTIONS USED BY PLAYBACK FUNCTIONS - Updated 2021 */
/* ********************************************************************************************* */
// Function needed by InitializePlayback() - SetupFormat() initializes a WAVEFORMATEX structure to the required parameters (sample rate, bits per sample, etc)
static void SetupFormat(WAVEFORMATEX* wf)
{
    wf->wFormatTag = WAVE_FORMAT_PCM;							// uncompressed audio format
    wf->nChannels = 1;											// Defines one audio channel
    wf->nSamplesPerSec = g_nSamplesPerSec;						// Sample rate for audio
    wf->wBitsPerSample = g_nBitsPerSample;						// Amount of bits per sample size
    wf->nBlockAlign = wf->nChannels * (wf->wBitsPerSample / 8);	// Number of bits in a sample across al channels
    wf->nAvgBytesPerSec = wf->nSamplesPerSec * wf->nBlockAlign;	// Calculates average bytes per second
    wf->cbSize = 0;												// Sets extra format size to 0
    return;
}

// Function used by Playbuffer() to wait for whatever is playing to finish or quit after waiting 10 seconds
static int WaitOnHeader(WAVEHDR* wh, char cDit)
{
    long	lTime = 0;
    // wait for whatever is being played, to finish. Quit after 10 seconds.

    for (; ; ) {
        if (wh->dwFlags & WHDR_DONE) return(1);

        // idle for a bit so as to free CPU
        Sleep(100L);
        lTime += 100;					// increment final time for 100 miliseconds
        if (lTime >= 10000) return(0);  // timeout period
        if (cDit) printf("%c", cDit);	// if cDit is not zero print it (display progress)
    }
}
