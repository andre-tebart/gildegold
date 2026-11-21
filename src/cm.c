/* cm.c -- the game messages exchanged with the server (net.c): the size of a message by its type, the send
   ring cm_SendRing (0x8000 slots) and its states, the receive list cm_RecvQueue (0x2000 records), long messages split
   into parts of type 7, the field blocks of an object (delta / plain values). Its .data block starts at 0x60bd80 after
   f3snd.c's literals (0x302, 2, -1 globals, the handler table 0x60bd98 of 0x4c6a20..). Tag "cm_ExecCommand" (0x4c4190,
   its texts at 0x60c0e8..). The ~0x6a message handlers and their helpers follow, then the lobby (0x4cd350..). Ends
   at 0x4cd9d0: the fight code is cmfight.c, the scenes cmscene.c (three objects by their .data blocks, .bss blocks and
   common groups, data layout s12).
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
   Open: not included yet (dcp headers conflicts cm HEADER): anm.h hmt.h hst.h rt.h menu.h;
   held by calls whose argument count differs from the definition (read the original's pushes):
   gv_PassSentence takes five arguments here (gv_PassSentenceK).
   Open: head: cm.h carries the parameter lists of cm_StartEventArg, cm_QueueVoice, cm_LoadVoiceBank,
   cm_RunSceneTicks, cm_RunSceneTicksOrClick, cm_WaitEvent, cm_DialogTimer, cm_PartyById, cm_UnloadVoiceBank,
   cm_MessageBox, cm_PlayVoice, cm_StopScript, cm_DeleteChar, cm_StartScript, cm_CutInfoById,
   cm_WaitEventOrClick, cm_ClearParty, cm_ResetCutInfos, cm_ShowPartyTexts, cm_AllocParty, cm_AddPartyMember,
   cm_WaitEventFull and cm_CreateChar; cm.h's declaration changes still flip the register tie-breaks of
   ai_TutorialInitChapter2 / ai_TutorialInitChapter3 in sv_t and of ord_CountGridObjects. Types only
   cm.c uses (CMNOTE, CMCUTTYPEMSG, CMFIELD, CMWALKPATHMSG) stay in its head for that reason. cm.h still
   repeats ord_AddGridAmount, gm_RemoveOwned, gm_BuildingSkill, gm_HealthRatio, rt_FreePoints and rt_CopyPoints
   (the owner headers' prototypes: gv.h, f3gm.h, rt.h; RTPOINT from rt_types.h).
   Open: head: message 0x3f has no record type in cm_types.h (a struct typedef there flips
   ord_CountGridObjects shape -> draft, a declaration-set tie-break the probe does not reproduce), so 0x3f / 0x40
   read the player / item head of the goods record through G17GOODSMSG.goods.
   The fight scene (cm_SetupFightScene .. cm_MostThreatenedCellNear) reads its fight record as CMFIGHTREC (a partial
   layout, defined in this file; cm_FightRecordT views cm_FightRecord as it).
   Open: types: CMFIGHTSLOT and CMCUTENTRY (cm_types.h) are one 0x2c-byte record in two views (a cutscene entry is sent
   as the fight command's slot, cm_RunCutOrders casts between them); one type for both is unsettled.
   Open: types: CMFIGHTREC (this file) is G28REC's raid view (cm_types.h: defenders 0x80, attackers 0xc0, carts 0x100,
   home 0x10c; escort 0x110 is G28REC's pad110), two declarations of one party record.
   unit of rea 8cef7d5b (src/game/cm.c); boundaries in decl/text.dcl */
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
extern GAPDATA full_data_602568[];


#define G17_I(m, o) (*(int *)&(m).data[o])
#define G17_S(m, o) (*(short *)&(m).data[o])
/* the block at 0x936420 is 0x24 bytes: the count then the eight player states at +4 (one object, so the
   0x24-byte memset / memcpy in 0x4cd580 and 0x4cdxx cannot run over a neighbour that LINK placed there) */
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
#define cm_LongMsgCut (*(CMCUTINFO *)&cm_LongMsg)  /* 0xa68d40, cm_LongMsg (cm.h: unsigned char[]) as the cutscene record message 0x26 joins */
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
   0x91 (the whole G17MSG body) for any other type. One return per type, as the original: its jump table has an
   entry per type (0x67 direct entries), which VC6 builds only when the cases are distinct statements */

