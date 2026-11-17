/* gv.c -- the graveyard module (tags "gv_AllocGraveyardField" / "gv_Alloc:_all_graves" 0x61120c..0x611240) with
   the avatar table 0x60f310 (32 records of 0xf4 bytes, first name "neVmMaven") and the effect slots 0x6f1008
   (8 of 0x170 bytes, "kugel" 0x611204). 0x4ed170..0x4f4e20: starts at the first user of the table 0x60f310,
   which begins the .data block after ahm.c's "ahm_AllocAhmInfo" 0x60f2f8; ends where ord.c's order handlers
   begin (their table 0x611768 follows gv's strings). The last functions of the file (0x4f2750 .. 0x4f1de0) are
   out of address order. Names are ours.
   The effect slots are GVEFFECT records read through the gv_EffectsT view: a building's cannon salvos, three parts
   each that are SHOTREC cannon balls (menu.h) fired with gv_FireShots (message 0x67, GVSHOTMSG), started by
   gv_StartShot, moved by gv_UpdateEffects and exploded by gv_ExplodeShot; the save game writes gv_LastTime, the cannon
   scene state gv_Cannon (cannon scene of sv_t) and the slots field by field.
   A graveyard building (kind 0x1d) holds a GVYARD (AIBLD.yard): four grave sizes, each a list of GVENTRY entries opened by
   GVYARD.open, counted per type level by gv_ListEntries / gv_EntryCount / gv_GraveCount and walked by gv_FindEntry; each
   entry points at its GVGRAVE records (a dead person). Graveyard orders are GVORDER records (message 0x60, sent by the
   gv_Order* senders through cm_SendGraveOrder, run by type: gv_BuryCharacter, gv_ReserveGrave, gv_ExtendGrave,
   gv_SetGraveHolder, gv_OpenSize, gv_AdjustEntry, ..); a grave move is GVMOVE (message 0x61, gv_OrderMoveGrave ->
   gv_MoveGrave), a raising of a dead person GVRAISE (message 0x62, gv_OrderRaiseDead -> gv_RaiseDead): the raised dead are
   kind 0x14 characters living at the graveyard (gv_RoomForRaised, decaying by gv_DecayRaisedDead); gv_GraveyardTurn is the
   building's turn step (care wear, lease ends via gv_LeasePrice / gv_OrderExtendGrave, new dead).
   The AI decision (law) table 0x611258 (gv_Decisions, 0x1d AIDEC records whose fn slots are the ai_Decide* callbacks of am.c)
   is read and set by gv_GetTableRecord / gv_SetTableRecord (message 0x46, GVDECMSG, gv_OnDecisionValue); breaking a law
   is a crime (gv_Accuse -> cm_SendCrime): the CRIME records ai_Crimes (cm.c, viewed as gv_CrimesT) and the evidence pairs
   ai_Evidence, saved by gv_WriteCrimes / gv_ReadCrimes.
   Open: the avatars, the effect slots, the graveyard and the crimes may be several modules sharing one .data block.
   The crime records' life: gv_FreeCrimeSlot / gv_FindCrime find them, gv_HandOverCrimes (message 0x3c) hands them over,
   gv_RetireCrime (message 0x23, sent by gv_SendCrimes) frees them; gv_AddEvidence / gv_EvidenceSlot keep the evidence
   pairs; gv_CulpritGuilt sums AIDEC.guilt of a culprit's open crimes; gv_RetireCrimesOf / gv_RetireCrimeIds retire them
   and tell the evidence holders; gv_HeldCrimesOf / gv_HeldCrimeIdsOf / gv_HeldCrimeCount and gv_RankCulprits /
   gv_RankCulpritsAll read what a character holds evidence of; gv_CrimeText (by law: gv_CrimeTextLaw2to8 / 9to16 /
   17to28) words a crime; gv_FindAccused / gv_EvidencePrice / gv_SortCrimesByLaw / gv_ClearCrimes serve the crime windows
   and the evidence trade. A trial's sentence 0 .. 9 is checked by gv_CheckSentence and carried out by gv_PassSentence;
   gv_CheckRankUp raises a character's rank (AICHAR.b00d) by his worth (gv_RankWorth).
   Open: gv_FindCrime and gv_EvidenceSlot still read ai_Crimes (cm.c's char[], the 0x2d-byte CRIME records) and ai_Evidence (cm.c, the int pairs) through raw offsets.
   Open: some functions index ai_PlayerSlots with the stride 0x5e8 where gv_ReserveGrave shows 0x2f4 (gv_PlayerRecs).
   Open: head: cm.h redeclares gv functions (gv_GrowGraveyard as struct GVYARD *); cm.c cannot include gv.h yet (two
   calls conflict: gv_CrimeText, gv_PassSentence).
   Open: GVEFFPART duplicates SHOTREC and UNKHITS-style copies remain (am.h G9KIND / G9CANNON view gv's records); gv.h
   does not include menu.h, so the parts are cast to SHOTREC.
   unit of rea 8cef7d5b (src/game/gv.c); boundaries in decl/text.dcl */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "am.h"                       /* the decision callbacks ai_Decide* of gv_Decisions (AIDEC.fn) */
#include "d3_types.h"                 /* D3COLOR, for d3_tools.h */
#include "d3_tools.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define gv_PlayerAiChar (*(AICHAR **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) as the AICHAR it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define gv_CrimesT (*(CRIME (*)[0x200])&ai_Crimes)  /* 0x91f700, ai_Crimes (cm.c: char[0x5a00]) as its 0x200 CRIME records */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define GAPDATA __declspec(align(1)) unsigned char
#define gv_EffectsT (*(GVEFFECT (*)[8])&gv_Effects)  /* 0x6f1008, gv_Effects (char[0xb80]) as the 8 GVEFFECT slots */
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
#include "mission.h"
#include "lc.h"
#include "vfs.h"
#include "d3_interface.h"
#include "d3_pose.h"
#include "d3_engine.h"
#include "menu.h"
#include "net.h"
extern int gm_PlayerRecords[688];

extern const float gv_SaleShares[];
extern const float gv_GraveWear[];

#define gv_AvatarsT (*(GVAVATAR (*)[0x20])&unk_Avatars)  /* 0x60f310, unk_Avatars (char[0x20][0xf4]) as the GVAVATAR records they are */
#define gv_FindEntryCb(cb) ((int (*)(void *, void *))(cb))  /* @original: gv_FindEntry's callback is int (*)() (gv.h, its callbacks take differing pointer types); the call pushes two pointers (0x4ef8df push esi, 0x4ef8e0 push ebx) and reads an int (0x4ef8e6 test eax) */

