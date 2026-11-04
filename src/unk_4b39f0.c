/* unk_4b39f0.c -- missions ("_AUFTRAEGE_VERGABE_HS_%.2d", "special\mission"): the running missions
   ai_Missions, the mission types ai_MissionTypes (.data right after hst.c's literals), an own random generator.
   Unit start and name are guesses (open boundary); it ends with save / load 0x4b62e0 / 0x4b63b0. Names are ours.
   Open: G13TYPE.unk18 and G13MIS.unk18 have no name evidence yet; G13TYPE.offerSpeech, G13MIS.progress / fulfilled stand
   on their users (the _AUFTRAEGE_VERGABE speech, ai_CountMissionEvent and the completion flag of he.c 0x56ce40). The
   two unk18 fields stay unk (zeroed / saved only).
   Open: the mission conditions read AICHAR / AIBLD bytes through the G13_B / G13_501190 views (type debt).
   Open: not included yet (dcp headers conflicts unk_4b39f0 HEADER): d2.h; held by own declarations that differ
   from the owner's: d2_Radios.
   unit of rea 8cef7d5b (src/game/unk_4b39f0.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_4b39f0.h"
#include "di_input.h"
#include "net.h"
#include "d2_types.h"                 /* D2OBJ, d2_Objs' record */
#include "ahm.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#include "f3snd.h"
#include "io_text.h"
#include "ls.h"
#include "main.h"
#include "sblib.h"
#include "sv.h"
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define io_PrintTextId ((int (*)(int, ...))io_PrintText)  /* 0x515960 (io_text.h: int io_PrintText(char *fmt, ...)): the call with a text id for fmt (an id of txt_Ptrs, computed from a mission type's text) */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
/* 0x4b39f0, clears the running missions */
int ai_ClearMissions(void)
{
    memset(ai_Missions, 0, sizeof(ai_Missions));
    return 1;
}
/* 0x4b3a10, a random number below n (LCG ai_MissionSeed) */

unsigned int ai_MissionRandom(unsigned int n)
{
    ai_MissionSeed = ai_MissionSeed * 0x41c64e6d + 0x3039;
    return (*((unsigned short *)&ai_MissionSeed + 1) % 0x7fffu) % n;
}
/* 0x4b3a50, the running mission of m's owner */

G13MIS *ai_FindOwnerMission(G13MIS *m)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type && ai_Missions[i].owner == m->owner)
            return &ai_Missions[i];
    }
    return 0;
}
/* 0x4b3a90, restarts the running mission of m's owner now
   @hard read in full, byte-exact against the original (ai_FindOwnerMission, ai_Time into e->time, then
   0x18 / progress / fulfilled = 0); the only open gate is Q2 on G13MIS.unk18 (unk_4b39f0_types.h, a shared
   type he.c reads at 0x56ce40; a field rename is manual): it is written 0 here, in ai_NewMission (0x4b3ca0)
   and ai_SetFirstMission (0x4b3bd0), saved and loaded (0x4b62e0 / 0x4b63b0), and read by no drafted function
   and by no relocation to ai_Missions+0x18 (dcp phi 0xf82338: no object) -> a name would be invention, and its
   rename is a shared-header decision. progress (0x1c: the events ai_CountMissionEvent counts, compared with
   t->goal) and fulfilled (0x20: set by he.c once ai_Unk4b4f00 holds) are named. */

void ai_RestartMission(G13MIS *m)
{
    G13MIS *e = ai_FindOwnerMission(m);

    if (e != 0) {
        e->time = ai_Time;
        e->unk18 = 0;
        e->progress = 0;
        e->fulfilled = 0;
    }
}
/* 0x4b3ae0, the mission type type */

G13TYPE *ai_GetMissionType(int type)
{
    int i;

    for (i = 0; i < 48; i++) {
        if (ai_MissionTypes[i].type == type)
            return &ai_MissionTypes[i];
    }
    return 0;
}
/* 0x4b3b10, 1 if owner has a mission */

int ai_HasMission(int owner)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type && ai_Missions[i].owner == owner)
            return 1;
    }
    return 0;
}
/* 0x4b3b40, the number of mission types of kind (0..5, else -1) */

int ai_CountMissionTypes(int kind)
{
    int i, n;

    if (kind < 0 || kind > 5)
        return -1;
    n = 0;
    for (i = 0; i < 48; i++) {
        if (ai_MissionTypes[i].kind == kind)
            n++;
    }
    return n;
}
/* 0x4b3b70, the first mission type of kind (after `after` if given) @deep B3 draft, 6 differing instructions: the original materialises the loop end &ai_MissionTypes[47] into ecx once (mov ecx,X) and compares the entry check and the loop test against it; ours compares the immediate twice (cmp eax,X). Same instructions otherwise (9 hunks incl. fill, 27/96 bytes). Leads tried, one build each: the cached local (G13TYPE *last = &ai_MissionTypes[47]; two uses - the compiler still folds it to the immediate, 6), the exclusive end &ai_MissionTypes[48] with p < end (10, worse), the direct form (6, kept). spell.py: no menu rule matched. Readability done this session: ai_FindMissionType -> ai_FindMissionType (its own comment: the first mission type of kind after the previous one, ai_MissionTypes -> ai_MissionTypes (the 48-entry mission type table, header comment), so Q1/Q2 hold; the remaining hunk is register allocation of a constant address. */

G13TYPE *ai_FindMissionType(int kind, G13TYPE *after)
{
    G13TYPE *p;
    int i;

    if (after == 0) {
        for (i = 0; i < 48; i++) {
            if (ai_MissionTypes[i].kind == kind)
                return &ai_MissionTypes[i];
        }
        return 0;
    }
    if (after >= &ai_MissionTypes[47])
        return 0;
    for (p = after + 1; p <= &ai_MissionTypes[47]; p++) {
        if (p->kind == kind)
            return p;
    }
    return 0;
}

G13TYPE ai_MissionTypes[48] = { { 0, -1, -1, -1, -1, -1, -1 }, { 1, 0, 6913, 3, 3, 10000, -1 }, { 2, 0, 6916, 4, 4, 4, -1 }, { 3, 0, 6919, 17, 2, 3, 2880 }, { 4, 0, 6922, 1, 1, 3, 4 }, { 5, 1, 6925, 0, 14, 40000, -1 }, { 6, 1, 6928, 3, 18, 30000, -1 }, { 7, 1, 6931, 19, 19, 6, -1 }, { 8, 1, 6934, 9, 9, 5, 7200 }, { 9, 1, 6937, 16, 16, 2, 6 }, { 10, 1, 6940, 2, 17, 4, 2880 }, { 11, 1, 6943, 12, 12, 3, -1 }, { 12, 1, 6946, 10, 10, 3, -1 }, { 13, 2, 6949, 14, 14, 75000, -1 }, { 14, 2, 6952, 18, 18, 60000, -1 }, { 15, 2, 6955, 5, 5, 13, 5760 }, { 16, 2, 6958, 6, 6, 14, 5760 }, { 17, 2, 6961, 7, 7, 19, 5760 }, { 18, 2, 6964, 8, 8, 20, 5760 }, { 19, 2, 6967, 23, 23, 3, 4 }, { 20, 2, 6970, 11, 21, 40000, -1 }, { 21, 3, 6973, 14, 14, 150000, -1 }, { 22, 3, 6976, 18, 18, 125000, -1 }, { 24, 3, 6982, 17, 17, 5, 14400 }, { 25, 3, 6985, 21, 33, 90000, -1 }, { 26, 3, 6988, 22, 34, 70000, -1 }, { 27, 3, 6991, 35, 35, 80000, -1 }, { 28, 3, 6994, 25, 37, 4, -1 }, { 29, 3, 6997, 29, 29, 4, 6 }, { 30, 4, 7000, 27, 27, 250000, -1 }, { 31, 4, 7003, 28, 28, 200000, -1 }, { 32, 4, 7006, 18, 18, 175000, -1 }, { 33, 4, 7009, 30, 30, 6, 6 }, { 34, 4, 7012, 31, 31, 25, 4320 }, { 35, 4, 7015, 32, 32, 26, 4320 }, { 36, 4, 7018, 33, 33, 150000, -1 }, { 37, 4, 7021, 34, 34, 125000, -1 }, { 38, 4, 7024, 36, 36, -1, -1 }, { 39, 4, 7027, 37, 25, 0, -1 }, { 40, 4, 7030, 38, 38, 5, -1 }, { 41, 4, 7033, 39, 39, 90, -1 }, { 43, 5, 7039, 41, 41, 18, -1 }, { 44, 5, 7042, 42, 42, 3, 8640 }, { 45, 5, 7045, 43, 43, 0, -1 }, { 46, 5, 7048, 44, 44, 500, 1440 }, { 47, 5, 7051, 45, 45, 60, 0 }, { 48, 5, 7054, 47, 46, 0, -1 }, { 49, 5, 7057, 46, 47, 0, -1 } };  /* 0x601f88 */
char ai_MissionMode = -2;  /* 0x624580, the mission kind chosen (5: one mission only, -1: none) */

