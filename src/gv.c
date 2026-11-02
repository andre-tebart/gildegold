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
   A graveyard building (kind 0x1d) holds a GVYARD (AIBLD.yard): four grave sizes, each a list of GVENTRY entries opened by
   GVYARD.open, counted per type level by gv_ListEntries / gv_EntryCount / gv_GraveCount and walked by gv_FindEntry; each
   entry points at its GVGRAVE records (a dead person). Graveyard orders are GVORDER records (message 0x60, sent by the
   gv_Order* senders through cm_SendGraveOrder, run by type: gv_BuryCharacter, gv_ReserveGrave, gv_ExtendGrave,
   gv_SetGraveHolder, gv_OpenSize, gv_AdjustEntry, ..); a grave move is GVMOVE (message 0x61, gv_OrderMoveGrave ->
   gv_MoveGrave), a raising of a dead person GVRAISE (message 0x62, gv_OrderRaiseDead -> gv_RaiseDead): the raised dead are
   kind 0x14 characters living at the graveyard (gv_RoomForRaised, decaying by gv_DecayRaisedDead); gv_GraveyardTurn is the
   building's turn step (care wear, lease ends via gv_LeasePrice / gv_OrderExtendGrave, new dead).
   The AI decision (law) table 0x611258 (unk_Tab611258, a char initializer read as 0x1d AIDEC records through gv_Decisions)
   is read and set by gv_GetTableRecord / gv_SetTableRecord (message 0x46, GVDECMSG, gv_OnDecisionValue); breaking a law
   is a crime (gv_Accuse -> cm_SendCrime): the CRIME records ai_Crimes (cm.c, viewed as gv_CrimesT) and the evidence pairs
   ai_Evidence, saved by gv_WriteCrimes / gv_ReadCrimes.
   Open: the avatars, the effect slots, the graveyard and the crimes may be several modules sharing one .data block.
   The crime records' life: gv_FreeCrimeSlot / gv_FindCrime find them, gv_HandOverCrimes (message 0x3c) hands them over,
   gv_RetireCrime (message 0x23, sent by gv_SendCrimes) frees them; gv_AddEvidence / gv_EvidenceSlot keep the evidence
   pairs; gv_CulpritGuilt sums AIDEC.guilt of a culprit's open crimes; gv_RetireCrimesOf / gv_RetireCrimeIds retire them
   and tell the evidence holders; gv_HeldCrimesOf / gv_HeldCrimeIdsOf / gv_HeldCrimeCount and gv_RankCulprits /
   gv_RankCulpritsAll read what a character holds evidence of; gv_CrimeText (by law: gv_CrimeTextLaw2to8 / 9to16 /
   17to28) words a crime.
   Open: gv_Graveyard (0x91f700) and unk_Tab91b6e0 are second names of ai_Crimes / ai_Evidence (the crime records, not
   graves); the functions from 0x4f41b0 on still read them through raw offsets; unk_Tab611258's initializer holds the
   callbacks (AIDEC.fn) as raw original addresses.
   Open: some functions index ai_PlayerSlots with the stride 0x5e8 where gv_ReserveGrave shows 0x2f4 (gv_PlayerRecs);
   gv_FindEntry's callback is an unprototyped int (*)() (its callbacks take differing pointer types).
   Open: head: cm.h redeclares gv functions (gv_GrowGraveyard as struct GVYARD *); cm.c cannot include gv.h yet (two
   calls conflict: gv_CrimeText, unk_Unk4f4460).
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
#define gv_Decisions (*(AIDEC (*)[0x1d])&unk_Tab611258)  /* 0x611258, the AI decision table (char initializer) as its 0x1d AIDEC records */
#define gv_CrimesT (*(CRIME (*)[0x200])&ai_Crimes)  /* 0x91f700, ai_Crimes (cm.c: char[0x5a00]) as its 0x200 CRIME records */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define GAPDATA __declspec(align(1)) unsigned char
#define gv_EffectsT (*(GVEFFECT (*)[8])&gv_Effects)  /* 0x6f1008, gv_Effects (char[0xb80]) as the 8 GVEFFECT slots */
#define gv_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is */
#define gv_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
typedef struct GVPLAYERREC {           /* 0x2f4 bytes, a player's slot record (ai_PlayerSlots, indexed by ai_OwnPlayer) */
    char pad00[0x54];
    unsigned short currency;            /* 0x54, the currency good, as gm_AddNegative / gm_AddObjekt take it */
    char pad56[0x29e];
} GVPLAYERREC;
#define gv_PlayerRecs (*(GVPLAYERREC (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (char[4][0x2f4]) as the player records */

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
#include "net.h"
extern int gm_PlayerRecords[688];

extern const float gv_SaleShares[];
extern const float gv_GraveWear[];

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

int gv_GravesInUse(unsigned char *o)
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
/* 0x4ef470, the number of grave entries (GVYARD.entries, 0x14 bytes each) the graveyard building o has in use, as gv_GraveCount
   by its type's level and GVYARD.open: level 1: 8, 12 with open[1]; level 2: 12 / 18 with open[1], + 2 with open[2]; else
   16 / 24 with open[1], + 4 with open[2], + 2 with open[3] (each open size adds its entries, gv_AllocGraveyard) */

int gv_EntryCount(unsigned char *o)
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
            return 0xc;
    } else if (((AIBTYPE *)bt)->level == 2) {
        n = 0xc;
        if (g->open[1] == 1)
            n = 0x12;
        if (g->open[2] == 1)
            n = n + 2;
    } else {
        n = 0x10;
        if (g->open[1] == 1)
            n = 0x18;
        if (g->open[2] == 1)
            n = n + 4;
        if (g->open[3] == 1)
            n = n + 2;
    }
    return n;
}
/* 0x4ef4f0, 1 unless the part i (0 .. 3) of the record r is missing (the pointer at +8 + 4 * i is 0 or the flag byte at +0x18 + i is 0) */

int gv_SizeOpen(char *r, int i)
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
/* 0x4ef570, the entry n (0x14 bytes) of the list i (0 .. 3, the grave size) of the graveyard record r, when that size is open
   (gv_SizeOpen) and n is below the size's entry count for the record's kind (list 0: 8 / 12 / 16, list 1: 4 / 6 / 8,
   list 2: - / 2 / 4, list 3: - / - / 2 for kind 1 / 2 / 3); 0 when r->kind is not 1 .. 3 or n is out of range */

int gv_ListEntry(char *r, int i, unsigned char n)
{
    GVYARD *g = (GVYARD *)r;
    int kind;

    kind = g->kind;
    if (kind < 1 || kind > 3)
        return 0;
    if (i == 0) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1 && n >= 8)
            return 0;
        if (kind == 2 && n >= 0xc)
            return 0;
        if (kind == 3 && n >= 0x10)
            return 0;
        return (int)&g->lists[0][n];
    }
    if (i == 1) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1 && n >= 4)
            return 0;
        if (kind == 2 && n >= 6)
            return 0;
        if (kind == 3 && n >= 8)
            return 0;
        return (int)&g->lists[1][n];
    }
    if (i == 2) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1)
            return 0;
        if (kind == 2 && n >= 2)
            return 0;
        if (kind == 3 && n >= 4)
            return 0;
        return (int)&g->lists[2][n];
    }
    if (i == 3) {
        if (gv_SizeOpen(r, i) == 0)
            return 0;
        if (kind == 1)
            return 0;
        if (kind == 2)
            return 0;
        if (kind == 3 && n >= 2)
            return 0;
        return (int)&g->lists[3][n];
    }
    return 0;
}
/* 0x4ef6b0, the index of the entry e (GVENTRY, its size the list 0 .. 3) within its list of the graveyard record r (GVYARD.lists); -1 for another list */

int gv_EntryIndex(char *r, int *e)
{
    GVYARD *g = (GVYARD *)r;
    GVENTRY *en = (GVENTRY *)e;

    if (en->size == 0)
        return en - g->lists[0];
    if (en->size == 1)
        return en - g->lists[1];
    if (en->size == 2)
        return en - g->lists[2];
    if (en->size == 3)
        return en - g->lists[3];
    return -1;
}
/* 0x4ef710, 1 when the +8 word of b is the +0x50 word of a and not 0x7fff */

int gv_EntryHeldBy(char *a, char *b)
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
/* 0x4ef7c0, the first entry (GVENTRY) of the list l (the grave size 0 .. 3; 0xff: all entries, GVYARD.entries) of the
   graveyard building o after the entry `after` (0: from the start) for which cb (a, entry) is true (no cb: any); the list
   ends after gv_ListEntries[l][level] entries (0xff: gv_EntryCount); 0 when none or the size is not open (gv_SizeOpen).
   after walks as the cursor (the original keeps the two in one register) */

unsigned gv_FindEntry(void *a, unsigned char *o, unsigned after, int (*cb)(), int l)
{
    AIBLD *bld = (AIBLD *)o;
    char *bt;
    GVYARD *g;
    GVENTRY *end;

    bt = ai_BTypes + bld->type + bld->type * 0x24c;
    g = bld->yard;
    if (l != 0xff && gv_SizeOpen((char *)g, l) == 0)
        return 0;
    if (l == 0)
        end = g->lists[0] + gv_ListEntries[0][((AIBTYPE *)bt)->cap247];
    else if (l == 1)
        end = g->lists[1] + gv_ListEntries[1][((AIBTYPE *)bt)->cap247];
    else if (l == 2)
        end = g->lists[2] + gv_ListEntries[2][((AIBTYPE *)bt)->cap247];
    else if (l == 3)
        end = g->lists[3] + gv_ListEntries[3][((AIBTYPE *)bt)->cap247];
    else if (l == 0xff)
        end = g->entries + gv_EntryCount(o);
    else
        return 0;
    if (after == 0) {
        if (l == 0)
            after = (unsigned)g->lists[0];
        else if (l == 1)
            after = (unsigned)g->lists[1];
        else if (l == 2)
            after = (unsigned)g->lists[2];
        else if (l == 3)
            after = (unsigned)g->lists[3];
        else
            after = (unsigned)g->entries;
    } else
        after += sizeof(GVENTRY);
    for (; after < (unsigned)end; after += sizeof(GVENTRY)) {
        if (cb == 0 || cb(a, after) != 0)
            return after;
    }
    return 0;
}
/* 0x4ef900, the first entry of the list 0xff of the object o whose +4 word is the callback's (0x4ef780) match for the entry e; 0 when e is 0 or e[0] is -1 */

unsigned gv_LoadGraveyard(unsigned char *o, int *e)
{
    if (e != 0 && e[0] != -1)
        return gv_FindEntry(e, o, 0, gv_EntryHasId, 0xff);
    return 0;
}
/* 0x4ef930, a random used grave (GVYARD.graves, first word not -1) of the graveyard building o, searched cyclically from a
   random start among its gv_GraveCount small graves; 0 when none or o is no graveyard (kind 0x1d) of level 1 .. 3 */

GVGRAVE *gv_RandomGrave(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVYARD *g;
    int n, k, i;

    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    g = bld->yard;
    if (bt->kind == 0x1d && bt->cap247 >= 1 && bt->cap247 <= 3) {
        n = gv_GraveCount(o);
        i = util_RandomU(n);
        k = n;
        if (n != 0) {
            do {
                if (g->graves[i].id != -1)
                    return &g->graves[i];
                k--;
                i = (i + 1) % n;
            } while (k != 0);
        }
    }
    return 0;
}
/* 0x4ef9c0, buries up to n new dead persons in the free entries of the smallest graves (list 0, gv_EntryIsFree) of the
   graveyard building o (kind 0x1d, level 1 .. 3): each grave gets a new object id, random traits, values and year, a random
   family and first name by sex and an icon from its gm_NameRecord; returns how many were buried (0 for no graveyard)
   @deep Read in full against the original (all 185 instructions reconciled): statements, field stores, the
   traits fpu loop, the icon do-while and the name strncpys match instruction for instruction. Tried: early
   return 0 first, the whole body inside the kind/level if (kept), i = 0 before the if (worse, fpu hunks).
   Remaining only the callee-saved register pushes/pops: the original pushes ebx/esi/edi in the prologue
   (+0x0b..+0x10) and pops them on the return-0 path (+0x241: pop edi; pop esi; xor eax,eax; pop ebx), ours
   pushes them after the kind/level check (+0x46) and returns 0 without pops; the [esp+0x10] vs [esp+0x1c]
   load of n is the same slot shifted by those pushes. Prologue placement is allocation, not meaning. */