/* 0x4ed170, picks a free avatar record (byte +0xf2 = 0) starting at a random one of the 32 (cyclic), marks it used (+0xf2 = 1) after setting the 14 floats at +0x30, +0x3c .. to the floats before them (+0x2c ..)
   times 0.001, and returns it; 0 when all are in use */

char *gv_AllocAvatar(void)
{
    unsigned i;
    int n;
    int j;

    i = util_RandomU(0x20) & 0xffff;
    n = 0x20;
    do {
        if (gv_AvatarsT[i].used == 0) {
            for (j = 0; j < 0xe; j++)
                gv_AvatarsT[i].mood[j][1] = (float)(gv_AvatarsT[i].mood[j][0] * 0.001);
            gv_AvatarsT[i].used = 1;
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
            name[4] = 0;
            name[1] = (char)(id >> 16);
            name[0] = (char)(id >> 24);
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
   the tail); 1 when all was written */

int gv_WriteEffects(void *f)
{
    int i;
    GVEFFECT *e;
    GVEFFPART *p;

    if (vfs_fwrite(&gv_LastTime.year, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_LastTime.hour, 2, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_LastTime.min, 4, 1, (struct VFSFILE *)f) <= 0)
        return 0;
    if (vfs_fwrite(&gv_LastTime.sec, 4, 1, (struct VFSFILE *)f) <= 0)
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
        p = &e->parts[0];
        if (vfs_fwrite(p->v0, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v1, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v2, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v4, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->a, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->b, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->state, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->steps, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        p = &e->parts[1];
        if (vfs_fwrite(p->v0, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v1, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v2, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v4, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->a, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->b, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->state, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->steps, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        p = &e->parts[2];
        if (vfs_fwrite(p->v0, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v1, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v2, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(p->v4, 16, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->a, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->b, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->c, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->state, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(&p->steps, 4, 1, (struct VFSFILE *)f) <= 0)
            return 0;
        if (vfs_fwrite(e->tail, 12, 1, (struct VFSFILE *)f) <= 0)
            return 0;
    }
    return 1;
}
/* 0x4ed910, reads what gv_WriteEffects wrote from the save file f (version v: the saves before 0x10055 have no fields
   f0c / f10, they are 1.0); sets each slot's callback to gv_EffectNotifyOwner with its gv_EffectArgs record and the
   parts' time to d3_Time; 1 when all was read */

int gv_ReadEffects(void *f, int v)
{
    int i;
    GVEFFECT *e;
    GVEFFARG *q;
    GVEFFPART *p;

    if (ls_Read(&gv_LastTime.year, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_LastTime.hour, 2, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_LastTime.min, 4, 1, (int)f) == 0)
        return 0;
    if (ls_Read(&gv_LastTime.sec, 4, 1, (int)f) == 0)
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
        p = &e->parts[0];
        if (ls_Read(p->v0, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v1, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v2, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v4, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->a, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->b, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->c, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->state, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->steps, 4, 1, (int)f) == 0)
            return 0;
        p->time = d3_Time;
        p = &e->parts[1];
        if (ls_Read(p->v0, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v1, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v2, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v4, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->a, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->b, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->c, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->state, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->steps, 4, 1, (int)f) == 0)
            return 0;
        p->time = d3_Time;
        p = &e->parts[2];
        if (ls_Read(p->v0, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v1, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v2, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(p->v4, 16, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->a, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->b, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->c, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->state, 4, 1, (int)f) == 0)
            return 0;
        if (ls_Read(&p->steps, 4, 1, (int)f) == 0)
            return 0;
        p->time = d3_Time;
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
    gv_LastTime = ai_Time;
    memset(&gv_Cannon.bld, 0, 0x20);
    gv_Cannon.bld = -1;
}
/* 0x4ede70, frees the objects (3 per slot at +0x15c, 0x447190) of all effect slots; the scene is switched with 0x4478e0 around it */

void gv_FreeEffectObjects(void)
{
    int sc;
    struct D3OBJECT **q;
    int i, k;

    sc = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    k = 0;
    do {
        i = 3;
        q = gv_EffectsT[k].objs;
        do {
            if (*q != 0) {
                d3if_Delete(*q);
                *q = 0;
            }
            q++;
            i--;
        } while (i != 0);
        k++;
    } while (k < 8);
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


/* 0x4edf80, the per-frame update of the effect slots (the cannon balls in flight): the minutes since gv_LastTime (* 1/60 * 0.1) are added
   to the three fade values of every used slot (up to 1.0); each part in flight (state 1) moves one step (ai_StepShot, ai_StepShotSlow
   when the slot's f14 is set) and gets its "kugel" object placed at the ball; a ball that left the map (3) or landed (2: the explosion
   gv_ExplodeShot) loses its object and is reported to the slot's callback (arg->cur = the part); a slot with three idle parts loses flags
   1 / 2 and is released when flag 4 is set, as is a slot without flag 2 */

void gv_UpdateEffects(void)
{
    int dt;
    int scene;
    float step;
    int i, j;
    int idle;

    dt = gm_MinutesBetween(&gv_LastTime, &ai_Time);
    if (dt < 0)
        dt = 0;
    gv_LastTime = ai_Time;
    scene = d3_CurrentScene;
    step = (float)dt * 0.016666668f;
    step = step * 0.1f;
    d3if_SelectScene(0, 1);
    for (i = 0; i < 8; i++) {
        if (gv_EffectsT[i].id == -1)
            continue;
        idle = 0;
        gv_EffectsT[i].f08 += step;
        gv_EffectsT[i].f0c += step;
        gv_EffectsT[i].f10 += step;
        gv_EffectsT[i].f08 = gv_EffectsT[i].f08 < 1.0f ? gv_EffectsT[i].f08 : 1.0f;
        gv_EffectsT[i].f0c = gv_EffectsT[i].f0c < 1.0f ? gv_EffectsT[i].f0c : 1.0f;
        gv_EffectsT[i].f10 = gv_EffectsT[i].f10 < 1.0f ? gv_EffectsT[i].f10 : 1.0f;
        if ((gv_EffectsT[i].flags & 2) == 0) {
            if (gv_EffectsT[i].flags & 4)
                gv_ReleaseEffect(gv_EffectsT[i].id);
            continue;
        }
        for (j = 0; j < 3; j++) {
            if (gv_EffectsT[i].parts[j].state == 0) {
                if (++idle >= 3) {
                    gv_EffectsT[i].flags &= ~3;
                    if (gv_EffectsT[i].flags & 4)
                        gv_ReleaseEffect(gv_EffectsT[i].id);
                }
            } else if (gv_EffectsT[i].parts[j].state == 1) {
                if (gv_EffectsT[i].f14 == 0 ? ai_StepShot((SHOTREC *)&gv_EffectsT[i].parts[j]) : ai_StepShotSlow((SHOTREC *)&gv_EffectsT[i].parts[j])) {
                    if (gv_EffectsT[i].objs[j] == 0) {
                        gv_EffectsT[i].objs[j] = d3_CreateObject(0, &d3_Origin, &d3_ZeroRot, "kugel");
                        gv_EffectsT[i].objs[j]->objectFlags |= 4;
                        gv_EffectsT[i].objs[j]->stateFlags |= 0xc;
                        gv_EffectsT[i].objs[j]->objectFlags &= ~8;
                    }
                    d3_SetObjectPos(gv_EffectsT[i].objs[j], (D3VECTOR *)gv_EffectsT[i].parts[j].v3);
                }
                if (gv_EffectsT[i].parts[j].state == 3) {
                    if (gv_EffectsT[i].objs[j] != 0) {
                        d3if_Delete(gv_EffectsT[i].objs[j]);
                        gv_EffectsT[i].objs[j] = 0;
                    }
                    if (gv_EffectsT[i].arg != 0)
                        gv_EffectsT[i].arg->cur = j;
                    gv_EffectsT[i].arg->recs[j].bldId = -1;
                    if (gv_EffectsT[i].cb != 0)
                        gv_EffectsT[i].cb(gv_EffectsT[i].arg);
                    gv_EffectsT[i].parts[j].state = 0;
                    gv_EffectsT[i].tail[j] = 0;
                } else if (gv_EffectsT[i].parts[j].state == 2) {
                    idle++;
                    if (gv_EffectsT[i].objs[j] != 0) {
                        d3if_Delete(gv_EffectsT[i].objs[j]);
                        gv_EffectsT[i].objs[j] = 0;
                    }
                    gv_ExplodeShot(gv_EffectsT[i].flags & 1, gv_EffectsT[i].id, (SHOTREC *)&gv_EffectsT[i].parts[j], gv_EffectsT[i].arg);
                    if (gv_EffectsT[i].arg != 0)
                        gv_EffectsT[i].arg->cur = j;
                    if (gv_EffectsT[i].cb != 0)
                        gv_EffectsT[i].cb(gv_EffectsT[i].arg);
                    gv_EffectsT[i].parts[j].state = 0;
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
/* 0x4ee360, the explosion of a cannon ball that landed (gv_UpdateEffects): finds the buildings (ai_Blds) whose box (d3t_GetMeshBounds
   around the world position) holds the ball's position (mode 1, weight 1 / (1 + 0.005 d)) or lies within twice a tenth of its energy
   of it (mode 2, weight 1 / (1 + (0.0002 d + 0.005) d)); of those with energy * weight > 100 the 3 strongest go to the records of out
   (the strength / 100, up to 100); starts the smoke, earth and explosion particles at the ball (bigger when a building was hit) and
   the sound "Explosion" at the nearest building; with damage set the owners of the hit buildings get message 0x4f with the strength
   (mode 1), or the building's level (field 0x5c) falls by it (mode 2)
   @deep kept: c copied field by field (shot in ebx pushed after the init loop, as the original), fd
   through dx (dx = sqrt(...); fd = dx: the original's fstp st(3)), fv = w * energy in each arm, pos / lo
   as D3VECTOR4 (the original's frame strides its vectors by 16): 383 -> 300 ins, frame 0x1a8 -> 0x1b0 of
   0x1b4. Every other vector subset as D3VECTOR4 tried (63 rows): worse. No change (= control): mode = 0
   at the loop top or before the init loop, int / float declaration orders, w1 / w2 order, fv = w1 kept
   apart, w1 computed into fv, the loop's i in a for-init. Left: the original keeps zero and mode in ebp
   across d3_PointToWorld (cmp eax,ebp after the call; xor ebp,ebp at the back edge) where ours
   zero-tests in edx / test eax,eax and spills mode to a slot; sel in esi (ours ebp); w1 computed before
   d3t_GetMeshBounds into a slot (fstp [esp+0x7c]) where VC6 forward-substitutes ours into the inside arm
   (the missing 4 frame bytes); the selection loop by pointer (lea edx,[esp+0xa8]) against our scaled
   index; the wind block with scale in a slot and wind x / z on the FPU stack. M10s on ai_FloatOne /
   ai_FloatHundred are pooled literals. (tiebreak: insensitive) prior: read the whole function against
   the original (hit record init, building loop with world position / distance / box tests as >= / <=
   chains, w1 / w2 weights, the 3-strongest selection, smoke / earth / explosion calls with float
   arguments through the real d3_tools prototypes and a D3COLOR, the sound, the damage messages as
   AIMSG); 469 vs 478 instructions, frame 0x1a8 vs 0x1b4. Left: register roles (ours zero in ebx / shot
   in esi / sel in ebp, the original's zero-and-mode in ebp / shot in ebx / sel in esi; hence ebx pushed
   after the init loop in the original), the original keeps w1 in a slot computed before
   d3t_GetMeshBounds where ours sinks it into the inside branch (fmul / fadd / fdivr after mov ebx,1),
   the selection loop walks &recs[k].value by pointer in the original (lea edx,[esp+0xa8]) and ours by a
   scaled index (mov esi,0xfffffff0), and the wind direction block keeps scale in a slot and wind x / z
   on the FPU stack (fld [ebx+0x40]; fld [ebx+0x48]; fld [esp+0x4c]; fmul st(2)) where ours folds them
   (fld [esi+0x4c]; fld st(0); fmul [esi+0x40]): FPU scheduling and allocation. Tried: sqrt without the
   float cast (no change), mode = 0 at the loop top (no change). */
void gv_ExplodeShot(int damage, int effectId, SHOTREC *shot, GVEFFARG *out)
{
    GVEFFARG h;
    D3VECTOR c;
    D3VECTOR half, mn, hi, dir;
    D3VECTOR4 pos, lo;
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
    c.x = shot->pos.x;
    c.y = shot->pos.y;
    c.z = shot->pos.z;
    for (i = 0; i < 256; i++) {
        sel = -1;
        m = shot->energy * 0.1f;
        bld = &ai_BldsT[i];
        if (bld->id == -1 || bld->type == 0 || bld->object == 0)
            continue;
        d3_PointToWorld((D3OBJECT *)bld->object, &((D3OBJECT *)bld->object)->pos, (D3VECTOR *)&pos);
        dx = c.x - pos.x;
        dy = c.y - pos.y;
        dz = c.z - pos.z;
        dx = sqrt(dz * dz + dy * dy + dx * dx);
        fd = dx;
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
            fv = w1 * shot->energy;
        } else {
            m = m + m;
            if (!(m + c.x >= lo.x && c.x - m <= hi.x && m + c.y >= lo.y && c.y - m <= hi.y && m + c.z >= lo.z && c.z - m <= hi.z))
                continue;
            mode = 2;
            fv = w2 * shot->energy;
        }
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
                msg.time = ai_Time;
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
/* 0x4eea70, the message 'nCTa' (0x4c6230) with the building bld's id; returns its result (ai_CannonScene waits on the slot it names) */

int gv_RequestEffectLoad(struct AIBLD *bld)
{
    int m[2];

    m[0] = 0x6154436e;
    m[1] = bld->id;
    return cm_SendEffectLoad(m);
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
   takes it (tail 1, the SHOTREC copied, state 1 in flight, time d3_Time, steps 0), m->v is taken off the slot's charge of
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
    e->parts[j].time = d3_Time;
    e->parts[j].state = 1;
    e->parts[j].steps = 0;
    if (w != 0)
        e->flags |= 1;
    return 1;
}
/* 0x4eeee0, sets the callback (+0x168) and its argument (+0x16c) of the slot of the id; without a callback the default one (0x4ed3f0) with the slot's record 0x6f0e48 + 0x38 * n */

void gv_SetEffectCallback(int id, void *cb, void *arg)
{
    GVEFFECT *e;

    if (id != -1) {
        e = (GVEFFECT *)gv_GetEffect(id);
        if (e != 0) {
            if (cb != 0) {
                e->cb = (int (*)(GVEFFARG *))cb;
                e->arg = arg;
                return;
            }
            e->cb = gv_EffectNotifyOwner;
            e->arg = &gv_EffectArgs[e - gv_EffectsT];
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
    float a, v;
    int kind, i;

    kind = max(min(n, 4), 0);
    out[0] = kind;
    out[1] = kind + 2;
    for (i = 1; i <= 3; i++) {
        a = (float)i * ((float)kind * 0.25f + 1.0f);
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

float gv_CannonItemFactor(char *o)
{
    AIBLD *bld;
    float x;

    bld = (AIBLD *)o;
    x = 1.0f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x259) != 0)
        x = 1.05f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x25b) != 0)
        x = x + 0.05f;
    if (gm_Query(bld->objList, 2, 6, 0, 0x261) != 0)
        return x + 0.05f;
    return x;
}

/* the cannon kinds' balls (5 records, 0x5edfe0..0x5ee01c: this unit's first .rdata, before the literals of
   gv_UpdateEffects 0x4edf80; am.c / sv_t.c read [0 .. 2], the kinds 3 and 4 nothing addresses) */
const G9BALL ai_CannonBalls[5] = { { 2000.0f, 3000.0f, 0.0599999987f }, { 2450.0f, 3850.0f, 0.0649999976f }, { 3600.0f, 5950.0f, 0.075000003f }, { 4200.0f, 7200.0f, 0.0799999982f }, { 4800.0f, 8600.0f, 0.0850000009f } };  /* 0x5edfe0 */
char unk_Avatars[0x20][0xf4] = { { 110, 101, 86, 109, 77, 97, 118, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 102, 32, 82, 101, 97, 100, 105, 110, 103, 0, 0, 0, 0, 0, 0, 64, 32, 96, 96, 64, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, -128, 94, 68, 102, 102, 6, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, -102, -103, 41, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 41, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 22, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 35, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 71, 65, 82, 110, 82, 97, 110, 103, 97, 114, 111, 101, 107, 0, 0, 0, 0, 0, 0, 0, 76, 104, 117, 116, 105, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 10, 96, 64, 84, 0, 0, 0, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, -128, 94, 68, 102, 102, 6, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, -102, -103, 41, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 22, 64, 117, 2, 26, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 64, 98, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, 0, 92, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 100, 68, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 97, 68, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 41, 0, 1, 122, 105, 103, 101, 117, 110, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 101, 78, 87, 115, 83, 119, 105, 110, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 83, 101, 118, 101, 114, 105, 110, 103, 0, 0, 0, 0, 64, -128, 64, 32, 64, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 58, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 95, 88, 69, 68, 68, 101, 120, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 111, 111, 109, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 25, 64, 48, 40, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 7, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 84, 65, 77, 95, 77, 97, 116, 116, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 101, 114, 32, 72, 101, 105, 108, 105, 103, 101, 0, 0, 0, 0, 0, 32, 0, 64, 32, -128, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 14, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 67, 73, 86, 42, 86, 105, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 114, 97, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -128, 32, 64, 0, 0, 0, 102, 102, 38, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 25, 64, -61, 100, 42, 59, 0, -128, 94, 68, 102, 102, 6, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 32, 64, -61, 100, 42, 59, 0, -64, 97, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 99, 68, -51, -52, 44, 64, -61, 100, 42, 59, 0, 0, 102, 68, -102, -103, 41, 64, -61, 100, 42, 59, 0, -128, 94, 68, 51, 51, 51, 64, -61, 100, 42, 59, 0, -64, 100, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 64, 98, 68, 102, 102, 22, 64, -61, 100, 42, 59, 0, 0, 92, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 100, 68, 51, 51, 35, 64, -61, 100, 42, 59, 0, 0, 97, 68, 0, 0, 48, 64, -61, 100, 42, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 22, 0, 1, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 77, 103, 84, 115, 70, 114, 97, 110, 99, 111, 105, 115, 32, 100, 101, 0, 0, 0, 0, 0, 83, 97, 105, 110, 116, 45, 71, 101, 114, 109, 97, 105, 110, 0, 0, 0, 21, 0, 84, 21, 126, 0, 0, 0, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 6, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 19, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 6, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 32, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 64, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 51, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 0, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 113, 77, 76, 109, 68, 111, 110, 32, 65, 108, 102, 111, 110, 115, 111, 0, 0, 0, 0, 0, 100, 101, 32, 77, 101, 108, 109, 97, 113, 117, 101, 0, 0, 0, 0, 0, 42, 0, 63, 84, 42, 0, 0, 0, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 64, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 25, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 0, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 0, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 82, 70, 118, 87, 97, 108, 116, 104, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 70, -4, 114, 115, 116, 101, 110, 98, 101, 114, 103, 0, 42, 0, 21, 84, 105, 0, 0, 0, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 6, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 6, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 41, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 57, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, -51, -52, 44, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -82, 65, -98, -17, -89, 60, 0, -128, 99, 68, 3, 0, 0, 68, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 67, 82, 101, 114, 82, 111, 119, 101, 110, 110, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, 114, 105, 107, 115, 115, 111, 110, 0, 0, 0, 0, 0, 0, 0, 0, 63, 84, 21, 42, 42, 0, 0, 0, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 44, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 32, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 54, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 25, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 100, 68, 0, 0, 32, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -95, 65, -98, -17, -89, 60, 0, -128, 99, 68, 36, 1, 0, 90, 105, 103, 101, 117, 110, 101, 114, 105, 110, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 100, 108, 103, 83, 116, 101, 102, 97, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 111, 108, 100, 98, 101, 114, 103, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 0, 42, 42, 0, 0, 0, -51, -52, 28, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 32, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 28, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 94, 68, 102, 102, 38, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 35, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 9, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -96, 65, -98, -17, -89, 60, 0, -128, 99, 68, 61, 0, 0, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 50, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 67, 112, 103, 65, 66, 111, 107, 116, 111, 114, 117, 115, 0, 0, 0, 0, 0, 0, 0, 0, 65, 112, 97, 103, 97, 99, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 126, 0, 126, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 64, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 60, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 64, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 0, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 57, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -80, 65, -98, -17, -89, 60, 0, -128, 99, 68, 42, 0, 0, 114, 97, 101, 117, 98, 101, 114, 109, 101, 105, 115, 116, 101, 114, 95, 83, 79, 76, 68, 65, 84, 0, 0, 0, 0, -1, -1, 0, 0 }, { 102, 76, 100, 119, 67, 97, 116, 104, 114, 105, 110, 103, 105, 110, 0, 0, 0, 0, 0, 0, 87, 105, 100, 111, 108, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 63, 0, 42, 42, 0, 0, 0, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 51, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 22, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, 102, 102, 6, 64, 52, -128, 55, 59, 0, -64, 100, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 67, 1, 0, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 105, 110, 51, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 72, 78, 115, 69, 103, 105, 110, 104, 97, 114, 100, 0, 0, 0, 0, 0, 0, 0, 0, 69, 105, 115, 101, 110, 104, 101, 98, 101, 114, 0, 0, 0, 0, 0, 0, 84, 63, 0, 21, 84, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 16, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 102, 102, 38, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 16, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 48, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 41, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 28, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 48, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 35, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 67, 0, 1, 119, 105, 114, 116, 51, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 103, 68, 108, 103, 71, 101, 116, 114, 117, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 71, 111, 108, 100, 103, 105, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 63, 0, 21, 0, 0, 0, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 44, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 9, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 19, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 16, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 16, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -98, 65, -98, -17, -89, 60, 0, -128, 99, 68, 72, 1, 0, 66, 117, 101, 114, 103, 101, 114, 105, 110, 51, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 84, 86, 108, 104, 77, 97, 114, 99, 117, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 101, 108, 118, 101, 116, 105, 99, 117, 115, 0, 0, 0, 0, 0, 0, 84, 84, 21, 42, 21, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 44, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 41, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 28, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 12, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 64, 98, 68, -102, -103, 9, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 19, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -98, 65, -98, -17, -89, 60, 0, -128, 99, 68, 20, 0, 0, 98, 117, 101, 114, 103, 101, 114, 50, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 115, 108, 102, 83, 68, 97, 114, 105, 111, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 116, 117, 114, 109, 102, 101, 108, 115, 0, 0, 0, 0, 0, 0, 0, 0, 21, 42, 84, 105, 0, 0, 0, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 12, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 28, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 57, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 44, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 19, 64, -26, 29, 39, 59, 0, 0, 97, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -94, 65, -98, -17, -89, 60, 0, -128, 99, 68, 48, 0, 0, 98, 117, 101, 114, 103, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 76, 104, 99, 114, 65, 114, 109, 97, 110, 100, 32, 74, 101, 97, 110, 0, 0, 0, 0, 0, 100, 101, 32, 82, 105, 99, 104, 101, 108, 105, 101, 117, 0, 0, 0, 0, 42, 0, 84, 21, 105, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 51, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 28, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 60, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 60, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 16, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 100, 68, 0, 0, 16, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -85, 65, -98, -17, -89, 60, 0, -128, 99, 68, 13, 0, 0, 97, 98, 116, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 108, 71, 80, 83, 83, 97, 115, 99, 104, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 112, 105, 101, 103, 101, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 42, 42, 84, 0, 0, 0, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 12, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 38, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 38, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 25, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -82, 65, -98, -17, -89, 60, 0, -128, 99, 68, 27, 0, 0, 100, 105, 101, 98, 95, 77, 65, 78, 78, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 83, 116, 108, 119, 83, 97, 114, 105, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 87, 101, 108, 116, 101, 110, 115, 116, 101, 105, 110, 0, 0, 0, 0, 0, 42, 105, 42, 0, 63, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 60, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 60, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 12, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 57, 64, -96, -85, 45, 59, 0, -128, 94, 68, 102, 102, 38, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 28, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 6, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 16, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 35, 0, 1, 80, 97, 116, 114, 105, 122, 105, 101, 114, 95, 77, 65, 78, 78, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 115, 76, 98, 76, 84, 105, 114, 122, 97, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 105, 101, 98, 101, 110, 102, 101, 108, 115, 0, 0, 0, 0, 0, 0, 84, 105, 42, 0, 21, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 51, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 35, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 51, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 16, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 41, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 16, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 41, 64, -26, 29, 39, 59, 0, 0, 97, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -81, 65, -98, -17, -89, 60, 0, -128, 99, 68, 71, 1, 1, 80, 97, 116, 114, 105, 122, 105, 101, 114, 105, 110, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 69, 117, 81, 108, 75, 97, 103, 97, 105, 110, 101, 32, 71, 46, 32, 74, 46, 0, 0, 0, 76, 97, 113, 117, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 0, 42, 84, 63, 0, 0, 0, -102, -103, 41, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, 25, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 102, 102, 38, 64, 10, -41, 35, 59, 0, -64, 97, 68, -102, -103, 25, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 57, 64, 124, -14, 48, 59, 0, 0, 102, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 25, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 54, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 26, 1, 1, 66, 117, 101, 114, 103, 101, 114, 105, 110, 95, 70, 82, 65, 85, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 83, 76, 118, 77, 68, 111, 114, 97, 104, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 97, 118, 101, 108, 105, 117, 115, 0, 0, 0, 0, 0, 0, 0, 0, 84, 105, 0, 21, 42, 0, 0, 0, -102, -103, 9, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 38, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 19, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 25, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 28, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 22, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 19, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 19, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 64, 0, 1, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 66, 116, 83, 101, 70, 101, 114, 114, 97, 110, 100, 111, 0, 0, 0, 0, 0, 0, 0, 0, 69, 115, 116, 101, 98, 97, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 21, 105, 0, 84, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, 12, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 0, 0, 48, 64, 10, -41, 35, 59, 0, -64, 97, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 57, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 16, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 60, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 32, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 54, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -90, 65, -98, -17, -89, 60, 0, -128, 99, 68, 1, 0, 1, 100, 105, 101, 98, 52, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 108, 78, 82, 98, 70, 114, 105, 101, 100, 114, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 66, 101, 114, 110, 111, 117, 108, 108, 105, 0, 0, 0, 0, 0, 0, 0, 105, 21, 21, 0, 126, 0, 0, 0, 102, 102, 54, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 6, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 6, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 19, 64, 124, -14, 48, 59, 0, 0, 102, 68, 51, 51, 51, 64, -96, -85, 45, 59, 0, -128, 94, 68, 0, 0, 64, 64, 52, -128, 55, 59, 0, -64, 100, 68, -102, -103, 57, 64, 88, 57, 52, 59, 0, 64, 98, 68, 0, 0, 32, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 19, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, -83, 65, -98, -17, -89, 60, 0, -128, 99, 68, 58, 0, 1, 80, 114, 105, 101, 115, 116, 101, 114, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 82, 98, 114, 66, 66, 101, 114, 110, 104, 97, 114, 116, 0, 0, 0, 0, 0, 0, 0, 0, 66, 105, 101, 114, 98, 114, 97, 117, 101, 114, 0, 0, 0, 0, 0, 0, 63, 42, 42, 21, 84, 0, 0, 0, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -102, -103, 25, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 44, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 6, 64, 117, 2, 26, 59, 0, 0, 92, 68, 51, 51, 51, 64, 88, 57, 52, 59, 0, 0, 100, 68, -51, -52, 12, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -86, 65, -98, -17, -89, 60, 0, -128, 99, 68, 8, 0, 0, 82, 97, 116, 115, 104, 101, 114, 114, 50, 95, 75, 85, 84, 84, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 105, 76, 77, 71, 71, 105, 109, 108, 105, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 90, 119, 101, 114, 103, 101, 110, 104, -10, 104, 0, 0, 0, 0, 0, 0, 42, 21, -87, 84, 21, 0, 0, 0, -51, -52, 28, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 19, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 19, 64, -96, -85, 45, 59, 0, -128, 99, 68, -51, -52, 44, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 102, 102, 6, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 12, 64, 117, 2, 26, 59, 0, 0, 92, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 100, 68, 51, 51, 51, 64, -26, 29, 39, 59, 0, 0, 97, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -92, 65, -98, -17, -89, 60, 0, -128, 99, 68, 49, 0, 0, 72, 97, 110, 100, 119, 101, 114, 107, 101, 114, 50, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 78, 77, 114, 103, 71, 101, 114, 109, 97, 110, 105, 99, 117, 115, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 77, 97, 105, 108, 97, 110, 100, 0, 0, 0, 0, 0, 21, 84, 21, 84, 42, 0, 0, 0, 51, 51, 35, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 16, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 28, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 16, 64, -96, -85, 45, 59, 0, -128, 99, 68, 51, 51, 19, 64, 124, -14, 48, 59, 0, 0, 102, 68, -102, -103, 25, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 51, 64, 52, -128, 55, 59, 0, -64, 100, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 64, 98, 68, -51, -52, 44, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 38, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -80, 65, -98, -17, -89, 60, 0, -128, 99, 68, 19, 0, 0, 98, 117, 101, 114, 103, 101, 114, 95, 77, 65, 78, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 116, 72, 110, 115, 65, 108, 99, 104, 97, 116, 114, 111, 110, 0, 0, 0, 0, 0, 0, 0, 69, 105, 115, 101, 110, 104, 97, 114, 116, 0, 0, 0, 0, 0, 0, 0, 42, 84, 0, 126, 0, 0, 0, 0, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, 32, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -102, -103, 25, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 32, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 12, 64, -96, -85, 45, 59, 0, -128, 94, 68, 51, 51, 19, 64, 52, -128, 55, 59, 0, -64, 100, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 64, 98, 68, 51, 51, 51, 64, 117, 2, 26, 59, 0, 0, 92, 68, -102, -103, 25, 64, 88, 57, 52, 59, 0, 0, 100, 68, 0, 0, 64, 64, -26, 29, 39, 59, 0, 0, 97, 68, -51, -52, 12, 64, 88, 57, 52, 59, 0, 0, 102, 68, 0, 0, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 53, 0, 0, 114, 97, 116, 115, 104, 101, 114, 114, 95, 83, 79, 76, 68, 65, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 82, 115, 109, 82, 71, 101, 114, 104, 97, 114, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 105, 101, 109, 101, 105, 115, 116, 101, 114, 0, 0, 0, 0, 0, 0, 105, 84, 0, 21, 42, 0, 0, 0, -72, 30, 21, 64, 88, 57, 52, 59, 0, 0, 102, 68, 51, 51, 35, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, -51, -52, 44, 64, 10, -41, 35, 59, 0, -64, 97, 68, 51, 51, 35, 64, -96, -85, 45, 59, 0, -128, 99, 68, -82, 71, 49, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 28, 64, 52, -128, 55, 59, 0, -64, 100, 68, -10, 40, 28, 64, 88, 57, 52, 59, 0, 64, 98, 68, -102, -103, 41, 64, 117, 2, 26, 59, 0, 0, 92, 68, -72, 30, 21, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 22, 64, -26, 29, 39, 59, 0, 0, 97, 68, 102, 102, 22, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, -82, 65, -98, -17, -89, 60, 0, -128, 99, 68, 25, 0, 1, 98, 117, 101, 114, 103, 101, 114, 51, 95, 68, 73, 67, 75, 69, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 77, 104, 68, 114, 74, 111, 104, 97, 110, 110, 101, 115, 32, 73, 46, 0, 0, 0, 0, 0, 118, 111, 110, 32, 82, -4, 100, 101, 115, 104, 101, 105, 109, 0, 0, 0, 84, 84, 21, 21, 42, 0, 0, 0, -82, 71, 49, 64, 88, 57, 52, 59, 0, 0, 102, 68, 102, 102, 38, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 51, 64, 10, -41, 35, 59, 0, -64, 97, 68, 102, 102, 38, 64, -96, -85, 45, 59, 0, -128, 99, 68, 0, 0, 0, 64, 124, -14, 48, 59, 0, 0, 102, 68, -51, -52, 28, 64, -96, -85, 45, 59, 0, -128, 94, 68, -102, -103, 41, 64, 52, -128, 55, 59, 0, -64, 100, 68, -72, 30, 21, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 22, 64, 117, 2, 26, 59, 0, 0, 92, 68, 102, 102, 38, 64, 88, 57, 52, 59, 0, 0, 100, 68, 102, 102, 22, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 32, 64, 88, 57, 52, 59, 0, 0, 102, 68, -51, -52, -84, 65, -98, -17, -89, 60, 0, -128, 99, 68, 35, 0, 1, 98, 97, 117, 101, 114, 95, 77, 65, 78, 78, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 }, { 110, 65, 84, 109, 87, 111, 116, 97, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 111, 110, 32, 77, 101, 117, 99, 104, 101, 108, 116, 97, 110, 110, 0, 63, 21, 63, 21, 84, 0, 0, 0, -51, -52, 28, 64, 46, -112, 32, 59, 0, 0, 102, 68, 51, 51, 35, 64, 82, 73, 29, 59, 0, -128, 94, 68, 0, 0, 0, 64, 39, -96, 9, 59, 0, 0, 92, 68, 51, 51, 35, 64, 10, -41, 35, 59, 0, -64, 97, 68, 0, 0, 48, 64, -96, -85, 45, 59, 0, -128, 99, 68, 102, 102, 38, 64, 124, -14, 48, 59, 0, 0, 102, 68, 0, 0, 32, 64, -96, -85, 45, 59, 0, -128, 94, 68, -51, -52, 44, 64, 52, -128, 55, 59, 0, -64, 100, 68, -61, -11, 40, 64, 88, 57, 52, 59, 0, 64, 98, 68, 102, 102, 6, 64, 117, 2, 26, 59, 0, 0, 92, 68, -51, -52, 44, 64, 88, 57, 52, 59, 0, 0, 100, 68, -102, -103, 9, 64, -26, 29, 39, 59, 0, 0, 97, 68, 0, 0, 48, 64, 88, 57, 52, 59, 0, 0, 102, 68, -102, -103, -87, 65, -98, -17, -89, 60, 0, -128, 99, 68, 51, 0, 0, 98, 117, 101, 114, 103, 101, 114, 51, 95, 68, 73, 67, 75, 69, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0 } };  /* 0x60f310 */
AIDEC gv_Decisions[0x1d] = {  /* 0x611258 */
    { 0, 1, 0, 0, 4, 0, 0, 1, 2, 0x18, 1, -1, 0, (void *)ai_DecideLawSeverity },
    { 1, 2, 0, 0, 1, 0, 0, 0, 0, 0x18, 2, -1, 0, (void *)ai_DecideLaw1 },
    { 2, 3, 0, 0, 2, 2, 6, 7, 0, 0x1a, 1, 0, 0, (void *)ai_DecideStateReligion },
    { 3, 2, 0, 0, 1, 2, 6, 1, 0, 0x12, 1, 1, 0, (void *)ai_DecideLaw3 },
    { 4, 2, 0, 0, 1, 4, 10, 1, 0, 0x16, 1, 2, 0, (void *)ai_DecideLaw4 },
    { 5, 1, 0, 0, 4, 0, 0, 0, 2, 0x0e, 1, -1, 0, (void *)ai_DecideUpkeepRate },
    { 6, 1, 0, 0, 4, 0, 0, 0, 2, 0x0a, 1, -1, 0, (void *)ai_DecideBuildingCost },
    { 7, 1, 0, 0, 4, 0, 0, 0, 2, 0x0b, 1, -1, 0, (void *)ai_DecideLaw7 },
    { 8, 2, 0, 0, 1, 4, 0, 1, 0, 0x1b, 1, 3, 0, (void *)ai_DecideLaw8 },
    { 9, 0, 0, 8, 18, 0, 0, 0, 10, 0x0f, 1, -1, 0, (void *)ai_DecideIncomeTaxRate },
    { 10, 0, 0, 6, 18, 0, 0, 0, 10, 0x0f, 2, -1, 0, (void *)ai_DecideGuildTaxRate },
    { 11, 0, 0, 6, 10, 0, 0, 0, 8, 0x15, 1, -1, 0, (void *)ai_DecideChurchTaxRate },
    { 12, 0, 0, 5, 11, 0, 0, 0, 8, 0x0c, 1, -1, 0, (void *)ai_DecideTradeTaxRate },
    { 13, 0, 0, 10, 25, 0, 0, 0, 16, 0x0e, 2, -1, 0, (void *)ai_DecideInheritanceTaxRate },
    { 14, 0, 0, 10, 25, 3, 10, 5, 16, 0x10, 1, 4, 0, (void *)ai_DecideBankRate },
    { 15, 1, 0, 0, 4, 0, 0, 0, 2, 0x19, 1, -1, 0, (void *)ai_DecideOfficePayRate },
    { 16, 2, 0, 0, 1, 3, 12, 1, 0, 0x0d, 1, 5, 0, (void *)ai_DecideGuiltWeights },
    { 17, 2, 0, 0, 1, 3, 12, 1, 0, 0x19, 2, 6, 0, (void *)ai_DecideGoodsGuiltWeights },
    { 18, 2, 0, 0, 1, 3, 12, 1, 0, 0x12, 1, 7, 0, (void *)ai_DecideTrankGuiltWeights },
    { 19, 2, 0, 0, 1, 3, 12, 1, 0, 0x13, 1, 8, 0, (void *)ai_DecideThreatenGuiltWeights },
    { 20, 2, 0, 0, 1, 2, 12, 1, 0, 0x13, 2, 9, 0, (void *)ai_DecideLaw20 },
    { 21, 2, 0, 0, 1, 5, 12, 1, 1, 0x11, 1, 10, 0, (void *)ai_DecideBeatUpGuiltWeights },
    { 22, 2, 0, 0, 1, 4, 12, 1, 1, 0x14, 1, 11, 0, (void *)ai_DecideBlackmailGuiltWeights },
    { 23, 2, 0, 0, 1, 5, 12, 1, 1, 0x17, 1, 12, 0, (void *)ai_DecideLaw23 },
    { 24, 2, 0, 0, 1, 5, 12, 1, 1, 0x16, 2, 13, 0, (void *)ai_DecideSabotageGuiltWeights },
    { 25, 2, 0, 0, 1, 5, 12, 1, 1, 0x17, 2, 14, 0, (void *)ai_DecideLaw25 },
    { 26, 2, 0, 0, 1, 5, 12, 1, 1, 0x14, 2, 15, 0, (void *)ai_DecideLaw26 },
    { 27, 2, 0, 0, 1, 5, 12, 1, 0, 0x1a, 2, 16, 0, (void *)ai_DecideLaw27 },
    { 28, 2, 0, 0, 1, 5, 12, 1, 0, 0x1b, 2, 17, 0, (void *)ai_DecideLaw28 },
};
int (*ord_OrderHandlers[0x3a])(char *c, int *m) = { ord_ApplyForOffice, ord_LevelOrder, ord_MedicusOrder, ord_AccusationOrder, ord_BlackmailBuildingOrder, ord_Level3Order, ord_ChangeProfessionOrder, ord_IntimidateOrder, ord_BlackmailContactOrder, ord_Level4Order, ord_Level4Order2, ord_Level4To5Order, ord_Level5UpOrder, ord_Level5UpOrder2, ord_DivorceOrder, ord_Level6UpOrder, ord_Level6UpOrder2, ord_WageReportOrder, ord_DismissOfficeOrder, ord_LawOrder, ord_LawOrder2, ord_Job11Order, ord_JobMultiOrder, ord_Job28Order, ord_Job29Order, ord_Job34Order, ord_JobRangeOrder, ord_JobMultiOrder2, ord_BewitchOrder, ord_CounterEspionageOrder, ord_Job17Order, ord_Job17Order2, ord_JobMultiOrder4, ord_MisappropriationOrder, ord_Job10Order, ord_Job15Order, ord_SeatSwapOrder, ord_HatredOrder, ord_JobMultiOrder3, ord_HearingOrder, ord_ExpelWorkerOrder, ord_ConvertOrder, ord_PeaceOrder, ord_PenanceOrder, ord_SpyTargetFlagOrder, ord_MiracleOrder, ord_Level7Order, ord_Level8Order, ord_Level9UpOrder, ord_InterrogationOrder, ord_DismissalOrder, ord_Flag800Order, ord_Flag800Order2, ord_InterrogationOrder2, ord_Flag200Order, ord_InterrogationOrder3, ord_Flag400Order, ord_DismissWorkerOrder };  /* 0x611768, the order handler of each order number */
char ord_KindTable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 50, 0, 0, 0, 0, 0, 12, 0, 75, 0, 0, 0, 0, 0, 20, 0, 125, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 3, 0, 0, 0, 15, 0, 0, 0, 9, 0, 0, 0, 25, 0, 0, 0, 16, 0, 0, 0, 20, 0, 6, 0, 20, 0, 0, 0, 30, 0, 9, 0, 60, 0, 0, 0, 45, 0, 12, 0, 110, 0, 0, 0, 30, 0, 0, 0, 10, 0, 0, 0, 45, 0, 0, 0, 30, 0, 0, 0, 70, 0, 0, 0, 60, 0, 0, 0, 10, 0, 0, 0, 10, 0, 0, 0, 20, 0, 0, 0, 30, 0, 0, 0, 25, 0, 0, 0, 60, 0, 0, 0, 30, 0, 3, 0, 15, 0, 0, 0, 45, 0, 6, 0, 50, 0, 0, 0, 80, 0, 9, 0, 120, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 0, 0, 15, 0, 0, 0, 50, 0, 0, 0, 35, 0, 0, 0, 75, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 10, 0, 30, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 100, 0, 0, 0, 20, 0, 0, 0, 30, 0, 12, 0, 30, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 8, 0, 20, 0, 0, 0, 50, 0, 10, 0, 40, 0, 0, 0, 75, 0, 12, 0, 80, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 3, 0, 20, 0, 0, 0, 50, 0, 5, 0, 50, 0, 0, 0, 75, 0, 6, 0, 100, 0, 0, 0, 30, 0, 4, 0, 25, 0, 0, 0, 50, 0, 5, 0, 40, 0, 0, 0, 80, 0, 6, 0, 70, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 5, 0, 0, 0, 40, 0, 0, 0, 10, 0, 0, 0, 80, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };  /* 0x617468 */
AITIME gv_LastTime = {0};  /* 0x6f0e38, the game time ai_Time of the last effect step (gv_UpdateEffects) */
GVEFFARG gv_EffectArgs[8] = {0};  /* 0x6f0e48, the argument of each effect slot's default callback */
char gv_Effects[2944] = {0};  /* 0x6f1008, 8 slots of */
float ai_RatingBase = 0;  /* 0x6f1b9c, the running rating (it moves towards ai.h's ai_Rating) */
struct AICHAR *ai_JudgeCatholic = 0;  /* 0x6f1bfc / */
struct AICHAR *ai_JudgeProtestant = 0;  /* 0x6f1c00 */
/* ai_PlayerCity: view of ai_PlayerSlots (tools/data_views.py) */
float ord_RatingRate = {0}; /* 0x91aaf4, the change rate of the rating (ord.c */
ORDSTATROUND ord_StatisticsHistory[16] = {0}; /* 0x91ab00, the statistics of the last 16 rounds */
char ord_RatingBytes[16] = {0};  /* 0x91b460, 5 + 1 bytes */
ORDMSGREC ord_MessageRecord = {0}; /* 0x91b480, the record the messages carry */
char gm_IncomeHistory[0x154] = {0};  /* 0x91b580, the 17-round history of the five income sums (rows of */
GVCANNON gv_Cannon = {0};  /* 0x925100, the cannon scene state */
