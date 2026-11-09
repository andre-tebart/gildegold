/* Declarations of sblib.c (0x425bc0..0x4273d0): the sample library ("sb_Init", "sblib successfully initialzied"): sample
   banks loaded on demand into a memory budget, and a fixed set of playing sample slots. SBSAMPLE (the playing slot)
   is declared here for its users; SBBANK / SBENTRY stay sblib.c's types. Names are ours. */
#ifndef SBLIB_H
#define SBLIB_H

struct SBSAMPLE;
struct SBBANK;
struct SBENTRY;

typedef struct SBSAMPLE {           /* 0x30 bytes, sb_Count of them at sb_Samples */
    int handle;
    struct SBENTRY *info;           /* 0x04, NULL = unused */
    int start;                      /* 0x08 */
    int loops;                      /* 0x0c */
    unsigned int fadestart;         /* 0x10 */
    union {
        unsigned int flags;         /* 0x14, 1 fade in, 2 fade out, 4 fixed variation, 8 stopped, 0x10 restart */
        volatile unsigned int volatileFlags;    /* the same dword through a volatile store, where VC6 must keep
                                                   two flag stores (sb_Update: stopped + fade bits) */
    };
    int delay;                      /* 0x18, repeat delay */
    unsigned int last;              /* 0x1c */
    int var;                        /* 0x20, variation index */
    unsigned int volume;            /* 0x24 */
    int pan;                        /* 0x28 */
    void (*done)(struct SBSAMPLE *s);   /* 0x2c */
} SBSAMPLE;

int sb_Init(int mem, int count, int a, int b, int c);                            /* 0x425bc0, opens the sound driver once and allocates the count sample slots */
void sb_Exit(void);                                                              /* 0x425ce0, shuts the sample library down: unloads every sample bank, frees the */
void sb_Update(void);                                                            /* 0x425d20, per-frame update of all samples: fades in / out (sb_FadeTime), repeat */
struct SBSAMPLE *sb_GetSample(struct SBBANK *bank, char *name);                                /* 0x426030, a free sample slot set up for the bank entry called name (volume 0x7f, centred, one loop), NULL without driver or entry */
struct SBENTRY *sb_FindEntry(struct SBBANK *bank, char *name);                                 /* 0x4260c0, finds a bank entry by name (case-insensitive, at most 50 chars) in */
void sb_SetRestart(struct SBSAMPLE *s, int on);                                         /* 0x426170, sets or clears the restart flag of sample s */
struct SBSAMPLE *sb_GetVariation(struct SBBANK *bank, char *name, int idx);                    /* 0x426190, allocates a sample slot for variation idx of the named type-2 entry */
void sb_SetPath(char *path);                                                     /* 0x426240, sets the directory sample bank names are opened in (sb_Path) and */
struct SBBANK *sb_FindBank(char *name);                                                 /* 0x426290, the first loaded bank whose name matches (case-insensitive, at most */
struct SBBANK *sb_FindBankFile(char *name);                                             /* 0x4262c0, the first loaded bank whose file path contains name, NULL when there */
struct SBSAMPLE *sb_FindSampleOf(struct SBENTRY *e, int *idx);                                 /* 0x4262f0, the first sample slot from index *idx on that plays entry e (*idx is */
struct SBENTRY *sb_FindOldestEntry(void);                                               /* 0x426350, the loaded bank entry used longest ago (lowest SB_TIME, data present) */
struct SBBANK *sb_GetBankOf(struct SBENTRY *e);                                                /* 0x426450, the loaded bank that owns entry e */
int sb_FreeMemory(int size);                                                     /* 0x4264a0, frees the oldest loaded entries (sb_FindOldestEntry) until size bytes */
struct SBSAMPLE *sb_AllocSample(void);                                                  /* 0x426540, the sample slot to reuse: the first idle slot, then the one with the */
struct SBBANK *sb_LastBank(void);                                                       /* 0x426630, the last bank in the list (where sb_LoadSampleBank appends), NULL */
int sb_LoadEntry(struct SBBANK *bank, struct SBENTRY *e);                                      /* 0x426660, loads the data of a bank entry from the bank file (type 1: one */
struct SBBANK *sb_LoadSampleBank(char *name, int preload);                              /* 0x426950, loads a sample bank file (a struct SBBANK header followed by its struct SBENTRY */
int sb_UnloadSampleBank(struct SBBANK *bank);                                           /* 0x426ba0, drops one reference of bank and, when the count reaches 0, stops the */
void sb_UnloadAll(void);                                                         /* 0x426db0, drops every loaded bank: the samples of every entry are stopped, the */
int sb_IsPlaying(struct SBSAMPLE *s);                                                   /* 0x426f20, nonzero when sample s is playing (its handle's status is 4) */
int sb_IsVariation(struct SBBANK *bank, char *name);                                    /* 0x426f60, 1 when the named entry of bank exists and is a variation set (type */
struct SBSAMPLE *sb_PlaySample(struct SBBANK *bank, char *name, int loops, int pan, int volume, int fade, int delay); /* 0x426fa0, plays the bank entry called name once on a fresh sample slot */
void sb_Resume(struct SBSAMPLE *s);                                                     /* 0x427010, resumes sample s (plays it when its entry data is not loaded) */
struct SBSAMPLE *sb_PlayVariation(struct SBBANK *bank, char *name, int idx, int loops, int pan, int volume, int fade); /* 0x427050, allocates a sample slot for variation idx of the named type-2 entry */
int sb_Play(struct SBSAMPLE *s, int fade, int delay);                                   /* 0x427110, starts sample s: loads the bank entry's data when it is missing */
void sb_Stop(struct SBSAMPLE *s, int fade);                                             /* 0x427290, stops sample s: with fade, sets the fade-out flag and the fade start */
void sb_SetPan(struct SBSAMPLE *s, int pan);                                            /* 0x4272f0, sets the pan of sample s (0..127) */
void sb_SetVolume(struct SBSAMPLE *s, int volume);                                      /* 0x427330, sets the sample volume (0..0x7f) and passes it to the driver when it */
void sb_SetMasterVolume(int volume);                                             /* 0x427370, sets the master volume of the sound driver */
void sb_SetLoops(struct SBSAMPLE *s, int loops);                                        /* 0x427390, sets the loop count of sample s */
int sb_IsInit(void);                                                             /* 0x4273c0 */

extern unsigned int sb_FadeTime;                             /* 0x5f8140 */
extern unsigned int sb_StopTime;                             /* 0x5f8144 */
extern int sb_Mem;                                           /* 0x633190 */
extern int sb_Used;                                          /* 0x633194 */
extern struct SBBANK * sb_Banks;                                    /* 0x633198 */
extern int sb_Driver;                                        /* 0x63319c */
extern char sb_Path[256];                                    /* 0x6331a8 */
extern struct SBSAMPLE * sb_Samples;                                /* 0x6332a8 */
extern int sb_Count;                                         /* 0x6332ac */
extern int sb_Stopping;                                      /* 0x6332b0 */
extern struct SBBANK sb_DummyBank;                                  /* 0x14607c0 */
extern char sb_Msg[256];                                     /* 0x1460920 */
extern struct SBSAMPLE sb_Dummy;                                    /* 0x1460a20 */
extern struct SBENTRY sb_DummyEntry;                                /* 0x1460a60 */
#endif