int gv_BuryNewDead(unsigned char *o, int n)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVENTRY *e;
    unsigned *rec;
    int i, k, r;

    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    if (bt->kind == 0x1d && bt->cap247 >= 1 && bt->cap247 <= 3) {
        for (i = 0; i < n; i++) {
            e = (GVENTRY *)gv_FindEntry(0, o, 0, gv_EntryIsFree, 0);
            if (e == 0)
                break;
            e->grave->id = ai_NextObjectId;
            ai_NextObjectId++;
            e->id = e->grave->id;
            e->w08 = 0x7fff;
            e->until = (unsigned short)(gv_TimeT.year + 5);
            e->b0c = 0xff;
            for (k = 0; k < 5; k++)
                e->grave->traits[k] = (unsigned char)(int)((float)util_RandomU(8) * 21.0f + 42.0f);
            e->grave->v2c = util_RandomU(9) + 1;
            e->grave->v30 = util_RandomU(2) + 1;
            e->grave->nameId = util_RandomU(0x5e) + 1;
            e->grave->year = gv_TimeT.year - util_RandomU(4) - 1;
            e->grave->sex = util_RandomU(2);
            strncpy(e->grave->family, txt_FamilyNames[util_RandomU(0x95)], 0xf);
            rec = gm_NameRecord(e->grave->sex, e->grave->nameId);
            e->grave->icon = 0xff;
            r = util_RandomU(4);
            k = 4;
            do {                                    /* one of the record's four words +0x34 that is set, from a random one */
                if (rec[0xd + r] != 0xff) {
                    e->grave->icon = rec[0xd + r] + 0x639;
                    break;
                }
                k--;
                r = (r + 1) % 4;
            } while (k != 0);
            if (e->grave->sex == 0)
                strncpy(e->grave->first, txt_MaleFirstNames[util_RandomU(0xbf)], 0xf);
            else
                strncpy(e->grave->first, txt_FemaleFirstNames[util_RandomU(0x70)], 0xf);
        }
        return i;
    }
    return 0;
}
/* 0x4efc10, allocates the graveyard record (GVYARD, "gv_AllocGraveyardField") of the graveyard building o (kind 0x1d, level
   1 .. 3) with its entries (gv_EntryTotals, "gv_Alloc:_all_graves") and graves (gv_GraveTotals, all free,
   "gv_Alloc:_all_zombies"); only the smallest size is open. The entries are laid out size by size, each pointing at its
   grave (0x44 << size bytes): level 1: 8 / 4, level 2: 12 / 6 / 2, level 3: 16 / 8 / 4 / 2 graves of the sizes 0 .. 3.
   Stores it as AIBLD.yard and returns it; 0 when out of memory or o is no graveyard
   @deep Read in full against the original (all 266 instructions reconciled, same count): the kind/level
   check, the three allocations with their memsets, the free marks, the lists/open setup and the per-level
   entry loops (gr advanced before e, as the original) match instruction for instruction. Remaining: (1)
   the register pair esi/ebp swapped (the original keeps g in esi and the type record in ebp; ours the
   reverse, which also makes ours 9 bytes larger through [ebp] encodings); with --regs esi=ebp,ebp=esi only
   (2) is left: the out-of-memory cleanup block (free entries, free graves, free g, return 0: 18
   instructions) that the original places inline after the graves allocation (jne to the init at +0x131)
   and ours at the end (+0x313); tried the cleanup behind a goto to the init and an entries && graves test
   before it (same or worse, VC6 lays the block out the same). Block placement and allocation only. */

