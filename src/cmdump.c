/* cmdump: the functions 0x004e58e0..0x004e5cd0 of the original, split from cm
   unit 0x004e58e0..0x004e5cd0, named by guess; boundaries in decl/text.dcl */
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
#include "cmdump.h"
#include "d2.h"
#define cm_DuelChooseRoundR ((void (*)(CMCUTINFO *))cm_DuelChooseRound)  /* @original cm_DuelAskPlayer jumps to it with its own arguments in place (a tail jmp, byte-matched): called with r alone, the duel record stays on the stack */
#include "d3.h"
struct D3SKYLAYER; struct D3SKY; struct D3SKYLAYER *d3sky_CreateLayer(struct D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset, float vscale, float voffset);  /* 0x45d880 */
#include "d3_tools.h"
#include "d3_quad.h"
#include "d3_light.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "menu_types.h"
extern char cm_CutInfos[16][0x35c];
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];


#define G17_I(m, o) (*(int *)&(m).data[o])
#define G17_S(m, o) (*(short *)&(m).data[o])
/* the block at 0x936420 is 0x24 bytes: the count then the eight player states at +4 (one object, so the
   0x24-byte memset / memcpy in 0x4cd580 and 0x4cdxx cannot run over a neighbour that LINK placed there) */
#define cm_LobbyListIds (*(int (*)[8])((char *)cm_LobbyList + 4))  /* 0x936424, the eight player states (a view) */
#define cm_LobbyPlayers ((CMLOBBYPLAYER *)cm_LobbyRecords)  /* 0x9364e0, the eight lobby player records (the CMJOIN cm_JoinChar follows at +0x2c0) */
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
#define d3sm_PickK ((int (*)(D3SM *, int, int, int *, int *))d3sm_Pick)  /* 0x45f2b0 (d3_sm.c: char d3sm_Pick(D3SM *, int, int, int *, int *)): @original this unit's callers read an int result (byte-matched cm_UpdatePointer 0x4cf73a: and eax, 0xff after the call) */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests */
#define d3t_CreateEarthK ((int (*)())d3t_CreateEarth)  /* 0x4168e0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateSmokeK ((int (*)())d3t_CreateSmoke)  /* 0x416340 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateExplosionK ((int (*)())d3t_CreateExplosion)  /* 0x416040 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateBloodK ((int (*)(void *, int, int, int, int, int))d3t_CreateBlood)  /* 0x415e90 (d3_tools.c: D3PARTSYS *d3t_CreateBlood(void *, int, float, float, float, float)): @original this unit passes the four floats as their int bit patterns and does not use the result (byte-matched cm_DuelShotAction 0x4de767: push 0x425c0000 .. push 0x3e800000, no fstp pair) */
#define d3t_SetColorK ((int (*)())d3t_SetColor)  /* 0x413550 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_SetGreyK ((int (*)())d3t_SetGrey)  /* 0x4134a0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define d2_LastWindowPtrC (*(char **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) read bytewise, as this unit does */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define d2_SetObjValue2 ((void (*)(int, int))d2_SetObjValue)  /* 0x475cb0 (d2.h: five parameters): @original cm_UpdatePortraits pushes only the object and the value (0x4cfab2 push 1 / push ecx; 0x4cfacd push 0 / push edx, add esp, 8) */
#define d2_SetObjValue4 ((void (*)(int, int, int, int))d2_SetObjValue)  /* 0x475cb0 (d2.h: five parameters): @original cm_OpenCharInfoWin (0x4ced5e), cm_FightFrame (0x4d698e), cm_AuctionAskPlayer (0x4e25c5), cm_DialogTimer (0x4e5458) and cm_MessageBox (0x4e56af) push four arguments */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (cm.h: char *) as the AIGOOD records it points to */
#define cm_CutInfosT (*(CMCUTINFO (*)[16])&cm_CutInfos)  /* 0x92cae0, cm_CutInfos (char[16][0x35c]) as the cutscene records */
#define cm_LongMsgCut (*(CMCUTINFO *)&cm_LongMsg)  /* 0xa68d40, cm_LongMsg (cm.h: unsigned char[]) as the cutscene record message 0x26 joins */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (cm.h: char *) as the AIBTYPE records it points to */
#define ai_TimeCopyT (*(AITIME *)&ai_TimeCopy)  /* 0x75e130, ai_TimeCopy (cm.h: int[4]) as the AITIME copy it holds */
#define ai_PlayerSlotsT (*(GMPLAYERSLOT (*)[4])&ai_PlayerSlots)  /* 0x7e3f40, ai_PlayerSlots (cm.h: char[][0x2f4]) as the player slots */
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
#define cm_GrenadesT (*(CMGRENADE (*)[32])&cm_Grenades)  /* 0x935a00, cm_Grenades (char[512]) as the 32 records */
typedef struct CMBOMB {             /* 8 bytes, one of the 32 bombs of cm_Bombs */
    D3OBJECT *obj;                  /* the bomb object "ob_BOMBE", 0 = a free slot */
    unsigned long time;             /* d3_Time when it was laid */
} CMBOMB;
#define cm_BombsT (*(CMBOMB (*)[32])&cm_Bombs)  /* 0x935c00, cm_Bombs (sv.h: int[0x40]) as the 32 bomb records */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (cm.h: char[][0x21c]) as the AICHAR records they are */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define d3sm_GetVertexI ((int (*)(D3SM *, int, int, D3VECTOR *))d3sm_GetVertex)  /* 0x45f100 (d3_sm.c: char, D3SM *, int, int, D3VECTOR *): @original this unit's callers read an int result (byte-matched ord_RandomCellPoint (ord.c): no 0f be c0 after the call) */
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */
#define ai_DecideTortureP ((int (*)(AIPLAN *))ai_DecideTorture)  /* 0x4941b0 (am.h: void): @original cm_TrialAskAI pushes the plan (0x4de3a6 push esi, add esp, 4) */
#define ai_DecideTortureInstrumentP ((int (*)(AIPLAN *))ai_DecideTortureInstrument)  /* 0x4941d0 (am.h: void): @original cm_TrialAskAI pushes the plan (0x4de3c6 push esi, add esp, 4) */
#define cm_FightersT (*(AICHAR (*)[32])&cm_Fighters)  /* 0x930600, cm_Fighters (char[], with the 0x1000 bytes after the 32 records) as its AICHAR records */
#define ai_SceneCharsT (*(AICHAR *(*)[32])&ai_SceneChars)  /* 0x7603a0, ai_SceneChars (f3init.h: int[32]) as the AICHAR pointers it holds */
#define cm_FlagObjectsT (*(D3OBJECT *(*)[32])&cm_FlagObjects)  /* 0x935980, cm_FlagObjects (cm.h: char *[]) as the D3OBJECTs it holds */
#define ai_TrustedOfFourB ((int (*)(unsigned char, AICHAR *, int *, int *))ai_TrustedOfFour)  /* 0x4aad30 (poker.h: unsigned char k, AIPER *p; cm.h declares it without a prototype): the prototype these calls need (cm_AskElectVote 0x4dab50 pushes its unsigned char office raw, cm_SessionAskAI 0x4dae90 a byte: mov al / push eax) */
#define d2_LastWindowPtrT (*(struct D2WIN **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) as the window record */
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
typedef struct CMFIGHTREC {
    char pad000[0xc];
    int owner;                      /* 0x00c, the player whose fight it is (cm_FightOwner) */
    char pad010[4];
    int placeId;                    /* 0x014, the defender's building: its type picks the scenario, its role the defender voices */
    char pad018[0x18];
    unsigned char nChars;           /* 0x030, the count of charIds */
    char pad031[3];
    union {
        struct {
            int attackerId;         /* 0x034, the attacking character (cm_FightAttacker) */
            int defenderId;         /* 0x038, the defending character (cm_FightDefender) */
        };
        int charIds[16];            /* 0x034, the netIds of the fight's leading characters, -1 = none (cm_CutInfos[i]
                                       is the cutscene record of charIds[i]) */
    };
    char pad074[8];
    unsigned char flags;            /* 0x07c, 1 in the open (8: in the town, else the mountain pass), 4 the mountain pass */
    char pad07d[3];
    int defenders[16];              /* 0x080, the netIds of the defending fighters, -1 = none */
    int attackers[16];              /* 0x0c0, the netIds of the attacking fighters, -1 = none */
    int carts[3];                   /* 0x100, the object ids of the carts in the fight, -1 = none */
    int attackerBldId;              /* 0x10c, the attacker's building: its role picks the attacker voices */
    int escort;                     /* 0x110, the carts' escort: the arming kind ai_ArmGateGuards keeps (0 none, 1 .. 3);
                                       the AI side of a won raid takes the loot when random 0 .. 4 is above it */
} CMFIGHTREC;

