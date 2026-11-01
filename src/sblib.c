/* sblib.c -- original file name unknown ("sb_Init", "sblib successfully initialzied").
   Sample/sound library on top of the sound driver wrappers: sample banks (files with a 0x144-byte
   header and 0x40-byte entries) loaded on demand into a memory budget, and a fixed set of playing
   sample slots. Extent in GildeGold.exe (inferred): 0x425bc0..0x4273d0 (0x4273d0 starts the script
   interpreter). Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/sblib.c); boundaries in decl/text.dcl */
struct SBSAMPLE;
struct SBBANK;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "sfx.h"
#include "mem.h"

typedef struct SBDATA {             /* a loaded sample: 12-byte header, then the file data */
    char fmt;                       /* 1 wav, 2 mp3 */
    int len;                        /* 0x04 */
    char *data;                     /* 0x08 */
} SBDATA;

typedef struct SBVAR {              /* a loaded variation: header, count SBDATAs, then their data */
    int count;
    SBDATA *items;                  /* 0x04 */
    int len;                        /* 0x08 */
} SBVAR;

typedef struct SBENTRY {            /* 0x40 bytes */
    int offset;                     /* file offset of the data */
    char name[50];                  /* 0x04 */
    char type;                      /* 0x36, 1 sample (SBDATA), 2 variation (SBVAR) */
    void *data;                     /* 0x38, NULL = not loaded */
    int used;                       /* 0x3c, time of last use */
} SBENTRY;

typedef struct SBBANK {             /* 0x144 bytes, also the file header */
    char name[50];
    char path[256];                 /* 0x32 */
    int count;                      /* 0x134 */
    SBENTRY *entries;               /* 0x138 */
    struct SBBANK *next;            /* 0x13c */
    int refs;                       /* 0x140 */
} SBBANK;

typedef struct SBSAMPLE {           /* 0x30 bytes, sb_Count of them at sb_Samples */
    int handle;
    SBENTRY *info;                  /* 0x04, NULL = unused */
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


struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);                                       /* 0x426030 */
SBENTRY *sb_FindEntry(SBBANK *bank, char *name);                                        /* 0x4260c0 */
SBSAMPLE *sb_FindSampleOf(SBENTRY *e, int *idx);                                        /* 0x4262f0 */
SBENTRY *sb_FindOldestEntry(void);                                                      /* 0x426350 */
SBBANK *sb_GetBankOf(SBENTRY *e);                                                       /* 0x426450 */
int sb_FreeMemory(int size);                                                            /* 0x4264a0 */
SBSAMPLE *sb_AllocSample(void);                                                         /* 0x426540 */
SBBANK *sb_LastBank(void);                                                              /* 0x426630 */
int sb_LoadEntry(SBBANK *bank, SBENTRY *e);                                             /* 0x426660 */
void sb_UnloadAll(void);                                                                /* 0x426db0 */
int sb_Play(struct SBSAMPLE *s, int fade, int delay);                                          /* 0x427110 */
void sb_Stop(struct SBSAMPLE *s, int fade);                                                    /* 0x427290 */

int sb_Mem = 0;  /* 0x633190 */
int sb_Used = 0;  /* 0x633194 */
SBBANK * sb_Banks = 0;  /* 0x633198 */
int sb_Driver = 0;  /* 0x63319c */
char sb_Path[256] = {0};  /* 0x6331a8 */
extern int sb_Count;
SBSAMPLE * sb_Samples = 0;  /* 0x6332a8 */
int sb_Count = 0;  /* 0x6332ac */
int sb_Stopping = 0;  /* 0x6332b0 */
unsigned int sb_FadeTime = 3000;  /* 0x5f8140 */
unsigned int sb_StopTime = 3000;  /* 0x5f8144 */
SBBANK sb_DummyBank = {0};  /* 0x14607c0 */
char sb_Msg[256] = {0};  /* 0x1460920 */
SBSAMPLE sb_Dummy = {0};  /* 0x1460a20 */
SBENTRY sb_DummyEntry = {0};  /* 0x1460a60 */

/* d3_Time * 13, spelled so that the volatile is read three times */
#define SB_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