GVYARD *gv_AllocGraveyard(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVYARD *g;
    GVENTRY *e;
    GVGRAVE *gr;
    int i;

    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    if (bt->kind == 0x1d && bt->cap247 >= 1 && bt->cap247 <= 3) {
        g = (GVYARD *)mem_Alloc(sizeof(GVYARD), "gv_AllocGraveyardField");
        if (g == 0)
            return 0;
        memset(g, 0, sizeof(GVYARD));
        g->id = bld->id;
        g->kind = bt->cap247;
        g->entries = (GVENTRY *)mem_Alloc(gv_EntryTotals[bt->cap247] * sizeof(GVENTRY), "gv_Alloc:_all_graves");
        if (g->entries != 0) {
            memset(g->entries, 0, gv_EntryTotals[bt->cap247] * sizeof(GVENTRY));
            g->graves = (GVGRAVE *)mem_Alloc(gv_GraveTotals[bt->cap247] * sizeof(GVGRAVE), "gv_Alloc:_all_zombies");
            if (g->graves != 0) {
                memset(g->graves, 0, gv_GraveTotals[bt->cap247] * sizeof(GVGRAVE));
                for (i = 0; i < gv_GraveTotals[bt->cap247]; i++)
                    g->graves[i].id = -1;
                e = g->entries;
                gr = g->graves;
                g->lists[0] = e;
                g->open[0] = 1;
                g->open[1] = 0;
                g->open[2] = 0;
                g->open[3] = 0;
                if (g->kind == 1) {
                    g->lists[1] = e + 8;
                    g->lists[2] = 0;
                    g->lists[3] = 0;
                    for (i = 0; i < 8; i++, gr += 1, e++) {
                        e->size = 0; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 4; i++, gr += 2, e++) {
                        e->size = 1; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                } else if (g->kind == 2) {
                    g->lists[1] = e + 12;
                    g->lists[2] = e + 18;
                    g->lists[3] = 0;
                    for (i = 0; i < 12; i++, gr += 1, e++) {
                        e->size = 0; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 6; i++, gr += 2, e++) {
                        e->size = 1; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 2; i++, gr += 4, e++) {
                        e->size = 2; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                } else {
                    g->lists[1] = e + 16;
                    g->lists[2] = e + 24;
                    g->lists[3] = e + 28;
                    for (i = 0; i < 16; i++, gr += 1, e++) {
                        e->size = 0; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 8; i++, gr += 2, e++) {
                        e->size = 1; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 4; i++, gr += 4, e++) {
                        e->size = 2; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                    for (i = 0; i < 2; i++, gr += 8, e++) {
                        e->size = 3; e->grave = gr; e->id = -1; e->w08 = 0xffff;
                    }
                }
                bld->yard = g;
                return g;
            }
        }
        if (g->entries != 0) {
            mem_Free(g->entries);
            g->entries = 0;
        }
        if (g->graves != 0) {
            mem_Free(g->graves);
            g->graves = 0;
        }
        mem_Free(g);
    }
    return 0;
}
/* 0x4eff50, moves the graveyard building o to the record of its new level (the type's level one above GVYARD.kind, which
   is below 3): allocates the larger record (gv_AllocGraveyard), copies the graves of the old sizes and the entries' state
   (all but their grave pointers) over, takes the open flags and frees the old record (gv_FreeGraveyard); returns the new
   record, or 0 (the old one stays) when o does not qualify or there is no memory
   @deep Read in full against the original (all 188 instructions reconciled): the qualification test, the
   alloc / restore, the per-level counts and grave memcpys (rep movsd 0x88 / 0xcc / 0xcc / 0x88), the three
   field-by-field entry copy loops, the open flags and the free match statement for statement. Remaining:
   the frame (original sub esp,0x10 with n2 at [esp+0x18]; ours push ecx with n2 at [esp+0x10]), the o /
   zero pair in ebp / edi swapped (ours edi / ebp), the old->lists[0]->grave load the original hoists above
   the level test while ours loads nw->lists[0] first, and the base/index order in the sib stores of the
   first copy loop ([edx+esi+4] vs [esi+edx+4]). Tried separate loop indices per list (no change) and s/d
   grave pointers set before the level test (worse: 146 code). Frame, allocation and scheduling only. */

GVYARD *gv_GrowGraveyard(unsigned char *o)
{
    AIBLD *bld = (AIBLD *)o;
    AIBTYPE *bt;
    GVYARD *old, *nw;
    int n0, n1, n2, i;

    old = bld->yard;
    bt = (AIBTYPE *)(ai_BTypes + bld->type + bld->type * 0x24c);
    if (bt->kind == 0x1d && old != 0 && old->kind < 3 && bt->cap247 - old->kind == 1) {
        bld->yard = 0;
        nw = gv_AllocGraveyard(o);
        if (nw == 0) {
            bld->yard = old;
            return 0;
        }
        if (bt->cap247 == 2) {                  /* from level 1: 8 graves of size 0, 4 of size 1 */
            n2 = 0;
            n0 = 8;
            n1 = 4;
            memcpy(nw->lists[0]->grave, old->lists[0]->grave, 8 * sizeof(GVGRAVE));
            memcpy(nw->lists[1]->grave, old->lists[1]->grave, 4 * 2 * sizeof(GVGRAVE));
        } else {                                /* from level 2: 12 / 6 / 2 graves of the sizes 0 .. 2 */
            n2 = 2;
            n0 = 12;
            n1 = 6;
            memcpy(nw->lists[0]->grave, old->lists[0]->grave, 12 * sizeof(GVGRAVE));
            memcpy(nw->lists[1]->grave, old->lists[1]->grave, 6 * 2 * sizeof(GVGRAVE));
            memcpy(nw->lists[2]->grave, old->lists[2]->grave, 2 * 4 * sizeof(GVGRAVE));
        }
        if (old->lists[0] != 0) {
            for (i = 0; i < n0; i++) {
                nw->lists[0][i].size = old->lists[0][i].size;
                nw->lists[0][i].id = old->lists[0][i].id;
                nw->lists[0][i].w08 = old->lists[0][i].w08;
                nw->lists[0][i].until = old->lists[0][i].until;
                nw->lists[0][i].b0c = old->lists[0][i].b0c;
            }
        }
        if (old->lists[1] != 0) {
            for (i = 0; i < n1; i++) {
                nw->lists[1][i].size = old->lists[1][i].size;
                nw->lists[1][i].id = old->lists[1][i].id;
                nw->lists[1][i].w08 = old->lists[1][i].w08;
                nw->lists[1][i].until = old->lists[1][i].until;
                nw->lists[1][i].b0c = old->lists[1][i].b0c;
            }
        }
        if (old->lists[2] != 0) {
            for (i = 0; i < n2; i++) {
                nw->lists[2][i].size = old->lists[2][i].size;
                nw->lists[2][i].id = old->lists[2][i].id;
                nw->lists[2][i].w08 = old->lists[2][i].w08;
                nw->lists[2][i].until = old->lists[2][i].until;
                nw->lists[2][i].b0c = old->lists[2][i].b0c;
            }
        }
        nw->open[0] = old->open[0];
        nw->open[1] = old->open[1];
        nw->open[2] = old->open[2];
        nw->open[3] = old->open[3];
        gv_FreeGraveyard((int *)old);
        return bld->yard;
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
        return ((int (*)(void *))cm_SendGraveOrder)(m);
    }
    return -1;
}
/* 0x4f0240, graveyard order (p: +4 the character, +8 the graveyard building, +0xc the grave size, +0x10 the entry): buries
   the dead character in the graves of that entry, in the first free grave or else the one buried longest ago (GVGRAVE.year):
   its id, names, skills, offices, sex and portrait and the burial year (ai_Time); returns 0, or a negative reason: -1 no
   building or one without owner or the player's own, -2 no graveyard, -3 no graveyard record, -4 the size is not open,
   -6 no character, -7 no such entry or its w08 is not the character's AICHAR.w050 */

int gv_BuryCharacter(int *p)
{
    AIBLD *bld;
    GVYARD *g;
    AICHAR *c;
    GVENTRY *e;
    GVGRAVE *lst, *best, *q;
    int i, list;
    char *s, *d;
    char ch;

    gm_FindById((int *)&bld, 0, 0, p[2]);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    if (gv_SizeOpen((char *)g, p[3]) == 0)
        return -4;
    gm_FindById(0, 0, (int *)&c, p[1]);
    if (c == 0)
        return -6;
    list = p[3];
    e = (GVENTRY *)gv_ListEntry((char *)g, list, (unsigned char)p[4]);
    if (e == 0 || e->w08 != c->w050)
        return -7;
    lst = e->grave;
    best = lst;
    q = lst;
    for (i = 0; i < gv_EntryCounts[list]; i++, q++) {
        if (best->id == -1)
            break;
        if (q->id == -1) {
            best = &lst[i];
            break;
        }
        if (best->year > q->year)
            best = q;
    }
    best->id = c->netId;
    s = c->first;
    d = best->first;
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    s = c->family;
    d = best->family;
    do {
        ch = *s++;
        *d++ = ch;
    } while (ch != 0);
    memcpy(best->traits, c->skills, sizeof(best->traits));
    best->v2c = c->prevOffice;
    best->v30 = c->b00d;
    best->nameId = c->jobId;
    best->sex = c->sex;
    best->year = gv_TimeT.year;
    best->icon = c->picture;
    return 0;
}
/* 0x4f0430, sends graveyard order type 1 (GVORDER, cm_SendGraveOrder): reserve a grave of the size `size` of the graveyard
   building o for the living character c (c[1]: its AICHAR.netId; the entry is left to the handler, 0xff) for `years` years at
   the price `price`; returns the sender's result, -1 when c or o is 0 */

int gv_OrderReserveGrave(int *c, char *o, int size, short years, int price)
{
    GVORDER m;

    if (c != 0 && o != 0) {
        m.chr = c[1];
        m.bld = ((AIBLD *)o)->id;
        m.size = size;
        m.type = 1;
        m.entry = 0xff;
        m.years = years;
        m.price = price;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0490, graveyard order type 1 (GVORDER m, sent by gv_OrderReserveGrave): reserves the first free entry of the size
   m->size of the graveyard building m->bld for the living character m->chr: the entry gets the character's netId and w050,
   runs until ai_Time.year + m->years, its graves are freed; with a price the buyer pays it (gm_AddNegative), the owner gets
   the size's share of it (gv_SaleShares, gm_AddObjekt) and an owner that is a player character (kind 6) other than the
   buyer is told (ai_NotifySale). Returns 0, or a negative reason as gv_BuryCharacter (-5 the size is out of range, -7 no
   free entry) */

int gv_ReserveGrave(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c, *owner;
    GVENTRY *e;
    int i;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    if (gv_SizeOpen((char *)bld->yard, m->size) == 0)
        return -4;
    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    if (m->size >= 0 && m->size <= 3) {
        e = (GVENTRY *)gv_FindEntry(c, (unsigned char *)bld, 0, gv_EntryIsFree, m->size);
        if (e == 0)
            return -7;
        e->id = c->netId;
        e->w08 = c->w050;
        e->until = m->years + (unsigned short)gv_TimeT.year;
        e->b0c = 0xff;
        for (i = 0; i < gv_EntryCounts[m->size]; i++)
            e->grave[i].id = -1;
        if (m->price > 0) {
            gm_AddNegative(c->netId, gv_PlayerRecs[ai_OwnPlayer].currency, m->price);
            gm_AddObjekt(gv_CharsT[bld->owner].netId, gv_PlayerRecs[ai_OwnPlayer].currency,
                         (int)(m->price * gv_SaleShares[m->size]));
            owner = &gv_CharsT[bld->owner];
            if (c != owner && gv_CharsT[bld->owner].kind == 6)
                ai_NotifySale((int)owner, (unsigned short *)c, (int)bld, m->size, m->price,
                              (int)(m->price * gv_SaleShares[m->size]));
        }
        return 0;
    }
    return -5;
}
/* 0x4f0700, the message 0x4c60b0 type 2 (graveyard id +4 of g, object id +1 of o, value v, byte b, short s, int x); -1 when g or o is 0 or b is -1 */

int gv_OrderExtendGrave(int *g, char *o, int v, int b, short s, int x)
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
        return ((int (*)(void *))cm_SendGraveOrder)(m);
    }
    return -1;
}
/* 0x4f0770, graveyard order type 2 (GVORDER m): extends the lease of the entry m->entry of the size m->size of the graveyard
   building m->bld by m->years (GVENTRY.until); with a price the character m->chr pays it (gm_AddNegative) and the owner
   gets the size's share of it (gv_SaleShares, gm_AddObjekt). Returns 0, or a negative reason as gv_BuryCharacter (-7 no
   such entry) */

int gv_ExtendGrave(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVYARD *g;
    GVENTRY *e;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    if (gv_SizeOpen((char *)g, m->size) == 0)
        return -4;
    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    e = (GVENTRY *)gv_ListEntry((char *)g, m->size, m->entry);
    if (e == 0)
        return -7;
    e->until += m->years;
    if (m->price > 0) {
        gm_AddNegative(c->netId, gv_PlayerRecs[ai_OwnPlayer].currency, m->price);
        gm_AddObjekt(gv_CharsT[bld->owner].netId, gv_PlayerRecs[ai_OwnPlayer].currency,
                     (int)(m->price * gv_SaleShares[m->size]));
    }
    return 0;
}
/* 0x4f0900, sends graveyard order type 3 (GVORDER, cm_SendGraveOrder): sets the holder of the entry `entry` of the size
   `size` of the graveyard building o to the character c (c[1]: its AICHAR.netId), or frees the entry when c is 0 (netId
   -1); returns the sender's result, -1 when o is 0 or entry is -1 */

int gv_OrderSetGraveHolder(int *c, char *o, int size, int entry)
{
    GVORDER m;
    int chr;

    if (o != 0 && entry != -1) {
        chr = c == 0 ? -1 : c[1];
        m.chr = chr;
        m.bld = ((AIBLD *)o)->id;
        m.type = 3;
        m.size = size;
        m.entry = (unsigned char)entry;
        m.years = 0;
        m.price = 0;
        return ((int (*)(void *))cm_SendGraveOrder)(&m);
    }
    return -1;
}
/* 0x4f0970, graveyard order type 3 (GVORDER m, sent by gv_OrderSetGraveHolder): makes the character m->chr the holder of
   the entry m->entry of the size m->size of the graveyard building m->bld (GVENTRY.id its netId, GVENTRY.w08 its
   AICHAR.w050), or frees the entry when m->chr is -1 (-1 / 0xffff). Returns 0, or a negative reason as gv_BuryCharacter
   (-6 no such character, -7 no such entry) */

int gv_SetGraveHolder(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVYARD *g;
    GVENTRY *e;
    int id;
    unsigned short key;

    id = -1;
    key = 0xffff;
    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    if (gv_SizeOpen((char *)g, m->size) == 0)
        return -4;
    if (m->chr != -1) {
        gm_FindById(0, 0, (int *)&c, m->chr);
        if (c == 0)
            return -6;
        id = m->chr;
        key = c->w050;
    }
    e = (GVENTRY *)gv_ListEntry((char *)g, m->size, m->entry);
    if (e == 0)
        return -7;
    e->w08 = key;
    e->id = id;
    return 0;
}
/* 0x4f0a90, sends graveyard order type 4 (GVORDER, cm_SendGraveOrder): opens the grave sizes up to `size` of the graveyard
   building o for the character c (c[1]: its AICHAR.netId, -1 when c is 0); returns the sender's result, -1 when o is 0 */

int gv_OrderOpenSize(int *c, char *o, int size)
{
    GVORDER m;
    int chr;

    if (o == 0)
        return -1;
    chr = c == 0 ? -1 : c[1];
    m.chr = chr;
    m.bld = ((AIBLD *)o)->id;
    m.type = 4;
    m.size = size;
    m.entry = 0;
    m.years = 0;
    m.price = 0;
    return ((int (*)(void *))cm_SendGraveOrder)(&m);
}
/* 0x4f0af0, graveyard order type 4 (GVORDER m, sent by gv_OrderOpenSize): opens the grave sizes 1 .. m->size (GVYARD.open)
   of the graveyard building m->bld, when that size's list exists and the yard's level (GVYARD.kind) reaches it. Returns 0,
   -8 for a size other than 1 .. 3, -7 when the size cannot be opened, otherwise a negative reason as gv_BuryCharacter
   @deep read whole against the original: every check, constant and branch matches (kind >= 3 / >= 2
   signed, the -7 / -8 tails, if-chain on m->size; a switch is further off). Remaining: the original
   cross-jumps size 1's 'open[1] = 1; return 0' into size 2's tail (+ 0x4f0bfe jne into 0x4f0be7, its pop
   edi after the stores at 0x4f0bea) while ours schedules pop edi before size 2's stores (- 0x4f0be4 pop
   edi, order 6) so no common tail exists and size 1 keeps its own copy (- je / mov [esi+0x19], cl / pop
   edi / xor / pop / pop / ret, code 8). Tried: goto into the shared tail (same code), switch (worse).
   Scheduling / tail merge only */

int gv_OpenSize(GVORDER *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVYARD *g;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    g = bld->yard;
    if (g == 0)
        return -3;
    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    if (m->size == 3) {
        if (g->lists[3] != 0 && g->kind >= 3) {
            g->open[3] = 1;
            g->open[2] = 1;
            g->open[1] = 1;
            return 0;
        }
    } else if (m->size == 2) {
        if (g->lists[2] != 0 && g->kind >= 2) {
            g->open[2] = 1;
            g->open[1] = 1;
            return 0;
        }
    } else if (m->size == 1) {
        if (g->lists[1] != 0) {
            g->open[1] = 1;
            return 0;
        }
    } else {
        return -8;
    }
    return -7;
}
/* 0x4f0c20, the message 0x4c60b0 type 5 (graveyard id -1, object id +1 of o, value v, byte b, int x); -1 when o is 0 or b is -1 */

int gv_OrderAdjustEntry(char *o, int v, int b, int x)
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
        return ((int (*)(void *))cm_SendGraveOrder)(m);
    }
    return -1;
}
/* 0x4f0c80, graveyard order type 5 (GVORDER m, sent by gv_OrderAdjustEntry): adds m->price (here a signed delta) to the
   byte GVENTRY.b0c of the entry m->entry of the size m->size of the graveyard building m->bld, clamped to 0 .. 0xff.
   Returns 0, or a negative reason as gv_BuryCharacter (-7 no such entry) */

int gv_AdjustEntry(GVORDER *m)
{
    AIBLD *bld;
    GVENTRY *e;
    int size;
    int v;

    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    size = m->size;
    if (gv_SizeOpen((char *)bld->yard, size) == 0)
        return -4;
    e = (GVENTRY *)gv_ListEntry((char *)bld->yard, size, m->entry);
    if (e == 0)
        return -7;
    v = e->b0c + m->price;
    v = v <= 0 ? 0 : v;
    if (v >= 0xff)
        v = 0xff;
    e->b0c = (unsigned char)v;
    return 0;
}
/* 0x4f0d70, the message 0x4c60b0 type 6 (graveyard id -1, object id +1 of o, value x at +0x13); -1 when o is 0 */

int gv_OrderNewDead(char *o, int x)
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
    return ((int (*)(void *))cm_SendGraveOrder)(m);
}
/* 0x4f0dd0, graveyard order type 6 (p: +8 the object id, +0x13 the number): makes that many new dead (0x4ef9c0); 0 when some were made, else -4 */

int gv_AddNewDead(unsigned char *p)
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
    n = gv_BuryNewDead(o, *(int *)(p + 0x13));
    return (n > 0) - 1 & 0xfffffffc;
}
/* 0x4f0e80, sends a grave move (GVMOVE, message 0x61, cm_SendGraveMove): the character c (c[1]: its AICHAR.netId) moves the
   entry `entry` of the size `from` of the graveyard building o to a free entry of the size `to` for the price `price` */

void gv_OrderMoveGrave(int *c, int to, char *o, int from, int entry, int price)
{
    GVMOVE m;

    m.chr = c[1];
    m.bld = ((AIBLD *)o)->id;
    m.from = from;
    m.entry = (unsigned char)entry;
    m.to = to;
    m.price = price;
    cm_SendGraveMove(&m);
}
/* 0x4f0ed0, runs a grave move (GVMOVE m, message 0x61, sent by gv_OrderMoveGrave): the entry m->entry of the size m->from
   of the graveyard building m->bld, held by the character m->chr (GVENTRY.w08 == AICHAR.w050), moves to the first free
   entry of the size m->to (gv_FindEntry, gv_EntryIsFree): its dead (GVGRAVE records in use) are packed into the free
   records of the new entry and cleared at the old one; the holder goes over, the new lease runs to ai_Time.year + 10.
   With a price the character pays it and the owner gets the target size's share (gv_SaleShares). Returns 0, or a
   negative reason as gv_BuryCharacter (-6 no such character, -7 no such held entry or no free one) */