char ai_CharKindNames[10][24] = { "DUMMY_NPC", "MEISTER_NPC", "POLITICIAN_NPC", "AI_NPC", "NPC", "PLAYER", "REMOTE_PLAYER", "RELATIVE_PLAYER", "USED_PLAYER", "INVISIBLE_NPC" };  /* 0x60eae0, the names of the NPC kinds */

/* 0x4e58e0, the debug dump of a person (log of the cutscenes): one line of tab separated fields (time, +4, id, the name of the kind (0x60eae0, 24 bytes each) ...) */

void ai_DumpPersonHead(char *c, char *buf)
{
    AICHAR *p;
    char *name;

    p = (AICHAR *)c;
    if (p->kind >= 10)
        name = "%i\t%i\t%i\t%s\t%s\t%i\t%s\t%i\t%s\t%s\t%s\t%s\t";
    else
        name = ai_CharKindNames[p->kind];
    sprintf(buf, "%i\t%i\t%i\t%s\t%s\t%i\t%s\t%i\t%s\t%s\t%s\t%s\t", ai_Time.year, p->netId, p->id, name, p->name, p->health,
            txt_SexNames[p->sex], p->w00a, txt_FaithNames[p->b00c], txt_HonourTitles[p->b00d], txt_JobNames[p->jobId],
            txt_TitleNames[(unsigned char)p->title]);
}
/* 0x4e5980, the debug dump of the floats +0x10 .. +0x20 (the 6th "%5.2f" gets the int +0x24 as the original does), the word +0x28 and the bit fields of +0x2c */