/* 0x4b3bd0, sets mission 0 (ai_Missions[0]): its owner and type, the time cleared */
void ai_SetFirstMission(int owner, int type)
{
    ai_Missions[0].type = type;
    ai_Missions[0].owner = owner;
    ai_Missions[0].time.year = 0;
    gm_SetTime(&ai_Missions[0].time, 0, 0, 0);
    ai_Missions[0].fulfilled = 0;
}
/* 0x4b3c10, gives owner a random mission of kind */

G13MIS *ai_NewRandomMission(int owner, int kind, int x)
{
    return ai_NewMission(owner, ai_RandomMissionType(kind), x);
}
/* 0x4b3c40, a random mission type of kind (0xff: none) */

int ai_RandomMissionType(int kind)
{
    int i, j, r, n;

    n = ai_CountMissionTypes(kind);
    if (n > 0) {
        r = ai_MissionRandom(n);
        for (i = 0, j = 0; i < 48; i++) {
            if (ai_MissionTypes[i].kind == kind)
                j++;
            if (j > r)
                return ai_MissionTypes[i].type;
        }
    }
    return 0xff;
}
/* 0x4b3ca0, gives owner a mission of type (in a free slot; game mode 5: always slot 0) */

G13MIS *ai_NewMission(int owner, int type, int x)
{
    int i;

    if (ai_MissionMode == 5) {
        ai_Missions[0].type = type;
        ai_Missions[0].owner = owner;
        ai_Missions[0].time.year = 0;
        gm_SetTime(&ai_Missions[0].time, 0, 0, 0);
        ai_Missions[0].fulfilled = 0;
        return &ai_Missions[0];
    }
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type == 0) {
            ai_Missions[i].type = type;
            ai_Missions[i].owner = owner;
            ai_Missions[i].time.year = 0;
            gm_SetTime(&ai_Missions[i].time, 0, 0, 0);
            ai_Missions[i].fulfilled = 0;
            return &ai_Missions[i];
        }
    }
    return 0;
}

int d2_OpenForm(int x, int y, char *name);                                     /* 0x4741f0 */
void d2_CentreViewWindows(int view);                                                     /* 0x475600 */
void d2_CloseView(int view);                                                     /* 0x4758f0 */
int d2_SetForm(int view, int n);                                                 /* 0x475f70 */
extern int d2_ClickedId;                /* 0xf8a668 */
int io_PrintText(char *fmt, ...);                                                /* 0x515960, defined elsewhere */
int sv_RunFrame(int cb, unsigned mask);                                      /* 0x5309a0, defined elsewhere */
int snd_PlayVoice(int id, int bank, char *text, int var);                             /* 0x4c2760, plays a speech sample */
int sb_IsPlaying(struct SBSAMPLE *s);                                                       /* 0x426f20 */
void sb_Stop(struct SBSAMPLE *s, int fade);                                                 /* 0x427290 */
extern int sv_PopRequest;                /* 0x6f22a0 */

/* 0x4b3d50, the mission offer dialog of the type: shows the mission form and its texts, plays the offer speech
   "_AUFTRAEGE_VERGABE_HS_nn", 1 when the player accepts */
int ai_MissionOfferDialog(int type, int id)
{
    char name[0x100];
    G13TYPE *t;
    int view, r;
    void *snd;

    r = 0;
    t = ai_GetMissionType(type);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintTextId(t->text + 1);
    io_PrintText("$N");
    io_PrintText((char *)0x7d);
    io_PrintText("$N");
    io_PrintText((char *)0x7e);
    sprintf(name, "_AUFTRAEGE_VERGABE_HS_%.2d", t->offerSpeech);
    snd = snd_PlayVoice(-7, 0, name, -1);
    while (sv_RunFrame((void (*)(void))ai_MissionOfferDialog, id)) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            r = 1;
            sv_PopRequest = 1;
        }
    }
    if (view != -1)
        d2_CloseView(view);
    if (sb_IsPlaying(snd))
        sb_Stop(snd, 0x19);
    return r;
}


typedef struct G13RADIO {               /* 0x8c bytes, d2.h's D2RADIO */
    int unk00;
    int n;                              /* 0x04 */
    int sel;                            /* 0x08, -1 = none */
    int objs[0x20];                     /* 0x0c */
} G13RADIO;

extern struct D2OBJ *d2_Objs;                 /* 0xf8f080 */
extern G13RADIO d2_Radios[8];           /* 0xfa5aa0 */
extern int d2_PointerObj;                /* 0x5fe710, focused object */
extern int d2_LastWindow;                /* 0x6ed938 */
extern unsigned char kb_Last;           /* 0x1468410 */
extern int sv_FrameMask;
void d2_CreateScrollButtons(int x, int y, int win, int obj, int pic);                      /* 0x472270 */
int d2_GetWinObj(int view, int idx, int k);                                      /* 0x475b80 */
int d2_CreateRadioGroup(int n, ...);                                             /* 0x46c490 */
void d2_RadioAdd(int g, int obj);                                                /* 0x46c560 */
void d2_RadioClear(int g);                                                       /* 0x46c600 */
void d2_MenuNavigate(int g);                                                        /* 0x46c6c0 */

/* 0x4b3e70, the dialog "special\CHOOSE_MISSION": lists the mission types of kind, offers the one picked
   (0x4b3d50); returns its type (0xff: none) */
int ai_ChooseMissionType(int kind)
{
    int chosen, view, win, g, o;
    G13TYPE *t;

    chosen = 0xff;
    view = d2_OpenForm(0, 0, "special\\CHOOSE_MISSION");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1ace);
    d2_SetForm(view, 2);
    win = d2_LastWindow;
    d2_SetForm(view, 0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
    d2_SetForm(view, 2);
    g = d2_CreateRadioGroup(0);
    for (t = ai_FindMissionType(kind, 0); t != 0; t = ai_FindMissionType(kind, t)) {
        o = d2_GetWinObj(view, 2, io_PrintText("%ia[%s]$N", t->text));
        d2_Objs[o].user = t;
        d2_RadioAdd(g, o);
    }
    while (sv_RunFrame((void (*)(void))ai_ChooseMissionType, 0x1c6)) {
        d2_MenuNavigate(g);
        if (dm_L.rup != 0 || kb_Last == 1 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_Radios[g].sel != -1
            && d2_PointerObj == d2_Radios[g].objs[d2_Radios[g].sel]) {
            t = (G13TYPE *)d2_Objs[d2_Radios[g].objs[d2_Radios[g].sel]].user;
            if (ai_MissionOfferDialog(t->type, sv_FrameMask)) {
                chosen = t->type;
                sv_PopRequest = 1;
            }
        }
    }
    d2_RadioClear(g);
    d2_CloseView(view);
    return chosen;
}

