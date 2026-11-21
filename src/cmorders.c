/* cmorders.c -- the fight orders and the fight loop (0x4d76a0..0x4d8b50): cm_RunCutOrders, cm_FightLoop,
   cm_FightMember, cm_ClearSceneChars. Split from cmfight: the original's cm_RunCutOrders does not know the leaf
   cm_CharObjDistXZ (it reloads x->scene->obj after every call) while cmfight's callers below 0x4d76a0 (cm_Attack,
   cm_NearestEnemy) keep their loads across it (bprobe at 0x4d76a0: only cm_RunCutOrders changes, 847 -> 273); the
   strings 0x60d300..0x60d35f are used only by these functions (cmfight's run ends with cm_PlantEscapeFlag's
   0x60d2f0), the .bss they read (cm_FightPaused 0x6f0ce4 ..) stays cmfight's.
   unit 0x004d76a0..0x004d8b50; boundaries in decl/text.dcl */
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
#include "cmfight.h"
#include "cmorders.h"
#include "d2.h"
#include "d3.h"
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
unsigned short util_Random(unsigned short n);  /* 0x42ec00, @original: tr.c defines it returning int; this unit's callers read the 16-bit result (mov si,ax / movzx): the original's units saw a 16-bit declaration */
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests */
#define d2_LastWindowPtrC (*(char **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) read bytewise, as this unit does */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define cm_LongMsgCut (*(CMCUTINFO *)&cm_LongMsg)  /* 0xa68d40, cm_LongMsg (cm.h: unsigned char[]) as the cutscene record message 0x26 joins */
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
typedef struct CMBOMB {             /* 8 bytes, one of the 32 bombs of cm_Bombs */
    D3OBJECT *obj;                  /* the bomb object "ob_BOMBE", 0 = a free slot */
    unsigned long time;             /* d3_Time when it was laid */
} CMBOMB;
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define cm_FightersT (*(AICHAR (*)[32])&cm_Fighters)  /* 0x930600, cm_Fighters (char[], with the 0x1000 bytes after the 32 records) as its AICHAR records */
#define ai_SceneCharsT (*(AICHAR *(*)[32])&ai_SceneChars)  /* 0x7603a0, ai_SceneChars (f3init.h: int[32]) as the AICHAR pointers it holds */
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
#define CM_CHR(c) ((CHARACTER *)(c)->model)  /* AICHAR.model (ai.h: AITASK *) as the CHARACTER it is */
typedef struct CMSPYREPORT {            /* an AIOBJ of kind 0xca (a spy report) as this unit reads it: the report's words
                                           0x1c .. 0x37 (HESPYREPORT) after the object's head */
    char pad1c[0x1c];
    HESPYREPORT report;
} CMSPYREPORT;

extern char ai_WeaponRecordData[920];
extern float cm_DefenseFactor;
extern int cm_FightOwner;
extern int cm_FightView;
extern int cm_FightTimeBar;
extern int cm_FightButton;
extern char * cm_PointList[32];
extern CMPORTRAIT cm_Portraits[6];
extern G18POP cm_DamagePops[16];
extern char cm_Fighters[21376];
extern struct D3OBJECT *cm_FlagObjects[32];
extern float cm_FightCellScale;
extern int cm_CharInfoWins[32];
extern char * cm_HoverObject;
extern int cm_FightPaused;
extern int cm_FightDefender;
extern int cm_FightAttacker;
extern char * cm_PickMarker;
extern int cm_FightWinner;
extern int cm_FightSettled;
extern int cm_FightLootValue;
extern int cm_UnreadBss;
extern char * cm_FightRecord;
extern int cm_SelectedCount;
extern struct SBBANK *cm_FightSounds;
extern struct SBBANK *cm_CrossbowSounds;
extern struct SBBANK *cm_BombSounds;
extern struct SBBANK *cm_DaggerSounds;
extern struct SBBANK *cm_PistolSounds;
extern struct SBBANK *cm_SwordSounds;
extern int cm_AttackerClickVoices;
extern int cm_DefenderClickVoices;
extern int cm_AttackerAttackVoices;
extern int cm_DefenderAttackVoices;
extern int cm_AttackerOrderVoices;
extern int cm_DefenderOrderVoices;
extern int cm_AttackerTauntVoices;
extern int cm_DefenderTauntVoices;
extern int cm_AttackerPainVoices;
extern int cm_DefenderPainVoices;
extern int cm_FightAmbient;
extern int cm_AttackerRole;
extern int cm_DefenderRole;
extern int cm_FlagObjectCount;
extern int cm_BootsObject;
extern int cm_PointCount;
extern int cm_FightForm;

