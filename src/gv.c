/* gv.c -- the graveyard module (tags "gv_AllocGraveyardField" / "gv_Alloc:_all_graves" 0x61120c..0x611240) with
   the avatar table 0x60f310 (32 records of 0xf4 bytes, first name "neVmMaven") and the effect slots 0x6f1008
   (8 of 0x170 bytes, "kugel" 0x611204). 0x4ed170..0x4f4e20: starts at the first user of the table 0x60f310,
   which begins the .data block after ahm.c's "ahm_AllocAhmInfo" 0x60f2f8; ends where ord.c's order handlers
   begin (their table 0x611768 follows gv's strings). The last functions of the file (0x4f2750 .. 0x4f1de0) are
   out of address order. Names are ours.
   The effect slots are GVEFFECT records read through the gv_EffectsT view: a building's cannon salvos, three parts
   each that are SHOTREC cannon balls (unk_5d33b0.h) fired with gv_FireShots (message 0x67, GVSHOTMSG), started by
   gv_StartShot, moved by gv_UpdateEffects and exploded by gv_ExplodeShot; the save game writes gv_LastTime, the cannon
   scene state gv_Cannon (cannon scene of unk_575860) and the slots field by field.
   Open: the avatars, the effect slots and the graveyard may be several modules sharing one .data block.
   Open: GVEFFPART duplicates SHOTREC and UNKHITS-style copies remain (am.h G9KIND / G9CANNON view gv's records); gv.h
   does not include unk_5d33b0.h, so the parts are cast to SHOTREC.
   unit of rea 8cef7d5b (src/game/gv.c); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "d3_types.h"                 /* D3COLOR, for d3_tools.h */
#include "d3_tools.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define gv_PlayerAiChar (*(AICHAR **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) as the AICHAR it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define GAPDATA __declspec(align(1)) unsigned char
#define gv_EffectsT (*(GVEFFECT (*)[8])&gv_Effects)  /* 0x6f1008, gv_Effects (char[0xb80]) as the 8 GVEFFECT slots */
#define gv_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is */
#define gv_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_91f6e3[];
extern GAPDATA full_data_91f6e4[];
#include "amt_types.h"
#include "ls.h"
#include "ahm.h"
#include "ai.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "d3_snd.h"
#include "f3gm.h"
#include "he.h"
#include "hst.h"
#include "io_text.h"
#include "main.h"
#include "mem.h"
#include "sblib.h"
#include "sv.h"
#include "tr.h"
#include "unk_4b39f0.h"
#include "unk_58bbd0.h"
#include "vfs.h"
#include "d3_interface.h"
#include "unk_458810.h"
#include "d3_engine.h"
#include "unk_5d33b0.h"
extern int gm_PlayerRecords[688];

extern const float unk_Tab5ee034[];

#define UNK_FW ((unsigned (*)())vfs_fwrite)
#define UNK_FR ((unsigned (*)())ls_Read)

typedef struct GVAVATAR {               /* 0xf4 bytes, a record of the avatar table unk_Avatars (32) */
    unsigned id;                        /* 0x00, four characters ("neVm"), the key of the save game's (id, used) pairs */
    char name[0x10];                    /* 0x04, "Maven" (the read report prints it) */
    char title[0x10];                   /* 0x14, "of Reading" */
    char pad24[0xce];
    unsigned char used;                 /* 0xf2, 1 while the record is in use (gv_AllocAvatar), 0 free */
    char padf3;
} GVAVATAR;
typedef char gv_AssertGVAVATAR[sizeof(GVAVATAR) == 0xf4 ? 1 : -1];
#define gv_AvatarsT (*(GVAVATAR (*)[0x20])&unk_Avatars)  /* 0x60f310, unk_Avatars (char[0x20][0xf4]) as the GVAVATAR records they are */

/* 0x4ed170, picks a free avatar record (byte +0xf2 = 0) starting at a random one of the 32 (cyclic), marks it used (+0xf2 = 1) after setting the 14 floats at +0x30, +0x3c .. to the floats before them (+0x2c ..)
   times 0.001, and returns it; 0 when all are in use */

char *gv_AllocAvatar(void)
{
    unsigned i;
    int n;
    int j;
    float *f;

    i = util_RandomU(0x20) & 0xffff;
    n = 0x20;
    do {
        if (unk_Avatars[i][0xf2] == 0) {
            j = 0xe;
            f = (float *)&unk_Avatars[i][0x30];
            do {
                *f = f[-1] * 0.001;
                f += 3;
                j--;
            } while (j != 0);
            unk_Avatars[i][0xf2] = 1;
            return unk_Avatars[i];
        }
        n--;
        i = (i + 1) & 0x1f;
    } while (n != 0);
    return 0;
}
/* 0x4ed1e0, writes the avatar table to the file f: the count 0x20, then per record the id (+0) and the used byte (+0xf2); 1 when all was written */

int gv_WriteAvatars(void *f)
{
    unsigned n, i;
    char *p;

    n = 0x20;
    if (f == 0)
        return 0;
    if (vfs_fwrite(&n, 4, 1, f) == 0)
        return 0;
    i = 0;
    if (n > 0) {
        p = &unk_Avatars[0][0xf2];
        do {
            if (vfs_fwrite(p - 0xf2, 4, 1, f) == 0)
                return 0;
            if (vfs_fwrite(p, 1, 1, f) == 0)
                return 0;
            i++;
            p += 0xf4;
        } while (i < n);
    }
    return 1;
}
/* 0x4ed270, reads the avatar table of 0x4ed1e0 from the file f: the used bytes are first all 0xff, then each saved (id, used) pair sets the record of that id; an unknown id and a record without a pair
   are reported (0x5de652, "Avatar >%s< not found in savegame, initializing to >unused<") and the count must be 0x20 ("Inconsisten Avatar count"); 1 when all was read. The reports are only
   formatted into a local buffer, never shown. */

int gv_ReadAvatars(void *f)
{
    unsigned n, i, j;
    unsigned id;
    unsigned char used;
    char buf[256];

    if (f == 0)
        return 0;
    if (ls_Read(&n, 4, 1, (int)f) == 0)
        return 0;
    if (n != 0x20)
        sprintf(buf, "Inconsisten Avatar count: <Expected: %i> <Found: %i>", 0x20, n);
    for (i = 0; i < 0x20; i++)
        gv_AvatarsT[i].used = 0xff;
    for (i = 0; i < n; i++) {
        if (ls_Read(&id, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&used, 1, 1, (int)f) == 0)
            return 0;
        for (j = 0; j < 0x20; j++) {
            if (gv_AvatarsT[j].id == id) {
                gv_AvatarsT[j].used = used;
                goto next;
            }
        }
        {
            char name[5] = {0};

            name[3] = (char)id;
            name[2] = (char)(id >> 8);
            name[1] = (char)(id >> 16);
            name[0] = (char)(id >> 24);
            name[4] = 0;
            sprintf(buf, "Avatar >%s< not found in savegame, initializing to >unused<", name);
        }
    next:
        ;
    }
    for (i = 0; i < 0x20; i++) {
        if (gv_AvatarsT[i].used == 0xff) {
            sprintf(buf, "Avatar >%s< not found in savegame, initializing to >unused<", gv_AvatarsT[i].name);
            gv_AvatarsT[i].used = 0;
        }
    }
    return 1;
}
/* 0x4ed3f0, the default callback of an effect slot (its argument: the slot's GVEFFARG): for the current record
   (cur), the building of its id (gm_BuildQuery(1, 1, id)); when its owner character is a player (kind 6), posts the
   Domizil message 12 to it, text 0x4071 + 0 / 1 / 2 (2 when the record's mode is 1 and its value above 25, 1 when
   mode 1 and the value at most 25) */

int gv_EffectNotifyOwner(GVEFFARG *a)
{
    int i;
    AIBLD *b;
    AICHAR *ch;
    int m;

    for (i = 0; i < 3; i++) {
        if (a->cur == i) {
            b = (AIBLD *)gm_BuildQuery(1, 1, a->recs[i].bldId);
            m = 0;
            if (b != 0 && b->owner != 0xffff) {
                ch = &gv_CharsT[b->owner];
                if (a->recs[i].mode == 1) {
                    m = 2;
                    if (!(a->recs[i].value > 25.0f))
                        m = 1;
                }
                if (ch->kind == 6)
                    ai_DomizilMessage12((char *)ch, (char *)b, m);
            }
        }
    }
    return 0;
}
/* 0x4ed480, writes the game state of the effect slots to the save file f: gv_LastTime field by field, the cannon scene state
   gv_Cannon*, then per slot of gv_Effects its saved fields (id, the three floats, the three parts without their v3 / time,
   the tail); 1 when all was written
   @deep Read in full against 0x4ed480..0x4ed904: all 43 writes match in order, size and target (globals,
   then per slot 5 + 3x9 + 1 fields of GVEFFECT), the jbe/ja tests come from the unsigned vfs_fwrite result
   <= 0, the loop is the index loop (jl against the end). 427 vs 427 instructions; the only difference is
   the strength-reduced slot pointer's bias: ours edi = slot + 0xc (- lea eax,[edi-0xc] / push edi for +0xc
   ...), the original's edi = slot + 0x38 (+ lea eax,[edi-0x38] ... push edi for parts[0].v2), every lea
   displacement shifted by 0x2c and the first five writes scheduled around it. Tried the pointer loop (int
   compare), the char offsets and the typed GVEFFECT members: same bias. The gv_ReadEffects loop has the
   same +0x38 bias, so it is the compiler's induction choice: allocation. */

