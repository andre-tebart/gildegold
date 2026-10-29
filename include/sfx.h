/* sfx.c -- digital sound output on top of the Miles Sound System (mss32.dll): drivers, samples,
   streams. Original file name unknown; the only leaked name is the mem_Alloc tag "sfx_OpenDigOutput".
   Starts at 0x42b720 right after the event script interpreter (evt.c) and ends at 0x42c398
   (sfx_NumDrivers); 0x42c3a0 is an mss32 import thunk, 0x42c3b0 starts the text-file parser (txt.c). msx.c calls these functions as snd_* (msx.h).
   Names are ours. */
#ifndef UNK_42B720_H
#define UNK_42B720_H

#define SFX_DRIVERS 16

typedef void *HDIGDRIVER;
typedef void *HSAMPLE;
typedef void *HSTREAM;

typedef struct SFXDRIVER {          /* 0x1c bytes, one per open digital output */
    HDIGDRIVER drv;
    unsigned short wFormatTag;      /* 0x04, PCMWAVEFORMAT passed to AIL_waveOutOpen */
    unsigned short nChannels;       /* 0x06 */
    unsigned long nSamplesPerSec;   /* 0x08 */
    unsigned long nAvgBytesPerSec;  /* 0x0c */
    unsigned short nBlockAlign;     /* 0x10 */
    unsigned short wBitsPerSample;  /* 0x12 */
    int used;                       /* 0x14, samples and streams allocated */
    int max;                        /* 0x18, entries in sfx_Samples[i] and sfx_Streams[i] */
} SFXDRIVER;

extern SFXDRIVER *sfx_Drivers[SFX_DRIVERS];     /* 0x6332f0 */
extern HSAMPLE *sfx_Samples[SFX_DRIVERS];       /* 0x633330 */
extern HSTREAM *sfx_Streams[SFX_DRIVERS];       /* 0x633370 */
extern int sfx_Initialized;                     /* 0x6333b0 */
extern int sfx_NumSamples;                      /* 0x6333b4, samples per driver (DIG_MIXER_CHANNELS) */

/* mss32.dll */
__declspec(dllimport) int __stdcall AIL_startup(void);
__declspec(dllimport) void __stdcall AIL_shutdown(void);
__declspec(dllimport) char *__stdcall AIL_last_error(void);
__declspec(dllimport) int __stdcall AIL_get_preference(unsigned int number);
__declspec(dllimport) int __stdcall AIL_set_preference(unsigned int number, int value);
__declspec(dllimport) int __stdcall AIL_waveOutOpen(HDIGDRIVER *drvr, void *lphWaveOut, int id, void *lpFormat);
__declspec(dllimport) void __stdcall AIL_waveOutClose(HDIGDRIVER drvr);
__declspec(dllimport) int __stdcall AIL_digital_handle_release(HDIGDRIVER drvr);
__declspec(dllimport) int __stdcall AIL_digital_handle_reacquire(HDIGDRIVER drvr);
__declspec(dllimport) void __stdcall AIL_set_digital_master_volume(HDIGDRIVER dig, int master_volume);
__declspec(dllimport) HSAMPLE __stdcall AIL_allocate_sample_handle(HDIGDRIVER dig);
__declspec(dllimport) void __stdcall AIL_release_sample_handle(HSAMPLE S);
__declspec(dllimport) void __stdcall AIL_init_sample(HSAMPLE S);
__declspec(dllimport) int __stdcall AIL_set_named_sample_file(HSAMPLE S, char *type, void *image, int size, int block);
__declspec(dllimport) void __stdcall AIL_start_sample(HSAMPLE S);
__declspec(dllimport) void __stdcall AIL_resume_sample(HSAMPLE S);
__declspec(dllimport) void __stdcall AIL_end_sample(HSAMPLE S);
__declspec(dllimport) void __stdcall AIL_set_sample_volume(HSAMPLE S, int volume);
__declspec(dllimport) void __stdcall AIL_set_sample_pan(HSAMPLE S, int pan);
__declspec(dllimport) void __stdcall AIL_set_sample_loop_count(HSAMPLE S, int count);
__declspec(dllimport) unsigned int __stdcall AIL_sample_status(HSAMPLE S);
__declspec(dllimport) HSTREAM __stdcall AIL_open_stream(HDIGDRIVER dig, char *filename, int stream_mem);
__declspec(dllimport) void __stdcall AIL_close_stream(HSTREAM stream);
__declspec(dllimport) void __stdcall AIL_start_stream(HSTREAM stream);
__declspec(dllimport) void __stdcall AIL_pause_stream(HSTREAM stream, int onoff);
__declspec(dllimport) void __stdcall AIL_set_stream_volume(HSTREAM stream, int volume);
__declspec(dllimport) void __stdcall AIL_set_stream_loop_count(HSTREAM stream, int count);
__declspec(dllimport) void __stdcall AIL_set_stream_ms_position(HSTREAM stream, int milliseconds);
__declspec(dllimport) int __stdcall AIL_stream_status(HSTREAM stream);
__declspec(dllimport) void __stdcall AIL_stream_ms_position(HSTREAM stream, int *total_ms, int *current_ms);
__declspec(dllimport) int __stdcall AIL_get_timer_highest_delay(void);
int MSS_auto_cleanup(void);                                                             /* 0x5de460, mss32.lib */
int sfx_TimerHighestDelay(void);                                                        /* 0x42c3a0, AIL_get_timer_highest_delay for msx */

