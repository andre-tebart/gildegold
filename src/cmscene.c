/* cmscene.c -- the scenes (0x4d8b50..0x4e58e0): the cutscene parties and their kinds, council session, trial, duel,
   execution, wedding, birth, auction, the scene loop (cm_ExecCutscene), the message box. Split from cmfight (data
   layout s12): its own .data block cm_DialogFrameFlags 0x60d360 .. cm_DialogLength after the fight strings, then its
   string run (first user cm_DrawFightTime 0x4d8b50); its .bss 0x6f0d68..; its common group 0x925510..0x930380
   (cm_SavedScene .. cm_OrderChances; cm_OrderTicks, read only by cmorders' cm_RunCutOrders, lies inside it). The first
   function is bounded only: after cm_FightLoop 0x4d88c0 (the last user of the fight strings), at or before
   cm_DrawFightTime; cm_FightMember / cm_ClearSceneChars (0x4d8b10 / 0x4d8b30, no strings or .data) stay with
   cm_FightLoop (now cmorders; bprobe at all three points: only cm_RunBirth, a draft, changes).
   Open: data: 0x60d38c / 0x60d3a0 (cm_RunAuction's "handwerker3_MANN" / "buerger2_MANN") are the original's
   string literals in cm's data block: the code inline-copies from them, and naming them as arrays makes VC6
   copy byte-wise (cm_RunAuction shape -> draft), so they stay gap bytes.
   unit 0x004d8b50..0x004e58e0; boundaries in decl/text.dcl */
#include "d2_types.h"
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <d3d.h>
#include "net.h"
#include "cm.h"
#include "cmscene.h"
#include "d2.h"
#include "d3.h"
void cm_SceneNothing(void);  /* unlinked, defined at the end of the unit (an empty body above its callers would let VC6 drop the calls) */

struct D3SKYLAYER; struct D3SKY; struct D3SKYLAYER *d3sky_CreateLayer(struct D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset, float vscale, float voffset);  /* 0x45d880 */
#include "d3_tools.h"
#include "d3_quad.h"
#include "d3_light.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "menu_types.h"
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_602568[];


#define G17_I(m, o) (*(int *)&(m).data[o])
#define G17_S(m, o) (*(short *)&(m).data[o])
/* the block at 0x936420 is 0x24 bytes: the count then the eight player states at +4 (one object, so the
   0x24-byte memset / memcpy in 0x4cd580 and 0x4cdxx cannot run over a neighbour that LINK placed there) */
#define ai_WeaponRecords ((AIWEAPON *)ai_WeaponRecordData)  /* 0x60c7c0, the ten 0x5c-byte weapon records ai_FindWeaponRecord searches (a view) */

#include "f3init.h"
#include "sv.h"
#include "ahm.h"
#include "ai.h"
#include "ch.h"
#include "d3_anim.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_a.h"
#include "d3_io_t.h"
#include "d3_sky.h"
#include "d3_sm.h"
#include "d3_vec.h"
#include "err.h"
#include "f3snd.h"
#include "io_text.h"
#include "ls.h"
#include "main.h"
#include "mem.h"
#include "msx.h"
#include "rd.h"
#include "sblib.h"
#include "tb.h"
#include "tr.h"
#include "d3_pose.h"
#include "sv_t.h"
#include "lc.h"
#include "shp.h"
#include "f3gm_types.h"
#include "f3gm.h"
#include "he.h"
#include "amt.h"
#include "am_types.h"
#include "mission.h"
#include "gv.h"
#include "poker.h"
#include "gfx.h"
unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: tr.c defines it returning int; this unit's callers read the 16-bit result (mov si,ax / movzx): the original's units saw a 16-bit declaration */
#define CM_KEEPER_FLAG 0x800000     /* flags1c8 bit 23 (he.c HE_LOVER_FLAG: has a keeper; f3gm.c ORD_F1C8_ANEW): message 0x19
                                       may not set it on a character that has it */
typedef struct CMCUTTYPEMSG {      /* 0x7c bytes, the data of the system message 0x11 cm_RunSceneParty sends (cm_SendExSysMessage) */
    int party;                      /* 0x00, the party's id (G28REC.id) */
    char text[0x78];                /* 0x04, "cut_type:%i" with the scene kind */
} CMCUTTYPEMSG;
typedef struct CMNOTE {             /* 0x8f8 bytes, a long message (AIMSG) with its texts right after it, as cm_SendPartyNote builds it */
    AIMSG head;                     /* 0x00 */
    char text[0x800];               /* 0xf8, the text, then the second one (textFlags 0x10) */
} CMNOTE;
typedef struct CMFIELD {            /* the head of one field of a field block (G17FIELDS.buf); its values follow at 0x04 */
    signed char size;               /* 0x00, a value's size: 1, 2 or 4 bytes */
    signed char n;                  /* 0x01, the values */
    unsigned short offset;          /* 0x02, the field's offset in the record */
    int values[1];                  /* 0x04, the values */
} CMFIELD;
typedef struct AIJOBTAIL {          /* AIJOB as message 0x1d sends and takes its data block: the record from 0xac */
    char pad00[0xac];
    char data[0xa0];                /* 0xac, the long message (cm_SetLongMsg 0xa0, cm_LongMsg) */
} AIJOBTAIL;
typedef struct CMNEWPARTYMSG {      /* 8 bytes, the data of message 0x27 (its handler cm_ExNewParty) */
    int partyId;                    /* 0x00, the id the new scene party gets (ai_NextPartyId) */
    int seed;                       /* 0x04, the party's random seed (G28REC.seed) */
} CMNEWPARTYMSG;
typedef struct CMJOINPARTYMSG {     /* 8 bytes, the data of message 0x29 (its handler cm_ExAddPartyMember) */
    int partyId;                    /* 0x00, the scene party record (cm_PartyById) */
    int member;                     /* 0x04, the member's netId (cm_AddPartyMember) */
} CMJOINPARTYMSG;
typedef struct CMWALKPATHMSG {      /* 0x24 bytes, the data of message 0x4f (its handler cm_ExWalkPath; no sender in
                                       the sources, the path cells of a walk) */
    char name[0x18];                /* 0x00, the character's object name ("sp_<id>") */
    int from;                       /* 0x18, the first cell written */
    int to;                         /* 0x1c, the cell past the path (the action's new path length) */
    unsigned char cells[4];         /* 0x20, up to two cells, x and z bytes each */
} CMWALKPATHMSG;
typedef struct CMPOSMSG {           /* 0x10 bytes read, the data of message 0x32 (its handler cm_ExChrSetPos) */
    int charId;                     /* 0x00, the character */
    D3VECTOR pos;                   /* 0x04, its new position (ch_SetPos) */
} CMPOSMSG;
typedef struct CMGRENADE {          /* 0x10 bytes, one of the 32 thrown bombs of cm_Grenades */
    D3OBJECT *obj;                  /* the object "ob_BOMBE", 0 = a free slot */
    D3ANIM *anim;                   /* 0x04, its flight path (d3anim_CreatePath) */
    int x, y;                       /* 0x08, the target cell */
} CMGRENADE;
typedef struct CMBOMB {             /* 8 bytes, one of the 32 bombs of cm_Bombs */
    D3OBJECT *obj;                  /* the bomb object "ob_BOMBE", 0 = a free slot */
    unsigned long time;             /* d3_Time when it was laid */
} CMBOMB;
typedef struct CMAMOUNTROW {        /* 0x38 bytes, one row of the special-amount list (ai_AmountRowStatus / ai_AmountRow read it
                                       as ints; menu.c GYPSYROW) */
    AICHAR *rec;                    /* 0x00, the character the row shows */
    int ctl;                        /* 0x04, the row's control object (d2_Objs index, ai_AmountRow) */
    int pad08[3];
    int y;                          /* 0x14 */
    int pad18[5];
    int x2c;                        /* 0x2c */
    int x30;                        /* 0x30 */
    int pad34;
} CMAMOUNTROW;

/* dcp unit split: the declarations of cm before the moved functions */
typedef struct CMSPYREPORT {            /* an AIOBJ of kind 0xca (a spy report) as this unit reads it: the report's words
                                           0x1c .. 0x37 (HESPYREPORT) after the object's head */
    char pad1c[0x1c];
    HESPYREPORT report;
} CMSPYREPORT;
typedef struct CMGUARDREC {
    char pad000[0x80];
    int netIds[16];                 /* 0x080, the netIds of the guards already armed, -1 = a free slot (cm_FreeNetIdSlot) */
    char pad0c0[0x50];
    int kind;                       /* 0x110, the kind of arming last asked for (1 .. 3) */
} CMGUARDREC;
#define G20_ME (ai_CharInts[ai_PlayerId][0])

int cm_DialogFrameFlags = 464838;  /* 0x60d360, the frame wait of the dialogs ( */
int cm_SceneDuration = 10;  /* 0x60d364 */
int cm_TextBox = -1;  /* 0x60d368 */
int cm_SceneEnterScript = -1;  /* 0x60d36c, -1 or the object waited for ( */
int cm_DuelVariantCount = 1;  /* 0x60d370, the number of duel variants in cm_DuelVariants (cm_DuelInitVariants) */
int cm_DialogLength = 1;  /* 0x60d374 */
int cm_SavedScene;  /* 0x925510 */
G28REC cm_Parties[96];  /* 0x925520 */
int cm_DuelRight;  /* 0x92bca0 */
int cm_DuelLeft;  /* 0x92bca4 */
int cm_DuelVariants[128];  /* 0x92bcc0, the shuffled order of the duel variants (gm_Shuffled fills it) */
int cm_OrderTicks;  /* 0x92bec0 */
unsigned short cm_DisabledObjs[768];  /* 0x92bee0 */
unsigned int cut_RandState;  /* 0x92c4e0, the random seed of the scenes */
CMKIND cm_Kinds[12];  /* 0x92c500, the scene kind table: run, callbacks, flag, check (cm_RegisterSceneKind) */
char cm_SceneState[1152];  /* 0x92c600 */
int cm_SceneChars[16];  /* 0x92ca80 */
int cm_SavedNoGroup;  /* 0x92cac0 */
CMCUTINFO cm_CutInfos[16];  /* 0x92cae0 */
float cm_OrderChances[0xb4];  /* 0x9300a0 */
int cm_TallyNo = 0;  /* 0x6f0d68, the "no" pictures of the deposition tally (cm_TallyStack's stack 1) */
int cm_TallyYes = 0;  /* 0x6f0d6c, the "yes" pictures of the deposition tally (cm_TallyStack's stack 0) */
int cm_TallyAbstain = 0;  /* 0x6f0d70, the abstentions of the deposition tally (cm_TallyStack's stack 2) */
char * cm_CutChar = 0;  /* 0x6f0d74 */
int cm_PartyTurn = 0;  /* 0x6f0d78 */
int cm_ScenePartyRuns = 0;  /* 0x6f0d7c */
int cm_SceneTicks = 0;  /* 0x6f0d80 */
int cm_SceneDeadline = 0;  /* 0x6f0d84 */
int cm_SceneTimedOut = 0;  /* 0x6f0d88 */
int cm_SkipEvents = 0;  /* 0x6f0d8c */
int * cm_CurrentFight = 0;  /* 0x6f0d90 */
int cm_DuelOver = 0;  /* 0x6f0d94 */
unsigned int cm_SceneInflateFlags = 0;  /* 0x6f0d98 */
int cm_SceneDepth = 0;  /* 0x6f0d9c, the scene nesting depth */
char * cm_SceneFade = 0;  /* 0x6f0da0, the fade */
unsigned int cm_FightStart = 0;  /* 0x6f0da4, fight start (d3_Time) */
unsigned char cm_DuelScore[2] = {0};  /* 0x6f0da8, the damage taken by the left / right duellist (cm_DuelShotAction adds it, cm_RunDuel heals it back) */
unsigned char cm_DuelLeftInsultWon = 0;  /* 0x6f0daa, the left duellist won the insult round (cm_DuelRound): the right one shoots worse */
unsigned char cm_DuelRightInsultWon = 0;  /* 0x6f0dab, the right duellist won the insult round: the left one shoots worse */
unsigned char cm_DuelLeftRollWon = 0;  /* 0x6f0dac, the left duellist won the roll round (cm_DuelRound): the right one's shot does less damage */
unsigned char cm_DuelRightRollWon = 0;  /* 0x6f0dad, the right duellist won the roll round: the left one's shot does less damage */
int cm_DuelVariantPos = 0;  /* 0x6f0db0, the cursor into cm_DuelVariants (cm_DuelVariant advances it) */
struct D3SKYLAYER *cm_SkyLayer = 0;  /* 0x6f0db4 */
struct D3SKY *cm_SceneSky = 0;  /* 0x6f0db8 */
int cm_SceneRain = 0;  /* 0x6f0dbc */
int cm_DialogStart = 0;  /* 0x6f0dc0 */
char cm_MessageBoxOpen = 0;  /* 0x6f0dc4 */

/* 0x4d8b50, shows the fight time (d3_Time minus the start 0x6f0da4, 14 ms per tick) as "mm : ss : ms ms" in a box at the top */

void cm_DrawFightTime()
{
    char buf[128];

    d2_SetFont(0x54);
    sprintf(buf, "%2i : %2i : %3i ms", (d3_Time - cm_FightStart) * 14 / 60000, (d3_Time - cm_FightStart) * 14 / 1000 % 60,
            (d3_Time - cm_FightStart) * 14 % 1000);
    d2_DrawTextBox(0, 10, 200, 24, d2_ScreenSurf, buf, 0x30);
    d2_AddDirty(0, 10, d2_TextDrawnW, d2_TextDrawnH);
}
/* 0x4d8c40, frees the 16 slots of 0x92ca80 */

void cm_DeleteSceneChars()
{
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_SceneChars[i] != 0)
            cm_DeleteChar(cm_SceneChars[i]);
    }
}

/* ---- cut.c (joined) ---- */

/* 0x4d8c70, draws the next picture 0x4df of a stack (o + 0x2c counts them, 10 up each) on the window 0x6ed938 */
void cm_StackPicture(int unused, CMAMOUNTROW *o)
{
    d2_WinOpenPic(0x8c, (short)(0x44 - (short)o->x2c * 10), 0x4df, d2_LastWindow);
    o->x2c++;
}
/* 0x4d8cb0, the fight result list (a) or one more picture of its three stacks: b set: the three headings (texts 0x12d3 / 0x12d4 / 0x12d5)
   in the window 0x6ed938 and the counters 0x6f0d6c / 0x6f0d68 / 0x6f0d70 cleared; else the next picture 0x4df of stack c (0, 1, 2) */

void cm_TallyStack(int a, int b, int c)
{
    char buf[0x40];
    int o;

    if (b != 0) {
        d2_RemoveWindowObjects(d2_LastWindow, 1);
        txt_Format(buf, 0x12d3);
        o = d2_CreateWinText(0, 0x20, buf, d2_LastWindow);
        d2_SetObjFont(o, 0x54);
        txt_Format(buf, 0x12d4);
        o = d2_CreateWinText(0, 0x3e, buf, d2_LastWindow);
        d2_SetObjFont(o, 0x54);
        txt_Format(buf, 0x12d5);
        o = d2_CreateWinText(0, 0x2f, buf, d2_LastWindow);
        d2_SetObjFont(o, 0x54);
        cm_TallyYes = 0;
        cm_TallyNo = 0;
        cm_TallyAbstain = 0;
        return;
    }
    if (c == 0) {
        d2_WinOpenPic((cm_TallyYes * 5 + 0x28) * 2, 0x20, 0x4df, d2_LastWindow);
        cm_TallyYes++;
    } else if (c == 1) {
        d2_WinOpenPic((cm_TallyNo * 5 + 0x28) * 2, 0x3e, 0x4df, d2_LastWindow);
        cm_TallyNo++;
    } else if (c == 2) {
        d2_WinOpenPic((cm_TallyAbstain * 5 + 0x28) * 2, 0x2f, 0x4df, d2_LastWindow);
        cm_TallyAbstain++;
    }
}
/* 0x4d8e20, plays a council session (the run function of scene kind 1; a: its party record, saved on entry and put back at
   the end). For each of the six columns of offices (6 .. 1: the sittings Wohnsitz, Kirche, Rat, Diebe, Wache, Zunft) it takes
   the offices amt_GetColumnOffices gives; a column that is not decided yet is only elected (amt_ElectOffices, on the host).
   Else the column's office holders (not held) become the party's voting members, the other present characters join as
   observers, and
   - every office with a deposition vote (amt_HasDeposalVote) is voted on (the "_ABSETZEN_*" voices, texts 0x12c4 .. 0x12d7):
     more yes than no deposes the holder (the host sends the office message 3), else the holder keeps it (message 1, or 4
     without a holder); the members' relations to the holder change by their answers;
   - every office open for applications (state 3, with applicants) is filled from its applicants (at most 4, AICHAR
     appliedOffice): a single one takes it, without voters one is drawn, else the members vote (texts 0x12d8 .. 0x12e5, a
     tie drawn with cm_Random); the host sends the office message 1 for the winner.
   The event scene "Sitzung_*.ed3" with the members' figures and its fly-in is loaded once per column when there is something
   to vote on, and left at the end of the column. */

void cm_RunCouncilSession(G28REC *a)
{
    int columns[6] = {6, 5, 4, 3, 2, 1};
    char scene[6][0x30] = {"Sitzung_Wohnsitz.ed3", "Sitzung_Kirche.ed3", "Sitzung_Rat.ed3", "Sitzung_Diebe.ed3",
                           "Sitzung_Wache.ed3", "Sitzung_zunft.ed3"};
    char fly[6][0x20] = {"sitzungen_flug_wohnsitz.esc", "sitzungen_flug_kirche.esc", "sitzungen_flug_rat.esc",
                         "sitzungen_flug_diebe.esc", "sitzungen_flug_wache.esc", "sitzungen_flug_zunft.esc"};
    G28REC saved;
    AMTREC offices[6];
    AICHAR *present[8];                 /* the party's characters at the start */
    AICHAR *holders[6];                 /* the column's office holders who are not held: the voters */
    int holderIdx[6];                   /* their office's index in offices[] */
    AICHAR *applicants[4];
    CMAMOUNTROW holderRows[6], applicantRows[4], row;
    CMSESSIONREQ depose, elect;
    char name[0x100], name2[0x100];
    int tally[3];                       /* the deposition answers: yes, no, abstain */
    int votes[4];                       /* the votes per applicant */
    AICHAR *c, *holder, *challenger;
    GMHEAD *head;
    CMCUTINFO *s;
    struct MSXTRACK *sound;
    int bankDepose, bankElect, loaded, view, viewBottom, viewTop, viewLeft;
    int npresent, k, kind, n, nh, m, left, nvoters, shown, napp, best, win, tie, r, i, j, e, slot;

    saved = *a;
    sound = 0;
    bankDepose = cm_LoadVoiceBank((int)"ABSETZUNG.sbf", 0);
    bankElect = cm_LoadVoiceBank((int)"AMTSWAHL.sbf", 0);
    cm_PlaySpecialTrack(0);
    npresent = 0;
    for (i = 0; i < a->used; i++) {
        c = (AICHAR *)gm_FindChar(a->members[i]);
        if (c != 0)
            present[npresent++] = c;
    }
    for (i = npresent; i < 8; i++)
        present[i] = 0;
    for (k = 0; k < 6; k++) {
        kind = columns[k];
        n = amt_GetColumnOffices(kind, 6, offices);
        for (i = 0, nh = 0; i < n; i++) {
            c = (AICHAR *)gm_FindChar(offices[i].owner);
            if (c != 0 && c->heldRounds == 0) {
                holderIdx[nh] = i;
                holders[nh] = c;
                nh++;
            }
        }
        sv_SyncText(&ch_EmptyName);
        if (amt_IsColumnDecided(kind) == 0) {
            if (a->host == ai_Chars[ai_PlayerId].netId)
                amt_ElectOffices(kind, n, offices);
            continue;
        }
        for (i = n; i < 6; i++)
            holders[i] = 0;
        /* the members: the voters first, then the other present characters as observers */
        for (i = 0; i < nh; i++) {
            a->members[i] = holders[i]->netId;
            cm_CutInfos[i].playerId = holders[i]->netId;
            cm_CutInfos[i].session.observer = 0;
            cm_CutInfos[i].ready = 0;
            cm_CutInfos[i].valid = 1;
        }
        m = nh;
        for (i = 0; i < npresent; i++) {
            for (j = 0; j < nh; j++)
                if (cm_CutInfos[j].playerId == present[i]->netId)
                    goto member;        /* already a voter */
            a->members[m] = present[i]->netId;
            cm_CutInfos[m].playerId = present[i]->netId;
            cm_CutInfos[m].ready = 0;
            cm_CutInfos[m].valid = 1;
            cm_CutInfos[m].session.observer = 1;
            m++;
        member:;
        }
        a->used = (unsigned char)m;
        for (; m < 16; m++) {
            a->members[m] = -1;
            cm_CutInfos[m].playerId = -1;
        }
        sv_SyncText(&ch_EmptyName);
        view = d2_OpenForm(0, 0, "cutscenes\\sitzung_main");
        d2_SetViewEnabled(view, 0);
        d2_PlaceMainWindow(view, 3);

        /* the depositions */
        left = 0;
        for (i = 0; i < n; i++)
            if (offices[i].state == 2)
                left++;
        loaded = 0;
        if (left != 0) {
            cm_LoadEventScene(scene[k]);
            if (sound == 0)
                sound = msx_PlayAmbient("Athmos\\Athmo_sitzung.mp3", 0x40);
            for (i = 0; i < nh; i++) {
                head = (GMHEAD *)gm_CharNameRecord(holders[i]->id);
                if (head != 0)
                    sprintf(name, "%i %s", holderIdx[i], head->name);
                else
                    sprintf(name, "%i stallbursche_MANN", holderIdx[i]);
                cm_StartEventArg("cutscenes\\sitzungen\\sitzungen_create.esc", (int)name);
            }
            for (i = 0; i < nh; i++) {
                sprintf(name, "%i", holderIdx[i]);
                cm_StartEventArg("cutscenes\\sitzungen\\sitzungen_create_2.esc", (int)name);
            }
            txt_Format(name, (int)"cutscenes\\sitzungen\\%s", fly[k]);
            cm_WaitEvent(cm_StartEventArg(name, (int)&ch_EmptyName));
            loaded = 1;
            sv_SyncText(&ch_EmptyName);
        }
        for (j = 0; j < n && left != 0; j++) {
            tally[0] = 0;
            tally[1] = 0;
            tally[2] = 0;
            if (amt_HasDeposalVote(kind, &offices[j]) == 0)
                continue;
            for (i = 0, nvoters = 0; i < nh; i++)
                if (holders[i]->officeKind != 0)
                    nvoters++;
            viewBottom = d2_OpenForm(0, 0, "cutscenes\\sitzung_bottom");
            d2_PlaceMainWindow(viewBottom, 3);
            viewTop = d2_OpenForm(0, 0, "cutscenes\\sitzung_top_absetzen");
            d2_PlaceMainWindow(viewTop, 3);
            left--;
            holder = (AICHAR *)gm_FindChar(offices[j].owner);
            challenger = (AICHAR *)gm_FindChar(offices[j].x14);
            d2_SetForm(viewTop, 1);
            row.rec = holder;
            row.y = 0;
            row.x30 = 0;
            ai_AmountRowStatus(&row);
            ai_AmountRow(0x28, 0, d2_LastWindow, 0x54, &row, 1);
            d2_SetForm(viewTop, 2);
            cm_TallyStack(d2_LastWindow, 1, 0);
            cm_QueueVoice(-8, bankDepose, (int)"_ABSETZEN_KAMMERN", kind - 1, 0x32);
            d2_SetViewEnabled(view, 1);
            d2_SetForm(view, 0);
            io_PrintText("$C");
            io_PrintText(0x12c4);
            io_PrintText(0x12c5, kind + 0x12a9, kind + 0x12bb, challenger->id, holder->id);
            cm_RunSceneTicks(0x9c4);
            d2_SetForm(viewBottom, 0);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            if (nvoters > 0) {
                int step;

                step = (0x320 - d2_Pics[0x5da].w * nvoters) / (nvoters + 1);
                for (i = 0, shown = 0; i < nh; i++) {
                    if (holders[i]->officeKind != 0) {
                        holderRows[i].rec = holders[i];
                        holderRows[i].y = 0;
                        holderRows[i].x30 = 0;
                        ai_AmountRowStatus(&holderRows[i]);
                        ai_AmountRow((shown + 1) * step + d2_Pics[0x5da].w * shown, 0, d2_LastWindow, 0x54, &holderRows[i], 0);
                        shown++;
                    }
                }
            }
            cm_RunSceneTicks(0x9c4);
            depose.mode = 1;
            depose.office = offices[j].idx;
            depose.kind = offices[j].kind;
            depose.ids[0] = offices[j].owner;
            depose.ids[1] = offices[j].x14;
            d2_SetViewEnabled(view, 0);
            cm_RunSceneParty(a, &depose);
            cm_RunSceneTicks(0x3e8);
            d2_SetViewEnabled(view, 1);
            /* each voter's answer: a picture on its stack, its voice and remark */
            for (i = 0; i < nh; i++) {
                s = &cm_CutInfos[i];
                if ((unsigned)s->session.answer >= 3)
                    continue;
                c = (AICHAR *)gm_FindChar(s->playerId);
                tally[s->session.answer]++;
                d2_SetForm(view, 0);
                d2_Objs[holderRows[i].ctl].highlighted = 1;
                io_PrintText("$C");
                io_PrintText(0x12c7, c->id);
                cm_RunSceneTicks(0x3e8);
                d2_SetForm(view, 0);
                if (s->session.answer == 0) {
                    cm_QueueVoice(c->id, bankDepose, (int)"_ABSETZEN_JA", cm_Random(4), 100);
                    io_PrintText(0x12c8);
                    if (s->session.click == 1)
                        io_PrintText(0x12cb + cm_Random(2), challenger->id);
                    else if (s->session.click == 2)
                        io_PrintText(0x12cf + cm_Random(2), challenger->id);
                    d2_SetForm(viewTop, 2);
                    cm_TallyStack(d2_LastWindow, 0, 0);
                } else if (s->session.answer == 1) {
                    cm_QueueVoice(c->id, bankDepose, (int)"_ABSETZEN_NEIN", cm_Random(4), 100);
                    io_PrintText(0x12ca);
                    if (s->session.click == 1)
                        io_PrintText(0x12cd + cm_Random(2), holder->id, challenger->id);
                    else if (s->session.click == 2)
                        io_PrintText(0x12d1 + cm_Random(2), holder->id);
                    d2_SetForm(viewTop, 2);
                    cm_TallyStack(d2_LastWindow, 0, 1);
                } else {
                    cm_QueueVoice(c->id, bankDepose, (int)"_ABSETZEN_ENTHALTUNG", cm_Random(4), 100);
                    io_PrintText(0x12c9);
                    d2_SetForm(viewTop, 2);
                    cm_TallyStack(d2_LastWindow, 0, 2);
                }
                cm_RunSceneTicks(0xfa0);
                d2_Objs[holderRows[i].ctl].highlighted = 0;
                cm_RunSceneTicks(0x3e8);
            }
            if (tally[0] > tally[1]) {
                cm_QueueVoice(-8, bankDepose, (int)"_ABSETZEN_ERGEBNIS_ABGESETZT", cm_Random(3), 100);
                d2_SetForm(view, 0);
                io_PrintText("$C");
                io_PrintText(0x12d6, tally[0], tally[1], holder->id);
                cm_RunSceneTicks(0xfa0);
                if (a->host == ai_Chars[ai_PlayerId].netId) {
                    for (i = 0; i < nh; i++) {
                        s = &cm_CutInfos[i];
                        if (s->playerId == holder->netId)
                            continue;
                        if (s->session.answer == 0)
                            cm_SendRelations(s->playerId, holder->netId, -40, 1);
                        else if (s->session.answer == 1)
                            cm_SendRelations(s->playerId, holder->netId, 10, 1);
                        else
                            cm_SendRelations(s->playerId, holder->netId, -20, 1);
                    }
                    slot = amt_SendOfficeMessage(offices[j].idx, 0, 0, 3, 0xff);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                }
            } else {
                c = (AICHAR *)gm_FindChar(offices[j].owner);
                cm_QueueVoice(-8, bankDepose, (int)"_ABSETZEN_ERGEBNIS_NICHT_ABGESETZT", cm_Random(3), 100);
                d2_SetForm(view, 0);
                io_PrintText("$C");
                io_PrintText(0x12d7, tally[0], tally[1], holder->id, holder->id);
                cm_RunSceneTicks(0xfa0);
                if (a->host == ai_Chars[ai_PlayerId].netId) {
                    for (i = 0; i < nh; i++) {
                        s = &cm_CutInfos[i];
                        if (s->playerId == holder->netId)
                            continue;
                        if (s->session.answer == 0)
                            cm_SendRelations(s->playerId, holder->netId, -40, 1);
                        else if (s->session.answer == 1)
                            cm_SendRelations(s->playerId, holder->netId, 10, 1);
                        else
                            cm_SendRelations(s->playerId, holder->netId, 5, 1);
                    }
                    if (c != 0)
                        slot = amt_SendOfficeMessage(offices[j].idx, (char *)c, 0, 1, 0xff);
                    else
                        slot = amt_SendOfficeMessage(offices[j].idx, 0, 0, 4, 0xff);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                }
            }
            d2_SetForm(view, 0);
            d2_SetViewEnabled(view, 0);
            io_PrintText("$C");
            d2_CloseView(viewTop);
            d2_CloseView(viewBottom);
            cm_RunSceneTicks(0x3e8);
            sv_SyncText(&ch_EmptyName);
        }

        /* the elections */
        left = 0;
        for (i = 0; i < n; i++)
            if (offices[i].state == 3 && offices[i].cnt != 0)
                left++;
        if (left != 0 && loaded == 0) {
            cm_LoadEventScene(scene[k]);
            if (sound == 0)
                sound = msx_PlayAmbient("Athmos\\Athmo_sitzung.mp3", 0x40);
            for (i = 0; i < nh; i++) {
                head = (GMHEAD *)gm_CharNameRecord(holders[i]->id);
                if (head != 0)
                    sprintf(name2, "%i %s", holderIdx[i], head->name);
                else
                    sprintf(name2, "%i stallbursche_MANN", holderIdx[i]);
                cm_StartEventArg("cutscenes\\sitzungen\\sitzungen_create.esc", (int)name2);
            }
            for (i = 0; i < nh; i++) {
                sprintf(name2, "%i", holderIdx[i]);
                cm_StartEventArg("cutscenes\\sitzungen\\sitzungen_create_2.esc", (int)name2);
            }
            txt_Format(name2, (int)"cutscenes\\sitzungen\\%s", fly[k]);
            cm_WaitEvent(cm_StartEventArg(name2, (int)&ch_EmptyName));
            loaded = 1;
            sv_SyncText(&ch_EmptyName);
        }
        for (j = 0; j < n && left != 0; j++) {
            votes[0] = 0;
            votes[1] = 0;
            votes[2] = 0;
            votes[3] = 0;
            if (offices[j].state != 3 || offices[j].cnt == 0)
                continue;
            left--;
            elect.mode = 2;
            elect.office = offices[j].idx;
            elect.kind = offices[j].kind;
            nvoters = 0;
            for (i = 0; i < nh; i++)
                if (amt_CanTakeOffice((char *)holders[i], elect.kind) != 0 && holders[i]->heldRounds == 0)
                    nvoters++;
            viewBottom = d2_OpenForm(0, 0, "cutscenes\\sitzung_bottom");
            d2_PlaceMainWindow(viewBottom, 3);
            viewLeft = d2_OpenForm(0, 0, "cutscenes\\sitzung_left_wahl");
            d2_PlaceMainWindow(viewLeft, 2);
            d2_SetForm(viewLeft, 0);
            napp = 0;
            for (i = 0; i < 768 && napp < offices[j].cnt && napp < 4; i++) {
                c = &ai_Chars[i];
                if (c->appliedOffice == offices[j].kind) {
                    applicants[napp] = c;
                    d2_SetForm(viewLeft, napp + 1);
                    applicantRows[napp].rec = c;
                    applicantRows[napp].y = 0;
                    applicantRows[napp].x2c = 0;
                    applicantRows[napp].x30 = 0;
                    ai_AmountRowStatus(&applicantRows[napp]);
                    ai_AmountRow((0xa0 - d2_Pics[0x5da].w) / 2, 0, d2_LastWindow, 0x54, &applicantRows[napp], 1);
                    elect.ids[napp] = c->netId;
                    napp++;
                }
            }
            if (napp == 0)
                continue;
            for (i = napp; i < 4; i++) {
                applicants[i] = 0;
                elect.ids[i] = -1;
            }
            cm_QueueVoice(-8, bankElect, (int)"_INTRO", kind - 1, 100);
            cm_QueueVoice(-8, bankElect, (int)"_BEWERBER", napp - 1, 0x1f4);
            d2_SetViewEnabled(view, 1);
            d2_SetForm(view, 0);
            io_PrintText("$C");
            io_PrintText(0x12d8);
            io_PrintText(0x12d9, kind + 0x12a9, kind + 0x12bb, offices[j].kind + 0x253);
            for (i = 0; i < napp; i++)
                io_PrintText(0x12db, applicants[i]->id, applicants[i]->id);
            cm_RunSceneTicks(0x9c4);
            d2_SetForm(viewBottom, 0);
            d2_RemoveWindowObjects(d2_LastWindow, 1);
            if (nvoters > 0) {
                int step;

                step = (0x320 - d2_Pics[0x5da].w * nvoters) / (nvoters + 1);
                shown = 0;
                for (i = 0; i < nh; i++) {
                    if (amt_CanTakeOffice((char *)holders[i], elect.kind) != 0) {
                        holderRows[i].rec = holders[i];
                        holderRows[i].y = 0;
                        holderRows[i].x30 = 0;
                        ai_AmountRowStatus(&holderRows[i]);
                        ai_AmountRow((shown + 1) * step + d2_Pics[0x5da].w * shown, 0, d2_LastWindow, 0x54, &holderRows[i], 0);
                        shown++;
                    }
                }
            }
            cm_RunSceneTicks(0x9c4);
            if (napp == 1) {
                /* a single applicant takes the office */
                cm_QueueVoice(-8, bankElect, (int)"_ERGEBNIS", 0, 100);
                d2_Objs[applicantRows[0].ctl].highlighted = 1;
                d2_SetForm(view, 0);
                io_PrintText("$C");
                io_PrintText(0x12e0, applicants[0]->id, offices[j].kind + 0x253);
                cm_RunSceneTicks(0xfa0);
                if (a->host == ai_Chars[ai_PlayerId].netId) {
                    slot = amt_SendOfficeMessage(offices[j].idx, (char *)applicants[0], 0, 1, 0xff);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                }
            } else if (nvoters == 0) {
                /* nobody to vote: one is drawn */
                r = cm_Random(napp);
                cm_QueueVoice(-8, bankElect, (int)"_ERGEBNIS", 1, 100);
                d2_SetForm(view, 0);
                io_PrintText("$C");
                io_PrintText(0x12e1);
                cm_RunSceneTicks(0x3e8);
                d2_Objs[applicantRows[r].ctl].highlighted = 1;
                io_PrintText(0x12e2, applicants[r]->id);
                cm_RunSceneTicks(0xfa0);
                if (a->host == ai_Chars[ai_PlayerId].netId) {
                    slot = amt_SendOfficeMessage(offices[j].idx, (char *)applicants[r], 0, 1, 0xff);
                    while (cm_GetSlotState(slot) == 0)
                        sv_PumpNet();
                }
            } else {
                d2_SetViewEnabled(view, 0);
                cm_RunSceneParty(a, &elect);
                cm_RunSceneTicks(0x3e8);
                d2_SetViewEnabled(view, 1);
                /* each voter's vote: its row and the applicant's row lit, a picture on the applicant's stack */
                for (i = 0; i < nh; i++) {
                    s = &cm_CutInfos[i];
                    if (s->session.answer != -1) {
                        for (e = 0; e < napp; e++) {
                            if (elect.ids[e] == s->session.answer) {
                                d2_Objs[holderRows[i].ctl].highlighted = 1;
                                d2_SetForm(view, 0);
                                io_PrintText("$C");
                                io_PrintText(0x12de, holders[i]->id);
                                votes[e]++;
                                cm_RunSceneTicks(0x3e8);
                                d2_Objs[applicantRows[e].ctl].highlighted = 1;
                                d2_SetForm(viewLeft, e + 1);
                                cm_StackPicture(e + 1, &applicantRows[e]);
                                d2_SetForm(view, 0);
                                io_PrintText(0x12df, applicants[e]->id);
                                cm_RunSceneTicks(0xfa0);
                                d2_Objs[holderRows[i].ctl].highlighted = 0;
                                d2_Objs[applicantRows[e].ctl].highlighted = 0;
                                break;
                            }
                        }
                    }
                }
                tie = 0;
                e = 0;
                best = -1;
                win = -1;
                for (; e < napp; e++) {
                    if (votes[e] > best) {
                        tie = 0;
                        best = votes[e];
                        win = e;
                    } else if (votes[e] == best)
                        tie = 1;
                }
                if (tie == 0) {
                    if (win != -1) {
                        d2_Objs[applicantRows[win].ctl].highlighted = 1;
                        cm_QueueVoice(-8, bankElect, (int)"_ERGEBNIS", 3, 100);
                        d2_SetForm(view, 0);
                        io_PrintText("$C");
                        io_PrintText(0x12e5, offices[j].kind + 0x253, applicants[win]->id);
                        cm_RunSceneTicks(0xfa0);
                        if (a->host == ai_Chars[ai_PlayerId].netId) {
                            slot = amt_SendOfficeMessage(offices[j].idx, (char *)applicants[win], 0, 1, 0xff);
                            while (cm_GetSlotState(slot) == 0)
                                sv_PumpNet();
                        }
                    }
                } else if (win != -1) {
                    /* a tie: from a random applicant on, the first with the most votes */
                    r = cm_Random(napp);
                    for (i = napp; i != 0; i--, r = (r + 1) % napp)
                        if (votes[r] == best) {
                            win = r;
                            break;
                        }
                    cm_QueueVoice(-8, bankElect, (int)"_ERGEBNIS", 2, 100);
                    d2_SetForm(view, 0);
                    io_PrintText("$C");
                    io_PrintText(0x12e3);
                    cm_RunSceneTicks(0x3e8);
                    d2_Objs[applicantRows[win].ctl].highlighted = 1;
                    io_PrintText(0x12e4, applicants[win]->id);
                    cm_RunSceneTicks(0xfa0);
                    if (a->host == ai_Chars[ai_PlayerId].netId) {
                        slot = amt_SendOfficeMessage(offices[j].idx, (char *)applicants[win], 0, 1, 0xff);
                        while (cm_GetSlotState(slot) == 0)
                            sv_PumpNet();
                    }
                }
            }
            d2_SetForm(view, 0);
            io_PrintText("$C");
            d2_SetViewEnabled(view, 0);
            d2_CloseView(viewLeft);
            d2_CloseView(viewBottom);
            cm_RunSceneTicks(0x3e8);
            sv_SyncText(&ch_EmptyName);
        }
        if (loaded != 0) {
            cm_FadeToBlack();
            cm_RunSceneTicks(0x3e8);
            cm_WaitEventFull(cm_StartEventArg("cutscenes\\sitzungen\\sitzungen_exit.esc", (int)&ch_EmptyName));
            cm_LeaveEventScene();
        }
        d2_CloseView(view);
    }
    if (a->host == ai_Chars[ai_PlayerId].netId)
        amt_UpdateOffices();
    cm_UnloadVoiceBank(bankDepose);
    cm_UnloadVoiceBank(bankElect);
    if (sound != 0)
        msx_StopAmbient(sound);
    cm_EndSpecialTrack();
    *a = saved;
    ai_CampaignStep(0x30, 0, 0, 0);
}
/* 0x4da9e0, the vote dialog "cutscenes\\sitzungen" (buttons from the text 0x12c6): the answer of the player's choice; *out the click */