int gv_WriteEffects(void *f)
{
    int i;
    GVEFFECT *e;

    if (vfs_fwrite(&gv_LastTime.year, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_LastTime.hour, 2, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_LastTime.min, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_LastTime.xy, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_Cannon.bld, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_Cannon.spare, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_Cannon.camTarget, 12, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_Cannon.sliderA, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_Cannon.sliderB, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_Cannon.selected, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    for (i = 0; i < 8; i++) {
        e = &gv_EffectsT[i];
        if (vfs_fwrite(&e->id, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->f08, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->f0c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->f10, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->f14, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[0].v0, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[0].v1, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[0].v2, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[0].v4, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[0].a, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[0].b, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[0].c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[0].state, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[0].steps, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[1].v0, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[1].v1, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[1].v2, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[1].v4, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[1].a, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[1].b, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[1].c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[1].state, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[1].steps, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[2].v0, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[2].v1, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[2].v2, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->parts[2].v4, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[2].a, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[2].b, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[2].c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[2].state, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&e->parts[2].steps, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->tail, 12, 1, (struct VFSFILE *)f) <= 0)
            return 0;
    }
    return 1;
}
/* 0x4ed910, reads what gv_WriteEffects wrote from the save file f (version v: the saves before 0x10055 have no fields
   f0c / f10, they are 1.0); sets each slot's callback to gv_EffectNotifyOwner with its gv_EffectArgs record and the
   parts' time to ai_ClockMs; 1 when all was read
   @deep Read in full against 0x4ed910..0x4ede42: the 10 global reads, per slot the 5 + 3x9 + 1 reads of
   GVEFFECT in order, the v >= 0x10055 branch (else f0c = f10 = 1.0), the three parts' time = ai_ClockMs
   and the arg / cb stores with q stepping through gv_EffectArgs all match (445 vs 446). The rest is the
   slot pointer's bias, as in gv_WriteEffects: ours esi = slot + 0xc (- lea eax,[esi-0xc] ...), the
   original's esi = slot + 0x38 (+ lea eax,[esi-0x38] ...), every displacement shifted by 0x2c; with its
   bias the original loads 1.0 into eax once for the two stores (+ mov eax,0x3f800000 / mov [esi-0x2c],eax
   / mov [esi-0x28],eax) where ours stores immediates. Induction choice: allocation. */

int gv_ReadEffects(void *f, int v)
{
    int i;
    GVEFFECT *e;
    GVEFFARG *q;

    if (ls_Read(&gv_LastTime.year, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_LastTime.hour, 2, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_LastTime.min, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_LastTime.xy, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_Cannon.bld, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_Cannon.spare, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_Cannon.camTarget, 12, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_Cannon.sliderA, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_Cannon.sliderB, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_Cannon.selected, 4, 1, (int)f) == 0)
        return 0;
    q = gv_EffectArgs;
    for (i = 0; i < 8; i++) {
        e = &gv_EffectsT[i];
        if (ls_Read(&e->id, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->f08, 4, 1, (int)f) == 0)
            return 0;
        if (v >= 0x10055) {
            if (ls_Read(&e->f0c, 4, 1, (int)f) == 0)
                return 0;
            if (ls_Read(&e->f10, 4, 1, (int)f) == 0)
                return 0;
        } else {
            e->f0c = 1.0f;
            e->f10 = 1.0f;
        }
        if (ls_Read(&e->f14, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[0].v0, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[0].v1, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[0].v2, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[0].v4, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[0].a, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[0].b, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[0].c, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[0].state, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[0].steps, 4, 1, (int)f) == 0)
            return 0;
        e->parts[0].time = ai_ClockMs;
        if (ls_Read(e->parts[1].v0, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[1].v1, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[1].v2, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[1].v4, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[1].a, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[1].b, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[1].c, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[1].state, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[1].steps, 4, 1, (int)f) == 0)
            return 0;
        e->parts[1].time = ai_ClockMs;
        if (ls_Read(e->parts[2].v0, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[2].v1, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[2].v2, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(e->parts[2].v4, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[2].a, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[2].b, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[2].c, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[2].state, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&e->parts[2].steps, 4, 1, (int)f) == 0)
            return 0;
        e->parts[2].time = ai_ClockMs;
        if (ls_Read(e->tail, 12, 1, (int)f) == 0)
            return 0;
        e->arg = q;
        e->cb = gv_EffectNotifyOwner;
        q++;
    }
    return 1;
}
/* 0x4eddf0, resets the effect slots gv_Effects: all zero, then id -1 and the floats f08 / f0c / f10 = 1.0; gv_LastTime := ai_Time;
   clears the cannon scene state (8 ints from gv_Cannon.bld) and sets gv_Cannon.bld to -1 */

void unk_ResetEffectSlots(void)
{
    int i;

    memset(gv_Effects, 0, 0xb80);
    for (i = 0; i < 8; i++) {
        gv_EffectsT[i].id = -1;
        gv_EffectsT[i].f08 = 1.0f;
        gv_EffectsT[i].f0c = 1.0f;
        gv_EffectsT[i].f10 = 1.0f;
    }
    gv_LastTime = gv_TimeT;
    memset(&gv_Cannon.bld, 0, 0x20);
    gv_Cannon.bld = -1;
}
/* 0x4ede70, frees the objects (3 per slot at +0x15c, 0x447190) of all effect slots; the scene is switched with 0x4478e0 around it */

void gv_FreeEffectObjects(void)
{
    int sc;
    int *p, *q;
    int i;

    sc = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    p = (int *)(gv_Effects + 0x15c);
    do {
        i = 3;
        q = p;
        do {
            if (*q != 0) {
                d3if_Delete((struct D3OBJECT *)*q);
                *q = 0;
            }
            q++;
            i--;
        } while (i != 0);
        p += 0x5c;
    } while ((int)p < (int)((unsigned)&gv_Effects + 0xcdc));
    d3if_SelectScene(sc, 1);
}
/* 0x4eded0, the effect slot (gv_Effects) whose id (+0) is id, 0 when none; -1 never matches */

char *gv_FindEffect(int id)
{
    int *p;
    int i;

    if (id != -1) {
        i = 0;
        p = (int *)gv_Effects;
        do {
            if (*p == id)
                return gv_Effects + i * 0x170;
            p += 0x5c;
            i++;
        } while ((int)p < (int)(gv_Effects + 8 * 0x170));
    }
    return 0;
}
/* 0x4edf10, releases the effect slot of the id: marks it (flags 4) and frees it (id -1, flags 0) unless one of its 3
   parts is still in use (part flags 2) */

void gv_ReleaseEffect(int id)
{
    int i, n;

    if (id != -1) {
        for (i = 0; i < 8; i++) {
            if (gv_EffectsT[i].id == id)
                goto found;
        }
        return;
found:
        gv_EffectsT[i].flags |= 4;
        for (n = 0; n < 3; n++) {
            if ((gv_EffectsT[i].parts[n].state & 2) != 0)
                return;
        }
        gv_EffectsT[i].id = -1;
        gv_EffectsT[i].flags = 0;
    }
}


/* 0x4ee360, the explosion of a cannon ball that landed (gv_UpdateEffects): finds the buildings (ai_Blds) whose box (d3t_GetMeshBounds
   around the world position) holds the ball's position (mode 1, weight 1 / (1 + 0.005 d)) or lies within twice a tenth of its energy
   of it (mode 2, weight 1 / (1 + (0.0002 d + 0.005) d)); of those with energy * weight > 100 the 3 strongest go to the records of out
   (the strength / 100, up to 100); starts the smoke, earth and explosion particles at the ball (bigger when a building was hit) and
   the sound "Explosion" at the nearest building; with damage set the owners of the hit buildings get message 0x4f with the strength
   (mode 1), or the building's level (field 0x5c) falls by it (mode 2)
   @deep read the whole function against the original (hit record init, building loop with world position /
   distance / box tests as >= / <= chains, w1 / w2 weights, the 3-strongest selection, smoke / earth /
   explosion calls with float arguments through the real d3_tools prototypes and a D3COLOR, the sound, the
   damage messages as AIMSG); 469 vs 478 instructions, frame 0x1a8 vs 0x1b4. Left: register roles (ours
   zero in ebx / shot in esi / sel in ebp, the original's zero-and-mode in ebp / shot in ebx / sel in esi;
   hence ebx pushed after the init loop in the original), the original keeps w1 in a slot computed before
   d3t_GetMeshBounds where ours sinks it into the inside branch (fmul / fadd / fdivr after mov ebx,1), the
   selection loop walks &recs[k].value by pointer in the original (lea edx,[esp+0xa8]) and ours by a scaled
   index (mov esi,0xfffffff0), and the wind direction block keeps scale in a slot and wind x / z on the FPU
   stack (fld [ebx+0x40]; fld [ebx+0x48]; fld [esp+0x4c]; fmul st(2)) where ours folds them (fld
   [esi+0x4c]; fld st(0); fmul [esi+0x40]): FPU scheduling and allocation. Tried: sqrt without the float
   cast (no change), mode = 0 at the loop top (no change). */
void gv_ExplodeShot(int damage, int effectId, SHOTREC *shot, GVEFFARG *out)
{
    GVEFFARG h;
    D3VECTOR c;
    D3VECTOR pos, half, mn, hi, lo, dir;
    float m, fd, best, w1, w2, fv, scale;
    float radius, height, z;
    float dx, dy, dz;
    int i, k, sel, mode, hits;
    AIBLD *bld, *nearest, *hit;
    D3COLOR color;
    struct SBSAMPLE *snd;
    AIMSG msg;
    char dmg;

    h.effectId = effectId;
    hits = 0;
    nearest = 0;
    best = 100000.0f;
    for (i = 0; i < 3; i++) {
        h.recs[i].bldId = -1;
        h.recs[i].dist = 1000000.0f;
        h.recs[i].mode = 0;
        h.recs[i].value = 0.0f;
    }
    c = shot->pos;
    for (i = 0; i < 256; i++) {
        sel = -1;
        m = shot->energy * 0.1f;
        bld = &ai_BldsT[i];
        if (bld->id == -1 || bld->type == 0 || bld->object == 0)
            continue;
        d3_PointToWorld((D3OBJECT *)bld->object, &((D3OBJECT *)bld->object)->pos, &pos);
        dx = c.x - pos.x;
        dy = c.y - pos.y;
        dz = c.z - pos.z;
        fd = sqrt(dz * dz + dy * dy + dx * dx);
        if (nearest == 0 || best > fd) {
            best = fd;
            nearest = bld;
        }
        w1 = 1.0f / (fd * 0.005f + 1.0f);
        w2 = 1.0f / ((fd * 0.0002f + 0.005f) * fd + 1.0f);
        d3t_GetMeshBounds(&mn, &half, (D3OBJECT *)bld->object);
        half.x -= mn.x;
        half.y -= mn.y;
        half.z -= mn.z;
        half.x *= 0.5f;
        half.y *= 0.5f;
        half.z *= 0.5f;
        lo.x = pos.x - half.x;
        lo.y = pos.y - half.y;
        lo.z = pos.z - half.z;
        hi.x = half.x + pos.x;
        hi.y = half.y + pos.y;
        hi.z = half.z + pos.z;
        if (c.x >= lo.x && c.x <= hi.x && c.y >= lo.y && c.y <= hi.y && c.z >= lo.z && c.z <= hi.z) {
            mode = 1;
            fv = w1;
        } else {
            m = m + m;
            if (!(m + c.x >= lo.x && c.x - m <= hi.x && m + c.y >= lo.y && c.y - m <= hi.y && m + c.z >= lo.z && c.z - m <= hi.z))
                continue;
            mode = 2;
            fv = w2;
        }
        fv = fv * shot->energy;
        if (fv <= 100.0f)
            continue;
        fv = fv * 0.01f;
        hits++;
        fv = fv < 100.0f ? fv : 100.0f;
        nearest = bld;
        for (k = 0; k < 3; k++) {
            if (h.recs[k].bldId == -1) {
                h.recs[k].value = fv;
                h.recs[k].mode = mode;
                h.recs[k].bldId = bld->id;
                h.recs[k].dist = fd;
                goto next;
            }
        }
        for (k = 0; k < 3; k++) {
            if (h.recs[k].value < fv && (sel == -1 || h.recs[k].value < h.recs[sel].value))
                sel = k;
        }
        if (sel != -1) {
            h.recs[sel].value = fv;
            h.recs[sel].mode = mode;
            h.recs[sel].bldId = bld->id;
            h.recs[sel].dist = fd;
        }
    next:;
    }
    m = shot->energy * 0.1f;
    scale = shot->wind.scale;
    color.b = color.g = color.r = 0xc0;
    color.a = 0x7f;
    dir.x = scale * shot->wind.pos[0];
    dir.z = scale * shot->wind.pos[2];
    dir.y = 0.0f;
    c.y = c.y + 10.0f;
    d3t_CreateSmoke(0, &c, 200, 25.0f, 70.0f, m * 0.25f, 900.0f, 100.0f, 70.0f, &dir, 1500, 7, color);
    d3t_CreateEarth(0, &c, 100, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, color);
    if (hits != 0) {
        z = m * 0.03f;
        height = m * 0.6f;
        radius = m * 0.2f;
    } else {
        z = m * 0.01f;
        height = m * 0.3f;
        radius = m * 0.1f;
    }
    d3t_CreateExplosion(0, &c, 150, radius, height, z, 80.0f, 200);
    snd = sb_GetSample(0, "Explosion");
    if (snd != 0)
        d3snd_PlayAt((struct SNDHANDLE *)snd, (D3OBJECT *)nearest->object, 0x6b, 12200.0f);
    if (damage != 0) {
        for (i = 0; i < 3; i++) {
            if (h.recs[i].bldId == -1)
                continue;
            gm_FindById(&hit, 0, 0, h.recs[i].bldId);
            if (hit == 0)
                continue;
            if (h.recs[i].mode == 1) {
                msg.type = 0x4f;
                if (hit->owner == 0xffff)
                    msg.to = gv_PlayerAiChar->netId;
                else
                    msg.to = gv_CharsT[hit->owner].netId;
                msg.fromId = hit->id;
                msg.time = gv_TimeT;
                msg.flags = 2;
                msg.arg[0] = hit->id;
                msg.arg[1] = (int)h.recs[i].value;
                msg.extraFlag = 1;
                cm_SendLongMessage((char *)&msg);
            } else if (h.recs[i].mode == 2) {
                dmg = (char)(int)-h.recs[i].value;
                cm_BeginFieldBlock((char *)hit, hit->id);
                cm_AddFieldValues(1, 1, 0x5c, &dmg);
                cm_SendFieldDeltas();
            }
        }
    }
    if (out != 0)
        *out = h;
}
/* 0x4edf80, the per-frame update of the effect slots (the cannon balls in flight): the minutes since gv_LastTime (* 1/60 * 0.1) are added
   to the three fade values of every used slot (up to 1.0); each part in flight (state 1) moves one step (ai_StepShot, ai_StepShotSlow
   when the slot's f14 is set) and gets its "kugel" object placed at the ball; a ball that left the map (3) or landed (2: the explosion
   gv_ExplodeShot) loses its object and is reported to the slot's callback (arg->cur = the part); a slot with three idle parts loses flags
   1 / 2 and is released when flag 4 is set, as is a slot without flag 2
   @deep read the whole function against the original (slot loop, fade clamps, the three part states,
   StepShot/StepShotSlow with the part as SHOTREC, kugel object create/place/delete, callback reporting,
   gv_ExplodeShot); 256 vs 260 instructions, frame 0x18 vs 0x1c. Left: the original keeps the first fade
   sum a in a stack slot ([esp+0x20]: fst then fld/fcomp/jp/fld) where ours keeps it in st0 (fcom/jnp/fstp
   st(0)) - FPU allocation, the extra frame slot; the fmul by 0.1 is scheduled before the AITIME copy in
   the original (order); the shot pointer in ebp (lea ecx,[ebp+0x30]) vs ours from the state pointer (lea
   ecx,[ebx-0x2c]); eax/edx swap at the recs[j].bldId store. Tried a = field = sum (worse, 42 code hunks). */

void gv_UpdateEffects(void)
{
    int dt;
    int scene;
    float step;
    float a, b, c;
    int i, j;
    int idle;
    SHOTREC *shot;

    dt = gm_MinutesBetween(&gv_LastTime, &ai_Time);
    if (dt < 0)
        dt = 0;
    gv_LastTime = gv_TimeT;
    scene = d3_CurrentScene;
    step = (float)dt * 0.016666668f * 0.1f;
    d3if_SelectScene(0, 1);
    for (i = 0; i < 8; i++) {
        if (gv_EffectsT[i].id == -1)
            continue;
        idle = 0;
        gv_EffectsT[i].f08 = a = step + gv_EffectsT[i].f08;
        gv_EffectsT[i].f0c = b = step + gv_EffectsT[i].f0c;
        gv_EffectsT[i].f10 = c = step + gv_EffectsT[i].f10;
        gv_EffectsT[i].f08 = a < 1.0f ? a : 1.0f;
        gv_EffectsT[i].f0c = b < 1.0f ? b : 1.0f;
        gv_EffectsT[i].f10 = c < 1.0f ? c : 1.0f;
        if ((gv_EffectsT[i].flags & 2) == 0) {
            if (gv_EffectsT[i].flags & 4)
                gv_ReleaseEffect(gv_EffectsT[i].id);
            continue;
        }
        for (j = 0; j < 3; j++) {
            shot = (SHOTREC *)&gv_EffectsT[i].parts[j];
            if (shot->state == 0) {
                if (++idle >= 3) {
                    gv_EffectsT[i].flags &= ~3;
                    if (gv_EffectsT[i].flags & 4)
                        gv_ReleaseEffect(gv_EffectsT[i].id);
                }
            } else if (shot->state == 1) {
                if (gv_EffectsT[i].f14 == 0 ? ai_StepShot(shot) : ai_StepShotSlow(shot)) {
                    if (gv_EffectsT[i].objs[j] == 0) {
                        gv_EffectsT[i].objs[j] = d3_CreateObject(0, &d3_Origin, (D3VECTOR *)ai_ZeroRot, "kugel");
                        gv_EffectsT[i].objs[j]->objectFlags |= 4;
                        gv_EffectsT[i].objs[j]->stateFlags |= 0xc;
                        gv_EffectsT[i].objs[j]->objectFlags &= ~8;
                    }
                    d3_SetObjectPos(gv_EffectsT[i].objs[j], &shot->pos);
                }
                if (shot->state == 3) {
                    if (gv_EffectsT[i].objs[j] != 0) {
                        d3if_Delete(gv_EffectsT[i].objs[j]);
                        gv_EffectsT[i].objs[j] = 0;
                    }
                    if (gv_EffectsT[i].arg != 0)
                        gv_EffectsT[i].arg->cur = j;
                    gv_EffectsT[i].arg->recs[j].bldId = -1;
                    if (gv_EffectsT[i].cb != 0)
                        gv_EffectsT[i].cb(gv_EffectsT[i].arg);
                    shot->state = 0;
                    gv_EffectsT[i].tail[j] = 0;
                } else if (shot->state == 2) {
                    idle++;
                    if (gv_EffectsT[i].objs[j] != 0) {
                        d3if_Delete(gv_EffectsT[i].objs[j]);
                        gv_EffectsT[i].objs[j] = 0;
                    }
                    gv_ExplodeShot(gv_EffectsT[i].flags & 1, gv_EffectsT[i].id, shot, gv_EffectsT[i].arg);
                    if (gv_EffectsT[i].arg != 0)
                        gv_EffectsT[i].arg->cur = j;
                    if (gv_EffectsT[i].cb != 0)
                        gv_EffectsT[i].cb(gv_EffectsT[i].arg);
                    shot->state = 0;
                    gv_EffectsT[i].tail[j] = 0;
                    gv_EffectsT[i].arg->cur = -1;
                }
            }
        }
        if (idle >= 3) {
            gv_EffectsT[i].flags &= ~3;
            if (gv_EffectsT[i].flags & 4)
                gv_ReleaseEffect(gv_EffectsT[i].id);
        }
    }
    d3if_SelectScene(scene, 1);
}
/* 0x4eea70, the message 'nCTa' (0x4c6230) with the id at +1 of the object o */

void unk_Unk4eea70(char *o)
{
    int m[2];

    m[0] = 0x6154436e;
    m[1] = *(int *)(o + 1);
    ai_Unk4c6230(m);
}
/* 0x4eeaa0, reserves the effect slot of the id: 1 when it has one already or a free one (id -1) is found (the first free one is taken);
   0 for id -1. The original tests first >= -1, so with no free slot it writes the id before slot 0 and returns 1 as well; the slots
   are walked as ints compared signed (the original's jl) */

int gv_ReserveEffect(int id)
{
    int *p;
    int first, i;

    first = -1;
    if (id != -1) {
        i = 0;
        p = (int *)gv_Effects;
        do {
            if (*p == -1) {
                if (first == -1)
                    first = i;
            } else if (*p == id)
                return 1;
            p += 0x5c;
            i++;
        } while ((int)p < (int)(gv_Effects + 8 * 0x170));
        if (first >= -1) {
            p = (int *)gv_Effects;
            p[first * 0x5c] = id;
            return 1;
        }
    }
    return 0;
}
/* 0x4eeb00, stops the effect slot of the id (a server message, cm.c): sets its flags to 4 (to be released) and frees it (id -1,
   flags 0) when none of its 3 parts is active (state 0); 1 when the slot exists, 0 otherwise */

int gv_StopEffect(int id)
{
    int i, n;

    if (id != -1) {
        for (i = 0; i < 8; i++) {
            if (gv_EffectsT[i].id == id)
                goto found;
        }
    }
    return 0;
found:
    gv_EffectsT[i].flags = 4;
    for (n = 0; n < 3; n++) {
        if (gv_EffectsT[i].parts[n].state != 0)
            return 1;
    }
    gv_EffectsT[i].id = -1;
    gv_EffectsT[i].flags = 0;
    return 1;
}
typedef struct GVSHOTMSG {              /* 0x80 bytes, the data of message 0x67 (cm_SendShots): a cannon ball fired at a building; the receiver (0x4eee00) starts it as a part of the effect slot */
    int id;                             /* 0x00, the building's id, the effect slot's id */
    float v;                            /* 0x04, taken off the slot's fade value of the mode (only the first ball of a salvo carries it, then 0) */
    int mode;                           /* 0x08, 0 one ball, 1 two, 2 three */
    union {
        SHOTREC shot;                   /* 0x0c, the ball as sent (gv_FireShots) */
        GVEFFPART part;                 /* the same record as the slot's part it becomes (0x4eee00) */
    };
    int pad74[3];
} GVSHOTMSG;

/* 0x4eeb70, fires up to three cannon balls (a, b, d: SHOTREC, b and d may be 0) of the character ch at the building bld: unless the
   building's effect slot is busy (flag 2), the first ball pays (gm_SendMoneyDelta(-cost), cm_SendMoveGoodsUnchecked of x; the ball a
   also files an accusation, gv_Accuse (8, 1, ch)), and each ball is sent as message 0x67 (GVSHOTMSG); returns the last answer, -1
   when nothing was sent
   @deep read the whole function against the original (slot busy test, mode by d / b, memset, the three
   ball blocks with payment / accusation, the GVSHOTMSG fields and SHOTREC copies, v2 cleared after the
   first ball, the return value of cm_SendShots); 167 vs 167 instructions. Left: slot assignment (v2 / mode
   and first / r swapped: mov [esp+8] vs [esp+0xc], [esp+0x18] vs [esp+0x1c]) and in the first ball block
   ours reads the parameter v where the original reads its copy v2 (mov ecx,[esp+0xe4] vs mov
   ecx,[esp+0x40]): VC6 forwards v2 = v in ours, the copy stays a slot in the original; m.v / m.mode store
   order follows from it ([esp+0x50] / [esp+0x54]). */

int gv_FireShots(AIBLD *bld, AICHAR *ch, SHOTREC *a, SHOTREC *b, SHOTREC *d, float v, int cost, int x)
{
    float v2;
    int first;
    int r;
    int mode;
    char *e;
    GVSHOTMSG m;

    v2 = v;
    first = 1;
    r = -1;
    mode = 0;
    if (bld != 0) {
        e = gv_GetEffect(bld->id);
        if (e != 0 && (((GVEFFECT *)e)->flags & 2) == 0) {
            if (d != 0)
                mode = 2;
            else if (b != 0)
                mode = 1;
            memset(&m, 0, sizeof(m));
            if (a != 0) {
                gv_Accuse(8, 1, ch->netId, -1, -1);
                gm_SendMoneyDelta(ch->netId, -cost);
                cm_SendMoveGoodsUnchecked(-1, ch->netId, ai_OwnPlayer, x);
                m.id = bld->id;
                m.mode = mode;
                m.v = v2;
                m.shot = *a;
                first = 0;
                v2 = 0.0f;
                r = cm_SendShots(&m);
            }
            if (b != 0) {
                if (first) {
                    gm_SendMoneyDelta(ch->netId, -cost);
                    cm_SendMoveGoodsUnchecked(-1, ch->netId, ai_OwnPlayer, x);
                    first = 0;
                }
                m.id = bld->id;
                m.mode = mode;
                m.v = v2;
                m.shot = *b;
                v2 = 0.0f;
                r = cm_SendShots(&m);
            }
            if (d != 0) {
                if (first) {
                    gm_SendMoneyDelta(ch->netId, -cost);
                    cm_SendMoveGoodsUnchecked(-1, ch->netId, ai_OwnPlayer, x);
                }
                m.id = bld->id;
                m.v = v2;
                m.mode = mode;
                m.shot = *d;
                r = cm_SendShots(&m);
            }
            return r;
        }
    }
    return -1;
}
/* 0x4eedd0, the effect slot of the id or 0 */

char *gv_GetEffect(int id)
{
    int *p;
    int i;

    i = 0;
    p = (int *)gv_Effects;
    do {
        if (*p == id)
            return gv_Effects + i * 0x170;
        p += 0x5c;
        i++;
    } while ((int)p < (int)((unsigned)&gv_Effects + 0xb80));
    return 0;
}
/* 0x4eee00, starts a received cannon ball (message 0x67, GVSHOTMSG) in the effect slot of m->id: the first free part (tail 0)
   takes it (tail 1, the SHOTREC copied, state 1 in flight, time ai_ClockMs, steps 0), m->v is taken off the slot's charge of
   m->mode (not below 0), the slot gets flag 2 (and 1 when w); 1 when started, 0 without a slot or a free part */

int gv_StartShot(GVSHOTMSG *m, int w)
{
    float v, f;
    int mode, j;
    GVEFFECT *e;
    GVEFFPART part;

    v = m->v;
    mode = m->mode;
    part = m->part;
    e = (GVEFFECT *)gv_GetEffect(m->id);
    if (e == 0)
        return 0;
    for (j = 0; j < 3; j++) {
        if (e->tail[j] == 0)
            goto found;
    }
    return 0;
found:
    e->tail[j] = 1;
    e->charge[mode] = f = e->charge[mode] - v;
    e->charge[mode] = f > 0.0f ? f : 0.0f;
    e->parts[j] = part;
    e->flags |= 2;
    e->parts[j].time = ai_ClockMs;
    e->parts[j].state = 1;
    e->parts[j].steps = 0;
    if (w != 0)
        e->flags |= 1;
    return 1;
}
/* 0x4eeee0, sets the callback (+0x168) and its argument (+0x16c) of the slot of the id; without a callback the default one (0x4ed3f0) with the slot's record 0x6f0e48 + 0x38 * n */

void unk_Unk4eeee0(int id, void *cb, void *arg)
{
    char *e;

    if (id != -1) {
        e = gv_GetEffect(id);
        if (e != 0) {
            if (cb != 0) {
                *(void **)(e + 0x168) = cb;
                *(void **)(e + 0x16c) = arg;
                return;
            }
            *(void **)(e + 0x168) = gv_EffectNotifyOwner;
            *(char **)(e + 0x16c) = (char *)gv_EffectArgs + ((e - gv_Effects) / 0x170) * 0x38;
        }
    }
}
/* 0x4eef50, the charge a salvo of c balls (1 .. 3) takes off its effect slot (gv_FireShots' v, am.c): the sum of the c terms t, t / 2,
   t / 4 with t = (a + 1) * (b * 0.25 + 25) * 0.002 + 0.1, a (the cannon's kind) clamped to 0 .. 4 and b to 0 .. 100 */

float gv_SalvoCharge(int a, int b, int c)
{
    int k, lvl;
    float bt, sum, t;

    k = max(min(a, 4), 0) + 1;
    lvl = max(min(b, 100), 0);
    bt = (float)lvl * 0.25f + 25.0f;
    c = max(min(c, 3), 1);
    t = (float)k * bt * 0.002f + 0.1f;
    sum = 0.0f;
    for (; c > 0; c--) {
        sum = sum + t;
        t = t * 0.5f;
    }
    return sum;
}
/* 0x4ef000, the stats of the cannon kind n (clamped 0 .. 4) into out (am.c G9KIND): out[0] = n, out[1] = n + 2 (the level the
   building needs, am.c 0x4a67a0), and for the salvos of i = 1 .. 3 balls out[1 + i] = 500 * 32 * i * t and out[4 + i] =
   700 * 32 * i * t as ints, t = n * 0.25 + 1 (the products go through v: in one expression VC6 folds 500 * 32) */

void gv_CannonKind(int n, int *out)
{
    float t, a, v;
    int kind, i;

    kind = max(min(n, 4), 0);
    out[0] = kind;
    out[1] = kind + 2;
    t = (float)kind * 0.25f + 1.0f;
    for (i = 1; i <= 3; i++) {
        a = (float)i * t;
        v = 500.0f * a;
        out[1 + i] = (int)(v * 32.0f);
        v = a * 700.0f;
        out[4 + i] = (int)(v * 32.0f);
    }
}
/* 0x4ef090, prices a cannon salvo for the character o: the cannon kind a (clamped 0 .. 4) gives the level *n = a + 2 it needs,
   the cost *price = (2 * b + 500) * (a * 0.25 + 1) * d * 32 from the strength b (0 .. 100) and the balls d (1 .. 3);
   1 when o's money reaches *n and its inventory's worth (gm_SumListAmounts) covers the cost */

int gv_CannonCost(char *o, int a, int b, int d, int *n, int *price)
{
    AICHAR *ch = (AICHAR *)o;
    int kind, lvl, balls;
    float fl;

    kind = max(min(a, 4), 0);
    lvl = max(min(b, 100), 0);
    balls = max(min(d, 3), 1);
    *n = kind + 2;
    fl = (float)lvl;
    *price = (int)((fl + fl + 500.0f) * ((float)kind * 0.25f + 1.0f) * balls * 32.0f);
    if (ch->money >= *n && gm_SumListAmounts(ch->inventory, ai_OwnPlayer) >= *price)
        return 1;
    return 0;
}
/* 0x4ef170, the cannon the building o has by the items in its object list (gm_Query(objList, 2, 6, 0, id)): *balls = 2 with item
   0x265, else 1 with 0x263, else 0 (the salvo has *balls + 1 balls, am.c 0x4a67a0); *kind = 4 with 0x264, 3 with 0x262, 2 with
   0x25e, 1 with 0x25a, else 0 (the cannon kind of gv_CannonKind / gv_CannonCost) */

void gv_BuildingCannon(char *o, unsigned *balls, unsigned *kind)
{
    AIBLD *bld = (AIBLD *)o;

    *kind = 0;
    *balls = 0;
    if (gm_Query(bld->objList, 2, 6, 0, 0x265) != 0)
        *balls = 2;
    else
        *balls = (gm_Query(bld->objList, 2, 6, 0, 0x263) != 0);
    if (gm_Query(bld->objList, 2, 6, 0, 0x264) != 0) {
        *kind = 4;
        return;
    }
    if (gm_Query(bld->objList, 2, 6, 0, 0x262) != 0) {
        *kind = 3;
        return;
    }
    if (gm_Query(bld->objList, 2, 6, 0, 0x25e) != 0) {
        *kind = 2;
        return;
    }
    *kind = (gm_Query(bld->objList, 2, 6, 0, 0x25a) != 0);
}
/* 0x4ef260, the scatter of the building o's cannon (am.c 0x4a6ae0 passes them to ai_ScatterShot): *spread = 0.02 and
   *speedSpread = 0.2 (0.017 / 0.17 with item 0x25c in its object list), minus 0.003 / 0.03 with item 0x25f and
   minus 0.004 / 0.04 with item 0x260 */

void gv_CannonScatter(char *o, float *spread, float *speedSpread)
{
    AIBLD *bld = (AIBLD *)o;
    float x, y;

    x = 0.02f;
    y = 0.2f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x25c) != 0) {
        x = 0.0169999991f;              /* the original's constant is 0x3c8b4395, one ulp below 0.017f */
        y = 0.17f;
    }
    if (gm_Query(bld->objList, 2, 6, 0, 0x25f) != 0) {
        x = x - 0.003f;
        y = y - 0.03f;
    }
    if (gm_Query(bld->objList, 2, 6, 0, 0x260) != 0) {
        x = x - 0.004f;
        y = y - 0.04f;
    }
    *spread = x;
    *speedSpread = y;
}
/* 0x4ef330, a factor of the object o: 1.0 (1.05 with item 0x259), plus 0.05 with item 0x25b and plus 0.05 with item 0x261 */

float unk_Unk4ef330(char *o)
{
    float x;

    x = 1.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x259) != 0)
        x = 1.05f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x25b) != 0)
        x = x + 0.05f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x261) != 0)
        return x + 0.05f;
    return x;
}
/* 0x4ef3c0, the number of small (0x44-byte) graves of the graveyard building o by its type's level (AIBTYPE +0x247) and the
   sizes in use (GVYARD.open): level 1: 8, 16 with open[1]; level 2: 12 / 24 with open[1], + 8 with open[2]; else 16 / 32 with
   open[1], + 16 with open[2], + 8 with open[3] */

int gv_GraveCount(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    char *bt;
    GVYARD *g;
    int n;

    bt = ai_BTypes + bld->type + bld->type * 0x24c;
    g = bld->yard;
    if (((AIBTYPE *)bt)->level == 1) {
        n = 8;
        if (g->open[1] == 1)
            return 0x10;
    } else if (((AIBTYPE *)bt)->level == 2) {
        n = 0xc;
        if (g->open[1] == 1)
            n = 0x18;
        if (g->open[2] == 1)
            n = n + 8;
    } else {
        n = 0x10;
        if (g->open[1] == 1)
            n = 0x20;
        if (g->open[2] == 1)
            n = n + 0x10;
        if (g->open[3] == 1)
            n = n + 8;
    }
    return n;
}
/* 0x4ef440, how many of the slots (0x4ef3c0 of them, 0x44 bytes each from the table at +0x20 of the record) are in use (first word not -1) */

int unk_Unk4ef440(unsigned char *o)
{
    int n, cnt;
    int *p;

    n = gv_GraveCount(o);
    cnt = 0;
    if (n == 0)
        return 0;
    if (n > 0) {
        p = *(int **)(*(char **)(o + 0x65) + 0x20);
        do {
            if (*p != -1)
                cnt++;
            p += 0x11;
            n--;
        } while (n != 0);
    }
    return cnt;
}
/* 0x4ef470, as 0x4ef3c0 with the sizes 8 / 12, 12 / 18 + 2, 16 / 24 + 4 + 2 */

