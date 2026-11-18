/* gs.c -- the laws and crimes (tag "gs_AddBeweis()" 0x611698; "gs" a guess for Gesetze), 0x4f2720..0x4f4e20: the AI
   decision (law) table gv_Decisions 0x611258 (0x1d AIDEC records, the fn slots are am.c's ai_Decide* callbacks; read and
   set by gv_GetTableRecord / gv_SetTableRecord, message 0x46), the CRIME records ai_Crimes (cm.c) and the evidence
   pairs ai_Evidence (saved by gv_WriteCrimes / gv_ReadCrimes), the crime texts, the trial's sentences and the rank
   check (gv_RankWorth). A unit of its own by its data: its .data block gv_Decisions 0x611258 follows grave.c's
   strings (allocation tags up to 0x611251), its strings ("Hm, das ist doch nicht verboten" 0x61166c ..
   "_NACHRICHTEN_HS_25" 0x6116d8) are used from 0x4f319d on, and its named .rdata gv_RankWorth 0x5ee0f8 follows
   grave's literals (0x5ee0c4..0x5ee0f8, first users up to 0x4f24c9). The start: gv_GetTableRecord 0x4f2720, the first
   user of gv_Decisions; the functions before it (gv_FindHeldEntry, gv_FindReservedEntry, gv_SeanceChance) are
   graveyard code with no data of either object. Names are ours (the gv_ prefix is kept from the joined unit).
   unit 0x004f2720..0x004f4e20, split from grave; boundaries in decl/text.dcl */
#include "txt_views.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "grave.h"
#include "gs.h"
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

/* dcp unit split: the declarations of gv before the moved functions */
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
int ai_NextCrimeId = 0;  /* 0x6f1b88 .. */
int gs_UnreadBss = 0;  /* 0x6f1b8c, no surviving instruction or pointer refers to it: data of functions /OPT:REF removed; size from the neighbours' addresses */
const int gv_RankWorth[10] = { 0, 160000, 320000, 640000, 1600000, 3200000, 6400000, 8000000, 16000000, 32000000 };  /* 0x5ee0f8, the worth each rank needs (hmt.c's council founders get up to [7]); 10 entries: the original's next object at 0x5ee120 is ord.c's literal -20.0. gv_CheckRankUp clamps the index to 10 and so reads [10], past the end (the low dword of that double, 0): the original's own overrun */

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
    return cm_SendDecisionValue(&m);
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
/* 0x4f2da0, the highest level (0x5092e0 (b, 10, 1)) among the school buildings (kind 7) of the character *c's, times 0.01; 0.75 when one has exactly 0x4b */

float gv_SchoolLevelFactor(unsigned short *c)
{
    int best, i, v;
    AIBLD *b;

    best = 0;
    for (i = 0, b = ai_BldsT; i < 0x100; i++, b++) {
        if (b->type != 0 && b->owner == *c && ((AIBTYPE *)(ai_BTypes + b->type * 0x24d))->kind == 7) {
            v = gm_BuildingSkill((unsigned char *)b, 10, 1);
            if (v == 0x4b)
                return 0.75f;
            if (v > best)
                best = v;
        }
    }
    return (float)best * 0.01f;
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
    escape = gv_SchoolLevelFactor((unsigned short *)culprit);
    if (util_RandomF() < escape)
        goto unreported;
    if (ai_InventoryHas(culprit, 0x22a) != 0 && util_RandomF() > 0.5)
        goto unreported;
    m.time = ai_Time;
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
   (eax: cm_RunTrial keeps it) */

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

    i = 0;
    do {
        if (gv_CrimesT[i].id == id)
            return i;
        i++;
    } while (i < 0x200);
    return -1;
}
/* 0x4f3700, sends every crime record with message 0x23 (cm_SendRetireCrime (id, flag)): an open one (state 1) with flag 0, a
   handed-over one (state above 1: a job id) whose job he_FindEvent (1, 1, state) no longer finds with flag 1; after 32
   sends it pumps the net until the last send's slot is done (cm_GetSlotState). */

