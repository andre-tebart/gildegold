/* ls.c -- the save game (0x519010 ..): the tag "ls_" ("ls:tmp" 0x623654, "ls_LoadCharacter(): invalid next or prev-action pointer ...", "ls_SendGame"), the language number of a name (0x519010: the table "GERMAN" .. 0x5ee978), the save game writer 0x5190b0 (header 0x5194c0: "SCENARIO", the player, the party records and the screen shot of 0xe100 bytes, then one writer per subsystem), the pointer <-> index conversions before / after a save (0x519a00 / 0x519aa0 / 0x519b50) and the writers of the building list (0x519bf0) ... The unit starts at its first global 0x62364c (the language number, -1). Drafts of layout 2026-10-23; the unit end is open (the strings run to "ls_SendGame" 0x62376c; 0x51c950 ..?).
   unit of rea 8cef7d5b (src/game/ls.c); boundaries in decl/text.dcl, notes in notes/units/ls.md */
struct D3SM;
struct D3TERRAIN;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "f3gm.h"
#include "gv.h"
#include "ai.h"
#include "vfs.h"
#include "he.h"
#include "txt.h"
#include "data_views.h"
#include "ls.h"
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#include "d3_types.h"
extern D3SCENE d3_Scenes[];
extern int cm_SaveAcks[8][2];
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int ai_Unk7d6a40;
extern char ai_Unk7d6a60[];  /* 0x7d6a60, the five 0x2a90-byte stock blocks (f3gm.c's definition) */
#define ai_Unk7d6a64 (*(int (*)[3])((char *)ai_Unk7d6a60 + 4))  /* 0x7d6a64, block 0's header tail (a view) */
extern unsigned ai_Unk91ac40[136];
extern unsigned char ord_Statistics;
extern unsigned char ord_StatisticsClock;
extern char ai_DataPath[608];
extern char ai_Unk925520[];
extern int ai_Unk92bca0;
extern GAPDATA full_data_92bcb3[];
extern int ai_Unk60bd88;
extern int ai_Unk60bd8c;
extern int ai_Unk60bd90;
#include "cm_types.h"
extern G17MSG ai_Unk9367e0[];
extern unsigned char ai_Unka68800[1024];
extern unsigned int ai_Unka68c04;
extern G17MSG ai_Unka69140[];
extern G17STAT ai_Unkf31160[];
extern unsigned int ai_Unkf8121c;
extern GAPDATA full_data_7604e8[];
extern void main_Unk537b60(int kind, int cur, int total);
extern int ai_Unk7d6a40;
#include "ch_types.h"
extern CHMOTION ch_Motions[];
extern int ai_Unk7e3f38;
extern struct G10GAME *ai_Unkf83654;

int ai_Unk61a658 = 1;  /* 0x61a658 */
int ls_Language = 65535;  /* 0x62364c, the language number ( */
int ls_Version = 65626;  /* 0x623650 */
const char ls_LanguageNames[][0x40] = { "GERMAN", "ENGLISH", "FRENCH", "ITALIAN", "SPANISH" };  /* 0x5ee978, the language names (5 of */
extern int gm_PlayerRecords[688];          /* 0x7d5f80, the 16 player records (0xac bytes each; gm_PlayerRecord, f3gm.c) */
/* gm_PlayerRecordBytes: view of gm_PlayerRecords (tools/data_views.py) */
extern char *ls_ActionQueue;            /* 0x631050, the action queue (ch.h: CHACTION *ch_Queue) */
extern char ch_EmptyName[];             /* 0x631090 */
extern int d3_CurrentScene;                /* 0x633444 */
extern struct AICHAR *ai_JudgeCatholic;  /* 0x6f1bfc */
extern struct AICHAR *ai_JudgeProtestant;  /* 0x6f1c00 */
int * ai_Unk633738 = 0;  /* 0x633738, the scene (its first word the kind: */
extern unsigned short ai_PlayerId;              /* 0x6f25b8 */
int ai_Unk6f1b88 = 0;  /* 0x6f1b88 .. */
int ai_Unk6f1bcc = 0;  /* 0x6f1bcc */
unsigned char ls_SaveSlot = 0;  /* 0x6f1c38, the slot the save header records */
extern int ai_Difficulty;                /* 0x6f23fc */
/* ai_Unk76fa24: view of cm_SaveAcks (tools/data_views.py) */
extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
extern struct AIOBJ *ai_Objects;             /* 0x7e3f34 */
extern char ai_Time[];             /* 0x770ae0, the clock */
extern int ai_UpdateStart;             /* 0xf89580 */
extern int ai_NextObjectId;                /* 0x61a654 */
int ai_Unk6f2618 = 0;  /* 0x6f2618 */
extern int ai_Unk6f1bb8, ai_Unk6f1bbc, ai_CharCount;   /* 0x6f1bb8, 0x6f1bbc, 0x6f1bc0 */
/* ls_RefTargets: view of ai_Unkf31160 (tools/data_views.py) */
int vfs_fclose(struct VFSFILE *f);                    /* 0x430aa0 */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                     /* 0x430470 */
void mem_Free(void *p);                    /* 0x422620 */
void dd_SplitRGB();                    /* 0x41e6d0 */
void d3_SaveScene(char *name, struct VFSFILE *f);                     /* 0x44aa10 */
int gm_FindObjectInst(int id);                     /* 0x4fe980 */
char * gm_FindBuildingById(int id);                     /* 0x502020 */
int cm_PartyById(int id);               /* 0x4e50a0, cm.h's prototype (that header redefines gv.h's globals here) */
int gm_GetWorth(unsigned short id);                     /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k);                     /* 0x5085b0 */
int amt_SaveAemter();                     /* 0x4ea690 */
int unk_Unk4f28d0();                     /* 0x4f28d0 */
int ai_Unk4b62e0();                     /* 0x4b62e0 */
int ai_Unk589aa0();                     /* 0x589aa0 */
int ai_Unk5a8940();                     /* 0x5a8940 */
int ahm_SaveWalk();                     /* 0x4ebfb0 */
int hmt_Save();                     /* 0x4adb20 */
int rt_SaveRoutes();                     /* 0x4bff40 */

#define LS_W(p, sz) if (vfs_fwrite((p), (sz), 1, f) <= 0) return 0;
#include "rd_types.h"                 /* G15BK */
int unk_Unk4f17f0();                     /* 0x4f17f0 */
int ahm_WriteRecord();                     /* 0x4ecf10 */

#define LS_R(p, sz) if (ls_Read((int)(p), (sz), 1, f) == 0) return 0;
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                     /* 0x430470 */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);          /* 0x41e670, an RGB triple to a 16 bit pixel */

extern int f3gm_ObjectCount;                /* 0x6f1bdc */
extern char *ai_Unk5fb5b0;              /* 0x5fb5b0, the current scene */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);  /* 0x45eeb0, d3_sm.h */
char d3_LoadScene(char *name, void *f, void (*progress)(int a, int i, int n));                     /* 0x44be60 */
char d3if_ClearScene(int index);                    /* 0x447e60 */
void d3fl_SetDetail(unsigned int n);                    /* 0x43f8a0 */
int ahm_ReadRecord(void *f, unsigned char *p);  /* 0x4ed040, ahm.h's prototype (that header's `const char *tag` for mem_Alloc conflicts with gv.h) */

struct CHARACTER *ch_AllocAt(int id);               /* 0x402a30, ch.h's prototype (that header needs d3.h, which conflicts with gv.h here) */
extern char *txt_Ptrs[];           /* 0x143a1c0, the text table */
extern int hst_TextCurrent, hst_TextFirst, hst_KeyNext, hst_KeyFirst;   /* 0x601060, 0x601058, 0x60106c, 0x601064 .. 0x60106c */
extern unsigned char hst_TextsOn;               /* 0x6f0724 */
extern char hst_Key[];                 /* 0xf83520, the history key ("G12", 0x20 bytes; the save keeps 0x10) */
#include "hst_types.h"
#include "f3init.h"
#include "sv.h"
extern G12ROW hst_Lists[];             /* 0xf83540, the four history lists */
int ai_LoadWind();                     /* 0x5dc0c0 */
int unk_Unk4ed910();                     /* 0x4ed910 */
int gm_FindById();                     /* 0x4fea50 */

extern struct AIBLD *ai_MarketBuilding;                /* 0x6f1bc8 */
/* ai_Unk6f261c: view of ai_Unk6f2618 (tools/data_views.py) */
int ai_Unk6f2620 = 0;  /* 0x6f2620 / */
unsigned char he_Events[339968] = {0};  /* 0x6fab40, the */
char * he_QuickList[10921] = {0};  /* 0x74db40, pointers to the */
int ai_Unk7585e4 = 0;  /* 0x7585e4 */
void (*he_RunHandlers[5544])(unsigned char *) = {0};  /* 0x758600, the event run handlers by type (he.c */
int ai_Unk75e134 = 0;  /* 0x75e134 */
int ai_Unk75e138 = 0;  /* 0x75e138 .. the clock copy */

extern struct G17MSG *ai_Unka68d24;  /* 0xa68d24 */
extern struct G17MSG *ai_Unka68c00;  /* 0xa68c00 */
extern struct G17MSG *cm_SendQueue;  /* 0xf81160 */
void he_FreeEvents();                    /* 0x53d590 */
extern int d3_CurrentScene;             /* 0x633444, the current scene index (d3.h) */
char d3if_SelectScene(int index, char keep);  /* 0x4478e0, d3_interface.c */

/* ai_Unk1469700: view of ch_Motions (tools/data_views.py) */
char *strstr();                   /* 0x5de6b0, strstr-like (the "hozfaeller" misspelling check) */

int ai_Unk430fa0(struct VFSFILE *f, long offset, int origin);                    /* 0x430fa0, fseek */
int ai_Unk431170();                     /* 0x431170, file length */
void cm_SendLoadSize(int size);         /* 0x4c44d0, cm.h's prototype (that header redefines gv.h's globals in this TU) */
int cm_SendLoadBlock(void *p);          /* 0x4c4500, the 0x80-byte block of the load buffer; -1: send ring full */
/* 0x519010, the language number of the name s (case-insensitive, one of the five of the table 0x5ee978), 0xffff when unknown; kept in 0x62364c */

int ls_LanguageOf(char *s)
{
    char buf[0x40];
    int i;

    strcpy(buf, s);
    _strupr(buf);
    ls_Language = 0xffff;
    for (i = 0; i < 5; i++) {
        if (strcmp(ls_LanguageNames[i], buf) == 0) {
            ls_Language = i;
            return i;
        }
    }
    return 0xffff;
}
/* 0x5190a0, the language number found by 0x519010 */


int ls_GetLanguage(void)
{
    return ls_Language;
}
/* 0x5190b0, loads / writes the save game: opens the file (0x430470, mode 0x5f8324), remembers the path in 0x75e640, makes the references plain (0x519a00), the header (0x5194c0) and the fixed globals, then the writers of the subsystems in this order: 0x519bf0 (buildings), 0x519d20, 0x51a1a0, 0x51a4b0, 0x51b340, 0x4ea690, 0x519b50 and, when (flags & 2) == 0, 0x4f28d0, 0x51b890, 0x51b910, 0x4ed1e0, 0x51bdc0, 0x51c2f0, 0x51c450, 0x51c560, 0x51c870, 0x51aff0, 0x4b62e0, 0x589aa0, 0x5a8940, 0x4ebfb0, 0x4adb20, 0x4bff40; the first one that fails closes the file (0x430aa0), restores the references (0x519aa0) and returns 0. Returns 1. ROUGH: the callee of the file functions is 0x430e30(data, size, count, file) */


int ls_SaveGame(int name, char *path, unsigned flags)
{
    int f;

    f = vfs_Tfopen(name, "wb", vfs_Root);
    if (f == 0)
        return 0;
    strcpy(ls_SavePath.path, path);
    unk_Unk519a00();
    if (ls_WriteSaveHeader(f, flags) == 0) {
        ((int (*)())vfs_fclose)();
        unk_Unk519aa0();
        return 0;
    }
    vfs_fwrite(&ai_NextObjectId, 4, 1, f);
    vfs_fwrite(&ai_Unk6f2618, 4, 1, f);
    vfs_fwrite(ai_Time, 0xe, 1, f);
    vfs_fwrite(&ai_OwnPlayer, 1, 1, f);
    vfs_fwrite(ai_PlayerCharC + 4, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1bb8, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1bbc, 4, 1, f);
    vfs_fwrite(&ai_CharCount, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1bcc, 4, 1, f);
    vfs_fwrite(ai_UpdateStart, 0x18, 1, f);
    vfs_fwrite(&ai_Unk61a658, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1b88, 4, 1, f);
#define LS_STEP(fn) if (fn(f) == 0) { ((void (*)())vfs_fclose)(); unk_Unk519aa0(); return 0; }
    LS_STEP(ls_WriteObjects)
    LS_STEP(unk_Unk519d20)
    LS_STEP(unk_Unk51a1a0)
    LS_STEP(unk_Unk51a4b0)
    LS_STEP(unk_Unk51b340)
    LS_STEP(amt_SaveAemter)
    LS_STEP(unk_Unk519b50)
    if ((flags & 2) == 0) {
        LS_STEP(unk_Unk4f28d0)
        LS_STEP(ls_WriteRelations)
        LS_STEP(unk_Unk51b910)
        LS_STEP(gv_WriteAvatars)
        LS_STEP(unk_Unk51bdc0)
        LS_STEP(unk_Unk51c2f0)
        LS_STEP(ls_WriteHistory)
        LS_STEP(unk_Unk51c560)
        LS_STEP(unk_Unk51c870)
        LS_STEP(unk_Unk51aff0)
        LS_STEP(ai_Unk4b62e0)
        LS_STEP(ai_Unk589aa0)
        LS_STEP(ai_Unk5a8940)
        LS_STEP(ahm_SaveWalk)
        LS_STEP(hmt_Save)
        LS_STEP(rt_SaveRoutes)
    }
    vfs_fclose(f);
    unk_Unk519aa0();
    return 1;
}
/* 0x5194c0, the header of the save game: the id 0x1005a (also kept in 0x623650), a flag word (the argument), the player's name (0x20 bytes), two words, a byte, the language byte, the clock, then the scenario record (the name "SCENARIO" and the first character's fields: +0xb0 / +0xc0 16 bytes, +0x8c, +0xd4, +0xce4 / +0xce6, +0x8d, a value of 0x50a640 and the byte +0x89; or the zeroed defaults when no character), the byte 0x6f1c38, the buildings 0x6f2308 / 0x6f230c / 0x6f2310 ids, the table 0x76fa24 (8 words of 8 bytes), and the screen shot (0x78 x 0xa0 pixels of 3 bytes made with 0x41e6d0 from the bitmap 0x766320, 0xe100 bytes); finally 0x6f23fc and the 0x60 bytes of 0x75e640. Returns 1 when everything was written. ROUGH */


int ls_WriteSaveHeader(int f, unsigned flags)
{
    char scen[0x10];
    char b0, b1, b2, b3;
    int fields[2];
    unsigned id;
    char *shot;
    int i, j, k, rgb, who;
    char name[0x23];
    int v[2];
    char c1, c2;
    int ref;

    strcpy(scen, "SCENARIO");
    b0 = b1 = b2 = b3 = 0;
    ref = -1;
    shot = (char *)mem_Alloc(0xe100, "ls:tmp");
    id = 0x1005a;
    ls_Version = 0x1005a;
    if (vfs_fwrite(&id, 4, 1, f) <= 0 || vfs_fwrite(&flags, 1, 1, f) <= 0)
        return 0;
    who = ai_OwnPlayer;
    strcpy(name, ai_PlayerSlots[who]);
    v[0] = *(int *)(ai_PlayerSlots[who] + 0x40);
    v[1] = *(int *)(ai_PlayerSlots[who] + 0x44);
    c1 = ai_PlayerSlots[who][0x48];
    c2 = ai_NumPlayers;
    if (vfs_fwrite(name, 0x20, 1, f) <= 0 || vfs_fwrite(v, 8, 1, f) <= 0 || vfs_fwrite(&c1, 1, 1, f) <= 0
        || vfs_fwrite(&c2, 1, 1, f) <= 0 || vfs_fwrite(ai_Time, 0xe, 1, f) <= 0)
        return 0;
    if (ai_PlayerId == -1) {
        if (vfs_fwrite(scen, 0x10, 1, f) <= 0 || vfs_fwrite(&b1, 1, 1, f) <= 0 || vfs_fwrite(&b3, 1, 1, f) <= 0
            || vfs_fwrite(&b0, 1, 1, f) <= 0 || vfs_fwrite(&ref, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&b2, 1, 1, f) <= 0)
            return 0;
    } else {
        char *c = ai_Chars[(unsigned short)ai_PlayerId];

        if (vfs_fwrite(c + 0x30, 0x10, 1, f) <= 0 || vfs_fwrite(c + 0x40, 0x10, 1, f) <= 0
            || vfs_fwrite(c + 0xc, 1, 1, f) <= 0 || vfs_fwrite(c + 0x54, 4, 1, f) <= 0
            || vfs_fwrite(c + 0x164, 1, 1, f) <= 0 || vfs_fwrite(c + 0x166, 1, 1, f) <= 0
            || vfs_fwrite(c + 0xd, 1, 1, f) <= 0)
            return 0;
        ref = ((int (*)(int))gm_ConvertMoney)(gm_GetWorth(ai_PlayerId));
        if (vfs_fwrite(&ref, 4, 1, f) <= 0 || vfs_fwrite(c + 9, 1, 1, f) <= 0)
            return 0;
    }
    if (vfs_fwrite(&ls_SaveSlot, 1, 1, f) <= 0)
        return 0;
    ref = ai_PlayerBuilding ? *(int *)(ai_PlayerBuilding + 1) : (ai_FallbackBuilding ? *(int *)(ai_FallbackBuilding + 1) : -1);
    vfs_fwrite(&ref, 4, 1, f);
    ref = sv_PanelShown ? *(int *)(sv_PanelShown + 2) : -1;
    vfs_fwrite(&ref, 4, 1, f);
    for (i = 0; i < 8; i++)
        vfs_fwrite(ai_Unk76fa24 + i * 8, 4, 1, f);
    k = 0;
    for (j = 0x78; j != 0; j--) {
        int px = k * 3 + 1 + (int)shot;

        for (i = 0xa0; i != 0; i--) {
            dd_SplitRGB(ai_ScreenImage[k], px - 1, px, px + 1);
            k++;
            px += 3;
        }
    }
    rgb = vfs_fwrite(shot, 0xe100, 1, f);
    if (rgb == 0)
        return 0;
    if (shot != 0)
        mem_Free(shot);
    vfs_fwrite(&ai_Difficulty, 4, 1, f);
    vfs_fwrite(ls_SavePath.path, 0x60, 1, f);
    return 1;
}
/* 0x519a00, before a save: the objects of the buildings get their building back (+0x200 of the building's scene object, +0x61), and the references of the 10-byte table 0xf31166 (kind byte at -5: 1 character, 2 building, 3 object, 9 ..) are turned into plain ids (the id inside the record: +4 / +1 / +2 / +0) @deep B: 50 code hunks, read in asmdiff: they are register / scheduling shapes with no meaning difference - the first loop swaps the index and base registers (ours [edx+eax*1] vs the original [eax+edx*1], k in edx vs eax), the second loop's comparison polarity (ours cmp/jge, the original cmp/jl) and the *p reload: the original reloads [eax] after the store in the kind==3 / kind==9 branches, ours forwards the register (edx) - the kinds, offsets (+4/+2/+1/+0) and bounds (0xa900 / 0xf81166) match the original at 0x519a00 exactly. Shape is match-deep's. Q2/Q7: the callees d3if_SelectScene, ai_Unkf31166 and the referenced globals are unnamed; Q6 is described. */