int unk_Unk4ef470(unsigned char *o)
{
    char *rec;
    char k, f;
    int n;

    rec = *(char **)(o + 0x65);
    f = rec[0x19];
    k = ai_BTypes[o[0] * 0x24d + 0x247];
    if (k == 1) {
        n = 8;
        if (f == 1)
            return 0xc;
    } else {
        if (k == 2) {
            n = 0xc;
            if (f == 1)
                n = 0x12;
            f = rec[0x1a];
        } else {
            n = 0x10;
            if (f == 1)
                n = 0x18;
            if (rec[0x1a] == 1)
                n = n + 4;
            f = rec[0x1b];
        }
        if (f == 1)
            n = n + 2;
    }
    return n;
}
/* 0x4ef4f0, 1 unless the part i (0 .. 3) of the record r is missing (the pointer at +8 + 4 * i is 0 or the flag byte at +0x18 + i is 0) */

int unk_Unk4ef4f0(char *r, int i)
{
    if (i == 0) {
        if (*(int *)(r + 8) == 0 || r[0x18] == 0)
            return 0;
    } else if (i == 1) {
        if (*(int *)(r + 0xc) == 0 || r[0x19] == 0)
            return 0;
    } else if (i == 2) {
        if (*(int *)(r + 0x10) == 0 || r[0x1a] == 0)
            return 0;
    } else if (i == 3 && (*(int *)(r + 0x14) == 0 || r[0x1b] == 0))
        return 0;
    return 1;
}
/* 0x4ef570, the entry n (0x14 bytes) of the list i (0 .. 3: the pointer at +8 + 4 * i of the record r) when the list exists (0x4ef4f0) and the level (+0x18 + i; here read from the flag byte, the original takes it from edx) allows
   that many entries; 0 when r[1] is not 1 .. 3 or n is out of range. ROUGH */

int unk_Unk4ef570(char *r, int i, unsigned char n)
{
    int lv;

    if (*(int *)(r + 4) > 0 && *(int *)(r + 4) < 4) {
        if (i == 0) {
            lv = r[0x18];
            if (unk_Unk4ef4f0(r, 0) != 0) {
                if ((lv != 1 || n < 8) && (lv != 2 || n < 0xc) && (lv != 3 || n < 0x10))
                    return *(int *)(r + 8) + n * 0x14;
            }
        } else if (i == 1) {
            lv = r[0x19];
            if (unk_Unk4ef4f0(r, 1) != 0) {
                if ((lv != 1 || n < 4) && (lv != 2 || n < 6) && (lv != 3 || n < 8))
                    return *(int *)(r + 0xc) + n * 0x14;
            }
        } else if (i == 2) {
            lv = r[0x1a];
            if (unk_Unk4ef4f0(r, 2) != 0 && lv != 1) {
                if ((lv != 2 || n < 2) && (lv != 3 || n < 4))
                    return *(int *)(r + 0x10) + n * 0x14;
            }
        } else if (i == 3) {
            lv = r[0x1b];
            if (unk_Unk4ef4f0(r, 3) != 0 && lv != 1 && lv != 2 && (lv != 3 || n < 2))
                return *(int *)(r + 0x14) + n * 0x14;
        }
    }
    return 0;
}
/* 0x4ef6b0, the index of the entry e (0x14 bytes, e[0] the list number 0 .. 3) within its list of the record r; -1 for another list */

int gv_EntryIndex(char *r, int *e)
{
    int base;

    base = e[0];
    if (base == 0)
        base = *(int *)(r + 8);
    else if (base == 1)
        base = *(int *)(r + 0xc);
    else if (base == 2)
        base = *(int *)(r + 0x10);
    else {
        if (base != 3)
            return -1;
        base = *(int *)(r + 0x14);
    }
    return ((int)e - base) / 0x14;
}
/* 0x4ef710, 1 when the +8 word of b is the +0x50 word of a and not 0x7fff */

int unk_Unk4ef710(char *a, char *b)
{
    short v;

    v = *(short *)(a + 0x50);
    if (*(short *)(b + 8) == v && v != 0x7fff)
        return 1;
    return 0;
}
/* 0x4ef740, 1 when the +4 words of a and b are equal */

int gv_EntrySameId(int *a, int *b)
{
    return b[1] == a[1];
}
/* 0x4ef760, 1 when the +4 word of b is -1 */

int gv_EntryIsFree(int *a, int *b)
{
    return b[1] == -1;
}
/* 0x4ef770, 1 when the +4 word of b is not -1 */

int gv_EntryIsUsed(int *a, int *b)
{
    return b[1] != -1;
}
/* 0x4ef780, 1 when the id a[0] is in the table (+0x10, 0x44 bytes per entry, gv_EntryCounts[b[0]] of them) of the entry b */

int gv_EntryHasId(int *a, int *b)
{
    int i, n;
    int *p;

    i = 0;
    n = gv_EntryCounts[b[0]];
    if (n > 0) {
        p = (int *)b[4];
        do {
            if (*p == a[0])
                return 1;
            i++;
            p += 0x11;
        } while (i < n);
    }
    return 0;
}
/* 0x4ef7c0, the first entry (0x14 bytes) of the list l (0 .. 3, 0xff = the list at +0x1c) of the record of the object o, after the entry `after` (0: from the start), for which cb (a, entry) is true (no cb: any);
   the list ends at the table limit unk_Tab5ee084[l][kind] (0xff: 0x4ef470 entries); 0 when none or the list is missing (0x4ef4f0) */

unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l)
{
    char *rec;
    char *b;
    unsigned end, cur;
    int n;

    b = ai_BTypes + o[0] * 0x24d;
    rec = *(char **)(o + 0x65);
    if (l != 0xff) {
        if (unk_Unk4ef4f0(rec, l) == 0)
            return 0;
    }
    if (l == 0)
        end = *(int *)(rec + 8) + unk_Tab5ee084[0][(unsigned char)b[0x247]] * 0x14;
    else if (l == 1)
        end = *(int *)(rec + 0xc) + unk_Tab5ee084[1][(unsigned char)b[0x247]] * 0x14;
    else if (l == 2)
        end = *(int *)(rec + 0x10) + unk_Tab5ee084[2][(unsigned char)b[0x247]] * 0x14;
    else if (l == 3)
        end = *(int *)(rec + 0x14) + unk_Tab5ee084[3][(unsigned char)b[0x247]] * 0x14;
    else {
        if (l != 0xff)
            return 0;
        n = unk_Unk4ef470(o);
        end = *(int *)(rec + 0x1c) + n * 0x14;
    }
    if (after == 0) {
        if (l == 0)
            cur = *(unsigned *)(rec + 8);
        else if (l == 1)
            cur = *(unsigned *)(rec + 0xc);
        else if (l == 2)
            cur = *(unsigned *)(rec + 0x10);
        else if (l == 3)
            cur = *(unsigned *)(rec + 0x14);
        else
            cur = *(unsigned *)(rec + 0x1c);
    } else
        cur = after + 0x14;
    while (1) {
        if (end <= cur)
            return 0;
        if (cb == 0 || cb(a, cur) != 0)
            break;
        cur += 0x14;
    }
    return cur;
}
/* 0x4ef900, the first entry of the list 0xff of the object o whose +4 word is the callback's (0x4ef780) match for the entry e; 0 when e is 0 or e[0] is -1 */

unsigned gv_LoadGraveyard(unsigned char *o, int *e)
{
    if (e != 0 && e[0] != -1)
        return gv_FindEntry(e, o, 0, gv_EntryHasId, 0xff);
    return 0;
}
/* 0x4ef930, a random used slot (first word not -1; 0x44 bytes each at the table +0x20 of the record, 0x4ef3c0 of them, from a random start, cyclic) of the object o of the building kind 0x1d with the kind byte 1 .. 3; 0 when none */

int unk_Unk4ef930(unsigned char *o)
{
    char *rec;
    char *b;
    int n, k;
    unsigned i;

    rec = *(char **)(o + 0x65);
    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] == 0x1d && (unsigned char)b[0x247] != 0 && (unsigned char)b[0x247] < 4) {
        n = gv_GraveCount(o);
        i = util_RandomU(n) & 0xffff;
        if (n != 0) {
            k = n;
            do {
                if (*(int *)(*(char **)(rec + 0x20) + i * 0x44) != -1)
                    return (int)(*(char **)(rec + 0x20) + i * 0x44);
                k--;
                i = (int)(i + 1) % n;
            } while (k != 0);
        }
    }
    return 0;
}
/* 0x4ef9c0, fills n (up to the free ones) free entries of the list 0 (entry id +4 = -1; 0x4ef760) of the graveyard of the object o (building kind 0x1d, kind byte 1 .. 3) with new dead persons: id from
   ai_NextObjectId, a record (+0x10: 0x44 bytes) with five random bytes (+0x24 .. : rand(8) * 21 + 42), age fields, a random name pair (0x143ac5c / 0x143af70 and 0x143b178) and the grave kind (0x4fb330); returns how many. ROUGH */

int unk_Unk4ef9c0(unsigned char *o, int n)
{
    unsigned char kind;
    char *e;
    char *z;
    char *g;
    int i, cnt, k;
    unsigned r;

    kind = (unsigned char)ai_BTypes[o[0] + o[0] * 0x24c + 0x247];
    if (ai_BTypes[o[0] + o[0] * 0x24c] != 0x1d || kind == 0 || kind > 3)
        return 0;
    i = 0;
    cnt = 0;
    if (n > 0) {
        do {
            e = (char *)gv_FindEntry(0, o, 0, gv_EntryIsFree, 0);
            if (e == 0)
                return cnt;
            z = *(char **)(e + 0x10);
            *(int *)z = ai_NextObjectId;
            ai_NextObjectId++;
            *(int *)(e + 4) = *(int *)z;
            *(short *)(e + 8) = 0x7fff;
            e[0xc] = (char)0xff;
            *(short *)(e + 10) = (short)(*(int *)ai_Time + 5);
            for (k = 0; k < 5; k++) {
                z[0x24 + k] = (char)(int)((float)util_RandomU(8) * 21.0f + 42.0f);
            }
            *(int *)(z + 0x2c) = util_RandomU(9) + 1;
            *(int *)(z + 0x30) = util_RandomU(2) + 1;
            *(int *)(z + 0x34) = util_RandomU(0x5e) + 1;
            *(int *)(z + 0x40) = *(int *)ai_Time - util_RandomU(4) - 1;
            *(int *)(z + 0x38) = util_RandomU(2);
            strncpy(z + 0x14, txt_FamilyNames[util_RandomU(0x95)], 0xf);
            g = (char *)unk_Unk4fb330(*(int *)(z + 0x38), *(int *)(z + 0x34));
            *(int *)(z + 0x3c) = 0xff;
            r = util_RandomU(4);
            k = 4;
            do {
                if (*(int *)(g + 0x34 + r * 4) != 0xff) {
                    *(int *)(z + 0x3c) = *(int *)(g + 0x34 + r * 4) + 0x639;
                    break;
                }
                k--;
                r = (r + 1) & 3;
            } while (k != 0);
            if (*(int *)(z + 0x38) == 0)
                strncpy(z + 4, txt_MaleFirstNames[util_RandomU(0xbf)], 0xf);
            else
                strncpy(z + 4, txt_FemaleFirstNames[util_RandomU(0x70)], 0xf);
            cnt++;
            i = cnt;
        } while (cnt < n);
    }
    return i;
}
/* 0x4efc10, allocates the graveyard record of the object o (0x24 bytes, "gv_AllocGraveyardField"; kind 1 .. 3 of the building kind 0x1d): +0 the object id, +4 the kind, +8 the entries (0x14 bytes: the list number,
   id -1, short 0xffff, the record at +0x10) of 8 / 12 / 16 small (0x44 bytes) graves, then 4 / 6 / 8 of 0x88, 2 / 2 / 4 of 0x110 and (kind 3) 2 of 0x220 bytes, the entry tables +0xc / +0x10 / +0x14 for the
   larger lists, the graves +0x1c ("gv_Alloc:_all_graves") and zombies +0x20 ("gv_Alloc:_all_zombies"); stored at +0x65 of o; 0 when out of memory. ROUGH */

int *gv_AllocGraveyard(unsigned char *o)
{
    int *g;
    int *p;
    int *recs;
    char *b;
    int cnt[4];
    int nl, i, j, n, total;

    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] == 0x1d && (unsigned char)b[0x247] != 0 && (unsigned char)b[0x247] < 4
        && (g = (int *)mem_Alloc(0x24, "gv_AllocGraveyardField")) != 0) {
        memset(g, 0, 0x24);
        g[0] = *(int *)(o + 1);
        g[1] = (unsigned char)b[0x247];
        g[7] = (int)mem_Alloc(unk_Tab5ee064[(unsigned char)b[0x247]] * 0x14, "gv_Alloc:_all_graves");
        if (g[7] != 0) {
            memset((void *)g[7], 0, unk_Tab5ee064[(unsigned char)b[0x247]] * 0x14);
            g[8] = (int)mem_Alloc(unk_Tab5ee074[(unsigned char)b[0x247]] * 0x44, "gv_Alloc:_all_zombies");
            if (g[8] != 0) {
                memset((void *)g[8], 0, unk_Tab5ee074[(unsigned char)b[0x247]] * 0x44);
                for (i = 0; i < unk_Tab5ee074[(unsigned char)b[0x247]]; i++)
                    *(int *)((char *)g[8] + i * 0x44) = -1;
                p = (int *)g[7];
                recs = (int *)g[8];
                g[2] = (int)p;
                *(char *)(g + 6) = 1;
                *((char *)g + 0x19) = 0;
                *((char *)g + 0x1a) = 0;
                *((char *)g + 0x1b) = 0;
                if (g[1] == 1) {
                    cnt[0] = 8; cnt[1] = 4; cnt[2] = 0; cnt[3] = 0;
                    nl = 2;
                    g[4] = 0;
                    g[5] = 0;
                } else if (g[1] == 2) {
                    cnt[0] = 12; cnt[1] = 6; cnt[2] = 2; cnt[3] = 0;
                    nl = 3;
                    g[5] = 0;
                } else {
                    cnt[0] = 16; cnt[1] = 8; cnt[2] = 4; cnt[3] = 2;
                    nl = 4;
                }
                total = 0;
                for (j = 0; j < nl; j++) {
                    if (j >= 1)
                        g[2 + j] = (int)(p + total * 5);
                    n = cnt[j];
                    for (i = 0; i < n; i++) {
                        p[0] = j;
                        p[4] = (int)recs;
                        p[1] = -1;
                        *(short *)(p + 2) = (short)0xffff;
                        recs = (int *)((char *)recs + (0x44 << j));
                        p += 5;
                    }
                    total += n;
                }
                *(int **)(o + 0x65) = g;
                return g;
            }
        }
        if (g[7] != 0) {
            mem_Free((void *)g[7]);
            g[7] = 0;
        }
        if (g[8] != 0) {
            mem_Free((void *)g[8]);
            g[8] = 0;
        }
        mem_Free(g);
    }
    return 0;
}
/* 0x4eff50, upgrades the graveyard record of the object o to the next size (kind - 1 .. : the building's kind byte is one more than the record's +4): allocates a new one (0x4efc10), copies the first grave
   records, the entries of the lists and the four flag bytes (+0x18 .. +0x1b) over, frees the old one (0x4f0180); returns the new record, or 0 (the old one stays) when it does not fit. ROUGH */

int unk_Unk4eff50(unsigned char *o)
{
    int *old, *nw;
    char *b;
    int *s, *d;
    int i, n0, n1, n2, k;

    old = *(int **)(o + 0x65);
    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] == 0x1d && old != 0 && old[1] < 3 && (unsigned char)b[0x247] - old[1] == 1) {
        *(int *)(o + 0x65) = 0;
        nw = gv_AllocGraveyard(o);
        if (nw != 0) {
            s = *(int **)(old[2] + 0x10);
            d = *(int **)(nw[2] + 0x10);
            if ((unsigned char)b[0x247] == 2) {
                k = 0;
                n0 = 8;
                n1 = 4;
                for (i = 0x88; i != 0; i--)
                    *d++ = *s++;
                s = *(int **)(old[3] + 0x10);
                d = *(int **)(nw[3] + 0x10);
            } else {
                k = 2;
                n0 = 0xc;
                n1 = 6;
                for (i = 0xcc; i != 0; i--)
                    *d++ = *s++;
                s = *(int **)(old[3] + 0x10);
                d = *(int **)(nw[3] + 0x10);
                for (i = 0xcc; i != 0; i--)
                    *d++ = *s++;
                s = *(int **)(old[4] + 0x10);
                d = *(int **)(nw[4] + 0x10);
            }
            for (i = 0x88; i != 0; i--)
                *d++ = *s++;
            if (old[2] != 0 && n0 != 0) {
                for (i = 0; n0 != 0; n0--, i += 0x14) {
                    *(int *)(i + nw[2]) = *(int *)(i + old[2]);
                    *(int *)(nw[2] + 4 + i) = *(int *)(i + 4 + old[2]);
                    *(short *)(nw[2] + 8 + i) = *(short *)(i + 8 + old[2]);
                    *(short *)(nw[2] + 10 + i) = *(short *)(i + 10 + old[2]);
                    *(char *)(nw[2] + 0xc + i) = *(char *)(i + 0xc + old[2]);
                }
            }
            if (old[3] != 0 && n1 != 0) {
                for (i = 0; n1 != 0; n1--, i += 0x14) {
                    *(int *)(i + nw[3]) = *(int *)(i + old[3]);
                    *(int *)(i + 4 + nw[3]) = *(int *)(i + 4 + old[3]);
                    *(short *)(i + 8 + nw[3]) = *(short *)(i + 8 + old[3]);
                    *(short *)(i + 10 + nw[3]) = *(short *)(i + 10 + old[3]);
                    *(char *)(i + 0xc + nw[3]) = *(char *)(i + 0xc + old[3]);
                }
            }
            if (old[4] != 0 && k != 0) {
                for (i = 0; k != 0; k--, i += 0x14) {
                    *(int *)(i + nw[4]) = *(int *)(i + old[4]);
                    *(int *)(i + 4 + nw[4]) = *(int *)(i + 4 + old[4]);
                    *(short *)(i + 8 + nw[4]) = *(short *)(i + 8 + old[4]);
                    *(short *)(i + 10 + nw[4]) = *(short *)(i + 10 + old[4]);
                    *(char *)(i + 0xc + nw[4]) = *(char *)(i + 0xc + old[4]);
                }
            }
            *((char *)nw + 0x18) = *((char *)old + 0x18);
            *((char *)nw + 0x19) = *((char *)old + 0x19);
            *((char *)nw + 0x1a) = *((char *)old + 0x1a);
            *((char *)nw + 0x1b) = *((char *)old + 0x1b);
            gv_FreeGraveyard(old);
            return *(int *)(o + 0x65);
        }
        *(int **)(o + 0x65) = old;
    }
    return 0;
}
/* 0x4f0180, frees a graveyard record g: its graves (+0x1c), zombies (+0x20) and itself */

void gv_FreeGraveyard(int *g)
{
    if (g != 0) {
        if (g[7] != 0) {
            mem_Free((void *)g[7]);
            g[7] = 0;
        }
        if (g[8] != 0) {
            mem_Free((void *)g[8]);
            g[8] = 0;
        }
        mem_Free(g);
    }
}
/* 0x4f01d0, the message 0x4c60b0 type 0 (graveyard id +4 of g, object id +1 of o, value v, byte b); -1 when g or o is 0 or b is -1 */

int unk_Unk4f01d0(int *g, char *o, int v, int b)
{
    char m[0x1d];

    if (g != 0 && o != 0 && b != -1) {
        *(int *)(m + 4) = g[1];
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)m = 0;
        m[0x10] = (char)b;
        *(int *)(m + 0xc) = v;
        *(short *)(m + 0x11) = 0;
        *(int *)(m + 0x13) = 0;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
/* 0x4f0240, the graveyard order of the message p (+4 a character, +8 an object id, +0xc the list, +0x10 the entry): the object's graveyard gets a new record of the dead character (names +0x30 / +0x40, +0x80 .., age
   and skills; time 0x770ae0); returns 0, or a negative reason (-1 no object / own object, -2 not a graveyard, -3 no record, -4 no list, -6 no character, -7 the entry does not fit). ROUGH */

int unk_Unk4f0240(int *p)
{
    unsigned char *o;
    char *c;
    int *rec, *e, *z, *best, *q;
    int *lst;
    int i, n, list;
    char *s, *d;
    char ch;

    gm_FindById(&o, 0, 0, p[2]);
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(int **)(o + 0x65);
    if (rec == 0)
        return -3;
    if (unk_Unk4ef4f0((char *)rec, p[3]) == 0)
        return -4;
    gm_FindById(0, 0, &c, p[1]);
    if (c == 0)
        return -6;
    list = p[3];
    e = (int *)unk_Unk4ef570((char *)rec, list, (unsigned char)p[4]);
    if (e == 0 || *(short *)((char *)e + 8) != *(short *)(c + 0x50))
        return -7;
    lst = (int *)e[4];
    i = 0;
    best = lst;
    q = lst;
    if (gv_EntryCounts[list] > 0) {
        do {
            if (*best == -1)
                break;
            if (*q == -1) {
                best = lst + i * 0x11;
                break;
            }
            if (q[0x10] < best[0x10])
                best = q;
            i++;
            q += 0x11;
        } while (i < gv_EntryCounts[list]);
    }
    *best = *(int *)(c + 4);
    s = c + 0x30;
    d = (char *)(best + 1);
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    s = c + 0x40;
    d = (char *)(best + 5);
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    best[9] = *(int *)(c + 0x80);
    *(char *)(best + 10) = c[0x84];
    best[0xb] = (unsigned char)c[0x167];
    best[0xc] = (unsigned char)c[0xd];
    best[0xd] = (unsigned char)c[0x164];
    best[0xe] = (unsigned char)c[9];
    best[0x10] = *(int *)ai_Time;
    best[0xf] = *(int *)(c + 0x18c);
    return 0;
}
/* 0x4f0430, the message 0x4c60b0 type 1 (graveyard id +4 of g, object id +1 of o, value v, short s, int x, byte 0xff); -1 when g or o is 0 */

int unk_Unk4f0430(int *g, char *o, int v, short s, int x)
{
    char m[0x1d];

    if (g != 0 && o != 0) {
        *(int *)(m + 4) = g[1];
        *(int *)(m + 0xc) = v;
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)m = 1;
        m[0x10] = (char)0xff;
        *(short *)(m + 0x11) = s;
        *(int *)(m + 0x13) = x;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
/* 0x4f0490, graveyard order type 0 (p: +8 the object id, +0xc the list, +0x10 / +0x11 / +0x13 values): the first free entry (+4 = -1) of the list gets the character p[+4]'s id, its short +0x50 and the time
   +0x11 + 0x770ae0; its record is cleared (ids -1); with a price (+0x13 > 0) the owner's money changes (0x500ae0 / 0x500970) and a player character (kind 6) is told (0x5a96e0). The result 0, or a negative reason
   (as 0x4f0240; -7 no free entry, -5 list out of range). ROUGH (the floats of the __ftol calls are lost) */

int unk_Unk4f0490(short *p)
{
    unsigned char *o;
    char *c;
    int *e;
    int i, n;
    int ow;
    unsigned short ch;

    gm_FindById(&o, 0, 0, *(int *)(p + 4));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), *(int *)(p + 6)) == 0)
        return -4;
    gm_FindById(0, 0, &c, *(int *)(p + 2));
    if (c == 0)
        return -6;
    if (*(int *)(p + 6) >= 0 && *(int *)(p + 6) < 4) {
        e = (int *)gv_FindEntry(c, o, 0, gv_EntryIsFree, *(int *)(p + 6));
        if (e == 0)
            return -7;
        e[1] = *(int *)(c + 4);
        *(short *)(e + 2) = *(short *)(c + 0x50);
        *(short *)((char *)e + 10) = *(short *)((char *)p + 0x11) + (short)*(int *)ai_Time;
        *((char *)e + 0xc) = (char)0xff;
        i = 0;
        if (gv_EntryCounts[*(int *)(p + 6)] > 0) {
            n = 0;
            do {
                i++;
                *(int *)(n + e[4]) = -1;
                n += 0x44;
            } while (i < gv_EntryCounts[*(int *)(p + 6)]);
        }
        if (*(int *)((char *)p + 0x13) > 0) {
            ow = *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8);
            gm_AddNegative(*(int *)(c + 4), ow, *(int *)((char *)p + 0x13));
            gm_AddObjekt(*(int *)(ai_Chars[*(unsigned short *)(o + 0x27)] + 4), ow, 0);
            ch = *(unsigned short *)(o + 0x27);
            if (c != ai_Chars[ch] && ai_Chars[ch][2] == 6)
                ai_Unk5a96e0(ai_Chars[ch], c, o, *(int *)(p + 6), *(int *)((char *)p + 0x13), 0);
        }
        return 0;
    }
    return -5;
}
/* 0x4f0700, the message 0x4c60b0 type 2 (graveyard id +4 of g, object id +1 of o, value v, byte b, short s, int x); -1 when g or o is 0 or b is -1 */