unsigned char ai_NumPlayers = 1;  /* 0x62489c */
void d2_RadioSelect(int g, int sel);                                             /* 0x46c630 */

/* 0x4b4040, the dialog "Menu\CHOOSEHISTORY": picks the mission kind ai_MissionMode (0..5, the first button:
   none) and then a mission (0x4b3e70); returns its type (0xff: cancelled)
   @deep behaviour: instructions identical under the esi<->edi renaming (tools/asmdiff.py --regs esi=edi,edi=esi leaves only the prologue/epilogue push/pop order), the 82-instruction code hunk at 0x4b4094 is not classified regs only because the joined hunk holds 7 registers (diffclass find_map limit 6). The swapped candidates are view (ours edi, original esi) and the first/o5/g colour (ours esi, original edi); c2prio: _first prio 182 -> esi, _view prio 181 -> edi, a 1-point allocation difference (the original ranks view above first). Last-bytes pass: spell.py dup-tail 0x4b4094 (no if/else arm at that block - difflib noise from the swap), push-pop 0x4b421f (one final return already; the original's je to the loop test is matched), order 0x4b4265 (same instruction set after the renaming); no meaning-class lead, no source form left. Tried: declaration order 'first, view' (no effect on C2 priorities). Readability done (Q1/Q2 cleared, Q6 present, Q7 clean). Left: C2 register-priority / colouring work (c2prio / vargen ren). */
int ai_ChooseMission(void)
{
    int o0, o1, o2, o3, o4, o5, o6;
    int view, first, g, r;

    r = 0;
    o6 = -1;
    view = d2_OpenForm(0, 0, "Menu\\CHOOSEHISTORY");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    first = io_PrintText((char *)0x1ac5, 0x1ac7, 0x1ac8, 0x1ac9, 0x1aca, 0x1acb, 0x1acc);
    o0 = d2_GetWinObj(view, 0, first);
    o1 = d2_GetWinObj(view, 0, first + 1);
    o2 = d2_GetWinObj(view, 0, first + 2);
    o3 = d2_GetWinObj(view, 0, first + 3);
    o4 = d2_GetWinObj(view, 0, first + 4);
    o5 = d2_GetWinObj(view, 0, first + 5);
    if (ai_NumPlayers > 1)
        o6 = d2_GetWinObj(view, 0, io_PrintText("%it[%s]", 0, 0x1acd));
    g = d2_CreateRadioGroup(6, o0, o1, o2, o3, o4, o5);
    d2_RadioSelect(g, 0);
    d2_RadioAdd(g, d2_GetWinObj(view, 0, io_PrintText((char *)0x7e)));
    while (sv_RunFrame((void (*)(void))ai_ChooseMission, sv_FrameMask)) {
        d2_MenuNavigate(g);
        if (dm_L.rup != 0 || kb_Last == 1 || d2_ClickedId == 1) {
            r = 0xff;
            sv_PopRequest = 1;
        }
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == o0) {
                ai_MissionMode = -1;
                r = 0;
            } else {
                if (d2_PointerObj == o1)
                    ai_MissionMode = 0;
                else if (d2_PointerObj == o2)
                    ai_MissionMode = 1;
                else if (d2_PointerObj == o3)
                    ai_MissionMode = 2;
                else if (d2_PointerObj == o4)
                    ai_MissionMode = 3;
                else if (d2_PointerObj == o5)
                    ai_MissionMode = 4;
                else if (d2_PointerObj == o6)
                    ai_MissionMode = 5;
                d2_RadioSelect(g, 0);
                r = ai_ChooseMissionType(ai_MissionMode);
                if (r == 0xff)
                    continue;
            }
            sv_PopRequest = 1;
        }
    }
    d2_RadioClear(g);
    d2_CloseView(view);
    return r;
}

void d2_SetObjEnabled(int obj, int on);                                              /* 0x475e40 */

/* 0x4b4280, 0x4b4040 with the first n kinds disabled (0x475e40) and a cancel button (returns 0xff); passes
   0x4b4040 to 0x5309a0 (sic) */
int ai_ChooseMissionFrom(int n)
{
    int o0, o1, o2, o3, o4, o5, o6;
    int view, first, g, r, sel;

    r = 0;
    view = d2_OpenForm(0, 0, "Menu\\CHOOSEHISTORY");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    first = io_PrintText((char *)0x1b9f);
    o0 = d2_GetWinObj(view, 0, first);
    o1 = d2_GetWinObj(view, 0, first + 1);
    o2 = d2_GetWinObj(view, 0, first + 2);
    o3 = d2_GetWinObj(view, 0, first + 3);
    o4 = d2_GetWinObj(view, 0, first + 4);
    o5 = d2_GetWinObj(view, 0, first + 5);
    o6 = d2_GetWinObj(view, 0, first + 6);
    g = d2_CreateRadioGroup(7, o0, o1, o2, o3, o4, o5, o6);
    sel = 1;
    if (n > 0) {
        d2_SetObjEnabled(o1, 0);
        sel = 2;
    }
    if (n > 1) {
        d2_SetObjEnabled(o2, 0);
        sel++;
    }
    if (n > 2) {
        d2_SetObjEnabled(o3, 0);
        sel++;
    }
    if (n > 3) {
        d2_SetObjEnabled(o4, 0);
        sel++;
    }
    if (n > 4) {
        d2_SetObjEnabled(o5, 0);
        sel = 0;
    }
    d2_RadioSelect(g, sel);
    while (sv_RunFrame((void (*)(void))ai_ChooseMission, sv_FrameMask)) {
        d2_MenuNavigate(g);
        if (d2_ClickedId == 0) {
            if (d2_PointerObj == o6) {
                ai_MissionMode = -1;
                r = 0xff;
            } else if (d2_PointerObj == o0) {
                ai_MissionMode = -1;
                r = 0;
            } else {
                if (d2_PointerObj == o1)
                    ai_MissionMode = 0;
                else if (d2_PointerObj == o2)
                    ai_MissionMode = 1;
                else if (d2_PointerObj == o3)
                    ai_MissionMode = 2;
                else if (d2_PointerObj == o4)
                    ai_MissionMode = 3;
                else if (d2_PointerObj == o5)
                    ai_MissionMode = 4;
                d2_RadioSelect(g, 0);
                r = ai_ChooseMissionType(ai_MissionMode);
                if (r == 0 || r == 0xff)
                    continue;
            }
            sv_PopRequest = 1;
        }
    }
    d2_RadioClear(g);
    d2_CloseView(view);
    return r;
}

#define G13_B(c, off) (((unsigned char *)(c))[off])
#define G13_501190 ((AIBLD *(*)(int, ...))gm_BuildQuery)  /* the variadic view (am.h repeats a 3-int one) */

int ai_TimedCondition(int x, int ok, int y);                                          /* 0x4b5160 */