void unk_Unk519a00(void)
{
    int saved = d3_CurrentScene;
    int k;
    int *p;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        if (ai_Blds[k] != 0 && *(int *)(ai_Blds + k + 0x61) != 0)
            *(int *)(*(int *)(ai_Blds + k + 0x61) + 0x200) = *(int *)(ai_Blds + k + 1);
    }
    d3if_SelectScene(saved, 1);
    for (p = ls_RefTargets; (int)p < (int)ls_RefTargets + 0x50000; p = (int *)((char *)p + 10)) {
        if (*p != 0) {
            char kind = *((char *)p - 5);

            if (kind == 1)
                *p = *(int *)(*p + 4);
            if (kind == 3)
                *p = *(int *)(*p + 2);
            if (kind == 2)
                *p = *(int *)(*p + 1);
            if (kind == 9)
                *p = *(int *)*p;
        }
    }
}
/* 0x519aa0, after a save: the buildings' objects point back to their buildings (+0x200) and the ids of the references table are turned into pointers again (0x505df0 character, 0x4fe980 object, 0x502020 building, 0x4e50a0 kind 9) @deep B: 40 code hunks, read in asmdiff (the equal stretches around them checked too): the kind order 1, 3, 2, 9, the offsets and the calls 0x505df0 / 0x4fe980 / 0x502020 / 0x4e50a0 match the original at 0x519aa0 exactly; the hunks are shapes - ours keeps the loop counter in another register of the first loop, the second loop's entry compares through ecx with jge where the original compares esi with jl, and the original reloads [esi] before the kind==3 / kind==2 pushes where ours forwards a register. Shape is match-deep's. Q2/Q7: the callees and globals are unnamed; Q6 is described. */


void unk_Unk519aa0(void)
{
    int saved = d3_CurrentScene;
    int k;
    int *p;
    char *b;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(char **)(*(int *)(b + 0x61) + 0x200) = b;
    }
    d3if_SelectScene(saved, 1);
    for (p = ls_RefTargets; (int)p < (int)ls_RefTargets + 0x50000; p = (int *)((char *)p + 10)) {
        if (*p != 0) {
            if (*((char *)p - 5) == 1)
                *p = (int)gm_FindChar(*p);
            if (*((char *)p - 5) == 3)
                *p = gm_FindObjectInst(*p);
            if (*((char *)p - 5) == 2)
                *p = gm_FindBuildingById(*p);
            if (*((char *)p - 5) == 9)
                *p = cm_PartyById(*p);
        }
    }
}
/* 0x519b50, after a load: the buildings' objects point back to their buildings, the scene is restored (0x44aa10) and the pointers are set again @deep B: 16 code hunks, all one register-order choice: the same addressing modes with the index and base registers swapped (ours [edx+eax*1] / [ecx+edx*1], the original [eax+edx*1] / [edx+ecx*1] - k lives in eax in the original and in edx/ecx in ours); every other instruction is equal (8 + 12 + 29 equal). No meaning difference: the two loops, 0x44aa10(0, f), the restores and the return 1 match the original at 0x519b50. Register assignment is match-deep's (vargen ren). Q2/Q7: the unnamed callees/globals; Q6 is described. */


int unk_Unk519b50(int f)
{
    int saved = d3_CurrentScene;
    int k;
    char *b;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(int *)(*(int *)(b + 0x61) + 0x200) = *(int *)(b + 1);
    }
    d3_SaveScene(0, f);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(char **)(*(int *)(b + 0x61) + 0x200) = b;
    }
    d3if_SelectScene(saved, 1);
    return 1;
}
/* 0x519bf0, the object list ai_Objects (0x5008 records of 0x43 bytes; the used ones have a nonzero first word): the count, then each used record in pieces (2, 4, 4, 4, 4, 1, 1 bytes and 0x1f bytes at +0x1c) */


int ls_WriteObjects(int f)
{
    int n = 0;
    int i, off;
    short *p = ai_Objects;
    short *q;
    char *r;

    for (i = 0x5008; i != 0; i--) {
        if (*p != 0)
            n++;
        p = (short *)((char *)p + 0x43);
    }
    if (vfs_fwrite(&n, 4, 1, f) <= 0)
        return 0;
    off = 0;
    do {
        r = (char *)(off + (int)ai_Objects);
        q = (short *)(off + (int)ai_Objects);
        if (*q != 0) {
            if (vfs_fwrite(r, 2, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 2, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 6, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 10, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0xe, 4, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x12, 1, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x13, 1, 1, f) <= 0)
                return 0;
            if (vfs_fwrite(r + 0x1c, 0x1f, 1, f) <= 0)
                return 0;
        }
        off += 0x43;
    } while (off < 0x14f218);
    return 1;
}
/* 0x519d20, the buildings: the count of the used ones (0x100 records of 0xa9 bytes at 0x889f74), then each in pieces (the fields +0 .. +0x5c, then the type specific tail: type 0x1e (the 0x600 bytes of 0x18-byte stock records behind +0x71), the types 0x48 .. 0x4a by 0x4f17f0, 0x4b .. 0x4d by 0x4ecf10, the others 0x30 bytes at +0x65), the 0x10 bytes at +0x99; at the end the table of the bounding boxes 0xf812e4 (count 0xf812e0, 0x60 bytes each: name, min, max) @deep B: 280 code + 114 regs hunks, one systematic register swap through the whole body: ours keeps the building record in edi and the file f in esi, the original esi / edi (sampled asmdiff at 0x519d9a .. 0x519ded: every block is push esi / lea edx,[edi+off] vs push edi / lea edx,[esi+off] with the same offsets and sizes); the count n lives in a stack slot in the original (inc DWORD PTR [esp+0x10]) and the original's frame is 1 slot (push ecx) against our 2 (sub esp,0x8), which shifts everything. The meaning matches (0x100 records of 0xa9 bytes, the field pieces, the type tails). Shape is match-deep's (vargen ren). Q2/Q7: the callees (0x430e30 -> vfs_fwrite is now named) and the ai_Unk globals are still unnamed; Q6 described. */


int unk_Unk519d20(int f)
{
    int n = 0, off = 0, i, j;
    char *p, c;

    p = ai_Blds;
    for (i = 0x100; i != 0; i--) {
        if (*p != 0)
            n++;
        p += 0xa9;
    }
    LS_W(&n, 4)
    do {
        p = ai_Blds + off;
        if (ai_Blds[off] != 0) {
            LS_W(p, 1)
            LS_W(p + 1, 4)
            LS_W(p + 5, 0x20)
            LS_W(p + 0x25, 2)
            LS_W(p + 0x27, 2)
            LS_W(p + 0x29, 2)
            LS_W(p + 0x2b, 4)
            LS_W(p + 0x2f, 1)
            LS_W(p + 0x34, 1)
            LS_W(p + 0x35, 4)
            LS_W(p + 0x39, 4)
            LS_W(p + 0x3d, 4)
            LS_W(p + 0x41, 4)
            LS_W(p + 0x45, 4)
            LS_W(p + 0x49, 4)
            LS_W(p + 0x4d, 4)
            LS_W(p + 0x51, 4)
            LS_W(p + 0x55, 4)
            LS_W(p + 0x5a, 2)
            LS_W(p + 0x5c, 1)
            c = *p;
            if (c == 0x1e) {
                for (j = 0; j < 0x600; j += 0x18) {
                    char *q = *(char **)(p + 0x71);

                    LS_W(q + j, 4)
                    LS_W(q + j + 4, 4)
                    LS_W(q + j + 8, 1)
                    LS_W(q + j + 9, 1)
                    LS_W(q + j + 10, 2)
                    LS_W(q + j + 0x10, 4)
                    LS_W(q + j + 0xc, 1)
                    LS_W(q + j + 0xd, 1)
                }
            } else if (c == 'H' || c == 'I' || c == 'J') {
                if (unk_Unk4f17f0(f, p) == 0)
                    return 0;
            } else if (c == 'K' || c == 'L' || c == 'M') {
                if (ahm_WriteRecord(f, p) == 0)
                    return 0;
            } else {
                LS_W(p + 0x65, 0x30)
            }
            LS_W(p + 0x99, 0x10)
        }
        off += 0xa9;
    } while (off < 0xa900);
    LS_W(&ai_NBauplaetze, 4)
    ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, ai_NBauplaetze * 0x60);
    for (i = 0; i < ai_NBauplaetze; i++) {
        LS_W(ai_Bauplaetze[i].name, 0x40)
        LS_W(&ai_Bauplaetze[i].x0, 0x10)
        LS_W(&ai_Bauplaetze[i].x1, 0x10)
    }
    return 1;
}
/* 0x51a1a0, the records of 0x7d5f80 .. 0x7d6a42 (0xac bytes each): the id, name, ids, the floats and ints at the offsets below; the fields +0x52 .. +0x6a only when the save game's version 0x623650 is above 0x10013 @deep B: 135 code hunks, read at the start and sampled: the base pointer 0x7d5f82 lands in eax in ours and in esi in the original, so the first block is sub eax,0x2 vs lea eax,[esi-0x2] and every later block's lea/sub and pairing shifts; the offsets (-2, 0, 0x12, 0x26, 0x1a, 0x1e, 0x2a, 0x2e, 0x32, 0x36, 0x3e, 0x42, 0x46, 0x4a, 0x4e and the 0x10013 version tail +0x52 .. +0x6a), the sizes and vfs_fwrite calls match the original at 0x51a1a0 exactly. Shape is match-deep's. Q2/Q7: gm_PlayerRecordBytes and ls_Version unnamed; Q6 described. */


int unk_Unk51a1a0(int f)
{
    char *p = gm_PlayerRecordBytes;

    for (;;) {
        LS_W(p - 2, 2)
        LS_W(p, 0x10)
        LS_W(p + 0x12, 4)
        LS_W(p + 0x26, 4)
        LS_W(p + 0x1a, 4)
        LS_W(p + 0x1e, 4)
        LS_W(p + 0x2a, 4)
        LS_W(p + 0x2e, 4)
        LS_W(p + 0x32, 4)
        LS_W(p + 0x36, 4)
        LS_W(p + 0x3e, 4)
        LS_W(p + 0x42, 4)
        LS_W(p + 0x46, 4)
        LS_W(p + 0x4a, 4)
        LS_W(p + 0x4e, 4)
        if (ls_Version > 0x10013) {
            LS_W(p + 0x52, 4)
            LS_W(p + 0x56, 4)
            LS_W(p + 0x5a, 4)
            LS_W(p + 0x3a, 4)
            LS_W(p + 0x62, 4)
            LS_W(p + 0x66, 4)
            LS_W(p + 0x6a, 4)
        }
        LS_W(p + 0x5e, 4)
        LS_W(p + 0x72, 0x11)
        LS_W(p + 0x86, 4)
        LS_W(p + 0x8a, 0x10)
        LS_W(p + 0x9a, 0xc)
        LS_W(p + 0xa6, 4)
        LS_W(p + 0x6e, 4)
        p += 0xac;
        if ((int)p > ((unsigned)&ai_Unk7d6a40 + 0x1))
            return 1;
    }
}
/* 0x51a4b0, writes the characters (the counterpart of 0x51de70): the player character, the count of the used
   records of 0x770b80 (0x21c bytes each, a record is used when its id is not -1), the two judges 0x6f1bfc /
   0x6f1c00 and the eight slots of 0x6f1c04 as ids (+4 of the record, -1 for none), then each used record in
   pieces (the pointers at +0x16c / +0x170 / +0x17c / +0x184 as ids, the words at +0x54 / +0x18c stored with an
   offset of -0x59a / -0x639). The record pointer runs at +2 (the original's bias). Returns 1.
   @deep draft: 836 differing instructions, every one read in the asmdiff/diffclass: the record field list and its order match the original at 0x51a5b4..0x51ad15 exactly (offsets c-2, c+0, c+2, c+4, c+6, c+7, c+8, c+0xa, c+0xb, c+0xe, c+0x12 .. c+0x20a, sizes 1/2/4/5/0x10/0x18/0x20/0xa8, the four reference fields with +1 / +4 and -1, the two -0x59a / -0x639 offsets), so no meaning error is left; what differs is stack slot choice (the original keeps local_c, cnt and local_4 in the top local slot [esp+0x1c], ours puts local_c in [esp+0x20] and cnt in [esp+0xc] and uses the dead argument slot differently) and push scheduling (the original hoists push esi / push 0x1 / push 0x4 above the je of the table loop and of the three pointer-field blocks, ours merges the branches and schedules the pushes after); needs vargen slot / order rounds. The two manual vfs_fwrite checks are fixed to the original's unsigned jbe (LS_W); the record loop now uses the natural unbiased pointer and VC6 biases the induction variable to +2 as the original does. Readability Q2/Q4 need AICHAR field names for the record (the save writes raw slices) + the judge/slot globals named. */


int unk_Unk51a4b0(int f)
{
  int cnt;
  char *c;
  short *psVar3;
  int *puVar4;
  int *piVar5;
  int local_c;
  int local_8;
  int local_4;

  local_c = 0xffffffff;
  LS_W(&ai_PlayerId, 2)
  cnt = 0;
  psVar3 = (short *)ai_Chars;
  do {
    if (*psVar3 != -1) {
      cnt = cnt + 1;
    }
    psVar3 = psVar3 + 0x10e;
  } while ((int)psVar3 < (int)&ai_Chars[768]);
  LS_W(&cnt, 4)
  if (ai_JudgeCatholic != 0) {
    LS_W(&ai_JudgeCatholic->netId, 4)
  }
  else {
    LS_W(&local_c, 4)
  }
  if (ai_JudgeProtestant != 0) {
    LS_W(&ai_JudgeProtestant->netId, 4)
  }
  else {
    LS_W(&local_c, 4)
  }
  piVar5 = (int *)ai_KindBChars;
  do {
    local_4 = 0xffffffff;
    if (*piVar5 != 0) {
      puVar4 = (int *)(*piVar5 + 4);
    }
    else {
      puVar4 = &local_4;
    }
    LS_W(puVar4, 4)
    piVar5 = piVar5 + 1;
  } while ((int)piVar5 < (int)&ai_KindBChars[8]);
  c = (char *)ai_Chars;
  do {
    if (*(short *)(c) != -1) {
      LS_W(c, 2)
      LS_W((c + 0x20c), 4)
      LS_W((c + 2), 1)
      LS_W((c + 4), 4)
      LS_W((c + 8), 1)
      LS_W((c + 9), 1)
      LS_W((c + 0xa), 2)
      LS_W((c + 0xc), 1)
      LS_W((c + 0xd), 1)
      LS_W((c + 0x10), 4)
      LS_W((c + 0x14), 4)
      LS_W((c + 0x18), 4)
      LS_W((c + 0x1c), 4)
      LS_W((c + 0x20), 4)
      LS_W((c + 0x24), 4)
      LS_W((c + 0x28), 2)
      LS_W((c + 0x2c), 4)
      LS_W((c + 0x30), 0x10)
      LS_W((c + 0x40), 0x10)
      LS_W((c + 0x50), 2)
      cnt = *(int *)((c + 0x54)) + -0x59a;
      LS_W(&cnt, 4)
      LS_W((c + 0x58), 1)
      LS_W((c + 0x5c), 0x20)
      LS_W((c + 0x7c), 4)
      LS_W((c + 0x80), 5)
      LS_W((c + 0x88), 0xa8)
      LS_W((c + 0x164), 1)
      LS_W((c + 0x165), 1)
      LS_W((c + 0x166), 1)
      LS_W((c + 0x167), 1)
      LS_W((c + 0x168), 1)
      LS_W((c + 0x169), 1)
      if (*(int *)((c + 0x16c)) != 0) {
        puVar4 = (int *)(*(int *)((c + 0x16c)) + 1);
      }
      else {
        puVar4 = &local_4;
        local_4 = 0xffffffff;
      }
      LS_W(puVar4, 4)
      if (*(int *)((c + 0x170)) != 0) {
        puVar4 = (int *)(*(int *)((c + 0x170)) + 1);
      }
      else {
        local_4 = 0xffffffff;
        puVar4 = &local_4;
      }
      LS_W(puVar4, 4)
      LS_W((c + 0x174), 4)
      if (*(int *)((c + 0x17c)) != 0) {
        puVar4 = (int *)(*(int *)((c + 0x17c)) + 4);
      }
      else {
        puVar4 = &local_4;
        local_4 = 0xffffffff;
      }
      LS_W(puVar4, 4)
      LS_W((c + 0x180), 1)
      cnt = *(int *)((c + 0x18c)) + -0x639;
      LS_W(&cnt, 4)
      LS_W((c + 0x190), 4)
      LS_W((c + 0x194), 4)
      LS_W((c + 0x198), 4)
      LS_W((c + 0x19c), 4)
      LS_W((c + 0x1a0), 4)
      LS_W((c + 0x1a4), 4)
      LS_W((c + 0x1a8), 4)
      LS_W((c + 0x1ac), 4)
      LS_W((c + 0x1b0), 1)
      LS_W((c + 0x1b1), 1)
      LS_W((c + 0x1b4), 0x10)
      LS_W((c + 0x1c8), 4)
      LS_W((c + 0x1cc), 4)
      LS_W((c + 0x1d0), 4)
      LS_W((c + 0x1d4), 0x10)
      LS_W((c + 0x1e4), 4)
      LS_W((c + 0x1e8), 4)
      LS_W((c + 0x1ec), 4)
      LS_W((c + 0x1f0), 4)
      LS_W((c + 0x1c5), 1)
      if (*(int **)((c + 0x184)) != 0) {
        local_8 = **(int **)((c + 0x184));
      }
      else {
        local_8 = 0xffffffff;
      }
      LS_W(&local_8, 4)
      LS_W((c + 0x1f4), 0x18)
      LS_W((c + 0x210), 4)
      LS_W((c + 0x214), 1)
      LS_W((c + 0x215), 1)
      LS_W((c + 0x216), 1)
      LS_W((c + 0x217), 1)
      LS_W((c + 0x218), 1)
    }
    c = c + 0x21c;
  } while ((int)c < (int)&ai_Chars[768]);
  return 1;
}
/* 0x51ad30, the index of the action queue entry p (0x631050, 0x194 bytes each, ch.h: CHACTION *ch_Queue), -1 for NULL */


unsigned ls_ActionIndex(char *p)
{
    return p ? (unsigned)(p - ls_ActionQueue) / 0x194 : 0xffffffff;
}
/* 0x51ad60, the action queue entry n (0x631050), NULL for -1 */