int cm_AskDeposeVote(AICHAR *a, AICHAR *b, AICHAR *c, int *out)
{
    int v, b1, b2, b3, dlg, r;
    int tmp[1];

    *out = 0;
    v = d2_OpenForm(0, 0, "cutscenes\\sitzungen");
    d2_PlaceMainWindow(v, 1);
    d2_SetForm(v, 0);
    r = io_PrintText(0x12c6, b->id, a->id);
    b1 = d2_GetWinObj(v, 0, r);
    d2_GetWinObj(v, 0, r + 1);
    b3 = d2_GetWinObj(v, 0, r + 2);
    dlg = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    cm_DialogTimer(dlg, 0x5dc, 1);
    r = ai_TrustedOfTwo(a, b, c, tmp);
    d2_ClickedId = -1;
    while (sv_RunFrame(cm_AskDeposeVote, cm_DialogFrameFlags) != 0) {
        if (cm_DialogTimer(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            r = 0;
            sv_PopRequest = 1;
            if (d2_PointerObj != b1)
                r = (d2_PointerObj != b3) + 1;
        }
    }
    d2_CloseView(v);
    return r;
}
/* 0x4dab50, the player's election vote dialog (the session member callback 0x4dada0, mode 2): b, the player's member, picks one of
   the applicants ids (netIds, up to 4) of the office office by its button under the text 0x12dc (fewer than two applicants
   of the office) / 0x12dd, before the timer slider (cm_DialogTimer, 1500 ms) runs out; returns the netId clicked, else the
   pick ai_TrustedOfFour proposes. *click is cleared; -1 when the office, its kind or every applicant is missing */

int cm_AskElectVote(unsigned char office, AICHAR *b, int *ids, int *click)
{
    AMTREC rec;
    AMTKIND kind;
    AICHAR *cand[4];                    /* first the office's applicants, then the characters of ids */
    int btn[4];
    int proposed[1];
    AICHAR *c;
    int n, i, k, view, dlg, res;

    *click = 0;
    if (amt_GetOffice(office, &rec) == 0)
        return -1;
    if (amt_GetKind(rec.kind, &kind) == 0)
        return -1;
    k = amt_GetOfficeApplicants(rec.kind, 2, (char **)cand);
    for (i = 0, n = 0; i < 4; i++) {
        c = (AICHAR *)gm_FindChar(ids[i]);
        if (c != 0)
            cand[n++] = c;
    }
    if (n == 0)
        return -1;
    res = ai_TrustedOfFour(office, (struct AIPER *)b, ids, proposed);
    view = d2_OpenForm(0, 0, "cutscenes\\sitzungen");
    d2_PlaceMainWindow(view, 1);
    d2_SetForm(view, 0);
    if (k < 2)
        io_PrintText(0x12dc, b->id, b->id, rec.kind + 0x253);
    else
        io_PrintText(0x12dd, b->id, b->id, rec.kind + 0x253);
    for (i = 0; i < n; i++)
        btn[i] = d2_GetWinObj(view, 0, io_PrintText("%ia[%1N3]$A", cand[i]->id));
    for (i = n; i < 4; i++)
        btn[i] = -1;
    dlg = d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    cm_DialogTimer(dlg, 0x5dc, 1);
    d2_ClickedId = -1;
    while (sv_RunFrame(cm_AskDeposeVote, cm_DialogFrameFlags) != 0) {
        if (cm_DialogTimer(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == btn[i]) {
                    res = cand[i]->netId;
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
    }
    d2_CloseView(view);
    return res;
}
/* 0x4dada0, the session member callback for the player's character (cm_RunSceneParty, scene kind 1): r is the member's record, m
   the vote (cm_RunCouncilSession). No answer (-1) for an observer, or a character without an office or held; else the
   player is asked: mode 1 the deposition of the holder m->ids[0] by the challenger m->ids[1] (cm_AskDeposeVote), mode 2 the
   election among the applicants m->ids (cm_AskElectVote, when the character may take the office). The answer and its click
   go to r's session fields, r's text is cleared */

void cm_SessionAskPlayer(CMCUTINFO *r, CMSESSIONREQ *m)
{
    AICHAR *c, *holder, *challenger;
    int res, click;

    r->session.answer = -1;
    r->session.click = -1;
    if (r->session.observer != 0)
        return;
    c = (AICHAR *)gm_FindChar(r->playerId);
    if (c == 0 || c->officeKind == 0 || c->heldRounds > 0)
        return;
    if (m->mode == 1) {
        holder = (AICHAR *)gm_FindChar(m->ids[0]);
        challenger = (AICHAR *)gm_FindChar(m->ids[1]);
        if (holder == 0 || challenger == 0)
            return;
        res = cm_AskDeposeVote(c, holder, challenger, &click);
        r->session.click = click;
        r->session.answer = res;
        sprintf(r->text, ch_EmptyName);
    } else {
        if (m->mode != 2)
            return;
        if (amt_CanTakeOffice((char *)c, m->kind) == 0)
            return;
        res = cm_AskElectVote(m->office, c, m->ids, &click);
        r->session.click = click;
        r->session.answer = res;
        sprintf(r->text, ch_EmptyName);
    }
}
/* 0x4dae90, the session member callback for the other characters (cm_RunSceneParty, scene kind 1), as cm_SessionAskPlayer but the
   character decides itself: mode 1 the one of holder and challenger it trusts more (ai_TrustedOfTwo), mode 2 the applicant
   it trusts most (ai_TrustedOfFour); the answer and its click go to r's session fields, r's text is cleared */

void cm_SessionAskAI(CMCUTINFO *r, CMSESSIONREQ *m)
{
    AICHAR *c, *holder, *challenger;
    int res, click;

    r->session.answer = -1;
    r->session.click = -1;
    if (r->session.observer != 0)
        return;
    c = (AICHAR *)gm_FindChar(r->playerId);
    if (c == 0 || c->officeKind == 0 || c->heldRounds > 0)
        return;
    if (m->mode == 1) {
        holder = (AICHAR *)gm_FindChar(m->ids[0]);
        challenger = (AICHAR *)gm_FindChar(m->ids[1]);
        if (holder == 0 || challenger == 0)
            return;
        res = ai_TrustedOfTwo(c, holder, challenger, &click);
        r->session.click = click;
        r->session.answer = res;
        sprintf(r->text, ch_EmptyName);
    } else {
        if (m->mode != 2)
            return;
        if (amt_CanTakeOffice((char *)c, m->kind) == 0)
            return;
        res = ai_TrustedOfFour(m->office, (struct AIPER *)c, m->ids, &click);
        r->session.click = click;
        r->session.answer = res;
        sprintf(r->text, ch_EmptyName);
    }
}
/* 0x4daf80, the check function of the trial (scene kind 2, run function 0x4db700) for its party p: the six participants
   (members[0 .. 4], members[5] only with six or more) must exist, not be of kind 0xf and be alive, and one held (heldRounds)
   must not have an event 0x3d of its own (else code 6, else 7). The first failing slot gives the code: slot i the code
   i + 1, slot 4 the code 4 as slot 3 does, slot 5 the code 5; slot 1 tests the held state of participant 0, as the
   original does. All available: the participants of kind 6 / 7 get their invitation (texts 0x11d0 .. 0x11d5 with
   0x11d7, cm_SendPartyNote), 1. Else every participant of kind 6 / 7 but the held one is told why (texts 0x11d8 ..
   0x11df, he_ShowNews), the crimes go back to open (cm_SendHandOverCrimes), 0 */

int cm_CheckTrial(G28REC *p)
{
    char intro[0x40];
    char msg[0x200];
    AICHAR *c0, *c1, *c2, *c3, *c4, *c5, *held;
    AIJOB *ev;
    int code;

    held = 0;
    code = 0;
    c0 = (AICHAR *)gm_FindChar(p->members[0]);
    if (c0 == 0 || c0->kind == 0xf || c0->health == 0)
        code = 1;
    else if (c0->heldRounds != 0) {
        held = c0;
        code = 7;
        for (ev = (AIJOB *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (AIJOB *)he_NextEvent())
            if (ev->ownerCh == c0->netId) {
                code = 6;
                break;
            }
    }
    c1 = (AICHAR *)gm_FindChar(p->members[1]);
    if (code == 0) {
        if (c1 == 0 || c1->kind == 0xf || c1->health == 0)
            code = 2;
        else if (c0->heldRounds != 0) {
            held = c0;
            code = 7;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->ownerCh == c0->netId) {
                    code = 6;
                    break;
                }
        }
    }
    c2 = (AICHAR *)gm_FindChar(p->members[2]);
    if (code == 0) {
        if (c2 == 0 || c2->kind == 0xf || c2->health == 0)
            code = 3;
        else if (c2->heldRounds != 0) {
            held = c2;
            code = 7;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->ownerCh == c2->netId) {
                    code = 6;
                    break;
                }
        }
    }
    c3 = (AICHAR *)gm_FindChar(p->members[3]);
    if (code == 0) {
        if (c3 == 0 || c3->kind == 0xf || c3->health == 0)
            code = 4;
        else if (c3->heldRounds != 0) {
            held = c3;
            code = 7;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->ownerCh == c3->netId) {
                    code = 6;
                    break;
                }
        }
    }
    c4 = (AICHAR *)gm_FindChar(p->members[4]);
    if (code == 0) {
        if (c4 == 0 || c4->kind == 0xf || c4->health == 0)
            code = 4;
        else if (c4->heldRounds != 0) {
            held = c4;
            code = 7;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->ownerCh == c4->netId) {
                    code = 6;
                    break;
                }
        }
    }
    c5 = (AICHAR *)gm_FindChar(p->members[5]);
    if (code == 0 && p->used >= 6) {
        if (c5 == 0 || c5->kind == 0xf || c5->health == 0)
            code = 5;
        else if (c5->heldRounds != 0) {
            held = c5;
            code = 7;
            for (ev = (AIJOB *)he_FindEvent(1, 0, 0x3d); ev != 0; ev = (AIJOB *)he_NextEvent())
                if (ev->ownerCh == c5->netId) {
                    code = 6;
                    break;
                }
        }
    }
    if (code == 0) {
        txt_Format(intro, (int)"%s", 0x11d7);
        if (c0->kind == 6 || c0->kind == 7) {
            txt_Format(msg, 0x11d0, c1->id);
            strcat(msg, intro);
            cm_SendPartyNote(c0, p, msg, ch_EmptyName);
        }
        if (c1->kind == 6 || c1->kind == 7) {
            txt_Format(msg, 0x11d1, c0->id);
            strcat(msg, intro);
            cm_SendPartyNote(c1, p, msg, ch_EmptyName);
        }
        if (c2->kind == 6 || c2->kind == 7) {
            txt_Format(msg, 0x11d2, c0->id, c1->id);
            strcat(msg, intro);
            cm_SendPartyNote(c2, p, msg, ch_EmptyName);
        }
        txt_Format(msg, 0x11d3, c0->id, c1->id);
        strcat(msg, intro);
        if (c3->kind == 6 || c3->kind == 7)
            cm_SendPartyNote(c3, p, msg, ch_EmptyName);
        if (c4->kind == 6 || c4->kind == 7)
            cm_SendPartyNote(c4, p, msg, ch_EmptyName);
        if (c5 != 0 && (c5->kind == 6 || c5->kind == 7)) {
            txt_Format(msg, 0x11d5, c0->id, c1->id);
            strcat(msg, intro);
            cm_SendPartyNote(c5, p, msg, ch_EmptyName);
        }
        return 1;
    }
    switch (code) {
    case 1:
        txt_Format(msg, 0x11d8);
        break;
    case 2:
        txt_Format(msg, 0x11d9);
        break;
    case 3:
        txt_Format(msg, 0x11da);
        break;
    case 4:
        txt_Format(msg, 0x11db);
        break;
    case 5:
        txt_Format(msg, 0x11dc);
        break;
    case 6:
        txt_Format(msg, 0x11df, held->id);
        break;
    case 7:
        txt_Format(msg, 0x11de, held->id);
        break;
    }
    if (c0 != 0 && c0 != held && (c0->kind == 6 || c0->kind == 7))
        he_ShowNews(c0->netId, -1, msg, 0, 0x5f5, 0);
    if (c1 != 0 && c1 != held && (c1->kind == 6 || c1->kind == 7))
        he_ShowNews(c1->netId, -1, msg, 0, 0x5f5, 0);
    if (c2 != 0 && c2 != held && (c2->kind == 6 || c2->kind == 7))
        he_ShowNews(c2->netId, -1, msg, 0, 0x5f5, 0);
    if (c3 != 0 && c3 != held && (c3->kind == 6 || c3->kind == 7))
        he_ShowNews(c3->netId, -1, msg, 0, 0x5f5, 0);
    if (c4 != 0 && c4 != held && (c4->kind == 6 || c4->kind == 7))
        he_ShowNews(c4->netId, -1, msg, 0, 0x5f5, 0);
    if (c5 != 0 && c5 != held && (c5->kind == 6 || c5->kind == 7))
        he_ShowNews(c5->netId, -1, msg, 0, 0x5f5, 0);
    cm_SendHandOverCrimes(p->members[0], p->members[0], p->crimeState, 0);
    return 0;
}

#define CM_WAIT_VOICE(h) if ((h) != 0) { do { if (sb_IsPlaying(h) == 0) break; cm_WaitTicks(1); } while (1); } cm_WaitTicks(1)  /* waits until the voice sample h has played */

/* 0x4db700, the trial (the run function of scene kind 2, checked by cm_CheckTrial) of the party m in "Gericht.ed3": the
   participants are m->members[0 .. 5] (c0 the accuser, c1 the accused, c2 the judge, c3 / c4 the assessors, c5 a sixth
   one; the request plan carries them as ids). The judge opens (texts 0x1491 / 0x1492), the accuser states the charge
   (0x1499), every group of crimes of the same law is read (gv_CrimeText, the guilt summed into the plan's score by the
   severity of the law record 0's value), the accused pleads (cm_RunSceneParty, mode 4: guilty or not; a confession lowers
   the score by the liking of the accused for the judge). Without a plea of guilt and with more than two crimes and a sixth
   participant the accused is tortured (mode 1: the accuser decides; mode 2 picks the instrument of the seven, mode 3 the
   accused's answer; the plan's roll decides whether he confesses: texts 0x14ca .. 0x1522, the torture chamber scene).
   Then the judge and the assessors vote (0x1528 .. 0x152e); with fewer than two votes for guilt the accused goes free,
   else the judge picks the sentence (mode 5, gv_CheckSentence / gv_PassSentence, texts 0x152f .. 0x1531). On the host the
   accuser, the judge and the assessors get the fees (cm_SendMoveGoodsUnchecked, a third of the plan's rating each) and
   with the item 0x22e the accused's relations change. Every voice bank loaded is freed at the end.
   @deep deep s47: 885 -> 128 ins, and 6 ins (only the three masks) in the probe below. Kept: (1) the
   reading loop's AIDEC dec block-local, the opening dec.cur read in its own block (frame 0x784 -> 0x760);
   (2) both clamps as max(min(v, hi), 0) (sel stays unclamped: the original indexes instruments/files/texts
   with the raw sel); (3) the assessor vote blocks with block locals (AICHAR *assessor; int vote): slot
   packing {p, c5, ev2} / {k, severity}; (4) the three verdict voices' sex test as (x == 1) & 1 (exact
   locally: xor; cmp; sete; and reg,1); (5) groups[i] = 1 before the crime copy (the original's mov eax,edx
   copy of the i*4 IV); plan.amount before plea, plan.plea = plea after the score line. Left: a global
   allocation knife edge: with the masks as & 3 (or & 0xff) the whole function is right except the masks
   and the torture path's inline strcpy store ([eax+edx] vs the original's [edx+eax]) - 6 ins; with & 1 the
   chain grouping loop .. plea goes wrong (128). Only the NOT GUILTY site's mask ((c0->sex == 1) & 1) + 2
   decides it (subset round: & 3 there alone -> 6 ins). Other single-constant probes flip it the same way:
   the 1st (line ~1355, the judge opens) or the 9th (torture confession arm) cm_SetViewEnabled(cm_TextBox,
   1) as 2. So the original's constant-1 candidate has slightly lower priority than ours (c2trace: a const
   piece materialised in the grouping loop cuts ncrimes#1's piece, op 2b4 'line 1788'). No lever for it
   found: site C spelled ?: 3 : 2 / (x == 1) + 2 / | 2 / ?: 1 : 0 / && / % 2u / 1u / (unsigned char) (=
   control or worse), symbol-count probes 0..63 (no), declaration order (no), CM_WAIT_VOICE loop constant
   (decided: no effect), bankPlea for the plea bank (= control). Next: look for one 1-reference fewer (or a
   differently-typed 1) near the judge's opening or the not-guilty arm that leaves the code equal; the
   strcpy store order may follow from the same allocation (it is [delta+src] in 90 byte functions). prior:
   Rebuilt whole from the original (2340 instructions), every region read against it: the plan record init,
   the crime grouping and reading loops, the plea, the torture (instrument tables, rolls, confession), the
   vote of judge and assessors, the verdict, fees, relations and the bank cleanup (including the original's
   double unload of bankF2). The callees, arguments, literals and statement order agree (ours 2333 vs
   2340). Left is allocation and layout: the frame 0x784 vs 0x760 and its slot renaming (slots 750),
   register choice (regs 134), and two clamps VC6 lowers differently: dec.cur to 0..4 (ours inline jmp/mov
   eax,4 at +0xaa, the original out of line at +0x24f) and the instrument 0..6 (ours setle/dec/and at
   +0x11ed, the original jle/jmp blocks). (references: M8 at +0x1297: ours refers to sprintf (0x5de652),
   the original's field to 0x4e2bd0 (cm_SetViewEnabled) (and 2 more), explained: the three M8 lines
   (+0x1297 sprintf|cm_SetViewEnabled, +0x1648, +0x1a4b) are alignment crossings: the same calls in the
   same order on both sides, paired one call apart where the surrounding pushes are scheduled differently;
   the proof stops only on the crime id array's frame offset (frame+0x154 vs +0x130)) */