/* 0x4b44e0, counts event ev (11, 19, 23, 28, 40) for the running mission of m's owner when it is of that type */
int ai_CountMissionEvent(G13MIS *m, int ev)
{
    G13MIS *e = ai_FindOwnerMission(m);

    if (e != 0) {
        switch (ev) {
        case 11:
        case 19:
        case 23:
        case 28:
        case 40:
            if (e->type == ev)
                e->progress++;
            return 1;
        }
    }
    return 0;
}
/* 0x4b4550, mission condition: the value of what 0x506430 finds for c is above t->goal
   @hard read in full, byte-exact (gm_PlayerRecord, 0 for none, gm_ConvertMoney(value +0x38, ai_OwnPlayer)
   > t->goal). Q1 / Q4 are both blocked on the five AIFIN money statistics at 0x2c .. 0x3c (ai.h AIFIN
   pad2c[0x14]): this condition reads +0x38, the three siblings ai_Unk4b4830 (0x2c + 0x34), ai_Unk4b4870
   (0x30) and ai_Unk4b48b0 (0x3c) read the others. No drafted function writes any of them (every
   player-record writer is named: madeValue, spyPaid, upgradeSpent, renovationSpent, officeSentences,
   roundFactors, the camera fields); ls_WritePlayerRecords (0x51a1a0) saves 0x2c / 0x30 / 0x34 and 0x3c,
   and 0x38 only for ls_Version > 0x10013, so the values come from saved data of an older build, and the
   only readers are these four mission conditions. The mission texts that would name the category (t->text,
   not in the repo) are the only remaining evidence; a field name (Q4 needs a typed AIFIN member) and a
   function name (Q1) would both be invention. Fixing Q4 alone means naming AIFIN+0x38, which Q2 then
   judges: that is the shared-header decision this hand-up is for. */

int ai_Unk4b4550(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x38), ai_OwnPlayer) > t->goal;
}
/* 0x4b4590, mission condition (types 2 / 7): the titles left in the job title group of c's title number
   (byte 0x164, gm_TitlesLeft) reach t->goal */

int ai_CondTitlesLeft(AICHAR *c, G13TYPE *t)
{
    return gm_TitlesLeft(G13_B(c, 0x164)) >= t->goal;
}
/* 0x4b45c0, mission condition (types 3 / 10 / 24): the character's rank (byte 0x0d, AICHAR.b00d) reaches
   t->goal, held for t->threshold minutes */

int ai_CondRankTimed(AICHAR *c, int x, G13TYPE *t)
{
    return ai_TimedCondition(x, G13_B(c, 0x0d) >= t->goal, t->threshold);
}
/* 0x4b45f0, mission condition: 0x50a640 of c (as money, 0x5085b0) reaches t->goal */

int ai_Unk4b45f0(AICHAR *c, G13TYPE *t)
{
    return gm_ConvertMoney(gm_GetWorth(c->id), ai_OwnPlayer) >= t->goal;
}
/* 0x4b4630, mission condition: 0x509ee0 of c (as money, 0x5085b0) reaches t->goal */

int ai_Unk4b4630(AICHAR *c, G13TYPE *t)
{
    return gm_ConvertMoney(gm_CharCurrency((void *)c, ai_OwnPlayer), ai_OwnPlayer) >= t->goal;
}
/* 0x4b4670, mission condition: at least t->goal of c's five values (bytes 0x80..0x84, / 42) reach t->threshold */

int ai_Unk4b4670(AICHAR *c, G13TYPE *t)
{
    int i, n;

    for (i = 0, n = 0; i < 5; i++) {
        if (G13_B(c, 0x80 + i) * 0.023809524f >= t->threshold)
            n++;
    }
    return n >= t->goal;
}
/* 0x4b46c0, mission condition: 0x4b5160 with c's byte 0x166 equal to t->goal */

int ai_Unk4b46c0(AICHAR *c, int x, G13TYPE *t)
{
    return ai_TimedCondition(x, G13_B(c, 0x166) == t->goal, t->threshold);
}

int gm_BuildingTypeOfJob(int k);                                                         /* 0x503fa0, defined elsewhere */

/* 0x4b46f0, mission condition: c has a building (kind >= 5, owner c) older than t->threshold minutes */
int ai_Unk4b46f0(AICHAR *c, G13TYPE *t)
{
    AIBLD *b;

    for (b = gm_BuildQuery(2, 4, c->id, 5, 2); b != 0; b = gm_BuildNext()) {
        if (G13_B(b, 0) >= 5 && *(int *)((char *)b + 0x65) == c->netId
            && gm_MinutesBetween((AITIME *)((char *)b + 0x69), &ai_Time) > t->threshold)
            return 1;
    }
    return 0;
}
/* 0x4b4760, mission condition: c has three buildings of the building type of its job (gm_BuildingTypeOfJob
   of AICHAR.jobId +0x164) that stock every needed good of their type's list (AIBTYPE.slot +0x23 of the
   0x24d-byte records at ai_BTypes, bit 15 = flag)
   @deep behaviour: one code hunk left, 3 instructions (44 -> 5 differing asm lines this session; the loop is matched): our lea edi,[eax+edx*4+0x23] folds the AIBTYPE.slot member offset into the SIB displacement, the original materializes the record address (lea eax,[eax+edx*4]) and adds the member offset in a second lea (lea edi,[eax+0x23]); the fill 2 hunk is the same 2-byte size difference. The real difference was the loop condition: the original source is 'for (b = gm_BuildQuery(...); n < 3 && b != 0; b = gm_BuildNext())' with 'n += ok' at the end of the body (other spellings tried and reverted: increment in the body + in-body 'if (n >= 3) return 1;', 'b != 0 && n < 3', continue + inverted tail, while - each 33-62 differing instructions). Tried for the slot address, one build each, all fold to one lea: ai_BTypes[b->type].slot (kept, typed), &ai_BTypes[b->type].slot[0], (unsigned short *)((char *)&ai_BTypes[b->type] + 0x23), char* record local + 0x23, AIBTYPE *bt = ai_BTypes + b->type then (char *)bt + 0x23, two-step w assignment. spell.py: the code hunk has no menu rule; meaning checked (same address, typed AIBTYPE.slot). Readability done (Q1/Q2/Q4 cleared, Q6 description, Q7 clean). */

int ai_HasThreeStockedBuildings(AICHAR *c, G13TYPE *t)
{
    AIBLD *b;
    unsigned short *w;
    int k, n, j, ok;

    k = gm_BuildingTypeOfJob(c->jobId);
    if (k == 0)
        return 0;
    n = 0;
    for (b = gm_BuildQuery(2, 4, c->id, 5, k); n < 3 && b != 0; b = gm_BuildNext()) {
        ok = 1;
        w = ai_BTypesT[b->type].slot;
        for (j = 0; j < 0x40 && *w != 0; j++, w++) {
            if (!gm_Query(b->objList, 2, 6, 0, *w & 0x7fff)) {
                ok = 0;
                break;
            }
        }
        n += ok;
    }
    if (n >= 3)
        return 1;
    return 0;
}
/* 0x4b4830, mission condition: values 0x2c + 0x34 of what 0x506430 finds for c are above t->goal */

int ai_Unk4b4830(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x2c) + *(int *)((char *)f + 0x34), ai_OwnPlayer) > t->goal;
}
/* 0x4b4870, the same with value 0x30 */

int ai_Unk4b4870(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x30), ai_OwnPlayer) > t->goal;
}
/* 0x4b48b0, the same with value 0x3c */

int ai_Unk4b48b0(AICHAR *c, G13TYPE *t)
{
    AIFIN *f = gm_PlayerRecord(c);

    if (f == 0)
        return 0;
    return gm_ConvertMoney(*(int *)((char *)f + 0x3c), ai_OwnPlayer) > t->goal;
}

typedef struct G13STAT {                /* 0x0c bytes, filled by 0x4b51f0 */
    int same;                           /* 0x00, the holders that match (the fillers' local same) */
    int n;                              /* 0x04 */
    float v;                            /* 0x08 */
} G13STAT;