/* 0x425bc0, opens the sound driver once and allocates the count sample slots (memory budget mem, default
   0x1fffcc bytes), zeroes them and initializes each through sfx_AllocSample, seeds rand with the time and
   prints the sblib message; side effects: sb_Driver / sb_Mem / sb_Count / sb_Samples. Returns 0, -1 when a
   sample slot failed to initialize, and -1 when the driver is already open or the allocation failed. */
int sb_Init(int mem, int count, int a, int b, int c)
{
    int ret = 0;
    int i;

    if (sb_Driver == 0) {
        sb_Dummy.info = &sb_DummyEntry;
        if (sfx_OpenDigOutput(&sb_Driver, a, b, c) == 0) {
            sb_Mem = mem;
            if (!mem)
                sb_Mem = 0x1fffcc;
            sb_Count = count;
            sb_Samples = mem_Alloc(count * sizeof(SBSAMPLE), "sb_Init");
            if (sb_Samples) {
                D3_ZERO_AT(sb_Samples, sb_Count * sizeof(SBSAMPLE));
                for (i = 0; i < sb_Count; i++)
                    if (sfx_AllocSample(sb_Driver, &sb_Samples[i]) == -1)
                        ret = -1;
                srand(time(NULL));
                sprintf(sb_Msg, "sblib successfully initialzied ! May use %d bytes of memory", mem);
                return ret;
            }
        }
    }
    return -1;
}
/* 0x425ce0, shuts the sample library down: unloads every sample bank, frees the sample slot array and
   closes the sound driver; side effects: the sb_ globals go back to their empty state. Safe to call twice. */

void sb_Exit(void)
{
    sb_UnloadAll();
    if (sb_Samples) {
        mem_Free(sb_Samples);
        sb_Samples = NULL;
    }
    if (sb_Driver) {
        sfx_CloseDigOutput(sb_Driver);
        sb_Driver = 0;
    }
}
/* 0x425d20, per-frame update of all samples: fades in / out (sb_FadeTime), repeat delays with loop counts,
   restart bookkeeping and the done callback; while sb_Stopping is set it fades everything out over
   sb_StopTime and clears the flag once no sample plays any more */

void sb_Update(void)
{
    int i, done;
    unsigned int now, el, fl;
    SBSAMPLE *s;

    if (!sb_Driver)
        return;
    if (!sb_Stopping) {
        if (sb_FadeTime) {
            now = SB_TIME;
            for (i = 0; i < sb_Count; i++) {
                s = &sb_Samples[i];
                if (!s->info)
                    continue;
                if (s->flags & 0x10) {
                    s->info->used = now;
                    s->start = now;
                }
                if (sfx_SampleStatus(s->handle) == 4) {
                    if (s->flags & 3) {
                        if (now - s->fadestart < sb_FadeTime) {
                            if (s->flags & 1)
                                sfx_SetSampleVolume(s->handle, (int)((float)(now - s->fadestart) / sb_FadeTime * s->volume));
                            else
                                sfx_SetSampleVolume(s->handle, (int)((float)(s->fadestart - now + sb_FadeTime) / sb_FadeTime * s->volume));
                        } else if (s->flags & 2) {
                            sfx_EndSample(s->handle);
                            /* the original stores the flags twice; the volatile store keeps VC6 from merging them */
                            fl = s->flags | 8;
                            s->volatileFlags = fl;
                            s->fadestart = 0;
                            s->flags = fl & ~3;
                        } else {
                            sfx_SetSampleVolume(s->handle, s->volume);
                            s->flags &= ~3;
                            s->fadestart = 0;
                        }
                    }
                } else if (s->delay == 0 || s->loops == 1 || (s->flags & 0xe)) {
                    if (s->done) {
                        s->done(s);
                        s->done = NULL;
                    }
                } else if (s->last) {
                    if (s->last + s->delay < SB_TIME) {
                        if (s->loops)
                            s->loops--;
                        sb_Play(s, 0, s->delay);
                    }
                } else
                    s->last = SB_TIME;
            }
        }
    } else {
        now = SB_TIME;
        done = 1;
        for (i = 0; i < sb_Count; i++) {
            s = &sb_Samples[i];
            if (s->info && sfx_SampleStatus(s->handle) == 4) {
                done = 0;
                el = SB_TIME;
                el -= s->fadestart;
                if (el < sb_StopTime)
                    sfx_SetSampleVolume(s->handle, (int)((float)el / sb_StopTime * s->volume));
                else
                    sb_Stop(s, 0);
            }
        }
        sb_Stopping = done == 0;
    }
}
/* 0x426030 */