void cm_RunTrial(G28REC *m)
{
    CRIME crime, other;
    AIPLAN plan;
    float factors[5];
    int crimeIds[32];                   /* the indexes of the crimes in the state m->crimeState (ai_Crimes) */
    int groups[32];                     /* per crime: how many of the same law it reads out, -1: read with another */
    int list[32];                       /* the crimes' ids, -1 filled */
    char names[0x100];
    char buf[0x200];                    /* a voice bank name, the sentence text */
    AICHAR *c0, *c1, *c2, *c3, *c4, *c5, *p;
    GMHEAD *head;
    struct MSXTRACK *sound;
    char *pathEnd, *end;
    float severity, sum;
    int ncrimes, nlist, plea, confessed, sel, scene, k, i, j, ev, ev2, h, votes, sentence, fee;
    int bankTrial, bankPlea, bankV, bankVerdict, bank8, bankF1, bankF2, bankT1, bankT2, bankT3, bankT4, bankE, bank6;

    confessed = 0;
    scene = -1;
    sum = 0;
    sound = 0;
    bankT1 = 0;
    bankT2 = 0;
    bankT3 = 0;
    bankF1 = 0;
    bankF2 = 0;
    bankT4 = 0;
    factors[0] = 1.4f;
    factors[1] = 1.2f;
    factors[2] = 1.0f;
    factors[3] = 0.8f;
    factors[4] = 0.6f;
    {
        AIDEC dec;

        gv_GetTableRecord(0, (int *)&dec);
        k = max(min(dec.cur, 4), 0);
    }
    severity = factors[k];
    cm_SetViewEnabled(cm_TextBox, 0);
    cm_LoadEventScene("Gericht.ed3");
    plan.score = 0;
    plan.plea = 0;
    plan.amount = 1000.0f;
    plan.ids[0] = m->members[0];
    plan.ids[1] = m->members[1];
    plan.ids[2] = m->members[2];
    plan.ids[3] = m->members[3];
    plan.ids[4] = m->members[4];
    plan.b10 = 0xa;
    plan.crimes = 0;
    plan.rating2c = 0;
    plan.rating30 = 0;
    plan.rating34 = 0;
    plan.instrument = 0;
    plan.tries = 0;
    plan.answer = 0;
    plan.tortured = 0;
    plan.success = 0;
    plan.penalty = 0;
    plan.ids[5] = m->members[5];
    plan.mode = 0;
    ncrimes = gv_FindCrimesByState(m->crimeState, crimeIds);
    plan.crimes = (unsigned char)ncrimes;
    c0 = (AICHAR *)gm_FindChar(m->members[0]);
    if (c0 == 0)
        return;
    c1 = (AICHAR *)gm_FindChar(m->members[1]);
    if (c1 == 0)
        return;
    c2 = (AICHAR *)gm_FindChar(m->members[2]);
    if (c2 == 0)
        return;
    c3 = (AICHAR *)gm_FindChar(m->members[3]);
    if (c3 == 0)
        return;
    c4 = (AICHAR *)gm_FindChar(m->members[4]);
    if (c4 == 0)
        return;
    c5 = (AICHAR *)gm_FindChar(m->members[5]);
    if (c5 == 0 && plan.crimes > 2 && m->members[5] != -1)
        return;
    if (cm_SkipEvents == 0)
        sound = msx_PlayAmbient("Athmos\\Athmo_sitzung.mp3", 0x40);
    if (cm_Random(2) != 0)
        cm_PlaySpecialTrack((int)"cd2\\ArmerDelinquent.mp3");
    else
        cm_PlaySpecialTrack((int)"cd2\\ImKerker.mp3");
    bankTrial = cm_LoadVoiceBank((int)"ADDON\\PROZESS_NEU.sbf", 0);
    plan.rating2c = ai_WealthRating(c0, c1);

    /* the figures of the court */
    sprintf(names, "%s ", ((GMHEAD *)gm_CharNameRecord(c2->id))->name);
    end = names + strlen(names);
    sprintf(end, "%s ", ((GMHEAD *)gm_CharNameRecord(c0->id))->name);
    end = names + strlen(names);
    sprintf(end, "%s ", ((GMHEAD *)gm_CharNameRecord(c1->id))->name);
    end = names + strlen(names);
    sprintf(end, "%s ", ((GMHEAD *)gm_CharNameRecord(c3->id))->name);
    end = names + strlen(names);
    sprintf(end, "%s", ((GMHEAD *)gm_CharNameRecord(c4->id))->name);
    cm_StartEventArg("cutscenes\\prozess\\prozess-create2.esc", (int)names);
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter.esc", (int)"2");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"2");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"2");
    cm_WaitEvent(cm_StartEventArg("cutscenes\\prozess\\prozess-intro.esc", (int)ch_EmptyName));

    /* the judge opens the session */
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter-reden.esc", (int)ch_EmptyName);
    cm_SetViewEnabled(cm_TextBox, 1);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText(0x1491);
    h = cm_PlayVoice(c2->id, bankTrial, (int)"_PROZESS_NEU", 5);
    CM_WAIT_VOICE(h);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    io_PrintText(0x1492, c0->id, c1->id, c0->id);
    cm_WaitTicks(4);

    /* the accuser states the charge */
    ev = cm_StartEventArg("cutscenes\\prozess\\prozess-klaeger.esc", (int)"1");
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    io_PrintText(0x1499, c1->id);
    h = cm_PlayVoice(c0->id, bankTrial, (int)"_PROZESS_NEU", c1->sex == 1);
    CM_WAIT_VOICE(h);
    cm_WaitEvent(ev);

    /* the crimes, grouped by the law broken */
    memset(groups, 0, sizeof(groups));
    for (nlist = 0; nlist < ncrimes; nlist++)
        list[nlist] = ai_Crimes[crimeIds[nlist]].id;
    for (i = nlist; i < 32; i++)
        list[i] = -1;
    for (i = 0; i < ncrimes; i++) {
        if (groups[i] == -1)
            continue;
        groups[i] = 1;
        crime = ai_Crimes[crimeIds[i]];
        for (j = i + 1; j < ncrimes; j++) {
            if (groups[j] == -1)
                continue;
            other = ai_Crimes[crimeIds[j]];
            if (crime.law == other.law) {
                groups[j] = -1;
                groups[i]++;
            }
        }
    }
    for (i = 0; i < ncrimes; i++) {
        char text[0x100];
        AIDEC dec;

        crime = ai_Crimes[crimeIds[i]];
        gv_GetTableRecord(crime.law, (int *)&dec);
        k = groups[i];
        sum += dec.guilt * severity;
        if (k == -1)
            continue;
        ev = cm_StartEventArg("cutscenes\\prozess\\prozess-klaeger.esc", (int)"1");
        gv_CrimeText(text, crime, k);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C$Z");
        io_PrintText(text);
        h = cm_PlayVoice(c0->id, bankTrial, (int)"_PROZESS_GESETZ", (unsigned char)dec.b1e);
        CM_WAIT_VOICE(h);
        cm_Random(0x1e);
        io_PrintText("$C");
        cm_WaitEvent(ev);
    }
    plan.score = sum;
    bankV = 0;

    /* the plea of the accused */
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter-reden.esc", (int)ch_EmptyName);
    io_PrintText(0x14ba, c1->id, ncrimes);
    h = cm_PlayVoice(c2->id, bankTrial, (int)"_PROZESS_NEU", (c1->sex != 0) + 2);
    CM_WAIT_VOICE(h);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    cm_StartEventArg("cutscenes\\prozess\\prozess-angeklagter.esc", (int)"0");
    cm_SetViewEnabled(cm_TextBox, 0);
    cm_RunSceneParty((char *)m, (int)&plan);
    if (m->host == ai_Chars[ai_PlayerId].netId)
        gv_RetireCrimeIds(c1->netId, nlist, list);
    cm_StartEventArg("cutscenes\\prozess\\prozess-angeklagter.esc", (int)"1");
    for (i = 0; i < m->used && cm_CutInfos[i].playerId != plan.ids[1]; i++)
        ;
    plan.amount = cm_CutInfos[i].session.amount;
    plea = cm_CutInfos[i].session.answer;
    cm_SetViewEnabled(cm_TextBox, 1);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    plan.mode = 4;
    if (plea == 1) {
        plan.score = sum - gm_Liking(c1->id, c2->id, 1) * sum * 0.002f;
        plan.plea = plea;
        h = cm_LoadVoiceBank((int)"PROZESS_3_SCHULDIG.sbf", 0);
        cm_QueueVoice(c1->id, h, (int)"_PROZESS_3_SCHULDIG", 0, 0);
        io_PrintText(0x14bd, c1->id, 0x14be);
    } else {
        plan.plea = 0;
        h = cm_LoadVoiceBank((int)"PROZESS_3_NICHT_SCHULDIG.sbf", 0);
        cm_QueueVoice(c1->id, h, (int)"_PROZESS_3_NICHT_SCHULDIG", 0, 0);
        io_PrintText(0x14bd, c1->id, 0x14c4);
        if (c5 != 0 && ncrimes > 2)
            plan.mode = 1;
    }
    cm_WaitTicks(2);
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter-reden.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter.esc", (int)ch_EmptyName);

    /* the torture */
    if (plan.mode == 1) {
        plan.rating30 = ai_WealthRatingHalf(c0, c1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x14ca, c0->id, c1->id, plan.rating30);
        cm_WaitTicks(4);
        ev = cm_StartEventArg("cutscenes\\prozess\\prozess-folter-1.esc", (int)ch_EmptyName);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        cm_SetViewEnabled(cm_TextBox, 0);
        cm_RunSceneParty((char *)m, (int)&plan);
        cm_WaitEvent(ev);
        for (i = 0; i < m->used && cm_CutInfos[i].playerId != plan.ids[0]; i++)
            ;
        k = cm_CutInfos[i].session.answer;
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        if (k == 1) {
            j = cm_Random(6);
            k = cm_Random(6);
            bankF1 = cm_LoadVoiceBank((int)"PROZESS_4_FOLTER_1.sbf", 0);
            bankF2 = cm_LoadVoiceBank((int)"PROZESS_4_FOLTER_2.sbf", 0);
            cm_QueueVoice(c0->id, bankF1, (int)"_PROZESS_4_FOLTER_1", j, 0x32);
            cm_QueueVoice(c0->id, bankF2, (int)"_PROZESS_4_FOLTER_2", k, 0x12c);
            io_PrintText(0x14d9);
            cm_WaitTicks(1);
            io_PrintText("$C");
            ev = cm_StartEventArg("cutscenes\\prozess\\prozess-folter-3.esc", (int)ch_EmptyName);
            plan.tortured = 1;
        } else {
            ev = cm_StartEventArg("cutscenes\\prozess\\prozess-folter-2.esc", (int)ch_EmptyName);
            io_PrintText(0x14d8);
            plan.tortured = 0;
        }
        cm_WaitTicks(4);
        cm_WaitEvent(ev);
        plan.success = 0;
        if (plan.tortured != 0) {
            char files[7][0x10] = {"dschraube.esc", "stiefel.esc", "peitsche.esc", "brandeisen.esc", "eistropfer.esc",
                                   "kaefig.esc", "streckbank.esc"};
            char instruments[7][0x20] = {"DAUMENSCHRAUBEN", "SPANISCHE_STIEFEL", "NEUNSCHWAENZIGE_PEITSCHE",
                                         "ESSE_UND_BRANDEISEN", "EISWASSERTROPFER", "HAENGENDER_KAEFIG", "STRECKBANK"};
            int texts[7] = {0x14ed, 0x14f2, 0x14f7, 0x14fc, 0x1501, 0x1506, 0x150b};
            char path[0x100];

            if (m->host == ai_Chars[ai_PlayerId].netId) {
                fee = plan.rating30 / 3;
                cm_SendMoveGoodsUnchecked(c2->netId, c0->netId, ai_OwnPlayer, fee);
                cm_SendMoveGoodsUnchecked(c3->netId, c0->netId, ai_OwnPlayer, fee);
                cm_SendMoveGoodsUnchecked(c4->netId, c0->netId, ai_OwnPlayer, fee);
            }
            strcpy(path, "cutscenes\\prozess\\folter\\");
            pathEnd = path + strlen(path);
            p = (AICHAR *)gm_FindChar(m->members[5]);
            d2_SetForm(cm_TextBox, 0);
            io_PrintText("$C");
            plan.mode = 2;
            ev = cm_StartEventArg("cutscenes\\prozess\\prozess-klaeger.esc", (int)"0");
            cm_SetViewEnabled(cm_TextBox, 0);
            cm_RunSceneParty((char *)m, (int)&plan);
            for (i = 0; i < m->used && cm_CutInfos[i].playerId != plan.ids[0]; i++)
                ;
            cm_WaitEvent(ev);
            ev2 = cm_StartEventArg("cutscenes\\prozess\\prozess-klaeger.esc", (int)"1");
            sel = cm_CutInfos[i].session.answer & 0xf;
            plan.instrument = max(min(sel, 6), 0);
            plan.tries = ai_PlanLevel(&plan);
            plan.answer = 0;
            sprintf(buf, "PROZESS_5_FOLTER2_%s.sbf", instruments[sel]);
            bankT1 = cm_LoadVoiceBank((int)"PROZESS_5_FOLTER2_GESTEHEN.sbf", 0);
            bankT2 = cm_LoadVoiceBank((int)buf, 0);
            bankT3 = cm_LoadVoiceBank((int)"PROZESS_5_FOLTER2_WEITER.sbf", 0);
            bankT4 = cm_LoadVoiceBank((int)"PROZESS_5_FOLTER2_UNSCHULDSBETEUERUNG.sbf", 0);
            sprintf(buf, "_PROZESS_5_FOLTER2_%s", instruments[sel]);
            cm_SetViewEnabled(cm_TextBox, 1);
            d2_SetForm(cm_TextBox, 0);
            io_PrintText("$C");
            io_PrintText(0x14dd, c0->id, 0xb20 + sel * 2);
            cm_WaitTicks(4);
            d2_SetForm(cm_TextBox, 0);
            io_PrintText("$C");
            cm_SetViewEnabled(cm_TextBox, 0);
            cm_WaitTicks(2);
            cm_WaitEvent(ev2);
            plan.mode = 3;
            cm_SetViewEnabled(cm_TextBox, 0);
            cm_RunSceneParty((char *)m, (int)&plan);
            for (i = 0; i < m->used && cm_CutInfos[i].playerId != plan.ids[5]; i++)
                ;
            plan.answer = cm_CutInfos[i].session.answer;
            end = names + strlen(names);
            head = (GMHEAD *)gm_CharNameRecord(p->id);
            if (head != 0)
                sprintf(end, " %s", head->name);
            else
                sprintf(end, " handwerker_MANN");
            scene = d3_CurrentScene;
            cm_LoadEventScene("ob_GEFAENGNIS_FOLTERKAMMER.ed3");
            cm_StartEventArg("cutscenes\\prozess\\folter-create.esc", (int)names);
            cm_SetViewEnabled(cm_TextBox, 1);
            d2_SetForm(cm_TextBox, 0);
            io_PrintText("$C");
            io_PrintText(0x14e4);
            h = cm_PlayVoice(c2->id, bankTrial, (int)"_PROZESS_NEU", 4);
            CM_WAIT_VOICE(h);
            strcpy(pathEnd, files[sel]);
            cm_SetViewEnabled(cm_TextBox, 0);
            d2_SetForm(cm_TextBox, 0);
            io_PrintText("$C");
            ev = cm_StartEventArg(path, (int)ch_EmptyName);
            cm_WaitTicks(5);
            cm_SetViewEnabled(cm_TextBox, 1);
            d2_SetForm(cm_TextBox, 0);
            k = cm_Random(5);
            cm_QueueVoice(c0->id, bankT2, (int)buf, k, 0x320);
            io_PrintText("$C");
            io_PrintText(0x14ec, texts[sel] + k);
            cm_WaitTicks(5);
            cm_SetViewEnabled(cm_TextBox, 0);
            ai_PlanRollTries(&plan);
            cm_WaitEvent(ev);
            cm_SetViewEnabled(cm_TextBox, 1);
            d2_SetForm(cm_TextBox, 0);
            if (plan.success == 0) {
                confessed = 0;
                k = cm_Random(6);
                cm_SetViewEnabled(cm_TextBox, 1);
                d2_SetForm(cm_TextBox, 0);
                io_PrintText("$C");
                io_PrintText(0x14e6 + k);
                cm_QueueVoice(c1->id, bankT4, (int)"_PROZESS_5_FOLTER2_UNSCHULDSBETEUERUNG", k, 0x1f4);
                cm_WaitTicks(5);
            } else {
                k = cm_Random(10);
                cm_QueueVoice(c1->id, bankT1, (int)"_PROZESS_5_FOLTER2_GESTEHEN", k, 0);
                cm_QueueVoice(c1->id, bankT3, (int)"_PROZESS_5_FOLTER2_WEITER", 0, 0x1f4);
                cm_SetViewEnabled(cm_TextBox, 1);
                d2_SetForm(cm_TextBox, 0);
                io_PrintText("$C");
                io_PrintText(0x1514 + k);
                io_PrintText("$N");
                io_PrintText(0x1510);
                cm_WaitTicks(4);
                confessed = 1;
            }
        }
        plan.mode = 4;
    }
    if (plan.tortured != 0) {
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        if (plan.success == 0) {
            io_PrintText(0x151f, c1->id, c0->id);
            plan.score *= 0.8f;
        } else {
            io_PrintText(0x151e, c1->id, c0->id);
            plan.score *= 1.1f;
        }
        cm_WaitTicks(4);
        ev = cm_StartEventArg("cutscenes\\prozess\\folterexit.esc", (int)ch_EmptyName);
        if (ev != 0)
            cm_WaitEvent(ev);
        if (cm_SkipEvents == 0 && scene != -1)
            d3if_SelectScene(scene, 0);
    }
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"1");
    cm_SetViewEnabled(cm_TextBox, 1);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    if (plan.tortured != 0) {
        bankV = cm_LoadVoiceBank((int)"PROZESS_6_FOLTERERGEBNIS.sbf", 0);
        cm_StartEventArg("cutscenes\\prozess\\prozess-richter-reden.esc", (int)ch_EmptyName);
        if (confessed != 0) {
            cm_QueueVoice(c2->id, bankV, (int)"_PROZESS_6_FOLTERERGEBNIS", 0, 100);
            io_PrintText(0x1520, c1->id, 0x1521);
        } else {
            cm_QueueVoice(c2->id, bankV, (int)"_PROZESS_6_FOLTERERGEBNIS", 1, 100);
            io_PrintText(0x1520, c1->id, 0x1522);
        }
        cm_WaitTicks(4);
    }

    /* the vote of the judge and the assessors */
    bank6 = cm_LoadVoiceBank((int)"PROZESS_6_ANKUENDIGUNG.sbf", 0);
    cm_QueueVoice(c2->id, bank6, (int)"_PROZESS_6_ANKUENDIGUNG", 0, 0);
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter-reden.esc", (int)ch_EmptyName);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    io_PrintText(0x1523);
    cm_WaitTicks(4);
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"1");
    cm_SetViewEnabled(cm_TextBox, 0);
    cm_RunSceneParty((char *)m, (int)&plan);
    cm_UnloadVoiceBank(bank6);
    for (i = 0; i < m->used && cm_CutInfos[i].playerId != plan.ids[2]; i++)
        ;
    bankE = cm_LoadVoiceBank((int)"PROZESS_6_ABSTIMMUNG_ERGEBNISSE.sbf", 0);
    cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-3.esc", (int)ch_EmptyName);
    cm_SetViewEnabled(cm_TextBox, 1);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    io_PrintText(0x1528, c2->id);
    cm_WaitTicks(2);
    votes = cm_CutInfos[i].session.answer;
    cm_QueueVoice(c2->id, bankE, (int)"_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", cm_Random(3) + cm_CutInfos[i].session.answer * 3, 100);
    io_PrintText(0x152c + votes);
    cm_WaitTicks(2);
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"1");
    if (plan.ids[3] != -1) {
        AICHAR *assessor;
        int vote;

        vote = cm_CutInfos[i + 1].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-1.esc", (int)ch_EmptyName);
        d2_SetForm(cm_TextBox, 0);
        assessor = (AICHAR *)gm_FindChar(plan.ids[3]);
        io_PrintText("$N");
        io_PrintText(0x1528, assessor->id);
        cm_WaitTicks(2);
        io_PrintText(0x152c + vote);
        cm_QueueVoice(c3->id, bankE, (int)"_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", cm_Random(3) + cm_CutInfos[i + 1].session.answer * 3, 100);
        cm_WaitTicks(2);
        votes += cm_CutInfos[i + 1].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"2");
    }
    if (plan.ids[4] != -1) {
        AICHAR *assessor;
        int vote;

        vote = cm_CutInfos[i + 2].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-2.esc", (int)ch_EmptyName);
        d2_SetForm(cm_TextBox, 0);
        assessor = (AICHAR *)gm_FindChar(plan.ids[4]);
        io_PrintText("$N");
        io_PrintText(0x1528, assessor->id);
        cm_WaitTicks(2);
        io_PrintText(0x152c + vote);
        cm_QueueVoice(c4->id, bankE, (int)"_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", cm_Random(3) + cm_CutInfos[i + 2].session.answer * 3, 100);
        cm_WaitTicks(2);
        votes += cm_CutInfos[i + 2].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"2");
    }
    cm_UnloadVoiceBank(bankE);
    cm_UnloadVoiceBank(bankV);

    /* the verdict */
    bank8 = cm_LoadVoiceBank((int)"PROZESS_8.sbf", 0);
    if ((unsigned)votes < 2) {
        bankVerdict = cm_LoadVoiceBank((int)"PROZESS_7_SCHULDIG.sbf", 0);
        bankV = cm_LoadVoiceBank((int)"PROZESS_7_STRAFEN.sbf", 0);
        plan.mode = 5;
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-3.esc", (int)ch_EmptyName);
        cm_StartEventArg("cutscenes\\prozess\\prozess-richter-reden.esc", (int)ch_EmptyName);
        cm_SetViewEnabled(cm_TextBox, 0);
        cm_RunSceneParty((char *)m, (int)&plan);
        for (i = 0; i < m->used && cm_CutInfos[i].playerId != plan.ids[2]; i++)
            ;
        sentence = cm_CutInfos[i].session.answer;
        if ((unsigned)sentence >= 4 && c0->kind == 6 && c1->officeKind != 0)
            ai_CountMissionEvent(c0, 0x13);
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x152f, c1->id);
        io_PrintText(0x1530);
        /* @original the three verdict voices pass the sex test masked with & 1 (xor; cmp; sete; and reg,1), the other sex tests of this function do not */
        cm_QueueVoice(c2->id, bankVerdict, (int)"_PROZESS_7_SCHULDIG", (c1->sex == 1) & 1, 100);
        cm_QueueVoice(c2->id, bankVerdict, (int)"_PROZESS_7_SCHULDIG", 2, 0xfa);
        cm_WaitTicks(3);
        gv_CheckSentence(c1, sentence);
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        gv_SentenceText(c1, sentence, buf);
        io_PrintText(buf);
        if (c1->officeKind != 0 && (sentence == 4 || sentence == 6 || sentence == 8)) {
            AIFIN *fin = (AIFIN *)gm_PlayerRecord((char *)c0);

            if (fin != 0)
                fin->officeSentences++;
        }
        cm_QueueVoice(c2->id, bankV, (int)"_PROZESS_7_STRAFEN", sentence, 100);
        cm_QueueVoice(c2->id, bank8, (int)"_PROZESS_8", (c1->sex == 1) & 1, 200);
        if (m->host == ai_Chars[ai_PlayerId].netId)
            gv_PassSentence(c1, sentence, c2, c3, c4);
        io_PrintText(0x1531);
        if (m->host == ai_Chars[ai_PlayerId].netId) {
            fee = plan.rating2c / 3;
            cm_SendMoveGoodsUnchecked(c2->netId, c1->netId, ai_OwnPlayer, fee);
            cm_SendMoveGoodsUnchecked(c3->netId, c1->netId, ai_OwnPlayer, fee);
            cm_SendMoveGoodsUnchecked(c4->netId, c1->netId, ai_OwnPlayer, fee);
        }
        cm_WaitTicks(6);
        io_PrintText("$C");
        io_PrintText(0x1557, plan.rating2c, c1->id);
        cm_WaitTicks(4);
    } else {
        bankVerdict = cm_LoadVoiceBank((int)"PROZESS_7_NICHT_SCHULDIG.sbf", 0);
        cm_QueueVoice(c2->id, bankVerdict, (int)"_PROZESS_7_NICHT_SCHULDIG", c1->sex == 1, 100);
        cm_QueueVoice(c2->id, bank8, (int)"_PROZESS_8", ((c0->sex == 1) & 1) + 2, 200);
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-3.esc", (int)ch_EmptyName);
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x152e, c1->id);
        cm_WaitTicks(4);
        io_PrintText("$C");
        io_PrintText(0x1557, plan.rating2c, c0->id);
        cm_WaitTicks(4);
        if (m->host == ai_Chars[ai_PlayerId].netId) {
            fee = plan.rating2c / 3;
            cm_SendMoveGoodsUnchecked(c2->netId, c0->netId, ai_OwnPlayer, fee);
            cm_SendMoveGoodsUnchecked(c3->netId, c0->netId, ai_OwnPlayer, fee);
            cm_SendMoveGoodsUnchecked(c4->netId, c0->netId, ai_OwnPlayer, fee);
        }
    }
    if (m->host == ai_Chars[ai_PlayerId].netId) {
        if (plan.tortured != 0 && plan.penalty < 0.0f)
            gm_AddHealth(c1->id, (int)plan.penalty);
        if (ai_InventoryHas(c1, 0x22e) != 0) {
            cm_SendRelations(c1->netId, c2->netId, -26, 1);
            cm_SendRelations(c0->netId, c2->netId, 26, 1);
            cm_SendRelations(c1->netId, c3->netId, -26, 1);
            cm_SendRelations(c0->netId, c3->netId, 26, 1);
            cm_SendRelations(c1->netId, c4->netId, -26, 1);
            cm_SendRelations(c0->netId, c4->netId, 26, 1);
        }
    }
    cm_FadeToBlack();
    cm_WaitEvent(cm_StartEventArg("cutscenes\\prozess\\prozess-ende.esc", (int)ch_EmptyName));
    cm_WaitEvent(cm_StartEventArg("cutscenes\\prozess\\prozess-exit.esc", (int)ch_EmptyName));
    if (sound != 0)
        msx_StopAmbient(sound);
    cm_EndSpecialTrack();
    snd_ClearQueue();
    if (bankTrial != 0)
        cm_UnloadVoiceBank(bankTrial);
    if (bankV != 0)
        cm_UnloadVoiceBank(bankV);
    if (bankVerdict != 0)
        cm_UnloadVoiceBank(bankVerdict);
    if (bank8 != 0)
        cm_UnloadVoiceBank(bank8);
    if (bankT1 != 0)
        cm_UnloadVoiceBank(bankT1);
    if (bankT2 != 0)
        cm_UnloadVoiceBank(bankT2);
    if (bankT3 != 0)
        cm_UnloadVoiceBank(bankT3);
    if (bankF1 != 0)
        cm_UnloadVoiceBank(bankF1);
    if (bankF2 != 0)
        cm_UnloadVoiceBank(bankF2);
    if (bankT4 != 0)
        cm_UnloadVoiceBank(bankF2);     /* the original frees bankF2 a second time here, not bankT4 */
}

#define CUT_BOX() d2_WinOpenSlider((d2_LastWindowPtr->w - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow)  /* the timer slider of a cutscene dialog */
#define CM_ASK_YESNO(r, yes, no)                                                    \
    while (sv_RunFrame(cm_TrialAskPlayer, cm_DialogFrameFlags) != 0) {              \
        if (cm_DialogTimer(dlg, 0, 0) == 0)                                         \
            sv_PopRequest = 1;                                                      \
        else if (d2_ClickedId != -1) {                                              \
            sv_PopRequest = 1;                                                      \
            if (d2_ClickedId == 1)                                                  \
                (r)->session.answer = (yes);                                        \
            else if (d2_ClickedId == 0)                                             \
                (r)->session.answer = (no);                                         \
        }                                                                           \
    }

/* 0x4dd960, the trial's member callback for the player's character (cm_RunSceneParty, scene kind 2): r is the member's record,
   m the trial's plan (cm_RunTrial). By m->mode the player, when he is the one asked, answers in a dialog ("cutscenes\prozess",
   "cutscenes\folterwahl") before the timer slider runs out (cm_DialogTimer, 1500 ms); the answer goes to r->session.answer:
   0 the accused's plea (ids[1], text 0x14bc: 1 guilty; his worth as the amount), 1 the accuser on the torture (ids[0], 0x14cb),
   2 the accuser's pick of three instruments in random order (0x14db / 0x14dc with their prices by the two worths), 3 the
   torturer's mercy (ids[5], 0x14e3; giving in raises his relation to the accuser by 0x23), 4 the vote of the judge or an
   assessor (ids[2] .. ids[4], 0x1529 ..), 5 the judge's sentence of three by the plan's rank (0x1532 / 0x1533, the ones
   gv_CheckSentence refuses disabled), 6 the accused's confession (0x1512). Without a click the default stays */