int ls_ActionAt(int n)
{
    if (n == -1)
        return 0;
    return (int)ls_ActionQueue + n * 0x194;
}
/* 0x51ad80, the record r of a character scene (+0 / +8 .. , the names +0x170 / +0x130, the object's bounding box from +0x34, the word `id`, the 0x40 bytes of +0x64 / the 0x40 bytes of its parent's +0x68): writes the pieces; returns 1 when all was written @deep draft: one code hunk left, the 0x1c-byte zero substitute (7 mov DWORD PTR [esp+X],0 stores in the original at 0x51aea1..0x51aed1 vs our rep stosd; tried int[7]={0} (mov + rep stosd), char[0x1c]+memset (rep stosd ecx=7), a zero loop (rep stosd), two arrays, a struct with members and one with ={0} (all force ebp), seven separate ints (dead, frame 0x40) - VC6 always inlines the zero with rep stosd; the original's 7 immediate stores need the shape a vargen round can search). Everything else matches: 233/232 instructions, the field order/sizes, the blank[0x40] buffer, zero+0xc, the per-branch != 0x40 checks and the <= 0 LS_W checks; the two condition chains are written from the non-zero case (q != 0 && ...) as the original lays them out. Q1/Q2/Q4 remain: the function name, ch_EmptyName, and the raw pointer casts of the scene record field offsets. */


int unk_Unk51ad80(char *r, int id, int f)
{
    char blank[0x40];
    char *q;

    LS_W(r, 4)
    LS_W(r + 8, 0x20)
    LS_W(r + 0x2c, 4)
    LS_W(r + 0x30, 4)
    LS_W(r + 0x8c, 2)
    LS_W(r + 0x170, 0x30)
    LS_W(r + 0x130, 0x40)
    if (*(int *)(r + 0x88) == 0 || *(char *)(*(int *)(r + 0x88) + 0x3d5) != 0) {
        int zero[7] = {0};

        LS_W(zero, 0x10)
        LS_W((char *)zero + 0xc, 0xc)
    } else {
        LS_W(*(char **)(r + 0x34) + 0x4c, 0x10)
        LS_W(*(char **)(r + 0x34) + 0x84, 0xc)
    }
    LS_W(*(char **)(r + 0x34) + 0x218, 1)
    LS_W(&id, 4)
    q = *(char **)(r + 100);
    if (q != 0) {
        LS_W(q, 0x40)
    }
    else {
        blank[0] = ch_EmptyName[0];
        memset(blank + 1, 0, 0x3f);
        LS_W(blank, 0x40)
    }
    q = *(char **)(r + 0x74);
    if (q != 0 && q != *(char **)(r + 0x70) && *(int *)(q + 0x68) != 0) {
        if (vfs_fwrite(*(char **)(q + 0x68), 1, 0x40, f) != 0x40)
            return 0;
    } else {
        blank[0] = ch_EmptyName[0];
        memset(blank + 1, 0, 0x3f);
        if (vfs_fwrite(blank, 1, 0x40, f) != 0x40)
            return 0;
    }
    return 1;
}


/* 0x51aff0, the characters' scene records and the table 0x631050 (0x194-byte records, 0x800 of them with the used flag at +9): the count and the records of the characters (0x770d04, 0x21c each) through 0x51ad80, the same for the objects of the kind 0x1d in the list 0x7e3f34, then the table's pointers are turned into indices (+0x14 the first word of the pointed record, +0x24 / +0x28 through 0x51ad30), each used record whose scene object (the list 0x1468ae0) exists is written in pieces (the flag byte, the vectors, the names, a 0xa0-byte block and a byte), and the pointers are restored (0x51ad60). Returns 1. ROUGH: the pieces of the record copy (local_188 .. local_4) are the fields of the 0x65-word copy */
extern char * ai_Unk1468ae0[0x300];             /* 0x1468ae0, the scene objects (0x300) */
extern char *ai_Goods;              /* 0x91aaf0, the object prototypes (0x41 bytes) */
int mem_IsValid();                     /* 0x4225e0 */
/* 0x51aff0 @deep draft: the loop structure is now read against the original at 0x51aff0..0x51b340 and the record pieces fixed - the draft wrote rec+0x70 / rec+0x2c / rec+0x38, the original at 0x51b262 / 0x51b29f / 0x51b2b9 writes rec+0xf0 (0xa0 bytes) / rec+0x24 / rec+0x28, exactly what the reader in unk_Unk51e900 reads back (t+0x3c, t+9, t+10), so three wrong offsets were a meaning error and are corrected; the flag/rec reset moved before the fourth loop (the original zeroes them once at 0x51b165..0x51b177), the tab local removed so both table loops re-read ls_ActionQueue per iteration as the original does. Remaining 209 code hunks: register allocation and scheduling - the original's first count loop runs in eax/ecx with no callee-saved register and the pushes ebx/ebp/esi/edi come only after it at 0x51b013, ours pushes them at entry (so cnt is memory-incremented in [esp+0x14] and the zero sits in edi), then the whole function is shifted (ebp/esi/ebx/edi roles and the esp-relative offsets of the rec pieces). Readability Q2/Q4 unchanged: the CHACTION typed view (ch.h) is blocked in ls.c by gv.h's K&R d3sm_GetVertex; the record is handled as int*. */
int unk_Unk51aff0(int f)
{
    int cnt = 0, i, k, n;
    int *p;
    int rec[0x65];
    char flag;

    for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182); p += 0x87) {
        if (*p)
            cnt++;
    }
    LS_W(&cnt, 4)
    for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182); p += 0x87) {
        if (*p != 0 && unk_Unk51ad80(*p, p[-0x60], f) == 0)
            return 0;
    }
    cnt = 0;
    {
        unsigned short *q = (unsigned short *)ai_Objects;

        for (i = 0x5008; i != 0; i--) {
            if (*(char *)(ai_Goods + *q * 0x41) == 0x1d)
                cnt++;
            q = (unsigned short *)((char *)q + 0x43);
        }
    }
    LS_W(&cnt, 4)
    for (k = 0; k < 0x14f218; k += 0x43) {
        char *q = (char *)ai_Objects + k;

        if (*(char *)(ai_Goods + *(unsigned short *)q * 0x41) == 0x1d
            && unk_Unk51ad80(*(int *)(q + 0x3b), *(int *)(q + 2), f) == 0)
            return 0;
    }
    for (k = 0; k < 0xc5440; k += 0x194) {
        int *r = (int *)(ls_ActionQueue + k);

        if (ls_ActionQueue[k + 9] == 0) {
            memset(r, 0, 0x194);
        } else {
            r[5] = *(int *)r[5];
            r[9] = ls_ActionIndex((char *)r[9]);
            r[10] = ls_ActionIndex((char *)r[10]);
        }
    }
    flag = 0;
    memset(rec, 0, sizeof(rec));
    for (k = 0; k < 0xc5440; k += 0x194) {
        int *r = (int *)(ls_ActionQueue + k);

        n = *(int *)(ls_ActionQueue + k + 0x14);
        if (n > -1 && n < 0x300) {
            n = ai_Unk1468ae0[n];
            if (mem_IsValid(n) == 0)
                __asm int 3
            if (n != 0 && *(int *)(*(int *)(n + 0x34) + 0x200) != 0) {
                flag = *((char *)r + 9);
                memcpy(rec, r, 0x194);
            }
            LS_W(&flag, 1)
            LS_W((char *)rec + 0xc, 4)
            LS_W((char *)rec + 0x14, 4)
            LS_W((char *)rec + 0x30, 0x20)
            LS_W((char *)rec + 0x50, 0x20)
            LS_W((char *)rec + 0xf0, 0xa0)
            LS_W((char *)rec + 0x190, 1)
            LS_W((char *)rec + 0x24, 4)
            LS_W((char *)rec + 0x28, 4)
        }
    }
    for (k = 0; k < 0xc5440; k += 0x194) {
        int *r = (int *)(ls_ActionQueue + k);

        if (ls_ActionQueue[k + 9] != 0) {
            r[5] = ai_Unk1468ae0[r[5]];
            r[9] = ls_ActionAt(r[9]);
            r[10] = ls_ActionAt(r[10]);
        }
    }
    return 1;
}
/* 0x51b340, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer*0xaa4 but the original computes edi = 0x7d6a60 + k*0x2a90 (eax=k; shl 4; add k; lea *5; *8; shl 4 = 0x2a90) and writes edi+0/+4/+8/+0xc; the player name +0x40 uses 0x7e3f80 + i*0xbd and the word +0x54 uses 0x7e3f94 + i*0x17a, the original uses the slot base + 0x40 / +0x54 (0x7e3f40 + i*0x2f4, lea [edi+0x40] / lea [edi+0x54] at 0x51b543 / 0x51b55c); the four multipliers are Ghidra's lost pointer scale (0xaa4 = 0x2a90/4, 0xbd = 0x2f4/4, 0x17a = 0x2f4/2). Fixing only these made unitdiff WORSE (261 -> 318 differing instructions, the corrected addressing changes the register/scheduling shape of the whole block), so the function needs the rewrite with the fix, not a base patch. The first write check is signed in the draft (if (iVar1 != 0)) where the original uses unsigned ja; the ROUGH comment stays. */


int unk_Unk51b340(int f)
{
  int iVar1;
  int iVar2;
  int iVar3;
  char *puVar4;
  int local_4;
  local_4 = 0x55;
  iVar1 = vfs_fwrite(&local_4,4,1,f);
  if (iVar1 != 0) {
    local_4 = 0;
    do {
      iVar1 = local_4;
      iVar3 = local_4 * 0x2a90;
      LS_W(((char *)(unsigned)&ai_Unk7d6a60) + local_4 * 0xaa4, 4)
      LS_W(((char *)(unsigned)&ai_Unk7d6a64) + iVar1 * 0xaa4, 4)
      LS_W(((char *)((unsigned)&ai_Unk7d6a64 + 0x4)) + iVar1 * 0xaa4, 4)
      LS_W(((char *)((unsigned)&ai_Unk7d6a64 + 0x8)) + iVar1 * 0xaa4, 4)
      iVar1 = 0;
      puVar4 = ((char *)((unsigned)ai_Unk7d6a60 + 0x18)) + iVar3;
      do {
        LS_W(puVar4 + -8, 2)
        LS_W(puVar4 + -4, 4)
        LS_W(puVar4, 4)
        LS_W(puVar4 + 4, 4)
        LS_W(puVar4 + 0x18, 4)
        LS_W(puVar4 + 0x1c, 4)
        LS_W(puVar4 + 0x28, 4)
        LS_W(puVar4 + 0x2c, 4)
        LS_W(puVar4 + 0x30, 4)
        LS_W(puVar4 + 0x34, 2)
        LS_W(puVar4 + 0x24, 4)
        iVar1 = iVar1 + 1;
        puVar4 = puVar4 + 0x80;
      } while (iVar1 < 0x55);
      local_4 = local_4 + 1;
    } while (local_4 < 5);
    local_4 = 0;
    while( 1 ) {
      iVar2 = local_4;
      iVar1 = local_4 * 0x2f4;
      iVar3 = vfs_fwrite(((char *)(unsigned)&ai_PlayerSlots) + iVar1,0x20,1,f);
      if (iVar3 == 0) break;
      LS_W(((char *)(unsigned)&ai_Unk7e3f80) + iVar2 * 0xbd, 8)
      LS_W(((char *)((unsigned)&ai_PlayerSlots + 0x48)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_PlayerSlots + 0x4c)) + iVar1, 4)
      LS_W(((char *)((unsigned)&ai_PlayerSlots + 0x50)) + iVar1, 4)
      LS_W(((char *)(unsigned)&ai_Unk7e3f94) + iVar2 * 0x17a, 2)
      LS_W(((char *)((unsigned)&ai_Unk7e3f94 + 0x4)) + iVar1, 8)
      LS_W(((char *)((unsigned)&ai_Unk7e3f94 + 0xc)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e3f94 + 0xd)) + iVar1, 1)
      iVar2 = 0;
      puVar4 = ((char *)((unsigned)&ai_Unk7e3f94 + 0x10)) + iVar1;
      do {
        LS_W(puVar4, 8)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 8;
      } while (iVar2 < 10);
      iVar2 = 0;
      puVar4 = ((char *)((unsigned)&ai_Unk7e3f94 + 0x60)) + iVar1;
      do {
        LS_W(puVar4, 8)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 8;
      } while (iVar2 < 10);
      LS_W(((char *)((unsigned)&ai_Unk7e3ff8 + 0x4c)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e3ff8 + 0x4d)) + iVar1, 1)
      iVar2 = 0;
      puVar4 = ((char *)(unsigned)&ai_Unk7e4048) + iVar1;
      do {
        LS_W(puVar4, 4)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 4;
      } while (iVar2 < 4);
      iVar2 = 0;
      puVar4 = ((char *)(unsigned)&ai_Unk7e4058) + iVar1;
      do {
        LS_W(puVar4, 4)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 4;
      } while (iVar2 < 4);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x10)) + iVar1, 4)
      iVar2 = 0;
      do {
        LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x14)) + iVar2 + iVar1, 1)
        iVar2 = iVar2 + 1;
      } while (iVar2 < 0xb);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x1f)) + iVar1, 1)
      iVar2 = 0;
      do {
        LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x20)) + iVar2 + iVar1, 1)
        iVar2 = iVar2 + 1;
      } while (iVar2 < 7);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x27)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x28)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x29)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x2a)) + iVar1, 1)
      iVar2 = 0;
      puVar4 = ((char *)((unsigned)&ai_Unk7e4058 + 0x2c)) + iVar1;
      do {
        LS_W(puVar4, 0x12)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 0x12;
      } while (iVar2 < 8);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0xbc)) + iVar1, 4)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0xc0)) + iVar1, 4)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0xc4)) + iVar1, 0xd0)
      LS_W(((char *)(unsigned)&ai_Unk7e422c) + iVar1, 8)
      local_4 = local_4 + 1;
      if (3 < local_4) {
        return 1;
      }
    }
  }
  return 0;
}
/* 0x51b890, writes the two relation maps ai_Relations / ai_RelationMemory (0x300 x 0x300 bytes, row by row) */


int ls_WriteRelations(int f)
{
  int i;
  char *p;
  p = ((char *)ai_Relations);
  do {
    i = 0;
    do {
      LS_W(p + i, 1)
      i = i + 1;
    } while (i < 0x300);
    p = p + 0x300;
  } while ((int)p < (int)ai_Relations + 0x300 * 0x300);
  p = ((char *)ai_RelationMemory);
  do {
    i = 0;
    do {
      LS_W(p + i, 1)
      i = i + 1;
    } while (i < 0x300);
    p = p + 0x300;
  } while ((int)p < (int)ai_RelationMemory + 0x300 * 0x300);
  return 1;
}
/* 0x51b910, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 81, all four hunks are two loops' induction-variable base: 0x51bc0f/0x51bd2e VC6 bases the IV at p+4 (lea [edi-8]..[edi+8], cmp edi-4) / row+2, the original keeps p (lea [edi-4]..[edi+0xc], cmp edi) / row; the 372 instructions before them are equal after the LS_W early-return rewrite (the if(r!=0){...} return 0 form made VC6 merge every return block, 361 hunks); read both loops and tried char* byte offsets, int*, int address, while and for spellings, VC6 canonicalizes the same; the loops' data/offsets are identical, only the base register spelling differs. Readability Q1/Q2/Q3/Q5 open: 34 save-image globals have no role evidence yet */


int unk_Unk51b910(int f)
{
  int iVar1;
  char *puVar3;
  char *puVar4;
  char *puVar5;
  LS_W(((char *)(unsigned)&ord_Statistics), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x4)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x8)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0xc)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x10)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x14)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x18)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x1c)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x20)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x24)), 4)
  LS_W(((char *)(unsigned)&ord_RatingRate), 4)
  LS_W(((char *)(unsigned)&ord_StatisticsClock), 4)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0x4)), 2)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0x6)), 4)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0xa)), 4)
  LS_W(((char *)(unsigned)&ai_RatingBase), 4)
  LS_W(((char *)(unsigned)&ai_Rating), 4)
  LS_W(((char *)(unsigned)&ord_MessageRecord), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x4)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x8)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0xc)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x10)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x14)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x18)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x1c)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x20)), 4)
  puVar4 = ((char *)((unsigned)&ord_StatisticsHistory + 0x4));
  do {
    LS_W(puVar4 - 4, 4)
    LS_W(puVar4, 4)
    LS_W(puVar4 + 4, 4)
    LS_W(puVar4 + 8, 4)
    LS_W(puVar4 + 0xc, 4)
    puVar4 = puVar4 + 0x14;
  } while ((int)puVar4 < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x4));
  LS_W(((char *)(unsigned)&gm_IncomeHistory), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x44), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x88), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0xcc), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x110), 0x44)
  puVar3 = ((char *)((unsigned)&ai_Grid + 0x2));
  do {
    iVar1 = 0;
    puVar5 = puVar3;
    do {
      LS_W(puVar5 - 2, 2)
      LS_W(puVar5, 2)
      LS_W(puVar5 + 2, 1)
      LS_W(puVar5 + 6, 4)
      iVar1 = iVar1 + 1;
      puVar5 = puVar5 + 0x18;
    } while (iVar1 < 8);
    puVar3 = puVar3 + 0xc0;
  } while ((int)puVar3 < ((unsigned)((unsigned)ai_Grid + 0x600) + 0x2));
  return 1;
}
/* 0x51bdb0, the save game step that writes the avatar table (gv_WriteAvatars) */


int ls_WriteAvatars(void *f)
{
    return gv_WriteAvatars(f);
}
/* 0x51bdc0, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 762: the draft copies Ghidra element indices as byte offsets - the player table 0x6fab40 has 0x400 records of 0x14c bytes (original add eax,0x14c at 0x51bdde) but the draft writes 0x6fab48 + i*0xa6, 0x6fab50 + i*0x53, 0x6fabb0 + i*0x53, 0x6fabbc[i*0x53], 0x6fabc0 + i*0x53 instead of base+8/0x10/0x70/0x7c/0x80; the 0x100 records at 0x74e1e0 have stride 0xa4 (add eax,0xa4 at 0x51c040; index*4 at 0x51c07c) and the draft steps a char* by 0x29; the 0x100 records at 0x758ca0 have stride 0x50 (add eax,0x50 at 0x51c1bd; fields +8/+0xc/+0x10, 0x40 bytes at +0x10) and the draft steps by 0x14; so the whole function needs the rewrite with corrected bases (the 0x51b340 note: patching addresses alone makes unitdiff worse because every block's register shape changes). Also the prologue: the original runs the first count loop in eax/ecx and pushes ebx/ebp/esi/edi only at 0x51bde8, ours pushes at entry (same shape as 0x51aff0). Q1/Q2/Q3/Q5 open, no evidence for the three record tables' names */