void ai_CharBuildingShare(AICHAR *c, int k, G13STAT *s);                                 /* 0x4b51f0 */

/* 0x4b48f0, mission condition: the share (in percent) of the characters (kind < 10) with c's byte 0x0c
   reaches t->goal */
int ai_CondCharShare(AICHAR *c, G13TYPE *t)
{
    int i, n, same;

    n = 0;
    same = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].kind < 10) {
            n++;
            if (G13_B(&ai_CharsT[i], 0x0c) == G13_B(c, 0x0c))
                same++;
        }
    }
    if ((float)same / n * 100.0f >= t->goal)
        return 1;
    return 0;
}
/* 0x4b4960, mission condition: 0x4b51f0 values 0x13, 4 and 0x10 of c reach 1 (4 / 0x10 only if present) */

int ai_Unk4b4960(AICHAR *c, G13TYPE *t)
{
    G13STAT s;

    ai_CharBuildingShare(c, 0x13, &s);
    if (s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 4, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 0x10, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    return 1;
}
/* 0x4b4a00, mission condition: 0x4b51f0 values 0x15, 0x14 and 0x12 of c: at least 3 each, all reaching 1 */

int ai_Unk4b4a00(AICHAR *c, G13TYPE *t)
{
    G13STAT s;

    ai_CharBuildingShare(c, 0x15, &s);
    if (s.n < 3 || s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 0x14, &s);
    if (s.n < 3 || s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 0x12, &s);
    if (s.n < 3 || s.v < 1.0f)
        return 0;
    return 1;
}
/* 0x4b4aa0, mission condition: 0x4b45c0 and at least three buildings of kind 7 */

int ai_Unk4b4aa0(AICHAR *c, int x, G13TYPE *t)
{
    AIBLD *b;
    int n;

    if (!ai_CondRankTimed(c, x, t))
        return 0;
    n = 0;
    for (b = gm_BuildQuery(2, 4, c->id, n, 7); b != 0; b = gm_BuildNext())
        n++;
    return n >= 3;
}

void ai_PlayerShare(int k, G13STAT *s);                                            /* 0x4b5270 */

/* 0x4b4af0, mission condition: values 0x15, 0x14, 0x12, 0x16, 8 and 9 of 0x4b5270 all reach 1 (where present)
   and their counts add up to t->goal */
int ai_PlayerHeldBuildings(AICHAR *c, G13TYPE *t)
{
    G13STAT s;
    int n;

    ai_PlayerShare(0x15, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n = s.same;
    ai_PlayerShare(0x14, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(0x12, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(0x16, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(8, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    n += s.same;
    ai_PlayerShare(9, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    return s.same + n >= t->goal;
}
/* 0x4b4c50, mission condition: 0x4b5160 with "values 0x0b, 0x0c and 0x0d of 0x4b5270 reach 1 (where present)
   and t->goal <= 3" */

int ai_Unk4b4c50(AICHAR *c, int x, G13TYPE *t)
{
    G13STAT s;

    ai_PlayerShare(0x0b, &s);
    if (s.n > 0 && s.v < 1.0f)
        return ai_TimedCondition(x, 0, t->threshold);
    ai_PlayerShare(0x0c, &s);
    if (s.n > 0 && s.v < 1.0f)
        return ai_TimedCondition(x, 0, t->threshold);
    ai_PlayerShare(0x0d, &s);
    if (s.n > 0 && s.v < 1.0f)
        return ai_TimedCondition(x, 0, t->threshold);
    return ai_TimedCondition(x, t->goal <= 3, t->threshold);
}
/* 0x4b4d50, mission condition: value 0 of 0x4b5270 reaches t->goal percent */

int ai_CondPlayerShare(AICHAR *c, G13TYPE *t)
{
    G13STAT s;

    ai_PlayerShare(0, &s);
    if (t->goal * 0.01f <= s.v)
        return 1;
    return 0;
}

float ord_WeightedRecordSum(int *out);                                                   /* 0x4f9d40, defined elsewhere */

/* 0x4b4d90, mission condition: 0x4f9d40 gives 0 */
int ai_Unk4b4d90(AICHAR *c, G13TYPE *t)
{
    char buf[0x30];

    if (ord_WeightedRecordSum(buf) == 0.0f)
        return 1;
    return 0;
}

typedef struct G13ECON {                /* 0x28 bytes, filled by 0x4f9e00 */
    float unk00;
    float unk04;                        /* 0x04 */
    float unk08;                        /* 0x08 */
    float unk0c;                        /* 0x0c */
    char pad10[8];
    float unk18;                        /* 0x18 */
    char pad1c[0x0c];
} G13ECON;

struct VFSFILE; float ord_GetStatistics(int *o);                                                /* 0x4f9e00, defined elsewhere */

/* 0x4b4dc0, mission condition (from year 10): 0x4b5160 with "t->goal is not below value 0x18 of 0x4f9e00" */
int ai_Unk4b4dc0(AICHAR *c, int x, G13TYPE *t)
{
    G13ECON e;

    if (ai_Time.year < 10)
        return 0;
    ord_GetStatistics(&e);
    if (t->goal < e.unk18)
        return ai_TimedCondition(x, 0, t->threshold);
    return ai_TimedCondition(x, 1, t->threshold);
}
/* 0x4b4e30, mission condition: 0x4f9e00 at most 0.1 and its values 0x0c, 0x04, 0x08 at least 0.3 */

int ai_Unk4b4e30(AICHAR *c, G13TYPE *t)
{
    G13ECON e;

    if (ord_GetStatistics(&e) > 0.1)
        return 0;
    if (e.unk0c < 0.3f)
        return 0;
    if (e.unk04 < 0.3f)
        return 0;
    if (e.unk08 < 0.3f)
        return 0;
    return 1;
}
/* 0x4b4eb0, mission condition: no building of the 0x501190(1, 6) list is held by a character of kind 5 */

int ai_Unk4b4eb0(AICHAR *c, G13TYPE *t)
{
    AIBLD *b;
    unsigned short w;

    for (b = G13_501190(1, 6); b != 0; b = gm_BuildNext()) {
        w = *(unsigned short *)((char *)b + 0x27);
        if (w != 0xffff && ai_CharsT[w].kind == 5)
            return 0;
    }
    return 1;
}

extern unsigned char ai_GameState;      /* 0x6f25ac, missions on */
int ai_Unk6f0728 = 0;  /* 0x6f0728 */

/* 0x4b4f00, 1 if mission m is fulfilled (the condition of its type; 11 / 19 / 23 / 28 / 40: events counted by
   0x4b44e0) */
int ai_Unk4b4f00(G13MIS *m)
{
    G13TYPE *t;
    AICHAR *c;

    t = ai_GetMissionType(m->type);
    if (!ai_GameState || t == 0)
        return 0;
    if (ai_Unk6f0728 == 1 && ai_MissionMode >= 0 && ai_MissionMode <= 5) {
        ai_Unk6f0728 = 0;
        return 1;
    }
    c = gm_FindChar(m->owner);
    if (c == 0)
        return 0;
    switch (m->type) {
    case 1:
    case 6:
    case 14:
    case 22:
    case 32:
        return ai_Unk4b4550(c, t);
    case 2:
    case 7:
        return ai_CondTitlesLeft(c, t);
    case 3:
    case 10:
    case 24:
        return ai_CondRankTimed(c, (int)m, t);
    case 5:
    case 13:
    case 21:
    case 30:
        return ai_Unk4b45f0(c, t);
    case 31:
        return ai_Unk4b4630(c, t);
    case 4:
    case 9:
    case 29:
        return ai_Unk4b4670(c, t);
    case 15:
    case 16:
    case 17:
    case 18:
    case 34:
    case 35:
        return ai_Unk4b46c0(c, (int)m, t);
    case 11:
    case 19:
    case 23:
    case 28:
    case 40:
        return m->progress >= t->goal;
    case 8:
        return ai_Unk4b46f0(c, t);
    case 12:
        return ai_HasThreeStockedBuildings(c, t);
    case 20:
    case 25:
    case 36:
        return ai_Unk4b4830(c, t);
    case 26:
    case 37:
        return ai_Unk4b4870(c, t);
    case 27:
        return ai_Unk4b48b0(c, t);
    case 41:
        return ai_CondCharShare(c, t);
    case 39:
        return ai_Unk4b4960(c, t);
    case 38:
        return ai_Unk4b4a00(c, t);
    case 33:
        return ai_Unk4b4aa0(c, (int)m, t);
    case 43:
        return ai_PlayerHeldBuildings(c, t);
    case 44:
        return ai_Unk4b4c50(c, (int)m, t);
    case 45:
        return ai_Unk4b4e30(c, t);
    case 46:
        return ai_Unk4b4dc0(c, (int)m, t);
    case 47:
        return ai_CondPlayerShare(c, t);
    case 48:
        return ai_Unk4b4d90(c, t);
    case 49:
        return ai_Unk4b4eb0(c, t);
    }
    return 0;
}

#define G13_4FE1E0 ((void (*)(AITIME *, char, char, char))gm_SetTime)

/* 0x4b5160, timed condition of mission x: not ok resets its time; ok starts it (once) and holds after min
   minutes */
int ai_TimedCondition(int x, int ok, int min)
{
    G13MIS *m = (G13MIS *)x;

    if (ok) {
        if (m->time.year == 0 && m->time.hour == 0 && *(int *)((char *)m + 0x0e) == 0
            && *(int *)((char *)m + 0x12) == 0) {
            m->time.year = ai_Time.year;
            G13_4FE1E0(&m->time, (char)ai_Time.hour, (char)ai_Time.min, (char)ai_Time.sec);
        }
        return gm_MinutesBetween(&m->time, &ai_Time) >= min;
    }
    m->time.year = 0;
    gm_SetTime(&m->time, 0, 0, 0);
    return 0;
}
/* 0x4b51f0, the share of c among the holders of the buildings of the 0x501190(1, 5, k) list (k 0: (1, 6)) */

void ai_CharBuildingShare(AICHAR *c, int k, G13STAT *s)
{
    AIBLD *b;
    int same, n;

    same = 0;
    if (k != 0)
        b = G13_501190(1, 5, k);
    else
        b = G13_501190(1, 6);
    n = 0;
    for (; b != 0; b = gm_BuildNext(), n++) {
        if (*(unsigned short *)((char *)b + 0x27) == c->id)
            same++;
    }
    s->same = same;
    s->n = n;
    if (n > 0)
        s->v = (float)same / n;
    else
        s->v = 0.0f;
}
/* 0x4b5270, the share of players (kind 6 / 7) among the holders of the buildings of the 0x501190(1, 5, k) list
   (k 0: (1, 6)) */

void ai_PlayerShare(int k, G13STAT *s)
{
    AIBLD *b;
    unsigned short w;
    int same, n;

    same = 0;
    if (k != 0)
        b = G13_501190(1, 5, k);
    else
        b = G13_501190(1, 6);
    n = 0;
    for (; b != 0; b = gm_BuildNext(), n++) {
        w = *(unsigned short *)((char *)b + 0x27);
        if (w != 0xffff && (ai_CharsT[w].kind == 6 || ai_CharsT[w].kind == 7))
            same++;
    }
    s->same = same;
    s->n = n;
    if (n > 0)
        s->v = (float)same / n;
    else
        s->v = 0.0f;
}

void ai_MissionInfoDialog(int type, int id);

/* 0x4b5310, shows the missions of owner again (0x4b5360) */
void ai_ShowMissions(int owner)
{
    int i, id;

    id = sv_FrameMask | 0x80;
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == owner)
            ai_MissionInfoDialog(ai_Missions[i].type, id | 0x80);
    }
}
/* 0x4b5360, the mission dialog of type type, information only (cf. ai_MissionOfferDialog) */

void ai_MissionInfoDialog(int type, int id)
{
    char name[0x100];
    G13TYPE *t;
    int view;
    void *snd;

    t = ai_GetMissionType(type);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintTextId(t->text + 1);
    io_PrintText("$N");
    io_PrintText((char *)0x7d);
    sprintf(name, "_AUFTRAEGE_VERGABE_HS_%.2d", t->offerSpeech);
    snd = snd_PlayVoice(-7, 0, name, -1);
    while (sv_RunFrame((void (*)(void))ai_MissionInfoDialog, id)) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    if (view != -1)
        d2_CloseView(view);
    if (sb_IsPlaying(snd))
        sb_Stop(snd, 0x19);
}

extern volatile unsigned long d3_Time;  /* 0x6333cc, milliseconds */
int sb_IsInit(void);                                                          /* 0x4273c0, sound on */
void snd_ClearQueue(void);                                                         /* 0x4c01b0, defined elsewhere */

/* G13_PLAY: plays snd until it ends or button ok is pressed; without sound waits 250 ms (or the button) */
#define G13_PLAY(snd, ok, id) {                                     \
    if (sb_IsPlaying(snd)) {                                        \
        do {                                                        \
            sv_RunFrame(0, (id) | 0x80);                           \
            if (d2_ClickedId != -1 && d2_PointerObj == (ok)) {       \
                if (sb_IsPlaying(snd))                              \
                    sb_Stop(snd, 1);                                \
                break;                                              \
            }                                                       \
        } while (sb_IsPlaying(snd));                                \
    }                                                               \
}
#define G13_WAIT(ok, id) {                                          \
    unsigned long _end = d3_Time + 0xfa;                            \
    while (_end > d3_Time) {                                        \
        sv_RunFrame(0, (id) | 0x80);                               \
        if (d2_ClickedId != -1 && d2_PointerObj == (ok))             \
            break;                                                  \
    }                                                               \
}

/* 0x4b5460, the mission success dialog of t: three general speeches ("AUFTRAEGE_ALLGEMEIN" 0..2), then
   "_AUFTRAEGE_ERFOLG_HS_nn", each cut short by the button
   @deep behaviour: 4 code hunks (20 differing instructions), all one pattern: 'unsigned long _end = d3_Time + 0xfa;' compiles here as mov ebx,[d3_Time]; add ebx,0xfa, the original as mov eax,[d3_Time]; lea ebx,[eax+0xfa] (the load goes to a temp, the sum into _end); the following load for 'while (_end > d3_Time)' and every other instruction match (the loop structure top test + bottom cmp ebx,[d3_Time] is identical). Meaning checked: same end time, same volatile reads. Tried, one build each: 0xfa + d3_Time (identical bytes), the inline while (d3_Time + 0xfa > d3_Time) without the local (much WORSE: 578 -> 606 bytes, 20 -> 222 asm lines, reverted). spell.py: the four code hunks have no menu rule (meaning check first); the dup-tail hunk 0x4b58ac is the alignment fill of the 4-instruction size difference (no if/else at the tail). Readability done this session: Q1/Q2 cleared (sb_IsInit is the canonical sblib.c name of 0x4273c0, G13TYPE.unk10 -> speech). */
void ai_MissionSuccessDialog(G13TYPE *t, int id)
{
    char name[0x100];
    int view, ok;
    void *snd;

    snd_ClearQueue();
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b9b);
    d2_SetForm(view, 2);
    io_PrintText("$C");
    ok = d2_GetWinObj(view, 2, io_PrintText((char *)0x7d));
    if (sb_IsInit()) {
        snd = snd_PlayVoice(-8, 0, "AUFTRAEGE_ALLGEMEIN", 0);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b9c);
    if (sb_IsInit()) {
        snd = snd_PlayVoice(-8, 0, "AUFTRAEGE_ALLGEMEIN", 1);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b9d);
    if (sb_IsInit()) {
        snd = snd_PlayVoice(-8, 0, "AUFTRAEGE_ALLGEMEIN", 2);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_SetForm(view, 1);
    io_PrintText("$C");
    io_PrintText((char *)0x1b9e);
    io_PrintTextId(t->text + 2);
    if (sb_IsInit()) {
        sprintf(name, "_AUFTRAEGE_ERFOLG_HS_%.2d", t->speech);
        snd = snd_PlayVoice(-7, 0, name, -1);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_CloseView(view);
}
/* 0x4b58b0, the dialog that ends the mission chain (text 0x1b99, the same text the result dialogs 0x4b5970 /
   0x4b5c20 show as their "no more missions" option): shown by 0x4b5f10 after the result dialog when the
   mission ended outside mission mode 5, one OK form, pops on a click or a mouse move
   @deep behaviour draft is one register decision: the original keeps the literal 1 in ebx (push ebx/pop ebx in the prologue, mov ebx,1; push ebx for d2_SetForm(view,1), mov [sv_PopRequest],ebx in the loop), ours emits push 1 / mov [sv_PopRequest],1; the slots [esp+0x38]->[esp+0x3c] and the fill hunks follow from the extra ebx push. The loop body reads the same (one shared store of the || form). spell.py has no menu rule on the code hunks. Tried, one edit + one build each, reverted with the bytes unchanged: 'ok = 1; d2_SetForm(view, ok); ... sv_PopRequest = ok', 'ok = 1; d2_SetForm(view, 1); ... sv_PopRequest = ok', 'for (ok = 1; sv_RunFrame(...); ) { ... sv_PopRequest = ok; }', and the 2026-10-25 'ok = 1' local: VC6 constant-folds every one (no ebx at all). Left: the constant materialisation / register allocation (c2prio: const#0 not materialised); the only matched sibling with this idiom (ai_MissionOfferDialog) gets ebx from 'r = 1', a variable with two definitions that is returned. */

void ai_MissionEndDialog(int id)
{
    int view;

    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b99);
    io_PrintText("$N");
    d2_SetForm(view, 2);
    io_PrintText((char *)0x7d);
    id |= 0x80;
    while (sv_RunFrame((void (*)(void))ai_MissionEndDialog, id)) {
        if (dm_L.rup != 0 || d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(view);
}

extern int ai_GameOver;
void sv_PopTo(int v);                                             /* 0x52fb50, defined elsewhere */
void main_GameLoop(int ctl);                                                         /* 0x53bb20, defined elsewhere */

/* 0x4b5950, sets ai_GameOver and runs 0x53bb20 through 0x52fb50 */
void ai_QuitGame(int x)
{
    ai_GameOver = 1;
    sv_PopTo(main_GameLoop);
}

int cm_SendExSysMessage(unsigned char id, void *msg);                                             /* 0x4c4db0, defined elsewhere (hmt.h: void) */
int cm_GetSlotState(int h);                                                         /* 0x4c3ae0, defined elsewhere */
void sv_PumpNet(void);                                                         /* 0x52fe00, defined elsewhere */
int sb_UnloadSampleBank(struct SBBANK *bank);                                             /* 0x426ba0 */
void * ai_MissionSampleBank = 0;  /* 0x6f072c, a sample bank */

/* 0x4b5970, the mission complete dialog (after 0x4b5460): a new mission of the same kind (message 0x0a, then
   0x4b5360), the end of the missions, or quitting (0x4b5950)
   @deep behaviour draft (readable): 157 differing instructions, code 195 - allocation only. Read every hunk against tools/disasm.py 0x4b5970: the three msg branches, the merged sv_PopRequest tail and the closing again/quit/sample-bank block are the same statements; ours spills h to a stack slot where the original keeps it in ebx (frame 0x194 vs 0x190, exactly h's 4 bytes: 8 locals vs 7), keeps the shared 0-constant in esi instead of ebx, and flags in ebx instead of eax. spell.py menu: reload x2 (prologue/slot reads - the frame shift, nothing to drop: id is the parameter and is read once; h is needed twice per branch), dup-tail (the sv_PopRequest store is already shared on both sides), width (no 16/32-bit field read found). Leads tried, one edit + one build each, all still 157: G13TYPE *t at function scope instead of block scope; int h declared inside the do body; nm/again/quit as declaration initializers (zero web). c2prio unavailable (no frida). What is left is the register/stack allocation of h and flags - match-deep's slot/priority work. */
void ai_MissionCompleteDialog(int id)
{
    int msg1[0x1f];
    int msg2[0x1f];
    int msg3[0x1f];
    AICHAR *c;
    G13MIS *nm;
    int again, quit, o1, o2, o3, view, flags, h;

    c = &ai_CharsT[ai_PlayerId];
    nm = 0;
    again = 0;
    quit = 0;
    {
        G13TYPE *t = ai_GetMissionType(ai_Missions[0].type);

        memset(&ai_Missions[0], 0, sizeof(G13MIS));
        id = (id & ~9) | 0x100;
        ai_MissionSuccessDialog(t, id);
    }
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b95, c->id);
    o1 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b98));
    o2 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b99));
    o3 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b9a));
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == o1) {
                nm = ai_NewRandomMission(c->netId, ai_MissionMode, 1);
                msg1[0] = c->netId;
                msg1[4] = 0;
                again = 1;
                msg1[8] = nm->type;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg1);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o2) {
                ai_MissionMode = -1;
                msg2[0] = c->netId;
                msg2[4] = 0;
                msg2[8] = 0;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg2);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o3) {
                ai_MissionMode = -1;
                msg3[0] = c->netId;
                quit = 1;
                msg3[4] = 1;
                msg3[8] = 0;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg3);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionCompleteDialog, flags));
    d2_CloseView(view);
    if (again)
        ai_MissionInfoDialog(nm->type, id);
    else if (quit)
        ai_QuitGame(id);
    if (ai_MissionSampleBank != 0)
        sb_UnloadSampleBank(ai_MissionSampleBank);
    ai_MissionSampleBank = 0;
}
/* 0x4b5c20, the mission complete dialog of the player's running mission: a mission of the next kind (only for
   kinds 0..3; message 0x0a, then 0x4b5360), the end of the missions, or quitting (0x4b5950) */