int gv_MoveGrave(GVMOVE *m)
{
    AIBLD *bld;
    AICHAR *c;
    GVENTRY *src;
    GVENTRY *dst;
    GVGRAVE *s;
    GVGRAVE *send;
    GVGRAVE *d;
    GVGRAVE *dend;
    int from;
    int n;

    gm_FindById(0, 0, (int *)&c, m->chr);
    if (c == 0)
        return -6;
    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return -1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return -2;
    if (bld->yard == 0)
        return -3;
    from = m->from;
    if (gv_SizeOpen((char *)bld->yard, from) == 0)
        return -4;
    src = (GVENTRY *)gv_ListEntry((char *)bld->yard, from, m->entry);
    if (src == 0 || src->w08 != c->w050)
        return -7;
    s = src->grave;
    n = gv_EntryCounts[from];
    dst = (GVENTRY *)gv_FindEntry(c, (unsigned char *)bld, 0, gv_EntryIsFree, m->to);
    if (dst == 0)
        return -7;
    d = dst->grave;
    dend = d + gv_EntryCounts[m->to];
    send = s + n;
    for (; d < dend; d++) {
        if (d->id == -1) {
            for (; s < send; s++) {
                if (s->id != -1) {
                    *d = *s;
                    memset(s, 0, sizeof(GVGRAVE));
                    s->id = -1;
                    break;
                }
            }
            if (s >= send)
                break;
        }
    }
moved:
    dst->id = src->id;
    src->id = -1;
    dst->w08 = src->w08;
    src->w08 = 0xffff;
    dst->until = (unsigned short)(gv_TimeT.year + 10);
    dst->b0c = 0xff;
    src->until = 0xff;
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->objId;
    if (m->price > 0) {
        gm_AddNegative(c->netId, gv_PlayerRecs[ai_OwnPlayer].currency, m->price);
        gm_AddObjekt(gv_CharsT[bld->owner].netId, gv_PlayerRecs[ai_OwnPlayer].currency,
                     (int)(m->price * gv_SaleShares[m->to]));
    }
    return 0;
}
/* 0x4f1170, sends a raising of a dead person (GVRAISE, message 0x62, cm_SendRaiseDead): the dead GVGRAVE.id `grave` of the
   entry `entry` of the size `size` of the graveyard building o, as kind `kind` (0x1b / 0x1c); first files gv_Accuse (0x1b,
   1, the owner's netId, -1, the building's id). Returns the sender's result, -1 when the building has no owner, is the
   player's own or entry is -1 */

int gv_OrderRaiseDead(char *o, int size, int entry, int grave, int kind)
{
    unsigned short owner;
    GVRAISE m;

    owner = ((AIBLD *)o)->owner;
    if (owner != 0xffff && owner != *ai_PlayerChar && entry != -1) {
        m.bld = ((AIBLD *)o)->id;
        m.size = size;
        m.entry = (unsigned char)entry;
        m.grave = grave;
        m.kind = kind;
        gv_Accuse(0x1b, 1, gv_CharsT[owner].netId, -1, m.bld);
        return ((int (*)(void *))cm_SendRaiseDead)(&m);
    }
    return -1;
}
/* 0x4f15e0, takes a random grave (0x4ef930) of the object o and its list entry (0x4ef900) and sends it with 0x4f1170 (type 0x1c); -1 when 0x4f1d50 (o, 0) is not 1 or nothing is found */

int gv_SendGrave(unsigned char *o)
{
    int *z;
    int *e;
    int k;

    if (gv_RoomForRaised(o, 0) != 1)
        return -1;
    z = (int *)gv_RandomGrave(o);
    if (z == 0)
        return -1;
    e = (int *)gv_LoadGraveyard(o, z);
    if (e == 0)
        return -1;
    k = gv_EntryIndex(*(char **)(o + 0x65), e);
    if (k == -1)
        return -1;
    return gv_OrderRaiseDead((char *)o, *e, k, *z, 0x1c);
}
/* 0x4f1660, the daily decay of a raised dead person c (kind 0x14, gv_RaiseDead; the daily character step of f3gm.c
   0x4fbe90 removes it when this returns 1): its life value f01c loses f014 less 50 * gv_PreserveRate of its graveyard
   (cm_SendFloatDelta), all of it (-1000) when the graveyard is gone, has no owner or is the player's own. Returns 1
   when the life value is used up, 0 while some is left and for other or dead characters */

int gv_DecayRaisedDead(AICHAR *c)
{
    AIBLD *bld;
    float loss, rest;

    if (c != 0 && c->kind == 0x14 && c->health != 0) {
        bld = c->bld;
        if (bld == 0 || bld->owner == 0xffff || bld->owner == *ai_PlayerChar) {
            cm_SendFloatDelta(c->netId, 0x1c, -1000.0f);
        } else {
            loss = c->f014 - gv_PreserveRate((char *)bld) * 50.0f;
            rest = c->f01c - loss;
            cm_SendFloatDelta(c->netId, 0x1c, -loss);
            if (rest <= 0.0f)
                return 1;
            return 0;
        }
        return 1;
    }
    return 0;
}
/* 0x4f16f0, a rate of the object o: the sum of 10 / 10 / 10 / 15 / 10 / 10 / 15 for the items 0x1ff, 0x202, 0x208, 0x20d, 0x214, 0x216, 0x219 in its list, times 0.0125 */

float gv_PreserveRate(char *o)
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
    if (gv_GraveTotals[*(int *)(g + 4)] > 0) {
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
        } while (i < gv_GraveTotals[*(int *)(g + 4)]);
    }
    z = *(char **)(g + 0x1c);
    i = 0;
    if (gv_EntryTotals[*(int *)(g + 4)] > 0) {
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
        } while (i < gv_EntryTotals[*(int *)(g + 4)]);
    }
    return 1;
}
/* 0x4f1d50, counts the raised dead persons of the graveyard building o (kind 0x14 characters whose bld is o) into *n
   (n may be 0); returns 1 while they are fewer than the building type's places k231 + k232, 0 when it is full, -1 for
   a building that is no graveyard (kind 0x1d) or has no GVYARD */

int gv_RoomForRaised(unsigned char *o, int *n)
{
    AIBLD *bld;
    AIBTYPE *t;
    AICHAR *c;
    int places;
    int cnt;

    bld = (AIBLD *)o;
    t = (AIBTYPE *)(ai_BTypes + bld->type * 0x24d);
    if (t->kind != 0x1d)
        return -1;
    if (bld->yard == 0)
        return -1;
    places = t->k232 + t->k231;
    cnt = 0;
    for (c = gv_CharsT; (int)&c->bld < (int)&gv_CharsT[768].bld; c++) {
        if (c->id != 0xffff && c->kind == 0x14 && c->bld == bld)
            cnt++;
    }
    if (n != 0)
        *n = cnt;
    return cnt < places;
}
/* 0x4f1200, runs a raising of a dead person (GVRAISE m, message 0x62, sent by gv_OrderRaiseDead): the dead GVGRAVE m->grave
   of the entry m->entry of the size m->size of the graveyard building m->bld becomes a living character of kind 0x14 in
   the first free ai_Chars slot (or one of kind 0xf, as ahm_CreateAlchemist): the building is its home, its title m->kind,
   its skills half the grave's traits, its names and sex the grave's, f014 = 1000 / (random * 4 + 8), its relations to
   all others random -0x20 .. 0x1f; it is placed at the building. The grave record is cleared; when the entry's id was
   this dead person's, it passes to the first other dead of the entry (none: the entry is freed). Returns the new
   character's id, 0xffff (no such entry or grave, no free slot) or a negative reason as gv_BuryCharacter (-6 another
   kind, -1 no owner or the player's own building, -2 not a graveyard, -3 no GVYARD, -4 the size not open)
   @deep read the whole function against the original (checks, entry/grave search, slot search, character
   init, name copies, start values, relation loop, place/icon, entry handover, grave clear): every
   statement, field offset, constant and callee matches; fixed this pass: byte jobId/officeKind stores (not
   a short at 0x164), the id = 0xffff store written through gv_CharsT[slot] (the original keeps it, a store
   through c is dropped as dead), the relation loop as ai_Relations[j][id] / [id][j] (now equal), store
   order of 0x18c/0x20c/title, f01c after util_RandomF. Left: allocation/IV choice only - the name copies:
   the original merges the two pointers into one IV with the d - s offset ('+ mov ecx,esi / + sub ecx,eax /
   + mov byte ptr [edx+eax],cl'), ours keeps two ('- lea edx,[esi+0x30] / - inc edx'), tried
   for/(c->first[j]=q->first[j]), do-while with ch, do-while with re-read: none gives the merged IV; the
   traits loop reloads q from [esp+0x10] in the original (ours keeps it in ecx); the ai_CharCount++ /
   ai_NextObjectId schedule ('+ mov ecx,[ai_CharCount] / + inc ecx' before the netId load, ours interleaved
   in edi) and the relatives memset pointer via edx ('+ lea edx,[edx+0x770bdc] / + mov edi,edx' vs '- lea
   edi,[edx+ai_Chars+0x5c]') */