int unk_Unk4f0700(int *g, char *o, int v, int b, short s, int x)
{
    char m[0x1d];

    if (g != 0 && o != 0 && b != -1) {
        *(int *)(m + 4) = g[1];
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)(m + 0xc) = v;
        m[0x10] = (char)b;
        *(int *)m = 2;
        *(short *)(m + 0x11) = s;
        *(int *)(m + 0x13) = x;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
/* 0x4f0770, graveyard order type 2 (p: +4 a character id, +8 the object id, +0xc the list, +0x10 the entry, +0x11 a time to add, +0x13 a price): adds the time to the entry's short +10; with a price the
   owner's money changes (0x500ae0 / 0x500970); 0 or a negative reason (as 0x4f0240). ROUGH */

int unk_Unk4f0770(char *p)
{
    unsigned char *o;
    int *c;
    char *rec;
    short *e;
    int ow;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(char **)(o + 0x65);
    if (rec == 0)
        return -3;
    if (unk_Unk4ef4f0(rec, *(int *)(p + 0xc)) == 0)
        return -4;
    gm_FindById(0, 0, &c, *(int *)(p + 4));
    if (c == 0)
        return -6;
    e = (short *)unk_Unk4ef570(rec, *(int *)(p + 0xc), (unsigned char)p[0x10]);
    if (e == 0)
        return -7;
    e[5] = e[5] + *(short *)(p + 0x11);
    if (*(int *)(p + 0x13) > 0) {
        ow = *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8);
        gm_AddNegative(c[1], ow, *(int *)(p + 0x13));
        gm_AddObjekt(*(int *)(ai_Chars[*(unsigned short *)(o + 0x27)] + 4), ow, 0);
    }
    return 0;
}
/* 0x4f0900, the message 0x4c60b0 type 3 (graveyard id +4 of g or -1, object id +1 of o, value v, byte b); -1 when o is 0 or b is -1 */

int unk_Unk4f0900(int *g, char *o, int v, int b)
{
    char m[0x1d];

    if (o != 0 && b != -1) {
        if (g == 0)
            *(int *)(m + 4) = -1;
        else
            *(int *)(m + 4) = g[1];
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)m = 3;
        *(int *)(m + 0xc) = v;
        m[0x10] = (char)b;
        *(short *)(m + 0x11) = 0;
        *(int *)(m + 0x13) = 0;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
/* 0x4f0970, graveyard order type 3 (p: +4 a character id or -1, +8 the object id, +0xc the list, +0x10 the entry): sets the entry's id (+4) and short (+8) to the character's (-1 / 0xffff when none);
   0 or a negative reason (as 0x4f0240). ROUGH */

int unk_Unk4f0970(unsigned char *p)
{
    unsigned char *o;
    int *c;
    char *rec;
    int id;
    short sh;
    char *e;

    id = -1;
    sh = (short)0xffff;
    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(char **)(o + 0x65);
    if (rec == 0)
        return -3;
    if (unk_Unk4ef4f0(rec, *(int *)(p + 0xc)) == 0)
        return -4;
    if (*(int *)(p + 4) != -1) {
        gm_FindById(0, 0, &c, *(int *)(p + 4));
        if (c == 0)
            return -6;
        id = *(int *)(p + 4);
        sh = *(short *)((char *)c + 0x50);
    }
    e = (char *)unk_Unk4ef570(rec, *(int *)(p + 0xc), p[0x10]);
    if (e == 0)
        return -7;
    *(short *)(e + 8) = sh;
    *(int *)(e + 4) = id;
    return 0;
}
/* 0x4f0a90, the message 0x4c60b0 type 4 (graveyard id +4 of g or -1, object id +1 of o, value v); -1 when o is 0 */

int unk_Unk4f0a90(int *g, char *o, int v)
{
    char m[0x1d];

    if (o == 0)
        return -1;
    if (g == 0)
        *(int *)(m + 4) = -1;
    else
        *(int *)(m + 4) = g[1];
    *(int *)(m + 8) = *(int *)(o + 1);
    *(int *)m = 4;
    *(int *)(m + 0xc) = v;
    m[0x10] = 0;
    *(short *)(m + 0x11) = 0;
    *(int *)(m + 0x13) = 0;
    return ((int (*)(void *))ai_Unk4c60b0)(m);
}
/* 0x4f0af0, graveyard order type 4 (p: +4 a character id, +8 the object id, +0xc the list 1 .. 3): switches on the list for the record rec (flags +0x19 / +0x1a / +0x1b = 1 for the lists up to it); 0 or a negative reason
   (-8 other list, -7 the list is missing or the kind too small, as 0x4f0240 otherwise) */

int unk_Unk4f0af0(char *p)
{
    unsigned char *o;
    char *rec;
    int *c;
    int l;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    rec = *(char **)(o + 0x65);
    if (rec == 0)
        return -3;
    gm_FindById(0, 0, &c, *(int *)(p + 4));
    if (c == 0)
        return -6;
    l = *(int *)(p + 0xc);
    if (l == 3) {
        if (*(int *)(rec + 0x14) != 0 && *(int *)(rec + 4) > 2) {
            rec[0x1b] = 1;
            rec[0x1a] = 1;
            rec[0x19] = 1;
            return 0;
        }
    } else if (l == 2) {
        if (*(int *)(rec + 0x10) != 0 && *(int *)(rec + 4) > 1) {
            rec[0x1a] = 1;
            rec[0x19] = 1;
            return 0;
        }
    } else {
        if (l != 1)
            return -8;
        if (*(int *)(rec + 0xc) != 0) {
            rec[0x19] = 1;
            return 0;
        }
    }
    return -7;
}
/* 0x4f0c20, the message 0x4c60b0 type 5 (graveyard id -1, object id +1 of o, value v, byte b, int x); -1 when o is 0 or b is -1 */

int unk_Unk4f0c20(char *o, int v, int b, int x)
{
    char m[0x1d];

    if (o != 0 && b != -1) {
        *(int *)(m + 8) = *(int *)(o + 1);
        m[0x10] = (char)b;
        *(int *)m = 5;
        *(int *)(m + 4) = -1;
        *(int *)(m + 0xc) = v;
        *(short *)(m + 0x11) = 0;
        *(int *)(m + 0x13) = x;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
/* 0x4f0c80, graveyard order type 5 (p: +8 the object id, +0xc the list, +0x10 the entry, +0x13 a delta): adds the delta to the byte +0xc of the entry, clamped to 0 .. 255; 0 or a negative reason (as 0x4f0240) */

int unk_Unk4f0c80(unsigned char *p)
{
    unsigned char *o;
    int l, v;
    char *e;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    l = *(int *)(p + 0xc);
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), l) == 0)
        return -4;
    e = (char *)unk_Unk4ef570(*(char **)(o + 0x65), l, p[0x10]);
    if (e == 0)
        return -7;
    v = (unsigned char)e[0xc] + *(int *)(p + 0x13);
    if (v < 1)
        v = 0;
    if (v > 0xfe)
        v = 0xff;
    e[0xc] = (char)v;
    return 0;
}
/* 0x4f0d70, the message 0x4c60b0 type 6 (graveyard id -1, object id +1 of o, value x at +0x13); -1 when o is 0 */

int unk_Unk4f0d70(char *o, int x)
{
    char m[0x1d];

    if (o == 0)
        return -1;
    *(int *)(m + 0xc) = 0;
    m[0x10] = 0;
    *(short *)(m + 0x11) = 0;
    *(int *)m = 6;
    *(int *)(m + 4) = -1;
    *(int *)(m + 8) = *(int *)(o + 1);
    *(int *)(m + 0x13) = x;
    return ((int (*)(void *))ai_Unk4c60b0)(m);
}
/* 0x4f0dd0, graveyard order type 6 (p: +8 the object id, +0x13 the number): makes that many new dead (0x4ef9c0); 0 when some were made, else -4 */

int unk_Unk4f0dd0(unsigned char *p)
{
    unsigned char *o;
    int n;

    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    n = unk_Unk4ef9c0(o, *(int *)(p + 0x13));
    return (n > 0) - 1 & 0xfffffffc;
}
/* 0x4f0e80, the message 0x4c60f0 (g +4, o +1, v, byte b, int x at +0xd, int y at +0x11) */

void unk_Unk4f0e80(int *g, int x, char *o, int v, char b, int y)
{
    char m[0x19];

    *(int *)m = g[1];
    *(int *)(m + 4) = *(int *)(o + 1);
    m[0xc] = b;
    *(int *)(m + 8) = v;
    *(int *)(m + 0xd) = x;
    *(int *)(m + 0x11) = y;
    ai_Unk4c60f0(m);
}
/* 0x4f0ed0, graveyard order type 1 (p: +0 a character id, +4 the object id, +8 the source list, +0xc the entry, +0xd the target list, +0x11 a price, +0x15 a counter): moves the grave of the entry (its 0x44-byte
   records, compacting the free ones) from the source list to the first free entry of the target list, copies the character id and short, time +10 + 0x770ae0, clears the source; with a price the owner's money changes;
   0 or a negative reason (as 0x4f0240, -7 no entry / free entry). ROUGH */

int unk_Unk4f0ed0(int *p)
{
    unsigned char *o;
    int *c;
    char *rec;
    char *src, *dst;
    int *s, *s2, *d, *send, *dend, *q;
    int n, i, list;

    gm_FindById(0, 0, &c, p[0]);
    if (c == 0)
        return -6;
    gm_FindById(&o, 0, 0, p[1]);
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return -1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return -2;
    if (*(int *)(o + 0x65) == 0)
        return -3;
    list = p[2];
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), list) == 0)
        return -4;
    src = (char *)unk_Unk4ef570(*(char **)(o + 0x65), list, *(unsigned char *)(p + 3));
    if (src != 0 && *(short *)(src + 8) == *(short *)((char *)c + 0x50)) {
        s = *(int **)(src + 0x10);
        n = gv_EntryCounts[list];
        dst = (char *)gv_FindEntry(c, o, 0, gv_EntryIsFree, *(int *)((char *)p + 0xd));
        if (dst != 0) {
            d = *(int **)(dst + 0x10);
            dend = d + gv_EntryCounts[*(int *)((char *)p + 0xd)] * 0x11;
            send = s + n * 0x11;
            for (; d < dend; d += 0x11) {
                if (*d == -1) {
                    while (1) {
                        if (send <= s)
                            goto done;
                        if (*s != -1)
                            break;
                        s += 0x11;
                    }
                    s2 = s;
                    q = d;
                    for (i = 0x11; i != 0; i--)
                        *q++ = *s2++;
                    s2 = s;
                    for (i = 0x11; i != 0; i--)
                        *s2++ = 0;
                    *s = -1;
                    if (send <= s)
                        break;
                }
            }
        done:
            *(int *)(dst + 4) = *(int *)(src + 4);
            *(int *)(src + 4) = -1;
            *(short *)(dst + 8) = *(short *)(src + 8);
            *(short *)(src + 8) = (short)0xffff;
            *(short *)(dst + 10) = (short)*(int *)ai_Time + 10;
            dst[0xc] = (char)0xff;
            *(short *)(src + 10) = 0xff;
            if (ai_Unk1460ae0 != -1)
                ai_NextObjectId = *(int *)((char *)p + 0x15);
            if (*(int *)((char *)p + 0x11) > 0) {
                gm_AddNegative(c[1], *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8), *(int *)((char *)p + 0x11));
                gm_AddObjekt(*(int *)(ai_Chars[*(unsigned short *)(o + 0x27)] + 4), *(int *)(ai_Unk7e3f94 + ai_OwnPlayer * 0x5e8), 0);
            }
            return 0;
        }
    }
    return -7;
}
/* 0x4f1170, the message 0x4c6130 about the object o (a character +0x27, not the player's): graveyard 0x4f2e30 (0x1b, 1, id, -1, object id) first; fields v at +8, byte b at +0x10, x at +0x11, y at +0x15; -1 for no character,
   the player's own object or b -1 */

int unk_Unk4f1170(char *o, int v, int b, int x, int y)
{
    unsigned short ch;
    char m[0x19];

    ch = *(unsigned short *)(o + 0x27);
    if (ch != 0xffff && ch != *(unsigned short *)ai_PlayerCharC && b != -1) {
        *(int *)(m + 8) = *(int *)(o + 1);
        m[0x10] = (char)b;
        *(int *)(m + 0x11) = x;
        *(int *)(m + 0xc) = v;
        *(int *)(m + 0x15) = y;
        gv_Accuse(0x1b, 1, *(int *)(ai_Chars[ch] + 4), -1, *(int *)(m + 8));
        return ((int (*)(void *))ai_Unk4c6130)(m);
    }
    return -1;
}
/* 0x4f15e0, takes a random grave (0x4ef930) of the object o and its list entry (0x4ef900) and sends it with 0x4f1170 (type 0x1c); -1 when 0x4f1d50 (o, 0) is not 1 or nothing is found */

int gv_SendGrave(unsigned char *o)
{
    int *z;
    int *e;
    int k;

    if (unk_Unk4f1d50(o, 0) != 1)
        return -1;
    z = (int *)unk_Unk4ef930(o);
    if (z == 0)
        return -1;
    e = (int *)gv_LoadGraveyard(o, z);
    if (e == 0)
        return -1;
    k = gv_EntryIndex(*(char **)(o + 0x65), e);
    if (k == -1)
        return -1;
    return unk_Unk4f1170((char *)o, *e, k, *z, 0x1c);
}
/* 0x4f1660, 1 while the character c (kind 0x14 at +2, +8 set) still owes its grave payment: with an object (+0x16c, not the player's) the owed amount (+0x14 - 50 * 0x4f16f0 (object)) is set (0x4c4ac0 (id, 0x1c, -owed))
   and the character is done when +0x1c does not exceed it; without one the amount is -1000.0; 0 for other characters */

int unk_Unk4f1660(char *c)
{
    char *o;
    float owed, rest;

    if (c != 0 && c[2] == 0x14 && c[8] != 0) {
        o = *(char **)(c + 0x16c);
        if (o == 0 || *(short *)(o + 0x27) == -1 || *(unsigned short *)(o + 0x27) == *(unsigned short *)ai_PlayerCharC) {
            ai_Unk4c4ac0(*(int *)(c + 4), 0x1c, 0xc47a0000);
        } else {
            owed = *(float *)(c + 0x14) - unk_Unk4f16f0(o) * 50.0f;
            rest = *(float *)(c + 0x1c) - owed;
            ai_Unk4c4ac0(*(int *)(c + 4), 0x1c, -owed);
            if (rest > 0.0f)
                return 0;
        }
        return 1;
    }
    return 0;
}
/* 0x4f16f0, a rate of the object o: the sum of 10 / 10 / 10 / 15 / 10 / 10 / 15 for the items 0x1ff, 0x202, 0x208, 0x20d, 0x214, 0x216, 0x219 in its list, times 0.0125 */

float unk_Unk4f16f0(char *o)
{
    int sum;

    sum = 0;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x1ff) != 0)
        sum = 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x202) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x208) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x20d) != 0)
        sum += 15;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x214) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x216) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x219) != 0)
        sum += 15;
    return (float)sum * 0.0125f;
}
/* 0x4f17f0, writes the graveyard record of the object o to the file f: +0, +4 (4 each), the four flag bytes +0x18 .. +0x1b, the zombie records (0x44 bytes: 4, 0x10, 0x10, 5, then six ints) of the
   kind's table 0x5ee074, then the graves (+0 +4 +8 short +10 short +0xc byte, and the index of the zombie record at +0x10) of the table 0x5ee064; 1 when all was written */

int unk_Unk4f17f0(void *f, unsigned char *o)
{
    char *g;
    char *z;
    int i, n, idx;

    g = *(char **)(o + 0x65);
    if (g == 0)
        return 0;
    if (UNK_FW(g, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 4, 4, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x18, 1, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x19, 1, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x1a, 1, 1, f) == 0)
        return 0;
    if (UNK_FW(g + 0x1b, 1, 1, f) == 0)
        return 0;
    z = *(char **)(g + 0x20);
    i = 0;
    if (unk_Tab5ee074[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FW(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 4, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x14, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x24, 5, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x2c, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x30, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x34, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x38, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x3c, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0x40, 4, 1, f) == 0)
                return 0;
            i++;
            z += 0x44;
        } while (i < unk_Tab5ee074[*(int *)(g + 4)]);
    }
    z = *(char **)(g + 0x1c);
    i = 0;
    if (unk_Tab5ee064[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FW(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 4, 4, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 8, 2, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 10, 2, 1, f) == 0)
                return 0;
            if (UNK_FW(z + 0xc, 1, 1, f) == 0)
                return 0;
            idx = (*(int *)(z + 0x10) - *(int *)(g + 0x20)) / 0x44;
            if (UNK_FW(&idx, 4, 1, f) == 0)
                return 0;
            i++;
            z += 0x14;
        } while (i < unk_Tab5ee064[*(int *)(g + 4)]);
    }
    return 1;
}
/* 0x4f1d50, counts the characters (kind 0x14 at +2, the object at +0x16c = o) of the graveyard o; *n = the count; 1 while it is below the building's two limit bytes (+0x231 + +0x232), -1 for another building or no record */

int unk_Unk4f1d50(unsigned char *o, int *n)
{
    char *b;
    unsigned char lim1, lim2;
    int cnt;
    char *c;
    int *p;

    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] != 0x1d)
        return -1;
    if (*(int *)(o + 0x65) == 0)
        return -1;
    lim1 = (unsigned char)b[0x232];
    lim2 = (unsigned char)b[0x231];
    cnt = 0;
    for (p = (int *)(ai_Chars[0] + 0x16c); (int)p < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x16a); p += 0x87) {
        c = (char *)p - 0x16c;
        if (*(short *)c != -1 && c[2] == 0x14 && (unsigned char *)*p == o)
            cnt++;
    }
    if (n != 0)
        *n = cnt;
    return cnt < (int)(lim1 + lim2);
}
/* 0x4f1200, raises the dead character (kind 0x14) of the graveyard entry p[+0x11] (list +0xc, entry +0x10 of the object +8) as a living one: the first free character slot (as 0x4ec500), skills = record bytes
   +0x24 >> 1, names from the record (+4 / +0x14), a random float 1000 / (rand * 4 + 8), relations random -0x20 .. 0x1f; the entry is cleared, and the first other used entry becomes the list's current one; returns
   the character id or 0xffff / a negative reason (as 0x4f0240). ROUGH */