void ai_DumpPersonStats(char *c, char *buf)
{
    AICHAR *p;
    unsigned int v;

    p = (AICHAR *)c;
    sprintf(buf, "%5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %i\t", p->f010, p->f014, p->f018, p->f01c, p->f020, p->v024, p->w028);
    v = p->v02c;
    sprintf(buf + strlen(buf), "%i\t %i\t %i\t %i\t %i\t %i\t %i\t %i\t %i\t", v & 0xf, (v >> 4) & 0xf, (v >> 8) & 0xf, (v >> 0xc) & 3, (v >> 0xe) & 7, (v >> 0x11) & 7,
            (v >> 0x14) & 7, (v >> 0x17) & 3, (v >> 0x19) & 0xf);
}
/* 0x4e5a40, the five bytes +0x80 .. +0x84 of a person as a tab separated line */

void ai_DumpPersonSkills(char *c, char *buf)
{
    AICHAR *p;

    p = (AICHAR *)c;
    sprintf(buf, "%i\t%i\t%i\t%i\t%i\t", p->skills[0], p->skills[1], p->skills[2], p->skills[3], p->skills[4]);
}
/* 0x4e5a90, the four names (0x143ab0c) of the bytes +0x166 .. +0x169 of a person */

void ai_DumpPersonOffices(char *c, char *buf)
{
    AICHAR *p;

    p = (AICHAR *)c;
    sprintf(buf, "%s\t%s\t%s\t%s\t", txt_OfficeNames[p->officeKind], txt_OfficeNames[p->prevOffice], txt_OfficeNames[p->appliedOffice],
            txt_OfficeNames[p->secondOffice]);
}
/* 0x4e5af0, the debug dump of a person's money fields (v190, money, spent198, wage, v1a0, v1a4), then its goods value (0x509e60) and its worth (0x50a640) */

void ai_DumpPersonMoney(char *c, char *buf)
{
    AICHAR *p;

    p = (AICHAR *)c;
    sprintf(buf, "%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t", p->v190, p->money, p->spent198, p->wage, p->v1a0, p->v1a4, gm_GetGoodsValue(c), gm_GetWorth(p->id));
}
/* 0x4e5b50, the debug dump of the 8 relatives of a person (the ids at +0x5c): "%i\t%s" of each one's netId and name, -1 and 0x143b3f4 (txt_NoRelative) when it does not exist */

void ai_DumpPersonRelatives(char *c, char *buf)
{
    char tmp[0x100];
    int *p;
    AICHAR *o;
    int i;

    p = ((AICHAR *)c)->relatives;
    for (i = 8; i != 0; i--) {
        o = (AICHAR *)gm_FindChar(*p);
        if (o != 0)
            sprintf(tmp, "%i\t%s", o->netId, o->name);
        else
            sprintf(tmp, "%i\t%s", -1, txt_NoRelative);
        strcat(buf, tmp);
        p++;
    }
}
/* 0x4e5bf0, the debug dump of a person (one line made by 0x4e58e0, 0x4e5980, 0x4e5a40, 0x4e5a90, 0x4e5af0 and 0x4e5b50 in a 0x1fb8 byte buffer; the end of the cut_ code) */

void ai_DebugDumpPerson(char *c)
{
    char buf[0x1fb8];
    AICHAR *p;

    p = (AICHAR *)c;
    if (p != 0 && p->id != 0xffff) {
        memset(buf, 0, sizeof(buf));
        ai_DumpPersonHead(c, buf + strlen(buf));
        ai_DumpPersonStats(c, buf + strlen(buf));
        ai_DumpPersonSkills(c, buf + strlen(buf));
        ai_DumpPersonOffices(c, buf + strlen(buf));
        ai_DumpPersonMoney(c, buf + strlen(buf));
        ai_DumpPersonRelatives(c, buf + strlen(buf));
    }
}