unsigned gv_RaiseDead(GVRAISE *m)
{
    AIBLD *bld;
    GVENTRY *e;
    GVGRAVE *g, *q, *z;
    AICHAR *c;
    int size, i, j, n, slot;
    char ch;
    float r;

    if (m->kind != 0x1b && m->kind != 0x1c)
        return (unsigned)-6;
    gm_FindById((int *)&bld, 0, 0, m->bld);
    if (bld == 0 || bld->owner == 0xffff || &gv_CharsT[bld->owner] == gv_PlayerAiChar)
        return (unsigned)-1;
    if (((AIBTYPE *)(ai_BTypes + bld->type * 0x24d))->kind != 0x1d)
        return (unsigned)-2;
    if (bld->yard == 0)
        return (unsigned)-3;
    size = m->size;
    if (gv_SizeOpen((char *)bld->yard, size) == 0)
        return (unsigned)-4;
    e = (GVENTRY *)gv_ListEntry((char *)bld->yard, size, m->entry);
    if (e == 0)
        return 0xffff;
    i = 0;
    if (gv_EntryCounts[size] > 0) {
        g = e->grave;
        do {
            q = g;
            if (q->id == m->grave)
                break;
            i++;
            g = q + 1;
        } while (i < gv_EntryCounts[size]);
        if (q->id == m->grave) {
            for (slot = 0; slot < 0x300; slot++)
                if (gv_CharsT[slot].id == 0xffff)
                    break;
            if (slot == 0x300) {
                for (slot = 0; slot < 0x300; slot++)
                    if (gv_CharsT[slot].kind == 0x0f)
                        break;
                if (slot == 0x300)
                    return 0xffff;
            }
            if (gv_CharsT[slot].kind == 0x0f)
                ai_CharCount--;
            c = &gv_CharsT[slot];
            memset(c, 0, sizeof(AICHAR));
            gv_CharsT[slot].netId = -1;
            gv_CharsT[slot].id = 0xffff;
            memset(gv_CharsT[slot].relatives, 0xff, sizeof(gv_CharsT[slot].relatives));
            ai_CharCount++;
            c->netId = ai_NextObjectId;
            ai_NextObjectId++;
            c->id = (unsigned short)slot;
            c->kind = 0x14;
            c->bld2 = bld;
            c->bld = bld;
            c->v1f0 = 0;
            c->partner = -1;
            c->b215 = 0;
            c->b216 = 0;
            c->b217 = 0;
            c->mask = 1;
            c->mood3Rate = 1.0f;
            c->health = 100;
            c->w00a = 0x10;
            c->jobId = 0;
            c->officeKind = 0;
            c->picture = -1;
            c->partyId = -1;
            c->title = (char)m->kind;
            c->ownName[0] = 0;
            c->v190 = 4;
            c->money = 0;
            c->b00d = 1;
            c->w050 = 0x7fff;
            for (j = 0; j < 5; j++)
                c->skills[j] = q->traits[j] >> 1;
            c->sex = (unsigned char)q->sex;
            j = 0;
            do {
                ch = q->first[j];
                c->first[j] = ch;
                j++;
            } while (ch != 0);
            j = 0;
            do {
                ch = q->family[j];
                c->family[j] = ch;
                j++;
            } while (ch != 0);
            c->f010 = 0;
            r = util_RandomF();
            c->f018 = 0;
            c->f01c = 1000.0f;
            c->f020 = 0;
            c->v024 = 0;
            c->w028 = 0;
            c->v02c = 0;
            c->f014 = 1000.0f / (r * 4.0f + 8.0f);
            for (j = 0; j < 0x300; j++) {
                ai_Relations[j][c->id] = (signed char)util_RandomU(0x40) - 0x20;
                ai_RelationMemory[j][c->id] = 0;
                ai_Relations[c->id][j] = (signed char)util_RandomU(0x40) - 0x20;
                ai_RelationMemory[c->id][j] = 0;
            }
            gm_PlaceOnStage(c->netId, bld->id, -1, 0, 0);
            gm_ChooseHeadIcon(&c->id);
            if (q->id == e->id) {
                n = 0;
                if (gv_EntryCounts[m->size] > 0) {
                    z = e->grave;
                    do {
                        if (z->id != -1 && z->id != q->id) {
                            e->id = z->id;
                            goto cleared;
                        }
                        n++;
                        z++;
                    } while (n < gv_EntryCounts[m->size]);
                }
                e->id = -1;
                e->w08 = 0xffff;
            }
        cleared:
            memset(q, 0, sizeof(GVGRAVE));
            q->id = -1;
            return c->id;
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
    if (gv_GraveTotals[*(int *)(g + 4)] > 0) {
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
        } while (i < gv_GraveTotals[*(int *)(g + 4)]);
    }
    i = 0;
    z = *(char **)(g + 0x1c);
    if (gv_EntryTotals[*(int *)(g + 4)] > 0) {
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
        } while (i < gv_EntryTotals[*(int *)(g + 4)]);
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
/* 0x4f2330, the price of a grave lease for the character c (its id) in a grave of the size kind: its worth in the player's
   money (gm_GetWorth, gm_ConvertMoney) clamped to 10000 .. 1000000, log10 of it squared * 100, times 0.25 / 0.5 / 0.75 for the
   sizes 0 .. 2; unless flag, also times (0.3 + 0.7 * care / 255) * 0.75 with care the entry's GVENTRY.b0c. Returns the price
   in the player's terms (gm_PlayerFactor) and stores the owner's share of it (gv_SaleShares[kind]) in *out */

int gv_LeasePrice(unsigned short *c, unsigned char care, int kind, int flag, int *out)
{
    double x, v;
    int r;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    x = max(min(1000000.0, x), 10000.0);
    v = log10(x);
    v = v * v * 100.0;
    if (kind == 0)
        v *= 0.25;
    else if (kind == 1)
        v *= 0.5;
    else if (kind == 2)
        v *= 0.75;
    if (flag == 0)
        v = (0.7f * (care / 255.0) + 0.3f) * (v * 0.75);
    r = gm_PlayerFactor((int)v, ai_OwnPlayer);
    *out = (int)(r * gv_SaleShares[kind]);
    return r;
}
/* 0x4f2440, the cost difference of two kinds: 0x4f2330 (c, 100, k1, 1) minus half of the one of k2; *out the difference times the factor of k2 */

int unk_Unk4f2440(unsigned short *c, int k1, int k2, int *out)
{
    int a, b, d;

    a = gv_LeasePrice(c, 100, k1, 1, &d);
    b = gv_LeasePrice(c, 100, k2, 1, &d);
    d = b - a / 2;
    *out = (int)(d * gv_SaleShares[k2]);
    return d;
}
/* 0x4f24a0, the price the plunder event 0x8a (he.c 0x5707a0) demands from the character c (its id): its worth in the player's
   money (gm_GetWorth, gm_ConvertMoney) clamped to 10000 .. 100000, x / (log10 x + 1) * 0.1, in the player's terms
   (gm_PlayerFactor) */

int gv_ProtectionPrice(unsigned short *c)
{
    double x;

    x = (double)gm_ConvertMoney(gm_GetWorth(*c), ai_OwnPlayer);
    x = max(min(100000.0, x), 10000.0);
    return gm_PlayerFactor((int)(x / (log10(x) + 1.0) * 0.1f), ai_OwnPlayer);
}
/* 0x4f2540, finds the grave entry held by the character a (gv_EntryHeldBy, any size) in the first graveyard building
   (kind 0x1d) of the 256 that has one: *o = the building, *e = the entry, 1; 0 (both 0) when none has */

int gv_FindHeldEntry(void *a, unsigned char **o, int *e)
{
    int i, r;
    AIBLD *b;

    *o = 0;
    *e = 0;
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
        if (b->type != 0 && ((AIBTYPE *)(ai_BTypes + b->type * 0x24d))->kind == 0x1d) {
            r = gv_FindEntry(a, (unsigned char *)b, 0, gv_EntryHeldBy, 0xff);
            if (r != 0) {
                *o = (unsigned char *)b;
                *e = r;
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4f25d0, finds the grave entry reserved for the character a (gv_EntrySameId: its id is a's netId, any size) in the
   first graveyard building (kind 0x1d) of the 256 that has one: *o = the building, *e = the entry, 1; 0 (both 0) when none
   has */

int gv_FindReservedEntry(void *a, unsigned char **o, int *e)
{
    int i, r;
    AIBLD *b;

    *o = 0;
    *e = 0;
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
        if (b->type != 0 && ((AIBTYPE *)(ai_BTypes + b->type * 0x24d))->kind == 0x1d) {
            r = gv_FindEntry(a, (unsigned char *)b, 0, gv_EntrySameId, 0xff);
            if (r != 0) {
                *o = (unsigned char *)b;
                *e = r;
                return 1;
            }
        }
    }
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
/* 0x4f2720, copies the AI decision record i (0 .. 0x1c, an AIDEC of the table 0x611258) to out (a struct copy); 0 for i > 0x1c */

int gv_GetTableRecord(int i, int *out)
{
    AIDEC *rec;

    if (i >= 0x1d)
        return 0;
    rec = (AIDEC *)out;
    *rec = gv_Decisions[i];
    return 1;
}
/* 0x4f2870, finds the AI decision record of the office held by the character c (AICHAR.officeKind, +0x166) with the number b and copies it to out; 0 when none */

int gv_FindTableRecord(char *c, unsigned b, int *out)
{
    AIDEC *rec;
    int i;

    for (i = 0; i < 0x1d; i++) {
        if ((unsigned char)c[0x166] == gv_Decisions[i].office && b == gv_Decisions[i].officeNo) {
            rec = (AIDEC *)out;
            *rec = gv_Decisions[i];
            return 1;
        }
    }
    return 0;
}
/* 0x4f28d0, writes the crime state to the save file f: the tag 0x1d, the current values (AIDEC.cur) of the 0x1d AI
   decision records, ai_NextCrimeId, the counts of the used crime records (ai_Crimes) and evidence pairs (ai_Evidence),
   then each used crime record field by field and each used pair; 1 when all was written */

int gv_WriteCrimes(void *f)
{
    int tag, crimes, evidence;
    int i;

    tag = 0x1d;
    crimes = 0;
    evidence = 0;
    if (f == 0)
        return 0;
    if (vfs_fwrite(&tag, 4, 1, f) == 0)
        return 0;
    for (i = 0; i < 0x1d; i++) {
        if (vfs_fwrite(&gv_Decisions[i].cur, 4, 1, f) == 0)
            return 0;
    }
    if (vfs_fwrite(&ai_NextCrimeId, 4, 1, f) == (unsigned)-1)
        return 0;
    for (i = 0; i < 0x200; i++) {
        if (gv_CrimesT[i].id != -1)
            crimes++;
    }
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] != -1 && ai_Evidence[i][1] != -1)
            evidence++;
    }
    if (vfs_fwrite(&crimes, 4, 1, f) == (unsigned)-1)
        return 0;
    if (vfs_fwrite(&evidence, 4, 1, f) == (unsigned)-1)
        return 0;
    for (i = 0; i < 0x200; i++) {
        if (gv_CrimesT[i].id != -1) {
            if (vfs_fwrite(&gv_CrimesT[i].id, 4, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].time, 0xe, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].victim, 4, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].culprit, 4, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].count, 2, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].law, 1, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].v1d, 4, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].place, 4, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&gv_CrimesT[i].state, 4, 1, f) == (unsigned)-1)
                return 0;
        }
    }
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] != -1 && ai_Evidence[i][1] != -1) {
            if (vfs_fwrite(&ai_Evidence[i][0], 4, 1, f) == (unsigned)-1)
                return 0;
            if (vfs_fwrite(&ai_Evidence[i][1], 4, 1, f) == (unsigned)-1)
                return 0;
        }
    }
    return 1;
}
/* 0x4f2da0, the highest level (0x5092e0 (b, 10, 1)) among the school buildings (kind 7) of the character *c's, times 0.01; 0.75 when one has exactly 0x4b */

float gv_SchoolLevelFactor(short *c)
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
/* 0x4f2b30, reads the crime state gv_WriteCrimes wrote from the save file f (the tag must be 0x1d): the AIDEC.cur values,
   ai_NextCrimeId, the counts (only saves of version 0x10041 on carry them; older ones hold 0x80 records and 0x200
   pairs), the crime records (the rest of ai_Crimes are made free) and the evidence pairs (ai_Evidence starts as all
   -1); 1 when all was read */

int gv_ReadCrimes(int f)
{
    unsigned crimes, evidence;
    int tag;
    int i;

    crimes = 0x80;
    evidence = 0x200;
    if (f == 0)
        return 0;
    if (ls_Read(&tag, 4, 1, f) == 0)
        return 0;
    if (tag != 0x1d)
        return 0;
    for (i = 0; i < 0x1d; i++) {
        if (ls_Read(&gv_Decisions[i].cur, 4, 1, f) == 0)
            return 0;
    }
    if (ls_Read(&ai_NextCrimeId, 4, 1, f) == 0)
        return 0;
    if (ls_Version >= 0x10041) {
        if (ls_Read(&crimes, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&evidence, 4, 1, f) == 0)
            return 0;
    }
    for (i = 0; i < crimes; i++) {
        if (ls_Read(&gv_CrimesT[i].id, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].time, 0xe, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].victim, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].culprit, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].count, 2, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].law, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].v1d, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].place, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&gv_CrimesT[i].state, 4, 1, f) == 0)
            return 0;
    }
    for (; i < 0x200; i++) {
        gv_CrimesT[i].id = -1;
        gv_CrimesT[i].victim = -1;
        gv_CrimesT[i].culprit = -1;
        gv_CrimesT[i].count = 0;
        gv_CrimesT[i].state = 0;
    }
    memset(ai_Evidence, 0xff, sizeof(ai_Evidence));
    for (i = 0; i < evidence; i++) {
        if (ls_Read(&ai_Evidence[i][0], 4, 1, f) == 0)
            return 0;
        if (ls_Read(&ai_Evidence[i][1], 4, 1, f) == 0)
            return 0;
    }
    return 1;
}
/* 0x4f2fe0, the text of the AI decision record i into out (txt_Format with the texts 0x13fe + 5 * id / 0x13fd + 5 * id):
   its current value as a number or, by AIDEC.valueText 1 / 2 / 3, as the name 0x13dd / 0x13e6 / 0x13ec + cur;
   0 for another valueText or no record */

int gv_TableRecordText(char *out, int i)
{
    AIDEC rec;
    int r;
    int t;

    r = 1;
    if (gv_GetTableRecord(i, (int *)&rec) == 0)
        return 0;
    t = rec.id * 5 + 0x13fd;
    switch (rec.valueText) {
    case 0:
        txt_Format(out, t + 1, t, rec.cur);
        break;
    case 1:
        txt_Format(out, t + 1, t, rec.cur + 0x13dd);
        break;
    case 2:
        txt_Format(out, t + 1, t, rec.cur + 0x13e6);
        break;
    case 3:
        txt_Format(out, t + 1, t, rec.cur + 0x13ec);
        break;
    default:
        r = 0;
    }
    return r;
}

/* 0x4f30a0, the text of a crime against the laws 2 .. 8: laws 2 / 3 / 4 / 8 pick the texts 0x140a / 0x140f / 0x1414 / 0x1428
   (+ 1 for mode <= 1) with the crime's time, law 2 also the text 0x13f9 / 0x13fb + v1d (year below 0x75 or not), law 4
   the victim's short id (0xfffe when not found); any other law the fixed text and 0, else 1. */