unsigned unk_Unk4f1200(char *p)
{
    unsigned char *o;
    char *rec, *e, *c;
    int *z, *z2, *q;
    int l, n, i, j, k, slot;
    float r;

    if (*(int *)(p + 0x15) != 0x1b && *(int *)(p + 0x15) != 0x1c)
        return (unsigned)-6;
    gm_FindById(&o, 0, 0, *(int *)(p + 8));
    if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff || ai_Chars[*(unsigned short *)(o + 0x27)] == ai_PlayerCharC)
        return (unsigned)-1;
    if (ai_BTypes[o[0] * 0x24d] != 0x1d)
        return (unsigned)-2;
    if (*(int *)(o + 0x65) == 0)
        return (unsigned)-3;
    l = *(int *)(p + 0xc);
    if (unk_Unk4ef4f0(*(char **)(o + 0x65), l) == 0)
        return (unsigned)-4;
    e = (char *)unk_Unk4ef570(*(char **)(o + 0x65), l, (unsigned char)p[0x10]);
    if (e == 0)
        return 0xffff;
    i = 0;
    if (gv_EntryCounts[l] > 0) {
        z = *(int **)(e + 0x10);
        do {
            q = z;
            if (*q == *(int *)(p + 0x11))
                break;
            i++;
            z = q + 0x11;
        } while (i < gv_EntryCounts[l]);
        if (*q == *(int *)(p + 0x11)) {
            for (slot = 0; slot < 0x300; slot++)
                if (*(short *)ai_Chars[slot] == -1)
                    break;
            if (slot == 0x300) {
                for (slot = 0; slot < 0x300; slot++)
                    if (ai_Chars[slot][2] == 0x0f)
                        break;
                if (slot == 0x300)
                    return 0xffff;
            }
            if (ai_Chars[slot][2] == 0x0f)
                ai_CharCount--;
            c = ai_Chars[slot];
            memset(c, 0, 0x21c);
            *(int *)(c + 4) = -1;
            *(unsigned short *)c = 0xffff;
            memset(c + 0x5c, 0xff, 0x20);
            ai_CharCount++;
            *(int *)(c + 4) = ai_NextObjectId;
            ai_NextObjectId++;
            *(unsigned short *)c = (unsigned short)slot;
            c[2] = 0x14;
            *(unsigned char **)(c + 0x170) = o;
            *(unsigned char **)(c + 0x16c) = o;
            *(int *)(c + 0x1f0) = 0;
            *(int *)(c + 0x210) = -1;
            c[0x215] = 0;
            c[0x216] = 0;
            c[0x217] = 0;
            c[0x218] = 1;
            *(float *)(c + 0x1e4) = 1.0f;
            c[8] = 100;
            *(short *)(c + 0xa) = 0x10;
            *(short *)(c + 0x164) = 0;
            c[0x165] = p[0x15];
            c[0x166] = 0;
            *(int *)(c + 0x18c) = -1;
            *(int *)(c + 0x20c) = -1;
            c[0x1f4] = 0;
            *(int *)(c + 0x190) = 4;
            *(int *)(c + 0x194) = 0;
            c[0xd] = 1;
            *(short *)(c + 0x50) = 0x7fff;
            for (j = 0; j < 5; j++)
                c[0x80 + j] = (char)((unsigned char)((char *)q)[0x24 + j] >> 1);
            c[9] = (char)q[0xe];
            {
                char *s = (char *)(q + 1);
                char *d = c + 0x30;
                char ch;
                do {
                    ch = *s++;
                    *d++ = ch;
                } while (ch != 0);
                s = (char *)(q + 5);
                d = c + 0x40;
                do {
                    ch = *s++;
                    *d++ = ch;
                } while (ch != 0);
            }
            *(int *)(c + 0x10) = 0;
            r = util_RandomF();
            *(int *)(c + 0x18) = 0;
            *(float *)(c + 0x1c) = 1000.0f;
            *(int *)(c + 0x20) = 0;
            *(int *)(c + 0x24) = 0;
            *(short *)(c + 0x28) = 0;
            *(int *)(c + 0x2c) = 0;
            k = 0;
            *(float *)(c + 0x14) = 1000.0f / (r * 4.0f + 8.0f);
            for (i = 0, j = 0; i < 0x90000; i += 0x300, j++) {
                ai_RelationMemory[0][*(unsigned short *)c + i] = 0;
                ai_Relations[0][*(unsigned short *)c + i] = (signed char)util_RandomU(0x40) - 0x20;
                ai_Relations[0][*(unsigned short *)c * 0x300 + j] = (signed char)util_RandomU(0x40) - 0x20;
                ai_RelationMemory[0][*(unsigned short *)c * 0x300 + j] = 0;
            }
            gm_PlaceOnStage(*(int *)(c + 4), *(int *)(o + 1), -1, 0, 0);
            gm_ChooseHeadIcon(c);
            rec = *(char **)(o + 0x65);
            if (*q == *(int *)(e + 4)) {
                n = 0;
                if (gv_EntryCounts[*(int *)(p + 0xc)] > 0) {
                    z2 = *(int **)(e + 0x10);
                    do {
                        if (*z2 != -1 && *z2 != *q) {
                            *(int *)(e + 4) = *z2;
                            goto cleared;
                        }
                        n++;
                        z2 += 0x11;
                    } while (n < gv_EntryCounts[*(int *)(p + 0xc)]);
                }
                *(int *)(e + 4) = -1;
                *(short *)(e + 8) = (short)0xffff;
            }
        cleared:
            z2 = q;
            for (n = 0x11; n != 0; n--)
                *z2++ = 0;
            *q = -1;
            return *(unsigned short *)c;
        }
    }
    return 0xffff;
}
/* 0x4f1aa0, reads the graveyard record of 0x4f17f0 from the file f for the object o (allocated by 0x4efc10): the same fields in the same order; the graves' zombie pointers are rebuilt from the saved indexes; 1 when all was read */

int gv_ReadGraveyard(void *f, unsigned char *o)
{
    char *g;
    char *z;
    int i, idx;

    g = (char *)gv_AllocGraveyard(o);
    if (g == 0)
        return 0;
    if (UNK_FR(g, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 4, 4, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x18, 1, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x19, 1, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x1a, 1, 1, f) == 0)
        return 0;
    if (UNK_FR(g + 0x1b, 1, 1, f) == 0)
        return 0;
    z = *(char **)(g + 0x20);
    i = 0;
    if (unk_Tab5ee074[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FR(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 4, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x14, 0x10, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x24, 5, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x2c, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x30, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x34, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x38, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x3c, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0x40, 4, 1, f) == 0)
                return 0;
            i++;
            z += 0x44;
        } while (i < unk_Tab5ee074[*(int *)(g + 4)]);
    }
    i = 0;
    z = *(char **)(g + 0x1c);
    if (unk_Tab5ee064[*(int *)(g + 4)] > 0) {
        do {
            if (UNK_FR(z, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 4, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 8, 2, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 10, 2, 1, f) == 0)
                return 0;
            if (UNK_FR(z + 0xc, 1, 1, f) == 0)
                return 0;
            if (UNK_FR(&idx, 4, 1, f) == 0)
                return 0;
            *(int *)(z + 0x10) = *(int *)(g + 0x20) + idx * 0x44;
            z += 0x14;
            i++;
        } while (i < unk_Tab5ee064[*(int *)(g + 4)]);
    }
    return 1;
}
/* 0x4f22a0, the entry of the list 0xff of the object o (0x4ef7c0 with 0x4ef770) whose byte +0xc is below m and the smallest of them; 0 when the owner is the player or has no item 0x1f8 (list 2, key 4) or none qualifies */

int unk_Unk4f22a0(char *o, unsigned char m)
{
    int best;
    unsigned e;

    best = 0;
    if (*(short *)(o + 0x27) != -1 && *(unsigned short *)(o + 0x27) != *(unsigned short *)ai_PlayerCharC
        && gm_Query(*(int *)(o + 0x5d), 2, 4, 0x1f8, 6) != 0) {
        for (e = gv_FindEntry(0, (unsigned char *)o, 0, gv_EntryIsUsed, 0xff); e != 0;
             e = gv_FindEntry(0, (unsigned char *)o, e, gv_EntryIsUsed, 0xff)) {
            if (*(unsigned char *)(e + 0xc) < m && (best == 0 || *(unsigned char *)(e + 0xc) < *(unsigned char *)(best + 0xc)))
                best = e;
        }
        return best;
    }
    return 0;
}
/* 0x4f2330, the cost of a character's action: x = 0x5085b0 (0x50a640 of the character id) clamped to 10000 .. 1000000, v = log10(x)^2 * 100 * (0.25, 0.5, 0.75 for kind 0, 1, 2); with flag 0 also times
   (0.3 + 0.7 * (a & 0xff) / 255) * 0.75; returns r = 0x5085f0 (v) and stores r * (0.8, 0.7, 0.6)[kind] in *out. ROUGH */

int unk_Unk4f2330(unsigned short *c, int a, int kind, int flag, int *out)
{
    double x, v;
    int r;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    if (x <= 1000000.0) {
        if (x <= 10000.0)
            x = 10000.0;
    } else
        x = 1000000.0;
    v = log10(x);
    v = v * v * 100.0;
    if (kind == 0)
        v *= 0.25;
    else if (kind == 1)
        v *= 0.5;
    else if (kind == 2)
        v *= 0.75;
    if (flag == 0)
        v = ((a & 0xff) * 0.00392156862745098 * 0.7f + 0.3f) * (v * 0.75);
    r = gm_PlayerFactor((int)v, ai_OwnPlayer);
    *out = (int)(r * unk_Tab5ee034[kind]);
    return r;
}
/* 0x4f2440, the cost difference of two kinds: 0x4f2330 (c, 100, k1, 1) minus half of the one of k2; *out the difference times the factor of k2 */

int unk_Unk4f2440(unsigned short *c, int k1, int k2, int *out)
{
    int a, b, d;

    a = unk_Unk4f2330(c, 100, k1, 1, &d);
    b = unk_Unk4f2330(c, 100, k2, 1, &d);
    d = b - a / 2;
    *out = (int)(d * unk_Tab5ee034[k2]);
    return d;
}
/* 0x4f24a0, calls 0x5085f0 with 0.1 * x / (log10(x) + 1) where x = 0x5085b0 (0x50a640 of the character id) clamped to 10000 .. 100000. ROUGH */

int unk_Unk4f24a0(unsigned short *c)
{
    double x;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    if (x <= 100000.0) {
        if (x <= 10000.0)
            x = 10000.0;
    } else
        x = 100000.0;
    return gm_PlayerFactor((int)(x / (log10(x) + 1.0) * 0.1f), ai_OwnPlayer);
}
/* 0x4f2540, finds the first graveyard (building kind 0x1d) of the 256 buildings whose list 0xff (0x4ef7c0) has an entry matching `a` by 0x4ef710; *o = the building, *e = the entry; 1 when found */

int unk_Unk4f2540(void *a, unsigned char **o, int *e)
{
    int i, r;
    unsigned char *b;

    i = 0;
    *o = 0;
    *e = 0;
    b = (unsigned char *)ai_Blds;
    do {
        if (*b != 0 && ai_BTypes[*b * 0x24d] == 0x1d) {
            r = gv_FindEntry(a, b, 0, unk_Unk4ef710, 0xff);
            if (r != 0) {
                *o = b;
                *e = r;
                return 1;
            }
        }
        i++;
        b += 0xa9;
    } while (i < 0x100);
    return 0;
}
/* 0x4f25d0, as 0x4f2540 with the match 0x4ef740 */

int unk_Unk4f25d0(void *a, unsigned char **o, int *e)
{
    int i, r;
    unsigned char *b;

    i = 0;
    *o = 0;
    *e = 0;
    b = (unsigned char *)ai_Blds;
    do {
        if (*b != 0 && ai_BTypes[*b * 0x24d] == 0x1d) {
            r = gv_FindEntry(a, b, 0, gv_EntrySameId, 0xff);
            if (r != 0) {
                *o = b;
                *e = r;
                return 1;
            }
        }
        i++;
        b += 0xa9;
    } while (i < 0x100);
    return 0;
}
/* 0x4f2660, a rate of the object o: 30 (item 0x20f) + 10 (0x21f) + 15 (0x222) + 25 (0x223), times 0.0125 * 0.5 + 0.5 */

float unk_Unk4f2660(char *o)
{
    float x;

    x = 0.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x20f) != 0)
        x = 30.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x21f) != 0)
        x = x + 10.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x222) != 0)
        x = x + 15.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x223) != 0)
        x = x + 25.0f;
    return x * 0.0125f * 0.5f + 0.5f;
}
/* 0x4f2720, copies the record i (0 .. 0x1c, 0x24 bytes at 0x611258) of the table to out; 0 for i > 0x1c */

int gv_GetTableRecord(int i, int *out)
{
    int *s;
    int k;

    if (i > 0x1c)
        return 0;
    s = (int *)(unk_Tab611258 + i * 0x24);
    for (k = 9; k != 0; k--)
        *out++ = *s++;
    return 1;
}
/* 0x4f2870, finds the record of the table whose bytes +0x1c / +0x1d are (c[+0x166], b) and copies it to out; 0 when none */

int gv_FindTableRecord(char *c, unsigned b, int *out)
{
    unsigned char *p;
    int i, k;
    int *s;

    i = 0;
    p = (unsigned char *)unk_Tab611258 + 0x1d;
    while ((unsigned char)c[0x166] != p[-1] || b != *p) {
        p += 0x24;
        i++;
        if ((int)p > ((unsigned)&unk_Tab611258 + 0x430))
            return 0;
    }
    s = (int *)(unk_Tab611258 + i * 0x24);
    for (k = 9; k != 0; k--)
        *out++ = *s++;
    return 1;
}
/* 0x4f28d0, writes the graveyard game state to the file f: the tag 0x1d, the ints +0x18 of the 0x1d table records (0x611270), 0x6f1b88, the counts of the used 0x2d-byte records at 0x91f700 (first int not -1) and of the used pairs at 0x91b6e0, then
   the used records (fields 4, 0xe, 4, 4, 2, 1, 4, 4, 4 bytes) and the used pairs; 1 when all was written. ROUGH */