void ai_MissionNextDialog(int id)
{
    int msg1[0x1f];
    int msg2[0x1f];
    AICHAR *c;
    G13MIS *nm;
    G13TYPE *t;
    int next, again, quit, o1, o2, o3, view, flags, h, i;

    o1 = -1;
    next = -1;
    again = 0;
    quit = 0;
    t = 0;
    c = &ai_CharsT[ai_PlayerId];
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == c->netId) {
            t = ai_GetMissionType(ai_Missions[i].type);
            next = t->kind + 1;
            memset(&ai_Missions[i], 0, sizeof(G13MIS));
            break;
        }
    }
    id = (id & ~9) | 0x100;
    ai_MissionSuccessDialog(t, id);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b94, c->id);
    if (next > -1 && next < 4)
        o1 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b98));
    o2 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b99));
    o3 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b9a));
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == o1) {
                again = 1;
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o2) {
                ai_MissionMode = -1;
                msg2[0] = c->netId;
                msg2[4] = 0;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg2);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o3) {
                ai_MissionMode = -1;
                msg1[0] = c->netId;
                quit = 1;
                msg1[4] = 1;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg1);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionNextDialog, flags));
    d2_CloseView(view);
    if (again) {
        nm = ai_NewRandomMission(c->netId, next, 1);
        msg1[0] = c->netId;
        msg1[4] = 0;
        h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg1);
        while (!cm_GetSlotState(h))
            sv_PumpNet();
        ai_MissionInfoDialog(nm->type, id);
        return;
    }
    if (quit)
        ai_QuitGame(id);
}
/* 0x4b5ef0, the mission complete dialog of the game mode (5: 0x4b5970, else 0x4b5c20) */