int unk_Unk51bdc0(int f)
{
  int uVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  char *puVar8;
  int *puVar9;
  int uVar10;
  int local_8;
  int local_4;
  uVar1 = f;
  local_4 = 0;
  pcVar2 = ((char *)(unsigned)&he_Events);
  local_8 = 0x400;
  do {
    if (*pcVar2 != '\0') {
      local_4 = local_4 + 1;
    }
    pcVar2 = pcVar2 + 0x14c;
  } while ((int)pcVar2 < (unsigned)((unsigned)he_Events + 0x53000));
  LS_W(&local_4, 4)
  local_8 = 0xa0;
  LS_W(&local_8, 4)
  local_8 = 0;
  do {
    iVar6 = local_8;
    iVar3 = local_8 * 0x14c;
    if ((((char *)(unsigned)&he_Events))[local_8 * 0x14c] != '\0') {
      LS_W(((char *)(unsigned)&he_Events) + iVar3, 1)
      LS_W(((char *)((unsigned)&he_Events + 0x4)) + iVar3, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x8)) + iVar6 * 0xa6, 2)
      LS_W(((char *)((unsigned)&he_Events + 0xc)) + iVar3, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x10)) + iVar6 * 0x53, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x14)) + iVar3, 0x30)
      LS_W(((char *)((unsigned)&he_Events + 0x44)) + iVar3, 0xe)
      LS_W(((char *)((unsigned)&he_Events + 0x52)) + iVar3, 0xe)
      LS_W(((char *)((unsigned)&he_Events + 0x60)) + iVar3, 0xe)
      LS_W(((char *)((unsigned)&he_Events + 0x70)) + iVar6 * 0x53, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x78)) + iVar3, 1)
      iVar4 = 0;
      puVar8 = ((char *)((unsigned)&he_Events + 0x8c)) + iVar3;
      do {
        LS_W(puVar8, 4)
        iVar4 = iVar4 + 1;
        puVar8 = puVar8 + 4;
      } while (iVar4 < 8);
      if (((((char *)((unsigned)&he_Events + 0x7c)))[iVar6 * 0x53] == 0) || ((((char *)((unsigned)&he_Events + 0x80)))[iVar6 * 0x53] == 0)) {
        puVar9 = &f;
        uVar10 = 4;
        f = 0;
      }
      else {
        LS_W(((char *)((unsigned)&he_Events + 0x80)) + iVar6 * 0x53, 4)
        uVar10 = (((char *)((unsigned)&he_Events + 0x80)))[iVar6 * 0x53];
        puVar9 = (int *)(((char *)((unsigned)&he_Events + 0x7c)))[iVar6 * 0x53];
      }
      iVar6 = vfs_fwrite(puVar9,uVar10,1,uVar1);
      if (iVar6 == 0) {
        return 0;
      }
      LS_W(((char *)((unsigned)&he_Events + 0xac)) + iVar3, 0xa0)
    }
    local_8 = local_8 + 1;
    if (0x3ff < local_8) {
      local_4 = 0;
      piVar7 = ((char *)((unsigned)&he_QuickList + 0x6a0));
      local_8 = 0x100;
      do {
        if (*piVar7 != 0) {
          local_4 = local_4 + 1;
        }
        piVar7 = piVar7 + 0x29;
      } while ((int)piVar7 < ((unsigned)&he_QuickList + 0xaaa0));
      LS_W(&local_4, 4)
      local_8 = 0;
      do {
        if ((((char *)((unsigned)&he_QuickList + 0x6a0)))[local_8 * 0x29] != 0) {
          LS_W((((char *)((unsigned)&he_QuickList + 0x6a0)))[local_8 * 0x29] + 4, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6a4)) + local_8 * 0x29, 1)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6a8)) + local_8 * 0x29, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6ac)) + local_8 * 0x29, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6b0)) + local_8 * 0x29, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6b4)) + local_8 * 0x29, 0xe)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6c2)) + local_8 * 0xa4, 0x80)
        }
        local_8 = local_8 + 1;
        if (0xff < local_8) {
          local_4 = 0;
          piVar7 = ((char *)((unsigned)&he_RunHandlers + 0x6ac));
          local_8 = 0x100;
          do {
            if (((piVar7[-1] != 0) || (*piVar7 != 0)) || ((char)piVar7[1] != '\0')) {
              local_4 = local_4 + 1;
            }
            piVar7 = piVar7 + 0x14;
          } while ((int)piVar7 < ((unsigned)((unsigned)he_RunHandlers + 0x56a0) + 0xc));
          iVar3 = vfs_fwrite(&local_4,4,1,uVar1);
          if (iVar3 != 0) {
            local_8 = 0;
            while (((((((char *)((unsigned)&he_RunHandlers + 0x6a8)))[local_8 * 0x14] == 0 && ((((char *)((unsigned)&he_RunHandlers + 0x6ac)))[local_8 * 0x14] == 0)
                     ) && ((((char *)((unsigned)&he_RunHandlers + 0x6b0)))[local_8 * 0x50] == '\0')) ||
                   (((iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6a0)) + local_8 * 0x14,1,1,uVar1), iVar3 != 0 &&
                     (iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6a4)) + local_8 * 0x14,4,1,uVar1), iVar3 != 0))
                    && ((iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6a8)) + local_8 * 0x14,4,1,uVar1), iVar3 != 0
                        && ((iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6ac)) + local_8 * 0x14,4,1,uVar1),
                            iVar3 != 0 &&
                            (iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6b0)) + local_8 * 0x50,0x40,1,uVar1),
                            iVar3 != 0))))))))) {
              local_8 = local_8 + 1;
              if (0xff < local_8) {
                return 1;
              }
            }
          }
          return 0;
        }
      } while( 1 );
    }
  } while( 1 );
}
/* 0x51c2f0, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 116, both are VC6 register spellings, not meaning: the count loop at 0x51c2f9 keeps the counter in ecx and stores it once at 0x51c31e (push ecx frame), ours keeps it in the stack slot (inc DWORD PTR [esp]); the record loop at 0x51c334 keeps the loop pointer in esi at 0x925534 (record base +0x14, accesses [esi-0x14]/[esi-0xc]/[esi]/[esi+0x12]/[esi+0x1c]/[esi+0x20]/[esi+0x60]/[esi+0x68]/[esi+4]/[esi-8]), ours bases esi at 0x925520 with shifted offsets, same addresses. Read the whole function: stride 0x114 (add eax,0x114), 96 records, field order/sizes/args match the original; tried char* byte offsets, int* element offsets, for and do-while spellings, VC6 canonicalises to the lowest base and spills the counter either way. Readability Q1/Q2/Q3/Q5 open (the 0x925520 table is unnamed) */


int unk_Unk51c2f0(int f)
{
  char *piVar1;
  int iVar2;
  char *puVar3;
  int local_4;
  local_4 = 0;
  piVar1 = ((char *)(unsigned)&ai_Unk925520);
  do {
    if (*(int *)piVar1 != -1) {
      local_4 = local_4 + 1;
    }
    piVar1 = piVar1 + 0x114;
  } while ((int)piVar1 < (unsigned)((unsigned)ai_Unk925520 + 0x6780));
  LS_W(&local_4, 4)
  puVar3 = ((char *)((unsigned)&ai_Unk925520 + 0x14));
  do {
    if (*(int *)(puVar3 - 0x14) != -1) {
      LS_W(puVar3 - 0x14, 4)
      LS_W(puVar3 - 0xc, 1)
      LS_W(puVar3, 4)
      LS_W(puVar3 + 0x12, 1)
      LS_W(puVar3 + 0x1c, 1)
      iVar2 = vfs_fwrite(puVar3 + 0x20,4,0x10,f);
      if (iVar2 == 0) {
        return 0;
      }
      LS_W(puVar3 + 0x60, 1)
      LS_W(puVar3 + 0x68, 0x98)
      LS_W(puVar3 + 4, 0xe)
      LS_W(puVar3 - 8, 4)
    }
    puVar3 = puVar3 + 0x114;
    if (((unsigned)&ai_Unk925520 + 0x6793) < (int)puVar3) {
      return 1;
    }
  } while( 1 );
}
/* 0x51c450, saves the history (hst.c) part of a save game: the texts-on byte hst_TextsOn, the offsets of the
   current text (hst_TextCurrent - hst_TextFirst) and of the next key-list entry (hst_KeyNext - hst_KeyFirst),
   the 0x10 bytes of the key hst_Key and the four 0x44-byte lists hst_Lists (used flag and eight id/value
   pairs each). Returns 1, or 0 when a write fails. */


int ls_WriteHistory(int f)
{
  int n;
  int i;
  int *list;
  int *entry;

  LS_W(&hst_TextsOn, 1)
  n = hst_TextCurrent - hst_TextFirst;
  LS_W(&n, 4)
  n = hst_KeyNext - hst_KeyFirst;
  LS_W(&n, 4)
  for (i = 0; i < 0x10; i++) {
    LS_W(hst_Key + i, 1)
  }
  /* list walks the lists by their e[0].v word (the word at +8 of each 0x44-byte record): list - 2 is the used
     flag, list - 1 and list are the id and value of each of the eight entries */
  for (list = &hst_Lists[0].e[0].v; (int)list < (int)&hst_Lists[4].e[0].v; list = list + 0x11) {
    LS_W(list - 2, 4)
    entry = list;
    for (i = 0; i < 8; i++) {
      LS_W(entry - 1, 4)
      LS_W(entry, 4)
      entry = entry + 2;
    }
  }
  return 1;
}
/* 0x51c560, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer */


int unk_Unk51c560(int f)
{
  int iVar1;
  int *piVar2;
  int *puVar3;
  char *puVar4;
  int *puVar5;
  unsigned local_a0;
  int local_9c [36];
  unsigned local_b;
  unsigned local_7;
  piVar2 = ((char *)((unsigned)&ai_Unk9367e0 + 0x91));
  do {
    puVar3 = (int *)((int)piVar2 + -0x91);
    puVar5 = local_9c;
    for (iVar1 = 0x26; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    iVar1 = *piVar2;
    *(char *)puVar5 = *(char *)puVar3;
    if (iVar1 == 0) {
      local_b = 0xffffffff;
    }
    else {
      local_b = (iVar1 - 0x9367e0U) / 0x99;
    }
    if (piVar2[1] == 0) {
      local_a0 = 0xffffffff;
    }
    else {
      local_a0 = (piVar2[1] - 0x9367e0U) / 0x99;
    }
    local_7 = local_a0;
    LS_W(local_9c, 0x99)
    piVar2 = (int *)((int)piVar2 + 0x99);
  } while ((int)piVar2 < ((unsigned)&ai_Unka68800 + 0x71));
  if ((*(int *)(unsigned)&ai_Unka68d24) == 0) {
    local_a0 = 0xffffffff;
  }
  else {
    local_a0 = ((*(int *)(unsigned)&ai_Unka68d24) - 0x9367e0U) / 0x99;
  }
  iVar1 = vfs_fwrite(&local_a0,4,1,f);
  if (iVar1 != 0) {
    if ((*(int *)(unsigned)&ai_Unka68c00) == 0) {
      local_a0 = 0xffffffff;
    }
    else {
      local_a0 = ((*(int *)(unsigned)&ai_Unka68c00) - 0x9367e0U) / 0x99;
    }
    LS_W(&local_a0, 4)
    piVar2 = ((char *)((unsigned)&ai_Unka69140 + 0x91));
    do {
      puVar3 = (int *)((int)piVar2 + -0x91);
      puVar5 = local_9c;
      for (iVar1 = 0x26; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      iVar1 = *piVar2;
      *(char *)puVar5 = *(char *)puVar3;
      if (iVar1 == 0) {
        local_b = 0xffffffff;
      }
      else {
        local_b = (iVar1 - 0xa69140U) / 0x99;
      }
      if (piVar2[1] == 0) {
        local_a0 = 0xffffffff;
      }
      else {
        local_a0 = (piVar2[1] - 0xa69140U) / 0x99;
      }
      local_7 = local_a0;
      LS_W(local_9c, 0x99)
      piVar2 = (int *)((int)piVar2 + 0x99);
    } while ((int)piVar2 < ((unsigned)&ls_RefTargets + 0x6b));
    if ((*(int *)(unsigned)&cm_SendQueue) == 0) {
      local_a0 = 0xffffffff;
    }
    else {
      local_a0 = ((*(int *)(unsigned)&cm_SendQueue) - 0xa69140U) / 0x99;
    }
    LS_W(&local_a0, 4)
    LS_W(((char *)(unsigned)&ai_Unkf81168), 4)
    puVar4 = ((char *)(unsigned)&ai_Unkf31160);
    while (iVar1 = vfs_fwrite(puVar4,10,1,f), iVar1 != 0) {
      puVar4 = puVar4 + 10;
      if (((unsigned)&ai_Unkf31160 + 0x4ffff) < (int)puVar4) {
        LS_W(((char *)(unsigned)&ai_Unkf8121c), 4)
        LS_W(((char *)(unsigned)&ai_Unka68c04), 4)
        LS_W(((char *)(unsigned)&ai_Unk60bd88), 4)
        iVar1 = vfs_fwrite(((char *)(unsigned)&ai_Unk60bd8c),4,1,f);
        if (iVar1 != 0) {
          iVar1 = vfs_fwrite(((char *)(unsigned)&ai_Unk60bd90),4,1,f);
          return iVar1 != 0;
        }
        return 0;
      }
    }
  }
  return 0;
}
/* 0x51c870, the table 0x7604e8 (three words per entry at +0x18 words apart; the last entry is followed by the two further blocks 0x5dbfc0 and 0x4ed480) @deep B3 code hunks 50 after the LS_W restructure (the draft mixed if(fwrite!=0) and direct calls; the original is the LS_W macro for all five writes): the rest is VC6's induction-variable base - the original keeps esi at the entry pointer 0x7604e8 (lea [esi-0x60]/[esi]/[esi+0x68]/[esi+0xc8], add esi,4, cmp 0x760548) while ours bases esi at 0x760550 (the +0x68 word) with offsets -0xc8/-0x68/0/+0x60, same addresses, and VC6 then merges the loop's return-0 blocks to the function end where the original reuses the first check's inline epilogue at 0x51c88c. Read the whole function (24 entries of four words, then 0x5dbfc0 and 0x4ed480); tried do-while and while spellings. Q1/Q2/Q3/Q5 open (0x7604e8 table unnamed, callees ai_SaveWind/unk_Unk4ed480 undeclared here) */


int unk_Unk51c870(int f)
{
    int *p;

    LS_W((char *)(unsigned)&ai_Unk760610, 4)
    p = (int *)(unsigned)&full_data_7604e8;
    do {
        LS_W(p - 0x18, 4)
        LS_W(p, 4)
        LS_W(p + 0x1a, 4)
        LS_W(p + 0x32, 4)
        p = p + 1;
    } while ((int)p < (unsigned)((unsigned)full_data_7604e8 + 0x60));
    if (ai_SaveWind(f) != 0)
        return unk_Unk4ed480(f) != 0;
    return 0;
}
/* 0x51c920, reads count items of size bytes into buf (vfs_fread, 0x430bd0, the counterpart of vfs_fwrite):
   true when the read returned a positive count */


int ls_Read(void *buf, int size, int count, int f)
{
    unsigned got = vfs_fread(buf, size, count, f);

    return got > 0;
}


short ai_Unk75e13c = 0;  /* 0x75e13c, its 2-byte last field ( */
LSSAVE ls_SavePath = {0};  /* 0x75e640 */
unsigned short ai_ScreenImage[19200] = {0};  /* 0x766320, the */
int ai_Unk76f920[1] = {0};  /* 0x76f920, the header of the loaded save game (id, flag byte, name, ..) */
/* 0x51c950, loads a save game: opens the file (0x430470, mode 0x5f1084), resets the game (0x503e80, 0x40b240, 0x50d490), reads the header (0x51cee0, versions 0x10026 .. 0x1005a only) and the fixed globals (each from its version: 0x6f1bcc from 0x10022, 0x61a658 from 0x10030, 0x6f1b88 from 0x1003d), then the readers of the subsystems in the order 0x51d3d0, 0x51d550, 0x51db00, 0x51de70, 0x51f080, 0x4ea7a0 (from 0x10045), 0x51d2d0, [unless the header flag +4 has bit 2 (a map only save): 0x4ea7a0, 0x4f2b30, 0x51f650 (from 0x10016), 0x51f720, 0x4ed270 (from 0x1002a), 0x51fb90, 0x520000, 0x520200, 0x520310, 0x5205f0 (from 0x1002e), 0x5206b0, then the references (0x511790 / 0x511840 / 0x51e900), the character pointers 0x6f1bfc / 0x6f1c00 (0x505df0), 0x4b63b0, 0x589b30 (from 0x10029), 0x5a8a20 (from 0x10047), 0x4ec140 (from 0x10049), 0x4ada80 (from 0x10052), 0x4c0040 (from 0x10053)]. The first one that fails closes the file, frees the buffer 0xf31140 and returns 0. At the end 0x528770, 0x511790, 0x513740, 0x513590(0x633738, 1) and the path 0x76f9bc is kept in 0x75e640. Returns 1. ROUGH */
int ai_Unk76f924 = 0;  /* 0x76f924 (the flag byte of the header) */
int ai_Unk76f9b8 = 0;  /* 0x76f9b8 */
char ai_Unk76f9bc[100] = {0};  /* 0x76f9bc */
extern char * ai_Unkf31140;                /* 0xf31140 */
extern int ai_Unk75e134;  /* 0x75e134, the clock copy */
extern int ai_Unk75e130;  /* 0x75e130 */
extern int ai_Unk75e138;  /* 0x75e138 */
#define LS_RD(p, sz) ls_Read((int)(p), (sz), 1, h)
/* 0x51c950 */
int unk_Unk51c950(int name, int kind)
{
    int h, v, i;

    h = vfs_Tfopen(name, "rb", vfs_Root);
    if (h == 0)
        return 0;
    gm_ResetCharacters();
    ch_DeleteAll();
    ai_PlayerCharC = 0;
    ai_JudgeCatholic = 0;
    ai_JudgeProtestant = 0;
    init_FreeScenes();
    if (ls_ReadSaveHeader(h, ai_Unk76f920) == 0 || ai_Unk76f920[0] > 0x1005a || ai_Unk76f920[0] < 0x10026
        || !LS_RD(&ai_NextObjectId, 4) || !LS_RD(&ai_Unk6f2618, 4) || !LS_RD(ai_Time, 0xe)
        || !LS_RD(&ai_OwnPlayer, 1) || !LS_RD(&ai_PlayerCharC, 4) || !LS_RD(&ai_Unk6f1bb8, 4)
        || !LS_RD(&ai_Unk6f1bbc, 4) || !LS_RD(&ai_CharCount, 4)) {
        vfs_fclose(h);
        return 0;
    }
    v = ls_Version;
    if (v > 0x10021)
        LS_RD(&ai_Unk6f1bcc, 4);
    LS_RD(ai_UpdateStart, 0x18);
    ai_Unk75e134 = *(int *)(ai_Time + 4);
    ai_Unk75e130 = *(int *)ai_Time;
    ai_Unk75e138 = *(int *)(ai_Time + 8);
    ai_Unk75e13c = *(short *)(ai_Time + 0xc);
    if ((ai_Unk76f924 & 2) == 0)
        ai_Difficulty = ai_Unk76f9b8;
    if (v > 0x1002f)
        LS_RD(&ai_Unk61a658, 4);
    if (v < 0x1003d)
        ai_Unk6f1b88 = 1000000;
    else
        LS_RD(&ai_Unk6f1b88, 4);
    if (ls_ReadObjects(h) == 0 || ls_ReadBuildings(h) == 0 || ls_ReadPlayerRecords(h) == 0 || ls_ReadCharacters(h) == 0
        || unk_Unk51f080(h) == 0 || (v >= 0x10045 && amt_LoadAemter(h, v) == 0) || unk_Unk51d2d0(h) == 0) {
        vfs_fclose(h);
        return 0;
    }
    if ((ai_Unk76f924 & 2) != 0) {
        vfs_fclose(h);
        ai_JudgeCatholic = 0;
        ai_JudgeProtestant = 0;
        return ls_FixupAfterLoad() != 0;
    }
    if (!((v > 0x10044 || amt_LoadAemter(h, v) != 0) && unk_Unk4f2b30(h) != 0 && (v < 0x10016 || ls_ReadRelations(h) != 0)
          && unk_Unk51f720(h) != 0 && (v < 0x1002a || gv_ReadAvatars((void *)h) != 0) && unk_Unk51fb90(h) != 0
          && unk_Unk520000(h) != 0 && unk_Unk520200(h) != 0)) {
        vfs_fclose(h);
        return 0;
    }
    if (unk_Unk520310(h) == 0) {
        vfs_fclose(h);
        ls_FixupAfterLoad();
        return 0;
    }
    if (!((v < 0x1002e || unk_Unk5205f0(h) != 0) && ls_FixupAfterLoad() != 0)) {
        vfs_fclose(h);
        return 0;
    }
    d3if_SelectScene(0, 1);
    unk_Unk511790();
    unk_Unk511840();
    if (unk_Unk51e900(h) == 0)
        goto fail;
    ai_JudgeCatholic = gm_FindChar(ai_JudgeCatholic);
    ai_JudgeProtestant = gm_FindChar(ai_JudgeProtestant);
    if (ai_Unk4b63b0(h) == 0)
        goto fail;
    if (!(v < 0x10029 || ai_Unk589b30(h) != 0))
        goto fail;
    if (v > 0x10046 && ai_Unk5a8a20(h) == 0)
        goto fail;
    if (v > 0x10048 && ahm_LoadWalk(h) == 0)
        goto fail;
    if (!(v < 0x10052 || hmt_Load(h, v) != 0))
        goto fail;
    if (v > 0x10052 && rt_LoadRoutes(h) == 0)
        goto fail;
    vfs_fclose(h);
    sv_Unk528770();
    unk_Unk511790();
    unk_Unk513740();
    init_SetDetailLevel((int)ai_Unk633738, 1);
    i = 0;
    do {
        ls_SavePath.path[i] = ai_Unk76f9bc[i];
    } while (ai_Unk76f9bc[i++] != 0);
    return 1;
fail:
    vfs_fclose(h);
    mem_Free(ai_Unkf31140);
    ai_Unkf31140 = 0;
    return 0;
}
/* 0x51cee0, the header of a save game: the id (kept in 0x623650, the version), the flag byte, the name, two words, the language and further bytes (each from the version noted), the 8 words of the party table, the screen shot (0xe100 bytes, converted back to the 16 bit bitmap 0x766320 with 0x41e670), the mode word (+0x98, 2 for versions below 0x10038) and the title (+0x9c, "Savegame" below 0x10039). Returns 1. ROUGH */


int ls_ReadSaveHeader(int f, int *h)
{
    char *hb = (char *)h;
    char *shot;
    int i, j, k = 0;
    unsigned char *px;

    shot = (char *)mem_Alloc(0xe100, "ls:tmp");
    LS_R(h, 4)
    ls_Version = *h;
    LS_R(hb + 4, 1)
    LS_R(hb + 5, 0x20)
    LS_R(hb + 0x28, 8)
    LS_R(hb + 0x30, 1)
    if (ls_Version >= 0x10033)
        LS_R(hb + 0x31, 1)
    if (ls_Version >= 0x10028) {
        LS_R(hb + 0x32, 0xe)
        LS_R(hb + 0x40, 0x10)
        if (ls_Version >= 0x10054) {
            LS_R(hb + 0x50, 0x10)
            LS_R(hb + 99, 1)
            LS_R(hb + 0x64, 4)
        }
        LS_R(hb + 0x60, 1)
        LS_R(hb + 0x61, 1)
        LS_R(hb + 0x62, 1)
        LS_R(hb + 0x68, 4)
        LS_R(hb + 0x6c, 1)
        LS_R(hb + 0x6d, 1)
        if (ls_Version < 0x1002b) {
            *(int *)(hb + 0x70) = -1;
            *(int *)(hb + 0x74) = -1;
        } else {
            LS_R(hb + 0x70, 4)
            LS_R(hb + 0x74, 4)
        }
        if (ls_Version >= 0x10034) {
            for (i = 0; i < 8; i++)
                LS_R(hb + 0x78 + i * 4, 4)
        }
        LS_R(shot, 0xe100)
        px = (unsigned char *)shot;
        for (j = 0x78; j != 0; j--) {
            for (i = 0xa0; i != 0; i--) {
                ((unsigned short *)(unsigned)&ai_ScreenImage)[k] = dd_RGB(px[0], px[1], px[2]);
                px += 3;
                k++;
            }
        }
    }
    if (shot != 0)
        mem_Free(shot);
    if (ls_Version >= 0x10025) {
        if (ls_Version < 0x10038)
            *(int *)(hb + 0x98) = 2;
        else
            LS_R(hb + 0x98, 4)
        if (ls_Version < 0x10039)
            strcpy(hb + 0x9c, "Savegame");
        else
            LS_R(hb + 0x9c, 0x60)
        return 1;
    }
    return 0;
}
/* 0x51d2d0, after a load: builds the scene (0x44be60 with the loader 0x537b60), the scene tree (0x45eeb0, size 0x100, 0x80 when the scene kind is 0x40), sets the detail bits (+0x71c, mask 0x1c, from 0x61adf8) and registers the two scene callbacks 0x51d4f0 (0x1c0) / 0x51d530 (0xc0) with 0x438e60. Returns 1 @deep B3 code hunks 52: same operations, VC6 scheduling/register shape - the scene size (0x100, 0x80 when the kind is 0x40) is materialised in eax and pushed once where the original duplicates push 2 per branch and pushes the constant at each site (0x51d323/0x51d32c); ours saves esi (the 'saved' local 0x633444) before the 0x45eeb0 call (push esi at 0x51d32d) and cleans 0x14 bytes later, the original pushes esi after the call (0x51d341) with the 45eeb0 args cleaned at 0x51d339 and mov ds:0xfd7c50,eax before the push; the +0x1c70 byte xor is only a register choice (eax/cl vs ecx/al). Read the whole function; tried the size select as an if/else and as an inline ternary, VC6 emits the same. Q1/Q2/Q3/Q5 open (0x537b60/0x51d4f0/0x51d530 function pointers, 0x71c field) */


int unk_Unk51d2d0(int f)
{
    int saved, size;
    unsigned char *b;

    d3if_SelectScene(1, 0);
    d3if_ClearScene(0);
    d3if_SelectScene(0, 0);
    d3_LoadScene(0, f, (void *)(unsigned)&main_Unk537b60);
    d3if_SelectScene(1, 0);
    d3if_SelectScene(0, 0);
    *(int *)ai_Unkfd7c50 = d3sm_Create(ai_Unk633738,
                                (*ai_Unk633738 == 0x80 || *ai_Unk633738 != 0x40) ? 0x100 : 0x80, 2);
    saved = d3_CurrentScene;
    d3if_SelectScene(0, 0);
    if (ai_Unk633738 != 0) {
        b = (unsigned char *)ai_Unk633738 + 0x71c * 4;
        *b = ((init_Detail << 2 ^ *b) & 0x1c) ^ *b;
        d3fl_SetDetail(0x80);
    }
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0x1c0, (void *)(unsigned)&ls_HookBuildingCB);
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, (void *)(unsigned)&ls_ClearObjectFlag);
    d3if_SelectScene(saved, 0);
    return 1;
}
/* 0x51d3d0, reads the object list 0x7e3f34 (after 0x4fe830 cleared it): the count, then each record in the pieces of the writer 0x519bf0; the count is kept in 0x6f1bdc. Returns 1 */