int gv_WriteGraveyard(void *f)
{
    int tag, cnt, cnt2;
    char *p;
    int *q;

    tag = 0x1d;
    cnt = 0;
    cnt2 = 0;
    if (f == 0)
        return 0;
    if (UNK_FW(&tag, 4, 1, f) == 0)
        return 0;
    for (p = unk_Tab611258 + 0x18; (int)p < ((unsigned)&unk_Tab611258 + 0x42c); p += 0x24) {
        if (UNK_FW(p, 4, 1, f) == 0)
            return 0;
    }
    if (UNK_FW(unk_Var6f1b88, 4, 1, f) == (unsigned)-1)
        return 0;
    for (q = (int *)gv_Graveyard; (int)q < (int)(gv_Graveyard + 0x5a00); q = (int *)((char *)q + 0x2d)) {
        if (*q != -1)
            cnt++;
    }
    for (q = (int *)(unk_Tab91b6e0 + 4); (int)q < ((unsigned)unk_Tab91b6e0 + 0x4004); q += 2) {
        if (q[-1] != -1 && *q != -1)
            cnt2++;
    }
    if (UNK_FW(&cnt, 4, 1, f) == (unsigned)-1)
        return 0;
    if (UNK_FW(&cnt2, 4, 1, f) == (unsigned)-1)
        return 0;
    for (q = (int *)gv_Graveyard; ; ) {
        if (*q != -1) {
            if (UNK_FW(q, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW(q + 1, 0xe, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x12, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x16, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x1a, 2, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW(q + 7, 1, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x1d, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x21, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x25, 4, 1, f) == (unsigned)-1)
                return 0;
        }
        q = (int *)((char *)q + 0x2d);
        if ((int)q > ((unsigned)&ai_Unk91f716 + 0x59e9)) {
            q = (int *)(unk_Tab91b6e0 + 4);
            do {
                if (q[-1] != -1 && *q != -1) {
                    if (UNK_FW(q - 1, 4, 1, f) == (unsigned)-1)
                        return 0;
                    if (UNK_FW(q, 4, 1, f) == (unsigned)-1)
                        return 0;
                }
                q += 2;
                if ((int)q > ((unsigned)unk_Tab91b6e0 + 0x4003))
                    return 1;
            } while (1);
        }
    }
}
/* 0x4f2da0, the highest level (0x5092e0 (b, 10, 1)) among the school buildings (kind 7) of the character *c's, times 0.01; 0.75 when one has exactly 0x4b */

float unk_Unk4f2da0(short *c)
{
    int best, i, v;
    unsigned char *b;

    best = 0;
    i = 0;
    b = (unsigned char *)ai_Blds;
    do {
        if (*b != 0 && *(short *)(b + 0x27) == *c && ai_BTypes[*b * 0x24d] == 7) {
            v = gm_BuildingSkill(b, 10, 1);
            if (v == 0x4b)
                return 0.75f;
            if (v > best)
                best = v;
        }
        i++;
        b += 0xa9;
    } while (i < 0x100);
    return (float)best * 0.01f;
}
/* 0x4f2b30, reads what 0x4f28d0 wrote from the file f (the tag must be 0x1d; saves after 0x10040 carry the counts 0x80 / 0x200 first, older ones assume them): the table ints, 0x6f1b88, the used 0x2d-byte records (the rest are set to -1 fields)
   and the pairs (the table 0x91b6e0 starts as all -1); 1 when all was read. ROUGH */

int unk_Unk4f2b30(void *f)
{
    unsigned n1, n2, i;
    int tag;
    char *p;
    int *q;

    n1 = 0x80;
    n2 = 0x200;
    if (f == 0)
        return 0;
    if (UNK_FR(&tag, 4, 1, f) == 0)
        return 0;
    if (tag != 0x1d)
        return 0;
    for (p = unk_Tab611258 + 0x18; (int)p < ((unsigned)&unk_Tab611258 + 0x42c); p += 0x24) {
        if (UNK_FR(p, 4, 1, f) == 0)
            return 0;
    }
    if (UNK_FR(unk_Var6f1b88, 4, 1, f) == 0)
        return 0;
    if (ls_Version > 0x10040) {
        if (UNK_FR(&n1, 4, 1, f) == 0)
            return 0;
        if (UNK_FR(&n2, 4, 1, f) == 0)
            return 0;
    }
    i = 0;
    if (n1 != 0) {
        p = gv_Graveyard + 4;
        do {
            if (UNK_FR(p - 4, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p, 0xe, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0xe, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x12, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x16, 2, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x18, 1, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x19, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x1d, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(p + 0x21, 4, 1, f) == 0)
                return 0;
            i++;
            p += 0x2d;
        } while (i < n1);
        if ((int)i > 0x1ff)
            goto pairs;
    }
    for (q = (int *)(gv_Graveyard + 0x12 + i * 0x2d); (int)q < (int)(gv_Graveyard + 0x5a12); q = (int *)((char *)q + 0x2d)) {
        *(int *)((char *)q - 0x12) = -1;
        q[0] = -1;
        q[1] = -1;
        *(short *)(q + 2) = 0;
        *(int *)((char *)q + 0x13) = 0;
    }
pairs:
    memset(unk_Tab91b6e0, 0xff, 0x4000);
    i = 0;
    if (n2 != 0) {
        q = (int *)(unk_Tab91b6e0 + 4);
        do {
            if (UNK_FR(q - 1, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(q, 4, 1, f) == 0)
                return 0;
            i++;
            q += 2;
        } while (i < n2);
    }
    return 1;
}
/* 0x4f2fe0, the text of the table record i into out (0x5176d0 with the texts 0x13fe + 5 * (b0) / 0x13fd + ..): by the second byte of the record 0: 0 as is, 1 / 2 / 3 add 0x13dd / 0x13e6 / 0x13ec to the last field; 0 for another value or no record */

int gv_TableRecordText(char *out, int i)
{
    unsigned rec[9];
    int b;

    if (gv_GetTableRecord(i, (int *)rec) != 0) {
        b = (rec[0] & 0xff) * 5;
        switch ((rec[0] >> 8) & 0xff) {
        case 0:
            txt_Format(out, b + 0x13fe, b + 0x13fd, rec[8]);
            return 1;
        case 1:
            rec[8] = rec[8] + 0x13dd;
            break;
        case 2:
            rec[8] = rec[8] + 0x13e6;
            break;
        case 3:
            rec[8] = rec[8] + 0x13ec;
            break;
        default:
            return 0;
        }
        txt_Format(out, b + 0x13fe, b + 0x13fd, rec[8]);
        return 1;
    }
    return 0;
}

#define R4(r, o) (*(int *)((r).b + (o)))

/* 0x4f30a0, the text of a record with the type byte (+0x1c) 2 .. 8: types 2 / 3 / 4 / 8 pick the text numbers 0x140a / 0x140f / 0x1414 / 0x1428 (+ 1 for mode < 2); anything else the fixed text; 1 when a text was built.
   +4 the int value (shown by address), +0x12 the character id (its short id or 0xfffe). */
int unk_Unk4f30a0(char *out, UNKREC r, int mode)
{
    int m;
    unsigned short *p;
    unsigned short id;

    m = mode < 2;
    p = (unsigned short *)gm_FindChar(R4(r, 0x12));
    if (p == 0)
        id = 0xfffe;
    else
        id = *p;
    switch ((unsigned char)r.b[0x1c]) {
    case 2:
        break;
    case 3:
        txt_Format(out, m + 0x140f, r.b + 4);
        return 1;
    case 4:
        txt_Format(out, m + 0x1414, r.b + 4, id);
        return 1;
    default:
        strcpy(out, "Hm, das ist doch nicht verboten");
        return 0;
    case 8:
        txt_Format(out, m + 0x1428, r.b + 4);
        return 1;
    }
    if (R4(r, 4) < 0x75) {
        txt_Format(out, m + 0x140a, r.b + 4, R4(r, 0x1d) + 0x13f9);
        return 1;
    }
    txt_Format(out, m + 0x140a, r.b + 4, R4(r, 0x1d) + 0x13fb);
    return 1;
}
/* 0x4f31d0, the text of a record with the type byte 0xe / 0x10 (texts 0x1446 / 0x1450, + 1 for mode < 2); anything else the fixed text; 1 when a text was built. */

int unk_Unk4f31d0(char *out, UNKREC r, int mode)
{
    int m;
    unsigned short *p;
    unsigned short id;

    m = mode < 2;
    p = (unsigned short *)gm_FindChar(R4(r, 0x12));
    if (p == 0)
        id = 0xfffe;
    else
        id = *p;
    if ((unsigned char)r.b[0x1c] != 0xe) {
        if ((unsigned char)r.b[0x1c] != 0x10) {
            strcpy(out, "Hm, das ist doch nicht verboten");
            return 0;
        }
        txt_Format(out, m + 0x1450, id, r.b + 4);
        return 1;
    }
    txt_Format(out, m + 0x1446, r.b + 4, id);
    return 1;
}
/* 0x4f3270, the text of a record with the type byte 0x11 .. 0x1c (texts 0x1455 + 5 * n, + 1 for mode < 2; the object name (0x4fea50 of the id at +0x21, default "Geb\xe4ude") goes in as "%1G" when found); returns 0 always. ROUGH */

int unk_Unk4f3270(char *out, UNKREC r, int mode)
{
    char name[0x80] = "Geb\xe4ude";
    int m, obj, k;
    unsigned short *p;
    unsigned id;

    m = mode < 2;
    gm_FindById(&obj, 0, 0, R4(r, 0x21));
    if (obj != 0)
        txt_Format(name, "%1G", obj);
    p = (unsigned short *)gm_FindChar(R4(r, 0x12));
    if (p == 0)
        id = 0xfffe;
    else
        id = *p;
    switch ((unsigned char)r.b[0x1c]) {
    case 0x11:
        txt_Format(out, m + 0x1455, r.b + 4);
        return 0;
    case 0x12:
        txt_Format(out, m + 0x145a, r.b + 4);
        return 0;
    case 0x13:
        txt_Format(out, m + 0x145f, id, r.b + 4);
        return 0;
    case 0x14:
        txt_Format(out, m + 0x1464, r.b + 4, id);
        return 0;
    case 0x15:
        txt_Format(out, m + 0x1469, r.b + 4, id);
        return 0;
    case 0x16:
        txt_Format(out, m + 0x146e, id, r.b + 4);
        return 0;
    case 0x17:
        txt_Format(out, m + 0x1473, r.b + 4, id);
        return 0;
    case 0x18:
        k = ls_GetLanguage();
        if (k < 0)
            return 0;
        if (k < 2)
            txt_Format(out, m + 0x1478, r.b + 4, id, name);
        else if (k == 2)
            txt_Format(out, m + 0x1478, r.b + 4, name, id);
        return 0;
    case 0x19:
        txt_Format(out, m + 0x147d, r.b + 4, id);
        return 0;
    case 0x1a:
        txt_Format(out, m + 0x1482, r.b + 4, id);
        return 0;
    case 0x1b:
        txt_Format(out, m + 0x1487, r.b + 4);
        return 0;
    case 0x1c:
        txt_Format(out, m + 0x148c, r.b + 4, id);
        return 0;
    }
    return 0;
}
/* 0x4f3580, picks the text builder by the type byte (+0x1c) of the record: below 9 0x4f30a0, below 0x11 0x4f31d0, below 0x1d 0x4f3270; 0 otherwise */

int unk_Unk4f3580(char *out, UNKREC r, int mode)
{
    if ((unsigned char)r.b[0x1c] < 9)
        return unk_Unk4f30a0(out, r, mode);
    if ((unsigned char)r.b[0x1c] < 0x11)
        return unk_Unk4f31d0(out, r, mode);
    if ((unsigned char)r.b[0x1c] < 0x1d)
        return unk_Unk4f3270(out, r, mode);
    return 0;
}
/* 0x4f3610, fills out[] with the indexes of the (up to 32) 0x2d-byte records of 0x91f700 whose int at +0x25 equals v */

void unk_Unk4f3610(int v, int *out)
{
    int n, i;
    int *p;

    n = 0;
    i = 0;
    p = (int *)(gv_Graveyard + 0x25);
    do {
        if (n > 0x1f)
            return;
        if (*p == v) {
            out[n] = i;
            n++;
        }
        p = (int *)((char *)p + 0x2d);
        i++;
    } while ((int)p < ((unsigned)&amt_Table + 0x5));
}
/* 0x4f3640, stores the pair (a, b) in the free slot (0x4f3780 (a, b)) of the table 0x91b6e0 ("gs_AddBeweis: Executed .." is printed); 1 when stored */

int unk_Unk4f3640(int b, int a)
{
    int k;
    char buf[512];

    k = unk_Unk4f3780(a, b);
    if (k >= 0) {
        ((int *)unk_Tab91b6e0)[k * 2] = a;
        ((int *)unk_Tab91b6e0)[k * 2 + 1] = b;
        sprintf(buf, "gs_AddBeweis: Executed .. own", a, b, k);
        return 1;
    }
    return 0;
}
/* 0x4f36b0, the last free (int +0x25 = 0) record of the 512 at 0x91f700 (stops at the first one found), -1 when none */

int unk_Unk4f36b0(void)
{
    int r, i, j;

    r = -1;
    i = 0;
    do {
        j = i;
        if (*(int *)(gv_Graveyard + 0x25 + i * 0x2d) == 0) {
            j = 0x200;
            r = i;
        }
        i = j + 1;
    } while (i < 0x200);
    return r;
}
/* 0x4f36e0, the index of the record with the id (+0) in 0x91f700 or -1 */

int unk_Unk4f36e0(int id)
{
    int i;
    int *p;

    i = 0;
    p = (int *)gv_Graveyard;
    do {
        if (*p == id)
            return i;
        p = (int *)((char *)p + 0x2d);
        i++;
    } while ((int)p < (int)((unsigned)&gv_Cannon.bld));
    return -1;
}
/* 0x4f3780, the slot for the pair (a, b) in the table 0x91b6e0 (2048 pairs): -2 when it exists already, else the first free one or -1 */

int unk_Unk4f3780(int a, int b)
{
    int ok, r, i;
    int *t;

    ok = 0;
    r = -1;
    i = 0;
    t = (int *)unk_Tab91b6e0;
    while (1) {
        if (!ok && t[i * 2] == -1 && t[i * 2 + 1] == -1) {
            r = i;
            ok = 1;
        }
        if (t[i * 2] == a && t[i * 2 + 1] == b)
            break;
        i++;
        if (i >= 0x800) {
            if (ok)
                return r;
            return -1;
        }
    }
    return -2;
}
/* 0x4f3910, changes the state (int +0x25) of the 0x2d-byte records of 0x91f700: without mode m every record in state s is set to 1 (count returned); with m the records of the pairs (a, ..) of 0x91b6e0 that are in state 1 and belong
   to the character b (+0x16) are set to s */

int unk_Unk4f3910(int a, int b, int s, int m)
{
    int cnt, i;
    int *p, *q;

    cnt = 0;
    if (m == 0) {
        p = (int *)(gv_Graveyard + 0x25);
        do {
            if (*p == s) {
                *p = 1;
                cnt++;
            }
            p = (int *)((char *)p + 0x2d);
        } while ((int)p < ((unsigned)&amt_Table + 0x5));
        return cnt;
    }
    p = (int *)(unk_Tab91b6e0 + 4);
    do {
        if (p[-1] == a) {
            i = 0;
            q = (int *)gv_Graveyard;
            do {
                if (*q == *p) {
                    if (*(int *)(gv_Graveyard + 0x25 + i * 0x2d) == 1 && *(int *)(gv_Graveyard + 0x16 + i * 0x2d) == b) {
                        *(int *)(gv_Graveyard + 0x25 + i * 0x2d) = s;
                        cnt++;
                    }
                    break;
                }
                q = (int *)((char *)q + 0x2d);
                i++;
            } while ((int)q < (unsigned)&gv_Cannon.bld);
        }
        p += 2;
        if ((int)p > ((unsigned)unk_Tab91b6e0 + 0x4003))
            return cnt;
    } while (1);
}
/* 0x4f39b0, how many records of 0x91f700 belong to the character c (+0x16) and are in state 1 */

int gv_CountOwnedGraves(int c)
{
    int cnt;
    int *p;

    cnt = 0;
    p = (int *)(gv_Graveyard + 0x25);
    do {
        if (*(int *)((char *)p - 0xf) == c && *p == 1)
            cnt++;
        p = (int *)((char *)p + 0x2d);
    } while ((int)p < (int)((unsigned)&amt_Table + 0x5));
    return cnt;
}
/* 0x4f39e0, the sum of the +0x18 ints of the table records (0x4f2720 of the byte +0x1c) of the records of the character c in state 1 */

int unk_Unk4f39e0(int c)
{
    int *p;
    int sum;
    int rec[9];

    p = (int *)(gv_Graveyard + 0x25);
    sum = 0;
    do {
        if (*(int *)((char *)p - 0xf) == c && *p == 1) {
            gv_GetTableRecord(*((unsigned char *)p - 9), rec);
            sum += rec[8];
        }
        p = (int *)((char *)p + 0x2d);
    } while ((int)p < ((unsigned)&amt_Table + 0x5));
    return sum;
}
/* 0x4f3fe0, fills out[0x20] (-1 first) with the indexes of the records of 0x91f700 that the character with the slot c (low word of t) owns of the graveyard player p: pairs (character id, record id) of 0x91b6e0 whose record belongs
   (+0x16) to p; returns the count (up to 0x21); 0 for no character or the slot 0 */

int unk_Unk4f3fe0(int p, int *out, unsigned t)
{
    int n, k, i;
    int *q;
    int *o;

    n = 0;
    if (gm_FindChar(p) == 0)
        return 0;
    if ((t & 0xffff) * 0x21c != -(unsigned)&ai_Chars) {
        q = (int *)(unk_Tab91b6e0 + 4);
        o = out;
        for (i = 0x20; i != 0; i--)
            *o++ = -1;
        do {
            if (q[-1] == *(int *)(ai_Chars[t & 0xffff] + 4)) {
                k = unk_Unk4f36e0(*q);
                if (k != -1 && *(int *)(gv_Graveyard + 0x16 + k * 0x2d) == p) {
                    out[n] = k;
                    n++;
                    if (n > 0x20)
                        return n;
                }
            }
            q += 2;
        } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
        return n;
    }
    return 0;
}
/* 0x4f4080, as 0x4f3fe0 but the records' ids (+0) are stored */

int unk_Unk4f4080(int p, int *out, unsigned t)
{
    int n, k, i;
    int *q;
    int *o;

    n = 0;
    if (gm_FindChar(p) == 0)
        return 0;
    if ((t & 0xffff) * 0x21c != -(unsigned)&ai_Chars) {
        q = (int *)(unk_Tab91b6e0 + 4);
        o = out;
        for (i = 0x20; i != 0; i--)
            *o++ = -1;
        do {
            if (q[-1] == *(int *)(ai_Chars[t & 0xffff] + 4)) {
                k = unk_Unk4f36e0(*q);
                if (k != -1 && *(int *)(gv_Graveyard + 0x16 + k * 0x2d) == p) {
                    out[n] = *(int *)(gv_Graveyard + k * 0x2d);
                    n++;
                    if (n > 0x20)
                        return n;
                }
            }
            q += 2;
        } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
        return n;
    }
    return 0;
}
/* 0x4f4130, the count of 0x4f3fe0 without the lists (stops at 0x21) */

int unk_Unk4f4130(int p, unsigned t)
{
    int n, k, id;
    int *q;

    n = 0;
    if (gm_FindChar(p) == 0)
        return 0;
    if ((t & 0xffff) * 0x21c != -(unsigned)&ai_Chars) {
        id = *(int *)(ai_Chars[t & 0xffff] + 4);
        q = (int *)(unk_Tab91b6e0 + 4);
        while (q[-1] != id || (k = unk_Unk4f36e0(*q)) == -1 || *(int *)(gv_Graveyard + 0x16 + k * 0x2d) != p || (n++, n < 0x21)) {
            q += 2;
            if ((int)q > ((unsigned)unk_Tab91b6e0 + 0x4003))
                return n;
        }
        return n;
    }
    return 0;
}
/* 0x4f41b0, for the character c: goes over the 512 records of 0x91f700 (0x2d bytes; int +0x25 != 0, int +0x16 = the accused character id != c's) whose pair (c's id, record id) is not yet in the table 0x91b6e0, counts them per accused
   character (up to 0x300), and returns the character with the most records; ids[] gets the ids (+0) of its records (up to max), *cnt their number; 0 when there is none. ROUGH */

char *gv_FindAccused(char *c, int max, int *ids, int *cnt)
{
    char *who[0x300];
    int count[0x300];
    char *rec[0x600];
    int i, j, n, nw, nr, best, bestn;
    char *o;
    int *q;

    *cnt = 0;
    memset(who, 0, sizeof(who));
    memset(count, 0, sizeof(count));
    nr = 0;
    n = 0;
    nw = 1;
    for (i = 0; i < 0x5a00; i += 0x2d) {
        if (*(int *)(gv_Graveyard + i + 0x25) == 0 || *(int *)(gv_Graveyard + i + 0x16) == *(int *)(c + 4))
            continue;
        o = gm_FindChar(*(int *)(gv_Graveyard + i + 0x16));
        if (o == 0)
            continue;
        for (q = (int *)(unk_Tab91b6e0 + 4); (int)q < ((unsigned)unk_Tab91b6e0 + 0x4004); q += 2)
            if (q[-1] == *(int *)(c + 4) && *q == *(int *)(gv_Graveyard + i))
                break;
        if ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004))
            continue;
        rec[nr++] = gv_Graveyard + i;
        for (j = 0; nw > 0 && j < 0x300; j++) {
            if (who[j] == 0) {
                who[j] = o;
                count[j]++;
                n = nw++;
                break;
            }
            if (*(int *)(who[j] + 4) == *(int *)(o + 4)) {
                count[j]++;
                break;
            }
            if (j + 1 >= nw)
                break;
        }
    }
    if (nr == 0 || n == 0)
        return 0;
    o = who[0];
    best = 0;
    bestn = 0;
    for (j = 0; j < n; j++) {
        if (count[j] > bestn) {
            o = who[j];
            bestn = count[j];
        }
    }
    j = 0;
    for (i = 0; i < nr && j < max; i++) {
        if (*(int *)(rec[i] + 0x16) == *(int *)(o + 4))
            ids[j++] = *(int *)rec[i];
    }
    *cnt = j;
    return o;
}
/* 0x4f4350, clears the graveyard game state: 0x6f1b88 = 0, the 512 records of 0x91f700 (ids -1, +0x16 = -1, int +0x25 = 0 ..), the 2048 pairs -1 */

void gv_ClearGraveyard(void)
{
    int *p;

    p = (int *)(gv_Graveyard + 0x12);
    *(int *)unk_Var6f1b88 = 0;
    do {
        p[1] = -1;
        p[0] = -1;
        *(int *)((char *)p - 0x12) = -1;
        *(int *)((char *)p + 0x13) = 0;
        p = (int *)((char *)p + 0x2d);
    } while ((int)p < (int)(gv_Graveyard + 0x5a12));
    p = (int *)(unk_Tab91b6e0 + 4);
    do {
        p[-1] = -1;
        p[0] = -1;
        p += 2;
    } while ((int)p < ((unsigned)unk_Tab91b6e0 + 0x4004));
}
/* 0x4f4420, a check of the character c for the order kind k: -1 without c, -3 for k >= 10, 0 for other kinds than 5 / 7, else -4 when the byte (+0x166 for 5, +0xd for 7) is 0 */

int unk_Unk4f4420(char *c, unsigned k)
{
    int r;
    char b;

    r = 0;
    if (c == 0)
        return -1;
    if (k < 10) {
        if (k == 5)
            b = c[0x166];
        else {
            if (k != 7)
                return 0;
            b = c[0xd];
        }
        if (b == 0)
            r = -4;
        return r;
    }
    return -3;
}
/* 0x4f4d10, runs 0x4f4970 for each of the 768 characters */

void unk_Unk4f4d10(void)
{
    int i;

    i = 0;
    do {
        unk_Unk4f4970(i);
        i++;
    } while (i < 0x300);
}
/* 0x4f4d30, the record id of the 0x91f700 entry (0x4f36e0) into: *c = the character's slot, *v = its liking (0x50c550 (c, *a, 1)) as an int, *k = the table record's low byte; 1 when the record exists and its table record has
   +0x18 set and the owner character is alive. ROUGH */

int gv_GetGraveInfo(int id, short *a, int *v, short *c, unsigned *k)
{
    int i;
    short *ch;
    int rec[9];

    if (id != -1) {
        i = unk_Unk4f36e0(id);
        if (i != -1) {
            if (gv_GetTableRecord(*(unsigned char *)(gv_Graveyard + 0x1c + i * 0x2d), rec) != 0 && rec[8] != 0) {
                ch = (short *)gm_FindChar(*(int *)(gv_Graveyard + 0x16 + i * 0x2d));
                if (ch != 0) {
                    *c = *ch;
                    gm_Unk505e60(*ch, 1);
                    *v = (int)gm_Liking(*ch, *a, 1);
                    *k = rec[0] & 0xff;
                    return 1;
                }
            }
        }
    }
    return 0;
}
/* 0x4f3700, for each of the 512 records of 0x91f700 by its state (int +0x25): state 1 sends 0x4c4e90 (id at +0, 0); a state above 1 whose 0x53e400 (1, 1, state) lookup fails sends 0x4c4e90 (id, 1); after 32 sends it waits for the last one (0x4c3ae0 / 0x52fe00). ROUGH */

void unk_Unk4f3700(void)
{
    int h, n, k;
    unsigned s;
    unsigned *p;
    int id, flag;

    h = -1;
    n = 0;
    p = (unsigned *)(gv_Graveyard + 0x25);
    do {
        s = *p;
        if (s == 1) {
            id = *(int *)((char *)p - 0x25);
            flag = 0;
            h = ((int (*)(int, int))ai_Unk4c4e90)(id, flag);
            n++;
        } else if (s > 1) {
            k = (int)he_FindEvent(1, 1, s);
            if (k == 0) {
                id = *(int *)((char *)p - 0x25);
                flag = 1;
                h = ((int (*)(int, int))ai_Unk4c4e90)(id, flag);
                n++;
            }
        }
        if (n > 0x1f) {
            if (h != -1) {
                while (cm_GetSlotState(h) == 0)
                    sv_PumpNet();
            }
            h = -1;
            n = 0;
        }
        p = (unsigned *)((char *)p + 0x2d);
    } while ((int)p < ((unsigned)&amt_Table + 0x5));
}
/* 0x4f37e0, retires the record of the id (0x4f36e0): its counter (short +0x1a) counts down, with mode m = 0 it stays while the counter is not 0, states other than 1 stay unless m forces it; the owner (0x505df0 of +0x16)
   is told (0x5a9430) when the player's pair is removed; the pairs of the id are cleared and the record freed (state 0, ids -1). 3 no record, 2 counting, 1 kept, 0 done */

int unk_Unk4f37e0(int id, int m)
{
    int k, st;
    short cnt;
    int *q;
    int *o;
    unsigned short pl;

    k = unk_Unk4f36e0(id);
    if (k == -1)
        return 3;
    cnt = *(short *)(gv_Graveyard + 0x1a + k * 0x2d);
    if (cnt > 0)
        *(short *)(gv_Graveyard + 0x1a + k * 0x2d) = cnt - 1;
    if (*(short *)(gv_Graveyard + 0x1a + k * 0x2d) != 0 && m == 0)
        return 2;
    st = *(int *)(gv_Graveyard + 0x25 + k * 0x2d);
    if (st != 1 && (m == 0 || st == 0))
        return 1;
    o = (int *)gm_FindChar(*(int *)(gv_Graveyard + 0x16 + k * 0x2d));
    ord_SubGridAmount(*(int *)(gv_Graveyard + 0x21 + k * 0x2d), *(int *)(gv_Graveyard + 0x1c + k * 0x2d));
    q = (int *)(unk_Tab91b6e0 + 4);
    pl = ai_PlayerId;
    do {
        if (id == *q) {
            if (pl != 0xffff && q[-1] == *(int *)(ai_Chars[pl] + 4) && o != 0 && m == 0) {
                ai_Unk5a9430(ai_Chars[pl], o);
                pl = ai_PlayerId;
            }
            q[-1] = -1;
            *q = -1;
        }
        q += 2;
    } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
    *(int *)(gv_Graveyard + 0x25 + k * 0x2d) = 0;
    *(int *)(gv_Graveyard + 0x16 + k * 0x2d) = -1;
    *(int *)(gv_Graveyard + k * 0x2d) = -1;
    return 0;
}
/* 0x4f4390, sorts n records of 0x2d bytes at r by the byte +0x1c, descending (a bubble sort with whole-record swaps) */

void unk_Unk4f4390(int n, char *r)
{
    int i, j;
    char *q;
    char tmp[0x2d];

    if (n != 1 && n - 1 >= 0) {
        i = 1;
        do {
            if (i < n) {
                q = r + 0x2d;
                for (j = n - i; j != 0; j--) {
                    if ((unsigned char)q[0x1c] < (unsigned char)r[0x1c]) {
                        memcpy(tmp, r, 0x2d);
                        memcpy(r, q, 0x2d);
                        memcpy(q, tmp, 0x2d);
                    }
                    q += 0x2d;
                }
            }
            r += 0x2d;
            i++;
        } while (i - 1 < n - 1);
    }
}
/* 0x4f3a30, retires up to n records of the character p's graveyard state-1 entries (0x4c4e90 (id, 1) each): the other player characters (kind 6 / 7, up to 8, flag per slot) whose pair (their id, the record id) exists in 0x91b6e0 are
   told (0x5a9430 (them, p's character)); returns how many records were retired. ROUGH */

int unk_Unk4f3a30(int p, int n)
{
    int cnt, off, k, j, m, i;
    char *c;
    int flag[8];
    int *who[8];
    char *q;
    int *t;

    cnt = 0;
    off = 0;
    c = gm_FindChar(p);
    m = 0;
    if (c != 0) {
        q = &ai_Chars[0][2];
        do {
            if (m > 7)
                break;
            if ((*q == 6 || *q == 7) && *(int *)(q + 2) != p) {
                flag[m] = 0;
                who[m] = (int *)(q - 2);
                m++;
            }
            q += 0x21c;
        } while ((int)q < (unsigned)((unsigned)ai_Chars + 0x65402));
    }
    do {
        if (n <= cnt)
            break;
        if (*(int *)(gv_Graveyard + 0x16 + off) == p && *(int *)(gv_Graveyard + 0x25 + off) == 1) {
            k = 0;
            do {
                j = 0;
                if (m > 0) {
                    i = *(int *)(unk_Tab91b6e0 + k);
                    do {
                        if (i == who[j][1] && *(int *)(unk_Tab91b6e0 + 4 + k) == *(int *)(gv_Graveyard + off))
                            flag[j] = 1;
                        j++;
                    } while (j < m);
                }
                k += 8;
            } while (k < 0x4000);
            ai_Unk4c4e90(*(int *)(gv_Graveyard + off), 1);
            cnt++;
        }
        off += 0x2d;
    } while (off < 0x5a00);
    j = 0;
    if (m > 0) {
        do {
            if (flag[j] != 0)
                ai_Unk5a9430(who[j], c);
            j++;
        } while (j < m);
    }
    return cnt;
}
/* 0x4f3b40, as 0x4f3a30 for the n record ids listed in ids[] (the records that exist in 0x91f700); returns how many were retired */

int unk_Unk4f3b40(int p, int n, int *ids)
{
    int cnt, m, i, k, j, id, t;
    char *c;
    int flag[8];
    int *who[8];
    char *q;
    int *r;

    cnt = 0;
    i = 0;
    c = gm_FindChar(p);
    m = 0;
    if (c != 0) {
        q = &ai_Chars[0][2];
        do {
            if (m > 7)
                break;
            if ((*q == 6 || *q == 7) && *(int *)(q + 2) != p) {
                flag[m] = 0;
                who[m] = (int *)(q - 2);
                m++;
            }
            q += 0x21c;
        } while ((int)q < (unsigned)((unsigned)ai_Chars + 0x65402));
    }
    if (n >= 1) {
        do {
            id = ids[i];
            r = (int *)gv_Graveyard;
            while (*r != id) {
                r = (int *)((char *)r + 0x2d);
                if ((int)r > ((unsigned)&ai_Unk91f716 + 0x59e9))
                    goto next;
            }
            k = 0;
            do {
                j = 0;
                if (m > 0) {
                    t = *(int *)(unk_Tab91b6e0 + k);
                    do {
                        if (t == who[j][1] && *(int *)(unk_Tab91b6e0 + 4 + k) == id)
                            flag[j] = 1;
                        j++;
                    } while (j < m);
                }
                k += 8;
            } while (k < 0x4000);
            ai_Unk4c4e90(id, 1);
            cnt++;
        next:
            i++;
        } while (i < n);
    }
    i = 0;
    if (m > 0) {
        do {
            if (flag[i] != 0)
                ai_Unk5a9430(who[i], c);
            i++;
        } while (i < m);
    }
    return cnt;
}
/* 0x4f3c50, the owners of the records that the character slot t holds (pairs in 0x91b6e0, record in state 1): counts how often each owner (+0x16) occurs and keeps the 32 best in ids[] / cnt[] sorted by count (descending, one
   insertion pass per owner); ids is first -1, cnt 0; returns the number of different owners (at most 0x20). ROUGH */

int gv_RankRecordOwners(int *ids, int *cnt, unsigned short t)
{
    int local[512];
    int n, i, k, c, j, own, id, x;
    int *q, *w, *p;

    n = 0;
    for (i = 0; i < 0x20; i++) {
        ids[i] = -1;
        cnt[i] = 0;
    }
    for (i = 0; i < 0x200; i++)
        local[i] = -1;
    q = (int *)(unk_Tab91b6e0 + 4);
    w = local;
    id = *(int *)(ai_Chars[t & 0xffff] + 4);
    do {
        if (q[-1] == id) {
            k = unk_Unk4f36e0(*q);
            if (k != -1 && *(int *)(gv_Graveyard + 0x25 + k * 0x2d) == 1) {
                *w = *(int *)(gv_Graveyard + 0x16 + k * 0x2d);
                w++;
            }
        }
        q += 2;
    } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
    p = local;
    j = 1;
    do {
        own = *p;
        if (own != -1) {
            *p = -1;
            n++;
            c = 1;
            for (i = j; i < 0x200; i++) {
                if (local[i] == own) {
                    c++;
                    local[i] = -1;
                }
            }
            if (cnt[0x1f] < c) {
                cnt[0x1f] = c;
                ids[0x1f] = own;
            }
            for (i = 0x1f, q = cnt + 0x1e; i > 0; i--, q--) {
                x = q[1];
                if (*q < x) {
                    k = ids[i];
                    ids[i] = ids[i - 1];
                    q[1] = *q;
                    ids[i - 1] = k;
                    *q = x;
                }
            }
        }
        p++;
        i = j < 0x200;
        j++;
    } while (i);
    if (n > 0x20)
        n = 0x20;
    return n;
}
/* 0x4f3dd0, as 0x4f3c50 over all the records of the character slot t (any state): groups the pairs by owner, per owner the count of records (c2[]) and of those in state 1 (c1[]); the 32 owners with the most records are kept in
   ids[] / c1[] / c2[] sorted by c2 (descending); returns the number of owners (at most 0x20). ROUGH */

int unk_Unk4f3dd0(int *ids, int *c1, int *c2, unsigned t)
{
    char *rec[512];
    int n, i, j, k, m, own, tot, st, id, x, y;
    int *q, *q2, *q3;
    unsigned *s;
    char *r, *r2;

    for (i = 0; i < 0x20; i++)
        ids[i] = -1;
    n = 0;
    for (i = 0; i < 0x20; i++)
        c1[i] = 0;
    for (i = 0; i < 0x20; i++)
        c2[i] = 0;
    for (i = 0; i < 0x200; i++)
        rec[i] = 0;
    q = (int *)(unk_Tab91b6e0 + 4);
    m = 0;
    id = *(int *)(ai_Chars[t & 0xffff] + 4);
    do {
        if (q[-1] == id) {
            k = unk_Unk4f36e0(*q);
            if (k != -1 && *(int *)(gv_Graveyard + 0x25 + k * 0x2d) != 0) {
                rec[m] = gv_Graveyard + k * 0x2d;
                m++;
            }
        }
        q += 2;
    } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
    if (m > 0) {
        j = 1;
        for (i = 0; ; i++) {
            tot = 1;
            r = rec[i];
            if (r != 0) {
                own = *(int *)(r + 0x16);
                n++;
                st = (*(int *)(r + 0x25) == 1);
                for (k = j; k < 0x200; k++) {
                    r2 = rec[k];
                    if (r2 != 0 && *(int *)(r2 + 0x16) == *(int *)(rec[i] + 0x16)) {
                        tot++;
                        if (*(int *)(r2 + 0x25) == 1)
                            st++;
                        rec[k] = 0;
                    }
                }
                rec[i] = 0;
                s = (unsigned *)c1 + 0x1f;
                if ((int)c1[0x1f] < tot) {
                    *s = st;
                    c2[0x1f] = tot;
                    ids[0x1f] = own;
                }
                q2 = c2 + 0x1e;
                for (k = 0x1f; k != 0; k--) {
                    x = q2[1];
                    if (*q2 < x) {
                        y = *s;
                        id = *(int *)((char *)s + ((char *)ids - (char *)c1));
                        *(int *)((char *)s + ((char *)ids - (char *)c1)) = *(int *)((char *)q2 + ((char *)ids - (char *)c2));
                        q2[1] = *q2;
                        *s = *(unsigned *)((char *)q2 + ((char *)c1 - (char *)c2));
                        *(int *)((char *)q2 + ((char *)ids - (char *)c2)) = id;
                        *q2 = x;
                        *(int *)((char *)q2 + ((char *)c1 - (char *)c2)) = y;
                    }
                    q2--;
                    s--;
                }
            }
            if (!(j < m))
                break;
            j++;
        }
        if (n > 0x20)
            return 0x20;
    }
    return n;
}
/* 0x4f4460, executes a character action (a = 0 .. 9) for the character c: 0 / 2 message 0x5c (value 3, kind 1 / 3), 1 / 3 message 0x5b (value 0.03 * the 0x50a640 value, kind 1 / 4), 4 / 6 / 8 0x4c5fa0 (id, 1 / 2 / 3, 0), 5 0x4e6680 (byte
   +0x166, kind 4), 7 sets byte 0xd of the character's record (2 or 1) and sends it, 9 a message to the players near (0x18 targets) and 0x4e6680 (kind 3). ROUGH */

void unk_Unk4f4460(unsigned short *c, int a)
{
    char m[0x240];
    char kind;
    int v, i, k;
    unsigned id;
    char *p;
    unsigned ids[6];
    int tg[0x12];

    v = gm_GetWorth(*c);
    switch (a) {
    case 0:
    case 1:
    case 2:
    case 3:
        *(int *)(m + 0x28) = *(int *)ai_Time;
        *(int *)(m + 0x2c) = *(int *)(ai_Time + 4);
        *(int *)(m + 0x30) = *(int *)(ai_Time + 8);
        *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
        m[0x36] = 1;
        m[4] = (a & 1) ? 0x5b : 0x5c;
        gm_AddTime(m + 0x28, 1, 0, 0);
        *(int *)(m + 8) = *(int *)(c + 2);
        *(int *)(m + 0xc) = -1;
        *(int *)(m + 0x58) = (a & 1) ? (int)(v * 0.03f) : 3;
        *(int *)(m + 0x5c) = (a == 0) ? 1 : (a == 1) ? 1 : (a == 2) ? 3 : 4;
        cm_SendLongMessage(m);
        return;
    case 4:
        ai_Unk4c5fa0(*(int *)(c + 2), 1, 0);
        return;
    case 5:
        ids[0] = *(unsigned char *)(c + 0xb3);
        amt_SendOfficeMessage(ids[0], 0, 0, 4, 0xff);
        return;
    case 6:
        ai_Unk4c5fa0(*(int *)(c + 2), 2, 0);
        return;
    case 7:
        kind = (*((unsigned char *)c + 0xd) > 2) + 1;
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldDelta(1, 1, 0xd, &kind);
        cm_SendFieldDeltas();
        return;
    case 8:
        ai_Unk4c5fa0(*(int *)(c + 2), 3, 0);
        return;
    case 9:
        ai_Unk4c5fa0(*(int *)(c + 2), 0xff, 1);
        tg[0] = *(int *)(c + 2);
        tg[1] = *(int *)((char *)ai_JudgeCatholic + 4);
        tg[2] = *(int *)((char *)ai_JudgeProtestant + 4);
        i = 3;
        for (p = ai_Chars[0] + 2; (int)p <= ((unsigned)((unsigned)ai_Chars + 0x65400) + 0x1) && i < 0x10; p += 0x21c) {
            if (tg[0] != *(int *)(p + 2) && (*p == 6 || *p == 7 || *p == 5))
                tg[i++] = *(int *)(p + 2);
        }
        *(char *)(m + 0x100) = (char)i;
        *(int *)(m + 0x120) = *(int *)(ai_Chars[ai_PlayerId] + 4);
        *(int *)(m + 0x114) = *(int *)ai_Time;
        *(int *)(m + 0x118) = -1;
        gm_AddTime(m + 0x114, 0x18, 0, 0);
        m[0x124] = 5;
        m[0xb0] = (char)util_RandomU(5);
        ai_Unk4c4fa0(m + 0x12c);
        if (amt_FindOffice(c, m + 0x18, ids) == 0)
            return;
        amt_SendOfficeMessage(ids[0], 0, 0, 3, 0xff);
        return;
    }
}
/* 0x4f48f0, the text of a character action a: 1 text 0x1538 (with 0.03 * the 0x50a640 value), 3 text 0x153e (4, the value), otherwise text 0x1535 + 3 * a */

void unk_Unk4f48f0(unsigned short *c, int a, char *out)
{
    int v;

    v = (int)(gm_GetWorth(*c) * 0.03f);
    switch (a) {
    case 1:
        txt_Format(out, 0x1538, v);
        return;
    case 3:
        txt_Format(out, 0x153e, 4, v);
        return;
    default:
        txt_Format(out, a * 3 + 0x1535);
        return;
    }
}
/* 0x4f4970, the level-up check of the character slot i (a living one of kind < 10 with +8 set): when its experience (0x50a640) exceeds the threshold table 0x5ee0f8 [level + 1] the level (+0xd) rises: at level 1 it needs the
   building kind 5 object 0xf with item 0x110 (the rank changes by 0x4000 via 0x4c4a60, the player gets the message 0x161d), otherwise the new level goes to the player's message 0x69 (type 9, level at +0x58) or the character's byte
   (0x4c3ba0 / 0x4c3c00 (1, 1, 0xd)); the other players are told too (the first 0x62489c of them; a kind 5 character moves the date by rand(8) * 7 + rand(4)). ROUGH */

void unk_Unk4f4970(unsigned i)
{
    char *c;
    int lv, exp, k, n;
    unsigned nl;
    char *o;
    char *it;
    char *q;
    char m[0xf8];
    char nb;
    int slot;

    slot = i & 0xffff;
    c = ai_Chars[slot];
    if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[8] != 0 && c[2] != 0) {
        exp = (int)gm_GetWorth(i);
        lv = (unsigned char)c[0xd];
        nl = lv + 1;
        if (nl > 10)
            nl = 10;
        if (unk_Tab5ee0f8[nl] < exp) {
            if (lv == 1) {
                o = (char *)gm_BuildQuery(1, 5, 0xf);
                if (o != 0 && (it = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x110)) != 0) {
                    cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x4000, 0);
                    if (c[2] == 6 || c[2] == 7) {
                        txt_Format(m, 0x161d, *(unsigned short *)c);
                        he_MessageBoxQuickjump(*(int *)(c + 4), -1, m, 0, 0x5fd, *(int *)(o + 1), *(int *)(it + 2), 0, "_NACHRICHTEN_HS_25");
                        return;
                    }
                }
            } else if (unk_Tab5ee0f8[nl] < exp && lv < 9) {
                nb = (char)(lv + 1);
                if (c[2] == 6 || c[2] == 7) {
                    memset(m, 0, 0xf8);
                    *(int *)(m + 0x4) = *(int *)(c + 4);
                    *(int *)(m + 0x28) = *(int *)ai_Time;
                    *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                    m[0] = 0x69;
                    *(int *)(m + 0x8) = -1;
                    m[0x36] = 9;
                    *(short *)(m + 0x30) = 7;
                    m[0x58] = nb;
                    *(int *)(m + 0x5c) = *(int *)(c + 4);
                    cm_SendLongMessage(m);
                } else {
                    cm_BeginFieldBlock(c, *(int *)(c + 4));
                    cm_AddFieldDelta(1, 1, 0xd, &nb);
                    cm_SendFieldDeltas();
                }
                if (c[2] == 5 || c[2] == 6 || c[2] == 7) {
                    n = 0;
                    q = &ai_Chars[0][2];
                    do {
                        if (n >= (int)ai_NumPlayers)
                            return;
                        if ((*q == 6 || *q == 7) && q - 2 != c) {
                            memset(m, 0, 0xf8);
                            *(int *)(m + 0x4) = *(int *)(q + 2);
                            *(int *)(m + 0x28) = *(int *)ai_Time;
                            *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                            m[0] = 0x69;
                            *(int *)(m + 0x8) = -1;
                            m[0x36] = 9;
                            *(short *)(m + 0x30) = 7;
                            if (c[2] == 5) {
                                k = util_RandomU(8);
                                k = util_RandomU(4);
                                ((void (*)(void *, int))gm_AddTime)(m + 0x28, k);
                            }
                            *(int *)(m + 0x5c) = *(int *)(c + 4);
                            m[0x58] = nb;
                            cm_SendLongMessage(m);
                            n++;
                        }
                        q += 0x21c;
                    } while ((int)q < (unsigned)((unsigned)ai_Chars + 0x65402));
                }
            }
        }
    }
}
/* 0x4f2750, the message 0x4c5930 (character id +4 of c or -1, byte i, int v clamped to the table record i's limits rec[1] .. rec[2]); -1 for a bad record or a dead character */
int gv_SetTableRecord(short *c, int i, int v)
{
    int rec[9];
    int m[3];
    int lim;

    if (gv_GetTableRecord(i, rec) == 0)
        return -1;
    if (c == 0)
        m[0] = -1;
    else {
        if (*c == -1)
            return -1;
        m[0] = *(int *)(c + 2);
    }
    lim = rec[1];
    if (v < rec[1] || (lim = rec[2], rec[2] < v))
        v = lim;
    *(char *)(m + 1) = (char)i;
    *(int *)((char *)m + 5) = v;
    return ((int (*)(char *))ai_Unk4c5930)(m);
}
/* 0x4f27d0, the message handler of 0x4f2750: stores the value at +5 in the table record (+0x18) of the record p[1] (byte) and tells the character p[0] (0x5a9110 with the text 0x4f2fe0) unless it is the player's own */