void * mem_Alloc(int size, char *tag);                                                   /* 0x4223c0 */
void mem_Free(void *p);                                                                 /* 0x422620 */

/* inline-asm clear, as D3_ZERO in d3.h */
#define SFX_ZERO_CAT(a, b) a##b
#define SFX_ZERO_LABEL(n) SFX_ZERO_CAT(sfx_zero_tail, n)
#define SFX_ZERO(dst, size) {                                       \
    int _zn = (size); void *_zp = (dst);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle SFX_ZERO_LABEL(__LINE__)                              \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm SFX_ZERO_LABEL(__LINE__): add ecx, ebx                    \
    __asm rep stosb                                                 \
}

int sfx_Startup(void);                                                                  /* 0x42b720, starts the Miles Sound System once (MSS_auto_cleanup, AIL_startup); 0, -1 if already up */
int sfx_Shutdown(void);                                                                 /* 0x42b750, closes all streams, samples and drivers and shuts AIL down; 0, -1 if not up */
int sfx_SetMasterVolume(SFXDRIVER *d, int vol);                                         /* 0x42b780, master volume 0..127 of the registered driver d; 0, -1 */
int sfx_OpenDigOutput(SFXDRIVER **out, int channels, unsigned int bits, int rate);      /* 0x42b7c0, opens a waveOut driver with the format from channels / bits / rate and its slots; *out, 0, -1 */
int sfx_CloseDigOutput(SFXDRIVER *d);                                                   /* 0x42b970, closes the registered driver d with its sample and stream tables; 0, -1 */
int sfx_CloseAll(void);                                                                 /* 0x42b9f0, closes every open driver; 0, -1 */
int sfx_Release(SFXDRIVER *d);                                                          /* 0x42ba30, releases the digital handle (mixer state) of the registered d; 0, -1 */
int sfx_ReleaseAll(void);                                                               /* 0x42ba70, releases the digital handle of every open driver; 0, -1 */
void sfx_Reacquire(SFXDRIVER *d, void *hwnd, unsigned int msg);                         /* 0x42bab0, reacquires d's digital handle and posts msg to hwnd on success */
void sfx_ReacquireAll(void *hwnd, unsigned int msg);                                    /* 0x42bb00, reacquires the digital handle of every open driver, posting msg */
int sfx_SetNumSamples(int n);                                                           /* 0x42bb40, sample slots per driver (AIL preference 1) while at most one driver is open; 0, -1 */
int sfx_AllocSample(SFXDRIVER *d, HSAMPLE *out);                                        /* 0x42bb80, allocates a sample handle in a free slot of the registered d and counts it used; *out, 0, -1 */
int sfx_SetSampleFile(HSAMPLE s, char *type, void *image, int size);                    /* 0x42bbf0, points the registered sample s at a named file image; 0, -1 */
int sfx_ReleaseSample(HSAMPLE s);                                                       /* 0x42bc40, releases the registered sample s and frees its slot and count; 0, -1 */
int sfx_ReleaseAllSamples(void);                                                        /* 0x42bca0, releases every sample of every open driver; 0, -1 */
int sfx_InitSample(HSAMPLE s);                                                          /* 0x42bd00, AIL_init_sample on the registered sample s; 0, -1 */
int sfx_StartSample(HSAMPLE s);                                                         /* 0x42bd30, starts the registered sample s; 0, -1 */
int sfx_ResumeSample(HSAMPLE s);                                                        /* 0x42bd60, resumes the registered sample s; 0, -1 */
int sfx_EndSample(HSAMPLE s);                                                           /* 0x42bd90, ends the registered sample s; 0, -1 */
int sfx_SetSampleVolume(HSAMPLE s, int vol);                                            /* 0x42bdc0, sets the volume of the registered sample s; 0, -1 */
int sfx_SetSamplePan(HSAMPLE s, int pan);                                               /* 0x42be00, sets the pan of the registered sample s; 0, -1 */
int sfx_SetSampleLoopCount(HSAMPLE s, int count);                                       /* 0x42be40, sets the loop count of the registered sample s; 0, -1 */
int sfx_SampleStatus(HSAMPLE s);                                                        /* 0x42be80, AIL sample status of the registered s, -1 if not registered */
int sfx_OpenStream(SFXDRIVER *d, char *file, HSTREAM *out);                             /* 0x42beb0, opens the stream file on the registered d in a free slot and counts it; *out, 0, -1 */
int sfx_CloseStream(HSTREAM s);                                                         /* 0x42bf20, closes the registered stream s and frees its slot and count; 0, -1 */
int sfx_CloseAllStreams(void);                                                          /* 0x42bf80, closes every stream of every open driver; 0, -1 */
int sfx_StartStream(HSTREAM s);                                                         /* 0x42bfe0, starts the registered stream s; 0, -1 */
int sfx_PauseStream(HSTREAM s, int pause);                                              /* 0x42c010, pauses (pause != 0) or resumes the registered stream s; 0, -1 */
int sfx_SetStreamVolume(HSTREAM s, int vol);                                            /* 0x42c050, sets the volume of the registered stream s; 0, -1 */
int sfx_SetStreamLoopCount(HSTREAM s, int count);                                       /* 0x42c090, sets the loop count of the registered stream s; 0, -1 */
int sfx_SetStreamPos(HSTREAM s, int ms);                                                /* 0x42c0d0, seeks the registered stream s to ms; 0, -1 */
int sfx_StreamStatus(HSTREAM s);                                                        /* 0x42c110, AIL stream status of the registered s, -1 if not registered */
int sfx_GetStreamPos(HSTREAM s, int *ms);                                               /* 0x42c140, reads the position of the registered stream s in ms; 0, -1 */
int sfx_GetStreamLength(HSTREAM s, int *ms);                                            /* 0x42c180, reads the length of the registered stream s in ms; 0, -1 */
int sfx_FreeDriverSlot(void);                                                          /* 0x42c1c0, the first free driver slot, -1 if all 16 are used */
int sfx_FindDriver(SFXDRIVER *d);                                                       /* 0x42c1e0, the slot of the driver d, -1 if not open */
int sfx_FreeSampleSlot(SFXDRIVER *d);                                                   /* 0x42c200, a free sample slot of d's table while d is not full, else -1 */
int sfx_FreeStreamSlot(SFXDRIVER *d);                                                   /* 0x42c240, a free stream slot of d's table while d is not full, else -1 */
int sfx_FindSample(HSAMPLE s);                                                          /* 0x42c280, the slot of the sample s in its driver, -1 */
int sfx_FindStream(HSTREAM s);                                                          /* 0x42c2c0, the slot of the stream s in its driver, -1 */
int sfx_SampleDriver(HSAMPLE s);                                                        /* 0x42c300, the slot of the driver that holds the sample s, -1 */
int sfx_StreamDriver(HSTREAM s);                                                        /* 0x42c340, the slot of the driver that holds the stream s, -1 */
int sfx_NumDrivers(void);                                                               /* 0x42c380, the number of open drivers */

#endif