void cm_TrialAskPlayer(CMCUTINFO *r, AIPLAN *m)
{
    char prices[7] = {8, 10, 12, 15, 18, 21, 24};
    int btn[7];                         /* the instrument buttons */
    int order[7];
    int sel[3];                         /* the sentence buttons */
    /* @original c's initializer emits no code; it moves c's first reference, which VC6's tie key for
       the two gm_GetWorth calls of mode 2 follows: the original calls c's first (without it accused's) */
    AICHAR *c = 0, *accused;
    int view, dlg, rank, obj, i, k, t, sum;

    r->session.answer = -1;
    for (i = 0; i < 7; i++)
        btn[i] = -1;
    c = (AICHAR *)gm_FindChar(r->playerId);
    accused = (AICHAR *)gm_FindChar(m->ids[1]);
    switch (m->mode) {
    case 0:
        if (r->playerId != m->ids[1])
            return;
        r->session.amount = (float)accused->v024 + accused->f01c;
        view = d2_OpenForm(0, 0, "cutscenes\\prozess");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x14bc, c->id);
        d2_ClickedId = -1;
        r->session.answer = 0;
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        CM_ASK_YESNO(r, 0, 1)
        d2_CloseView(view);
        return;
    case 1:
        if (r->playerId != m->ids[0])
            return;
        view = d2_OpenForm(0, 0, "cutscenes\\prozess");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x14cb, c->id, accused->id);
        d2_ClickedId = -1;
        r->session.answer = 0;
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        CM_ASK_YESNO(r, 0, 1)
        d2_CloseView(view);
        return;
    case 2:
        if (r->playerId != m->ids[0])
            return;
        gm_Shuffled(7, order);
        sum = (gm_GetWorth(c->id) + gm_GetWorth(accused->id)) / 2000;
        view = d2_OpenForm(0, 0, "cutscenes\\folterwahl");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x14db, c->id);
        for (i = 0; i < 3; i++) {
            k = order[i];
            t = io_PrintText(0x14dc, k * 2 + 0xb20, (unsigned char)prices[k] * sum);
            btn[i] = d2_GetWinObj(view, 0, t);
        }
        d2_ClickedId = -1;
        r->session.answer = order[cm_Random(3)];
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        while (sv_RunFrame(cm_TrialAskPlayer, cm_DialogFrameFlags) != 0) {
            if (cm_DialogTimer(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                for (i = 0; i < 3; i++)
                    if (d2_PointerObj == btn[i]) {
                        r->session.answer = order[i];
                        sv_PopRequest = 1;
                        break;
                    }
            }
        }
        d2_CloseView(view);
        return;
    case 3:
        if (r->playerId != m->ids[5])
            return;
        view = d2_OpenForm(0, 0, "cutscenes\\folterwahl");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x14e3, c->id, accused->id, accused->id);
        d2_ClickedId = -1;
        r->session.answer = 0;
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        while (sv_RunFrame(cm_TrialAskPlayer, cm_DialogFrameFlags) != 0) {
            if (cm_DialogTimer(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    r->session.answer = 0;
                else if (d2_ClickedId == 0) {
                    r->session.answer = 1;
                    cm_SendRelations(c->netId, accused->netId, 0x23, 1);
                }
            }
        }
        d2_CloseView(view);
        return;
    case 6:
        if (r->playerId != m->ids[1])
            return;
        view = d2_OpenForm(0, 0, "cutscenes\\prozess");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x1512, c->id);
        d2_ClickedId = -1;
        r->session.answer = 0;
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        CM_ASK_YESNO(r, 1, 0)
        d2_CloseView(view);
        return;
    case 4:
        t = r->playerId;
        if (t != m->ids[2] && t != m->ids[3] && t != m->ids[4])
            return;
        view = d2_OpenForm(0, 0, "cutscenes\\prozess");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x1529, c->id);
        io_PrintText(0x152a);
        io_PrintText(0x152b);
        d2_ClickedId = -1;
        r->session.answer = 1;
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        CM_ASK_YESNO(r, 1, 0)
        d2_CloseView(view);
        return;
    case 5:
        if (r->playerId != m->ids[2])
            return;
        sel[0] = -1;
        sel[1] = -1;
        sel[2] = -1;
        rank = ai_PlanRank(m);
        view = d2_OpenForm(0, 0, "cutscenes\\prozess");
        d2_PlaceMainWindow(view, 3);
        d2_SetForm(view, 0);
        io_PrintText(0x1532, c->id);
        for (i = 0; i < 3; i++) {
            obj = d2_GetWinObj(view, 0, io_PrintText(0x1533, rank * 3 + 0x1534 + i * 3));
            sel[i] = obj;
            if (gv_CheckSentence(accused, i + rank) != 0)
                d2_SetObjEnabled(obj, 0);
        }
        if (sel[1] != -1)
            r->session.answer = rank + 1;
        else if (cm_Random(2) != 0)
            r->session.answer = rank + 2;
        else
            r->session.answer = rank;
        d2_ClickedId = -1;
        dlg = CUT_BOX();
        cm_DialogTimer(dlg, 0x5dc, 1);
        while (sv_RunFrame(cm_TrialAskPlayer, cm_DialogFrameFlags) != 0) {
            if (cm_DialogTimer(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_PointerObj == sel[0])
                    r->session.answer = rank;
                else if (d2_PointerObj == sel[1])
                    r->session.answer = rank + 1;
                else if (d2_PointerObj == sel[2])
                    r->session.answer = rank + 2;
            }
        }
        d2_CloseView(view);
        return;
    }
}
/* 0x4de340, the trial's ask-AI callback (cm_RunTrial's scene party, scene kind 2; the plan m, the member's record r): when
   r->playerId is the one the plan asks, the character answers its mode 0 .. 6 by the decision functions 0x494020 ..
   0x4946c0 and the answer goes to r->session.answer (mode 0 the plea's amount into r->session.amount too) */

void cm_TrialAskAI(CMCUTINFO *r, AIPLAN *m)
{
    AICHAR *c;
    int id, res;

    r->session.answer = -1;
    switch (m->mode) {
    case 0:
        if (r->playerId != m->ids[1])
            return;
        c = (AICHAR *)gm_FindChar(m->ids[1]);
        r->session.amount = (float)c->v024 + c->f01c;
        r->session.answer = ai_PlanWorth(m);
        return;
    case 1:
        if (r->playerId != m->ids[0])
            return;
        r->session.answer = ai_DecideTorture(m);
        return;
    case 2:
        if (r->playerId != m->ids[0])
            return;
        r->session.answer = ai_DecideTortureInstrument(m);
        return;
    case 3:
        if (r->playerId != m->ids[5])
            return;
        res = ai_DecideTortureMercy(m);
        r->session.answer = res;
        if (res == 1)
            cm_SendRelations(r->playerId, m->ids[1], 0x23, res);
        return;
    case 6:
        if (r->playerId != m->ids[1])
            return;
        r->session.answer = util_Random(2);
        return;
    case 4:
        id = r->playerId;
        if (id != m->ids[2] && id != m->ids[3] && id != m->ids[4])
            return;
        c = (AICHAR *)gm_FindChar(((AICHAR *)cm_CutChar)->netId);
        r->session.answer = ai_DecideAcquittal(c, m);
        return;
    case 5:
        if (r->playerId != m->ids[2])
            return;
        c = (AICHAR *)gm_FindChar(m->ids[2]);
        r->session.answer = ai_DecideSentence(c, m);
        return;
    }
}
/* 0x4de4b0, the sink-to-ground action's callback (cm_DuelSinkDown): a blood puddle "ub_Blutlache" turned at random at
   the character's place, as cm_FallDownAction without the sound */

void cm_SinkDownAction(CHACTION *a)
{
    D3VECTOR rot;
    D3OBJECT *b;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    rot.y = cm_Random(0x168) * 3.14159274f * 0.005555555555555556;
    b = d3_CreateObject(0, &a->ch->obj->pos, &rot, "ub_Blutlache");
    d3t_PlayAnim(b, "sonstiges\\ub_Blutlache.baf", 0x12);
}
/* 0x4de530, a fighter whose life (0x4fc8e0) is under 0.2 sinks to the ground ("bewegung/zu_boden_sinken"): 1 */