int ls_ReadObjects(int f)
{
    int n, i;
    char *r;

    gm_ClearObjects();
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        r = (char *)ai_Objects + i * 0x43;
        LS_R(r, 2)
        LS_R(r + 2, 4)
        LS_R(r + 6, 4)
        LS_R(r + 10, 4)
        LS_R(r + 0xe, 4)
        LS_R(r + 0x12, 1)
        LS_R(r + 0x13, 1)
        LS_R(r + 0x1c, 0x1f)
    }
    f3gm_ObjectCount = n;
    return 1;
}
/* 0x51d4f0, scene callback: runs init_HookObject (o, building, 2) for every used record of the 0x100 of ai_Blds
   (0xa9 bytes) whose id (+1) is the owner (+0x200) of the scene object o. Returns 1
   @deep Q4 blocked: the two remaining raw accesses (*(int *)(o + 0x200) the owner id of the scene object, *(int *)(p + 1) the building id) need typed fields - AIBLD from ai.h for the building record and a named D3OBJECT.+0x200 field from d3.h; ls.c cannot include ai.h (30 compile errors: gv.h declares ai_Blds/ai_Chars/gm_BuildQuery/ai_Unk6ffe90/... with different types than ai.h, first C2040 at include/gv.h(12)) nor get AIBLD any other way, and d3.h's D3OBJECT has only char pad200[0x8] there, so the field name is a shared-header decision. Everything else done: byte match kept, Q1/Q2/Q3/Q5/Q6/Q7 pass after init_HookObject and ai_Blds */


char ls_HookBuildingCB(char *o)
{
    int i;
    char *p;

    for (i = 0; i < 0xa900; i += 0xa9) {
        p = ai_Blds + i;
        if (*p != 0 && *(int *)(o + 0x200) == *(int *)(p + 1))
            init_HookObject(o, (unsigned short *)p, 2);
    }
    return 1;
}
/* 0x51d530, scene callback: clears the flag 4 in +0x213 of the object */

char ls_ClearObjectFlag(char *o)
{
    o[0x213] &= ~4;
    return 1;
}
/* 0x51d550, reads the buildings ai_Blds from the save file f (the counterpart of the writer 0x519d20): for each of n records the fields (+0 .. +0x5c), for the type 0x1e the plant map ("f3g:PlantMap", 0x600 bytes of 0x18-byte plants, its pointer at +0x71), for the types 0x48 .. 0x4a / 0x4b .. 0x4d the type specific record at +0x65 (read by gv_ReadGraveyard / ahm_ReadRecord, or built by gv_AllocGraveyard / a new "AHM_INFO" record for the versions below 0x10047 / 0x10049), then the 0x10 flag bytes at +0x99 (for the versions below 0x10043: zero, 0xffffffff at +0xa5, bit 0 set) and the fixed +0x30 = 0x1388 and +0x95 = -1; at the end the building sites ai_Bauplaetze (ai_NBauplaetze records of 0x60 bytes). Returns 1, 0 when a read fails. @deep Q4: the 13 raw accesses need the AIBLD fields at +0x65 / +0x71 / +0x41 / the 0x18-byte plant record (ai.h; ls.c cannot include ai.h: gv.h declares ai_Blds/ai_Chars/... with other types, the same type debt as 0x51d4f0). B: the meaning is verified instruction by instruction (all hunks read against 0x51d550..0x51daff); the remaining code hunks are source shape: the off=0 / k=0 preheader init and the k / off stack slots ([esp+0x10] / [esp+0x20] swapped), the K/L/M and H/I/J version branches laid out with the old-version arm inline where the original puts the read inline, and the i / j register pair of the building-site loop. The plant map loop now re-reads the pointer field (+0x71) every read like the original (1200 -> 935 differing bytes). */

int ls_ReadBuildings(int f)
{
    int n, k, off = 0, i, r;
    char *p;
    char *q;
    int *pi;
    unsigned *flags;
    int dummy;

    gm_ClearBuildings();
    LS_R(&n, 4)
    for (k = 0; k < n; k++) {
        p = ai_Blds + off;
        LS_R(p, 1)
        LS_R(p + 1, 4)
        LS_R(p + 5, 0x20)
        LS_R(p + 0x25, 2)
        LS_R(p + 0x27, 2)
        LS_R(p + 0x29, 2)
        LS_R(p + 0x2b, 4)
        LS_R(p + 0x2f, 1)
        LS_R(p + 0x34, 1)
        LS_R(p + 0x35, 4)
        LS_R(p + 0x39, 4)
        LS_R(p + 0x3d, 4)
        pi = (int *)(p + 0x41);
        LS_R(pi, 4)
        if (*pi > 4)
            *pi = 2;
        LS_R(p + 0x45, 4)
        if (ls_Version >= 0x10028)
            LS_R(p + 0x49, 4)
        if (ls_Version < 0x10032)
            LS_R(&dummy, 4)
        if (ls_Version >= 0x10057) {
            LS_R(p + 0x4d, 4)
            LS_R(p + 0x51, 4)
            LS_R(p + 0x55, 4)
        }
        LS_R(p + 0x5a, 2)
        LS_R(p + 0x5c, 1)
        if (*p == 0x1e) {
            *(char **)(p + 0x71) = mem_Alloc(0x600, "f3g:PlantMap");
            for (i = 0; i < 0x600; i += 0x18) {
                LS_R(*(char **)(p + 0x71) + i, 4)
                LS_R(*(char **)(p + 0x71) + i + 4, 4)
                LS_R(*(char **)(p + 0x71) + i + 8, 1)
                LS_R(*(char **)(p + 0x71) + i + 9, 1)
                LS_R(*(char **)(p + 0x71) + i + 10, 2)
                LS_R(*(char **)(p + 0x71) + i + 0x10, 4)
                LS_R(*(char **)(p + 0x71) + i + 0xc, 1)
                LS_R(*(char **)(p + 0x71) + i + 0xd, 1)
                *(int *)(*(char **)(p + 0x71) + i + 0x14) = 0;
            }
        } else {
            if (*p == 'H' || *p == 'I' || *p == 'J') {
                if (ls_Version < 0x10047) {
                    LS_R(p + 0x65, 0x30)
                    *(int *)(p + 0x65) = (int)gv_AllocGraveyard(p);
                    goto tail;
                }
                r = gv_ReadGraveyard(f, p);
            } else if (*p == 'K' || *p == 'L' || *p == 'M') {
                if (ls_Version < 0x10049) {
                    int *a;

                    LS_R(p + 0x65, 0x30)
                    a = (int *)mem_Alloc(0x3c, "AHM_INFO");
                    *(int **)(p + 0x65) = a;
                    *a = -1;
                    goto tail;
                }
                r = ahm_ReadRecord(f, p);
            } else {
                r = ls_Read((int)(p + 0x65), 0x30, 1, f);
            }
            if (r == 0)
                return 0;
        }
tail:
        flags = (unsigned *)(p + 0x99);
        if (ls_Version < 0x10043) {
            memset(p + 0x99, 0, 0x10);
            p[0xa5] = -1;
            p[0xa6] = -1;
            p[0xa7] = -1;
            p[0xa8] = -1;
            *flags |= 1;
        } else {
            LS_R(flags, 0x10)
        }
        p[0x30] = 0x88;
        p[0x31] = 0x13;
        p[0x32] = 0;
        p[0x33] = 0;
        p[0x95] = -1;
        p[0x96] = -1;
        p[0x97] = -1;
        p[0x98] = -1;
        off += 0xa9;
    }
    LS_R(&ai_NBauplaetze, 4)
    ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, ai_NBauplaetze * 0x60);
    for (i = 0; i < ai_NBauplaetze; i++) {
        LS_R(ai_Bauplaetze[i].name, 0x40)
        LS_R(&ai_Bauplaetze[i].x0, 0x10)
        LS_R(&ai_Bauplaetze[i].x1, 0x10)
    }
    return 1;
}
/* 0x51db00, reads the 16 player records gm_PlayerRecords (0xac bytes each; the counterpart of the writer 0x51a1a0): the fields, each version gated (+0x26 from 0x1002c, +0x52 .. +0x6a from 0x10014, +0x5e = 1.0 below 0x10056, the 0x11 (0x0e below 0x10046) name bytes at +0x72 from 0x10015, +0x6e from 0x10018). Returns 1, 0 when a read fails. @deep byte + readable blocked on two Q rules. Q3: the loop bound 0x7d6a42 (the end of gm_PlayerRecords in the +2 byte view, = 0x7d5f80 + 16 * 0xac + 2) is an image literal; spelling it as an extern at that address (gm_PlayerRecordsEnd, tried) changes the codegen to 84 differing asm lines instead of 0, and (int)gm_PlayerRecordBytes + 0xac0 doubles the frame - the literal form is byte-identical (the do-while over the pointer is what the original does: add esi,0xac; cmp esi,0x7d6a42; jl). Q4: the one raw access left is *(int *)(p + 0x5e) = 0x3f800000 (the 1.0 float default for the versions below 0x10056) - it needs the 0xac-byte player-record fields, ai.h blocked in ls.c (gv.h type debt). Everything else fixed: v removed so every version check reads ls_Version directly (287 instructions became equal), the '> 0x1002b/0x10013/0x10014/0x10017/0x10045' bounds written as the original's '>= 0x1002c/0x10014/0x10015/0x10018/0x10046', the 0x5e branch laid out with the read inline. */


int ls_ReadPlayerRecords(int f)
{
    char *p = gm_PlayerRecordBytes;
    int i, n;

    do {
        LS_R(p - 2, 2)
        LS_R(p, 0x10)
        LS_R(p + 0x12, 4)
        if (ls_Version >= 0x1002c)
            LS_R(p + 0x26, 4)
        LS_R(p + 0x1a, 4)
        LS_R(p + 0x1e, 4)
        LS_R(p + 0x2a, 4)
        LS_R(p + 0x2e, 4)
        LS_R(p + 0x32, 4)
        LS_R(p + 0x36, 4)
        LS_R(p + 0x3e, 4)
        LS_R(p + 0x42, 4)
        LS_R(p + 0x46, 4)
        LS_R(p + 0x4a, 4)
        LS_R(p + 0x4e, 4)
        if (ls_Version >= 0x10014) {
            LS_R(p + 0x52, 4)
            LS_R(p + 0x56, 4)
            LS_R(p + 0x5a, 4)
            LS_R(p + 0x3a, 4)
            LS_R(p + 0x62, 4)
            LS_R(p + 0x66, 4)
            LS_R(p + 0x6a, 4)
        }
        if (ls_Version >= 0x10056) {
            LS_R(p + 0x5e, 4)
        } else {
            *(int *)(p + 0x5e) = 0x3f800000;
        }
        if (ls_Version >= 0x10015) {
            n = (((ls_Version >= 0x10046) - 1) & 0xfffffffd) + 0x11;
            for (i = 0; i < n; i++)
                LS_R(p + i + 0x72, 1)
        }
        LS_R(p + 0x86, 4)
        LS_R(p + 0x8a, 0x10)
        LS_R(p + 0x9a, 0xc)
        LS_R(p + 0xa6, 4)
        if (ls_Version >= 0x10018)
            LS_R(p + 0x6e, 4)
        p += 0xac;
    } while ((int)p < (int)((char *)gm_PlayerRecords + 0xac2));  /* one past the 16th record (gm_PlayerRecordBytes + 0xac0; a byte offset, the array is int[]); the original's literal 0x7d6a42 is an absolute address that does not hold in the linked image, where it made the loop read ~70 records and the .cty load fail */
    return 1;
}
/* 0x51de70, reads the characters (the counterpart of the header writer 0x5194c0 and the character writer 0x51a4b0): ai_PlayerId, ai_CharCount, the two judge pointers ai_JudgeCatholic / ai_JudgeProtestant and the eight slots of ai_KindBChars (from 0x10016), then ai_CharCount records of the 0x21c bytes of ai_Chars (the id word first, it picks the record): each field from its version (the reference fields +0x54 / +0x18c get their base 0x59a / 0x639 added, +0x174 is 2 instead of 4 below 0x10020), some fields with their defaults for older versions (+0x20c = -1 and +0x190 = 4 below 0x1003e, +0x1cc = 0 below 0x10049); the last chunk +0x210 .. +0x218 from 0x10036. Returns 1, 0 when a read fails. @deep Q4: the 8 raw accesses need the AICHAR fields of ai_Chars (+0x54 / +0x18c the reference fields, +0x20c, +0x190, +0x1cc the defaulted ones, +0x16c / +0x170 / +0x17c zeroed then read); ai.h cannot be included in ls.c (gv.h declares ai_Blds/ai_Chars/... with other types, type debt). B: 467 code hunks = the Ghidra conversion's shape, not a meaning fix: read against 0x51de70..0x51e74f - the order, offsets, sizes and version gates match the disassembly (ai_PlayerId 2, count 4, the two judges 4, the 8 slots from 0x10016, then per record id 2 -> ai_Chars[id & 0xffff] and the field list +2 .. +0x218 with its gates 0x1003e / 0x10030 / 0x10023 / 0x10020 / 0x10029 / 0x10035 / 0x10049 and the +0x59a / +0x639 bases), so this is the rewrite that 0x51a4b0 has as @deep, not a base reading fix. Q1/Q2/Q3/Q6 done: ls_ReadCharacters, ai_CharCount / ai_JudgeCatholic / ai_JudgeProtestant / ai_KindBChars named project-wide, the 0x6f1c04 loop by symbol. */