SBSAMPLE *sb_GetSample(SBBANK *bank, char *name)
{
    SBSAMPLE *s;

    if (sb_Driver) {
        s = sb_AllocSample();
        if (s) {
            s->info = sb_FindEntry(bank, name);
            if (s->info) {
                s->done = NULL;
                s->loops = 1;
                s->pan = abs(0x7f) / 2;
                s->volume = 0x7f;
                sfx_SetSampleVolume(s->handle, 0x7f);
                sfx_SetSamplePan(s->handle, s->pan);
                sfx_SetSampleLoopCount(s->handle, s->loops);
            } else
                s = NULL;
        }
        return s;
    }
    return &sb_Dummy;
}
/* 0x4260c0, finds a bank entry by name (case-insensitive, at most 50 chars) in the given bank, or in every
   loaded bank when bank is NULL; returns the entry, NULL when nothing matches, &sb_DummyEntry when the
   driver is closed */

SBENTRY *sb_FindEntry(SBBANK *bank, char *name)
{
    int i = 0;
    SBENTRY *e = NULL;
    SBBANK *b;
    int n;

    if (!sb_Driver)
        return &sb_DummyEntry;
    if (bank) {
        n = bank->count;
        for (; i < n; i++)
            if (!_strnicmp(bank->entries[i].name, name, 50)) {
                e = &bank->entries[i];
                break;
            }
    } else {
        for (b = sb_Banks; b && !(e = sb_FindEntry(b, name)); b = b->next)
            ;
    }
    return e;
}
/* 0x426170 */

void sb_SetRestart(SBSAMPLE *s, int on)
{
    if (on)
        s->flags |= 0x10;
    else
        s->flags &= ~0x10;
}
/* 0x426190, allocates a sample slot for variation idx of the named type-2 entry (the bank entry is loaded
   on demand, the bank looked up when bank is NULL) and marks the slot as fixed-variation; returns the slot,
   NULL when the entry is no variation, idx is out of range or no bank owns it, &sb_Dummy when the driver is
   closed */

SBSAMPLE *sb_GetVariation(SBBANK *bank, char *name, int idx)
{
    SBSAMPLE *s;

    if (!sb_Driver)
        return &sb_Dummy;
    s = sb_AllocSample();
    if (s) {
        s->info = sb_FindEntry(bank, name);
        if (s->info) {
            if (!bank && !(bank = sb_GetBankOf(s->info)))
                return NULL;
            if (s->info->type == 2) {
                if (!s->info->data)
                    sb_LoadEntry(bank, s->info);
                if (s->info->data && idx < ((SBVAR *)s->info->data)->count) {
                    s->loops = 1;
                    s->pan = 0x3f;
                    s->flags |= 4;
                    s->volume = 0x7f;
                    s->var = idx;
                    return s;
                }
            }
        }
    }
    return NULL;
}
/* 0x426240, sets the directory sample bank names are opened in (sb_Path) and appends a backslash unless
   path already ends in one; a NULL path leaves it unchanged */

void sb_SetPath(char *path)
{
    if (path) {
        strcpy(sb_Path, path);
        if (path[strlen(path) - 1] != '\\')
            strcat(sb_Path, "\\");
    }
}
/* 0x426290, the first loaded bank whose name matches (case-insensitive, at most 50 chars), NULL when
   there is none */

SBBANK *sb_FindBank(char *name)
{
    SBBANK *bank;

    for (bank = sb_Banks; bank && _strnicmp(bank->name, name, 50); bank = bank->next)
        ;
    return bank;
}
/* 0x4262c0, the first loaded bank whose file path contains name, NULL when there is none */