int cm_MessageSize(G17MSG *m)
{
    unsigned char *p;
    int n, k;
    int i;

    switch (m->type) {
    case 3:
        return 0x1c;
    case 0x13:
        return 0x1c;
    case 0x1a:
        return 0x1c;
    case 0x25:
        return 0x1c;
    case 0x2b:
        return 0x1c;
    case 0x34:
        return 0x1c;
    case 0x36:
        return 0x1c;
    case 0x38:
        return 0x1c;
    case 0x47:
        return 0x1c;
    case 0x52:
        return 0x1c;
    case 0x54:
        return 0x1c;
    case 0x5b:
        return 0x1c;
    case 0x5d:
        return 0x1c;
    case 4:
        return 0x11;
    case 8:
        return 0x14;
    case 0x1c:
        return 0x14;
    case 0x28:
        return 0x14;
    case 0x31:
        return 0x14;
    case 0x3b:
        return 0x14;
    case 0x42:
        return 0x14;
    case 0x43:
        return 0x14;
    case 0x4a:
        return 0x14;
    case 0x4d:
        return 0x14;
    case 0x57:
        return 0x14;
    case 0x58:
        return 0x14;
    case 0x69:
        return 0x14;
    case 10:
        return 0x50;
    case 0xb:
        return 0x2a;
    case 0xc:
        return 0x49;
    case 0xf:
        return 0x21;
    case 0x2c:
        return 0x21;
    case 0x11:
        return 0x27;
    case 0x3d:
        return 0x27;
    case 0x12:
        return 0x1e;
    case 0x1e:
        return 0x1e;
    case 0x14:
        return 0x16;
    case 0x15:
        return 0x39;
    case 0x16:
    case 0x17:
        /* @original 0x16 (deltas) and 0x17 (values) set up their head each on its own; the two arms compile
           alike and VC6 merged them, leaving a dead cmp dl,0x16 */
        if (m->type == 0x16) {
            n = 0x15;
            p = m->fields.buf;
        } else {
            n = 0x15;
            p = m->fields.buf;
        }
        for (i = m->fields.n; i > 0; i--) {
            k = p[1] * p[0] + 4;
            n += k;
            p += k;
        }
        return n;
    case 0x18:
        return m->data[4] * 5 + 0x15;
    case 0x19:
        return 0x24;
    case 0x53:
        return 0x24;
    case 0x1d:
        return 0x5f;
    case 0x20:
        return m->data[0] != 0x0e ? 0x8d : 0x11;
    case 0x22:
        return 0x3d;
    case 0xd:
        return 0x18;
    case 0x21:
        return 0x18;
    case 0x23:
        return 0x18;
    case 0x24:
        return 0x18;
    case 0x26:
        return 0x18;
    case 0x27:
        return 0x18;
    case 0x29:
        return 0x18;
    case 0x2a:
        return 0x18;
    case 0x33:
        return 0x18;
    case 0x39:
        return 0x18;
    case 0x3a:
        return 0x18;
    case 0x5a:
        return 0x18;
    case 0x66:
        return 0x18;
    case 0x68:
        return 0x18;
    case 0x2d:
        return 0x1f;
    case 0x45:
        return 0x1f;
    case 0x2e:
        return 0x20;
    case 0x3c:
        return 0x20;
    case 0x2f:
        return 0x3c;
    case 0x30:
        return 0x3c;
    case 0x32:
        return 0x30;
    case 0x35:
        return 0x5d;
    case 0x37:
        return 0x45;
    case 0x3e:
        return 0x34;
    case 0x4f:
        return 0x34;
    case 0x5e:
        return 0x34;
    case 99:
        return 0x34;
    case 0x40:
        return 0x2f;
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
        return 0x90;
    case 0x67:
        return 0x90;
    case 0x4c:
        return 0x35;
    case 0x4e:
        return 0x40;
    case 0x50:
        return 0x44;
    case 0x51:
        return 0x44;
    case 0x55:
        return 0x61;
    case 0x56:
        return 0x38;
    case 0x5c:
        return 0x1a;
    case 0x61:
        return 0x29;
    case 0x62:
        return 0x29;
    case 0x1b:
        return 0x28;
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
            last->next7 = m->seq;
        last = m;
        cm_SendStates[i].state = 0;
        cm_SendStates[i].idx = i;
        if (cm_SendQueue != 0) {
            for (p = cm_SendQueue; p->next != 0; p = p->next)
                ;
            m->prev = p;
            p->next = m;
        } else {
            cm_SendQueue = m;
            m->prev = 0;
        }
        m->next = 0;
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
   @deep s(this): 16 -> 0 ins / 1 byte (shape). Kept: the parts loop as p->next7 = 0; p = r; memcpy(..,
   p->data, 0x80); p->type = 0; (p in edx across the copy, as the original); the first search as r = 0; for
   (q ..) if (match) { r = q; break; } if (r == 0) return 0; (puts m in esi and the next7 CSE in ecx: with
   the plain for / break they tie at prio 8 and m took ecx); the m->next7 == 0 exit as goto fail with fail:
   return 0; at the end (the second search's return 0 then merges into that copy: its loop guard jumps to
   +0x11 as the original). Left (1 byte): the first search's loop guard (empty receive queue) jumps to the
   first search's own return copy (+0x38) in ours, to the top return copy (+0x11) in the original.
   Mechanism (c2tail --il merge): our first-search return 0 block sits right behind the loop bottom (the
   found path's trampoline is removed, so it is entered by fall-through) and the pair merger does not merge
   two fall-through copies of 1 instruction; in the original it was merged into the top copy and the loop
   bottom got an epilogue duplicate. Tried without success: that return as goto fail (the shared block is
   then laid out behind loop 1: 8 ins), as goto a second end label, as if (r != 0) goto found; return 0;,
   if (r != 0) {..} [else] return 0; (tail 38), an explicit q = cm_RecvQueue; if (q == 0) return 0; guard
   with do-while, for(;;) with return inside, while / for-init forms, return (unsigned)r / m->next7
   spellings; the label order at the end changes it (fail2 before fail: 2 bytes). prior: deep s33: the
   original clears p->next7 BEFORE the part's memcpy (mov [edx+0xc],ebx between mov ecx,0x20 and rep movsd;
   n += 0x80 then lands after the copy as the original): with that order (variant pfirst) the layout and
   every instruction order match and only registers are left (32 ins, regs): the original m in esi /
   m->next7's CSE in ecx / p in edx, ours m in ecx / next7 in esi / p in ecx. c2trace --seq of pfirst: p
   (62) is served first and takes ecx; m and the next7 temp tie at prio 8 (tie keys 6 / 4: m first, takes
   ecx too, no conflict with p); next7 then only has ebp / esi / edi (p and the hoisted p->sender temp hold
   ecx / edx over its range: p's piece starts at the function's first line). Tried with pfirst: a seq local
   (= pfirst), m reused as p (= pfirst), climb --rounds 2 from pfirst (best = start). The tree keeps the
   control (16 ins: memcpy before the clear) until the registers follow. */

unsigned int cm_JoinSplit(G17MSG *m)
{
    G17MSG *r, *p, *q;
    int n;

    if (m->next7 == 0)
        goto fail;
    r = 0;
    for (q = cm_RecvQueue; q != 0; q = q->next)
        if (q->sender == m->sender && q->seq == m->next7) {
            r = q;
            break;
        }
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
        p->next7 = 0;
        p = r;
        memcpy(cm_LongMsg + n, p->data, 0x80);
        p->type = 0;
    }
    r->next7 = 0;
    return cm_LongMsgLen;
fail:
    return 0;
}
/* 0x4c39c0, queues a copy of src in the send ring; returns its slot, -1: closed or the slot is still in use */

int cm_QueueSend(G17MSG *src)
{
    G17MSG *m, *p;
    unsigned int i;

    if (net_Conn.closed != 0)
        return -1;
    i = cm_LastSeq + 1;
    m = &cm_SendRing[i & 0x7fff];
    i &= 0x7fff;
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
    if (cm_GroupFirstSeq != cm_GroupEndSeq) {
        for (s = cm_GroupFirstSeq; s < cm_GroupEndSeq; s++) {
            if (cm_SendStates[s & 0x7fff].state == 0) {
                ret = 0;
                break;
            }
            if (cm_SendStates[s & 0x7fff].state == 2)
                ret = -1;
        }
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
   @original as cm_AddFieldValue: the loops walk the parameter v itself (likely VC6's cast-as-lvalue
   `*((T *)v)++`, written through the union q) and the object through its own pointer o; indexed loops let
   VC6 fuse the three pointers into one base with two differences */

int cm_AddFieldDelta(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    union { unsigned char *b; unsigned short *w; short *s; int *i; } p;    /* the entry: size, n, field, the deltas */
    union { void *v; unsigned char *b; short *s; int *i; } q;              /* the caller's values */
    union { unsigned char *b; short *s; int *i; } o;                       /* the object's values */
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
    o.b = (unsigned char *)cm_FieldObject + field;
    switch (size) {
    case 1:
        for (i = 0; i < n; i++) {
            q.v = v;
            *p.b++ = *q.b++ - *o.b++;
            v = q.v;
        }
        break;
    case 2:
        for (i = 0; i < n; i++) {
            q.v = v;
            *p.s++ = *q.s++ - *o.s++;
            v = q.v;
        }
        break;
    case 4:
        for (i = 0; i < n; i++) {
            q.v = v;
            *p.i++ = *q.i++ - *o.i++;
            v = q.v;
        }
        break;
    }
    return 0;
}
/* 0x4c3d00, adds n values of size bytes at field (a plain copy) */

int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    union { unsigned char *b; unsigned short *w; } p;    /* the entry: size, n (bytes), field (a word) */
    unsigned int len;

    if (size != 1 && size != 2 && size != 4)
        return 1;
    len = size * n + 4 + cm_FieldBlockLen;
    if (len >= 0x77)
        return 1;
    p.b = &cm_FieldBlock.buf[cm_FieldBlockLen];
    cm_FieldBlockLen = len;
    cm_FieldBlock.n++;
    *p.b++ = size;
    *p.b++ = n;
    *p.w = field;
    memcpy(p.b + 2, v, size * n);
    return 0;
}
/* 0x4c3d90, as 0x4c3d00, value by value
   @original the copies walk the parameter v itself (likely VC6's cast-as-lvalue `*((T *)v)++`, which our checker
   does not parse; written through the union q): v is loaded after each loop's guard, where a typed local
   pointer per case loads it before the guard */

int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    union { unsigned char *b; unsigned short *w; short *s; int *i; } p;    /* the entry: size, n, field, the values */
    union { void *v; unsigned char *b; short *s; int *i; } q;              /* the caller's values */
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
    case 1:
        for (i = 0; i < n; i++) {
            q.v = v;
            *p.b++ = *q.b++;
            v = q.v;
        }
        break;
    case 2:
        for (i = 0; i < n; i++) {
            q.v = v;
            *p.s++ = *q.s++;
            v = q.v;
        }
        break;
    case 4:
        for (i = 0; i < n; i++) {
            q.v = v;
            *p.i++ = *q.i++;
            v = q.v;
        }
        break;
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
/* 0x4c4080, appends a copy of src to the receive list; 1: no free record (closes the connection) */

int cm_EnqueueReceived(G17MSG *src)
{
    G17MSG *m, *p;

    m = cm_RecvFree;
    if (m == 0) {
        if (net_Conn.s != INVALID_SOCKET)
            net_Close();
        return 1;
    }
    cm_RecvFree = cm_RecvFree->next;
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
   when the connection is closed, fails all of them */

int cm_ExecCommands(void)
{
    G17MSG *m, *t, *next, *grp;
    G17STAT *s;
    int ok, i;

    if (net_Conn.closed != 0) {
        while (cm_RecvQueue != 0) {
            m = cm_RecvQueue;
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
        t = m;
        next = m->next;
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
/* 0x4c4830, message 0x10; returns the send slot (cm_QueueSend) */

int cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d)
{
    G17MSG m;

    m.moveGoods.from = a;
    m.moveGoods.to = b;
    m.type = 0x10;
    m.moveGoods.player = c;
    m.moveGoods.amount = d;
    return cm_QueueSend(&m);
}
/* 0x4c4880, message 0x11; returns the message's slot handle (gm_SyncMarket waits on it) */

int cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f)
{
    G17MSG m;

    m.type = 0x11;
    m.sell.buyer = a;
    m.sell.seller = b;
    m.sell.item = c;
    m.sell.player = e;
    m.sell.count = d;
    m.sell.price = f;
    return cm_QueueSend(&m);
}
/* 0x4c48e0, message 0x12: produce goods; returns the send slot (cm_QueueSend) */

int cm_SendMakeGoods(int a, int b, int c)
{
    G17MSG m;

    m.make.objId = a;
    m.type = 0x12;
    m.make.item = b;
    m.make.runs = c;
    return cm_QueueSend(&m);
}
/* 0x4c4920, message 0x13: the cart route; returns the send slot (cm_QueueSend) */

int cm_SendCartRoute(int a, int b, int c)
{
    G17MSG m;

    m.move.to = a;
    m.type = 0x13;
    m.move.from = b;
    m.move.objId = c;
    return cm_QueueSend(&m);
}
/* 0x4c4960, message 0x14 */

void cm_SendRemoveGoods(int a, int b)
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

int cm_SendSetField(int a, int b, int c, int d, int e)
{
    G17MSG m;

    m.setField.objId = a;
    m.setField.offset = b;
    m.setField.size = c;
    m.type = 0x19;
    m.setField.set = d;
    m.setField.clear = e;
    return cm_QueueSend(&m);
}
/* 0x4c4ac0, sends message 0x1a: adds delta to the float field at the offset field of the character with the netId id */

int cm_SendFloatDelta(int id, int field, float delta)
{
    G17MSG m;

    m.floatDelta.id = id;
    m.type = 0x1a;
    m.floatDelta.field = field;
    m.floatDelta.delta = delta;
    return cm_QueueSend(&m);
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
    buf = mem_Alloc(len + 0xf8, "tmp");
    memcpy(buf, msg, 0xf8);
    memcpy(buf + 0xf8, extra, len);
    cm_SetLongMsg(len + 0xf8, buf);
    mem_Free(buf);
    return cm_QueueSend(&m);
}
/* 0x4c4c60, message 0x1d about p (three AITIMEs, its name from cm_EventNames); p + 0xac as the long message.
   Returns cm_QueueSend's result (the callers' slot; the key is pushed as an int and stored as a byte) */

int cm_SendExSetHE(int k, char *p)
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
    return cm_QueueSend(&m);
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

    m.type = 0x2d;
    m.obj.value = value;
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
/* 0x4c52e0, message 0x35 about character id and building bld; c -1 and not f: the building's object (or -1, flag 1) */

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
            if (gm_IsHomeType((unsigned char *)b) != 0) {
                o = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xfd);
                if (o != 0)
                    m.go.objId = o->objId;
            } else {
                o = (AIOBJ *)gm_GetGebaeudeHauptraum((unsigned char *)b);
                if (o != 0)
                    m.go.objId = o->objId;
            }
        }
        if (o == 0) {
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

int cm_SendOwnership(int bldId, int masterId, int ownerId)
{
    G17MSG m;

    m.type = 0x38;
    m.owner.bldId = bldId;
    m.owner.masterId = masterId;
    m.owner.ownerId = ownerId;
    return cm_QueueSend(&m);
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

int cm_SendHandOverCrimes(int a, int b, int c, int d)
{
    G17MSG m;

    m.type = 0x3c;
    m.handOver.owner = a;
    m.handOver.culprit = b;
    m.handOver.state = c;
    m.handOver.hand = d;
    return cm_QueueSend(&m);
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
/* 0x4c59f0, message 0x49 with a text: spawns a character; returns the send slot (cm_QueueSend) */

int cm_SendSpawnCharacter(unsigned char a, int b, int c, unsigned char d, unsigned char e, int f, const char *s)
{
    G17MSG m;

    m.spawn.kind = a;
    m.spawn.bldId = b;
    m.spawn.objId = c;
    m.spawn.job = d;
    m.type = 0x49;
    m.spawn.title = e;
    m.spawn.item = f;
    strcpy(m.spawn.text, s);
    return cm_QueueSend(&m);
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
    int i;

    m.type = 0x4c;
    if (kind == 0)
        return -1;
    m.geb.owner = owner;
    m.geb.kind = (unsigned char)kind;
    m.geb.pos = *pos;
    for (i = 0; i < 3; i++)
        m.geb.rot[i] = rot[i];
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
/* 0x4c5c40, message 0x50 (0x2c bytes of p, the id *cm_CurrentFight). p is the entry's address passed as an integer:
   the callers' entry locals count as escaped in the original (cm_FightFrame copies into its entry before the pointer
   loads behind it; cm_RunCutOrders orders three stores to m the same way), which a pointer parameter of this body
   does not give (VC6 knows a function defined above does not let p escape) */

int cm_SendCutEntry(int a, int p)
{
    G17MSG m;

    m.type = 0x50;
    m.cutEntry.playerId = a;
    if (cm_CurrentFight != 0)
        m.cutEntry.fightId = *cm_CurrentFight;
    else
        m.cutEntry.fightId = -1;
    memcpy(&m.cutEntry.entry, (void *)p, sizeof(m.cutEntry.entry));
    return cm_QueueSend(&m);
}
/* 0x4c5ca0, message 0x51, as 0x4c5c40 */

int cm_SendCutEntryUpdate(int a, int p)
{
    G17MSG m;

    m.type = 0x51;
    m.cutEntry.playerId = a;
    if (cm_CurrentFight != 0)
        m.cutEntry.fightId = *cm_CurrentFight;
    else
        m.cutEntry.fightId = -1;
    memcpy(&m.cutEntry.entry, (void *)p, sizeof(m.cutEntry.entry));
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
   of the fighter c's body and its health; for an attack (2) also the target's current health */

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
        m.fight.pos.x = b->pos.x;
        m.fight.pos.y = b->pos.y;
        m.fight.pos.z = b->pos.z;
        m.fight.rot.x = b->rotation.x;
        m.fight.rot.y = b->rotation.y;
        m.fight.rot.z = b->rotation.z;
        m.fight.health = c->v024;
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

int cm_SendGraveOrder(void *p)
{
    G17MSG m;

    m.type = 0x60;
    memcpy(m.data, p, 0x1f);
    return cm_QueueSend(&m);
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

int cm_SendRaiseDead(void *p)
{
    G17MSG m;

    m.type = 0x62;
    memcpy(m.data, p, 0x19);
    return cm_QueueSend(&m);
}
/* 0x4c6170, message 99 (0x24 bytes of p) */

int cm_SendCreateAlchemist(void *p)
{
    G17MSG m;

    m.type = 99;
    memcpy(m.data, p, 0x24);
    return cm_QueueSend(&m);
}
/* 0x4c61b0, message 100 (0x18 bytes of p) */

int cm_SendCommand(void *p)
{
    G17MSG m;

    m.type = 100;
    memcpy(m.data, p, 0x18);
    return cm_QueueSend(&m);
}
/* 0x4c61f0, sends message 0x65: the master of party p becomes the character q (its netId; its handler 0x4cd130
   sets the party record's host); returns the message's send slot (cm_QueueSend) */

int cm_SendMasterChange(G28REC *p, AICHAR *q)
{
    G17MSG m;

    m.type = 0x65;
    m.master.partyId = p->id;
    m.master.masterId = q->netId;
    return cm_QueueSend(&m);
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
    RTPOINT buf[8];
    RTPOINT *next;
    int i, n;

    n = rt_PointCount(pts);
    m.type = 0x68;
    m.route.objId = o->objId;
    m.route.count = n;
    for (i = 0; i < n; i++) {
        next = pts->next;
        buf[i] = *pts;
        pts = next;
    }
    cm_SetLongMsg(m.route.count << 6, (char *)buf);
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
    ORDBLD *bb;                 /* the buyer's building: its own slot in the original, not b again */

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
        if (gm_FindById((int *)&bb, (int *)&o, (int *)&c, m->sell.buyer) == 0)
            return 1;
        if (bb != 0) {
            if (bb->owner != 0xffff)
                r = (ORDCHAR *)&ai_Chars[bb->owner];
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
    union {
        unsigned char *b;
        unsigned short *w;
        short *s;
        int *i;
    } p;
    ORDCHAR *r;
    unsigned int i, j, n;
    int size, id;

    m->fields.id = cm_ResolveId((int)m, m->fields.id);
    if (gm_FindById((int *)&b, (int *)&o, (int *)&c, m->fields.id) == 0)
        return 1;
    if (b != 0)
        base = (char *)b;
    else if (o != 0)
        base = (char *)o;
    else
        base = (char *)c;
    p.b = m->fields.buf;
    for (i = 0; i < m->fields.n; i++) {
        size = (char)*p.b++;
        n = (char)*p.b++;
        a = base + *p.w++;
        if (c != 0) {
            if (a == (char *)&c->netId)
                return 1;
            if (a == (char *)&c->partner) {
                id = *p.i + c->partner;
                r = (ORDCHAR *)gm_FindChar(id);
                if (r == 0)
                    return 1;
                if (r->partner != -1 && r->partner != c->netId)
                    return 1;
            }
        }
        switch (size) {
        case 1:
            for (j = 0; j < n; j++)
                p.b++;
            break;
        case 2:
            for (j = 0; j < n; j++)
                p.s++;
            break;
        case 4:
            for (j = 0; j < n; j++)
                p.i++;
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
    if (b != 0)
        base = (char *)b;
    else if (o != 0)
        base = (char *)o;
    else
        base = (char *)c;
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
            c = &ai_Chars[k];
            if (c != 0 && c != ai_PlayerChar && c->health != 0 && c->kind != 0xf)
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

    if (m->ready.partyId == -1)
        return 1;
    for (i = 0; i < 0x10; i++) {
        c = (AICHAR *)gm_FindChar(m->ready.members[i]);
        if (c != 0 && (c->kind == 6 || c->kind == 7) && c->partyId != -1 && c->partyId != m->ready.partyId)
            return 1;
    }
    return 0;
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
            if (ai_Chars[k].id != 0xffff && ai_Chars[k].netId == id)
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
    cm_LastCharId = ai_Chars[k].netId;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)&ai_Chars[k];
    }
    return 0;
}
/* 0x4c6cd0, handler of type 0xc: creates a named character (gm_CreateCharacter, type 6 with s, else 7) with its first
   and family name; a player character's record and its relatives[1] get the family name too. The generator is
   seeded with the message's dword at 0x12 (inside age / job / ref54), not with its seed: as the original does */

int cm_ExCreateNamedCharacter(G17MSG *m, G17STAT *s)
{
    AICHAR *c;
    AIFIN *t;
    char *r;
    unsigned int k;
    int type;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = m->named.objectId;
    else
        m->named.seed = rand();
    util_Seed(m->named.seedRead);
    if (s != 0)
        type = 6;
    else
        type = 7;
    k = gm_CreateCharacter(type, m->named.parent1, m->named.parent2, m->named.age, 0, m->named.job, 0, m->named.a8);
    if (k == 0xffff)
        return 1;
    cm_LastCharId = ai_Chars[k].netId;
    strncpy(ai_Chars[k].first, m->named.first, 0x10);
    c = &ai_Chars[k];
    t = (AIFIN *)gm_PlayerRecord((char *)c);
    if (t != 0) {
        t->w00 = ai_Chars[k].w050;
        strncpy(t->family, m->named.family, 0x10);
        strncpy(ai_Chars[k].family, m->named.family, 0x10);
        r = gm_FindChar(ai_Chars[k].relatives[1]);
        if (r != 0)
            strncpy(((AICHAR *)r)->family, m->named.family, 0x10);
        t->camAngle = -1.0f;
    }
    ai_Chars[k].ref54 = m->named.ref54;
    ai_Chars[k].b00c = m->named.b00c;
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
            s->value = (int)&ai_Chars[k];
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
        if (gm_TakeObjekt(m->moveGoods.to, ai_PlayerSlots[m->moveGoods.player].moneyGood, m->moveGoods.amount) == 0)
            return 1;
    }
    if (m->moveGoods.from != -1) {
        m->moveGoods.from = cm_ResolveId((int)m, m->moveGoods.from);
        if (gm_AddObjekt(m->moveGoods.from, ai_PlayerSlots[m->moveGoods.player].moneyGood, m->moveGoods.amount) == 0)
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
        gm_AddNegative(m->moveGoods.to, ai_PlayerSlots[m->moveGoods.player].moneyGood, m->moveGoods.amount);
    }
    if (m->moveGoods.from != -1) {
        m->moveGoods.from = cm_ResolveId((int)m, m->moveGoods.from);
        gm_AddObjekt(m->moveGoods.from, ai_PlayerSlots[m->moveGoods.player].moneyGood, m->moveGoods.amount);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c70b0, handler of type 0x11, "cm_ExSellObjekt": moves count of the goods item from the seller (a building's,
   an object's or a character's list, gm_FindById) to the buyer; with a price, the buyer pays count * price in the
   money good of the player slot to the seller (gm_TakeObjekt / gm_AddObjekt) and both book it. A store (an object of
   kind 0x2a / 0x116) of the own player character gives the status text of its building and a sound
   @deep s(this): the whole rest of the function (gm_AllocRaum push copy at 0x4c77a3, the goods-record regs
   at 0x4c7737, cm_LastObjectId 0x4c7663, the gm_TakeObjekt argument order 0x4c767f, the y2/ys copies at
   0x4c76b8 / 0x4c7807, txt_Format at 0x4c7847) is ONE allocation cascade: the bulk split (142 blocks,
   regions) queues its candidates in another order. Probe that matches all of it: a fresh candidate for
   m->sell.buyer at the second buyer block, 'short t; .. if ((t = m->sell.buyer) != -1) { ..
   gm_CreateObject(t, ..)' = 5 ins (only its own mov dx / cmp dx / movsx left; climb --rounds 3 found it,
   nothing better). c2trace: control's m->sell.buyer at 0x4c7640 is a piece (temp#2, edx, prio 13) of the
   CSE web first created at 'if (m->sell.buyer != -1)' of the FIRST buyer block (3062); the short local is
   a new candidate with the same prio 13 created later (ties: later-created first), which reorders the
   split queue. So the original's value at the second buyer test is a candidate created there, not a piece
   of the first block's web. No lever (= control): an int / long / unsigned local for it (folded), assigned
   in the test or before, the third use at the s block (160), gm_AllocRaum forms (nested if / else return 1
   = same ins, early x2 == 0 return 112, || 143, ternary / else o2 = 0 = control), a block local or a
   separate local / x2 for the status building (128/129), switch or locals for gd->type (= control or
   worse), (char)m in cm_ResolveId (386+). Next: what kills or splits the m->sell.buyer CSE between 3063
   and 3139 in the original (a call taking m, a store through m, the field read through another view), or a
   prototype of gm_CreateObject with a short id (f3gmt; 22 callers). prior: size queue s17 (127 -> 103 ins,
   raw 2089 -> 2059 vs 2061: padded size): the two gotos gone, each its plain form: the room check as 'if
   (store kinds) y2 = y; if ((y2 != 0 || y->type == 0x1dd) && gm_FreeCapacity(..) < count) return 1;' (121;
   two FreeCapacity calls 144 / 145), the money goto as if (ys != 0 && r2 != 0) (= control); the buyer's
   room 'if (x2 != 0) o2 = gm_AllocRaum(..); if (o2 == 0) return 1;' (112, 2071) and the seller's 'else if
   (x1 != 0) RemoveFromBuilding else return 1' (103, 2059: its failure jumps to a shared return-1 copy as
   the original). Left: register rotation from the gm_TakeObjekt call on (0x4c768b: the original pushes
   count * price first), the ys type test's mov ecx,eax copy, the goods-record address add order
   (0x4c774f). */

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
                r1 = &ai_Chars[x1->owner];
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
                r2 = &ai_Chars[x2->owner];
            else
                r2 = 0;
        } else if (y != 0) {
            if (y->type == 0x2a || y->type == 0x1db || y->type == 0x1dc || y->type == 0x116)
                y2 = y;
            if ((y2 != 0 || y->type == 0x1dd)
                && gm_FreeCapacity((short *)y, m->sell.item, m->sell.count) < m->sell.count)
                return 1;
            q = &y->list;
            r2 = (AICHAR *)gm_ObjectBuildingOwner((unsigned short *)y);
        } else {
            q = &z->inventory;
            r2 = z;
        }
        if (ai_Goods[m->sell.item].type == 0x1d)
            o2 = 0;
        else
            o2 = (AIOBJ *)gm_Query((void *)*q, 1, 0, m->sell.item);
        if (m->sell.price != 0 && r2 != 0) {
            if (gm_SumSlotAmounts((void *)r2->inventory, m->sell.player) < m->sell.count * m->sell.price)
                return 1;
        }
    }
    if (ys != 0 && r2 != 0) {
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
            if (ai_Goods[m->sell.item].type != 2 && ai_Goods[m->sell.item].type != 6)
                gm_RemoveObject(p, m->sell.item);
            else if (x1 != 0)
                gm_RemoveObjectFromBuilding((int)x1, m->sell.item);
            else
                return 1;
        }
        if (m->sell.price != 0) {
            gm_AddObjekt(r1->netId, ai_PlayerSlots[m->sell.player].moneyGood, m->sell.count * m->sell.price);
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
        } else if (ai_Goods[m->sell.item].type == 2 || ai_Goods[m->sell.item].type == 6) {
            if (x2 != 0)
                o2 = (AIOBJ *)gm_AllocRaum((unsigned char *)x2, m->sell.item);
            if (o2 == 0)
                return 1;
        } else {
            o2 = (AIOBJ *)gm_CreateObject(m->sell.buyer, m->sell.item, m->sell.count);
            if (o2 == 0)
                return 1;
            if (y2 != 0) {
                AIGOOD *gd = &ai_Goods[m->sell.item];

                o2->count++;
                if ((gd->type == 0x17 || gd->type == 0x25)
                    && (y2->type == 0x2a || y2->type == 0x116) && r2 != 0 && (r2->kind == 6 || r2->kind == 7))
                    o2->reserve = 4;
            }
        }
        cm_LastObjectId = o2->objId;
        if (m->sell.price != 0 && r2 != 0) {
            gm_TakeObjekt(r2->netId, ai_PlayerSlots[m->sell.player].moneyGood, m->sell.count * m->sell.price);
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
            sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
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
    g = &ai_Goods[m->make.item];
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
                if (i != j && ai_Chars[i].id != 0xffff && ai_Chars[j].id != 0xffff) {
                    if (v > hi)
                        v += dn;
                    else if (v < lo)
                        v += dp;
                    v = min(max(v, -0x7f), 0x7f);
                    ai_Relations[i][j] = (signed char)v;
                }
            }
        }
    } else {
        for (ci = 0; ci < 0x300; ci++)
            if (ai_Chars[ci].netId == to)
                break;
        if (ci >= 0x300 || ai_Chars[ci].id == 0xffff)
            return 1;
        if (kind == 3) {
            v = ai_Chars[ci].partner;
            for (j = 0; j < 0x300; j++)
                if (ai_Chars[j].netId != v)
                    ai_RelationMemory[j][ci] = (signed char)(int)(ai_RelationMemory[j][ci] * m->rel.scale);
        } else {
            for (cj = 0; cj < 0x300; cj++)
                if (ai_Chars[cj].netId == from)
                    break;
            if (cj >= 0x300 || ai_Chars[cj].id == 0xffff)
                return 1;
            if (kind == 2) {
                v = ai_Relations[cj][ci];
                ai_RelationMemory[cj][ci] = 0;
                v += m->rel.delta;
                v = min(max(-0x7f, v), 0x7f);
                ai_Relations[cj][ci] = (signed char)v;
            } else if (kind == 1) {
                v = ai_Relations[cj][ci] + m->rel.delta;
                v = min(max(-0x7f, v), 0x7f);
                ai_Relations[cj][ci] = (signed char)v;
                if (m->rel.delta >= 0)
                    v /= 2;
                v += ai_RelationMemory[cj][ci];
                ai_RelationMemory[cj][ci] = (signed char)min(max(-0x7f, v), 0x7f);
            } else if (kind == 0) {
                v = ai_Relations[cj][ci] + m->rel.delta;
                v = min(max(-0x7f, v), 0x7f);
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
            if (c->kind == 7 || (c->kind != 6 && s == 0))
                return 1;
        }
        if (s == 0 && (msg.flags & 2))
            msg.flags = (msg.flags & ~2) | 4;
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
            for (c = (AICHAR *)ai_Chars; (int)c < (int)&ai_Chars[0x300]; c++) {
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
            ai_TimeCopy = ai_Time;
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
                    ai_SetFirstMission(ai_Chars[ai_PlayerId].netId, msg.sys.arg[8]);
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
                if (ai_Chars[ai_PlayerId].partyId != msg.sys.arg[0] || msg.sys.arg[0] == -1)
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
        rec = &ai_Crimes[slot];
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
            if (j->id4 == ai_Crimes[slot].culprit) {
                if (gv_AddEvidence(rec->id, ai_Chars[j->chr].netId) == 0)
                    break;
                if (s != 0) {
                    c = (AICHAR *)gm_FindChar(j->id4);
                    if (c != 0)
                        ai_NotifyPlayerOfChar((unsigned short *)c, (unsigned short *)&ai_Chars[j->chr]);
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
        if (cm_CutInfos[i].playerId == m->cut.playerId) {
            if (cm_CurrentFight != 0 && *cm_CurrentFight == m->cut.fightId) {
                cm_CutInfos[i] = cm_LongMsgCut;
                cm_CutInfos[i].recvFlag = 0;
                sprintf(buf, "cm_ExSendCutInfo: PlayerId:%i Valid:%i  Ready:%i  Time:%i", cm_CutInfos[i].playerId,
                        cm_CutInfos[i].valid, cm_CutInfos[i].ready, d3_Time);
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
        s->value = (int)&cm_CutInfos[i];
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
    } else
        ret = 1;
    if (o != 0) {
        o->seed = d->seed;
        if (s != 0) {
            s->state = 1;
            s->kind = 9;
            s->value = (int)o;
        }
    }
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
                d3if_SelectScene(init_BuildingUniverse[(unsigned char)ai_BTypes[b->type].kind], 1);
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
            d3if_SelectScene(init_BuildingUniverse[(unsigned char)ai_BTypes[b->type].kind], 1);
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
    if (m->place.objId != -1)
        i = init_ObjectSceneUniverse(i);
    else if (m->place.bldId != -1)
        i = init_BuildingSceneUniverse(b);
    else
        i = 0;
    if (i == -1)
        return 2;
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
            strcpy(r->why, "cm_ExChrWalkToDummy");
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
    }
    if (q != 0 && q->id != 0xfd)
        k = init_ObjectSceneUniverse((unsigned short *)q);
    else if (b != 0 && gm_IsHomeType((unsigned char *)b) != 0) {
        k = init_BuildingSceneUniverse((unsigned char *)b);
        q = (AIOBJ *)gm_Query((void *)b->objList, 1, 0, 0xfd);
    } else
        k = 0;
    if (k >= 0) {
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
    }
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
   @deep s2: g = &ai_Goods[q->id] named at both gate sites (cm_ExChrUseGates's form) 319 -> 273; door
   block stores x,y,z then hasPos (the original jumps into the other branch's tail at the z store); goto
   done -> q==0 chained as if/else-if (same code); memcpy -> strcpy of TRANSPORTER_ENTRY (same code). Tree:
   draft 275 ins / 1622 bytes. Hunk 1 (every scratch register behind it rotates): the inline strcpy at 4648
   (original add ecx,0x3d; sub eax,ecx; lea edx,[eax+0x70], ours lea edx,[eax+0x70]; add ecx,0x3d; sub
   edx,ecx). c2trace: in ours r->why (r+0x70) is one CSE temp (temp#501, 4 refs per copy, 13 webs); in the
   original form r has 5 refs and the temp 2, i.e. VC6 did not make r+0x70 a temp. flip.py: only a
   duplicated decided test on a FIELD load flips it (dup 'if (p->group != -1)' before cb's query -> shape 8
   ins; dup objId tests 14/46; dup p->transport/m->go.text other code); dup of register tests (p, to, s,
   cb) and every other natural form do nothing; c2trace allocation identical, so it is decided before
   allocation (CSE of the address). With the dup, left 8 ins = sib order of 4 copies (4665,4675 ours
   [edx+ecx]/[edx+eax], original [ecx+edx]/[eax+edx]; 4704,4711 the reverse); flip on the dup base finds no
   single mutation for them (one meaning-changing hit: del the 0xfd query). climb (no inexpr) on both
   bases: nothing. Not levers (= control): every dst/src spelling of the strcpy ((char*)r+0x70, &r->why[0],
   args+0x10, (char*)m+0x3d), a G17GOTOMSG *go local (4 forms), declaration order (all moves), renaming r
   per site, the strcpy-entry/memcpy, two-ifs/else-block for the room-id errors. Worse: cb init as
   else/ternary (frame), nested p->group != -1 around home/else-if, else-if operand swap, to->id swap, r !=
   0 / else return in the home branch (307), i = 0 first (261, other code). Beware climb's inexpr:i@155 (68
   ins): a dangling-else meaning change, not a lead. prior: read whole against the original and fixed the
   meaning (NoCase door lookup on the target, the out-of-building branch compares with the 0x44 building, o
   kept from function start, the last gate through the start building cb, the objId/universe test as
   if/else, if/else FindObject calls VC6 merges, hm loaded before d3_PointToWorld, 16-byte vector frame).
   Frame equal now; left: instruction choice/scheduling of the inline strcpy into r->why (ours lea
   edx,[eax+0x70]; add ecx,0x3d; sub edx,ecx | the original add ecx,0x3d; sub eax,ecx; lea edx,[eax+0x70])
   at every copy, ai_Goods index lea eax,[eax+edx+1] | add eax,edx; inc eax, the text test add eax,0x1d;
   cmp byte [eax],0 | mov cl,[edx+0x1d]; lea eax,[edx+0x1d]; test cl,cl, and loads scheduled before/after
   pushes (0x4ca0ec, 0x4ca392, 0x4ca43c); dst spellings (char*)r+0x70, r->args+0x10, (char*)m+0x3d src
   tried, no change */
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
    AIGOOD *g;

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
                g = &ai_Goods[q->id];
                sprintf(gate, "dummy_gate_ob_%s", strupr(g->name));
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
                r->move.pos.z = v.z;
                r->move.hasPos = 1;
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
            if (q == 0)
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
            } else if (q->id == 0xfd) {
                if (to->id != p->group) {
                    k = init_BuildingSceneUniverse((unsigned char *)to);
                    if (k < 0)
                        return 0;
                    ch_AddAction(p, 0x38, 0);
                    r = ch_AddAction(p, 0x33, k, to->id, q->objId);
                    if (r != 0) {
                        strcpy(r->why, m->go.reason);
                        strcpy(r->u.name, "dummy_TRANSPORTER_ENTRY");
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
                g = &ai_Goods[q->id];
                sprintf(gate, "dummy_gate_ob_%s", strupr(g->name));
                if (cb != 0) {
                    r = ch_UseGate(p, "dummy_EINGANG", gate, i, cb->id, q->objId);
                    if (r != 0)
                        strcpy(r->why, m->go.reason);
                }
            }
        }
    }
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
        g = &ai_Goods[q->id];
        k = init_ObjectSceneUniverse((unsigned short *)q);
        sprintf(gate2, "dummy_gate_ob_%s", strupr(g->name));
    }
    g = &ai_Goods[r->id];
    sprintf(gate, "dummy_gate_ob_%s", strupr(g->name));
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
    gm_AssignBuilding(b, c2->id, c1->id);
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
    if (ai_BTypes[b->type].kind == 1) {
        ord_GetKindRecord(b->type, kind);
        if (b->residents >= (unsigned short)kind[0])
            return 1;
        b->residents++;
    }
    o = c->bld2;
    if (o != 0) {
        if (ai_BTypes[o->type].kind == 1 && o->residents > 0)
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
    char path[0x104];
    char found[0x100];
    D3VECTOR4 v;
    D3VECTOR rot;
    D3VECTOR dir;
    AIBLD *b;
    AIBTYPE *t;
    D3OBJECT *o;
    EVTSCRIPT *sc;
    int i, save, season, size;

    if (net_Conn.s != (unsigned int)-1)
        ai_NextObjectId = m->upgrade.nextObjId;
    b = (AIBLD *)gm_BuildQuery(1, 1, m->upgrade.bldId);
    if (b == 0)
        return 1;
    t = &ai_BTypes[b->type];
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
    strcat(name, ai_BTypes[b->type].name);
    strupr(name + 2);
    for (i = 0; i < 4; i++) {
        if (t->kind == 0x21) {
            if (ai_Chars[b->owner].sex == 1)
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
            d3_PointToWorld((D3OBJECT *)b->object, &((D3OBJECT *)b->object)->pos, (D3VECTOR *)&v);
            o = d3_LoadGroup(path, 0, 0);
            if (o == 0) {
                sprintf(name, "cm_ExGebUpgrade(): Could not load 3D-Object-Group '%s'...", path);
                err_Message(name);
            } else {
                float radius, key;

                d3_DirToWorld((struct D3OBJECT *)b->object, &d3_ZAxis, &dir);
                rot.x = 0;
                rot.y = 0;
                rot.z = 0;
                rot.y = d3_VecAngle(&d3_ZAxis, &dir);
                d3_SetObjectPos(o, (D3VECTOR *)&v);
                d3_SetObjectRot(o, &rot);
                ai_DropObjectAt(o, (D3VECTOR *)&v);
                if (d3_QuadTree != 0)
                    d3q_RemoveObject((D3OBJECT *)b->object, (D3QNODE *)d3_QuadTree);
                d3if_Delete((D3OBJECT *)b->object);
                if (o->info == 0)
                    d3_InflateObject(o);
                if (o->material != 0) {
                    if (d3t_GetBoundingSphere(o, d3_Camera, (D3VECTOR *)&v, &radius) != 0) {
                        size = (int)(radius * 1.8);
                        sc = evt_Load("Effekte/geb_upgrade.esc");
                        if (sc != 0) {
                            evt_RunScriptArgs(sc, 6, (int)v.x, (int)v.y, (int)v.z, (int)radius, size, o);
                            sc->group = 0;
                        }
                    }
                }
                init_HookObject(o, b, 2);
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
    for (bld = &ai_Chars[0].bld; (int)bld < (int)&ai_Chars[0x300].bld;
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
    memcpy(&c->flags1b4, &b->workSet, sizeof(b->workSet));
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
        s->state = (r == 0) + 1;
        s->kind = 0;
        s->value = 0;
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
        gm_PlaceOnStage(ai_Chars[id].netId, b->id, t->objId, 0, 0);
    else
        gm_PlaceOnStage(ai_Chars[id].netId, b->id, -1, 0, 0);
    ai_Chars[id].bld = 0;
    ai_Chars[id].kind = 0xd;
    if (m->spawn.item != 0) {
        gm_CreateObject(ai_Chars[id].netId, m->spawn.item, 1);
        cm_UpdateHandObject((char *)&ai_Chars[id]);
    }
    cm_LastCharId = ai_Chars[id].netId;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)&ai_Chars[id];
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

    p = strstr(cm_ChatLog, "$A");
    if (p != 0) {
        do {
            p += 2;
            n++;
            p = strstr(p, "$A");
        } while (p != 0);
        if (n > 5)
            cm_ChatLog[0] = 0;
    }
    for (i = 0; i < 8; i++) {
        if (m->chat.to[i] == ai_Chars[ai_PlayerId].netId) {
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
    {
        char name[MAX_PATH];

        strcpy(name, "gb_");
        strcat(name, ai_BTypes[b->type].name);
        strupr(name + 2);
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
                    init_HookObject(o, b, 2);
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

    p = ch_FindByName(m->objCell.name);
    if (p == 0)
        return 1;
    if (m->objCell.name[0] != 0)
        q = ch_FindByName(m->objCell.name2);
    if (s != 0) {
        r = ch_AddAction(p, 0x2d, m->objCell.x, m->objCell.z, d3t_SceneIndex(p->scene));
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
    ch_AddAction(p, 0x3a, m->objCell.x, m->objCell.z, d3t_SceneIndex(p->scene));
    return 0;
}
/* 0x4cc110, message 0x4f (no sender in the sources; reasoned: the path cells of a walk): writes up to two cells
   of the named character's walk action 0x3a into its path, from the cell "from" up to "to"; 1 when the name is
   not found, 2 when the character runs no 0x3a action */

int cm_ExWalkPath(G17MSG *m, G17STAT *s)
{
    CHARACTER *p;
    CHACTION *a;
    int idx, i;

    p = ch_FindByName(((CMWALKPATHMSG *)m->data)->name);
    if (p == 0)
        return 1;
    a = p->actions;
    if (a == 0 || a->type != 0x3a)
        return 2;
    i = 0;
    if (a->u.walk.path != 0) {
        a->u.walk.pathLen = ((CMWALKPATHMSG *)m->data)->to;
        idx = ((CMWALKPATHMSG *)m->data)->from;
        for (; i < 2 && idx < a->u.walk.pathLen; i++, idx++) {
            ((unsigned char *)a->u.walk.path)[idx * 2] = ((CMWALKPATHMSG *)m->data)->cells[i * 2];
            ((unsigned char *)a->u.walk.path)[idx * 2 + 1] = ((CMWALKPATHMSG *)m->data)->cells[i * 2 + 1];
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

    if (m->cutEntry.fightId != ai_Chars[ai_PlayerId].partyId)
        return 1;
    for (i = 0; i < 16; i++) {
        if (cm_CutInfos[i].playerId == m->cutEntry.playerId) {
            rec = &cm_CutInfos[i];
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

    if (m->cutEntry.fightId != ai_Chars[ai_PlayerId].partyId)
        return 1;
    for (i = 0; i < 16; i++) {
        if (cm_CutInfos[i].playerId == m->cutEntry.playerId) {
            r = (CMCUTENTRY *)cm_CutEntrySlot((char *)&cm_CutInfos[i], cm_FindFighter(m->cutEntry.entry.charId));
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
        if (ai_Time.year - q->cornerYear > 1) {
            q->recs[0] = -1;
            q->recs[1] = -1;
            q->recs[2] = -1;
            q->cornerYear = ai_Time.year - 1;
        } else
            return 1;
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
            ai_ShowPurchaseNews(c, (char *)&ai_Chars[ai_PlayerId], (int)b, m->script.price);
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
            ch = &ai_Chars[b->owner];
            if (ch != 0 && (char *)ch != (char *)ai_PlayerChar && ch->health != 0 && ch->kind != 0xf)
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
    c->heldRounds = v >= 0xff ? 0xff : v;
    c->heldRounds = c->heldRounds > 0 ? c->heldRounds : 0;
    if (c->id == ai_PlayerId && ai_GameState != 0 && c->heldRounds > 0)
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
    if (r != 0)
        return 0;
    return 1;
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
    k = c->skills[k] + m->skill.delta;
    f = (float)k;
    /* MAX(MIN(f, 252), 0) as macros expand it: the MIN is evaluated again for the result */
    f = (f < 252.0f ? f : 252.0f) > 0.0f ? (f < 252.0f ? f : 252.0f) : 0.0f;
    c->skills[m->skill.skill] = (unsigned char)f;
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
        s->value = (int)&ai_Chars[id];
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
    id = ahm_CreateAlchemist((AHMCREATEMSG *)m->data);
    if (id == 0xffff)
        return -1;
    if (s != 0) {
        s->state = 1;
        s->kind = 1;
        s->value = (int)&ai_Chars[id];
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
    prev = 0;
    n = m->route.count;
    memcpy(pts, cm_LongMsg, n * sizeof(RTPOINT));
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
    CMLOBBYPLAYER *p;

    p = cm_LobbyFindPlayer(id);
    if (p != 0)
        sprintf(cm_LobbyChat[cm_LobbyChatNext].name, "%s %s", p->firstName, p->familyName);
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

    if (cm_LobbyChatCount != 0) {
        i = cm_LobbyChatNext - cm_LobbyChatCount;
        cm_LobbyChatPos = i;
        if (i < 0) {
            i = cm_LobbyChatNext + 1;
            cm_LobbyChatPos = i;
        }
        return &cm_LobbyChat[i];
    }
    return 0;
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
/* 0x4cd510, all players of the list (cm_LobbyList[1 .. 8]) have state 1 and their number is the one asked for */

int cm_LobbyAllReady()
{
    unsigned int n;
    int cnt, i;
    CMLOBBYPLAYER *q;

    if (cm_NetGame.mode == 0x3f)
        n = cm_NetGame.hdr.players;
    else
        n = cm_NetGame.hdr.players;
    if (cm_LobbyList[0] < n)
        return 0;
    cnt = 0;
    for (i = 0; i < 8; i++) {
        if (cm_LobbyList[1 + i] != 0) {
            q = cm_LobbyFindPlayer(cm_LobbyList[1 + i]);
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
    cm_LobbyStateFlags.open = 1;
    cm_LobbyPlayerId = lobbyId;
    cm_LobbyHelloValue = value;
    cm_LobbyStateFlags.closed = 0;
}
/* 0x4cd5d0, the lobby's player list cm_LobbyList (the count, then the eight player ids) */

unsigned int *cm_LobbyPlayerList()
{
    return cm_LobbyList;
}
/* 0x4cd5e0, the lobby record (CMLOBBYPLAYER) of the player id, else a free one; 0 for id 0 */

CMLOBBYPLAYER *cm_LobbyFindPlayer(int id)
{
    return (CMLOBBYPLAYER *)cm_LobbyRecord(id);
}
/* 0x4cd5f0, this player's lobby record (the one of the id cm_LobbyPlayerId) */

CMLOBBYPLAYER *cm_LobbyMyPlayer()
{
    return (CMLOBBYPLAYER *)cm_LobbyRecord(cm_LobbyPlayerId);
}
/* 0x4cd600, the lobby state: [0] the lobby is open (cm_LobbyStart), [1] a lobby message came in (cm_ExLobby) */

int *cm_LobbyState()
{
    return &cm_LobbyStateFlags.open;
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
    struct G17LOBBYSTATE s;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    s.id = cm_LobbyPlayerId;
    s.value = v;
    m.lobby.kind = 1;
    m.lobby.state = s;
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
    m.type = 0x5f;
    t.from = cm_LobbyPlayerId;
    t.to = to;
    strncpy(t.text, text, 0x3f);
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
            cm_LobbyStateFlags.closed = 1;
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
    cm_LobbyStateFlags.changed = 1;
    return 0;
}

/* dcp unit split: the declarations that moved to cmfight (copies: drop what cm does not use) */
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

extern unsigned int ai_MissionSeed;
extern char * sv_SelectedObject;
extern char cm_ChatLog[2048];
extern int cm_SaveAcks[8][2];
/* ai_CharSex: view of ai_Chars (tools/data_views.py) */
/* ai_Unk770cec: view of ai_Chars (tools/data_views.py) */
extern int ai_Evidence[0x800][2];
extern int cm_SavedScene;
extern int cm_DuelRight;
extern int cm_DuelLeft;
extern int cm_DuelVariants[128];
extern int cm_OrderTicks;
extern unsigned short cm_DisabledObjs[768];
extern unsigned int cut_RandState;
extern CMKIND cm_Kinds[12];
extern char cm_SceneState[1152];
extern int cm_SceneChars[16];
extern int cm_SavedNoGroup;
extern float cm_OrderChances[0xb4];
extern char * cm_PointList[32];
extern CMPORTRAIT cm_Portraits[6];
extern G18POP cm_DamagePops[16];
extern char cm_Fighters[21376];
extern struct D3OBJECT *cm_FlagObjects[32];
extern float cm_FightCellScale;
extern int cm_CharInfoWins[32];
extern G18LOG cm_LobbyChat[15];
extern int cm_LobbyChatNext;
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
/* txt_HonourTitles: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_JobNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_TitleNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_OfficeNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_FaithNames: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_SexNames: view of txt_Ptrs (build/mk_txt_views.py) */
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
const int cm_SpeedTicks[] = { 1155, 937, 770, 577, 385, 0 };  /* 0x5edc10 */
int cm_SpeedTick = 770;  /* 0x60bd80 */
unsigned int cm_SpeedLevel = 2;  /* 0x60bd84, = 2 (0..4) */
int cm_LastCharId = -1;  /* 0x60bd88, = -1 */
int cm_LastBuildingId = -1;  /* 0x60bd8c, = -1 */
int cm_LastObjectId = -1;  /* 0x60bd90, = -1 */
int cm_LeftPlayerId = -1;  /* 0x60bd94, = -1 */
int (*cm_Handlers[0x6a])(G17MSG *m, G17STAT *s) = { cm_ExNothing, cm_ExGroupPassed, ai_AlwaysTrue, cm_ExNothing, cm_ExNetFlags, cm_ExNothing, cm_ExNothing, cm_ExNothing, cm_ExLoadSize, cm_ExLoadBlock, cm_ExCreateBuilding, cm_ExCreateCharacter, cm_ExCreateNamedCharacter, cm_ExTakeOverCharacter, cm_ExDeleteObject, cm_ExMoveGoods, cm_ExMoveGoodsUnchecked, cm_ExSellObjekt, cm_ExMakeGoods, cm_ExMoveObject, cm_ExRemoveGoods, cm_ExCreateObject, cm_ExFieldDeltas, cm_ExFieldValues, cm_ExMoodDeltas, cm_ExSetField, cm_ExFloatDelta, cm_ExRelations, cm_ExLongMessage, cm_ExSetHE, cm_ExTime, cm_ExGameOver, cm_ExSysMessage, cm_ExKillCharacter, cm_ExAddStraftat, cm_ExRetireCrime, cm_ExProofExecuted, cm_ExDropEvidence, cm_ExSendCutInfo, cm_ExNewParty, cm_ExAllocCutscene, cm_ExAddPartyMember, cm_ExLeaveParty, cm_ExRemoveOwned, cm_ExSetPlant, cm_ExCreateSermon, cm_ExChrWalkTo, cm_ExChrWalkToDummy, cm_ExChrMoveToUniverse, cm_ExChrStop, cm_ExChrSetPos, cm_ExCartState, cm_ExPlaceOnStage, cm_ExChrGotoBuilding, cm_ExChrUseGates, cm_ExChrAnim, cm_ExOwnership, cm_ExSetHome, cm_ExGebUpgrade, cm_ExBuildingDestroyed, cm_ExHandOverCrimes, cm_ExHire, cm_ExCharModel, cm_ExGoodsRecord, cm_ExGoodsDelta, cm_ExLoadStatistics, cm_ExUpdateSmoke, cm_ExSkillsChanged, cm_ExApplication, cm_ExOfficeMessage, cm_ExDecisionValue, cm_ExChangePlayerHe, cm_ExSetJob, cm_ExSpawnCharacter, cm_ExFreeObject, cm_ExChat, cm_ExCreateGeb, cm_ExCharHalt, cm_ExObjCell, cm_ExWalkPath, cm_ExCutEntry, cm_ExCutEntryUpdate, cm_ExDamage, cm_ExDarkCorner, cm_ExTableRequest, cm_ExFightCommand, cm_ExMessageRecord, cm_ExChrDelete, cm_ExCancelParty, cm_ExCutsceneReady, cm_ExMoneyDelta, cm_ExHeldRounds, cm_ExOfficeSwap, cm_ExAddSkill, cm_ExSpyReport, cm_ExLobbyMessage, cm_ExGraveOrder, cm_ExGraveMove, cm_ExRaiseDead, cm_ExCreateAlchemist, cm_ExAlchemistOrder, cm_ExMasterChange, cm_ExEffect, cm_ExShots, cm_ExSetRoute, cm_ExClearRoute };  /* 0x60bd98, the handler of each message type */
int (*cm_Checks[0x6a])(G17MSG *m) = { sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckMoveGoods, sv_ReturnZero, cm_CheckSellObject, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckFieldDeltas, sv_ReturnZero, sv_ReturnZero, cm_CheckSetField, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckOwnership, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckApplication, cm_CheckOfficeMessage, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckDarkCorner, cm_CheckTableRequest, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, cm_CheckCutsceneReady, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero };  /* 0x60bf40, the check of each type (groups 5..6) */
int cm_RecvCount = 0;  /* 0x6f0cd0, received messages */
int cm_NoticeCount = 0;  /* 0x6f0cd4 */
int cm_LobbyChatCount = 0;  /* 0x6f0cd8, the lines in the ring (at most 15) */
int cm_LobbyChatPos = 0;  /* 0x6f0cdc, the index of the line the chat window reads next (cm_LobbyChatFirst / 0x4cd4c0) */
extern unsigned int ai_MissionSeed;
extern char * sv_SelectedObject;
extern char cm_ChatLog[2048];
extern int cm_SaveAcks[8][2];
/* ai_Unk770d8c: view of ai_Chars (tools/data_views.py) */
G18LOG cm_LobbyChat[15];  /* 0x935da0 */
int cm_LobbyChatNext;  /* 0x9363f4 */
CMLOBBYSTATE cm_LobbyStateFlags;  /* 0x936400, the lobby state cm_LobbyState hands out (open, changed, closed): one object of
                                    17..32 bytes, LINK's common pad puts it at 0x936400 after cm_LobbyChatNext's end 0x9363f8
                                    (tools/exe/commongaps.py); 20 taken */
unsigned int cm_LobbyList[9];  /* 0x936420, the count then the eight player states (0x24 bytes) */
CMNETGAME cm_NetGame;  /* 0x936460, the network game (hdr.players 0x936496, mode 0x9364dc) */
char cm_LobbyRecords[752];  /* 0x9364e0 */
int cm_LobbyPlayerId;  /* 0x9367d0, this player's lobby id */
int cm_LobbyHelloValue;  /* 0x9367d4, the hello answer's second value */
G17MSG cm_RecvPool[0x2000];  /* 0x9367e0, the free receive records */
int cm_LastSyncCount;  /* 0xa687e0, "cm_LastSyncCount" (net.h) */
int cm_NextNetStep;  /* 0xa687e4, d3_Time of the start */
CMSPLITBUF cm_SplitMsg;  /* 0xa68800, a long message to split (cm_SplitMsgLen bytes) */
G17MSG * cm_RecvFree;  /* 0xa68c00, the free list */
unsigned int cm_GroupEndSeq;  /* 0xa68c04 */
int cm_FieldBlockLen;  /* 0xa68c08, bytes used in cm_FieldBlock.buf */
G17FIELDS cm_FieldBlock;  /* 0xa68c20 */
G17FIELDS ai_NeedBlock;  /* 0xa68ca0 */
unsigned int cm_LastRequCount;  /* 0xa68d1c, "cm_LastRequCount" */
int ai_NeedBlockLen;  /* 0xa68d20, bytes used in ai_NeedBlock.buf */
G17MSG * cm_RecvQueue;  /* 0xa68d24, the received messages */
char * cm_FieldObject;  /* 0xa68d2c, the object of the field block (fields are offsets into it) */
int cm_SplitMsgLen;  /* 0xa68d30, length of cm_SplitMsg */
struct AICHAR * ai_NeedBlockChar;  /* 0xa68d34, the character ai_BeginNeedBlock builds the block for */
unsigned char cm_LongMsg[1024];  /* 0xa68d40, a joined long message */
G17MSG cm_SendRing[0x8000];  /* 0xa69140, the send ring */
char * cm_LoadBuf;  /* 0xf31140, the load buffer ("cm_LoadBuf") */
int cm_DayEndRequest;  /* 0xf31144 */
G17STAT cm_SendStates[0x8000];  /* 0xf31160 */
G17MSG * cm_SendQueue;  /* 0xf81160, the messages to send */
unsigned int cm_LongMsgLen;  /* 0xf81164, length of cm_LongMsg */
G17MSG cm_RecvMsg;  /* 0xf81180, the message being received */
unsigned int cm_GroupFirstSeq;  /* 0xf8121c */
unsigned cm_LoadSize;  /* 0xa68d28, the received bytes and the expected length (compared unsigned */
int cm_LastSeq;  /* 0xf81168 .. */
unsigned cm_LoadReceived;  /* 0xf81220 / */
extern AIWATCH ai_Watch;
extern char d2_CaptionCopy[0x80];
/* ai_Unkf8f39c: view of d2_Wins (tools/data_views.py) */
extern int cm_PickCellX;
extern int cm_PickCellZ;
/* txt_AmountUnit: view of txt_Ptrs (build/mk_txt_views.py) */