int gv_CrimeTextLaw2to8(char *out, CRIME r, int mode)
{
    int ok;
    int text;
    unsigned short *victim;
    unsigned short id;

    ok = 1;
    text = mode <= 1;
    victim = (unsigned short *)gm_FindChar(r.victim);
    if (victim != 0)
        id = *victim;
    else
        id = 0xfffe;
    switch (r.law) {
    case 2:
        if (r.time.year < 0x75)
            txt_Format(out, text + 0x140a, &r.time, r.v1d + 0x13f9);
        else
            txt_Format(out, text + 0x140a, &r.time, r.v1d + 0x13fb);
        break;
    case 3:
        txt_Format(out, text + 0x140f, &r.time);
        break;
    case 4:
        txt_Format(out, text + 0x1414, &r.time, id);
        break;
    case 8:
        txt_Format(out, text + 0x1428, &r.time);
        break;
    default:
        strcpy(out, "Hm, das ist doch nicht verboten");
        ok = 0;
    }
    return ok;
}
/* 0x4f31d0, the text of a crime against the laws 0xe / 0x10: the texts 0x1446 / 0x1450 (+ 1 for mode <= 1) with the
   crime's time and the victim's short id (0xfffe when not found); any other law the fixed text and 0, else 1. */

int gv_CrimeTextLaw9to16(char *out, CRIME r, int mode)
{
    int ok;
    int text;
    unsigned short *victim;
    unsigned short id;

    ok = 1;
    text = mode <= 1;
    victim = (unsigned short *)gm_FindChar(r.victim);
    if (victim != 0)
        id = *victim;
    else
        id = 0xfffe;
    switch (r.law) {
    case 0xe:
        txt_Format(out, text + 0x1446, &r.time, id);
        break;
    case 0x10:
        txt_Format(out, text + 0x1450, id, &r.time);
        break;
    default:
        strcpy(out, "Hm, das ist doch nicht verboten");
        ok = 0;
    }
    return ok;
}
/* 0x4f3270, the text of a crime against the laws 0x11 .. 0x1c: the text 0x1455 + 5 * (law - 0x11) (+ 1 for mode <= 1)
   with the crime's time and, by law, the victim's short id (0xfffe when not found); law 0x18 also names the place's
   building ("%1G", "Geb\xe4ude" when there is none), its argument order by ls_GetLanguage. Returns 0. */

int gv_CrimeTextLaw17to28(char *out, CRIME r, int mode)
{
    char name[0x80] = "Geb\xe4ude";
    int text, bld;
    unsigned short *victim;
    unsigned short id;

    text = mode <= 1;
    gm_FindById(&bld, 0, 0, r.place);
    if (bld != 0)
        txt_Format(name, "%1G", bld);
    victim = (unsigned short *)gm_FindChar(r.victim);
    if (victim != 0)
        id = *victim;
    else
        id = 0xfffe;
    switch (r.law) {
    case 0x11:
        txt_Format(out, text + 0x1455, &r.time);
        break;
    case 0x12:
        txt_Format(out, text + 0x145a, &r.time);
        break;
    case 0x13:
        txt_Format(out, text + 0x145f, id, &r.time);
        break;
    case 0x14:
        txt_Format(out, text + 0x1464, &r.time, id);
        break;
    case 0x15:
        txt_Format(out, text + 0x1469, &r.time, id);
        break;
    case 0x16:
        txt_Format(out, text + 0x146e, id, &r.time);
        break;
    case 0x17:
        txt_Format(out, text + 0x1473, &r.time, id);
        break;
    case 0x18:
        switch (ls_GetLanguage()) {
        case 0:
        case 1:
            txt_Format(out, text + 0x1478, &r.time, id, name);
            break;
        case 2:
            txt_Format(out, text + 0x1478, &r.time, name, id);
            break;
        }
        break;
    case 0x19:
        txt_Format(out, text + 0x147d, id, &r.time);
        break;
    case 0x1a:
        txt_Format(out, text + 0x1482, &r.time, id);
        break;
    case 0x1b:
        txt_Format(out, text + 0x1487, &r.time);
        break;
    case 0x1c:
        txt_Format(out, text + 0x148c, &r.time, id);
        break;
    }
    return 0;
}
/* 0x4f3580, picks the text builder by the type byte (+0x1c) of the record: below 9 0x4f30a0, below 0x11 0x4f31d0, below 0x1d 0x4f3270; 0 otherwise */

int gv_CrimeText(char *out, CRIME r, int mode)
{
    if (r.law < 9)
        return gv_CrimeTextLaw2to8(out, r, mode);
    if (r.law < 0x11)
        return gv_CrimeTextLaw9to16(out, r, mode);
    if (r.law < 0x1d)
        return gv_CrimeTextLaw17to28(out, r, mode);
    return 0;
}
/* 0x4f3610, fills out[] with the indexes of the (up to 32) crime records whose state is state; returns their count
   (eax: ai_Unk4db700 keeps it) */

int gv_FindCrimesByState(int state, int *out)
{
    int n, i;

    n = 0;
    for (i = 0; i < 0x200; i++) {
        if (n >= 0x20)
            break;
        if (gv_CrimesT[i].state == state) {
            out[n] = i;
            n++;
        }
    }
    return n;
}
/* 0x4f3640, the character owner holds evidence of the crime crime: stores the pair in its slot of ai_Evidence
   (gv_EvidenceSlot: the first free one; none when full or already there) and prints a trace; 1 when stored */

int gv_AddEvidence(int crime, int owner)
{
    int slot;
    char buf[512];

    slot = gv_EvidenceSlot(owner, crime);
    if (slot > -1) {
        ai_Evidence[slot][0] = owner;
        ai_Evidence[slot][1] = crime;
        sprintf(buf, "gs_AddBeweis(): Executed---- owner = %i, st_id = %i, slot = %i", owner, crime, slot);
        return 1;
    }
    return 0;
}
/* 0x4f36b0, the index of the first free crime record (state 0), -1 when none */

int gv_FreeCrimeSlot(void)
{
    int slot, i;

    slot = -1;
    for (i = 0; i < 0x200; i++) {
        if (gv_CrimesT[i].state == 0) {
            slot = i;
            i = 0x200;
        }
    }
    return slot;
}
/* 0x4f36e0, the index of the record with the id (+0) in 0x91f700 or -1 */

int gv_FindCrime(int id)
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

int gv_EvidenceSlot(int a, int b)
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
/* 0x4f3910, message 0x3c: with hand, every open crime (state 1) against culprit that owner holds evidence of (ai_Evidence)
   gets the state state (handed over); without, every crime in the state state is open again (1). Returns the count. */

int gv_HandOverCrimes(int owner, int culprit, int state, int hand)
{
    int count, i, k;

    count = 0;
    if (hand != 0) {
        for (i = 0; i < 0x800; i++) {
            if (ai_Evidence[i][0] == owner) {
                for (k = 0; k < 0x200; k++) {
                    if (gv_CrimesT[k].id == ai_Evidence[i][1]) {
                        if (gv_CrimesT[k].state == 1 && gv_CrimesT[k].culprit == culprit) {
                            gv_CrimesT[k].state = state;
                            count++;
                        }
                        break;
                    }
                }
            }
        }
    } else {
        for (k = 0; k < 0x200; k++) {
            if (gv_CrimesT[k].state == state) {
                gv_CrimesT[k].state = 1;
                count++;
            }
        }
    }
    return count;
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
/* 0x4f39e0, the guilt of the culprit c: the sum of AIDEC.guilt of the laws broken by the open crimes (state 1) of the culprit c */

int gv_CulpritGuilt(int c)
{
    int k;
    int sum;
    AIDEC rec;

    sum = 0;
    for (k = 0; k < 0x200; k++) {
        if (gv_CrimesT[k].culprit == c && gv_CrimesT[k].state == 1) {
            gv_GetTableRecord(gv_CrimesT[k].law, (int *)&rec);
            sum += rec.guilt;
        }
    }
    return sum;
}
/* 0x4f3fe0, fills out[0x20] (-1 first) with the indexes (of ai_Crimes) of the crimes of the culprit that the character t
   holds evidence of (ai_Evidence); returns the count; 0 when the culprit does not exist. A 33rd crime is still written
   (out[0x20]) before the walk stops, as in the original. */

int gv_HeldCrimesOf(int culprit, int *out, unsigned short t)
{
    int n, i, k;
    ORDCHAR *ch;

    n = 0;
    if (gm_FindChar(culprit) == 0)
        return 0;
    ch = &ORD_CHARS[t];
    if (ch == 0)
        return 0;
    memset(out, 0xff, 0x20 * sizeof(int));
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == ch->netId) {
            k = gv_FindCrime(ai_Evidence[i][1]);
            if (k != -1 && gv_CrimesT[k].culprit == culprit) {
                out[n++] = k;
                if (n > 0x20)
                    break;
            }
        }
    }
    return n;
}
/* 0x4f4080, as gv_HeldCrimesOf, but the crimes' ids (CRIME.id) are stored; t is a character index in its low word
   (unsigned here: he.c and unk_58bbd0.c declare the function without a prototype) */

int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned t)
{
    int n, i, k;
    ORDCHAR *ch;

    n = 0;
    if (gm_FindChar(culprit) == 0)
        return 0;
    ch = &ORD_CHARS[(unsigned short)t];
    if (ch == 0)
        return 0;
    memset(out, 0xff, 0x20 * sizeof(int));
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == ch->netId) {
            k = gv_FindCrime(ai_Evidence[i][1]);
            if (k != -1 && gv_CrimesT[k].culprit == culprit) {
                out[n++] = gv_CrimesT[k].id;
                if (n > 0x20)
                    break;
            }
        }
    }
    return n;
}
/* 0x4f4130, the count of gv_HeldCrimesOf without the list: how many crimes of the culprit the character t (low word)
   holds evidence of, stopping past 0x20; 0 when the culprit does not exist */

int gv_HeldCrimeCount(int culprit, unsigned t)
{
    int n, i, k;
    ORDCHAR *ch;

    n = 0;
    if (gm_FindChar(culprit) == 0)
        return 0;
    ch = &ORD_CHARS[(unsigned short)t];
    if (ch == 0)
        return 0;
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == ch->netId) {
            k = gv_FindCrime(ai_Evidence[i][1]);
            if (k != -1 && gv_CrimesT[k].culprit == culprit) {
                n++;
                if (n > 0x20)
                    break;
            }
        }
    }
    return n;
}
/* 0x4f41b0, the character with the most open crimes the character c holds no evidence of yet: goes over the open
   crimes (state > 0) of a living culprit other than c whose pair (c's netId, crime id) is not in ai_Evidence, counts
   them per culprit (up to 0x300 culprits) and returns the culprit with the most; ids[] gets the ids of its crimes (up
   to max), *cnt their number; 0 when there is none (the seance's ghost: ai_Unk585cb0) */

char *gv_FindAccused(char *c, int max, int *ids, int *cnt)
{
    int count[0x300];
    AICHAR *who[0x300];
    CRIME *rec[0x200];
    int i, j, k, n, nw, nr, bestn;
    AICHAR *ch, *o, *best;

    ch = (AICHAR *)c;
    *cnt = 0;
    memset(who, 0, sizeof(who));
    memset(count, 0, sizeof(count));
    best = 0;
    bestn = 0;
    n = 0;
    nr = 0;
    nw = 1;
    for (i = 0; i < 0x200; i++) {
        if (gv_CrimesT[i].state > 0 && gv_CrimesT[i].culprit != ch->netId) {
            o = (AICHAR *)gm_FindChar(gv_CrimesT[i].culprit);
            if (o != 0) {
                for (k = 0; k < 0x800; k++)
                    if (ai_Evidence[k][0] == ch->netId && ai_Evidence[k][1] == gv_CrimesT[i].id)
                        goto next;      /* c holds evidence of it already */
                rec[nr++] = &gv_CrimesT[i];
                for (j = 0; j < nw; j++) {
                    if (j >= 0x300)
                        break;
                    if (who[j] == 0) {
                        who[j] = o;
                        n = nw;
                        count[j]++;
                        nw++;
                        break;
                    }
                    if (who[j]->netId == o->netId) {
                        count[j]++;
                        break;
                    }
                }
            }
        }
next:;
    }
    if (nr == 0 || n == 0)
        return 0;
    for (j = 0; j < n; j++) {
        if (count[j] > bestn) {
            best = who[j];
            bestn = count[j];
        }
    }
    k = 0;
    for (i = 0; i < nr; i++) {
        if (k >= max)
            break;
        if (rec[i]->culprit == best->netId)
            ids[k++] = rec[i]->id;
    }
    *cnt = k;
    return (char *)best;
}
/* 0x4f4350, clears the crimes: ai_NextCrimeId = 0, every CRIME record free (culprit, victim and id -1, state 0) and
   every evidence pair (-1, -1) */

