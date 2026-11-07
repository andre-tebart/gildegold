/* cm.c -- the game messages exchanged with the server (net.c): the size of a message by its type, the send
   ring cm_SendRing (0x8000 slots) and its states, the receive list cm_RecvQueue (0x2000 records), long messages split
   into parts of type 7, the field blocks of an object (delta / plain values). Its .data block starts at 0x60bd80 after
   f3snd.c's literals (0x302, 2, -1 globals, the handler table 0x60bd98 of 0x4c6a20..). Tag "cm_ExecCommand" (0x4c4190,
   its texts at 0x60c0e8..). The ~0x6a message handlers and their helpers follow, then the fight code (0x4cf670..)
   and the cutscenes (cut_, 0x4d8c70..: trial, duel, execution, wedding, auction, ...; joined: the strings run
   monotonic 0x60cb58..0x60ec88 and the ints 0x60d360.. serve both halves). Ends at 0x4e5bf0 (amt.c follows).
   Names are ours.
   The message handlers read their data through the G17MSG union (cm_types.h), one struct per message type (the
   party, position and path messages keep their structs in this file's head; message 0x1c is the exception: its
   data is a bare nextEventId, the long message itself is in cm_LongMsg); the amt and
   gv payloads (AMTAPP, AMTMSG, GVDECMSG) are their types headers' (amt_types.h, gv_types.h), which cm_types.h includes
   for the union. The scene
   callbacks registered by cm_InitScenes (cm_RunDuel, cm_RunExecution, cm_RunWedding, cm_RunBirth, cm_RunDeath,
   cm_RunBankruptcy, cm_RunAuction, their checks and member callbacks) read the party record G28REC; the auction's
   member callbacks (cm_AuctionAskPlayer / cm_AuctionAskAI) read its state CMAUCTION. The spy report objects (AIOBJ
   kind 0xca) are read as CMSPYREPORT (defined above cm_ExSpyReport: the report's words 0x1c .. 0x37, HESPYREPORT).
   Open: not included yet (dcp headers conflicts cm HEADER): am.h anm.h d2.h f3gm.h gfx.h hmt.h hst.h rt.h
   poker.h mission.h menu.h; held by calls whose argument count differs from the definition (read the
   original's pushes): ai_DecideTorture, ai_DecideTortureInstrument, d2_SetObjValue, gm_SkillPoints (rt.h: its d2_SetObjValue);
   gv_PassSentence takes five arguments here (gv_PassSentenceK).
   Open: head: cm.h declares this unit's cutscene helpers (cm_QueueVoice, cm_LoadVoiceBank, cm_StartEventArg, ..) and
   cm_PartyById / cm_ClearParty without parameters (their real prototypes drop ai_TutorialInitChapter2 byte -> shape and
   ai_TutorialInitChapter3 shape -> draft); cm_QueueVoice has callers with four arguments, and declaration changes in
   cm.h / cm_types.h flip the register tie-breaks of ai_TutorialInitChapter2 / 3 (sv_t, tie-break) and of
   ord_CountGridObjects. Types only
   cm.c uses (CMNOTE, CMCUTTYPEMSG, CMFIELD, CMWALKPATHMSG) stay in its head for that reason. Meanwhile cm.h's own declarations of ord_AddGridAmount,
   gm_RemoveOwned, gm_BuildingSkill, gm_HealthRatio, rt_FreePoints and rt_CopyPoints are the owner headers' prototypes
   (gv.h, f3gm.h, rt.h; RTPOINT from rt_types.h); the other calls into those headers stay M7.
   Open: head: f3gm.h has no prototype of gm_ShowBuildingObjects (cm_ExSkillsChanged calls it undeclared, the f3gm.h
   include above is blocked); message 0x3f has no record type in cm_types.h (a struct typedef there flips
   ord_CountGridObjects shape -> draft, a declaration-set tie-break the probe does not reproduce), so 0x3f / 0x40
   read the player / item head of the goods record through G17GOODSMSG.goods.
   The fight scene (cm_SetupFightScene .. cm_MostThreatenedCellNear) reads its fight record as CMFIGHTREC (a partial
   layout, defined in this file; cm_FightRecordT views cm_FightRecord as it).
   Open: types: CMFIGHTSLOT and CMCUTENTRY (cm_types.h) are one 0x2c-byte record in two views (a cutscene entry is sent
   as the fight command's slot, cm_RunCutOrders casts between them); one type for both is unsettled.
   Open: layout: the fight code 0x4cf670.. may be a TU of its own (cm_UpdatePointer reloads ai_PickedObject after
   calls of ai_LedSelectBits, a leaf defined above in this unit, as if it were external).
   unit of rea 8cef7d5b (src/game/cm.c); boundaries in decl/text.dcl */
#include "d2_types.h"
extern D2WIN d2_Wins[];
extern D2RADIO d2_Radios[];
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "net.h"
#include "cm.h"
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
extern OPTIONBYTES ai_GfxFloor;
extern OPTIONBYTES ai_GfxFog;  /* 0x6f35cc, menu.c: the fog option byte */
extern char cm_CutInfos[16][0x35c];
extern char gm_TagTmp[4];  /* 0x60c1ec, f3gm.c: the "tmp" allocation tag (mem_Alloc; f3gm.h is not included here) */
#define GAPDATA __declspec(align(1)) unsigned char

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern GAPDATA full_data_930370[];
extern int gm_PlayerRecords[688];
extern GAPDATA full_data_602568[];
extern GAPDATA full_data_60d5a4[];
extern GAPDATA full_data_60d5b4[];
extern GAPDATA full_data_92bcb8[];


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
#include "he.h"
#include "amt.h"
#include "am_types.h"
#include "gv.h"
#define d3sm_PickK ((int (*)())d3sm_Pick)  /* 0x45f2b0 (d3_sm.c): @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests */
#define d3t_CreateEarthK ((int (*)())d3t_CreateEarth)  /* 0x4168e0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateSmokeK ((int (*)())d3t_CreateSmoke)  /* 0x416340 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateExplosionK ((int (*)())d3t_CreateExplosion)  /* 0x416040 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define gm_AssignBuildingK ((int (*)())gm_AssignBuilding)  /* 0x502b60 (f3gm.c): @original cm_ExOwnership (0x4caaa0, byte) passes the two shorts zero-extended without a prototype (xor ecx / xor edx, mov cx) */
#define d3t_CreateBloodK ((int (*)())d3t_CreateBlood)  /* 0x415e90 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_SetColorK ((int (*)())d3t_SetColor)  /* 0x413550 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_SetGreyK ((int (*)())d3t_SetGrey)  /* 0x4134a0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define d2_LastWindowPtrC (*(char **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) read bytewise, as this unit does */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
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
#define d3sm_GetVertexI ((int (*)())d3sm_GetVertex)  /* 0x45f100 (d3_sm.c: char, D3SM *, int, int, D3VECTOR *): @original this unit's callers read an int result (byte-matched ord_RandomCellPoint (ord.c): no 0f be c0 after the call) */
#define ch_CreateK ((struct CHARACTER *(*)())ch_Create)  /* 0x403390 (ch.h: name, pos): @original cm_CreateChar (byte) passes its own arguments on untouched (jmp ch_Create): a call without a prototype */
#define ch_AddActionCBK ((struct CHACTION *(*)())ch_AddActionCB)  /* 0x40b400 (ch.h): @original cm.c's calls were compiled without a prototype (default promotions) */
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */
#define ahm_CreateAlchemistI ((int (*)(int *))ahm_CreateAlchemist)  /* 0x4ec500 (ahm.h: unsigned short): @original the caller 0x4ccf90 reads the id as an int (no and eax,0xffff) */
#define cm_FightersT (*(AICHAR (*)[32])&cm_Fighters)  /* 0x930600, cm_Fighters (char[], with the 0x1000 bytes after the 32 records) as its AICHAR records */
#define ai_SceneCharsT (*(AICHAR *(*)[32])&ai_SceneChars)  /* 0x7603a0, ai_SceneChars (f3init.h: int[32]) as the AICHAR pointers it holds */
#define cm_FlagObjectsT (*(D3OBJECT *(*)[32])&cm_FlagObjects)  /* 0x935980, cm_FlagObjects (cm.h: char *[]) as the D3OBJECTs it holds */
#define ai_TrustedOfFourB ((int (*)(unsigned char, AICHAR *, int *, int *))ai_TrustedOfFour)  /* 0x4aad30 (poker.h: int k): @original cm_AskElectVote (0x4dab50, byte) pushes its unsigned char office raw (push ebp), cm_SessionAskAI (0x4dae90) a byte (mov al / push eax) */
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
/* 0x4c3180, the size in bytes of the game message m by its type: fixed per type, the lists 0x16 / 0x17 a 0x15-byte
   head and data[4] records of w * h + 4 bytes, 0x18 a head and data[4] records of 5, 0x20 by its kind byte data[0];
   0x91 (the whole G17MSG body) for any other type
   @deep Read whole against the original: the 42 case bodies, their values and their order (the 0x18 group
   after case 0x22, the 0x28 group and 0x5f after 0x61/0x62, as the original's block layout), the list loop
   of 0x16/0x17 (int count, jle) and 0x18 / 0x20 all match; the decoded jump table maps every type 3..0x69
   to the same value as ours. Left: VC6 lowers our switch through a byte index table (xor edx,edx; mov
   dl,[ecx+X]; jmp [edx*4+X]) where the original jumps directly through a 0x67-entry table (mov dl,[eax];
   mov ecx,edx; and ecx,0xff; ... jmp [ecx*4+X]), and the original's dead 'cmp dl, 0x16' in the 0x16/0x17
   body. Tried: explicit labels for the default types (with default:, with their own return), one case per
   type (no grouping), the type in an unsigned char local, ternaries on t == 0x16 in the count / pointer /
   head size: the table form never changed. A variant round (switch-lowering heuristic). */

int cm_MessageSize(G17MSG *m)
{
    unsigned char *p;
    int n, k;
    int i;
    unsigned char t;

    t = m->type;
    switch (t) {
    case 3:
    case 0x13:
    case 0x1a:
    case 0x25:
    case 0x2b:
    case 0x34:
    case 0x36:
    case 0x38:
    case 0x47:
    case 0x52:
    case 0x54:
    case 0x5b:
    case 0x5d:
        return 0x1c;
    case 4:
        return 0x11;
    case 8:
    case 0x1c:
    case 0x28:
    case 0x31:
    case 0x3b:
    case 0x42:
    case 0x43:
    case 0x4a:
    case 0x4d:
    case 0x57:
    case 0x58:
    case 0x69:
        return 0x14;
    case 10:
        return 0x50;
    case 0xb:
        return 0x2a;
    case 0xc:
        return 0x49;
    case 0xf:
    case 0x2c:
        return 0x21;
    case 0x11:
    case 0x3d:
        return 0x27;
    case 0x12:
    case 0x1e:
        return 0x1e;
    case 0x14:
        return 0x16;
    case 0x15:
        return 0x39;
    case 0x16:
    case 0x17:
        n = 0x15;
        p = m->fields.buf;
        for (i = m->fields.n; i > 0; i--) {
            k = p[1] * p[0] + 4;
            n += k;
            p += k;
        }
        return n;
    case 0x18:
        return m->data[4] * 5 + 0x15;
    case 0x19:
    case 0x53:
        return 0x24;
    case 0x1d:
        return 0x5f;
    case 0x20:
        return m->data[0] != 0x0e ? 0x8d : 0x11;
    case 0x22:
        return 0x3d;
    case 0xd:
    case 0x21:
    case 0x23:
    case 0x24:
    case 0x26:
    case 0x27:
    case 0x29:
    case 0x2a:
    case 0x33:
    case 0x39:
    case 0x3a:
    case 0x5a:
    case 0x66:
    case 0x68:
        return 0x18;
    case 0x2d:
    case 0x45:
        return 0x1f;
    case 0x2e:
    case 0x3c:
        return 0x20;
    case 0x2f:
    case 0x30:
        return 0x3c;
    case 0x32:
        return 0x30;
    case 0x35:
        return 0x5d;
    case 0x37:
        return 0x45;
    case 0x3e:
    case 0x4f:
    case 0x5e:
    case 99:
        return 0x34;
    case 0x40:
    case 0x60:
        return 0x2f;
    case 0x41:
        return 0x4a;
    case 0x44:
        return 0x17;
    case 0x46:
        return 0x19;
    case 0x48:
        return 0x15;
    case 0x49:
        return 0x36;
    case 0x4b:
    case 0x67:
        return 0x90;
    case 0x4c:
        return 0x35;
    case 0x4e:
        return 0x40;
    case 0x50:
    case 0x51:
        return 0x44;
    case 0x55:
        return 0x61;
    case 0x56:
        return 0x38;
    case 0x5c:
        return 0x1a;
    case 0x61:
    case 0x62:
        return 0x29;
    case 0x1b:
    case 100:
        return 0x28;
    case 0x5f:
        return 0x80;
    }
    return 0x91;
}
/* 0x4c3480, init of the message layer: chains the 0x2000 receive records into the free list, empties the queues,
   sets the 0x8000 send states to idle, points the net receive at cm_RecvMsg and says hello to the server
   (cm_SendHello / cm_WaitHello; the net is closed when no answer comes) */

void ai_InitStates(void)
{
    int i;

    cm_LeftPlayerId = -1;
    for (i = 0; i < 0x2000; i++) {
        cm_RecvPool[i].type = 0;
        if (i == 0)
            cm_RecvPool[i].prev = 0;
        else
            cm_RecvPool[i].prev = &cm_RecvPool[i - 1];
        if (i < 0x1fff)
            cm_RecvPool[i].next = &cm_RecvPool[i + 1];
        else
            cm_RecvPool[i].next = 0;
    }
    cm_RecvFree = cm_RecvPool;
    cm_RecvQueue = 0;
    cm_RecvMsg.type = 0;
    cm_LastSeq = 0;
    cm_SendQueue = 0;
    for (i = 0; i < 0x8000; i++) {
        cm_SendStates[i].idx = -1;
        cm_SendStates[i].state = 1;
    }
    net_Conn.rx = (NETPKT *)&cm_RecvMsg;
    cm_SendHello();
    if (cm_WaitHello() != 0)
        net_Close();
    cm_NextNetStep = d3_Time;
}
/* 0x4c3560, resets the message layer as ai_InitStates does, without the hello to the server: the receive records
   back on the free list, the queues empty, the send states idle */

void ai_ResetMessages(void)
{
    int i;

    for (i = 0; i < 0x2000; i++) {
        cm_RecvPool[i].type = 0;
        if (i == 0)
            cm_RecvPool[i].prev = 0;
        else
            cm_RecvPool[i].prev = &cm_RecvPool[i - 1];
        if (i < 0x1fff)
            cm_RecvPool[i].next = &cm_RecvPool[i + 1];
        else
            cm_RecvPool[i].next = 0;
    }
    cm_RecvFree = cm_RecvPool;
    cm_RecvQueue = 0;
    cm_RecvMsg.type = 0;
    cm_LastSeq = 0;
    cm_SendQueue = 0;
    for (i = 0; i < 0x8000; i++) {
        cm_SendStates[i].idx = -1;
        cm_SendStates[i].state = 1;
    }
}
/* 0x4c3600, sends message 3 (0xa03ef57c, 0) */

void cm_SendHello(void)
{
    G17MSG m;

    m.type = 3;
    m.hello.magic = 0xa03ef57c;
    m.hello.lobbyId = 0;
    cm_QueueSend(&m);
}
/* 0x4c3630, waits for the answer to 0x4c3600; 1: none */

int cm_WaitHello(void)
{
    G17MSG *m;

    m = cm_WaitMessage(3, 0x165);
    if (m == 0)
        return 1;
    if (m->hello.magic != 0xa03ef57c)
        return 1;
    cm_LobbyStart(m->hello.lobbyId, m->hello.value);
    cm_FreeReceived(m);
    return 0;
}
/* 0x4c3680, sends the queued messages one by one while the connection takes them (0 when the send is still busy);
   offline (no socket) they go straight into the receive list (cm_EnqueueReceived), 1: no free record */

int cm_FlushSendQueue(void)
{
    if (cm_SendQueue != 0) {
        if (net_Conn.s == INVALID_SOCKET) {
            while (1) {
                if (cm_EnqueueReceived(cm_SendQueue) != 0)
                    return 1;
                cm_SendQueue = cm_SendQueue->next;
                if (cm_SendQueue == 0)
                    break;
                cm_SendQueue->prev = 0;
            }
        } else {
            do {
                if (net_Conn.tx != 0) {
                    net_Send();
                    if (net_Conn.tx != 0)
                        return 0;
                }
                net_Conn.tx = (NETPKT *)cm_SendQueue;
                cm_SendQueue = cm_SendQueue->next;
                if (cm_SendQueue != 0)
                    cm_SendQueue->prev = 0;
                net_Send();
            } while (cm_SendQueue != 0);
        }
    }
    return 0;
}
/* 0x4c3720, queues cm_SplitMsg as parts of type 7 (0x80 bytes each); returns the seq of the first part */

int cm_QueueSplit(void)
{
    G17MSG *m, *p, *last;
    unsigned int i;
    int first, off;

    if (cm_SplitMsgLen == 0)
        return 0;
    if (net_Conn.closed != 0)
        return 0;
    last = 0;
    first = cm_LastSeq + 1;
    for (off = 0; off < cm_SplitMsgLen; off += 0x80) {
        cm_LastSeq++;
        i = cm_LastSeq & 0x7fff;
        m = &cm_SendRing[i];
        m->type = 7;
        m->size = cm_MessageSize(m);
        memcpy(m->data, (unsigned char *)&cm_SplitMsg + off, 0x80);
        m->sender = 0;
        m->idx = i;
        m->seq = cm_LastSeq;
        m->next7 = 0;
        if (last != 0)
            last->next7 = cm_LastSeq;
        cm_SendStates[i].state = 0;
        cm_SendStates[i].idx = i;
        if (cm_SendQueue == 0) {
            cm_SendQueue = m;
            m->prev = 0;
        } else {
            for (p = cm_SendQueue; p->next != 0; p = p->next)
                ;
            m->prev = p;
            p->next = m;
        }
        m->next = 0;
        last = m;
    }
    cm_SplitMsgLen = 0;
    return first;
}
/* 0x4c3850, 1 when every part of the split message m is in the receive queue (or m is a part itself / not split):
   follows the next7 chain from the first part, which holds the whole length, 0x7e bytes in it and 0x80 in each
   further part */

int cm_SplitComplete(G17MSG *m)
{
    G17MSG *r, *p, *q;
    int n;

    if (m->type == 7 || m->next7 == 0)
        return 1;
    for (r = cm_RecvQueue; r != 0; r = r->next) {
        if (r->sender == m->sender && r->seq == m->next7) {
            for (n = 0x7e, p = r; n < r->splitLen; n += 0x80) {
                for (q = cm_RecvQueue; q != 0; q = q->next)
                    if (q->sender == p->sender && q->seq == p->next7)
                        break;
                if (q == 0)
                    return 0;
                p = q;
            }
            return 1;
        }
    }
    return 0;
}
/* 0x4c38e0, joins the parts of the split message m into cm_LongMsg and frees them; returns the length
   @deep read against the original in full: meaning matches (both search loops as
   for(r=cm_RecvQueue;r;r=r->next) if(match) break; if(!r) return 0; p->next7 cleared after the part copy).
   Tried: seq local, while forms, combined early return, store orders. Left: allocation only, m in ecx and
   next7 in esi where the original has m in esi, next7 in ecx (slots 16 / regs 8 hunks), and the code hunk
   at 0x4c391f '- mov [ecx+0xc],ebx; xor ecx,ecx | + xor ecx,ecx; mov [esi+0xc],ebx' (the same store under
   that renaming, scheduled one later), order 10 at 0x4c398c */

unsigned int cm_JoinSplit(G17MSG *m)
{
    G17MSG *r, *p;
    int n;

    if (m->next7 == 0)
        return 0;
    for (r = cm_RecvQueue; r != 0; r = r->next)
        if (r->sender == m->sender && r->seq == m->next7)
            break;
    if (r == 0)
        return 0;
    m->next7 = 0;
    cm_LongMsgLen = r->splitLen;
    memcpy(cm_LongMsg, r->data + 2, 0x7e);
    r->type = 0;
    for (n = 0x7e, p = r; n < (int)cm_LongMsgLen; n += 0x80) {
        for (r = cm_RecvQueue; r != 0; r = r->next)
            if (r->sender == p->sender && r->seq == p->next7)
                break;
        if (r == 0)
            return 0;
        memcpy(cm_LongMsg + n, r->data, 0x80);
        p->next7 = 0;
        r->type = 0;
        p = r;
    }
    r->next7 = 0;
    return cm_LongMsgLen;
}
/* 0x4c39c0, queues a copy of src in the send ring; returns its slot, -1: closed or the slot is still in use
   @deep read in full against the original: the append was if/else inverted with a while walk from p =
   cm_SendQueue (fixed; now 91/91 instructions, all else equal). Tried m = cm_SendRing + i (no change).
   Left: scheduling only at +0x19: ours loads net_Conn.s into eax before computing the slot address in ebp
   ('- mov eax,[net_Conn]; mov ebp,ebx; shl ebp,4; add ebp,ebx; ... lea ebp,[ebp+ebp*8+cm_SendRing]'), the
   original computes it in eax first then loads net_Conn.s ('+ mov eax,ebx; shl eax,4; add eax,ebx; lea
   ebp,[eax+eax*8+cm_SendRing]; mov eax,[net_Conn]'): the same 6 instructions, another order and temp
   register */

int cm_QueueSend(G17MSG *src)
{
    G17MSG *m, *p;
    unsigned int i;

    if (net_Conn.closed != 0)
        return -1;
    i = (cm_LastSeq + 1) & 0x7fff;
    m = &cm_SendRing[i];
    if (net_Conn.s != INVALID_SOCKET && (G17MSG *)net_Conn.tx == m)
        return -1;
    for (p = cm_SendQueue; p != 0; p = p->next)
        if (p == m)
            return -1;
    cm_LastSeq++;
    *m = *src;
    m->size = cm_MessageSize(m);
    m->sender = 0;
    m->idx = i;
    m->seq = cm_LastSeq;
    m->next7 = 0;
    if (src->type == 0x20 && src->data[0] == 0x0e)
        cm_LastSyncCount = m->seq;
    cm_SendStates[i].state = 0;
    cm_SendStates[i].idx = i;
    p = cm_SendQueue;
    if (p != 0) {
        while (p->next != 0)
            p = p->next;
        m->prev = p;
        p->next = m;
    } else {
        cm_SendQueue = m;
        m->prev = 0;
    }
    m->next = 0;
    if (cm_SplitMsgLen != 0)
        m->next7 = cm_QueueSplit();
    return i;
}
/* 0x4c3ae0, the state of slot h, -1: no slot */

int cm_GetSlotState(int h)
{
    if (h >= 0 && h < 0x8000)
        return cm_SendStates[h].state;
    return -1;
}
/* 0x4c3b10, the value of slot h */

int cm_GetSlotValue(int h)
{
    if (h >= 0 && h < 0x8000)
        return cm_SendStates[h].value;
    return 0;
}
/* 0x4c3b30, marks the start of a group of messages */

void cm_BeginSendGroup(void)
{
    cm_GroupFirstSeq = cm_LastSeq + 1;
}
/* 0x4c3b40, marks its end */

void cm_EndSendGroup(void)
{
    cm_GroupEndSeq = cm_LastSeq + 1;
}
/* 0x4c3b50, the state of the group: 1 done, 0 pending, -1 one failed */

int cm_SendGroupState(void)
{
    int ret;
    unsigned int s;

    ret = 1;
    if (cm_GroupFirstSeq == cm_GroupEndSeq)
        return ret;
    for (s = cm_GroupFirstSeq; s < cm_GroupEndSeq; s++) {
        if (cm_SendStates[s & 0x7fff].state == 0)
            return 0;
        if (cm_SendStates[s & 0x7fff].state == 2)
            ret = -1;
    }
    return ret;
}
/* 0x4c3ba0, starts a field block about object o (id) */

void cm_BeginFieldBlock(char *o, int id)
{
    int a, b;

    cm_FieldBlock.id = id;
    cm_FieldBlock.n = 0;
    memset(cm_FieldBlock.buf, 0, sizeof(cm_FieldBlock.buf));
    cm_FieldBlockLen = 0;
    if (gm_FindById(&a, &b, &id, id) != 0)
        cm_FieldObject = o;
}
/* 0x4c3c00, adds n values of size bytes at field as differences to the object's values
   @deep read in full against the original: fixed the signature (size, n unsigned char, field unsigned
   short: cmp cl / and 0xff / and 0xffff; all declarations updated, no grade fell), the bound (len >= 0x77,
   jb), the field offset read once (cm_FieldObject + field before the switch: the dec/dec/sub-2 switch on
   size). 101 vs 100 instructions, the head now equal but for the esi/edi push order. Tried: 3 typed
   pointers (counter i, i = n down, n itself), mixed pointer/index, char pointers with casts: all keep an
   extra register (ebp) and worse. Left: the copy loops' induction form: ours (indexed) '- sub ecx,edx; sub
   eax,edx; mov ebx,[ecx+edx]; mov [eax+edx],esi' where the original walks three pointers '+ mov ebx,[ecx];
   add ecx,4; mov [eax],esi; add eax,4' (cases 1, 2, 4), and edi/esi swapped */

int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    unsigned char *p, *b;
    unsigned int i, len;

    if (size != 1 && size != 2 && size != 4)
        return 1;
    len = n * size + cm_FieldBlockLen + 4;
    if (len >= 0x77)
        return 1;
    p = &cm_FieldBlock.buf[cm_FieldBlockLen];
    cm_FieldBlock.n++;
    cm_FieldBlockLen = len;
    *p++ = size;
    *p++ = n;
    *(unsigned short *)p = field;
    p += 2;
    b = (unsigned char *)cm_FieldObject + field;
    switch (size) {
    case 1:
        for (i = 0; i < n; i++)
            p[i] = ((unsigned char *)v)[i] - b[i];
        break;
    case 2:
        for (i = 0; i < n; i++)
            ((short *)p)[i] = ((short *)v)[i] - ((short *)b)[i];
        break;
    case 4:
        for (i = 0; i < n; i++)
            ((int *)p)[i] = ((int *)v)[i] - ((int *)b)[i];
        break;
    }
    return 0;
}
/* 0x4c3d00, adds n values of size bytes at field (a plain copy) */

int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    union { unsigned char *b; unsigned short *w; } p;    /* the entry: size, n (bytes), field (a word) */
    unsigned int k, len;

    if (size != 1 && size != 2 && size != 4)
        return 1;
    k = size * n;
    len = k + 4 + cm_FieldBlockLen;
    if (len >= 0x77)
        return 1;
    p.b = &cm_FieldBlock.buf[cm_FieldBlockLen];
    cm_FieldBlockLen = len;
    cm_FieldBlock.n++;
    *p.b++ = size;
    *p.b++ = n;
    *p.w = field;
    memcpy(p.b + 2, v, k);
    return 0;
}
/* 0x4c3d90, as 0x4c3d00, value by value */

int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    union { unsigned char *b; unsigned short *w; short *s; int *i; } p;    /* the entry: size, n, field, the values */
    unsigned int i, len;

    if (size != 1 && size != 2 && size != 4)
        return 1;
    len = n * size + cm_FieldBlockLen + 4;
    if (len >= 0x77)
        return 1;
    p.b = &cm_FieldBlock.buf[cm_FieldBlockLen];
    cm_FieldBlock.n++;
    cm_FieldBlockLen = len;
    *p.b++ = size;
    *p.b++ = n;
    *p.w++ = field;
    switch (size) {
    case 1: {
        unsigned char *s = (unsigned char *)v;
        for (i = 0; i < n; i++)
            *p.b++ = *s++;
        break;
    }
    case 2: {
        short *s = (short *)v;
        for (i = 0; i < n; i++)
            *p.s++ = *s++;
        break;
    }
    case 4: {
        int *s = (int *)v;
        for (i = 0; i < n; i++)
            *p.i++ = *s++;
        break;
    }
    }
    return 0;
}
/* 0x4c3e70, starts the second field block about the character p (its netId) */

void ai_BeginNeedBlock(struct AICHAR *p)
{
    ai_NeedBlock.id = p->netId;
    ai_NeedBlock.n = 0;
    memset(ai_NeedBlock.buf, 0, sizeof(ai_NeedBlock.buf));
    ai_NeedBlockLen = 0;
    ai_NeedBlockChar = p;
}
/* 0x4c3eb0, adds the pair (k, v) to the second field block */

int ai_AddNeedDelta(int k, float v)
{
    union { unsigned char *b; float *f; } p;    /* the pair: the kind (a byte), the delta */
    unsigned int len;

    len = ai_NeedBlockLen + 5;
    if (len >= 0x77)
        return 1;
    p.b = &ai_NeedBlock.buf[ai_NeedBlockLen];
    ai_NeedBlockLen = len;
    ai_NeedBlock.n++;
    *p.b++ = k;
    *p.f = v;
    return 0;
}
/* 0x4c3ef0, sets cm_SpeedLevel (0..4) through message 0x12 */

void cm_SetSpeedLevel(unsigned int v)
{
    int buf[31];

    if (v >= 4)
        v = 4;
    if (v != cm_SpeedLevel) {
        buf[0] = v;
        cm_SendExSysMessage(0x12, buf);
    }
}
/* 0x4c3f20, raises cm_SpeedLevel */

void cm_RaiseSpeedLevel(void)
{
    int buf[31];

    if (cm_SpeedLevel < 4) {
        buf[0] = cm_SpeedLevel + 1;
        cm_SendExSysMessage(0x12, buf);
    }
}
/* 0x4c3f50, lowers cm_SpeedLevel */

void cm_LowerSpeedLevel(void)
{
    int buf[31];

    if (cm_SpeedLevel > 0) {
        buf[0] = cm_SpeedLevel - 1;
        cm_SendExSysMessage(0x12, buf);
    }
}
/* 0x4c3f80, returns the speed level cm_SpeedLevel (unsigned: he_MoveReaper converts it as one) */

unsigned int cm_GetSpeedLevel(void)
{
    return cm_SpeedLevel;
}
/* 0x4c3f90, message 0x13 with the player's id */

int cm_SendPlayerId(void)
{
    int buf[31];

    buf[0] = ai_CharInts[ai_PlayerId][0];
    cm_SendExSysMessage(0x13, buf);
}
/* 0x4c3fd0, receives the waiting messages into the receive list; 1: no free record */

int cm_ReceiveMessages(void)
{
    if (net_Conn.s != INVALID_SOCKET) {
        if (net_Conn.rx != 0) {
            net_Recv();
            if (net_Conn.rx != 0)
                return 0;
        }
        do {
            cm_RecvCount++;
            if (cm_EnqueueReceived(&cm_RecvMsg) != 0)
                return 1;
            net_Conn.rx = (NETPKT *)&cm_RecvMsg;
            net_Recv();
        } while (net_Conn.rx == 0);
    }
    return 0;
}
/* 0x4c4030, waits up to timeout ms for a message of type type */

G17MSG *cm_WaitMessage(unsigned char type, int timeout)
{
    G17MSG *m;
    unsigned int end;

    end = d3_Time + timeout;
    while (d3_Time < end) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        for (m = cm_RecvQueue; m != 0; m = m->next)
            if (m->type == type)
                return m;
    }
    return 0;
}
/* 0x4c4080, appends a copy of src to the receive list; 1: no free record (closes the connection)
   @deep read against the original 0x4c4080..0x4c4126 instruction by instruction: same statements in the
   same order (free-list pop, struct copy rep movsd/movsb, cm_MessageSize, walk to tail / else head insert,
   the out-of-line else now matches after flipping the condition). Left is allocation: the original keeps
   &m->next in ebp (push ebp; lea ebp,[ebx+0x95]; mov dword ptr [ebp],0 in both branches) where ours stores
   [ebx+0x95] directly, and the pops of edi/esi/ebp are scheduled into the tails (pop edi before mov
   [ebx+0x91]); the add esp,4 after cm_MessageSize is scheduled after the load of cm_RecvQueue. 47 vs 53
   ins = the ebp push/lea/pops and duplicated pops in the two tails. */

int cm_EnqueueReceived(G17MSG *src)
{
    G17MSG *m, *p;

    m = cm_RecvFree;
    if (m == 0) {
        if (net_Conn.s != INVALID_SOCKET)
            net_Close();
        return 1;
    }
    cm_RecvFree = m->next;
    *m = *src;
    m->size = cm_MessageSize(m);
    if (cm_RecvQueue != 0) {
        for (p = cm_RecvQueue; p->next != 0; p = p->next)
            ;
        m->prev = p;
        m->next = 0;
        p->next = m;
    } else {
        m->prev = 0;
        m->next = 0;
        cm_RecvQueue = m;
    }
    return 0;
}
/* 0x4c4130, takes m out of the receive list and frees it */

void cm_FreeReceived(G17MSG *m)
{
    if (m->prev != 0)
        m->prev->next = m->next;
    else
        cm_RecvQueue = m->next;
    if (m->next != 0)
        m->next->prev = m->prev;
    m->prev = 0;
    m->next = cm_RecvFree;
    cm_RecvFree = m;
}
/* 0x4c4190, "cm_ExecCommand": runs the received messages (groups 5..6 checked first by 0x4c43c0), frees them;
   when the connection is closed, fails all of them
   @deep read against 0x4c4190..0x4c43b0: fixed the meaning (no cm_RecvQueue=0 after the closed-connection
   loop, the state reset loop is an int index with jl, the group check clears ok only on 0). Same
   statements and calls in the same order now; left is allocation: ours keeps 0 in ebx (cmp reg,ebx / mov
   [eax+1],bl) and grp/ok in ebp/edi, the original codes immediates, keeps grp in ebx, ok in ebp and
   &t->next in edi (lea edi,[esi+0x95]; mov edx,[edi]; mov [edi],edx); it loads cm_RecvQueue into esi
   before the closed test and reloads it after sv_PopTo (scheduling). 162 vs 165 ins. */

int cm_ExecCommands(void)
{
    G17MSG *m, *t, *next, *grp;
    G17STAT *s;
    int ok, i;

    if (net_Conn.closed != 0) {
        while ((m = cm_RecvQueue) != 0) {
            if (m->idx != -1) {
                s = &cm_SendStates[m->idx & 0x7fff];
                s->state = 2;
                s->kind = 0;
                s->value = 0;
            }
            cm_FreeReceived(m);
        }
        for (i = 0; i < 0x8000; i++)
            cm_SendStates[i].state = 2;
        if (ai_GameOver == 0) {
            ai_GameOver = 1;
            sv_PopTo(main_GameLoop);
        }
    }
    grp = 0;
    for (m = cm_RecvQueue; m != 0; m = next) {
        next = m->next;
        t = m;
        ok = 1;
        if (grp == 0) {
            if (m->type == 5)
                grp = m;
        } else if (m->type == 6) {
            if (cm_CheckGroup(grp) == 0)
                ok = 0;
            t = grp;
            next = grp->next;
            grp = 0;
        }
        if (grp != 0)
            ok = 0;
        if (t->type != 7 && cm_SplitComplete(t) != 0 && ok) {
            if (t == cm_RecvQueue) {
                cm_RecvQueue = next;
            } else {
                t->prev->next = t->next;
                if (t->next != 0)
                    t->next->prev = t->prev;
            }
            t->prev = 0;
            t->next = cm_RecvFree;
            cm_RecvFree = t;
            cm_JoinSplit(t);
            if (t->type < 0x6a) {
                if (t->idx == -1) {
                    s = 0;
                } else {
                    s = &cm_SendStates[t->idx & 0x7fff];
                    s->state = 2;
                    s->kind = 0;
                    s->value = 0;
                    if (t->seq != cm_LastRequCount + 1) {
                        if (t->seq == (unsigned int)cm_LastSyncCount) {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Received a Command with Count == cm_LastSyncCount, cmd->Count is %li, LastRequCnt is %li", t->seq, cm_LastRequCount);
                        } else if (t->type == 0x20 && t->data[0] == 0x0e) {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Received Sync, cmd->Count is %li, LastRequCnt is %li", t->seq, cm_LastRequCount);
                        } else {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Lost a Command, cmd->Count is %li, cm_LastRequCount is %li", t->seq, cm_LastRequCount);
                            cm_LastRequCount = t->seq;
                        }
                    } else {
                        cm_LastRequCount = t->seq;
                    }
                }
                cm_Handlers[t->type](t, s);
            }
        }
    }
    return 0;
}
/* 0x4c43c0, checks the messages of the group grp (5 .. 6): all pass: 1 at both ends, else 2 for all */

int cm_CheckGroup(G17MSG *grp)
{
    G17MSG *p;
    int ok;

    ok = 1;
    p = grp->next;
    while (ok && p->type != 6) {
        if (cm_Checks[p->type](p) != 0)
            ok = 0;
        p = p->next;
    }
    if (ok) {
        grp->type = 1;
        for (p = grp; p->type != 6; p = p->next)
            ;
        p->type = 1;
        return 1;
    }
    for (p = grp; p->type != 6; p = p->next)
        p->type = 2;
    p->type = 2;
    return 1;
}
/* 0x4c4450, resolves the ids -2 / -3 / -4 */

int cm_ResolveId(int a, int id)
{
    if (id == -2)
        return cm_LastCharId;
    if (id == -3)
        return cm_LastBuildingId;
    if (id == -4)
        id = cm_LastObjectId;
    return id;
}
/* 0x4c4480, sets the long message (sent as parts with the next message); 0: one is pending or too long */

int cm_SetLongMsg(unsigned short len, void *p)
{
    if (cm_SplitMsgLen != 0)
        return 0;
    if (len > 0x3fe)
        return 0;
    cm_SplitMsg.len = len;
    memcpy(cm_SplitMsg.data, p, len);
    cm_SplitMsgLen = len + 2;
    return 1;
}
/* 0x4c44d0, message 8: sends the size of the load buffer that the blocks of message 9 follow (the handler
   of type 8 allocates it, "cm_LoadBuf") */

void cm_SendLoadSize(int v)
{
    G17MSG m;

    m.type = 8;
    m.load.size = v;
    cm_QueueSend(&m);
}
/* 0x4c4500, message 9: sends one 0x80-byte block of the load buffer (the handler of type 9 appends it);
   returns the send slot of 0x4c39c0, -1 when the ring is full (the ls.c caller tests that) */

int cm_SendLoadBlock(void *p)
{
    G17MSG m;

    m.type = 9;
    memcpy(m.data, p, 0x80);
    return cm_QueueSend(&m);
}
/* 0x4c4540, message 5: starts a group */

void cm_BeginGroup(const char *s)
{
    G17MSG m;

    m.type = 5;
    strncpy((char *)m.data, s, 0x80);
    cm_QueueSend(&m);
}
/* 0x4c4580, message 6: ends a group; returns the queue slot of the message (cm_QueueSend; ai_ExchangeWindow waits on it) */

int cm_EndGroup(const char *s)
{
    G17MSG m;

    m.type = 6;
    strncpy((char *)m.data, s, 0x80);
    return cm_QueueSend(&m);
}
/* 0x4c45c0, message 10: creates a building of kind n for the character b (the 0x30 bytes of p, or zeros) */

int cm_SendMessage10(unsigned char n, int b, void *p, short d)
{
    G17MSG m;

    m.type = 10;
    if (n == 0)
        return -1;
    m.build.kind = n;
    m.build.owner = b;
    m.build.flags = d;
    if (p != 0) {
        memcpy(m.build.block, p, 0x30);
        m.build.hasBlock = 1;
    } else {
        memset(m.build.block, 0, 0x30);
        m.build.hasBlock = 0;
    }
    return cm_QueueSend(&m);
}
/* 0x4c4650, message 0xb */

int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h)
{
    G17MSG m;

    m.type = 0xb;
    m.chr.type = a;
    m.chr.parent1 = b;
    m.chr.parent2 = c;
    m.chr.age = d;
    m.chr.bldId = e;
    m.chr.job = f;
    m.chr.title = g;
    m.chr.a8 = h;
    return cm_QueueSend(&m);
}
/* 0x4c46d0, message 0xc (two names of 0x10 bytes) */

int cm_SendMessage0c(int a, int b, short c, unsigned char d, int e, unsigned char f, unsigned char g, const char *s1, const char *s2)
{
    G17MSG m;

    m.type = 0xc;
    m.named.parent1 = a;
    m.named.parent2 = b;
    m.named.age = c;
    m.named.job = d;
    m.named.ref54 = e;
    m.named.a8 = f;
    m.named.b00c = g;
    strncpy(m.named.first, s1, 0x10);
    strncpy(m.named.family, s2, 0x10);
    return cm_QueueSend(&m);
}
/* 0x4c4770, message 0xd */

void cm_SendTakeOverCharacter(int a, int b)
{
    G17MSG m;

    m.type = 0xd;
    m.takeover.taker = a;
    m.takeover.taken = b;
    cm_QueueSend(&m);
}
/* 0x4c47b0, message 0xe: deletes the object id (its callers pass the whole AIOBJ.objId; the message carries its low
   16 bits) */

void cm_SendDeleteObject(int id)
{
    G17MSG m;

    m.type = 0xe;
    m.del.objectId = (unsigned short)id;
    cm_QueueSend(&m);
}
/* 0x4c47e0, message 0xf: moves d of the goods / money of player c from a to b (-1: none); returns its send slot (cm_GetSlotState) */

int cm_SendMoveGoods(int a, int b, unsigned char c, int d)
{
    G17MSG m;

    m.moveGoods.from = a;
    m.moveGoods.to = b;
    m.type = 0xf;
    m.moveGoods.player = c;
    m.moveGoods.amount = d;
    return cm_QueueSend(&m);
}
/* 0x4c4830, message 0x10 */

void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d)
{
    G17MSG m;

    m.moveGoods.from = a;
    m.moveGoods.to = b;
    m.type = 0x10;
    m.moveGoods.player = c;
    m.moveGoods.amount = d;
    cm_QueueSend(&m);
}
/* 0x4c4880, message 0x11 */

void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f)
{
    G17MSG m;

    m.type = 0x11;
    m.sell.buyer = a;
    m.sell.seller = b;
    m.sell.item = c;
    m.sell.player = e;
    m.sell.count = d;
    m.sell.price = f;
    cm_QueueSend(&m);
}
/* 0x4c48e0, message 0x12 */

void cm_SendMakeGoods(int a, short b, int c)
{
    G17MSG m;

    m.make.objId = a;
    m.type = 0x12;
    m.make.item = b;
    m.make.runs = c;
    cm_QueueSend(&m);
}
/* 0x4c4920, message 0x13 */

void cm_SendCartRoute(int a, int b, int c)
{
    G17MSG m;

    m.move.to = a;
    m.type = 0x13;
    m.move.from = b;
    m.move.objId = c;
    cm_QueueSend(&m);
}
/* 0x4c4960, message 0x14 */

void cm_SendRemoveGoods(int a, short b)
{
    G17MSG m;

    m.type = 0x14;
    m.removeGoods.objId = a;
    m.removeGoods.item = b;
    cm_QueueSend(&m);
}
/* 0x4c49a0, message 0x16: the field block (deltas); returns the send slot (cm_QueueSend) */

int cm_SendFieldDeltas(void)
{
    G17MSG m;

    m.type = 0x16;
    memcpy(m.data, &cm_FieldBlock, sizeof(G17FIELDS));
    return cm_QueueSend(&m);
}
/* 0x4c49e0, message 0x17: the field block (values); returns the cm slot of the send (cm_QueueSend) */

int cm_SendFieldValues(void)
{
    G17MSG m;

    m.type = 0x17;
    memcpy(m.data, &cm_FieldBlock, sizeof(G17FIELDS));
    return cm_QueueSend(&m);
}
/* 0x4c4a20, message 0x18: the second field block */

void ai_SendNeedBlock(void)
{
    G17MSG m;

    m.type = 0x18;
    memcpy(m.data, &ai_NeedBlock, sizeof(G17FIELDS));
    cm_QueueSend(&m);
}
/* 0x4c4a60, message 0x19 */

void cm_SendSetField(int a, int b, int c, int d, int e)
{
    G17MSG m;

    m.setField.objId = a;
    m.setField.offset = b;
    m.setField.size = c;
    m.type = 0x19;
    m.setField.set = d;
    m.setField.clear = e;
    cm_QueueSend(&m);
}
/* 0x4c4ac0, sends message 0x1a: adds delta to the float field at the offset field of the character with the netId id */

void cm_SendFloatDelta(int id, int field, float delta)
{
    G17MSG m;

    m.floatDelta.id = id;
    m.type = 0x1a;
    m.floatDelta.field = field;
    m.floatDelta.delta = delta;
    cm_QueueSend(&m);
}
/* 0x4c4b00, sends message 0x1b: the relation of the character a to b changes by c (d: 0 / 1 / 2, the callers' kind of change), with the difficulty values; returns the send slot (cm_QueueSend) */

int cm_SendRelations(int a, int b, int c, int d)
{
    G17MSG m;

    m.rel.from = a;
    m.rel.to = b;
    m.type = 0x1b;
    m.rel.delta = c;
    m.rel.kind = d;
    m.rel.scale = ai_DifficultyScale;
    m.rel.level = (int)ai_DifficultyValue;
    return cm_QueueSend(&m);
}
/* 0x4c4b70, message 0x1c: msg (0xf8 bytes, AIMSG) as a long message; zeros at 0x38..0x57 become -1 */

int cm_SendLongMessage(char *msg)
{
    G17MSG m;
    int i;

    m.type = 0x1c;
    for (i = 0; i < 8; i++)
        if (((AIMSG *)msg)->ids[i] == 0)
            ((AIMSG *)msg)->ids[i] = -1;
    cm_SetLongMsg(0xf8, msg);
    cm_QueueSend(&m);
}
/* 0x4c4bc0, message 0x1c with len extra bytes after msg (0xf8 bytes) */

int cm_SendLongMessageText(char *msg, unsigned int len, void *extra)
{
    G17MSG m;
    char *buf;

    m.type = 0x1c;
    if (gm_FindChar(((AIMSG *)msg)->to) == 0)
        return -1;
    buf = mem_Alloc(len + 0xf8, gm_TagTmp);
    memcpy(buf, msg, 0xf8);
    memcpy(buf + 0xf8, extra, len);
    cm_SetLongMsg(len + 0xf8, buf);
    mem_Free(buf);
    return cm_QueueSend(&m);
}
/* 0x4c4c60, message 0x1d about p (three AITIMEs, its name from cm_EventNames); p + 0xac as the long message */

void cm_SendExSetHE(unsigned char k, char *p)
{
    G17MSG m;
    AIJOB *ev;

    ev = (AIJOB *)p;
    m.type = 0x1d;
    m.setHE.id = ev->id;
    m.setHE.start = ev->start;
    m.setHE.end = ev->end;
    m.setHE.clock60 = ev->clock60;
    m.setHE.kind = k;
    strcpy(m.setHE.name, cm_EventNames[(unsigned char)ev->kind]);
    cm_SetLongMsg(0xa0, ((AIJOBTAIL *)ev)->data);
    cm_QueueSend(&m);
}
/* 0x4c4d40, message 0x1e with the time t; -1: t is not valid (0x4fe210) */

int cm_SendTime(AITIME *t)
{
    G17MSG m;

    if (gm_CompareTime(t, &ai_Time) == 0)
        return -1;
    m.type = 0x1e;
    m.time = *t;
    return cm_QueueSend(&m);
}
/* 0x4c4db0, sends message 0x20 (the extended system message, handled by cm_ExSysMessage): sub type id and 0x7c bytes of msg */

int cm_SendExSysMessage(unsigned char id, void *msg)
{
    G17MSG m;

    m.type = 0x20;
    m.data[0] = id;
    if (msg != 0)
        memcpy(&m.data[1], msg, 0x7c);
    return cm_QueueSend(&m);
}
/* 0x4c4e00, message 0x21: has the server kill the character netId (mode 1, or gm_MayRemoveChar's 0 to keep it) */

void cm_SendKillCharacter(int netId, int mode)
{
    G17MSG m;

    m.type = 0x21;
    m.kill.netId = netId;
    m.kill.mode = mode;
    cm_QueueSend(&m);
}
/* 0x4c4e40, sends message 0x22, a new crime: the CRIME record p (0x2d bytes) with the id ai_NextCrimeId, the state 1 and a random seed; the send's result */

int cm_SendCrime(void *p)
{
    G17MSG m;

    m.type = 0x22;
    memcpy(&m.crime, p, sizeof(CRIME));
    m.crime.id = ai_NextCrimeId;
    m.crime.state = 1;
    m.crime.seed = rand();
    return cm_QueueSend(&m);
}
/* 0x4c4e90, sends message 0x23 (crime id a, force b: retire the crime record, cm_ExRetireCrime); the send slot of cm_QueueSend */

int cm_SendRetireCrime(int id, int force)
{
    G17MSG m;

    m.type = 0x23;
    m.retireCrime.id = id;
    m.retireCrime.force = force;
    return cm_QueueSend(&m);
}
/* 0x4c4ed0, message 0x24: the proof pair (the crime id, the character's netId) into ai_Evidence */

void cm_SendProofExecuted(int id, int netId)
{
    G17MSG m;

    m.type = 0x24;
    m.proof.netId = netId;
    m.proof.id = id;
    cm_QueueSend(&m);
}
/* 0x4c4f10, message 0x25: drops the proof pair (the crime id, the character's netId) from ai_Evidence */

void cm_SendDropEvidence(int id, int netId, int value)
{
    G17MSG m;

    m.dropEvidence.netId = netId;
    m.type = 0x25;
    m.dropEvidence.id = id;
    m.dropEvidence.value = value;
    cm_QueueSend(&m);
}
/* 0x4c4f50, message 0x26: asks the server for the cutscene record of player playerId in fight fightId
   (cm_ExSendCutInfo) */

int cm_RequestSendCutInfo(int playerId, int fightId)
{
    G17MSG m;
    char buf[0x100];

    m.type = 0x26;
    m.cut.playerId = playerId;
    m.cut.fightId = fightId;
    sprintf(buf, "cm_RequestSendCutInfo(): PlayerId:%i Time:%i", playerId, d3_Time);
    return cm_QueueSend(&m);
}
/* 0x4c4fa0, message 0x27: p (0x114 bytes) as the long message */

int cm_SendNewParty(void *p)
{
    G17MSG m;

    m.type = 0x27;
    cm_SetLongMsg(0x114, p);
    return cm_QueueSend(&m);
}
/* 0x4c4fd0, message 0x2a: removes the member netId from the scene party partyId (cm_RemovePartyMember) */

int cm_SendLeaveParty(int partyId, int member)
{
    G17MSG m;

    m.type = 0x2a;
    m.leaveParty.partyId = partyId;
    m.leaveParty.member = member;
    return cm_QueueSend(&m);
}
/* 0x4c5010, message 0x2b: asks to remove the entry `entry` from the holder's object list (objId -1: the
   holder's own list) - cm_ExRemoveOwned; returns its send slot */

int cm_SendMoveObject(int holder, int objId, int entry)
{
    G17MSG m;

    m.type = 0x2b;
    m.remove.holder = holder;
    m.remove.objId = objId;
    m.remove.entry = entry;
    return cm_QueueSend(&m);
}
/* 0x4c5050, message 0x2c: sets the plant of the type at the cell (x, z) of the building's vegetation map (fall: its fall model,
   under the id fallId; a later message without fall and that id puts it back); returns the send slot */

int cm_SetPlant(int bldId, unsigned char x, unsigned char z, int type, unsigned char fall, int fallId)
{
    G17MSG m;

    m.type = 0x2c;
    m.plant.bldId = bldId;
    m.plant.x = x;
    m.plant.z = z;
    m.plant.type = type;
    m.plant.fall = fall;
    m.plant.fallId = fallId;
    return cm_QueueSend(&m);
}
/* 0x4c50b0, message 0x2d: creates an object of kind 0x1b5 in the building owner, its +0x1c set to value;
   returns the send slot */

int cm_SendObject1b5(int owner, G17B7 value)
{
    G17MSG m;

    m.obj.value = value;
    m.type = 0x2d;
    m.obj.owner = owner;
    return cm_QueueSend(&m);
}
/* 0x4c5100, message 0x2e: the character netId ("sp_<netId>") walks to the cell (x, z) in scene (0: the
   current one) - handler 0x4c97c0 */

void cm_SendChrWalkTo(int netId, int x, int z, int scene)
{
    G17MSG m;

    m.type = 0x2e;
    m.walk.netId = netId;
    m.walk.x = x;
    m.walk.z = z;
    m.walk.scene = scene;
    cm_QueueSend(&m);
}
/* 0x4c5150, message 0x2f: the character netId walks to the object `name` of the building bldId (objId: the
   object instance, -1: none) and sits down - handler 0x4c9830 */

void cm_RequestChrMove(int netId, int bldId, int objId, const char *name)
{
    G17MSG m;

    m.type = 0x2f;
    m.place.netId = netId;
    m.place.bldId = bldId;
    m.place.objId = objId;
    strcpy(m.place.name, name);
    cm_QueueSend(&m);
}
/* 0x4c51b0, message 0x30: sends the character netId to the building bldId / room objId of the universe (the
   dummy name is the scene object it walks to, < 0x20 characters) - handler 0x4c99b0 */

void cm_RequestChrMoveToUniverse(int netId, int bldId, int objId, const char *name)
{
    G17MSG m;

    m.type = 0x30;
    m.place.netId = netId;
    m.place.bldId = bldId;
    m.place.objId = objId;
    if (strlen(name) < 0x20)
        strcpy(m.place.name, name);
    else
        err_Message("cm_RequestChrMoveToUniverse(): dummyname is too long...");
    cm_QueueSend(&m);
}
/* 0x4c5230, sends message 0x31: its handler (0x4c9c10) stops the actions of the person "sp_<charId>" */

void cm_SendChrStop(int charId)
{
    G17MSG m;

    m.type = 0x31;
    m.stop.charId = charId;
    cm_QueueSend(&m);
}
/* 0x4c5260, message 0x33: the cart (object id) stopped (state 0) or on the road (state 1) - handler 0x4c9d00 */

void cm_SendCartState(int cartId, int state)
{
    G17MSG m;

    m.type = 0x33;
    m.cart.cartId = cartId;
    m.cart.state = state;
    cm_QueueSend(&m);
}
/* 0x4c52a0, sends message 0x34: its handler (0x4c9d80) places character charId on the stage at building bldId (object objId,
   gm_PlaceOnStage); returns the queue handle (cm_QueueSend) */

int cm_SendPlaceOnStage(int charId, int bldId, int objId)
{
    G17MSG m;

    m.type = 0x34;
    m.stage.charId = charId;
    m.stage.bldId = bldId;
    m.stage.objId = objId;
    return cm_QueueSend(&m);
}
/* 0x4c52e0, message 0x35 about character id and building bld; c -1 and not f: the building's object (or -1, flag 1)
   @deep read against 0x4c52e0..0x4c5400: the branch polarities fixed (s2/s/gm_IsHomeType tested != 0 as
   the original lays them out), the data typed (G17GOTOMSG, AIBLD/AIOBJ). Same statements and calls; left
   is a tail duplication: after gm_Query the original repeats the o test and the objId store (test eax,eax
   / je / mov edx,[eax+2] / mov [esp+0x28],edx / jmp) in the gm_IsHomeType branch where ours jumps to the
   one shared copy after gm_GetGebaeudeHauptraum (code 4 at 0x4c53ba), plus the eax/ecx/edx renaming that
   follows (regs 14). Tried a ?: in the condition: no change. 95 vs 99 ins = the duplicated 4. */

int cm_SendChrGotoBuilding(int id, int bld, int c, const char *s, int f, const char *s2)
{
    G17MSG m;
    char *ch;
    AIBLD *b;
    AIOBJ *o;

    m.type = 0x35;
    m.go.charId = id;
    m.go.bldId = bld;
    m.go.objId = c;
    m.go.noObject = (unsigned char)f;
    if (s2 != 0)
        strncpy(m.go.reason, s2, 0x1f);
    else
        m.go.reason[0] = 0;
    if (s != 0)
        strcpy(m.go.text, s);
    else
        m.go.text[0] = 0;
    ch = gm_FindChar(id);
    if (ch != 0 && ch[8] == 0)
        return -1;
    if (f == 0 && c == -1) {
        o = 0;
        b = (AIBLD *)gm_FindBuildingById(bld);
        if (b != 0) {
            if (gm_IsHomeType((unsigned char *)b) != 0)
                o = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xfd);
            else
                o = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)b);
        }
        if (o != 0) {
            m.go.objId = o->objId;
        } else {
            m.go.noObject = 1;
            m.go.objId = -1;
        }
    }
    return cm_QueueSend(&m);
}
/* 0x4c5410, message 0x37: the character charId plays the animation name count times - handler 0x4caa20 */

void cm_SendChrAnim(int charId, const char *name, int count)
{
    G17MSG m;

    m.type = 0x37;
    m.anim.charId = charId;
    m.anim.count = count;
    strncpy(m.anim.name, name, 0x2f);
    cm_QueueSend(&m);
}
/* 0x4c5460, sends message 0x36: its handler (0x4ca8a0) lets the person "sp_<charId>" use the gates of building bldId
   (room object objId) */

void cm_SendChrUseGates(int charId, int bldId, int objId)
{
    G17MSG m;

    m.type = 0x36;
    m.gate.charId = charId;
    m.gate.bldId = bldId;
    m.gate.objId = objId;
    cm_QueueSend(&m);
}
/* 0x4c54a0, message 0x38: the building bldId gets the owner ownerId with the master masterId - handler 0x4caaa0 */

void cm_SendOwnership(int bldId, int masterId, int ownerId)
{
    G17MSG m;

    m.type = 0x38;
    m.owner.bldId = bldId;
    m.owner.masterId = masterId;
    m.owner.ownerId = ownerId;
    cm_QueueSend(&m);
}
/* 0x4c54e0, sends the message 0x3a with a building id (he_RunUpgrade, when the building event 0x1e ends; reasoned:
   the upgrade is done) */

void cm_SendUpgradeDone(int bldId)
{
    G17MSG m;

    m.type = 0x3a;
    m.upgrade.bldId = bldId;
    cm_QueueSend(&m);
}
/* 0x4c5510, message 0x3b: the building bldId is destroyed - handler 0x4cb100 */

void cm_SendBuildingDestroyed(int bldId)
{
    G17MSG m;

    m.type = 0x3b;
    m.destroy.bldId = bldId;
    cm_QueueSend(&m);
}
/* 0x4c5540, message 0x39 (b first) */

void cm_SendBoss(int a, int b)
{
    G17MSG m;

    m.type = 0x39;
    m.home.charId = a;
    m.home.bldId = b;
    cm_QueueSend(&m);
}
/* 0x4c5580, message 0x3c */

void cm_SendHandOverCrimes(int a, int b, int c, int d)
{
    G17MSG m;

    m.type = 0x3c;
    m.handOver.owner = a;
    m.handOver.culprit = b;
    m.handOver.state = c;
    m.handOver.hand = d;
    cm_QueueSend(&m);
}
/* 0x4c55d0, message 0x3d: hires a character of kind b for the building p (the applicant for title c / job d,
   gm_FindApplicant, else a new one) with a home (gm_FindHome) */

int cm_SendMessage3d(char *p, unsigned char b, int c, int d)
{
    G17MSG m;
    AICHAR *q;
    AIBLD *r;

    m.type = 0x3d;
    if (p == 0)
        return -1;
    q = (AICHAR *)gm_FindApplicant((struct ORDBLD *)p, c, d);
    m.hire.bldId = ((AIBLD *)p)->id;
    if (q != 0) {
        m.hire.charId = q->netId;
        r = q->bld2 == 0 ? (AIBLD *)gm_FindHome(q->id, (struct ORDBLD *)p) : 0;
    } else {
        m.hire.charId = -1;
        r = (AIBLD *)gm_FindHome(0xffff, (struct ORDBLD *)p);
    }
    if (r != 0)
        m.hire.homeId = r->id;
    else
        m.hire.homeId = -1;
    m.hire.kind = b;
    m.hire.title = (unsigned char)c;
    m.hire.job = (unsigned char)d;
    return cm_QueueSend(&m);
}
/* 0x4c5690, message 0x3e: the character a and a text, the model name it takes (reasoned guess: its one caller,
   gm_DailyCharacters, sends "stallbursche_MANN" / "magd_FRAU" by the sex when a character comes of age) */

void cm_SendCharModel(int a, const char *s)
{
    G17MSG m;

    m.model.charId = a;
    m.type = 0x3e;
    strcpy(m.model.name, s);
    cm_QueueSend(&m);
}
/* 0x4c56d0, message 0x3f: k and 0x80 bytes of p */

void cm_SendGoodsRecord(void *p, unsigned char k)
{
    G17MSG m;

    m.type = 0x3f;
    m.data[0] = k;
    memcpy(&m.data[1], p, 0x80);
    cm_QueueSend(&m);
}
/* 0x4c5710, message 0x40: the differences of the goods record p to the player k's own record of the same item
   (gm_FindGoods), the deltas its handler 0x4cb730 adds */

void cm_SendGoodsDelta(GMSTOCK *p, unsigned char k)
{
    G17MSG m;
    GMSTOCK t;
    GMSTOCK *o;

    o = (GMSTOCK *)gm_FindGoods(k, p->item);
    m.type = 0x40;
    t = *p;
    t.demand -= o->demand;
    t.mean -= o->mean;
    t.f2c -= o->f2c;
    t.price -= o->price;
    t.daily[0] -= o->daily[0];
    t.daily[1] -= o->daily[1];
    t.daily[2] -= o->daily[2];
    m.goods.player = k;
    m.goods.item = p->item;
    m.goods.daily[0] = t.daily[0];
    m.goods.daily[1] = t.daily[1];
    m.goods.daily[2] = t.daily[2];
    m.goods.demand = t.demand;
    m.goods.mean = t.mean;
    m.goods.f2c = t.f2c;
    m.goods.price = t.price;
    cm_QueueSend(&m);
}
/* 0x4c5800, message 0x41 (0x3a bytes of p) */

void cm_SendMessage41(void *p)
{
    G17MSG m;

    m.type = 0x41;
    memcpy(m.data, p, 0x3a);
    cm_QueueSend(&m);
}
/* 0x4c5840, message 0x42 */

void cm_SendUpdateSmoke(int a)
{
    G17MSG m;

    m.type = 0x42;
    m.smoke.bldId = a;
    cm_QueueSend(&m);
}
/* 0x4c5870, queues the message 0x43 with a character's netId (sent after its skills were raised: he_RunTraining,
   the trial step picks of lc; reasoned, the receiver not read) */

void cm_SendSkillsChanged(int a)
{
    G17MSG m;

    m.type = 0x43;
    m.skills.charId = a;
    cm_QueueSend(&m);
}
/* 0x4c58a0, message 0x44: sends the office application (the int id, the two office bytes and the kind of p, G17B7) and returns the
   send slot of 0x4c39c0 */

int cm_SendApplication(AMTAPP *p)
{
    G17MSG m;

    m.type = 0x44;
    m.app = *p;
    return cm_QueueSend(&m);
}
/* 0x4c58e0, message 0x45: sends the 15-byte office record p (amt_SendOfficeMessage's AMTMSG; the int id, the two character ids, the
   kind and the code) and returns the send slot of 0x4c39c0 */

int cm_SendOfficeRecord(AMTMSG *p)
{
    G17MSG m;

    m.type = 0x45;
    m.office = *p;
    return cm_QueueSend(&m);
}
/* 0x4c5930, sends message 0x46, an AI decision record's new value (the 9 bytes of p, a GVDECMSG of gv_SetTableRecord); the send's result */

int cm_SendDecisionValue(GVDECMSG *p)
{
    G17MSG m;

    m.type = 0x46;
    m.decision = *p;
    return cm_QueueSend(&m);
}
/* 0x4c5970, sends message 0x47 (character a, b, c); the event handlers send (a, 0, 0) before they walk a character away (reasoned: clears what it is doing) */

void cm_SendCharClear(int a, int b, int c)
{
    G17MSG m;

    m.clear.charId = a;
    m.type = 0x47;
    m.clear.room = b;
    m.clear.spare = c;
    cm_QueueSend(&m);
}
/* 0x4c59b0, sends message 0x48: its handler (0x4cb9b0) gives the character id the job (gm_SetJob) */

void cm_SendSetJob(int id, int job)
{
    G17MSG m;

    m.type = 0x48;
    m.setJob.charId = id;
    m.setJob.job = (unsigned char)job;
    cm_QueueSend(&m);
}
/* 0x4c59f0, message 0x49 with a text */

void cm_SendSpawnCharacter(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s)
{
    G17MSG m;

    m.spawn.kind = a;
    m.spawn.objId = c;
    m.spawn.bldId = b;
    m.spawn.job = d;
    m.spawn.item = f;
    m.type = 0x49;
    m.spawn.title = e;
    strcpy(m.spawn.text, s);
    cm_QueueSend(&m);
}
/* 0x4c5a70, message 0x4a */

void cm_SendFreeObject(int a)
{
    G17MSG m;

    m.type = 0x4a;
    m.freeObject.objId = a;
    cm_QueueSend(&m);
}
/* 0x4c5aa0, message 0x4b (0x80 bytes of p) */

void cm_SendChat(void *p)
{
    G17MSG m;

    m.type = 0x4b;
    memcpy(m.data, p, 0x80);
    cm_QueueSend(&m);
}
/* 0x4c5ae0, "cm_RequestCreateGeb": message 0x4c, a building of kind for the character owner at pos with the
   rotation rot; -1 for kind 0 */

int cm_RequestCreateGeb(int owner, int kind, D3VECTOR4 *pos, int *rot)
{
    G17MSG m;
    char buf[0x100];

    m.type = 0x4c;
    if (kind == 0)
        return -1;
    m.geb.owner = owner;
    m.geb.kind = (unsigned char)kind;
    m.geb.pos = *pos;
    m.geb.rot[0] = rot[0];
    m.geb.rot[1] = rot[1];
    m.geb.rot[2] = rot[2];
    sprintf(buf, "cm_RequestCreateGeb(): pos %f   %f   %f", pos->x, pos->y, pos->z);
    return cm_QueueSend(&m);
}
/* 0x4c5b90, sends message 0x4d (character a); the event handlers send it before they walk a character away (reasoned: halts it) */

void cm_SendCharHalt(int a)
{
    G17MSG m;

    m.type = 0x4d;
    m.halt.charId = a;
    cm_QueueSend(&m);
}
/* 0x4c5bc0, message 0x4e with two texts */

int cm_SendObjCell(const char *s, int a, int b, const char *s2)
{
    G17MSG m;

    m.type = 0x4e;
    strcpy(m.objCell.name, s);
    m.objCell.x = a;
    m.objCell.z = b;
    if (s2 != 0)
        strcpy(m.objCell.name2, s2);
    else
        m.objCell.name2[0] = 0;
    return cm_QueueSend(&m);
}
/* 0x4c5c40, message 0x50 (0x2c bytes of p, the id *cm_CurrentFight) */

int cm_SendCutEntry(int a, void *p)
{
    G17MSG m;

    m.type = 0x50;
    m.cutEntry.playerId = a;
    if (cm_CurrentFight == 0)
        m.cutEntry.fightId = -1;
    else
        m.cutEntry.fightId = *cm_CurrentFight;
    memcpy(&m.cutEntry.entry, p, sizeof(m.cutEntry.entry));
    return cm_QueueSend(&m);
}
/* 0x4c5ca0, message 0x51, as 0x4c5c40 */

int cm_SendCutEntryUpdate(int a, void *p)
{
    G17MSG m;

    m.type = 0x51;
    m.cutEntry.playerId = a;
    if (cm_CurrentFight == 0)
        m.cutEntry.fightId = -1;
    else
        m.cutEntry.fightId = *cm_CurrentFight;
    memcpy(&m.cutEntry.entry, p, sizeof(m.cutEntry.entry));
    return cm_QueueSend(&m);
}
/* 0x4c5d00, message 0x52 */

void cm_SendDamage(int a, int b, int c)
{
    G17MSG m;

    m.damage.netId = a;
    m.type = 0x52;
    m.damage.damage = b;
    m.damage.health = c;
    cm_QueueSend(&m);
}
/* 0x4c5d40, message 0x53 (0x14 bytes of p) */

void cm_SendScriptMessage(void *p)
{
    G17MSG m;

    m.type = 0x53;
    memcpy(m.data, p, 0x14);
    cm_QueueSend(&m);
}
/* 0x4c5d80, sends message 0x54, the table request p (a G17TABLEMSG); returns the slot cm_QueueSend gives it
   (ai_StammtischJoinWindow waits on it with cm_GetSlotState) */

int cm_SendTableRequest(G17TABLEMSG *p)
{
    G17MSG m;

    m.type = 0x54;
    m.table = *p;
    return cm_QueueSend(&m);
}
/* 0x4c5dc0, message 0x55: the command cmd of the fighter slot f of the current fight, with the position and rotation
   of the fighter c's body and its health; for an attack (2) also the target's current health
   @deep Read whole against 0x4c5dc0..0x4c5e7a: the same stores (fight id, cmd, the 0x2c-byte slot by rep
   movsd, health, pos x/y/z, rot x/y/z, the target's health for cmd 2), calls and branches. Left is
   allocation: the original keeps esi/edi pushed to the end and copies the six vector dwords through esi
   (mov esi,[eax+0x4c] / mov [esp+0x1d],esi ...), ours pops esi/edi right after the rep movsd and copies
   through ecx, so every m offset differs by 8 (slots) and the cmd-2 push uses edx vs ecx. Tried: struct
   copies of pos / rot (keeps esi but adds lea / add for the addresses, 34 ins), memcpy of 12 bytes (same),
   one struct copy plus element copies (29 ins, bigger), element copies (this, 43 ins, size 183 vs 186). */

int cm_SendFightCommand(AICHAR *c, int cmd, CMFIGHTSLOT *f)
{
    G17MSG m;
    D3OBJECT *b;
    AICHAR *t;

    m.fight.fight = *cm_CurrentFight;
    m.type = 0x55;
    m.fight.cmd = (unsigned char)cmd;
    m.fight.slot = *f;
    if (c != 0 && c->model != 0) {
        b = c->model->body;
        m.fight.health = c->v024;
        m.fight.pos.x = b->pos.x;
        m.fight.pos.y = b->pos.y;
        m.fight.pos.z = b->pos.z;
        m.fight.rot.x = b->rotation.x;
        m.fight.rot.y = b->rotation.y;
        m.fight.rot.z = b->rotation.z;
        if (cmd == 2) {
            t = (AICHAR *)cm_FindFighter(m.fight.slot.target);
            if (t != 0)
                m.fight.slot.health = t->v024;
        }
    }
    return cm_QueueSend(&m);
}
/* 0x4c5e80, message 0x56 (0x28 bytes of p) */

void cm_SendMessage56(void *p)
{
    G17MSG m;

    m.type = 0x56;
    memcpy(m.data, p, 0x28);
    cm_QueueSend(&m);
}
/* 0x4c5ec0, sends message 0x57: its handler (0x4cc920) deletes the scene character (+0x184) of character a */

void cm_SendChrDelete(int a)
{
    G17MSG m;

    m.type = 0x57;
    m.chrDelete.charId = a;
    cm_QueueSend(&m);
}
/* 0x4c5ef0, sends message 0x58, the cancel of party a: its handler (0x4cc980) ends the group cm_PartyById finds */

void cm_SendCancelParty(int a)
{
    G17MSG m;

    m.type = 0x58;
    m.cancelParty.partyId = a;
    cm_QueueSend(&m);
}
/* 0x4c5f20, message 0x59 (0x48 bytes of p) */

int cm_SendCutsceneReady(void *p)
{
    G17MSG m;

    m.type = 0x59;
    memcpy(m.data, p, 0x48);
    return cm_QueueSend(&m);
}
/* 0x4c5f60, sends message 0x5a: the money delta b of character a (its handler 0x4ccaa0 adds it to +0x194,
   clamped 0..0x32) */

void cm_SendMoneyDelta(int a, int b)
{
    G17MSG m;

    m.type = 0x5a;
    m.money.charId = a;
    m.money.delta = b;
    cm_QueueSend(&m);
}
/* 0x4c5fa0, sends message 0x5b: the heldRounds delta b of character a, with amt_ClearOfficesIfAll's all flag c
   (its handler 0x4ccb10 adds b to heldRounds, clamped 0..0xff) */

void cm_SendMessage5b(int a, int b, int c)
{
    G17MSG m;

    m.heldRounds.charId = a;
    m.type = 0x5b;
    m.heldRounds.delta = b;
    m.heldRounds.all = c;
    cm_QueueSend(&m);
}
/* 0x4c5fe0, sends message 0x5c, the office-swap request p (an AMTTRADE, built by amt_SendOfficeSwap); returns the
   slot cm_QueueSend gives it (lc.c's counter-espionage wrapper 0x5cb1e0 waits on it) */

int cm_SendTradeMessage(AMTTRADE *p)
{
    G17MSG m;

    m.type = 0x5c;
    m.trade = *p;
    return cm_QueueSend(&m);
}
/* 0x4c6020, sends message 0x5d: the delta c of the skill index b of character a (its handler 0x4ccc20 applies it) */

void cm_SendAddSkill(int a, int b, int c)
{
    G17MSG m;

    m.skill.charId = a;
    m.type = 0x5d;
    m.skill.skill = b;
    m.skill.delta = c;
    cm_QueueSend(&m);
}
/* 0x4c6060, sends message 0x5e: the spy report p of building o (its handler 0x4ccce0 sets the job record 0xca of
   the building from it; o's id goes at data +4) */

void cm_SendSpyReport(AIBLD *o, HESPYREPORT *p)
{
    G17MSG m;

    m.spy.bldId = o->id;
    m.type = 0x5e;
    m.spy.rep = *p;
    cm_QueueSend(&m);
}
/* 0x4c60b0, message 0x60 (0x1f bytes of p) */

void cm_SendGraveOrder(void *p)
{
    G17MSG m;

    m.type = 0x60;
    memcpy(m.data, p, 0x1f);
    cm_QueueSend(&m);
}
/* 0x4c60f0, message 0x61 (0x19 bytes of p), the slot handle cm_QueueSend returns */

int cm_SendGraveMove(void *p)
{
    G17MSG m;

    m.type = 0x61;
    memcpy(m.data, p, 0x19);
    return cm_QueueSend(&m);
}
/* 0x4c6130, message 0x62 (0x19 bytes of p) */

void cm_SendRaiseDead(void *p)
{
    G17MSG m;

    m.type = 0x62;
    memcpy(m.data, p, 0x19);
    cm_QueueSend(&m);
}
/* 0x4c6170, message 99 (0x24 bytes of p) */

void cm_SendCreateAlchemist(void *p)
{
    G17MSG m;

    m.type = 99;
    memcpy(m.data, p, 0x24);
    cm_QueueSend(&m);
}
/* 0x4c61b0, message 100 (0x18 bytes of p) */

void cm_SendCommand(void *p)
{
    G17MSG m;

    m.type = 100;
    memcpy(m.data, p, 0x18);
    cm_QueueSend(&m);
}
/* 0x4c61f0, sends message 0x65: the master of party p becomes the character q (its netId; its handler 0x4cd130
   sets the party record's host) */

void cm_SendMasterChange(G28REC *p, AICHAR *q)
{
    G17MSG m;

    m.type = 0x65;
    m.master.partyId = p->id;
    m.master.masterId = q->netId;
    cm_QueueSend(&m);
}
/* 0x4c6230, message 0x66 'nCTa' (reserve an effect, the id at p[1]); returns what cm_QueueSend returns */

int cm_SendEffectLoad(int *p)
{
    G17MSG m;

    m.type = 0x66;
    m.effect.tag = p[0];
    m.effect.effectId = p[1];
    return cm_QueueSend(&m);
}
/* 0x4c6260, sends message 0x67, the 0x80 bytes of p: cannon balls fired at a building (GVSHOTMSG, gv.c gv_SendShots; the receiver
   starts them in the effect slot, gv.c 0x4eee00); returns what cm_QueueSend returns */

int cm_SendShots(void *p)
{
    G17MSG m;

    m.type = 0x67;
    memcpy(m.data, p, 0x80);
    return cm_QueueSend(&m);
}
/* 0x4c62a0, message 0x68: the route points pts (RTPOINT, next at +0x3c) of the cart o as the long message */

void cm_SendSetRoute(AIOBJ *o, RTPOINT *pts)
{
    G17MSG m;
    char buf[0x200];
    char *d;
    RTPOINT *next;
    int i;

    i = rt_PointCount(pts);
    m.type = 0x68;
    m.route.objId = o->objId;
    m.route.count = i;
    for (d = buf; i > 0; i--) {
        next = pts->next;
        memcpy(d, pts, sizeof(RTPOINT));
        d += sizeof(RTPOINT);
        pts = next;
    }
    cm_SetLongMsg(m.route.count << 6, buf);
    cm_QueueSend(&m);
}
/* 0x4c6320, message 0x69: the cart o's id (its handler 0x4cd2f0 frees its route) */

void cm_SendClearRoute(AIOBJ *o)
{
    G17MSG m;

    m.type = 0x69;
    m.route.objId = o->objId;
    cm_QueueSend(&m);
}
/* 0x4c6350, check of message 0xf: 1 when the holder of its to id has less money (the currency of player
   m->moveGoods.player) than the amount it moves */

int cm_CheckMoveGoods(G17MSG *m)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    int *list;

    if (m->moveGoods.to != -1) {
        m->moveGoods.to = cm_ResolveId((int)m, m->moveGoods.to);
        if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->moveGoods.to) == 0)
            return 1;
        if (b != 0)
            list = &b->objList;
        else if (o != 0)
            list = &o->list;
        else
            list = &c->inventory;
        if (gm_SumSlotAmounts((void *)*list, m->moveGoods.player) - m->moveGoods.amount < 0)
            return 1;
    }
    return 0;
}
/* 0x4c63e0, check of message 0x11 (trade): 1 if the seller lacks the goods or the money, the buyer lacks room or
   cannot pay */

int cm_CheckSellObject(G17MSG *m)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    char *p;
    ORDCHAR *r;
    int *list;

    if (m->sell.seller != -1) {
        m->sell.seller = cm_ResolveId((int)m, m->sell.seller);
        if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->sell.seller) == 0)
            return 1;
        if (b != 0)
            list = &b->objList;
        else if (o != 0)
            list = &o->list;
        else
            list = &c->inventory;
        p = gm_Query((void *)*list, 1, 0, m->sell.item);
        if (p == 0)
            return 1;
        if (((ORDOBJ *)p)->count < m->sell.count)
            return 1;
    }
    if (m->sell.buyer != -1) {
        m->sell.buyer = cm_ResolveId((int)m, m->sell.buyer);
        if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->sell.buyer) == 0)
            return 1;
        if (b != 0) {
            if (b->owner != 0xffff)
                r = (ORDCHAR *)ai_Chars[b->owner];
            else
                r = 0;
        } else if (o != 0) {
            if (o->type == 0x2a || o->type == 0x116 || o->type == 0x1dd) {
                if (gm_FreeCapacity((short *)o, m->sell.item, m->sell.count) < m->sell.count)
                    return 1;
            }
            r = (ORDCHAR *)gm_ObjectBuildingOwner((unsigned short *)o);
        } else {
            if (gm_TakeableCount((char *)c, m->sell.item, m->sell.count) < m->sell.count)
                return 1;
            r = c;
        }
        if (m->sell.price != 0 && r != 0) {
            if (gm_SumSlotAmounts((void *)r->inventory, m->sell.player) < m->sell.count * m->sell.price)
                return 1;
        }
    }
    return 0;
}
/* 0x4c65b0, check of message 0x16 (field deltas): 1 when a field of the object is its id or its partner (+0x5c) and
   that partner's own partner is another character */

int cm_CheckFieldDeltas(G17MSG *m)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    char *base, *a;
    unsigned char *p;
    CMFIELD *f;
    ORDCHAR *r;
    unsigned int i, j, n;
    int size;

    m->fields.id = cm_ResolveId((int)m, m->fields.id);
    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->fields.id) == 0)
        return 1;
    if (b != 0)
        base = (char *)b;
    else if (o != 0)
        base = (char *)o;
    else
        base = (char *)c;
    p = m->fields.buf;
    for (i = 0; i < m->fields.n; i++) {
        f = (CMFIELD *)p;
        size = f->size;
        n = f->n;
        a = base + f->offset;
        p += 4;
        if (c != 0) {
            if (a == (char *)&c->netId)
                return 1;
            if (a == (char *)&c->partner) {
                r = (ORDCHAR *)gm_FindChar(f->values[0] + c->partner);
                if (r == 0)
                    return 1;
                if (r->partner != -1 && r->partner != c->netId)
                    return 1;
            }
        }
        switch (size) {
        case 1:
            for (j = 0; j < n; j++)
                p += 1;
            break;
        case 2:
            for (j = 0; j < n; j++)
                p += 2;
            break;
        case 4:
            for (j = 0; j < n; j++)
                p += 4;
            break;
        }
    }
    return 0;
}
/* 0x4c66c0, check of message 0x19: 1 when it sets flag 0x800000 of field 0x1c8 of a character that has it */

int cm_CheckSetField(G17MSG *m)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;
    char *base;

    m->setField.objId = cm_ResolveId((int)m, m->setField.objId);
    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->setField.objId) == 0)
        return 1;
    base = (char *)b;
    if (base == 0) {
        base = (char *)o;
        if (base == 0)
            base = (char *)c;
    }
    if (c != 0 && base + m->setField.offset == (char *)&c->flags1c8
        && (m->setField.set & CM_KEEPER_FLAG) && (c->flags1c8 & CM_KEEPER_FLAG))
        return 1;
    return 0;
}
/* 0x4c6750, check of message 0x38 (ownership): 1 (rejected) unless the building it hands over has the
   flags5a bit 2, the mark the sale sets around cm_SendOwnership (he.c 0x55af70's theft mark too) */

int cm_CheckOwnership(G17MSG *m)
{
    AIBLD *b;

    b = (AIBLD *)gm_BuildQuery(1, 1, m->owner.bldId);
    if (b != 0 && (b->flags5a & 2))
        return 0;
    return 1;
}
/* 0x4c6780, check of message 0x44 (office application): 1 when amt_CheckApplication accepts it */

int cm_CheckApplication(G17MSG *m)
{
    return amt_CheckApplication(&m->app) == 0;
}
/* 0x4c67a0, check of message 0x45 (office record): 1 when amt_CheckOfficeMessage accepts it */

int cm_CheckOfficeMessage(G17MSG *m)
{
    return amt_CheckOfficeMessage(&m->office) == 0;
}
/* 0x4c67c0, check of message 0x53 for a tavern's dark corner (the kind 0x12c object of building data[8]): 1
   (rejected) when 'cler' comes within a year of the corner's last reset, or when 'buy ' names none of its three
   offers; 0 when it may go through; 1 for any other sub-command */

int cm_CheckDarkCorner(G17MSG *m)
{
    AIBLD *b;
    AIOBJ *o;
    int i;

    if (m->script.op == 0x636c6572) {               /* 'cler' */
        b = (AIBLD *)gm_BuildQuery(1, 1, m->script.bldId);
        if (b != 0) {
            o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12c);
            if (o != 0)
                return ai_Time.year - o->cornerYear <= 1;
        }
    } else if (m->script.op == 0x62757920) {        /* 'buy ' */
        b = (AIBLD *)gm_BuildQuery(1, 1, m->script.bldId);
        if (b != 0) {
            o = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12c);
            if (o != 0) {
                for (i = 0; i < 3; i++)
                    if (o->recs[i] == m->script.offer)
                        return 0;
            }
        }
    }
    return 1;
}
/* 0x4c6870, check of message 0x54 on the table (object kind 0x12d) of the building bldId: 0 when 'join' finds a free
   seat and the character not seated yet, when 'leav' finds the character seated, when 'cler' finds no table or the
   building's owner gone (dead, the player, or kind 0xf); else 1 */

int cm_CheckTableRequest(G17MSG *m)
{
    AIBLD *b;
    AIOBJ *t;
    AICHAR *c;
    int i;
    unsigned short k;

    if (m->table.op == 'join') {
        b = (AIBLD *)gm_BuildQuery(1, 1, m->table.bldId);
        if (b != 0 && (t = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12d)) != 0) {
            for (i = 0; i < 4; i++)
                if (t->seats[i] == m->table.charId)
                    return 1;
            for (i = 0; i < 4; i++)
                if (t->seats[i] == -1)
                    break;
            if (i < 4)
                return 0;
        }
    } else if (m->table.op == 'leav') {
        b = (AIBLD *)gm_BuildQuery(1, 1, m->table.bldId);
        if (b != 0 && (t = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12d)) != 0) {
            for (i = 0; i < 4; i++)
                if (t->seats[i] == m->table.charId)
                    break;
            if (i < 4)
                return 0;
        }
    } else if (m->table.op == 'cler' && (b = (AIBLD *)gm_BuildQuery(1, 1, m->table.bldId)) != 0) {
        k = b->owner;
        if (k != 0xffff) {
            c = &ai_CharsT[k];
            if (c != 0 && c != (AICHAR *)ai_PlayerChar && c->health != 0 && c->kind != 0xf)
                return 1;
        }
        return gm_Query((void *)b->objList, 2, 6, 0, 0x12d) == 0;
    }
    return 1;
}
/* 0x4c69c0, check of message 0x59 (the party may start its cutscene): 1 (rejected) when one of its 16 members
   is a player character (kind 6 / 7) already in another scene party; 1 when the party id is -1, else 0 */

int cm_CheckCutsceneReady(G17MSG *m)
{
    AICHAR *c;
    int i;

    if (m->ready.partyId != -1) {
        for (i = 0; i < 0x10; i++) {
            c = (AICHAR *)gm_FindChar(m->ready.members[i]);
            if (c != 0 && (c->kind == 6 || c->kind == 7) && c->partyId != -1 && c->partyId != m->ready.partyId)
                return 1;
        }
        return 0;
    }
    return 1;
}
/* 0x4c6a20, the handler of the message types that carry no command (0 the empty slot, 3 hello, 5 group begin, 6
   group end, 7 a part of a split message): nothing to do, s->state = 1; 1 for any other type */

int cm_ExNothing(G17MSG *m, G17STAT *s)
{
    if (m->type != 0 && m->type != 5 && m->type != 6 && m->type != 7)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6a50, the handler of message type 1 (the mark cm_CheckGroup puts on a group whose checks all passed):
   nothing to do, s->state = 1 */

int cm_ExGroupPassed(G17MSG *m, G17STAT *s)
{
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6a60, the handler of message type 4: ORs its first byte into cm_NetFlags (the flags of the session) */

int cm_ExNetFlags(G17MSG *m, G17STAT *s)
{
    cm_NetFlags |= m->data[0];
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6a90, handler of type 8: allocates the load buffer */

int cm_ExLoadSize(G17MSG *m, G17STAT *s)
{
    cm_LoadSize = m->load.size;
    cm_LoadBuf = mem_Alloc(cm_LoadSize, "cm_LoadBuf");
    cm_LoadReceived = 0;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6ad0, handler of type 9: appends 0x80 bytes to the load buffer */

int cm_ExLoadBlock(G17MSG *m, G17STAT *s)
{
    memcpy(cm_LoadBuf + cm_LoadReceived, m->data, 0x80);
    cm_LoadReceived += 0x80;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6b20, handler of type 10: creates the building the message names (its kind, owner, flags and 0x30-byte
   block), the owner character resolved by id */

int cm_ExCreateBuilding(G17MSG *m, G17STAT *s)
{
    ORDBLD *b;
    unsigned short k;
    int id;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->build.objectId;
    if (m->build.owner == -1) {
        k = 0xffff;
    } else {
        id = cm_ResolveId((int)m, m->build.owner);
        m->build.owner = id;
        for (k = 0; k < 0x300; k++)
            if (ai_CharsT[k].id != 0xffff && ai_CharsT[k].netId == id)
                break;
        if (k >= 0x300)
            return 1;
    }
    b = (ORDBLD *)gm_CreateBuilding(m->build.kind, k);
    if (b == 0)
        return 1;
    cm_LastBuildingId = b->id;
    b->flags5aw = m->build.flags;
    if (m->build.hasBlock != 0)
        memcpy(&b->v65, m->build.block, 0x30);
    if (s != 0) {
        s->state = 1;
        s->kind = 2;
        s->value = (int)b;
    }
    return 0;
}
/* 0x4c6bf0, handler of type 0xb: creates the character of the message (gm_CreateCharacter) in the building bldId,
   the random generator seeded with its seed; the new character is cm_LastCharId and s's value */

int cm_ExCreateCharacter(G17MSG *m, G17STAT *s)
{
    struct ORDBLD *b;
    unsigned int k;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->chr.objectId;
    else
        m->chr.seed = rand();
    util_Seed(m->chr.seed);
    if (m->chr.bldId == -1) {
        b = 0;
    } else {
        m->chr.bldId = cm_ResolveId((int)m, m->chr.bldId);
        b = (struct ORDBLD *)gm_BuildQuery(1, 1, m->chr.bldId);
        if (b == 0)
            return 1;
    }
    k = gm_CreateCharacter(m->chr.type, m->chr.parent1, m->chr.parent2, m->chr.age, b, m->chr.job, m->chr.title, m->chr.a8);
    if (k == 0xffff)
        return 1;
    cm_LastCharId = ai_CharsT[k].netId;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)&ai_CharsT[k];
    }
    return 0;
}
/* 0x4c6cd0, handler of type 0xc: creates a named character (gm_CreateCharacter, type 6 with s, else 7) with its first
   and family name; a player character's record and its relatives[1] get the family name too. The generator is
   seeded with the message's dword at 0x12 (inside age / job / ref54), not with its seed: as the original does
   @deep Read whole against 0x4c6cd0..0x4c6e00: the same calls with the same arguments (util_Seed of the
   dword at data+0x12 as the original, gm_CreateCharacter(s ? 6 : 7, parent1, parent2, age, 0, job, 0, a8),
   the three strncpy of first / family, gm_FindChar of relatives[1]), the same stores (cm_LastCharId, the
   AIFIN w00 / family / camAngle -1.0, ref54, b00c, the G17STAT) and branches, every AICHAR field addressed
   as ai_CharsT[k].field off the k*0x21c index as the original does. Left is scheduling / allocation: the
   original computes the ternary (mov eax,[esp+0x10]; neg; sbb; add eax,7) before the byte argument loads,
   ours after them in edx (hunks 0x4c6cfe / 0x4c6d0d); the netId load for cm_LastCharId sits between the
   strncpy pushes in other registers (0x4c6d4b..0x4c6d70: ours lea eax / mov edx, the original mov eax /
   lea edx); the ref54 / s loads at 0x4c6de0 in ecx / eax swapped. Tried: the ternary into a local before
   the call (no change), the strncpy before the cm_LastCharId store (moves the store after the call:
   further from the original, reverted). */

int cm_ExCreateNamedCharacter(G17MSG *m, G17STAT *s)
{
    AICHAR *c;
    AIFIN *t;
    char *r;
    unsigned int k;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->named.objectId;
    else
        m->named.seed = rand();
    util_Seed(m->named.seedRead);
    k = gm_CreateCharacter(s != 0 ? 6 : 7, m->named.parent1, m->named.parent2, m->named.age, 0, m->named.job, 0, m->named.a8);
    if (k == 0xffff)
        return 1;
    cm_LastCharId = ai_CharsT[k].netId;
    strncpy(ai_CharsT[k].first, m->named.first, 0x10);
    c = &ai_CharsT[k];
    t = (AIFIN *)gm_PlayerRecord((char *)c);
    if (t != 0) {
        t->w00 = ai_CharsT[k].w050;
        strncpy(t->family, m->named.family, 0x10);
        strncpy(ai_CharsT[k].family, m->named.family, 0x10);
        r = gm_FindChar(ai_CharsT[k].relatives[1]);
        if (r != 0)
            strncpy(((AICHAR *)r)->family, m->named.family, 0x10);
        t->camAngle = -1.0f;
    }
    ai_CharsT[k].ref54 = m->named.ref54;
    ai_CharsT[k].b00c = m->named.b00c;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)c;
    }
    return 0;
}
/* 0x4c6e10, handler of type 0xd: the character taker takes over the character taken (gm_TakeOverCharacter); with s,
   a grave entry the taken one holds (gv_FindHeldEntry) is ordered buried, and s's value is the taker's record */

int cm_ExTakeOverCharacter(G17MSG *m, G17STAT *s)
{
    char *c;
    int k;

    k = gm_TakeOverCharacter(m->takeover.taker, m->takeover.taken);
    if (s != 0) {
        s->state = 1;
        if (k >= 0) {
            AIBLD *y;
            int *x;

            c = gm_FindChar(m->takeover.taken);
            if (gv_FindHeldEntry(c, (unsigned char **)&y, (int *)&x) != 0)     /* y the graveyard building, x the entry (gv.h GVENTRY, its size first) */
                gv_OrderBury((int *)c, (char *)y, *x, (unsigned char)gv_EntryIndex((char *)y->yard, x));
            s->kind = 1;
            s->value = (int)ai_Chars[k];
        } else {
            s->kind = 0;
            s->value = 0;
        }
    }
    return 0;
}
/* 0x4c6ec0, handler of type 0xe: removes what the message's object id names (its building freed, its character
   killed) */

int cm_ExDeleteObject(G17MSG *m, G17STAT *s)
{
    ORDBLD *b;
    ORDOBJ *o;
    ORDCHAR *c;

    m->del.objectId = (unsigned short)cm_ResolveId((int)m, m->del.objectId);
    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->del.objectId) == 0)
        return 1;
    if (b != 0) {
        if (gm_FreeBuilding((char *)b) != 0)
            return 1;
    } else {
        if (o != 0)
            return 1;
        gm_KillSpieler(c->id, 1);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6f60, handler of type 0xf: takes the amount of the player's money good from the to holder (the payer
   cm_CheckMoveGoods checks) and adds it to the from holder, -1: none */

int cm_ExMoveGoods(G17MSG *m, G17STAT *s)
{
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->moveGoods.objectId;
    if (m->moveGoods.to != -1) {
        m->moveGoods.to = cm_ResolveId((int)m, m->moveGoods.to);
        if (gm_TakeObjekt(m->moveGoods.to, ai_PlayerSlotsT[m->moveGoods.player].moneyGood, m->moveGoods.amount) == 0)
            return 1;
    }
    if (m->moveGoods.from != -1) {
        m->moveGoods.from = cm_ResolveId((int)m, m->moveGoods.from);
        if (gm_AddObjekt(m->moveGoods.from, ai_PlayerSlotsT[m->moveGoods.player].moneyGood, m->moveGoods.amount) == 0)
            return 1;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7010, handler of type 0x10: as cm_ExMoveGoods (takes the player's money good from the to holder and adds
   it to the from holder) but unchecked: gm_AddNegative / gm_AddObjekt, no failure */

int cm_ExMoveGoodsUnchecked(G17MSG *m, G17STAT *s)
{
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->moveGoods.objectId;
    if (m->moveGoods.to != -1) {
        m->moveGoods.to = cm_ResolveId((int)m, m->moveGoods.to);
        gm_AddNegative(m->moveGoods.to, ai_PlayerSlotsT[m->moveGoods.player].moneyGood, m->moveGoods.amount);
    }
    if (m->moveGoods.from != -1) {
        m->moveGoods.from = cm_ResolveId((int)m, m->moveGoods.from);
        gm_AddObjekt(m->moveGoods.from, ai_PlayerSlotsT[m->moveGoods.player].moneyGood, m->moveGoods.amount);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c70b0, handler of type 0x11, "cm_ExSellObjekt": moves count of the goods item from the seller (a building's,
   an object's or a character's list, gm_FindById) to the buyer; with a price, the buyer pays count * price in the
   money good of the player slot to the seller (gm_TakeObjekt / gm_AddObjekt) and both book it. A store (an object of
   kind 0x2a / 0x116) of the own player character gives the status text of its building and a sound
   @deep Read whole against 0x4c70b0..0x4c78bd and rewritten typed (G17SELLMSG, AIOBJ / AIBLD / AICHAR /
   GMSTOCK / GMPLAYERSLOT): the same calls in the same order with the same arguments, stores, constants and
   strings; fixed on the way: one buf[0x200] for the sprintf texts and the status text (frame 0x228 now
   equal), the reserve check as one expression (the original spills the ListStock term), the room check of
   the buyer object shared by the store kinds and 0x1dd (one gm_FreeCapacity call), the goods record taken
   before o2->count++. Left: block layout and scheduling - the y2 = y store is laid out after the room
   check (0x4c735d, the original inline with a jmp), the return-1 tails the original shares (0x4c7594 /
   0x4c77ad jump to +0x66d, ours inline), the gm_TakeObjekt / txt_Format argument loads in another order
   (0x4c768b, 0x4c7865), register renamings (0x4c766f, 0x4c76d1, 0x4c77ed, 0x4c7841). Tried: if / else-if /
   goto forms of the room check (this one the closest, 179 -> 173 ins). (size: 7 of the instruction
   difference are shared / inline tails) */

int cm_ExSellObjekt(G17MSG *m, G17STAT *s)
{
    AICHAR *r1;
    AIOBJ *y;
    AIOBJ *o2;
    AIOBJ *ys;
    AIOBJ *y2;
    AIBLD *x2;
    AICHAR *z;
    AIBLD *x1;
    int *p;
    AIOBJ *o;
    AICHAR *r2;
    int *q;
    char buf[0x200];

    r1 = 0;
    y = 0;
    o2 = 0;
    ys = 0;
    y2 = 0;
    p = 0;
    x1 = 0;
    x2 = 0;
    z = 0;
    o = 0;
    r2 = 0;
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->sell.objectId;
    if (m->sell.seller != -1) {
        m->sell.seller = cm_ResolveId((int)m, m->sell.seller);
        if (gm_FindById((int *)&x1, (int *)&y, (int *)&z, m->sell.seller) == 0)
            return 1;
        if (x1 != 0) {
            p = &x1->objList;
            if (x1->owner != 0xffff)
                r1 = &ai_CharsT[x1->owner];
            else
                r1 = 0;
        } else if (y != 0) {
            if (y->type == 0x2a || y->type == 0x1db || y->type == 0x1dc || y->type == 0x116 || y->type == 0x1dd)
                ys = y;
            p = &y->list;
            r1 = (AICHAR *)gm_ObjectBuildingOwner((unsigned short *)y);
            if (r1 == 0)
                return 1;
        } else {
            r1 = z;
            p = &z->inventory;
        }
        o = (AIOBJ *)gm_Query((void *)*p, 1, 0, m->sell.item);
        if (o == 0) {
            sprintf(buf, "cm_ExSellObjekt(): Object not found: ObjProt:%i Menge:%i CmdCnt:", m->sell.item, m->sell.count, m->seq);
            return 1;
        }
        if (ys != 0 && gm_ListAmount((short *)ys, (char *)o) < m->sell.count) {
            sprintf(buf, "cm_ExSellObjekt() Not enough Objekts at source: ObjProt:%i Menge:%i CmdCnt:%i", m->sell.item, m->sell.count, m->seq);
            return 1;
        }
        if (o->count < m->sell.count) {
            sprintf(buf, "cm_ExSellObjekt() Not enough Objekts at source: ObjProt:%i Menge:%i CmdCnt:%i", m->sell.item, m->sell.count, m->seq);
            return 1;
        }
        if (m->sell.price != 0 && r1 != 0)
            gm_SumSlotAmounts((void *)r1->inventory, m->sell.player);
    }
    if (m->sell.buyer != -1) {
        m->sell.buyer = cm_ResolveId((int)m, m->sell.buyer);
        if (gm_FindById((int *)&x2, (int *)&y, (int *)&z, m->sell.buyer) == 0)
            return 1;
        if (x2 != 0) {
            q = &x2->objList;
            if (x2->owner != 0xffff)
                r2 = &ai_CharsT[x2->owner];
            else
                r2 = 0;
        } else if (y != 0) {
            if (y->type == 0x2a || y->type == 0x1db || y->type == 0x1dc || y->type == 0x116) {
                y2 = y;
                goto room;
            }
            if (y->type == 0x1dd) {
            room:
                if (gm_FreeCapacity((short *)y, m->sell.item, m->sell.count) < m->sell.count)
                    return 1;
            }
            q = &y->list;
            r2 = (AICHAR *)gm_ObjectBuildingOwner((unsigned short *)y);
        } else {
            q = &z->inventory;
            r2 = z;
        }
        if (ai_GoodsT[m->sell.item].type == 0x1d)
            o2 = 0;
        else
            o2 = (AIOBJ *)gm_Query((void *)*q, 1, 0, m->sell.item);
        if (m->sell.price != 0 && r2 != 0) {
            if (gm_SumSlotAmounts((void *)r2->inventory, m->sell.player) < m->sell.count * m->sell.price)
                return 1;
        }
    }
    if (ys != 0) {
        if (r2 == 0)
            goto money;
        if ((AICHAR *)gm_ObjectBuildingOwner((unsigned short *)o) != r2 && o->reserve != 0) {
            if (gm_ListAmount((short *)ys, (char *)o) - ((unsigned int)(gm_ListStock((short *)ys, o->type) * o->reserve) >> 2)
                < (unsigned int)m->sell.count)
                return 1;
        }
    }
    if (r2 != 0 && x2 == 0 && y2 == 0 && y == 0) {
        m->sell.count = gm_TakeableCount((char *)r2, m->sell.item, m->sell.count);
        if (m->sell.count == 0)
            return 1;
    }
money:
    if (m->sell.price != 0) {
        GMSTOCK *g;

        if (y2 == 0 && y != 0 && y->type == 0x1dd) {
            g = (GMSTOCK *)gm_FindGoods(ai_OwnPlayer, m->sell.item);
            if (g != 0)
                g->daily[2] += m->sell.count;
        }
        if (ys != 0 && ys->type == 0x1dd) {
            g = (GMSTOCK *)gm_FindGoods(ai_OwnPlayer, m->sell.item);
            if (g != 0)
                g->daily[3] += m->sell.count;
        }
    }
    if (m->sell.seller != -1) {
        o->count -= m->sell.count;
        if (o->count <= 0) {
            if (ai_GoodsT[m->sell.item].type != 2 && ai_GoodsT[m->sell.item].type != 6) {
                gm_RemoveObject(p, m->sell.item);
            } else {
                if (x1 == 0)
                    return 1;
                gm_RemoveObjectFromBuilding((int)x1, m->sell.item);
            }
        }
        if (m->sell.price != 0) {
            gm_AddObjekt(r1->netId, ai_PlayerSlotsT[m->sell.player].moneyGood, m->sell.count * m->sell.price);
            if (ys != 0 && ys->type == 0x1db)
                r1->v1a4 += m->sell.count * m->sell.price;
            else if (ys != 0 && ys->type == 0x1dc)
                r1->v1a0 += m->sell.count * m->sell.price;
            else
                r1->wage += m->sell.count * m->sell.price;
        }
    }
    if (m->sell.buyer != -1) {
        if (o2 != 0) {
            o2->count += m->sell.count;
        } else if (ai_GoodsT[m->sell.item].type == 2 || ai_GoodsT[m->sell.item].type == 6) {
            if (x2 == 0)
                return 1;
            o2 = (AIOBJ *)gm_AllocRaum((unsigned char *)x2, m->sell.item);
            if (o2 == 0)
                return 1;
        } else {
            o2 = (AIOBJ *)gm_CreateObject(m->sell.buyer, m->sell.item, m->sell.count);
            if (o2 == 0)
                return 1;
            if (y2 != 0) {
                AIGOOD *gd = &ai_GoodsT[m->sell.item];

                o2->count++;
                if ((gd->type == 0x17 || gd->type == 0x25)
                    && (y2->type == 0x2a || y2->type == 0x116) && r2 != 0 && (r2->kind == 6 || r2->kind == 7))
                    o2->reserve = 4;
            }
        }
        cm_LastObjectId = o2->objId;
        if (m->sell.price != 0 && r2 != 0) {
            gm_TakeObjekt(r2->netId, ai_PlayerSlotsT[m->sell.player].moneyGood, m->sell.count * m->sell.price);
            if (y2 != 0 && y2->type == 0x1db)
                r2->v1a0 += m->sell.count * m->sell.price;
            else if (y2 != 0 && y2->type == 0x1dc)
                r2->v1a4 += m->sell.count * m->sell.price;
            else
                r2->spent198 += m->sell.count * m->sell.price;
        }
    }
    if (ys != 0 && (ys->type == 0x2a || ys->type == 0x116) && r1 != 0 && m->sell.price != 0
        && r1->id == ai_PlayerId) {
        x1 = (AIBLD *)gm_FindBuildingById(ys->ownerId);
        if (x1 != 0) {
            txt_Format(buf, 0x7c, x1->type * 14 + 0x48e, m->sell.price);
            sv_SetStatusText(buf);
            sb_PlaySampleI(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        }
    }
    if (s != 0) {
        s->state = 1;
        if (m->sell.buyer != -1) {
            s->kind = 3;
            s->value = (int)o2;
        }
    }
    return 0;
}
/* 0x4c78c0, handler of type 0x12: the workshop object y (kind 0x2a / 0x116, its building's owner r) makes up to
   m->make.runs runs of the item: as many as each input (AIGOOD.in, AIGOOD.need of it per run) in y's list and the
   room for the output allow (state kind 8: none); the inputs leave y's list (an emptied one is removed) and are
   booked as used (GMSTOCK.daily[1]), the output joins it (a new object: kind 6 of the state when that fails; one
   more in a store 0x2a; reserve 4 for the goods kinds 0x17 / 0x25 of an owner of kind 6 / 7) and is booked as
   made (GMSTOCK.daily[0]); its price value goes to the building's round sum and the owner's AIFIN.madeValue */

int cm_ExMakeGoods(G17MSG *m, G17STAT *s)
{
    AIOBJ *y;
    AIGOOD *g;
    int n;
    AICHAR *r;
    unsigned short *pp;
    AIOBJ *o;
    GMSTOCK *st;
    AIBLD *b;
    AIFIN *f;
    int i, v;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->make.objectId;
    m->make.objId = cm_ResolveId((int)m, m->make.objId);
    if (gm_FindById(0, (int *)&y, 0, m->make.objId) == 0)
        return 1;
    if (y->type != 0x2a && y->type != 0x116)
        return 1;
    r = (AICHAR *)gm_ObjectBuildingOwner((unsigned short *)y);
    if (r == 0)
        return 1;
    g = &ai_GoodsT[m->make.item];
    n = m->make.runs;
    pp = g->need;
    for (i = 4; i != 0; i--, pp++) {
        if (pp[4] != 0) {
            o = (AIOBJ *)gm_Query((void *)y->list, 1, 0, pp[4]);
            if (o != 0) {
                if (n >= gm_ListAmount((short *)y, (char *)o) / *pp)
                    n = gm_ListAmount((short *)y, (char *)o) / *pp;
            } else {
                n = 0;
            }
        }
    }
    v = gm_FreeCapacity((short *)y, m->make.item, g->batch * n) / g->batch;
    if (n >= v)
        n = v;
    if (n == 0) {
        if (s != 0)
            s->kind = 8;
        return 1;
    }
    for (i = 0; i < 4; i++) {
        if (g->in[i] != 0) {
            o = (AIOBJ *)gm_Query((void *)y->list, 1, 0, g->in[i]);
            if (o == 0) {
                if (s != 0)
                    s->kind = 8;
                return 1;
            }
            o->count -= g->need[i] * n;
            st = (GMSTOCK *)gm_FindGoods(ai_OwnPlayer, g->in[i]);
            if (st != 0)
                st->daily[1] += g->batch * n;
            if (o->count == 0)
                gm_RemoveObject(&y->list, g->in[i]);
        }
    }
    o = (AIOBJ *)gm_Query((void *)y->list, 1, 0, m->make.item);
    if (o != 0) {
        o->count += g->batch * n;
    } else {
        o = (AIOBJ *)gm_CreateObject(y->objId, m->make.item, g->batch * n);
        if (o == 0) {
            if (s != 0)
                s->kind = 6;
            return 1;
        }
        if (y->type == 0x2a)
            o->count++;
        if ((g->type == 0x17 || g->type == 0x25) && (y->type == 0x2a || y->type == 0x116) && (r->kind == 6 || r->kind == 7))
            o->reserve = 4;
    }
    v = (int)(gm_ItemPrice(m->make.item, 100) * n);
    b = (AIBLD *)gm_ObjectBuilding((int)y);
    if (b != 0)
        b->roundSums[0] += v;
    st = (GMSTOCK *)gm_FindGoods(ai_OwnPlayer, m->make.item);
    if (st != 0)
        st->daily[0] += g->batch * n;
    f = (AIFIN *)gm_PlayerRecord((char *)r);
    if (f != 0)
        f->madeValue += v;
    if (s != 0) {
        s->state = 1;
        if (m->make.objId != -1) {
            s->kind = 3;
            s->value = (int)o;
        }
    }
    return 0;
}
/* 0x4c7c30, handler of type 0x13: moves the object m->move.objId from the list of the holder m->move.from (a
   building's, an object's or a character's, gm_FindById) to the front of the list of m->move.to and makes that its
   owner; 1 when a holder or the object is not found */

int cm_ExMoveObject(G17MSG *m, G17STAT *s)
{
    AIBLD *x;
    AIOBJ *y;
    AICHAR *z;
    int *from;
    int *to;
    AIOBJ *prev;

    m->move.from = cm_ResolveId((int)m, m->move.from);
    if (gm_FindById((int *)&x, (int *)&y, (int *)&z, m->move.from) == 0)
        return 1;
    if (x != 0)
        from = &x->objList;
    else if (y != 0)
        from = &y->list;
    else
        from = &z->inventory;
    m->move.to = cm_ResolveId((int)m, m->move.to);
    if (gm_FindById((int *)&x, (int *)&y, (int *)&z, m->move.to) == 0)
        return 1;
    if (x != 0)
        to = &x->objList;
    else if (y != 0)
        to = &y->list;
    else
        to = &z->inventory;
    m->move.objId = cm_ResolveId((int)m, m->move.objId);
    prev = 0;
    for (y = (AIOBJ *)*from; y != 0; y = y->next) {
        if (y->objId == m->move.objId) {
            if (prev == 0)
                *from = (int)y->next;
            else
                prev->next = y->next;
            break;
        }
        prev = y;
    }
    if (y == 0)
        return 1;
    y->ownerId = m->move.to;
    y->next = (AIOBJ *)*to;
    *to = (int)y;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7d80, handler of type 0x14: removes the message's item from the workshop object's list when the list
   holds exactly one of it */

int cm_ExRemoveGoods(G17MSG *m, G17STAT *s)
{
    ORDOBJ *y;
    AIOBJ *o;

    m->removeGoods.objId = cm_ResolveId((int)m, m->removeGoods.objId);
    if (gm_FindById(0, (int *)&y, 0, m->removeGoods.objId) == 0)
        return 1;
    if (y->type != 0x2a && y->type != 0x116)
        return 1;
    o = (AIOBJ *)gm_Query((void *)y->list, 1, 0, m->removeGoods.item);
    if (o == 0)
        return 1;
    if (o->count != 1)
        return 1;
    gm_RemoveObject(&y->list, m->removeGoods.item);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7e20, handler of type 0x15: creates an object of the message's item in the holder and copies its 0x1f-byte
   kind data into it */

int cm_ExCreateObject(G17MSG *m, G17STAT *s)
{
    AIOBJ *o;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->createObj.objectId;
    m->createObj.holder = cm_ResolveId((int)m, m->createObj.holder);
    o = (AIOBJ *)gm_CreateObject(m->createObj.holder, m->createObj.item, 1);
    if (o == 0)
        return 1;
    cm_LastObjectId = o->objId;
    memcpy(&o->lvl1c, m->createObj.block, 0x1f);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7e90, handler of type 0x16 (cm_SendFieldDeltas): adds the field block's deltas to the object m->fields.id (a
   building, an object or a character, gm_FindById): per field (its head: a signed size byte, a count byte, an unsigned short offset) its count of byte,
   short or int values onto the object's bytes at the offset (another size is skipped) */

int cm_ExFieldDeltas(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *base;
    union {
        char *b;
        short *s;
        int *i;
    } d;
    union {
        unsigned char *b;
        unsigned short *w;
        short *s;
        int *i;
    } p;
    unsigned int i, j, n;
    int size, id;

    id = cm_ResolveId((int)m, m->fields.id);
    if (gm_FindById((int *)&x, (int *)&y, (int *)&z, id) == 0)
        return 1;
    m->fields.id = id;
    if (x != 0)
        base = x;
    else if (y != 0)
        base = y;
    else
        base = z;
    p.b = m->fields.buf;
    for (i = 0; i < m->fields.n; i++) {
        size = (char)*p.b++;
        n = (char)*p.b++;
        d.b = base + *p.w++;
        switch (size) {
        case 1:
            for (j = 0; j < n; j++)
                *d.b++ += *p.b++;
            break;
        case 2:
            for (j = 0; j < n; j++)
                *d.s++ += *p.s++;
            break;
        case 4:
            for (j = 0; j < n; j++)
                *d.i++ += *p.i++;
            break;
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7f70, handler of type 0x17 (cm_SendFieldValues): writes the field block's values to the object m->fields.id (a
   building, an object or a character, gm_FindById): per field (its head: a signed size byte, a count byte, an unsigned
   short offset) its count of byte, short or int values over the object's bytes at the offset (another size is skipped) */

int cm_ExFieldValues(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *base;
    union {
        char *b;
        short *s;
        int *i;
    } d;
    union {
        unsigned char *b;
        unsigned short *w;
        short *s;
        int *i;
    } p;
    unsigned int i, j, n;
    int size;

    m->fields.id = cm_ResolveId((int)m, m->fields.id);
    if (gm_FindById((int *)&x, (int *)&y, (int *)&z, m->fields.id) == 0)
        return 1;
    if (x != 0)
        base = x;
    else if (y != 0)
        base = y;
    else
        base = z;
    p.b = m->fields.buf;
    for (i = 0; i < m->fields.n; i++) {
        size = (char)*p.b++;
        n = (char)*p.b++;
        d.b = base + *p.w++;
        switch (size) {
        case 1:
            for (j = 0; j < n; j++)
                *d.b++ = *p.b++;
            break;
        case 2:
            for (j = 0; j < n; j++)
                *d.s++ = *p.s++;
            break;
        case 4:
            for (j = 0; j < n; j++)
                *d.i++ = *p.i++;
            break;
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8050, handler of type 0x18: adds the (k, value) pairs of the field block (a k byte, a float each) to the
   character m->fields.id's mood[k] values, each kept in 0..1000 */

int cm_ExMoodDeltas(G17MSG *m, G17STAT *s)
{
    AICHAR *chr;
    union {
        unsigned char *b;
        float *f;
    } p;
    unsigned int i, k;

    m->fields.id = cm_ResolveId((int)m, m->fields.id);
    if (gm_FindById(0, 0, (int *)&chr, m->fields.id) == 0)
        return 1;
    p.b = m->fields.buf;
    for (i = 0; i < m->fields.n; i++) {
        k = *p.b++;
        chr->mood[k][2] = *p.f++ + chr->mood[k][2];
        chr->mood[k][2] = min(max(0.0, chr->mood[k][2]), 1000.0);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8110, handler of type 0x19: clears the bits clear of the field at offset (size bytes: 1, 2 or 4) of the object
   objId, then sets the bits set; the message's state goes 1 */

int cm_ExSetField(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z;
    union {
        char *c;
        unsigned char *b;
        unsigned short *w;
        int *i;
    } a;

    m->setField.objId = cm_ResolveId((int)m, m->setField.objId);
    if (gm_FindById(&x, &y, &z, m->setField.objId) == 0)
        return 1;
    a.c = x;
    if (a.c == 0) {
        a.c = y;
        if (a.c == 0)
            a.c = z;
    }
    a.c += m->setField.offset;
    switch (m->setField.size) {
    case 1:
        *a.b &= ~(unsigned char)m->setField.clear;
        *a.b |= (unsigned char)m->setField.set;
        break;
    case 2:
        *a.w &= ~(unsigned short)m->setField.clear;
        *a.w |= (unsigned short)m->setField.set;
        break;
    case 4:
        *a.i &= ~m->setField.clear;
        *a.i |= m->setField.set;
        break;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c81d0, handler of type 0x1a: adds the message's delta to the float field at offset of the object id (a
   building, an object or a character, gm_FindById); the message's state goes 1 */

int cm_ExFloatDelta(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z;
    union {
        char *c;
        float *f;
    } a;

    m->floatDelta.id = cm_ResolveId((int)m, m->floatDelta.id);
    if (gm_FindById(&x, &y, &z, m->floatDelta.id) == 0)
        return 1;
    a.c = z;
    if (a.c == 0) {
        a.c = y;
        if (a.c == 0)
            a.c = x;
    }
    a.c += m->floatDelta.field;
    *a.f += m->floatDelta.delta;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8240, handler of type 0x1b (cm_SendRelations): changes the relations ai_Relations / ai_RelationMemory between
   the characters m->rel.from and m->rel.to (netIds); kind 4 drifts every relation towards 0 by the level (the
   higher, the narrower the band left alone), 3 scales everyone's memory towards `to` (but its partner's) by scale,
   0..2 add delta to from's relation to `to` (1 half of it, a positive one, to the memory as well, 2 clears the
   memory); each kept in -0x7f..0x7f */

int cm_ExRelations(G17MSG *m, G17STAT *s)
{
    int from, to, kind, i, j, v, hi, lo, dn, dp;
    unsigned int n, ci, cj;

    from = cm_ResolveId((int)m, m->rel.from);
    m->rel.from = from;
    to = cm_ResolveId((int)m, m->rel.to);
    kind = m->rel.kind;
    m->rel.to = to;
    if (kind == 4) {
        n = min(max(m->rel.level, 0), 4);
        if (n == 4) {
            hi = 0x28;
            lo = -0x73;
            dn = -0x14;
            dp = 3;
        } else if (n == 3) {
            hi = 0x37;
            lo = -0x69;
            dn = -0x10;
            dp = 5;
        } else if (n == 2) {
            hi = 0x46;
            lo = -0x5f;
            dn = -0xc;
            dp = 7;
        } else if (n == 1) {
            hi = 0x55;
            lo = -0x55;
            dn = -8;
            dp = 9;
        } else {
            hi = 100;
            lo = -0x4b;
            dn = -5;
            dp = 0xb;
        }
        for (i = 0; i < 0x300; i++) {
            for (j = 0; j < 0x300; j++) {
                v = ai_Relations[i][j];
                if (i != j && ai_CharsT[i].id != 0xffff && ai_CharsT[j].id != 0xffff) {
                    if (v > hi)
                        v += dn;
                    else if (v < lo)
                        v += dp;
                    if (v <= -0x7f)
                        v = -0x7f;
                    else if (v >= 0x7f)
                        v = 0x7f;
                    ai_Relations[i][j] = (signed char)v;
                }
            }
        }
    } else {
        for (ci = 0; ci < 0x300; ci++)
            if (ai_CharsT[ci].netId == to)
                break;
        if (ci >= 0x300)
            return 1;
        if (ai_CharsT[ci].id == 0xffff)
            return 1;
        if (kind == 3) {
            v = ai_CharsT[ci].partner;
            for (j = 0; j < 0x300; j++)
                if (ai_CharsT[j].netId != v)
                    ai_RelationMemory[j][ci] = (signed char)(int)(ai_RelationMemory[j][ci] * m->rel.scale);
        } else {
            for (cj = 0; cj < 0x300; cj++)
                if (ai_CharsT[cj].netId == from)
                    break;
            if (cj >= 0x300)
                return 1;
            if (ai_CharsT[cj].id == 0xffff)
                return 1;
            if (kind == 2) {
                v = ai_Relations[cj][ci];
                ai_RelationMemory[cj][ci] = 0;
                v += m->rel.delta;
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[cj][ci] = (signed char)v;
            } else if (kind == 1) {
                v = ai_Relations[cj][ci] + m->rel.delta;
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[cj][ci] = (signed char)v;
                if (m->rel.delta >= 0)
                    v /= 2;
                v += ai_RelationMemory[cj][ci];
                if (v < -0x7f)
                    ai_RelationMemory[cj][ci] = -0x7f;
                else if (v >= 0x7f)
                    ai_RelationMemory[cj][ci] = 0x7f;
                else
                    ai_RelationMemory[cj][ci] = (signed char)v;
            } else if (kind == 0) {
                v = ai_Relations[cj][ci] + m->rel.delta;
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[cj][ci] = (signed char)v;
            }
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c85c0, handler of type 0x1c: creates the job / message box of the long message (AIMSG, 0xf8 bytes copied
   from cm_LongMsg) and its texts (after it in cm_LongMsg), and adopts the message's next event id */

int cm_ExLongMessage(G17MSG *m, G17STAT *s)
{
    AIMSG msg;
    AICHAR *c;
    AIJOB *ev;
    char *t1, *t2;

    memcpy(&msg, cm_LongMsg, sizeof msg);
    c = (AICHAR *)gm_FindChar(msg.to);
    if (net_Conn.s != INVALID_SOCKET) {
        if (msg.flags & 1) {
            if (c == 0)
                return 1;
            if (c->kind == 7)
                return 1;
            if (c->kind != 6) {
                if (s == 0)
                    return 1;
                goto set;
            }
        }
        if (s == 0 && (msg.flags & 2))
            msg.flags = (msg.flags & ~2) | 4;
set:
        ai_NextEventId = m->nextEventId;
    }
    if (msg.type == 0x11 && he_IsMessageKindFiltered(msg.arg[0]) != 0)
        return 0;
    ev = (AIJOB *)he_CreateEvent((unsigned char *)&msg);
    if (ev == 0)
        return 1;
    if (ev->kind == 0x11) {
        t1 = (char *)&cm_LongMsg[0xf8];
        if (ev->modes & 0x10) {
            t2 = t1 + strlen(t1) + 1;
            ev->text = mem_Alloc(strlen(t1) + strlen(t2) + 2, "messagebox");
            strcpy(ev->text, t1);
            strcpy(ev->text + strlen(t1) + 1, t2);
            ev->textSize = strlen(t2) + strlen(t1) + 2;
        } else {
            ev->text = mem_Alloc(strlen(t1) + 1, "messagebox");
            strcpy(ev->text, t1);
            ev->textSize = strlen(ev->text) + 1;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 4;
        s->value = (int)ev;
    }
    return 0;
}
/* 0x4c87a0, handler of type 0x1d ("cm_ExSetHE"): sets the start, end and clock60 times, the stage and the argument
   block of the event whose id the message names */

int cm_ExSetHE(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    AIJOB *ev;

    if (s != 0) {
        s->state = 2;
        s->kind = 4;
        s->value = 0;
    }
    ev = (AIJOB *)he_FindEvent(1, 1, m->setHE.id);
    if (ev == 0) {
        sprintf(buf, "cm_ExSetHE(): He not found. id:%i name:%s", m->setHE.id, m->setHE.name);
        return 1;
    }
    ev->start = m->setHE.start;
    ev->end = m->setHE.end;
    ev->clock60 = m->setHE.clock60;
    ev->stage = (char)m->setHE.kind;
    memcpy(&ev->u, cm_LongMsg, 0xa0);
    if (s != 0) {
        s->state = 1;
        s->kind = 4;
        s->value = (int)ev;
    }
    return 0;
}
/* 0x4c88a0, handler of type 0x1e: the server's game time (an AITIME); when it is later than ours, the clock moves
   on and the world's steps run (events, the rating base, the characters, the goods reserves, the master carts) */

int cm_ExTime(G17MSG *m, G17STAT *s)
{
    AICHAR *c;

    if (gm_CompareTime(&ai_Time, &m->time) < 0) {
        ai_Time = m->time;
        if (cm_RunEvents)
            he_RunEvents();
        if (cm_RunRating) {
            ord_AdvanceRatingBase();
            if (ai_Time.min % 6 == 0)
                gm_CountCharacters();
        }
        if (cm_RunCharacters)
            ai_UpdateNeeds();
        memset(&ai_Watch, 0, 0xb00);
        if (cm_RunCharacters) {
            ai_TradeMsgCount = 0;
            for (c = (AICHAR *)ai_Chars; (int)c < (int)ai_Chars[0x300]; c++) {
                if (c->id != 0xffff)
                    ai_RunMasterEvaluation(c);
            }
            ai_UpdateCharacters();
        }
        lc_UpdateGoodsReserves();
        sv_MarkMasterCarts();
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8990, handler of type 0x1f: the game is over, back to the main loop (reasoned: the server's end-of-game
   message, the same pop as ai_QuitGame and the closed connection) */

int cm_ExGameOver(G17MSG *m, G17STAT *s)
{
    sv_PopTo(main_GameLoop);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c89b0, handler of type 0x20, the system message, on a copy of it by its kind: 0 seeds the random numbers and
   sets up the weather, 2 the day end request (and the net flags 8 / 0x10), 3 sets the game clock (with the clock
   update off), 4 shows a building's scene, 5 seeds, 6 the number of players, 7 the net player, 8 / 10 take / end
   the mission partner (2 in the state when it is not ours), 9 the mission mode, 12 rebuilds the default scene's
   height map, 13 colours the banners, 15 a save request, 16 a save acknowledgement, 17 counts a notice, 18 sets
   the game speed (0..4), 19 the player who left, 20 a pending election; the state 1 (2: refused) */

int cm_ExSysMessage(G17MSG *m, G17STAT *s)
{
    int ret = 1;
    unsigned char kind;
    int i, k, save;
    struct AIBLD *b;
    AIOBJ *o;

    {
        G17MSG msg = *m;

        kind = msg.sys.kind;
        switch (kind) {
        case 0: {
            AITIME t;

            memset(&t, 0, sizeof(t));
            t.year = msg.sys.arg[0];
            util_Seed(msg.sys.arg[1]);
            sv_SetupWeather((struct AITIME *)&t, 1);
            break;
        }
        case 2:
            cm_DayEndRequest = 1;
            if (msg.sys.arg[0] != 0)
                cm_NetFlags = (cm_NetFlags & ~0x10) | 8;
            else
                cm_NetFlags = (cm_NetFlags & ~8) | 0x10;
            break;
        case 3:
            k = tb_IsFunctionOff(sv_UpdateClock);
            if (k == 0)
                tb_SetFunctionOff(sv_UpdateClock, 1);
            ai_Time.year = msg.sys.time.year;
            ai_Time.sec = msg.sys.time.sec;
            ai_Time.min = msg.sys.time.min;
            ai_Time.hour = msg.sys.time.hour;
            ai_TimeCopyT = ai_Time;
            if (k == 0)
                tb_SetFunctionOff(sv_UpdateClock, 0);
            break;
        case 4:
            b = (struct AIBLD *)gm_BuildQuery(1, 1, msg.sys.arg[0]);
            if (b != 0) {
                o = (AIOBJ *)gm_Query((void *)b->objList, 1, 1, msg.sys.arg[1]);
                if (o != 0)
                    init_ShowBuildingScene((unsigned char *)b, o->id);
                else
                    init_ShowBuildingScene((unsigned char *)b, 0xfd);
            }
            break;
        case 5:
            util_Seed(msg.sys.arg[0]);
            hst_SeedPending = 0;
            break;
        case 7:
            ai_NetPlayer = msg.sys.arg[0];
            break;
        case 6:
            ai_NumPlayers = msg.sys.b;
            break;
        case 8:
            if (ai_MissionPartner == -1) {
                k = msg.sys.arg[0];
                if (gm_FindChar(k) != 0) {
                    ai_MissionPartner = k;
                    break;
                }
            }
            ret = 2;
            break;
        case 9:
            ai_MissionMode = msg.sys.b;
            ai_MissionSeed = msg.sys.arg[1];
            break;
        case 10:
            if (ai_MissionPartner == msg.sys.arg[0]) {
                ai_MissionPartner = -1;
                if (msg.sys.arg[4] == 1)
                    ai_GameOver = 1;
                if (ai_MissionMode == 5)
                    ai_SetFirstMission(ai_CharsT[ai_PlayerId].netId, msg.sys.arg[8]);
            } else
                ret = 2;
            break;
        case 12:
            save = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            d3sm_Build((struct D3SM *)d3_DefaultScene->hmap, d3_Terrain);
            d3if_SelectScene(save, 1);
            ch_Saved0 = 0;
            break;
        case 13:
            sv_ColourBanners(msg.sys.arg[0]);
            break;
        case 15:
            cm_RequestedSaveSlot = msg.sys.save.slot;
            i = 0;
            do
                cm_RequestedSaveName[i] = msg.sys.save.name[i];
            while (msg.sys.save.name[i++] != 0);
            break;
        case 16:
            for (i = 0; i < 8; i++) {
                if (cm_SaveAcks[i][0] == -1) {
                    cm_SaveAcks[i][0] = msg.sys.arg[0];
                    cm_SaveAcks[i][1] = msg.sys.arg[1];
                    break;
                }
            }
            break;
        case 17:
            if (ai_PlayerId != 0xffff) {
                if (ai_CharsT[ai_PlayerId].partyId != msg.sys.arg[0] || msg.sys.arg[0] == -1)
                    cm_NoticeCount++;
            }
            break;
        case 18:
            k = m->sys.arg[0];
            if (k >= 4)
                k = 4;
            cm_SpeedLevel = k;
            cm_SpeedTick = cm_SpeedTicks[k];
            sv_SetStatusText(txt_SpeedNames[k]);
            ai_ApplyOptions();
            break;
        case 19:
            cm_LeftPlayerId = msg.sys.arg[0];
            break;
        case 20:
            hmt_SetPendingElection(msg.sys.arg[0]);
            break;
        }
    }
    {
        char buf[0x80];

        sprintf(buf, "cm_ExSysMessage() type: %i", kind);
    }
    if (s != 0) {
        s->kind = 0;
        s->state = ret;
        s->value = 0;
    }
    return 0;
}
/* 0x4c8de0, handler of type 0x21: kills the character the message names (its netId) with its mode
   (cm_SendKillCharacter), after stopping the event script it runs */

int cm_ExKillCharacter(G17MSG *m, G17STAT *s)
{
    int ok = 0;
    AICHAR *c;

    if (s != 0)
        s->state = 2;
    c = (AICHAR *)gm_FindChar(m->kill.netId);
    if (c != 0) {
        ok = 1;
        sv_MoveToGroup(0, 0, c->id, 0);
        if (c->model != 0 && c->model->scriptId != -1)
            evt_Stop(evt_FindById(c->model->scriptId));
        gm_KillSpieler(c->id, m->kill.mode);
    }
    if (s != 0 && ok)
        s->state = 1;
    return 0;
}
/* 0x4c8e70, handler of type 0x22, "cm_ExAddStraftat": stores the crime m->crime in a free slot of ai_Crimes under a
   new id (the sender's when connected, else the next of ai_NextCrimeId); a culprit of kind 6 counts mission event
   0x17; without a place the victim's building (bld2, else bld) is it; the culprit's grid cells around the place get
   an amount (ord_AddGridAmount); every job 0x18 about the culprit gets the evidence, its character told (with a
   state); the state 1, kind 6, the record; -1 when ai_Crimes is full */

int cm_ExAddStraftat(G17MSG *m, G17STAT *s)
{
    char buf[0x200];
    CRIME *rec;
    AICHAR *c;
    AIBLD *b;
    AIJOB *j;
    int slot, place;
    int x, y;
    D3VECTOR4 pos;                      /* the engine pads its vectors to 16 bytes */

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextCrimeId = m->crime.id;
    else
        ai_NextCrimeId = ai_NextCrimeId + 1;
    slot = gv_FreeCrimeSlot();
    if (slot != -1) {
        c = (AICHAR *)gm_FindChar(m->crime.culprit);
        if (c != 0 && c->kind == 6)
            ai_CountMissionEvent(c, 0x17);
        rec = &((CRIME *)ai_Crimes)[slot];
        *rec = m->crime;
        rec->id = ai_NextCrimeId;
        sprintf(buf, "cm_ExAddStraftat(): Executed---- st_id = %i, taeter = %i, opfer = %i", ai_NextCrimeId,
                m->crime.culprit, m->crime.victim);
        place = m->crime.place;
        if (place == -1) {
            c = (AICHAR *)gm_FindChar(m->crime.victim);
            if (c != 0) {
                if (c->bld2 != 0)
                    place = c->bld2->id;
                else if (c->bld != 0)
                    place = c->bld->id;
                else
                    util_Seed(m->crime.seed);
            } else
                util_Seed(m->crime.seed);
        }
        ord_AddGridAmount(place, m->crime.law, &x, &y);
        gm_FindById((int *)&b, 0, 0, place);
        if (b == 0)
            ord_RandomCellPoint(x, y, &pos);
        else if (b->object != 0) {
            pos.x = ((D3OBJECT *)b->object)->pos.x;
            pos.y = ((D3OBJECT *)b->object)->pos.y;
            pos.z = ((D3OBJECT *)b->object)->pos.z;
        }
        for (j = (AIJOB *)he_FindEvent(1, 0, 0x18); j != 0; j = (AIJOB *)he_NextEvent()) {
            if (j->id4 == ((CRIME *)ai_Crimes)[slot].culprit) {
                if (gv_AddEvidence(rec->id, ai_CharsT[j->chr].netId) == 0)
                    break;
                if (s != 0) {
                    c = (AICHAR *)gm_FindChar(j->id4);
                    if (c != 0)
                        ai_NotifyPlayerOfChar((unsigned short *)c, (unsigned short *)&ai_CharsT[j->chr]);
                }
            }
        }
        if (s != 0) {
            s->state = 1;
            s->kind = 6;
            s->value = (int)rec;
        }
        return 0;
    }
    return -1;
}
/* 0x4c9090, handler of type 0x23: retires the crime the message names (cm_SendRetireCrime), force: even while it
   counts down; gv_RetireCrime's result 1 / 2 / 3 (it stays / is handed over / no such record) is returned as is */

int cm_ExRetireCrime(G17MSG *m, G17STAT *s)
{
    switch (gv_RetireCrime(m->retireCrime.id, m->retireCrime.force)) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c90d0, message 0x24: stores the proof pair (the crime id, the character's netId) in ai_Evidence */

int cm_ExProofExecuted(G17MSG *m, G17STAT *s)
{
    int i;

    i = gv_EvidenceSlot(m->proof.netId, m->proof.id);
    if (i > -1) {
        ai_Evidence[i][0] = m->proof.netId;
        ai_Evidence[i][1] = m->proof.id;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9110, message 0x25: drops the proof pair (the crime id, the character's netId) from ai_Evidence */

int cm_ExDropEvidence(G17MSG *m, G17STAT *s)
{
    int i;

    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == m->dropEvidence.netId && ai_Evidence[i][1] == m->dropEvidence.id) {
            ai_Evidence[i][0] = -1;
            ai_Evidence[i][1] = -1;
            if (s != 0)
                s->state = 1;
            return 0;
        }
    }
    return 1;
}
/* 0x4c9170, handler of type 0x26, "cm_ExSendCutInfo": the joined long message (a CMCUTINFO) becomes the cutscene
   record of the player m->cut.playerId when it belongs to the current fight; 1 when the player has no record */

int cm_ExSendCutInfo(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_CutInfosT[i].playerId == m->cut.playerId) {
            if (cm_CurrentFight != 0 && *cm_CurrentFight == m->cut.fightId) {
                cm_CutInfosT[i] = cm_LongMsgCut;
                cm_CutInfosT[i].recvFlag = 0;
                sprintf(buf, "cm_ExSendCutInfo: PlayerId:%i Valid:%i  Ready:%i  Time:%i", cm_CutInfosT[i].playerId,
                        cm_CutInfosT[i].valid, cm_CutInfosT[i].ready, d3_Time);
            }
            break;
        }
    }
    if (i >= 16) {
        if (cm_CurrentFight != 0 && *cm_CurrentFight == m->cut.fightId)
            sprintf(buf, "cm_ExSendCutInfo(): io-buffer has not been written, because player not found (player id %i)",
                    m->cut.playerId);
        return 1;
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 8;
        s->value = (int)&cm_CutInfosT[i];
    }
    return 0;
}
/* 0x4c9280, message 0x27: adds the joined party record (cm_LongMsg, 0x114 bytes) as a new scene party, with the
   party id and the random seed the message carries */

int cm_ExNewParty(G17MSG *m, G17STAT *s)
{
    char buf[0x114];
    G28REC *o;
    CMNEWPARTYMSG *d;
    int ret = 0;

    memcpy(buf, cm_LongMsg, 0x114);
    d = (CMNEWPARTYMSG *)m->data;
    if (net_Conn.s != -1)
        ai_NextPartyId = d->partyId;
    o = (G28REC *)cm_AllocParty(buf);
    if (o != 0) {
        if (ai_GameFlags & 4)
            o->flags |= 0x80;
        o->seed = d->seed;
        if (s != 0) {
            s->state = 1;
            s->kind = 9;
            s->value = (int)o;
        }
    } else
        ret = 1;
    if (o->kind == 5) {
        if (ai_CharInts[ai_PlayerId][0] != o->members[0] && !(ai_GameFlags & 4))
            o->flags &= 0x7f;
    }
    ai_NextPartyId++;
    return ret;
}
/* 0x4c9340, handler of type 0x28: allocates a cutscene ("cm_ExAllocCutscene") */

int cm_ExAllocCutscene(G17MSG *m, G17STAT *s)
{
    char buf[0x114];
    char *o;

    memcpy(buf, cm_LongMsg, 0x114);
    o = cm_AllocParty(buf);
    if (o == 0) {
        err_Message("cm_ExAllocCutscene(): Cutscene-Queue is full!");
        return 1;
    }
    if (ai_GameFlags & 4)
        o[0x26] |= 0x80;
    if (s != 0) {
        s->state = 1;
        s->kind = 9;
        s->value = (int)o;
    }
    return 0;
}
/* 0x4c93b0, message 0x29: adds the member netId (data[4]) to the scene party data[0] */

int cm_ExAddPartyMember(G17MSG *m, G17STAT *s)
{
    CMJOINPARTYMSG *d;

    d = (CMJOINPARTYMSG *)m->data;
    if (cm_PartyById(d->partyId) == 0)
        return 1;
    if (cm_AddPartyMember(d->partyId, d->member) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9400, message 0x2a: removes the member netId (data[4]) from the scene party data[0] */

int cm_ExLeaveParty(G17MSG *m, G17STAT *s)
{
    if (cm_PartyById(m->leaveParty.partyId) == 0)
        return 1;
    if (cm_RemovePartyMember(m->leaveParty.partyId, m->leaveParty.member) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9450, handler of type 0x2b: removes from the holder data[0] (gm_FindById: a building, a character or an
   object) the entry m->remove.entry (gm_RemoveOwned) - from a building's object list, or with an objId from the
   list of its object of that id; -2 when the building has no such object, -1 when the holder is unknown, else
   gm_RemoveOwned's non-0 result; 0 and the state 1 when removed (the state 2 before) */

int cm_ExRemoveOwned(G17MSG *m, G17STAT *s)
{
    AIBLD *bld;
    AIOBJ *obj;
    AICHAR *chr;
    int r;

    if (s != 0)
        s->state = 2;
    gm_FindById((int *)&bld, (int *)&obj, (int *)&chr, m->remove.holder);
    if (bld != 0) {
        if (m->remove.objId == -1)
            r = gm_RemoveOwned(&bld->objList, m->remove.entry);
        else {
            obj = (AIOBJ *)gm_Query((void *)bld->objList, 2, 6, 1, m->remove.objId);
            if (obj != 0)
                r = gm_RemoveOwned(&obj->list, m->remove.entry);
            else
                return -2;
        }
        if (r != 0)
            return r;
    } else if (chr != 0) {
        r = gm_RemoveOwned(&chr->inventory, m->remove.entry);
        if (r != 0)
            return r;
    } else if (obj != 0) {
        r = gm_RemoveOwned(&obj->list, m->remove.entry);
        if (r != 0)
            return r;
    } else
        return -1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9520, handler of type 0x2c: sets the plant m->plant at its cell of the building's vegetation map (in the
   building's scene, veg_SetPlant) when the type fits there (with fall always), stamping the building and the
   fallId; with fall a plant already felled by another (fallId != -1) refuses (1); without fall and with a fallId
   the plant felled with that id is put back (its object rebuilt, fallId -1). The state 1 (2: nothing set), kind
   5, the plant */

int cm_ExSetPlant(G17MSG *m, G17STAT *s)
{
    char r = 2;
    int fits = 0;
    int save = d3_CurrentScene;
    G15PLANT *o = 0;
    AIBLD *b;
    G15TREE *t;

    if (net_Conn.s != INVALID_SOCKET)
        veg_NextId = m->plant.id;
    b = (AIBLD *)gm_BuildQuery(1, 1, m->plant.bldId);
    if (b != 0) {
        t = veg_FindType(m->plant.type);
        if (t != 0 && m->plant.fall == 0) {
            if (veg_Fits((G15PLANT *)b->plants, m->plant.x, m->plant.z, t->size) != 0)
                fits = 1;
        } else
            fits = 1;
        if (m->plant.fall != 0) {
            o = veg_FindPlantAt((G15PLANT *)b->plants, m->plant.x, m->plant.z);
            if (o != 0 && o->fallId != -1)
                return 1;
        } else if (m->plant.fallId != -1) {
            o = veg_FindPlantAt((G15PLANT *)b->plants, m->plant.x, m->plant.z);
            if (o != 0 && o->fallId == m->plant.fallId) {
                d3if_SelectScene(init_BuildingUniverse[(unsigned char)ai_BTypesT[b->type].kind], 1);
                d3if_Delete((struct D3OBJECT *)o->obj);
                o->obj = 0;
                veg_LoadModel(o, 0);
                o->fallId = -1;
                d3if_SelectScene(save, 1);
            }
            if (s != 0) {
                s->value = (int)o;
                s->state = 1;
                s->kind = 5;
            }
            return 0;
        }
        if (fits != 0) {
            d3if_SelectScene(init_BuildingUniverse[(unsigned char)ai_BTypesT[b->type].kind], 1);
            o = veg_SetPlant((G15PLANT *)b->plants, m->plant.x, m->plant.z, m->plant.type, m->plant.fall);
            d3if_SelectScene(save, 1);
        }
        if (o != 0) {
            r = 1;
            o->bldId = b->id;
            if (m->plant.fall != 0)
                o->fallId = m->plant.fallId;
        }
    }
    if (s != 0) {
        s->state = r;
        s->kind = 5;
        s->value = (int)o;
    }
    return 0;
}
/* 0x4c9740, message 0x2d: creates the church's sermon object (kind 0x1b5) in the building owner (data[4], resolved
   by cm_ResolveId), copies the sermon settings (data[8], AISERMON) into it and sets it 0x40 percent full */

int cm_ExCreateSermon(G17MSG *m, G17STAT *s)
{
    AIOBJ *o;
    int k;

    if (net_Conn.s != -1)
        ai_NextObjectId = m->obj.objectId;
    k = cm_ResolveId((int)m, m->obj.owner);
    m->obj.owner = k;
    o = (AIOBJ *)gm_CreateObject(k, 0x1b5, 1);
    if (o == 0)
        return 1;
    cm_LastObjectId = o->objId;
    o->fill = 0x40;
    o->sermon = m->obj.value.sermon;
    o->sermon.enabled = 0;
    if (s != 0) {
        s->state = 1;
        s->kind = 3;
        s->value = (int)o;
    }
    return 0;
}
/* 0x4c97c0, message 0x2e: the character sp_<netId> (data[0]) gets the action 0x2d to walk to the cell (x, z) in the
   scene (data[4], data[8], data[0xc]) */

int cm_ExChrWalkTo(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", m->walk.netId);
    p = ch_FindByName(buf);
    if (p != 0) {
        ch_AddAction(p, 0x2d, m->walk.x, m->walk.z, m->walk.scene);
        if (s != 0) {
            s->state = 2;
            s->kind = 0;
            s->value = 0;
        }
    }
    return 0;
}
/* 0x4c9830, handler of message 0x2f (cm_RequestChrMove): the character "sp_<netId>" walks to the object `name`
   (objId: the object instance, -1: none) of the building bldId and sits down, or gets the action 0x39 */

int cm_ExChrWalkToDummy(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    CHARACTER *p;
    D3OBJECT *o;
    CHACTION *r;
    int i = 0;
    char *b = 0;
    int save;

    sprintf(buf, "sp_%i", m->place.netId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    if (m->place.objId != -1)
        i = gm_FindObjectInst(m->place.objId);
    else
        b = gm_FindBuildingById(m->place.bldId);
    save = d3_CurrentScene;
    if (m->place.objId != -1) {
        i = init_ObjectSceneUniverse(i);
        if (i == -1)
            return 2;
    } else {
        if (m->place.bldId != -1) {
            i = init_BuildingSceneUniverse(b);
            if (i == -1)
                return 2;
        } else
            i = 0;
    }
    if (init_SceneDirty[i * 0x3e0] != 0)
        return 3;
    if (m->place.name[0] != 0) {
        d3if_SelectScene(i, 1);
        o = d3_FindObject(0, 0x160, 0, m->place.name);
        d3if_SelectScene(save, 1);
        if (o == 0)
            return 4;
        ch_StandUp(p, "sitzend/aufstehen");
        r = ch_WalkToObject(p, o);
        if (r != 0)
            memcpy(r->why, cm_WalkToDummyWhy, 20);
    } else {
        ch_StandUp(p, "sitzend/aufstehen");
        ch_AddAction(p, 0x39, i);
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c99b0, handler of message 0x30 (cm_RequestChrMoveToUniverse): the character "sp_<netId>" is sent to the
   building bldId / room objId with the action 0x33 (a move to the object `name`, or to the building's door) */

int cm_ExChrMoveToUniverse(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    CHARACTER *p;
    AIBLD *b = 0;
    AIOBJ *q = 0;
    CHACTION *r;
    short *c;
    D3OBJECT *o;
    int x = -1;
    int k, v, save;

    sprintf(buf, "sp_%i", m->place.netId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    if (m->place.bldId != -1) {
        b = (AIBLD *)gm_BuildQuery(1, 1, m->place.bldId);
        if (b == 0)
            return 2;
    }
    if (m->place.objId != -1) {
        q = (AIOBJ *)gm_Query((void *)b->objList, 1, 1, m->place.objId);
        if (q == 0)
            return 3;
        if (q->id != 0xfd) {
            k = init_ObjectSceneUniverse((unsigned short *)q);
            goto chk;
        }
    }
    if (b != 0 && gm_IsHomeType((unsigned char *)b) != 0) {
        k = init_BuildingSceneUniverse((unsigned char *)b);
        q = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xfd);
        goto chk;
    }
    k = 0;
    goto body;
chk:
    if (k < 0)
        goto end;
body:
    v = -1;
    if (q != 0)
        v = q->objId;
    if (b != 0)
        x = b->id;
    if (k != 0) {
        c = gm_FindChar(m->place.netId);
        if (c != 0)
            gm_SelectModelVariant((char *)c);
    }
    ch_AddAction(p, 0x38, 0);
    if (k != 0) {
        r = ch_AddAction(p, 0x33, k, x, v);
        if (r != 0)
            strcpy(r->u.name, m->place.name);
    } else if (b->object != 0) {
        r = ch_AddAction(p, 0x33, 0, -1, -1);
        if (r != 0) {
            save = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            o = d3_FindObject((int)b->object, 0x100, 0, m->place.name);
            if (o != 0) {
                d3_PointToWorld(o, &o->pos, &r->move.pos);
                r->move.hasPos = 1;
            } else
                strcpy(r->u.name, m->place.name);
            d3if_SelectScene(save, 1);
        }
    }
    ch_AddAction(p, 0x38, 1);
end:
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9c10, handler of message 0x31 (cm_SendChrStop): stops the actions of the character "sp_<charId>" */

int cm_ExChrStop(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    CHARACTER *p;

    sprintf(buf, "sp_%i", m->stop.charId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 2;
    ch_StopActions(p);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9c80, handler of message 0x32: puts the character "sp_<charId>" at the position pos */

int cm_ExChrSetPos(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    CHARACTER *p;
    CMPOSMSG *d;

    d = (CMPOSMSG *)m->data;
    sprintf(buf, "sp_%i", d->charId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    ch_SetPos(p, &d->pos, 1);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9d00, handler of message 0x33 (cm_SendCartState): the cart "sp_<cartId>" gets the action 0x38 with state */

int cm_ExCartState(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    CHARACTER *p;

    sprintf(buf, "sp_%i", m->cart.cartId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    ch_AddAction(p, 0x38, m->cart.state);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9d80, handler of message 0x34 (cm_SendPlaceOnStage): places the character charId on the stage of the
   building bldId / room objId (gm_PlaceOnStage) */

int cm_ExPlaceOnStage(G17MSG *m, G17STAT *s)
{
    int o;

    o = gm_PlaceOnStage(m->stage.charId, m->stage.bldId, m->stage.objId, 0, 0);
    if (o == 0)
        return 1;
    if (s != 0) {
        s->state = 2;
        s->kind = 7;
        s->value = o;
    }
    return 0;
}

/* 0x4c9dc0, handler of message 0x35 ("cm_ExChrGotoBuilding"): sends the character "sp_<charId>" from the building
   it is in (group) to the building bldId and its room objId, queueing the actions on the way: out of a home
   building through the gate of its room (ch_UseGate) and its transporter, out of another building through its
   door ("dummy_EINGANG"), a walk (0x2d) to the door ("dummy_TUER") of the target, the scene change 0x33 between
   two 0x38 actions, then the room's gate or the sit-down 0x39. Each action gets the message's reason.
   @deep read whole against the original and fixed the meaning (NoCase door lookup on the target, the
   out-of-building branch compares with the 0x44 building, o kept from function start, the last gate
   through the start building cb, the objId/universe test as if/else, if/else FindObject calls VC6 merges,
   hm loaded before d3_PointToWorld, 16-byte vector frame). Frame equal now; left: instruction
   choice/scheduling of the inline strcpy into r->why (ours lea edx,[eax+0x70]; add ecx,0x3d; sub edx,ecx |
   the original add ecx,0x3d; sub eax,ecx; lea edx,[eax+0x70]) at every copy, ai_Goods index lea
   eax,[eax+edx+1] | add eax,edx; inc eax, the text test add eax,0x1d; cmp byte [eax],0 | mov
   cl,[edx+0x1d]; lea eax,[edx+0x1d]; test cl,cl, and loads scheduled before/after pushes (0x4ca0ec,
   0x4ca392, 0x4ca43c); dst spellings (char*)r+0x70, r->args+0x10, (char*)m+0x3d src tried, no change */
int cm_ExChrGotoBuilding(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char gate[0x80];
    D3VECTOR4 v;                        /* the engine's padded vector (the original's frame holds 16 bytes) */
    int x, z;
    float y;
    CHARACTER *p;
    AIBLD *from, *to, *cb;
    AIOBJ *q, *q2;
    D3OBJECT *o;
    CHACTION *r;
    D3SM *hm;
    int save, scene, k, i;

    cb = 0;
    o = 0;
    sprintf(buf, "sp_%i", m->go.charId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    from = (AIBLD *)gm_BuildQuery(1, 0, 0x44);
    if (m->go.bldId != -1) {
        to = (AIBLD *)gm_BuildQuery(1, 1, m->go.bldId);
        if (to == 0)
            return 1;
    } else
        to = from;
    if (p->group != -1)
        cb = (AIBLD *)gm_BuildQuery(1, 1, p->group);
    save = d3_CurrentScene;
    if (cb != 0 && gm_IsHomeType((unsigned char *)cb) != 0) {
        scene = d3t_SceneIndex(p->scene);
        d3if_SelectScene(scene, 1);
        if (scene < 0)
            return 0;
        if (p->scriptGroup != -1) {
            q = (AIOBJ *)gm_Query((void *)cb->objList, 1, 1, p->scriptGroup);
            if (q != 0 && q->id != 0xfd) {
                sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_GoodsT[q->id].name));
                q2 = (AIOBJ *)gm_Query((void *)cb->objList, 1, 0, 0xfd);
                k = init_BuildingSceneUniverse((unsigned char *)cb);
                if (k >= 0 && q2 != 0) {
                    ch_StandUp(p, "sitzend/aufstehen");
                    r = ch_UseGate(p, gate, "dummy_gate_ob_AUSSEN_RAUM", k, cb->id, q2->objId);
                    if (r != 0)
                        strcpy(r->why, m->go.reason);
                }
            }
        }
        if (to->id != cb->id) {
            k = init_BuildingSceneUniverse((unsigned char *)cb);
            if (k < 0)
                return 0;
            d3if_SelectScene(k, 1);
            o = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY");
            if (o != 0) {
                hm = (D3SM *)d3_Scenes[k].hmap;
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
                if (d3_GetHeightAt(hm, (D3VECTOR *)&v, &x, &z, &y) != 0) {
                    r = ch_AddAction(p, 0x2d, x, z, k);
                    if (r != 0)
                        strcpy(r->why, m->go.reason);
                }
            }
            ch_AddAction(p, 0x38, 0);
            r = ch_AddAction(p, 0x33, 0, -1, -1);
            ch_AddAction(p, 0x38, 1);
            if (r == 0) {
                d3if_SelectScene(save, 1);
                return 1;
            }
            strcpy(r->why, m->go.reason);
            if (cb->object != 0) {
                d3if_SelectScene(0, 1);
                o = d3_FindObject((int)cb->object, 0x100, 0, "dummy_TUER");
                if (o == 0)
                    o = d3_FindObjectNoCase((int)to->object, 0x100, 0, "dummy_TUER");
                if (o != 0)
                    d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
                else
                    d3_PointToWorld((D3OBJECT *)cb->object, &((D3OBJECT *)cb->object)->pos, (D3VECTOR *)&v);
                r->move.pos.x = v.x;
                r->move.pos.y = v.y;
                r->move.hasPos = 1;
                r->move.pos.z = v.z;
            }
        }
    } else if (p->group != from->id && p->group != -1) {
        scene = d3t_SceneIndex(p->scene);
        if (scene < 0)
            return 0;
        d3if_SelectScene(scene, 1);
        ch_StandUp(p, "sitzend/aufstehen");
        o = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (o != 0) {
            hm = (D3SM *)d3_Scenes[scene].hmap;
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
            if (d3_GetHeightAt(hm, (D3VECTOR *)&v, &x, &z, &y) != 0) {
                r = ch_AddAction(p, 0x2d, x, z, scene);
                if (r != 0)
                    strcpy(r->why, m->go.reason);
            }
        }
        ch_AddAction(p, 0x38, 0);
        r = ch_AddAction(p, 0x33, 0, -1, -1);
        ch_AddAction(p, 0x38, 1);
        if (r != 0) {
            strcpy(r->why, m->go.reason);
            strcpy(r->u.name, "dummy_TUER");
            if (cb != 0) {
                o = d3_FindObject((int)cb->object, 0x100, 0, "dummy_TUER");
                if (o != 0)
                    d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
                else if (cb->object != 0)
                    d3_PointToWorld((D3OBJECT *)cb->object, &((D3OBJECT *)cb->object)->pos, (D3VECTOR *)&v);
                else
                    err_Message("cm_ExChrGotoBuilding(): start->D3Object is NULL!");
                r->move.hasPos = 1;
                r->move.pos.x = v.x;
                r->move.pos.y = v.y;
                r->move.pos.z = v.z;
            }
        }
    }
    if (gm_IsHomeType((unsigned char *)to) == 0) {
        d3if_SelectScene(0, 1);
        if (m->go.text[0] != 0)
            o = d3_FindObject(0, 0x100, 0, m->go.text);
        else if (to->object != 0) {
            if (p->transport != 0)
                o = (D3OBJECT *)he_RandomTransporter(to->object);
            else {
                o = d3_FindObject((int)to->object, 0x100, 0, "dummy_TUER");
                if (o == 0)
                    o = d3_FindObjectNoCase((int)to->object, 0x100, 0, "dummy_TUER");
            }
            if (o == 0)
                o = (D3OBJECT *)to->object;
        }
        if (o != 0) {
            hm = (D3SM *)d3_Scenes[0].hmap;
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
            if (d3_GetHeightAt(hm, (D3VECTOR *)&v, &x, &z, &y) != 0) {
                r = ch_AddAction(p, 0x2d, x, z, 0);
                if (r != 0)
                    strcpy(r->why, m->go.reason);
            }
        }
        if (m->go.noObject == 0) {
            if (m->go.objId != -1)
                i = init_ObjectSceneUniverse((unsigned short *)gm_Query((void *)to->objList, 1, 1, m->go.objId));
            else
                i = 0;
            if (i >= 0) {
                if (m->go.objId == -1)
                    err_Message("cm_ExChrGotoBuilding(): invalid room-id (cm_ID_NONE)");
                else if (gm_Query((void *)to->objList, 1, 1, m->go.objId) == 0)
                    err_Message("cm_ExChrGotoBuilding(): invalid room-id (raum==NULL)");
                ch_AddAction(p, 0x38, 0);
                r = ch_AddAction(p, 0x33, i, to->id, m->go.objId);
                if (r != 0)
                    strcpy(r->why, m->go.reason);
                ch_AddAction(p, 0x38, 1);
            } else
                err_Message("cm_ExChrGotoBuilding(): Could not find universe...");
            r = ch_AddAction(p, 0x39, i);
            if (r != 0)
                strcpy(r->why, m->go.reason);
        }
    } else {
        if (to->id != p->group) {
            d3if_SelectScene(0, 1);
            o = d3_FindObject((int)to->object, 0x100, 0, "dummy_TUER");
            if (o == 0)
                o = d3_FindObjectNoCase((int)to->object, 0x100, 0, "dummy_TUER");
            if (o != 0) {
                hm = (D3SM *)d3_Scenes[0].hmap;
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
                if (d3_GetHeightAt(hm, (D3VECTOR *)&v, &x, &z, &y) != 0) {
                    r = ch_AddAction(p, 0x2d, x, z, 0);
                    if (r != 0)
                        strcpy(r->why, m->go.reason);
                }
            }
        }
        if (m->go.noObject == 0) {
            q = (AIOBJ *)gm_Query((void *)to->objList, 1, 1, m->go.objId);
            if (q == 0) {
                q = (AIOBJ *)gm_Query((void *)to->objList, 1, 0, 0xfd);
                if (q == 0) {
                    i = init_BuildingSceneUniverse((unsigned char *)to);
                    if (i >= 0) {
                        ch_AddAction(p, 0x38, 0);
                        r = ch_AddAction(p, 0x33, i, to->id, -1);
                        if (r != 0) {
                            strcpy(r->why, m->go.reason);
                            strcpy(r->u.name, "dummy_TRANSPORTER");
                        }
                        ch_AddAction(p, 0x38, 1);
                    }
                    goto done;
                }
            }
            if (q->id == 0xfd) {
                if (to->id != p->group) {
                    k = init_BuildingSceneUniverse((unsigned char *)to);
                    if (k < 0)
                        return 0;
                    ch_AddAction(p, 0x38, 0);
                    r = ch_AddAction(p, 0x33, k, to->id, q->objId);
                    if (r != 0) {
                        strcpy(r->why, m->go.reason);
                        memcpy(r->u.name, "dummy_TRANSPORTER_ENTRY", 24);
                    }
                    ch_AddAction(p, 0x38, 1);
                    d3if_SelectScene(k, 1);
                    if (p->transport != 0)
                        o = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER");
                    else
                        o = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
                    if (o != 0) {
                        hm = (D3SM *)d3_Scenes[k].hmap;
                        d3_PointToWorld(o, &o->pos, (D3VECTOR *)&v);
                        if (d3_GetHeightAt(hm, (D3VECTOR *)&v, &x, &z, &y) != 0) {
                            r = ch_AddAction(p, 0x2d, x, z, k);
                            if (r != 0)
                                strcpy(r->why, m->go.reason);
                        }
                    }
                }
            } else {
                i = init_ObjectSceneUniverse((unsigned short *)q);
                sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_GoodsT[q->id].name));
                if (cb != 0) {
                    r = ch_UseGate(p, "dummy_EINGANG", gate, i, cb->id, q->objId);
                    if (r != 0)
                        strcpy(r->why, m->go.reason);
                }
            }
        }
    }
done:
    d3if_SelectScene(save, 1);
    if (s != 0) {
        s->state = 1;
        s->kind = 7;
        s->value = (int)p;
    }
    return 0;
}
/* 0x4ca8a0, handler of message 0x36 (cm_SendChrUseGates): the character "sp_<charId>" goes from the room it is in
   (scriptGroup, else the building's main room 0xfd) through the gates "dummy_gate_ob_<goods name>" to the room objId
   of building bldId, through "dummy_gate_ob_AUSSEN_RAUM" when that is the main room */
int cm_ExChrUseGates(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char gate[0x80];
    char gate2[0x80];
    CHARACTER *p;
    AIBLD *b;
    AIOBJ *q, *r;
    AIGOOD *g;
    int k;

    sprintf(buf, "sp_%i", m->gate.charId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    b = (AIBLD *)gm_BuildQuery(1, 1, m->gate.bldId);
    q = (AIOBJ *)gm_Query((void *)b->objList, 1, 1, m->gate.objId);
    if (p->scriptGroup != -1)
        r = (AIOBJ *)gm_Query((void *)b->objList, 1, 1, p->scriptGroup);
    else
        r = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xfd);
    if (q->id == 0xfd) {
        k = init_BuildingSceneUniverse((unsigned char *)b);
        sprintf(gate2, "dummy_gate_ob_AUSSEN_RAUM");
    } else {
        g = &ai_GoodsT[q->id];
        k = init_ObjectSceneUniverse((unsigned short *)q);
        sprintf(gate2, "dummy_gate_ob_%s", _strupr(g->name));
    }
    sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_GoodsT[r->id].name));
    if (k >= 0)
        ch_UseGate(p, gate, gate2, k, b->id, q->objId);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4caa20, handler of message 0x37 (cm_SendChrAnim): queues the animation name on the character "sp_<charId>"
   count times */

int cm_ExChrAnim(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    CHARACTER *p;

    sprintf(buf, "sp_%i", m->anim.charId);
    p = ch_FindByName(buf);
    if (p == 0)
        return 2;
    ch_QueueAnim(p, m->anim.name, m->anim.count);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4caaa0, handler of message 0x38 (cm_SendOwnership): hands the building bldId to the owner ownerId with the
   master masterId (gm_AssignBuilding) */

int cm_ExOwnership(G17MSG *m, G17STAT *s)
{
    AIBLD *b;
    AICHAR *c1, *c2;

    b = (AIBLD *)gm_BuildQuery(1, 1, m->owner.bldId);
    if (b == 0)
        return 1;
    c1 = (AICHAR *)gm_FindChar(m->owner.ownerId);
    if (c1 == 0 && m->owner.ownerId != -1)
        return 1;
    c2 = (AICHAR *)gm_FindChar(m->owner.masterId);
    if (c2 == 0 && m->owner.masterId != -1)
        return 2;
    gm_AssignBuildingK(b, c2->id, c1->id);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cab40, handler of message 0x39 (cm_SendBoss): the building bldId becomes the home (bld2) of the character
   charId (-1: none); a home building (AIBTYPE.kind 1) counts its residents up to the capacity of its ord kind record
   (fails when full), the former home counts one down */
int cm_ExSetHome(G17MSG *m, G17STAT *s)
{
    int kind[2];
    AIBLD *b, *o;
    AICHAR *c;

    b = (AIBLD *)gm_BuildQuery(1, 1, m->home.bldId);
    if (b == 0)
        return 1;
    c = (AICHAR *)gm_FindChar(m->home.charId);
    if (c == 0 && m->home.charId != -1)
        return 1;
    if (ai_BTypesT[b->type].kind == 1) {
        ord_GetKindRecord(b->type, kind);
        if (b->residents >= (unsigned short)kind[0])
            return 1;
        b->residents++;
    }
    o = c->bld2;
    if (o != 0) {
        if (ai_BTypesT[o->type].kind == 1 && o->residents > 0)
            o->residents--;
    }
    c->bld2 = b;
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cac40, handler of message 0x3a ("cm_ExGebUpgrade"): the building bldId goes one level up (its type is the next of
   its line, refused at the line's maxLevel) and half way to a level of 100; capacities and stock follow its new type,
   a graveyard grows. Its model "<data>gebaeude/*gb_<NAME>[_A.._C].ogr" (_A / _B by the owner's sex for type 0x21)
   is loaded in the town scene and replaces the old object at its place and heading, with the upgrade effect
   script, the hooks, banners, the season's textures, torches, lights and a restarted chimney smoke. */
int cm_ExGebUpgrade(G17MSG *m, G17STAT *s)
{
    char name[0x80];
    char path[0x100];
    char found[0x100];
    D3VECTOR v;
    D3VECTOR rot;
    D3VECTOR dir;
    AIBLD *b;
    AIBTYPE *t;
    D3OBJECT *o;
    EVTSCRIPT *sc;
    float radius, key;
    int i, save, season, size;

    if (net_Conn.s != (unsigned int)-1)
        ai_NextObjectId = m->upgrade.nextObjId;
    b = (AIBLD *)gm_BuildQuery(1, 1, m->upgrade.bldId);
    if (b == 0)
        return 1;
    t = &ai_BTypesT[b->type];
    if (t->cap247 >= t->maxLevel) {
        err_Message("cm_ExSetGebUpgrade(): already highest level");
        return 1;
    }
    b->type++;
    b->level = b->level + (100 - b->level) / 2;
    gm_SetCapacities((unsigned char *)b);
    gm_StockBuildingObjects((char *)b);
    if (t->kind == 0x1d)
        gv_GrowGraveyard((unsigned char *)b);
    strcpy(name, "gb_");
    strcat(name, ai_BTypesT[b->type].name);
    _strupr(name + 2);
    for (i = 0; i < 4; i++) {
        if (t->kind == 0x21) {
            if (ai_CharsT[b->owner].sex == 1)
                sprintf(path, "%sgebaeude/*%s_A.ogr", ai_DataPath, name);
            else
                sprintf(path, "%sgebaeude/*%s_B.ogr", ai_DataPath, name);
        } else if (i != 0)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, name, (char)(i + 0x40));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, name);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, found) != 0) {
            save = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            d3_PointToWorld((D3OBJECT *)b->object, &((D3OBJECT *)b->object)->pos, &v);
            o = d3_LoadGroup(path, 0, 0);
            if (o == 0) {
                sprintf(name, "cm_ExGebUpgrade(): Could not load 3D-Object-Group '%s'...", path);
                err_Message(name);
            } else {
                d3_DirToWorld((struct XOBJ *)b->object, (float *)&d3_ZAxis, (float *)&dir);
                rot.x = 0;
                rot.y = 0;
                rot.z = 0;
                rot.y = d3_VecAngle(&d3_ZAxis, &dir);
                d3_SetObjectPos(o, &v);
                d3_SetObjectRot(o, &rot);
                ai_DropObjectAt(o, &v);
                if (d3_QuadTree != 0)
                    d3q_RemoveObject((D3OBJECT *)b->object, (D3QNODE *)d3_QuadTree);
                d3if_Delete((D3OBJECT *)b->object);
                if (o->info == 0)
                    d3_InflateObject(o);
                if (o->material != 0) {
                    if (d3t_GetBoundingSphere(o, d3_Camera, &v, &radius) != 0) {
                        size = (int)(radius * cm_EffectRadiusScale);
                        sc = evt_Load("Effekte/geb_upgrade.esc");
                        if (sc != 0) {
                            evt_RunScriptArgs(sc, 6, (int)v.x, (int)v.y, (int)v.z, (int)radius, size, o);
                            sc->group = 0;
                        }
                    }
                }
                init_HookObject((char *)o, (unsigned short *)b, 2);
                sv_CreateBanners((char *)b);
                if (b->object != 0) {
                    season = gm_ClockSeason(&ai_Time);
                    if (season == 1)
                        init_TextureSet = 1;
                    else if (season == 0)
                        init_TextureSet = 0;
                    else if (season == 2)
                        init_TextureSet = 2;
                    else if (season == 3)
                        init_TextureSet = 3;
                    init_TextureSetHook((int)b->object);
                    d3_ForEachObject2(d3_DefaultScene, (D3OBJECT *)b->object, 0x2c0, (void (*)(D3OBJECT *))init_StripTorches);
                    key = (float)sv_DayPhase + sv_DayPhaseFrac;
                    d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)b->object, 0x21e, (D3OBJECTCB)d3_SetLightKeyCB, &key);
                    d3_RelightObjects((D3OBJECT *)b->object);
                    if (b->smokeEffect != -1) {
                        sc = evt_FindById(b->smokeEffect);
                        if (sc != 0)
                            evt_Stop(sc);
                        b->smokeEffect = -1;
                        sv_UpdateChimneySmoke(b);
                    }
                }
            }
            d3if_SelectScene(save, 1);
            break;
        }
    }
    gm_RefreshGoodsLists(ai_OwnPlayer);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb100, message 0x3b (cm_SendBuildingDestroyed): destroys the building m->destroy.bldId - frees its carts,
   drops it from every character whose bld it is, deletes its scene object, stops its chimney smoke effect, then
   frees the building record */

int cm_ExBuildingDestroyed(G17MSG *m, G17STAT *s)
{
    int save = d3_CurrentScene;
    AIBLD *b;
    AIOBJ *o;
    AIBLD **bld;
    EVTSCRIPT *sc;

    b = (AIBLD *)gm_BuildQuery(1, 1, m->destroy.bldId);
    if (b == 0)
        return 1;
    if ((char *)b == sv_SelectedObject)
        sv_ResetSelection();
    for (o = (AIOBJ *)gm_Query(0, 2, 7, 4, 0x1d); o != 0; o = (AIOBJ *)gm_QueryNext()) {
        if (o->bldId == b->id)
            gm_FreeObject((int)o);
    }
    /* the bld of every character record (the end is the field of the record after the table) */
    for (bld = &ai_CharsT[0].bld; (int)bld < (int)&ai_CharsT[0x300].bld;
         bld = (AIBLD **)((char *)bld + sizeof(AICHAR))) {
        if (*bld == b)
            *bld = 0;
    }
    d3if_SelectScene(0, 1);
    if (d3_QuadTree != 0)
        d3q_RemoveObject((D3OBJECT *)b->object, (D3QNODE *)d3_QuadTree);
    d3if_Delete((D3OBJECT *)b->object);
    if (b->smokeEffect != -1) {
        sc = evt_FindById(b->smokeEffect);
        if (sc != 0)
            evt_Stop(sc);
        b->smokeEffect = -1;
    }
    d3if_SelectScene(save, 1);
    gm_FreeBuilding((char *)b);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb210, message 0x3c (cm_SendHandOverCrimes): hands the crimes over (or reopens them); the state is 2 unless
   gv_HandOverCrimes reports crimes, then 1 */

int cm_ExHandOverCrimes(G17MSG *m, G17STAT *s)
{
    char r = 2;

    if (gv_HandOverCrimes(m->handOver.owner, m->handOver.culprit, m->handOver.state, m->handOver.hand) > 0)
        r = 1;
    if (s != 0) {
        s->state = r;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb250, handler of message 0x3d (cm_SendMessage3d): hires a character for the building bldId: the applicant charId
   (a new one of the kind, title and job when it has none or already works), placed in its home's (homeId, else the
   applicant's own) or the building's main room; a worker of the titles 1 / 2 / 0xf / 0x10 / 0x13 / 0x14 gets the
   item 0x154 when it holds no weapon, the others drop the weapon they hold; it works in the building from now,
   moves into the home, gets the building's work settings (and its budget with a job) and a fresh job */
int cm_ExHire(G17MSG *m, G17STAT *s)
{
    AIBLD *b, *home = 0;
    AICHAR *c;
    AIOBJ *t;
    AIWEAPON *w;
    unsigned char title;

    if (net_Conn.s != (unsigned int)-1)
        ai_NextObjectId = m->hire.objectId;
    else
        m->hire.seed = rand();
    util_Seed(m->hire.seed);
    b = (AIBLD *)gm_BuildQuery(1, 1, m->hire.bldId);
    if (b == 0)
        return 1;
    c = (AICHAR *)gm_FindChar(m->hire.charId);
    if (m->hire.homeId != -1) {
        home = (AIBLD *)gm_BuildQuery(1, 1, m->hire.homeId);
        if (home == 0)
            return 1;
    } else if (c != 0)
        home = c->bld2;
    if (c == 0 || c->bld != 0)
        c = (AICHAR *)gm_SpawnCharacter(6, m->hire.kind, 0x12, m->hire.job, m->hire.title, (char *)home);
    if (c == 0) {
        if (s != 0) {
            s->state = 2;
            s->kind = 1;
            s->value = 0;
        }
        return 2;
    }
    if (m->hire.title != 0)
        c->title = m->hire.title;
    if (m->hire.job != 0)
        c->jobId = m->hire.job;
    else
        c->jobId = 0;
    c->kind = m->hire.kind;
    if (c->model == 0) {
        if (home != 0) {
            t = (AIOBJ *)gm_HomeObject((unsigned char *)home);
            if (t != 0 && t->objId != -1)
                gm_PlaceOnStage(c->netId, home->id, t->objId, 0, 0);
        } else {
            t = (AIOBJ *)gm_BuildingMainObject((unsigned char *)b);
            if (t != 0 && t->objId != -1)
                gm_PlaceOnStage(c->netId, b->id, t->objId, 0, 0);
        }
    }
    title = m->hire.title;
    if (title != 2 && title != 1 && title != 0x10 && title != 0xf && title != 0x14 && title != 0x13) {
        w = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (w->id != 0) {
            gm_TakeObjekt(c->netId, w->id, 1);
            cm_UpdateHandObject((char *)c);
        }
    } else {
        w = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (w->id == 0) {
            gm_CreateObject(c->netId, 0x154, 1);
            cm_UpdateHandObject((char *)c);
        }
    }
    c->bld = b;
    if (home != 0)
        gm_MoveIntoHome((char *)c, (char *)home);
    c->v174 = 0;
    c->flags1c8 &= ~0x2000;
    ai_ResetJob(c);
    memcpy(&c->flags1b4, b->workSet, sizeof(b->workSet));
    if (m->hire.job != 0)
        c->budget = b->budget;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)c;
    }
    return 0;
}
#define CM_CHR(c) ((CHARACTER *)(c)->model)  /* AICHAR.model (ai.h: AITASK *) as the CHARACTER it is */

/* 0x4cb4a0, handler of message 0x3e (cm_SendCharModel): the character charId gets the model "<body>_<name>": a new
   scene object at the old one's place and angles (its name, owner and pick id kept), relit and marked as a
   character's body; the model's text after its last '_' goes to the body name, the text before it to the full
   name (an empty body name without a '_'; the second strrchr searches past the last '_' and finds none) */

int cm_ExCharModel(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    D3VECTOR rot;
    D3VECTOR4 pos;                      /* the engine's padded vector (the original's frame holds 16 bytes) */
    AICHAR *c;
    D3OBJECT *t;
    void *owner;
    CHARACTER *ch;
    char *p, *q;
    int save;

    c = (AICHAR *)gm_FindChar(m->model.charId);
    if (c == 0 || c->model == 0)
        return 1;
    save = d3_CurrentScene;
    d3if_SelectScene(d3t_SceneIndex(CM_CHR(c)->scene), 1);
    ch = CM_CHR(c);
    pos.x = ch->obj->pos.x;
    pos.y = ch->obj->pos.y;
    pos.z = ch->obj->pos.z;
    rot.x = ch->obj->rotation.x;
    rot.y = ch->obj->rotation.y;
    rot.z = ch->obj->rotation.z;
    owner = ch->obj->owner;
    t = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, m->model.name);
    if (t != 0) {
        strcpy(t->name, CM_CHR(c)->obj->name);
        strcpy(CM_CHR(c)->name, m->model.name);
        d3if_Delete(CM_CHR(c)->obj);
        CM_CHR(c)->obj = t;
        CM_CHR(c)->obj->owner = owner;
        CM_CHR(c)->obj->pickId = 0x3000000;
        d3_RelightObject(CM_CHR(c)->obj);
        CM_CHR(c)->obj->stateFlags = (CM_CHR(c)->obj->stateFlags & 0xf7) | 4;
        CM_CHR(c)->obj->kind = 2;
        strcpy(buf, m->model.name);
        p = strrchr(buf, '_');
        if (p != 0) {
            p++;
            q = strrchr(p, '_');
            if (q != 0)
                *q = 0;
            strcpy(CM_CHR(c)->body, p);
            p[-1] = 0;
            strcpy(CM_CHR(c)->fullname, buf);
        } else
            CM_CHR(c)->body[0] = ch_EmptyName[0];
        d3if_SelectScene(save, 1);
        if (s != 0) {
            s->state = 2;
            s->kind = 1;
            s->value = (int)c;
        }
        return 0;
    }
    d3if_SelectScene(save, 1);
    return 1;
}
/* 0x4cb6e0, message 0x3f (cm_SendGoodsRecord): replaces the goods record of the message's player and item
   (gm_FindGoods) by the 0x80-byte record that follows the player byte */

int cm_ExGoodsRecord(G17MSG *m, G17STAT *s)
{
    char *r;

    r = gm_FindGoods(m->goods.player, m->goods.item);
    if (r == 0)
        return 1;
    memcpy(r, &m->data[1], 0x80);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb730, message 0x40 (cm_SendGoodsDelta): adds the three daily counters and the four floats of the message to
   the goods record (gm_FindGoods) of its player and item */

int cm_ExGoodsDelta(G17MSG *m, G17STAT *s)
{
    GMSTOCK *g;

    g = (GMSTOCK *)gm_FindGoods(m->goods.player, m->goods.item);
    if (g == 0)
        return 1;
    g->daily[0] += m->goods.daily[0];
    g->daily[1] += m->goods.daily[1];
    g->daily[2] += m->goods.daily[2];
    g->demand += m->goods.demand;
    g->mean += m->goods.mean;
    g->f2c += m->goods.f2c;
    g->price += m->goods.price;
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb7c0, message 0x41 (cm_SendMessage41): restores the statistics record from the message's 0x3a-byte payload */

int cm_ExLoadStatistics(G17MSG *m, G17STAT *s)
{
    ord_LoadStatistics(&m->data[0]);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb7f0, message 0x42 (cm_SendUpdateSmoke): restarts the chimney smoke of the building m->smoke.bldId when it
   is no home and has a scene object */

int cm_ExUpdateSmoke(G17MSG *m, G17STAT *s)
{
    AIBLD *b;

    b = (AIBLD *)gm_BuildQuery(1, 1, m->smoke.bldId);
    if (b != 0 && b->object != 0 && gm_IsHomeType((unsigned char *)b) == 0)
        sv_UpdateChimneySmoke(b);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb850, message 0x43 (cm_SendSkillsChanged): shows the objects of the building (bld2) of the character whose
   skills were raised */

int cm_ExSkillsChanged(G17MSG *m, G17STAT *s)
{
    AICHAR *c;

    c = (AICHAR *)gm_FindChar(m->skills.charId);
    if (c != 0 && c->bld2 != 0)
        gm_ShowBuildingObjects((unsigned char *)c->bld2);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb890, message 0x44 (cm_SendApplication): accepts the office application (amt_AcceptApplication); the state
   is 2 when it failed, 1 when it was accepted, and the handler returns 1 on a failure */

int cm_ExApplication(G17MSG *m, G17STAT *s)
{
    int r;

    r = amt_AcceptApplication(&m->app);
    if (s != 0) {
        s->kind = 0;
        s->state = (r == 0) + 1;
        s->value = 0;
    }
    return r == 0;
}
/* 0x4cb8d0, message 0x45 (cm_SendOfficeRecord): executes the office message (amt_ExecuteOfficeMessage); the state is 2
   when it failed, 1 when it went through, and the handler returns 1 on a failure */

int cm_ExOfficeMessage(G17MSG *m, G17STAT *s)
{
    int r;

    r = amt_ExecuteOfficeMessage(&m->data[0]);
    if (s != 0) {
        s->kind = 0;
        s->state = (r == 0) + 1;
        s->value = 0;
    }
    return r == 0;
}
/* 0x4cb910, message 0x46 (cm_SendDecisionValue): stores the decision record's new value (gv_OnDecisionValue); the
   state is 2 when the record was bad, 1 when it was stored, and the handler returns 0 */

int cm_ExDecisionValue(G17MSG *m, G17STAT *s)
{
    int r;

    r = gv_OnDecisionValue(&m->data[0]);
    if (s != 0) {
        s->kind = 0;
        s->value = 0;
        s->state = (r == 0) + 1;
    }
    return 0;
}
/* 0x4cb940, message 0x47 (cm_SendCharClear): moves the character out of its group (sv_MoveToGroup) to the
   message's room, unless its id is 0xffff; an id gm_FindChar does not find is an error */

int cm_ExChangePlayerHe(G17MSG *m, G17STAT *s)
{
    AICHAR *c;

    c = (AICHAR *)gm_FindChar(m->clear.charId);
    if (c == 0)
        err_Message("cm_ExChangePlayerHe(): invalid sp->id");
    if (c->id != 0xffff)
        sv_MoveToGroup((unsigned char *)c->bld, (short *)m->clear.room, c->id, 0);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb9b0, message 0x48 (cm_SendSetJob): gives the character the job (gm_SetJob) and chooses its head icon;
   an id gm_FindChar does not find returns 1 */

int cm_ExSetJob(G17MSG *m, G17STAT *s)
{
    AICHAR *c;

    c = (AICHAR *)gm_FindChar(m->setJob.charId);
    if (c == 0)
        return 1;
    gm_SetJob(c->id, m->setJob.job);
    gm_ChooseHeadIcon(&c->id);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cba10, handler of message 0x49 (sent by 0x4c59f0, he.c's spy "SPIONAGE"): creates a character of the kind with
   job and title (gm_CreateCharacter) at the building bldId (an id cm_ResolveId maps; -1: none), in its room objId
   (-1: none), without a workplace, as kind 0xd, holding the item; cm_LastCharId is its id */
int cm_ExSpawnCharacter(G17MSG *m, G17STAT *s)
{
    AIBLD *b;
    AIOBJ *t;
    int id, k;

    if (net_Conn.s != (unsigned int)-1)
        ai_NextObjectId = m->spawn.objectId;
    else
        m->spawn.seed = timeGetTime();
    if (s != 0) {
        s->state = 2;
        s->kind = 1;
        s->value = 0;
    }
    util_Seed(m->spawn.seed);
    if (m->spawn.bldId == -1)
        b = 0;
    else {
        k = cm_ResolveId((int)m, m->spawn.bldId);
        m->spawn.bldId = k;
        b = (AIBLD *)gm_BuildQuery(1, 1, k);
        if (b == 0)
            return 1;
    }
    id = gm_CreateCharacter(m->spawn.kind, -1, -1, 0x14, 0, m->spawn.job, m->spawn.title, 0);
    if (id == 0xffff)
        return 1;
    if (m->spawn.objId != -1 && (t = (AIOBJ *)gm_FindObjectInst(m->spawn.objId)) != 0)
        gm_PlaceOnStage(ai_CharsT[id].netId, b->id, t->objId, 0, 0);
    else
        gm_PlaceOnStage(ai_CharsT[id].netId, b->id, -1, 0, 0);
    ai_CharsT[id].bld = 0;
    ai_CharsT[id].kind = 0xd;
    if (m->spawn.item != 0) {
        gm_CreateObject(ai_CharsT[id].netId, m->spawn.item, 1);
        cm_UpdateHandObject((char *)&ai_CharsT[id]);
    }
    cm_LastCharId = ai_CharsT[id].netId;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)&ai_CharsT[id];
    }
    return 0;
}
/* 0x4cbb90, message 0x4a (cm_SendFreeObject): frees the object gm_FindObjectInst finds; an object it does not
   find returns 1 */

int cm_ExFreeObject(G17MSG *m, G17STAT *s)
{
    char *t;

    t = gm_FindObjectInst(m->freeObject.objId);
    if (t == 0)
        return 1;
    gm_FreeObject(t);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cbbd0, handler of message 0x4b (cm_SendChat): the chat text is appended to cm_ChatLog (cleared first once it
   holds more than 5 "$A" line breaks) when the player's character is among the 8 receivers or the message is our
   own (s != 0); sv_ChatTime notes when */
int cm_ExChat(G17MSG *m, G17STAT *s)
{
    char *p;
    int n = 0;
    int found = 0;
    int i;

    p = strstr(cm_ChatLog, cm_TextNewline);
    if (p != 0) {
        do {
            p += 2;
            n++;
            p = strstr(p, cm_TextNewline);
        } while (p != 0);
        if (n > 5)
            cm_ChatLog[0] = 0;
    }
    for (i = 0; i < 8; i++) {
        if (m->chat.to[i] == ai_CharsT[ai_PlayerId].netId) {
            found = 1;
            break;
        }
    }
    if (s != 0 || found) {
        strcat(cm_ChatLog, m->chat.text);
        sv_ChatTime = d3_Time;
        if (s != 0) {
            s->state = 1;
            s->kind = 0;
            s->value = 0;
        }
    }
    return 0;
}
/* 0x4cbca0, handler of message 0x4c, "cm_ExCreateGeb": creates a building of kind m->geb.kind for the character
   m->geb.owner (gm_CreateBuilding), finds its model "<data>gebaeude/*<NAME>.ogr" or "*<NAME>_A .. _C.ogr" in the
   VFS, loads it into scene 0 at m->geb.pos / m->geb.rot, drops it onto the ground, hooks it to the building record
   and removes the "vg_" objects (d3_ForEachObject over the default scene with ai_VgFilter) under the building's
   construction site (lc_GetGebaeudeBauplatz); banners and the road follow. s (when given) gets the building
   (state 1, kind 2). Returns 1 when the character or the building is missing, else 0 */

int cm_ExCreateGeb(G17MSG *m, G17STAT *s)
{
    char name[MAX_PATH];
    char tmp[0x80];
    char path[MAX_PATH];
    char text[0x100];
    char found[MAX_PATH];
    AIBUILDSITE site;
    AIVGLIST list;
    D3VECTOR4 pos;
    AIBLD *b;
    AICHAR *c;
    D3OBJECT *o;
    int scene, i;

    c = (AICHAR *)gm_FindChar(m->geb.owner);
    if (c == 0)
        return 1;
    if (net_Conn.s != -1)
        ai_NextObjectId = m->geb.objectId;
    b = (AIBLD *)gm_CreateBuilding(m->geb.kind, c->id);
    if (b == 0)
        return 1;
    cm_LastBuildingId = b->id;
    strcpy(name, "gb_");
    strcat(name, ai_BTypes + (unsigned)b->type * 0x24d + 1);
    _strupr(name + 2);
    for (i = 0; i < 4; i++) {
        if (i != 0)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, name, (char)(i + 0x40));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, name);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, found) != 0) {
            scene = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            pos.x = m->geb.pos.x;
            pos.y = m->geb.pos.y;
            pos.z = m->geb.pos.z;
            o = d3_LoadGroup(path, 0, 0);
            if (o == 0) {
                sprintf(name, "cm_ExCreateGeb(): Could not load 3D-Object-Group '%s'...", path);
            } else {
                memset(&site, 0, sizeof(site));
                d3_SetObjectPos(o, (D3VECTOR *)&pos);
                d3_SetObjectRot(o, m->geb.rot);
                ai_DropObjectAt(o, (D3VECTOR *)&pos);
                init_HookObject((char *)o, (unsigned short *)b, 2);
                ai_BuildingName(b->type, tmp);
                strcpy(site.name, tmp);
                sprintf(name, "cm_ExCreateGeb(): bauplatze name:%s", tmp);
                if (lc_GetGebaeudeBauplatz(&site, b) != 0) {
                    memset(&list, 0, sizeof(list));
                    d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)site.object, 0x2c0,
                                     (D3OBJECTCB)ai_VgFilter, &list);
                    for (i = 0; i < list.count; i++) {
                        if (d3_QuadTree != 0)
                            d3q_RemoveObject((D3OBJECT *)list.objs[i], (D3QNODE *)d3_QuadTree);
                        sprintf(text, "cm_ExCreateGeb(): killing objectptr: %s", (char *)list.objs[i]);
                        sprintf(text, "cm_ExCreateGeb(): killing objectname: %s", (char *)list.objs[i]);
                        d3if_Delete((D3OBJECT *)list.objs[i]);
                    }
                } else
                    err_Message("cm_ExCreateGeb(): could not lc_GetGebaeudeBauplatz()...");
                sv_CreateBanners((char *)b);
                rd_BuildRoad(b->object, b->type);
            }
            d3if_SelectScene(scene, 1);
            break;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 2;
        s->value = (int)b;
    }
    return 0;
}
/* 0x4cbff0, message 0x4d (cm_SendCharHalt): stops the event script the character's model runs; an id
   gm_FindChar does not find returns 1 */

int cm_ExCharHalt(G17MSG *m, G17STAT *s)
{
    AICHAR *c;
    AITASK *g;

    c = (AICHAR *)gm_FindChar(m->halt.charId);
    if (c == 0)
        return 1;
    g = c->model;
    if (g != 0 && g->scriptId != -1)
        evt_Stop(evt_FindById(g->scriptId));
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc050, message 0x4e (cm_SendObjCell): walks the character name (data[0]) to the cell x / z and, when the
   message names a second object, targets it by name; with a stat record the walk (0x2d) is queued, else the
   action 0x3a */

int cm_ExObjCell(G17MSG *m, G17STAT *s)
{
    CHARACTER *p, *q = 0;
    CHACTION *r;
    int sc;

    p = ch_FindByName(m->objCell.name);
    if (p == 0)
        return 1;
    if (m->objCell.name[0] != 0)
        q = ch_FindByName(m->objCell.name2);
    sc = d3t_SceneIndex(p->scene);
    if (s != 0) {
        r = ch_AddAction(p, 0x2d, m->objCell.x, m->objCell.z, sc);
        r->u.walk.flags |= 2;                   /* 0x2: the walk names its target (no reader names the bit) */
        if (q != 0) {
            strcpy(r->u.walk.tname, m->objCell.name2);
            r->u.walk.flags |= CH_WALK_TARGET;
        }
        s->state = 1;
        s->kind = 0;
        s->value = 0;
        return 0;
    }
    ch_AddAction(p, 0x3a, m->objCell.x, m->objCell.z, sc);
    return 0;
}
/* 0x4cc110, message 0x4f (no sender in the sources; reasoned: the path cells of a walk): writes up to two cells
   of the named character's walk action 0x3a into its path, from the cell "from" up to "to"; 1 when the name is
   not found, 2 when the character runs no 0x3a action */

int cm_ExWalkPath(G17MSG *m, G17STAT *s)
{
    CMWALKPATHMSG *d;
    CHARACTER *p;
    CHACTION *a;
    unsigned char *cell;
    int idx, i;

    d = (CMWALKPATHMSG *)m->data;
    p = ch_FindByName(d->name);
    if (p == 0)
        return 1;
    a = p->actions;
    if (a == 0 || a->type != 0x3a)
        return 2;
    if (a->u.walk.path != 0) {
        a->u.walk.pathLen = d->to;
        idx = d->from;
        cell = d->cells;
        for (i = 0; i < 2 && idx < a->u.walk.pathLen; i++) {
            ((unsigned char *)a->u.walk.path)[idx * 2] = cell[0];
            ((unsigned char *)a->u.walk.path)[idx * 2 + 1] = cell[1];
            cell += 2;
            idx++;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc1b0, handler of message 0x50: when the message's fight is our player's party, the entry m->cutEntry.entry is
   stored into the cutscene record of player m->cutEntry.playerId, in the slot of its character (a fighter or any
   character; cm_CutEntrySlot finds or takes the slot); an entry of state 0 frees the slot again (charId -1). Returns 1
   for a message of another fight, else 0 */

int cm_ExCutEntry(G17MSG *m, G17STAT *s)
{
    CMCUTINFO *rec;
    CMCUTENTRY *r;
    char *c;
    int i;

    if (m->cutEntry.fightId != ai_CharsT[ai_PlayerId].partyId)
        return 1;
    for (i = 0; i < 16; i++) {
        if (cm_CutInfosT[i].playerId == m->cutEntry.playerId) {
            rec = &cm_CutInfosT[i];
            c = cm_FindFighter(m->cutEntry.entry.charId);
            if (c == 0)
                c = (char *)gm_FindChar(m->cutEntry.entry.charId);
            if (c != 0) {
                r = (CMCUTENTRY *)cm_CutEntrySlot((char *)rec, c);
                *r = m->cutEntry.entry;
                if (r->state == 0)
                    r->charId = -1;
            }
            break;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc270, handler of message 0x51: as cm_ExCutEntry (0x50), but the entry of the character (a fighter) in the
   cutscene record of player m->cutEntry.playerId is overwritten only when it is in the message's state (and, in
   state 2, has its stateArg). Returns 1 for a message of another fight, else 0 */

int cm_ExCutEntryUpdate(G17MSG *m, G17STAT *s)
{
    CMCUTENTRY *r;
    int i;

    if (m->cutEntry.fightId != ai_CharsT[ai_PlayerId].partyId)
        return 1;
    for (i = 0; i < 16; i++) {
        if (cm_CutInfosT[i].playerId == m->cutEntry.playerId) {
            r = (CMCUTENTRY *)cm_CutEntrySlot((char *)&cm_CutInfosT[i], cm_FindFighter(m->cutEntry.entry.charId));
            if (r->state == m->cutEntry.entry.state) {
                if (r->state != 2 || r->stateArg == m->cutEntry.entry.stateArg)
                    *r = m->cutEntry.entry;
            }
            break;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc330, handler of message 0x52 (cm_SendDamage): shows the damage on the fighter and takes it from its v024
   (the decaying value sent as field 0x24), then sets its health to the message's */

int cm_ExDamage(G17MSG *m, G17STAT *s)
{
    AICHAR *r;

    r = (AICHAR *)cm_FindFighter(m->damage.netId);
    if (r != 0) {
        cm_ShowDamage((char *)r, m->damage.damage, 0);
        r->v024 -= m->damage.damage;
        r->health = m->damage.health;
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc390, handler of message 0x53 for a tavern's dark corner (the execute side of cm_CheckDarkCorner): 'cler'
   resets the three evidence offers and the year of the corner's kind 0x12c object in building m->script.bldId,
   'buy ' clears the offer m->script.offer (and tells the owner, ai_ShowPurchaseNews); s->state is 1 when done,
   2 (refused) when the command is, and 1 is returned then */

int cm_ExDarkCorner(G17MSG *m, G17STAT *s)
{
    AIBLD *b;
    AIOBJ *q;
    unsigned short *c;
    int i, k;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (m->script.op == 0x636c6572) {               /* 'cler' */
        b = (AIBLD *)gm_BuildQuery(1, 1, m->script.bldId);
        if (b == 0)
            return 1;
        q = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12c);
        if (q == 0)
            return 1;
        if (ai_Time.year - q->cornerYear <= 1)
            return 1;
        q->recs[0] = -1;
        q->recs[1] = -1;
        q->recs[2] = -1;
        q->cornerYear = ai_Time.year - 1;
    } else if (m->script.op == 0x62757920) {        /* 'buy ' */
        c = (unsigned short *)gm_FindChar(m->script.netId);
        if (c == 0)
            return 1;
        b = (AIBLD *)gm_BuildQuery(1, 1, m->script.bldId);
        if (b == 0)
            return 1;
        q = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12c);
        if (q == 0)
            return 1;
        k = m->script.offer;
        for (i = 0; i < 3; i++) {
            if (q->recs[i] == k)
                break;
        }
        if (i >= 3)
            return 1;
        q->recs[i] = -1;
        if (ai_PlayerId == b->owner)
            ai_ShowPurchaseNews(c, ai_Chars[ai_PlayerId], (int)b, m->script.price);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cc510, handler of message 0x54 (cm_SendTableRequest): the seats of the table (object kind 0x12d) in the
   building m->table.bldId: 'join' seats the character m->table.charId in the first free seat (not twice), 'leav'
   frees its seat, 'cler' frees it everywhere (refused while the building's owner is another living character not of
   kind 0xf); the inn meeting script he_StammtischCommand hears of it when s is given. s->state is 2 (refused) unless the
   request is carried out; returns 1 when refused, 0 when done */

int cm_ExTableRequest(G17MSG *m, G17STAT *s)
{
    AIBLD *b;
    AIOBJ *q;
    AICHAR *ch;
    int i, j;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (m->table.op == 0x6a6f696e) {               /* 'join' */
        b = (AIBLD *)gm_BuildQuery(1, 1, m->table.bldId);
        if (b == 0)
            return 1;
        q = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        for (i = 0; i < 4; i++) {
            if (q->seats[i] == m->table.charId)
                return 1;
        }
        for (j = 0; j < 4; j++) {
            if (q->seats[j] == -1)
                break;
        }
        if (j >= 4)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_StammtischCommand(m->table.op, b, q, m->table.charId);
        }
        q->seats[j] = m->table.charId;
        return 0;
    }
    if (m->table.op == 0x6c656176) {               /* 'leav' */
        b = (AIBLD *)gm_BuildQuery(1, 1, m->table.bldId);
        if (b == 0)
            return 1;
        q = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        for (j = 0; j < 4; j++) {
            if (q->seats[j] == m->table.charId)
                break;
        }
        if (j >= 4)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_StammtischCommand(m->table.op, b, q, m->table.charId);
        }
        q->seats[j] = -1;
        return 0;
    }
    if (m->table.op == 0x636c6572) {               /* 'cler' */
        b = (AIBLD *)gm_BuildQuery(1, 1, m->table.bldId);
        if (b == 0)
            return 1;
        if (b->owner != 0xffff) {
            ch = &ai_CharsT[b->owner];
            if (ch != 0 && (char *)ch != ai_PlayerCharC && ch->health != 0 && ch->kind != 0xf)
                return 1;
        }
        q = (AIOBJ *)gm_Query((void *)b->objList, 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_StammtischCommand(m->table.op, b, q, m->table.charId);
        }
        for (j = 0; j < 4; j++) {
            if (q->seats[j] == m->table.charId)
                q->seats[j] = -1;
        }
        return 0;
    }
    return 1;
}
/* 0x4cc720, moves the body of the fighter c (its model's body object) to pos and turns it to rot, each only when it
   is not already within 8.0 of it, and stores v in c->v024 (the fight message's field at 0x21) */

void cm_PlaceFighter(AICHAR *c, D3VECTOR *pos, D3VECTOR *rot, int v)
{
    if (c != 0 && c->model != 0) {
        if (d3_VecEqual(&c->model->body->pos, pos, 8.0f) == 0)
            d3_SetObjectPos(c->model->body, pos);
        if (d3_VecEqual(&c->model->body->rotation, rot, 8.0f) == 0)
            d3_SetObjectRot(c->model->body, rot);
        c->v024 = v;
    }
}
/* 0x4cc7b0, handler of message 0x55 (cm_SendFightCommand): runs the command m->fight.cmd of the fighter
   m->fight.slot.id in the current fight (only when the message is for our fight), at the position, rotation and
   health the message carries */

int cm_ExFightCommand(G17MSG *m, G17STAT *s)
{
    AICHAR *c;

    if (cm_CurrentFight == 0 || *cm_CurrentFight != m->fight.fight)
        return 1;
    c = (AICHAR *)cm_FindFighter(m->fight.slot.id);
    switch (m->fight.cmd) {
    case 2:                                         /* attack */
        if (c->health != 0) {
            cm_PlaceFighter(c, &m->fight.pos, &m->fight.rot, m->fight.health);
            cm_FighterAct(&m->fight.slot);
        }
        break;
    case 6:                                         /* stop */
        if (c->health != 0) {
            cm_PlaceFighter(c, &m->fight.pos, &m->fight.rot, m->fight.health);
            cm_FightSlotStop(&m->fight.slot);
        }
        break;
    case 1:                                         /* cheer */
        if (c->health != 0)
            cm_FightSlotCheer(&m->fight.slot);
        break;
    case 4:                                         /* pick up the object at its place */
        if (c->health != 0)
            cm_FightSlotPickUp(&m->fight.slot);
        break;
    case 5:                                         /* put down the object it carries */
        if (c->health != 0)
            cm_FightSlotPutDown(&m->fight.slot);
        break;
    case 7:                                         /* the winner so far */
        cm_FightSlotWinner(&m->fight.slot);
        break;
    case 3:                                         /* escape */
        if (c->health != 0) {
            cm_PlaceFighter(c, &m->fight.pos, &m->fight.rot, m->fight.health);
            cm_PlantEscapeFlag(&m->fight.slot);
        }
        break;
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc8e0, handler of message 0x56 (cm_SendMessage56): hands the message record to ord_HandleMessageRecord
   ('init' / 'set ' copy it into ord_MessageRecord, 'dsbl' / 'enbl' toggle its enabled flag); 1 when refused */

int cm_ExMessageRecord(G17MSG *m, G17STAT *s)
{
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (ord_HandleMessageRecord(&m->data[0]) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cc920, handler of message 0x57 (cm_SendChrDelete): deletes the scene character of the character
   m->chrDelete.charId and clears its AICHAR.scene; s->state is 1 when deleted, 2 (refused) and 1 returned when
   the character or its scene character is missing */

int cm_ExChrDelete(G17MSG *m, G17STAT *s)
{
    AICHAR *c;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    c = (AICHAR *)gm_FindChar(m->chrDelete.charId);
    if (c != 0 && c->scene != 0) {
        ch_Delete(c->scene);
        c->scene = 0;
        if (s != 0)
            s->state = 1;
        return 0;
    }
    return 1;
}
/* 0x4cc980, handler of message 0x58 (cm_SendCancelParty): ends the party m->cancelParty.partyId: clears the
   AICHAR.partyId of each of its members (the link back to the record's id) and clears the record; s->state is 1 */

int cm_ExCancelParty(G17MSG *m, G17STAT *s)
{
    G28REC *q;
    AICHAR *c;
    int i, id;

    q = (G28REC *)cm_PartyById(m->cancelParty.partyId);
    if (q == 0)
        return 1;
    for (i = 0; i < q->used; i++) {
        id = q->members[i];
        if (id != -1) {
            c = (AICHAR *)gm_FindChar(id);
            if (c != 0 && c->partyId == q->id)
                c->partyId = -1;
        }
    }
    cm_ClearParty(m->cancelParty.partyId);
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cca10, handler of message 0x59 (cm_SendCutsceneReady): marks the party m->ready.partyId ready (G28REC.state)
   and sets its 16 members' AICHAR.partyId to it; s->state is 2 (refused) when the check refuses, 1 when done */

int cm_ExCutsceneReady(G17MSG *m, G17STAT *s)
{
    G28REC *q;
    AICHAR *c;
    int i;

    if (cm_CheckCutsceneReady(m) != 0) {
        if (s != 0)
            s->state = 2;
        return 1;
    }
    if (s != 0)
        s->state = 2;
    q = (G28REC *)cm_PartyById(m->ready.partyId);
    if (q != 0)
        q->state = 1;
    for (i = 0; i < 16; i++) {
        c = (AICHAR *)gm_FindChar(m->ready.members[i]);
        if (c != 0)
            c->partyId = m->ready.partyId;
    }
    if (s != 0) {
        s->state = 1;
        s->kind = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4ccaa0, handler of message 0x5a (cm_SendMoneyDelta): adds m->money.delta to the AICHAR.money of the character
   m->money.charId, kept in 0 .. 0x32; s->state is 1 when done, 2 (refused) and 1 returned when it is missing */

int cm_ExMoneyDelta(G17MSG *m, G17STAT *s)
{
    AICHAR *c;
    int v;

    c = (AICHAR *)gm_FindChar(m->money.charId);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (c == 0)
        return 1;
    v = m->money.delta + c->money;
    v = v < 0 ? 0 : v;
    if (v >= 0x32)
        v = 0x32;
    c->money = v;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4ccb10, handler of message 0x5b (cm_SendMessage5b): adds m->heldRounds.delta to the AICHAR.heldRounds of the
   character m->heldRounds.charId, kept in 0 .. 0xff; for our own character in a running game it pops to the game
   frame, and m->heldRounds.all goes to amt_ClearOfficesIfAll */

int cm_ExHeldRounds(G17MSG *m, G17STAT *s)
{
    AICHAR *c;
    int v;

    c = (AICHAR *)gm_FindChar(m->heldRounds.charId);
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (c == 0)
        return 1;
    v = c->heldRounds + m->heldRounds.delta;
    v = v < 0 ? 0 : v;
    if (v >= 0xff)
        v = 0xff;
    c->heldRounds = v;
    v = c->heldRounds != 0 ? c->heldRounds : 0;
    c->heldRounds = v;
    if (c->id == ai_PlayerId && ai_GameState != 0 && (unsigned char)v != 0)
        sv_PopTo(ai_GameFrame);
    amt_ClearOfficesIfAll((char *)c, m->heldRounds.all);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4ccbd0, handler of message 0x5c, the office-swap request m->trade (an AMTTRADE, sent by amt_SendOfficeSwap): sets
   s->state 2 and executes the swap with amt_ExecuteSwapMessage, s->state 1 when it went through; returns 1 when the
   swap was refused (no such character, office or money), else 0 */

int cm_ExOfficeSwap(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    r = amt_ExecuteSwapMessage(&m->trade);
    if (s != 0 && r != 0)
        s->state = 1;
    if (r == 0)
        return 1;
    return 0;
}
/* 0x4ccc20, handler of message 0x5d: adds data[8] to the skill data[4] (0 .. 4, AICHAR.skills) of the character
   data[0], clamped to 0 .. 252. s->state is 2 (refused) for an unknown character or skill (returns 1), else 1 */

int cm_ExAddSkill(G17MSG *m, G17STAT *s)
{
    AICHAR *c;
    int k;
    float f;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    c = (AICHAR *)gm_FindChar(m->skill.charId);
    if (c == 0)
        return 1;
    k = m->skill.skill;
    if (k < 0 || k > 4)
        return 1;
    f = (float)(c->skills[k] + m->skill.delta);
    /* MAX(MIN(f, 252), 0) as macros expand it: the MIN is evaluated again for the result */
    f = (f < 252.0f ? f : 252.0f) > 0.0f ? (f < 252.0f ? f : 252.0f) : 0.0f;
    c->skills[k] = (unsigned char)f;
    if (s != 0)
        s->state = 1;
    return 0;
}
typedef struct CMSPYREPORT {            /* an AIOBJ of kind 0xca (a spy report) as this unit reads it: the report's words
                                           0x1c .. 0x37 (HESPYREPORT) after the object's head */
    char pad1c[0x1c];
    HESPYREPORT report;
} CMSPYREPORT;

/* 0x4ccce0, handler of message 0x5e, the spy report m->spy (a HESPYREPORT, sent by cm_SendSpyReport) on the building
   m->spy.bldId: stores it in the report object 0xca of that building (a new one when it has none yet, m->spy.objId
   being the server's ai_NextObjectId), the building it reports on being m->spy.rep.bld; s->state 2, then 1; returns 1
   when either building is missing, else 0 */

int cm_ExSpyReport(G17MSG *m, G17STAT *s)
{
    AIBLD *a, *b;
    AIOBJ *q;
    CMSPYREPORT *r;
    int v;

    if (net_Conn.s != -1)
        ai_NextObjectId = m->spy.objId;
    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    a = (AIBLD *)gm_FindBuildingById(m->spy.rep.bld);
    if (a == 0)
        return 1;
    b = (AIBLD *)gm_FindBuildingById(m->spy.bldId);
    if (b == 0)
        return 1;
    q = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xca);
    while (q != 0) {
        if (((CMSPYREPORT *)q)->report.bld == a->id)
            goto found;
        q = (AIOBJ *)gm_QueryNext();
    }
    q = (AIOBJ *)gm_CreateObject(b->id, 0xca, 1);
    if (q == 0)
        return 1;
    cm_LastObjectId = q->objId;
found:
    r = (CMSPYREPORT *)q;
    r->report.when = m->spy.rep.when;
    r->report.bld = m->spy.rep.bld;
    r->report.goods = m->spy.rep.goods;
    r->report.skill = m->spy.rep.skill;
    r->report.know = r->report.know + m->spy.rep.know;
    if (r->report.know > 0x64)
        v = 0x64;
    else
        v = r->report.know;
    r->report.know = v;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cce00, the handler table's entry for message 0x5f, the network lobby: the body is cm_ExLobby (0x4cd870), which
   VC6 reaches with a tail jump */

int cm_ExLobbyMessage(G17MSG *m, G17STAT *s)
{
    return cm_ExLobby(m, s);
}
/* 0x4cce10, handler of message 0x60, the grave order m->data (a GVORDER): in a net game its objId is the server's
   ai_NextObjectId, otherwise its seed is filled with rand(); the order's type 0 .. 6 picks gv_BuryCharacter ..
   gv_AddNewDead (gv.h); returns their result, -1 for an unknown type; s->state 2, then 1 when the order succeeded */

int cm_ExGraveOrder(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (net_Conn.s != -1)
        ai_NextObjectId = ((GVORDER *)m->data)->objId;
    else
        ((GVORDER *)m->data)->seed = rand();
    switch (((GVORDER *)m->data)->type) {
    case 0:
        r = gv_BuryCharacter((int *)m->data);
        break;
    case 1:
        r = gv_ReserveGrave((GVORDER *)m->data);
        break;
    case 2:
        r = gv_ExtendGrave((GVORDER *)m->data);
        break;
    case 3:
        r = gv_SetGraveHolder((GVORDER *)m->data);
        break;
    case 4:
        r = gv_OpenSize((GVORDER *)m->data);
        break;
    case 5:
        r = gv_AdjustEntry((GVORDER *)m->data);
        break;
    case 6:
        r = gv_AddNewDead(m->data);
        break;
    default:
        return -1;
    }
    if (s != 0 && r == 0)
        s->state = 1;
    return r;
}
/* 0x4cced0, handler of message 0x61, the grave move m->data (a GVMOVE): moves the dead of one grave size to another
   with gv_MoveGrave; s->state 2, then 1 when the move went through; returns gv_MoveGrave's result */

int cm_ExGraveMove(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    r = gv_MoveGrave(&m->data[0]);
    if (s != 0 && r == 0)
        s->state = 1;
    return r;
}
/* 0x4ccf10, handler of message 0x62, the raising of a dead person m->data (a GVRAISE): in a net game its objId is the
   server's ai_NextObjectId, otherwise its seed is filled with rand() and seeds util_Random; gv_RaiseDead raises the
   character, whose AICHAR pointer goes to s->value (s->state 2, then 1; kind 1); returns -1 when gv_RaiseDead found
   no grave or free slot (0xffff), else 0 */

int cm_ExRaiseDead(G17MSG *m, G17STAT *s)
{
    int id;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (net_Conn.s != -1)
        ai_NextObjectId = ((GVRAISE *)m->data)->objId;
    else
        ((GVRAISE *)m->data)->seed = rand();
    util_Seed(((GVRAISE *)m->data)->seed);
    id = gv_RaiseDead((GVRAISE *)m->data);
    if (id == 0xffff)
        return -1;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
/* 0x4ccf90, handler of message 99, the alchemist creation m->data (an AHMCREATEMSG): in a net game its charId is the
   server's ai_NextObjectId, otherwise its seed is filled with rand() and seeds util_Random; ahm_CreateAlchemist makes
   the alchemist, whose AICHAR pointer goes to s->value (s->state 2, then 1; kind 1); returns -1 when no character was
   made (0xffff), else 0 */

int cm_ExCreateAlchemist(G17MSG *m, G17STAT *s)
{
    int id;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (net_Conn.s != -1)
        ai_NextObjectId = ((AHMCREATEMSG *)m->data)->charId;
    else
        ((AHMCREATEMSG *)m->data)->seed = rand();
    util_Seed(((AHMCREATEMSG *)m->data)->seed);
    id = ahm_CreateAlchemistI((int *)m->data);
    if (id == 0xffff)
        return -1;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
/* 0x4cd010, handler of message 0x64 (the alchemist orders, AHMORDER): the command o->tag ('SmSa' add value,
   'ErFi' delete, 'SsCa' set share, 'cRSa' set a place's value, 'Rdaa' add a place, 'Rdsa' remove a place) is applied
   to the alchemist record of the character o->charId's building. Returns -1 for an unknown character, -2 when it has
   no alchemist record, -3 for an unknown command, else 0; s->state is 1 when the command succeeded, else 2 */

int cm_ExAlchemistOrder(G17MSG *m, G17STAT *s)
{
    AHMORDER *o;
    AHMCHAR *c;
    AHMOBJ *g;
    int r;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    c = (AHMCHAR *)gm_FindChar(((AHMORDER *)m->data)->charId);
    if (c == 0)
        return -1;
    if (ahm_HasRecord((char *)c) == 0)
        return -2;
    o = (AHMORDER *)m->data;
    switch ((unsigned int)o->tag) {
    case AHM_CMD_ADD_VALUE:
        r = ahm_AddValue((int *)o, (char *)c->obj->rec);
        break;
    case AHM_CMD_DELETE:
        g = c->obj;
        r = ahm_DeleteAlchemist((int)o, (int *)g->rec, (unsigned char *)g);
        break;
    case AHM_CMD_SET_SHARE:
        r = ahm_SetShare((int *)o, (char *)c->obj->rec);
        break;
    case AHM_CMD_SET_PLACE:
        r = ahm_SetPlaceValue((int *)o, (char *)c->obj->rec);
        break;
    case AHM_CMD_ADD_PLACE:
        g = c->obj;
        r = ahm_AddPlace((int *)o, (char *)g->rec, (unsigned char *)g);
        break;
    case AHM_CMD_REMOVE_PLACE:
        g = c->obj;
        r = ahm_RemovePlace((int *)o, (char *)g->rec, (unsigned char *)g);
        break;
    default:
        return -3;
    }
    if (s != 0 && r != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd130, handler of message 0x65, the master change m->master (a G17MASTERMSG): the party m->master.partyId gets
   the character m->master.masterId as its host (G28REC.host); s->state 2, then 1; returns 1 when the party is gone */

int cm_ExMasterChange(G17MSG *m, G17STAT *s)
{
    G28REC *q;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    q = (G28REC *)cm_PartyById(m->master.partyId);
    if (q == 0)
        return 1;
    q->host = m->master.masterId;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd180, handler of message 0x66: 'nCTa' reserves the effect m->effect.effectId, 'nCTd' stops it;
   s->state 2, then 1 when the effect call succeeds; returns -1 on a failed or unknown tag */

int cm_ExEffect(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    if (m->effect.tag == 0x6154436e)
        r = gv_ReserveEffect(m->effect.effectId);
    else if (m->effect.tag == 0x6454436e)
        r = gv_StopEffect(m->effect.effectId);
    else
        return -1;
    if (s != 0 && r != 0)
        s->state = 1;
    if (r != 0)
        return 0;
    return -1;
}
/* 0x4cd1e0, handler of message 0x67: starts the received cannon ball (GVSHOTMSG, the message data) in its effect
   slot; s->state 2, then 1 when the shot started; returns -1 without a slot or a free part */

int cm_ExShots(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    r = gv_StartShot((struct GVSHOTMSG *)&m->data[0], s);
    if (s != 0 && r != 0)
        s->state = 1;
    if (r != 0)
        return 0;
    return -1;
}
/* 0x4cd220, handler of type 0x68: the route points (RTPOINT, m->route.count of them) of the joined message become
   the route of the cart m->route.objId (its old route is freed): linked in a local copy, then rt_CopyPoints */

int cm_ExSetRoute(G17MSG *m, G17STAT *s)
{
    RTPOINT pts[9];
    AIOBJ *cart;
    RTPOINT *prev;
    int n, i;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    cart = (AIOBJ *)gm_FindObjectInst(m->route.objId);
    if (cart == 0)
        return -1;
    if (cart->route != 0) {
        rt_FreePoints(cart->route);
        cart->route = 0;
    }
    n = m->route.count;
    memcpy(pts, cm_LongMsg, n * sizeof(RTPOINT));
    prev = 0;
    for (i = 0; i < n; i++) {
        pts[i].prev = prev;
        pts[i].next = &pts[i + 1];
        prev = &pts[i];
    }
    pts[i - 1].next = 0;
    cart->route = rt_CopyPoints(pts);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd2f0, handler of message 0x69 (cm_SendClearRoute): frees the route of the cart m->route.objId (its
   route list, AIOBJ.route); s->state 2, then 1; returns -1 when the object is gone */

int cm_ExClearRoute(G17MSG *m, G17STAT *s)
{
    AIOBJ *q;

    if (s != 0) {
        s->state = 2;
        s->kind = 0;
        s->value = 0;
    }
    q = (AIOBJ *)gm_FindObjectInst(m->route.objId);
    if (q == 0)
        return -1;
    if (q->route != 0) {
        rt_FreePoints(q->route);
        q->route = 0;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd350, a free slot of the eight CMLOBBYPLAYER records of cm_LobbyRecords (id 0), 0: none */

char *cm_LobbyFreeRecord()
{
    int i = 0;
    CMLOBBYPLAYER *p;

    for (p = cm_LobbyPlayers; (int)p < (int)(cm_LobbyPlayers + 8); p++) {
        if (p->id == 0)
            return (char *)&cm_LobbyPlayers[i];
        i++;
    }
    return 0;
}
/* 0x4cd380, the lobby record (CMLOBBYPLAYER) with the id, else a free one (cm_LobbyFreeRecord); 0 for id 0 */

char *cm_LobbyRecord(int id)
{
    int i;
    CMLOBBYPLAYER *p;

    if (id == 0)
        return 0;
    i = 0;
    for (p = cm_LobbyPlayers; (int)p < (int)(cm_LobbyPlayers + 8); p++) {
        if (p->id == id)
            return (char *)&cm_LobbyPlayers[i];
        i++;
    }
    return cm_LobbyFreeRecord();
}
/* 0x4cd3c0, appends the chat line text of the player id sent to the player to (0: everybody) to the lobby chat ring cm_LobbyChat[15]; an unknown sender is named "???????" */

void cm_LobbyAddChat(int id, int to, char *text)
{
    char *p;

    p = cm_LobbyFindPlayer(id);
    if (p != 0)
        sprintf(cm_LobbyChat[cm_LobbyChatNext].name, "%s %s", p + 0x12, p + 0x22);
    else
        strcpy(cm_LobbyChat[cm_LobbyChatNext].name, "???????");
    cm_LobbyChat[cm_LobbyChatNext].id = id;
    cm_LobbyChat[cm_LobbyChatNext].to = to;
    strcpy(cm_LobbyChat[cm_LobbyChatNext].text, text);
    if (++cm_LobbyChatNext >= 15)
        cm_LobbyChatNext = 0;
    if (cm_LobbyChatCount < 15)
        cm_LobbyChatCount++;
}
/* 0x4cd480, the first line the lobby chat window reads: sets the read index cm_LobbyChatPos to
   cm_LobbyChatNext - cm_LobbyChatCount (to cm_LobbyChatNext + 1 when that is negative) and returns that
   line of the ring cm_LobbyChat; 0 when the ring is empty */

G18LOG *cm_LobbyChatFirst()
{
    int i;

    if (cm_LobbyChatCount == 0)
        return 0;
    i = cm_LobbyChatNext - cm_LobbyChatCount;
    cm_LobbyChatPos = i;
    if (i < 0) {
        i = cm_LobbyChatNext + 1;
        cm_LobbyChatPos = i;
    }
    return &cm_LobbyChat[i];
}
/* 0x4cd4c0, the next line the lobby chat window reads after cm_LobbyChatFirst: advances the read index
   cm_LobbyChatPos (15 wraps to 0) and returns that line; 0 when the cursor reaches the write index
   cm_LobbyChatNext */

G18LOG *cm_LobbyChatNextLine()
{
    int i = cm_LobbyChatPos;

    if (i == cm_LobbyChatNext)
        return 0;
    i++;
    cm_LobbyChatPos = i;
    if (i == cm_LobbyChatNext)
        return 0;
    if (i == 15) {
        i = 0;
        cm_LobbyChatPos = 0;
    }
    return &cm_LobbyChat[i];
}
/* 0x4cd500, empties the lobby chat ring: the write index cm_LobbyChatNext and the line count
   cm_LobbyChatCount go to 0 */

void cm_LobbyClearChat()
{
    cm_LobbyChatNext = 0;
    cm_LobbyChatCount = 0;
}
/* 0x4cd510, all players of the list (cm_LobbyListIds[8]) have state 1 and their number is the one asked for */

int cm_LobbyAllReady()
{
    unsigned int n;
    int cnt = 0;
    int *p;
    CMLOBBYPLAYER *q;

    if (cm_NetGame.mode == 0x3f)
        n = cm_NetGame.hdr.players;
    else
        n = cm_NetGame.hdr.players;
    if (cm_LobbyList[0] < n)
        return 0;
    for (p = cm_LobbyListIds; (int)p < (int)(cm_LobbyListIds + 8); p++) {
        if (*p != 0) {
            q = (CMLOBBYPLAYER *)cm_LobbyFindPlayer(*p);
            if (q == 0)
                return 0;
            if (q->ready == 1)
                cnt++;
        }
    }
    return cnt == n;
}
/* 0x4cd580, starts the lobby on the server's hello answer: clears the list and the records, marks it open and
   takes this player's lobby id and the hello's second value */

void cm_LobbyStart(int lobbyId, int value)
{
    memset(cm_LobbyList, 0, 0x24);
    memset(cm_LobbyRecords, 0, 0x2b8);
    cm_LobbyStateFlags = 1;
    cm_LobbyPlayerId = lobbyId;
    cm_LobbyHelloValue = value;
    cm_LobbyClosed = 0;
}
/* 0x4cd5d0, the lobby's player list cm_LobbyList (the count, then the eight player ids) */

unsigned int *cm_LobbyPlayerList()
{
    return cm_LobbyList;
}
/* 0x4cd5e0, the lobby record (CMLOBBYPLAYER) of the player id, else a free one; 0 for id 0 */

char *cm_LobbyFindPlayer(int id)
{
    return cm_LobbyRecord(id);
}
/* 0x4cd5f0, this player's lobby record (the one of the id cm_LobbyPlayerId) */

CMLOBBYPLAYER *cm_LobbyMyPlayer()
{
    return (CMLOBBYPLAYER *)cm_LobbyRecord(cm_LobbyPlayerId);
}
/* 0x4cd600, the lobby state: [0] the lobby is open (cm_LobbyStart), [1] a lobby message came in (cm_ExLobby) */

int *cm_LobbyState()
{
    return &cm_LobbyStateFlags;
}
/* 0x4cd610, sends the lobby message 0x5f kind 0 (asks for the player list); returns the send slot (cm_GetSlotState) */

int cm_LobbyRequestList()
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.lobby.kind = 0;
    m.type = 0x5f;
    return cm_QueueSend(&m);
}
/* 0x4cd650, kind 1: this player's ready flag v; returns the send slot */

int cm_LobbySetReady(int v)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.lobby.state.id = cm_LobbyPlayerId;
    m.type = 0x5f;
    m.lobby.kind = 1;
    m.lobby.state.value = v;
    return cm_QueueSend(&m);
}
/* 0x4cd6a0, kind 3: this player leaves the lobby; returns the send slot */

int cm_LobbyLeave()
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    m.lobby.kind = 3;
    m.lobby.state.id = cm_LobbyPlayerId;
    return cm_QueueSend(&m);
}
/* 0x4cd6e0, kind 6: a chat line (0x3f characters) of this player to the player id to (0: everybody); returns the
   send slot */

int cm_LobbySendChat(int to, char *text)
{
    G17MSG m;
    G17LOBBYCHAT t;

    memset(m.data, 0, 0x80);
    t.from = cm_LobbyPlayerId;
    t.to = to;
    strncpy(t.text, text, 0x3f);
    m.type = 0x5f;
    t.text[0x3f] = 0;
    m.lobby.kind = 6;
    m.lobby.chat = t;
    return cm_QueueSend(&m);
}
/* 0x4cd760, kind 2: this player joins with the record p (its id is set here); returns the send slot */

int cm_LobbyJoin(CMLOBBYPLAYER *p)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    p->id = cm_LobbyPlayerId;
    m.lobby.kind = 2;
    m.lobby.player = *p;
    return cm_QueueSend(&m);
}
/* 0x4cd7c0, kind 4: this player's changed record p (its id is set here); returns the send slot */

int cm_LobbyUpdatePlayer(CMLOBBYPLAYER *p)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    p->id = cm_LobbyPlayerId;
    m.lobby.kind = 4;
    m.lobby.player = *p;
    return cm_QueueSend(&m);
}
/* 0x4cd820, kind 5: asks for the lobby record of the player id (lobby_FetchPlayers sends one per listed
   player and waits on the send slot it returns) */

int cm_LobbyRequestPlayer(int id)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    m.lobby.kind = 5;
    m.lobby.player.id = id;
    return cm_QueueSend(&m);
}
/* 0x4cd870, the handler of type 0x5f (also 0x4cce00), the network lobby by m->lobby.kind (0..6): the player list,
   a player's ready flag or record, a player leaving, a chat line; sets cm_LobbyChanged */

int cm_ExLobby(G17MSG *m, G17STAT *s)
{
    CMLOBBYPLAYER *p;
    G17LOBBYCHAT t;

    if (s != 0)
        s->state = 2;
    switch (m->lobby.kind) {
    case 0:
        memcpy(cm_LobbyList, m->lobby.list, sizeof(m->lobby.list));
        break;
    case 1:
        p = (CMLOBBYPLAYER *)cm_LobbyRecord(m->lobby.state.id);
        if (p == 0)
            return -2;
        memcpy(p, &m->lobby.state, sizeof(m->lobby.state));
        break;
    case 2:
        p = (CMLOBBYPLAYER *)cm_LobbyFreeRecord();
        if (p == 0)
            return -1;
        memcpy(p, &m->lobby.player, sizeof(CMLOBBYPLAYER));
        break;
    case 3:
        p = (CMLOBBYPLAYER *)cm_LobbyRecord(m->lobby.state.id);
        if (p == 0)
            return -2;
        if (m->lobby.state.value == 1)
            cm_LobbyClosed = 1;
        memset(p, 0, sizeof(CMLOBBYPLAYER));
        break;
    case 4:
        p = (CMLOBBYPLAYER *)cm_LobbyRecord(m->lobby.player.id);
        if (p == 0)
            return -2;
        memcpy(p, &m->lobby.player, sizeof(CMLOBBYPLAYER));
        break;
    case 5:
        p = (CMLOBBYPLAYER *)cm_LobbyRecord(m->lobby.player.id);
        if (p == 0)
            return -2;
        memcpy(p, &m->lobby.player, sizeof(CMLOBBYPLAYER));
        break;
    case 6:
        memcpy(&t, &m->lobby.chat, sizeof(t));
        cm_LobbyAddChat(t.from, t.to, t.text);
        break;
    default:
        return 1;
    }
    if (s != 0)
        s->state = 1;
    cm_LobbyChanged = 1;
    return 0;
}
/* 0x4cd9d0, the weapon record (AIWEAPON) of the ten at ai_WeaponRecords with the goods item id id, 0: none */

char *ai_FindWeaponRecord(int id)
{
    int i = 0;
    AIWEAPON *p;

    for (p = ai_WeaponRecords; (int)p < (int)(ai_WeaponRecords + 10); p++) {
        if (p->id == id)
            return (char *)(ai_WeaponRecords + i);
        i++;
    }
    return 0;
}
/* 0x4cda00, the first weapon record (ai_FindWeaponRecord) of the kind 5 items of the character c's inventory */

char *cm_CharWeaponAny(char *c)
{
    AIOBJ *q;
    char *r;

    for (q = (AIOBJ *)gm_Query(((AICHAR *)c)->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        r = ai_FindWeaponRecord(q->id);
        if (r != 0)
            return r;
    }
    return ai_FindWeaponRecord(0);
}
/* 0x4cda50, as cm_CharWeaponAny; a record of kind 1 / 2 whose item has nothing left (AIOBJ.usesLeft) is not taken */

char *cm_CharWeapon(char *c)
{
    AIOBJ *q;
    AIWEAPON *r;

    for (q = (AIOBJ *)gm_Query(((AICHAR *)c)->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        r = (AIWEAPON *)ai_FindWeaponRecord(q->id);
        if (r != 0) {
            if ((r->kind == 1 || r->kind == 2) && q->usesLeft <= 0)
                return ai_FindWeaponRecord(0);
            return (char *)r;
        }
    }
    return ai_FindWeaponRecord(0);
}
/* 0x4cdac0, the inventory entry (AIOBJ, not its weapon record) of the search cm_CharWeapon does, 0: none */

char *cm_CharWeaponItem(char *c)
{
    AIOBJ *q;
    AIWEAPON *r;

    for (q = (AIOBJ *)gm_Query(((AICHAR *)c)->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        r = (AIWEAPON *)ai_FindWeaponRecord(q->id);
        if (r != 0) {
            if ((r->kind == 1 || r->kind == 2) && q->usesLeft <= 0)
                return 0;
            return (char *)q;
        }
    }
    return 0;
}
/* 0x4cdb20, gives up every weapon item of the character c's inventory: sends each as sold to nobody (cm_SendExSellObject, buyer -1) */

void cm_DiscardWeapons(char *c)
{
    AICHAR *ch = (AICHAR *)c;
    AIOBJ *q;
    int id;

    for (q = (AIOBJ *)gm_Query(ch->inventory, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
        id = q->id;
        if (ai_FindWeaponRecord(id) != 0)
            cm_SendExSellObject(-1, ch->netId, id, q->count, ai_OwnPlayer, 0);
    }
}
/* 0x4cdb80, after a fight, of a character c with the flag 0x20000000: with the chance c->b1c5 in 10 its weapons are
   taken (cm_DiscardWeapons), b1c5 is negated and the owner of its building (a character of kind 6 / 7) gets the news
   0x1bca naming the weapon; returns 1. Else b1c5 becomes 1, the flag is cleared; returns 0 */

char cm_FightWeaponLoss(char *ch)
{
    char buf[0x800];
    char one = 1;
    char neg;
    AICHAR *c = (AICHAR *)ch;
    AICHAR *owner;
    AIWEAPON *w;

    if (c != 0 && (c->flags1c8 & 0x20000000) != 0) {
        neg = c->b1c5;
        if (util_RandomU(10) < (unsigned char)neg) {
            neg = -neg;
            cm_DiscardWeapons((char *)c);
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValues(1, 1, (int)((char *)c - cm_FieldObject) + 0x1c5, &neg);
            cm_SendFieldDeltas();
            if (c->bld != 0 && c->bld->owner != 0xffff) {
                owner = &ai_CharsT[c->bld->owner];
                if (owner->kind == 6 || owner->kind == 7) {
                    w = (AIWEAPON *)cm_CharWeapon((char *)c);
                    if (w->id != 0) {
                        txt_Format(buf, 0x1bca, w->id * 2 + 0x9a8, c->id, c->bld);
                        he_ShowNews(owner->netId, -1, buf, 0, 0x5f2, 0);
                    }
                }
            }
            return 1;
        }
        cm_BeginFieldBlock((char *)c, c->netId);
        cm_AddFieldValues(1, 1, (int)((char *)c - cm_FieldObject) + 0x1c5, &one);
        cm_SendFieldDeltas();
        cm_SendSetField(c->netId, 0x1c8, 4, 0, 0x20000000);
    }
    return 0;
}
/* 0x4cdcf0, the fighting strength of the character c: its health ratio times its skill 3 times a factor of the
   weapon it carries (cm_CharWeapon's item: 0x158 1.0, 0x154 0.5, 0x156 / 0x16e / 0x172 0.8, 0x174 / 0x176 0.9,
   0x15e / 0x160 0.7, another 0.3; none 0.5) */

float cm_FightStrength(char *c)
{
    float f = 0.5f;
    float skill;
    AIWEAPON *w;

    skill = gm_GetSkill(c, 3);
    w = (AIWEAPON *)cm_CharWeapon(c);
    if (w != 0) {
        switch (w->id) {
        case 0x158:
            f = 1.0f;
            break;
        case 0x154:
            f = 0.5f;
            break;
        case 0x156:
        case 0x16e:
        case 0x172:
            f = 0.8f;
            break;
        case 0x174:
        case 0x176:
            f = 0.9f;
            break;
        case 0x15e:
        case 0x160:
            f = 0.7f;
            break;
        default:
            f = 0.3f;
        }
    }
    return gm_HealthRatio(c) * skill * f;
}
/* 0x4cde20, puts the weapon the character c carries (cm_CharWeapon) into its scene character's right hand as the
   model "ob_<GOOD NAME>" (unless it holds that already); without a weapon the hand is emptied */

void cm_UpdateHandObject(char *ch)
{
    char buf[0x80];
    AICHAR *c = (AICHAR *)ch;
    AIWEAPON *w;
    AIGOOD *g;

    w = (AIWEAPON *)cm_CharWeapon(ch);
    if (w != 0 && w->id != 0) {
        g = &ai_GoodsT[w->id];
        if (c->scene == 0)
            return;
        sprintf(buf, "ob_%s", _strupr(g->name));
        if (c->scene->rightHand != 0 && _strcmpi(c->scene->rightHand->name, buf) == 0)
            return;
        if (c->scene->rightHand != 0)
            ch_SetHandObject(c->scene, 2, 0);
        ch_SetHandObject(c->scene, 2, buf);
        return;
    }
    if (c->scene != 0 && c->scene->rightHand != 0)
        ch_SetHandObject(c->scene, 2, 0);
}
/* 0x4cdf00, the record of the character c in the 16 records (0x2c bytes, id at +0) of rec + 0x94, else a free one
   (id -1), 0: none */

int *cm_CutEntrySlot(char *rec, char *c)
{
    CMCUTINFO *r = (CMCUTINFO *)rec;
    int id = ((AICHAR *)c)->netId;
    int i;

    for (i = 0; i < 16; i++) {
        if (r->entries[i].charId == id)
            return &r->entries[i].charId;
    }
    for (i = 0; i < 16; i++) {
        if (r->entries[i].charId == -1) {
            memset(&r->entries[i], 0, sizeof(CMCUTENTRY));
            r->entries[i].charId = ((AICHAR *)c)->netId;
            r->entries[i].state = 0;
            return &r->entries[i].charId;
        }
    }
    return 0;
}
/* 0x4cdf80, the entry (CMCUTENTRY) of the character c in the first cm_FightRecord[0x30] cutscene records
   (cm_CutInfos), 0: none */

int *cm_FightCutEntry(char *c)
{
    int n = (unsigned char)cm_FightRecord[0x30];
    CMCUTINFO *r;
    int i, j;

    for (i = 0, r = cm_CutInfosT; i < n; i++, r++) {
        for (j = 0; j < 16; j++) {
            if (r->entries[j].charId == ((AICHAR *)c)->netId)
                return &r->entries[j].charId;
        }
    }
    return 0;
}
/* 0x4cdfe0, 1 when the cell (x, y) of the height map d3_DefaultScene->hmap is inside and passable */

int cm_CellUsable(int x, int y)
{
    D3SM *g = (D3SM *)d3_DefaultScene->hmap;
    int w = g->size;
    int type;

    if (x > w || x < 0 || y < 0 || y > w || x >= w || y >= w)
        return 0;
    type = g->entries[w * y + x].type;
    if (type == 0 || type == 0xd)
        return 0;
    return 1;
}
/* 0x4ce040, the nearest usable cell (cm_CellUsable) of the current scene's height map around the cell (a, b), in
   diamonds of the distance d = 0 .. 0x3f, rows top to bottom: *ox, *oy, returns 1; 0: none */

int cm_FindNearCell(int a, int b, int *ox, int *oy)
{
    int w = ((D3SM *)d3_DefaultScene->hmap)->size;
    int d, y, x, k, ylo, yhi, xlo, xhi;

    for (d = 0; d < 0x40; d++) {
        yhi = w - 1 < b + d ? w - 1 : b + d;
        ylo = b - d < 0 ? 0 : b - d;
        for (y = ylo; y <= yhi; y++) {
            k = d - abs(b - y);
            xhi = w - 1 < k + a ? w - 1 : k + a;
            xlo = a - k < 0 ? 0 : a - k;
            for (x = xlo; x <= xhi; x++) {
                if (cm_CellUsable(x, y) != 0) {
                    *ox = x;
                    *oy = y;
                    return 1;
                }
            }
        }
    }
    return 0;
}
/* 0x4ce130, the fighter (an AICHAR, 0x21c bytes) with the netId id among the 32 of cm_Fighters, 0 when none */

char *cm_FindFighter(int id)
{
    int i = 0;
    char *p;

    for (p = cm_Fighters + 4; (int)p < (int)(cm_Fighters + 4 + 32 * 0x21c); p += 0x21c) {
        if (((AICHAR *)(p - 4))->id != 0xffff && ((AICHAR *)(p - 4))->netId == id)
            return cm_Fighters + i * 0x21c;
        i++;
    }
    return 0;
}
/* 0x4ce170, the select bits of the fighter c: 0x800 when its side leader (bld in a fight) is the character idx,
   or when gm_AllowAll is set */

int ai_LedSelectBits(int idx, char *c)
{
    int r = 0;

    if ((AICHAR *)((AICHAR *)c)->bld == (AICHAR *)ai_Chars[idx])
        r = 0x800;
    if (gm_AllowAll != 0)
        r = 0x800;
    return r;
}
/* 0x4ce1b0, the distance of the character c's model to the object o (x / z) */

float cm_CharObjDistXZ(char *c, char *o)
{
    D3VECTOR d, b;

    d.x = ((D3OBJECT *)o)->pos.x;
    d.z = ((D3OBJECT *)o)->pos.z;
    b = ((AICHAR *)c)->scene->obj->pos;
    d.x -= b.x;
    d.z -= b.z;
    return (float)sqrt(d.z * d.z + d.x * d.x);
}
/* 0x4ce200, the distance in the x / z plane of the character c's body to the vertex of cell (x, y) (the frame holds two 16-byte vectors) */

float cm_CharCellDistXZ(char *c, int x, int y)
{
    D3VECTOR4 v, d;
    D3VECTOR b;

    d3sm_GetVertex((struct D3SM *)d3_DefaultScene->hmap, x, y, (D3VECTOR *)&v);
    v.y = 0;
    b.x = ((AICHAR *)c)->scene->obj->pos.x;
    b.z = ((AICHAR *)c)->scene->obj->pos.z;
    d.x = v.x - b.x;
    d.z = v.z - b.z;
    return (float)sqrt(d.z * d.z + d.x * d.x);
}
/* 0x4ce270, 1 when the two characters are not in the same building (+0x16c) */

int cm_OtherBuilding(char *a, char *b)
{
    return ((AICHAR *)a)->bld != ((AICHAR *)b)->bld;
}
/* 0x4ce290, a bomb object "ob_BOMBE" at the position p in a free slot of the 32 (object, time) of cm_Bombs */

CMBOMB *cm_LayBomb(D3VECTOR *p)
{
    D3VECTOR rot;
    int i;
    CMBOMB *b;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    for (i = 0; i < 32; i++) {
        if (cm_BombsT[i].obj == 0) {
            b = &cm_BombsT[i];
            b->obj = d3_CreateObject(0, p, &rot, "ob_BOMBE");
            cm_BombsT[i].time = d3_Time;
            return b;
        }
    }
    return 0;
}
/* 0x4ce300, updates the bombs of cm_Bombs: a bomb older than 0x15e ms explodes (the sound "Explosion_s", smoke, earth
   and explosion effects at its position) and hurts every scene character within 170.0 by a random weapon damage (0x174)
   scaled by its life value and the distance; returns the number of used slots
   @deep read whole against the original: bomb walk (index loop, jl), the vectors a/b (D3VECTOR4, a reused
   as the difference), the colour, the three effect calls prototyped (float pushes), the scene-char walk,
   the damage and the if/else tail-merged cm_ShowDamage calls all match. Left: allocation only - the
   original caches 0 in edi (xor edi,edi; push edi / mov [ebp],edi, edi re-zeroed after the inner loop)
   where ours uses immediates, it loads c into eax then edi with ch (ebx) read before the health test
   (ours: edi then ebx after), the frame is 0x3c vs our 0x38 (one slot more: dist at +0x20, t at +0x1c),
   and ours schedules fld dist / fmul 1/170 / fsubr 1.0 before the k * (f01c * 0.01) product instead of
   after the _ftol. Tried: t as a statement or inline, operand order, declaration order: no change. */

int cm_UpdateBombs()
{
    int cnt;
    AIWEAPON *w;
    CMBOMB *rec;
    AICHAR *c;
    CHARACTER *ch;
    D3OBJECT *bo;
    D3VECTOR4 a, b;
    D3COLOR col;
    float dist;
    unsigned int k;
    int i, j, t, amt;

    cnt = 0;
    w = (AIWEAPON *)ai_FindWeaponRecord(0x174);
    for (i = 0; i < 32; i++) {
        rec = &cm_BombsT[i];
        if (rec->obj == 0)
            continue;
        cnt++;
        if (d3_Time <= rec->time + 0x15e)
            continue;
        a.x = -243.0f;
        a.y = -90.0f;
        a.z = 190.0f;
        a.w = 0;
        b.x = -50.0f;
        b.y = 0.0f;
        b.z = 38.0f;
        b.w = 0;
        sb_PlaySampleI(cm_BombSounds, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        col.r = 0xc0;
        col.g = 0xc0;
        col.b = 0xc0;
        col.a = 0x3f;
        a.x = rec->obj->pos.x;
        a.y = rec->obj->pos.y;
        a.z = rec->obj->pos.z;
        d3t_CreateSmoke(0, (D3VECTOR *)&a, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, (D3VECTOR *)&b, 0x190, 6, col);
        d3t_CreateEarth(0, (D3VECTOR *)&a, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, col);
        d3t_CreateExplosion(0, (D3VECTOR *)&a, 0x1e, 120.0f, 80.0f, 20.0f, 60.0f, 0x3c);
        for (j = 0; j < 32; j++) {
            c = ai_SceneCharsT[j];
            if (c == 0)
                continue;
            ch = c->scene;
            if (c->health == 0)
                continue;
            bo = rec->obj;
            a.x = bo->pos.x - ch->obj->pos.x;
            a.y = bo->pos.y - ch->obj->pos.y;
            a.z = bo->pos.z - ch->obj->pos.z;
            dist = (float)sqrt(a.z * a.z + a.y * a.y + a.x * a.x);
            if (dist < 170.0f) {
                k = cm_Random(w->dmgMax - w->dmgMin) + w->dmgMin;
                t = (int)(k * (c->f01c * 0.01));
                amt = (int)(t * (1.0 - dist * 0.0058823530562222f));
                ai_SceneCharsT[j]->v024 -= amt;
                if ((int)ai_SceneCharsT[j]->bld == cm_FightAttacker)
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 1);
                else
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 2);
                if (cm_CheckDeath(c) == 0)
                    ch_QueueAnim(ch, "kampf/bauch_greifen", 1);
            }
        }
        d3if_Delete(rec->obj);
        rec->obj = 0;
    }
    return cnt;
}
/* 0x4ce5d0, throws a bomb: the first free slot of cm_Grenades gets the object "ob_BOMBE" at p (60.0 higher) and a
   4-key path animation (up to half way + 50.0, then down) to the vertex of the cell (x, y); 0 when all 32 are used */

CMGRENADE *cm_ThrowGrenade(D3VECTOR *p, int x, int y)
{
    D3ANIMKEY keys[4];
    D3VECTOR4 mid;
    D3VECTOR4 pos;
    D3VECTOR rot;
    D3ANIMFLAGS af;
    D3VECTOR4 d;
    struct D3SM *g;
    int i;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    g = (struct D3SM *)d3_DefaultScene->hmap;
    for (i = 0; i < 32; i++) {
        if (cm_GrenadesT[i].obj != 0)
            continue;
        pos.x = p->x;
        pos.y = (float)(p->y + 60.0);
        pos.z = p->z;
        cm_GrenadesT[i].obj = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, "ob_BOMBE");
        cm_GrenadesT[i].obj->objectFlags |= 4;
        d3sm_GetVertex(g, x, y, (D3VECTOR *)&d);
        d.x -= p->x;
        d.y -= p->y;
        d.z -= p->z;
        memset(keys, 0, sizeof(keys));
        mid.x = (float)(d.x * 0.5);
        mid.y = (float)(d.y * 0.5 + 50.0);
        mid.z = (float)(d.z * 0.5);
        D3_ZERO(&af, sizeof(af));
        af.b[1] |= 0x14;
        af.b[8] |= 2;
        af.b[1] |= 8;
        keys[0].time = 0x12;
        keys[1].time = 0x12;
        keys[1].pos.x = mid.x;
        keys[1].pos.y = mid.y;
        keys[1].pos.z = mid.z;
        keys[1].rot.x = 3.1415927f;
        keys[1].rot.y = 3.1415927f;
        keys[1].rot.z = 3.1415927f;
        keys[2].time = 1;
        d.y = (float)(d.y - 60.0);
        keys[2].pos.x = d.x;
        keys[2].pos.y = d.y;
        keys[2].pos.z = d.z;
        keys[2].rot.x = 3.1415927f;
        keys[2].rot.y = 3.1415927f;
        keys[2].rot.z = 3.1415927f;
        keys[3].pos.x = d.x;
        keys[3].pos.y = d.y;
        keys[3].pos.z = d.z;
        keys[3].rot.x = 3.1415927f;
        keys[3].rot.y = 3.1415927f;
        keys[3].rot.z = 3.1415927f;
        cm_GrenadesT[i].anim = d3anim_CreatePath(cm_GrenadesT[i].obj, af, 4, keys);
        cm_GrenadesT[i].x = x;
        cm_GrenadesT[i].y = y;
        return &cm_GrenadesT[i];
    }
    return 0;
}
/* 0x4ce830, updates the thrown bombs of cm_Grenades: one whose path has ended (flag 0x20 of D3ANIM.flags2d) explodes
   (smoke, earth and explosion effects at its position), lowers the terrain at its cell by 5 and clears the cell's
   type there (the map rebuilt), sounds "Explosion_s" and hurts every scene character within 100.0 of the cell's
   vertex by a random weapon damage (0x176) scaled by its life value and the distance; returns the number of used
   slots
   @deep read whole against the original: grenade walk (index loop over CMGRENADE, base +0xc, jl),
   path-ended test D3ANIM.flags2d & 0x20, vectors a/b (D3VECTOR4, a reused for the vertex difference),
   colour, Smoke/Earth/Explosion prototyped (float pushes, 70/50/20/60), the terrain cell (types / height
   at x + y*size, reloaded d3_Terrain for the clear, MakeDivide then MakeLight), the sound, the scene-char
   damage with the tail-merged cm_ShowDamage calls and kampf/arm_greifen all match. Left: allocation only,
   as cm_UpdateBombs - the original caches 0 in ebx (xor ebx,ebx; push ebx / cmp eax,ebx, re-zeroed after
   the inner loop) where ours uses immediates, so the terrain index and the vertex call use other registers
   (imul edx,[eax] / add ecx,edx vs imul eax,[ecx] / add eax,ebp), c goes eax->ebp with ch read before the
   health test, frame 0x40 vs 0x3c (one slot more), and the (1.0 - dist*0.01f) product is scheduled before
   the k product in ours. (references: M8 at +0x1a1: ours refers to d3fl_MakeDivide (0x441300), the
   original's field to 0x441860 (d3fl_MakeLight), explained: the M8 pairs offsets across a
   register-allocation hunk: both sides call d3fl_MakeDivide then d3fl_MakeLight with d3_Terrain (asmdiff
   --symbols: ours 0x4ce9d1 / 0x4ce9dd, the original 0x4ce9c2 / 0x4ce9ce)) */

int cm_UpdateGrenades()
{
    int cnt;
    AIWEAPON *w;
    struct D3SM *g;
    CMGRENADE *rec;
    AICHAR *c;
    CHARACTER *ch;
    D3VECTOR4 a, b;
    D3COLOR col;
    D3FLMAP *map;
    float dist;
    unsigned int k;
    int i, j, t, amt, cell;

    g = (struct D3SM *)d3_DefaultScene->hmap;
    cnt = 0;
    w = (AIWEAPON *)ai_FindWeaponRecord(0x176);
    for (i = 0; i < 32; i++) {
        rec = &cm_GrenadesT[i];
        if (rec->obj == 0)
            continue;
        cnt++;
        if (!(rec->anim->flags2d & 0x20))
            continue;
        a.x = -243.0f;
        a.y = -90.0f;
        a.z = 190.0f;
        a.w = 0;
        b.x = -50.0f;
        b.y = 0.0f;
        b.z = 38.0f;
        b.w = 0;
        col.r = 0xc0;
        col.g = 0xc0;
        col.b = 0xc0;
        col.a = 0x3f;
        a.x = rec->obj->pos.x;
        a.y = rec->obj->pos.y;
        a.z = rec->obj->pos.z;
        d3t_CreateSmoke(0, (D3VECTOR *)&a, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, (D3VECTOR *)&b, 0x190, 6, col);
        d3t_CreateEarth(0, (D3VECTOR *)&a, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, col);
        d3t_CreateExplosion(0, (D3VECTOR *)&a, 0x1e, 70.0f, 50.0f, 20.0f, 60.0f, 0x3c);
        map = (D3FLMAP *)d3_Terrain;
        cell = rec->x + rec->y * map->size;
        if (map->types[cell] != 0) {
            map->height[cell] -= 5;
            ((D3FLMAP *)d3_Terrain)->types[rec->x + rec->y * ((D3FLMAP *)d3_Terrain)->size] = 0;
            d3fl_MakeDivide((D3FLMAP *)d3_Terrain);
            d3fl_MakeLight((D3FLMAP *)d3_Terrain);
        }
        sb_PlaySampleI(cm_BombSounds, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        for (j = 0; j < 32; j++) {
            c = ai_SceneCharsT[j];
            if (c == 0)
                continue;
            ch = c->scene;
            if (c->health == 0)
                continue;
            d3sm_GetVertex(g, rec->x, rec->y, (D3VECTOR *)&a);
            a.x -= ch->obj->pos.x;
            a.y -= ch->obj->pos.y;
            a.z -= ch->obj->pos.z;
            dist = (float)sqrt(a.z * a.z + a.y * a.y + a.x * a.x);
            if (dist < 100.0f) {
                k = cm_Random(w->dmgMax - w->dmgMin) + w->dmgMin;
                t = (int)(k * (c->f01c * 0.01));
                amt = (int)(t * (1.0 - dist * 0.01f));
                ai_SceneCharsT[j]->v024 -= amt;
                if ((int)ai_SceneCharsT[j]->bld == cm_FightAttacker)
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 1);
                else
                    cm_ShowDamage((char *)ai_SceneCharsT[j], amt, 2);
                if (cm_CheckDeath(c) == 0)
                    ch_QueueAnim(ch, "kampf/arm_greifen", 1);
            }
        }
        d3if_Delete(rec->obj);
        rec->obj = 0;
    }
    return cnt;
}
/* 0x4ceb70, the game record the object c stands for (its owner) */

int cm_ObjectOwner(char *c)
{
    return (int)((D3OBJECT *)c)->owner;
}
/* 0x4ceb80, waits for the event id to end, then stops it */

void cm_WaitEventStop(int id)
{
    char *e;

    if (cm_SkipEvents != 0)
        return;
    if (sv_RunFrame(0, 0x7d6c6) != 0) {
        do {
            if (dm_L.rup != 0 || kb_Last != 0)
                break;
            if (evt_FindById(id) == 0)
                return;
        } while (sv_RunFrame(0, 0x7d6c6) != 0);
    }
    e = (char *)evt_FindById(id);
    if (e != 0)
        evt_Stop(e);
    di_ClearButtons();
}
/* 0x4cec00, closes the 32 windows of cm_CharInfoWins */

void cm_CloseCharInfoWins()
{
    int *p;

    for (p = cm_CharInfoWins; (int)p < (int)(cm_CharInfoWins + 0x20); p++) {
        if (*p != -1) {
            d2_RemoveWindow(*p);
            *p = -1;
        }
    }
}
/* 0x4cec30, the info window of the character c: a window 16 pixels around the screen box of its body (at least 110
   wide; pictures 0x7b0 / 0x7b1 in its corners) with the value bar of skill 3 and the health slider (0..100), in a free
   slot of cm_CharInfoWins; first the record of c in a (cm_CutEntrySlot) is looked up or taken */

void cm_OpenCharInfoWin(int a, char *c)
{
    D3RECT r;
    int x0, y0, w, h, i, hp, bar;

    if (d3if_ScreenBox(((AICHAR *)c)->scene->obj, &r) == 0)
        return;
    x0 = r.left - 0x10;
    w = r.right - r.left + 0x20;
    y0 = r.top - 0x10;
    if (w <= 0x6e)
        w = 0x6e;
    h = r.bottom - r.top + 0x20;
    for (i = 0; i < 0x20; i++) {
        if (cm_CharInfoWins[i] != -1)
            continue;
        cm_CutEntrySlot((char *)a, c);
        cm_CharInfoWins[i] = d2_CreateWindow(x0, y0, w, h, 0x10);
        d2_WinOpenPic(0, 0, 0x7b0, d2_LastWindow);
        d2_WinOpenPic(w - 0x18, 0, 0x7b1, d2_LastWindow);
        d2_Wins[d2_LastWindow].font = 0x54;
        sv_ValueBar(4, 4, gm_SkillPoints(c, 3), 0x4df);
        hp = (int)(gm_HealthRatio(c) * 100.0);
        bar = d2_WinOpenSlider(0, 0xe, 0x40, 0, 0x64, 0x75, 0x442, d2_LastWindow);
        d2_SetObjValue(bar, 0, 0x64, hp);
        return;
    }
}
/* 0x4ced80, refreshes the character info windows: closes them, then opens one for every selected fighter (a: the
   fight record the cut entries go to) */

void cm_UpdateCharInfoWins(int a)
{
    char *p;

    cm_CloseCharInfoWins();
    for (p = cm_Fighters; (int)p < (int)(cm_Fighters + 0x4380); p += 0x21c) {
        if (p[0x188] != 0)
            cm_OpenCharInfoWin(a, p);
    }
}
/* 0x4cedc0, shows the damage v over the character c as a floating number (v relative to its life value f01c, in
   percent) in a free slot of the 16 of cm_DamagePops (kind 1 / 2 drawn with signs, else plain) */

void cm_ShowDamage(char *c, int v, int kind)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_DamagePops[i].v != 0)
            continue;
        cm_DamagePops[i].v = (int)((double)v / (((AICHAR *)c)->f01c * 0.01));
        cm_DamagePops[i].c = (AICHAR *)c;
        cm_DamagePops[i].t = 64.0f;
        cm_DamagePops[i].kind = kind;
        return;
    }
}
/* 0x4cee20, shows / moves the damage numbers of cm_ShowDamage over their characters (created on first show, kind 1 /
   2 between sign glyphs); each rises half a pixel a step and is removed after 128 steps; a character off screen hides
   its number and ends the pass */

void cm_UpdateDamagePops()
{
    char buf[0x40];
    D3RECT box;
    int i, x, y0;

    for (i = 0; i < 16; i++) {
        if (cm_DamagePops[i].v == 0)
            continue;
        if (d3if_ScreenBox(cm_DamagePops[i].c->scene->obj, &box) == 0) {
            if (cm_DamagePops[i].h != -1)
                d2_EnableObject(cm_DamagePops[i].h, 0);
            return;
        }
        x = box.left + 0x10;
        y0 = box.top - 0x30;
        if (cm_DamagePops[i].h == -1) {
            if (cm_DamagePops[i].kind == 1)
                sprintf(buf, "%c-%i%c", 0x5c, cm_DamagePops[i].v, 0x5d);
            else if (cm_DamagePops[i].kind == 2)
                sprintf(buf, "%c-%i%c", 0x5e, cm_DamagePops[i].v, 0x5f);
            else
                sprintf(buf, "-%i", cm_DamagePops[i].v);
            cm_DamagePops[i].h = d2_CreateScreenText(x, (int)(y0 + cm_DamagePops[i].t), buf);
        } else
            d2_MoveObj(x, (int)(y0 + cm_DamagePops[i].t), cm_DamagePops[i].h);
        if (cm_DamagePops[i].h != -1)
            d2_EnableObject(cm_DamagePops[i].h, 1);
        cm_DamagePops[i].t = (float)(cm_DamagePops[i].t - 0.5);
        if (cm_DamagePops[i].t <= 0.0f) {
            d2_RemoveObj(cm_DamagePops[i].h);
            cm_DamagePops[i].v = 0;
            cm_DamagePops[i].h = -1;
            cm_DamagePops[i].c = 0;
        }
    }
}
/* 0x4cef80, clears the 16 damage numbers of cm_DamagePops (no screen text, no value, no character) */

void cm_InitDamagePops()
{
    int i;

    for (i = 0; i < 16; i++) {
        cm_DamagePops[i].h = -1;
        cm_DamagePops[i].v = 0;
        cm_DamagePops[i].c = 0;
    }
}
/* 0x4cefa0, removes all damage numbers of cm_DamagePops (their screen texts too) */

void cm_ClearDamagePops()
{
    int i;

    for (i = 0; i < 16; i++) {
        if (cm_DamagePops[i].h != -1)
            d2_RemoveObj(cm_DamagePops[i].h);
        cm_DamagePops[i].h = -1;
        cm_DamagePops[i].v = 0;
        cm_DamagePops[i].c = 0;
    }
}
/* 0x4cefe0, closes the fight view (cm_FightView) and marks it closed (-1) */

void cm_CloseFightView()
{
    if (cm_FightView != -1) {
        d2_CloseView(cm_FightView);
        cm_FightView = -1;
    }
}
/* 0x4cf000, the per-tick update of the scene characters (ai_SceneChars): on the fight owner's machine, every 0x15e
   ticks, a character whose health is 2..4 loses one point and the damage of weapon 0x16e (the poison, reasoned) is
   sent for it (health 1 is reset to 100); a character holding weapon 0x172 is shown to the others only near them
   (faded by the distance to the nearest one in another building, rest 255) or half-transparent when cm_CutChar
   sees it (0x800); a fighter in cutscene state 2 turns towards its partner; ends with the damage numbers
   @deep Read whole against 0x4cf000..0x4cf370: the same calls with the same arguments (VecEqual c then o,
   SetObjectAlpha with the info pointer and the D3TEXFLAGS by value, SetObjectRotXYZ with the float angles,
   fabs(ang) > 0.08), constants (255 - 255 * (best * 0.01f)) and stores. Left: the hunk at +0x77..+0xa1, a
   scheduling difference: the original loads c->health (xor edx,edx; mov dl,[esi+8]; push edx) after the
   _ftol of the damage, ours before the fild (xor ecx,ecx; mov cl,[esi+8]; push ecx), with the qword temp
   at [esp+0x20] vs [esp+0x24] by that push; tried v as a separate statement and inline, n int / unsigned:
   the same code. Besides: regs hunks and an fpu hunk (fld rotation.y before / fadd after the fmul 0.25). */

void cm_UpdateSceneChars()
{
    AICHAR *c, *d, *o;
    AIWEAPON *w;
    CMCUTENTRY *e;
    CHARACTER *g;
    D3OBJECT *body;
    D3TEXFLAGS col;
    int i, j, fl, found;
    unsigned int n;
    float best, dist, ang;
    unsigned char b;

    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && d3_Time % 0x15e == 0) {
        for (i = 0; i < 32; i++) {
            c = ai_SceneCharsT[i];
            if (c == 0)
                continue;
            b = c->health;
            if (b > 4 || b <= 0)
                continue;
            b--;
            c->health = b;
            if (b == 1) {
                c->health = 100;
                continue;
            }
            w = (AIWEAPON *)ai_FindWeaponRecord(0x16e);
            n = cm_Random(w->dmgMax - w->dmgMin);
            cm_SendDamage(c->netId, (int)((n + w->dmgMin) * c->f01c * 0.01), c->health);
        }
    }
    for (i = 0; i < 32; i++) {
        c = ai_SceneCharsT[i];
        if (c == 0)
            continue;
        w = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (w != 0 && w->id == 0x172) {
            fl = 0;
            if (cm_CutChar != 0)
                fl = ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c);
            if (fl & 0x800) {
                col.all = 0;
                col.b[2] = 0;
                col.b[0] = 0x80;
                d3if_ShowTree(c->scene->obj, 1);
                c->scene->obj->stateFlags &= 0xfd;
                d3_SetObjectAlpha(c->scene->obj, c->scene->obj->info, col);
            } else {
                best = 999999.0f;
                found = 0;
                for (j = 0; j < 32; j++) {
                    d = ai_SceneCharsT[j];
                    if (d != 0 && cm_OtherBuilding((char *)c, (char *)d) != 0 && d->health != 0) {
                        dist = cm_CharObjDistXZ((char *)c, (char *)d->scene->obj);
                        if (dist < 100.0f) {
                            found = 1;
                            if (dist < best)
                                best = dist;
                        }
                    }
                }
                d3if_ShowTree(c->scene->obj, found);
                if (found == 0)
                    goto turn;
                col.all = 0;
                col.b[2] = 0;
                col.b[0] = (unsigned char)(int)(255.0f - 255.0f * (best * 0.01f));
                c->scene->obj->stateFlags &= 0xfd;
                d3_SetObjectAlpha(c->scene->obj, c->scene->obj->info, col);
            }
            c->scene->obj->stateFlags |= 2;
        }
turn:
        e = (CMCUTENTRY *)cm_FightCutEntry((char *)c);
        if (e != 0 && e->state == 2 && e->turn != 0) {
            o = (AICHAR *)cm_FindFighter(e->stateArg);
            if (o != 0) {
                if (d3_VecEqual(&c->scene->obj->pos, &o->scene->obj->pos, 10.0f) == 0) {
                    ang = d3_YawTo(c->scene->obj, &o->scene->obj->pos.x);
                    g = c->scene;
                    if (g->actions == 0 || (g->actions->type != 0x2d && g->actions->type != 0x3a)) {
                        if (fabs(ang) > 0.08) {
                            body = g->obj;
                            d3_SetObjectRotXYZ(body, body->rotation.x, body->rotation.y + ang * 0.25, body->rotation.z);
                        }
                    }
                }
            }
        }
    }
    cm_UpdateDamagePops();
}
/* 0x4cf370, draws the scroll arrows (sprite ids 0 .. 7 of the sheet d2_GetPic(0x52c)): the mouse hit tests and
   whether the camera's preset views d3_Camera->views[1] .. [4] are set (their position not d3_Origin) pick the
   pressed (2) or the free (1) look; the top and bottom arrows sit 75 screen units in from the edge (the top one's
   sprite 30 units tall) */

void cm_DrawScrollArrows()
{
    int e = 0, up = 0, a = 0, b = 0;
    int t;

    t = d2_GetPic(0x52c);
    if (sv_BorderViewX == -1) {
        up = 1;
        if (d3_VecEqual(&d3_Camera->views[4].pos, &d3_Origin, 0.2f) == 0)
            up = 2;
    } else if (sv_BorderViewX == 1) {
        e = 1;
        if (d3_VecEqual(&d3_Camera->views[3].pos, &d3_Origin, 0.2f) == 0)
            e = 2;
    } else if (sv_BorderViewX == 0) {
        if (d3_VecEqual(&d3_Camera->views[3].pos, &d3_Origin, 0.2f) == 0)
            e = 1;
        if (d3_VecEqual(&d3_Camera->views[4].pos, &d3_Origin, 0.2f) == 0)
            up = 1;
    }
    if (sv_BorderViewY == -1) {
        a = 1;
        if (d3_VecEqual(&d3_Camera->views[1].pos, &d3_Origin, 0.2f) == 0)
            a = 2;
    } else if (sv_BorderViewY == 1) {
        b = 1;
        if (d3_VecEqual(&d3_Camera->views[2].pos, &d3_Origin, 0.2f) == 0)
            b = 2;
    } else if (sv_BorderViewY == 0) {
        if (d3_VecEqual(&d3_Camera->views[1].pos, &d3_Origin, 0.2f) == 0)
            a = 1;
        if (d3_VecEqual(&d3_Camera->views[2].pos, &d3_Origin, 0.2f) == 0)
            b = 1;
    }
    d2_SetClipRect(0, 0, ((D2SURF *)d2_ScreenSurf)->w, ((D2SURF *)d2_ScreenSurf)->h);
    if (a == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * 525.0f - 30.0f), d2_ScreenSurf, t, 0);
    else if (a == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * 525.0f - 30.0f), d2_ScreenSurf, t, a);
    if (e == 2)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, e);
    else if (e == 1)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 3);
    if (up == 2)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 4);
    else if (up == 1)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 5);
    if (b == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * cm_Float75), d2_ScreenSurf, t, 6);
    else if (b == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * cm_Float75), d2_ScreenSurf, t, 7);
}
/* 0x4cf670, the mouse pick: the object under the cursor (ai_PickedObject; with characters selected and nothing picked,
   the ground cell under the cursor is looked up, else the pick marker is hidden) and the cursor look d2_PointerLook:
   by the weapon of the selected characters cm_CutChar sees (0x160 -> 0x18 .. 0x176 -> 0x25; more than one: 0x23) over
   a character it does not see (or with key 0x36 down), "sp_ESCAPE" 0x1c, "sp_CONQUER" 0x1b, "WARE" 0xe;
   cm_HoverObject keeps the object the look was decided for
   @deep Read whole against 0x4cf670..0x4cf9c0: the same calls and arguments (d3if_Pick, d3sm_Pick (x, z) /
   d3sm_GetVertex, v.y += 10.0f into the escaped vector, d3if_Show of cm_PickMarker, the 31-entry
   ai_SceneChars loop with the weapon if-chain and n, strcmp sp_ESCAPE inline, strncmp sp_CONQUER / WARE,
   the kind 2 / owner test), the same stores and returns. Left: allocation. Ours keeps ai_PickedObject in a
   callee-saved register across the calls of ai_LedSelectBits (VC6 sees its definition above in this unit:
   a leaf that stores nothing) and reuses it for cm_HoverObject; the original reloads it after every call
   (mov ecx, [ai_PickedObject] at +0x307 / +0x32b) and after the loop (+0x215), so it keeps n in ebx, cur
   in ebp and the loop pointer in edi, where ours spills n and the loop pointer to the frame (0x1c vs 0x18)
   and pushes 6 more instructions. Tried a pointer walk (jb instead of the original's jl: the index form is
   right) and a call through a K&R cast: no change. Hypothesis: the fight code 0x4cf670.. was its own TU in
   the original (the lead comment's join), so ai_LedSelectBits was external there (references: M8 at
   +0x32c: ours refers to cm_HoverObject (0x6f0ce0), the original's field to 0x6f22e8 (ai_PickedObject),
   explained: the M8 at +0x32c is the same store cm_HoverObject = ai_PickedObject: the original's field is
   the reload of ai_PickedObject that ours replaces by the register it cached, not another object) */

void cm_UpdatePointer()
{
    int i, n = 0, cur = 0x23;
    AICHAR *c;
    AIWEAPON *w;
    struct D3HMAP *map;
    int z, x;
    D3VECTOR v;

    map = d3_DefaultScene->hmap;
    ai_PickedObject = 0;
    d2_PointerLook = 0;
    if (dm_L.x > 0 && dm_L.x < d2_ScreenW && dm_L.y > 0 && dm_L.y < d2_ScreenH &&
        d2_PointerObj == -1) {
        ai_PickedObject = d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x40, 1);
        if (ai_PickedObject == 0 && cm_SelectedCount != 0) {
            if ((d3sm_PickK(map, dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z) & 0xff) != -1) {
                d3sm_GetVertexI(map, x, z, &v);
                v.y += 10.0f;
                d2_PointerLook = 0;
            }
        } else if (cm_PickMarker != 0) {
            d3if_Show(cm_PickMarker, 0);
        }
    }
    if (ai_PickedObject == 0) {
        cm_HoverObject = 0;
        ai_PickedChar = 0;
        return;
    }
    for (i = 0; i < 31; i++) {
        c = ai_SceneCharsT[i];
        if (c != 0 && c->selected != 0) {
            if (ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c) & 0x800) {
                if (kb_Down[0x36] == 0) {
                    w = (AIWEAPON *)cm_CharWeapon((char *)c);
                    if (w != 0) {
                        if (w->id == 0x160)
                            cur = 0x18;
                        else if (w->id == 0x174)
                            cur = 0x19;
                        else if (w->id == 0x16e)
                            cur = 0x1e;
                        else if (w->id == 0x154)
                            cur = 0x1a;
                        else if (w->id == 0x158)
                            cur = 0x1f;
                        else if (w->id == 0x15e)
                            cur = 0x21;
                        else if (w->id == 0x156)
                            cur = 0x23;
                        else if (w->id == 0x176)
                            cur = 0x25;
                    }
                    n++;
                }
            }
        }
    }
    if (n > 1)
        cur = 0x23;
    if (ai_PickedObject != 0 && d2_PointerObj == -1) {
        if (strcmp(ai_PickedObject, "sp_ESCAPE") == 0 && n != 0) {
            d2_PointerLook = 0x1c;
            cm_HoverObject = ai_PickedObject;
            return;
        }
        if (strncmp(ai_PickedObject, "sp_CONQUER", 10) == 0 && n != 0) {
            d2_PointerLook = 0x1b;
            cm_HoverObject = ai_PickedObject;
            return;
        }
        if (strncmp(ai_PickedObject, "WARE", 4) == 0 && n != 0) {
            d2_PointerLook = 0xe;
            cm_HoverObject = ai_PickedObject;
            return;
        }
        if (((D3OBJECT *)ai_PickedObject)->kind == 2 && n != 0) {
            if (!(ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)((D3OBJECT *)ai_PickedObject)->owner) & 0x800) ||
                kb_Down[0x36] != 0) {
                cm_HoverObject = ai_PickedObject;
                d2_PointerLook = cur;
                return;
            }
        } else
            d2_PointerLook = 0;
        cm_HoverObject = ai_PickedObject;
        return;
    }
    d2_PointerLook = 0;
}

#define G19_E(e, o) (*(int *)((e) + (o)))
#define txt_FormatS ((void (*)(char *, char *, ...))txt_Format)  /* txt_Format with a format text where the id goes (the token engine takes either) */

/* 0x4cf9c0, refreshes the portrait boxes cm_Portraits: after a click a portrait's checked state becomes its
   character's selection; each box of a character whose body is of type 1 hides its window and frame, the others
   show the selection, the name with the health in percent ("%1N1 %i%"), the picture of the weapon (replaced when the
   weapon changed) and, for a kind 1 / 2 weapon, the count left; cm_SelectedCount counts the selected ones
   @deep Read whole against 0x4cf9c0..0x4cfc0a: the same calls, arguments, stores and branches in the same
   order (click loop on checked, the type 1 body hiding d2_Wins[win] / d2_Wins[frame].parent, selection,
   txt_Format name / health, the weapon picture replaced when D2OBJ.pic differs, the uses left text). Left:
   VC6's choices only: ours keeps -1 in ebp (or ebp, -1; cmp reg, ebp) where the original compares with the
   immediate and uses ebp for d2_Objs, and ours biases the loop pointer of the second loop to
   &cm_Portraits[0].win (mov esi, cm_Portraits+8; fields at -8 / -4 / +0 .. +0x14, end +0xc8) where the
   original walks from the struct (+0 .. +0x1c, end +0xc0). Tried a CMPORTRAIT * walk (worse: 49 ins) and
   the index form (this) (references: M8 at +0x74: ours refers to cm_Portraits+0x8 (0x930408), the
   original's field to 0x930400 (cm_Portraits) (and 1 more), explained: the two M8 are the biased induction
   pointer: +0x74 is the loop start &cm_Portraits[0].win against &cm_Portraits[0], +0x234 its end
   &cm_Portraits[6].win against &cm_Portraits[6] (cm_DamagePops follows the array); every field access
   lands on the same address) */

void cm_UpdatePortraits()
{
    char buf[0x100];
    int i;
    AICHAR *c;
    AIWEAPON *w;
    AIOBJ *item;

    if (d2_ClickedId != -1) {
        for (i = 0; i < 6; i++) {
            if (cm_Portraits[i].win != -1 && d2_PointerObj == cm_Portraits[i].portrait) {
                if (d2_Objs[cm_Portraits[i].portrait].checked != 0)
                    cm_Portraits[i].c->selected = 1;
                else
                    cm_Portraits[i].c->selected = 0;
            }
        }
    }
    d2_ClearPointerItems();
    cm_SelectedCount = 0;
    for (i = 0; i < 6; i++) {
        if (cm_Portraits[i].win == -1)
            continue;
        c = cm_Portraits[i].c;
        if (c->scene->obj->type == 1) {
            d2_EnableObject(d2_Wins[cm_Portraits[i].win].parent, 0);
            d2_EnableObject(d2_Wins[cm_Portraits[i].frame].parent, 0);
            continue;
        }
        if (c->selected != 0) {
            d2_AddPointerItem(c->picture, 1);
            d2_SetObjValue(cm_Portraits[i].portrait, 1);
            cm_SelectedCount++;
        } else
            d2_SetObjValue(cm_Portraits[i].portrait, 0);
        txt_FormatS(buf, "%1N1 %i%", cm_Portraits[i].c->id, (int)(gm_HealthRatio(cm_Portraits[i].c) * 100.0));
        d2_SetObjText(cm_Portraits[i].text, buf);
        w = (AIWEAPON *)cm_CharWeaponAny((char *)cm_Portraits[i].c);
        if (w == 0 || w->id == 0)
            continue;
        if (cm_Portraits[i].weaponPic == -1 || d2_Objs[cm_Portraits[i].weaponPic].pic != w->id + 0xdf) {
            if (cm_Portraits[i].weaponPic != -1)
                d2_RemoveObj(cm_Portraits[i].weaponPic);
            d2_LastWindow = cm_Portraits[i].win;
            cm_Portraits[i].weaponPic = d2_WinOpenPic(0x54, 0xf, w->id + 0xdf, cm_Portraits[i].win);
            d2_ScaleObject(cm_Portraits[i].weaponPic, -2);
        }
        if (cm_Portraits[i].countText == -1)
            continue;
        if (w->kind == 1 || w->kind == 2) {
            item = (AIOBJ *)cm_CharWeaponItem((char *)cm_Portraits[i].c);
            if (item != 0) {
                sprintf(buf, "%i", item->usesLeft);
                d2_SetObjText(cm_Portraits[i].countText, buf);
                continue;
            }
        }
        if (cm_Portraits[i].countText != -1)
            d2_SetObjText(cm_Portraits[i].countText, &ch_EmptyName);
    }
}
#define G19_R(id, o, t) (*(t *)(d2_ObjsC + (id) * 0x2ec + (o)))

/* 0x4cfc10, builds the portrait boxes cm_Portraits (at most 6, 0x80 pixels apart) for the characters of
   ai_SceneChars that cm_CutChar sees (0x800): a frame picture, a window with the weapon picture and its uses left text
   (when the character has a weapon), the portrait (checkable), the name text and the value bar of skill 3; then
   cm_UpdatePortraits fills them */

void cm_BuildPortraits()
{
    AICHAR *c;
    AIWEAPON *w;
    int i, n, x = 0;
    float t;

    for (i = 0; i < 6; i++) {
        cm_Portraits[i].win = -1;
        cm_Portraits[i].portrait = -1;
        cm_Portraits[i].unused0c = -1;
        cm_Portraits[i].text = -1;
        cm_Portraits[i].countText = -1;
        cm_Portraits[i].weaponPic = -1;
    }
    for (i = 0, n = 0; i < 31 && n < 6; i++) {
        c = ai_SceneCharsT[i];
        if (c == 0)
            continue;
        if (!(ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c) & 0x800))
            continue;
        cm_Portraits[n].frame = d2_CreatePicObj(x + 0x18, 0xf, 0x5da);
        cm_Portraits[n].win = d2_CreateWindow(x, 0, 0x80, 0x4b, 0x110);
        w = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (w != 0 && w->id != 0) {
            cm_Portraits[n].weaponPic = d2_WinOpenPic(0x54, 0xf, w->id + 0xdf, d2_LastWindow);
            d2_ScaleObject(cm_Portraits[n].weaponPic, -2);
            cm_Portraits[n].countText = d2_CreateWinText(0x54, 0x26, &ch_EmptyName, d2_LastWindow);
            d2_Objs[cm_Portraits[n].countText].font = 0x54;
            d2_Objs[cm_Portraits[n].countText].w = 0x18;
            d2_Objs[cm_Portraits[n].countText].centred = 1;
        }
        cm_Portraits[n].c = c;
        d2_Wins[cm_Portraits[n].win].mt = 0;
        d2_Wins[cm_Portraits[n].win].font = 0x54;
        cm_Portraits[n].portrait = d2_WinOpenPic(0x28, 0x11, c->picture, d2_LastWindow);
        d2_Objs[cm_Portraits[n].portrait].checkable = 1;
        cm_Portraits[n].text = d2_CreateWinText(0, 0, &ch_EmptyName, d2_LastWindow);
        d2_Objs[cm_Portraits[n].text].font = 0x54;
        d2_Objs[cm_Portraits[n].text].w = 0x80;
        d2_Objs[cm_Portraits[n].text].centred = 1;
        t = gm_SkillPoints(c, 3);
        sv_ValueBar(0x24, 0x41, (int)t, 0x4df);
        x += 0x80;
        n++;
    }
    cm_UpdatePortraits();
}
/* 0x4cfe70, removes the portrait boxes cm_Portraits[6] again */

void cm_ClosePortraits()
{
    int i;
    int t;

    for (i = 0; i < 6; i++) {
        if (cm_Portraits[i].win != -1) {
            d2_RemoveWindow(cm_Portraits[i].win);
            t = cm_Portraits[i].frame;
            cm_Portraits[i].win = -1;
            d2_RemoveObj(t);
        }
    }
}
/* 0x4cfeb0, the click on an own character object (ai_PickedObject, D3OBJECT.kind 2) while characters are selected: the first
   selected standing scene character is marked (AICHAR.selected) and gets the click comment "_ARBEITER_KOMMENTARE_KLICK" from
   the attacker's or the defender's voice bank (by its building); the picked object becomes ai_ClickedObject. */

void cm_ClickSceneChar()
{
    AICHAR *t = (AICHAR *)cm_CutChar;
    AICHAR *c, *o, **pp;
    D3OBJECT *sel;

    ai_ClickedObject = 0;
    for (pp = ai_SceneCharsT; (int)pp < (int)(ai_SceneCharsT + 32); pp++) {
        c = *pp;
        if (c != 0 && c->selected != 0 && (ai_LedSelectBits(t->id, (char *)c) & 0x800) && c->scene != 0 &&
            c->health != 0)
            break;
    }
    if (dm_L.lup != 0 && d2_PointerObj == -1) {
        sel = (D3OBJECT *)ai_PickedObject;
        if (sel != 0 && sel->kind == 2) {
            o = sel->owner;
            if ((ai_LedSelectBits(t->id, (char *)o) & 0x800) && kb_Down[0x36] == 0 &&
                (c == 0 || cm_OtherBuilding((char *)c, (char *)o) == 0)) {
                cm_ClearMarks();
                o->selected = 1;
                if ((int)o->bld == cm_FightAttacker)
                    snd_QueueVoice(o->id, cm_AttackerClickVoices, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                else
                    snd_QueueVoice(o->id, cm_DefenderClickVoices, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                sel = (D3OBJECT *)ai_PickedObject;
            }
        }
        ai_ClickedObject = (int)sel;
    }
}
/* 0x4cffd0, orders the character c of the cutscene record rec (CMCUTINFO) to attack: its entry (cm_CutEntrySlot, b0c and
   held cleared there) is copied with state 2 and tgt's netId, for the weapon kinds 0x15e / 0x160 also the cell (x, z) of
   tgt's body, for 0x174 / 0x176 that cell alone, and sent as message 0x50 (cm_SendCutEntry); held is set for the
   formation types 1 .. 3. A weapon of those kinds with no uses left shows its txt_WeaponEmpty text instead. -1 when c,
   tgt, its body or the entry is missing; else the original returns nothing (eax as the last call left it)
   @deep Read whole against the original: the three weapon-empty checks were inverted (the original returns
   -1 only when the item exists with usesLeft <= 0) - fixed; byte +0xc of the entry is a byte store
   (CMCUTENTRY.b0c). Left: VC6 cross-jumps ours' 0x174 ok path into the 0x176 block (L[4]=0, x, z, then the
   shared charId/state tail), while the original keeps the 0x174 path inline at 0x4d00f9 (mov edx,[ebp+4];
   ... mov [esp+0xc],edx; mov [esp+0x1c],esi; [esp+0x20]; [esp+0x24]; jmp 0x4d01c5) - the same stores in
   the same source order; reordering the 0x174 statements does not change ours. The other hunks are regs
   (eax->ecx ecx->edx) of the same blocks. Block layout / tail-merge, no meaning difference. */

int cm_OrderAttack(char *rec, char *c, char *tgt, int x, int z)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    AICHAR *ch = (AICHAR *)c;
    AICHAR *t = (AICHAR *)tgt;
    AIWEAPON *w;
    AIOBJ *item;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    float y;

    if (ch == 0)
        return -1;
    w = (AIWEAPON *)cm_CharWeapon(c);
    if (t == 0 || t->scene->obj == 0)
        return -1;
    if (d3_GetHeightAt(hmap, &t->scene->obj->pos, &x, &z, &y) == 0)
        return -1;
    e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    if (e == 0)
        return -1;
    e->held = 0;
    e->b0c = 0;
    memcpy(&order, e, sizeof(order));
    if (w == 0 || w->id == 0x154 || w->id == 0x156 || w->id == 0x158 || w->id == 0x16e || w->id == 0x172 ||
        w->id == 0) {
        order.stateArg = t->netId;
        order.charId = ch->netId;
        order.state = 2;
    } else if (w->id == 0x15e || w->id == 0x160) {
        item = (AIOBJ *)cm_CharWeaponItem(c);
        if (item != 0 && item->usesLeft <= 0) {
            sv_SetStatusText((char *)txt_WeaponEmpty15e);
            return -1;
        }
        order.charId = ch->netId;
        order.stateArg = t->netId;
        order.x = x;
        order.z = z;
        order.state = 2;
    } else if (w->id == 0x174) {
        item = (AIOBJ *)cm_CharWeaponItem(c);
        if (item != 0 && item->usesLeft <= 0) {
            sv_SetStatusText((char *)txt_WeaponEmpty174);
            return -1;
        }
        order.charId = ch->netId;
        order.stateArg = 0;
        order.x = x;
        order.z = z;
        order.state = 2;
    } else if (w->id == 0x176) {
        item = (AIOBJ *)cm_CharWeaponItem(c);
        if (item != 0 && item->usesLeft <= 0) {
            sv_SetStatusText((char *)txt_WeaponEmpty176);
            return -1;
        }
        order.stateArg = 0;
        order.x = x;
        order.z = z;
        order.charId = ch->netId;
        order.state = 2;
    }
    if (ch->formation == 1 || ch->formation == 2 || ch->formation == 3)
        order.held = 1;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, &order);
}
/* 0x4d0200, orders the character c of the cutscene record rec (CMCUTINFO) to flee to the escape object tgt
   ("sp_ESCAPE"): its entry (cm_CutEntrySlot, b0c cleared there) is copied with state 3 and the ground cell of tgt's
   world position in stateArg / x, and sent as message 0x50 (cm_SendCutEntry). -1 when tgt, c or rec is missing or the
   position has no cell; else the original returns nothing (VC6 keeps x, z, y in the dead parameters' slots) */

int cm_OrderEscape(char *rec, char *c, char *tgt)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    D3OBJECT *o = (D3OBJECT *)tgt;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    float v[4];                         /* d3_PointToWorld writes the point and the angle after it */
    int x, z;
    float y;

    if (o == 0 || c == 0 || rec == 0)
        return -1;
    d3_PointToWorld(o, &o->pos, (D3VECTOR *)v);
    if (d3_GetHeightAt(hmap, (D3VECTOR *)v, &x, &z, &y) == 0)
        return -1;
    e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    if (e == 0)
        return -1;
    e->b0c = 0;
    memcpy(&order, e, sizeof(order));
    order.charId = ((AICHAR *)c)->netId;
    order.stateArg = x;
    order.state = 3;
    order.x = z;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, &order);
}
/* 0x4d02d0, an order kind 4 (the walk to a named dummy) for the character c with the name of the object tgt
   (CMCUTENTRY.dummy) and its ground cell (cellX, cellZ); returns 1, 0 when the point is not on the ground. */

int cm_OrderConquer(char *rec, char *c, char *tgt)
{
    char *sc = (char *)d3_DefaultScene->hmap;
    CMCUTENTRY *q;
    float v[4];
    int x, z, h;

    cm_CharWeapon(c);
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &x, &z, &h) == 0)
        return 0;
    q = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    q->b0c = 0;
    strcpy(q->dummy, tgt);
    q->charId = ((AICHAR *)c)->netId;
    q->state = 4;
    q->cellX = x;
    q->cellZ = z;
    return 1;
}
/* 0x4d0380, an order kind 6 (walk to the goods store) for the character c: its entry (cm_CutEntrySlot, b0c cleared
   there) is copied with state 6, the name of the object tgt (dummy), held = a and turn = the low byte of tgt's point,
   and sent as message 0x50 (cm_SendCutEntry). -1 when the point is not on the ground or the character c is in a job
   (AICHAR.v1ac). */

int cm_OrderWare(char *rec, char *c, char *tgt, unsigned char a)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    D3OBJECT *o = (D3OBJECT *)tgt;
    CMCUTENTRY *q;
    CMCUTENTRY order;
    float v[4];
    int x, z;
    float y;

    cm_CharWeapon(c);
    d3_PointToWorld(o, &o->pos, (D3VECTOR *)v);
    if (d3_GetHeightAt(hmap, (D3VECTOR *)v, &x, &z, &y) == 0 || ((AICHAR *)c)->v1ac != 0)
        return -1;
    q = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    q->b0c = 0;
    memcpy(&order, q, sizeof(order));
    order.charId = ((AICHAR *)c)->netId;
    order.state = 6;
    strcpy(order.dummy, tgt);
    order.held = a;
    order.turn = ((char *)v)[0];
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, &order);
}
/* 0x4d0460, orders the character c of the cutscene record rec (CMCUTINFO) to the best ground cell within 8 rings of
   its own (cm_BestCellNear, formation type 4): its entry (cm_CutEntrySlot, b0c cleared there) is copied with state 7 and
   that cell in stateArg / x, and sent as message 0x50 (cm_SendCutEntry). -1 when c's body is not on the ground or no
   cell is found; else the original returns nothing (VC6 keeps x in c's dead parameter slot) */

int cm_OrderToBestCell(char *rec, char *c)
{
    D3SM *hmap = (D3SM *)d3_DefaultScene->hmap;
    AICHAR *ch = (AICHAR *)c;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    int x, z;
    float y;

    if (d3_GetHeightAt(hmap, &ch->scene->obj->pos, &x, &z, &y) == 0)
        return -1;
    if (cm_BestCellNear(c, x, z, 8, &x, &z) == 0)
        return -1;
    e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    e->b0c = 0;
    memcpy(&order, e, sizeof(order));
    order.charId = ch->netId;
    order.stateArg = x;
    order.state = 7;
    order.x = z;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, &order);
}
/* 0x4d0520, the formation type 3's order for the character c: its entry (cm_CutEntrySlot, b0c cleared there) is copied
   with state 8 and no target and sent as message 0x50 (cm_SendCutEntry); case 8 of cm_RunCutOrders then sends fight
   command 6 once the previous order slot is done (the character waits). */

void cm_OrderWait(char *rec, char *c)
{
    CMCUTENTRY *q;
    CMCUTENTRY order;

    q = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
    q->b0c = 0;
    memcpy(&order, q, sizeof(order));
    order.charId = ((AICHAR *)c)->netId;
    order.state = 8;
    cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, &order);
}

#define cm_OrderWare3 ((int (*)(char *, char *, char *))cm_OrderWare)

/* 0x4d0570, the order of a mouse click (dm_L.lup) for the character c of the cutscene record rec (CMCUTINFO): on a
   character's body (D3OBJECT.kind 2) an attack (cm_OrderAttack) when it is not in c's building or shift is down; on
   "sp_ESCAPE" cm_OrderEscape, on "sp_CONQUER.." cm_OrderConquer, on "WARE.." cm_OrderWare; with nothing picked the ground
   cell under the pointer (d3sm_Pick) is sent in state 1 (stateArg / x), else txt_ClickNoGround is shown. Returns 1 when
   an order was given
   @deep Read whole against the original: fixed the shift key (kb_Down[0x36], DIK_RSHIFT) and the
   ground-click path (ret = 1 falls through to the common return: the original keeps 1 in bl/ebx for both
   state and result); every call, string and store now lines up. Left: one hunk at +0x11a, the argument
   evaluation of d3sm_Pick(d3_DefaultScene->hmap, dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z): the original loads
   [d3_DefaultScene] first and pushes the +0x1a sums in ecx/edx, ours loads dm_L.y first (movsx edx / lea
   eax,[esp+0x10] / ... / mov edx,[ecx+0xb0]) - the same five pushes of the same values; tried the K&R
   d3sm_PickK spelling and a prototyped call: no change. Scheduling only; the
   prover stops at the inlined strcmp (rep cmpsb). */
char cm_OrderClick(char *rec, char *c)
{
    char ret = 0;
    D3OBJECT *sel;
    char *o;
    CMCUTENTRY *e;
    CMCUTENTRY order;
    int x, z;

    if (dm_L.lup != 0) {
        sel = (D3OBJECT *)ai_ClickedObject;
        if (sel != 0) {
            if (sel->kind == 2) {
                o = (char *)sel->owner;
                if (cm_OtherBuilding(c, o) != 0 || kb_Down[DIK_RSHIFT] != 0) {
                    if (cm_OrderAttack(rec, c, o, 0, 0) != -1)
                        ret = 1;
                }
            } else if (strcmp(sel->name, "sp_ESCAPE") == 0) {
                cm_OrderEscape(rec, c, (char *)sel);
                ret = 1;
            } else if (strncmp(sel->name, "sp_CONQUER", 10) == 0) {
                cm_OrderConquer(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            } else if (strncmp((char *)ai_ClickedObject, "WARE", 4) == 0) {
                cm_OrderWare3(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            }
        } else if (d3sm_Pick((D3SM *)d3_DefaultScene->hmap, dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z)) {
            e = (CMCUTENTRY *)cm_CutEntrySlot(rec, c);
            e->b0c = 0;
            memcpy(&order, e, sizeof(order));
            order.charId = ((AICHAR *)c)->netId;
            order.stateArg = x;
            order.state = 1;
            order.x = z;
            cm_SendCutEntry(((CMCUTINFO *)rec)->playerId, &order);
            ret = 1;
        } else
            sv_SetStatusText((char *)txt_ClickNoGround);
    }
    return ret;
}
/* 0x4d0740, the per-tick mouse handling of the fight for the cutscene record rec: the left button held starts the
   rubber band (sv_StartBand, the marks cleared first), its release ends it; a click gives every marked scene character
   (AICHAR.selected) that cm_CutChar may command (ai_LedSelectBits 0x800) and that is alive its order (cm_OrderClick),
   answered with the voice "_K_C2C_BEFEHL_ERHALTEN" of its side, and drops the mark. Then the marked characters' bodies
   are greyed (d3t_SetGrey, D3OBJECT.stateFlags bit 2) and the unmarked ones restored, unless the body is fading; a
   right click clears every mark (cm_ClearMarks)
   @deep Read whole against the original: every call, store and branch of the band / click part lines up;
   the highlight loop reads the same fields (scene->flags 0x40 fading, selected, obj->stateFlags & 2) and
   greys / restores the same way. Left (+0x11a..): the original loads selected into dl and stateFlags into
   cl once before the branch (test bl, cl / and cl, bl / cmp cl, bl with bl = 2), ours reads stateFlags in
   each branch (test byte ptr [ecx+0x212], bl; mov dl, [ecx+0x212]; and dl, bl) - one load of an unchanged
   byte against two, the rest regs (ecx<->eax). Tried a local for the flags (unsigned char / int, with and
   without a local for selected): VC6 then hoists 0x40 into bl instead of 2 and the hunk grows. Allocation
   / load CSE only. */

void cm_FightMouse(char *rec)
{
    AICHAR **pp;
    AICHAR *c;
    D3OBJECT *body;

    if (dm_L.lhold == 0 && sv_BandActive != 0)
        sv_EndBand();
    if (dm_L.lhold != 0 && dm_L.rdown == 0) {
        if (sv_BandActive == 0 && di_Absolute != 0 && d2_PointerObj == -1) {
            cm_ClearMarks();
            sv_StartBand();
        }
    } else if (dm_L.lup != 0) {
        for (pp = ai_SceneCharsT; (int)pp < (int)(ai_SceneCharsT + 32); pp++) {
            c = *pp;
            if (c != 0 && c->selected != 0 && (ai_LedSelectBits(((AICHAR *)cm_CutChar)->id, (char *)c) & 0x800) &&
                c->scene != 0 && c->health != 0) {
                if (cm_OrderClick(rec, (char *)c)) {
                    if (cm_CutChar == (char *)cm_FightAttacker)
                        snd_PlayVoice(c->id, cm_AttackerOrderVoices, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    else
                        snd_PlayVoice(c->id, cm_DefenderOrderVoices, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    c->selected = 0;
                }
            }
        }
    }
    for (pp = ai_SceneCharsT; (int)pp < (int)(ai_SceneCharsT + 32); pp++) {
        c = *pp;
        if (c != 0 && !(c->scene->flags.bytes.lo & 0x40)) {
            body = c->scene->obj;
            if (c->selected) {
                if (!(body->stateFlags & 2))
                    d3t_SetGrey(body, 1);
            } else if ((body->stateFlags & 2) == 2)
                d3t_SetGrey(body, 0);
        }
    }
    if (dm_L.rup != 0)
        cm_ClearMarks();
}
/* 0x4d08c0, clears the marks (AICHAR.selected) of the 32 cm_Fighters and the grey highlight of their bodies
   (D3OBJECT.stateFlags bit 2: d3t_SetColor black), then the pointer items, the character info windows and the fight
   view */

void cm_ClearMarks()
{
    AICHAR *r;
    D3OBJECT *body;

    for (r = cm_FightersT; (int)&r->scene < (int)&cm_FightersT[32].scene; r++) {
        if (r->selected != 0)
            r->selected = 0;
        if (r->scene != 0) {
            body = r->scene->obj;
            if ((body->stateFlags & 2) == 2)
                d3t_SetColor(body, 0, 0, 0);
        }
    }
    d2_ClearPointerItems();
    cm_CloseCharInfoWins();
    cm_CloseFightView();
}
/* 0x4d0920, the fight copy of the character with the netId id: the first free record of cm_Fighters (id 0xffff; 0 when
   all 32 are taken) gets a copy of the AICHAR, without its object v1ac and scene character, and a new scene character
   (gm_CreateCharModel at the dummy object, path type 4, flag 0x8000) with its hand object, the speed of its weapon
   (AIWEAPON.speed * 2/7) and "cmb_<netId>" as its body's name; the weapon item's uses are reset to 6 (kind 2) or 15 */

char *cm_AddFighter(int id, char *dummy)
{
    AICHAR *c;
    AIWEAPON *w;
    AIOBJ *item;
    int i;

    c = (AICHAR *)gm_FindChar(id);
    for (i = 0; i < 32; i++) {
        if (cm_FightersT[i].id == 0xffff) {
            memcpy(&cm_FightersT[i], c, sizeof(AICHAR));
            cm_FightersT[i].v1ac = 0;
            cm_FightersT[i].scene = 0;
            gm_CreateCharModel(&cm_FightersT[i], d3_CurrentScene, 0, 0, 0, dummy);
            cm_FightersT[i].scene->pathType = 4;
            cm_FightersT[i].scene->flags.bytes.hi |= 0x80;
            cm_UpdateHandObject((char *)&cm_FightersT[i]);
            w = (AIWEAPON *)cm_CharWeapon((char *)c);
            cm_FightersT[i].scene->walkSpeed = w->speed * cm_WeaponSpeedScale;
            sprintf(cm_FightersT[i].scene->obj->name, "cmb_%i", cm_FightersT[i].netId);
            item = (AIOBJ *)cm_CharWeaponItem((char *)c);
            if (item != 0) {
                if (w->kind == 2)
                    item->usesLeft = 6;
                else
                    item->usesLeft = 0xf;
            }
            return (char *)&cm_FightersT[i];
        }
    }
    return 0;
}
/* 0x4d0a30, fills ai_SceneChars (32 pointers, cleared first) with the cm_Fighters whose scene character is in the scene
   d3_DefaultScene, in record order; at most 31 are taken */

void cm_CollectSceneChars()
{
    AICHAR **p;
    AICHAR *r;

    memset(ai_SceneCharsT, 0, sizeof(ai_SceneCharsT));
    p = ai_SceneCharsT;
    for (r = cm_FightersT; (int)&r->scene < (int)&cm_FightersT[32].scene; r++) {
        if ((int)p >= (int)&ai_SceneCharsT[31])
            break;
        if (r->scene != 0 && r->scene->scene == d3_DefaultScene)
            *p++ = r;
    }
}
/* 0x4d0a80, the object walk's callback for the flag dummies: "dummy_sp_ESCAPE" gets a flag object "ub_FLUCHTFLAGGE"
   (named "sp_ESCAPE", the animation "anim_FLUCHTFLAGGE.baf"), "dummy_CONQUER" one "ub_CONQUER" (named
   "sp_CONQUER_<n>"), both kind 4 with mask 1 and kept in cm_FlagObjects[cm_FlagObjectCount++]; returns 1. */

char cm_CreateDummyFlag(char *o)
{
    D3OBJECT *obj = (D3OBJECT *)o;
    D3OBJECT *r;
    float pos[4];
    float rot[3];

    if (strstr(o, "dummy_sp_ESCAPE") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)pos);
        r = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ub_FLUCHTFLAGGE");
        cm_FlagObjectsT[cm_FlagObjectCount] = r;
        strcpy(r->name, "sp_ESCAPE");
        cm_FlagObjectsT[cm_FlagObjectCount]->kind = 4;
        cm_FlagObjectsT[cm_FlagObjectCount]->mask = 1;
        cm_FlagObjectsT[cm_FlagObjectCount]->stateFlags = (cm_FlagObjectsT[cm_FlagObjectCount]->stateFlags & 0xf7) | 4;
        cm_FlagObjectsT[cm_FlagObjectCount]->stateFlags |= 0x40;
        d3t_PlayAnim(cm_FlagObjectsT[cm_FlagObjectCount], "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
        cm_FlagObjectCount++;
        return 1;
    }
    if (strstr(o, "dummy_CONQUER") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)pos);
        r = d3_CreateObject(0, (D3VECTOR *)pos, (D3VECTOR *)rot, "ub_CONQUER");
        cm_FlagObjectsT[cm_FlagObjectCount] = r;
        d3if_SelectTextureSet(r, (D3SURF *)r->info, 1, 1);
        sprintf(cm_FlagObjectsT[cm_FlagObjectCount]->name, "sp_CONQUER_%i", cm_FlagObjectCount);
        cm_FlagObjectsT[cm_FlagObjectCount]->owner = 0;
        cm_FlagObjectsT[cm_FlagObjectCount]->kind = 4;
        cm_FlagObjectsT[cm_FlagObjectCount]->mask = 1;
        cm_FlagObjectsT[cm_FlagObjectCount]->stateFlags = (cm_FlagObjectsT[cm_FlagObjectCount]->stateFlags & 0xf7) | 4;
        cm_FlagObjectCount++;
    }
    return 1;
}
/* 0x4d0ca0, walks the objects of the scene with the callback 0x4d0a80 (the flag dummies) */

void cm_PlaceFlagObjects()
{
    cm_FlagObjectCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x100, cm_CreateDummyFlag, 0);
}
/* 0x4d0cd0, the first free (-1) netId slot of the 16 of a CMGUARDREC, else -1 */

int cm_FreeNetIdSlot(int *a)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (a[i] == -1)
            return i;
    }
    return -1;
}

/* the record ai_ArmGateGuards fills: a local of the town gate event's run handler (he_RunAmbush passes its
   address); only these two fields are read here, the rest of its layout is not known */
typedef struct CMGUARDREC {
    char pad000[0x80];
    int netIds[16];                 /* 0x080, the netIds of the guards already armed, -1 = a free slot (cm_FreeNetIdSlot) */
    char pad0c0[0x50];
    int kind;                       /* 0x110, the kind of arming last asked for (1 .. 3) */
} CMGUARDREC;
#define ai_KindBCharsT (*(AICHAR *(*)[8])&ai_KindBChars)  /* 0x6f1c04, ai_KindBChars (f3init.h: char *[8]) as the AICHAR pointers it holds */

/* arms the next guard: takes the first free slot i of rec, the character ai_KindBChars[i] gives up its weapons and
   gets the item ITEM, loses its v024 health and its skill sk83 is set to SKILL (a field delta of the character).
   The original repeats this code inline (the four copies of 0x4d0cf0 reload ai_KindBChars[i] each time) */
#define CM_ARM_GUARD(ITEM, SKILL)                                                                             i = cm_FreeNetIdSlot(rec->netIds);                                                                           rec->netIds[i] = ai_KindBCharsT[i]->netId;                                                               cm_DiscardWeapons((char *)ai_KindBCharsT[i]);                                                            cm_SendExSellObject(ai_KindBCharsT[i]->netId, -1, ITEM, 1, ai_OwnPlayer, 0);                              gm_AddHealth(ai_KindBCharsT[i]->id, -ai_KindBCharsT[i]->v024);                                            skill = SKILL;                                                                                            cm_BeginFieldBlock((char *)ai_KindBCharsT[i], ai_KindBCharsT[i]->netId);                                  cm_AddFieldDelta(1, 1, (unsigned short)((char *)ai_KindBCharsT[i] - cm_FieldObject + 0x83), &skill);      cm_SendFieldDeltas()

/* 0x4d0cf0, arms the guards of the town gate event: kind 1 two guards with the item 0x156 (sk83 = 0xa8), kinds 2 / 3
   one with 0x158 and one with 0x160 (sk83 = 0xd2); a nonzero kind is kept in rec->kind (reasoned: the items replace
   the weapons cm_DiscardWeapons sends away, the characters are the kind 0xb ones of ai_KindBChars) */
void ai_ArmGateGuards(CMGUARDREC *rec, unsigned char kind)
{
    unsigned char skill;
    int i;

    if (kind != 0)
        rec->kind = kind;
    if (kind == 1) {
        CM_ARM_GUARD(0x156, 0xa8);
        CM_ARM_GUARD(0x156, 0xa8);
    } else if (kind == 2 || kind == 3) {
        CM_ARM_GUARD(0x158, 0xd2);
        CM_ARM_GUARD(0x160, 0xd2);
    }
}
/* 0x4d0f80, the object walk's callback: an object of kind 3 gets its mask (D3OBJECT.mask) cleared; returns 1 */

char cm_ClearKind3MaskCB(char *o)
{
    D3OBJECT *obj = (D3OBJECT *)o;

    if (obj->kind == 3)
        obj->mask = 0;
    return 1;
}

/* the voice banks of a fight side: KIND is the role's suffix (DIEBE thieves, RAEUBER robbers, SOELDNER mercenaries) */
#define CM_LOAD_VOICES(SIDE, KIND)                                                         \
    cm_##SIDE##ClickVoices = snd_LoadVoiceBank("ARBEITER_KOMMENTARE_KLICK_" KIND ".sbf", 0); \
    cm_##SIDE##AttackVoices = snd_LoadVoiceBank("K_C2C_ANGRIFF_" KIND ".sbf", 0);         \
    cm_##SIDE##OrderVoices = snd_LoadVoiceBank("K_C2C_BEFEHL_ERHALTEN_" KIND ".sbf", 0);  \
    cm_##SIDE##TauntVoices = snd_LoadVoiceBank("K_C2C_HOHN_" KIND ".sbf", 0);             \
    cm_##SIDE##PainVoices = snd_LoadVoiceBank("K_C2C_SCHMERZ_" KIND ".sbf", 0)

/* a fight record as cm_SetupFightScene and its caller 0x4d63e0 read it (the rest of its layout is not known here) */
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
} CMFIGHTREC;
#define cm_FightRecordT (*(CMFIGHTREC **)&cm_FightRecord)  /* 0x6f0d04, cm_FightRecord (cm.h: char *) as the fight record it points to */
#define cm_FightAttackerC (*(AICHAR **)&cm_FightAttacker)  /* 0x6f0cec, cm_FightAttacker (cm.h: int) as the character it points to */
#define cm_FightDefenderC (*(AICHAR **)&cm_FightDefender)  /* 0x6f0ce8, cm_FightDefender (cm.h: int) as the character it points to */

/* 0x4d0fa0, sets up the fight scene for the fight record rec: the scenario "Kampfszenario_<place>.ed3" by the flags or
   the type of the defender's building (cm_LoadEventScene; a robbers' camp gets a 15 cells wide border), the fight
   tables cleared, the weapon sounds (kampf\*.sbf), the voices of both sides by their roles (cm_FightRole), the boots
   object, the attackers placed at "dummy_OFFENSE_ENTRY" and the defenders (walking 1.2 times as fast) at
   "dummy_DEFENSE_ENTRY", the scene characters collected, the carts "ob_KARREN_<size>_KAMPF.ogr" put at their
   dummies "dummy_KARREN_0<n>" with up to three of their goods shown as "WARE_0<n>" (shuffled), the border cells of the
   height map set to the ground type 0xd and cm_DefenseFactor from the defender's building
   @deep Read in full against 0x4d0fa0..0x4d1a0b and rewritten: the scenario choice (AIBTYPE record 0x24d,
   kind / level), the fight tables (index loops: VC6 jl on the strength-reduced pointer), the sounds and
   the role voices (copy branch last), the boots, the fighters (cm_AddFighter, side leader at +0x16c, the
   defenders' walkSpeed *1.2), the scene chars, the nine WARE objects, the cart loop (nested ifs, goods
   query for-loop store/next/n<3 order, order walked by a char pointer as the original's inc edi), the
   border cells and cm_DefenseFactor; frame now 0x13c on both sides and 763 vs 765 instructions. Left: the
   cart loop's allocation (the original keeps i+1 in edi and spills it, n in a stack slot reloaded for k >
   n - 1, the dummy object in ebx; ours inc esi, n in a register: 'inc esi' vs 'lea edi,[esi+1]', 'cmp
   ebx,edi' vs 'test ebx,ebx', the cart pointer slot [esp+0x18] vs [esp+0x10]) and the slot layout of order
   / goods / pos (the prover stops at gm_ShuffledBytes(9, frame-0x10 vs frame-0x1c)). (references: M8 at
   +0x96b: ours refers to d3_DefaultScene (0x5fb5b0), the original's field to 0x633738 (d3_Terrain),
   explained: the M8 at +0x96b is an alignment artifact: both sides load d3_DefaultScene and
   d3_Terrain and push d3_Terrain then hmap for d3sm_Build (ours ecx=scene eax=terrain, the original
   edx=scene ecx=terrain); the aligner pairs our scene load with the original's terrain load by register) */
void cm_SetupFightScene(CMFIGHTREC *rec)
{
    char name[0x100];
    D3VECTOR pos;
    char order[16];
    AIOBJ *goods[3];
    int k, n, border, i;
    int *cart;
    AIBLD *b;
    AIOBJ *obj, *load, *w;
    D3OBJECT *o;
    AICHAR *r;
    D3SM *sc;
    AIBTYPE *t;
    char *p;

    border = 10;
    if (rec->flags & 1) {
        if (rec->flags & 8)
            cm_LoadEventScene("Kampfszenario_STADT.ed3");
        else
            cm_LoadEventScene("Kampfszenario_BERGPASS.ed3");
    } else if (rec->flags & 4) {
        cm_LoadEventScene("Kampfszenario_BERGPASS.ed3");
    } else {
        b = (AIBLD *)gm_BuildQuery(1, 1, rec->placeId);
        if (gm_IsHomeType((unsigned char *)b) != 0) {
            t = &ai_BTypesT[b->type];
            if (t->kind == 0x10) {
                cm_LoadEventScene("Kampfszenario_RAEUBERLAGER.ed3");
                border = 15;
            } else if (t->kind == 0xc)
                cm_LoadEventScene("Kampfszenario_STEINBRUCH.ed3");
            else if (t->kind == 0xb)
                cm_LoadEventScene("Kampfszenario_WALDSTUECK.ed3");
            else if (t->kind == 0xd)
                cm_LoadEventScene("Kampfszenario_MINE.ed3");
        } else if (ai_BTypesT[b->type].kind == 4) {
            if (ai_BTypesT[b->type].level == 1)
                cm_LoadEventScene("Kampfszenario_SCHMUGGLERLOCH.ed3");
            else if (ai_BTypesT[b->type].level == 2)
                cm_LoadEventScene("Kampfszenario_UNTERSCHLUPF.ed3");
            else if (ai_BTypesT[b->type].level == 3)
                cm_LoadEventScene("Kampfszenario_DIEBESGILDE.ed3");
        } else if (ai_BTypesT[b->type].kind == 0x13) {
            if (ai_BTypesT[b->type].level == 1)
                cm_LoadEventScene("Kampfszenario_STADTWACHE_KLEIN.ed3");
            else if (ai_BTypesT[b->type].level == 2)
                cm_LoadEventScene("Kampfszenario_STADTWACHE_MITTEL.ed3");
            else if (ai_BTypesT[b->type].level == 3)
                cm_LoadEventScene("Kampfszenario_STADTWACHE_GROSS.ed3");
        }
    }
    sc = (D3SM *)d3_DefaultScene->hmap;
    cm_ApplySeason();
    if (d3_Terrain != 0) {
        D3IF_SCENE(d3_CurrentScene).sky = D3IF_SCENE(0).sky;
        d3_Sky = D3IF_SCENE(0).sky;
        sv_UpdateWeather();
        d3sky_Init(d3_Sky, 1);
    }
    memset(cm_CharInfoWins, -1, 32 * sizeof(int));
    for (i = 0; i < 16; i++) {
        cm_DamagePops[i].h = -1;
        cm_DamagePops[i].v = 0;
        cm_DamagePops[i].c = 0;
    }
    memset(&cm_FightersT[32], 0, 0x1000);
    memset(cm_Bombs, 0, sizeof(cm_Bombs));
    memset(cm_Grenades, 0, 0x200);
    memset(cm_Fighters, 0, 32 * sizeof(AICHAR));
    for (i = 0; i < 32; i++)
        cm_FightersT[i].id = 0xffff;
    for (i = 0; i < 6; i++)
        cm_Portraits[i].win = -1;
    cm_FightSounds = sb_LoadSampleBank("kampf.sbf", 1);
    cm_CrossbowSounds = sb_LoadSampleBank("kampf\\armbrust.sbf", 1);
    cm_BombSounds = sb_LoadSampleBank("kampf\\bombeundtopf.sbf", 1);
    cm_DaggerSounds = sb_LoadSampleBank("kampf\\dolche.sbf", 1);
    cm_PistolSounds = sb_LoadSampleBank("kampf\\pistole.sbf", 1);
    cm_SwordSounds = sb_LoadSampleBank("kampf\\schwerter.sbf", 1);
    b = (AIBLD *)gm_BuildQuery(1, 1, rec->attackerBldId);
    if (b != 0)
        cm_AttackerRole = cm_FightRole((unsigned char *)b);
    if (cm_AttackerRole == 2) {
        CM_LOAD_VOICES(Attacker, "DIEBE");
    } else if (cm_AttackerRole == 1) {
        CM_LOAD_VOICES(Attacker, "RAEUBER");
    } else if (cm_AttackerRole == 0) {
        CM_LOAD_VOICES(Attacker, "SOELDNER");
    }
    b = (AIBLD *)gm_BuildQuery(1, 1, rec->placeId);
    if (b != 0)
        cm_DefenderRole = cm_FightRole((unsigned char *)b);
    if (cm_DefenderRole != cm_AttackerRole) {
        if (cm_DefenderRole == 2) {
            CM_LOAD_VOICES(Defender, "DIEBE");
        } else if (cm_DefenderRole == 1) {
            CM_LOAD_VOICES(Defender, "RAEUBER");
        } else if (cm_DefenderRole == 0) {
            CM_LOAD_VOICES(Defender, "SOELDNER");
        }
    } else {
        cm_DefenderClickVoices = cm_AttackerClickVoices;
        cm_DefenderAttackVoices = cm_AttackerAttackVoices;
        cm_DefenderOrderVoices = cm_AttackerOrderVoices;
        cm_DefenderTauntVoices = cm_AttackerTauntVoices;
        cm_DefenderPainVoices = cm_AttackerPainVoices;
    }
    cm_BootsObject = d3io_LoadObjectRef("ub_STIEFEL", "ub_STIEFEL");
    if (sc == 0) {
        d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create((struct D3TERRAIN *)d3_Terrain,
                                                             ((D3IFSCENE *)d3_DefaultScene)->mapSize, 2);
        sc = (D3SM *)d3_DefaultScene->hmap;
    }
    cm_FightCellScale = sc->cell.x * 1.1;
    sv_ScrollSpeed = 0.15f;
    sv_CameraEaseOn = 0;
    cm_FightRecord = (char *)rec;
    cm_PlaceFlagObjects();
    cm_FightAttacker = (int)gm_FindChar(rec->attackerId);
    cm_FightDefender = (int)gm_FindChar(rec->defenderId);
    if (ai_CharsT[ai_PlayerId].netId == cm_FightOwner && (rec->flags & 1)) {
        cm_BeginSendGroup();
        for (i = 0; i < 3; i++) {
            if (rec->carts[i] != -1)
                cm_SendExSellObject(-2, -1, 0x154, 1, ai_OwnPlayer, 0);
        }
        cm_EndSendGroup();
        if (cm_SendGroupState() == 0) {
            do
                sv_PumpNet();
            while (cm_SendGroupState() == 0);
        }
    }
    for (i = 0; i < 16; i++) {
        if (rec->attackers[i] != -1) {
            r = (AICHAR *)cm_AddFighter(rec->attackers[i], "dummy_OFFENSE_ENTRY");
            r->bld = (AIBLD *)cm_FightAttacker;     /* in a fight: the side's leader */
            if (r->scene != 0)
                d3if_SelectTextureSet(r->scene->obj, (struct D3SURF *)r->scene->obj->info, 5, 1);
        }
    }
    for (i = 0; i < 16; i++) {
        if (rec->defenders[i] != -1) {
            r = (AICHAR *)cm_AddFighter(rec->defenders[i], "dummy_DEFENSE_ENTRY");
            r->bld = (AIBLD *)cm_FightDefender;
            r->scene->walkSpeed *= 1.2;
            if (r->scene != 0)
                d3if_SelectTextureSet(r->scene->obj, (struct D3SURF *)r->scene->obj->info, 4, 1);
        }
    }
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, cm_ClearKind3MaskCB);
    memset(ai_SceneChars, 0, sizeof(ai_SceneChars));
    n = 0;
    for (i = 0; i < 32; i++) {
        if (cm_FightersT[i].id != 0xffff && cm_FightersT[i].scene != 0 &&
            cm_FightersT[i].scene->scene == d3_DefaultScene)
            ai_SceneCharsT[n++] = &cm_FightersT[i];
    }
    for (i = 0; i < 9; i++) {
        sprintf(name, "WARE_0%i", i + 1);
        o = d3_FindObject(0, 0x40, 0, name);
        if (o != 0)
            d3if_Show(o, 0);
    }
    cart = rec->carts;
    for (i = 0; i < 3; i++, cart++) {
        goods[0] = 0;
        goods[1] = 0;
        goods[2] = 0;
        gm_ShuffledBytes(9, (unsigned char *)order);
        sprintf(name, "dummy_KARREN_0%i", i + 1);
        o = d3_FindObject(0, 0x100, 0, name);
        if (o != 0) {
            if (*cart != -1) {
                obj = (AIOBJ *)gm_Query(0, 2, 7, 1, *cart);
                if (obj == 0) {
                    gm_FindObjectInst(*cart);
                    *cart = -1;
                } else {
                    load = (AIOBJ *)gm_Query((void *)obj->list, 1, 0, 0x1dd);
                    n = 0;
                    for (w = (AIOBJ *)gm_Query((void *)load->list, 1, 5); n < 3 && w != 0; w = (AIOBJ *)gm_QueryNext())
                        goods[n++] = w;
                    d3_PointToWorld(o, &o->pos, &pos);
                    if (obj->type == 0x134)
                        sprintf(name, "%s*ob_KARREN_KLEIN_KAMPF.ogr", ai_DataPath);
                    else if (obj->type == 0x135)
                        sprintf(name, "%s*ob_KARREN_MITTEL_KAMPF.ogr", ai_DataPath);
                    else if (obj->type == 0x136)
                        sprintf(name, "%s*ob_KARREN_GROSS_KAMPF.ogr", ai_DataPath);
                    o = d3_LoadGroup(name, 0, 0);
                    d3_SetObjectPos(o, &pos);
                    o->owner = obj;
                    p = order;
                    for (k = 0; k < 3; k++) {
                        sprintf(name, "WARE_0%i", *p++ + 1);       /* the shuffled goods places */
                        o = d3_FindObject(0, 0xc0, 0, name);
                        if (o != 0) {
                            if (k > n - 1)
                                d3if_Show(o, 0);
                            else {
                                o->owner = goods[k];
                                d3if_Show(o, 1);
                                o->mask = 1;
                            }
                        }
                    }
                }
            } else
                d3if_ShowTree(o, 0);
        }
    }
    d3sm_Build((struct D3SM *)d3_DefaultScene->hmap, (struct D3TERRAIN *)d3_Terrain);
    for (i = 0; i < border; i++) {
        for (k = i; k < sc->size; k++) {
            sc->entries[(sc->size - 1) * i + k].type = 0xd;
            sc->entries[(sc->size - i - 1) * sc->size + k].type = 0xd;
            sc->entries[sc->size * k + i].type = 0xd;
            sc->entries[sc->size * (k + 1) - i - 1].type = 0xd;
        }
    }
    if (b != 0)
        cm_DefenseFactor = (float)gm_BuildingSkill((unsigned char *)b, 6, 1) + 1.4f;
    else
        cm_DefenseFactor = 1.0f;
}
/* 0x4d1a10, ends the fight scene of the fight record rec (the counterpart of cm_SetupFightScene): fades to black, unloads
   the sound banks and voices (the defender's only when its role differs), deletes the flag objects cm_FlagObjects and
   the boots object; every fighter with a scene character carries the health lost in the fight back to its character
   (gm_AddHealth, not for the kind 0xb ones; only in the own fight or without a network connection) and loses the scene
   character; then the scroll speed, the sky, the scene and the caption are put back */

void cm_EndFightScene(CMFIGHTREC *rec)
{
    AICHAR *k;
    int i;

    cm_FadeToBlack();
    if (cm_PickMarker != 0) {
        d3if_Delete((struct D3OBJECT *)cm_PickMarker);
        cm_PickMarker = 0;
    }
    sb_UnloadSampleBank(cm_FightSounds);
    sb_UnloadSampleBank(cm_CrossbowSounds);
    sb_UnloadSampleBank(cm_BombSounds);
    sb_UnloadSampleBank(cm_DaggerSounds);
    sb_UnloadSampleBank(cm_PistolSounds);
    sb_UnloadSampleBank(cm_SwordSounds);
    sb_UnloadSampleBank(cm_AttackerClickVoices);
    sb_UnloadSampleBank(cm_AttackerAttackVoices);
    sb_UnloadSampleBank(cm_AttackerOrderVoices);
    sb_UnloadSampleBank(cm_AttackerTauntVoices);
    sb_UnloadSampleBank(cm_AttackerPainVoices);
    if (cm_AttackerRole != cm_DefenderRole) {
        sb_UnloadSampleBank(cm_DefenderClickVoices);
        sb_UnloadSampleBank(cm_DefenderAttackVoices);
        sb_UnloadSampleBank(cm_DefenderOrderVoices);
        sb_UnloadSampleBank(cm_DefenderTauntVoices);
        sb_UnloadSampleBank(cm_DefenderPainVoices);
    }
    for (i = 0; i < cm_FlagObjectCount; i++)
        d3if_Delete((struct D3OBJECT *)cm_FlagObjects[i]);
    if (cm_BootsObject != 0) {
        d3io_ReleaseObject(cm_BootsObject);
        cm_BootsObject = 0;
    }
    for (i = 0; i < 32; i++) {
        if (cm_FightersT[i].id != 0xffff && cm_FightersT[i].scene != 0) {
            if (rec->owner == ai_CharsT[ai_PlayerId].netId || net_Conn.s == INVALID_SOCKET) {
                k = (AICHAR *)gm_FindChar(cm_FightersT[i].netId);
                if (k->kind != 0xb)
                    gm_AddHealth(k->id, cm_FightersT[i].v024 - k->v024);
            }
            ch_Delete(cm_FightersT[i].scene);
            cm_FightersT[i].scene = 0;
        }
    }
    sv_ScrollSpeed = 1.0f;
    if (d3_Terrain != 0) {
        d3_Sky = 0;
        D3IF_SCENE(d3_CurrentScene).sky = 0;
    }
    cm_LeaveEventScene();
    snd_EndSpecialTrack();
    sv_CameraEaseOn = 1;
    d2_CopyCaption((char *)&ch_EmptyName);
}
/* 0x4d1c20, places the fighters ids[16] (-1 = none) of one side of the fight (side "OFF" / "DEF") on the ground of the
   scene: a fighter with a ranged weapon (the goods 0x174 0x176 0x15e 0x160) at the next dummy "dummy_<side>_FERN_%02i",
   the others at "dummy_<side>_NAHK_%02i", each on the free cell nearest the dummy's height map cell (ch_FindFreeCell),
   turned the way the dummy faces; stops when a dummy lies off the height map */
void cm_PlaceFighters(int *ids, char *side)
{
    int melee, ranged, i;
    int z, cz, cx, x;
    D3VECTOR rot;
    float h;
    D3VECTOR4 pos;                  /* d3_PointToWorld writes past the vector */
    D3VECTOR dir;
    char name[0x100];
    D3SM *sc;
    AICHAR *c;
    int *weapon;
    D3OBJECT *o;

    ranged = 1;
    melee = 1;
    sc = (D3SM *)d3_DefaultScene->hmap;
    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = (AICHAR *)cm_FindFighter(ids[i]);
        if (c == 0)
            continue;
        weapon = (int *)cm_CharWeapon((char *)c);
        if (weapon != 0 && (*weapon == 0x174 || *weapon == 0x176 || *weapon == 0x15e || *weapon == 0x160))
            sprintf(name, "dummy_%s_FERN_%02i", side, ranged++);
        else
            sprintf(name, "dummy_%s_NAHK_%02i", side, melee++);
        o = d3_FindObject(0, 0x100, 0, name);
        if (o == 0)
            continue;
        d3_PointToWorld(o, &o->pos, (D3VECTOR *)&pos);
        if (d3_GetHeightAt(sc, (D3VECTOR *)&pos, &x, &z, &h) == 0)
            return;
        ch_FindFreeCell(sc, &c->scene->obj->pos, &c->scene->obj->pos, x, z, &cx, &cz, 2, 0);
        d3sm_GetVertex(sc, cx, cz, (D3VECTOR *)&pos);
        ch_SetPos(c->scene, (D3VECTOR *)&pos, 1);
        rot.x = 0;
        rot.y = 0;
        rot.z = 0;
        d3_DirToWorld((struct XOBJ *)o, (float *)&d3_ZAxis, (float *)&dir);
        rot.y = d3_VecAngle((D3VECTOR *)&d3_ZAxis, &dir);
        d3_SetObjectRot(c->scene->obj, &rot);
    }
}
/* 0x4d1e00, the intro of the fight: a random fight track (when the music is on) and the cutscene "Kampf_Intro_Fluege" */

void cm_FightIntro()
{
    int r;
    EVTSCRIPT *s;

    d2_CutsceneActive = 1;
    if (snd_MusicEnabled != 0) {
        r = util_RandomU(100) & 0xffff;
        if (r < 0x14)
            snd_PlaySpecialTrack("cd2\\Krieg.mp3");
        else if (r < 0x28)
            snd_PlaySpecialTrack("cd1\\RuestetEuch.mp3");
        else if (r < 0x3c)
            snd_PlaySpecialTrack("cd1\\Flucht.mp3");
        else if (r < 0x50)
            snd_PlaySpecialTrack("cd2\\DerSeuchenzug.mp3");
        else
            snd_PlaySpecialTrack("cd2\\AmKuehlenGrabe.mp3");
    }
    sv_FrameMask = 0x7d6c6;
    sv_UpdateWeather();
    sv_UpdateSkyPhase(&ai_Time);
    s = evt_Load("cutscenes\\kampf\\Kampf_Intro_Fluege.esc");
    if (s != 0) {
        evt_RunScript(s);
        cm_WaitEventStop(s->id);
    }
    d2_CutsceneActive = 0;
}
/* 0x4d1eb0, the outro: stops the stream cm_FightAmbient and runs the cutscene "Kampf_outro_flug" */

void cm_FightOutro()
{
    EVTSCRIPT *s;

    if (cm_FightAmbient != 0)
        msx_StopAmbient(cm_FightAmbient);
    sv_FrameMask = 0x7d6c6;
    d2_CutsceneActive = 1;
    s = evt_Load("cutscenes\\kampf\\Kampf_outro_flug.esc");
    if (s != 0) {
        evt_RunScript(s);
        cm_WaitEventStop(s->id);
    }
    d2_CutsceneActive = 0;
}
/* 0x4d1f10, the enemy of the fighter c to go for: among the scene characters ai_SceneChars of another side
   (cm_OtherBuilding) still alive, the one whose distance to c weighted by its health ratio (gm_HealthRatio) is less
   than the plain distance of the one found before (from 1e6 on); 0 when none. The loops for a weapon of kind 2 and
   for the others are the same */

AICHAR *cm_NearestEnemy(AICHAR *c)
{
    float best;
    AICHAR *res;
    int i;
    float d;

    best = 1000000.0f;
    res = 0;
    if (((AIWEAPON *)cm_CharWeapon((char *)c))->kind != 2) {
        for (i = 0; i < 32; i++) {
            if (ai_SceneCharsT[i] != 0 && cm_OtherBuilding((char *)c, (char *)ai_SceneCharsT[i]) != 0 &&
                ai_SceneCharsT[i]->health != 0) {
                d = cm_CharObjDistXZ((char *)c, (char *)ai_SceneCharsT[i]->scene->obj);
                if (gm_HealthRatio((short *)ai_SceneCharsT[i]) * d < best) {
                    res = ai_SceneCharsT[i];
                    best = d;
                }
            }
        }
    } else {
        for (i = 0; i < 32; i++) {
            if (ai_SceneCharsT[i] != 0 && cm_OtherBuilding((char *)c, (char *)ai_SceneCharsT[i]) != 0 &&
                ai_SceneCharsT[i]->health != 0) {
                d = cm_CharObjDistXZ((char *)c, (char *)ai_SceneCharsT[i]->scene->obj);
                if (gm_HealthRatio((short *)ai_SceneCharsT[i]) * d < best) {
                    res = ai_SceneCharsT[i];
                    best = d;
                }
            }
        }
    }
    return res;
}
/* 0x4d2010, the threat at the height map vertex (a, b) for the fighter c: every living enemy within limit of it adds
   70 / its distance, every other living fighter of c's own side takes it off (the sides are the attackers / defenders
   of the fight record cm_FightRecord) */

float cm_ThreatAt(AICHAR *c, int a, int b, float limit)
{
    float threat;
    D3VECTOR4 v;
    D3SM *sc;
    int *own, *enemies;
    AICHAR *o;
    float d, dx, dy, dz;
    int i;

    sc = (D3SM *)d3_DefaultScene->hmap;
    threat = 0.0f;
    if ((int)c->bld == cm_FightAttacker) {      /* in a fight: the side's leader */
        own = cm_FightRecordT->attackers;
        enemies = cm_FightRecordT->defenders;
    } else {
        own = cm_FightRecordT->defenders;
        enemies = cm_FightRecordT->attackers;
    }
    d3sm_GetVertex(sc, a, b, (D3VECTOR *)&v);
    for (i = 0; i < 16; i++) {
        if (enemies[i] != -1) {
            o = (AICHAR *)cm_FindFighter(enemies[i]);
            if (o != 0 && o->health > 0) {
                dx = v.x - o->scene->obj->pos.x;
                dy = v.y - o->scene->obj->pos.y;
                dz = v.z - o->scene->obj->pos.z;
                d = sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    threat += 70.0f / d;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (own[i] != -1) {
            o = (AICHAR *)cm_FindFighter(own[i]);
            if (o != 0 && o->health > 0 && o != c) {
                dx = v.x - o->scene->obj->pos.x;
                dy = v.y - o->scene->obj->pos.y;
                dz = v.z - o->scene->obj->pos.z;
                d = sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    threat -= 70.0f / d;
            }
        }
    }
    return threat;
}
/* 0x4d2170, the safest cell around (cx, cy) for the fighter c: walks the diamond rings 0 .. r - 1 around it inside the
   height map, rates every cell whose ground type is not 0 / 0xd (the border) with cm_ThreatAt (limit r cells: r * the
   cell size * 2) and puts the least rated one into (*ox, *oy); returns 0 when no cell was rated */

int cm_BestCellNear(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    D3SM *sc;
    float best;
    float f;
    int k, row, col, hi, hic, m, type;

    sc = (D3SM *)d3_DefaultScene->hmap;
    best = 1000000000.0f;
    for (k = 0; k < r; k++) {
        hi = sc->size - 1;
        if (hi >= cy + k)
            hi = cy + k;
        for (row = cy - k < 0 ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = sc->size - 1;
            if (hic >= cx + m)
                hic = cx + m;
            for (col = cx - m < 0 ? 0 : cx - m; col <= hic; col++) {
                type = sc->entries[sc->size * row + col].type;
                if (type != 0 && type != 0xd) {
                    f = cm_ThreatAt((AICHAR *)c, col, row, r * sc->cell.x * 2);
                    if (f < best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    return best != 1000000000.0f;
}
/* 0x4d22d0, the most threatened cell around (cx, cy) for the fighter c: the rings of cm_BestCellNear, the cell with the
   greatest cm_ThreatAt rating goes into (*ox, *oy); returns 1 when that rating is above 5 (0 too when no cell rated
   above 0) */

int cm_MostThreatenedCellNear(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    D3SM *sc;
    float best;
    float f;
    int k, row, col, hi, hic, m, type;

    sc = (D3SM *)d3_DefaultScene->hmap;
    best = 0.0f;
    for (k = 0; k < r; k++) {
        hi = sc->size - 1;
        if (hi >= cy + k)
            hi = cy + k;
        for (row = cy - k < 0 ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = sc->size - 1;
            if (hic >= cx + m)
                hic = cx + m;
            for (col = cx - m < 0 ? 0 : cx - m; col <= hic; col++) {
                type = sc->entries[sc->size * row + col].type;
                if (type != 0 && type != 0xd) {
                    f = cm_ThreatAt((AICHAR *)c, col, row, r * sc->cell.x * 2);
                    if (f > best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    if (best != 0.0f && best > 5.0f)
        return 1;
    return 0;
}
/* 0x4d2450, a callback of the object walk: collects the objects named "sp_ESCAPE" into list[cm_PointCount++] */

char cm_CollectEscapeObject(char *o, int *list)
{
    if (_stricmp(o, "sp_ESCAPE") == 0) {
        list[cm_PointCount] = (int)o;
        cm_PointCount++;
    }
    return 1;
}
/* 0x4d2490, the closest "sp_ESCAPE" object (0x4d2450) to the character c, or 0 (limit 1e6) */

char *cm_NearestEscapeObject(char *c)
{
    int list[0x20];
    D3VECTOR4 v;
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    D3OBJECT *m, *o;
    int i, n;

    cm_PointCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0xc0, cm_CollectEscapeObject, list);
    m = ((AICHAR *)c)->scene->obj;
    d3_PointToWorld(m, &m->pos, (D3VECTOR *)&v);
    n = cm_PointCount;
    for (i = 0; i < n; i++) {
        o = (D3OBJECT *)list[i];
        dx = v.x - o->pos.x;
        dy = v.y - o->pos.y;
        dz = v.z - o->pos.z;
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = (char *)o;
        }
    }
    return res;
}
/* 0x4d2550, a callback: collects the objects "WARE_" whose owner (+0x200, a record with the side in +0xa) is not the side
   of the character c into cm_PointList[cm_PointCount++] */

char cm_CollectWareObject(char *o, char *c)
{
    AICHAR *side = (AICHAR *)((AICHAR *)c)->bld;    /* in a fight bld is the side's leader (cm_AddFighter) */

    if (strncmp(o, "WARE_", 5) == 0 && ((AIOBJ *)((D3OBJECT *)o)->owner)->fightSide != side->id)
        cm_PointList[cm_PointCount++] = o;
    return 1;
}
/* 0x4d25a0, the closest object of 0x4d2550 to the character c, or 0 */

char *cm_NearestWareObject(char *c)
{
    D3VECTOR4 v;
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    D3OBJECT *m, *o;
    int i, n;

    cm_PointCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, cm_CollectWareObject, c);
    m = ((AICHAR *)c)->scene->obj;
    d3_PointToWorld(m, &m->pos, (D3VECTOR *)&v);
    n = cm_PointCount;
    for (i = 0; i < n; i++) {
        o = (D3OBJECT *)cm_PointList[i];
        dx = v.x - o->pos.x;
        dy = v.y - o->pos.y;
        dz = v.z - o->pos.z;
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = (char *)o;
        }
    }
    return res;
}
/* 0x4d2650, a callback: collects the objects "sp_CONQUER*" into cm_PointList[cm_PointCount++] */

char cm_CollectConquerPoint(char *o)
{
    if (strncmp(o, "sp_CONQUER", 10) == 0)
        cm_PointList[cm_PointCount++] = o;
    return 1;
}
/* 0x4d2680, the conquer point (an "sp_CONQUER" object, cm_CollectConquerPoint) to go for: the least distance to the
   fighter c's body, weighted by the point's owner (10 for an owner of another side (cm_OtherBuilding), 15 for an
   owner of c's own side other than c, 25 for c itself); 0 when the point chosen is c's own */

D3OBJECT *cm_NearestConquerPoint(AICHAR *c)
{
    D3VECTOR4 v;
    float best;
    D3OBJECT *res;
    D3OBJECT *body, *o;
    void *owner;
    float d, dx, dy, dz;
    int i;

    best = 1000000.0f;
    res = 0;
    cm_PointCount = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, cm_CollectConquerPoint, c);
    body = c->model->body;
    d3_PointToWorld(body, &body->pos, (D3VECTOR *)&v);
    for (i = 0; i < cm_PointCount; i++) {
        o = (D3OBJECT *)cm_PointList[i];
        dx = v.x - o->pos.x;
        dy = v.y - o->pos.y;
        dz = v.z - o->pos.z;
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (o->owner != 0 && cm_OtherBuilding((char *)c, o->owner) != 0)
            d = (float)(d * 10.0);
        else if (o->owner != 0 && cm_OtherBuilding((char *)c, o->owner) == 0 && o->owner != c)
            d = (float)(d * 15.0);
        else if (o->owner != 0)
            d = (float)(d * 25.0);
        if (d < best) {
            best = d;
            res = o;
        }
    }
    if (res->owner == c)
        res = 0;
    return res;
}
/* 0x4d27a0, builds the fight AI's order chances cm_OrderChances[6 kinds][5 strength levels][6 orders]: the
   percentages stored one by one, then each row made cumulative (v[k] = v[k] * 0.01 + v[k - 1]), and in each row
   a value equal to an earlier one cleared (an order that adds no chance is never drawn)
   @deep Read all 187 instructions against the original: the 120 stores are equal, the two loops do the
   same work (cumulative row times 0.01; clear a later value equal to an earlier one in each 6-row). Left:
   (1) loop 1, the original keeps t in a stack slot (push ecx frame; fst [esp+0x10] / fld [esp+0x10] around
   the k != 0 branch) where ours keeps it in st(0) (fst [eax]; jmp; fstp st(0)); (2) loop 2, the original
   recomputes (b + i) * 6 (lea eax,[ebx+ebp]; lea eax,[eax+eax*2]; add eax,eax) in every j round and keeps
   b (ebp += 5, cmp 0x1e) and i (ebx, cmp 5) apart, ours strength-reduces them to one row offset (edi += 6,
   a countdown, cmp 0xb4). Tried: a flat index n, a pointer walk with a signed compare (now, as the
   original's jl), a [30][6] view, a precomputed e = (b + i) * 6 + j: none changes VC6's induction choice.
   Induction / allocation only. */

void cm_InitOrderChances()
{
    int row, k, b, i, j;
    float t;
    float *p;

    memset(cm_OrderChances, 0, 0x2d0);
    cm_OrderChances[0x0] = 10.0f;
    cm_OrderChances[0x1] = 10.0f;
    cm_OrderChances[0x4] = 30.0f;
    cm_OrderChances[0x5] = 50.0f;
    cm_OrderChances[0x6] = 40.0f;
    cm_OrderChances[0x7] = 20.0f;
    cm_OrderChances[0xa] = 20.0f;
    cm_OrderChances[0xb] = 20.0f;
    cm_OrderChances[0xc] = 80.0f;
    cm_OrderChances[0xd] = 20.0f;
    cm_OrderChances[0x10] = 0;
    cm_OrderChances[0x11] = 0;
    cm_OrderChances[0x12] = 70.0f;
    cm_OrderChances[0x13] = 30.0f;
    cm_OrderChances[0x16] = 0;
    cm_OrderChances[0x17] = 0;
    cm_OrderChances[0x18] = 50.0f;
    cm_OrderChances[0x19] = 50.0f;
    cm_OrderChances[0x1c] = 0;
    cm_OrderChances[0x1d] = 0;
    cm_OrderChances[0x1e] = 10.0f;
    cm_OrderChances[0x1f] = 10.0f;
    cm_OrderChances[0x22] = 30.0f;
    cm_OrderChances[0x23] = 50.0f;
    cm_OrderChances[0x24] = 80.0f;
    cm_OrderChances[0x25] = 20.0f;
    cm_OrderChances[0x28] = 0;
    cm_OrderChances[0x29] = 0;
    cm_OrderChances[0x2a] = 80.0f;
    cm_OrderChances[0x2b] = 20.0f;
    cm_OrderChances[0x2e] = 0;
    cm_OrderChances[0x2f] = 0;
    cm_OrderChances[0x30] = 60.0f;
    cm_OrderChances[0x31] = 40.0f;
    cm_OrderChances[0x34] = 0;
    cm_OrderChances[0x35] = 0;
    cm_OrderChances[0x36] = 50.0f;
    cm_OrderChances[0x37] = 50.0f;
    cm_OrderChances[0x3a] = 0;
    cm_OrderChances[0x3b] = 0;
    cm_OrderChances[0x3c] = 5.0f;
    cm_OrderChances[0x3e] = 5.0f;
    cm_OrderChances[0x40] = 20.0f;
    cm_OrderChances[0x41] = 70.0f;
    cm_OrderChances[0x42] = 50.0f;
    cm_OrderChances[0x44] = 10.0f;
    cm_OrderChances[0x46] = 20.0f;
    cm_OrderChances[0x47] = 20.0f;
    cm_OrderChances[0x48] = 80.0f;
    cm_OrderChances[0x4a] = 20.0f;
    cm_OrderChances[0x4c] = 0;
    cm_OrderChances[0x4d] = 0;
    cm_OrderChances[0x4e] = 70.0f;
    cm_OrderChances[0x50] = 30.0f;
    cm_OrderChances[0x52] = 0;
    cm_OrderChances[0x53] = 0;
    cm_OrderChances[0x54] = 50.0f;
    cm_OrderChances[0x56] = 50.0f;
    cm_OrderChances[0x58] = 0;
    cm_OrderChances[0x59] = 0;
    cm_OrderChances[0x5a] = 20.0f;
    cm_OrderChances[0x5c] = 80.0f;
    cm_OrderChances[0x5e] = 0;
    cm_OrderChances[0x5f] = 0;
    cm_OrderChances[0x60] = 30.0f;
    cm_OrderChances[0x62] = 70.0f;
    cm_OrderChances[0x64] = 0;
    cm_OrderChances[0x65] = 0;
    cm_OrderChances[0x66] = 40.0f;
    cm_OrderChances[0x68] = 60.0f;
    cm_OrderChances[0x6a] = 0;
    cm_OrderChances[0x6b] = 0;
    cm_OrderChances[0x6c] = 60.0f;
    cm_OrderChances[0x70] = 0;
    cm_OrderChances[0x71] = 0;
    cm_OrderChances[0x76] = 0;
    cm_OrderChances[0x77] = 0;
    cm_OrderChances[0x7a] = 0;
    cm_OrderChances[0x80] = 0;
    cm_OrderChances[0x86] = 0;
    cm_OrderChances[0x88] = 0;
    cm_OrderChances[0x89] = 0;
    cm_OrderChances[0x8c] = 0;
    cm_OrderChances[0x8e] = 0;
    cm_OrderChances[0x8f] = 0;
    cm_OrderChances[0x92] = 0;
    cm_OrderChances[0x94] = 0;
    cm_OrderChances[0x95] = 0;
    cm_OrderChances[0x98] = 0;
    cm_OrderChances[0x9e] = 0;
    cm_OrderChances[0xa4] = 0;
    cm_OrderChances[0xa6] = 0;
    cm_OrderChances[0xa7] = 0;
    cm_OrderChances[0xaa] = 0;
    cm_OrderChances[0xac] = 0;
    cm_OrderChances[0xad] = 0;
    cm_OrderChances[0xb0] = 0;
    cm_OrderChances[0xb2] = 0;
    cm_OrderChances[0xb3] = 0;
    cm_OrderChances[0x6e] = 40.0f;
    cm_OrderChances[0x72] = 70.0f;
    cm_OrderChances[0x74] = 30.0f;
    cm_OrderChances[0x78] = 20.0f;
    cm_OrderChances[0x7c] = 10.0f;
    cm_OrderChances[0x7d] = 70.0f;
    cm_OrderChances[0x7e] = 30.0f;
    cm_OrderChances[0x82] = 20.0f;
    cm_OrderChances[0x83] = 50.0f;
    cm_OrderChances[0x84] = 100.0f;
    cm_OrderChances[0x8a] = 100.0f;
    cm_OrderChances[0x90] = 100.0f;
    cm_OrderChances[0x96] = 20.0f;
    cm_OrderChances[0x9a] = 10.0f;
    cm_OrderChances[0x9b] = 70.0f;
    cm_OrderChances[0x9c] = 30.0f;
    cm_OrderChances[0xa0] = 20.0f;
    cm_OrderChances[0xa1] = 50.0f;
    cm_OrderChances[0xa2] = 100.0f;
    cm_OrderChances[0xa8] = 100.0f;
    cm_OrderChances[0xae] = 100.0f;
    for (p = cm_OrderChances; (int)p < (int)&cm_OrderChances[6 * 5 * 6];) {
        for (row = 0; row < 5; row++) {
            for (k = 0; k < 6; k++, p++) {
                t = (float)(*p * 0.01);
                *p = t;
                if (k != 0)
                    *p = t + p[-1];
            }
        }
    }
    for (b = 0; b < 6 * 5; b += 5) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 6; j++) {
                for (k = j + 1; k < 6; k++) {
                    if (cm_OrderChances[(b + i) * 6 + k] == cm_OrderChances[(b + i) * 6 + j])
                        cm_OrderChances[(b + i) * 6 + k] = 0;
                }
            }
        }
    }
}
/* 0x4d2c00, the row of cm_OrderChances the fight AI draws the orders of side from: the kind by the fight flags (bit 0 /
   1 / 2, then side being the attacker cm_FightAttacker or the defender cm_FightDefender; 6 for neither), the level
   0 .. 4 by the ratio of the side's own strength to the enemy's (the sums of cm_FightStrength over the fighters still
   standing, health neither 0 nor 4) against 1.75 / 1.25 / 0.75 / 0.5; the defender holding against the attacker's
   "sp_CONQUER" flags gets level 1 for one taken, 0 for two or three */

float *cm_OrderChancesFor(int side)
{
    int kind;
    float def, att, r;
    D3VECTOR4 v;
    char *o;
    int i, cnt, lvl;

    kind = 6;
    if ((cm_FightRecordT->flags & 1) && cm_FightAttacker == side)
        kind = 0;
    else if ((cm_FightRecordT->flags & 1) && cm_FightDefender == side)
        kind = 1;
    else if ((cm_FightRecordT->flags & 2) && cm_FightAttacker == side)
        kind = 2;
    else if ((cm_FightRecordT->flags & 2) && cm_FightDefender == side)
        kind = 3;
    else if ((cm_FightRecordT->flags & 4) && cm_FightAttacker == side)
        kind = 4;
    else if ((cm_FightRecordT->flags & 4) && cm_FightDefender == side)
        kind = 5;
    def = 0.0f;
    for (i = 0; i < 16; i++) {
        if (cm_FightRecordT->defenders[i] != -1) {
            o = cm_FindFighter(cm_FightRecordT->defenders[i]);
            if (((AICHAR *)o)->health != 0 && ((AICHAR *)o)->health != 4)
                def = cm_FightStrength(o) + def;
        }
    }
    att = 0.0f;
    for (i = 0; i < 16; i++) {
        if (cm_FightRecordT->attackers[i] != -1) {
            o = cm_FindFighter(cm_FightRecordT->attackers[i]);
            if (((AICHAR *)o)->health != 0 && ((AICHAR *)o)->health != 4)
                att = cm_FightStrength(o) + att;
        }
    }
    cnt = 0;
    for (i = 0; i < cm_FlagObjectCount; i++) {
        if (strstr(cm_FlagObjects[i], "sp_CONQUER") != 0) {
            d3_PointToWorld((D3OBJECT *)cm_FlagObjects[i], &((D3OBJECT *)cm_FlagObjects[i])->pos, (D3VECTOR *)&v);
            o = ((D3OBJECT *)cm_FlagObjects[i])->owner;
            if (o != 0 && (int)((AICHAR *)o)->bld == cm_FightAttacker)
                cnt++;
        }
    }
    if (side == cm_FightAttacker)
        r = att / def;
    else
        r = def / att;
    if (r >= 1.75f)
        lvl = 4;
    else if (r >= 1.25f)
        lvl = 3;
    else if (r >= 0.75f)
        lvl = 2;
    else if (r >= 0.5f)
        lvl = 1;
    else
        lvl = 0;
    if (side == cm_FightDefender) {
        if (cnt == 1)
            lvl = cnt;
        else if (cnt == 2 || cnt == 3)
            lvl = 0;
    }
    return &cm_OrderChances[(kind * 5 + lvl) * 6];
}
/* 0x4d2e30, how well the fighter c suits the order 0 .. 5 (the fight AI gives each order drawn to the fighter rated
   highest): 0 (attack) its strength (cm_FightStrength) times 1, 0.5 when it has an order already, 0.2 with a used-up
   weapon; 1 the inverse (1 / strength times 0.4, 0.5 with another order, 1 while on order 1 or with a used-up weapon);
   2 1 on order 2, else 0.5; 3 1; 4 / 5 0.2 on order 2, else 1 / its health ratio (gm_HealthRatio); 0 for others */

float cm_OrderRating(AICHAR *c, int order)
{
    float r;
    float f;
    int kind;
    unsigned char cur;

    r = 0.0f;
    switch (order) {
    case 0:
        f = 1.0f;
        if (c->formation != 0)
            f = 0.5f;
        kind = ((AIWEAPON *)cm_CharWeapon((char *)c))->kind;
        if ((kind == 1 || kind == 2) && ((AIOBJ *)cm_CharWeaponItem((char *)c))->usesLeft <= 0)
            f = 0.2f;
        r = cm_FightStrength((char *)c) * f;
        break;
    case 1:
        cur = c->formation;
        if (cur == 1)
            f = 1.0f;
        else {
            f = 0.4f;
            if (cur != 0)
                f = 0.5f;
        }
        kind = ((AIWEAPON *)cm_CharWeapon((char *)c))->kind;
        if ((kind == 1 || kind == 2) && ((AIOBJ *)cm_CharWeaponItem((char *)c))->usesLeft <= 0)
            f = 1.0f;
        r = f / cm_FightStrength((char *)c);
        break;
    case 2:
        if (c->formation != 2)
            r = 0.5f;
        else
            r = 1.0f;
        break;
    case 3:
        r = 1.0f;
        break;
    case 4:
    case 5:
        if (c->formation == 2)
            r = 0.2f;
        else
            r = 1.0f / gm_HealthRatio((short *)c);
        break;
    }
    return r;
}
/* 0x4d2f80, gives the fighters of the leading character side (cm_FightAttacker: the attackers, else the defenders of
   the fight record) their orders: once per fighter an order 0 .. 5 is drawn (util_RandomF against
   cm_OrderChancesFor) and given (AICHAR.formation) to the fighter still without a new order rated best for it
   (cm_OrderRating), a fighter holding an order already only on a second roll above 0.25; then each fighter carries
   out its order (cm_CarryOutOrder, with the leader's cutscene record cm_CutInfos) */

void cm_GiveFightOrders(int side)
{
    AICHAR *c[16];
    int *ids;
    float *row;
    char *cut;
    AICHAR *sel;
    float r, best, w;
    int i, j, order, n, left, selidx;

    n = 0;
    selidx = 0;
    order = 0;
    cut = 0;
    row = cm_OrderChancesFor(side);
    for (i = 0; i < cm_FightRecordT->nChars; i++) {
        if (cm_FightRecordT->charIds[i] == ((AICHAR *)side)->netId) {
            cut = cm_CutInfos[i];
            break;
        }
    }
    if (side == cm_FightAttacker)
        ids = cm_FightRecordT->attackers;
    else
        ids = cm_FightRecordT->defenders;
    for (i = 0; i < 16; i++) {
        if (ids[i] != -1) {
            c[i] = (AICHAR *)cm_FindFighter(ids[i]);
            n++;
        } else
            c[i] = 0;
    }
    for (left = n; left > 0; left--) {
        r = util_RandomF();
        for (j = 0; j < 6; j++) {
            if (r <= row[j]) {
                order = j;
                break;
            }
        }
        best = -1.0f;
        for (i = 0; i < 16; i++) {
            if (c[i] != 0) {
                w = cm_OrderRating(c[i], order);
                if (w > best) {
                    best = w;
                    selidx = i;
                }
            }
        }
        sel = c[selidx];
        if (sel->formation == 0 || cm_RandomFloat() > 0.25) {
            sel->formation = (unsigned char)order;
            c[selidx] = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        if (ids[i] != -1)
            cm_CarryOutOrder(cut, cm_FindFighter(ids[i]));
    }
}
/* 0x4d3120, the formation order for the character c of the fight record rec by its formation type (AICHAR.formation):
   0 the best free cell near the character for a kind 2 weapon (cm_MostThreatenedCellNear), else the closest selected
   enemy (cm_NearestEnemy), then cm_OrderAttack; 1 the closest "WARE_" object (cm_NearestWareObject) -> cm_OrderWare3;
   2 the closest "sp_CONQUER" point (cm_NearestConquerPoint) -> cm_OrderConquer; 3 cm_OrderWait; 4 cm_OrderToBestCell;
   5 the closest "sp_ESCAPE" object (cm_NearestEscapeObject) -> cm_OrderEscape */

void cm_CarryOutOrder(char *rec, char *c)
{
    D3SM *sc;
    AIWEAPON *k;
    char *o;
    int x, y, h;
    float v[4];
    int r;

    if (c != 0 && ((AICHAR *)c)->scene != 0) {
        if (((AICHAR *)c)->health == 0)
            return;
        if (((AICHAR *)c)->scene->obj->type == 1)
            return;
    }
    switch (((AICHAR *)c)->formation) {
    case 0:
        k = (AIWEAPON *)cm_CharWeapon(c);
        if ((k->kind == 1 || k->kind == 2) && ((AIOBJ *)cm_CharWeaponItem(c))->usesLeft <= 0)
            return;
        if (k->kind == 2) {
            sc = (D3SM *)d3_DefaultScene->hmap;
            if ((char)d3_GetHeightAt(sc, &((AICHAR *)c)->scene->obj->pos, &x, &y, &h) == 0)
                return;
            r = (int)(500.0f / sc->cell.x);
            if (cm_MostThreatenedCellNear(c, x, y, r, &x, &y) == 0)
                return;
            cm_OrderAttack(rec, c, 0, x, y);
            return;
        }
        o = (char *)cm_NearestEnemy((AICHAR *)c);
        if (o == 0)
            return;
        cm_OrderAttack(rec, c, o, 0, 0);
        return;
    case 5:
        o = cm_NearestEscapeObject(c);
        if (o == 0)
            return;
        cm_OrderEscape(rec, c, o);
        return;
    case 1:
        o = cm_NearestWareObject(c);
        if (o == 0)
            return;
        cm_OrderWare3(rec, c, o);
        return;
    case 2:
        o = (char *)cm_NearestConquerPoint((AICHAR *)c);
        if (o == 0)
            return;
        cm_OrderConquer(rec, c, o);
        return;
    case 4:
        cm_OrderToBestCell(rec, c);
        return;
    case 3:
        cm_OrderWait(rec, c);
        return;
    }
}
/* 0x4d3300, a character falls: the sound "Koerperfaellt" and a blood puddle "ub_Blutlache" (random turn) at its place */

void cm_FallDownAction(char *c)
{
    D3VECTOR rot;
    D3OBJECT *b;

    rot.x = 0;
    rot.y = 0;
    rot.z = 0;
    rot.y = (float)(cm_Random(0x168) * cm_Pi * cm_Inv180);
    sb_PlaySampleI(0, "Koerperfaellt", 1, 0x3f, 0x7f, 0, 0);
    b = d3_CreateObject(0, &((CHACTION *)c)->ch->obj->pos, &rot, "ub_Blutlache");
    d3t_PlayAnim(b, "sonstiges\\ub_Blutlache.baf", 0x12);
}
/* 0x4d3390, the death of the fighter c once its health ratio (gm_HealthRatio) is below 0.05: blood spray at the body
   (d3t_CreateBlood), its actions stopped and the fall queued (cm_FallDownAction with the sound "kampf/todumfallen"),
   health 0, the body's kind cleared, the cry "Schmerzschrei", v024 set below zero (-life - 20 with a chance of a half,
   text 0x1181, else 10 - life, text 0x1182) and the text shown; a ware it carried (v1ac) is put at the body, shown and
   handed to the defender. Returns 1, 0 while it lives */

int cm_CheckDeath(AICHAR *c)
{
    char buf[0x100];
    CHACTION *a;
    D3OBJECT *ware;

    cm_CharWeapon((char *)c);
    if (gm_HealthRatio((short *)c) < 0.05) {
        d3t_CreateBlood(c->scene->obj, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
        ch_StopActions(c->scene);
        a = ch_AddActionCB(c->scene, (CHACTFUNC)cm_FallDownAction, 0x14, 0x36, 1);
        strcpy(a->u.name, "kampf/todumfallen");
        c->health = 0;
        c->scene->obj->kind = 0;
        sb_PlaySample(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
        if (cm_Random(100) > 50) {
            c->v024 = (int)(-(int)c->f01c - 20.0);
            txt_Format(buf, 0x1181, c->id);
        } else {
            c->v024 = (int)(10.0 - (int)c->f01c);
            txt_Format(buf, 0x1182, c->id);
        }
        ware = (D3OBJECT *)c->v1ac;
        if (ware != 0) {
            d3_SetWorldPosRot(ware, (float *)&c->scene->obj->pos, &c->scene->obj->matrix);
            d3if_Show(ware, 1);
            c->v1ac = 0;
            ((AIOBJ *)cm_ObjectOwner((char *)ware))->fightSide = ((AICHAR *)cm_FightDefender)->id;
        }
        sv_SetStatusText(buf);
        return 1;
    }
    return 0;
}

#define G19_FIGHTER_SYNC(ch)                                                          \
    r = (AIOBJ *)cm_CharWeaponItem((char *)(ch));                                          \
    if (r != 0) {                                                                     \
        vv = -1;                                                                      \
        cm_BeginFieldBlock((char *)r, r->objId);                                            \
        cm_AddFieldValues(4, 1, (unsigned short)((char *)&r->usesLeft - cm_FieldObject), &vv); \
        cm_SendFieldDeltas();                                                               \
    }

/* 0x4d3550, the hit action of the fight (queued on the hitter, a's hit record): a hit counts only while the victim's
   body is within the range of the hitter's weapon; a used-up weapon item of the player's fight is synced (its uses
   field -1); a damage shows (cm_ShowDamage) with the pain voice "_K_C2C_SCHMERZ" of the victim's side (3 in 10) or
   the cry "Schmerzschrei" (3 in 10); while both stand, the sound of the hitter's weapon ("Treffer" for a hit, else
   "Daneben_s" of the sword / dagger bank, "schuss_s" for the pistol), then, unless the victim dies (cm_CheckDeath),
   blood and a random flinch "kampf/arm_greifen" / "kampf/bauch_greifen" for a hit (the weapon 0x16e stuns: health 4,
   text 0x1188), "kampf/ausweichen_seite" (pistol, 0x160) or "kampf/stich_abwehr" for a miss, when the victim is idle */

void cm_HitAction(CHACTION *a)
{
    int damage, ok;
    AIWEAPON *hw, *vw;
    int v;
    char anims[2][0x20] = {"kampf/arm_greifen", "kampf/bauch_greifen"};
    char buf[0x100];
    AICHAR *victim, *hitter;
    AIOBJ *item;
    int rnd;

    hitter = a->hit.hitter;
    ok = 1;
    victim = a->hit.victim;
    damage = a->hit.damage;
    vw = (AIWEAPON *)cm_CharWeapon((char *)victim);
    hw = (AIWEAPON *)cm_CharWeapon((char *)hitter);
    ch_GetHMap(victim->scene);
    if (hw == 0 || cm_CharObjDistXZ((char *)hitter, (char *)victim->scene->obj) > hw->range) {
        damage = 0;
        ok = 0;
    }
    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && (hw->kind == 1 || hw->kind == 2)) {
        item = (AIOBJ *)cm_CharWeaponItem((char *)hitter);
        if (item != 0) {
            v = -1;
            cm_BeginFieldBlock((char *)item, item->objId);
            cm_AddFieldValues(4, 1, (unsigned short)((char *)item - cm_FieldObject + 0x20), &v);
            cm_SendFieldDeltas();
        }
    }
    if (damage != 0) {
        rnd = cm_Random(10);
        if ((int)victim->bld == cm_FightAttacker)
            cm_ShowDamage((char *)victim, damage, 1);
        else
            cm_ShowDamage((char *)victim, damage, 2);
        if (rnd > 8) {
            if (a->hit.sideId != ((AICHAR *)cm_FightAttacker)->netId)
                snd_QueueVoice(victim->id, cm_AttackerPainVoices, "_K_C2C_SCHMERZ", -1, 500);
            else
                snd_QueueVoice(victim->id, cm_DefenderPainVoices, "_K_C2C_SCHMERZ", -1, 500);
        } else if (rnd > 5)
            sb_PlaySample(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
    }
    if (victim->health != 0 && hitter->health != 0 && ok != 0) {
        if (hw->id == 0x156 || hw->id == 0x158) {
            if (damage != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            else
                sb_PlaySampleI(cm_SwordSounds, "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
        } else if (hw->id == 0x154 || hw->id == 0x16e || hw->id == 0x172) {
            if (damage != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            else
                sb_PlaySampleI(cm_DaggerSounds, "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
        } else if (hw->id == 0x15e)
            sb_PlaySampleI(cm_PistolSounds, "schuss_s", 1, 0x3f, 0x7f, 0, 0);
        else if (hw->id == 0x160) {
            if (damage != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
        } else if (hw->id == 0)
            sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
        if (cm_CheckDeath(victim) == 0) {
            if (damage != 0) {
                d3t_CreateBlood(victim->scene->obj, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
                if (victim->scene->actions == 0 || victim->scene->actions->type == 0x3b) {
                    ch_StopActions(victim->scene);
                    ch_QueueAnimSpeed(victim->scene, anims[cm_Random(2)], 1, 1.4f);
                }
                if (hw->id == 0x16e) {
                    victim->health = 4;
                    txt_Format(buf, 0x1188, vw->text054, victim->id);
                    sv_SetStatusText(buf);
                }
            } else if (victim->scene->actions == 0 || victim->scene->actions->type == 0x3b) {
                ch_StopActions(victim->scene);
                if (hw->id == 0x15e || hw->id == 0x160)
                    ch_QueueAnimSpeed(victim->scene, "kampf/ausweichen_seite", 1, 1.4f);
                else
                    ch_QueueAnimSpeed(victim->scene, "kampf/stich_abwehr", 1, 1.4f);
            }
        }
    }
}
/* 0x4d3980, an explosion event: the fuse sound "ZuendschnurBrennt_l_01", the attacker's worker count synced, 0x4ce290 */

void cm_LayBombAction(char *ev)
{
    AICHAR *att = ((CHACTION *)ev)->pot.thrower;
    AIWEAPON *k;
    AIOBJ *r;
    int vv;

    k = (AIWEAPON *)cm_CharWeapon((char *)att);
    cm_LayBomb(&att->scene->obj->pos);
    sb_PlaySampleI(cm_BombSounds, "ZuendschnurBrennt_l_01", 3, 0x3f, 0x7f, 0, 0);
    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && (k->kind == 1 || k->kind == 2)) {
        G19_FIGHTER_SYNC(att);
    }
}
/* 0x4d3a30, a bomb event: for the player's side the thrower's used weapon item is synced (its uses field -1),
   then a grenade (cm_ThrowGrenade, 0x4ce5d0) is thrown from the thrower's body to the cell aimed at */

void cm_ThrowBombAction(char *ev)
{
    CHACTION *b = (CHACTION *)ev;
    AICHAR *att = b->bomb.thrower;
    AIOBJ *r;
    int vv;

    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner) {
        G19_FIGHTER_SYNC(att);
    }
    cm_ThrowGrenade(&att->scene->obj->pos, b->bomb.x, b->bomb.y);
}
/* 0x4d3ac0, a fighter's cheer (fight command 1, cm_FightSlotCheer with "gestik/jubeln"): the cheering character's
   model is hidden (ch_SetVisible, 0x402480) */

void cm_CheerAction(char *ev)
{
    CHACTION *b = (CHACTION *)ev;

    ch_SetVisible(b->cheer.fighter->scene, 0);
}
/* 0x4d3ae0, the end of a pick-up (fight command 4, cm_FightSlotPickUp): the object picked up gets the fight's
   attacker side as its owner (AIOBJ.fightSide), is carried by the fighter (AICHAR.v1ac) and hidden on the ground */

void cm_PickUpAction(char *ev)
{
    CHACTION *b = (CHACTION *)ev;
    AICHAR *att = b->pickUp.fighter;
    D3OBJECT *o = b->pickUp.object;
    AIOBJ *p;

    p = (AIOBJ *)cm_ObjectOwner((char *)o);
    if (p != 0) {
        p->fightSide = ((AICHAR *)cm_FightAttacker)->id;
        att->v1ac = (int)o;
    }
    d3if_ShowTree(o, 0);
}
/* 0x4d3b30, decides the fight of rec: the winner known already (cm_FightWinner), else the attacker cm_FightAttacker
   when no attacker of rec stands any more (health neither 0 nor 4, the body not hidden: type 1), the defender
   cm_FightDefender when no defender does, and the defender too when the attacker's side owns every "sp_CONQUER" flag;
   the winner's cutscene entry (cm_CutEntrySlot, the record of the first leader of cm_FightRecord) is sent with state 5
   (cm_SendCutEntry). Returns the winner, 0 while the fight goes on */

AICHAR *cm_CheckFightWinner(CMFIGHTREC *rec)
{
    AICHAR *winner;
    int flags, owned;
    int nDef, nAtt;
    D3VECTOR4 v;
    CMCUTENTRY entry;
    AICHAR *c;
    CMCUTENTRY *slot;
    int i, n;

    winner = 0;
    owned = 0;
    flags = 0;
    if (cm_FightWinner != 0)
        return (AICHAR *)cm_FightWinner;
    nDef = 0;
    for (i = 0; i < 16; i++) {
        if (rec->defenders[i] != -1) {
            c = (AICHAR *)cm_FindFighter(rec->defenders[i]);
            if (c->health != 0 && c->health != 4 && c->scene->obj->type != 1)
                nDef++;
        }
    }
    nAtt = 0;
    for (i = 0; i < 16; i++) {
        if (rec->attackers[i] != -1) {
            c = (AICHAR *)cm_FindFighter(rec->attackers[i]);
            if (c->health != 0 && c->health != 4 && c->scene->obj->type != 1)
                nAtt++;
        }
    }
    if (nAtt <= 0)
        winner = (AICHAR *)cm_FightAttacker;
    else if (nDef <= 0)
        winner = (AICHAR *)cm_FightDefender;
    for (i = 0; i < cm_FlagObjectCount; i++) {
        if (strstr(cm_FlagObjects[i], "sp_CONQUER") != 0) {
            d3_PointToWorld((D3OBJECT *)cm_FlagObjects[i], &((D3OBJECT *)cm_FlagObjects[i])->pos, (D3VECTOR *)&v);
            flags++;
            c = ((D3OBJECT *)cm_FlagObjects[i])->owner;
            if (c != 0 && (int)c->bld == cm_FightAttacker)
                owned++;
        }
    }
    if (flags != 0 && owned == flags)
        winner = (AICHAR *)cm_FightDefender;
    if (winner != 0) {
        n = cm_FightRecordT->nChars;
        for (i = 0; i < n && cm_FightRecordT->charIds[i] == -1; i++)
            ;
        slot = (CMCUTENTRY *)cm_CutEntrySlot((char *)&cm_CutInfosT[i], (char *)winner);
        entry = *slot;
        entry.charId = winner->netId;
        entry.state = 5;
        cm_SendCutEntry(cm_CutInfosT[i].playerId, &entry);
    }
    return winner;
}

#define G19_4C4880U ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)

/* 0x4d3d10, loot of a won fight: a random share (0.07 + 0.001 * 0 .. 49 of the loser's goods value 0x509e60, at
   most 0x9c400) is moved from the loser's record to the winner's (money message 0x4c47e0), then each object of the
   loser's stock (0x4ffe90 list 1) is moved to the winner (0x4c4880, 80 .. 99 % of its count) and its price added; the
   value goes to the winner's record (0x506430, AIFIN.lootTakings) */
int cm_LootStock(AIBLD *a, AIBLD *b, AICHAR *d, AICHAR *t)
{
    AIOBJ *oa, *ob, *q;
    int cnt, v;

    oa = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x116);
    ob = (AIOBJ *)gm_Query(a->objList, 2, 6, 0, 0x2a);
    v = gm_GetGoodsValue((char *)t);
    v = v < 0 ? 0 : v;
    v = v < 0x9c400 ? v : 0x9c400;
    v = (int)(((double)(unsigned int)cm_Random(0x32) * 0.001 + 0.07) * v);
    cm_SendMoveGoods(d->netId, t->netId, ai_OwnPlayer, v);
    if (ob != 0 && oa != 0) {
        for (q = (AIOBJ *)gm_Query(ob->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
            cnt = (int)(((double)(unsigned int)(cm_Random(0x14) + 0x50) * 0.01) * (double)q->count);
            if (cnt < 1)
                cnt = 1;
            G19_4C4880U(oa->objId, ob->objId, q->id, cnt, ai_OwnPlayer, 0);
            v = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * cnt + v);
        }
    }
    q = (AIOBJ *)gm_PlayerRecord((char *)d);
    if (q != 0)
        ((AIFIN *)q)->lootTakings += v;
    return v;
}
/* 0x4d3ea0, the loot of the carts of a won fight: for each of the 3 cart ids (CMFIGHTREC.carts), the goods of the
   cart (0x4ffe90 chains) go to the winner (0x4c4880, at least 1, f of the count; a free pick-up with b == 0) and their
   price is added; with no winner record half of the value goes to the money message 0x4c47e0; the value goes to the
   record of the attacker side (0x506430, AIFIN.lootTakings) */
int cm_LootCarts(CMFIGHTREC *a, AIBLD *b, AICHAR *d, int unused, float f)
{
    AIOBJ *ob, *q, *p;
    int cnt, i;
    int dmg = 0;
    float x;

    if (cm_FightAttacker == 0 || cm_FightDefender == 0)
        return 0;
    {
        ob = (AIOBJ *)gm_Query(b->objList, 2, 6, 0, 0x116);
        for (i = 0; i < 3; i++) {
            if (a->carts[i] != -1) {
                p = (AIOBJ *)gm_Query(0, 2, 7, 1, a->carts[i]);
                p = (AIOBJ *)gm_Query(p->list, 1, 0, 0x1dd);
                for (q = (AIOBJ *)gm_Query(p->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
                    gm_ObjectBuildingOwner((unsigned short *)q);
                    x = (float)q->count * f;
                    if (ai_FloatOne > x)
                        x = ai_FloatOne;
                    cnt = (int)x;
                    if (ob != 0)
                        G19_4C4880U(ob->objId, q->ownerId, q->id, cnt, ai_OwnPlayer, 0);
                    else
                        G19_4C4880U(-1, q->ownerId, q->id, cnt, ai_OwnPlayer, 0);
                    dmg = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * cnt + dmg);
                }
                if (ob == 0) {
                    dmg = (int)(dmg * 0.5);
                    cm_SendMoveGoods(d->netId, -1, ai_OwnPlayer, dmg);
                }
                q = (AIOBJ *)gm_PlayerRecord((char *)cm_FightAttacker);
                if (q != 0)
                    ((AIFIN *)q)->lootTakings += dmg;
            }
        }
    }
    return dmg;
}
/* 0x4d4050, the fight role of a building kind (first byte of its 0x24d-byte record): 0x13 -> 0, 0x10 -> 1, 4 -> 2, else 0 */

int cm_FightRole(unsigned char *b)
{
    char *r = ai_BTypes + *b * 0x24d;
    char k = *r;

    if (k == 0x13)
        return 0;
    if (k == 0x10)
        return 1;
    return k == 4 ? 2 : 0;
}
/* 0x4d4090, the loot list of a won fight: for each of the 3 cart ids at +0x100 of cm_FightRecord the goods (mode 0 all,
   1 only those of the player, 2 only the others) get a cell of 5 per row (120 x 96 pixels from x, y): their picture
   (0x525 frame, 0xdf + id), the count "%i " plus a suffix, the value "%T"; returns the sum of the values */

int cm_LootList(int x, int y, int mode)
{
    int i;
    int j = 0;
    int total = 0;
    int px, py;
    int t, t2;
    AIOBJ *q;
    char buf[0x100];

    for (i = 0; i < 3; i++) {
        if (cm_FightRecordT->carts[i] == -1)
            continue;
        q = (AIOBJ *)gm_Query(0, 2, 7, 1, cm_FightRecordT->carts[i]);
        q = (AIOBJ *)gm_Query(q->list, 1, 0, 0x1dd);
        for (q = (AIOBJ *)gm_Query(q->list, 1, 5); q != 0; q = (AIOBJ *)gm_QueryNext()) {
            if (mode != 0 && !(mode == 1 && q->fightSide == ai_PlayerId) &&
                !(mode == 2 && q->fightSide != ai_PlayerId))
                continue;
            px = x + (j % 5) * 120;
            py = y + (j / 5) * 96;
            d2_WinOpenPic(px, py, 0x525, d2_LastWindow);
            d2_WinOpenPic(px, py, q->id + 0xdf, d2_LastWindow);
            t = d2_CreateWinText(px - 0x10, py - 0xf, &ch_EmptyName, d2_LastWindow);
            d2_Objs[t].font = 0x56;
            d2_Objs[t].w = 0x50;
            d2_Objs[t].centred = 1;
            sprintf(buf, "%i ", q->count);
            strcat(buf, txt_AmountUnit);
            d2_SetObjText(t, buf);
            t2 = d2_CreateWinText(px - 0x10, py + 0x34, &ch_EmptyName, d2_LastWindow);
            d2_Objs[t2].font = 0x56;
            d2_Objs[t2].w = 0x50;
            d2_Objs[t2].centred = 1;
            txt_FormatS(buf, "%T", (int)gm_GoodsPrice(q->id, ai_OwnPlayer) * q->count);
            d2_SetObjText(t2, buf);
            total = (int)(gm_GoodsPrice(q->id, ai_OwnPlayer) * q->count + total);
            j++;
        }
    }
    return total;
}
/* 0x4d4340, the portrait row of the fight: for each of the 16 ids (-1 = none) a column (width = (0x6ed980 + 8 - 0x30) /
   (count + 1), 6 per row, rows of 100 pixels from x, y) with the frame 0x5da, the portrait, the health bar (0x504e00),
   the name and health text ("%1N1 %i%"), the building icon and the state icon (0x3b / 0x37) of the character */

void cm_FightPortraits(int *ids, int x, int y)
{
    int i, n = 0;
    int w, px, py, v, t, f, ic;
    AICHAR *c;
    AIWEAPON *o;
    char buf[0x100];

    for (i = 0; i < 16; i++) {
        if (ids[i] != -1)
            n++;
    }
    w = (d2_LastWindowPtr->w - 0x30) / (n + 1);
    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = (AICHAR *)cm_FindFighter(ids[i]);
        px = (i % 6 + 1) * w + x;
        py = y + i / 6 * 100;
        d2_WinOpenPic(px - 0x10, py, 0x5da, d2_LastWindow);
        d2_WinOpenPic(px - 4, py, c->picture, d2_LastWindow);
        v = gm_SkillPoints(c, 3);
        sv_ValueBar(px, py + 0x3c, (int)(double)(float)v, 0x4eb);
        t = d2_CreateWinText(px - 0x24, py - 0x11, &ch_EmptyName, d2_LastWindow);
        d2_Objs[t].w = 0x78;
        d2_Objs[t].centred = 1;
        d2_Objs[t].font = 0x56;
        txt_FormatS(buf, "%1N1 %i%", c->id, (int)(gm_HealthRatio(c) * 100.0));
        d2_SetObjText(t, buf);
        o = (AIWEAPON *)cm_CharWeapon((char *)c);
        if (o != 0 && o->id != 0) {
            ic = d2_WinOpenPic(px + 0x28, py + 0xf, o->id + 0xdf, d2_LastWindow);
            d2_ScaleObject(ic, -2);
        }
        if (c->health == 0) {
            f = (int)-c->f01c;
            if ((double)c->v024 == (double)f - 20.0)
                d2_WinOpenPic(px, py + 6, 0x3b, d2_LastWindow);
            else
                d2_WinOpenPic(px, py + 6, 0x37, d2_LastWindow);
        }
    }
}
/* 0x4d4540, the dialog "cutscenes\combat_start" that ends a fight (a: the fight record; its +0x7c bits 2 / 1 / 4 pick the
   text): the line of the winner (texts 0x1138 .. 0x113c, 0x1125 + role), the loot list 0x4d4090,
   the portrait row 0x4d4340, and the buttons: the player picks the share 10 / 50 / 90 (chance share <= random 10 .. 99)
   or the AI side picks at random; the loot goes through 0x4d3ea0, the result is 0x6f0cfc (-1: refused), 0x6f0cf8 = 1 when
   the loot was taken. Returns 1 when the fight goes on. Rough draft.
   @deep Read against the original in full: fixed the loot total (summed into total and printed by 0x1141,
   not v), the per-block rc init (int rc[0x2e] = {0} in the loot block: mov [esp+0x24],0 + rep stosd 0x2d),
   the share test (rand+10 <= st takes the loot, else refuse), b4 = 0 on the AI path, tb/tc 0x400 (frame
   now 0x8cc both). No ref/literal/call differences left, frame equal, ins 607 vs 611. Left: allocation -
   the original keeps win in a slot ([esp+0x1c], mov ebx,[esp+0x1c] before d2_SetForm(win,1) at 0x4d4797
   and reloads at each d2_SetForm/d2_GetWinObj), ca in ebp, a reloaded from [esp+0x8e0]; ours holds win in
   ebx and a in ebp; the three PrintText branches share one 'mov edi,eax' join (0x4d4795) where ours keeps
   a copy per branch; b1..b3 in ebx/ebp/slot vs our slots. */

int cm_FightStartDialog(char *a)
{
    int res = 1;
    int st = 0;
    int win, ca, cb, role, edi, i, n, r, v, sel, esi, total;
    int b1, b2, b3, b4;
    float fv;
    char *p;
    char tb[0x400];
    char tc[0x400];

    win = d2_OpenForm(0, 0, "cutscenes\\combat_start");
    ((void (*)(int, int))d2_PlaceMainWindow)(win, 3);
    cm_FightSettled = 0;
    ca = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x14));
    cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
    role = cm_FightRole((unsigned char *)cb);
    d2_SetForm(win, 1);
    if (*(char *)(cm_FightAttacker + 2) == 6) {
        if (a[0x7c] & 2) {
            io_PrintText(0x1138, role + 0x1125, cb, *(unsigned short *)cm_FightDefender, ca);
            d2_SetForm(win, 2);
            edi = io_PrintText(0x113a, *(unsigned short *)cm_FightDefender, ca);
        } else if (a[0x7c] & 1) {
            for (i = 0; i < 3; i++) {
                if (*(int *)(a + 0x100 + i * 4) != -1)
                    st++;
            }
            fv = gm_GetSkill(cm_FightAttacker, 3);
            io_PrintText(0x112e, cb, *(unsigned short *)cm_FightDefender);
            d2_SetForm(win, 2);
            io_PrintText(0x112f, st, 0xc10);
            if (fv >= 0.5) {
                if (*(int *)(a + 0x110) != 0)
                    io_PrintText(0x1134);
                else
                    io_PrintText(0x1135);
            }
            if (fv >= 0.75) {
                v = gm_SkillPoints(cm_FightDefender, 3);
                r = (int)(v * 0.023809524f);
                if (r > 4)
                    r = 4;
                r = r < 0 ? 0 : r;
                io_PrintText(0x1131, r + 0x1128);
            }
            if (*(char *)(cm_FightDefender + 9) == 0)
                edi = io_PrintText(0x1133, 0x1136);
            else
                edi = io_PrintText(0x1133, 0x1137);
        } else if (a[0x7c] & 4) {
            io_PrintText(0x113b, cb, *(unsigned short *)cm_FightDefender);
            d2_SetForm(win, 2);
            edi = io_PrintText(0x113c, *(unsigned short *)cm_FightDefender, ca);
        }
        d2_SetForm(win, 1);
        io_PrintText(0x1132);
        cm_FightPortraits((int *)(a + 0xc0), 0, 0xb4);
        d2_GetWinObj(win, 2, edi);
        edi++;
        d2_GetWinObj(win, 2, edi);
        while (sv_RunFrame(cm_FightStartDialog, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                cm_FightSettled = 1;
                res = 0;
            }
            if (d2_ClickedId == 0) {
                res = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                cm_FightSettled = 1;
                res = 0;
                sv_PopRequest = 1;
            }
        }
    }
    if ((a[0x7c] & 1) && *(char *)(cm_FightDefender + 2) == 6 && cm_FightSettled == 0) {
        int rc[0x2e] = {0};

        total = 0;
        for (i = 0; i < 3; i++) {
            if (*(int *)(a + 0x100 + i * 4) != -1)
                st++;
        }
        p = gm_Query(0, 2, 7, 1, *(int *)(a + 0x100));
        p = *(char **)(p + 0x24);
        d2_SetForm(win, 1);
        io_PrintText("$C");
        if (p != 0) {
            p = gm_FindBuildingById(*(int *)(p + 0xb4));
            esi = 0;
            for (i = 0; i < 16; i++) {
                if (*(int *)(a + 0xc0 + i * 4) != -1)
                    esi++;
            }
            if (p != 0)
                io_PrintText(0x113d, st, 0xc10, p, *(unsigned short *)cm_FightAttacker, esi);
        }
        v = gm_SkillPoints(cm_FightDefender, 3);
        r = (int)(v * 0.023809524f);
        if (r > 4)
            r = 4;
        if (*(int *)(a + 0x110) != 0)
            io_PrintText(0x113e, (r < 0 ? 0 : r) + 0x1128);
        else
            io_PrintText(0x113f);
        cm_LootList(0x10, 0x96, 0);
        for (i = 0; i < 3; i++) {
            n = *(int *)(a + 0x100 + i * 4);
            if (n != -1) {
                p = (char *)gm_FindObjectInst(n);
                if (p != 0) {
                    gm_StallGoodsList(p, rc);
                    for (esi = 0; esi < rc[1]; esi++)
                        total = (int)(gm_GoodsPrice(*(unsigned short *)((char *)rc + 0x48 + esi * 2), ai_OwnPlayer) * rc[0x1c + esi] + total);
                }
            }
        }
        d2_SetForm(win, 2);
        io_PrintText("$C");
        esi = io_PrintText(0x1141, (int)(total * 0.1), (int)(total * 0.5), (int)(total * 0.8));
        b1 = d2_GetWinObj(win, 2, esi);
        b2 = d2_GetWinObj(win, 2, esi + 1);
        b3 = d2_GetWinObj(win, 2, esi + 2);
        edi = 0;
        for (i = 0; i < 16; i++) {
            if (*(int *)(a + 0x80 + i * 4) != -1)
                edi++;
        }
        b4 = d2_GetWinObj(win, 2, esi + 3);
        if (edi == 0)
            d2_EnableObject(b4, 0);
        while (sv_RunFrame(cm_FightStartDialog, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                res = 0;
            }
            if (d2_ClickedId == -1)
                continue;
            sel = d2_PointerObj;
            if (sel == b4) {
                sv_PopRequest = 1;
                res = 1;
            } else if (sel == b1 || sel == b2 || sel == b3) {
                st = 0;
                if (sel == b1)
                    st = 10;
                else if (sel == b2)
                    st = 50;
                else if (sel == b3)
                    st = 90;
                if ((unsigned short)util_RandomU(0x5a) + 10 <= st) {
                    cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                    txt_Format(tb, 0x1142);
                    cm_MessageBox(tb, 0);
                    fv = (float)(st * 0.01);
                    sv_PopRequest = 1;
                    b4 = 0;
                    cm_FightSettled = 1;
                    cm_FightLootValue = cm_LootCarts(cm_FightRecord, cb, cm_FightAttacker, cm_FightDefender, fv);
                } else {
                    txt_Format(tc, 0x1143);
                    cm_MessageBox(tc, 0);
                    sv_PopRequest = 1;
                    res = 1;
                }
            }
        }
    } else if ((a[0x7c] & 1) && *(char *)(cm_FightDefender + 2) != 6 && cm_FightSettled == 0) {
        n = 0;
        for (i = 0; i < 16; i++) {
            if (*(int *)(a + 0x80 + i * 4) != -1)
                n++;
        }
        if (n == 0 || (unsigned short)util_RandomU(5) > *(int *)(a + 0x110)) {
            st = 0;
            r = (unsigned short)util_RandomU(4);
            if (r == 0)
                st = 10;
            else if (r == 1)
                st = 90;
            else if (r >= 2)
                st = 50;
            if ((unsigned short)util_RandomU(0x5a) + 10 <= st) {
                cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                fv = (float)(st * 0.01);
                b4 = 0;
                cm_FightSettled = 1;
                cm_FightLootValue = cm_LootCarts(cm_FightRecord, cb, cm_FightAttacker, cm_FightDefender, fv);
            } else {
                cm_FightLootValue = -1;
                res = 1;
            }
        }
    }
    if (win != -1)
        d2_CloseView(win);
    return res;
}

#define G20_US(p) (*(unsigned short *)(p))
#define G20_NAME(id) (((char **)txt_ObjectNames)[(id) * 2])
#define G20_ME (ai_CharInts[ai_PlayerId][0])
#define G20_ATT ((char *)cm_FightDefender)
#define G20_DEF ((char *)cm_FightAttacker)
/* the pump of the dialog: the click on a button (f8a668 == 0) or the abort flag closes it */
#define G20_PUMP()                                                                                      \
    while (sv_RunFrame(cm_FightEndDialog, 0x7d6c6)) {                                                       \
        if (dm_L.rup != 0 || d2_ClickedId == 0)                                                    \
            sv_PopRequest = 1;                                                                           \
    }
/* the winner takes a share of the loser's money (message 0x4c47e0) and of the goods of ob (80 .. 99 %), the text lines
   go to tc; the money part v is added to the winner's record afterwards (0x506430, +0x4d of the building) */
#define G20_TAKE()                                                                                      \
    ba[0] = ch_EmptyName;                                                                               \
    memset(ba + 1, 0, 0x3ff);                                                                           \
    txt_Format(tc, 0x11a5, G20_US(G20_DEF), ca);                                                      \
    cm_MessageBox(tc, 0);                                                                                \
    ob = gm_Query(*(int *)(ca + 0x5d), 2, 6, 0, 0x2a);                                              \
    if (ob == 0)                                                                                        \
        ob = gm_Query(*(int *)(ca + 0x5d), 2, 6, 0, 0x116);                                         \
    v = gm_GetGoodsValue(cm_FightDefender);                                                                     \
    v = v < 0x9c400 ? v : 0x9c400;                                                                      \
    v = (int)(((double)(unsigned int)cm_Random(0x32) * 0.001 + 0.07) * (v < 0 ? 0 : v));             \
    txt_Format(tc, "$Z%T$A", v);                                                                      \
    cm_SendMoveGoods(*(int *)(G20_DEF + 4), *(int *)(G20_ATT + 4), ai_OwnPlayer, v);                        \
    if (ob != 0 && obj != 0) {                                                                          \
        for (q = gm_Query(*(int *)(ob + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {         \
            cnt = (int)((double)(unsigned int)(cm_Random(0x14) + 0x50) * 0.01 * *(int *)(q + 0xe));  \
            if (cnt < 1)                                                                                \
                cnt = 1;                                                                                \
            sprintf(tmp, "%i %s$A", cnt, G20_NAME(G20_US(q)));                                          \
            strcat(tc, tmp);                                                                            \
            if (G20_ME == cm_FightOwner)                                                                 \
                G19_4C4880U(*(int *)(obj + 2), *(int *)(ob + 2), G20_US(q), cnt, ai_OwnPlayer, 0);      \
            v = (int)(gm_GoodsPrice(G20_US(q), ai_OwnPlayer) * cnt + v);                                 \
        }                                                                                               \
    }
/* the end of the take: the text shown, the money added to the winner's record and the building's */
#define G20_TAKE_END()      \
    d2_SetForm(win, 1);                                                                                 \
    io_PrintText(tc);                                                                                   \
    q = gm_PlayerRecord(G20_DEF);                                                                          \
    if (q != 0)                                                                                         \
        *(int *)(q + 0x1c) += v;                                                                        \
    if (*(int *)(a + 0x10c) != -1) {                                                                    \
        q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));                                                    \
        *(int *)(q + 0x4d) += v;                                                                        \
    }
/* the winner loses face: the text 0x11a6 and a drop of 0x14 - random of the defender's mood (0x50cad0) */
#define G20_FACE()                                                                                      \
    tc[0] = ch_EmptyName;                                                                               \
    memset(tc + 1, 0, 0x3ff);                                                                           \
    txt_Format(tc, 0x11a6, G20_US(G20_DEF), ca);                                                      \
    cm_MessageBox(tc, 0);                                                                                \
    if (G20_ME == cm_FightOwner)                                                                         \
        gm_ChangeBuildingState(ca, 0x14 - cm_Random(0x14));
/* nothing taken: the text 0x11a7 and the fight with the tavern keepers' men: every character of the table of 0x770b80
   that is a guard (kinds 10 .. 12) gets 0x4c4b00 */
#define G20_GUARDS(bb)                                                                                    \
    txt_Format(bb, 0x11a7, G20_US(G20_DEF), G20_US(G20_DEF));                                         \
    cm_MessageBox(bb, 0);                                                                                \
    for (p = (char *)ai_CharInts - 4; p < (char *)(unsigned)((unsigned)ai_CharInts + 0x653fc); p += 0x21c) {                                  \
        if (*(short *)p != -1 && p[8] != 0 &&                                                           \
            ((k = gm_TitleGroup((unsigned char)p[0x164])) == 10 || k == 11 || k == 12))                  \
            cm_SendRelations(*(int *)(G20_DEF + 4), *(int *)(p + 4), 6, 1);                                 \
    }

/* 0x4d4d70, the end of a fight (a: the fight record, its +0x7c bits 1 / 2 / 4 say how it was fought): the dialog
   "cutscenes\combat_end" with the winner's text (0x1189 .. 0x118f, 0x1191, 0x1193, 0x1197), the portrait rows of both
   sides (0x4d4340), then by the kind of the fight: the loot of the carts (bit 1: money 0x4d4090, goods moved to the
   winner), the destruction of a building (bit 4: the 0x5db cells with the bars, money by 0x4c47e0), or the capture
   of the defeated character (bit 2: texts 0x119f .. 0x11a7 pick one of four outcomes: 1 the building is taken,
   2 a share of money and goods is taken, 3 the winner loses face, 0 the guards of the town are called). Rough draft.
   @deep Read against the original in full (0x4d4d70..0x4d60a3). Fixed: frame 0x1124 (ba 0x800, bb, tc
   0x400, tmp 0x100: alloca_probe now on both sides), TAKE/FACE write tc (the 0x134 buffer), path 1's
   guards text in ba and the AI branch's in bb, the AI branch tests gm_TitleJob once and picks k = 0 (text
   0x11a4 only) or rand(3)+1, gm_PlayerFactor gets ai_OwnPlayer, 1.0f is ai_FloatOne, obj = 0 first. The
   original shares code between branches (path 1 n==3 jumps into the AI branch's FACE at 0x4d5f72, the AI
   TAKE joins path 1's TAKE tail at 0x4d590d, TAKE and the &4 branch join the &1 pump at 0x4d5402): written
   as goto face / take_done / pump. Left: the shared pump at 0x4d5402 is unrotated in the original (one
   sv_RunFrame, back edges jmp 0x4d5407), ours VC6 rotates it whatever the form (while, for(;;), goto loop
   tried: 6 vs 5 sv_RunFrame); ours keeps total in esi like v so the &4 tail (+0x4d += total) merges with
   TAKE_END (gm_BuildQuery 7 vs 8) where the original keeps total in a slot; side lives in a slot and edi
   on our side, the original only edi then one store; the rest regs/slots. ins 1510 vs 1508. (references:
   M8 at +0xfd5: ours refers to cm_FightAttacker (0x6f0cec), the original's field to 0x6f0ce8
   (cm_FightDefender), explained: M8 +0xfd5 is the alignment pairing two loads of one cm_SendMoveGoods call
   whose registers are swapped: both sides push (cm_FightAttacker+4, cm_FightDefender+4, ai_OwnPlayer, v) -
   ours loads Defender into eax and Attacker into edx, the original Defender into edx and Attacker into
   ecx) */
void cm_FightEndDialog(char *a)
{
    char ba[0x800];
    char bb[0x400];
    char tc[0x400];
    char tmp[0x100];
    char *obj, *side, *c, *ca, *ob, *q, *p, *kr;
    int win, i, n, role = 0, v, cnt, total, na, nb, id, k;
    int h, row, col, px, py;

    obj = 0;
    d2_CopyCaption(&ch_EmptyName);
    sv_SetStatusText(&ch_EmptyName);
    if (*(int *)(a + 0x10c) != -1) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        obj = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x116);
        if (obj == 0)
            obj = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x2a);
    }
    if (cm_FightOwner == G20_ME) {
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0x80 + i * 4);
            if (id != -1 && (c = gm_FindChar(id)) != 0)
                cm_FightWeaponLoss(c);
        }
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0xc0 + i * 4);
            if (id != -1 && (c = gm_FindChar(id)) != 0)
                cm_FightWeaponLoss(c);
        }
    }
    if (cm_FightWinner == 0) {
        na = 0;
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0x80 + i * 4);
            if (id != -1) {
                c = cm_FindFighter(id);
                if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                    na++;
            }
        }
        nb = 0;
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0xc0 + i * 4);
            if (id != -1) {
                c = cm_FindFighter(id);
                if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                    nb++;
            }
        }
        if (nb > na)
            side = G20_DEF;
        else
            side = G20_ATT;
    } else {
        side = G20_ATT;
        if (cm_FightWinner != cm_FightAttacker)
            side = G20_DEF;
    }
    win = d2_OpenForm(0, 0, "cutscenes\\combat_end");
    ((void (*)(int, int))d2_PlaceMainWindow)(win, 3);
    d2_SetForm(win, 2);
    io_PrintText(0x7d);
    d2_SetForm(win, 1);
    if (G20_ME == *(int *)(side + 4)) {
        if (side == G20_ATT) {
            if (a[0x7c] & 1) {
                io_PrintText(0x1189, G20_US(G20_ATT), G20_US(G20_DEF));
                io_PrintText(0x118a);
            } else
                io_PrintText(0x1189, G20_US(G20_ATT), G20_US(G20_DEF));
        } else
            io_PrintText(0x118e, G20_US(G20_DEF), G20_US(G20_ATT));
    } else if (side == G20_DEF) {
        io_PrintText(0x118c, G20_US(G20_ATT), G20_US(G20_DEF));
        if (a[0x7c] & 1) {
            q = gm_Query(0, 2, 7, 1, *(int *)(a + 0x100));
            if (*(int *)(q + 0x24) != 0 && gm_FindBuildingById(*(int *)(*(char **)(q + 0x24) + 0xb4)) != 0)
                io_PrintText(0x118d);
        }
    } else
        io_PrintText(0x118f, G20_US(G20_DEF), G20_US(G20_ATT));
    G20_PUMP();
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1191);
    c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
    kr = ai_BTypes + (unsigned char)c[0] * 0x24d;
    if (*kr == 0x13)
        role = 0;
    else if (*kr == 0x10)
        role = 1;
    else if (*kr == 4)
        role = 2;
    io_PrintText(0x1193, G20_US(G20_DEF), role + 0x1125);
    cm_FightPortraits((int *)(a + 0xc0), 0, 100);
    ca = gm_BuildQuery(1, 1, *(int *)(a + 0x14));
    if (ca != 0) {
        kr = ai_BTypes + (unsigned char)ca[0] * 0x24d;
        if (*kr == 0x13)
            role = 0;
        else if (*kr == 0x10)
            role = 1;
        else if (*kr == 4)
            role = 2;
    } else
        role = 0;
    io_PrintText("$7A");
    io_PrintText(0x1193, G20_US(G20_ATT), role + 0x1125);
    cm_FightPortraits((int *)(a + 0x80), 0, 0x12c);
    G20_PUMP();
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1197);
    if (a[0x7c] & 1) {
        if (G20_US(&ai_PlayerId) == G20_US(G20_ATT))
            cm_LootList(0x24, 0x32, 2);
        else if (G20_US(&ai_PlayerId) == G20_US(G20_DEF)) {
            v = cm_LootList(0x24, 0x32, side == G20_DEF ? 0 : 1);
            q = gm_PlayerRecord(G20_DEF);
            if (q != 0)
                *(int *)(q + 0x1c) += v;
            if (*(int *)(a + 0x10c) != -1) {
                q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                *(int *)(q + 0x4d) += v;
            }
        }
        for (i = 0; i < 3; i++) {
            id = *(int *)(a + 0x100 + i * 4);
            if (id == -1)
                continue;
            p = gm_Query(0, 2, 7, 1, id);
            p = gm_Query(*(int *)(p + 0x14), 1, 0, 0x1dd);
            for (q = gm_Query(*(int *)(p + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
                if (side == G20_DEF)
                    *(int *)(q + 0xa) = G20_US(G20_DEF);
                if (*(int *)(q + 0xa) == G20_US(G20_DEF)) {
                    if (obj != 0 && cm_FightOwner == G20_ME) {
                        gm_ObjectBuildingOwner(q);
                        G19_4C4880U(*(int *)(obj + 2), *(int *)(q + 6), G20_US(q), *(int *)(q + 0xe), ai_OwnPlayer, 0);
                    }
                    gm_GoodsPrice(G20_US(q), ai_OwnPlayer);
                }
            }
        }
        pump:
        G20_PUMP();
    } else if ((a[0x7c] & 2) && side == G20_DEF && G20_US(side) == ai_PlayerId) {
        ca = gm_BuildQuery(1, 1, *(int *)(a + 0x14));
        txt_Format(ba, 0x119f, ca, G20_US(G20_DEF), ca);
        k = ca[0];
        if (k != 0x1e && k != 0x20 && k != 0x1f) {
            txt_Format(bb, 0x11a0, (unsigned char)ca[0] * 14 + 0x48e);
            strcat(ba, bb);
        }
        txt_Format(bb, 0x11a1);
        strcat(ba, bb);
        txt_Format(bb, 0x11a2);
        strcat(ba, bb);
        txt_Format(bb, 0x11a3);
        strcat(ba, bb);
        n = cm_MessageBox(ba, 0x11);
        k = ca[0];
        if (k == 0x1e || k == 0x20 || k == 0x1f)
            n++;
        if (n == 1) {
            k = ai_BTypes[(unsigned char)ca[0] * 0x24d];
            if (k == 4 || k == 0x10)
                ai_CountMissionEvent(G20_DEF, 0x1c);
            cm_SendOwnership(*(int *)(ca + 1), *(int *)(G20_DEF + 4), *(int *)(G20_DEF + 4));
            txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
            cm_MessageBox(ba, 0);
        } else if (n == 2) {
            G20_TAKE();
        take_done:
            G20_TAKE_END();
            goto pump;
        } else if (n == 3) {
            goto face;
        } else if (n == 0) {
            G20_GUARDS(ba);
        }
    } else if (a[0x7c] & 4) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        if (ai_BTypes[(unsigned char)c[0] * 0x24d] == 0x13) {
            total = 0;
            for (i = 0; i < 16; i++) {
                id = *(int *)(a + 0x80 + i * 4);
                if (id == -1)
                    continue;
                c = cm_FindFighter(id);
                col = i % 4;
                row = i / 4 + 1;
                px = col * 80 + 0x30;
                py = row * 100;
                d2_WinOpenPic(px, py, 0x5db, d2_LastWindow);
                d2_WinOpenPic(px + 2, py, *(int *)(c + 0x18c), d2_LastWindow);
                sv_ValueBar(px, py + 0x3c, (unsigned char)c[0x83], 0x4eb);
                h = 0;
                if (c[8] == 0) {
                    v = (int)((unsigned char)c[0x83] * 0.023809524f + ai_FloatOne);
                    d2_WinOpenPic(px + 6, py + 6, 0x3b, d2_LastWindow);
                    h = gm_PlayerFactor(v * 40, ai_OwnPlayer);
                }
                if (side == G20_DEF) {
                    if ((unsigned char)c[8] > 0) {
                        v = 0;
                        cm_BeginFieldBlock(c, *(int *)(c + 4));
                        cm_AddFieldValue(4, 1, (int)c - (int)cm_FieldObject + 0x16c, &v);
                        cm_SendFieldDeltas();
                    }
                    txt_Format(tmp, "%T", h);
                    if (h != 0)
                        d2_CreateWinText(px, py + 0x50, tmp, d2_LastWindow);
                    total += h;
                }
            }
            if (total != 0)
                cm_SendMoveGoods(*(int *)(G20_DEF + 4), -1, ai_OwnPlayer, total);
            else if (G20_US(&ai_PlayerId) == G20_US(G20_DEF))
                io_PrintText(0x11b6);
            if (*(int *)(a + 0x10c) != -1) {
                q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                *(int *)(q + 0x4d) += total;
            }
            goto pump;
        }
    } else if ((a[0x7c] & 2) && side == G20_DEF && G20_DEF[2] != 6 && G20_DEF[2] != 7) {
        k = 0;
        if (!gm_TitleJob(G20_US(G20_DEF), gm_BuildingJob((unsigned char)*kr)) || *kr == 0xd || *kr == 0xc || *kr == 0xb)
            k = (unsigned short)util_RandomU(3) + 1;
        if (k == 0) {
            txt_Format(bb, 0x11a4, G20_US(G20_DEF), ca);
            cm_MessageBox(bb, 0);
        } else if (k == 2) {
            G20_TAKE();
            goto take_done;
        } else if (k == 3) {
        face:
            G20_FACE();
        } else if (k == 1) {
            G20_GUARDS(bb);
        }
    }
    di_ClearEvents(0x10);
    d2_CloseView(win);
}
/* 0x4d60b0, the speech texts of a fight (a: the fight record): for both leaders (the characters of +0x34 / +0x38) one
   line each, by the kind of the fight (bit 1: 0x11e0 / 0x11e1, bit 2: 0x11e3 / 0x11e4 with the role of the target
   building 0x4d4050, bit 4: 0x11e6 / 0x11e7), the suffix of the kind (0x143e948 / 0x143e954 / 0x143e960) appended and
   handed to 0x4e48b0. 0 when a leader is missing or the leaders are the same, a cart has no goods (0x50aa00) or the
   target is gone. Rough draft.
   @deep Read against the original in full: each branch ends with its own cm_SendPartyNote(cb, ...) call (the
   original has 5 calls: &1 and &4 share the second call's tail at 0x4d6373, &2 keeps its own with its own
   return 1), now on our side too (calls equal, frame equal, ins 283 vs 282). Left: allocation - the
   original keeps i in a slot ([esp+0x10], 0x4d60fc / 0x4d6142..0x4d614d) and cb in esi + slot [esp+0x18],
   ca in ebp; ours i in edi, cb only in the slot (mov eax,[esp+0x10] before each mov dx,[eax]), the target
   building in esi vs edi; role kept in ebp on our side vs a slot ([esp+0x30]). */

int ai_Unk4d60b0(char *a)
{
    char buf[0x400];
    int rc[0x2e];
    char *ca, *cb, *edi, *p;
    int i, role;

    ca = gm_FindChar(*(int *)(a + 0x34));
    cb = gm_FindChar(*(int *)(a + 0x38));
    if (ca == 0 || cb == 0 || ca == cb)
        return 0;
    if (a[0x7c] & 1) {
        for (i = 0; i < 3; i++) {
            if (*(int *)(a + 0x100 + i * 4) != -1) {
                p = (char *)gm_FindObjectInst(*(int *)(a + 0x100 + i * 4));
                if (p == 0)
                    return 0;
                gm_StallGoodsList(p, rc);
                if (rc[0x2c] <= 0)
                    return 0;
            }
        }
    }
    edi = gm_FindBuildingById(*(int *)(a + 0x14));
    if (gm_BuildQuery(1, 1, *(int *)(a + 0x10c)) == 0)
        return 0;
    if (a[0x7c] & 1) {
        txt_Format(buf, 0x11e0, G20_US(cb));
        strcat(buf, ai_Unk143e948);
        cm_SendPartyNote(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e1, G20_US(ca));
        strcat(buf, ai_Unk143e948);
        cm_SendPartyNote(cb, a, buf, &ch_EmptyName);
    } else if (a[0x7c] & 2) {
        role = cm_FightRole((unsigned char *)gm_FindBuildingById(*(int *)(a + 0x10c))) + 0x1125;
        txt_Format(buf, 0x11e3, role, G20_US(cb), edi);
        strcat(buf, ai_Unk143e954);
        cm_SendPartyNote(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e4, G20_US(ca), role, edi);
        strcat(buf, ai_Unk143e954);
        cm_SendPartyNote(cb, a, buf, &ch_EmptyName);
    } else if (a[0x7c] & 4) {
        txt_Format(buf, 0x11e6, G20_US(cb));
        strcat(buf, ai_Unk143e960);
        cm_SendPartyNote(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e7, G20_US(ca));
        strcat(buf, ai_Unk143e960);
        cm_SendPartyNote(cb, a, buf, &ch_EmptyName);
    }
    return 1;
}
/* 0x4d63e0, a fight starts (a: the fight record): characters that are gone are dropped from both id lists, a cart
   without goods or no defender ends it (bad = 2); with the player present (0x6f0d8c == 0) the fight scene is built and
   fought (cm_SetupFightScene .. cm_FightIntro, the party run by cm_RunSceneParty), cm_FightStartDialog settles the
   loot and cm_FightEndDialog shows the end, else (remote) it is decided at once by strength (sum of cm_FightStrength +
   random 30 per side, every fighter loses up to 20 * random 10 health) and the loot goes through cm_LootStock /
   cm_LootCarts. */

void cm_RunFight(CMFIGHTREC *a)
{
    char buf[0x800];
    int rc[0x2e];
    int bad = 0, nb = 0, seed, i, id, sa, sb, w, n, r;
    char *c, *p, *win, *e1, *e2;
    AICHAR *ch;
    AIBLD *bld;
    int *e;

    seed = cut_GetRandSeed();
    for (i = 0; i < 16; i++) {
        id = a->defenders[i];
        if (id != -1 && gm_FindChar(id) == 0) {
            bad = 1;
            a->defenders[i] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        id = a->attackers[i];
        if (id != -1) {
            if (gm_FindChar(id) == 0) {
                bad = 1;
                a->attackers[i] = -1;
            } else
                nb++;
        }
    }
    if (a->flags & 1) {
        for (i = 0; i < 3; i++) {
            id = a->carts[i];
            if (id != -1) {
                p = (char *)gm_FindObjectInst(id);
                if (p != 0) {
                    gm_StallGoodsList(p, rc);
                    if (rc[0x2c] > 0)
                        continue;
                }
                bad = 2;
            }
        }
    }
    if (gm_BuildQuery(1, 1, a->attackerBldId) == 0 || bad == 2 || nb == 0)
        return;
    if (cm_SkipEvents == 0) {
        cm_SceneDuration = 200;
        cm_FightOwner = a->owner;
        cm_SetupFightScene(a);
        cm_CollectSceneChars();
        cm_PlaceFighters(a->attackers, "OFF");
        cm_PlaceFighters(a->defenders, "DEF");
        cm_FightIntro((char *)a);
        if (cm_FightFixedLight != 0)
            d3_SetLightSet(2, 0, 1.0f, 1);
        else
            sv_UpdateSkyPhase(&ai_Time);
        cm_FightWinner = 0;
        cm_FightSettled = 0;
        cm_FightLootValue = 0;
        cm_FightStartDialog((char *)a);
        cut_RandSeed(seed);
        cm_CollectSceneChars();
        cm_CollectSceneChars();
        cm_RunSceneParty((char *)a, 0);
        if (cm_FightWinner == 0 && cm_FightSettled == 0)
            cm_RunSceneParty((char *)a, 0);
        if (cm_FightSettled == 0) {
            for (i = 0; i < 16; i++) {
                if (cm_DamagePops[i].h != -1)
                    d2_RemoveObj(cm_DamagePops[i].h);
                cm_DamagePops[i].h = -1;
                cm_DamagePops[i].v = 0;
                cm_DamagePops[i].c = 0;
            }
            cm_FightEndDialog((char *)a);
            cm_FightOutro((char *)a);
            cm_EndFightScene(a);
            return;
        }
        if (cm_FightLootValue != 0 && cm_FightAttackerC->kind == 6) {
            e1 = 0;
            bld = (AIBLD *)gm_BuildQuery(1, 1, cm_FightRecordT->attackerBldId);
            if (bld != 0)
                e1 = gm_Query(bld->objList, 2, 6, 0, 0x116);
            if (cm_FightLootValue > 0) {
                if (e1 == 0)
                    txt_Format(buf, 0x1144, cm_FightLootValue);
                else
                    txt_Format(buf, 0x1145, cm_FightLootValue);
            } else
                txt_Format(buf, 0x1146);
            cm_MessageBox(buf, 0x10);
        }
        cm_EndFightScene(a);
        return;
    }
    sa = 0;
    sb = 0;
    cm_FightAttacker = gm_FindChar(a->attackerId);
    cm_FightDefender = gm_FindChar(a->defenderId);
    for (i = 0; i < 16; i++) {
        id = a->defenders[i];
        if (id != -1 && (ch = (AICHAR *)gm_FindChar(id)) != 0 && ch->health != 0 && ch->health != 4) {
            sa = (int)(cm_FightStrength((char *)ch) + sa);
            r = (unsigned short)util_RandomU(10);
            gm_AddHealth(ch->id, -r * 20);
        }
    }
    for (i = 0; i < 16; i++) {
        id = a->attackers[i];
        if (id != -1 && (ch = (AICHAR *)gm_FindChar(id)) != 0 && ch->health != 0 && ch->health != 4) {
            sb = (int)(cm_FightStrength((char *)ch) + sb);
            r = (unsigned short)util_RandomU(10);
            gm_AddHealth(ch->id, -r * 20);
        }
    }
    if ((unsigned short)util_RandomU(30) + sb > (unsigned short)util_RandomU(30) + sa)
        win = G20_DEF;
    else
        win = G20_ATT;
    for (i = 0; i < 16; i++) {
        id = a->defenders[i];
        if (id != -1 && (ch = (AICHAR *)gm_FindChar(id)) != 0)
            cm_FightWeaponLoss((char *)ch);
    }
    for (i = 0; i < 16; i++) {
        if (a->attackers[i] != -1 && (ch = (AICHAR *)gm_FindChar(a->defenders[i])) != 0)
            cm_FightWeaponLoss((char *)ch);
    }
    if (a->flags & 2) {
        if (win == G20_DEF) {
            e1 = gm_BuildQuery(1, 1, a->attackerBldId);
            e2 = gm_BuildQuery(1, 1, a->placeId);
            cm_LootStock(e2, e1, G20_DEF, G20_ATT);
        }
    } else if (!(a->flags & 4) && (a->flags & 1) && win == G20_DEF) {
        c = gm_BuildQuery(1, 1, a->attackerBldId);
        cm_LootCarts((char *)a, c, G20_DEF, cm_FightDefender, 100.0f);
    }
}
/* 0x4d6930, the result of the fight in the record r (+0x354 / +0x358): 0x6f0cf8 set -> 0 and the ransom 0x6f0cfc, else 1 */

void cm_FightResult(char *r)
{
    if (cm_FightSettled != 0) {
        ((CMCUTINFO *)r)->result = 0;
        ((CMCUTINFO *)r)->lootValue = cm_FightLootValue;
    } else
        ((CMCUTINFO *)r)->result = 1;
}
/* 0x4d6970, one frame of the fight UI (r: the fight record): the countdown bar cm_FightTimeBar sized to the fight area,
   the redraws 0x525ff0 .. 0x4ce830, and the click on the button cm_FightButton (while 0x6f0d88 / 0x6f0cf4 are clear) sends
   message 0x50 with the cut entry of the player's character (0x4cdf00) in state 5, else the dialog is closed (0x6f22a0) */

void cm_FightFrame(char *r)
{
    CMCUTENTRY sl;
    CMCUTENTRY *q;

    d2_SetObjValue(cm_FightTimeBar, 0, cm_SceneDuration, cm_SceneDeadline - cm_SceneTicks + 1);
    sv_BorderCameraView();
    cm_UpdatePortraits();
    cm_UpdatePointer();
    cm_ClickSceneChar();
    cm_FightMouse(r);
    cm_UpdateSceneChars();
    cm_UpdateCharInfoWins(r);
    cm_UpdateBombs();
    cm_UpdateGrenades();
    if (cm_SceneTimedOut == 0 && cm_FightWinner == 0) {
        if (d2_ClickedId != -1 && d2_PointerObj == cm_FightButton) {
            q = (CMCUTENTRY *)cm_CutEntrySlot(r, cm_CutChar);
            memcpy(&sl, q, sizeof(CMCUTENTRY));
            sl.charId = ((AICHAR *)cm_CutChar)->netId;
            sl.state = 5;
            cm_SendCutEntry(((CMCUTINFO *)r)->playerId, &sl);
        }
    } else
        sv_PopRequest = 1;
}
/* 0x4d6a50, builds the fight UI: the form "cutscenes\combat" (cm_FightForm), the button cm_FightButton (text
   cm_FightButtonText) and the bar cm_FightTimeBar at the bottom, then the portrait boxes (0x4cfe70 / 0x4cfc10) */

void cm_BuildFightUI(char *a)
{
    int v, t;

    v = d2_OpenForm(0, 0, "cutscenes\\combat");
    cm_FightForm = v;
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    t = d2_WinCreateSlider(0x10, 0xc, 0xbf, cm_FightButtonText, d2_LastWindow);
    cm_FightButton = t;
    d2_Objs[t].centred = 1;
    d2_CenterObj(cm_FightButton);
    t = d2_WinOpenSlider((d2_LastWindowPtr->w - 0x190) / 2, 0x37, 0x190, 0, cm_SceneDuration, 0x83, 0x42, d2_LastWindow);
    cm_FightTimeBar = t;
    d2_Objs[t].font = 0x54;
    d2_Objs[d2_Wins[d2_LastWindow].parent].interactive = 1;
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    cm_ClosePortraits();
    cm_BuildPortraits();
}
/* 0x4d6b70, closes the fight UI: the form cm_FightForm, the marks (0x4d08c0), the portrait boxes */

void cm_CloseFightUI()
{
    d2_CloseView(cm_FightForm);
    cm_FightForm = 0;
    cm_ClearMarks();
    cm_ClosePortraits();
}
/* 0x4d6ba0, the action of one fighter (f): the target's health is set; a blow (not with the pot 0x174 or the bomb
   0x176) by a fighter that is idle or walking / standing (action 0x2d / 0x3a) shouts the attack (1 in 10), takes the
   damage off the target when it lands and queues the strike: "kampf/schlagen1" without a weapon, else cm_HitAction
   with the weapon's animation (speed 1.4; a landing blow of the weapons 0x16e / 0x172 ends the action), then a pause.
   The pot queues cm_LayBombAction (the thrower's own fight places it at the free cell near him, cm_SendObjCell), the
   bomb cm_ThrowBombAction with the throw sound; both end the action. */

void cm_FighterAct(CMFIGHTSLOT *f)
{
    struct D3HMAP *h = d3_DefaultScene->hmap;
    AICHAR *c, *tgt, *side;
    AIWEAPON *k;
    CHACTION *x, *ev;
    char *s, *d;
    int dmg, lands, ox, oy;
    float py;

    if (f->active == 0 || f->id == -1)
        return;
    c = (AICHAR *)cm_FindFighter(f->id);
    k = (AIWEAPON *)cm_CharWeapon((char *)c);
    side = (AICHAR *)c->bld;  /* in a fight: the leader of the fighter's side */
    tgt = (AICHAR *)cm_FindFighter(f->target);
    if (tgt != 0)
        tgt->v024 = f->health;
    if (!(k != 0 && (k->id == 0x174 || k->id == 0x176))) {
        if (f->hit != 0 && ((x = c->scene->actions) == 0 || x->type == 0x2d || x->type == 0x3a)) {
            if ((unsigned short)util_RandomU(10) > 8) {
                if (side->netId == cm_FightAttackerC->netId)
                    snd_QueueVoice(c->id, cm_AttackerAttackVoices, "_K_C2C_ANGRIFF", -1, 500);
                else
                    snd_QueueVoice(c->id, cm_DefenderAttackVoices, "_K_C2C_ANGRIFF", -1, 500);
            }
            lands = f->kill;
            dmg = f->damage;
            if (lands != 0)
                tgt->v024 -= dmg;
            if (k == 0)
                ch_QueueAnim(c->scene, "kampf/schlagen1", 1);
            else {
                if (k->id == 0) {
                    if (lands != 0)
                        ev = ch_AddActionCBK(c->scene, (unsigned)&cm_HitAction, 8, 0x2e, 1);
                    else
                        ev = ch_AddActionCBK(c->scene, (unsigned)&cm_HitAction, 2, 0x2e, 1);
                } else if (lands != 0)
                    ev = ch_AddActionCBK(c->scene, (unsigned)&cm_HitAction, 6, 0x2e, 1);
                else
                    ev = ch_AddActionCBK(c->scene, (unsigned)&cm_HitAction, 2, 0x2e, 1);
                ev->hit.speed = 1.4f;
                ev->hit.victim = tgt;
                ev->hit.hitter = c;
                ev->hit.damage = dmg;
                ev->hit.sideId = side->netId;
                for (s = k->anim, d = ev->u.name; (*d++ = *s++) != 0;)
                    ;
                if ((k->id == 0x16e || k->id == 0x172) && lands != 0) {
                    f->id = -1;
                    f->active = 0;
                }
            }
            ch_AddAction(c->scene, 0x3b, cm_Random(0x1e) + 0x8c);
        }
    }
    if (k->id == 0x174 && f->kill != 0) {
        ev = ch_AddActionCBK(c->scene, (unsigned)&cm_LayBombAction, 6, 0x2e, 1);
        ev->pot.victim = tgt;
        ev->pot.thrower = c;
        ev->pot.x = f->x;
        ev->pot.y = f->y;
        strcpy(ev->u.name, "spezial/einhaendig_vom_boden_nehmen");
        if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner) {
            if (!d3_GetHeightAt((struct D3SM *)h, &c->scene->obj->pos, &cm_PickCellX, &cm_PickCellZ, &py))
                return;
            cm_BestCellNear((char *)c, cm_PickCellX, cm_PickCellZ, 8, &ox, &oy);
            cm_SendObjCell(c->scene->obj->name, ox, oy, 0);
            f->id = -1;
            f->active = 0;
        }
    }
    if (k->id == 0x176 && f->hit != 0) {
        ev = ch_AddActionCBK(c->scene, (unsigned)&cm_ThrowBombAction, 5, 0x2e, 1);
        ev->bomb.thrower = c;
        ev->bomb.x = f->x;
        ev->bomb.y = f->y;
        ev->bomb.sideId = side->netId;
        for (s = k->anim, d = ev->u.name; (*d++ = *s++) != 0;)
            ;
        ch_AddAction(c->scene, 0x3b, cm_Random(0x1e) + 0x8c);
        sb_PlaySampleI(cm_BombSounds, "topfwerfen_s", 1, 0x3f, 0x7f, 0, 0);
        f->id = -1;
        f->active = 0;
    }
}
/* 0x4d6f60, stops the actions of the fighter in the fight slot f (id, active): 0x405610 on its scene character */

void cm_FightSlotStop(CMFIGHTSLOT *f)
{
    AICHAR *c;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        if (c != 0)
            ch_StopActions(c->scene);
    }
}
/* 0x4d6f90, the cheer of the winner in the fight slot f: event 0x4d3ac0 with the animation "gestik/jubeln" */

void cm_FightSlotCheer(CMFIGHTSLOT *f)
{
    AICHAR *c;
    CHACTION *ev;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        ev = ch_AddActionCBK(c->scene, (unsigned)&cm_CheerAction, 5, 0x2e, 1);
        ev->cheer.fighter = c;
        strcpy(ev->u.name, "gestik/jubeln");
    }
}
/* 0x4d7000, the fighter slot f picks up the object lying at its place (dummy): the object (0x449690) is marked as
   owned by the character's side (AIOBJ.fightSide), event 0x4d3ae0 */

void cm_FightSlotPickUp(CMFIGHTSLOT *f)
{
    AICHAR *c, *side;
    D3OBJECT *o;
    CHACTION *ev;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        side = (AICHAR *)c->bld;
        ch_StopActions(c->scene);
        o = d3_FindObject(0, 0x40, 0, f->dummy);
        if (o != 0) {
            ((AIOBJ *)o->owner)->fightSide = side->id;
            ev = ch_AddActionCBK(c->scene, (unsigned)&cm_PickUpAction, 5, 0x2e, 1);
            ev->pickUp.fighter = c;
            ev->pickUp.object = o;
            strcpy(ev->u.name, "spezial/einhaendig_vom_boden_nehmen");
        }
    }
}
/* 0x4d70a0, the fighter slot f puts down the object it carries (AICHAR.v1ac) at its place */

void cm_FightSlotPutDown(CMFIGHTSLOT *f)
{
    AICHAR *c;
    D3OBJECT *o, *body;
    CHACTION *a;

    if (f->active != 0 && f->id != -1) {
        c = (AICHAR *)cm_FindFighter(f->id);
        ch_StopActions(c->scene);
        d3_FindObject(0, 0x40, 0, f->dummy);
        o = (D3OBJECT *)c->v1ac;
        if (o != 0) {
            body = c->scene->obj;
            d3_SetWorldPosRot(o, (float *)&body->pos, &body->matrix);
            d3if_Show(o, 1);
            c->v1ac = 0;
        }
        a = ch_AddAction(c->scene, 0x2e, 1);
        strcpy(a->u.name, "spezial/einhaendig_vom_boden_nehmen");
    }
}
/* 0x4d7150, the character in the fight slot f is the winner so far (cm_FightWinner): text 0x11b7 */

void cm_FightSlotWinner(CMFIGHTSLOT *f)
{
    char buf[0x100];
    AICHAR *c;

    c = (AICHAR *)gm_FindChar(f->id);
    cm_FightWinner = (int)c;
    txt_Format(buf, 0x11b7, c->id);
    sv_SetStatusText(buf);
}
/* 0x4d7190, the escape flag of a fighter action f: the escape dummy named by f gets the fighter as its owner (the
   fighter marked fled, text 0x11b8); when the owner changes the old flag is deleted and a new "ub_FLUCHTFLAGGE" object
   (animated, the texture set 'g' + ref54 of the side leader of the kinds 5 .. 7, named "CONQUER_FLAG") is put at the
   dummy's place (the name of the fled bit is a reasoned guess from the flag).
   @deep Read against the original in full; typed (CMFIGHTSLOT with the dummy name, D3OBJECT.flagObj at
   0x204, CHARACTER.flags.fled, AICHAR.ref54) and the fled bit set through the bitfield (or byte ptr
   [eax+0x8d], 4 as the original). Left: one code hunk at 0x4d72cc: the original tests the texture set k
   (mov al,[eax+0x54]; add al,0x67; test al,al; mov [esp+0xc],al; je), ours reuses the add's flags (no
   test) - tried k char / unsigned char / int arithmetic, k = 0 before the if with the test after it, the
   assignment inside the condition, if (k): all compile without the test; the rest is order (the 0x212 /
   0x220 / 0x224 stores) and regs. ins 128 vs 129. */

void cm_PlantEscapeFlag(CMFIGHTSLOT *f)
{
    AICHAR *c = 0;
    D3OBJECT *o, *r;
    AICHAR *side;
    char buf[0x100];
    D3VECTOR4 pos;
    D3VECTOR rot;
    char k;

    if (f->active != 0) {
        o = d3_FindObject(0, 0x40, 0, f->dummy);
        if (f->id != -1) {
            c = (AICHAR *)cm_FindFighter(f->id);
            c->scene->flags.fled = 1;
            txt_Format(buf, 0x11b8, c->id);
            sv_SetStatusText(buf);
        }
        if ((AICHAR *)o->owner != c) {
            if (o->flagObj != 0) {
                d3if_Delete(o->flagObj);
                o->flagObj = 0;
            }
            if (c != 0) {
                rot.x = 0;
                rot.y = 0;
                rot.z = 0;
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&pos);
                r = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, "ub_FLUCHTFLAGGE");
                cm_FlagObjects[cm_FlagObjectCount] = (char *)d3_CreateObject(0, (D3VECTOR *)&pos, &rot, &ch_EmptyName);
                d3t_PlayAnim(r, "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
                side = (AICHAR *)c->bld;  /* in a fight: the leader of the fighter's side */
                k = 0;
                if (side != 0 && side->ref54 != 0 && (side->kind == 5 || side->kind == 6 || side->kind == 7))
                    k = side->ref54 + 'g';
                if (k != 0)
                    d3if_SelectTextureSet(r, (struct D3SURF *)r->info, k, 1);
                sprintf(r->name, "CONQUER_FLAG");
                r->owner = 0;
                r->stateFlags = (r->stateFlags & 0xf7) | 0x44;
                r->kind = 4;
                r->mask = 1;
                o->flagObj = r;
            } else
                o->flagObj = 0;
        }
        o->owner = c;
    }
}
/* 0x4d7360, the attack of the fighter c on the fighter g (rec: c's cutscene record): -1 when c cannot attack (a used-up
   weapon of the kinds 1 / 2, c's actions busy). In range (or with a weapon that is not one of the melee kinds) the hit
   is rolled (random 255 against the weapon's accuracy percent of c's skill sk83) with the damage (dmgMin .. dmgMax
   percent of g's life value, * 1.25 when c's side leader has the item 0x2b9); in the player's own fight a melee attack
   also orders the other side's AI to strike back (cm_OrderAttack, unless an order is pending); the fight command 2 goes
   out (cm_SendFightCommand). Out of range c walks to the free cell near g (cm_SendObjCell), unless c holds its place.
   Returns the send slot, -1 when nothing was sent.
   @deep Read against the original in full. Fixed: the walk takes g's body position (g->scene->obj->pos,
   was g + 0x34), the side-leader bonus is 1.25 (the 0x5ecebc float, was 1.5), the AI order is renewed when
   the pending one's send slot is still busy (cm_GetSlotState != 0, was == 0), the damage is computed in
   the slot record itself, the range test as !(dist > range) in one condition with the melee kinds (walk
   out of line as the original), both exits return the send slot (cm_SendFightCommand / cm_SendObjCell now
   return cm_QueueSend's slot: the original has no xor eax after them). Instruction count equal (273).
   Left: allocation - the original packs ix / iz / py and the fild temporaries into the dead parameter
   slots (rec / c / g at [esp+0x48..0x50], frame 0x34) where ours keeps two of them in the frame (0x3c); g
   in esi and the weapon in edi (ours esi / edi swapped), the random value in esi; the g load moved before
   the rep stosd. */

int cm_Attack(CMCUTINFO *rec, AICHAR *c, AICHAR *g)
{
    struct D3HMAP *h = d3_DefaultScene->hmap;
    CMFIGHTSLOT s;
    AIWEAPON *k;
    AIOBJ *item;
    AICHAR *side;
    CMCUTINFO *other;
    CMCUTENTRY *e;
    int i, n, hit, ix, iz, state;
    unsigned short r;
    float py;

    memset(&s, 0, sizeof(s));
    s.id = c->netId;
    s.active = 2;
    s.target = g->netId;
    if (c->formation == 1 || c->formation == 2 || c->formation == 3)
        s.held = 1;
    k = (AIWEAPON *)cm_CharWeapon((char *)c);
    if (k->kind == 1 || k->kind == 2) {
        item = (AIOBJ *)cm_CharWeaponItem((char *)c);
        if (item != 0 && item->usesLeft <= 0)
            return -1;
    }
    if (!(k->id == 0x154 || k->id == 0x156 || k->id == 0x158 || k->id == 0x16e || k->id == 0x172 || k->id == 0 ||
          k->id == 0x176) ||
        !(cm_CharObjDistXZ((char *)c, (char *)g->scene->obj) > k->range)) {
        if (c->scene->actions != 0)
            return -1;
        r = util_RandomU(0xff);
        side = (AICHAR *)c->bld;  /* in a fight: the leader of the fighter's side */
        if (r > 0xff - (int)(k->accuracy * 0.01 * c->sk83)) {
            n = k->dmgMax - k->dmgMin;
            hit = 1;
            s.damage = (int)((double)(unsigned int)(cm_Random(n) + k->dmgMin) * g->f01c * 0.01);
        } else {
            hit = 0;
            s.damage = 0;
        }
        if (side != 0 && ai_InventoryHas(side, 0x2b9) != 0)
            s.damage = (int)(s.damage * 1.25f);
        s.hit = 1;
        s.kill = hit;
        if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && k->kind == 0) {
            other = 0;
            n = cm_FightRecordT->nChars;
            for (i = 0; i < n; i++) {
                if (cm_FightRecordT->charIds[i] != -1 && rec != &cm_CutInfosT[i]) {
                    other = &cm_CutInfosT[i];
                    break;
                }
            }
            e = (CMCUTENTRY *)cm_CutEntrySlot((char *)other, (char *)g);
            if (e->orderSlot == -1 || cm_GetSlotState(e->orderSlot) != 0) {
                state = e->state;
                e->orderSlot = -1;
                if (state == 0) {
                    item = (AIOBJ *)cm_CharWeaponItem((char *)g);
                    if (item != 0 && item->usesLeft > 0)
                        e->orderSlot = cm_OrderAttack((char *)other, (char *)g, (char *)c, 0, 0);
                }
            }
        }
        cm_SendSetField(c->netId, 0x1c8, 4, 0x20000000, 0);
        return cm_SendFightCommand(c, 2, &s);
    }
    if (c->scene->actions != 0 || s.held != 0)
        return -1;
    if (!d3_GetHeightAt((struct D3SM *)h, &g->scene->obj->pos, &ix, &iz, &py))
        return -1;
    if (cm_FindNearCell(ix, iz, &ix, &iz) == 0)
        return -1;
    return cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
}


/* 0x4d76a0, runs the orders of the fighters of the cutscene record rec (its 16 entries; until the fight has a winner):
   an entry whose last order message went out (orderSlot 1 or no longer pending) takes its next step by its state:
   5 stop (command 7), 1 / 7 walk to a cell ("cmb_<netId>" placed near it, command 6 on arrival), 2 attack the fighter
   stateArg (in range the hit is rolled as in cm_Attack, with cm_DefenseFactor for the defender's side; the pot 0x174
   and the bomb 0x176 have their own ranges; ranged weapons first walk to a free cell in reach; the stunning weapons
   0x16e / 0x172 step back after a landing blow), 3 walk to a cell and stand (command 1), 6 carry goods: to the
   dummy (step 0, command 4), to the goods store "dummy_sp_OFF_WARENSAVE" / "dummy_sp_DEF_WARENSAVE" of the side (step
   1) and back (step 2, command 5), 4 follow the owner of a dummy (command 3, or cm_Attack on an enemy in reach),
   8 hold (command 6). A finished order sends the entry back to state 0 (cm_SendCutEntry). A dead fighter with a
   weapon frees its entry and ends the run.
   @deep Reconstructed against the original in full (0x4d76a0..0x4d88b8, all 8 states): the case order of
   the source (5, 1, 2, 3, 6, 4, 7, 8), the hit chance times cm_DefenseFactor for the defender's side, the
   damage without a target 9.0 * n, the AI counter-order with one cm_CharWeaponItem call, the ranged walk
   with cm_BestCellNear's reach (range / cell size * 0.9), the goods-carry state 6 on the bytes 0x20..0x22
   (store cell, step) with cm_SendCutEntryUpdate, the four name buffers (0x30 each, frame 0x138 equal), the
   senders' return values as the order slot (cm_SendCutEntry / cm_SendCutEntryUpdate now return
   cm_QueueSend's slot). Switch tables equal, ins 1437 vs 1434. Left: allocation and block layout - the
   loop keeps slot in edx at the top (ours reloads it from [esp+0x68]), the original cross-jumps the
   identical tails differently (state 2's stop joins state 5's command at 0x4d7789 where ours joins state 5
   into state 7's tail; the four fcompp of the reach test share one fnstsw/and/jne tail at 0x4d7a34, the
   cell-distance path and the pot (0x174) block lie out of line after 0x4d7a3f), the damage's fild/fld
   order in the x != 0 path; the rest regs / slots. (references: M10 at +0x587: our constant 9.0
   (__real@4022000000000000) where the original's field refers to 0.01 at 0x5ece60 (and 2 more), explained:
   the M8 / M10 lines are alignment pairings, not other objects: +0x587 pairs our 9.0 multiply of the x ==
   0 path with the original's 0.01 multiply of the x != 0 path (both paths have both constants, the
   original lays the 9.0 path out of line at 0x4d7cca); +0x920 / +0x9e8 pair the two pushes of
   &cm_PickCellZ / &cm_PickCellX of one d3_GetHeightAt call (both sides push Z then X) across a
   one-instruction shift) */
void cm_RunCutOrders(CMCUTINFO *rec)
{
    D3SM *h = (D3SM *)d3_DefaultScene->hmap;
    CMCUTENTRY m;
    CMCUTENTRY *p;
    AICHAR *c, *x;
    AICHAR *tgt = 0;
    AIWEAPON *k;
    AIOBJ *item;
    CMCUTINFO *other;
    CMCUTENTRY *e;
    D3OBJECT *o;
    D3VECTOR4 q;
    char *nm;
    char name2[0x30];
    char name3[0x30];
    char name1[0x30];
    char name7[0x30];
    int slot, i, n, res, b, hit, ix, iz, st, cx, cz;
    unsigned short r;
    float py;

    for (slot = 0; slot < 16; slot++) {
        if (cm_FightWinner != 0)
            return;
        p = &rec->entries[slot];
        if (p != 0)
            m = *p;
        if (p->state == 0 || p->charId == -1)
            continue;
        if (p->state == 5)
            c = (AICHAR *)gm_FindChar(p->charId);
        else
            c = (AICHAR *)cm_FindFighter(p->charId);
        k = (AIWEAPON *)cm_CharWeapon((char *)c);
        if ((c == 0 || c->health == 0) && k != 0) {
            p->charId = -1;
            p->state = 0;
            return;
        }
        switch (p->state) {
        case 5:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            m.state = 0;
            m.charId = c->netId;
            cm_SendFightCommand(c, 7, (CMFIGHTSLOT *)&m);
            res = cm_SendCutEntry(rec->playerId, &m);
            goto done;
        case 1:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            if (p->b0c == 0) {
                if (cm_FindNearCell(p->stateArg, p->x, &ix, &iz) == 0)
                    break;
                sprintf(name1, "cmb_%i", c->netId);
                p->b0c = 1;
                cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                res = cm_SendObjCell(name1, ix, iz, 0);
                goto done;
            }
            d3sm_GetVertex(h, p->stateArg, p->x, (D3VECTOR *)&q);
            if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 30.0f))
                break;
            m.charId = c->netId;
            m.state = 0;
            cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
            res = cm_SendCutEntry(rec->playerId, &m);
            goto done;
        case 2:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            x = (AICHAR *)cm_FindFighter(p->stateArg);
            if (x != 0 && (x->health == 0 || x->scene->obj->type == 1 || c == 0 || c->health == 0)) {
                m.state = 0;
                m.charId = c->netId;
                cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                res = cm_SendCutEntry(rec->playerId, &m);
                goto done;
            }
            if (k->kind == 1 || k->kind == 2) {
                item = (AIOBJ *)cm_CharWeaponItem((char *)c);
                if (item != 0 && item->usesLeft <= 0) {
                    m.charId = c->netId;
                    m.state = 0;
                    res = cm_SendCutEntry(rec->playerId, &m);
                    goto done;
                }
            }
            b = 1;
            if (k->id != 0x174) {
                if (k->id == 0x176) {
                    if (x != 0) {
                        if (cm_CharObjDistXZ((char *)c, (char *)x->scene->obj) > k->range)
                            b = 0;
                        if (p->b0c == 0 && cm_CharObjDistXZ((char *)c, (char *)x->scene->obj) < k->range * 0.1)
                            b = 0;
                    } else {
                        if (cm_CharCellDistXZ((char *)c, p->x, p->z) > k->range)
                            b = 0;
                        if (p->b0c == 0 && cm_CharCellDistXZ((char *)c, p->x, p->z) < k->range * 0.1)
                            b = 0;
                    }
                } else if (x != 0) {
                    if (cm_CharObjDistXZ((char *)c, (char *)x->scene->obj) > k->range)
                        b = 0;
                } else if (cm_CharCellDistXZ((char *)c, p->x, p->z) > k->range)
                    b = 0;
            } else {
                b = 0;
                if (p->turn == 0 && p->b0c == 0) {
                    if (cm_FindNearCell(p->x, p->z, &ix, &iz) != 0) {
                        p->x = ix;
                        p->z = iz;
                        cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                    }
                    p->b0c = 1;
                } else if (c->scene->actions == 0 && p->b0c != 0) {
                    d3sm_GetVertex(h, p->x, p->z, (D3VECTOR *)&q);
                    b = d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 120.0f) != 0;
                }
            }
            if (b != p->turn || c->scene->actions == 0) {
                p->turn = b;
                if (b != 0) {
                    if (k->kind == 2)
                        hit = 1;
                    else {
                        r = util_RandomU(0xff);
                        if ((AICHAR *)c->bld == cm_FightDefenderC)
                            st = (int)(k->accuracy * 0.01 * c->sk83 * cm_DefenseFactor);
                        else
                            st = (int)(k->accuracy * 0.01 * c->sk83);
                        hit = r > 0xff - st;
                    }
                    if (hit != 0) {
                        n = cm_Random(k->dmgMax - k->dmgMin) + k->dmgMin;
                        if (x != 0)
                            p->damage = (int)((double)(unsigned int)n * x->f01c * 0.01);
                        else
                            p->damage = (int)((double)(unsigned int)n * 9.0);
                    } else
                        p->damage = 0;
                    p->kill = hit;
                    if (hit != 0 && c->bld != 0 && ai_InventoryHas(c->bld, 0x2b9) != 0)
                        p->damage = (int)(p->damage * 1.25f);
                    if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner && k->kind == 0 && x != 0) {
                        other = 0;
                        n = cm_FightRecordT->nChars;
                        for (i = 0; i < n; i++) {
                            if (cm_FightRecordT->charIds[i] != -1 && rec != &cm_CutInfosT[i]) {
                                other = &cm_CutInfosT[i];
                                break;
                            }
                        }
                        e = (CMCUTENTRY *)cm_CutEntrySlot((char *)other, (char *)x);
                        if (e->orderSlot == -1 || cm_GetSlotState(e->orderSlot) != 0) {
                            st = e->state;
                            e->orderSlot = -1;
                            if (st == 0) {
                                item = (AIOBJ *)cm_CharWeaponItem((char *)x);
                                if (item != 0 && item->usesLeft > 0) {
                                    cm_SendSetField(c->netId, 0x1c8, 4, 0x20000000, 0);
                                    e->orderSlot = cm_OrderAttack((char *)other, (char *)x, (char *)c, 0, 0);
                                }
                            }
                        }
                    }
                }
            }
            if (k->kind == 0) {
                if (p->turn != 0)
                    goto strike;
                if (c->scene->actions == 0 && p->held == 0) {
                    if (!d3_GetHeightAt(h, &x->scene->obj->pos, &ix, &iz, &py))
                        break;
                    p->x = ix;
                    p->z = iz;
                    sprintf(name2, "cmb_%i", c->netId);
                    p->orderSlot = cm_SendObjCell(name2, ix, iz, x->scene->obj->name);
                }
            } else if (k->kind == 1 || k->id == 0x176) {
                if (p->turn != 0)
                    goto strike;
                if (c->scene->actions == 0 && p->held == 0) {
                    if (!d3_GetHeightAt(h, &c->scene->obj->pos, &cx, &cz, &py))
                        break;
                    if (x != 0 && !d3_GetHeightAt(h, &x->scene->obj->pos, &p->x, &p->z, &py))
                        break;
                    if (cm_BestCellNear((char *)c, p->x, p->z, (int)(k->range / h->cell.x * 0.9), &ix, &iz) != 0)
                        p->orderSlot = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                }
            }
            if (p->turn != 0) {
            strike:
                if (c->scene->actions == 0) {
                    if (k->kind == 2 && p->kill != 0) {
                        cm_SendSetField(c->netId, 0x1c8, 4, 0x20000000, 0);
                        cm_SendFightCommand(c, 2, (CMFIGHTSLOT *)p);
                        m.charId = c->netId;
                        m.state = 0;
                        if (!d3_GetHeightAt(h, &c->scene->obj->pos, &cm_PickCellX, &cm_PickCellZ, &py))
                            break;
                        if (cm_BestCellNear((char *)c, cm_PickCellX, cm_PickCellZ, 0xc, &ix, &iz) != 0)
                            p->orderSlot = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                        p->orderSlot = cm_SendCutEntry(rec->playerId, &m);
                    } else {
                        cm_SendSetField(c->netId, 0x1c8, 4, 0x20000000, 0);
                        p->orderSlot = cm_SendFightCommand(c, 2, (CMFIGHTSLOT *)p);
                    }
                }
            }
            if ((k->id == 0x16e || k->id == 0x172) && p->kill != 0) {
                if (!d3_GetHeightAt(h, &c->scene->obj->pos, &cm_PickCellX, &cm_PickCellZ, &py))
                    break;
                if (cm_BestCellNear((char *)c, cm_PickCellX, cm_PickCellZ, 0xc, &ix, &iz) != 0)
                    p->orderSlot = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                m.charId = c->netId;
                m.state = 0;
                res = cm_SendCutEntry(rec->playerId, &m);
                goto done;
            }
            break;
        case 3:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            if (p->b0c == 0 && !d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 50.0f) &&
                cm_FindNearCell(p->stateArg, p->x, &ix, &iz) != 0) {
                sprintf(name3, "cmb_%i", c->netId);
                p->b0c = 1;
                p->orderSlot = cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                p->orderSlot = cm_SendObjCell(name3, ix, iz, 0);
            }
            d3sm_GetVertex(h, p->stateArg, p->x, (D3VECTOR *)&q);
            if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 50.0f))
                break;
            cm_SendFightCommand(c, 1, (CMFIGHTSLOT *)&m);
            m.state = 0;
            m.charId = c->netId;
            res = cm_SendCutEntry(rec->playerId, &m);
            goto done;
        case 6:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            if (d3_FindObject(0, 0x40, 0, p->dummy) == 0) {
                cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                m.charId = c->netId;
                m.state = 0;
                p->orderSlot = cm_SendCutEntry(rec->playerId, &m);
            }
            if (p->storeStep == 0) {
                if (c->scene->actions == 0) {
                    p->b0c = 1;
                    cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                    if (cm_FindNearCell(p->storeX, p->storeZ, &ix, &iz) == 0)
                        break;
                    res = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                    goto done;
                }
                d3sm_GetVertex(h, p->storeX, p->storeZ, (D3VECTOR *)&q);
                if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 50.0f))
                    break;
                cm_SendFightCommand(c, 4, (CMFIGHTSLOT *)&m);
                m.storeStep = 1;
                res = cm_SendCutEntryUpdate(rec->playerId, &m);
                goto done;
            } else if (p->storeStep == 1) {
                if (c->scene->actions != 0)
                    break;
                if ((AICHAR *)c->bld == cm_FightAttackerC)
                    nm = "dummy_sp_OFF_WARENSAVE";
                else
                    nm = "dummy_sp_DEF_WARENSAVE";
                o = d3_FindObject(0, 0x100, 0, nm);
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&q);
                if (!d3_GetHeightAt(h, (D3VECTOR *)&q, &ix, &iz, &py))
                    break;
                if (cm_FindNearCell(ix, iz, &ix, &iz) == 0)
                    break;
                m.storeX = ix;
                m.storeZ = iz;
                m.storeStep = 2;
                cm_SendCutEntryUpdate(rec->playerId, &m);
                res = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                goto done;
            } else if (p->storeStep == 2) {
                d3sm_GetVertex(h, p->storeX, p->storeZ, (D3VECTOR *)&q);
                if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 50.0f))
                    break;
                cm_SendFightCommand(c, 5, (CMFIGHTSLOT *)&m);
                m.charId = c->netId;
                m.state = 0;
                res = cm_SendCutEntry(rec->playerId, &m);
                goto done;
            }
            break;
        case 4:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            o = d3_FindObject(0, 0x40, 0, p->dummy);
            if (o != 0) {
                tgt = (AICHAR *)o->owner;
                d3_PointToWorld(o, &o->pos, (D3VECTOR *)&q);
                if (tgt == 0 || (cm_OtherBuilding((char *)c, (char *)tgt) != 0 &&
                                 (tgt->health == 0 || !d3_VecEqual((D3VECTOR *)&q, &tgt->scene->obj->pos, 49.5f)))) {
                    if (p->b0c == 0 && !d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 49.5f) &&
                        cm_FindNearCell(p->cellX, p->cellZ, &ix, &iz) != 0) {
                        p->b0c = 1;
                        p->cellX = ix;
                        p->cellZ = iz;
                        cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                        res = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                        goto done;
                    }
                    if (p->b0c == 1 && c->scene->actions == 0 &&
                        !d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 49.5f)) {
                        p->b0c = 0;
                        break;
                    }
                    if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 49.5f))
                        break;
                    cm_SendFightCommand(c, 3, (CMFIGHTSLOT *)&m);
                    m.state = 0;
                    m.charId = c->netId;
                    res = cm_SendCutEntry(rec->playerId, &m);
                    goto done;
                }
            }
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)&q);
            if (cm_OtherBuilding((char *)c, (char *)o->owner) != 0 && tgt->health > 0 &&
                d3_VecEqual((D3VECTOR *)&q, &tgt->scene->obj->pos, 49.5f)) {
                res = cm_Attack(rec, c, (AICHAR *)o->owner);
                goto done;
            }
            if (tgt->health > 0)
                break;
            m.charId = c->netId;
            m.state = 0;
            res = cm_SendCutEntry(rec->playerId, &m);
            goto done;
        case 7:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            if (p->b0c == 0) {
                if (cm_FindNearCell(p->stateArg, p->x, &ix, &iz) == 0)
                    break;
                sprintf(name7, "cmb_%i", c->netId);
                p->b0c = 1;
                cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                res = cm_SendObjCell(name7, ix, iz, 0);
                goto done;
            }
            d3sm_GetVertex(h, p->stateArg, p->x, (D3VECTOR *)&q);
            if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 30.0f))
                break;
            m.charId = c->netId;
            m.state = 0;
            cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
            res = cm_SendCutEntry(rec->playerId, &m);
            goto done;
        case 8:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            if (p->b0c != 0)
                break;
            p->b0c = 1;
            res = cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
            goto done;
        }
        cm_OrderTicks += 0x1f;
        continue;
    done:
        p->orderSlot = res;
        cm_OrderTicks += 0x1f;
    }
}
/* 0x4d88c0, the fight loop: starts one of two fight tracks, then runs until the fight ends (cm_FightWinner / cm_FightSettled): the pause
   toggle (key 0x39 = space) with the character animations and the "pause" text, the frame of the UI, and every 0x15e timer calls
   the characters' fight-AI (0x4d2f80) and the fighters' commands (0x4d76a0). Ends with the end-of-fight code 0x4d6b70. */

void cm_FightLoop(char *a)
{
    unsigned int t;
    int i, r;
    AICHAR *o;

    t = tb_Calls - 0x15e;
    r = util_RandomU(100);
    cm_CollectSceneChars();
    cm_BuildFightUI(a);
    cm_FightAmbient = 0;
    if (r < 0x32)
        cm_FightAmbient = msx_PlayAmbient("Athmos\\Kampfathmo_1.mp3", 0xb);
    else
        cm_FightAmbient = msx_PlayAmbient("Athmos\\Kampfathmo_2.mp3", 0xb);
    cm_FightPaused = 0;
    while (sv_RunFrame(cm_FightLoop, 0xfd6c6) != 0 && cm_FightSettled == 0) {
        if ((ai_GameFlags & 4) == 0 && kb_Last == 0x39) {
            cm_FightPaused = cm_FightPaused == 0;
            for (i = 0; i < 32; i++) {
                o = ai_SceneCharsT[i];
                if (o != 0 && CM_CHR(o)->scene == d3_DefaultScene)
                    ch_PauseAnim(CM_CHR(o), cm_FightPaused);
            }
            sv_SetStatusText(&ch_EmptyName);
        }
        if (cm_FightPaused != 0)
            sv_SetStatusText(txt_FightPaused);
        else
            cm_FightFrame(a);
        if (cm_FightWinner == 0 && cm_FightSettled == 0 && cm_FightPaused == 0) {
            if (tb_Calls > t + 0x1a4) {
                for (i = 0; i < cm_FightRecordT->nChars; i++) {
                    if (cm_FightRecordT->charIds[i] != -1) {
                        o = (AICHAR *)gm_FindChar(cm_FightRecordT->charIds[i]);
                        if (o != 0 && o->kind != 6 && o->kind != 7)
                            cm_GiveFightOrders((int)o);
                    }
                }
                t = tb_Calls;
            }
            if (((AICHAR *)cm_CutChar)->netId == cm_FightOwner) {
                cm_CheckFightWinner(cm_FightRecordT);
                for (i = 0; i < cm_FightRecordT->nChars; i++) {
                    if (cm_FightRecordT->charIds[i] != -1)
                        cm_RunCutOrders(&cm_CutInfosT[i]);
                }
            }
        }
        if (cm_FightWinner != 0 || cm_FightSettled != 0)
            sv_PopRequest = 1;
    }
    cm_CloseFightUI();
}
/* 0x4d8b10, the fight member callback (kind 3, a: the member record): on the first run of the scene party only the fight
   result (cm_FightResult), on the later runs the fight loop
   @deep read whole function against the original: same test of cm_ScenePartyRuns, same two callees with
   the incoming a, same layout. Only difference: the original calls cm_FightResult with push
   [esp+4]/call/add esp,4/ret where ours tail-jumps (jmp cm_FightResult); the second branch is a tail jmp
   on both sides. Tried if/else, reversed condition (wrong order), int-cast callee, unprototyped cast,
   int-returning caller: all still tail-call. Hunk: +0x19 '- jmp X' vs '+ mov eax,[esp+4]; push eax; call
   X; add esp,4; ret'. A VC6 tail-call decision, no meaning difference. */

void cm_FightMember(char *a)
{
    if (cm_ScenePartyRuns == 0) {
        cm_FightResult(a);
        return;
    }
    cm_FightLoop(a);
}
/* 0x4d8b30, clears the 16 ints at 0x92ca80 */

void cm_ClearSceneChars()
{
    memset(cm_SceneChars, 0, 0x40);
}
/* 0x4d8b50, shows the fight time (d3_Time minus the start 0x6f0da4, 14 ms per tick) as "mm : ss : ms ms" in a box at the top */

void cm_DrawFightTime()
{
    char buf[128];

    d2_SetFont(0x54);
    sprintf(buf, "%2i : %2i : %3i ms", (d3_Time * 14 - cm_FightStart * 14) / 60000, (d3_Time * 14 - cm_FightStart * 14) / 1000 % 60,
            (d3_Time * 14 - cm_FightStart * 14) % 1000);
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
    ((int (*)(short, short, int, int))d2_WinOpenPic)(0x8c, (short)(0x44 - (short)o->x2c * 10), 0x4df, d2_LastWindow);
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
   to vote on, and left at the end of the column.
   @deep Rebuilt whole from the original (1985 instructions), every block read against it: the array
   initialisers in the original's order, the voter/observer member setup, the deposition loop (answers,
   voices, remarks, relations as an if/else chain, office messages) and the election loop (applicant scan
   by index, the -1/0 fill loop, single/drawn/voted winner, tie draw). No call, store, reference or literal
   differs (diffclass: no differs/ref lines; ours 1989 vs 1985). Left is allocation only: frame 0x904 vs
   0x900 (one more spilled scalar; tie/win spilled to [esp+0x24]/[esp+0x14] where the original keeps them
   in edx/esi because ours caches bankElect in edi: hunks +0x1845..+0x1870), the holders loop keeps 0 in
   edi where the original keeps 1 (+0x48c: '- xor edi,edi' / '+ mov edi,1'), and the slot renaming that
   follows from the frame shift. */

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
    int npresent, k, kind, n, nh, m, left, nvoters, shown, step, napp, best, win, tie, r, i, j, e, slot;

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
        nh = 0;
        for (i = 0; i < n; i++) {
            c = (AICHAR *)gm_FindChar(offices[i].owner);
            if (c != 0 && c->heldRounds == 0) {
                holderIdx[nh] = i;
                holders[nh] = c;
                nh++;
            }
        }
        sv_SyncText(&ch_EmptyName);
        if (amt_IsColumnDecided(kind) == 0) {
            if (a->host == ai_CharsT[ai_PlayerId].netId)
                amt_ElectOffices(kind, n, offices);
            continue;
        }
        for (i = n; i < 6; i++)
            holders[i] = 0;
        /* the members: the voters first, then the other present characters as observers */
        for (i = 0; i < nh; i++) {
            a->members[i] = holders[i]->netId;
            cm_CutInfosT[i].playerId = holders[i]->netId;
            cm_CutInfosT[i].session.observer = 0;
            cm_CutInfosT[i].ready = 0;
            cm_CutInfosT[i].valid = 1;
        }
        m = nh;
        for (i = 0; i < npresent; i++) {
            for (j = 0; j < nh; j++)
                if (cm_CutInfosT[j].playerId == present[i]->netId)
                    goto member;        /* already a voter */
            a->members[m] = present[i]->netId;
            cm_CutInfosT[m].playerId = present[i]->netId;
            cm_CutInfosT[m].ready = 0;
            cm_CutInfosT[m].valid = 1;
            cm_CutInfosT[m].session.observer = 1;
            m++;
        member:;
        }
        a->used = (unsigned char)m;
        for (; m < 16; m++) {
            a->members[m] = -1;
            cm_CutInfosT[m].playerId = -1;
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
            nvoters = 0;
            for (i = 0; i < nh; i++)
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
                step = (0x320 - d2_Pics[0x5da].w * nvoters) / (nvoters + 1);
                shown = 0;
                for (i = 0; i < nh; i++) {
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
                s = &cm_CutInfosT[i];
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
                if (a->host == ai_CharsT[ai_PlayerId].netId) {
                    for (i = 0; i < nh; i++) {
                        s = &cm_CutInfosT[i];
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
                if (a->host == ai_CharsT[ai_PlayerId].netId) {
                    for (i = 0; i < nh; i++) {
                        s = &cm_CutInfosT[i];
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
                c = &ai_CharsT[i];
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
                if (a->host == ai_CharsT[ai_PlayerId].netId) {
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
                if (a->host == ai_CharsT[ai_PlayerId].netId) {
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
                    s = &cm_CutInfosT[i];
                    if (s->session.answer == -1)
                        continue;
                    for (e = 0; e < napp; e++) {
                        if (elect.ids[e] != s->session.answer)
                            continue;
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
                best = -1;
                win = -1;
                tie = 0;
                for (e = 0; e < napp; e++) {
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
                        if (a->host == ai_CharsT[ai_PlayerId].netId) {
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
                    if (a->host == ai_CharsT[ai_PlayerId].netId) {
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
    if (a->host == ai_CharsT[ai_PlayerId].netId)
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
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 1);
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
    res = ai_TrustedOfFourB(office, b, ids, proposed);
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
    dlg = d2_WinOpenSlider((d2_LastWindowPtrT->w - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
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
    } else {
        if (m->mode != 2)
            return;
        if (amt_CanTakeOffice((char *)c, m->kind) == 0)
            return;
        res = cm_AskElectVote(m->office, c, m->ids, &click);
        r->session.click = click;
    }
    r->session.answer = res;
    sprintf(r->text, ch_EmptyName);
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
    } else {
        if (m->mode != 2)
            return;
        if (amt_CanTakeOffice((char *)c, m->kind) == 0)
            return;
        res = ai_TrustedOfFourB(m->office, c, m->ids, &click);
        r->session.click = click;
    }
    r->session.answer = res;
    sprintf(r->text, ch_EmptyName);
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

extern char ai_Unk60de4c;
extern char ai_Unk60dd68;
extern char ai_Unk60dd24;
#define gv_PassSentenceK ((void (*)())gv_PassSentence)  /* 0x4f4460 (gv.h: two parameters): @original cm_RunTrial pushes five arguments (the accused, the sentence, the judge, the two assessors), a call without a prototype */
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
   @deep Rebuilt whole from the original (2340 instructions), every region read against it: the plan record
   init, the crime grouping and reading loops, the plea, the torture (instrument tables, rolls,
   confession), the vote of judge and assessors, the verdict, fees, relations and the bank cleanup
   (including the original's double unload of bankF2). The callees, arguments, literals and statement order
   agree (ours 2333 vs 2340). Left is allocation and layout: the frame 0x784 vs 0x760 and its slot renaming
   (slots 750), register choice (regs 134), and two clamps VC6 lowers differently: dec.cur to 0..4 (ours
   inline jmp/mov eax,4 at +0xaa, the original out of line at +0x24f) and the instrument 0..6 (ours
   setle/dec/and at +0x11ed, the original jle/jmp blocks). (references: M8 at +0x1297: ours refers to
   sprintf (0x5de652), the original's field to 0x4e2bd0 (cm_SetViewEnabled) (and 2 more), explained: the
   three M8 lines (+0x1297 sprintf|cm_SetViewEnabled, +0x1648, +0x1a4b) are alignment crossings: the same
   calls in the same order on both sides, paired one call apart where the surrounding pushes are scheduled
   differently; the proof stops only on the crime id array's frame offset (frame+0x154 vs +0x130)) */

void cm_RunTrial(G28REC *m)
{
    AIDEC dec;
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
    gv_GetTableRecord(0, (int *)&dec);
    k = dec.cur;
    if (k < 4) {
        if (k <= 0)
            k = 0;
    } else
        k = 4;
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
        list[nlist] = ((CRIME *)ai_Crimes)[crimeIds[nlist]].id;
    for (i = nlist; i < 32; i++)
        list[i] = -1;
    for (i = 0; i < ncrimes; i++) {
        if (groups[i] == -1)
            continue;
        groups[i] = 1;
        crime = ((CRIME *)ai_Crimes)[crimeIds[i]];
        for (j = i + 1; j < ncrimes; j++) {
            if (groups[j] == -1)
                continue;
            other = ((CRIME *)ai_Crimes)[crimeIds[j]];
            if (crime.law == other.law) {
                groups[j] = -1;
                groups[i]++;
            }
        }
    }
    for (i = 0; i < ncrimes; i++) {
        char text[0x100];

        crime = ((CRIME *)ai_Crimes)[crimeIds[i]];
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
    if (m->host == ai_CharsT[ai_PlayerId].netId)
        gv_RetireCrimeIds(c1->netId, nlist, list);
    cm_StartEventArg("cutscenes\\prozess\\prozess-angeklagter.esc", (int)"1");
    for (i = 0; i < m->used && cm_CutInfosT[i].playerId != plan.ids[1]; i++)
        ;
    plea = cm_CutInfosT[i].session.answer;
    plan.amount = cm_CutInfosT[i].session.amount;
    cm_SetViewEnabled(cm_TextBox, 1);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    plan.mode = 4;
    if (plea == 1) {
        plan.plea = plea;
        plan.score = sum - gm_Liking(c1->id, c2->id, 1) * sum * 0.002f;
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
        for (i = 0; i < m->used && cm_CutInfosT[i].playerId != plan.ids[0]; i++)
            ;
        k = cm_CutInfosT[i].session.answer;
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

            if (m->host == ai_CharsT[ai_PlayerId].netId) {
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
            for (i = 0; i < m->used && cm_CutInfosT[i].playerId != plan.ids[0]; i++)
                ;
            cm_WaitEvent(ev);
            ev2 = cm_StartEventArg("cutscenes\\prozess\\prozess-klaeger.esc", (int)"1");
            sel = cm_CutInfosT[i].session.answer & 0xf;
            if (sel < 6) {
                if (sel > 0)
                    plan.instrument = sel;
                else
                    plan.instrument = 0;
            } else
                plan.instrument = 6;
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
            for (i = 0; i < m->used && cm_CutInfosT[i].playerId != plan.ids[5]; i++)
                ;
            plan.answer = cm_CutInfosT[i].session.answer;
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
    for (i = 0; i < m->used && cm_CutInfosT[i].playerId != plan.ids[2]; i++)
        ;
    bankE = cm_LoadVoiceBank((int)"PROZESS_6_ABSTIMMUNG_ERGEBNISSE.sbf", 0);
    cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-3.esc", (int)ch_EmptyName);
    cm_SetViewEnabled(cm_TextBox, 1);
    d2_SetForm(cm_TextBox, 0);
    io_PrintText("$C");
    io_PrintText(0x1528, c2->id);
    cm_WaitTicks(2);
    votes = cm_CutInfosT[i].session.answer;
    cm_QueueVoice(c2->id, bankE, (int)"_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", cm_Random(3) + cm_CutInfosT[i].session.answer * 3, 100);
    io_PrintText(0x152c + votes);
    cm_WaitTicks(2);
    cm_StartEventArg("cutscenes\\prozess\\prozess-richter.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"1");
    cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer2.esc", (int)"1");
    if (plan.ids[3] != -1) {
        k = cm_CutInfosT[i + 1].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-1.esc", (int)ch_EmptyName);
        d2_SetForm(cm_TextBox, 0);
        p = (AICHAR *)gm_FindChar(plan.ids[3]);
        io_PrintText("$N");
        io_PrintText(0x1528, p->id);
        cm_WaitTicks(2);
        io_PrintText(0x152c + k);
        cm_QueueVoice(c3->id, bankE, (int)"_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", cm_Random(3) + cm_CutInfosT[i + 1].session.answer * 3, 100);
        cm_WaitTicks(2);
        votes += cm_CutInfosT[i + 1].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-beisitzer1.esc", (int)"2");
    }
    if (plan.ids[4] != -1) {
        k = cm_CutInfosT[i + 2].session.answer;
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-2.esc", (int)ch_EmptyName);
        d2_SetForm(cm_TextBox, 0);
        p = (AICHAR *)gm_FindChar(plan.ids[4]);
        io_PrintText("$N");
        io_PrintText(0x1528, p->id);
        cm_WaitTicks(2);
        io_PrintText(0x152c + k);
        cm_QueueVoice(c4->id, bankE, (int)"_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", cm_Random(3) + cm_CutInfosT[i + 2].session.answer * 3, 100);
        cm_WaitTicks(2);
        votes += cm_CutInfosT[i + 2].session.answer;
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
        for (i = 0; i < m->used && cm_CutInfosT[i].playerId != plan.ids[2]; i++)
            ;
        sentence = cm_CutInfosT[i].session.answer;
        if ((unsigned)sentence >= 4 && c0->kind == 6 && c1->officeKind != 0)
            ai_CountMissionEvent(c0, 0x13);
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x152f, c1->id);
        io_PrintText(0x1530);
        cm_QueueVoice(c2->id, bankVerdict, (int)"_PROZESS_7_SCHULDIG", c1->sex == 1, 100);
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
        cm_QueueVoice(c2->id, bank8, (int)"_PROZESS_8", c1->sex == 1, 200);
        if (m->host == ai_CharsT[ai_PlayerId].netId)
            gv_PassSentenceK(c1, sentence, c2, c3, c4);
        io_PrintText(0x1531);
        if (m->host == ai_CharsT[ai_PlayerId].netId) {
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
        cm_QueueVoice(c2->id, bank8, (int)"_PROZESS_8", (c0->sex == 1) + 2, 200);
        cm_StartEventArg("cutscenes\\prozess\\prozess-abstimmen-3.esc", (int)ch_EmptyName);
        cm_SetViewEnabled(cm_TextBox, 1);
        d2_SetForm(cm_TextBox, 0);
        io_PrintText("$C");
        io_PrintText(0x152e, c1->id);
        cm_WaitTicks(4);
        io_PrintText("$C");
        io_PrintText(0x1557, plan.rating2c, c0->id);
        cm_WaitTicks(4);
        if (m->host == ai_CharsT[ai_PlayerId].netId) {
            fee = plan.rating2c / 3;
            cm_SendMoveGoodsUnchecked(c2->netId, c0->netId, ai_OwnPlayer, fee);
            cm_SendMoveGoodsUnchecked(c3->netId, c0->netId, ai_OwnPlayer, fee);
            cm_SendMoveGoodsUnchecked(c4->netId, c0->netId, ai_OwnPlayer, fee);
        }
    }
    if (m->host == ai_CharsT[ai_PlayerId].netId) {
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

#define CUT_BOX() d2_WinOpenSlider((d2_LastWindowPtrT->w - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow)  /* the timer slider of a cutscene dialog */
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
    AICHAR *c, *accused;
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
        CM_ASK_YESNO(r, 1, 0)
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
        r->session.answer = util_RandomU(2) & 0xffff;
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
    rot.y = (float)(cm_Random(0x168) * cm_Pi * cm_Inv180);
    b = d3_CreateObject(0, &a->ch->obj->pos, &rot, "ub_Blutlache");
    d3t_PlayAnim(b, "sonstiges\\ub_Blutlache.baf", 0x12);
}
/* 0x4de530, a fighter whose life (0x4fc8e0) is under 0.2 sinks to the ground ("bewegung/zu_boden_sinken"): 1 */

int cm_DuelSinkDown(char *c, int dmg)
{
    AICHAR *ch = (AICHAR *)c;
    CHACTION *o;

    if (gm_HealthRatio(c) < 0.2) {
        d3t_CreateBloodK(ch->scene->obj, 0x40, 0x3e800000, 0x3dcccccd, 0x3e0f5c29, 0x425c0000);
        ch_StopActions(ch->scene);
        o = ch_AddActionCBK(ch->scene, cm_SinkDownAction, 0x12, 0x36, 1);
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
    AICHAR *sh = a->shot.shooter;
    AICHAR *tg = a->shot.target;
    float acc, b;
    int hit, dmg, n;

    d2_SetForm(cm_TextBox, 0);
    acc = (float)gm_GetSkill((char *)sh, 3);
    gm_GetSkill((char *)tg, 3);
    acc = (float)(acc * 0.8);
    if (ai_InventoryHas(sh, 0x16f) != 0)
        acc = (float)(acc * 1.5);
    if (((int)sh == cm_DuelLeft && cm_DuelRightInsultWon != 0) || ((int)sh == cm_DuelRight && cm_DuelLeftInsultWon != 0))
        acc = (float)(acc * 0.66);
    if ((float)cm_RandomFloat() < acc) {
        hit = 1;
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
        cm_ShowDamage((char *)tg, (int)(tg->f01c * 0.01 * dmg), 0);
    } else {
        hit = 0;
        dmg = 0;
    }
    sb_PlaySampleI(0, "Schuss", 1, 0x3f, 0x7f, 0, 0);
    if (cm_DuelSinkDown((char *)tg, dmg) == 0) {
        if (hit != 0) {
            d3t_CreateBloodK(tg->scene->obj, 0x40, 0x3e800000, 0x3dcccccd, 0x3e0f5c29, 0x425c0000);
            if (tg->scene->actions == 0)
                ch_QueueAnim(tg->scene, "kampf/taumeln", 1);
            d2_SetForm(cm_TextBox, 0);
            n = cm_DuelVariant(6);
            io_PrintText(n + 0x1102, tg->id);
            snd_PlayVoice(tg->id, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_TREFFER", n);
            b = (float)(tg->f01c * dmg * 0.01);
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
        act = ch_AddActionCBK(a->scene, cm_DuelShotAction, 6, 0x2e, 1);
        act->shot.target = b;
        act->shot.shooter = a;
        strcpy(act->u.name, "kampf/schiessen_pistole");
        cm_WaitTicks(4);
        return;
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
    int i;

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
    if (m->host != ai_CharsT[ai_PlayerId].netId)
        return;
    amt_SendOfficeMessage(office.idx, 0, 0, 4, 0xff);
    for (i = 0; i < 768; i++) {
        c = &ai_CharsT[i];
        if (c->kind == 6 || c->kind == 7) {
            txt_Format(buf, 0x10c7, w->id, (w->sex != 0) + 0x10c8);
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
   player's host sends the relations of both; then the scene is torn down
   @deep reconstructed from the original's disassembly (all 873 instructions read: the block-scoped scene
   name table, the cheat round (BETRUG marks, the seconds' shots), the rounds, the result / damage / office
   texts, the teardown); instruction counts equal (873/873), the frame equal (0xa08). Left: two hunks of
   allocation and scheduling only. 0x4df899: the original computes (w->sex != 0) + 0x10c8 in eax with cl as
   the byte temp (mov cl,[edi+9]; xor eax,eax; test cl,cl; setne al), ours in ecx with al (mov al,[edi+9];
   xor ecx,ecx; ...), the same pushes after a renaming. 0x4df935: before the host test the original
   computes the ai_CharsT index first and reloads m from [esp+0xa1c] after add esp,4, ours loads m
   ([esp+0xa20]) before the add (same compare, netId == m->host). The rest are regs hunks. The prover stops
   at the scene name table's initializer (a 4-byte store across two cells). */

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
        r = util_RandomU(3) - 5;
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
        io_PrintText(cm_TextNewline);
        snd_PlayVoice(r, snd2, "_DUELL_INTRO_C2_SEKUNDANTEN", n);
        act = ch_AddActionCBK(ca.scene, cm_DuelShotAction, 6, 0x2e, 1);
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
        act = ch_AddActionCBK(cb.scene, cm_DuelShotAction, 6, 0x2e, 1);
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
            if (amt_FindOffice((char *)w, &kind, &office) != 0 && m->host == ai_CharsT[ai_PlayerId].netId) {
                amt_SendOfficeMessage(office.idx, 0, 0, 4, 0xff);
                for (i = 0; i < 768; i++) {
                    AICHAR *c = &ai_CharsT[i];

                    if (c->kind == 6 || c->kind == 7) {
                        txt_Format(text, 0x10c7, w->id, (w->sex != 0) + 0x10c8);
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
    if (ai_CharsT[ai_PlayerId].netId == m->host) {
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
/* 0x4dfa20, the duel's aftermath (the fourth callback of the duel event, ai_Unk4e5730 kind 4) for the duellists
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
        if (gm_HealthRatio((short *)a) <= ai_FloatZero) {
            if (b->kind != 6 && b->kind != 7)
                return 0;
            he_ShowNews(ai_CharsT[b->id].netId, -1, txt_DuelDeathNews, 0, 0x5ec, 0);
            return 0;
        }
        if (gm_HealthRatio((short *)b) <= ai_FloatZero) {
            if (a->kind != 6 && a->kind != 7)
                return 0;
            he_ShowNews(ai_CharsT[a->id].netId, -1, txt_DuelDeathNews, 0, 0x5ec, 0);
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
        he_ShowNews(ai_CharsT[b->id].netId, -1, txt_DuelHeldNews, 0, 0x5ec, 0);
    if (a->kind == 6 || a->kind == 7)
        he_ShowNews(ai_CharsT[a->id].netId, -1, txt_DuelHeldNews, 0, 0x5ec, 0);
    return 0;
}
/* 0x4dfcb0, the player's choice of the duel round as a dialog ("cutscenes\\duell", text 0x10de with cm_CutChar's
   name, the timer slider): r->duel.round = 1 (the cheat) for the answer 0, 0 for the answer 1 and when the time runs
   out; r's text cleared */

void cm_DuelRoundDialog(CMCUTINFO *r)
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
    other = b;
    if (a->netId != ((AICHAR *)cm_CutChar)->netId)
        other = a;
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

void cm_DuelAskPlayer(CMCUTINFO *r)
{
    if (cm_ScenePartyRuns == 0)
        cm_DuelRoundDialog(r);
    else
        cm_DuelChooseRoundR(r);
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
/* 0x4e0040, the execution's aftermath (the fourth callback of the execution event, ai_Unk4e5730 kind 5 with the scene
   0x4e01e0) for the delinquent m->members[0]: unless m->zero7d is set, the delinquent's player (kinds 6 / 7) gets the note
   text 0x11c5 + txt_ExecNoteTail; then every player's character among the other members gets the note text 0x11c8 with
   the delinquent's name (cm_SendPartyNote). The loop is written in both branches, as the original has it twice.
   Returns 0 when the delinquent is not found, else 1 */

int cm_ExecutionAftermath(G28REC *m)
{
    char buf[0x200];
    AICHAR *c, *o;
    int i;

    if (m->zero7d == 0) {
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
#define CUT_NAMEREC(c) ((c) != 0 ? (char *)gm_CharNameRecord((c)->id) : (char *)gm_NameRecord((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1))  /* a character's name record, a random one for none */
/* 0x4e01e0, one execution (the scene callback of the execution event, ai_Unk4e5730 kind 5) of the delinquent
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
                    if ((ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7) && ai_CharsT[i].netId != a->netId &&
                        ai_CharsT[i].netId != b->netId)
                        he_ShowNews(ai_CharsT[i].netId, -1, buf, 0, 0x5fe, "_PA_WERBUNG_EVENT_NACHRICHT_HOCHZEIT_M1_00");
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
   cm_EndWedding (its result returned); 0 when one of the couple is not found */

int cm_RunWedding(G28REC *m)
{
    int st[4];
    char buf[0x100];
    AICHAR *a, *b, *t;
    char *na, *nb;
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
    na = (char *)gm_CharNameRecord(a->id) + 4;
    nb = (char *)gm_CharNameRecord(b->id) + 4;
    if (a->sex != 0) {
        t = a;
        a = b;
        b = t;
        sprintf(buf, "%s %s %i", nb, na, 1);
    } else
        sprintf(buf, "%s %s %i", na, nb, 1);
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
   name box's text is sent as the child's first name (16 bytes at +0x30). Nothing when one of them is not found
   @deep read whole against the original 0x4e0f70..0x4e1212 and fixed: void (no return value), the name
   buffer 16 bytes and sent as the first name (cm_AddFieldDelta(1, 0x10, 0x30, name)), the voice calls with
   the bank and 5 arguments, the name-box loop (Enter or no focused input -> read the box), the text
   0x1aae/0x1aaf as if/else. The only code hunks left (+0xb8..+0x10b) are argument scheduling of
   cm_QueueVoice(-8, bank, "_GEBURT", n, 0): the original pushes the last 0 before evaluating n (push 0 /
   push 2 hoisted, the four cm_Random calls share one add esp,4 at the join, add esp 0x14 after the call),
   ours evaluates n first (add esp,4 per branch, push 0 after, the stack cleanup merged into the next
   call's add esp 0x20); tried n as a nested ?: inside the argument list and as an if/else before the call:
   identical object. The rest is slots (the prologue saves ebp before esi: '+ push ebp / + push esi / + mov
   esi,[esp+0x20]' vs '- push esi / - mov esi,[esp+0x1c] / - push ebp'), regs (ebx<->edi) and the epilogue
   order */

void cm_RunBirth(G28REC *m)
{
    char name[0x10];
    AICHAR *a, *b;
    int bank, sc, view, box, n, k;

    a = (AICHAR *)gm_FindChar(m->members[0]);
    b = (AICHAR *)gm_FindChar(m->members[1]);
    if (a == 0 || b == 0)
        return;
    n = 8;
    cm_BeginFieldBlock((char *)b, b->netId);
    cm_AddFieldDelta(1, 1, 2, &n);
    cm_SendFieldDeltas();
    cm_DialogFrameFlags = 0x702c6;
    d2_CutsceneActive = 1;
    bank = cm_LoadVoiceBank("GEBURT.sbf", 0);
    cm_PlaySpecialTrack("cd2\\Geburt.mp3");
    cm_LoadEventScene("Geburt.ed3");
    sc = cm_StartScript("cutscenes\\geburt\\enter_Geburt.esc");
    if (cm_RunSceneTicksOrClick(0x1964, 0, 0) == 0) {
        if (a->sex == 0)
            n = b->sex == 0 ? cm_Random(2) : cm_Random(2) + 2;
        else
            n = b->sex == 0 ? cm_Random(2) + 4 : cm_Random(2) + 6;
        cm_QueueVoice(-8, bank, "_GEBURT", n, 0);
        if (cm_RunSceneTicksOrClick(0x3e8, 0, 0) == 0) {
            for (k = cm_Random(4) + 2; k > 0; k--)
                cm_QueueVoice(-8, bank, "_GEBURT_GESCHREI", cm_Random(5), (unsigned short)util_RandomU(0x800) + 0x3e8);
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
    if (gm_CharacterHealth(gm_FindChar(m->members[0])) > ai_FloatZero)
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
   estate is forfeited (gm_ForfeitEstate) and its text shown
   @deep read whole against the original 0x4e1250..0x4e16f3 and rebuilt: void, the voices _TOD_HS 0, 2..6
   with the bank and their delays, the fog fade (sv_RunFrame's result ignored, the click ends it,
   min(t*1/825, ai_FloatOne)), the light scale 1.0f, the bells, the revive branch as revive =
   cm_MessageBox(..) with one family-tree call, the field slots, a 0x1fb8 estate buffer from the frame. One
   instruction differs: +0xf0 '- test eax, eax' vs '+ cmp eax, ebx' (the test of the first
   cm_RunSceneTicksOrClick result; the original compares with ebx, its cached 0). Tried rc = ..; if (rc ==
   0), if ((rc = ..) == 0), rc = 0 / == rc, and a goto: the same test eax,eax. Only VC6's choice of the
   zero register at this compare is left */

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
        if (rc == 0) {
            t0 = d3_Time;
            while (d3_Time < t0 + 0x339 && rc == 0) {
                sv_RunFrame(0, cm_DialogFrameFlags);
                if (dm_L.rup != 0)
                    rc = 1;
                d3_SetFogSet(0, 1, (float)(d3_Time - t0) * (1.0f / 0x339) < ai_FloatOne ? (float)(d3_Time - t0) * (1.0f / 0x339) : 1.0f);
            }
            d3_SetFogSet(0, 1, 1.0f);
            cm_RunSceneTicksOrClick(0x2cec, 0, 0);
        }
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
    char voice[4];
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
    t = &ai_BTypesT[b->type];
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
        sprintf(model, "%s", "buerger3_DICKER");
        speaker = -5;
        sprintf(voice, "%s", "M1");
    } else if (speaker == 1) {
        sprintf(model, "%s", "wirt2_DICKER");
        speaker = -4;
        sprintf(voice, "%s", "M2");
    } else {
        sprintf(model, "%s", "buergerin2_FRAU");
        speaker = -2;
        sprintf(voice, "%s", "W1");
    }
    o = d3_FindObject(0, 0x100, 0, "dummy_AUKTIONATOR");
    if (o != 0)
        d3_PointToWorld(o, &o->pos, (D3VECTOR *)pos);
    auct = (CHARACTER *)cm_CreateChar(model, pos);
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
            persons[i] = (CHARACTER *)cm_CreateChar(buf, pos);
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
    round = 0;  /* then the year of the sale */
    done = 0;   /* then the price paid */
    for (; round < 5 && done == 0; round++) {
        int nbid, top;

        rounds = 10 - round;
        req.countdown = rounds;
        top = 0;
        nbid = 0;
        cm_RunSceneParty(m, &req);
        if (rounds > 6 && cm_Random(3) != 0)
            cm_QueueVoice(cm_Random(4) != 0 ? -4 : -2, bankRemarks, "_VERSTEIGERUNG_KOMMENTARE", cm_Random(6), cm_Random(0x200) + 0x32);
        for (i = 0; i < m->used; i++) {
            if (cm_CutInfosT[i].session.click != 0 && chars[i] != 0) {
                req.bids[i] = cm_CutInfosT[i].session.answer;
                if (top < req.bids[i]) {
                    top = req.bids[i];
                    best = i;
                    req.price = top;
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
        done = req.bids[best];
        round = ai_Time.year;
        cm_QueueVoice(speaker, bankBids, "_VERSTEIGERUNG_GEBOTE", 6, 200);
        d2_SetForm(form, 0);
        w = chars[best];
        io_PrintText(0x1aa4, w->id, req.price);
        if (m->host == G20_ME) {
            holder = 0;
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].officeKind == 0xb) {
                    holder = &ai_CharsT[i];
                    break;
                }
            }
            cm_BeginFieldBlock((char *)b, b->id);
            cm_AddFieldDelta(4, 1, 0x69, &done);
            cm_AddFieldDelta(4, 1, 0x6d, &round);
            cm_SendFieldDeltas();
            cm_SendOwnership(b->id, ((AICHAR *)ai_PlayerChar)->netId, w->netId);
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
            cm_SendOwnership(b->id, ((AICHAR *)ai_PlayerChar)->netId, ((AICHAR *)ai_PlayerChar)->netId);
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
    float lim, k;
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
    k = (float)cm_RandomFloat() * 0.16666667f + 0.3f;
    if (gm_GetGoodsValue((char *)c) * k < lim) {
        k = (float)cm_RandomFloat() * 0.16666667f + 0.3f;
        v = (int)(gm_GetGoodsValue((char *)c) * k);
    } else
        v = (int)lim;
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
    if ((ai_PlayerBuildingT == b && b->mainRoom == id) || (ai_FallbackBuildingT == b && b->mainRoom == id)) {
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
            k = (unsigned char)ai_BTypesT[b->type].kind;
            if (init_BuildingUniverse[k] != 0) {
                if (init_BuildingUniverse[k] == (char)0xff || init_SceneDirty[init_BuildingUniverse[k] * 0x3e0] != 0)
                    init_LoadBuildingScene(k);
                k = (unsigned char)ai_BTypesT[b->type].kind;
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

/* ai_Unk925538: view of cm_Parties (tools/data_views.py) */

/* the random seed of the scenes (0x92c4e0), a linear congruence; the number is the high word modulo 0x7fff */
#define G28_HI (*(unsigned short *)((char *)&ai_Unk92c4e0 + 2))  /* 0x92c4e2 */


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
        } else {
            ((D3FLMAP *)d3_Terrain)->lod = 1;
            sh = 1;
        }
        d3fl_SetDetail(0x40);
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
/* 0x4e3230, 0x403390 (arguments, if any, are passed on untouched) unless the replay mode is on */

int cm_CreateChar(void)
{
    if (cm_SkipEvents != 0)
        return 0;
    return ch_CreateK();
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
/* 0x4e32d0 */

int cm_QueueVoice(int a, int b, int c, int d, int e)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_QueueVoice(a, b, c, d, e);
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
/* 0x4e3420, runs the scene for a ticks (frame wait with the flag 0x8000): the end sets the stop flag 0x6f22a0 */

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
/* 0x4e3620 */

int cm_PlaySpecialTrack(char *a)
{
    if (cm_SkipEvents == 0) {
        snd_ClearQueue();
        return ((int (*)(char *))snd_PlaySpecialTrack)(a);
    }
}
/* 0x4e3640 */

int cm_EndSpecialTrack(void)
{
    if (cm_SkipEvents == 0)
        return ((int (*)())snd_EndSpecialTrack)();
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
    int *ids;
    int j, i, q;

    ids = party->members;
    for (j = 0; j < 16; j++, ids++) {
        cm_CutInfosT[j].fresh = 1;
        cm_CutInfosT[j].valid = 0;
        cm_CutInfosT[j].ready = 0;
        cm_CutInfosT[j].recvFlag = 1;
        if (party != 0) {
            cm_CutInfosT[j].playerId = *ids;
            if (party->kind == 3) {
                for (i = 0; i < party->used; i++) {
                    if (cm_CutInfosT[i].playerId == -1)
                        continue;
                    for (q = 0; q < 16; q++) {
                        cm_CutInfosT[i].entries[q].charId = -1;
                        cm_CutInfosT[i].entries[q].state = 0;
                    }
                }
            }
        } else
            cm_CutInfosT[j].playerId = -1;
        cm_CutInfosT[j].text[0] = 0;
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
        if (cm_CutInfosT[i].fresh != 0 && cm_CutInfosT[i].valid == 0 && cm_CutInfosT[i].playerId != -1) {
            ok = 0;
            break;
        }
    }
    return ok;
}
/* 0x4e37a0, the scene party m (+0x30 count): every member slot of 0x92cae0 that is used and not yet announced (+0xc) shows its text (+0x14, then 0x60c6c0) */

int cm_ShowPartyTexts(char *m)
{
    int i;
    char *s;

    s = cm_CutInfos;
    for (i = 0; i < (unsigned char)m[0x30]; i++, s += 0x35c) {
        if (*(int *)(s + 0xc) == 0 && *(int *)(s + 8) != 0) {
            d2_SetForm(*(int *)(m + 0x2c), 0);
            io_PrintText(s + 0x14);
            io_PrintText((unsigned)&cm_TextNewline);
            *(int *)(s + 0xc) = 1;
        }
    }
}
/* 0x4e3800, the highest index below n (1 ..) of the ids m +0x34 that is not -1; 0 without */

int cm_PrevPartyMember(char *m, int n)
{
    int i;

    for (i = n - 1; i > 0; i--)
        if (*(int *)(m + 0x34 + i * 4) != -1)
            return i;
    return 0;
}
/* 0x4e3830, runs the scene party m for the scene context ctx: the party's cutscene records are reset (cm_ResetCutInfos),
   the owner announces the scene ("cut_type:%i", message 0x11). In the local run (ai_GameFlags & 8) every member's callback
   of the scene kind (cm_KindAskPlayer for a player's character, kind 6, cm_KindAskOther for the others) fills his record and its
   text is shown. In the net run (& 4) the records are sent as the long message with cm_RequestSendCutInfo: with the kind's
   flag cm_KindAllAtOnce 1 the owner answers for the computer members, then each player for his own character, all at once;
   with 0 the members answer one after the other (a member once the one before him has answered, the owner the computer
   ones), until cm_PartyAnswered. The counter cm_ScenePartyRuns grows by one per run; the random seed is kept
   @deep reconstructed from the original's 551 instructions, every block read and compared (the local run,
   the all-at-once net run with the owner's loop over the computer members and the player's own answer, the
   one-after-the-other run with its two waits); calls, stores and branches line up one to one (ours 554 /
   551). Left is allocation: the loop induction variables (i, &members[i], &cm_CutInfosT[i].valid) sit in
   other registers / slots (the original keeps the k==1 owner loop's counters at esp+0x14/0x10/0x18 and c
   of the last loop in esp+0x10, ours in ebx), code hunks at +0x1f4 (mov ecx,[esp+0x10] vs [esp+0x14]) and
   +0x68b (lea eax,[esi*8] vs [ebx*8]); the prover stops on the stack depth at a pushed block (references:
   M8 at +0x55d: ours refers to cm_SceneDuration (0x60d364), the original's field to 0x6f0d80
   (cm_SceneTicks) (and 1 more), explained: the two M8 lines are crossings of the alignment, not other
   objects: at +0x55d both sides load cm_SceneTicks and cm_SceneDuration and add them (ours in the other
   register order), at +0x6ba both store cm_PartyTurn then cm_CutChar, the alignment pairing one store with
   the other) */

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
        if (party->host == ai_CharsT[ai_PlayerId].netId && cm_SkipEvents == 0) {
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
                ((int (*)(CMCUTINFO *, int))cm_KindAskPlayer[party->kind * 5])(&cm_CutInfosT[i], ctx);
            } else {
                cb = (int (*)(CMCUTINFO *, int))cm_KindAskOther[party->kind * 5];
                if (cb != 0)
                    cb(&cm_CutInfosT[i], ctx);
            }
            d2_SetForm(party->window, 0);
            io_PrintText(cm_CutInfosT[i].text);
            io_PrintText(&cm_TextNewline);
        }
    } else if (ai_GameFlags & 4) {
        cm_CutChar = (char *)&ai_CharsT[ai_PlayerId];
        if (cm_KindAllAtOnce[party->kind * 20] == 1) {
            if (party->host == ai_CharsT[ai_PlayerId].netId) {
                /* the owner answers for the computer members */
                for (i = 0; i < party->used; i++) {
                    c = (AICHAR *)gm_FindChar(party->members[i]);
                    if (c == 0 || c->kind == 6 || c->kind == 7 || cm_CutInfosT[i].valid != 0)
                        continue;
                    cm_CutChar = (char *)c;
                    rec = cm_CutInfosT[i];
                    cb = (int (*)(CMCUTINFO *, int))cm_KindAskOther[party->kind * 5];
                    if (cb != 0)
                        cb(&rec, ctx);
                    rec.valid = 1;
                    rec.ready = 0;
                    cm_SetLongMsg(0x35c, &rec);
                    h = cm_RequestSendCutInfo(c->netId, party->id);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    cm_ShowPartyTexts(m);
                    sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
                }
            }
            /* each player answers for his own character */
            cm_CutChar = (char *)&ai_CharsT[ai_PlayerId];
            for (i = 0; i < party->used; i++) {
                if (party->members[i] == ((AICHAR *)cm_CutChar)->netId) {
                    rec = cm_CutInfosT[i];
                    cm_SceneDeadline = cm_SceneTicks + cm_SceneDuration;
                    cm_SceneTimedOut = 0;
                    ((int (*)(CMCUTINFO *, int))cm_KindAskPlayer[party->kind * 5])(&rec, ctx);
                    rec.valid = 1;
                    rec.ready = 0;
                    cm_SetLongMsg(0x35c, &rec);
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
        } else if (cm_KindAllAtOnce[party->kind * 20] == 0) {
            /* one after the other: the player's character answers once the member before him has */
            me = -1;
            prev = -1;
            for (i = 0; i < party->used; i++) {
                if (party->members[i] == ai_CharsT[ai_PlayerId].netId) {
                    me = i;
                    break;
                }
            }
            if (me != -1)
                prev = cm_PrevPartyMember(m, me);
            while (cm_PartyAnswered(m) == 0) {
                cm_ShowPartyTexts(m);
                sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
                if (me != -1 && (me == 0 || cm_CutInfosT[prev].valid == 1) && cm_CutInfosT[me].valid == 0) {
                    cm_PartyTurn = me;
                    rec = cm_CutInfosT[me];
                    cm_CutChar = (char *)gm_FindChar(party->members[me]);
                    cm_ShowPartyTexts(m);
                    cm_SceneDeadline = cm_SceneTicks + cm_SceneDuration;
                    cm_SceneTimedOut = 0;
                    ((int (*)(CMCUTINFO *, int))cm_KindAskPlayer[party->kind * 5])(&rec, ctx);
                    rec.valid = 1;
                    rec.ready = 0;
                    cm_SetLongMsg(0x35c, &rec);
                    h = cm_RequestSendCutInfo(((AICHAR *)cm_CutChar)->netId, party->id);
                    while (cm_GetSlotState(h) == 0)
                        sv_PumpNet();
                    cm_ShowPartyTexts(m);
                    sv_RunFrame(0, cm_DialogFrameFlags | 0x8000);
                }
                /* the owner answers for the first computer member whose turn it is */
                if (party->host == ai_CharsT[ai_PlayerId].netId) {
                    for (i = 0; i < party->used; i++) {
                        c = (AICHAR *)gm_FindChar(party->members[i]);
                        if (c == 0 || c->kind == 6 || c->kind == 7)
                            continue;
                        if (i != 0 && cm_CutInfosT[cm_PrevPartyMember(m, i)].valid != 1)
                            continue;
                        if (cm_CutInfosT[i].valid != 0)
                            continue;
                        cm_PartyTurn = i;
                        cm_CutChar = (char *)c;
                        next = cm_CutInfosT[i];
                        cb = (int (*)(CMCUTINFO *, int))cm_KindAskOther[party->kind * 5];
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
   seeds the random numbers with m->seed and calls the run function of the kind (cm_KindRun); then restores all of it,
   fades in again, and the owner gives every member's character back its scene party field (+0x20c = -1, a field
   message) */

void cm_ExecCutscene(char *m)
{
    G28REC *party = (G28REC *)m;
    int ours, shown, panel;
    float savedSpeed;
    int savedFog, savedLowPoly;
    int rect[4], pos[4], rot[4];
    char buf[0x200];
    int savedBox, i, v;
    AICHAR *c;
    unsigned int fl;

    savedSpeed = cm_SpeedFactor;
    savedLowPoly = ch_LowPoly;
    ours = 0;
    shown = 0;
    savedFog = 0;
    panel = 0;
    sprintf(buf, "Exec Cutscene(Mainfunc start) <Type = %i> <id = %i", party->kind, party->id);
    rect[2] = d2_DirtyRectRight;
    rect[0] = d2_DirtyRectLeft[0];
    rect[1] = d2_DirtyRectTop;
    rect[3] = d2_DirtyRectBottom;
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
        } else if ((ai_GameFlags & 4) && cm_IsPartyMember(ai_CharsT[ai_PlayerId].netId, m) == 0)
            cm_SkipEvents = 1;
    }
    if (cm_SkipEvents == 0) {
        fl = (sv_FrameMask & ~8) | 0x10080;
        cm_SceneFade = (char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
        while ((*cm_SceneFade & 4) == 0)
            sv_RunFrame(0, fl);
        pos[0] = sv_ViewPosX;
        pos[1] = sv_ViewPosY;
        pos[2] = sv_ViewPosZ;
        rot[0] = sv_ViewRotX;
        rot[1] = sv_ViewRotY;
        rot[2] = sv_ViewRotZ;
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
        ((void (*)(void))d3io_NoopCallback)();  /* @original called without its argument (no push) */
        cm_ClearSceneChars();
        if (party->flags & 0x20) {
            /* the cinemascope letterbox: 75 (scaled) lines off the top and the bottom */
            cm_DialogFrameFlags &= ~0x100;
            d3_SetViewport((float)d2_ScreenW, d2_ScreenH - d2_ScreenScale * 150.0f, 0, cm_Float75 * d2_ScreenScale,
                           d3_ZNear, d3_ZFar, d3_ScaleX, d2_ScreenW, d2_ScreenH);
            d2_DirtyRectRight = 0;
            d2_DirtyRectLeft[0] = d2_ScreenW;
            d2_DirtyRectTop = (int)(d2_ScreenScale * cm_Float75);
            d2_DirtyRectBottom = (int)(d2_ScreenH - d2_ScreenScale * cm_Float75);
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
    ((void (*)(char *))cm_KindRun[party->kind * 5])(m);
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
            d2_DirtyRectRight = rect[2];
            d2_DirtyRectLeft[0] = rect[0];
            d2_DirtyRectTop = rect[1];
            d2_DirtyRectBottom = rect[3];
            if (panel != 0 && !(party->flags & 0x40)) {
                sv_SetPanelEnabled(1);
                sv_RedrawScreen();
            }
        }
        d3if_SelectScene(cm_SavedScene, 0);
        if (d3_Sky != 0)
            d3sky_Init(d3_Sky, 1);
        sv_ViewPosX = pos[0];
        sv_ViewPosY = pos[1];
        sv_ViewPosZ = pos[2];
        sv_ViewRotX = rot[0];
        sv_ViewRotY = rot[1];
        sv_ViewRotZ = rot[2];
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
    if (party->host == ai_CharsT[ai_PlayerId].netId) {
        for (i = 0; i < 16; i++) {
            cm_CutInfosT[i].ready = 0;
            cm_CutInfosT[i].fresh = 0;
            cm_CutInfosT[i].valid = 0;
            if (party->members[i] != -1 && (c = (AICHAR *)gm_FindChar(party->members[i])) != 0) {
                v = -1;
                cm_BeginFieldBlock(c, c->netId);
                cm_AddFieldValue(4, 1, (char *)c - cm_FieldObject + 0x20c, &v);
                cm_SendFieldValues();
            }
        }
    }
    if (party->host != -1) {
        v = -1;
        if ((c = (AICHAR *)gm_FindChar(party->host)) != 0) {
            cm_BeginFieldBlock(c, c->netId);
            cm_AddFieldValue(4, 1, (char *)c - cm_FieldObject + 0x20c, &v);
            cm_SendFieldValues();
        }
    }
    cm_CurrentFight = 0;
    evt_NoGroup = cm_SavedNoGroup;
}
/* 0x4e4870, 1 when the character id is one of the party m (+0x30 count, +0x34 ids) and still exists */

int cm_IsPartyMember(int id, char *m)
{
    int i, n;

    n = *(unsigned char *)(m + 0x30);
    for (i = 0; i < n; i++) {
        if (*(int *)(m + 0x34 + i * 4) == id) {
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
/* 0x4e4a50, "cut_CheckMaster": 1 when the owner (m +0xc) is one of the party m (+0x30 count, +0x34 ids); else the first member of the kind 6 / 7 that
   still exists (as long as none was found, 16 slots) becomes the new owner: the log "new master for id %i => %i", a request (0x4c61f0) and the wait for its
   answer (0x4c3ae0 with the frame wait 0x52fe00); 0 then. The log "invalid master for id %i" comes first. ROUGH */

int cm_CheckMaster(char *m)
{
    char buf[0x80];
    int flag, i, h;
    char *c;

    if (cm_IsPartyMember(*(int *)(m + 0xc), m) == 0) {
        flag = 1;
        c = (char *)gm_FindChar(*(int *)(m + 0xc));
        sprintf(buf, "cut_CheckMaster(): invalid master for id %i", *(int *)m);
        for (i = 0; i < 0x10 && flag != 0; i++) {
            if (*(int *)(m + 0x34 + i * 4) != -1) {
                c = (char *)gm_FindChar(*(int *)(m + 0x34 + i * 4));
                if (c != 0 && (c[2] == 6 || c[2] == 7))
                    flag = 0;
            }
        }
        if (c != 0 && *(int *)(c + 4) != *(int *)(m + 0xc)) {
            sprintf(buf, "cut_CheckMaster(): new master for id %i => %i", *(int *)m, *(int *)(c + 4));
            h = ((int (*)())cm_SendMasterChange)(m, c);
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
   (cm_KindCheck; 0 cancels the party, message 0x58). Once its time has come the party is done when it has no check, when
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
            p->host = ai_CharsT[ai_PlayerId].netId;
        if (ai_CharsT[ai_PlayerId].netId == p->host && p->state == 0 && gm_CompareTime(&ai_Time, &soon) > 0) {
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
            if (cm_KindCheck[p->kind * 5] != 0 && ((int (*)(char *))cm_KindCheck[p->kind * 5])((char *)p) == 0) {
                cm_SendCancelParty(p->id);
                continue;
            }
        }
        if (cm_KindCheck[p->kind * 5] == 0 && gm_CompareTime(&ai_Time, &p->time) > 0)
            p->done = 1;
        if (gm_CompareTime(&ai_Time, &p->time) > 0 && p->done == 0) {
            if (cm_IsPartyMember(ai_CharsT[ai_PlayerId].netId, (char *)p) != 0 && (p->flags & 0x80)) {
                if (ai_CharsT[ai_PlayerId].partyId == p->id)
                    p->done = 1;
            } else if (p->host == ai_CharsT[ai_PlayerId].netId && (p->flags & 2))
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
/* 0x4e4f40, the pending party with the smallest id that has the flag 1 (used, id not -1): its record (0x4e50a0), 0 without */

int ai_Unk4e4f40(void)
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
/* 0x4e4f80, runs all pending parties (0x4e4f40) of the player pl that he is a member of: his field +0x208 holds the id of the party while it runs
   (0x4e4060), every party is dropped (0x4e5180) and the field is sent back (0x4c3d90); 1 when one ran. ROUGH */

char ai_RunPendingParties(int pl)
{
    char ret;
    int save;
    int p;

    ret = 0;
    save = ai_CharInts[pl][0x208 / 4];
    p = ai_Unk4e4f40();
    while (p != 0) {
        if (cm_IsPartyMember(ai_CharInts[pl][0], (char *)p) != 0) {
            ai_CharInts[pl][0x208 / 4] = *(int *)p;
            cm_ExecCutscene(p);
            ai_CharInts[pl][0x208 / 4] = -1;
            ret = 1;
        }
        cm_ClearParty(*(int *)p);
        p = ai_Unk4e4f40();
    }
    cm_BeginFieldBlock((char *)ai_CharInts[pl] - 4, ai_CharInts[pl][0]);
    cm_AddFieldValue(4, 1, (unsigned short)(pl * 0x21c - (int)cm_FieldObject + (int)ai_Unk770d8c), &save);
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
/* 0x4e50e0, the member slot (0x92cae0, 0x35c bytes; the id at +0x10) with the id */

int cm_CutInfoById(int id)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(cm_CutInfos[i] + 0x10) == id)
            return (int)cm_CutInfos[i];
    }
    return 0;
}
/* 0x4e5120, the first free party record (id -1), 0 without */

int ai_Unk4e5120(void)
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

    r = (G28REC *)ai_Unk4e5120();
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
    i = 0;
    if (r != 0) {
        n = r->used;
        for (; i < n; i++) {
            if (*(int *)((char *)r + 0x34 + i * 4) == who)
                return 0;
        }
        if (r->used < 0x10) {
            *(int *)((char *)r + 0x34 + n * 4) = who;
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
    int i, n;

    r = (G28REC *)cm_PartyById(id);
    if (r == 0)
        return 0;
    n = r->used;
    for (i = 0; i < n; i++) {
        if (*(int *)((char *)r + 0x34 + i * 4) == who) {
            *(int *)((char *)r + 0x34 + i * 4) = -1;
            break;
        }
    }
    if (cm_CurrentFight != 0 && *cm_CurrentFight == id) {
        for (i = 0; i < 16; i++) {
            if (*(int *)(cm_CutInfos[i] + 0x10) == who) {
                *(int *)(cm_CutInfos[i] + 0x10) = -1;
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

    ai_Unk92c4e0 = seed;
    sprintf(buf, "cut_randseed: %i", seed);
}
/* 0x4e52c0, the seed (logged "cut_getrandseed: %i") */

unsigned int cut_GetRandSeed(void)
{
    char buf[0x40];

    sprintf(buf, "cut_getrandseed: %i", ai_Unk92c4e0);
    return ai_Unk92c4e0;
}
/* 0x4e52f0, a random number below n (logged "cut_rnd: %i"), 0 for n == 0 */

unsigned int cm_Random(unsigned int n)
{
    char buf[0x40];

    if (n == 0)
        return 0;
    ai_Unk92c4e0 = ai_Unk92c4e0 * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rnd: %i", G28_HI % 0x7fffu);
    return (G28_HI % 0x7fffu) % n;
}
/* 0x4e5360, a random float in [0, 1) (logged "cut_rndfloat: %f") */

double cm_RandomFloat(void)
{
    char buf[0x40];

    ai_Unk92c4e0 = ai_Unk92c4e0 * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rndfloat: %f", (float)(G28_HI % 0x7fffu) * 3.0518509e-5f);
    return (float)(G28_HI % 0x7fffu) * 3.0518509e-5f;
}
/* 0x4e53e0, the progress of the dialog dlg: with c set the clock 0x6333cc and the duration a are stored (and the window cleared, 0x475e40);
   else the left percent of the duration is the value; the slider (0x475cb0, 0 .. 100) is set to it, returned */

int cm_DialogTimer(int dlg, int a, int c)
{
    int v;

    v = 100;
    if (c != 0) {
        ai_Unk6f0dc0 = d3_Time;
        ai_Unk60d374 = a;
        d2_SetObjEnabled(dlg, 0);
    } else {
        v = (int)((1.0f - (float)(unsigned int)(d3_Time - ai_Unk6f0dc0) / ai_Unk60d374) * 100.0f);
        v = v > 0 ? v : 0;
    }
    d2_SetObjValue(dlg, 0, 100, v);
    return v;
}
/* 0x4e5470, a message box for the text id with the flags (2: "Messagebox_Autosize", 0x10: "_BIG", 0x20: "_VERY_BIG", 0x100: "_NONE_PERGA" (form 1), else "misc\\Messagebox";
   8: the frame wait flags 0x717c6, 0x80: a timer slider (0x500 ms), 0x40: raises the dialog's windows every frame, 4: ends after 500 ms): the dialog's
   objects are put into a radio group (all but the type 0x40), the loop runs the frame wait sv_RunFrame until the stop request; the answer: the selected
   button index + 1 when the text window is clicked, 0 on a right click on it or the escape key, else -1; not reentrant (cm_MessageBoxOpen).
   @deep read whole against the original; fixed the d2_OpenForm (x, y, name) and sv_RunFrame (cb, mask)
   argument orders, the frame mask (& ~0x38: and al, 0xc7), the clicked window (d2_LastWindow after
   io_PrintText, the original keeps it in id's slot), typed D2WIN/D2OBJ access. Left: allocation only: ours
   keeps ret in ebp (or ebp, -1 / xor ebp, ebp / mov eax, ebp) and fl in a register, the original keeps ret
   at [esp+0x14] and fl at [esp+0x10] with no ebp (frame 0x10 vs 0x18), and the original holds 0 in esi
   from the start (push esi for the d2_OpenForm / d2_CreateRadioGroup zeros, i starts in it); slider = ret
   loads the slot there (or esi, -1 here). */

int cm_MessageBox(int id, int flags)
{
    unsigned long t0;
    int ret, win, textwin, radio, i, k, slider, fl, timed, raise, ends;

    t0 = d3_Time;
    ret = -1;
    win = 0;
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
    d2_CentreViewWindows(fl);
    cm_MessageBoxOpen = 1;
    radio = d2_CreateRadioGroup(0);
    for (i = 0; i < d2_LastWindowPtr->nobjs; i++) {
        k = d2_LastWindowPtr->objs[i];
        if (d2_Objs[k].type != 0x40)
            d2_RadioAdd(radio, k);
    }
    d2_RadioSelect(radio, 0);
    d2_SetForm(win, 0);
    timed = flags & 0x80;
    if (timed != 0)
        slider = d2_WinOpenSlider(0x1c, d2_LastWindowPtr->h - 0x18, d2_LastWindowPtr->w - 0x40, 0, 0x1f4, 0x83, 0x42, d2_LastWindow);
    else
        slider = ret;
    if (sv_RunFrame(cm_MessageBox, fl) != 0) {
        raise = flags & 0x40;
        ends = flags & 4;
        do {
            d2_MenuNavigate(radio);
            if (raise != 0)
                d2_RaiseViewWindows(win);
            if (d2_ClickedId == 0) {
                if (d2_HitWindow == textwin) {
                    ret = d2_RadioSel[radio * 35] + 1;
                    sv_PopRequest = 1;
                } else if (kb_Last == 1) {
                    ret = 0;
                    sv_PopRequest = 1;
                }
            } else if (d2_ClickedId == 1 && d2_HitWindow == textwin || kb_Last == 1) {
                ret = 0;
                sv_PopRequest = 1;
            }
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (timed != 0 && slider != -1)
                d2_SetObjValue(slider, 0, 0x1f4, d3_Time - t0);
            if (ends != 0 && d3_Time > t0 + 0x1f4)
                sv_PopRequest = 1;
        } while (sv_RunFrame(cm_MessageBox, fl) != 0);
    }
    cm_MessageBoxOpen = 0;
    d2_RadioClear(radio);
    d2_CloseView(win);
    return ret;
}
/* 0x4e5730, registers the scene kind: the run function (+0), the callbacks of the members of the kind 6 (+4) and of the others (+8), the check (+0x10) and
   the flag +0xc (1: cm_RunSceneParty asks the members all at once, 0: one after the other) */

void ai_Unk4e5730(int kind, int run, int cb4, int cb8, int check, char flag)
{
    cm_KindRun[kind * 5] = run;
    cm_KindAskPlayer[kind * 5] = cb4;
    cm_KindAskOther[kind * 5] = cb8;
    cm_KindCheck[kind * 5] = check;
    cm_KindAllAtOnce[kind * 20] = flag;
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
    ai_Unk4e5730(0, (int)cm_RunPictureShow, (int)d3io_NoopCallback, 0, 0, 0);
    ai_Unk4e5730(1, (int)cm_RunCouncilSession, (int)cm_SessionAskPlayer, (int)cm_SessionAskAI, 0, 0);
    ai_Unk4e5730(2, (int)cm_RunTrial, (int)cm_TrialAskPlayer, (int)cm_TrialAskAI, (int)cm_CheckTrial, 0);
    ai_Unk4e5730(3, (int)cm_RunFight, (int)cm_FightMember, 0, (int)ai_Unk4d60b0, 1);
    ai_Unk4e5730(4, (int)cm_RunDuel, (int)cm_DuelAskPlayer, (int)cm_DuelAskAI, (int)cm_DuelAftermath, 0);
    ai_Unk4e5730(5, (int)cm_RunExecution, 0, 0, (int)cm_ExecutionAftermath, 0);
    ai_Unk4e5730(6, (int)cm_RunWedding, 0, 0, (int)cm_CheckWedding, 0);
    ai_Unk4e5730(7, (int)cm_RunBirth, 0, 0, (int)cm_CheckBirth, 0);
    ai_Unk4e5730(8, (int)cm_RunDeath, 0, 0, (int)cm_DeathSceneCheck, 0);
    ai_Unk4e5730(9, (int)cm_RunBankruptcy, 0, 0, 0, 0);
    ai_Unk4e5730(10, (int)cm_RunAuction, (int)cm_AuctionAskPlayer, (int)cm_AuctionAskAI, (int)cm_CheckAuction, 1);
    ai_Unk4e5730(11, (int)cm_RunSale, 0, 0, 0, 0);
    cm_ResetCutInfos(0);
}
/* 0x4e58e0, the debug dump of a person (log of the cutscenes): one line of tab separated fields (time, +4, id, the name of the kind (0x60eae0, 24 bytes each) ...) */

void ai_Unk4e58e0(char *c, char *buf)
{
    char *name;

    if ((unsigned char)c[2] < 10)
        name = ai_Unk60eae0[(unsigned char)c[2]];
    else
        name = "%i\t%i\t%i\t%s\t%s\t%i\t%s\t%i\t%s\t%s\t%s\t%s\t";
    sprintf(buf, "%i\t%i\t%i\t%s\t%s\t%i\t%s\t%i\t%s\t%s\t%s\t%s\t", *(int *)&ai_Time, *(int *)(c + 4), *(unsigned short *)c, name, c + 0x30, (unsigned char)c[8],
            ai_Unk143b3e4[(unsigned char)c[9]], *(unsigned short *)(c + 0xa), ai_Unk143b3d8[(unsigned char)c[0xc]], ai_Unk143a610[(unsigned char)c[0xd]],
            ai_Unk143a680[(unsigned char)c[0x164]], ai_Unk143a9f4[(unsigned char)c[0x165]]);
}
/* 0x4e5980, the debug dump of the floats +0x10 .. +0x20 (the 6th "%5.2f" gets the int +0x24 as the original does), the word +0x28 and the bit fields of +0x2c */

void ai_Unk4e5980(char *c, char *buf)
{
    unsigned int v;

    sprintf(buf, "%5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %i\t", *(float *)(c + 0x10), *(float *)(c + 0x14), *(float *)(c + 0x18), *(float *)(c + 0x1c),
            *(float *)(c + 0x20), *(int *)(c + 0x24), *(short *)(c + 0x28));
    v = *(unsigned int *)(c + 0x2c);
    sprintf(buf + strlen(buf), "%i\t %i\t %i\t %i\t %i\t %i\t %i\t %i\t %i\t", v & 0xf, (v >> 4) & 0xf, (v >> 8) & 0xf, (v >> 0xc) & 3, (v >> 0xe) & 7, (v >> 0x11) & 7,
            (v >> 0x14) & 7, (v >> 0x17) & 3, (v >> 0x19) & 0xf);
}
/* 0x4e5a40, the five bytes +0x80 .. +0x84 of a person as a tab separated line */

void ai_Unk4e5a40(char *c, char *buf)
{
    sprintf(buf, "%i\t%i\t%i\t%i\t%i\t", (unsigned char)c[0x80], (unsigned char)c[0x81], (unsigned char)c[0x82], (unsigned char)c[0x83], (unsigned char)c[0x84]);
}
/* 0x4e5a90, the four names (0x143ab0c) of the bytes +0x166 .. +0x169 of a person */

void ai_Unk4e5a90(char *c, char *buf)
{
    sprintf(buf, "%s\t%s\t%s\t%s\t", ai_Unk143ab0c[(unsigned char)c[0x166]], ai_Unk143ab0c[(unsigned char)c[0x167]], ai_Unk143ab0c[(unsigned char)c[0x168]],
            ai_Unk143ab0c[(unsigned char)c[0x169]]);
}
/* 0x4e5af0, the debug dump of the ints +0x190 .. +0x1a4 of a person, then 0x509e60 (c) and 0x50a640 (its id) */

void ai_Unk4e5af0(char *c, char *buf)
{
    sprintf(buf, "%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t", *(int *)(c + 0x190), *(int *)(c + 0x194), *(int *)(c + 0x198), *(int *)(c + 0x19c), *(int *)(c + 0x1a0), *(int *)(c + 0x1a4),
            gm_GetGoodsValue(c), ((int (*)(unsigned short))gm_GetWorth)(*(unsigned short *)c));
}
/* 0x4e5b50, the debug dump of the 8 relatives (the ids at +0x5c): "%i\t%s" of each one's +4 and name (+0x30), -1 and 0x143b3f4 when it does not exist */

void ai_Unk4e5b50(char *c, char *buf)
{
    char tmp[0x100];
    int *p;
    char *o;
    int i;

    p = (int *)(c + 0x5c);
    for (i = 8; i != 0; i--) {
        o = (char *)gm_FindChar(*p);
        if (o != 0)
            sprintf(tmp, "%i\t%s", *(int *)(o + 4), o + 0x30);
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

    if (c != 0 && *(unsigned short *)c != 0xffff) {
        memset(buf, 0, sizeof(buf));
        ai_Unk4e58e0(c, buf + strlen(buf));
        ai_Unk4e5980(c, buf + strlen(buf));
        ai_Unk4e5a40(c, buf + strlen(buf));
        ai_Unk4e5a90(c, buf + strlen(buf));
        ai_Unk4e5af0(c, buf + strlen(buf));
        ai_Unk4e5b50(c, buf + strlen(buf));
    }
}

extern unsigned int ai_MissionSeed;
extern int ai_TimeCopy[4];
extern char * sv_SelectedObject;
extern char cm_ChatLog[2048];
extern int cm_SaveAcks[8][2];
/* ai_CharSex: view of ai_Chars (tools/data_views.py) */
/* ai_Unk770cec: view of ai_Chars (tools/data_views.py) */
extern int ai_Evidence[0x800][2];
extern char ai_Crimes[0x5a00];
extern int cm_SavedScene;
extern int cm_DuelRight;
extern int cm_DuelLeft;
extern int cm_DuelVariants[128];
extern int cm_OrderTicks;
extern unsigned short cm_DisabledObjs[768];
extern unsigned int ai_Unk92c4e0;
extern int cm_KindRun[1];
extern int cm_KindAskPlayer[1];
extern int cm_KindAskOther[1];
extern char cm_KindAllAtOnce[4];
extern int cm_KindCheck[60];
extern char cm_SceneState[1152];
extern int cm_SceneChars[16];
extern int cm_SavedNoGroup;
extern char cm_CutInfos[16][0x35c];
extern float cm_OrderChances[0xb4];
extern char * cm_PointList[32];
extern CMPORTRAIT cm_Portraits[6];
extern G18POP cm_DamagePops[16];
extern char cm_Fighters[21376];
extern char * cm_FlagObjects[32];
extern char cm_Grenades[512];
extern float cm_FightCellScale;
extern int cm_CharInfoWins[32];
extern G18LOG cm_LobbyChat[15];
extern int cm_LobbyChatNext;
extern int cm_LobbyStateFlags;
extern int cm_LobbyChanged;
extern int cm_LobbyClosed;
extern unsigned int cm_LobbyList[9];
extern CMNETGAME cm_NetGame;
extern char cm_LobbyRecords[752];
extern int cm_LobbyPlayerId;
extern int cm_LobbyHelloValue;
extern G17MSG cm_RecvPool[0x2000];
extern int cm_LastSyncCount;
extern int cm_NextNetStep;
extern CMSPLITBUF cm_SplitMsg;
extern G17MSG * cm_RecvFree;
extern unsigned int cm_GroupEndSeq;
extern int cm_FieldBlockLen;
extern G17FIELDS cm_FieldBlock;
extern G17FIELDS ai_NeedBlock;
extern unsigned int cm_LastRequCount;
extern int ai_NeedBlockLen;
extern G17MSG * cm_RecvQueue;
extern char * cm_FieldObject;
extern int cm_SplitMsgLen;
extern struct AICHAR * ai_NeedBlockChar;
extern unsigned char cm_LongMsg[1024];
extern G17MSG cm_SendRing[0x8000];
extern char * cm_LoadBuf;
extern int cm_DayEndRequest;
extern G17STAT cm_SendStates[0x8000];
extern G17MSG * cm_SendQueue;
extern unsigned int cm_LongMsgLen;
extern G17MSG cm_RecvMsg;
extern unsigned int cm_GroupFirstSeq;
extern char d2_CaptionCopy[0x80];
extern int cm_PickCellX;
extern int cm_PickCellZ;
/* ai_Unkfd7c50: view of d3_Scenes (tools/data_views.py) */
/* ai_Unk143a610: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143a680: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143a9f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143ab0c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b3d8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b3e4: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FightPaused: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_WeaponEmpty15e: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_WeaponEmpty174: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_WeaponEmpty176: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_ClickNoGround: view of txt_Ptrs (build/mk_txt_views.py) */
/* cm_FightButtonText: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_AuctionNoteLocal: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_AuctionNoteNet: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_BirthNoteTail: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_ExecNoteTail: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DuelNoteTail: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DuelDeathNews: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DuelHeldNews: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e948: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e954: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e960: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_DeathTreeCaption: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_SpeedNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_ScenePartySync: view of txt_Ptrs (build/mk_txt_views.py) */
float ai_FloatZero = 0.0f;  /* 0x5eb788 */
/* 0x5eb798 / 0x5eb79c are the pooled floats 70.0f / 0.25f (0x428c0000 / 0x3e800000), not a
   double: a 8-byte definition here claims two literals another unit owns. */
/* 0x5eb7ac is the pooled float 0.5f (0x3f000000): use the literal at the call sites. */
double ai_Unk5eb7b0 = 15.0;  /* 0x5eb7b0 */
float cm_Pi = 3.14159274f;  /* 0x5eb7dc */
float cm_Float75 = 75.0f;  /* 0x5eb7fc */
float ai_Unk5eb81c = 30.0f;  /* 0x5eb81c */
float ai_Unk5eb878 = 1.39999998f;  /* 0x5eb878 */
/* 0x5eb8b0 is the pooled float 0.2f (0x3e4ccccd): use the literal at the call sites. */
double ai_Unk5eb8b8 = 1.2;  /* 0x5eb8b8 */
double ai_Unk5eb8e0 = 20.0;  /* 0x5eb8e0 */
float ai_Unk5eb94c = 0.75f;  /* 0x5eb94c */
float ai_Unk5eb988 = 5.0f;  /* 0x5eb988 */
double ai_Unk5eb990 = 10.0;  /* 0x5eb990 */
double ai_Unk5eb9b0 = 0.25;  /* 0x5eb9b0 */
const double cm_WeaponSpeedScale = 0.2857142857142857;  /* 0x5eb9b8 */
double ai_Unk5ebbe8 = 25.0;  /* 0x5ebbe8 */
float ai_Unk5ebc1c = 1.75f;  /* 0x5ebc1c */
double ai_Unk5ece60 = 0.01;  /* 0x5ece60 */
const float ai_Unk5ecebc = 1.25f;  /* 0x5ecebc */
double ai_Unk5edbd0 = 0.050000000000000003;  /* 0x5edbd0 */
const int cm_SpeedTicks[] = { 1155, 937, 770, 577, 385, 0 };  /* 0x5edc10 */
const double cm_EffectRadiusScale = 1.8;  /* 0x5edc28 */
const float ai_Unk5edc50 = 525.0f;  /* 0x5edc50 */
const double ai_Unk5edc58 = 1.1000000000000001;  /* 0x5edc58, read-only: the original keeps it in .rdata */
const double ai_Unk5edc60 = 6.5012084524677184e-315;  /* 0x5edc60 */
const double cm_Inv180 = 0.0055555555555555558;  /* 0x5edc68 */
int evt_LastId = -1;  /* 0x5f8438 */
int ai_Unk5f8b10 = 6251111;  /* 0x5f8b10 */
float d2_ScreenScale = 1.0f;  /* 0x5fe708 */
int cm_SpeedTick = 770;  /* 0x60bd80 */
unsigned int cm_SpeedLevel = 2;  /* 0x60bd84, = 2 (0..4) */
int cm_LastCharId = -1;  /* 0x60bd88, = -1 */
int cm_LastBuildingId = -1;  /* 0x60bd8c, = -1 */
int cm_LastObjectId = -1;  /* 0x60bd90, = -1 */
int cm_LeftPlayerId = -1;  /* 0x60bd94, = -1 */
int (*cm_Handlers[0x6a])(G17MSG *m, G17STAT *s) = { cm_ExNothing, cm_ExGroupPassed, ai_AlwaysTrue, cm_ExNothing, cm_ExNetFlags, cm_ExNothing, cm_ExNothing, cm_ExNothing, cm_ExLoadSize, cm_ExLoadBlock, cm_ExCreateBuilding, cm_ExCreateCharacter, cm_ExCreateNamedCharacter, cm_ExTakeOverCharacter, cm_ExDeleteObject, cm_ExMoveGoods, cm_ExMoveGoodsUnchecked, cm_ExSellObjekt, cm_ExMakeGoods, cm_ExMoveObject, cm_ExRemoveGoods, cm_ExCreateObject, cm_ExFieldDeltas, cm_ExFieldValues, cm_ExMoodDeltas, cm_ExSetField, cm_ExFloatDelta, cm_ExRelations, cm_ExLongMessage, cm_ExSetHE, cm_ExTime, cm_ExGameOver, cm_ExSysMessage, cm_ExKillCharacter, cm_ExAddStraftat, cm_ExRetireCrime, cm_ExProofExecuted, cm_ExDropEvidence, cm_ExSendCutInfo, cm_ExNewParty, cm_ExAllocCutscene, cm_ExAddPartyMember, cm_ExLeaveParty, cm_ExRemoveOwned, cm_ExSetPlant, cm_ExCreateSermon, cm_ExChrWalkTo, cm_ExChrWalkToDummy, cm_ExChrMoveToUniverse, cm_ExChrStop, cm_ExChrSetPos, cm_ExCartState, cm_ExPlaceOnStage, cm_ExChrGotoBuilding, cm_ExChrUseGates, cm_ExChrAnim, cm_ExOwnership, cm_ExSetHome, cm_ExGebUpgrade, cm_ExBuildingDestroyed, cm_ExHandOverCrimes, cm_ExHire, cm_ExCharModel, cm_ExGoodsRecord, cm_ExGoodsDelta, cm_ExLoadStatistics, cm_ExUpdateSmoke, cm_ExSkillsChanged, cm_ExApplication, cm_ExOfficeMessage, cm_ExDecisionValue, cm_ExChangePlayerHe, cm_ExSetJob, cm_ExSpawnCharacter, cm_ExFreeObject, cm_ExChat, cm_ExCreateGeb, cm_ExCharHalt, cm_ExObjCell, cm_ExWalkPath, cm_ExCutEntry, cm_ExCutEntryUpdate, cm_ExDamage, cm_ExDarkCorner, cm_ExTableRequest, cm_ExFightCommand, cm_ExMessageRecord, cm_ExChrDelete, cm_ExCancelParty, cm_ExCutsceneReady, cm_ExMoneyDelta, cm_ExHeldRounds, cm_ExOfficeSwap, cm_ExAddSkill, cm_ExSpyReport, cm_ExLobbyMessage, cm_ExGraveOrder, cm_ExGraveMove, cm_ExRaiseDead, cm_ExCreateAlchemist, cm_ExAlchemistOrder, cm_ExMasterChange, cm_ExEffect, cm_ExShots, cm_ExSetRoute, cm_ExClearRoute };  /* 0x60bd98, the handler of each message type */
int (*cm_Checks[0x6a])(G17MSG *m) = { sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckMoveGoods, sv_ReturnZero, cm_CheckSellObject, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckFieldDeltas, sv_ReturnZero, sv_ReturnZero, cm_CheckSetField, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckOwnership, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckApplication, cm_CheckOfficeMessage, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckDarkCorner, cm_CheckTableRequest, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckCutsceneReady, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero };  /* 0x60bf40, the check of each type (groups 5..6) */
char cm_WalkToDummyWhy[20] = "cm_ExChrWalkToDummy";  /* 0x60c4a0 */
char cm_TextNewline[4] = { 36, 65, 0, 0 };  /* 0x60c6c0 */
char ai_WeaponRecordData[920] = { 84, 1, 0, 0, 120, 5, 20, 0, 1, 4, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0, 86, 1, 0, 0, 95, 10, 15, 0, 1, 4, 1, 0, 0, 0, -96, 66, 5, 107, 97, 109, 112, 102, 47, 115, 116, 114, 101, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 17, 0, 0, 0, 0, 0, 0, 88, 1, 0, 0, 80, 20, 40, 0, 1, 4, 1, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 104, 105, 101, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 17, 0, 0, 0, 0, 0, 0, 114, 1, 0, 0, 110, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 17, 0, 0, 0, 0, 0, 0, 110, 1, 0, 0, 100, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 17, 0, 0, 0, 0, 0, 0, 116, 1, 0, 0, 100, 0, 30, 1, 2, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 17, 0, 0, 2, 0, 0, 0, 118, 1, 0, 0, 90, 0, 30, 1, 1, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 17, 0, 0, 2, 0, 0, 0, 94, 1, 0, 0, 60, 5, 20, 1, 1, 3, 0, 0, 0, 0, -56, 67, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 112, 105, 115, 116, 111, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 106, 17, 0, 0, 1, 0, 0, 0, 96, 1, 0, 0, 70, 10, 30, 1, 1, 3, 0, 0, 0, 0, 22, 68, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 97, 114, 109, 98, 114, 117, 115, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, 17, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, -106, 2, 8, 0, 1, 5, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 99, 104, 108, 97, 103, 101, 110, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0 };  /* 0x60c7c0, the ten 0x5c-byte records ai_FindWeaponRecord searches (G12REC10, hst.c); the last has id 0, the fallback */
float cm_DefenseFactor = 1.0f;  /* 0x60cb58 */
int cm_FightOwner = -1;  /* 0x60cb5c */
int cm_FightView = -1;  /* 0x60cb60 */
int cm_FightTimeBar = -1;  /* 0x60cb64 */
int cm_FightButton = -1;  /* 0x60cb68 */
int cm_DialogFrameFlags = 464838;  /* 0x60d360, the frame wait of the dialogs ( */
int cm_SceneDuration = 10;  /* 0x60d364 */
int cm_TextBox = -1;  /* 0x60d368 */
int cm_SceneEnterScript = -1;  /* 0x60d36c, -1 or the object waited for ( */
int cm_DuelVariantCount = 1;  /* 0x60d370, the number of duel variants in cm_DuelVariants (cm_DuelInitVariants) */
int ai_Unk60d374 = 1;  /* 0x60d374 */
char ai_Unk60dd24 = 48;  /* 0x60dd24 */
char ai_Unk60dd68 = 49;  /* 0x60dd68 */
/* 0x4db700 .. 0x4dd95d (one function; Ghidra's 0x4dc7c0 lies inside it), the trial scene (the run function of the scene kind 2, called with the party m) of a session request m (+0x30 count, +0x34 .. +0x48 the characters: accused, accuser, judge, two
   assessors ..): "Gericht.ed3" / "ADDON\\PROZESS_NEU.sbf", the scenes "cutscenes\\prozess\\prozess-*.esc" (create, judge, assessors, intro,
   accuser, accused), the texts 0x1491 .. 0x1557 through 0x515960 and the voices "_PROZESS_*" (0x4e32d0 / 0x4e32b0); the accusation, the
   charge (witnesses), the question of guilt, the torture sub-scenes "cutscenes\\prozess\\folter*" (the instrument names 0x60dad0 ..
   0x60db84 are the .esc names), the vote ("prozess-abstimmen-1 .. 3"), the verdict (guilty / not guilty, penalties 0x4f4420 / 0x4f48f0 /
   0x4f4460) and the cleanup of all sound slots. The stack frame is 0x760 bytes. ROUGH: the structure and the calls (read from the call
   digest, arguments in call order but mostly 0), not the arguments of 0x4f2720 / 0x4f3580 / 0x4f3b40 and the mood floats (0x3fb33333 ..) */
char ai_Unk60de4c = 50;  /* 0x60de4c, scene slots of the trial */
char ai_Unk60eae0[10][24] = { "DUMMY_NPC", "MEISTER_NPC", "POLITICIAN_NPC", "AI_NPC", "NPC", "PLAYER", "REMOTE_PLAYER", "RELATIVE_PLAYER", "USED_PLAYER", "INVISIBLE_NPC" };  /* 0x60eae0, the names of the NPC kinds */
int cm_RunCharacters = 1;  /* 0x624570, 1: a later server time also updates the characters' needs and runs the masters' evaluations (cm_ExTime); never written (reasoned: a switch) */
int cm_RunRating = 1;  /* 0x624574, 1: a later server time also advances the rating base and recounts the characters every 6 minutes */
int cm_RunEvents = 1;  /* 0x624578, 1: a later server time also runs the event handlers */
char cm_EventNames[115][0x20] = { "HE_NULL", "HE_HISTORIE", "HE_UPGRADE_DUMMY", "HE_ABBAU", "HE_PRODUKTION", "HE_PRODUKTION_FERTIG_MSG", "HE_ARBEITER_EINSTELLEN", "HE_MEISTER_EINSTELLEN", "HE_VORKOMMEN_SUCHEN", "HE_NEUE_VORKOMMEN_SUCHEN", "HE_NEUE_VORKOMMEN_SUCHEN_MSG", "HE_AI_WATCH_TRANSPORT", "HE_OBJEKT_BAUEN", "HE_GEBAEUDE_BAUEN", "HE_MAUER_BAUEN", "HE_TRANSPORT", "HE_REPARATUR", "HE_MESSAGEBOX", "HE_LAGERSLOTS_BAUEN", "HE_LAGER_ERWEITERN", "HE_KRAEUTER_SAMMELN", "HE_GETIER_FANGEN", "HE_SPIELER_TRAINIEREN", "HE_GEHEIM_TREFFEN", "HE_SPIONAGE", "HE_SABOTAGE", "HE_PRUEGEL", "HE_GEB_VERKAUFEN", "HE_GEB_RENOVIEREN", "HE_GEB_VERMIETEN_DUMMY", "HE_GEB_UPGRADEN", "HE_GEB_RAUM_UPGRADEN", "HE_GEB_ABREISSEN", "HE_GEB_KAUFANGEBOT", "HE_SITZUNG", "HE_KREDIT", "HE_MEISTER_PRUEFUNG", "HE_MEISTER_PRUEFUNG_MSG", "HE_MEISTER_BESTANDEN", "HE_GELDBOTE", "HE_ERNTEN", "HE_STAMMTISCH_MSG", "HE_STAMMTISCH", "HE_GOTTESDIENST", "HE_PROZESS", "HE_DROHEN_MSG", "HE_DROHEN", "HE_ANSCHWAERZEN_MSG", "HE_BUCH_SCHREIBEN", "HE_ENTLASSEN", "HE_BEFOERDERN", "HE_PERSONAL_KUENDIGEN", "HE_PERSONAL_STEHLEN", "HE_PAMPHLET", "HE_FEST_MSG", "HE_FEST", "HE_GELAGE", "HE_KERKER_MEISTER_BESTECHEN", "HE_SIM_PERSONAL", "HE_MARKT_SUPERVISOR", "HE_ENTFUEHREN", "HE_SPIELER_ENTFUEHRT", "HE_LOESEGELD", "HE_EINBRUCH", "HE_GEB_SPIONAGE", "HE_WERBUNG", "HE_RANDOM_EVENT", "HE_STREIFE", "HE_RAZZIA", "HE_ERPRESSUNG", "HE_BELEIDIGUNG_MSG", "HE_BELEIDIGUNG", "HE_UEBERFALL", "HE_ANGRIFF", "HE_RANDOM_GESELLE", "HE_FURCHT_EINFLOESSEN", "HE_MISSION_SUPERVISOR", "HE_MISSION_DONE_MSG", "HE_EVT_GROSS_BRAND", "HE_EVT_BRAND", "HE_EVT_WIRBELSTURM", "HE_EVT_RAUBRITTER", "HE_EVT_KOMMENTARE", "HE_EVT_STADTFEST", "HE_EVT_SPENDE", "HE_EVT_BETEILIGUNG_MSG", "HE_EVT_BETEILIGUNG", "HE_EVT_SOELDNER_PLUENDERN", "HE_EVT_SOELDNER_MARODIEREN", "HE_EVT_PEST", "HE_ESKORTE_VERHINDERN", "HE_GELDSTRAFE", "HE_PRANGER", "HE_NACHWUCHS", "HE_BERUFSAUSBILDUNG", "HE_UNI", "HE_TALENT_SPIELER", "HE_TASCHENDIEB", "HE_SCHUTZGELD", "HE_SCHUTZGELD_MSG", "HE_BETRIEBS_PRUEFUNG", "HE_UMLAND_PATROL", "HE_SIM_UNFALL", "HE_PACHT", "HE_SIM_WIRTSHAUS", "HE_TITEL_VERGABE", "HE_AMT_VERGABE", "HE_BARDE", "HE_HEILEN", "HE_SIM_EINWANDERUNG", "HE_INIT_BETRIEB", "HE_GELIEBTE", "HE_SIM_KRANKHEITEN", "HE_DIE_PLAYER", "HE_KILL_PLAYER" };  /* 0x625820, names ("HE_NULL", ..) */
int ch_LowPoly = 0;  /* 0x631064 */
struct D3SCENE *ch_Saved0 = 0;  /* 0x631084 */
EVTSCRIPT *evt_Scripts = 0;  /* 0x6332b4, [128] */
unsigned int d3_SceneFlags = 0;  /* 0x63349c */
int cm_RecvCount = 0;  /* 0x6f0cd0, received messages */
int cm_NoticeCount = 0;  /* 0x6f0cd4 */
int cm_LobbyChatCount = 0;  /* 0x6f0cd8, the lines in the ring (at most 15) */
int cm_LobbyChatPos = 0;  /* 0x6f0cdc, the index of the line the chat window reads next (cm_LobbyChatFirst / 0x4cd4c0) */
char * cm_HoverObject = 0;  /* 0x6f0ce0 */
int cm_FightPaused = 0;  /* 0x6f0ce4, fight paused */
int cm_FightDefender = 0;  /* 0x6f0ce8 */
int cm_FightAttacker = 0;  /* 0x6f0cec */
char * cm_PickMarker = 0;  /* 0x6f0cf0 */
int cm_FightWinner = 0;  /* 0x6f0cf4 */
int cm_FightSettled = 0;  /* 0x6f0cf8 */
int cm_FightLootValue = 0;  /* 0x6f0cfc */
char * cm_FightRecord = 0;  /* 0x6f0d04 */
int cm_SelectedCount = 0;  /* 0x6f0d08 */
int cm_FightSounds = 0;  /* 0x6f0d0c */
int cm_CrossbowSounds = 0;  /* 0x6f0d10 */
int cm_BombSounds = 0;  /* 0x6f0d14 */
int cm_DaggerSounds = 0;  /* 0x6f0d18 */
int cm_PistolSounds = 0;  /* 0x6f0d1c */
int cm_SwordSounds = 0;  /* 0x6f0d20 */
int cm_AttackerClickVoices = 0;  /* 0x6f0d24 */
int cm_DefenderClickVoices = 0;  /* 0x6f0d28 */
int cm_AttackerAttackVoices = 0;  /* 0x6f0d2c */
int cm_DefenderAttackVoices = 0;  /* 0x6f0d30 */
int cm_AttackerOrderVoices = 0;  /* 0x6f0d34 */
int cm_DefenderOrderVoices = 0;  /* 0x6f0d38 */
int cm_AttackerTauntVoices = 0;  /* 0x6f0d3c */
int cm_DefenderTauntVoices = 0;  /* 0x6f0d40 */
int cm_AttackerPainVoices = 0;  /* 0x6f0d44 */
int cm_DefenderPainVoices = 0;  /* 0x6f0d48 */
int cm_FightAmbient = 0;  /* 0x6f0d4c */
int cm_AttackerRole = 0;  /* 0x6f0d50 */
int cm_DefenderRole = 0;  /* 0x6f0d54 */
int cm_FlagObjectCount = 0;  /* 0x6f0d58 */
int cm_BootsObject = 0;  /* 0x6f0d5c */
int cm_PointCount = 0;  /* 0x6f0d60 */
int cm_FightForm = 0;  /* 0x6f0d64 */
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
int ai_Unk6f0dc0 = 0;  /* 0x6f0dc0 */
char cm_MessageBoxOpen = 0;  /* 0x6f0dc4 */
char * ai_PickedObject = 0;  /* 0x6f22e8 */
int ai_PickedChar = 0;  /* 0x6f22f8 */
int cm_TutorialActive = 0;  /* 0x6f2438 */
int cm_FightFixedLight = 0;  /* 0x6f244c */
int cm_NetFlags = 0;  /* 0x6f2594, flags */
int cm_RequestedSaveSlot = 0;  /* 0x6f25c8 */
char cm_RequestedSaveName[72] = {0};  /* 0x6f25d0 */
/* ai_GfxFloorLod: view of ai_GfxFloor (tools/data_views.py) */
unsigned int ai_MissionSeed = 0;  /* 0x75e120, the random seed of the missions (as mission.h) */
int ai_TimeCopy[4] = {0};  /* 0x75e130, the clock copy 0x75e130..0x75e13e (14 bytes, written whole by cm.c / sv.c): [0] the year; ai_Unk75e134 / 75e138 / 75e13c are views */
char * sv_SelectedObject = 0;  /* 0x75ec60 */
char cm_ChatLog[2048] = {0};  /* 0x75fba0 */
int cm_SaveAcks[8][2] = {0};  /* 0x76fa20 */
/* ai_Unk770d8c: view of ai_Chars (tools/data_views.py) */
int ai_Evidence[0x800][2] = {0}; /* 0x91b6e0 */
char ai_Crimes[0x5a00] = {0}; /* 0x91f700, the crime records (0x2d bytes each) up to 0x925100; f3gm's goods
                                    table ai_Unk91f716 is the same storage, a view at +0x16 */
int cm_SavedScene = 0;  /* 0x925510 */
G28REC cm_Parties[96] = {0};  /* 0x925520 */
int cm_DuelRight = 0;  /* 0x92bca0 */
int cm_DuelLeft = 0;  /* 0x92bca4 */
int cm_DuelVariants[128] = {0};  /* 0x92bcc0, the shuffled order of the duel variants (gm_Shuffled fills it) */
int cm_OrderTicks = 0;  /* 0x92bec0 */
unsigned short cm_DisabledObjs[768] = {0};  /* 0x92bee0 */
unsigned int ai_Unk92c4e0 = 0;  /* 0x92c4e0, the random seed of the scenes */
int cm_KindRun[1] = {0};  /* 0x92c500, the scene kind table: run, callbacks, flag, check */
int cm_KindAskPlayer[1] = {0};  /* 0x92c504, the scene kind table (5 ints per kind) */
int cm_KindAskOther[1] = {0};  /* 0x92c508 */
char cm_KindAllAtOnce[4] = {0};  /* 0x92c50c */
int cm_KindCheck[60] = {0};  /* 0x92c510, the answer callbacks of the kind table (5 ints per kind) */
char cm_SceneState[1152] = {0};  /* 0x92c600 */
int cm_SceneChars[16] = {0};  /* 0x92ca80 */
int cm_SavedNoGroup = 0;  /* 0x92cac0 */
char cm_CutInfos[16][0x35c] = {0};  /* 0x92cae0 */
float cm_OrderChances[0xb4] = {0};  /* 0x9300a0 */
char * cm_PointList[32] = {0};  /* 0x930380 */
CMPORTRAIT cm_Portraits[6] = {0};  /* 0x930400 */
G18POP cm_DamagePops[16] = {0};  /* 0x9304c0 */
char cm_Fighters[21376] = {0};  /* 0x930600 */
char * cm_FlagObjects[32] = {0};  /* 0x935980 */
char cm_Grenades[512] = {0};  /* 0x935a00 */
float cm_FightCellScale = 0;  /* 0x935d00 */
int cm_CharInfoWins[32] = {0};  /* 0x935d20 */
G18LOG cm_LobbyChat[15] = {0};  /* 0x935da0 */
int cm_LobbyChatNext = 0;  /* 0x9363f4 */
int cm_LobbyStateFlags = 0;  /* 0x936400, [0] the lobby is open, [1] a lobby message came in */
int cm_LobbyChanged = 0;  /* 0x936404 */
int cm_LobbyClosed = 0;  /* 0x936408 */
unsigned int cm_LobbyList[9] = {0};  /* 0x936420, the count then the eight player states (0x24 bytes) */
CMNETGAME cm_NetGame = {0};  /* 0x936460, the network game (hdr.players 0x936496, mode 0x9364dc) */
char cm_LobbyRecords[752] = {0};  /* 0x9364e0 */
int cm_LobbyPlayerId = 0;  /* 0x9367d0, this player's lobby id */
int cm_LobbyHelloValue = 0;  /* 0x9367d4, the hello answer's second value */
G17MSG cm_RecvPool[0x2000] = {0};  /* 0x9367e0, the free receive records */
int cm_LastSyncCount = 0;  /* 0xa687e0, "cm_LastSyncCount" (net.h) */
int cm_NextNetStep = 0;  /* 0xa687e4, d3_Time of the start */
CMSPLITBUF cm_SplitMsg = {0};  /* 0xa68800, a long message to split (cm_SplitMsgLen bytes) */
G17MSG * cm_RecvFree = 0;  /* 0xa68c00, the free list */
unsigned int cm_GroupEndSeq = 0;  /* 0xa68c04 */
int cm_FieldBlockLen = 0;  /* 0xa68c08, bytes used in cm_FieldBlock.buf */
G17FIELDS cm_FieldBlock = {0};  /* 0xa68c20 */
G17FIELDS ai_NeedBlock = {0};  /* 0xa68ca0 */
unsigned int cm_LastRequCount = 0;  /* 0xa68d1c, "cm_LastRequCount" */
int ai_NeedBlockLen = 0;  /* 0xa68d20, bytes used in ai_NeedBlock.buf */
G17MSG * cm_RecvQueue = 0;  /* 0xa68d24, the received messages */
char * cm_FieldObject = 0;  /* 0xa68d2c, the object of the field block (fields are offsets into it) */
int cm_SplitMsgLen = 0;  /* 0xa68d30, length of cm_SplitMsg */
struct AICHAR * ai_NeedBlockChar = 0;  /* 0xa68d34, the character ai_BeginNeedBlock builds the block for */
unsigned char cm_LongMsg[1024] = {0};  /* 0xa68d40, a joined long message */
G17MSG cm_SendRing[0x8000] = {0};  /* 0xa69140, the send ring */
char * cm_LoadBuf = 0;  /* 0xf31140, the load buffer ("cm_LoadBuf") */
int cm_DayEndRequest = 0;  /* 0xf31144 */
G17STAT cm_SendStates[0x8000] = {0};  /* 0xf31160 */
G17MSG * cm_SendQueue = 0;  /* 0xf81160, the messages to send */
unsigned int cm_LongMsgLen = 0;  /* 0xf81164, length of cm_LongMsg */
G17MSG cm_RecvMsg = {0};  /* 0xf81180, the message being received */
unsigned int cm_GroupFirstSeq = 0;  /* 0xf8121c */
AIWATCH ai_Watch = {0}; /* 0xf895a0, what the watched building's masters did (0xb00 bytes) */
char d2_CaptionCopy[0x80] = {0}; /* 0xf8aae0, the d2 caption / status text buffer (gfx.h's d2_CaptionCopy, 0x80 bytes up to d2_InputDragValue at 0xf8ab60) */
/* ai_Unkf8f39c: view of d2_Wins (tools/data_views.py) */
int cm_PickCellX = {0}; /* 0xfa5f00 */
int cm_PickCellZ = {0}; /* 0xfa5f04 */
/* txt_AmountUnit: view of txt_Ptrs (build/mk_txt_views.py) */