void ai_MissionCompleteByMode(int id)
{
    if (ai_MissionMode == 5)
        ai_MissionCompleteDialog(id);
    else
        ai_MissionNextDialog(id);
}

int ai_MissionPartner = -1;  /* 0x6248a0, the mission partner (-1 none) */

/* 0x4b5f10, the mission result dialog of mission 0 (0x4b5460, text 0x1b95); then the mission again (0x4b5360),
   the mission end dialog (0x4b58b0, game mode not 5) or quitting (0x4b5950, ai_GameOver)
   @deep behaviour draft (readable): a pure callee-saved colour swap. asmdiff --regs ebp=ebx,ebx=ebp leaves only the push order of the ebp/ebx pair, the [ebp+0] vs [ebx] addressing byte, our 32-bit or ebp,0x80 against the original's or bl,0x80 (consequence: no bpl byte form in 32-bit) and the 4-byte tail fill. Read: c (&ai_Chars[ai_PlayerId]) and t (ai_GetMissionType) are the two swapped candidates; ours c=ebp/t=ebx, the original c=ebx/t=ebp; flags and the -1 of d2_ClickedId inherit c's register. spell.py menu has no regs entry; leads tried, one edit + one build each: t declared before c (allocation unchanged, reverted), t assigned before c (c -> ebx but the ai_GetMissionType call moves ahead of the lea and the prologue reorders: 20 -> 32 differing instructions, reverted). c2prio needs frida (not installed). The swap needs the priority/slot work. */