void gv_ClearCrimes(void)
{
    int i;

    ai_NextCrimeId = 0;
    for (i = 0; i < 0x200; i++) {
        gv_CrimesT[i].culprit = -1;
        gv_CrimesT[i].victim = -1;
        gv_CrimesT[i].id = -1;
        gv_CrimesT[i].state = 0;
    }
    for (i = 0; i < 0x800; i++) {
        ai_Evidence[i][0] = -1;
        ai_Evidence[i][1] = -1;
    }
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
        i = gv_FindCrime(id);
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
/* 0x4f3700, sends every crime record with message 0x23 (cm_SendRetireCrime (id, flag)): an open one (state 1) with flag 0, a
   handed-over one (state above 1: a job id) whose job he_FindEvent (1, 1, state) no longer finds with flag 1; after 32
   sends it pumps the net until the last send's slot is done (cm_GetSlotState).
   @deep read in full against the original (states, he_FindEvent check, cm_SendRetireCrime calls, the
   32-send wait loop as while (slot != -1 && cm_GetSlotState(slot) == 0)): every instruction pairs; the
   only remaining difference is the strength-reduced base of the index loop over gv_CrimesT: ours walks
   &gv_CrimesT[i] (mov esi, ai_Crimes; mov eax, [esi + 0x25]; mov ecx, [esi]; cmp esi, ai_Crimes+0x5a00),
   the original &gv_CrimesT[i].state (mov esi, 0x91f725; mov eax, [esi]; mov ecx, [esi - 0x25]; cmp esi,
   0x925125). Tried: state read in the condition, for(;;) wait loop. An induction-variable choice, not
   meaning. */

void gv_SendCrimes(void)
{
    int slot, sent, i;
    unsigned state;

    slot = -1;
    sent = 0;
    for (i = 0; i < 0x200; i++) {
        state = gv_CrimesT[i].state;
        if (state == 1) {
            slot = cm_SendRetireCrime(gv_CrimesT[i].id, 0);
            sent++;
        } else if (state > 1 && he_FindEvent(1, 1, state) == 0) {
            slot = cm_SendRetireCrime(gv_CrimesT[i].id, 1);
            sent++;
        }
        if (sent >= 0x20) {
            while (slot != -1 && cm_GetSlotState(slot) == 0)
                sv_PumpNet();
            slot = -1;
            sent = 0;
        }
    }
}
/* 0x4f37e0, retires the crime record of id (message 0x23): its count counts down; it stays (2) while the count is not 0
   unless force, and (1) while it is handed over (state above 1) without force or free. Else the scene's grid amount of the
   law is taken back (ord_SubGridAmount), every evidence pair of the crime is cleared (telling the player whose pair it
   was, ai_ShowEvidenceVoidNews, without force) and the record freed (state 0, culprit and id -1): 0. 3 when there is no
   record of id. */

int gv_RetireCrime(int id, int force)
{
    int k, i;
    short count;
    short *culprit;

    k = gv_FindCrime(id);
    if (k == -1)
        return 3;
    count = gv_CrimesT[k].count;
    if (count > 0)
        gv_CrimesT[k].count = count - 1;
    if (gv_CrimesT[k].count != 0 && force == 0)
        return 2;
    if (gv_CrimesT[k].state != 1 && (force == 0 || gv_CrimesT[k].state == 0))
        return 1;
    culprit = gm_FindChar(gv_CrimesT[k].culprit);
    ord_SubGridAmount(gv_CrimesT[k].place, gv_CrimesT[k].law);
    for (i = 0; i < 0x800; i++) {
        if (id == ai_Evidence[i][1]) {
            if (ai_PlayerId != 0xffff && ai_Evidence[i][0] == gv_CharsT[ai_PlayerId].netId && culprit != 0 && force == 0) {
                ai_ShowEvidenceVoidNews((int)&gv_CharsT[ai_PlayerId], (unsigned short *)culprit);
                        }
            ai_Evidence[i][0] = -1;
            ai_Evidence[i][1] = -1;
        }
    }
    gv_CrimesT[k].state = 0;
    gv_CrimesT[k].culprit = -1;
    gv_CrimesT[k].id = -1;
    return 0;
}
/* 0x4f4390, sorts the n crime records at r by the law broken (CRIME.law), ascending: an exchange sort with whole-record
   swaps (the crime list windows ai_Unk5b4340 / ai_Unk5cdb90 sort their copies of the records held) */

void gv_SortCrimesByLaw(int n, CRIME *r)
{
    int i, j;
    CRIME tmp;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (r[i].law > r[j].law) {
                tmp = r[i];
                r[i] = r[j];
                r[j] = tmp;
            }
        }
    }
}
/* 0x4f3a30, retires (cm_SendRetireCrime) up to n open crimes (state 1) of the culprit; every other player character
   (kind 6 / 7, the first 8) holding evidence of one of them (ai_Evidence) is told (ai_ShowEvidenceVoidNews). Returns
   how many crimes were retired. */

int gv_RetireCrimesOf(int culprit, int n)
{
    int cnt, i, k, j, m;
    short *c;
    int told[8];
    ORDCHAR *who[8];

    cnt = 0;
    c = gm_FindChar(culprit);
    m = 0;
    if (c != 0) {
        for (i = 0; i < 0x300; i++) {
            if (m >= 8)
                break;
            if ((ORD_CHARS[i].kind == 6 || ORD_CHARS[i].kind == 7) && ORD_CHARS[i].netId != culprit) {
                told[m] = 0;
                who[m] = &ORD_CHARS[i];
                m++;
            }
        }
    }
    for (i = 0; i < 0x200; i++) {
        if (cnt >= n)
            break;
        if (gv_CrimesT[i].culprit == culprit && gv_CrimesT[i].state == 1) {
            for (k = 0; k < 0x800; k++) {
                for (j = 0; j < m; j++) {
                    if (ai_Evidence[k][0] == who[j]->netId && ai_Evidence[k][1] == gv_CrimesT[i].id)
                        told[j] = 1;
                }
            }
            cm_SendRetireCrime(gv_CrimesT[i].id, 1);
            cnt++;
        }
    }
    for (j = 0; j < m; j++) {
        if (told[j] != 0)
            ai_ShowEvidenceVoidNews((int)who[j], (unsigned short *)c);
    }
    return cnt;
}
/* 0x4f3b40, as gv_RetireCrimesOf for the n crimes whose ids are listed in ids[] (those that exist in ai_Crimes); returns
   how many were retired */

int gv_RetireCrimeIds(int culprit, int n, int *ids)
{
    int cnt, i, k, e, j, m, id;
    short *c;
    int told[8];
    ORDCHAR *who[8];

    cnt = 0;
    c = gm_FindChar(culprit);
    m = 0;
    if (c != 0) {
        for (i = 0; i < 0x300; i++) {
            if (m >= 8)
                break;
            if ((ORD_CHARS[i].kind == 6 || ORD_CHARS[i].kind == 7) && ORD_CHARS[i].netId != culprit) {
                told[m] = 0;
                who[m] = &ORD_CHARS[i];
                m++;
            }
        }
    }
    for (i = 0; i < n; i++) {
        id = ids[i];
        for (k = 0; k < 0x200; k++) {
            if (gv_CrimesT[k].id == id) {
                for (e = 0; e < 0x800; e++) {
                    for (j = 0; j < m; j++) {
                        if (ai_Evidence[e][0] == who[j]->netId && ai_Evidence[e][1] == id)
                            told[j] = 1;
                    }
                }
                cm_SendRetireCrime(id, 1);
                cnt++;
                break;
            }
        }
    }
    for (i = 0; i < m; i++) {
        if (told[i] != 0)
            ai_ShowEvidenceVoidNews((int)who[i], (unsigned short *)c);
    }
    return cnt;
}
/* 0x4f3c50, the culprits of the open crimes (state 1) the character t holds evidence of (ai_Evidence): counts the crimes
   per culprit and keeps the 32 most frequent in ids[] / cnt[], sorted by count (descending, one insertion pass per
   culprit); ids is first -1, cnt 0; returns the number of different culprits (at most 0x20)
   @deep read whole function against the original: all statements reconciled (init, memset of the culprit
   list, evidence walk with gv_FindCrime, count + insertion pass); only the register of n++ differs: ours
   'mov ecx,[esp+0x10]; inc ecx; mov [esp+0x10],ecx' before 'mov ecx,1' (c), the original's uses ebp and
   stores after 'mov ecx,1' (0x4f3d15..0x4f3d27); statement orders tried (n++ before/after culprits[i]=-1 /
   c=1), this one is closest */

int gv_RankCulprits(int *ids, int *cnt, unsigned short t)
{
    int culprits[0x200];
    int n, m, i, k, c, own, hi, x;
    ORDCHAR *ch;

    n = 0;
    for (i = 0; i < 0x20; i++) {
        ids[i] = -1;
        cnt[i] = 0;
    }
    memset(culprits, 0xff, sizeof(culprits));
    m = 0;
    ch = &ORD_CHARS[t];
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == ch->netId) {
            k = gv_FindCrime(ai_Evidence[i][1]);
            if (k != -1 && gv_CrimesT[k].state == 1)
                culprits[m++] = gv_CrimesT[k].culprit;
        }
    }
    for (i = 0; i < 0x200; i++) {
        own = culprits[i];
        if (own != -1) {
            n++;
            culprits[i] = -1;
            c = 1;
            for (k = i + 1; k < 0x200; k++) {
                if (culprits[k] == own) {
                    c++;
                    culprits[k] = -1;
                }
            }
            if (c > cnt[0x1f]) {
                cnt[0x1f] = c;
                ids[0x1f] = own;
            }
            for (k = 0x1f; k > 0; k--) {
                hi = cnt[k];
                if (hi > cnt[k - 1]) {
                    x = ids[k];
                    ids[k] = ids[k - 1];
                    cnt[k] = cnt[k - 1];
                    ids[k - 1] = x;
                    cnt[k - 1] = hi;
                }
            }
        }
    }
    if (n > 0x20)
        n = 0x20;
    return n;
}
/* 0x4f3dd0, as gv_RankCulprits over all the crimes (state not 0) the character t holds evidence of: per culprit the
   count of crimes (tot) and of the open ones (state 1, st); the 32 culprits with the most crimes are kept in ids[] /
   open[] / total[], sorted by total (descending, one insertion pass per culprit); returns the number of culprits (at
   most 0x20). The test of a new culprit compares tot with open[0x1f], not total[0x1f] (as the original does).
   @deep read whole function against the original and reconciled every statement (memsets, evidence walk
   with gv_FindCrime and the unsigned state test, per-culprit count of total/open, the open[0x1f] compare
   as the original codes it, the 3-array insertion pass, the clamp); instruction counts equal (154/154),
   only stack-slot assignment differs: the m/i+1/&rec[i] slots rotate (0x1c/0x20/0x24) and in the sort the
   counter/x/y slots rotate (ours 0x10/0x18/0x28, the original's 0x18/0x28/0x10: the original's counter
   shares own's slot and y st's), plus 'xor edi,edi; mov [m],edi' scheduled before 'mov esi,
   &ai_Evidence[0][1]' at 0x4f3e25 */

int gv_RankCulpritsAll(int *ids, int *open, int *total, unsigned short t)
{
    CRIME *rec[0x200];
    int n, m, i, k, tot, st, own, x, y, id;
    ORDCHAR *ch;

    memset(ids, 0xff, 0x20 * sizeof(int));
    n = 0;
    memset(open, 0, 0x20 * sizeof(int));
    memset(total, 0, 0x20 * sizeof(int));
    memset(rec, 0, sizeof(rec));
    m = 0;
    ch = &ORD_CHARS[t];
    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == ch->netId) {
            k = gv_FindCrime(ai_Evidence[i][1]);
            if (k != -1 && (unsigned)gv_CrimesT[k].state > 0)
                rec[m++] = &gv_CrimesT[k];
        }
    }
    for (i = 0; i < m; i++) {
        tot = 1;
        st = 0;
        if (rec[i] != 0) {
            own = rec[i]->culprit;
            n++;
            if (rec[i]->state == 1)
                st = 1;
            for (k = i + 1; k < 0x200; k++) {
                if (rec[k] != 0 && rec[k]->culprit == rec[i]->culprit) {
                    tot++;
                    if (rec[k]->state == 1)
                        st++;
                    rec[k] = 0;
                }
            }
            rec[i] = 0;
            if (tot > open[0x1f]) {
                open[0x1f] = st;
                total[0x1f] = tot;
                ids[0x1f] = own;
            }
            for (k = 0x1f; k != 0; k--) {
                x = total[k];
                if (x > total[k - 1]) {
                    id = ids[k];
                    y = open[k];
                    ids[k] = ids[k - 1];
                    total[k] = total[k - 1];
                    open[k] = open[k - 1];
                    ids[k - 1] = id;
                    total[k - 1] = x;
                    open[k - 1] = y;
                }
            }
        }
    }
    return n > 0x20 ? 0x20 : n;
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
/* 0x4f2750, sets the AI decision record i (AIDEC.cur) to v, clamped to the record's min .. max: sends message 0x46
   (cm_SendDecisionValue) with the netId of the character c (-1 for none); -1 for a bad record or a free character */