/* declarations of cmfight between its functions that the moved functions read */
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
#define cm_FightRecordT (*(CMFIGHTREC **)&cm_FightRecord)  /* 0x6f0d04, cm_FightRecord (cm.h: char *) as the fight record it points to */
#define cm_FightAttackerC (*(AICHAR **)&cm_FightAttacker)  /* 0x6f0cec, cm_FightAttacker (cm.h: int) as the character it points to */
#define cm_FightDefenderC (*(AICHAR **)&cm_FightDefender)  /* 0x6f0ce8, cm_FightDefender (cm.h: int) as the character it points to */

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
   @deep deep s50: the lead of the previous note was a unit boundary: cm_RunCutOrders .. cm_ClearSceneChars are
   now unit cmorders (layout commit), so the leaf cm_CharObjDistXZ is unknown here and x->scene->obj is reloaded
   after each call as in the original: 949 -> 436 ins (size 4558 vs 4599). Next: read the remaining hunks
   against the original again from the top (the cross-jumped tails of the states and the slot of the loop's slot
   pointer noted below are older findings). prior: Lead (scratch copies of cm.c, obj bytes against the image):
   574 of the differing instructions are one cause. The original reloads x->scene->obj for every
   cm_CharObjDistXZ call (mov eax,[edi+0x184]; mov ecx,[eax+0x34]; push ecx); ours keeps it in edi across the
   calls because VC6 knows cm_CharObjDistXZ (defined above) writes no memory. A dead global store in
   cm_CharObjDistXZ (if (0) cm_LastSeq = 0;) or an external copy of it takes this function from 849 to 275
   differing instructions of 1445 with nothing else changed here; cm_NearestEnemy (byte) allows the global store
   but no indirect store and no external call. So the original cm_CharObjDistXZ's text holds a global store or a
   call of a known global-writing function that leaves no code (it is byte now, D3VECTOR4 v, d); not found, see
   the unit's Open: summary line. Done this session: cm_SendCutEntry / cm_SendCutEntryUpdate take the entry's
   address as an integer, so m counts as escaped (three store orders, 955 -> 953). prior: Reconstructed against
   the original in full (0x4d76a0..0x4d88b8, all 8 states): the case order of the source (5, 1, 2, 3, 6, 4, 7,
   8), the hit chance times cm_DefenseFactor for the defender's side, the damage without a target 9.0 * n, the
   AI counter-order with one cm_CharWeaponItem call, the ranged walk with cm_BestCellNear's reach (range / cell
   size * 0.9), the goods-carry state 6 on the bytes 0x20..0x22 (store cell, step) with cm_SendCutEntryUpdate,
   the four name buffers (0x30 each, frame 0x138 equal), the senders' return values as the order slot
   (cm_SendCutEntry / cm_SendCutEntryUpdate now return cm_QueueSend's slot). Switch tables equal, ins 1437 vs
   1434. Left: allocation and block layout - the loop keeps slot in edx at the top (ours reloads it from
   [esp+0x68]), the original cross-jumps the identical tails differently (state 2's stop joins state 5's command
   at 0x4d7789 where ours join... */
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
    int slot, i, n, res, b, hit, ix, iz, st, cx, cz, r;
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
            res = cm_SendCutEntry(rec->playerId, (int)&m);
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
            res = cm_SendCutEntry(rec->playerId, (int)&m);
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
                res = cm_SendCutEntry(rec->playerId, (int)&m);
                goto done;
            }
            if (k->kind == 1 || k->kind == 2) {
                item = (AIOBJ *)cm_CharWeaponItem((char *)c);
                if (item != 0 && item->usesLeft <= 0) {
                    m.charId = c->netId;
                    m.state = 0;
                    res = cm_SendCutEntry(rec->playerId, (int)&m);
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
                        r = util_Random(0xff);
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
                            if (cm_FightRecordT->charIds[i] != -1 && rec != &cm_CutInfos[i]) {
                                other = &cm_CutInfos[i];
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
                        p->orderSlot = cm_SendCutEntry(rec->playerId, (int)&m);
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
                res = cm_SendCutEntry(rec->playerId, (int)&m);
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
            res = cm_SendCutEntry(rec->playerId, (int)&m);
            goto done;
        case 6:
            if (p->orderSlot != 1 && cm_GetSlotState(p->orderSlot) == 0)
                break;
            p->orderSlot = -1;
            if (d3_FindObject(0, 0x40, 0, p->dummy) == 0) {
                cm_SendFightCommand(c, 6, (CMFIGHTSLOT *)&m);
                m.charId = c->netId;
                m.state = 0;
                p->orderSlot = cm_SendCutEntry(rec->playerId, (int)&m);
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
                res = cm_SendCutEntryUpdate(rec->playerId, (int)&m);
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
                cm_SendCutEntryUpdate(rec->playerId, (int)&m);
                res = cm_SendObjCell(c->scene->obj->name, ix, iz, 0);
                goto done;
            } else if (p->storeStep == 2) {
                d3sm_GetVertex(h, p->storeX, p->storeZ, (D3VECTOR *)&q);
                if (!d3_VecEqual((D3VECTOR *)&q, &c->scene->obj->pos, 50.0f))
                    break;
                cm_SendFightCommand(c, 5, (CMFIGHTSLOT *)&m);
                m.charId = c->netId;
                m.state = 0;
                res = cm_SendCutEntry(rec->playerId, (int)&m);
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
                    res = cm_SendCutEntry(rec->playerId, (int)&m);
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
            res = cm_SendCutEntry(rec->playerId, (int)&m);
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
            res = cm_SendCutEntry(rec->playerId, (int)&m);
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

void cm_FightLoop(char *a, int ctx)
{
    unsigned int t;
    int i, r;
    AICHAR *o;

    t = tb_Calls - 0x15e;
    r = util_Random(100);
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
                        cm_RunCutOrders(&cm_CutInfos[i]);
                }
            }
        }
        if (cm_FightWinner != 0 || cm_FightSettled != 0)
            sv_PopRequest = 1;
    }
    cm_CloseFightUI();
}
/* 0x4d8b10, the fight member callback (kind 3, cm_Kinds[].askPlayer: a the member record, ctx the party's
   context): on the first run of the scene party only the fight result (cm_FightResult), on the later runs the
   fight loop, which gets the callback's two arguments (VC6 tail-jumps to it; the one-argument cm_FightResult
   call is not a tail call because the callee's argument count differs from the caller's) */

void cm_FightMember(char *a, int ctx)
{
    if (cm_ScenePartyRuns == 0) {
        cm_FightResult(a);
        return;
    }
    cm_FightLoop(a, ctx);
}
/* 0x4d8b30, clears the 16 ints at 0x92ca80 */

void cm_ClearSceneChars()
{
    memset(cm_SceneChars, 0, 0x40);
}