void ai_MissionResultDialog(int id)
{
    AICHAR *c;
    G13TYPE *t;
    int r, view, flags;

    r = 0;
    c = &ai_CharsT[ai_PlayerId];
    t = ai_GetMissionType(ai_Missions[0].type);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 0);
    id = (id & ~9) | 0x100;
    ai_MissionSuccessDialog(t, id);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b95, c->id);
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (ai_MissionPartner == -1) {
            if (ai_GameOver)
                r = 3;
            else
                r = (ai_MissionMode != 5) + 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionResultDialog, flags));
    d2_CloseView(view);
    if (r == 1)
        ai_MissionInfoDialog(ai_Missions[0].type, id);
    else if (r == 2)
        ai_MissionEndDialog(id);
    else if (r == 3)
        ai_QuitGame(id);
}
/* 0x4b6040, the mission dialog with text 0x1b97 for the character ai_MissionPartner (another player's mission);
   quits (0x4b5950) when ai_GameOver */

void ai_MissionPartnerDialog(int id)
{
    AICHAR *c;
    int r, view, flags;

    r = 0;
    c = gm_FindChar(ai_MissionPartner);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b97, c->id, c->id);
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (ai_MissionPartner == -1) {
            r = (ai_GameOver != 0) + 1;
            sv_PopRequest = 1;
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionPartnerDialog, flags));
    d2_CloseView(view);
    if (r == 2)
        ai_QuitGame(id);
}
/* 0x4b60f0, the other result dialog of the game mode (5: 0x4b5f10, else 0x4b6040) */

void ai_MissionResultByMode(int id)
{
    if (ai_MissionMode == 5)
        ai_MissionResultDialog(id);
    else
        ai_MissionPartnerDialog(id);
}

void main_PlayOutro(void);                                                         /* 0x53cb40, defined elsewhere */

/* 0x4b6110, the single player's mission complete sequence: 0x4b5460, then the choice of the next mission
   (0x4b4280 from the next kind on): none (message 0x0a), cancel (message 0x0a with 1, then 0x4b5950) or the
   new mission (0x4b3ca0, message 0x0a) */
void ai_MissionCompleteSingle(int id)
{
    int msg[0x1f];
    AICHAR *c;
    G13TYPE *t;
    int next, r, h, i;

    t = 0;
    next = -1;
    c = &ai_CharsT[ai_PlayerId];
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type != 0 && ai_Missions[i].owner == c->netId) {
            t = ai_GetMissionType(ai_Missions[i].type);
            next = t->kind + 1;
            memset(&ai_Missions[i], 0, sizeof(G13MIS));
            break;
        }
    }
    id = (id & ~9) | 0x100;
    ai_MissionSuccessDialog(t, id);
    main_PlayOutro();
    r = ai_ChooseMissionFrom(next);
    if (r == 0) {
        ai_MissionMode = -1;
        msg[0] = c->netId;
        msg[4] = 0;
        h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg);
        while (!cm_GetSlotState(h))
            sv_PumpNet();
        return;
    }
    if (r == 0xff) {
        ai_MissionMode = -1;
        msg[0] = c->netId;
        msg[4] = 1;
        h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg);
        while (!cm_GetSlotState(h))
            sv_PumpNet();
        ai_QuitGame(id);
        return;
    }
    ai_NewMission(c->netId, r, 1);
    msg[0] = c->netId;
    msg[4] = 0;
    h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg);
    while (!cm_GetSlotState(h))
        sv_PumpNet();
}
/* 0x4b62a0, the mission complete sequence: single player 0x4b6110; network: 0x4b5ef0 for the player who did
   it (ai_MissionPartner), 0x4b60f0 for the others */

void ai_MissionCompleteSequence(int id)
{
    if (net_Conn.s != -1) {
        if (ai_CharsT[ai_PlayerId].netId == ai_MissionPartner)
            ai_MissionCompleteByMode(id);
        else
            ai_MissionResultByMode(id);
    } else
        ai_MissionCompleteSingle(id);
}

unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);                             /* 0x430e30 */

/* 0x4b62e0, saves the mission kind and the running missions to f */
int ai_SaveMissions(void *f)
{
    int i;

    if (f == 0)
        return 0;
    if (vfs_fwrite(&ai_MissionMode, 1, 1, f) == -1)
        return 0;
    for (i = 0; i < 0x80; i++) {
        if (vfs_fwrite(&ai_Missions[i].type, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].owner, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].time, 0x0e, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].unk18, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].progress, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].fulfilled, 1, 1, f) == -1)
            return 0;
    }
    return 1;
}

int ls_Read(void *buf, int size, int n, int f);                             /* 0x51c920, ls.c: true when the fread returned a positive count */

/* 0x4b63b0, loads the mission kind and the running missions from f (cf. 0x4b62e0) */
int ai_Unk4b63b0(void *f)
{
    int i;

    if (f == 0)
        return 0;
    if (!ls_Read(&ai_MissionMode, 1, 1, f))
        return 0;
    for (i = 0; i < 0x80; i++) {
        if (!ls_Read(&ai_Missions[i].type, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].owner, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].time, 0x0e, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].unk18, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].progress, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].fulfilled, 1, 1, f))
            return 0;
    }
    return 1;
}

int ai_GameOver = 0;  /* 0x6f259c */
int sv_FrameMask = 0;  /* 0x760638 */
G13MIS ai_Missions[0x80] = {0};  /* 0xf82320 */