int unk_Unk4f27d0(int *p)
{
    int rec[9];
    char buf[0x100];
    char *c;

    if (gv_GetTableRecord((char)p[1], rec) == 0)
        return 0;
    *(int *)(unk_Tab611258 + 0x18 + (char)p[1] * 0x24) = *(int *)((char *)p + 5);
    if (*(int *)(ai_Chars[ai_PlayerId] + 4) != p[0]) {
        c = gm_FindChar(p[0]);
        if (c != 0) {
            gv_TableRecordText(buf, (char)p[1]);
            ai_Unk5a9110(c, buf);
        }
    }
    return 1;
}
/* 0x4f2e30, an accusation by the character a of the character b (obj) with the table record i and the value v: the record's rule (byte +0x18, by kind 1 .. 7) compares v with its limit (+0x14); when it holds and the school
   factor (0x4f2da0 of a) does not exceed a random float (0x42ec20) (and, with the skill 0x22a, a second one is above 0.5), the message 0x4c4e40 is sent (time, record byte, value, ids); -1 otherwise. ROUGH */

int gv_Accuse(int i, int v, int a, int b, int obj)
{
    int rec[9];
    char *ca, *cb;
    int tgt;
    float f;
    char m[0x40];
    int ok;

    if (gv_GetTableRecord(i, rec) == 0)
        return -1;
    ca = gm_FindChar(a);
    cb = gm_FindChar(b);
    gm_FindById(&tgt, 0, 0, obj);
    ok = 1;
    switch ((unsigned char)rec[4]) {
    case 1:
        if (rec[5] != v)
            return -1;
        break;
    case 2:
        if (rec[5] == v)
            return -1;
        break;
    case 3:
        if (rec[5] <= v)
            return -1;
        break;
    case 4:
        if (rec[5] < v)
            return -1;
        break;
    case 5:
        if (v <= rec[5])
            return -1;
        break;
    case 6:
        if (v < rec[5])
            return -1;
        break;
    case 7:
        if (rec[5] == v)
            return -1;
        if (rec[5] == 2)
            return -1;
        break;
    default:
        return -1;
    }
    f = unk_Unk4f2da0((short *)ca);
    if (f <= util_RandomF()) {
        if (ai_InventoryHas(ca, 0x22a) == 0 || util_RandomF() <= 0.5f) {
            *(int *)(m + 4) = *(int *)ai_Time;
            *(int *)(m + 8) = *(int *)(ai_Time + 4);
            *(short *)(m + 0x10) = *(short *)(ai_Time + 12);
            *(int *)(m + 0x1b) = v;
            m[0x1a] = (char)rec[0];
            *(int *)(m + 0x16) = (int)cb;
            *(short *)(m + 0x1e) = (short)rec[2];
            *(int *)(m + 0x2b) = 1;
            *(int *)(m + 0x27) = obj;
            return ((int (*)(void *))ai_Unk4c4e40)(m);
        }
    }
    return -1;
}
/* 0x4f1de0, the per-turn work of the graveyard object o (owner +0x27): walks the entries of the list 0xff that are in use (0x4ef7c0 with 0x4ef770); an entry whose character (+4) is gone (0x505df0) is cleared or written off
   (0x4f0c20 / 0x4f0900 messages, at most 0x40 per round, then it waits for the answer 0x4c3ae0), a living one whose time (+10) is up is paid for / released (0x4f0700 / 0x4f0900 / message 0x92), its owner may lose
   liking (0x4c4b00); at the end up to 4 graves are topped up (0x4f0d70 with 1 .. 2 new dead) and a player owner is told (0x5a9780 / 0x5a9690). ROUGH: the __ftol arguments and several constants are not read. */

void unk_Unk4f1de0(char *o)
{
    unsigned short ow;
    int h, rounds;
    int *e;
    char *pl;
    int paid, nrel, sum;
    unsigned s;
    int t, k, n;
    char *c;
    char m[0xf8];
    int hv;

    ow = *(unsigned short *)(o + 0x27);
    h = -1;
    pl = 0;
    rounds = 0;
    nrel = 0;
    paid = 0;
    sum = 0;
    if (ow != 0xffff && ow != *(unsigned short *)ai_PlayerCharC)
        pl = ai_Chars[ow];
    e = (int *)gv_FindEntry(0, (unsigned char *)o, 0, gv_EntryIsUsed, 0xff);
    do {
        if (e == 0) {
            n = unk_Unk4ef440((unsigned char *)o);
            if (n < 4)
                unk_Unk4f0d70(o, (util_RandomU(2) & 0xffff) + 1);
            if (pl != 0 && (pl[2] == 6 || pl[2] == 7)) {
                if (paid > 0)
                    ai_Unk5a9780(pl, o, paid, sum);
                if (nrel != 0)
                    ai_Unk5a9690(pl, o);
            }
            return;
        }
        if (rounds > 0x3f && h != -1) {
            while (cm_GetSlotState(h) == 0) {
                cm_FlushSendQueue();
                cm_ReceiveMessages();
                cm_ExecCommands();
            }
            rounds = 0;
            h = -1;
        }
        c = gm_FindChar(e[1]);
        if (c == 0) {
            t = e[0];
            k = 0;
            if (gv_EntryCounts[t] > 0) {
                int *z = (int *)e[4];
                do {
                    if (*z != -1) {
                        t = (int)util_RandomF();
                        if (t < 1 || ((gv_EntryIndex(*(char **)(o + 0x65), e) ^ *(int *)ai_Time) & 1) != 0)
                            goto next;
                        h = unk_Unk4f0c20(o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), -t);
                        goto counted;
                    }
                    k++;
                    z += 0x11;
                } while (k < gv_EntryCounts[t]);
            }
            h = unk_Unk4f0900(0, o, t, gv_EntryIndex(*(char **)(o + 0x65), e));
        counted:
            rounds++;
        } else {
            t = (int)util_RandomF();
            if (t > 0) {
                if (((gv_EntryIndex(*(char **)(o + 0x65), e) ^ *(int *)ai_Time) & 1) == 0) {
                    h = unk_Unk4f0c20(o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), -t);
                    rounds++;
                }
            }
            if (pl != 0) {
                if (c != pl) {
                    s = *(int *)ai_Time & 3;
                    if (s == (*(unsigned *)(c + 4) & 3) && (float)(unsigned char)e[3] * 0.003921569f < 0.66f) {
                        h = ((int (*)(void *, void *, int))cm_SendRelations)(*(int *)(pl + 4), *(int *)(c + 4), -5 - (util_RandomU(0x10) & 0xffff));
                        rounds++;
                        nrel++;
                    }
                }
                if (*(unsigned short *)((char *)e + 10) == *(unsigned short *)ai_Time) {
                    h = unk_Unk4f2330((unsigned short *)c, *((unsigned char *)e + 0xc), e[0], 0, &hv);
                    if (c == pl) {
                        h = unk_Unk4f0700((int *)c, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), 10, 0);
                    } else if (c[2] == 6 || c[2] == 7) {
                        memset(m, 0, 0xf8);
                        *(int *)(m + 4) = *(int *)(c + 4);
                        *(int *)(m + 0x28) = *(int *)ai_Time;
                        *(int *)(m + 0x2c) = *(int *)(ai_Time + 4);
                        *(int *)(m + 0x30) = *(int *)(ai_Time + 8);
                        *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                        m[0x36] = 9;
                        m[0] = (char)0x92;
                        *(int *)(m + 0x58) = *(int *)(o + 1);
                        *(int *)(m + 0x5c) = e[0];
                        m[0xb0] = (char)gv_EntryIndex(*(char **)(o + 0x65), e);
                        *(int *)(m + 0xb8) = 10;
                        *(int *)(m + 0xb4) = h;
                        h = cm_SendLongMessage(m);
                    } else {
                        k = (int)gm_Unk509ee0(c, ai_OwnPlayer);
                        if (h < k) {
                            h = unk_Unk4f0700((int *)c, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), 10, h);
                            rounds++;
                            paid++;
                            goto next;
                        }
                        h = unk_Unk4f0900(0, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e));
                    }
                    rounds++;
                }
            }
        }
    next:
        e = (int *)gv_FindEntry(0, (unsigned char *)o, (unsigned)e, gv_EntryIsUsed, 0xff);
    } while (1);
}