int cm_DuelSinkDown(char *c, int dmg)
{
    AICHAR *ch = (AICHAR *)c;
    CHACTION *o;

    if (gm_HealthRatio(c) < 0.2) {
        d3t_CreateBlood(ch->scene->obj, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
        ch_StopActions(ch->scene);
        o = ch_AddActionCB(ch->scene, cm_SinkDownAction, 0x12, 0x36, 1);
        strcpy(o->u.name, "bewegung/zu_boden_sinken");
        ch->scene->obj->kind = 0;
        cm_DuelOver = 1;
        return 1;
    }
    return 0;
}
/* 0x4de5e0, starts the duel's variant order: gm_Shuffled (0x505ca0) fills cm_DuelVariants with 0 .. n - 1, the count and
   the cursor are set */

void cm_DuelInitVariants(int n)
{
    gm_Shuffled(n, cm_DuelVariants);
    cm_DuelVariantCount = n;
    cm_DuelVariantPos = 0;
}
/* 0x4de610, the next duel variant: the cursor's entry of cm_DuelVariants modulo n, the cursor advanced */

int cm_DuelVariant(int n)
{
    int i, r;

    i = cm_DuelVariantPos % cm_DuelVariantCount;
    r = cm_DuelVariants[i] % n;
    cm_DuelVariantPos = cm_DuelVariantPos + 1;
    return r;
}
/* 0x4de640, the pistol shot of a duel (the action cm_DuelRound queues: a->shot.shooter shoots at a->shot.target): the hit
   chance gm_GetSkill(shooter, 3) * 0.8 (* 1.5 with the item 0x16f, * 0.66 for the fighter who lost the insult round), on a hit
   the damage 5 + (0..15) when the other won the roll round, else 15 + (0..30) (* 0.75 when the target has the item 0x2b5), added
   to the target's score byte cm_DuelScore[0] / [1] and shown (cm_ShowDamage); the shot sound, then the target sinks
   (cm_DuelSinkDown: the duel is over) or, on a hit, bleeds, staggers and loses the damage from v024 and its health (text
   0x1102 + variant), on a miss dodges (text 0x1109 + variant) */

void cm_DuelShotAction(CHACTION *a)
{
    AICHAR *sh;
    AICHAR *tg;
    float acc, b;
    int hit, dmg, n;

    d2_SetForm(cm_TextBox, 0);
    sh = a->shot.shooter;
    tg = a->shot.target;
    acc = (float)gm_GetSkill((char *)sh, 3);
    gm_GetSkill((char *)tg, 3);
    acc = acc * 0.8;
    if (ai_InventoryHas(sh, 0x16f) != 0)
        acc = (float)(acc * 1.5);
    if (((int)sh == cm_DuelLeft && cm_DuelRightInsultWon != 0) || ((int)sh == cm_DuelRight && cm_DuelLeftInsultWon != 0))
        acc = (float)(acc * 0.66);
    if ((float)cm_RandomFloat() < acc)
        hit = 1;
    else
        hit = 0;
    if (hit) {
        if (((int)sh == cm_DuelLeft && cm_DuelRightRollWon != 0) || ((int)sh == cm_DuelRight && cm_DuelLeftRollWon != 0))
            dmg = cm_Random(0xf) + 5;
        else
            dmg = cm_Random(0x1e) + 0xf;
        if (ai_InventoryHas(tg, 0x2b5) != 0)
            dmg = (int)(dmg * 0.75f);
        if ((int)tg == cm_DuelLeft)
            cm_DuelScore[0] += dmg;
        else
            cm_DuelScore[1] += dmg;
        cm_ShowDamage((char *)tg, (int)((tg->f01c * 0.01) * dmg), 0);
    } else
        dmg = 0;
    sb_PlaySample(0, "Schuss", 1, 0x3f, 0x7f, 0, 0);
    if (cm_DuelSinkDown((char *)tg, dmg) == 0) {
        if (hit != 0) {
            d3t_CreateBlood(tg->scene->obj, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
            if (tg->scene->actions == 0)
                ch_QueueAnim(tg->scene, "kampf/taumeln", 1);
            d2_SetForm(cm_TextBox, 0);
            n = cm_DuelVariant(6);
            io_PrintText(n + 0x1102, tg->id);
            snd_PlayVoice(tg->id, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_TREFFER", n);
            b = (float)(tg->f01c * 0.01 * dmg);
            tg->v024 = (int)(tg->v024 - b);
            gm_AddHealth(tg->id, (int)-b);
        } else {
            if (tg->scene->actions == 0)
                ch_QueueAnim(tg->scene, "kampf/stich_ausweichen", 1);
            n = cm_DuelVariant(9);
            io_PrintText(n + 0x1109, tg->id);
            snd_PlayVoice(tg->id, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_DANEBEN", n);
        }
    } else
        cm_DuelOver = 1;
}
/* 0x4de900, one fighter's turn of a duel round, by the round its player chose (CMCUTINFO duel.round): 2 the insult
   (texts 0x10e8 / 0x10e9 + variant, "gestik/abfaellig"; a's skill 4 + a roll against b's: the win sets a's insult byte
   cm_DuelLeftInsultWon / cm_DuelRightInsultWon, texts 0x10f2 .. 0x10f5 by b's kind), 3 a roll against a's skill 2 (the win sets cm_DuelLeftRollWon /
   cm_DuelRightRollWon, texts 0x10f6 .. 0x10f9), 4 the pistol shot (texts 0x10fa + variant, the action cm_DuelShotAction at b); nothing once
   the duel is over. side ("A" / "B") is not read */

void cm_DuelRound(AICHAR *a, AICHAR *b, char *side)
{
    CMCUTINFO *rec;
    CHACTION *act;
    int n;
    float fa, fb;

    if (cm_DuelOver != 0)
        return;
    rec = (CMCUTINFO *)cm_CutInfoById(a->netId);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    switch (rec->duel.round) {
    case 4:
        d2_SetForm(cm_TextBox, 0);
        n = cm_DuelVariant(7);
        io_PrintText(n + 0x10fa, a->id);
        snd_PlayVoice(a->id, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN", n);
        cm_WaitTicks(2);
        io_PrintText("$N");
        act = ch_AddActionCB(a->scene, cm_DuelShotAction, 6, 0x2e, 1);
        act->shot.target = b;
        act->shot.shooter = a;
        strcpy(act->u.name, "kampf/schiessen_pistole");
        break;
    case 3:
        d2_SetForm(cm_TextBox, 0);
        fa = gm_GetSkill((char *)a, 2);
        if ((float)cm_RandomFloat() < fa) {
            if (a->kind == 6 || a->kind == 7)
                io_PrintText(0x10f6, a->id);
            else
                io_PrintText(0x10f7, a->id);
            if ((int)a == cm_DuelLeft)
                cm_DuelLeftRollWon = 1;
            else
                cm_DuelRightRollWon = 1;
        } else {
            if (a->kind == 6 || a->kind == 7)
                io_PrintText(0x10f8, a->id);
            else
                io_PrintText(0x10f9, a->id);
        }
        io_PrintText("$N");
        break;
    case 2:
        d2_SetForm(cm_TextBox, 0);
        io_PrintText(0x10e8, a->id);
        n = cm_DuelVariant(8);
        io_PrintText(n + 0x10e9);
        snd_PlayVoice(a->id, 0, "_DUELL_INTRO_C2_AUSWAHL_LAESTERN", n);
        io_PrintText("$N");
        ch_QueueAnim(a->scene, "gestik/abfaellig", 1);
        cm_WaitTicks(2);
        fa = gm_GetSkill((char *)a, 4);
        fb = gm_GetSkill((char *)b, 4);
        fa = (float)(cm_RandomFloat() + fa);
        if ((float)(cm_RandomFloat() + fb) < fa) {
            d2_SetForm(cm_TextBox, 0);
            if (b->kind == 6 || b->kind == 7)
                io_PrintText(0x10f2, b->id);
            else
                io_PrintText(0x10f3, b->id);
            if ((int)a == cm_DuelLeft)
                cm_DuelLeftInsultWon = 1;
            else
                cm_DuelRightInsultWon = 1;
        } else {
            d2_SetForm(cm_TextBox, 0);
            if (b->kind == 6 || b->kind == 7)
                io_PrintText(0x10f4, b->id);
            else
                io_PrintText(0x10f5, b->id);
        }
        io_PrintText("$N");
        break;
    }
    cm_WaitTicks(4);
}
/* 0x4debe0, the duel's outcome without the scene (cut_MainDuell when the events are skipped): of the duellists
   m->members[0] / [1] the one with the lower gm_GetSkill(.., 3) (a on a tie) loses an office he holds
   (amt_SendOfficeMessage 4) when m's host is the player's character, and every character of the kinds 6 / 7 gets the news
   0x10c7 (with 0x10c8 + his sex) */

void cm_DuelOutcome(G28REC *m)
{
    char buf[0x400];
    AICHAR *a, *b, *w, *c;
    float fa;
    AMTKIND kind;
    AMTREC office;
    int i, txt;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (a == 0 || b == 0)
        return;
    fa = gm_GetSkill((char *)a, 3);
    w = b;
    if (!(gm_GetSkill((char *)b, 3) < fa))
        w = a;
    if (w->officeKind == 0)
        return;
    if (amt_FindOffice((char *)w, &kind, &office) == 0)
        return;
    if (m->host != ai_Chars[ai_PlayerId].netId)
        return;
    amt_SendOfficeMessage(office.idx, 0, 0, 4, 0xff);
    for (i = 0; i < 768; i++) {
        c = &ai_Chars[i];
        if (c->kind == 6 || c->kind == 7) {
            if (w->sex != 0)
                txt = 0x10c9;
            else
                txt = 0x10c8;
            txt_Format(buf, 0x10c7, w->id, txt);
            he_ShowNews(c->netId, -1, buf, 0, 0x5fe, &ch_EmptyName);
        }
    }
}
/* 0x4ded10, "cut_MainDuell": the duel of the characters m->members[0] / [1] (copies ca / cb; with cm_SkipEvents set only
   the outcome, cm_DuelOutcome): the scene sc[m->variant], light, fog and music by the season, the scene
   "cutscenes\\duell\\enter_DUELL.esc", the duellists put at "dummy_DUELLANT_A_ENTRY" / "_B_ENTRY" with pistols, the three
   intro voices, the walk to the start and shot marks; a side whose player chose the cheat (round 1) walks to its
   "dummy_DUELL_BETRUG" mark and its second shoots at the other (cm_DuelShotAction); then up to three rounds of cm_DuelRound
   for both sides (the doctor's text when both won one) until a fighter is down (cm_DuelOver): the loser is the one with the
   lower health (text 0x111d), else a draw (0x111c); for each side the damage text (0x111e .. 0x1121: the cheater's own
   lines) and the damage taken off its health and v024; the loser loses an office he holds (texts 0x1123 + sex, the news
   0x10c7 to every character of the kinds 6 / 7, when m's host is the player's character) or the draw text 0x1122; the
   player's host sends the relations of both; then the scene is torn down */

void cm_RunDuel(G28REC *m)
{
    AICHAR *a, *b, *w;
    AICHAR ca, cb;
    AMTKIND kind;
    AMTREC office;
    CMCUTINFO *rec;
    CHACTION *act;
    char buf[200];
    char text[0x400];
    int i, n, r, v, snd1, snd2;
    D3OBJECT *mark;
    int scene;
    int hita, hitb;
    float fa;

    {
        char sc[6][0x20] = {"duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3"};
        int k;

        hita = 0;
        hitb = 0;
        r = util_Random(3) - 5;
        w = 0;
        a = (AICHAR *)gm_FindChar(m->members[0]);
        b = (AICHAR *)gm_FindChar(m->members[1]);
        if (a == 0 || b == 0)
            return;
        if (cm_SkipEvents != 0) {
            cm_DuelOutcome(m);
            return;
        }
        cm_ResetCutInfos(m);
        v = m->variant;
        cm_LoadEventScene(sc[v]);
        cm_DuelInitVariants(10);
        cm_ApplySeason();
        k = gm_ClockSeason(&ai_Time);
        if (k == 3)
            k = 0;
        else if (k == 0 || k == 2)
            k = 1;
        else if (k == 1)
            k = 2;
        d3_SetLightSet(k, 0, 1.0f, 1);
        d3_SetFogSet(0, 1, 0);
        cm_InitDamagePops();
        if (k == 0)
            cm_PlaySpecialTrack("cd2\\satisfaktion.mp3");
        else if (k == 1)
            cm_PlaySpecialTrack("cd2\\DerSeuchenzug.mp3");
        else
            cm_PlaySpecialTrack("cd2\\AmKuehlenGrabe.mp3");
    }
    snd1 = (int)sb_LoadSampleBank("duell_sfx.sbf", 0);
    snd2 = snd_LoadVoiceBank("duell.sbf", 0);
    scene = cm_StartScript("cutscenes\\duell\\enter_DUELL.esc");
    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    ca = *a;
    cb = *b;
    cm_DuelLeft = (int)&ca;
    cm_DuelRight = (int)&cb;
    cm_DuelScore[0] = 0;
    cm_DuelScore[1] = 0;
    gm_CreateCharModel(&ca, d3_CurrentScene, 0, 0, 0, "dummy_DUELLANT_A_ENTRY");
    gm_CreateCharModel(&cb, d3_CurrentScene, 0, 0, 0, "dummy_DUELLANT_B_ENTRY");
    if (ca.scene == 0 || cb.scene == 0) {
        err_Message("cut_MainDuell(): Could not create character");
        return;
    }
    ch_SetHandObject(ca.scene, 2, "ob_PISTOLE");
    ca.scene->pathType = 4;
    ch_SetHandObject(cb.scene, 2, "ob_PISTOLE");
    cb.scene->pathType = 4;
    for (i = 0; i < 3; i++) {
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(v * 3 + 0x10cb + i);
        sprintf(buf, "_DUELL_INTRO_NEU_0%d", i);
        n = cm_PlayVoice(-7, 0, buf, -1);
        while (n != 0 && sb_IsPlaying(n) != 0)
            cm_WaitTicks(2);
    }
    ch_WalkToObject(ca.scene, d3_FindObject(0, 0x100, 0, "dummy_DUELL_START_A"));
    ch_AddAction(ca.scene, 0x35, d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_A"));
    ch_WalkToObject(cb.scene, d3_FindObject(0, 0x100, 0, "dummy_DUELL_START_B"));
    ch_AddAction(cb.scene, 0x35, d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_B"));
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    io_PrintText(0x10dd);
    cm_WaitTicks(4);
    io_PrintText("$C");
    snd_PlayVoice(-7, 0, "_DUELL_INTRO_BEGRUESSUNG_RUECKEN", -1);
    cm_RunSceneParty(m, 0);

    /* a cheater (round 1) walks to his cheat mark, and his second shoots at the other */
    rec = (CMCUTINFO *)cm_CutInfoById(ca.netId);
    if (rec->duel.round == 1)
        mark = d3_FindObject(0, 0x100, 0, "dummy_DUELL_BETRUG_A");
    else
        mark = d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_A");
    ch_WalkToObject(ca.scene, mark);
    ch_AddAction(ca.scene, 0x35, cb.scene->obj);
    if (rec->duel.round == 1) {
        n = cm_DuelVariant(6);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText(n + 0x10df);
        io_PrintText("$A");
        snd_PlayVoice(r, snd2, "_DUELL_INTRO_C2_SEKUNDANTEN", n);
        act = ch_AddActionCB(ca.scene, cm_DuelShotAction, 6, 0x2e, 1);
        act->shot.target = &cb;
        act->shot.shooter = &ca;
        strcpy(act->u.name, "kampf/schiessen_pistole");
        hita = 1;
    }
    rec = (CMCUTINFO *)cm_CutInfoById(cb.netId);
    if (rec->duel.round == 1)
        mark = d3_FindObject(0, 0x100, 0, "dummy_DUELL_BETRUG_B");
    else
        mark = d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_B");
    ch_WalkToObject(cb.scene, mark);
    ch_AddAction(cb.scene, 0x35, ca.scene->obj);
    if (rec->duel.round == 1) {
        n = cm_DuelVariant(6);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText(n + 0x10df);
        snd_PlayVoice(r, snd2, "_DUELL_INTRO_C2_SEKUNDANTEN", n);
        act = ch_AddActionCB(cb.scene, cm_DuelShotAction, 6, 0x2e, 1);
        act->shot.target = &ca;
        act->shot.shooter = &cb;
        strcpy(act->u.name, "kampf/schiessen_pistole");
        hitb = 1;
    }
    cm_WaitTicks(6);

    /* the rounds */
    cm_DuelOver = 0;
    for (i = 0; i < 3 && cm_DuelOver == 0; i++) {
        cm_RunSceneParty(m, m);
        rec = (CMCUTINFO *)cm_CutInfoById(cb.netId);
        cm_DuelLeftInsultWon = 0;
        cm_DuelRightInsultWon = 0;
        cm_DuelLeftRollWon = 0;
        cm_DuelRightRollWon = 0;
        if (rec->duel.round == 2 || rec->duel.round == 3) {
            cm_DuelRound(&cb, &ca, "B");
            cm_DuelRound(&ca, &cb, "A");
        } else {
            cm_DuelRound(&ca, &cb, "A");
            cm_DuelRound(&cb, &ca, "B");
        }
        if ((cm_DuelLeftInsultWon != 0 || cm_DuelLeftRollWon != 0) && (cm_DuelRightInsultWon != 0 || cm_DuelRightRollWon != 0)) {
            n = cm_Random(9);
            d2_SetForm(cm_TextBox, 0);
            io_PrintText(n + 0x1113);
            snd_PlayVoice(-5, 0, "_DUELL_INTRO_C2_DOKTOR", n);
            cm_WaitTicks(5);
        }
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
    }

    /* the result: the one with the lower health is down */
    if (cm_DuelOver != 0) {
        fa = gm_HealthRatio((short *)&ca);
        if ((float)gm_HealthRatio((short *)&cb) < fa)
            w = &cb;
        else
            w = &ca;
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x111d, w->id, w->id);
        cm_WaitTicks(5);
    } else {
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x111c);
    }
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    if (hita == 0) {
        if (cm_DuelScore[0] != 0) {
            io_PrintText(0x111e, ca.id);
            gm_AddHealth(ca.id, (int)(cm_DuelScore[0] * 5.0));
            ca.v024 = (int)(ca.v024 + cm_DuelScore[0] * 5.0);
        } else
            io_PrintText(0x111f, ca.id, ca.id);
    } else if (cm_DuelScore[0] != 0)
        io_PrintText(0x1120, ca.id);
    else
        io_PrintText(0x1121, ca.id);
    cm_WaitTicks(4);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    if (hitb == 0) {
        if (cm_DuelScore[1] != 0) {
            io_PrintText(0x111e, cb.id);
            gm_AddHealth(cb.id, (int)(cm_DuelScore[1] * 5.0));
            cb.v024 = (int)(cb.v024 + cm_DuelScore[1] * 5.0);
        } else
            io_PrintText(0x111f, cb.id, cb.id);
    } else if (cm_DuelScore[1] != 0)
        io_PrintText(0x1120, cb.id);
    else
        io_PrintText(0x1121, cb.id);
    cm_WaitTicks(4);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");

    /* the loser's office */
    if (w != 0) {
        if (w->officeKind != 0) {
            io_PrintText(w->sex + 0x1123, w->id);
            snd_PlayVoice(-8, 0, "_DUELL_INTRO_D2", w->sex + 1);
            if (amt_FindOffice((char *)w, &kind, &office) != 0 && m->host == ai_Chars[ai_PlayerId].netId) {
                amt_SendOfficeMessage(office.idx, 0, 0, 4, 0xff);
                for (i = 0; i < 768; i++) {
                    AICHAR *c = &ai_Chars[i];

                    if (c->kind == 6 || c->kind == 7) {
                        int txt;

                        if (w->sex != 0)
                            txt = 0x10c9;
                        else
                            txt = 0x10c8;
                        txt_Format(text, 0x10c7, w->id, txt);
                        he_ShowNews(c->netId, -1, text, 0, 0x5fe, &ch_EmptyName);
                    }
                }
            }
        }
    } else {
        io_PrintText(0x1122, ca.id, cb.id);
        snd_PlayVoice(-8, 0, "_DUELL_INTRO_D2", 0);
    }
    cm_WaitTicks(5);
    if (ai_Chars[ai_PlayerId].netId == m->host) {
        cm_SendRelations(ca.netId, cb.netId, 0xf, 1);
        cm_SendRelations(cb.netId, ca.netId, 0xf, 1);
    }
    cm_FadeToBlack();
    cm_DeleteChar(ca.scene);
    ca.scene = 0;
    cm_DeleteChar(cb.scene);
    cb.scene = 0;
    cm_StopScript(scene);
    cm_WaitEventFull(scene);
    cm_LeaveEventScene();
    sb_UnloadSampleBank(snd2);
    sb_UnloadSampleBank(snd1);
    cm_ClearDamagePops();
    cm_EndSpecialTrack();
}
/* 0x4dfa20, the duel's aftermath (the fourth callback of the duel event, cm_RegisterSceneKind kind 4) for the duellists
   m->members[0] / [1]: when one of them is held (heldRounds) the players' characters (kinds 6 / 7) among them get the news
   txt_DuelHeldNews; else when one is dead (gm_HealthRatio <= 0) the other's player gets txt_DuelDeathNews; else each
   player's character among them sends a note naming the other (text 0x11ca + txt_DuelNoteTail, cm_SendPartyNote) and the
   duel is accused (gv_Accuse 0x14, by the building of a, else of b). Returns 1 when a note went out */

int cm_DuelAftermath(G28REC *m)
{
    char buf[0x200];
    AICHAR *a, *b;
    int flag;
    int bld;

    flag = 0;
    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (a == 0 || b == 0)
        return 0;
    if (b->heldRounds == 0 && a->heldRounds == 0) {
        if (gm_HealthRatio((short *)a) <= 0.0f) {
            if (b->kind != 6 && b->kind != 7)
                return 0;
            he_ShowNews(ai_Chars[b->id].netId, -1, txt_DuelDeathNews, 0, 0x5ec, 0);
            return 0;
        }
        if (gm_HealthRatio((short *)b) <= 0.0f) {
            if (a->kind != 6 && a->kind != 7)
                return 0;
            he_ShowNews(ai_Chars[a->id].netId, -1, txt_DuelDeathNews, 0, 0x5ec, 0);
            return 0;
        }
        if (a->kind == 6 || a->kind == 7) {
            txt_Format(buf, 0x11ca, b->id);
            strcat(buf, txt_DuelNoteTail);
            cm_SendPartyNote(a, m, buf, &ch_EmptyName);
            flag = 1;
        }
        if (b->kind == 6 || b->kind == 7) {
            txt_Format(buf, 0x11ca, a->id);
            strcat(buf, txt_DuelNoteTail);
            cm_SendPartyNote(b, m, buf, &ch_EmptyName);
            flag = 1;
        }
        if (flag != 0) {
            bld = -1;
            if (a->bld2 != 0)
                bld = a->bld2->id;
            else if (b->bld2 != 0)
                bld = b->bld2->id;
            gv_Accuse(0x14, 1, a->netId, b->netId, bld);
        }
        return flag;
    }
    if (b->kind == 6 || b->kind == 7)
        he_ShowNews(ai_Chars[b->id].netId, -1, txt_DuelHeldNews, 0, 0x5ec, 0);
    if (a->kind == 6 || a->kind == 7)
        he_ShowNews(ai_Chars[a->id].netId, -1, txt_DuelHeldNews, 0, 0x5ec, 0);
    return 0;
}
/* 0x4dfcb0, the player's choice of the duel round as a dialog ("cutscenes\\duell", text 0x10de with cm_CutChar's
   name, the timer slider): r->duel.round = 1 (the cheat) for the answer 0, 0 for the answer 1 and when the time runs
   out; r's text cleared */

void cm_DuelRoundDialog(CMCUTINFO *r, G28REC *m)
{
    int view, dlg;

    view = d2_OpenForm(0, 0, "cutscenes\\duell");
    d2_PlaceMainWindow(view, 3);
    d2_SetForm(view, 0);
    io_PrintText(0x10de, ((AICHAR *)cm_CutChar)->id);
    r->duel.round = 0;
    r->text[0] = ch_EmptyName[0];
    dlg = CUT_BOX();
    cm_DialogTimer(dlg, cm_SceneDuration * 0x4b, 1);
    while (sv_RunFrame(cm_DuelRoundDialog, cm_DialogFrameFlags) != 0) {
        if (cm_DialogTimer(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            r->duel.round = 1;
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            r->duel.round = 0;
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(view);
}
/* 0x4dfdd0, the player's choice of the duel round when the party scene runs ("cutscenes\\duell", text 0x10e6 with
   cm_ScenePartyRuns, cm_CutChar's name and the opponent's: of m->members[0] / [1] the one that is not cm_CutChar):
   r->duel.round = 2 / 3 / 4 for the three answers, 4 when the time runs out; r's text cleared */

void cm_DuelChooseRound(CMCUTINFO *r, G28REC *m)
{
    AICHAR *a, *b, *other;
    int view, first, insult, roll, shot, dlg;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    other = a->netId == ((AICHAR *)cm_CutChar)->netId ? b : a;
    view = d2_OpenForm(0, 0, "cutscenes\\duell");
    d2_PlaceMainWindow(view, 3);
    d2_SetForm(view, 0);
    first = io_PrintText(0x10e6, cm_ScenePartyRuns, ((AICHAR *)cm_CutChar)->id, other->id);
    insult = d2_GetWinObj(view, 0, first);
    roll = d2_GetWinObj(view, 0, first + 1);
    shot = d2_GetWinObj(view, 0, first + 2);
    r->duel.round = 4;
    r->text[0] = ch_EmptyName[0];
    dlg = CUT_BOX();
    cm_DialogTimer(dlg, cm_SceneDuration * 0x4b, 1);
    while (sv_RunFrame(cm_DuelChooseRound, cm_DialogFrameFlags) != 0) {
        cm_UpdateDamagePops();
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == insult)
                r->duel.round = 2;
            else if (d2_PointerObj == roll)
                r->duel.round = 3;
            else if (d2_PointerObj == shot)
                r->duel.round = 4;
            sv_PopRequest = 1;
        }
        if (cm_DialogTimer(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(view);
}
/* 0x4dff80, the answer of a computer duellist (the member record r's duel.round): with cm_ScenePartyRuns == 0 the
   question of cm_DuelRoundDialog (1 / 0 by a roll of 10 > 7), else the choice of cm_DuelChooseRound (4 with 50 %, 2 with
   35 %, 3 with 15 %) */

void cm_DuelAskAI(CMCUTINFO *r)
{
    int n;

    if (cm_ScenePartyRuns == 0) {
        if ((unsigned int)cm_Random(10) > 7)
            r->duel.round = 1;
        else
            r->duel.round = 0;
        return;
    }
    n = cm_Random(100);
    if (n > 0x32)
        r->duel.round = 4;
    else if (n > 0xf)
        r->duel.round = 2;
    else
        r->duel.round = 3;
}
/* 0x4dfff0, the player's round of the duel (the player's callback of the scene kind 4): the round dialog
   cm_DuelRoundDialog without the party scene, else the party's choice cm_DuelChooseRound */

void cm_DuelAskPlayer(CMCUTINFO *r, G28REC *m)
{
    if (cm_ScenePartyRuns == 0)
        cm_DuelRoundDialog(r, m);
    else
        cm_DuelChooseRound(r, m);
}
/* 0x4e0010, the run function of scene kind 0, the picture show: loads the scene "cutscene_toby.ed3", runs the party
   twice (its members' texts, the kind's callbacks are the noop) and leaves the scene */

void cm_RunPictureShow(G28REC *m)
{
    cm_LoadEventScene("cutscene_toby.ed3");
    cm_RunSceneParty((char *)m, 0);
    cm_RunSceneParty((char *)m, 0);
    cm_WaitTicks(5);
    cm_LeaveEventScene();
}
/* 0x4e0040, the execution's aftermath (the fourth callback of the execution event, cm_RegisterSceneKind kind 5 with the scene
   0x4e01e0) for the delinquent m->members[0]: unless m->quiet is set, the delinquent's player (kinds 6 / 7) gets the note
   text 0x11c5 + txt_ExecNoteTail; then every player's character among the other members gets the note text 0x11c8 with
   the delinquent's name (cm_SendPartyNote). The loop is written in both branches, as the original has it twice.
   Returns 0 when the delinquent is not found, else 1 */

int cm_ExecutionAftermath(G28REC *m)
{
    char buf[0x200];
    AICHAR *c, *o;
    int i;

    if (m->quiet == 0) {
        c = (AICHAR *)gm_FindChar(m->members[0]);
        if (c == 0)
            return 0;
        if (c->kind == 6 || c->kind == 7) {
            txt_Format(buf, 0x11c5);
            strcat(buf, txt_ExecNoteTail);
            cm_SendPartyNote(c, m, buf, &ch_EmptyName);
        }
        txt_Format(buf, 0x11c8, c->id);
        for (i = 1; i < m->used; i++) {
            o = (AICHAR *)gm_FindChar(m->members[i]);
            if (o != 0 && (o->kind == 6 || o->kind == 7))
                cm_SendPartyNote(o, m, buf, &ch_EmptyName);
        }
    } else {
        c = (AICHAR *)gm_FindChar(m->members[0]);
        if (c == 0)
            return 0;
        txt_Format(buf, 0x11c8, c->id);
        for (i = 1; i < m->used; i++) {
            o = (AICHAR *)gm_FindChar(m->members[i]);
            if (o != 0 && (o->kind == 6 || o->kind == 7))
                cm_SendPartyNote(o, m, buf, &ch_EmptyName);
        }
    }
    return 1;
}
#define CUT_NAMEREC(c) ((c) != 0 ? (char *)gm_CharNameRecord((c)->id) : (char *)gm_NameRecord(util_Random(2), util_Random(0x5e) + 1))  /* a character's name record, a random one for none */
/* 0x4e01e0, one execution (the scene callback of the execution event, cm_RegisterSceneKind kind 5) of the delinquent
   m->members[0] with the others m->members[1] .. [3], the way m->variant 0 .. 4 (head, gallows (1, 2), shovel, cannon):
   the scene "Richtplatz.ed3" with a random sky of a random weather (1 heavy, 2 medium, else fair; the heavy one also calls
   cm_StartSceneRain(500)), the track "ArmerDelinquent", the four names (members [1], [0], [2], [3], a random name for one
   not found) to "hinrichtung-create", the intro; at the host's player the delinquent's health goes down by (f01c + 5);
   the way's scene, the pause dur[way], the exit scene; then cm_StopSceneRain unless the weather was fair (w 0, as the
   original tests it), the sky destroyed. Nothing when the delinquent is not found */

void cm_RunExecution(G28REC *m)
{
    int w = cm_Random(3);
    int dur[5] = {0x36b0, 0x2ee0, 0x2ee0, 0x3a98, 0x2710};
    char buf[0x100];
    AICHAR *v, *p1, *p2, *p3;
    char *end;

    v = (AICHAR *)gm_FindChar(m->members[0]);
    if (v == 0)
        return;
    p1 = (AICHAR *)gm_FindChar(m->members[1]);
    p2 = (AICHAR *)gm_FindChar(m->members[2]);
    p3 = (AICHAR *)gm_FindChar(m->members[3]);
    cm_DialogFrameFlags = 0x702c6;
    d2_CutsceneActive = 1;
    cm_LoadEventScene("Richtplatz.ed3");
    d3_SetLightSet(0, 0, 1.0f, 1);
    d3_SetFogSet(0, 0, 0);
    if (w == 1)
        cm_CreateSceneSky(sv_SkyHeavy[cm_Random(3)].name);
    else if (w == 2)
        cm_CreateSceneSky(sv_SkyMedium[cm_Random(3)].name);
    else
        cm_CreateSceneSky(sv_SkyFair[cm_Random(4)].name);
    cm_PlaySpecialTrack("cd2\\ArmerDelinquent.mp3");
    if (w == 1)
        cm_StartSceneRain(0x1f4);
    sprintf(buf, "%s ", CUT_NAMEREC(p1) + 4);
    end = buf + strlen(buf);
    sprintf(end, "%s ", CUT_NAMEREC(v) + 4);
    end = buf + strlen(buf);
    sprintf(end, "%s ", CUT_NAMEREC(p2) + 4);
    end = buf + strlen(buf);
    sprintf(end, "%s", CUT_NAMEREC(p3) + 4);
    cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-create.esc", buf);
    cm_WaitEvent(cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-intro.esc", &ch_EmptyName));
    if (m->host == ai_CharInts[ai_PlayerId][0])
        gm_AddHealth(v->id, (int)-(v->f01c + 5.0f));
    switch (m->variant) {
    case 0:
        cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-kopf.esc", &ch_EmptyName);
        break;
    case 1:
    case 2:
        cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-galgen.esc", &ch_EmptyName);
        break;
    case 3:
        cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-shaufen.esc", &ch_EmptyName);
        break;
    case 4:
        cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-kanone.esc", &ch_EmptyName);
        break;
    }
    cm_RunSceneTicks(dur[m->variant]);
    cm_FadeToBlack();
    cm_WaitEvent(cm_StartEventArg("cutscenes\\hinrichtung\\hinrichtung-exit.esc", &ch_EmptyName));
    if (w != 0)
        cm_StopSceneRain();
    cm_DestroySceneSky();
    cm_FightStart = 0;
    cm_LeaveEventScene();
    d2_CutsceneActive = 0;
    cm_EndSpecialTrack();
}
/* 0x4e0580, the check function of the wedding (scene kind 6, run function 0x4e0770) for its party m: of its two
   members m->members[0] / [1] the first gets the note text 0x11c3 naming the second (cm_SendPartyNote) when both exist
   and are of different sex; when the first is a player's character (kind 6 / 7) or an official (kind 5) every other
   player's character gets the news text 0x11c4 with both names (he_ShowNews). 1 when the first is a player's character,
   else both get the field 0x1c8 (cm_SendSetField) and 0 */

int cm_CheckWedding(G28REC *m)
{
    char buf[0x200];
    AICHAR *a, *b;
    int i;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (a != 0) {
        if (b != 0 && a->sex != b->sex) {
            if (a->kind == 6 || a->kind == 7) {
                txt_Format(buf, 0x11c3, b->id);
                cm_SendPartyNote(a, m, buf, "_PA_WERBUNG_EVENT_ZUSAGE_M1_00");
            }
            if (a->kind == 6 || a->kind == 7 || a->kind == 5) {
                txt_Format(buf, 0x11c4, a->id, b->id);
                for (i = 0; i < 768; i++) {
                    if ((ai_Chars[i].kind == 6 || ai_Chars[i].kind == 7) && ai_Chars[i].netId != a->netId &&
                        ai_Chars[i].netId != b->netId)
                        he_ShowNews(ai_Chars[i].netId, -1, buf, 0, 0x5fe, "_PA_WERBUNG_EVENT_NACHRICHT_HOCHZEIT_M1_00");
                }
            }
            if (a->kind == 6 || a->kind == 7)
                return 1;
        }
        cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x40000);
    }
    if (b != 0)
        cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x40000);
    return 0;
}
/* 0x4e06d0, ends a wedding scene s (s[0] the scene, s[1] its view, s[2] a sample bank, s[3] a playing sample): the exit scene
   "cutscenes\\hochzeit\\hochzeit-exit.esc", the globals reset. Returns 1 */

int cm_EndWedding(int *s)
{
    int v;

    cm_FadeToBlack();
    if (s[3] != 0)
        sb_Stop(s[3], 0);
    v = s[2];
    s[3] = 0;
    sb_UnloadSampleBank(v);
    if (s[1] != -1)
        d2_CloseView(s[1]);
    if (s[0] != -1)
        cm_StopScript(s[0]);
    s[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-exit.esc", &ch_EmptyName);
    cm_WaitEvent(s[0]);
    s[0] = -1;
    s[1] = -1;
    cm_FightStart = 0;
    cm_LeaveEventScene();
    cm_EndSpecialTrack();
    d2_CutsceneActive = 0;
    return 1;
}

#define WED_STEP(ms) if ((r = cm_RunSceneTicksOrClick((ms), cm_EndWedding, st)) != 0) return r  /* a pause; a click ends the wedding */
#define WED_WAIT() if ((r = cm_WaitEventOrClick(st[0], cm_EndWedding, st)) != 0) return r  /* waits for the speech scene st[0] */
#define WED_VOICE(tag) st[3] = cm_PlayVoice(-7, st[2], (tag), 0); while (sb_IsPlaying(st[3]) != 0) { WED_STEP(0x3e8); }  /* plays a voice of the bank and waits for it */
/* 0x4e0770, the wedding (the scene callback of the wedding event) of the couple m->members[0] / [1] (a the one with sex 0,
   swapped so; the players' characters, kind 6, count the mission event 0x28; both get the field 0x1c8 set): the scene
   "KIRCHE_HOCHZEIT.ed3", the track "Hochzeit", the sample bank "ADDON\\HOCHZEIT_NEU.sbf", the scenes hochzeit-create (the
   names "a b 1") / -intro and the speeches -reden1 .. -reden4 with their texts 0x1aa9 .. 0x1aad and voices, the pauses
   between; st holds the scene, the view, the bank and the voice for cm_EndWedding. A click ends it at once through
   cm_EndWedding (its result returned); 0 when one of the couple is not found
   @deep deep s28: climb round 1 (183 mutants, 159 codes): nothing below 12 ins. Not re-run: the s11/s12
   guard forms. Left as the s12 note says: the two guards' own epilogue copies (jne + pop edi/esi/add/ret)
   where the original je's into the main exit after the ebx region. prior: size queue s12 (no change):
   c2tail --il pre shows the mechanism of the guard copies: the two 'if (x == 0) return 0' guards lie
   before the ebx save (opba ebx after them, the name heads in ebx up to the sprintf) and return through
   their own exit label (ret; jmp L9fbc: the epilogue without ebx) while every WED_STEP / WED_WAIT return
   and the final return go to the main exit L6fa4; the guard label's short epilogue is then copied into
   both guards. The original's guards je to the main exit, so there the two exits were one label (or merged
   before the copy). c2tail's merger log shows only the sprintf call merge (no pair / move of the
   epilogues). Next: what makes VC6 give the pre-save returns the main exit label (a guard after the first
   ebx use? the save region's start?). */

int cm_RunWedding(G28REC *m)
{
    int st[4];
    char buf[0x100];
    AICHAR *a, *b, *t;
    GMHEAD *ha, *hb;
    int r;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    if (a == 0)
        return 0;
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (b == 0)
        return 0;
    if (a->kind == 6)
        ai_CountMissionEvent(a, 0x28);
    if (b->kind == 6)
        ai_CountMissionEvent(b, 0x28);
    cm_DialogFrameFlags = 0x702c6;
    d2_CutsceneActive = 1;
    cm_SendSetField(a->netId, 0x1c8, 4, 0, 0x40000);
    cm_SendSetField(b->netId, 0x1c8, 4, 0, 0x40000);
    ha = (GMHEAD *)gm_CharNameRecord(a->id);
    hb = (GMHEAD *)gm_CharNameRecord(b->id);
    if (a->sex == 0)
        sprintf(buf, "%s %s %i", ha->name, hb->name, 1);
    else {
        t = b;
        b = a;
        a = t;
        sprintf(buf, "%s %s %i", hb->name, ha->name, 1);
    }
    st[2] = cm_LoadVoiceBank("ADDON\\HOCHZEIT_NEU.sbf", 0);
    cm_LoadEventScene("KIRCHE_HOCHZEIT.ed3");
    cm_PlaySpecialTrack("cd2\\Hochzeit.mp3");
    st[1] = d2_OpenForm(0, 0, "cutscenes\\hochzeit");
    d2_PlaceMainWindow(st[1], 3);
    d2_SetViewEnabled(st[1], 0);
    st[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-create.esc", buf);
    st[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-intro.esc", &ch_EmptyName);
    st[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-reden1.esc", &ch_EmptyName);
    WED_STEP(0x5dc);
    d2_SetViewEnabled(st[1], 1);
    d2_SetForm(st[1], 0);
    io_PrintText(0x1aa9, a->id, b->id);
    WED_VOICE("_HOCHZEIT_NEU_MAIN_00");
    WED_STEP(0x3e8);
    d2_SetForm(st[1], 0);
    io_PrintText("$C");
    d2_SetViewEnabled(st[1], 0);
    WED_WAIT();
    WED_STEP(0x3e8);

    st[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-reden2.esc", &ch_EmptyName);
    d2_SetForm(st[1], 0);
    d2_SetViewEnabled(st[1], 1);
    io_PrintText(0x1aac, a->id, b->id);
    WED_VOICE("_HOCHZEIT_NEU_MAIN_03");
    WED_STEP(0x3e8);
    d2_SetForm(st[1], 0);
    io_PrintText("$C");
    d2_SetViewEnabled(st[1], 0);
    WED_STEP(0x1f4);
    d2_SetViewEnabled(st[1], 1);
    d2_SetForm(st[1], 0);
    io_PrintText(0x1aab);
    WED_VOICE("_HOCHZEIT_NEU_M");
    WED_STEP(0x3e8);
    WED_WAIT();

    st[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-reden3.esc", &ch_EmptyName);
    d2_SetViewEnabled(st[1], 0);
    d2_SetForm(st[1], 0);
    io_PrintText("$C");
    WED_STEP(0x5dc);
    d2_SetViewEnabled(st[1], 1);
    d2_SetForm(st[1], 0);
    io_PrintText(0x1aaa, b->id, a->id);
    WED_VOICE("_HOCHZEIT_NEU_MAIN_01");
    WED_STEP(0x1f4);
    d2_SetViewEnabled(st[1], 0);
    d2_SetForm(st[1], 0);
    io_PrintText("$C");
    WED_STEP(0x1f4);
    d2_SetViewEnabled(st[1], 1);
    d2_SetForm(st[1], 0);
    io_PrintText(0x1aab);
    WED_VOICE("_HOCHZEIT_NEU_W");
    WED_STEP(0x1f4);
    WED_WAIT();

    st[0] = cm_StartEventArg("cutscenes\\hochzeit\\hochzeit-reden4.esc", &ch_EmptyName);
    d2_SetForm(st[1], 0);
    io_PrintText(0x1aad);
    WED_VOICE("_HOCHZEIT_NEU_MAIN_04");
    WED_STEP(0x1f4);
    d2_SetViewEnabled(st[1], 0);
    d2_SetForm(st[1], 0);
    io_PrintText("$C");
    WED_WAIT();
    st[0] = -1;
    return cm_EndWedding(st);
}
/* 0x4e0e90, the check function of the birth (scene kind 7, run function 0x4e0f70) for its party m: the mother
   m->members[0] and the father m->members[1]; when the mother's partner (her relatives[0]) exists and is not dead
   (kind 0xf) she gets the note text 0x11c1 with txt_BirthNoteTail (cm_SendPartyNote) and 1 is returned (the birth runs);
   else the father is killed (cm_SendKillCharacter) and 0 */

int cm_CheckBirth(G28REC *m)
{
    char buf[0x200];
    AICHAR *a, *b, *p;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (a != 0 && b != 0) {
        p = (AICHAR *)gm_FindChar(a->relatives[0]);
        if (p != 0 && p->kind != 0xf) {
            if (a->kind == 6 || a->kind == 7) {
                txt_Format(buf, 0x11c1);
                strcat(buf, txt_BirthNoteTail);
                cm_SendPartyNote(a, m, buf, &ch_EmptyName);
            }
            return 1;
        }
        cm_SendKillCharacter(b->netId, 1);
    }
    return 0;
}
/* 0x4e0f70, a birth (the scene callback of the birth event, kind 7) of the child m->members[1] to the mother m->members[0]:
   the child's kind (+2) set to 8 and sent; the sample bank "GEBURT.sbf", the track "Geburt", the scene "Geburt.ed3" with
   "cutscenes\\geburt\\enter_Geburt.esc"; unless a click ends a pause: the voice "_GEBURT" by the sexes of mother and child
   (random 0 / 1, + 2 for a girl, + 4 when members[0] has sex 1), 2 .. 5 screams "_GEBURT_GESCHREI" with random delays; then the
   view "cutscenes\\geburt" with the text 0x1aae (a boy) / 0x1aaf and the name box ("$L%t" with the child's first name,
   15 characters) until Enter or the box losing the focus gives a non-empty name, the frames end or the tutorial runs; the
   name box's text is sent as the child's first name (16 bytes at +0x30). Nothing when one of them is not found */

void cm_RunBirth(G28REC *m)
{
    char name[0x10];
    AICHAR *a, *b;
    int bank, sc, view, box, k;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (a == 0 || b == 0)
        return;
    {
        int kind;

        kind = 8;
        cm_BeginFieldBlock((char *)b, b->netId);
        cm_AddFieldDelta(1, 1, 2, &kind);
        cm_SendFieldDeltas();
    }
    cm_DialogFrameFlags = 0x702c6;
    d2_CutsceneActive = 1;
    bank = cm_LoadVoiceBank("GEBURT.sbf", 0);
    cm_PlaySpecialTrack("cd2\\Geburt.mp3");
    cm_LoadEventScene("Geburt.ed3");
    sc = cm_StartScript("cutscenes\\geburt\\enter_Geburt.esc");
    if (cm_RunSceneTicksOrClick(0x1964, 0, 0) == 0) {
        if (a->sex == 0) {
            if (b->sex == 0)
                cm_QueueVoice(-8, bank, "_GEBURT", cm_Random(2), 0);
            else
                cm_QueueVoice(-8, bank, "_GEBURT", cm_Random(2) + 2, 0);
        } else {
            if (b->sex == 0)
                cm_QueueVoice(-8, bank, "_GEBURT", cm_Random(2) + 4, 0);
            else
                cm_QueueVoice(-8, bank, "_GEBURT", cm_Random(2) + 6, 0);
        }
        if (cm_RunSceneTicksOrClick(0x3e8, 0, 0) == 0) {
            for (k = cm_Random(4) + 2; k > 0; k--)
                cm_QueueVoice(-8, bank, "_GEBURT_GESCHREI", cm_Random(5), util_Random(0x800) + 0x3e8);
            cm_RunSceneTicksOrClick(0x1b58, 0, 0);
        }
    }
    view = d2_OpenForm(0, 0, "cutscenes\\geburt");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    if (b->sex == 0)
        io_PrintText(0x1aae);
    else
        io_PrintText(0x1aaf);
    d2_SetForm(view, 2);
    box = d2_GetWinObj(view, 2, io_PrintText("$L%t", b->first));
    d2_SetObjMax(box, 0xf);
    d2_FocusInput(box);
    do {
        if (kb_Last == 0x1c || d2_FocusedInput == 0) {
            strcpy(name, (char *)d2_GetObjValue(box));
            if (name[0] != 0)
                break;
            d2_FocusInput(box);
        }
    } while (sv_RunFrame(0, cm_DialogFrameFlags) != 0 && cm_TutorialActive == 0);
    cm_BeginFieldBlock((char *)b, b->netId);
    cm_AddFieldDelta(1, 0x10, 0x30, name);
    cm_SendFieldDeltas();
    d2_CloseView(view);
    cm_FadeToBlack();
    cm_StopScript(sc);
    cm_WaitEvent(sc);
    snd_ClearQueue();
    cm_UnloadVoiceBank(bank);
    cm_FightStart = 0;
    cm_LeaveEventScene();
    cm_EndSpecialTrack();
    d2_CutsceneActive = 0;
}
/* 0x4e1220, the check of the death scene (kind 8): 1 when the health (gm_CharacterHealth) of the dying m->members[0] is
   not above 0 */

int cm_DeathSceneCheck(G28REC *m)
{
    if (gm_CharacterHealth(gm_FindChar(m->members[0])) > 0.0f)
        return 0;
    return 1;
}
/* 0x4e1250, a death (the scene callback of the death event, kind 8) of m->members[0]: the light set by the hour (the
   first of the hours 8 / 10 / 12 / 15 / 18 / 20 above ai_Time.hour, 6 after 20), the track "AmKuehlenGrabe", the sample
   bank "tod.sbf", the scene "Tod.ed3" with the sky "Sky_Schoen_02" and "cutscenes\\tod\\enter_TOD.esc"; unless a click ends
   a pause: the voices "_TOD_HS" 0, 2 .. 6 queued with their delays, then the fog faded in over 0x339 ticks; the bells
   "GLOCKEN_r". Then: with an adult relative (gm_HasAdultRelative) the player whose person (flags +0x1cc: 1 without 2) may
   play on is asked (text 0x40fb): yes revives the person (f01c 100, v024 0, the flag 2), no (or another person) opens the
   family tree with txt_DeathTreeCaption; without one such a person is told (0x40fc) and revived all the same; else in a
   game of the flag 8 the game is over (text 0x1ab4 with relatives, 0x1ab3 without, back to main_GameLoop); else the
   estate is forfeited (gm_ForfeitEstate) and its text shown */

void cm_RunDeath(G28REC *m)
{
    char buf[0x400];
    char estate[0x1fb8];
    AICHAR *a = (AICHAR *)gm_FindChar(m->members[0]);
    int hours[6] = {8, 10, 12, 15, 18, 20};
    int stage, i, sc, bank, rc, kin, revive;
    unsigned int t0;

    stage = 6;
    cm_DialogFrameFlags = 0x702c6;
    d2_CutsceneActive = 1;
    for (i = 0; i < 6; i++) {
        if (ai_Time.hour < hours[i]) {
            stage = i;
            break;
        }
    }
    cm_PlaySpecialTrack("cd2\\AmKuehlenGrabe.mp3");
    bank = snd_LoadVoiceBank("tod.sbf", 1);
    cm_LoadEventScene("Tod.ed3");
    cm_CreateSceneSky("Sky_Schoen_02");
    d3_SetLightSet(stage, 0, 1.0f, 1);
    d3_SetFogSet(0, 0, 0);
    sc = cm_StartEventArg("cutscenes\\tod\\enter_TOD.esc", &ch_EmptyName);
    rc = cm_RunSceneTicksOrClick(0xbb8, 0, 0);
    if (rc == 0) {
        cm_QueueVoice(-8, bank, "_TOD_HS", 0, 0);
        cm_QueueVoice(-8, bank, "_TOD_HS", 2, 0xfa0);
        cm_QueueVoice(-8, bank, "_TOD_HS", 3, 0xbb8);
        cm_QueueVoice(-8, bank, "_TOD_HS", 4, 0x7d0);
        cm_QueueVoice(-8, bank, "_TOD_HS", 5, 0xfa0);
        cm_QueueVoice(-8, bank, "_TOD_HS", 6, 0x1f4);
        rc = cm_RunSceneTicksOrClick(0x2af8, 0, 0);
    }
    if (rc == 0) {
        t0 = d3_Time;
        while (d3_Time < t0 + 0x339 && rc == 0) {
            sv_RunFrame(0, cm_DialogFrameFlags);
            if (dm_L.rup != 0)
                rc = 1;
            d3_SetFogSet(0, 1, (float)(d3_Time - t0) * (1.0f / 0x339) < 1.0f ? (float)(d3_Time - t0) * (1.0f / 0x339) : 1.0f);
        }
        d3_SetFogSet(0, 1, 1.0f);
        cm_RunSceneTicksOrClick(0x2cec, 0, 0);
    }
    sb_PlaySample(0, "GLOCKEN_r", 1, 0x3f, 0x7f, 0, 0);
    d2_CutsceneActive = 0;
    if (gm_HasAdultRelative((char *)a, &kin) != 0) {
        revive = 0;
        if ((a->flags1cc & 1) != 0 && (a->flags1cc & 2) == 0) {
            txt_Format(buf, 0x40fb, 0x40f5, a->id);
            revive = cm_MessageBox(buf, 0x10);
        }
        if (revive == 0)
            ai_FamilyTreeWindow(ai_CharInts[ai_PlayerId][0], 0xa, txt_DeathTreeCaption, cm_DialogFrameFlags);
        else {
            float life;
            int zero;

            zero = 0;
            life = 100.0f;
            cm_BeginFieldBlock((char *)a, a->netId);
            cm_AddFieldDelta(4, 1, 0x1c, &life);
            cm_AddFieldDelta(4, 1, 0x24, &zero);
            cm_SendFieldDeltas();
            cm_SendSetField(a->netId, 0x1cc, 4, 2, 0);
        }
    } else if ((a->flags1cc & 1) != 0 && (a->flags1cc & 2) == 0) {
        int zero;
        float life;

        zero = 0;
        life = 100.0f;
        txt_Format(buf, 0x40fc, 0x40f5, a->id);
        cm_MessageBox(buf, 0x10);
        cm_BeginFieldBlock((char *)a, a->netId);
        cm_AddFieldDelta(4, 1, 0x1c, &life);
        cm_AddFieldDelta(4, 1, 0x24, &zero);
        cm_SendFieldDeltas();
        cm_SendSetField(a->netId, 0x1cc, 4, 2, 0);
    } else if ((ai_GameFlags & 8) != 0) {
        a->health = 0;
        ai_GameOver = 1;
        if (kin != 0)
            txt_Format(buf, 0x1ab4);
        else
            txt_Format(buf, 0x1ab3, a->id);
        cm_MessageBox(buf, 0);
        sv_PopTo((unsigned)&main_GameLoop);
    } else {
        gm_ForfeitEstate((struct ORDCHAR *)a, estate, 1);
        cm_MessageBox(estate, 0x10);
    }
    cm_FadeToBlack();
    cm_StopScript(sc);
    cm_WaitEvent(sc);
    cm_DestroySceneSky();
    cm_LeaveEventScene();
    snd_ClearQueue();
    sb_UnloadSampleBank(bank);
    cm_EndSpecialTrack();
    cm_FightStart = 0;
}
/* 0x4e1700, a bankruptcy (the scene callback of the bankruptcy event, kind 9) of m->members[0]: the light set by the hour
   (as cm_RunDeath), the sample bank "pleite.sbf", the track "DerSeuchenzug", the scene "Pleite.ed3" with the sky
   "sky_dunkel_01" and "cutscenes\\pleite\\enter_Pleite.esc" (given the light set as text); unless a click ends a pause:
   the voices "_PLEITE_HS" 0 .. 2, the rain (250), the voices 3 .. 5. Then the text 0x1ab1 (0x3fda in a game of the flag 4)
   with the person's name as a message; in a game of the flag 8 the game is over (back to main_GameLoop), else the estate is
   forfeited (gm_ForfeitEstate, its text not shown) */

void cm_RunBankruptcy(G28REC *m)
{
    char stagetext[0x100];
    char buf[0x200];
    char estate[0x1fb8];
    AICHAR *a = (AICHAR *)gm_FindChar(m->members[0]);
    int hours[6] = {8, 10, 12, 15, 18, 20};
    int i, stage, sc, bank;

    stage = 6;
    cm_DialogFrameFlags = 0x702c6;
    d2_CutsceneActive = 1;
    for (i = 0; i < 6; i++) {
        if (ai_Time.hour < hours[i]) {
            stage = i;
            break;
        }
    }
    bank = snd_LoadVoiceBank("pleite.sbf", 1);
    cm_PlaySpecialTrack("cd2\\DerSeuchenzug.mp3");
    cm_LoadEventScene("Pleite.ed3");
    d3_SetLightSet(stage, 0, 1.0f, 1);
    d3_SetFogSet(0, 0, 0);
    cm_CreateSceneSky("sky_dunkel_01");
    sprintf(stagetext, "%i", stage);
    sc = cm_StartEventArg("cutscenes\\pleite\\enter_Pleite.esc", stagetext);
    if (cm_RunSceneTicksOrClick(0x1770, 0, 0) == 0) {
        cm_QueueVoice(-8, bank, "_PLEITE_HS", 0, 0);
        cm_QueueVoice(-8, bank, "_PLEITE_HS", 1, 0x9c4);
        cm_QueueVoice(-8, bank, "_PLEITE_HS", 2, 0x1f4);
        if (cm_RunSceneTicksOrClick(0x3e80, 0, 0) == 0) {
            cm_StartSceneRain(0xfa);
            if (cm_RunSceneTicksOrClick(0x2af8, 0, 0) == 0) {
                cm_QueueVoice(-8, bank, "_PLEITE_HS", 3, 0x251c);
                cm_QueueVoice(-8, bank, "_PLEITE_HS", 4, 0x1194);
                cm_QueueVoice(-8, bank, "_PLEITE_HS", 5, 0x1f4);
                cm_RunSceneTicksOrClick(0x5dc0, 0, 0);
            }
        }
    }
    d2_CutsceneActive = 0;
    if ((ai_GameFlags & 4) != 0)
        txt_Format(buf, 0x3fda, a->id);
    else
        txt_Format(buf, 0x1ab1, a->id);
    cm_MessageBox(buf, 0);
    cm_FadeToBlack();
    cm_StopScript(sc);
    cm_WaitEvent(sc);
    cm_DestroySceneSky();
    cm_StopSceneRain();
    cm_LeaveEventScene();
    snd_ClearQueue();
    sb_UnloadSampleBank(bank);
    cm_EndSpecialTrack();
    cm_FightStart = 0;
    if ((ai_GameFlags & 8) != 0) {
        a->health = 0;
        ai_GameOver = 1;
        sv_PopTo((unsigned)&main_GameLoop);
        return;
    }
    gm_ForfeitEstate((struct ORDCHAR *)a, estate, 0);
}
/* 0x4e19a0, the check function of the auction (scene kind 10, run function 0x4e1a80) for its party m: the auctioned
   building m->auction.building and its start price m->auction.price go into the text 0x3fd9, joined with
   txt_AuctionNoteNet in the net run (ai_GameFlags & 4) or txt_AuctionNoteLocal; every member of the kinds 6 / 7 gets
   it as a note (cm_SendPartyNote). 0 when the building is gone (the auction is cancelled), else 1 */

int cm_CheckAuction(G28REC *m)
{
    char buf[0x200];
    AICHAR *p;
    AIBLD *o;
    int i;

    o = (AIBLD *)gm_FindBuildingById(m->auction.building);
    if (o == 0)
        return 0;
    txt_Format(buf, 0x3fd9, o, m->auction.price);
    if ((ai_GameFlags & 4) != 0)
        strcat(buf, txt_AuctionNoteNet);
    else
        strcat(buf, txt_AuctionNoteLocal);
    for (i = 0; i < m->used; i++) {
        p = (AICHAR *)gm_FindChar(m->members[i]);
        if (p != 0 && (p->kind == 6 || p->kind == 7))
            cm_SendPartyNote(p, m, buf, &ch_EmptyName);
    }
    return 1;
}
/* 0x4e1a80, an auction (the scene callback of the auction event, kind 10) of the building m->auction.building among the
   bidders m->members[0 .. used - 1] from the price m->auction.price: only for the building kinds 0xb / 0xd / 0xc (forest
   "WALDSTUECK", mine "MINE", quarry "STEINBRUCH"; else nothing, the track is not ended). The voice banks
   VERSTEIGERUNG_INTRO / _GEBOTE / _KOMMENTARE, the scene "Versteigerung.ed3" (the easel "ub_STAFELEI" textured by the
   kind), one of three auctioneers at random (model, voice "M1" / "M2" / "W1", speaker -5 / -4 / -2) on "dummy_AUKTIONATOR",
   every bidder found on his place "dummy_RP_%02i" (his head record's model). Text 0x1aa2, then up to 5 rounds: the bidders
   answer through cm_RunSceneParty (CMAUCTION; the bids in cm_CutInfos[i].session), fewer than 2 bids end it, else the
   highest bid leads and the price rises by 0x20 (the auctioneer's voice by how many bid, text 0x1aa3). The leader wins
   (text 0x1aa4); the host's game sends the building's price (field 0x69) and year (0x6d), the ownership and the payment
   (9/10 to the object 0x115 of the first building, 1/10 to the holder of office kind 0xb when there is one); with no
   leader text 0x1aa5 and the building stays with the host */

void cm_RunAuction(G28REC *m)
{
    char voice[3];
    char kindName[0x40];
    char model[0x40];
    char buf[0x200];
    AIBLD *b, *first;
    AIBTYPE *t;
    AICHAR *chars[8];
    AICHAR *w, *holder;
    CHARACTER *persons[8];
    CHARACTER *auct;
    D3OBJECT *o;
    AIOBJ *q;
    GMHEAD *rec;
    CMAUCTION req;
    float pos[4];
    int i, k, speaker, bankIntro, bankBids, bankRemarks, behavior, enter, form;
    int best, rounds, round, done;

    b = (AIBLD *)gm_BuildQuery(1, 1, m->auction.building);
    best = -1;
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    pos[3] = 0;
    speaker = cm_Random(3);  /* which auctioneer, then his speaker id */
    cm_PlaySpecialTrack(0);
    if (b == 0)
        return;
    t = &ai_BTypes[b->type];
    if (t->kind == 0xb)
        sprintf(kindName, "%s", "WALDSTUECK");
    else if (t->kind == 0xd)
        sprintf(kindName, "%s", "MINE");
    else if (t->kind == 0xc)
        sprintf(kindName, "%s", "STEINBRUCH");
    else
        return;
    req.price = m->auction.price;
    req.building = m->auction.building;
    req.count = m->used;
    for (i = 0; i < m->used; i++) {
        req.bids[i] = 0;
        chars[i] = (AICHAR *)gm_FindChar(m->members[i]);
        req.bids[i] = chars[i] != 0 ? 0 : -1;
        req.ids[i] = m->members[i];
    }
    for (; i < 8; i++) {
        chars[i] = 0;
        req.bids[i] = 0;
        req.ids[i] = -1;
    }
    bankIntro = cm_LoadVoiceBank("VERSTEIGERUNG_INTRO.sbf", 0);
    bankBids = cm_LoadVoiceBank("VERSTEIGERUNG_GEBOTE.sbf", 0);
    bankRemarks = cm_LoadVoiceBank("VERSTEIGERUNG_KOMMENTARE.sbf", 0);
    cm_LoadEventScene("Versteigerung.ed3");
    o = d3_FindObject(0, 0x40, 0, "ub_STAFELEI");
    if (o != 0) {
        if (t->kind == 0xb)
            d3if_SelectTextureSet(o, (struct D3SURF *)o->info, 0, 1);
        else if (t->kind == 0xc)
            d3if_SelectTextureSet(o, (struct D3SURF *)o->info, 1, 1);
        else if (t->kind == 0xd)
            d3if_SelectTextureSet(o, (struct D3SURF *)o->info, 2, 1);
    }
    /* the auctioneer */
    if (speaker == 0) {
        speaker = -5;
        sprintf(model, "%s", "buerger3_DICKER");
        sprintf(voice, "%s", "M1");
    } else if (speaker == 1) {
        speaker = -4;
        sprintf(model, "%s", "wirt2_DICKER");
        sprintf(voice, "%s", "M2");
    } else {
        speaker = -2;
        sprintf(model, "%s", "buergerin2_FRAU");
        sprintf(voice, "%s", "W1");
    }
    o = d3_FindObject(0, 0x100, 0, "dummy_AUKTIONATOR");
    if (o != 0)
        d3_PointToWorld(o, &o->pos, (D3VECTOR *)pos);
    auct = cm_CreateChar(model, (D3VECTOR *)pos);
    if (auct != 0) {
        ch_PlaceAtObject(auct, o);
        strcpy(auct->name, "Auktionator");
        strcpy(auct->obj->name, "Auktionator");
    }
    /* the bidders on their places */
    for (i = 0; i < m->used; i++) {
        sprintf(buf, "dummy_RP_%02i", i);
        o = d3_FindObject(0, 0x100, 0, buf);
        if (o != 0)
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)pos);
        w = chars[i];
        if (w != 0 && (rec = (GMHEAD *)gm_CharNameRecord(w->id)) != 0)
            sprintf(buf, "%s", rec->name);
        else {
            k = cm_Random(3);
            if (k == 1)
                strcpy(buf, "buerger_MANN");
            else if (k == 2)
                strcpy(buf, "buerger2_MANN");
            else
                strcpy(buf, "handwerker3_MANN");
        }
        if (w != 0) {
            persons[i] = cm_CreateChar(buf, (D3VECTOR *)pos);
            ch_PlaceAtObject(persons[i], o);
        } else
            persons[i] = 0;
    }
    behavior = cm_StartEventArg("cutscenes\\auktion\\behavior_auktionator.esc", &ch_EmptyName);
    enter = cm_StartEventArg("cutscenes\\auktion\\Enterfly_auktion.esc", &ch_EmptyName);
    sprintf(buf, "_VERSTEIGERUNG_INTRO_%s_00", voice);
    cm_QueueVoice(-7, bankIntro, buf, -1, 0);
    sprintf(buf, "_VERSTEIGERUNG_INTRO_%s", kindName);
    k = cm_Random(3);
    cm_QueueVoice(speaker, bankIntro, buf, k, 0x1f4);
    form = d2_OpenForm(0, 0, "cutscenes\\pacht_main");
    if (cm_SkipEvents != 0)
        d2_SetViewEnabled(form, 0);
    d2_PlaceMainWindow(form, 3);
    d2_SetForm(form, 0);
    io_PrintText(0x1aa2, b, req.price);
    cm_WaitEvent(enter);
    /* the rounds */
    round = 0;
    done = 0;
    for (; round < 5 && done == 0; round++) {
        int nbid, top;

        rounds = 10 - round;
        top = 0;
        nbid = 0;
        req.countdown = rounds;
        cm_RunSceneParty(m, &req);
        if (rounds > 6 && cm_Random(3) != 0)
            cm_QueueVoice(cm_Random(4) != 0 ? -4 : -2, bankRemarks, "_VERSTEIGERUNG_KOMMENTARE", cm_Random(6), cm_Random(0x200) + 0x32);
        for (i = 0; i < m->used; i++) {
            if (cm_CutInfos[i].session.click != 0 && chars[i] != 0) {
                req.bids[i] = cm_CutInfos[i].session.answer;
                if (top < req.bids[i]) {
                    top = req.bids[i];
                    best = i;
                    req.price = req.bids[best];
                }
                nbid++;
            } else
                req.bids[i] = -1;
        }
        if (nbid < 2)
            done = 1;
        else if (rounds > 6) {
            if (nbid < m->used / 2)
                cm_QueueVoice(speaker, bankBids, "_VERSTEIGERUNG_GEBOTE", cm_Random(4) + 2, 0xfa);
            else
                cm_QueueVoice(speaker, bankBids, "_VERSTEIGERUNG_GEBOTE", cm_Random(2), 0xfa);
            req.price += 0x20;
            d2_SetForm(form, 0);
            io_PrintText(0x1aa3, b, round + 2, 5, nbid, req.bids[best]);
        }
        cm_WaitTicks(5);
    }
    if (best > -1) {
        int price, year;

        price = req.bids[best];
        year = ai_Time.year;
        cm_QueueVoice(speaker, bankBids, "_VERSTEIGERUNG_GEBOTE", 6, 200);
        d2_SetForm(form, 0);
        w = chars[best];
        io_PrintText(0x1aa4, w->id, req.price);
        if (m->host == G20_ME) {
            holder = 0;
            for (i = 0; i < 768; i++) {
                if (ai_Chars[i].officeKind == 0xb) {
                    holder = &ai_Chars[i];
                    break;
                }
            }
            cm_BeginFieldBlock((char *)b, b->id);
            cm_AddFieldDelta(4, 1, 0x69, &price);
            cm_AddFieldDelta(4, 1, 0x6d, &year);
            cm_SendFieldDeltas();
            cm_SendOwnership(b->id, ai_PlayerChar->netId, w->netId);
            first = (AIBLD *)gm_FirstBuilding(1);
            q = (AIOBJ *)gm_Query((void *)first->objList, 2, 6, 0, 0x115);
            if (w == holder)
                cm_SendMoveGoodsUnchecked(q->objId, w->netId, 0, (int)(req.bids[best] * 0.9f));
            else if (holder != 0) {
                cm_SendMoveGoodsUnchecked(q->objId, w->netId, 0, (int)(req.bids[best] * 0.9f));
                cm_SendMoveGoodsUnchecked(holder->netId, w->netId, 0, (int)(req.bids[best] * 0.1f));
            } else
                cm_SendMoveGoodsUnchecked(q->objId, w->netId, 0, req.bids[best]);
        }
    } else {
        cm_QueueVoice(speaker, bankBids, "_VERSTEIGERUNG_GEBOTE", 7, 200);
        d2_SetForm(form, 0);
        io_PrintText(0x1aa5, b);
        if (m->host == G20_ME)
            cm_SendOwnership(b->id, ai_PlayerChar->netId, ai_PlayerChar->netId);
    }
    cm_QueueVoice(speaker, bankBids, "_VERSTEIGERUNG_GEBOTE", 8, 0x1f4);
    cm_WaitEvent(cm_StartEventArg("cutscenes\\auktion\\Auktion_Outro.esc", &ch_EmptyName));
    d2_CloseView(form);
    cm_FadeToBlack();
    cm_StopScript(behavior);
    cm_WaitEvent(behavior);
    for (i = 0; i < m->used; i++)
        if (persons[i] != 0)
            cm_DeleteChar(persons[i]);
    if (auct != 0)
        cm_DeleteChar(auct);
    cm_LeaveEventScene();
    snd_ClearQueue();
    cm_UnloadVoiceBank(bankIntro);
    cm_UnloadVoiceBank(bankBids);
    cm_UnloadVoiceBank(bankRemarks);
    cm_EndSpecialTrack();
}
/* 0x4e24c0, the auction's member callback for the player's character (cm_RunSceneParty, scene kind 10): r is the member's
   record, m the auction (cm_RunAuction). Nothing unless r->playerId is one of the bidders still in (m->bids[i] != -1) and his
   goods are worth the price; else r->session.answer = the price and the dialog "cutscenes\\pacht" (texts 0x1aa6 / 0x1aa7)
   with a value box between the price and the limit ai_AmountLimit gives (the price raised by 0x20 * 10000, at most the worth;
   both in the player's money, gm_ConvertMoney), before the timer slider runs out (cm_DialogTimer, 1500 ms): button 0 bids
   (r->session.click = 1, the answer the box's value back in the game's money), button 1 passes */

void cm_AuctionAskPlayer(CMCUTINFO *r, CMAUCTION *m)
{
    AICHAR *c;
    int i, price, worth, lim, lo, hi, view, t, box, dlg;

    r->session.click = 0;
    for (i = 0; i < m->count && m->ids[i] != r->playerId; i++)
        ;
    if (i >= m->count || m->bids[i] == -1)
        return;
    r->session.answer = m->price;
    gm_BuildQuery(1, 1, m->building);
    price = m->price;
    c = (AICHAR *)gm_FindChar(r->playerId);
    if (c == 0)
        return;
    worth = gm_GetGoodsValue((char *)c);
    ai_AmountLimit(price, worth, 0x20, &lim);
    lo = gm_ConvertMoney(price, ai_OwnPlayer);
    hi = gm_ConvertMoney(lim, ai_OwnPlayer);
    if (worth < price)
        return;
    view = d2_OpenForm(0, 0, "cutscenes\\pacht");
    d2_PlaceMainWindow(view, 3);
    d2_ClickedId = -1;
    d2_SetForm(view, 0);
    t = io_PrintText(0x1aa6, ai_OwnPlayer);
    io_PrintText(0x1aa7);
    box = d2_GetWinObj(view, 0, t);
    d2_SetObjValue(box, lo, hi, lo);
    dlg = CUT_BOX();
    cm_DialogTimer(dlg, 0x5dc, 1);
    d2_ClickedId = -1;
    while (sv_RunFrame(cm_AuctionAskPlayer, cm_DialogFrameFlags) != 0) {
        if (cm_DialogTimer(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            sv_PopRequest = 1;
            if (d2_ClickedId == 1)
                ;                       /* passes */
            else if (d2_ClickedId == 0) {
                r->session.click = 1;
                r->session.answer = gm_PlayerFactor(d2_GetObjValue(box), ai_OwnPlayer);
            }
        }
    }
    d2_CloseView(view);
}
/* 0x4e26d0, the auction's member callback for a computer bidder (cm_RunSceneParty, scene kind 10): r is the member's record,
   m the auction (cm_RunAuction). Nothing unless r->playerId is one of the bidders still in (m->bids[i] != -1); else
   r->session.answer = the price, and the bidder's limit is m->countdown * 32000 + the price, or his worth
   (gm_GetGoodsValue) times a random factor 0.3 .. 0.47 when that is less. A limit below the price, or price / limit not
   below a random 0 .. 1, gives no bid; else r->session.click = 1 and the bid is the price plus a random part of the margin
   (at most the price again) scaled by m->countdown / 10, plus its remainder modulo 32 */

void cm_AuctionAskAI(CMCUTINFO *r, CMAUCTION *m)
{
    AICHAR *c;
    float lim;
    int i, price, v, x;

    r->session.click = 0;
    for (i = 0; i < m->count && m->ids[i] != r->playerId; i++)
        ;
    if (i >= m->count || m->bids[i] == -1)
        return;
    r->session.answer = m->price;
    gm_BuildQuery(1, 1, m->building);
    price = m->price;
    c = (AICHAR *)gm_FindChar(r->playerId);
    if (c == 0)
        return;
    lim = (float)(m->countdown * 32000 + price);
    v = (int)(gm_GetGoodsValue((char *)c) * ((float)cm_RandomFloat() * 0.16666667f + 0.3f) < lim
              ? gm_GetGoodsValue((char *)c) * ((float)cm_RandomFloat() * 0.16666667f + 0.3f) : lim);
    if (v < price)
        return;
    lim = (float)price;
    if ((float)cm_RandomFloat() > lim / v) {
        if (v > price * 2)
            v = price * 2;
        v -= price;
        r->session.click = 1;
        x = (int)(cm_Random(v) * (m->countdown * 0.1f) + lim);
        r->session.answer = x + x % 32;
    }
}
/* 0x4e2860, the run function of scene kind 11, the sale of the object m->thing (of the seller m->members[0] to the
   buyers m->members[1 .. 4]): the scene of the object (gm_FindById, cm_ShowObjectScene with the place id 0x17), or
   "ob_SALON.ed3" when it has none; a pause of 5 ticks, then every buyer and the seller raise their relation to each
   other by 10 (cm_SendRelations, both ways) */

void cm_RunSale(G28REC *m)
{
    int o, f, i;
    int *p;

    f = 0;
    cm_PlaySpecialTrack(0);
    gm_FindById(&o, 0, 0, m->thing);
    if (o != 0)
        cm_ShowObjectScene((char *)o, 0x17);
    else {
        f = 1;
        cm_LoadEventScene("ob_SALON.ed3");
    }
    cm_WaitTicks(5);
    p = &m->members[1];
    for (i = 0; i < 4; i++, p++) {
        if (*p != -1) {
            cm_SendRelations(m->members[0], *p, 10, 1);
            cm_SendRelations(*p, m->members[0], 10, 1);
        }
    }
    if (f != 0) {
        cm_FadeToBlack();
        cm_LeaveEventScene();
    }
    cm_EndSpecialTrack();
}

#define G27_FRAME(a, b) d2_BlitPart(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, (a), 0, 0, (b))
/* 0x4e2910, shows the scene of the object o (a building record) for the place id: when o is the current building
   (ai_PlayerBuilding / ai_FallbackBuilding) and its main room is that id, only the frozen screen is drawn (the party's
   window flag 0x20) and the screen fades to black; else the place of the id in o's object list (gm_Query) gets its
   scene selected - of the object (init_ObjectUniverse, loaded first when it is not loaded or still dirty) or, for a
   home, of the building kind (init_BuildingUniverse) - and shown (init_ShowBuildingScene); the scene flags are
   inflated without the stop bit 2. Nothing in the replay mode (cm_SkipEvents) */
void cm_ShowObjectScene(char *o, int id)
{
    AIBLD *b;
    int k;

    if (cm_SkipEvents != 0)
        return;
    b = (AIBLD *)o;
    if ((ai_PlayerBuilding == b && b->mainRoom == id) || (ai_FallbackBuilding == b && b->mainRoom == id)) {
        if (((G28REC *)cm_CurrentFight)->flags & 0x20) {
            d2_ClearSurface(d2_ScreenSurf);
            G27_FRAME(d2_ScreenSurf, d2_BackSurf);
            sv_RunFrame(0, 0x86);
            G27_FRAME(d2_BackSurf, d2_ScreenSurf);
        }
        if (cm_SceneFade != 0) {
            d2_ReleaseFade(cm_SceneFade);
            cm_SceneFade = 0;
            d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
        }
        return;
    }
    if (gm_Query(b->objList, 1, 0, id) != 0) {
        if (gm_IsHomeType(o) == 0) {
            if (init_ObjectUniverse[id] != (char)0xfe) {
                if (init_ObjectUniverse[id] == (char)0xff || init_SceneDirty[init_ObjectUniverse[id] * 0x3e0] != 0)
                    init_LoadObjectScene(id);
                d3if_SelectScene(init_ObjectUniverse[id], 0);
                init_ShowBuildingScene(o, id);
            }
        } else {
            k = (unsigned char)ai_BTypes[b->type].kind;
            if (init_BuildingUniverse[k] != 0) {
                if (init_BuildingUniverse[k] == (char)0xff || init_SceneDirty[init_BuildingUniverse[k] * 0x3e0] != 0)
                    init_LoadBuildingScene(k);
                k = (unsigned char)ai_BTypes[b->type].kind;
                d3if_SelectScene(init_BuildingUniverse[k], 0);
                init_ShowBuildingScene(o, 0xfd);
            }
        }
    }
    if (d3_SceneFlags != 0) {
        cm_SceneInflateFlags = d3_SceneFlags;
        d3_InflateScene(d3_SceneFlags & ~2);
        cm_SceneInflateFlags = cm_SceneInflateFlags & ~2;
    }
    if (((G28REC *)cm_CurrentFight)->flags & 0x20) {
        d2_ClearSurface(d2_ScreenSurf);
        G27_FRAME(d2_ScreenSurf, d2_BackSurf);
        sv_RunFrame(0, 0x86);
        G27_FRAME(d2_BackSurf, d2_ScreenSurf);
    }
    if (cm_SceneFade != 0) {
        d2_ReleaseFade(cm_SceneFade);
        cm_SceneFade = 0;
        d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
    }
}


/* the random seed of the scenes (0x92c4e0), a linear congruence; the number is the high word modulo 0x7fff */


/* 0x4e2bd0, d2_SetViewEnabled for the scene helpers: in the replay mode (cm_SkipEvents) the view is always disabled */
void cm_SetViewEnabled(int view, int on)
{
    if (cm_SkipEvents != 0) {
        d2_SetViewEnabled(view, 0);
        return;
    }
    d2_SetViewEnabled(view, on);
}
/* 0x4e2bf0, loads the script name (evt_Load), runs it (evt_RunScript) and returns its id; 0 when it does not load or in
   the replay mode (cm_SkipEvents) */

int cm_StartScript(char *name)
{
    EVTSCRIPT *p;

    if (cm_SkipEvents == 0) {
        p = evt_Load(name);
        if (p != 0) {
            evt_RunScript(p);
            return p->id;
        }
    }
    return 0;
}
/* 0x4e2c30, the same with a second argument for 0x429c40 */

int cm_StartEventArg(char *name, int arg)
{
    EVTSCRIPT *p;

    if (cm_SkipEvents == 0) {
        p = evt_Load(name);
        if (p != 0) {
            evt_RunCommand(p, arg);
            return p->id;
        }
    }
    return 0;
}
/* 0x4e2c70, runs the frame wait (0x5309a0, flag 0x8000) and 0x4284a0(a) until it says 0 */

int cm_WaitEvent(int a)
{
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(0, cm_DialogFrameFlags | 0x8000) != 0) {
            do {
                if (evt_FindById(a) == 0)
                    break;
            } while (sv_RunFrame(0, cm_DialogFrameFlags | 0x8000) != 0);
        }
    }
}
/* 0x4e2cc0, like 0x4e2c70, but ends with the callback cb(arg) and 1 as soon as 0x14673a0 is set */

int cm_WaitEventOrClick(int a, int (*cb)(int), int arg)
{
    if (cm_SkipEvents != 0)
        return 0;
    while (sv_RunFrame(0, cm_DialogFrameFlags | 0x8000) != 0) {
        if (evt_FindById(a) == 0)
            return 0;
        if (dm_L.rup != 0) {
            if (cb != 0)
                return cb(arg);
            return 1;
        }
    }
    return 0;
}
/* 0x4e2d40, 0x4e2c70 with the frame wait flags 0x79706 */

int cm_WaitEventFull(int a)
{
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(0, 0x79706) != 0) {
            do {
                if (evt_FindById(a) == 0)
                    break;
            } while (sv_RunFrame(0, 0x79706) != 0);
        }
    }
}
/* 0x4e2d90, 0x429e90 of what 0x4284a0 finds */

int cm_StopScript(int a)
{
    int r;

    if (cm_SkipEvents == 0) {
        r = (char *)evt_FindById(a);
        if (r != 0)
            return evt_Stop((EVTSCRIPT *)r);
    }
}
/* 0x4e2dc0, evt_Stop of every used script of evt_Scripts (128) that runs (flags bit 1) for the group -2 */

int cm_StopGroupScripts(void)
{
    int i;
    EVTSCRIPT *e;

    for (i = 0; i < 0x80; i++) {
        e = &evt_Scripts[i];
        if (e->id != -1 && (e->flags & 1) && e->group == -2)
            evt_Stop(e);
    }
    return 0;
}
/* 0x4e2e00, waits frame by frame (sv_RunFrame, flags 0x79706) until no used script of evt_Scripts (128) runs for the group
   -2 (the ones cm_StopGroupScripts stops), or the frames end; cm_LeaveEventScene waits so */

void cm_WaitGroupScripts(void)
{
    int i;
    EVTSCRIPT *e;

    while (sv_RunFrame(0, 0x79706) != 0) {
        e = evt_Scripts;
        i = 0;
        do {
            if (e->id != -1 && e->group == -2)
                goto busy;
            i++;
            e++;
        } while (i < 0x80);
        return;
    busy:;
    }
}
/* 0x4e2e60, loads the scene file name into a free scene ("scenes/*%s"), selects it, sets the detail flags of its +0x1c70 byte by the
   setting 0x6f35ca (bits 3 / 4), freezes the screen (0x6ed924 / 0x6ed92c) when the window flag +0x26 & 0x20 of 0x6f0d90 is set, runs the
   scene's callback (+0x3c8 -> +0x210) once, counts the scene depth 0x6f0d9c and fades from black ("BLACK", 0x5a ticks). Not in the replay mode */

void cm_LoadEventScene(char *name)
{
    char path[0x80];
    int idx, sh, k;

    if (cm_SkipEvents != 0)
        return;
    idx = d3t_FindFreeScene();
    d3if_SelectScene(idx, 0);
    sprintf(path, "scenes/*%s", name);
    d3_LoadScene(path, 0, 0);
    d3if_SelectScene(0, 1);
    d3if_SelectScene(idx, 0);
    if (d3_Terrain != 0) {
        if (ai_GfxFloorLod == 0) {
            ((D3FLMAP *)d3_Terrain)->lod = 2;
            sh = 2;
            d3fl_SetDetail(0x40);
        } else {
            ((D3FLMAP *)d3_Terrain)->lod = 1;
            sh = 1;
            d3fl_SetDetail(0x40);
        }
        d3fl_SetShift((struct D3FLMAP *)d3_Terrain, sh >> 1, 1);
    }
    if (((G28REC *)cm_CurrentFight)->flags & 0x20) {
        d2_ClearSurface(d2_ScreenSurf);
        G27_FRAME(d2_ScreenSurf, d2_BackSurf);
        sv_RunFrame(0, 0x86);
        G27_FRAME(d2_BackSurf, d2_ScreenSurf);
    }
    if (gm_ClockSeason(&ai_Time) == 3)
        sv_UpdateSkyColors();
    k = cm_SceneDepth + 1;
    evt_NoGroup = -2;
    cm_SceneDepth = k;
    if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[4].fn != 0)
        d3_DefaultScene->events[4].fn(d3_DefaultScene, 4, d3_DefaultScene->events[4].name, 0);
    cm_SceneEnterScript = evt_LastId;
    d3if_Prerender(1);
    if (cm_SceneFade != 0) {
        d2_ReleaseFade(cm_SceneFade);
        cm_SceneFade = 0;
    }
    d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
}
/* 0x4e3060, fades the screen to black ("BLACK", 0x5a ticks) and waits until the fade has reached its end flag (+0 & 4) */

int cm_FadeToBlack(void)
{
    if (cm_SkipEvents == 0) {
        cm_SceneFade = (char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
        if (!(*cm_SceneFade & 4)) {
            do {
                sv_RunFrame(0, cm_DialogFrameFlags | 0x80);
            } while (!(*cm_SceneFade & 4));
        }
    }
}
/* 0x4e30c0, leaves the event scene (unless replaying or no scene is entered, cm_SceneDepth): stops the script that was the
   last when the scene was entered (cm_SceneEnterScript), runs the scene's event 5 (D3SCENE.events), waits while the last
   started script (evt_LastId) runs, stops and waits for the scene's scripts (group -2: cm_StopGroupScripts,
   cm_WaitGroupScripts), waits while the entering script runs; then frees the scene's height map (d3sm_Destroy), the
   camera's path and its move, resets the 3D interface (d3if_Reset) and lowers cm_SceneDepth */

void cm_LeaveEventScene(void)
{
    EVTSCRIPT *e;

    if (cm_SkipEvents != 0 || cm_SceneDepth <= 0)
        return;
    if (cm_SceneEnterScript != -1) {
        e = evt_FindById(cm_SceneEnterScript);
        if (e != 0)
            evt_Stop(e);
    }
    if (d3_DefaultScene != 0 && d3_DefaultScene->events != 0 && d3_DefaultScene->events[5].fn != 0)
        d3_DefaultScene->events[5].fn(d3_DefaultScene, 5, d3_DefaultScene->events[5].name, 0);
    if (evt_LastId != -1) {
        e = evt_FindById(evt_LastId);
        while (e != 0 && (e->flags & 1)) {
            sv_RunFrame(0, cm_DialogFrameFlags | 0x80);
            e = evt_FindById(evt_LastId);
        }
    }
    cm_StopGroupScripts();
    cm_WaitGroupScripts();
    if (cm_SceneEnterScript != -1) {
        e = evt_FindById(cm_SceneEnterScript);
        while (e != 0 && (e->flags & 1)) {
            sv_RunFrame(0, cm_DialogFrameFlags | 0x80);
            e = evt_FindById(cm_SceneEnterScript);
        }
    }
    if (d3_DefaultScene->hmap != 0) {
        d3sm_Destroy(d3_DefaultScene->hmap);
        d3_DefaultScene->hmap = 0;
    }
    d3anim_FreePath(d3_Camera);
    d3t_CamMove = 0;
    d3t_CamMoving = 0;
    d3if_Reset();
    cm_SceneDepth--;
}
/* 0x4e3210, the scene's tick counter 0x6f0d80: 0x6f0d88 tells whether it was past 0x6f0d84 */

int cm_SceneTick(void)
{
    cm_SceneTimedOut = cm_SceneTicks > cm_SceneDeadline;
    cm_SceneTicks++;
}
/* 0x4e3230, 0x403390 (its arguments are passed on untouched) unless the replay mode is on */

CHARACTER *cm_CreateChar(char *name, D3VECTOR *pos)
{
    if (cm_SkipEvents != 0)
        return 0;
    return ch_Create(name, pos);
}
/* 0x4e3250, the scene helpers 0x4e3250 .. 0x4e3640 pass their arguments on to the sound / object calls unless the replay mode (0x6f0d8c) is on */

int cm_DeleteChar(int a)
{
    if (cm_SkipEvents != 0)
        return 1;
    return ch_Delete(a);
}
/* 0x4e3270, loads the voice bank name (snd_LoadVoiceBank; a set preloads its entries), 0 in the replay mode */

int cm_LoadVoiceBank(char *name, int a)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_LoadVoiceBank(name, a);
}
/* 0x4e3290, drops the reference of the voice bank a (sb_UnloadSampleBank), 0 in the replay mode */

int cm_UnloadVoiceBank(int a)
{
    if (cm_SkipEvents != 0)
        return 0;
    return sb_UnloadSampleBank(a);
}
/* 0x4e32b0, plays the voice sample of speaker id in bank (text, var pick the sample; snd_PlayVoice), 0 in the replay mode */

int cm_PlayVoice(int id, int bank, char *text, int var)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_PlayVoice(id, bank, text, var);
}
/* 0x4e32d0, queues the voice sample of speaker id in bank (text, var pick the sample; snd_QueueVoice) after delay, 0 in the replay mode */

int cm_QueueVoice(int id, int bank, char *text, int var, unsigned int delay)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_QueueVoice(id, bank, text, var, delay);
}
/* 0x4e32f0, unless replaying: the scene's sky cm_SceneSky (also d3_Sky; d3sky_Create horizon -2.25, fog start -0.2, scale 600)
   with the one layer cm_SkyLayer of the texture tex (flags 0, scale 3.0, v scale 0.8, v offset -0.25, as the SVSKYREC records
   give them), standing still and faded in to 255 over 10.0 */

void cm_CreateSceneSky(char *tex)
{
    D3TEXFLAGS flags;

    if (cm_SkipEvents == 0) {
        d3_Sky = cm_SceneSky = d3sky_Create(-2.25f, -0.2f, 600.0f);
        flags.all = 0;
        flags.b[2] = 0;
        cm_SkyLayer = d3sky_CreateLayer(cm_SceneSky, 0, tex, flags, 3.0f, 0, 0.8f, -0.25f);
        d3sky_SetLayerSpeed(cm_SceneSky, cm_SkyLayer, 0);
        d3sky_FadeLayer(cm_SceneSky, cm_SkyLayer, 0xff, 10.0f);
    }
}
/* 0x4e3390, releases the scene's sound / object slot 0x6f0db8 (0x45d800 first when 0x6f0db4 is set) */

int cm_DestroySceneSky(void)
{
    if (cm_SkipEvents == 0) {
        if (cm_SkyLayer != 0)
            d3sky_DeleteLayer(cm_SceneSky, cm_SkyLayer);
        d3sky_Destroy(cm_SceneSky);
        d3_Sky = 0;
    }
}
/* 0x4e33d0, remembers the result of 0x413b30(a) in 0x6f0dbc */

int cm_StartSceneRain(int a)
{
    if (cm_SkipEvents == 0)
        cm_SceneRain = d3t_CreateRain(a);
}
/* 0x4e33f0, frees the thing 0x4e33d0 remembered */

int cm_StopSceneRain(void)
{
    if (cm_SkipEvents == 0) {
        d3t_DeleteRain(cm_SceneRain);
        cm_SceneRain = 0;
    }
}
/* 0x4e3420, runs the scene for a ticks (frame wait with the flag 0x8000): the end sets the stop flag 0x6f22a0
   @deep deep s23: left one temp more in the original: a is loaded into ecx (mov ecx,[esp+8]; add esi,ecx),
   the later temps rotate edx / eax. end = a + end / end + a / unsigned a: = control; end = cm_SceneTicks +
   a worse. flip.py: only end += a written twice (a with two references) gives the original's load, so the
   original references a once more somewhere that leaves no code; not found. */

int cm_WaitTicks(int a)
{
    int end;

    end = cm_SceneTicks;
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(cm_WaitTicks, cm_DialogFrameFlags | 0x8000) != 0) {
            end += a;
            do {
                if (cm_SceneTicks >= end)
                    sv_PopRequest = 1;
                cm_UpdateDamagePops();
                cm_ShowPartyTexts(cm_CurrentFight);
            } while (sv_RunFrame(cm_WaitTicks, cm_DialogFrameFlags | 0x8000) != 0);
        }
    }
}
/* 0x4e3490, runs the scene's frames (sv_RunFrame with cm_RunSceneTicks) for ticks * 1/14 time units of d3_Time, showing the
   party's texts each frame (cm_UpdateDamagePops, cm_ShowPartyTexts); a right click (dm_L.rup) ends it at once with cb(arg) as the
   result (1 without cb), else 0. Nothing (0) when replaying */

int cm_RunSceneTicksOrClick(int ticks, int (*cb)(int), int arg)
{
    int start, res;

    start = d3_Time;
    res = 0;
    if (cm_SkipEvents != 0)
        return 0;
    if (sv_RunFrame(cm_RunSceneTicks, cm_DialogFrameFlags | 0x8000) != 0) {
        do {
            if (dm_L.rup != 0) {
                if (cb != 0)
                    res = cb(arg);
                else
                    res = 1;
                sv_PopRequest = 1;
            } else if ((double)d3_Time >= ticks * (1.0 / 14) + start)
                sv_PopRequest = 1;
            cm_UpdateDamagePops();
            cm_ShowPartyTexts(cm_CurrentFight);
        } while (sv_RunFrame(cm_RunSceneTicks, cm_DialogFrameFlags | 0x8000) != 0);
    }
    return res;
}
/* 0x4e3570, runs the scene for a ticks of 14 ms (d3_Time based): the end sets the stop flag 0x6f22a0 */

int cm_RunSceneTicks(int a)
{
    int start;
    double lim;

    start = d3_Time;
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(cm_RunSceneTicks, cm_DialogFrameFlags | 0x8000) != 0) {
            lim = (double)a * (1.0 / 14) + start;
            do {
                if ((double)d3_Time >= lim)
                    sv_PopRequest = 1;
                cm_UpdateDamagePops();
                cm_ShowPartyTexts(cm_CurrentFight);
            } while (sv_RunFrame(cm_RunSceneTicks, cm_DialogFrameFlags | 0x8000) != 0);
        }
    }
}
/* 0x4e3620, clears the sound queue and plays the special track name (snd_PlaySpecialTrack), nothing in the replay mode */

void cm_PlaySpecialTrack(char *name)
{
    if (cm_SkipEvents == 0) {
        snd_ClearQueue();
        snd_PlaySpecialTrack(name);
    }
}
/* 0x4e3640, ends the special track and restores the ambient sounds and the music volume (snd_EndSpecialTrack), nothing in the replay mode */

void cm_EndSpecialTrack(void)
{
    if (cm_SkipEvents == 0)
        snd_EndSpecialTrack();
}
/* 0x4e3650, sets the object tables 0x5fb5b0 up with the two 0xc0 byte callbacks 0x512de0 / 0x512e40 (0x438e60) after 0x4fe350, then 0x5128d0 */

void cm_ApplySeason(void)
{
    gm_ClockSeason(&ai_Time);
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_PlantTextureSetHook);
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_TextureSetHook);
    init_SetSeasonTextures();
}
/* 0x4e36a0, resets the 16 cutscene records cm_CutInfos for the party m: each one fresh 1, ready / valid 0, recvFlag 1, no
   text, and the member's netId m->members[j] as its playerId (-1 without a party); for a fight (kind 3) the 16 entries
   of each of the first m->used records whose playerId is set are freed (charId -1, state 0) */