SBBANK *sb_FindBankFile(char *name)
{
    SBBANK *bank;

    for (bank = sb_Banks; bank && !strstr(bank->path, name); bank = bank->next)
        ;
    return bank;
}
/* 0x4262f0, the first sample slot from index *idx on that plays entry e (*idx is updated to it);
   returns NULL when no slot does, &sb_Dummy when the driver is closed */

SBSAMPLE *sb_FindSampleOf(SBENTRY *e, int *idx)
{
    SBSAMPLE *samples;
    SBSAMPLE *s = NULL;
    int i;

    if (!sb_Driver)
        return &sb_Dummy;
    samples = sb_Samples;
    if (samples) {
        for (i = *idx; i < sb_Count; i++) {
            if (samples[i].info == e) {
                *idx = i;
                s = &samples[i];
                break;
            }
        }
    }
    return s;
}
/* 0x426350, the loaded bank entry used longest ago (lowest SB_TIME, data present) whose samples have all
   finished, so its data can be freed; returns &sb_DummyEntry when the driver is closed and NULL when
   nothing is loaded */

SBENTRY *sb_FindOldestEntry(void)
{
    SBENTRY *e = NULL;
    SBBANK *bank = sb_Banks;
    SBSAMPLE *s;
    int i, k;

    if (!sb_Driver)
        return &sb_DummyEntry;
    if (bank) {
        do {
            i = 0;
            if (!e) {
                for (; i < bank->count; i++) {
                    if (bank->entries[i].data) {
                        e = &bank->entries[i];
                        break;
                    }
                }
            }
            for (; i < bank->count; i++)
                if (e->used > bank->entries[i].used && bank->entries[i].data)
                    e = &bank->entries[i];
            bank = bank->next;
        } while (bank);
        if (e && (k = 0, s = sb_FindSampleOf(e, &k)) != NULL) {
            while (1) {
                if (sfx_SampleStatus(s->handle) == 4 || (s->flags | 0x10) != 0) {
                    e = NULL;
                    break;
                }
                s->info = NULL;
                k++;
                if (!(s = sb_FindSampleOf(e, &k)))
                    break;
            }
        }
    }
    return e;
}
/* 0x426450, the loaded bank that owns entry e; returns NULL when no bank does, &sb_DummyBank when the
   driver is closed */

SBBANK *sb_GetBankOf(SBENTRY *e)
{
    SBBANK *bank = sb_Banks;
    int i;

    if (!sb_Driver)
        return &sb_DummyBank;
    for (;;) {
        if (!bank)
            return bank;
        for (i = 0; i < bank->count; i++)
            if (&bank->entries[i] == e)
                return bank;
        bank = bank->next;
    }
}
/* 0x4264a0, frees the oldest loaded entries (sb_FindOldestEntry) until size bytes fit into the memory
   budget again, subtracting their size from sb_Used; returns 0, -1 when the oldest entry has no data or
   nothing more can be freed */

int sb_FreeMemory(int size)
{
    SBENTRY *e;

    while (sb_Mem - sb_Used < size) {
        e = sb_FindOldestEntry();
        if (e) {
            if (!e->data)
                return -1;
            if (e->type == 1)
                sb_Used -= ((SBDATA *)e->data)->len + 12;
            else
                sb_Used -= ((SBVAR *)e->data)->len + 12;
            mem_Free(e->data);
            e->data = NULL;
        } else if (sb_Mem - sb_Used < size)
            return -1;
    }
    return 0;
}
/* 0x426540, the sample slot to reuse: the first idle slot, then the one with the lowest start time among
   the idle slots; ends its sample and resets flags / delay / loops. Returns NULL when there is no slot
   array, &sb_Dummy when the driver is closed */

SBSAMPLE *sb_AllocSample(void)
{
    SBSAMPLE *s = NULL;
    int i;

    if (!sb_Driver)
        return &sb_Dummy;
    if (sb_Samples) {
        for (i = 0; i < sb_Count; i++) {
            if (sfx_SampleStatus(sb_Samples[i].handle) != 4 && !(sb_Samples[i].flags & 0x10)) {
                s = &sb_Samples[i++];
                break;
            }
        }
        if (s) {
            for (; i < sb_Count; i++)
                if (sb_Samples[i].start < s->start && sfx_SampleStatus(sb_Samples[i].handle) != 4
                    && !(sb_Samples[i].flags & 0x10))
                    s = &sb_Samples[i];
            sfx_InitSample(s->handle);
            s->flags = 0;
            s->delay = 0;
            s->loops = 1;
        }
    }
    return s;
}
/* 0x426630, the last bank in the list (where sb_LoadSampleBank appends), NULL when the list is empty,
   &sb_DummyBank when the driver is closed */