extern float ord_RatingRate;
extern char ord_StatisticsHistory[320];
extern char ord_RatingBytes[16];
extern char ord_MessageRecord[0x24];
const char ai_ZeroRot[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x5ebbb4 */
const float unk_Tab5ee034[] = { 0.800000012f, 0.699999988f, 0.600000024f, 0.5f, 15.0f, 12.5f, 10.0f, 7.5f };  /* 0x5ee034 */
const int gv_EntryCounts[] = { 1, 2, 4, 8 };  /* 0x5ee054, entries per kind */
const int unk_Tab5ee064[] = { 0, 12, 20, 30 };  /* 0x5ee064 */
const int unk_Tab5ee074[] = { 0, 16, 32, 64 };  /* 0x5ee074 */
const int unk_Tab5ee084[4][4] = { { 0, 8, 12, 16 }, { 0, 4, 6, 8 }, { 0, 0, 2, 4 }, { 0, 0, 0, 2 } };  /* 0x5ee084 */
const int unk_Tab5ee0f8[] = { 0, 160000, 320000, 640000, 1600000, 3200000, 6400000 };  /* 0x5ee0f8, experience thresholds per level */
char unk_Avatars[0x20][0xf4] = { { 110, 101, 86, 109, 77, 97, 118, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 102, 32, 82, 101, 97, 100, 105, 110, 103, 0, 0, 0, 0, 0, 0, 64, 32, 96, 96, 64, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, -128, 94, 68, 102, 102, 6, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, -102, -103, 41, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 41, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 22, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 35, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 71, 65, 82, 110, 82, 97, 110, 103, 97, 114, 111, 101, 107, 0, 0, 0, 0, 0, 0, 0, 76, 104, 117, 116, 105, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 10, 96, 64, 84, 0, 0, 0, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, -128, 94, 68, 102, 102, 6, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, -102, -103, 41, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 22, 64, 117, 2, 26, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 64, 98, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, 0, 92, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 100, 68, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 97, 68, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 41, 0, 1, 122, 105, 103, 101, 117, 110, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 101, 78, 87, 115, 83, 119, 105, 110, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 83, 101, 118, 101, 114, 105, 110, 103, 0, 0, 0, 0, 64, -128, 64, 32, 64, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 58, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 95, 88, 69, 68, 68, 101, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 111, 111, 109, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 25, 64, 48, 40, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 7, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 84, 65, 77, 95, 77, 97, 116, 116, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 101, 114, 32, 72, 101, 105, 108, 105, 103, 101, 0, 0, 0, 0, 0, 32, 0, 64, 32, -128, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 14, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 67, 73, 86, 42, 86, 105, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 114, 97, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -128, 32, 64, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 22, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 77, 103, 84, 115, 70, 114, 97, 110, 99, 111, 105, 115, 32, 100, 101, 0, 0, 0, 0, 0, 83, 97, 105, 110, 116, 45, 71, 101, 114, 109, 97, 105, 110, 0, 0, 0, 21, 0, 84, 21, 126, 0, 0, 0, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 6, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 19, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 6, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 32, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 64, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 51, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 0, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 113, 77, 76, 109, 68, 111, 110, 32, 65, 108, 102, 111, 110, 115, 111, 0, 0, 0, 0, 0, 100, 101, 32, 77, 101, 108, 109, 97, 113, 117, 101, 0, 0, 0, 0, 0, 42, 0, 63, 84, 42, 0, 0, 0, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 64, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 25, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 0, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 0, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 82, 70, 118, 87, 97, 108, 116, 104, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 70, -4, 114, 115, 116, 101, 110, 98, 101, 114, 103, 0, 42, 0, 21, 84, 105, 0, 0, 0, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 6, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 6, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 41, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 57, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, -51, -52, 44, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -82, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 0, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 67, 82, 101, 114, 82, 111, 119, 101, 110, 110, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, 114, 105, 107, 115, 115, 111, 110, 0, 0, 0, 0, 0, 0, 0, 0, 63, 84, 21, 42, 42, 0, 0, 0, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 44, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 54, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 25, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 100, 68, 0, 0, 32, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -95, 65, -98, -17, -89, 60, 0, -128, 99, 68, 36, 1, 0, 90, 105, 103, 101, 117, 110, 101, 114, 105, 110, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 100, 108, 103, 83, 116, 101, 102, 97, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 111, 108, 100, 98, 101, 114, 103, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 0, 42, 42, 0, 0, 0, -51, -52, 28, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 32, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 28, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 94, 68, 102, 102, 38, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 35, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 9, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -96, 65, -98, -17, -89, 60, 0, -128, 99, 68, 61, 0, 0, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 50, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 67, 112, 103, 65, 66, 111, 107, 116, 111, 114, 117, 115, 0, 0, 0, 0, 0, 0, 0, 0, 65, 112, 97, 103, 97, 99, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 126, 0, 126, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 64, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 60, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 64, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 0, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 57, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -80, 65, -98, -17, -89, 60, 0, -128, 99, 68, 42, 0, 0, 114, 97, 101, 117, 98, 101, 114, 109, 101, 105, 115, 116, 101, 114, 95, 83, 79, 76, 68, 65, 84, 0, 0, 0, 0, -1, -1, 0, 0 }, { 102, 76, 100, 119, 67, 97, 116, 104, 114, 105, 110, 103, 105, 110, 0, 0, 0, 0, 0, 0, 87, 105, 100, 111, 108, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 63, 0, 42, 42, 0, 0, 0, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 51, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 22, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, 102, 102, 6, 64, 52, -128, 55, 59, 0, -64, 100, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 67, 1, 0, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 105, 110, 51, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 72, 78, 115, 69, 103, 105, 110, 104, 97, 114, 100, 0, 0, 0, 0, 0, 0, 0, 0, 69, 105, 115, 101, 110, 104, 101, 98, 101, 114, 0, 0, 0, 0, 0, 0, 84, 63, 0, 21, 84, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 16, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 102, 102, 38, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 16, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 48, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 41, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 28, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 48, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 35, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 67, 0, 1, 119, 105, 114, 116, 51, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 103, 68, 108, 103, 71, 101, 116, 114, 117, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 111, 108, 100, 103, 105, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 63, 0, 21, 0, 0, 0, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 44, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 9, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 19, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 16, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 16, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -98, 65, -98, -17, -89, 60, 0, -128, 99, 68, 72, 1, 0, 66, 117, 101, 114, 103, 101, 114, 105, 110, 51, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 84, 86, 108, 104, 77, 97, 114, 99, 117, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 101, 108, 118, 101, 116, 105, 99, 117, 115, 0, 0, 0, 0, 0, 0, 84, 84, 21, 42, 21, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 44, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 41, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 28, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 12, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 64, 98, 68, -102, -103, 9, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 19, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -98, 65, -98, -17, -89, 60, 0, -128, 99, 68, 20, 0, 0, 98, 117, 101, 114, 103, 101, 114, 50, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 115, 108, 102, 83, 68, 97, 114, 105, 111, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 116, 117, 114, 109, 102, 101, 108, 115, 0, 0, 0, 0, 0, 0, 0, 0, 21, 42, 84, 105, 0, 0, 0, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 12, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 28, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 57, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 44, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 19, 64, -26, 29, 39, 59, 0, 0, 97, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -94, 65, -98, -17, -89, 60, 0, -128, 99, 68, 48, 0, 0, 98, 117, 101, 114, 103, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 76, 104, 99, 114, 65, 114, 109, 97, 110, 100, 32, 74, 101, 97, 110, 0, 0, 0, 0, 0, 100, 101, 32, 82, 105, 99, 104, 101, 108, 105, 101, 117, 0, 0, 0, 0, 42, 0, 84, 21, 105, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 51, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 28, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 60, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 60, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 16, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 100, 68, 0, 0, 16, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -85, 65, -98, -17, -89, 60, 0, -128, 99, 68, 13, 0, 0, 97, 98, 116, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 108, 71, 80, 83, 83, 97, 115, 99, 104, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 112, 105, 101, 103, 101, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 42, 42, 84, 0, 0, 0, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 12, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 38, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 38, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -82, 65, -98, -17, -89, 60, 0, -128, 99, 68, 27, 0, 0, 100, 105, 101, 98, 95, 77, 65, 78, 78, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 83, 116, 108, 119, 83, 97, 114, 105, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 87, 101, 108, 116, 101, 110, 115, 116, 101, 105, 110, 0, 0, 0, 0, 0, 42, 105, 42, 0, 63, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 60, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 60, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 12, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 57, 64, -96, -85, 45, 59, 0, -128, 94, 68, 102, 102, 38, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 28, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 6, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 35, 0, 1, 80, 97, 116, 114, 105, 122, 105, 101, 114, 95, 77, 65, 78, 78, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 115, 76, 98, 76, 84, 105, 114, 122, 97, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 105, 101, 98, 101, 110, 102, 101, 108, 115, 0, 0, 0, 0, 0, 0, 84, 105, 42, 0, 21, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 51, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 35, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 51, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 16, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 16, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 41, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -81, 65, -98, -17, -89, 60, 0, -128, 99, 68, 71, 1, 1, 80, 97, 116, 114, 105, 122, 105, 101, 114, 105, 110, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 69, 117, 81, 108, 75, 97, 103, 97, 105, 110, 101, 32, 71, 46, 32, 74, 46, 0, 0, 0, 76, 97, 113, 117, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 42, 84, 63, 0, 0, 0, -102, -103, 41, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 102, 102, 38, 64, 10, -41, 35, 59, 0, -64, 97, 68, -102, -103, 25, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 57, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 25, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 54, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 26, 1, 1, 66, 117, 101, 114, 103, 101, 114, 105, 110, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 83, 76, 118, 77, 68, 111, 114, 97, 104, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 97, 118, 101, 108, 105, 117, 115, 0, 0, 0, 0, 0, 0, 0, 0, 84, 105, 0, 21, 42, 0, 0, 0, -102, -103, 9, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 38, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 19, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 25, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 28, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 22, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 19, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 64, 0, 1, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 116, 83, 101, 70, 101, 114, 114, 97, 110, 100, 111, 0, 0, 0, 0, 0, 0, 0, 0, 69, 115, 116, 101, 98, 97, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 21, 105, 0, 84, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 12, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 48, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 57, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 16, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 60, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 32, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 54, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -90, 65, -98, -17, -89, 60, 0, -128, 99, 68, 1, 0, 1, 100, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 108, 78, 82, 98, 70, 114, 105, 101, 100, 114, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 66, 101, 114, 110, 111, 117, 108, 108, 105, 0, 0, 0, 0, 0, 0, 0, 105, 21, 21, 0, 126, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 6, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 6, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 19, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 57, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 32, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, -83, 65, -98, -17, -89, 60, 0, -128, 99, 68, 58, 0, 1, 80, 114, 105, 101, 115, 116, 101, 114, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 82, 98, 114, 66, 66, 101, 114, 110, 104, 97, 114, 116, 0, 0, 0, 0, 0, 0, 0, 0, 66, 105, 101, 114, 98, 114, 97, 117, 101, 114, 0, 0, 0, 0, 0, 0, 63, 42, 42, 21, 84, 0, 0, 0, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 25, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 6, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 100, 68, -51, -52, 12, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 8, 0, 0, 82, 97, 116, 115, 104, 101, 114, 114, 50, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 105, 76, 77, 71, 71, 105, 109, 108, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 90, 119, 101, 114, 103, 101, 110, 104, -10, 104, 0, 0, 0, 0, 0, 0, 42, 21, -87, 84, 21, 0, 0, 0, -51, -52, 28, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 51, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 49, 0, 0, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 50, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 78, 77, 114, 103, 71, 101, 114, 109, 97, 110, 105, 99, 117, 115, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 77, 97, 105, 108, 97, 110, 100, 0, 0, 0, 0, 0, 21, 84, 21, 84, 42, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 16, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 28, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 16, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 19, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 25, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 44, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -80, 65, -98, -17, -89, 60, 0, -128, 99, 68, 19, 0, 0, 98, 117, 101, 114, 103, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 116, 72, 110, 115, 65, 108, 99, 104, 97, 116, 114, 111, 110, 0, 0, 0, 0, 0, 0, 0, 69, 105, 115, 101, 110, 104, 97, 114, 116, 0, 0, 0, 0, 0, 0, 0, 42, 84, 0, 126, 0, 0, 0, 0, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 32, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 32, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 19, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 51, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, 0, 0, 64, 64, -26, 29, 39, 59, 0, 0, 97, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 53, 0, 0, 114, 97, 116, 115, 104, 101, 114, 114, 95, 83, 79, 76, 68, 65, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 82, 115, 109, 82, 71, 101, 114, 104, 97, 114, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 105, 101, 109, 101, 105, 115, 116, 101, 114, 0, 0, 0, 0, 0, 0, 105, 84, 0, 21, 42, 0, 0, 0, -72, 30, 21, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 35, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 99, 68, -82, 71, 49, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 28, 64, 52, -128, 55, 59, 0, -64, 100, 68, -10, 40, 28, 64, 88, 57, 52, 59, 0, 64, 98, 68, -102, -103, 41, 64, 117, 2, 26, 59, 0, 0, 92, 68, -72, 30, 21, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 22, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -82, 65, -98, -17, -89, 60, 0, -128, 99, 68, 25, 0, 1, 98, 117, 101, 114, 103, 101, 114, 51, 95, 68, 73, 67, 75, 69, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 77, 104, 68, 114, 74, 111, 104, 97, 110, 110, 101, 115, 32, 73, 46, 0, 0, 0, 0, 0, 118, 111, 110, 32, 82, -4, 100, 101, 115, 104, 101, 105, 109, 0, 0, 0, 84, 84, 21, 21, 42, 0, 0, 0, -82, 71, 49, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 38, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 0, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 28, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 41, 64, 52, -128, 55, 59, 0, -64, 100, 68, -72, 30, 21, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 22, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 22, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 35, 0, 1, 98, 97, 117, 101, 114, 95, 77, 65, 78, 78, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 110, 65, 84, 109, 87, 111, 116, 97, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 77, 101, 117, 99, 104, 101, 108, 116, 97, 110, 110, 0, 63, 21, 63, 21, 84, 0, 0, 0, -51, -52, 28, 64, 46, -112, 32, 59, 0, 0, 102, 68, 51, 51, 35, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 35, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 48, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 38, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 44, 64, 52, -128, 55, 59, 0, -64, 100, 68, -61, -11, 40, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 6, 64, 117, 2, 26, 59, 0, 0, 92, 68, -51, -52, 44, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 9, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -87, 65, -98, -17, -89, 60, 0, -128, 99, 68, 51, 0, 0, 98, 117, 101, 114, 103, 101, 114, 51, 95, 68, 73, 67, 75, 69, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 } };  /* 0x60f310 */
char unk_Tab611258[] = { 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 24, 1, -1, 0, 0, 14, 73, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 2, -1, 0, -64, 15, 73, 0, 2, 3, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 26, 1, 0, 0, 0, 18, 73, 0, 3, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 18, 1, 1, 0, -32, 18, 73, 0, 4, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 10, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 22, 1, 2, 0, 48, 20, 73, 0, 5, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 14, 1, -1, 0, 96, 21, 73, 0, 6, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 10, 1, -1, 0, -16, 21, 73, 0, 7, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 11, 1, -1, 0, 80, 22, 73, 0, 8, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 27, 1, 3, 0, -128, 43, 73, 0, 9, 0, 0, 0, 8, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 15, 1, -1, 0, -48, 22, 73, 0, 10, 0, 0, 0, 6, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 15, 2, -1, 0, 96, 24, 73, 0, 11, 0, 0, 0, 6, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 21, 1, -1, 0, 64, 26, 73, 0, 12, 0, 0, 0, 5, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 12, 1, -1, 0, 32, 27, 73, 0, 13, 0, 0, 0, 10, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 14, 2, -1, 0, -16, 27, 73, 0, 14, 0, 0, 0, 10, 0, 0, 0, 25, 0, 0, 0, 3, 0, 0, 0, 10, 0, 0, 0, 5, 0, 0, 0, 16, 0, 0, 0, 16, 1, 4, 0, -80, 28, 73, 0, 15, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 25, 1, -1, 0, -128, 29, 73, 0, 16, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 13, 1, 5, 0, -128, 30, 73, 0, 17, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 25, 2, 6, 0, -80, 31, 73, 0, 18, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 18, 1, 7, 0, -64, 32, 73, 0, 19, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 19, 1, 8, 0, -48, 33, 73, 0, 20, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 19, 2, 9, 0, -32, 34, 73, 0, 21, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 17, 1, 10, 0, 16, 36, 73, 0, 22, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 20, 1, 11, 0, 64, 37, 73, 0, 23, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 23, 1, 12, 0, -128, 38, 73, 0, 24, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 22, 2, 13, 0, -64, 39, 73, 0, 25, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 23, 2, 14, 0, 0, 41, 73, 0, 26, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 20, 2, 15, 0, 64, 42, 73, 0, 27, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 26, 2, 16, 0, 80, 45, 73, 0, 28, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 12, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 27, 2, 17, 0, -112, 46, 73, 0 };  /* 0x611258 */
int (*ord_OrderHandlers[0x3a])(char *c, int *m) = { ord_ApplyForOffice, ord_LevelOrder, unk_Unk4f5070, ord_SpyTargetOrder, ord_BlackmailBuildingOrder, ord_Level3Order, ord_EspionageResultOrder, ord_BlackmailContactVariantOrder, ord_BlackmailContactOrder, ord_Level4Order, ord_Level4Order2, ord_Level4To5Order, ord_Level5UpOrder, ord_Level5UpOrder2, unk_Unk4f5800, ord_Level6UpOrder, ord_Level6UpOrder2, ord_WageReportOrder, ord_EmbezzlementOrder, ord_LawOrder, ord_LawOrder2, ord_Job11Order, ord_JobMultiOrder, ord_Job28Order, ord_Job29Order, ord_Job34Order, ord_JobRangeOrder, ord_JobMultiOrder2, ord_EmbezzlementContractOrder, ord_CounterEspionageOrder, ord_Job17Order, ord_Job17Order2, ord_JobMultiOrder4, ord_MisappropriationOrder, ord_Job10Order, ord_Job15Order, ord_CounterEspionageWindowOrder, ord_EspionageWindowOrder, ord_JobMultiOrder3, ord_EspionagePaymentOrder, ord_EspionageOrder, ord_EspionagePaymentOrder2, ord_EspionageRelationOrder, ord_EspionageAmountOrder, ord_SpyTargetFlagOrder, ord_EspionageOverviewOrder, ord_Level7Order, ord_Level8Order, ord_Level9UpOrder, ord_InterrogationOrder, ord_DismissalOrder, ord_Flag800Order, ord_Flag800Order2, ord_InterrogationOrder2, ord_Flag200Order, ord_InterrogationOrder3, ord_Flag400Order, ord_DismissWorkerOrder };  /* 0x611768, the order handler of each order number */
char ord_KindTable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 50, 0, 0, 0, 0, 0, 12, 0, 75, 0, 0, 0, 0, 0, 20, 0, 125, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 3, 0, 0, 0, 15, 0, 0, 0, 9, 0, 0, 0, 25, 0, 0, 0, 16, 0, 0, 0, 20, 0, 6, 0, 20, 0, 0, 0, 30, 0, 9, 0, 60, 0, 0, 0, 45, 0, 12, 0, 110, 0, 0, 0, 30, 0, 0, 0, 10, 0, 0, 0, 45, 0, 0, 0, 30, 0, 0, 0, 70, 0, 0, 0, 60, 0, 0, 0, 10, 0, 0, 0, 10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0, 25, 0, 0, 0, 60, 0, 0, 0, 30, 0, 3, 0, 15, 0, 0, 0, 45, 0, 6, 0, 50, 0, 0, 0, 80, 0, 9, 0, 120, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 0, 0, 15, 0, 0, 0, 50, 0, 0, 0, 35, 0, 0, 0, 75, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 100, 0, 0, 0, 20, 0, 0, 0, 30, 0, 12, 0, 30, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 8, 0, 20, 0, 0, 0, 50, 0, 10, 0, 40, 0, 0, 0, 75, 0, 12, 0, 80, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 4, 0, 25, 0, 0, 0, 50, 0, 5, 0, 40, 0, 0, 0, 80, 0, 6, 0, 70, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };  /* 0x617468 */
AITIME gv_LastTime = {0};  /* 0x6f0e38, the game time ai_Time of the last effect step (gv_UpdateEffects) */
GVEFFARG gv_EffectArgs[8] = {0};  /* 0x6f0e48, the argument of each effect slot's default callback */
char gv_Effects[2944] = {0};  /* 0x6f1008, 8 slots of */
int ai_RatingBase = 0;  /* 0x6f1b9c, the rating's old value (stored as bits; ai.h's ai_Rating follows it at */
struct AICHAR *ai_JudgeCatholic = 0;  /* 0x6f1bfc / */
struct AICHAR *ai_JudgeProtestant = 0;  /* 0x6f1c00 */
/* ai_Unk7e3f94: view of ai_PlayerSlots (tools/data_views.py) */
float ord_RatingRate = {0}; /* 0x91aaf4, the change rate of the rating (ord.c */
char ord_StatisticsHistory[320] = {0}; /* 0x91ab00 */
char ord_RatingBytes[16] = {0};  /* 0x91b460, 5 + 1 bytes */
char ord_MessageRecord[0x24] = {0}; /* 0x91b480, the 9-int record the messages carry, then the tag */
char gm_IncomeHistory[0x154] = {0};  /* 0x91b580, the 17-round history of the five income sums (rows of */
GVCANNON gv_Cannon = {0};  /* 0x925100, the cannon scene state */