int cm_ResetCutInfos(char *m)
{
    G28REC *party = (G28REC *)m;
    int j, i, q;

    for (j = 0; j < 16; j++) {
        cm_CutInfos[j].fresh = 1;
        cm_CutInfos[j].valid = 0;
        cm_CutInfos[j].ready = 0;
        cm_CutInfos[j].recvFlag = 1;
        if (party != 0) {
            cm_CutInfos[j].playerId = party->members[j];
            if (party->kind == 3) {
                for (i = 0; i < party->used; i++) {
                    if (cm_CutInfos[i].playerId == -1)
                        continue;
                    /* @original the playerId is tested twice: VC6 drops the second test, but the second reference
                       makes playerId the row walk's base (cmp [esi]; lea eax,[esi+0x88]) */
                    if (cm_CutInfos[i].playerId != -1) {
                        for (q = 0; q < 16; q++) {
                            cm_CutInfos[i].entries[q].charId = -1;
                            cm_CutInfos[i].entries[q].state = 0;
                        }
                    }
                }
            }
        } else
            cm_CutInfos[j].playerId = -1;
        cm_CutInfos[j].text[0] = 0;
    }
}
/* 0x4e3750, 1 when every member of the party m has answered: no cutscene record of its first m->used ones is still fresh
   (as cm_ResetCutInfos left it) without being valid and with a player (playerId != -1); cm_RunSceneParty waits on it */