int ls_ReadCharacters(int f)
{
    int *ref;               /* the reference field read (+0x54 / +0x18c): its saved index plus the base */
    int v, ok;
    unsigned idx;
    int *slots;
    int sz, k, cnt;
    unsigned id;            /* the id word read at the start of a record */
    int word;
    char *c;

  LS_R(&ai_PlayerId, 2)
  LS_R(&cnt, 4)
  ai_CharCount = cnt;
  LS_R(&ai_JudgeCatholic, 4)
  LS_R(&ai_JudgeProtestant, 4)
  if (0x10016 < ls_Version) {
    slots = ai_KindBChars;
    do {
      LS_R(slots, 4)
      slots = slots + 1;
    } while (slots < ai_KindBChars + 8);
  }
  k = 0;
  if (0 < cnt) {
    do {
      ok = ls_Read((int)&id, 2, 1, f);
      v = ls_Version;
      if (ok == 0) {
        return 0;
      }
      idx = id & 0xffff;
      c = ai_Chars[idx];
      *(short *)c = (short)id;
      if (v < 0x1003e) {
        *(int *)(c + 0x20c) = 0xffffffff;
      }
      else {
        LS_R((c + 0x20c), 4)
      }
      LS_R((c + 0x2), 1)
      LS_R((c + 0x4), 4)
      LS_R((c + 0x8), 1)
      LS_R((c + 0x9), 1)
      LS_R((c + 0xa), 2)
      LS_R((c + 0xc), 1)
      LS_R((c + 0xd), 1)
      LS_R((c + 0x10), 4)
      LS_R((c + 0x14), 4)
      LS_R((c + 0x18), 4)
      LS_R((c + 0x1c), 4)
      LS_R((c + 0x20), 4)
      LS_R((c + 0x24), 4)
      if (0x1003a < ls_Version) {
        LS_R((c + 0x28), 2)
        LS_R((c + 0x2c), 4)
      }
      LS_R((c + 0x30), 0x10)
      if ((0x10030 < ls_Version) &&
         ls_Read((int)((c + 0x40)), 0x10, 1, f) == 0) {
        return 0;
      }
      LS_R((c + 0x50), 2)
      ref = (int *)(c + 0x54);
      LS_R(ref, 4)
      *ref = *ref + 0x59a;
      LS_R((c + 0x58), 1)
      LS_R((c + 0x5c), 0x20)
      if ((0x10023 < ls_Version) &&
         ls_Read((int)((c + 0x7c)), 4, 1, f) == 0) {
        return 0;
      }
      LS_R((c + 0x80), 5)
      LS_R((c + 0x88), 0xa8)
      LS_R((c + 0x164), 1)
      LS_R((c + 0x165), 1)
      LS_R((c + 0x166), 1)
      LS_R((c + 0x167), 1)
      LS_R((c + 0x168), 1)
      LS_R((c + 0x169), 1)
      *(int *)(c + 0x16c) = 0;
      LS_R((c + 0x16c), 4)
      *(int *)(c + 0x170) = 0;
      LS_R((c + 0x170), 4)
      if (ls_Version < 0x10020) {
        sz = 2;
      }
      else {
        sz = 4;
      }
      LS_R((c + 0x174), sz)
      *(int *)(c + 0x17c) = 0;
      LS_R((c + 0x17c), 4)
      LS_R((c + 0x180), 1)
      ref = (int *)(c + 0x18c);
      LS_R(ref, 4)
      *ref = *ref + 0x639;
      LS_R((c + 0x190), 4)
      if (ls_Version < 0x1003e) {
        *(int *)((c + 0x190)) = 4;
      }
      LS_R((c + 0x194), 4)
      LS_R((c + 0x198), 4)
      LS_R((c + 0x19c), 4)
      LS_R((c + 0x1a0), 4)
      LS_R((c + 0x1a4), 4)
      LS_R((c + 0x1a8), 4)
      LS_R((c + 0x1ac), 4)
      LS_R((c + 0x1b0), 1)
      LS_R((c + 0x1b1), 1)
      LS_R((c + 0x1b4), 0x10)
      LS_R((c + 0x1c8), 4)
      if (ls_Version < 0x10049) {
        *(int *)(c + 0x1cc) = 0;
      }
      else {
        LS_R((c + 0x1cc), 4)
      }
      LS_R((c + 0x1d0), 4)
      LS_R((c + 0x1d4), 0x10)
      LS_R((c + 0x1e4), 4)
      LS_R((c + 0x1e8), 4)
      LS_R((c + 0x1ec), 4)
      if ((0x10029 < ls_Version) &&
         ls_Read((int)((c + 0x1f0)), 4, 1, f) == 0) {
        return 0;
      }
      if ((0x10020 < ls_Version) &&
         ls_Read((int)((c + 0x1c5)), 1, 1, f) == 0) {
        return 0;
      }
      LS_R(&word, 4)
      *(int *)(c + 0x184) = word;
      if ((0x10020 < ls_Version) &&
         ls_Read((int)((c + 0x1f4)), 0x18, 1, f) == 0) {
        return 0;
      }
      if (0x10035 < ls_Version) {
        LS_R((c + 0x210), 4)
        LS_R((c + 0x214), 1)
        LS_R((c + 0x215), 1)
        LS_R((c + 0x216), 1)
        LS_R((c + 0x217), 1)
        LS_R((c + 0x218), 1)
      }
      k = k + 1;
    } while (k < cnt);
  }
  return 1;
}
/* 0x51e750, reads one ch.c CHARACTER record from the save (allocated by ch_AllocAt from the id read first): its name (+8, 0x20 bytes), the group / script group (+0x2c / +0x30), the flag word +0x8c (bit 5 set when the flag byte read is 1), the full name +0x170 (0x30) and the body name +0x130 (0x40), +0x38 (0x10), +0x48 (0xc), the texts +0x1a8 (from 0x10013) and +0x1e8 (from 0x1005a), the flags &= 0xdffb, then the reference index read last goes to +0x12c (0x51e900 turns it into a pointer). Returns the record */


int ls_ReadCharacter(int f)
{
    int id, ent, v;
    unsigned short *fl;         /* the flag word at +0x8c */
    unsigned char *flagByte;    /* its low byte (bit 5 = 0x20) */
    int *ref;                   /* the saved reference index at +0x12c */
    char b;

    LS_R(&id, 4)
    ent = (int)ch_AllocAt(id);
    LS_R(ent + 8, 0x20)
    LS_R(ent + 0x2c, 4)
    LS_R(ent + 0x30, 4)
    fl = (unsigned short *)(ent + 0x8c);
    LS_R(fl, 2)
    LS_R(ent + 0x170, 0x30)
    LS_R(ent + 0x130, 0x40)
    LS_R(ent + 0x38, 0x10)
    LS_R(ent + 0x48, 0xc)
    LS_R(&b, 1)
    if (b == 1) {
        flagByte = (unsigned char *)(ent + 0x8c);
        *flagByte |= 0x20;
    }
    LS_R(&id, 4)
    if (ls_Version >= 0x10013)
        LS_R(ent + 0x1a8, 0x40)
    if (ls_Version >= 0x1005a)
        LS_R(ent + 0x1e8, 0x40)
    v = id;
    *fl &= 0xdffb;
    ref = (int *)(ent + 0x12c);
    *ref = v;
    return ent;
}
/* 0x51e900, reads the scene objects and the character scene records of a save game: the two lists of object records (0x51e750; first the ones that point to characters (+0x12c via 0x505df0), then those that point to buildings (0x4ffe90)), then for each scene object (the list 0x1468ae0, 0x300): its universe (0x4478e0), its name "%s_%s" / "holzfaeller_%s" ("hozfaeller" is a misspelling in the data), the model "*%s.bgf" (found with 0x42fca0, else the name of the object's character 0x4fb390 and the name is cut at the last underscore), the placement (0x403070, the owner ids, +0x1a8 / +0x1e8 texts), the object's owner: a character record (0x50d380; "sp_%i"; the karren objects "ob_KARREN_klein / mittel / GROSS" for the kinds 0x134 / 0x135 / 0x136 are added with their colour) or a building (0x43b860 and "sp_%i", the building's owner link +0x1d0 / +0x180); finally the table 0x631050 (the records of 0x194 bytes, the count 0x300 below 0x10044 else 2000 or 0x300: the fields written by 0x51aff0) and its link fixing (invalid links are reported "ls_LoadCharacter(): invalid next or prev-action pointer for action-type:%i" / "invalid stockptr for action-type:%i (character per...)" and the record is cleared). Returns 1. ROUGH* views), Q2 on ~15 unk callees/globals (0x501190, 0x4ffe90, 0x511dc0/0x511e60, 0x4478e0, 0x42fca0, 0x4fb390, 0x403070, 0x406c60, 0x40b530, 0x43b860, 0x4cde20, 0x50d380, 0x446e10, 0x446f30, 0x4034e0, 0x509ff0, 0x402480, 0x404810, 0x505df0, 0x633444, 0x6333fc). */


int unk_Unk51e900(int f)
{
    int n, i, k, saved;
    int *o;
    char *rec, *name, *obj;
    char buf[128], file[260], dummy[260];
    char flag;
    int *t;
    char *own;

    n = 0;
    saved = d3_CurrentScene;
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        rec = (char *)ls_ReadCharacter(f);
        *(int *)(rec + 300) = (int)gm_FindChar(*(int *)(rec + 300));
    }
    LS_R(&n, 4)
    for (i = 0; i < n; i++) {
        rec = (char *)ls_ReadCharacter(f);
        *(int *)(rec + 300) = gm_Query(0, 2, 7, 1, *(int *)(rec + 300));
    }
    for (o = ai_Unk1468ae0; (int)o < (unsigned)((unsigned)ai_Unk1468ae0 + 0xc00); o++) {
        obj = (char *)*o;
        if (obj != 0) {
            char *bld = 0, *part = 0;
            int uni;

            bld = (char *)gm_BuildQuery(1, 1, *(int *)(obj + 0x2c));
            if (bld == 0) {
                uni = 0;
            } else {
                part = (char *)gm_Query(*(int *)(bld + 0x5d), 2, 6, 1, *(int *)(obj + 0x30));
                if (part == 0 || *(short *)part == 0xfd)
                    uni = init_BuildingSceneUniverse((unsigned char *)bld);
                else
                    uni = init_ObjectSceneUniverse((unsigned short *)part);
            }
            d3if_SelectScene(uni, 1);
            name = obj + 0x130;
            sprintf(buf, "%s_%s", obj + 0x170, name);
            if (strstr(buf, "hozfaeller") != 0)
                sprintf(buf, "holzfaeller_%s", name);
            sprintf(file, "*%s.bgf", buf);
            _strupr(file);
            if (vfs_GetPath(file, vfs_Root, vfs_Root, dummy) == 0) {
                char *c = *(char **)(obj + 300);
                char *s, *u;

                c[0x1f4] = 0;
                s = (char *)unk_Unk4fb390(*(unsigned short *)c) + 4;
                strcpy(obj + 0x170, s);
                u = strrchr(obj + 0x170, 0x5f);
                if (u != 0) {
                    strcpy(name, u + 1);
                    *u = 0;
                }
                sprintf(buf, "%s_%s", obj + 0x170, name);
            }
            ch_CreateMesh(obj, buf, obj + 0x38);
            if (bld != 0)
                *(int *)(obj + 0x2c) = *(int *)(bld + 1);
            if (part != 0)
                *(int *)(obj + 0x30) = *(int *)(part + 2);
            if (*(int *)(obj + 0x34) != 0) {
                if (obj[0x1a8] != 0)
                    ch_SetHandObject(obj, 2, obj + 0x1a8);
                if (obj[0x1e8] != 0) {
                    char *u = strrchr(obj + 0x1e8, 0x5f);

                    if (u != 0)
                        *u = 0;
                    ch_SetSitAnim(obj, obj + 0x1e8);
                }
                own = *(char **)(obj + 300);
                if (own != 0) {
                    if ((unsigned)own < (unsigned)&ai_Chars || (unsigned)own > ((unsigned)&ai_Unk770d8c + 0x651f3)) {
                        init_HookObject(*(int *)(obj + 0x34), own, 1);
                        sprintf(*(char **)(obj + 0x34), "sp_%i", *(int *)(own + 2));
                        *(char **)(own + 0x3b) = obj;
                        {
                            unsigned short kind = *(unsigned short *)own;
                            char *model = 0;
                            char *o2;

                            if (kind == 0x134)
                                model = "ob_KARREN_klein";
                            else if (kind == 0x135)
                                model = "ob_KARREN_mittel";
                            else if (kind == 0x136)
                                model = "ob_KARREN_GROSS";
                            if (model != 0) {
                                o2 = (char *)d3_CreateObject(0, obj + 0x38, obj + 0x48, model);
                                if (o2 != 0) {
                                    char *c2 = (char *)gm_Unk509ff0(own);
                                    char col;

                                    if (c2 != 0 && *(int *)(c2 + 0x54) != 0 && (c2[2] == 5 || c2[2] == 6 || c2[2] == 7))
                                        col = c2[0x54] + 'g';
                                    else
                                        col = 0;
                                    d3if_SelectTextureSet(o2, *(int *)(o2 + 0x1cc), col, 1);
                                    ch_AttachTransport(obj, o2);
                                    *(int *)(o2 + 0x224) = 1;
                                    init_HookObject(o2, own, 1);
                                }
                            }
                        }
                    } else {
                        d3_SetObjectRot(*(int *)(obj + 0x34), obj + 0x48);
                        sprintf(*(char **)(obj + 0x34), "sp_%i", *(int *)(own + 4));
                        *(char **)(*(int *)(obj + 0x34) + 0x200) = own;
                        *(unsigned *)(*(int *)(obj + 0x34) + 0x48) = *(unsigned short *)own | 0x3000000;
                        *(char **)(own + 0x184) = obj;
                        if (*(char **)(own + 0x17c) != 0)
                            own[0x180] = **(char **)(own + 0x17c);
                        ai_Unk4cde20(own);
                    }
                }
                if ((obj[0x8c] & 0x20) != 0)
                    ch_SetVisible(obj, 0);
            }
        }
    }
    d3if_SelectScene(saved, 1);
    i = 0;
    k = 0;
    *(int *)ls_ActionQueue = 0;
    if (ls_Version < 0x10044)
        n = 0x300;
    else
        n = ((ls_Version > 0x10057) - 1 & 0xfffffd30) + 2000;
    do {
        t = (int *)(ls_ActionQueue + k);
        memset(t, 0, 0x194);
        LS_R(&flag, 1)
        *((char *)t + 9) = flag;
        LS_R(t + 3, 4)
        LS_R(t + 5, 4)
        LS_R(t + 0xc, 0x20)
        LS_R(t + 0x14, 0x20)
        LS_R(t + 0x3c, 0xa0)
        LS_R(t + 100, 1)
        LS_R(t + 9, 4)
        LS_R(t + 10, 4)
        i++;
        k += 0x194;
    } while (i < n);
    for (k = 0; k < 0xc5440; k += 0x194) {
        t = (int *)(ls_ActionQueue + k);
        if (*((unsigned char *)t + 9) != 0) {
            int obj2 = ai_Unk1468ae0[t[5]];

            t[5] = obj2;
            if (obj2 != 0) {
                t[9] = ls_ActionAt(t[9]);
                t[10] = ls_ActionAt(t[10]);
                t[0] = ai_Unk1469700[*((unsigned char *)t + 9) * 0x13];
                t[3] = 0;
                if (t[9] == 0) {
                    int ob = t[5];

                    if (ob == 0) {
                        sprintf(dummy, "ls_LoadCharacter(): invalid stockptr for action-type:%i (character per...)", *((unsigned char *)t + 9));
                        *((char *)t + 9) = 0;
                    } else {
                        if (*(int *)(ob + 0x128) != 0)
                            ch_ClearActions(ob);
                        *(int **)(t[5] + 0x128) = t;
                    }
                }
                if (*((char *)t + 9) == '3') {
                    int b2 = gm_BuildQuery(1, 1, t[0xd]);

                    if (b2 == 0) {
                        t[0xc] = 0;
                        t[0x5c] = 0;
                    } else {
                        short *pr = (short *)gm_Query(*(int *)(b2 + 0x5d), 2, 6, 1, t[0xe]);

                        if (pr == 0 || *pr == 0xfd)
                            t[0xc] = t[0x5c] = init_BuildingSceneUniverse((unsigned char *)b2);
                        else
                            t[0xc] = t[0x5c] = init_ObjectSceneUniverse((unsigned short *)pr);
                    }
                } else if (*((char *)t + 9) == '-') {
                    t[0x55] = -1;
                    t[0x3d] = 0;
                }
                if (t[9] == -1 || t[10] == -1)
                    goto bad;
                continue;
            }
        } else {
            if (t[9] == -1 || t[10] == -1)
                goto bad;
            continue;
        }
        memset(t, 0, 0x194);
        continue;
bad:
        sprintf(file, "ls_LoadCharacter(): invalid next or prev-action pointer for action-type:%i", *((unsigned char *)t + 9));
        t[9] = 0;
        t[10] = 0;
        for (i = 0; i < 0xc5440; i += 0x194) {
            if (*(int **)(i + 0x28 + ls_ActionQueue) == t)
                *(int *)(i + 0x28 + ls_ActionQueue) = 0;
            if (*(int **)(i + 0x24 + ls_ActionQueue) == t)
                *(int *)(i + 0x24 + ls_ActionQueue) = 0;
        }
        memset(t, 0, 0x194);
    }
    return 1;
}
/* 0x51f080, reads the five player blocks of 0x7d6a60 (0x2a90 bytes: four words and n records of 0x80 bytes, n = 0x3e or the count read first from 0x10048; the counterpart of 0x51b340) and the player slots of 0x7e3f40 (0x2f4 bytes: the name, the 8 bytes +0x40, the fields up to +0x2a4 + 0x48 in the order of the writer, from version 0x10037 the 8 bytes +0x2ec); the town name of the first slot is looked up in the text table ("_STADTAUSWAHL_%s_INFO+0", 0x42e770) and copied to +0x20 (else the slot's own name). Returns 1. ROUGH */