int gv_SetTableRecord(AICHAR *c, int i, int v)
{
    AIDEC rec;
    GVDECMSG m;
    int id;

    if (gv_GetTableRecord(i, (int *)&rec) == 0)
        return -1;
    if (c == 0)
        id = -1;
    else {
        if (c->id == 0xffff)
            return -1;
        id = c->netId;
    }
    if (v < rec.min)
        v = rec.min;
    else if (v > rec.max)
        v = rec.max;
    m.id = id;
    m.rec = (unsigned char)i;
    m.value = v;
    return cm_SendDecisionValue((char *)&m);
}
/* 0x4f27d0, the handler of message 0x46 (gv_SetTableRecord): stores the value as the decision record's AIDEC.cur and,
   unless the character is the player's own, tells it (ai_ShowCharacterNews with gv_TableRecordText); 0 for a bad record */

int gv_OnDecisionValue(GVDECMSG *p)
{
    AIDEC rec;
    char buf[0x100];
    short *c;

    if (gv_GetTableRecord(p->rec, (int *)&rec) == 0)
        return 0;
    gv_Decisions[p->rec].cur = p->value;
    if (gv_CharsT[ai_PlayerId].netId != p->id) {
        c = gm_FindChar(p->id);
        if (c != 0) {
            gv_TableRecordText(buf, p->rec);
            ai_ShowCharacterNews(c, buf);
        }
    }
    return 1;
}
/* 0x4f2e30, the character a commits a crime against the character b at the object obj by breaking the law (AI decision
   record) i with the value v: the law's rule (AIDEC.rule 1 .. 7) compares its current value with v; when it is
   broken, unless the culprit's school level (gv_SchoolLevelFactor) beats a random float or, holding the item 0x22a, a
   second random float is above 0.5, the crime is reported (cm_SendCrime); -1 when nothing is reported */

int gv_Accuse(int i, int v, int a, int b, int obj)
{
    AIDEC rec;
    char *culprit;
    int place;
    float escape;
    CRIME m;

    if (gv_GetTableRecord(i, (int *)&rec) == 0)
        return -1;
    culprit = (char *)gm_FindChar(a);
    gm_FindChar(b);
    gm_FindById(&place, 0, 0, obj);
    switch ((unsigned char)rec.rule) {
    case 1:
        if (rec.cur != v)
            goto unreported;
        break;
    case 2:
        if (rec.cur == v)
            goto unreported;
        break;
    case 6:
        if (rec.cur > v)
            goto unreported;
        break;
    case 5:
        if (rec.cur >= v)
            goto unreported;
        break;
    case 4:
        if (rec.cur < v)
            goto unreported;
        break;
    case 3:
        if (rec.cur <= v)
            goto unreported;
        break;
    case 7:
        if (rec.cur == v)
            goto unreported;
        if (rec.cur == 2)
            goto unreported;
        break;
    default:
        goto unreported;
    }
    escape = gv_SchoolLevelFactor((short *)culprit);
    if (util_RandomF() < escape)
        goto unreported;
    if (ai_InventoryHas(culprit, 0x22a) != 0 && util_RandomF() > 0.5)
        goto unreported;
    m.time = gv_TimeT;
    m.v1d = v;
    m.law = (unsigned char)rec.id;
    m.victim = b;
    m.culprit = a;
    m.count = (short)rec.crimeCount;
    m.state = 1;
    m.place = obj;
    return cm_SendCrime(&m);
unreported:
    return -1;
}
/* 0x4f1de0, the turn step of the graveyard building o (main.c): walks its used grave entries (gv_FindEntry with
   gv_EntryIsUsed, all sizes); after 0x40 orders it waits for the last one's answer (cm_GetSlotState). An entry whose
   holder is gone is freed (gv_OrderSetGraveHolder) when no dead lies in it, else its care b0c wears down by
   b0c / 255 * gv_GraveWear[size] * (random + 1) / 2 (gv_OrderAdjustEntry, on every other turn by the entry's parity);
   a held entry wears the same way. With an owner that is not the player: a holder other than the owner whose grave
   is in bad care (below 0.66) loses relation to the owner (cm_SendRelations -5 .. -20, by the year and the holder's
   netId); when the lease runs out (GVENTRY.until == the year) it is renewed for 10 years (gv_OrderExtendGrave): free
   for the owner, offered to a player (message 0x92, AIMSG.lease), paid by a character with enough money (0.44 of
   gm_CharCurrency above gv_LeasePrice), else the entry is freed. At the end the graveyard gets 1 .. 2 new dead while
   fewer than 4 graves are in use (gv_OrderNewDead), and a player owner is told the paid leases (ai_TellGravesPaid)
   and the angered holders (ai_TellGraveNeglect)
   @deep read the whole function against the original 0x4f1de0..0x4f229b (the wait block, the gone-holder
   search with gv_OrderSetGraveHolder / wear, the held-entry wear, the relations check, the lease end with
   gv_LeasePrice / gv_OrderExtendGrave / the 0x92 offer (AIMSG.lease) / gm_CharCurrency * 0.44 / release,
   the tail with gv_GravesInUse / gv_OrderNewDead / the two reports): every statement, callee, argument,
   field and constant matches (fixed this pass: the sum += share of paid leases, the 0.44 money test, the
   message layout +4 type / +8 to / +0xc from / +0x28 time / +0x36 flags 9 / +0x58.. the lease, the
   0x5ee044 wear table, gv_LeasePrice's byte care argument, the c != 0 path's cached yard). Left
   (diffclass: order 114, code 6): the original keeps the wait loop unrotated ('+ jmp 0x4f1e67' back to the
   cm_GetSlotState test) where ours duplicates the test at the bottom ('- push edi / - call cm_GetSlotState
   / - test eax,eax / - je'); tried while (== 0), while (!..), for(;;) with break, the reset inside the
   break block and a goto loop: all rotate. The float wear product is scheduled b0c * (1/255) * table in
   the original, ours b0c * table * f * (1/255) (VC6 moves the literal; written as / 255.0f and *
   0.003921569f, grouped both ways: same) */

void gv_GraveyardTurn(char *o)
{
    AIBLD *bld;
    AICHAR *pl;
    AICHAR *c;
    GVENTRY *e;
    GVGRAVE *g;
    GVYARD *yard;
    AIMSG m;
    int h, rounds, nrel, paid, sum;
    int k, t, size, price, share;
    float f;

    bld = (AIBLD *)o;
    h = -1;
    pl = 0;
    rounds = 0;
    nrel = 0;
    paid = 0;
    sum = 0;
    if (bld->owner != 0xffff && bld->owner != *ai_PlayerChar)
        pl = &gv_CharsT[bld->owner];
    e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)bld, 0, gv_EntryIsUsed, 0xff);
    while (e != 0) {
        if (rounds >= 0x40 && h != -1) {
            while (cm_GetSlotState(h) == 0) {
                cm_FlushSendQueue();
                cm_ReceiveMessages();
                cm_ExecCommands();
            }
            rounds = 0;
            h = -1;
        }
        c = (AICHAR *)gm_FindChar(e->id);
        if (c == 0) {
            size = e->size;
            for (k = 0, g = e->grave; k < gv_EntryCounts[size]; k++, g++) {
                if (g->id != -1) {
                    f = (util_RandomF() + 1.0f) * 0.5f;
                    t = (int)(f * ((float)e->b0c / 255.0f * gv_GraveWear[e->size]));
                    if (t <= 0 || ((gv_EntryIndex((char *)bld->yard, (int *)e) ^ gv_TimeT.year) & 1) != 0)
                        goto next;
                    h = gv_OrderAdjustEntry(o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e), -t);
                    goto counted;
                }
            }
            h = gv_OrderSetGraveHolder(0, o, size, gv_EntryIndex((char *)bld->yard, (int *)e));
        } else {
            f = (util_RandomF() + 1.0f) * 0.5f;
            size = e->size;
            t = (int)(f * ((float)e->b0c / 255.0f * gv_GraveWear[size]));
            if (t > 0 && ((gv_EntryIndex((char *)(yard = bld->yard), (int *)e) ^ gv_TimeT.year) & 1) == 0) {
                h = gv_OrderAdjustEntry(o, size, gv_EntryIndex((char *)yard, (int *)e), -t);
                rounds++;
            }
            if (pl == 0)
                goto next;
            if (c != pl && gv_TimeT.year % 4 == (int)(c->netId & 3)
                && (float)e->b0c / 255.0f < 0.66f) {
                h = cm_SendRelations(pl->netId, c->netId, -5 - util_RandomU(0x10), 1);
                rounds++;
                nrel++;
            }
            if (e->until != gv_TimeT.year)
                goto next;
            h = price = gv_LeasePrice(&c->id, e->b0c, e->size, 0, &share);
            if (c == pl) {
                h = gv_OrderExtendGrave((int *)c, o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e), 10, 0);
            } else if (c->kind == 6 || c->kind == 7) {
                memset(&m, 0, sizeof(m));
                m.to = c->netId;
                m.fromId = bld->id;
                m.time = gv_TimeT;
                m.type = 0x92;
                m.flags = 9;
                m.lease.bld = bld->id;
                m.lease.size = e->size;
                m.lease.entry = (unsigned char)gv_EntryIndex((char *)bld->yard, (int *)e);
                m.lease.price = price;
                m.lease.years = 10;
                h = cm_SendLongMessage((char *)&m);
            } else if ((int)(gm_CharCurrency((char *)c, ai_OwnPlayer) * 0.44f) > price) {
                h = gv_OrderExtendGrave((int *)c, o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e), 10, price);
                rounds++;
                paid++;
                sum += share;
                goto next;
            } else {
                h = gv_OrderSetGraveHolder(0, o, e->size, gv_EntryIndex((char *)bld->yard, (int *)e));
            }
        }
    counted:
        rounds++;
    next:
        e = (GVENTRY *)gv_FindEntry(0, (unsigned char *)bld, (unsigned)e, gv_EntryIsUsed, 0xff);
    }
    if (gv_GravesInUse((unsigned char *)bld) < 4)
        gv_OrderNewDead(o, util_RandomU(2) + 1);
    if (pl != 0 && (pl->kind == 6 || pl->kind == 7)) {
        if (paid > 0)
            ai_TellGravesPaid((int)pl, (int)bld, paid, sum);
        if (nrel != 0)
            ai_TellGraveNeglect((int)pl, (int)bld);
    }
}

extern float ord_RatingRate;
extern char ord_StatisticsHistory[320];
extern char ord_RatingBytes[16];
extern char ord_MessageRecord[0x24];
const char ai_ZeroRot[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x5ebbb4 */
const float gv_SaleShares[] = { 0.800000012f, 0.699999988f, 0.600000024f, 0.5f };  /* 0x5ee034, the owner's share of a grave sale per size */
const float gv_GraveWear[] = { 15.0f, 12.5f, 10.0f, 7.5f };  /* 0x5ee044, the care a grave entry of each size loses per step (gv_GraveyardTurn: times (random + 1) / 2 and the care / 255) */
const int gv_EntryCounts[] = { 1, 2, 4, 8 };  /* 0x5ee054, entries per kind */
const int gv_EntryTotals[] = { 0, 12, 20, 30 };  /* 0x5ee064 */
const int gv_GraveTotals[] = { 0, 16, 32, 64 };  /* 0x5ee074 */
const int gv_ListEntries[4][4] = { { 0, 8, 12, 16 }, { 0, 4, 6, 8 }, { 0, 0, 2, 4 }, { 0, 0, 0, 2 } };  /* 0x5ee084 */
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