int cm_PartyAnswered(char *m)
{
    G28REC *party = (G28REC *)m;
    int i, ok;

    ok = 1;
    for (i = 0; i < party->used; i++) {
        if (cm_CutInfos[i].fresh != 0 && cm_CutInfos[i].valid == 0 && cm_CutInfos[i].playerId != -1) {
            ok = 0;
            break;
        }
    }
    return ok;
}
/* 0x4e37a0, shows the texts of the scene party m: every one of its used members' cutscene records
   (cm_CutInfos) that is valid and not yet announced (recvFlag) gets its text printed in the party's
   window, separated by "$A" */

int cm_ShowPartyTexts(char *m)
{
    G28REC *party = (G28REC *)m;
    CMCUTINFO *s;
    int i;

    s = cm_CutInfos;
    for (i = 0; i < party->used; i++, s++) {
        if (s->recvFlag == 0 && s->valid != 0) {
            d2_SetForm(party->window, 0);
            io_PrintText(s->text);
            io_PrintText("$A");
            s->recvFlag = 1;
        }
    }
}
/* 0x4e3800, the highest index below n (1 ..) of the scene party m's members that is not -1; 0 without */

int cm_PrevPartyMember(char *m, int n)
{
    G28REC *party = (G28REC *)m;
    int i;

    for (i = n - 1; i > 0; i--)
        if (party->members[i] != -1)
            return i;
    return 0;
}
/* 0x4e3830, runs the scene party m for the scene context ctx: the party's cutscene records are reset (cm_ResetCutInfos),
   the owner announces the scene ("cut_type:%i", message 0x11). In the local run (ai_GameFlags & 8) every member's callback
   of the scene kind (cm_Kinds[].askPlayer for a player's character, kind 6, .askOther for the others) fills his record and its
   text is shown. In the net run (& 4) the records are sent as the long message with cm_RequestSendCutInfo: with the kind's
   flag cm_Kinds[].allAtOnce 1 the owner answers for the computer members, then each player for his own character, all at once;
   with 0 the members answer one after the other (a member once the one before him has answered, the owner the computer
   ones), until cm_PartyAnswered. The counter cm_ScenePartyRuns grows by one per run; the random seed is kept
   @deep deep s54: the copies r of the three askPlayer / askOther blocks as block locals (CMCUTINFO r; in
   each block; next stays function-scope, block-local next 75..87): 69 -> 65; in the player's turn r =
   cm_CutInfos[me] before cm_PartyTurn = me: 63. Left only the all-at-once owner loop: the original keeps
   the members IV (&party->members[i]) in esi as a split piece over head / tests / latch (reload mov
   esi,[esp+0x24] only at the body's end) and loads the cut IV for the valid test into ecx; ours keeps the
   members IV in memory (c2trace pass 3 piece: head use +4, reload -4, latch use +4, latch store -4 =
   savings 0). KEY: ONE more reference of party->members[i] in that loop's head or test chain (region r1,
   w2) is the whole lever: '|| party->members[i] == 0' after the valid test plus r = cm_CutInfos[i] BEFORE
   cm_CutChar = c = 2 ins, only the probe's own cmp [esi],0 / je left. Every compare form keeps its code
   (== -1, > 0x7fffffff, (char) casts, &party->members[i] == 0, an id pointer test before the deref; after
   the deref it folds with no reference). A reference in the body (r2 / r3) or after the loop does not
   help. Late-removed constructs that do count: a repeated test (cm_CutInfos[i].valid != 0 again: no code,
   one more cut IV reference; a dead && right side does not count), identical-arm ternaries in the call
   argument ((i < 16 ? M : M) and other i-against-constant conditions: 14 ins, but the i compare is LFTR'd
   onto the cut IV, which then takes edi; conditions on memory keep a load + test). Two latch copies (while
   loop, i++ before continue and at the end) split the cut IV in two (162) or, with a ci pointer, keep two
   uncross-jumped latches (136). Removed before allocation (= control): empty if, dead local store,
   self-assignment, (void) / ?: (void)0 statements, flag-guarded tests, x - x / x ^ x, a members test as
   the body's last statement. = control: tests as positive if / split / nested, assign in test, ((G28REC
   *)m)->members, a ci pointer, memcpy, no cb local, c / h / cb block-local, while forms, do-while(0) /
   for(;;) break probes, r scopes (64), a real CMCUTINFO array for the view macro, G20_ME for the host
   tests, gm_FindChar parameter types; worse: the A counter as prev / me (77 / 67), a hand-rotated loop
   (95). climb 3 rounds from the tree, 2 from r-first: nothing; climb from the probe: only r-first. Open:
   which ordinary statement gives that one reference without code. prior: s51: climb round 1 found the
   lever: in the owner's turn loop of the one-after-the-other run, next = cm_CutInfos[i] copied before the
   cm_PartyTurn / cm_CutChar stores (237 -> 69; climb 3 more rounds: nothing). Left: (1) the all-at-once
   owner loop's IV slots: the original has the cut-record IV at 0x10, i at 0x14, the members IV at 0x18 and
   keeps the members IV in esi for the gm_FindChar argument (reloaded from its slot at the latch, the rep
   movsd source taken from the cut slot), ours members IV at 0x14 via eax, i at 0x18, esi = the loaded cut
   pointer; (2) the one-after-the-other player's turn: the original pushes party->members[me] before the
   rep movsd of rec (after the cm_PartyTurn store), ours copies first. No lever: the loop's continue as one
   positive if, the valid test split, rec copied before cm_CutChar = c in the first owner loop (73), next
   instead of rec there (75), a separate counter j for any subset of the four loops (all = control). */

void cm_RunSceneParty(char *m, int ctx)
{
    G28REC *party = (G28REC *)m;
    CMCUTTYPEMSG msg;
    CMCUTINFO rec;
    CMCUTINFO next;
    int saved, i, me, prev, h;
    AICHAR *c;
    int (*cb)(CMCUTINFO *, int);

    saved = cut_GetRandSeed();
    cm_ResetCutInfos(m);
    if (ai_GameFlags & 4) {
        if (party->host == ai_Chars[ai_PlayerId].netId && cm_SkipEvents == 0) {
            memset(&msg, 0, sizeof(msg));
            msg.party = party->id;
            sprintf(msg.text, "cut_type:%i", party->kind);
            cm_SendExSysMessage(0x11, &msg);
        }
        if (cm_SkipEvents == 0)
            sv_SyncText(txt_ScenePartySync);
    }
    sv_SetStatusText(&ch_EmptyName);
    if (ai_GameFlags & 8) {
        for (i = 0; i < party->used; i++) {
            cm_PartyTurn = i;
            if (party->members[i] == -1)
                continue;
            cm_CutChar = (char *)gm_FindChar(party->members[i]);
            if (cm_CutChar == 0) {
                party->members[i] = -1;
                continue;
            }
            if (((AICHAR *)cm_CutChar)->kind == 6) {
                cm_SceneDeadline = cm_SceneTicks + cm_SceneDuration;
                cm_SceneTimedOut = 0;
                ((int (*)(CMCUTINFO *, int))cm_Kinds[party->kind].askPlayer)(&cm_CutInfos[i], ctx);
            } else {
                cb = (int (*)(CMCUTINFO *, int))cm_Kinds[party->kind].askOther;
                if (cb != 0)
                    cb(&cm_CutInfos[i], ctx);
            }
            d2_SetForm(party->window, 0);
            io_PrintText(cm_CutInfos[i].text);
            io_PrintText("$A");
        }
    } else if (ai_GameFlags & 4) {
        cm_CutChar = (char *)&ai_Chars[ai_PlayerId];
        if (cm_Kinds[party->kind].allAtOnce == 1) {
            if (party->host == ai_Chars[ai_PlayerId].netId) {
                /* the owner answers for the computer members */
                for (i = 0; i < party->used; i++) {
                    CMCUTINFO r;

                    c = (AICHAR *)gm_FindChar(party->members[i]);
                    if (c == 0 || c->kind == 6 || c->kind == 7 || cm_CutInfos[i].valid != 0)
                        continue;
                    cm_CutChar = (char *)c;
                    r = cm_CutInfos[i];
                    cb = (int (*)(CMCUTINFO *, int))cm_Kinds[party->kind].askOther;
                    if (cb != 0)
                        cb(&r, ctx);
                    r.valid = 1;
                    r.ready = 0;
                    cm_SetLongMsg(0x35c, &r);
                    h = cm_RequestSendCutInfo(c->netId, party->id);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    cm_ShowPartyTexts(m);
                    sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
                }
            }
            /* each player answers for his own character */
            cm_CutChar = (char *)&ai_Chars[ai_PlayerId];
            for (i = 0; i < party->used; i++) {
                if (party->members[i] == ((AICHAR *)cm_CutChar)->netId) {
                    CMCUTINFO r;

                    r = cm_CutInfos[i];
                    cm_SceneDeadline = cm_SceneTicks + cm_SceneDuration;
                    cm_SceneTimedOut = 0;
                    ((int (*)(CMCUTINFO *, int))cm_Kinds[party->kind].askPlayer)(&r, ctx);
                    r.valid = 1;
                    r.ready = 0;
                    cm_SetLongMsg(0x35c, &r);
                    h = cm_RequestSendCutInfo(((AICHAR *)cm_CutChar)->netId, party->id);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    break;
                }
            }
            while (cm_PartyAnswered(m) == 0) {
                cm_ShowPartyTexts(m);
                sv_SetStatusText(txt_ScenePartyWait);
                sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
            }
        } else if (cm_Kinds[party->kind].allAtOnce == 0) {
            /* one after the other: the player's character answers once the member before him has */
            me = -1;
            prev = -1;
            for (i = 0; i < party->used; i++) {
                if (party->members[i] == ai_Chars[ai_PlayerId].netId) {
                    me = i;
                    break;
                }
            }
            if (me != -1)
                prev = cm_PrevPartyMember(m, me);
            while (cm_PartyAnswered(m) == 0) {
                cm_ShowPartyTexts(m);
                sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
                if (me != -1 && (me == 0 || cm_CutInfos[prev].valid == 1) && cm_CutInfos[me].valid == 0) {
                    CMCUTINFO r;

                    r = cm_CutInfos[me];
                    cm_PartyTurn = me;
                    cm_CutChar = (char *)gm_FindChar(party->members[me]);
                    cm_ShowPartyTexts(m);
                    cm_SceneDeadline = cm_SceneTicks + cm_SceneDuration;
                    cm_SceneTimedOut = 0;
                    ((int (*)(CMCUTINFO *, int))cm_Kinds[party->kind].askPlayer)(&r, ctx);
                    r.valid = 1;
                    r.ready = 0;
                    cm_SetLongMsg(0x35c, &r);
                    h = cm_RequestSendCutInfo(((AICHAR *)cm_CutChar)->netId, party->id);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    cm_ShowPartyTexts(m);
                    sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
                }
                /* the owner answers for the first computer member whose turn it is */
                if (party->host == ai_Chars[ai_PlayerId].netId) {
                    for (i = 0; i < party->used; i++) {
                        c = (AICHAR *)gm_FindChar(party->members[i]);
                        if (c == 0 || c->kind == 6 || c->kind == 7)
                            continue;
                        if (i != 0 && cm_CutInfos[cm_PrevPartyMember(m, i)].valid != 1)
                            continue;
                        if (cm_CutInfos[i].valid != 0)
                            continue;
                        next = cm_CutInfos[i];
                        cm_PartyTurn = i;
                        cm_CutChar = (char *)c;
                        cb = (int (*)(CMCUTINFO *, int))cm_Kinds[party->kind].askOther;
                        if (cb != 0)
                            cb(&next, ctx);
                        next.valid = 1;
                        next.ready = 0;
                        cm_SetLongMsg(0x35c, &next);
                        h = cm_RequestSendCutInfo(c->netId, party->id);
                        while (cm_GetSlotState(h) == 0)
                            sv_PumpNet();
                        break;
                    }
                }
            }
            cm_ShowPartyTexts(m);
            sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
        }
    }
    cm_ScenePartyRuns = cm_ScenePartyRuns + 1;
    cut_RandSeed(saved);
}
/* 0x4e3fe0, switches the scene mode on: stops the clock (tb_SetFunctionOff(sv_UpdateClock, 1)), then pauses the
   animation of every character of ch_List whose flags bit 4 (+0x8c) is not set */

int cm_PauseSceneChars(void)
{
    CHARACTER **pp;

    tb_SetFunctionOff(sv_UpdateClock, 1);
    for (pp = ch_List; (int)pp < (int)(ch_List + CH_MAX); pp++) {
        if (*pp != 0 && !((*pp)->flags.bytes.lo & 4))
            ch_PauseAnim(*pp, 1);
    }
}
/* 0x4e4020, the reverse: the characters with the bit 4 get ch_PauseAnim(ch, 0) */

int cm_ResumeSceneChars(void)
{
    CHARACTER **pp;

    tb_SetFunctionOff(sv_UpdateClock, 0);
    for (pp = ch_List; (int)pp < (int)(ch_List + CH_MAX); pp++) {
        if (*pp != 0 && ((*pp)->flags.bytes.lo & 4))
            ch_PauseAnim(*pp, 0);
    }
}
/* 0x4e4060, runs the scene party m, the main function of the cutscenes: clears the scene state cm_SceneState and decides
   whether the scene is only skipped (cm_SkipEvents: flags 2 with no player's character among the members in the local run,
   or 0x4e4870 says so in the net run); unless skipped, fades to "BLACK", keeps the camera, the fog and the dirty rect,
   starts the scene timer 0x4e3210, disables the shown d2 objects and for a cinemascope party (flags 0x20) narrows the
   viewport to the letterbox. Opens the scene window "cutscenes\\announcement" (the trial: "cutscenes\\prozess_main"),
   seeds the random numbers with m->seed and calls the run function of the kind (cm_Kinds[].run); then restores all of it,
   fades in again, and the owner gives every member's character back its scene party field (+0x20c = -1, a field
   message) */