int unk_Unk51f080(int f)
{
    int n = 0x3e, i, j;
    int *p5, *p7;
    char *p, *s, *q;
    char buf[256];
    int idx;

    if (ls_Version >= 0x10048 && ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    for (p5 = (int *)((unsigned)&ai_Unk7d6a64 + 0x4); (int)p5 < (int)((unsigned)&ai_Unk7d6a64 + 0xd4d4); p5 += 0xaa4) {  /* the bound is the address + 0xd4d4 (the end of the five 0x2a90-byte blocks); ls.c declares ai_Unk7d6a64 as a scalar, so the plain `(unsigned)ai_Unk7d6a64` read the byte value and skipped the loop */
        LS_R(p5 - 2, 4)
        LS_R(p5 - 1, 4)
        LS_R(p5, 4)
        LS_R(p5 + 1, 4)
        i = 0;
        if (n > 0) {
            p7 = p5 + 4;
            do {
                LS_R(p7 - 2, 2)
                LS_R(p7 - 1, 4)
                LS_R(p7, 4)
                LS_R(p7 + 1, 4)
                LS_R(p7 + 6, 4)
                LS_R(p7 + 7, 4)
                LS_R(p7 + 10, 4)
                LS_R(p7 + 11, 4)
                LS_R(p7 + 12, 4)
                LS_R(p7 + 13, 2)
                LS_R(p7 + 9, 4)
                i++;
                p7 += 0x20;
            } while (i < n);
        }
    }
    s = (char *)((unsigned)&ai_PlayerSlots + 0x48);
    for (;;) {
        p = s - 0x48;
        LS_R(p, 0x20)
        LS_R(s - 8, 8)
        LS_R(s, 1)
        LS_R(s + 4, 4)
        LS_R(s + 8, 4)
        LS_R(s + 0xc, 2)
        LS_R(s + 0x10, 8)
        LS_R(s + 0x18, 1)
        LS_R(s + 0x19, 1)
        i = 0;
        q = s + 0x1c;
        do {
            LS_R(q, 8)
            i++;
            q += 8;
        } while (i < 10);
        i = 0;
        q = p + 0xb4;
        do {
            LS_R(q, 8)
            i++;
            q += 8;
        } while (i < 10);
        LS_R(p + 0x104, 1)
        LS_R(p + 0x105, 1)
        i = 0;
        q = p + 0x108;
        do {
            LS_R(q, 4)
            i++;
            q += 4;
        } while (i < 4);
        i = 0;
        q = p + 0x118;
        do {
            LS_R(q, 4)
            i++;
            q += 4;
        } while (i < 4);
        LS_R(p + 0x128, 4)
        i = 0;
        do {
            LS_R(p + i + 0x12c, 1)
            i++;
        } while (i < 0xb);
        LS_R(p + 0x137, 1)
        i = 0;
        do {
            LS_R(p + i + 0x138, 1)
            i++;
        } while (i < 7);
        if (ls_Read((int)(p + 0x13f), 1, 1, f) == 0)
            break;
        LS_R(p + 0x140, 1)
        LS_R(p + 0x141, 1)
        LS_R(p + 0x142, 1)
        i = 0;
        q = p + 0x144;
        do {
            LS_R(q, 0x12)
            i++;
            q += 0x12;
        } while (i < 8);
        LS_R(p + 0x1d4, 4)
        LS_R(p + 400 + 0x48, 4)
        LS_R(p + 0x1dc, 0xd0)
        if (ls_Version >= 0x10037 && ls_Read((int)(p + 0x2ec), 8, 1, f) == 0)
            return 0;
        s += 0x2f4;
        if ((int)s > ((unsigned)((unsigned)ai_PlayerSlots + 0xbe0) + 0x37)) {
            sprintf(buf, "_STADTAUSWAHL_%s_INFO+0", ai_PlayerSlots[0]);
            _strupr(buf);
            idx = txt_FindKey(buf);
            if (idx == -1) {
                strcpy(ai_PlayerSlots[0] + 0x20, ai_PlayerSlots[0]);
                return 1;
            }
            strcpy(ai_PlayerSlots[0] + 0x20, txt_Ptrs[idx]);
            return 1;
        }
    }
    return 0;
}
/* 0x51f650, reads the two relation maps ai_Relations / ai_RelationMemory (n x n bytes, a row of 0x300: n = 0x100 below 0x1003a, 0x100 below 0x10042 else 0x300; the second map from 0x10036). Returns 1 */


int ls_ReadRelations(int f)
{
    int n, i, j;
    char *p;

    if (ls_Version < 0x1003a)
        n = 0x100;
    else
        n = ((ls_Version >= 0x10042) - 1 & 0xffffff00) + 0x300;
    i = 0;
    if (n > 0) {
        p = (char *)ai_Relations;
        do {
            j = 0;
            do {
                LS_R(j + (int)p, 1)
                j++;
            } while (j < n);
            p += 0x300;
            i++;
        } while (i < n);
    }
    if (ls_Version >= 0x10036 && (i = 0, n > 0)) {
        p = (char *)ai_RelationMemory;
        do {
            j = 0;
            do {
                LS_R(j + (int)p, 1)
                j++;
            } while (j < n);
            p += 0x300;
            i++;
        } while (i < n);
    }
    return 1;
}
/* 0x51f720, reads the global tables 0x91b4c0 .. 0x91b690 (the counterpart of the writer 0x51b910). ROUGH: converted from the pseudocode */


int unk_Unk51f720(int f)
{
  int iVar1;
  int iVar2;
  short *puVar3;
  int *puVar4;
  short *puVar5;
  LS_R(((char *)(unsigned)&ord_Statistics), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x4)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x8)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0xc)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x10)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x14)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x18)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x1c)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x20)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x24)), 4)
  LS_R(((char *)(unsigned)&ord_RatingRate), 4)
  LS_R(((char *)(unsigned)&ord_StatisticsClock), 4)
  LS_R(((char *)((unsigned)&ord_StatisticsClock + 0x4)), 2)
  LS_R(((char *)((unsigned)&ord_StatisticsClock + 0x6)), 4)
  LS_R(((char *)((unsigned)&ord_StatisticsClock + 0xa)), 4)
  LS_R(((char *)(unsigned)&ai_RatingBase), 4)
  LS_R(((char *)(unsigned)&ai_Rating), 4)
  LS_R(((char *)(unsigned)&ord_MessageRecord), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x4)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x8)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0xc)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x10)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x14)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x18)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x1c)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x20)), 4)
  puVar4 = ((char *)((unsigned)&ord_StatisticsHistory + 0x4));
  do {
    LS_R(puVar4 + -1, 4)
    LS_R(puVar4, 4)
    LS_R(puVar4 + 1, 4)
    LS_R(puVar4 + 2, 4)
    LS_R(puVar4 + 3, 4)
    puVar4 = puVar4 + 5;
  } while ((int)puVar4 < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x4));
  LS_R(((char *)(unsigned)&gm_IncomeHistory), 0x44)
  LS_R(((char *)(unsigned)&gm_IncomeHistory + 0x44), 0x44)
  LS_R(((char *)(unsigned)&gm_IncomeHistory + 0x88), 0x44)
  iVar1 = ls_Read((int)((unsigned)&gm_IncomeHistory + 0xcc), 0x44, 1, f);
  if (iVar1 != 0) {
    LS_R(((char *)(unsigned)&gm_IncomeHistory + 0x110), 0x44)
    puVar3 = ((char *)((unsigned)&ai_Grid + 0x2));
    do {
      iVar1 = 0;
      puVar5 = puVar3;
      do {
        LS_R(puVar5 + -1, 2)
        LS_R(puVar5, 2)
        LS_R(puVar5 + 1, 1)
        LS_R(puVar5 + 3, 4)
        iVar1 = iVar1 + 1;
        puVar5 = puVar5 + 0xc;
      } while (iVar1 < 8);
      puVar3 = puVar3 + 0x60;
      if (((unsigned)&ord_RatingBytes + 0x1) < (int)puVar3) {
        return 1;
      }
    } while( 1 );
  }
  return 0;
}
/* 0x51fb80, the save game's reader of the avatar table: forwards to gv_ReadAvatars (0x4ed270) */


int ls_ReadAvatars(void *f)
{
    return gv_ReadAvatars(f);
}
/* 0x51fb90, reads the 0x14c-byte records of 0x6fab40 (the counterpart of 0x51bdc0): the counts, then each record in pieces (the 8 words +0x8c from 0x1002f), the text buffer of +0x7c (size +0x80, "messagebox", 0x400 bytes) and the type specific block of +0xac (the kind 0xc gets a 0x58-byte "f3_alwin_t:d" record at +0xbc, the kind 0xd -1 and, for the type 6, +0xd0 = 0), the records flagged 8 are listed in 0x74db40; then the 0x29-word table 0x74e1e0 (0x2c-byte entries) and the 0x14-word table 0x758ca0. Returns 1. ROUGH @deep diff read against the source (97 lines, 70 code / 6 regs / 4 order): meaning verified - the three 4-byte counts, the record loop (rec = 0x6fab40 + idx*0x14c, the 11 pieces incl. the version-gated 8 words +0x8c, the text buffer +0x7c / +0x80 via mem_Alloc and the direct read, the kind 0xc / 0xd block of +0xac and the +0xbc pointer / -1 / +0xd0 clear), the flagged records into 0x74db40, then the 0x29-int table 0x74e1e0 and the 0x14-int table 0x758ca4. Left is codegen: a store-order pair (the +0x7c store before/after the following load) and the induction-variable base - the original keeps the middle-loop pointer 8 bytes above ours (its edi with 0/+4/+8/... vs our edi-8 with -8/-4/0/...; both reach the same 0x74e1e8 start), the next recorded attempt here ('pointer one induction step ahead', notes/units/ls.md Open work). Readability: Q3 on every table literal (0x6fab40, 0x74db40, 0x74e1e0, 0x758ca4), Q2 on he_FreeEvents / ai_Unk7585e4 / ai_Unk6f261c / ai_Unk6f2620, Q1/Q5. */

int unk_Unk51fb90(int f)
{
    int n, size2, i, idx;
    char *rec;
    int *pi;
    char *tb;

    he_FreeEvents();
    LS_R(&n, 4)
    LS_R(&size2, 4)
    ai_Unk6f261c = 0;
    ai_Unk6f2620 = 0;
    ai_Unk7585e4 = 0;
    if (n > 0) {
        do {
            idx = ai_Unk7585e4;
            rec = (char *)(unsigned)&he_Events + idx * 0x14c;
            LS_R(rec, 1)
            LS_R(rec + 4, 4)
            LS_R(rec + 8, 2)
            LS_R(rec + 0xc, 4)
            LS_R(rec + 0x10, 4)
            LS_R(rec + 0x14, 0x30)
            LS_R(rec + 0x44, 0xe)
            LS_R(rec + 0x52, 0xe)
            LS_R(rec + 0x60, 0xe)
            LS_R(rec + 0x70, 4)
            LS_R(rec + 0x78, 1)
            if (ls_Version >= 0x1002f) {
                for (i = 0; i < 8; i++)
                    LS_R(rec + 0x8c + i * 4, 4)
            }
            pi = (int *)(rec + 0x80);
            *(int *)(rec + 0x7c) = 0;
            *pi = 0;
            LS_R(pi, 4)
            if (*pi != 0) {
                tb = (char *)mem_Alloc(0x400, "messagebox");
                *(char **)(rec + 0x7c) = tb;
                if (ls_Read((int)tb, *pi, 1, f) == 0)
                    return 0;
            }
            if (*rec == 0xc) {
                LS_R(rec + 0xac, size2)
                *(char **)(rec + 0xbc) = (char *)mem_Alloc(0x58, "f3_alwin_t:d");
            } else if (*rec == 0xd) {
                LS_R(rec + 0xac, size2)
                *(int *)(rec + 0xbc) = -1;
                if (*(int *)(rec + 0x70) == 6)
                    rec[0xd0] = 0;
            } else {
                LS_R(rec + 0xac, size2)
            }
            if ((rec[0x78] & 8) != 0) {
                he_QuickList[ai_Unk6f2620] = rec;
                ai_Unk6f2620++;
            }
            ai_Unk7585e4++;
        } while (ai_Unk7585e4 < n);
    }
    ai_Unk6f261c = ai_Unk7585e4;
    LS_R(&n, 4)
    i = 0;
    if (n > 0) {
        pi = (int *)((unsigned)&he_QuickList + 0x6a0);
        do {
            *pi = 0;
            LS_R(pi, 4)
            LS_R(pi + 1, 1)
            LS_R(pi + 2, 4)
            LS_R(pi + 3, 4)
            LS_R(pi + 4, 4)
            LS_R(pi + 5, 0xe)
            LS_R((int)pi + 0x22, 0x80)
            i++;
            pi += 0x29;
        } while (i < n);
    }
    LS_R(&n, 4)
    if (n > 0) {
        i = 0;
        pi = (int *)((unsigned)&he_RunHandlers + 0x6a4);
        do {
            LS_R(pi - 1, 1)
            LS_R(pi, 4)
            LS_R(pi + 1, 4)
            LS_R(pi + 2, 4)
            LS_R(pi + 3, 0x40)
            i++;
            pi += 0x14;
        } while (i < n);
    }
    return 1;
}
/* 0x520000, reads the party records (0x925520, 0x114 bytes; the counterpart of 0x51c2f0): the count, then per record the words and bytes (+0x20 is 16 bytes of 0x40-byte pieces below 0x10059, else 16 words; the clock at +4 is set from 0x770ae0 and 0x4fe140 below 0x1003f; +0x14 = -1 below 0x10040). Returns 1 @deep read 0x520000..0x5201f1 against the source; two real fixes: the temp buffer is 0x1000 bytes (the original's frame is 0x1008 with __chkstk, ours was 0x408 for tmp[0x400]) and the three version branches are written from the >= side so the non-zero arm is inline (the original jl's to the <0x10059 / <0x1003f / <0x10040 arm); 131 -> 95 code hunks, 100 -> 70 asm lines. Meaning verified: the count, the per-record reads p-0x14 / p-0xc / p / p+0x12 / p+0x1c, the tmp[0x40] + memcpy to p+0x20 vs the 4-byte p+0x20 from 0x10059, the +0x60 / +0x68 (0x98) reads, the 0xe memcpy from 0x770ae0 + 0x4fe140(p+4, i+1, 0, 0) vs the 0xe read from 0x1003f, the -1 to p-8 below 0x10040, the 0x114 stride and count loop. Left is codegen: the IV base - the original's ebx = 0x925534, ours = 0x925540 (0xc higher, every displacement differs by 0xc), and i in esi (the original) vs edi (ours) which is entangled with the memcpy's esi restore. Readability: Q1, Q2 (gm_AddTime, ai_Time), Q3 (0x925534), Q4, Q7 (gm_AddTime has no prototype). */


int unk_Unk520000(int f)
{
    char tmp[0x1000];
    int n, i;
    char *p;

    LS_R(&n, 4)
    i = 0;
    p = (char *)((unsigned)&ai_Unk925520 + 0x14);
    for (; i < n; i++) {
        LS_R(p - 0x14, 4)
        LS_R(p - 0xc, 1)
        LS_R(p, 4)
        LS_R(p + 0x12, 1)
        LS_R(p + 0x1c, 1)
        if (ls_Version >= 0x10059) {
            LS_R(p + 0x20, 4)
        } else {
            LS_R(tmp, 0x40)
            memcpy(p + 0x20, tmp, 0x40);
        }
        LS_R(p + 0x60, 1)
        LS_R(p + 0x68, 0x98)
        if (ls_Version >= 0x1003f) {
            LS_R(p + 4, 0xe)
        } else {
            memcpy(p + 4, ai_Time, 0xe);
            gm_AddTime(p + 4, i + 1, 0, 0);
        }
        if (ls_Version >= 0x10040)
            LS_R(p - 8, 4)
        else
            *(int *)(p - 8) = -1;
        p += 0x114;
    }
    return 1;
}
/* 0x520200, reads the mission table 0x6f0724 / 0x601058 .. 0x60106c, 0xf83520 (0x10 bytes), and the 0x11-word records from 0xf83548 (8 pairs of words each). Returns 1 @deep diff read against the source and the original: meaning verified - the 1-byte hst_TextsOn read, ai_Unk4b1d20, the two 4-byte counts (hst_TextCurrent / hst_KeyNext = the first + n), the 16 bytes at 0xf83520, the record loop p = 0xf83548 with the 8 word pairs read from p-4 / p and p += 0x11 to > 0xf83657 - all match. The 100 code hunks are the known epilogue shape (the original inlines pop pop pop pop ret after every failed read, ours shares one return-0 block) plus register/order scheduling (inc ebx before/after add esi, xor ebx placement). Readability: Q1, Q2 ai_Unk4b1d20 (also Q7: undeclared), Q3 the 0xf83520 / 0xf83548 / 0xf83657 image literals. */