void gv_SendCrimes(void)
{
    int slot, sent, i;

    slot = -1;
    sent = 0;
    for (i = 0; i < 0x200; i++) {
        if (gv_CrimesT[i].state == 1) {
            slot = cm_SendRetireCrime(gv_CrimesT[i].id, 0);
            sent++;
        } else if (gv_CrimesT[i].state > 1 && he_FindEvent(1, 1, gv_CrimesT[i].state) == 0) {
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
/* 0x4f3780, the slot for the pair (a, b) in the table ai_Evidence (2048 pairs): -2 when it exists already, else the first free one or -1 */

int gv_EvidenceSlot(int a, int b)
{
    int ok, r, i;
    int *t;

    ok = 0;
    r = -1;
    i = 0;
    t = (int *)ai_Evidence;
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
                ai_ShowEvidenceVoidNews((unsigned short *)&gv_CharsT[ai_PlayerId], (unsigned short *)culprit);
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
    int i;

    cnt = 0;
    for (i = 0; i < 0x200; i++) {
        if (gv_CrimesT[i].culprit == c && gv_CrimesT[i].state == 1)
            cnt++;
    }
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
            ai_ShowEvidenceVoidNews((unsigned short *)who[j], (unsigned short *)c);
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
            ai_ShowEvidenceVoidNews((unsigned short *)who[i], (unsigned short *)c);
    }
    return cnt;
}
/* 0x4f3c50, the culprits of the open crimes (state 1) the character t holds evidence of (ai_Evidence): counts the crimes
   per culprit and keeps the 32 most frequent in ids[] / cnt[], sorted by count (descending, one insertion pass per
   culprit); ids is first -1, cnt 0; returns the number of different culprits (at most 0x20) */

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
            c = 1;
            n++;
            culprits[i] = -1;
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
/* 0x4f4080, as gv_HeldCrimesOf, but the crimes' ids (CRIME.id) are stored; t is the ai_Chars index of the character
   holding the evidence (he.c's fight outcome 0x546c40 pushes it as a word without zero-extending: a 16-bit parameter) */

int gv_HeldCrimeIdsOf(int culprit, int *out, unsigned short t)
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
                out[n++] = gv_CrimesT[k].id;
                if (n > 0x20)
                    break;
            }
        }
    }
    return n;
}
/* 0x4f4130, the count of gv_HeldCrimesOf without the list: how many crimes of the culprit the character t (low word)
   holds evidence of, stopping past 0x20; 0 when the culprit does not exist (t is a character index, a 16-bit
   word: lc.c's callers push it unextended) */