void cm_ExecCutscene(char *m)
{
    G28REC *party = (G28REC *)m;
    int ours, shown, panel;
    float savedSpeed;
    int savedFog, savedLowPoly;
    int rect[4];
    float pos[4], rot[4];
    char buf[0x200];
    int savedBox, i;
    AICHAR *c;
    unsigned int fl;

    savedSpeed = cm_SpeedFactor;
    savedLowPoly = ch_LowPoly;
    ours = 0;
    shown = 0;
    savedFog = 0;
    panel = 0;
    sprintf(buf, "Exec Cutscene(Mainfunc start) <Type = %i> <id = %i", party->kind, party->id);
    rect[2] = d2_DirtyRect.right;
    rect[0] = d2_DirtyRect.left;
    rect[1] = d2_DirtyRect.top;
    rect[3] = d2_DirtyRect.bottom;
    cm_SavedScene = d3_CurrentScene;
    cm_SavedNoGroup = evt_NoGroup;
    cm_ScenePartyRuns = 0;
    cm_InitDamagePops();
    AI_ZERO_AT(offset cm_SceneState, 0x480);  /* @vc6 clang's MS asm cannot take the offset of an incomplete array */
    cm_SkipEvents = 0;
    party->flags |= 4;
    if (sv_FrameMask & 0x200000)
        party->flags |= 0x40;
    cm_DialogFrameFlags = 0x717c6;
    if (party->flags & 0x40)
        cm_DialogFrameFlags = 0x716c6;
    if (ai_GameFlags & 8) {
        /* ours: no player's character (kind 6) is a member */
        ours = 1;
        for (i = 0; i < party->used; i++) {
            if (party->members[i] != -1 && (c = (AICHAR *)gm_FindChar(party->members[i])) != 0 && c->kind == 6) {
                ours = 0;
                break;
            }
        }
        for (i = 0; i < party->used; i++) {
            if (party->members[i] != -1 && (c = (AICHAR *)gm_FindChar(party->members[i])) != 0 && c->kind == 6) {
                ours = 0;
                break;
            }
        }
        if (ours != 0 && !party->flags & 2)  /* as the original: (!flags) & 2, never true */
            return;
    }
    if (party->flags & 2) {
        if (ai_GameFlags & 8) {
            if (ours != 0)
                cm_SkipEvents = 1;
        } else if ((ai_GameFlags & 4) && cm_IsPartyMember(ai_Chars[ai_PlayerId].netId, m) == 0)
            cm_SkipEvents = 1;
    }
    if (cm_SkipEvents == 0) {
        fl = (sv_FrameMask & ~8) | 0x10080;
        cm_SceneFade = (char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
        while ((*cm_SceneFade & 4) == 0)
            sv_RunFrame(0, fl);
        pos[0] = sv_ViewPos[0];
        pos[1] = sv_ViewPos[1];
        pos[2] = sv_ViewPos[2];
        rot[0] = sv_ViewRot[0];
        rot[1] = sv_ViewRot[1];
        rot[2] = sv_ViewRot[2];
        if (ai_GameFlags & 8)
            cm_PauseSceneChars();
        tb_OpenFunction(cm_SceneTick, 0x46);
        savedFog = d3_FogByDistance;
        d3_FogByDistance = 0;
        cm_SpeedFactor = 0.6f;
        d3o_FogScale = 1.0f;
        ch_LowPoly = 0;
        d2_SetPointerSprite(0, 0);
        sv_ClearSelection();
        d2_ClearPointerItems();
        sv_ResetScreenClip();
        d2_CaptionCopy[0] = 0;
        for (i = 0, shown = 0; i < 0x300; i++) {
            if (d2_Objs[i].type != 0 && d2_Objs[i].disabled == 0) {
                cm_DisabledObjs[shown] = i;
                d2_Objs[i].disabled = 1;
                shown++;
            }
        }
        cm_SceneNothing();
        cm_ClearSceneChars();
        if (party->flags & 0x20) {
            /* the cinemascope letterbox: 75 (scaled) lines off the top and the bottom */
            cm_DialogFrameFlags &= ~0x100;
            d3_SetViewport((float)d2_ScreenW, d2_ScreenH - d2_ScreenScale * 150.0f, 0, 75.0f * d2_ScreenScale,
                           d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
            d2_DirtyRect.right = 0;
            d2_DirtyRect.left = d2_ScreenW;
            d2_DirtyRect.top = (int)(d2_ScreenScale * 75.0f);
            d2_DirtyRect.bottom = (int)(d2_ScreenH - d2_ScreenScale * 75.0f);
            if (sv_PanelEnabled != 0) {
                sv_SetPanelEnabled(0);
                panel = 1;
            } else
                panel = 0;
        }
    } else if (cm_CurrentFight == 0)
        cm_ResetCutInfos(m);
    savedBox = cm_TextBox;
    if (party->kind == 2)
        party->window = d2_OpenForm(0, 0, "cutscenes\\prozess_main");
    else if (party->flags & 0x20)
        party->window = d2_OpenForm(0, 0, "cutscenes\\announcement_cinemascope");
    else
        party->window = d2_OpenForm(0, 0, "cutscenes\\announcement");
    cm_TextBox = party->window;
    d2_PlaceMainWindow(cm_TextBox, 3);
    if (cm_SkipEvents != 0)
        d2_SetViewEnabled(cm_TextBox, 0);
    cut_RandSeed(party->seed);
    sprintf(buf, "Exec Cutscene(Mainfunc) <Type = %i>", party->kind);
    cm_CurrentFight = (int *)m;
    ((void (*)(char *))cm_Kinds[party->kind].run)(m);
    d2_CloseView(party->window);
    cm_TextBox = savedBox;
    if (cm_SkipEvents == 0) {
        cm_DeleteSceneChars();
        ch_LowPoly = savedLowPoly;
        if (ai_GameFlags & 8)
            cm_ResumeSceneChars();
        cm_SceneDuration = 10;
        tb_CloseFunction(cm_SceneTick);
        for (i = 0; i < shown; i++)
            d2_Objs[cm_DisabledObjs[i]].disabled = 0;
        if ((party->flags & 0x20) && (!(party->flags & 0x40) || (party->flags & 1))) {
            d3_SetViewport((float)(sv_ViewRectRight - sv_ViewRectLeft), (float)(sv_ViewRectBottom - sv_ViewRectTop),
                           (float)sv_ViewRectLeft, (float)sv_ViewRectTop, d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW,
                           d2_ScreenH);
            d2_DirtyRect.right = rect[2];
            d2_DirtyRect.left = rect[0];
            d2_DirtyRect.top = rect[1];
            d2_DirtyRect.bottom = rect[3];
            if (panel != 0 && !(party->flags & 0x40)) {
                sv_SetPanelEnabled(1);
                sv_RedrawScreen();
            }
        }
        d3if_SelectScene(cm_SavedScene, 0);
        if (d3_Sky != 0)
            d3sky_Init(d3_Sky, 1);
        sv_ViewPos[0] = pos[0];
        sv_ViewPos[1] = pos[1];
        sv_ViewPos[2] = pos[2];
        sv_ViewRot[0] = rot[0];
        sv_ViewRot[1] = rot[1];
        sv_ViewRot[2] = rot[2];
        d3_SetFogSet(0, 1, 0);
        d3_FogByDistance = savedFog;
        cm_SpeedFactor = savedSpeed;
        if (ai_PlayerBuilding == 0 && ai_FallbackBuilding == 0)
            d3o_FogScale = 1.0 - (ai_GfxFog.word & 0xff) * 0.01;
        init_SetDetailLevel(d3_Terrain, 0);
        d3if_Prerender(1);
        if (cm_SceneFade != 0) {
            d2_ReleaseFade((struct D2FADE *)cm_SceneFade);
            cm_SceneFade = 0;
            d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 10);
        }
    }
    cm_ResetCutInfos(m);
    if (party->host == ai_Chars[ai_PlayerId].netId) {
        for (i = 0; i < 16; i++) {
            cm_CutInfos[i].ready = 0;
            cm_CutInfos[i].fresh = 0;
            cm_CutInfos[i].valid = 0;
            if (party->members[i] != -1 && (c = (AICHAR *)gm_FindChar(party->members[i])) != 0) {
                int v = -1;

                cm_BeginFieldBlock(c, c->netId);
                cm_AddFieldValue(4, 1, (char *)c - cm_FieldObject + 0x20c, &v);
                cm_SendFieldValues();
            }
        }
    }
    if (party->host != -1) {
        int v = -1;

        if ((c = (AICHAR *)gm_FindChar(party->host)) != 0) {
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldValue(4, 1, (char *)c - cm_FieldObject + 0x20c, &v);
            cm_SendFieldValues();
        }
    }
    cm_CurrentFight = 0;
    evt_NoGroup = cm_SavedNoGroup;
}
/* 0x4e4870, 1 when the character id is one of the scene party m's members and still exists */

int cm_IsPartyMember(int id, char *m)
{
    G28REC *party = (G28REC *)m;
    int i, n;

    n = party->used;
    for (i = 0; i < n; i++) {
        if (party->members[i] == id) {
            if (gm_FindChar(id) != 0)
                return 1;
            break;
        }
    }
    return 0;
}
/* 0x4e48b0, sends the note text (and the second text extra, joined after it, textFlags bit 0x10) about the party b to
   the character a as a long message (0x1c, kind 0x11, text id 0x5ec): with it the party's building b->thing, the party's
   id and the object id of the building's main room (of a home: its object of type 0xfd), -1 without; textFlags 0xe
   when the party's flags have bit 0x10 and it has a building, else 0xc */

void cm_SendPartyNote(AICHAR *a, G28REC *b, char *text, char *extra)
{
    G28REC *party = b;
    CMNOTE note;
    AIBLD *bld;
    AIOBJ *o;
    int len;

    memset(&note.head, 0, sizeof(note.head));
    note.head.type = 0x11;
    note.head.fromId = -1;
    note.head.flags = 9;
    note.head.to = a->netId;
    note.head.arg[0] = 0x5ec;
    if (party->thing != -1 && (party->flags & 0x10))
        note.head.textFlags = 0xe;
    else
        note.head.textFlags = 0xc;
    note.head.arg[1] = party->thing;
    note.head.arg[3] = party->id;
    note.head.arg[2] = -1;
    if (party->thing != -1) {
        bld = (AIBLD *)gm_FindBuildingById(party->thing);
        if (bld != 0) {
            if (gm_IsHomeType((unsigned char *)bld) == 0) {
                o = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)bld);
                if (o != 0)
                    note.head.arg[2] = o->objId;
            } else {
                o = (AIOBJ *)gm_Query((void *)bld->objList, 1, 0, 0xfd);
                if (o != 0)
                    note.head.arg[2] = o->objId;
            }
        }
    }
    strcpy(note.text, text);
    len = strlen(note.text) + 1;
    if (extra != 0 && *extra != 0) {
        memcpy(note.text + len, extra, strlen(extra) + 1);
        note.head.textFlags |= 0x10;
        cm_SendLongMessageText((char *)&note, len + strlen(extra) + 1, note.text);
    } else
        cm_SendLongMessageText((char *)&note, strlen(text) + 1, text);
}
/* 0x4e4a50, "cut_CheckMaster": 1 when the scene party m's host is one of its members; else the first member
   of kind 6 / 7 that still exists (as long as none was found, 16 slots) becomes the new host: the log
   "new master for id %i => %i", a request (cm_SendMasterChange) and the wait for its answer
   (cm_GetSlotState with the frame wait sv_PumpNet); 0 then. The log "invalid master for id %i" comes first. */

int cm_CheckMaster(char *m)
{
    G28REC *party = (G28REC *)m;
    char buf[0x80];
    AICHAR *c;
    int flag, i, h;

    if (cm_IsPartyMember(party->host, m) == 0) {
        flag = 1;
        c = (AICHAR *)gm_FindChar(party->host);
        sprintf(buf, "cut_CheckMaster(): invalid master for id %i", party->id);
        for (i = 0; i < 0x10 && flag != 0; i++) {
            if (party->members[i] != -1) {
                c = (AICHAR *)gm_FindChar(party->members[i]);
                if (c != 0 && (c->kind == 6 || c->kind == 7))
                    flag = 0;
            }
        }
        if (c != 0 && c->netId != party->host) {
            sprintf(buf, "cut_CheckMaster(): new master for id %i => %i", party->id, c->netId);
            h = cm_SendMasterChange(party, c);
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
            return 0;
        }
    }
    return 1;
}
/* 0x4e4b50, "cut_CutsceneReady": whether the party m can start. On its owner's side (flags bit 7 clear) every member that is
   gone, or a player's character (kind 6 / 7) still in another party, is sent request 0x2a and the last request is waited
   for; on the other side such a member makes 0. Then the owner is checked (cm_CheckMaster, -1 when it had to change) and the
   party's id with its 16 members goes out as message 0x59: 0 when the answer is 2, else 1 */

int cm_CutsceneReady(char *m)
{
    G28REC *party = (G28REC *)m;
    int buf[0x12];
    int i, h;
    AICHAR *c;

    memset(buf, 0, sizeof(buf));
    i = 0;
    if (!(party->flags & 0x80)) {
        h = -1;
        for (; i < party->used; i++) {
            if (party->members[i] == -1)
                continue;
            c = (AICHAR *)gm_FindChar(party->members[i]);
            if (c == 0 || (c->partyId != -1 && c->partyId != party->id && (c->kind == 6 || c->kind == 7)))
                h = cm_SendLeaveParty(party->id, party->members[i]);
        }
        while (h != -1 && cm_GetSlotState(h) == 0)
            sv_PumpNet();
    } else {
        for (; i < party->used; i++) {
            if (party->members[i] == -1)
                continue;
            c = (AICHAR *)gm_FindChar(party->members[i]);
            if (c != 0 && c->partyId != -1 && c->partyId != party->id && (c->kind == 6 || c->kind == 7))
                return 0;
        }
    }
    if (cm_CheckMaster(m) == 0)
        return -1;
    buf[0] = party->id;
    memcpy(buf + 1, party->members, sizeof(party->members));
    cm_BeginGroup("cut_CutsceneReady");
    h = cm_SendCutsceneReady(buf);
    cm_EndGroup("cut_CutsceneReady");
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    return cm_GetSlotState(h) != 2;
}
/* 0x4e4cc0, the per-frame update of the 96 scene parties cm_Parties (used, not flags 1 / 4): a party without an owner
   gets the player's character in the local run (or cm_NetFlags & 8); within the hour before its time the owner asks
   cm_CutsceneReady (-1: again in 5 minutes, 0: in 30, from 22 o'clock on 9 hours later) and the kind's check
   (cm_Kinds[].check; 0 cancels the party, message 0x58). Once its time has come the party is done when it has no check, when
   the player's character is a member of it running in its own party (flags 0x80) or owns it with flags 2; else it is
   dropped (cm_ClearParty). A done party within its hour runs (cm_ExecCutscene) and is dropped */

int cm_UpdateParties(void)
{
    G28REC *p;
    AITIME soon;
    int i, k;

    for (i = 0; i < 96; i++) {
        p = &cm_Parties[i];
        if (p->used == 0 || (p->flags & 5))
            continue;
        soon = p->time;
        gm_AddTime(&soon, 0, -60, 0);
        if (p->host == -1 && ((cm_NetFlags & 8) || (ai_GameFlags & 8)))
            p->host = ai_Chars[ai_PlayerId].netId;
        if (ai_Chars[ai_PlayerId].netId == p->host && p->state == 0 && gm_CompareTime(&ai_Time, &soon) > 0) {
            k = cm_CutsceneReady((char *)p);
            if (k == -1) {
                gm_AddTime(&p->time, 0, 5, 0);
                if (p->time.hour >= 0x16)
                    gm_AddTime(&p->time, 9, 0, 0);
                continue;
            }
            if (k == 0) {
                gm_AddTime(&p->time, 0, 0x1e, 0);
                if (p->time.hour >= 0x16)
                    gm_AddTime(&p->time, 9, 0, 0);
                continue;
            }
            if (cm_Kinds[p->kind].check != 0 && ((int (*)(char *))cm_Kinds[p->kind].check)((char *)p) == 0) {
                cm_SendCancelParty(p->id);
                continue;
            }
        }
        if (cm_Kinds[p->kind].check == 0 && gm_CompareTime(&ai_Time, &p->time) > 0)
            p->done = 1;
        if (gm_CompareTime(&ai_Time, &p->time) > 0 && p->done == 0) {
            if (cm_IsPartyMember(ai_Chars[ai_PlayerId].netId, (char *)p) != 0 && (p->flags & 0x80)) {
                if (ai_Chars[ai_PlayerId].partyId == p->id)
                    p->done = 1;
            } else if (p->host == ai_Chars[ai_PlayerId].netId && (p->flags & 2))
                p->done = 1;
            else
                cm_ClearParty(p->id);
        }
        if (gm_CompareTime(&ai_Time, &soon) > 0 && p->done != 0) {
            sv_PanelFlag = 0;
            cm_ExecCutscene((char *)p);
            cm_ClearParty(p->id);
        }
    }
    return 0;
}
/* 0x4e4f40, the pending scene party with the smallest id (used, id not -1, flags 1): its record (cm_PartyById), 0 without */

int cm_FirstPendingParty(void)
{
    int i;
    unsigned int best;

    best = (unsigned int)-1;
    for (i = 0; i < 96; i++) {
        if (cm_Parties[i].used != 0 && cm_Parties[i].id != -1 && (cm_Parties[i].flags & 1) != 0 && best >= (unsigned int)cm_Parties[i].id)
            best = cm_Parties[i].id;
    }
    if (best != (unsigned int)-1)
        return cm_PartyById(best);
    return 0;
}
/* 0x4e4f80, runs all pending parties (cm_FirstPendingParty) of the player pl that he is a member of: his
   partyId holds the id of the party while it runs (cm_ExecCutscene), every party is dropped (cm_ClearParty)
   and the field is sent back (cm_AddFieldValue / cm_SendFieldValues); 1 when one ran */

char ai_RunPendingParties(int pl)
{
    G28REC *p;
    char ret;
    int save;

    ret = 0;
    save = ai_Chars[pl].partyId;
    p = (G28REC *)cm_FirstPendingParty();
    while (p != 0) {
        if (cm_IsPartyMember(ai_Chars[pl].netId, (char *)p) != 0) {
            ai_Chars[pl].partyId = p->id;
            cm_ExecCutscene((char *)p);
            ai_Chars[pl].partyId = -1;
            ret = 1;
        }
        cm_ClearParty(p->id);
        p = (G28REC *)cm_FirstPendingParty();
    }
    cm_BeginFieldBlock((char *)&ai_Chars[pl], ai_Chars[pl].netId);
    cm_AddFieldValue(4, 1, (unsigned short)((int)&ai_Chars[pl].partyId - (int)cm_FieldObject), &save);
    cm_SendFieldValues();
    return ret;
}
/* 0x4e5050, the used party of the kind a with the thing b: its record, 0 without */

int cm_FindPartyOfKind(int a, int b)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (cm_Parties[i].used != 0 && cm_Parties[i].kind == a && cm_Parties[i].thing == b)
            return (int)&cm_Parties[i];
    }
    return 0;
}
/* 0x4e50a0, the used party with the id: its record, 0 without */

int cm_PartyById(int id)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (cm_Parties[i].used != 0 && cm_Parties[i].id == id)
            return (int)&cm_Parties[i];
    }
    return 0;
}
/* 0x4e50e0, the cutscene record cm_CutInfos whose playerId is id; 0 without */

int cm_CutInfoById(int id)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_CutInfos[i].playerId == id)
            return (int)&cm_CutInfos[i];
    }
    return 0;
}
/* 0x4e5120, the first free scene party record of cm_Parties (id -1), 0 without; cm_AllocParty stores a
   new party there */

int cm_FreePartySlot(void)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (cm_Parties[i].id == -1)
            return (int)&cm_Parties[i];
    }
    return 0;
}
/* 0x4e5150, stores a copy of the party src (0x114 bytes) in a free record, with the next id 0x61a658; the byte +0x31 defaults to 8; the record */

int cm_AllocParty(int *src)
{
    G28REC *r;

    r = (G28REC *)cm_FreePartySlot();
    if (r != 0) {
        memcpy(r, src, 0x114);
        r->id = ai_NextPartyId;
        if (((unsigned char *)r)[0x31] == 0)
            ((unsigned char *)r)[0x31] = 8;
    }
    return (int)r;
}
/* 0x4e5180, clears the party record of the id (id -1 afterwards) */

void cm_ClearParty(int id)
{
    G28REC *r;

    r = (G28REC *)cm_PartyById(id);
    if (r != 0) {
        memset(r, 0, 0x114);
        r->id = -1;
    }
}
/* 0x4e51b0, adds the character who to the party id (+0x30 count, +0x34 ids, at most 16) unless he is in it: 1 when added */

int cm_AddPartyMember(int id, int who)
{
    G28REC *r;
    int i, n;

    r = (G28REC *)cm_PartyById(id);
    if (r != 0) {
        i = 0;
        n = r->used;
        for (; i < n; i++) {
            if (r->members[i] == who)
                return 0;
        }
        if (r->used < 0x10) {
            r->members[n] = who;
            r->used++;
            return 1;
        }
    }
    return 0;
}
/* 0x4e5210, removes the character who from the party id (his id becomes -1) and, when it is the running party 0x6f0d90, from its member slots (0x92caf0); 0
   when there is no such party */

int cm_RemovePartyMember(int id, int who)
{
    G28REC *r;
    int i;

    r = (G28REC *)cm_PartyById(id);
    if (r == 0)
        return 0;
    for (i = 0; i < r->used; i++) {
        if (r->members[i] == who) {
            r->members[i] = -1;
            break;
        }
    }
    if (cm_CurrentFight != 0 && *cm_CurrentFight == id) {
        for (i = 0; i < 16; i++) {
            if (cm_CutInfos[i].playerId == who) {
                cm_CutInfos[i].playerId = -1;
                break;
            }
        }
    }
    return 1;
}
/* 0x4e52a0, sets the seed (logged "cut_randseed: %i") */

void cut_RandSeed(unsigned int seed)
{
    char buf[0x40];

    cut_RandState = seed;
    sprintf(buf, "cut_randseed: %i", seed);
}
/* 0x4e52c0, the seed (logged "cut_getrandseed: %i") */

unsigned int cut_GetRandSeed(void)
{
    char buf[0x40];

    sprintf(buf, "cut_getrandseed: %i", cut_RandState);
    return cut_RandState;
}
/* 0x4e52f0, a random number below n (logged "cut_rnd: %i"), 0 for n == 0 */

unsigned int cm_Random(unsigned int n)
{
    char buf[0x40];

    if (n == 0)
        return 0;
    cut_RandState = cut_RandState * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rnd: %i", (cut_RandState >> 16) % 0x7fffu);
    return ((cut_RandState >> 16) % 0x7fffu) % n;
}
/* 0x4e5360, a random float in [0, 1) (logged "cut_rndfloat: %f") */

double cm_RandomFloat(void)
{
    char buf[0x40];

    cut_RandState = cut_RandState * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rndfloat: %f", (float)((cut_RandState >> 16) % 0x7fffu) * 3.0518509e-5f);
    return (float)((cut_RandState >> 16) % 0x7fffu) * 3.0518509e-5f;
}
/* 0x4e53e0, the progress of the dialog dlg: with c set the clock 0x6333cc and the duration a are stored (and the window cleared, 0x475e40);
   else the left percent of the duration is the value; the slider (0x475cb0, 0 .. 100) is set to it, returned */

int cm_DialogTimer(int dlg, int a, int c)
{
    int v;

    v = 100;
    if (c != 0) {
        cm_DialogStart = d3_Time;
        cm_DialogLength = a;
        d2_SetObjEnabled(dlg, 0);
    } else {
        v = (int)((1.0f - (float)(unsigned int)(d3_Time - cm_DialogStart) / cm_DialogLength) * 100.0f);
        v = v > 0 ? v : 0;
    }
    d2_SetObjValue(dlg, 0, 100, v);
    return v;
}
/* 0x4e5470, a message box for the text id with the flags (2: "Messagebox_Autosize", 0x10: "_BIG", 0x20: "_VERY_BIG", 0x100: "_NONE_PERGA" (form 1), else "misc\\Messagebox";
   8: the frame wait flags 0x717c6, 0x80: a timer slider (0x500 ms), 0x40: raises the dialog's windows every frame, 4: ends after 500 ms): the dialog's
   objects are put into a radio group (all but the type 0x40), the loop runs the frame wait sv_RunFrame until the stop request; the answer: the selected
   button index + 1 when the text window is clicked, 0 otherwise (a right click on it, the escape key, a timeout); not reentrant (cm_MessageBoxOpen). */

int cm_MessageBox(int id, int flags)
{
    unsigned long t0;
    int ret, win, textwin, radio, i, k, slider, fl;

    t0 = d3_Time;
    slider = -1;
    ret = 0;
    if (cm_MessageBoxOpen != 0)
        return 0;
    fl = sv_FrameMask & ~0x38;
    if (flags & 2)
        win = d2_OpenForm(0, 0, "misc\\Messagebox_Autosize");
    else if (flags & 0x10)
        win = d2_OpenForm(0, 0, "misc\\Messagebox_BIG");
    else if (flags & 0x20)
        win = d2_OpenForm(0, 0, "misc\\Messagebox_VERY_BIG");
    else if (flags & 0x100) {
        win = d2_OpenForm(0, 0, "misc\\Messagebox_NONE_PERGA");
        d2_SetForm(win, 1);
    } else
        win = d2_OpenForm(0, 0, "misc\\Messagebox");
    if (flags & 8)
        fl = 0x717c6;
    io_PrintText(id);
    textwin = d2_LastWindow;
    d2_CentreViewWindows(win);
    cm_MessageBoxOpen = 1;
    radio = d2_CreateRadioGroup(0);
    for (i = 0; i < d2_LastWindowPtr->nobjs; i++) {
        k = d2_LastWindowPtr->objs[i];
        if (d2_Objs[k].type != 0x40)
            d2_RadioAdd(radio, k);
    }
    d2_RadioSelect(radio, 0);
    d2_SetForm(win, 0);
    if (flags & 0x80)
        slider = d2_WinOpenSlider(0x1c, d2_LastWindowPtr->h - 0x18, d2_LastWindowPtr->w - 0x40, 0, 0x1f4, 0x83, 0x42, d2_LastWindow);
    while (sv_RunFrame(cm_MessageBox, fl) != 0) {
        d2_MenuNavigate(radio);
        if (flags & 0x40)
            d2_RaiseViewWindows(win);
        if (d2_ClickedId == 0 && d2_HitWindow == textwin) {
            ret = d2_Radios[radio].sel + 1;
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1 && d2_HitWindow == textwin || kb_Last == 1) {
            ret = 0;
            sv_PopRequest = 1;
        }
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
        if ((flags & 0x80) && slider != -1)
            d2_SetObjValue(slider, 0, 0x1f4, d3_Time - t0);
        if ((flags & 4) && d3_Time > t0 + 0x1f4)
            sv_PopRequest = 1;
    }
    cm_MessageBoxOpen = 0;
    d2_RadioClear(radio);
    d2_CloseView(win);
    return ret;
}
/* 0x4e5730, registers the scene kind: the run function (+0), the callbacks of the members of the kind 6 (+4) and of the others (+8), the check (+0x10) and
   the flag +0xc (1: cm_RunSceneParty asks the members all at once, 0: one after the other) */

void cm_RegisterSceneKind(int kind, int run, int cb4, int cb8, int check, char flag)
{
    cm_Kinds[kind].run = run;
    cm_Kinds[kind].askPlayer = cb4;
    cm_Kinds[kind].askOther = cb8;
    cm_Kinds[kind].check = check;
    cm_Kinds[kind].allAtOnce = flag;
}
/* 0x4e5770, the init of the scene code: the party records are cleared (id -1) and the scene kinds are registered: 0 the picture show (0x4e0010), 1 the council
   votes (0x4d8e20), 2 the trial (0x4db700), 3 the fight (0x4d63e0), 4 the duel, 5 the execution, 6 the wedding, 7 the birth, 8 the death, 9 the bankruptcy, 10
   the auction (0x4e1a80), 11 the sale (0x4e2860) */

void cm_InitScenes(void)
{
    int i;

    memset(cm_Parties, 0, 0x6780);
    for (i = 0; i < 96; i++)
        cm_Parties[i].id = -1;
    cm_RegisterSceneKind(0, (int)cm_RunPictureShow, (int)d3io_NoopCallback, 0, 0, 0);
    cm_RegisterSceneKind(1, (int)cm_RunCouncilSession, (int)cm_SessionAskPlayer, (int)cm_SessionAskAI, 0, 0);
    cm_RegisterSceneKind(2, (int)cm_RunTrial, (int)cm_TrialAskPlayer, (int)cm_TrialAskAI, (int)cm_CheckTrial, 0);
    cm_RegisterSceneKind(3, (int)cm_RunFight, (int)cm_FightMember, 0, (int)cm_CheckFight, 1);
    cm_RegisterSceneKind(4, (int)cm_RunDuel, (int)cm_DuelAskPlayer, (int)cm_DuelAskAI, (int)cm_DuelAftermath, 0);
    cm_RegisterSceneKind(5, (int)cm_RunExecution, 0, 0, (int)cm_ExecutionAftermath, 0);
    cm_RegisterSceneKind(6, (int)cm_RunWedding, 0, 0, (int)cm_CheckWedding, 0);
    cm_RegisterSceneKind(7, (int)cm_RunBirth, 0, 0, (int)cm_CheckBirth, 0);
    cm_RegisterSceneKind(8, (int)cm_RunDeath, 0, 0, (int)cm_DeathSceneCheck, 0);
    cm_RegisterSceneKind(9, (int)cm_RunBankruptcy, 0, 0, 0, 0);
    cm_RegisterSceneKind(10, (int)cm_RunAuction, (int)cm_AuctionAskPlayer, (int)cm_AuctionAskAI, (int)cm_CheckAuction, 1);
    cm_RegisterSceneKind(11, (int)cm_RunSale, 0, 0, 0, 0);
    cm_ResetCutInfos(0);
}

/* unlinked: an empty function; LINK folds it (/OPT:ICF) into d3io_NoopCallback, 0x44ee40, where the original's
   calls of it in the scene runner (no push) point */
void cm_SceneNothing(void)
{
}