SBBANK *sb_LastBank(void)
{
    SBBANK *bank = sb_Banks;

    if (!sb_Driver)
        return &sb_DummyBank;
    if (bank)
        while (bank->next)
            bank = bank->next;
    return bank;
}
/* 0x426660, loads the data of a bank entry from the bank file (type 1: one sample, type 2: a variation set
   with its items), frees older data first when sb_Mem would be exceeded; returns 0, or -1 on failure */

int sb_LoadEntry(SBBANK *bank, SBENTRY *e)
{
    void *p = NULL;
    int off = 0;
    int ret = 0;
    FILE *f;
    SBDATA hdr, *d;
    SBVAR vhdr, *v;
    int i;

    if (!sb_Driver)
        return 0;
    if (!e->data && bank) {
        f = fopen(bank->path, "rb");
        if (f) {
                if (fseek(f, e->offset, SEEK_SET) == 0) {
                    if (e->type == 1) {
                        if (fread(&hdr, 12, 1, f) == 1) {
                            if (sb_Mem - sb_Used < hdr.len + 12)
                                sb_FreeMemory(hdr.len + 12);
                            p = d = mem_Alloc(hdr.len + 12, "sb_LoadEntry");
                            if (d) {
                                d->fmt = hdr.fmt;
                                d->len = hdr.len;
                                d->data = (char *)(d + 1);
                                /* reading into (d + 1) / d->len, not d->data / hdr.len, is what gives the original's store order */
                                if (fread((char *)(d + 1), d->len, 1, f) == 1) {
                                    e->used = SB_TIME;
                                    sb_Used += hdr.len + 12;
                                } else {
                                    mem_Free(p);
                                    p = NULL;
                                    ret = -1;
                                }
                            } else
                                sprintf(sb_Msg, "sb_LoadEntry: m_alloc schlug fehl f\xfcr %s", e->name);
                        } else
                            ret = -1;
                        e->data = p;
                    } else if (e->type == 2 && fread(&vhdr, 12, 1, f) == 1) {
                        if (sb_Mem - sb_Used < vhdr.len + 12)
                            ret = sb_FreeMemory(vhdr.len + 12);
                        p = v = mem_Alloc(vhdr.len + 12, "sb_LoadEntry");
                        if (v) {
                            v->count = vhdr.count;
                            v->len = vhdr.len;
                            v->items = (SBDATA *)(v + 1);
                            if (fread((SBDATA *)(v + 1), v->len, 1, f) == 1) {
                                for (i = 0; i < v->count; i++) {
                                    v->items[i].data = (char *)v->items + v->count * 12 + off;
                                    off += v->items[i].len;
                                }
                                sb_Used += vhdr.len + 12;
                                e->used = SB_TIME;
                                sprintf(sb_Msg, "Variation %s loaded ! MemUsed is now %d bytes.", e->name, sb_Used);
                            } else {
                                mem_Free(p);
                                p = NULL;
                                ret = -1;
                            }
                        } else
                            ret = -1;
                        e->data = p;
                    } else
                        ret = -1;
                } else
                    ret = -1;
            fclose(f);
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x426950, loads a sample bank file (a SBBANK header followed by its SBENTRY table) into the bank list,
   looking the file up in sb_Path and returning an already loaded bank with its reference count raised;
   preload loads the data of every entry. Returns the bank, &sb_DummyBank when the driver is closed and
   NULL when the file cannot be opened or read */

SBBANK *sb_LoadSampleBank(char *name, int preload)
{
    SBBANK hdr;
    char path[256];
    char msg[512];
    SBBANK *bank, *last;
    FILE *f;
    int i;

    if (!sb_Driver)
        return &sb_DummyBank;
    bank = sb_FindBankFile(name);
    if (!bank) {
        strcpy(path, sb_Path);
        strcat(path, name);
        f = fopen(path, "rb");
        if (f) {
        if (fread(&hdr, sizeof(SBBANK), 1, f) == 1
            && (bank = mem_Alloc(hdr.count * sizeof(SBENTRY) + sizeof(SBBANK), "sb_LoadSampleBank")) != NULL) {
            strncpy(bank->name, hdr.name, 50);
            strncpy(bank->path, path, 256);
            bank->count = hdr.count;
            bank->next = NULL;
            bank->entries = (SBENTRY *)(bank + 1);
            if (fread(bank->entries, sizeof(SBENTRY), bank->count, f) == bank->count) {
                last = sb_LastBank();
                if (last)
                    last->next = bank;
                else
                    sb_Banks = bank;
                sb_Used += hdr.count * sizeof(SBENTRY) + sizeof(SBBANK);
                sprintf(sb_Msg, "Samplebank %s loaded. MemUsed is now %d bytes.", name, sb_Used);
                bank->refs = 1;
                if (preload) {
                    for (i = 0; i < hdr.count; i++)
                        sb_LoadEntry(bank, &bank->entries[i]);
                }
            } else {
                mem_Free(bank);
                bank = NULL;
            }
        }
        fclose(f);
        } else
            sprintf(msg, "sb_LoadSampleBank: Samplebank %s konnte nicht geladen werden!", path);
    } else
        bank->refs++;
    return bank;
}
/* 0x426ba0, drops one reference of bank and, when the count reaches 0, stops the samples of its entries,
   frees their data and the bank itself (sb_Used is reduced by the data sizes and the bank block); returns 0,
   1 when the driver is closed and -1 when nothing is used or bank is NULL */

int sb_UnloadSampleBank(SBBANK *bank)
{
    int ret = 0;
    SBBANK *b, *prev = NULL;
    SBSAMPLE *s;
    SBENTRY *e;
    int i, k;
    char *path;

    if (!sb_Driver)
        return 1;
    if (sb_Used && bank) {
        for (b = sb_Banks; b; b = b->next) {
            if (b == bank) {
                if (--b->refs <= 0) {
                    if (prev)
                        prev->next = b->next;
                    else
                        sb_Banks = b->next;
                    path = bank->path;
                    sprintf(sb_Msg, "Unloading Samplebank %s ... MemUsed before unloading %d bytes.", path, sb_Used);
                    for (i = 0; i < b->count; i++) {
                        e = &b->entries[i];
                        if (b->entries[i].data) {
                            k = 0;
                            for (s = sb_FindSampleOf(e, &k); s; s = sb_FindSampleOf(&b->entries[i], &k)) {
                                sb_Stop(s, 0);
                                s->info = NULL;
                                k++;
                            }
                            e = &b->entries[i];
                            if (b->entries[i].type == 1)
                                sb_Used -= ((SBDATA *)e->data)->len + 12;
                            else
                                sb_Used -= ((SBVAR *)e->data)->len + 12;
                            mem_Free(b->entries[i].data);
                            b->entries[i].data = NULL;
                            b->entries[i].data = NULL;
                        }
                    }
                    sb_Used -= b->count * sizeof(SBENTRY) + sizeof(SBBANK);
                    sprintf(sb_Msg, "Finished unloading Samplebank %s ... MemUsed after unloading %d bytes.", path, sb_Used);
                    mem_Free(b);
                }
                break;
            }
            prev = b;
        }

    } else
        ret = -1;
    return ret;
}
/* 0x426db0, drops every loaded bank: the samples of every entry are stopped, the entry data and the bank
   blocks are freed and sb_Used is reduced by their sizes; sb_Banks ends up NULL. Does nothing when the
   driver is closed or no memory is used */

void sb_UnloadAll(void)
{
    SBBANK *bank, *next;
    SBSAMPLE *s;
    SBENTRY *e;
    int i, k;

    if (sb_Driver && sb_Used) {
        bank = sb_Banks;
        while (bank) {
            bank->refs = 0;
            for (i = 0; i < bank->count; i++) {
                e = &bank->entries[i];
                if (e->data) {
                    k = 0;
                    for (s = sb_FindSampleOf(e, &k); s; s = sb_FindSampleOf(&bank->entries[i], &k)) {
                        sb_Stop(s, 0);
                        s->info = NULL;
                        k++;
                    }
                    e = &bank->entries[i];
                    if (bank->entries[i].type == 1)
                        sb_Used -= ((SBDATA *)e->data)->len + 12;
                    else
                        sb_Used -= ((SBVAR *)e->data)->len + 12;
                    mem_Free(bank->entries[i].data);
                    bank->entries[i].data = NULL;
                    bank->entries[i].data = NULL;
                }
            }
            next = bank->next;
            sb_Used -= bank->count * sizeof(SBENTRY) + sizeof(SBBANK);
            mem_Free(bank);
            bank = next;
        }
        sb_Banks = NULL;
    }
}
/* 0x426f20, nonzero when sample s is playing (its handle's status is 4); 1 when the driver is closed and
   0 for an empty or finished slot */

int sb_IsPlaying(SBSAMPLE *s)
{
    int ret = 0;

    if (!sb_Driver)
        return 1;
    if (s && s->handle && sfx_SampleStatus(s->handle) == 4)
        ret = -1;
    return ret;
}
/* 0x426f60, 1 when the named entry of bank exists and is a variation set (type 2), also 1 when the driver
   is closed; 0 otherwise */

int sb_IsVariation(SBBANK *bank, char *name)
{
    SBENTRY *e;

    if (!sb_Driver)
        return 1;
    e = sb_FindEntry(bank, name);
    if (e && e->type == 2)
        return 1;
    return 0;
}
/* 0x426fa0 */

SBSAMPLE *sb_PlaySample(SBBANK *bank, char *name, int loops, int pan, int volume, int fade, int delay)
{
    SBSAMPLE *s;

    if (!sb_Driver)
        return &sb_Dummy;
    s = sb_AllocSample();
    if (s) {
        s->info = sb_FindEntry(bank, name);
        if (s->info) {
            s->loops = loops;
            s->pan = pan;
            s->volume = volume;
            sb_Play(s, fade, delay);
        } else
            s = NULL;
    }
    return s;
}
/* 0x427010 */

void sb_Resume(SBSAMPLE *s)
{
    if (sb_Driver && s->handle) {
        if (!s->info->data)
            sb_Play(s, 0, 0);
        else
            sfx_ResumeSample(s->handle);
    }
}
/* 0x427050, allocates a sample slot for variation idx of the named type-2 entry (the entry is loaded on
   demand, the bank looked up when bank is NULL), sets loops / pan / volume / the fixed-variation flag and
   starts it with the fade; returns the slot, NULL when the entry is no loaded variation or idx is out of
   range, &sb_Dummy when the driver is closed */

SBSAMPLE *sb_PlayVariation(SBBANK *bank, char *name, int idx, int loops, int pan, int volume, int fade)
{
    SBSAMPLE *s;
    SBENTRY *e;

    if (!sb_Driver)
        return &sb_Dummy;
    s = sb_AllocSample();
    if (s) {
        e = s->info = sb_FindEntry(bank, name);
        if (e && (bank || (bank = sb_GetBankOf(e))) && e->type == 2) {
            if (!e->data)
                sb_LoadEntry(bank, e);
            if (s->info->data && idx < ((SBVAR *)s->info->data)->count) {
                s->loops = loops;
                s->pan = pan;
                s->volume = volume;
                s->flags |= 4;
                s->var = idx;
                sb_Play(s, fade, 0);
            } else
                s = NULL;
        } else
            s = NULL;
    }
    return s;
}
/* 0x427110, starts sample s: loads the bank entry's data when it is missing, picks the SBDATA (a type-1
   sample, or a variation item - s->var when the fixed-variation flag is set, else a random one), maps fmt 1
   to ".wav" and 2 to ".mp3", hands data / len to sfx_SetSampleFile and sets volume / pan / loops / delay and
   the restart bookkeeping before sfx_StartSample; return 0, -1 when the bank, the load, the entry type or
   format or the driver handover fails
   @deep behaviour: 20 code hunk lines are one block-layout difference - the original keeps the failure epilogue inline after the sfx_SetSampleFile test (jne + or ebp,-1; pop; mov eax,ebp; ret at 0x4271b5, the four failure sites jump into it) with the success epilogue at the end, ours puts the single end: return ret block at the function end (mov eax,ebp; pop...; ret; or ebp,-1); every source shape (return -1 per site, ret = -1; goto end, end: return ret, if/else, 162 variants tried) gives the shared end block, so the statements and operands agree and only VC6's block layout / ret register role differs (asmdiff, listing) */

int sb_Play(SBSAMPLE *s, int fade, int delay)
{
    int ret = 0;
    SBENTRY *e;
    SBBANK *bank;
    SBDATA *d;
    SBVAR *v;
    char *ext;
    int r;

    if (!sb_Driver)
        return 0;
    e = s->info;
    if (!e->data) {
        bank = sb_GetBankOf(e);
        if (!bank) {
            ret = -1;
            goto end;
        }
        ret = sb_LoadEntry(bank, e);
        if (ret)
            goto end;
    }
    if (s->info->type == 1)
        d = s->info->data;
    else if (s->info->type == 2) {
        v = s->info->data;
        if (s->flags & 4)
            r = s->var;
        else
            r = rand() % v->count;
        d = &v->items[r];
    } else {
        ret = -1;
        goto end;
    }
    if (d->fmt == 1)
        ext = ".wav";
    else if (d->fmt == 2)
        ext = ".mp3";
    else {
        ret = -1;
        goto end;
    }
    if (sfx_SetSampleFile(s->handle, ext, d->data, d->len) == -1) {
        ret = -1;
        goto end;
    }
    if (fade) {
        s->fadestart = SB_TIME;
        s->flags |= 1;
        sfx_SetSampleVolume(s->handle, 0);
    } else
        sfx_SetSampleVolume(s->handle, s->volume);
    sfx_SetSamplePan(s->handle, s->pan);
    if (!(s->flags & 4) && delay) {
        sfx_SetSampleLoopCount(s->handle, 1);
        s->last = 0;
        s->delay = delay;
    } else {
        sfx_SetSampleLoopCount(s->handle, s->loops);
        s->delay = 0;
    }
    s->info->used = s->start = SB_TIME;
    s->flags &= ~8;
    sfx_StartSample(s->handle);
end:
    return ret;
}
/* 0x427290, stops sample s: with fade, sets the fade-out flag and the fade start (sb_Update ramps the
   volume down); without, stops the sample at the driver now and sets the stopped flag. Does nothing when
   the driver is closed or s is NULL */

void sb_Stop(SBSAMPLE *s, int fade)
{
    if (sb_Driver && s) {
        if (fade) {
            s->flags |= 2;
            s->fadestart = SB_TIME;
        } else {
            sfx_EndSample(s->handle);
            s->flags |= 8;
        }
    }
}
/* 0x4272f0 */

void sb_SetPan(SBSAMPLE *s, int pan)
{
    if (sb_Driver && s && pan >= 0 && pan <= 0x7f && s->pan != pan) {
        s->pan = pan;
        sfx_SetSamplePan(s->handle, pan);
    }
}
/* 0x427330, sets the sample volume (0..0x7f) and passes it to the driver when it differs; values out of
   range, a closed driver and a NULL sample are ignored */

void sb_SetVolume(SBSAMPLE *s, int volume)
{
    if (sb_Driver && s && volume >= 0 && volume <= 0x7f && s->volume != volume) {
        s->volume = volume;
        sfx_SetSampleVolume(s->handle, volume);
    }
}
/* 0x427370, sets the master volume of the sound driver; ignored when the library is not initialized */

void sb_SetMasterVolume(int volume)
{
    if (sb_Driver)
        sfx_SetMasterVolume(sb_Driver, volume);
}
/* 0x427390 */

void sb_SetLoops(SBSAMPLE *s, int loops)
{
    if (sb_Driver && s) {
        s->loops = loops;
        sfx_SetSampleLoopCount(s->handle, loops);
    }
}
/* 0x4273c0 */

int sb_IsInit(void)
{
    return sb_Driver != 0;
}