int gv_HeldCrimeCount(int culprit, unsigned short t)
{
    int n, i, k;
    ORDCHAR *ch;

    n = 0;
    if (gm_FindChar(culprit) == 0)
        return 0;
    ch = &ORD_CHARS[t];
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
   to max), *cnt their number; 0 when there is none (the seance's ghost: ai_SeanceGhost) */

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
/* 0x4f4390, sorts the n crime records at r by the law broken (CRIME.law), ascending: an exchange sort with whole-record
   swaps (the crime list windows ai_EvidenceDetailsWindow / ai_AccuseWindow sort their copies of the records held) */

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
/* 0x4f4420, whether the sentence k (0 .. 9, the trial's punishment choices of cm.c's "cutscenes\prozess") can be passed on
   the character c: 0 yes, -1 without c, -3 for k >= 10, -4 when c lacks what it takes (5: an office, AICHAR.officeKind;
   7: AICHAR.b00d). The name is a reasoned guess from the trial window. */

int gv_CheckSentence(AICHAR *c, unsigned k)
{
    int r;

    r = 0;
    if (c == 0)
        return -1;
    if (k >= 10)
        return -3;
    if (k == 5) {
        if (c->officeKind == 0)
            r = -4;
    } else if (k == 7) {
        if (c->b00d == 0)
            r = -4;
    }
    return r;
}
/* 0x4f4460, carries out the sentence a (0 .. 9, the trial's punishments, see gv_CheckSentence) on the character c:
   0 / 2 the long message 0x5c (arg 3, kind 1 / 3) and 1 / 3 the message 0x5b (3 per cent of c's worth, kind 1 / 4), due in
   an hour; 4 / 6 / 8 cm_SendMessage5b (c, 1 / 2 / 3, 0); 5 takes his office (amt_SendOfficeMessage, mode 4); 7 raises
   lowers the rank AICHAR.b00d (to 2 when above 2, else 1) and sends it; 9 cm_SendMessage5b (c, 0xff, 1) and a scene party (G28REC, message
   0x27) of c, both judges and up to 13 player characters (kinds 5 / 6 / 7) held by the player's character a day later
   at 10, then frees an office of c (amt_FindOffice, mode 3). */

void gv_PassSentence(AICHAR *c, int a)
{
    unsigned char b;
    int worth;
    AIMSG m;
    AMTREC office;
    G28REC party;
    AMTKIND kind;
    int i, k;

    worth = gm_GetWorth(c->id);
    switch (a) {
    case 0:
        m.type = 0x5c;
        m.flags = 1;
        m.time = ai_Time;
        gm_AddTime((struct AITIME *)&m.time, 1, 0, 0);
        m.to = c->netId;
        m.fromId = -1;
        m.arg[0] = 3;
        m.arg[1] = 1;
        cm_SendLongMessage((char *)&m);
        break;
    case 1:
        m.type = 0x5b;
        m.flags = 1;
        m.time = ai_Time;
        gm_AddTime((struct AITIME *)&m.time, 1, 0, 0);
        m.to = c->netId;
        m.fromId = -1;
        m.arg[0] = (int)(worth * 0.03f);
        m.arg[1] = 1;
        cm_SendLongMessage((char *)&m);
        break;
    case 2:
        m.type = 0x5c;
        m.flags = 1;
        m.time = ai_Time;
        gm_AddTime((struct AITIME *)&m.time, 1, 0, 0);
        m.to = c->netId;
        m.fromId = -1;
        m.arg[0] = 3;
        m.arg[1] = 3;
        cm_SendLongMessage((char *)&m);
        break;
    case 3:
        m.type = 0x5b;
        m.flags = 1;
        m.time = ai_Time;
        gm_AddTime((struct AITIME *)&m.time, 1, 0, 0);
        m.to = c->netId;
        m.fromId = -1;
        m.arg[0] = (int)(worth * 0.03f);
        m.arg[1] = 4;
        cm_SendLongMessage((char *)&m);
        break;
    case 4:
        cm_SendMessage5b(c->netId, 1, 0);
        break;
    case 5:
        amt_SendOfficeMessage(c->officeKind, 0, 0, 4, 0xff);
        break;
    case 6:
        cm_SendMessage5b(c->netId, 2, 0);
        break;
    case 7:
        b = (c->b00d > 2) + 1;
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldDelta(1, 1, 0xd, &b);
        cm_SendFieldDeltas();
        break;
    case 8:
        cm_SendMessage5b(c->netId, 3, 0);
        break;
    case 9:
        cm_SendMessage5b(c->netId, 0xff, 1);
        party.members[0] = c->netId;
        party.members[1] = ai_JudgeCatholic->netId;
        party.members[2] = ai_JudgeProtestant->netId;
        i = 3;
        for (k = 0; i < 0x10 && k < 768; k++) {
            if (c->netId != gv_CharsT[k].netId
                && (gv_CharsT[k].kind == 6 || gv_CharsT[k].kind == 7 || gv_CharsT[k].kind == 5))
                party.members[i++] = gv_CharsT[k].netId;
        }
        party.used = (unsigned char)i;
        party.host = gv_CharsT[ai_PlayerId].netId;
        party.thing = -1;
        party.time = ai_Time;
        gm_AddTime((struct AITIME *)&party.time, 0x18, 0, 0);
        party.time.hour = 10;
        party.time.min = 0;
        party.kind = 5;
        party.variant = (unsigned char)util_Random(5);
        party.quiet = 0;
        party.flags = 0x22;
        if (c->kind == 6 || c->kind == 7)
            party.flags = 0xa2;
        cm_SendNewParty(&party);
        if (amt_FindOffice((char *)c, &kind, &office) != 0)
            amt_SendOfficeMessage(office.idx, 0, 0, 3, 0xff);
        break;
    }
}
/* 0x4f48f0, the text of a character action a: 1 text 0x1538 (with 0.03 * the 0x50a640 value), 3 text 0x153e (4, the value), otherwise text 0x1535 + 3 * a */

void gv_SentenceText(unsigned short *c, int a, char *out)
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
/* 0x4f4970, the rank check of the character slot i (in use, kind 1 .. 9, alive): when his worth (gm_GetWorth) exceeds
   the threshold gv_RankWorth [rank + 1] (at most [10]) his rank AICHAR.b00d rises. From rank 1 only with the item 0x110
   in the building (5, 0xf): field 0x1c8 gets 0x4000 (cm_SendSetField) and a player's character the news 0x161d
   ("_NACHRICHTEN_HS_25"), the rank itself is not raised here. Below rank 9 the new rank goes to a player's character as
   the message 0x69 (flags 9, due at 7 o'clock), to any other one as the field 0xd; the rise of a kind 5 / 6 / 7 character
   is told to the other player characters (the first ai_NumPlayers of them; a kind 5 one's news a few hours later). */

void gv_CheckRankUp(unsigned i)
{
    AICHAR *c;
    AIBLD *bld;
    AIOBJ *item;
    int worth, rank, next, n, k;
    unsigned char nb;
    char text[0x200];
    AIMSG m;

    c = &gv_CharsT[(unsigned short)i];
    if (c->id == 0xffff || c->kind >= 10 || c->health == 0 || c->kind == 0)
        return;
    worth = gm_GetWorth(i);
    rank = c->b00d;
    next = rank + 1;
    if (next > 10)
        next = 10;
    if (worth > gv_RankWorth[next] && rank == 1) {
        bld = (AIBLD *)gm_BuildQuery(rank, 5, 0xf);
        if (bld == 0)
            return;
        item = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 0, 0x110);
        if (item == 0)
            return;
        cm_SendSetField(c->netId, 0x1c8, 4, 0x4000, 0);
        if (c->kind == 6 || c->kind == 7) {
            txt_Format(text, 0x161d, c->id);
            he_MessageBoxQuickjump(c->netId, -1, text, 0, 0x5fd, bld->id, item->objId, 0, "_NACHRICHTEN_HS_25");
        }
        return;
    } else if (worth > gv_RankWorth[next] && rank < 9) {
        nb = rank + 1;
        if (c->kind == 6 || c->kind == 7) {
            AI_ZERO(&m, sizeof(m));
            m.to = c->netId;
            m.time = ai_Time;
            m.type = 0x69;
            m.fromId = -1;
            m.flags = 9;
            m.time.hour = 7;
            m.time.min = 0;
            m.rankUp.rank = nb;
            m.rankUp.ch = c->netId;
            cm_SendLongMessage((char *)&m);
        } else {
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldDelta(1, 1, 0xd, &nb);
            cm_SendFieldDeltas();
        }
        if (c->kind != 5 && c->kind != 6 && c->kind != 7)
            return;
        n = 0;
        for (k = 0; k < 768; k++) {
            if (n >= ai_NumPlayers)
                return;
            if ((gv_CharsT[k].kind == 6 || gv_CharsT[k].kind == 7) && &gv_CharsT[k] != c) {
                AI_ZERO(&m, sizeof(m));
                m.to = gv_CharsT[k].netId;
                m.time = ai_Time;
                m.type = 0x69;
                m.fromId = -1;
                m.flags = 9;
                m.time.hour = 7;
                m.time.min = 0;
                if (c->kind == 5)
                    gm_AddTime((struct AITIME *)&m.time, util_RandomU(4), util_RandomU(8) * 7, 0);
                m.rankUp.rank = nb;
                m.rankUp.ch = c->netId;
                cm_SendLongMessage((char *)&m);
                n++;
            }
        }
    }
}
/* 0x4f4d10, the rank check (gv_CheckRankUp) of every one of the 768 character slots */