int unk_Unk520200(int f)
{
    int n, i, j;
    int *p, *q;

    LS_R(&hst_TextsOn, 1)
    ai_Unk4b1d20();
    LS_R(&n, 4)
    hst_TextCurrent = hst_TextFirst + n;
    if (ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    hst_KeyNext = hst_KeyFirst + n;
    for (i = 0; i < 0x10; i++)
        LS_R((char *)(unsigned)&hst_Key + i, 1)
    p = (int *)((unsigned)&hst_Lists + 0x8);
    while (ls_Read((int)(p - 2), 4, 1, f) != 0) {
        q = p;
        for (j = 0; j < 8; j++) {
            LS_R(q - 1, 4)
            LS_R(q, 4)
            q += 2;
        }
        p += 0x11;
        if ((int)p > ((unsigned)((unsigned)hst_Lists + 0x114) + 0x3))
            return 1;
    }
    return 0;
}
/* 0x520310, reads the two record pools 0x9367e0 / 0xa69140 (0x99-byte records: the references at +0x91 / +0x95 are indices turned into pointers (-1 = NULL)), the pool heads 0xa68d24 / 0xa68c00 / 0xf81160, the reference table 0xf31160 (10 bytes each, 0x2000 or 0x8000 of them from 0x1003c) and the words 0xf81168, 0xf8121c, 0xa68c04, 0x60bd88 .. 0x60bd90. Returns true when all was read @deep diff read against the source: meaning verified - the 0x99-byte pool loop 0x9367e0 .. (refs at +0x91/+0x95, -1 -> NULL, else pool + n*0x99), the pool heads a68d24 / a68c00, the 0x2000/0x8000 table from 0x1003c, the reference table 0xf31160 (10-byte entries, p[1]=0, *p=-1, byte -2 = 1), the second pool loop 0xa69140, the f81160 head and the final reads (0xf8121c, 0xa68c04, 0x60bd88 / 0x60bd8c / 0x60bd90) all match. Left is codegen: the IV base - ours keeps esi at p+4 (reads [esi-0x4] / [esi] and the record at esi-0x95), the original keeps esi at p (record at esi-0x91, refs at [esi] / [esi+0x4]) - same addresses, and the loop bound n in a stack slot vs ecx; plus the shared vs inline return-0 epilogue (xor eax,eax at 0x52039e). Readability: Q1, Q2 (ai_Unka68c00 / a68d24 / f81160 / f81168), Q3 (12 image literals), Q4. */


int unk_Unk520310(int f)
{
    int *p;
    int i, n;
    char *q;

    for (p = (int *)((unsigned)&ai_Unk9367e0 + 0x91); (int)p < ((unsigned)&ai_Unka68800 + 0x71); p = (int *)((char *)p + 0x99)) {
        LS_R((char *)p - 0x91, 0x99)
        if (*p == -1)
            *p = 0;
        else
            *p = (unsigned)&ai_Unk9367e0 + *p * 0x99;
        if (p[1] == -1)
            p[1] = 0;
        else
            p[1] = (unsigned)&ai_Unk9367e0 + p[1] * 0x99;
    }
    LS_R(&n, 4)
    ai_Unka68d24 = n == -1 ? 0 : (unsigned)&ai_Unk9367e0 + n * 0x99;
    i = 0;
    LS_R(&n, 4)
    ai_Unka68c00 = n == -1 ? 0 : (unsigned)&ai_Unk9367e0 + n * 0x99;
    n = ((ls_Version < 0x1003c) - 1 & 0x6000) + 0x2000;
    for (p = (int *)((unsigned)&ai_Unkf31160 + 0x2); (int)p < ((unsigned)((unsigned)ai_Unkf31160 + 0x50000) + 0x2); p = (int *)((char *)p + 10)) {
        p[1] = 0;
        *p = -1;
        *((char *)p - 2) = 1;
    }
    if (n != 0) {
        for (p = (int *)((unsigned)&ai_Unka69140 + 0x91); i < n; p = (int *)((char *)p + 0x99)) {
            LS_R((char *)p - 0x91, 0x99)
            if (*p == -1)
                *p = 0;
            else
                *p = (unsigned)&ai_Unka69140 + *p * 0x99;
            if (p[1] == -1)
                p[1] = 0;
            else
                p[1] = (unsigned)&ai_Unka69140 + p[1] * 0x99;
            i++;
        }
    }
    i = 0;
    if (ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    cm_SendQueue = n == -1 ? 0 : (unsigned)&ai_Unka69140 + n * 0x99;
    LS_R(&ai_Unkf81168, 4)
    n = ((ls_Version < 0x1003c) - 1 & 0x6000) + 0x2000;
    if (n != 0) {
        q = (char *)(unsigned)&ai_Unkf31160;
        do {
            LS_R(q, 10)
            i++;
            q += 10;
        } while (i < n);
    }
    if (ls_Read((int)(unsigned)&ai_Unkf8121c, 4, 1, f) == 0)
        return 0;
    LS_R((unsigned)&ai_Unka68c04, 4)
    *(int *)(unsigned)&ai_Unk60bd88 = 0;
    if (ls_Read((int)(unsigned)&ai_Unk60bd88, 4, 1, f) == 0)
        return 0;
    LS_R((unsigned)&ai_Unk60bd8c, 4)
    return ls_Read((int)(unsigned)&ai_Unk60bd90, 4, 1, f) != 0;
}
/* 0x5205f0, reads the table 0x7604e8 (the counterpart of 0x51c870): the count 0x760610 and the entries; from 0x10050 the two further blocks 0x5dc0c0 and 0x4ed910. Returns 1 @deep read against the original 0x5205f0..0x5206af: meaning verified - the 0x760610 count, the entry loop p = 0x7604e8 with the reads at p-0x60 / p / p+0x68 / p+0xc8 (our int offsets -0x18 / 0 / +0x1a / +0x32 are the same addresses) to p < 0x760548 (24 entries), then the version gate (our > 0x1004f is the original's jl 0x10050) and the two calls 0x5dc0c0(f) / 0x4ed910(f, ls_Version). Left is codegen: VC6 re-bases our loop pointer to p+0x68 (reads [esi-0xc8] / [esi-0x68] / [esi+0x60], condition lea eax,[esi-0x68]) where the original keeps esi = 0x7604e8 and reads [esi-0x60] / [esi] / [esi+0x68] / [esi+0xc8], plus the f register (edi in the original). Readability: Q1, Q2 (ai_LoadWind / unk_Unk4ed910), Q3 (0x760610, 0x7604e8, 0x760548). */


int unk_Unk5205f0(int f)
{
    int *p;

    if (ls_Read((int)(unsigned)&ai_Unk760610, 4, 1, f) == 0)
        return 0;
    p = (int *)(unsigned)&full_data_7604e8;
    do {
        LS_R(p - 0x18, 4)
        LS_R(p, 4)
        LS_R(p + 0x1a, 4)
        LS_R(p + 0x32, 4)
        p++;
    } while ((int)p < (unsigned)((unsigned)full_data_7604e8 + 0x60));
    if (ls_Version > 0x1004f && ai_LoadWind(f) != 0 && unk_Unk4ed910(f, ls_Version) != 0)
        return 1;
    return 0;
}
/* 0x5206b0, the fix-up after a load, the counterpart of 0x519a00 before a save: it turns the loaded ids back into pointers: the player's character (ai_PlayerChar = gm_FindChar, 0 = fail), the market building (ai_MarketBuilding = gm_BuildQuery (1, 5, 10)), the eight kind 0xb characters (ai_KindBChars), the reference fields of the ai_Chars records (gm_FindById into +0x16c and +0x170, he_FindEvent into +0x17c, +0x184 cleared), the objects of ai_Objects (an object with an owner is chained into the owner's list: the heads at +0x5d building / +0x14 object / +0x178 character, +0x3f the next link; objects of the good type 0x1d lose their +0x24 word), the event records of he_Events (the kinds 0x29 0x2d 0x2f 0x11 0x25 5 10 0x26 or flagged 8 are re-initialised through he_RunHandlers[kind] with ai_GameState = 2, the stored +0x70 saved and put back; the kinds 0xd / 0xf / 0x78 / 0x76 / 0x14 get their own fixes), the first word of the 0xa4-byte records at 0x74e1e0 (an event owner id, he_FindEvent (1, 1, id)) and the target words of the reference records (ls_RefTargets, by kind: 1 gm_FindChar, 3 gm_FindObjectInst, 2 gm_FindBuildingById, 9 cm_PartyById); it closes with gm_ResetStock. Returns 1, 0 when the player's character is gone @deep B3: code hunks 223, slots 18, regs 12, fill 6. All hunks read this session and in the 2026-10-24/25 sessions: every statement, constant, callee, field offset and size matches the original 0x5206b0..0x520a2f; the 0xffff / 0xef immediates, the kind bytes and the callback literals were scanned twice. Last-bytes leads tried: the IV based at 0x770cf0 (iv-shift, written as the original's biased pointer) read 226 asm lines, reverted; the compare written from the >= side (ls_Version >= 0x10017) 221 -> 217, kept; the loop based at the other member 0x770cec with the bound 0x7d60ec 221 -> 190 asm lines (code 333 -> 290), kept; the id test written as *(unsigned short *)id != 0xffff (the same form that materialises 0xffff in edi in 0x520b80) 190 -> 179, kept. Readability then advanced to 552 bytes / 165 asm lines: the Q3 symbols (ai_Chars, ai_KindBChars, he_Events, he_QuickList, ls_RefTargets), the Q4 typed pointer views (id, otype, owner, list, next, ev70, ev10, evb8, v24) and a/b scoped into the object loop, which removed the frame hunk (0x14 -> 0xc). Also tried and reverted: he_Events + 0x53000 as the event loop bound (WORSE, 188 asm lines) and the packed LSREF struct view of the reference records (WORSE, 186). Left for the deep toolbox: the character loop's bound computed as lea esi-4 against the original's direct cmp esi (the VC6 IV base, ours +4), the object loop's ai_Objects preheader/reload order, the a/b/c slot order ([esp+0x14]/[esp+0x18] against [esp+0x10]/[esp+0x14]) and the hoisted push ebp/edi. Q3 gap: 0x74e1e0 and 0x7585e0, the 256 records of 0xa4 bytes whose first word is an event owner id (he_FindEvent kind 1); he.c only memsets the block at init, the save writer 0x51bdc0 writes it and no unit names it, so there is no evidence for a name and the two literals stay. Q4 gap: the AIOBJ word at +0x24 (cleared for the objects of the good type 0x1d) - ai.h pads it and no unit reads it with a meaning, and an unkNN field name is itself a Q2 placeholder, so one raw access stays (a shared-header field decision). Q1/Q2/Q5/Q6/Q7 pass. */


int ls_FixupAfterLoad(void)
{
    int *p, *q;
    char *r;
    int i, saved, v, c;
    unsigned char kind, *pb;
    unsigned short *id, *otype;
    int *owner, *ev70, *ev10, *evb8, *list, *next;
    unsigned char **v24;

    ai_PlayerCharC = (char *)gm_FindChar((int)ai_PlayerCharC);
    if (ai_PlayerCharC == 0)
        return 0;
    ai_MarketBuilding = (int)gm_BuildQuery(1, 5, 10);
    if (ls_Version >= 0x10017) {
        for (q = (int *)ai_KindBChars; (int)q < (int)(ai_KindBChars + 8); q++)
            *q = (int)gm_FindChar(*q);
    }
    for (p = (int *)((char *)ai_Chars + 0x16c); (int)p < (int)(ai_Chars + 768) + 0x16c; p = (int *)((char *)p + 0x21c)) {
        id = (unsigned short *)((char *)p - 0x16c);
        if (*id != 0xffff) {
            if (p[0] == -1)
                p[0] = 0;
            else
                gm_FindById(p, 0, 0, p[0]);
            if (p[1] == -1)
                p[1] = 0;
            else
                gm_FindById(p + 1, 0, 0, p[1]);
            if (p[4] == -1)
                p[4] = 0;
            else
                p[4] = (int)he_FindEvent(1, 1, p[4]);
            p[6] = 0;
        }
    }
    for (i = 0; i < f3gm_ObjectCount; i++) {
        int a, b;

        r = (char *)ai_Objects + i * 0x43;
        otype = (unsigned short *)r;
        owner = (int *)(r + 6);
        if (ai_Goods[*otype * 0x41] == 0x1d)
            *(int *)(r + 0x24) = 0;
        if (*owner != -1 && gm_FindById(&a, &b, &c, *owner) != 0) {
            int link;

            if (a == 0) {
                if (b == 0) {
                    list = (int *)(c + 0x178);
                    link = *list;
                    if (link == 0) {
                        *list = (int)r;
                        continue;
                    }
                } else {
                    list = (int *)(b + 0x14);
                    link = *list;
                    if (link == 0) {
                        *list = (int)r;
                        continue;
                    }
                }
            } else {
                list = (int *)(a + 0x5d);
                link = *list;
                if (link == 0) {
                    *list = (int)r;
                    continue;
                }
            }
            next = (int *)(link + 0x3f);
            while (*next != 0) {
                link = *next;
                next = (int *)(link + 0x3f);
            }
            *next = (int)r;
        }
    }
    saved = d3_CurrentScene;
    d3if_SelectScene(0, 0);
    for (pb = he_Events; (int)pb < (int)he_QuickList; pb += 0x14c) {
        kind = *pb;
        if (kind != 0) {
            if ((kind == 0x29 || kind == 0x2d || kind == 0x2f || kind == 0x11 || kind == 0x25 || kind == 5
                 || kind == 10 || kind == 0x26 || (pb[0x78] & 8) != 0)
                && (v = *(ev70 = (int *)(pb + 0x70))) != 0) {
                *ev70 = 0;
                ai_GameState = 2;
                if (kind == 0x11)
                    pb[0xf0] &= 0xef;
                he_RunHandlers[kind](pb);
                ai_GameState = 0;
                *ev70 = v;
            }
            kind = *pb;
            if (kind != 0xc) {
                if (kind == 0xd) {
                    evb8 = (int *)(pb + 0xb8);
                    gm_FindById(&c, 0, 0, *evb8);
                    if (c != 0)
                        he_ShowConstruction((int)pb);
                } else if (kind == 0xf) {
                    ev10 = (int *)(pb + 0x10);
                    v = gm_FindObjectInst(*ev10);
                    if (v != 0) {
                        v24 = (unsigned char **)(v + 0x24);
                        *v24 = pb;
                    }
                } else if (kind == 0x78) {
                    memset(pb + 200, 0, 8);
                } else if (kind == 0x76) {
                    memset(pb + 0xb0, 0, 4);
                } else if (kind == 0x14) {
                    sv_CountGroupMembers((char *)pb);
                    pb[0xac] = pb[0x14] + pb[0x18];
                }
            }
        }
    }
    d3if_SelectScene(saved, 0);
    for (q = (int *)((unsigned)&he_QuickList + 0x6a0); (int)q < ((unsigned)&he_QuickList + 0xaaa0); q += 0x29) {
        if (*q != 0)
            *q = (int)he_FindEvent(1, 1, *q);
    }
    for (q = ls_RefTargets; (int)q < (int)ls_RefTargets + 0x50000; q = (int *)((char *)q + 10)) {
        if (*q != 0) {
            if (*((char *)q - 5) == 1)
                *q = (int)gm_FindChar(*q);
            if (*((char *)q - 5) == 3)
                *q = gm_FindObjectInst(*q);
            if (*((char *)q - 5) == 2)
                *q = (int)gm_FindBuildingById(*q);
            if (*((char *)q - 5) == 9)
                *q = cm_PartyById(*q);
        }
    }
    gm_ResetStock();
    return 1;
}
/* 0x520a30, sends the save game file `name` to the other players: opens it "rbn" (vfs_Tfopen), takes its length (vfs_ftell, rounded up to a multiple of 0x80), reads it into a mem_Alloc'd "ls_SendGame" buffer (vfs_fread) and hands it out in blocks of 0x80 bytes (cm_SendLoadBlock, the size first in cm_SendLoadSize; a Sleep(15) after every 16th block; the network pumps sv_PumpMessages / sv_PumpNet between), then waits until nothing is queued in cm_SendQueue. Closes the file and frees the buffer. Returns 1, 0 when the file or the buffer fails */


int ls_SendGame(char *name)
{
    VFSFILE *fh;
    int len, size, cnt = 0;
    char *buf, *p;

    fh = vfs_Tfopen(name, "rbn", (int)vfs_Root);
    if (fh == 0)
        return 0;
    vfs_fseek(fh, 0, SEEK_END);
    len = (int)vfs_ftell(fh);
    size = (len / 0x80 + 1) * 0x80;
    vfs_fseek(fh, 0, SEEK_SET);
    buf = (char *)mem_Alloc(size, "ls_SendGame");
    if (buf == 0) {
        vfs_fclose(fh);
        return 0;
    }
    if (vfs_fread(buf, len, 1, fh) == -1) {
        mem_Free(buf);
        vfs_fclose(fh);
        return 0;
    }
    cm_SendLoadSize(size);
    p = buf;
    if (size > 0) {
        int sent = 0;

        do {
            if (cm_SendLoadBlock(p) != -1) {
                p += 0x80;
                sent += 0x80;
                if (cnt++ % 16 == 0)
                    Sleep(0xf);
            }
            sv_PumpMessages();
            sv_PumpNet();
        } while (sent < size);
    }
    while (cm_SendQueue != 0) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    mem_Free(buf);
    vfs_fclose(fh);
    return 1;
}
/* 0x520b80, takes the save game that arrived over the network (the buffer 0xf31140, length 0xa68d28, wait for 0xf81220 to reach it), opens it as a memory file (0x4311d0), reads the header and the globals like 0x51c950 (only when the header flag has bit 2: the network start flag, and the version is below 0x1005b), the readers 0x51d3d0, 0x51d550, 0x51db00, 0x51de70 (the characters of the kind 6 become kind 5), 0x51f080, 0x4ea7a0 (from 0x10045), 0x51d2d0 (unless the header flag has bit 4) and 0x5206b0. The first failing step closes the file, frees the buffer and returns 0. Returns 1. ROUGH */


int ls_ReceiveGame(void)
{
    int fh, i;
    int hdr[0x40];
    char *p;

    while (ai_Unkf31140 == 0 || ai_Unkf81220 < ai_Unka68d28) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    fh = vfs_MemOpen(ai_Unkf31140, ai_Unka68d28, "rb");
    if (fh == 0) {
        mem_Free(ai_Unkf31140);
        ai_Unkf31140 = 0;
        return 0;
    }
    ai_PlayerCharC = 0;
    ls_ReadSaveHeader(fh, hdr);
    if ((((char *)hdr)[4] & 2) != 0 && hdr[0] <= 0x1005a) {
#define LS_RF(p, sz) ls_Read((int)(p), (sz), 1, fh)
        if (!LS_RF(&ai_NextObjectId, 4) || !LS_RF(&ai_Unk6f2618, 4) || !LS_RF(ai_Time, 0xe)
            || !LS_RF(&ai_OwnPlayer, 1) || !LS_RF(&ai_PlayerCharC, 4) || !LS_RF(&ai_Unk6f1bb8, 4)
            || !LS_RF(&ai_Unk6f1bbc, 4) || !LS_RF(&ai_CharCount, 4)) {
            vfs_fclose(fh);
            goto fail;
        }
        ai_Unk75e130 = *(int *)ai_Time;
        ai_Unk75e134 = *(int *)(ai_Time + 4);
        ai_Unk75e138 = *(int *)(ai_Time + 8);
        ai_Unk75e13c = *(short *)(ai_Time + 0xc);
        LS_RF(&ai_Unk6f1bcc, 4);
        LS_RF(ai_UpdateStart, 0x18);
        if (ls_Version >= 0x10030)
            LS_RF(&ai_Unk61a658, 4);
        if (ls_Version < 0x1003d)
            ai_Unk6f1b88 = 1000000;
        else
            LS_RF(&ai_Unk6f1b88, 4);
        if (ls_ReadObjects(fh) == 0) {
            ((int (*)())vfs_fclose)();
            goto fail;
        }
        if (ls_ReadBuildings(fh) != 0) {
            if (ls_ReadPlayerRecords(fh) == 0) {
                ((int (*)())vfs_fclose)();
                goto fail;
            }
            if (ls_ReadCharacters(fh) == 0) {
                vfs_fclose(fh);
                goto fail;
            }
            p = ai_Chars[0] + 2;
            do {
                if (*(unsigned short *)(p - 2) != 0xffff && *p == 6)
                    *p = 5;
                p += 0x21c;
            } while ((int)p < (unsigned)((unsigned)ai_Chars + 0x65402));
            if (unk_Unk51f080(fh) != 0) {
                if (ls_Version >= 0x10045 && amt_LoadAemter(fh, ls_Version) == 0) {
                    vfs_fclose(fh);
                    goto fail;
                }
                if ((((char *)hdr)[4] & 4) == 0 && unk_Unk51d2d0(fh) == 0) {
                    vfs_fclose(fh);
                    goto fail;
                }
                if (ls_FixupAfterLoad() != 0) {
                    vfs_fclose(fh);
                    mem_Free(ai_Unkf31140);
                    ai_Unkf31140 = 0;
                    return 1;
                }
            }
        }
    }
    vfs_fclose(fh);
fail:
    mem_Free(ai_Unkf31140);
    ai_Unkf31140 = 0;
    return 0;
}

unsigned ai_Unka68d28 = 0;  /* 0xa68d28, the received bytes and the expected length (compared unsigned */
int ai_Unkf81168 = 0;  /* 0xf81168 .. */
unsigned ai_Unkf81220 = 0;  /* 0xf81220 / */