void gv_CheckRankUps(void)
{
    int i;

    for (i = 0; i < 0x300; i++)
        gv_CheckRankUp(i);
}
/* 0x4f4d30, the price of the evidence of the crime id offered by the character seller (the 'dunkle ecke' shops,
   ai_DecideBuyDunkleEcke): (liking of the culprit for the seller * 0.005 + 0.5) * (the culprit's level gm_CharLevel + the
   law's AIDEC.guilt) * 1600 into *price, the culprit's id into *culprit, the law into *law; 1 when the crime exists, its
   law carries guilt and the culprit exists, else 0 */

int gv_EvidencePrice(int id, AICHAR *seller, int *price, short *culprit, unsigned *law)
{
    int i;
    AICHAR *ch;
    AIDEC rec;
    int level;

    if (id == -1)
        return 0;
    i = gv_FindCrime(id);
    if (i == -1)
        return 0;
    if (gv_GetTableRecord(gv_CrimesT[i].law, (int *)&rec) == 0 || rec.guilt == 0)
        return 0;
    ch = (AICHAR *)gm_FindChar(gv_CrimesT[i].culprit);
    if (ch == 0)
        return 0;
    *culprit = ch->id;
    level = gm_CharLevel(ch->id, 1);
    *price = (int)((gm_Liking(ch->id, seller->id, 1) * 0.005f + 0.5f) * (level + rec.guilt) * 1600.0f);
    *law = rec.id;
    return 1;
}
