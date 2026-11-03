/* cm.c -- the game messages exchanged with the server (net.c): the size of a message by its type, the send
   ring cm_SendRing (0x8000 slots) and its states, the receive list cm_RecvQueue (0x2000 records), long messages split
   into parts of type 7, the field blocks of an object (delta / plain values). Its .data block starts at 0x60bd80 after
   f3snd.c's literals (0x302, 2, -1 globals, the handler table 0x60bd98 of 0x4c6a20..). Tag "cm_ExecCommand" (0x4c4190,
   its texts at 0x60c0e8..). The ~0x6a message handlers and their helpers follow, then the fight code (0x4cf670..)
   and the cutscenes (cut_, 0x4d8c70..: trial, duel, execution, wedding, auction, ...; joined: the strings run
   monotonic 0x60cb58..0x60ec88 and the ints 0x60d360.. serve both halves). Ends at 0x4e5bf0 (amt.c follows).
   Names are ours.
   Open: not included yet (dcp headers conflicts cm HEADER): am.h amt.h anm.h ch.h d2.h f3gm.h gfx.h gv.h he.h
   hmt.h hst.h rt.h unk_4a7140.h unk_4b39f0.h unk_5d33b0.h; held by calls whose argument count differs from the
   definition (read the original's pushes): ai_Unk4941b0, ai_Unk4941d0, ch_Create, d2_SetObjValue, gm_SkillPoints,
   gv_CrimeText, gv_PassSentence, he_Unk555210.
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
#include "d3.h"
struct D3SKYLAYER; struct D3SKY; struct D3SKYLAYER *d3sky_CreateLayer(struct D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset, float vscale, float voffset);  /* 0x45d880 */
#include "d3_tools.h"
#include "d3_quad.h"
#include "d3_light.h"
#include "di_input.h"
#include "evt.h"
#include "data_views.h"
#include "unk_5d33b0_types.h"
extern OPTIONBYTES ai_GfxFloor;
extern char ai_Unk92cae0[16][0x35c];
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
#define ai_Unk936424 (*(int (*)[8])((char *)ai_Unk936420 + 4))  /* 0x936424, the eight player states (a view) */

#include "time_view.h"
#include "f3init.h"
#include "sv.h"
#include "ahm.h"
#include "ai.h"
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
#include "unk_458810.h"
#include "unk_575860.h"
#include "unk_58bbd0.h"
#include "shp.h"
#define d3sm_PickK ((int (*)())d3sm_Pick)  /* 0x45f2b0 (d3_sm.c): @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) */
#define d3sky_CreateLayerK ((int (*)())d3sky_CreateLayer)  /* 0x45d880 (d3_sky.c): @original this unit's calls were compiled without a prototype (as the K&R declaration it had before the definition's spelling) (floats as bit patterns, the flags as an int) */
#define d3sky_CreateK ((int (*)())d3sky_Create)  /* 0x45da30 (d3_sky.c): @original the calls pass the three floats as dword bit patterns without a prototype (ai_Unk4e32f0 0x4e32f0: push 0x44160000 ..) */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests */
#define d3t_CreateEarthK ((int (*)())d3t_CreateEarth)  /* 0x4168e0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateSmokeK ((int (*)())d3t_CreateSmoke)  /* 0x416340 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_CreateExplosionK ((int (*)())d3t_CreateExplosion)  /* 0x416040 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define gm_AssignBuildingK ((int (*)())gm_AssignBuilding)  /* 0x502b60 (f3gm.c): @original ai_Unk4caaa0 (0x4caaa0, byte) passes the two shorts zero-extended without a prototype (xor ecx / xor edx, mov cx) */
#define d3t_CreateBloodK ((int (*)())d3t_CreateBlood)  /* 0x415e90 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_SetColorK ((int (*)())d3t_SetColor)  /* 0x413550 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define d3t_SetGreyK ((int (*)())d3t_SetGrey)  /* 0x4134a0 (d3_tools.c): @original this unit's calls were compiled without a prototype (float arguments as double, an int result) */
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define d2_LastWindowPtrC (*(char **)&d2_LastWindowPtr)  /* 0x6ed980, d2_LastWindowPtr (d2.c: D2WIN *) read bytewise, as this unit does */
#define d2_ObjsC (*(char **)&d2_Objs)  /* 0xf8f080, d2_Objs (d2.c: D2OBJ *) read bytewise, as this unit does (+ obj * 0x2ec) */
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */
#define ai_FallbackBuildingT (*(struct AIBLD **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) as the AIBLD it points to */
#define ai_PlayerBuildingT (*(struct AIBLD **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) as the AIBLD it points to */
#define d3sm_GetVertexI ((int (*)())d3sm_GetVertex)  /* 0x45f100 (d3_sm.c: char, D3SM *, int, int, D3VECTOR *): @original this unit's callers read an int result (byte-matched unk_Unk4f86a0 (ord.c): no 0f be c0 after the call) */
#define ch_FindFreeCellK ((int (*)())ch_FindFreeCell)  /* 0x4072a0 (ch.h: char, 9 parameters): @original cm.c's call at 8227 passes 8 arguments without a prototype and reads an int */
#define ch_AddActionCBK ((struct CHACTION *(*)())ch_AddActionCB)  /* 0x40b400 (ch.h): @original cm.c's calls were compiled without a prototype (default promotions) */
#define amt_GetOfficeI ((int (*)(int, struct AMTREC *))amt_GetOffice)  /* 0x4e6f30 (amt.h: unsigned char i): @original this unit pushes its int index raw (0x4dab50 / 0x4aad30: push ebp / push edx, no zero-extension) */
#define ahm_CreateAlchemistI ((int (*)(int *))ahm_CreateAlchemist)  /* 0x4ec500 (ahm.h: unsigned short): @original the caller 0x4ccf90 reads the id as an int (no and eax,0xffff) */
#define ai_Unk7603a0V (*(char * (*)[])&ai_SceneChars)  /* 0x7603a0, @original ai_Unk4cfeb0 (0x4cfeb0, byte) walks the table as pointers, `pp < ai_Unk760420V` its end: the int[32] / float spellings make it draft (15) */
#define ai_Unk760420V (*(char * (*)[])&sv_SkyScale)  /* 0x760420, @original the end of ai_Unk7603a0V's walk in ai_Unk4cfeb0 (0x4cfeb0, byte) */
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
        p = (unsigned char *)m + 0x15;
        for (i = m->data[4]; i > 0; i--) {
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
    *(int *)&m.data[0] = 0xa03ef57c;
    *(int *)&m.data[4] = 0;
    ai_Unk4c39c0(&m);
}
/* 0x4c3630, waits for the answer to 0x4c3600; 1: none */

int cm_WaitHello(void)
{
    G17MSG *m;

    m = ai_Unk4c4030(3, 0x165);
    if (m == 0)
        return 1;
    if (*(int *)&m->data[0] != 0xa03ef57c)
        return 1;
    ai_Unk4cd580(*(int *)&m->data[4], *(int *)&m->data[8]);
    ai_Unk4c4130(m);
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
/* 0x4c3720, queues ai_Unka68800 as parts of type 7 (0x80 bytes each); returns the seq of the first part */

int ai_Unk4c3720(void)
{
    G17MSG *m, *p, *last;
    unsigned int i;
    int first, off;

    if (ai_Unka68d30 == 0)
        return 0;
    if (net_Conn.closed != 0)
        return 0;
    last = 0;
    first = cm_LastSeq + 1;
    for (off = 0; off < ai_Unka68d30; off += 0x80) {
        cm_LastSeq++;
        i = cm_LastSeq & 0x7fff;
        m = &cm_SendRing[i];
        m->type = 7;
        m->size = cm_MessageSize(m);
        memcpy(m->data, ai_Unka68800 + off, 0x80);
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
    ai_Unka68d30 = 0;
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
/* 0x4c38e0, joins the parts of the split message m into ai_Unka68d40 and frees them; returns the length */

unsigned int ai_Unk4c38e0(G17MSG *m)
{
    G17MSG *r, *p;
    int n;

    if (m->next7 == 0 || cm_RecvQueue == 0)
        return 0;
    for (r = cm_RecvQueue; r->sender != m->sender || r->seq != m->next7; r = r->next)
        if (r->next == 0)
            return 0;
    m->next7 = 0;
    ai_Unkf81164 = r->splitLen;
    memcpy(ai_Unka68d40, r->data + 2, 0x7e);
    r->type = 0;
    for (n = 0x7e, p = r; n < (int)ai_Unkf81164; n += 0x80) {
        if (cm_RecvQueue == 0)
            return 0;
        for (r = cm_RecvQueue; r->sender != p->sender || r->seq != p->next7; r = r->next)
            if (r->next == 0)
                return 0;
        p->next7 = 0;
        memcpy(ai_Unka68d40 + n, r->data, 0x80);
        r->type = 0;
        p = r;
    }
    r->next7 = 0;
    return ai_Unkf81164;
}
/* 0x4c39c0, queues a copy of src in the send ring; returns its slot, -1: closed or the slot is still in use */

int ai_Unk4c39c0(G17MSG *src)
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
    if (ai_Unka68d30 != 0)
        m->next7 = ai_Unk4c3720();
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
    if (cm_GroupFirstSeq != cm_GroupEndSeq && cm_GroupFirstSeq < cm_GroupEndSeq) {
        for (s = cm_GroupFirstSeq; s < cm_GroupEndSeq; s++) {
            if (cm_SendStates[s & 0x7fff].state == 0)
                return 0;
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

    ai_Unka68c20.id = id;
    ai_Unka68c20.n = 0;
    memset(ai_Unka68c20.buf, 0, sizeof(ai_Unka68c20.buf));
    ai_Unka68c08 = 0;
    if (gm_FindById(&a, &b, &id, id) != 0)
        cm_FieldObject = o;
}
/* 0x4c3c00, adds n values of size bytes at field as differences to the object's values */

int cm_AddFieldDelta(int size, int n, short field, void *v)
{
    unsigned char *p;
    int o;
    unsigned int len;

    o = ai_Unka68c08;
    if (size != 1 && size != 2 && size != 4)
        return 1;
    len = n * size + 4 + o;
    if (len > 0x76)
        return 1;
    p = &ai_Unka68c20.buf[o];
    ai_Unka68c20.n++;
    ai_Unka68c08 = len;
    p[0] = size;
    p[1] = n;
    *(unsigned short *)(p + 2) = field;
    p += 4;
    if (size == 1) {
        unsigned char *s = (unsigned char *)v, *b = (unsigned char *)cm_FieldObject + field;
        for (; n != 0; n--)
            *p++ = *s++ - *b++;
    } else if (size == 2) {
        short *s = (short *)v, *b = (short *)(cm_FieldObject + field), *d = (short *)p;
        for (; n != 0; n--)
            *d++ = *s++ - *b++;
    } else if (size == 4) {
        int *s = (int *)v, *b = (int *)(cm_FieldObject + field), *d = (int *)p;
        for (; n != 0; n--)
            *d++ = *s++ - *b++;
    }
    return 0;
}
/* 0x4c3d00, adds n values of size bytes at field (a plain copy) */

int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    unsigned char *p;
    int o;
    unsigned int k, len;

    o = ai_Unka68c08;
    if (size != 1 && size != 2 && size != 4)
        return 1;
    k = size * n;
    len = k + 4 + o;
    if (len > 0x76)
        return 1;
    p = &ai_Unka68c20.buf[o];
    ai_Unka68c20.n++;
    ai_Unka68c08 = len;
    p[0] = size;
    p[1] = n;
    *(unsigned short *)(p + 2) = field;
    memcpy(p + 4, v, k);
    return 0;
}
/* 0x4c3d90, as 0x4c3d00, value by value */

int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    unsigned char *p;
    int o;
    unsigned int len;

    o = ai_Unka68c08;
    if (size != 1 && size != 2 && size != 4)
        return 1;
    len = n * size + 4 + o;
    if (len > 0x76)
        return 1;
    p = &ai_Unka68c20.buf[o];
    ai_Unka68c20.n++;
    ai_Unka68c08 = len;
    p[0] = size;
    p[1] = n;
    *(unsigned short *)(p + 2) = field;
    p += 4;
    if (size == 1) {
        unsigned char *s = (unsigned char *)v;
        for (; n != 0; n--)
            *p++ = *s++;
    } else if (size == 2) {
        short *s = (short *)v, *d = (short *)p;
        for (; n != 0; n--)
            *d++ = *s++;
    } else if (size == 4) {
        int *s = (int *)v, *d = (int *)p;
        for (; n != 0; n--)
            *d++ = *s++;
    }
    return 0;
}
/* 0x4c3e70, starts the second field block about p (its id at +4) */

void ai_BeginNeedBlock(char *p)
{
    ai_Unka68ca0.id = *(int *)(p + 4);
    ai_Unka68ca0.n = 0;
    memset(ai_Unka68ca0.buf, 0, sizeof(ai_Unka68ca0.buf));
    ai_Unka68d20 = 0;
    ai_Unka68d34 = p;
}
/* 0x4c3eb0, adds the pair (k, v) to the second field block */

int ai_AddNeedDelta(int k, float v)
{
    unsigned char *p;
    int o;

    o = ai_Unka68d20;
    if (ai_Unka68d20 + 5 > 0x76)
        return 1;
    p = &ai_Unka68ca0.buf[ai_Unka68d20];
    ai_Unka68ca0.n++;
    ai_Unka68d20 += 5;
    *p = k;
    *(float *)&ai_Unka68ca0.buf[o + 1] = v;
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

void ai_Unk4c3f20(void)
{
    int buf[31];

    if (cm_SpeedLevel < 4) {
        buf[0] = cm_SpeedLevel + 1;
        cm_SendExSysMessage(0x12, buf);
    }
}
/* 0x4c3f50, lowers cm_SpeedLevel */

void ai_Unk4c3f50(void)
{
    int buf[31];

    if (cm_SpeedLevel > 0) {
        buf[0] = cm_SpeedLevel - 1;
        cm_SendExSysMessage(0x12, buf);
    }
}
/* 0x4c3f80 */

int ai_Unk4c3f80(void)
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
            ai_Unk6f0cd0++;
            if (cm_EnqueueReceived(&cm_RecvMsg) != 0)
                return 1;
            net_Conn.rx = (NETPKT *)&cm_RecvMsg;
            net_Recv();
        } while (net_Conn.rx == 0);
    }
    return 0;
}
/* 0x4c4030, waits up to timeout ms for a message of type type */

G17MSG *ai_Unk4c4030(char type, int timeout)
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
    cm_RecvFree = m->next;
    *m = *src;
    m->size = cm_MessageSize(m);
    if (cm_RecvQueue == 0) {
        m->prev = 0;
        m->next = 0;
        cm_RecvQueue = m;
        return 0;
    }
    for (p = cm_RecvQueue; p->next != 0; p = p->next)
        ;
    m->prev = p;
    m->next = 0;
    p->next = m;
    return 0;
}
/* 0x4c4130, takes m out of the receive list and frees it */

void ai_Unk4c4130(G17MSG *m)
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
    int ok;

    if (net_Conn.closed != 0) {
        while ((m = cm_RecvQueue) != 0) {
            if (m->idx != -1) {
                s = &cm_SendStates[m->idx & 0x7fff];
                s->state = 2;
                s->unk01 = 0;
                s->value = 0;
            }
            ai_Unk4c4130(m);
        }
        cm_RecvQueue = 0;
        for (s = cm_SendStates; s < cm_SendStates + 0x8000; s++)
            s->state = 2;
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
            ok = ai_Unk4c43c0(grp) != 0;
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
            ai_Unk4c38e0(t);
            if (t->type < 0x6a) {
                if (t->idx == -1) {
                    s = 0;
                } else {
                    s = &cm_SendStates[t->idx & 0x7fff];
                    s->state = 2;
                    s->unk01 = 0;
                    s->value = 0;
                    if (t->seq != ai_Unka68d1c + 1) {
                        if (t->seq == (unsigned int)cm_LastSyncCount) {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Received a Command with Count == cm_LastSyncCount, cmd->Count is %li, LastRequCnt is %li", t->seq, ai_Unka68d1c);
                        } else if (t->type == 0x20 && t->data[0] == 0x0e) {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Received Sync, cmd->Count is %li, LastRequCnt is %li", t->seq, ai_Unka68d1c);
                        } else {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Lost a Command, cmd->Count is %li, cm_LastRequCount is %li", t->seq, ai_Unka68d1c);
                            ai_Unka68d1c = t->seq;
                        }
                    } else {
                        ai_Unka68d1c = t->seq;
                    }
                }
                ai_Unk60bd98[t->type](t, s);
            }
        }
    }
    return 0;
}
/* 0x4c43c0, checks the messages of the group grp (5 .. 6): all pass: 1 at both ends, else 2 for all */

int ai_Unk4c43c0(G17MSG *grp)
{
    G17MSG *p;
    int ok;

    ok = 1;
    for (p = grp->next; p->type != 6; p = p->next) {
        if (ai_Unk60bf40[p->type](p) != 0) {
            ok = 0;
            break;
        }
    }
    if (ok) {
        grp->type = 1;
        while (grp->type != 6)
            grp = grp->next;
        grp->type = 1;
        return 1;
    }
    while (grp->type != 6) {
        grp->type = 2;
        grp = grp->next;
    }
    grp->type = 2;
    return 1;
}
/* 0x4c4450, resolves the ids -2 / -3 / -4 */

int ai_Unk4c4450(int a, int id)
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

int ai_Unk4c4480(unsigned short len, void *p)
{
    if (ai_Unka68d30 != 0)
        return 0;
    if (len > 0x3fe)
        return 0;
    *(unsigned short *)ai_Unka68800 = len;
    memcpy(ai_Unka68800 + 2, p, len);
    ai_Unka68d30 = len + 2;
    return 1;
}
/* 0x4c44d0, message 8: sends the size of the load buffer that the blocks of message 9 follow (the handler
   of type 8 allocates it, "cm_LoadBuf") */

void cm_SendLoadSize(int v)
{
    G17MSG m;

    m.type = 8;
    G17_I(m, 0) = v;
    ai_Unk4c39c0(&m);
}
/* 0x4c4500, message 9: sends one 0x80-byte block of the load buffer (the handler of type 9 appends it);
   returns the send slot of 0x4c39c0, -1 when the ring is full (the ls.c caller tests that) */

int cm_SendLoadBlock(void *p)
{
    G17MSG m;

    m.type = 9;
    memcpy(m.data, p, 0x80);
    return ai_Unk4c39c0(&m);
}
/* 0x4c4540, message 5: starts a group */

void cm_BeginGroup(const char *s)
{
    G17MSG m;

    m.type = 5;
    strncpy((char *)m.data, s, 0x80);
    ai_Unk4c39c0(&m);
}
/* 0x4c4580, message 6: ends a group */

void cm_EndGroup(const char *s)
{
    G17MSG m;

    m.type = 6;
    strncpy((char *)m.data, s, 0x80);
    ai_Unk4c39c0(&m);
}
/* 0x4c45c0, message 10 (0x30 bytes of p, or zeros) */

int cm_SendMessage10(unsigned char n, int b, void *p, short d)
{
    G17MSG m;

    m.type = 10;
    if (n == 0)
        return -1;
    *(unsigned short *)&m.data[4] = n;
    G17_I(m, 6) = b;
    G17_S(m, 0xa) = d;
    if (p == 0) {
        memset(&m.data[0x10], 0, 0x30);
        G17_I(m, 0xc) = 0;
    } else {
        memcpy(&m.data[0x10], p, 0x30);
        G17_I(m, 0xc) = 1;
    }
    return ai_Unk4c39c0(&m);
}
/* 0x4c4650, message 0xb */

int cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h)
{
    G17MSG m;

    m.type = 0xb;
    m.data[4] = a;
    G17_I(m, 5) = b;
    G17_I(m, 9) = c;
    *(unsigned short *)&m.data[0xd] = d;
    G17_I(m, 0xf) = e;
    m.data[0x13] = f;
    m.data[0x14] = g;
    m.data[0x15] = h;
    return ai_Unk4c39c0(&m);
}
/* 0x4c46d0, message 0xc (two names of 0x10 bytes) */

int cm_SendMessage0c(int a, int b, short c, unsigned char d, int e, unsigned char f, unsigned char g, const char *s1, const char *s2)
{
    G17MSG m;

    m.type = 0xc;
    G17_I(m, 4) = a;
    G17_I(m, 8) = b;
    G17_S(m, 0xc) = c;
    m.data[0xe] = d;
    G17_I(m, 0xf) = e;
    m.data[0x13] = f;
    m.data[0x14] = g;
    strncpy((char *)&m.data[0x15], s1, 0x10);
    strncpy((char *)&m.data[0x25], s2, 0x10);
    return ai_Unk4c39c0(&m);
}
/* 0x4c4770, message 0xd */

void ai_Unk4c4770(int a, int b)
{
    G17MSG m;

    m.type = 0xd;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c47b0, message 0xe */

void ai_Unk4c47b0(short a)
{
    G17MSG m;

    m.type = 0xe;
    G17_S(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c47e0, message 0xf */

void cm_SendMoveGoods(int a, int b, unsigned char c, int d)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    m.type = 0xf;
    m.data[0xc] = c;
    G17_I(m, 0xd) = d;
    ai_Unk4c39c0(&m);
}
/* 0x4c4830, message 0x10 */

void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    m.type = 0x10;
    m.data[0xc] = c;
    G17_I(m, 0xd) = d;
    ai_Unk4c39c0(&m);
}
/* 0x4c4880, message 0x11 */

void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f)
{
    G17MSG m;

    m.type = 0x11;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_S(m, 8) = c;
    m.data[0xe] = e;
    G17_I(m, 0xf) = d;
    G17_I(m, 0x13) = f;
    ai_Unk4c39c0(&m);
}
/* 0x4c48e0, message 0x12 */

void ai_Unk4c48e0(int a, short b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x12;
    G17_S(m, 4) = b;
    G17_I(m, 6) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c4920, message 0x13 */

void ai_Unk4c4920(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x13;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c4960, message 0x14 */

void cm_SendRemoveGoods(int a, short b)
{
    G17MSG m;

    m.type = 0x14;
    G17_I(m, 0) = a;
    G17_S(m, 4) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c49a0, message 0x16: the field block (deltas) */

void cm_SendFieldDeltas(void)
{
    G17MSG m;

    m.type = 0x16;
    memcpy(m.data, &ai_Unka68c20, sizeof(G17FIELDS));
    ai_Unk4c39c0(&m);
}
/* 0x4c49e0, message 0x17: the field block (values) */

void cm_SendFieldValues(void)
{
    G17MSG m;

    m.type = 0x17;
    memcpy(m.data, &ai_Unka68c20, sizeof(G17FIELDS));
    ai_Unk4c39c0(&m);
}
/* 0x4c4a20, message 0x18: the second field block */

void ai_SendNeedBlock(void)
{
    G17MSG m;

    m.type = 0x18;
    memcpy(m.data, &ai_Unka68ca0, sizeof(G17FIELDS));
    ai_Unk4c39c0(&m);
}
/* 0x4c4a60, message 0x19 */

void cm_SendSetField(int a, int b, int c, int d, int e)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    m.type = 0x19;
    G17_I(m, 0xc) = d;
    G17_I(m, 0x10) = e;
    ai_Unk4c39c0(&m);
}
/* 0x4c4ac0, sends message 0x1a: adds delta to the float field at the offset field of the character with the netId id */

void cm_SendFloatDelta(int id, int field, float delta)
{
    G17MSG m;

    G17_I(m, 0) = id;
    m.type = 0x1a;
    G17_I(m, 4) = field;
    *(float *)&m.data[8] = delta;
    ai_Unk4c39c0(&m);
}
/* 0x4c4b00, sends message 0x1b: the relation of the character a to b changes by c (d: 0 / 1 / 2, the callers' kind of change), with the difficulty values; returns the send slot (ai_Unk4c39c0) */

int cm_SendRelations(int a, int b, int c, int d)
{
    G17MSG m;

    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    m.type = 0x1b;
    G17_I(m, 8) = c;
    G17_I(m, 0xc) = d;
    *(float *)&m.data[0x10] = ai_DifficultyScale;
    G17_I(m, 0x14) = (int)ai_DifficultyValue;
    return ai_Unk4c39c0(&m);
}
/* 0x4c4b70, message 0x1c: msg (0xf8 bytes, AIMSG) as a long message; zeros at 0x38..0x57 become -1 */

int cm_SendLongMessage(char *msg)
{
    G17MSG m;
    int i;

    m.type = 0x1c;
    for (i = 0; i < 8; i++)
        if (((int *)(msg + 0x38))[i] == 0)
            ((int *)(msg + 0x38))[i] = -1;
    ai_Unk4c4480(0xf8, msg);
    ai_Unk4c39c0(&m);
}
/* 0x4c4bc0, message 0x1c with len extra bytes after msg (0xf8 bytes) */

int ai_Unk4c4bc0(char *msg, unsigned int len, void *extra)
{
    G17MSG m;
    char *buf;

    m.type = 0x1c;
    if (gm_FindChar(*(int *)(msg + 8)) == 0)
        return -1;
    buf = mem_Alloc(len + 0xf8, "tmp");
    memcpy(buf, msg, 0xf8);
    memcpy(buf + 0xf8, extra, len);
    ai_Unk4c4480(len + 0xf8, buf);
    mem_Free(buf);
    return ai_Unk4c39c0(&m);
}
/* 0x4c4c60, message 0x1d about p (three AITIMEs, its name from ai_Unk625820); p + 0xac as the long message */

void cm_SendExSetHE(unsigned char k, char *p)
{
    G17MSG m;

    m.type = 0x1d;
    G17_I(m, 0) = *(int *)(p + 4);
    *(G17T14 *)&m.data[4] = *(G17T14 *)(p + 0x44);
    *(G17T14 *)&m.data[0x12] = *(G17T14 *)(p + 0x52);
    *(G17T14 *)&m.data[0x20] = *(G17T14 *)(p + 0x60);
    m.data[0x2e] = k;
    strcpy((char *)&m.data[0x2f], ai_Unk625820[*(unsigned char *)p]);
    ai_Unk4c4480(0xa0, p + 0xac);
    ai_Unk4c39c0(&m);
}
/* 0x4c4d40, message 0x1e with the time t; -1: t is not valid (0x4fe210) */

int cm_SendTime(char *t)
{
    G17MSG m;

    if (gm_CompareTime(t, ai_Time) == 0)
        return -1;
    *(G17T14 *)m.data = *(G17T14 *)t;
    m.type = 0x1e;
    return ai_Unk4c39c0(&m);
}
/* 0x4c4db0, sends message 0x20 (the extended system message, handled by cm_ExSysMessage): sub type id and 0x7c bytes of msg */

int cm_SendExSysMessage(unsigned char id, void *msg)
{
    G17MSG m;

    m.type = 0x20;
    m.data[0] = id;
    if (msg != 0)
        memcpy(&m.data[1], msg, 0x7c);
    return ai_Unk4c39c0(&m);
}
/* 0x4c4e00, message 0x21 */

void cm_SendKillCharacter(int a, int b)
{
    G17MSG m;

    m.type = 0x21;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c4e40, sends message 0x22, a new crime: the CRIME record p (0x2d bytes) with the id ai_NextCrimeId, the state 1 and a random seed; the send's result */

int cm_SendCrime(void *p)
{
    G17MSG m;

    m.type = 0x22;
    memcpy(m.data, p, 0x2d);
    G17_I(m, 0) = ai_NextCrimeId;
    G17_I(m, 0x25) = 1;
    G17_I(m, 0x29) = rand();
    return ai_Unk4c39c0(&m);
}
/* 0x4c4e90, sends message 0x23 (crime id a, force b: retire the crime record, ai_Unk4c9090); the send slot of ai_Unk4c39c0 */

int cm_SendRetireCrime(int a, int b)
{
    G17MSG m;

    m.type = 0x23;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    return ai_Unk4c39c0(&m);
}
/* 0x4c4ed0, message 0x24 */

void cm_SendProofExecuted(int a, int b)
{
    G17MSG m;

    m.type = 0x24;
    G17_I(m, 4) = b;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c4f10, message 0x25 */

void ai_Unk4c4f10(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 4) = b;
    m.type = 0x25;
    G17_I(m, 0) = a;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c4f50, "cm_RequestSendCutInfo": message 0x26 */

void ai_Unk4c4f50(int id, int b)
{
    G17MSG m;
    char buf[0x100];

    m.type = 0x26;
    G17_I(m, 0) = id;
    G17_I(m, 4) = b;
    sprintf(buf, "cm_RequestSendCutInfo(): PlayerId:%i Time:%i", id, d3_Time);
    ai_Unk4c39c0(&m);
}
/* 0x4c4fa0, message 0x27: p (0x114 bytes) as the long message */

int cm_SendNewParty(void *p)
{
    G17MSG m;

    m.type = 0x27;
    ai_Unk4c4480(0x114, p);
    return ai_Unk4c39c0(&m);
}
/* 0x4c4fd0, message 0x2a */

void ai_Unk4c4fd0(int a, int b)
{
    G17MSG m;

    m.type = 0x2a;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c5010, message 0x2b */

void cm_SendMoveObject(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x2b;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c5050, message 0x2c */

void cm_SetPlant(int a, unsigned char b, unsigned char c, short d, unsigned char e, int f)
{
    G17MSG m;

    m.type = 0x2c;
    G17_I(m, 4) = a;
    m.data[8] = b;
    m.data[9] = c;
    G17_S(m, 0xa) = d;
    m.data[0xc] = e;
    G17_I(m, 0xd) = f;
    ai_Unk4c39c0(&m);
}
/* 0x4c50b0, message 0x2d */

void ai_Unk4c50b0(int a, int b, short c)
{
    G17MSG m;

    G17_S(m, 0xc) = c;
    G17_I(m, 8) = b;
    m.type = 0x2d;
    G17_I(m, 4) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5100, sends message 0x2e: its handler (0x4c97c0) gives the person "sp_<a>" the walk action 0x2d to (b, c) in scene d */

void cm_SendChrWalkTo(int a, int b, int c, int d)
{
    G17MSG m;

    m.type = 0x2e;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    G17_I(m, 0xc) = d;
    ai_Unk4c39c0(&m);
}
/* 0x4c5150, message 0x2f with a name */

void cm_RequestChrMove(int a, int b, int c, const char *s)
{
    G17MSG m;

    m.type = 0x2f;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    strcpy((char *)&m.data[0xc], s);
    ai_Unk4c39c0(&m);
}
/* 0x4c51b0, "cm_RequestChrMoveToUniverse": message 0x30 with a dummy name (< 0x20 characters) */

void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s)
{
    G17MSG m;

    m.type = 0x30;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    if (strlen(s) < 0x20)
        strcpy((char *)&m.data[0xc], s);
    else
        err_Message("cm_RequestChrMoveToUniverse(): dummyname is too long...");
    ai_Unk4c39c0(&m);
}
/* 0x4c5230, sends message 0x31: its handler (0x4c9c10) stops the actions of the person "sp_<a>" */

void cm_SendChrStop(int a)
{
    G17MSG m;

    m.type = 0x31;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5260, message 0x33 */

void ai_Unk4c5260(int a, int b)
{
    G17MSG m;

    m.type = 0x33;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c52a0, message 0x34 */

void ai_Unk4c52a0(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x34;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c52e0, message 0x35 about character id and building bld; c -1 and not f: the building's object (or -1, flag 1) */

int cm_SendChrGotoBuilding(int id, int bld, int c, const char *s, int f, const char *s2)
{
    G17MSG m;
    char *ch, *b, *o;

    m.type = 0x35;
    G17_I(m, 0) = id;
    G17_I(m, 4) = bld;
    G17_I(m, 8) = c;
    m.data[0xc] = (unsigned char)f;
    if (s2 == 0)
        m.data[0x2d] = 0;
    else
        strncpy((char *)&m.data[0x2d], s2, 0x1f);
    if (s == 0)
        m.data[0xd] = 0;
    else
        strcpy((char *)&m.data[0xd], s);
    ch = gm_FindChar(id);
    if (ch != 0 && ch[8] == 0)
        return -1;
    if (f == 0 && c == -1) {
        o = 0;
        b = gm_FindBuildingById(bld);
        if (b != 0) {
            if (gm_IsHomeType(b) == 0)
                o = gm_GetGebaeudeHauptraum(b);
            else
                o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        }
        if (o != 0) {
            G17_I(m, 8) = *(int *)(o + 2);
        } else {
            m.data[0xc] = 1;
            G17_I(m, 8) = -1;
        }
    }
    return ai_Unk4c39c0(&m);
}
/* 0x4c5410, message 0x37 with a name */

void ai_Unk4c5410(int a, const char *s, unsigned char c)
{
    G17MSG m;

    m.type = 0x37;
    G17_I(m, 0) = a;
    m.data[0x34] = c;
    strncpy((char *)&m.data[4], s, 0x2f);
    ai_Unk4c39c0(&m);
}
/* 0x4c5460, sends message 0x36: its handler (0x4ca8a0) lets the person "sp_<a>" use the gates of building b (room object c) */

void cm_SendChrUseGates(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x36;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c54a0, message 0x38 */

void cm_SendOwnership(int a, int b, int c)
{
    G17MSG m;

    m.type = 0x38;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c54e0, message 0x3a */

void ai_Unk4c54e0(int a)
{
    G17MSG m;

    m.type = 0x3a;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5510, message 0x3b */

void cm_SendBuildingDestroyed(int a)
{
    G17MSG m;

    m.type = 0x3b;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5540, message 0x39 (b first) */

void cm_SendBoss(int a, int b)
{
    G17MSG m;

    m.type = 0x39;
    G17_I(m, 4) = a;
    G17_I(m, 0) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c5580, message 0x3c */

void ai_Unk4c5580(int a, int b, int c, int d)
{
    G17MSG m;

    m.type = 0x3c;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    G17_I(m, 0xc) = d;
    ai_Unk4c39c0(&m);
}
/* 0x4c55d0, message 0x3d about p at (c, d) */

int cm_SendMessage3d(char *p, unsigned char b, int c, int d)
{
    G17MSG m;
    unsigned short *q;
    char *r;

    m.type = 0x3d;
    if (p == 0)
        return -1;
    q = (unsigned short *)gm_FindApplicant((struct ORDBLD *)p, c, d);
    G17_I(m, 0) = *(int *)(p + 1);
    if (q == 0) {
        G17_I(m, 9) = -1;
        r = (char *)gm_FindHome(0xffff, (struct ORDBLD *)p);
    } else {
        G17_I(m, 9) = *(int *)&q[2];
        r = *(int *)&q[0xb8] == 0 ? (char *)gm_FindHome(q[0], (struct ORDBLD *)p) : 0;
    }
    if (r != 0)
        G17_I(m, 4) = *(int *)(r + 1);
    else
        G17_I(m, 4) = -1;
    m.data[8] = b;
    m.data[0xd] = (unsigned char)c;
    m.data[0xe] = (unsigned char)d;
    return ai_Unk4c39c0(&m);
}
/* 0x4c5690, message 0x3e: the character a and a text, the model name it takes (reasoned guess: its one caller,
   gm_DailyCharacters, sends "stallbursche_MANN" / "magd_FRAU" by the sex when a character comes of age) */

void cm_SendCharModel(int a, const char *s)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x3e;
    strcpy((char *)&m.data[4], s);
    ai_Unk4c39c0(&m);
}
/* 0x4c56d0, message 0x3f: k and 0x80 bytes of p */

void cm_SendGoodsRecord(void *p, unsigned char k)
{
    G17MSG m;

    m.type = 0x3f;
    m.data[0] = k;
    memcpy(&m.data[1], p, 0x80);
    ai_Unk4c39c0(&m);
}
/* 0x4c5710, message 0x40: the differences of p (0x80 bytes) to the object k / id of list k */

void cm_SendGoodsDelta(char *p, unsigned char k)
{
    G17MSG m;
    char t[0x80];
    char *o;

    o = gm_FindGoods(k, *(short *)p);
    m.type = 0x40;
    memcpy(t, p, 0x80);
    *(float *)&m.data[0xf] = *(float *)(t + 0x20) - *(float *)(o + 0x20);
    *(float *)&m.data[0x13] = *(float *)(t + 0x24) - *(float *)(o + 0x24);
    *(float *)(t + 0x2c) -= *(float *)(o + 0x2c);
    *(float *)(t + 0x38) -= *(float *)(o + 0x38);
    G17_I(m, 3) = *(int *)(t + 0x10) - *(int *)(o + 0x10);
    G17_S(m, 1) = *(short *)p;
    G17_I(m, 7) = *(int *)(t + 0x14) - *(int *)(o + 0x14);
    G17_I(m, 0xb) = *(int *)(t + 0x18) - *(int *)(o + 0x18);
    m.data[0] = k;
    *(float *)&m.data[0x17] = *(float *)(t + 0x2c);
    *(float *)&m.data[0x1b] = *(float *)(t + 0x38);
    ai_Unk4c39c0(&m);
}
/* 0x4c5800, message 0x41 (0x3a bytes of p) */

void cm_SendMessage41(void *p)
{
    G17MSG m;

    m.type = 0x41;
    memcpy(m.data, p, 0x3a);
    ai_Unk4c39c0(&m);
}
/* 0x4c5840, message 0x42 */

void ai_Unk4c5840(int a)
{
    G17MSG m;

    m.type = 0x42;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5870, message 0x43 */

void ai_Unk4c5870(int a)
{
    G17MSG m;

    m.type = 0x43;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c58a0, message 0x44: sends the office application (the int id, the two office bytes and the kind of p, G17B7) and returns the
   send slot of 0x4c39c0 */

int cm_SendApplication(char *p)
{
    G17MSG m;

    m.type = 0x44;
    *(G17B7 *)m.data = *(G17B7 *)p;
    return ai_Unk4c39c0(&m);
}
/* 0x4c58e0, message 0x45: sends the 15-byte office record p (amt_SendOfficeMessage's AMTMSG; the int id, the two character ids, the
   kind and the code) and returns the send slot of 0x4c39c0 */

int cm_SendOfficeRecord(char *p)
{
    G17MSG m;

    m.type = 0x45;
    *(G17B15 *)m.data = *(G17B15 *)p;
    return ai_Unk4c39c0(&m);
}
/* 0x4c5930, sends message 0x46, an AI decision record's new value (the 9 bytes of p, a GVDECMSG of gv_SetTableRecord); the send's result */

int cm_SendDecisionValue(char *p)
{
    G17MSG m;

    m.type = 0x46;
    *(G17B9 *)m.data = *(G17B9 *)p;
    return ai_Unk4c39c0(&m);
}
/* 0x4c5970, message 0x47 */

void ai_Unk4c5970(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x47;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c59b0, sends message 0x48: its handler (0x4cb9b0) gives the character id the job (gm_SetJob) */

void cm_SendSetJob(int id, int job)
{
    G17MSG m;

    m.type = 0x48;
    G17_I(m, 0) = id;
    m.data[4] = job;
    ai_Unk4c39c0(&m);
}
/* 0x4c59f0, message 0x49 with a text */

void ai_Unk4c59f0(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s)
{
    G17MSG m;

    m.data[4] = a;
    G17_I(m, 0xb) = c;
    G17_I(m, 7) = b;
    m.data[0xf] = d;
    m.data[0x11] = f;
    m.type = 0x49;
    m.data[0x10] = e;
    strcpy((char *)&m.data[0x16], s);
    ai_Unk4c39c0(&m);
}
/* 0x4c5a70, message 0x4a */

void ai_Unk4c5a70(int a)
{
    G17MSG m;

    m.type = 0x4a;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5aa0, message 0x4b (0x80 bytes of p) */

void cm_SendChat(void *p)
{
    G17MSG m;

    m.type = 0x4b;
    memcpy(m.data, p, 0x80);
    ai_Unk4c39c0(&m);
}
/* 0x4c5ae0, "cm_RequestCreateGeb": message 0x4c, a building of kind k at pos */

int cm_RequestCreateGeb(int a, int k, float *pos, int *v)
{
    G17MSG m;
    char buf[0x100];

    m.type = 0x4c;
    if (k == 0)
        return -1;
    m.data[8] = (unsigned char)k;
    G17_I(m, 0) = a;
    *(float *)&m.data[9] = pos[0];
    *(float *)&m.data[0xd] = pos[1];
    *(float *)&m.data[0x11] = pos[2];
    *(float *)&m.data[0x15] = pos[3];
    G17_I(m, 0x19) = v[0];
    G17_I(m, 0x1d) = v[1];
    G17_I(m, 0x21) = v[2];
    sprintf(buf, "cm_RequestCreateGeb(): pos %f   %f   %f", pos[0], pos[1], pos[2]);
    return ai_Unk4c39c0(&m);
}
/* 0x4c5b90, message 0x4d */

void ai_Unk4c5b90(int a)
{
    G17MSG m;

    m.type = 0x4d;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5bc0, message 0x4e with two texts */

void ai_Unk4c5bc0(const char *s, int a, int b, const char *s2)
{
    G17MSG m;

    m.type = 0x4e;
    strcpy((char *)m.data, s);
    G17_I(m, 0x18) = a;
    G17_I(m, 0x1c) = b;
    if (s2 != 0)
        strcpy((char *)&m.data[0x20], s2);
    else
        m.data[0x20] = 0;
    ai_Unk4c39c0(&m);
}
/* 0x4c5c40, message 0x50 (0x2c bytes of p, the id *ai_Unk6f0d90) */

void ai_Unk4c5c40(int a, void *p)
{
    G17MSG m;

    m.type = 0x50;
    G17_I(m, 0) = a;
    if (ai_Unk6f0d90 == 0)
        G17_I(m, 0x30) = -1;
    else
        G17_I(m, 0x30) = *ai_Unk6f0d90;
    memcpy(&m.data[4], p, 0x2c);
    ai_Unk4c39c0(&m);
}
/* 0x4c5ca0, message 0x51, as 0x4c5c40 */

void ai_Unk4c5ca0(int a, void *p)
{
    G17MSG m;

    m.type = 0x51;
    G17_I(m, 0) = a;
    if (ai_Unk6f0d90 == 0)
        G17_I(m, 0x30) = -1;
    else
        G17_I(m, 0x30) = *ai_Unk6f0d90;
    memcpy(&m.data[4], p, 0x2c);
    ai_Unk4c39c0(&m);
}
/* 0x4c5d00, message 0x52 */

void ai_Unk4c5d00(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x52;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c5d40, message 0x53 (0x14 bytes of p) */

void cm_SendScriptMessage(void *p)
{
    G17MSG m;

    m.type = 0x53;
    memcpy(m.data, p, 0x14);
    ai_Unk4c39c0(&m);
}
/* 0x4c5d80, message 0x54 (three ints of p) */

void cm_SendTableRequest(int *p)
{
    G17MSG m;

    m.type = 0x54;
    *(G17B12 *)m.data = *(G17B12 *)p;
    ai_Unk4c39c0(&m);
}
/* 0x4c5dc0, message 0x55: 0x2c bytes of p, the position / direction of o's model; k 2: also the id of 0x4ce130 */

void ai_Unk4c5dc0(char *o, int k, void *p)
{
    G17MSG m;
    char *v, *r;

    G17_I(m, 0) = *ai_Unk6f0d90;
    m.type = 0x55;
    m.data[4] = (unsigned char)k;
    memcpy(&m.data[0x25], p, 0x2c);
    if (o != 0 && *(int *)(o + 0x184) != 0) {
        v = *(char **)(*(char **)(o + 0x184) + 0x34);
        G17_I(m, 0x21) = *(int *)(o + 0x24);
        G17_I(m, 5) = *(int *)(v + 0x4c);
        G17_I(m, 9) = *(int *)(v + 0x50);
        G17_I(m, 0xd) = *(int *)(v + 0x54);
        G17_I(m, 0x15) = *(int *)(v + 0x84);
        G17_I(m, 0x19) = *(int *)(v + 0x88);
        G17_I(m, 0x1d) = *(int *)(v + 0x8c);
        if (k == 2) {
            r = ai_Unk4ce130(G17_I(m, 0x35));
            if (r != 0)
                G17_I(m, 0x4d) = *(int *)(r + 0x24);
        }
    }
    ai_Unk4c39c0(&m);
}
/* 0x4c5e80, message 0x56 (0x28 bytes of p) */

void cm_SendMessage56(void *p)
{
    G17MSG m;

    m.type = 0x56;
    memcpy(m.data, p, 0x28);
    ai_Unk4c39c0(&m);
}
/* 0x4c5ec0, message 0x57 */

void ai_Unk4c5ec0(int a)
{
    G17MSG m;

    m.type = 0x57;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5ef0, message 0x58 */

void ai_Unk4c5ef0(int a)
{
    G17MSG m;

    m.type = 0x58;
    G17_I(m, 0) = a;
    ai_Unk4c39c0(&m);
}
/* 0x4c5f20, message 0x59 (0x48 bytes of p) */

void ai_Unk4c5f20(void *p)
{
    G17MSG m;

    m.type = 0x59;
    memcpy(m.data, p, 0x48);
    ai_Unk4c39c0(&m);
}
/* 0x4c5f60, message 0x5a */

void cm_SendMoneyDelta(int a, int b)
{
    G17MSG m;

    m.type = 0x5a;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    ai_Unk4c39c0(&m);
}
/* 0x4c5fa0, message 0x5b */

void cm_SendMessage5b(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x5b;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c5fe0, message 0x5c: the 10-byte trade record p (the office-swap request of amt_SendOfficeSwap) */

void cm_SendTradeMessage(char *p)
{
    G17MSG m;

    m.type = 0x5c;
    *(G17B10 *)m.data = *(G17B10 *)p;
    ai_Unk4c39c0(&m);
}
/* 0x4c6020, message 0x5d */

void ai_Unk4c6020(int a, int b, int c)
{
    G17MSG m;

    G17_I(m, 0) = a;
    m.type = 0x5d;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    ai_Unk4c39c0(&m);
}
/* 0x4c6060, message 0x5e: o's id (+1) and 0x1c bytes of p */

void ai_Unk4c6060(char *o, void *p)
{
    G17MSG m;

    G17_I(m, 4) = *(int *)(o + 1);
    m.type = 0x5e;
    memcpy(&m.data[8], p, 0x1c);
    ai_Unk4c39c0(&m);
}
/* 0x4c60b0, message 0x60 (0x1f bytes of p) */

void cm_SendGraveOrder(void *p)
{
    G17MSG m;

    m.type = 0x60;
    memcpy(m.data, p, 0x1f);
    ai_Unk4c39c0(&m);
}
/* 0x4c60f0, message 0x61 (0x19 bytes of p), the slot handle ai_Unk4c39c0 returns */

int cm_SendGraveMove(void *p)
{
    G17MSG m;

    m.type = 0x61;
    memcpy(m.data, p, 0x19);
    return ai_Unk4c39c0(&m);
}
/* 0x4c6130, message 0x62 (0x19 bytes of p) */

void cm_SendRaiseDead(void *p)
{
    G17MSG m;

    m.type = 0x62;
    memcpy(m.data, p, 0x19);
    ai_Unk4c39c0(&m);
}
/* 0x4c6170, message 99 (0x24 bytes of p) */

void cm_SendCreateAlchemist(void *p)
{
    G17MSG m;

    m.type = 99;
    memcpy(m.data, p, 0x24);
    ai_Unk4c39c0(&m);
}
/* 0x4c61b0, message 100 (0x18 bytes of p) */

void cm_SendCommand(void *p)
{
    G17MSG m;

    m.type = 100;
    memcpy(m.data, p, 0x18);
    ai_Unk4c39c0(&m);
}
/* 0x4c61f0, message 0x65 */

void ai_Unk4c61f0(int *p, char *q)
{
    G17MSG m;

    m.type = 0x65;
    G17_I(m, 0) = *p;
    G17_I(m, 4) = *(int *)(q + 4);
    ai_Unk4c39c0(&m);
}
/* 0x4c6230, message 0x66 (two ints of p); returns what ai_Unk4c39c0 returns */

int ai_Unk4c6230(int *p)
{
    G17MSG m;

    m.type = 0x66;
    G17_I(m, 0) = p[0];
    G17_I(m, 4) = p[1];
    return ai_Unk4c39c0(&m);
}
/* 0x4c6260, sends message 0x67, the 0x80 bytes of p: cannon balls fired at a building (GVSHOTMSG, gv.c gv_SendShots; the receiver
   starts them in the effect slot, gv.c 0x4eee00); returns what ai_Unk4c39c0 returns */

int cm_SendShots(void *p)
{
    G17MSG m;

    m.type = 0x67;
    memcpy(m.data, p, 0x80);
    return ai_Unk4c39c0(&m);
}
/* 0x4c62a0, message 0x68: the route points pts (0x40 bytes each, next at +0x3c) of o as the long message */

void ai_Unk4c62a0(char *o, char *pts)
{
    G17MSG m;
    char buf[0x200];
    char *d, *next;
    int i;

    G17_I(m, 4) = rt_PointCount(pts);
    G17_I(m, 0) = *(int *)(o + 2);
    m.type = 0x68;
    for (i = G17_I(m, 4), d = buf; i > 0; i--) {
        next = *(char **)(pts + 0x3c);
        memcpy(d, pts, 0x40);
        d += 0x40;
        pts = next;
    }
    ai_Unk4c4480(G17_I(m, 4) << 6, buf);
    ai_Unk4c39c0(&m);
}
/* 0x4c6320, message 0x69: o's id (+2) */

void ai_Unk4c6320(char *o)
{
    G17MSG m;

    m.type = 0x69;
    G17_I(m, 0) = *(int *)(o + 2);
    ai_Unk4c39c0(&m);
}
/* 0x4c6350, check of message 0xf: 1 if the owner of id has less than the amount of item k */

int ai_Unk4c6350(G17MSG *m)
{
    char *x, *y, *z;
    int *v;

    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_FindById(&x, &y, &z, *(int *)&m->data[4]) == 0)
            return 1;
        if (x != 0)
            v = (int *)(x + 0x5d);
        else if (y != 0)
            v = (int *)(y + 0x14);
        else
            v = (int *)(z + 0x178);
        if (gm_SumSlotAmounts(*v, m->data[0xc]) - *(int *)&m->data[0xd] < 0)
            return 1;
    }
    return 0;
}
/* 0x4c63e0, check of message 0x11 (trade): 1 if the buyer lacks room / the seller lacks the goods or the money */

int ai_Unk4c63e0(G17MSG *m)
{
    char *x, *y, *z, *o, *r;
    int *v;

    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_FindById(&x, &y, &z, *(int *)&m->data[4]) == 0)
            return 1;
        if (x != 0)
            v = (int *)(x + 0x5d);
        else if (y != 0)
            v = (int *)(y + 0x14);
        else
            v = (int *)(z + 0x178);
        o = gm_Query(*v, 1, 0, *(unsigned short *)&m->data[8]);
        if (o == 0)
            return 1;
        if (*(int *)(o + 0xe) < *(int *)&m->data[0xf])
            return 1;
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
            return 1;
        if (x != 0) {
            if (*(unsigned short *)(x + 0x27) != 0xffff)
                r = ai_Chars[*(unsigned short *)(x + 0x27)];
            else
                r = 0;
        } else if (y != 0) {
            if (*(short *)y == 0x2a || *(short *)y == 0x116 || *(short *)y == 0x1dd) {
                if (gm_FreeCapacity(y, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                    return 1;
            }
            r = gm_ObjectBuildingOwner(y);
        } else {
            if (gm_TakeableCount(z, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                return 1;
            r = z;
        }
        if (*(int *)&m->data[0x13] != 0 && r != 0) {
            if (gm_SumSlotAmounts(*(int *)(r + 0x178), m->data[0xe]) < *(int *)&m->data[0xf] * *(int *)&m->data[0x13])
                return 1;
        }
    }
    return 0;
}
/* 0x4c65b0, check of message 0x16 (field deltas): 1 if a field is the id (+4) or a character reference (+0x5c) that
   does not belong to the object */

int ai_Unk4c65b0(G17MSG *m)
{
    char *x, *y, *z, *base, *a, *c;
    unsigned char *p;
    unsigned int i, j, n;
    int size;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    if (x != 0)
        base = x;
    else if (y != 0)
        base = y;
    else
        base = z;
    p = &m->data[5];
    for (i = 0; i < m->data[4]; i++) {
        size = (char)p[0];
        n = (char)p[1];
        a = base + *(unsigned short *)(p + 2);
        p += 4;
        if (z != 0) {
            if (a == z + 4)
                return 1;
            if (a == z + 0x5c) {
                c = gm_FindChar(*(int *)p + *(int *)(z + 0x5c));
                if (c == 0)
                    return 1;
                if (*(int *)(c + 0x5c) != -1 && *(int *)(c + 0x5c) != *(int *)(z + 4))
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
/* 0x4c66c0, check of message 0x19: 1 if it sets flag 0x800000 of field 0x1c8 of a character that has it */

int ai_Unk4c66c0(G17MSG *m)
{
    char *x, *y, *z, *base;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    base = x;
    if (base == 0) {
        base = y;
        if (base == 0)
            base = z;
    }
    if (z != 0 && base + *(int *)&m->data[4] == z + 0x1c8 && (*(int *)&m->data[0xc] & ((unsigned)&ai_Relations + 0x1aca0)) && (*(int *)(z + 0x1c8) & ((unsigned)&ai_Relations + 0x1aca0)))
        return 1;
    return 0;
}
/* 0x4c6750, check of message 0x38: 1 unless building id has flag 2 at 0x5a */

int ai_Unk4c6750(G17MSG *m)
{
    char *b;

    b = gm_BuildQuery(1, 1, *(int *)&m->data[0]);
    if (b != 0 && (b[0x5a] & 2))
        return 0;
    return 1;
}
/* 0x4c6780, check of message 0x44 */

int ai_Unk4c6780(G17MSG *m)
{
    return amt_CheckApplication(m->data) == 0;
}
/* 0x4c67a0, check of message 0x45 */

int ai_Unk4c67a0(G17MSG *m)
{
    return amt_CheckOfficeMessage(m->data) == 0;
}
/* 0x4c67c0, check of message 0x53 ('cler' / 'buy ' on the object 300 of building data[8]) */

int ai_Unk4c67c0(G17MSG *m)
{
    char *b, *o;
    int i;

    if (*(int *)&m->data[0] == 0x636c6572) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0) {
            o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 300);
            if (o != 0)
                return *(int *)ai_Time - *(int *)(o + 0x28) <= 1;
        }
    } else if (*(int *)&m->data[0] == 0x62757920) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0) {
            o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 300);
            if (o != 0) {
                for (i = 0; i < 3; i++)
                    if (((int *)(o + 0x1c))[i] == *(int *)&m->data[0xc])
                        return 0;
            }
        }
    }
    return 1;
}
/* 0x4c6870, check of message 0x54 ('join' / 'leav' / 'cler' on the object 0x12d of building data[8]) */

int ai_Unk4c6870(G17MSG *m)
{
    char *b, *o, *c;
    int i;
    unsigned short k;

    if (*(int *)&m->data[0] == 0x6a6f696e) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0 && (o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d)) != 0) {
            for (i = 0; i < 4; i++)
                if (((int *)(o + 0x1c))[i] == *(int *)&m->data[4])
                    return 1;
            for (i = 0; i < 4; i++)
                if (((int *)(o + 0x1c))[i] == -1)
                    return i > 3;
        }
    } else if (*(int *)&m->data[0] == 0x6c656176) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0 && (o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d)) != 0) {
            for (i = 0; i < 4; i++)
                if (((int *)(o + 0x1c))[i] == *(int *)&m->data[4])
                    break;
            if (i < 4)
                return 0;
        }
    } else if (*(int *)&m->data[0] == 0x636c6572 && (b = gm_BuildQuery(1, 1, *(int *)&m->data[8])) != 0) {
        k = *(unsigned short *)(b + 0x27);
        if (k != 0xffff) {
            c = ai_Chars[k];
            if (c != 0 && c != ai_PlayerCharC && c[8] != 0 && c[2] != 0xf)
                return 1;
        }
        return gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d) == 0;
    }
    return 1;
}
/* 0x4c69c0, check of message 0x59: 1 if one of the 16 characters is (6 / 7) bound to another id */

int ai_Unk4c69c0(G17MSG *m)
{
    char *c;
    int i;

    if (*(int *)&m->data[0] != -1) {
        for (i = 0; i < 0x10; i++) {
            c = gm_FindChar(((int *)&m->data[4])[i]);
            if (c != 0 && (c[2] == 6 || c[2] == 7) && *(int *)(c + 0x20c) != -1 && *(int *)(c + 0x20c) != *(int *)&m->data[0])
                return 1;
        }
        return 0;
    }
    return 1;
}
/* 0x4c6a20, handler of the types 0, 3, 5, 6, 7 (nothing to do); 1 for any other type */

int ai_Unk4c6a20(G17MSG *m, G17STAT *s)
{
    if (m->type != 0 && m->type != 5 && m->type != 6 && m->type != 7)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6a50, handler of type 1 */

int ai_Unk4c6a50(G17MSG *m, G17STAT *s)
{
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6a60, handler of type 4: sets flags of cm_NetFlags */

int ai_Unk4c6a60(G17MSG *m, G17STAT *s)
{
    cm_NetFlags |= m->data[0];
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6a90, handler of type 8: allocates the load buffer */

int ai_Unk4c6a90(G17MSG *m, G17STAT *s)
{
    cm_LoadSize = *(int *)&m->data[0];
    cm_LoadBuf = mem_Alloc(cm_LoadSize, "cm_LoadBuf");
    cm_LoadReceived = 0;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6ad0, handler of type 9: appends 0x80 bytes to the load buffer */

int ai_Unk4c6ad0(G17MSG *m, G17STAT *s)
{
    memcpy(cm_LoadBuf + cm_LoadReceived, m->data, 0x80);
    cm_LoadReceived += 0x80;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6b20, handler of type 10: creates a building of kind data[4] for the character data[6] */

int ai_Unk4c6b20(G17MSG *m, G17STAT *s)
{
    char *b;
    unsigned short k;
    int id;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0];
    if (*(int *)&m->data[6] == -1) {
        k = 0xffff;
    } else {
        id = ai_Unk4c4450((int)m, *(int *)&m->data[6]);
        *(int *)&m->data[6] = id;
        for (k = 0; k < 0x300; k++)
            if (*(unsigned short *)ai_Chars[k] != 0xffff && *(int *)(ai_Chars[k] + 4) == id)
                break;
        if (k >= 0x300)
            return 1;
    }
    b = gm_CreateBuilding(m->data[4], k);
    if (b == 0)
        return 1;
    cm_LastBuildingId = *(int *)(b + 1);
    *(short *)(b + 0x5a) = *(short *)&m->data[0xa];
    if (*(int *)&m->data[0xc] != 0)
        memcpy(b + 0x65, &m->data[0x10], 0x30);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 2;
        s->value = (int)b;
    }
    return 0;
}
/* 0x4c6bf0, handler of type 0xb: creates a character (0x507690), seeded with data[0x16] */

int ai_Unk4c6bf0(G17MSG *m, G17STAT *s)
{
    char *b;
    unsigned int k;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0];
    else
        *(int *)&m->data[0x16] = rand();
    util_Seed(*(int *)&m->data[0x16]);
    if (*(int *)&m->data[0xf] == -1) {
        b = 0;
    } else {
        *(int *)&m->data[0xf] = ai_Unk4c4450((int)m, *(int *)&m->data[0xf]);
        b = gm_BuildQuery(1, 1, *(int *)&m->data[0xf]);
        if (b == 0)
            return 1;
    }
    k = gm_CreateCharacter(m->data[4], *(int *)&m->data[5], *(int *)&m->data[9], *(unsigned short *)&m->data[0xd], b, m->data[0x13], m->data[0x14], m->data[0x15]);
    if (k == 0xffff)
        return 1;
    cm_LastCharId = *(int *)(ai_Chars[k] + 4);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[k];
    }
    return 0;
}
/* 0x4c6cd0, handler of type 0xc: creates a named character (0x507690), names it (data[0x15], data[0x25]) */

int ai_Unk4c6cd0(G17MSG *m, G17STAT *s)
{
    char *c, *t, *r;
    unsigned int k;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0];
    else
        *(int *)&m->data[0x35] = rand();
    util_Seed(*(int *)&m->data[0x12]);
    k = gm_CreateCharacter(s != 0 ? 6 : 7, *(int *)&m->data[4], *(int *)&m->data[8], *(unsigned short *)&m->data[0xc], 0, m->data[0xe], 0, m->data[0x13]);
    if (k == 0xffff)
        return 1;
    c = ai_Chars[k];
    cm_LastCharId = *(int *)(c + 4);
    strncpy(c + 0x30, (char *)&m->data[0x15], 0x10);
    t = gm_PlayerRecord(c);
    if (t != 0) {
        *(short *)t = *(short *)(c + 0x50);
        strncpy(t + 2, (char *)&m->data[0x25], 0x10);
        strncpy(c + 0x40, (char *)&m->data[0x25], 0x10);
        r = gm_FindChar(*(int *)(c + 0x60));
        if (r != 0)
            strncpy(r + 0x40, (char *)&m->data[0x25], 0x10);
        *(float *)(t + 0x88) = -1.0f;
    }
    *(int *)(c + 0x54) = *(int *)&m->data[0xf];
    c[0xc] = m->data[0x14];
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)c;
    }
    return 0;
}
/* 0x4c6e10, handler of type 0xd: character b joins a (0x5064d0) */

int ai_Unk4c6e10(G17MSG *m, G17STAT *s)
{
    char *c, *y;
    int *x;
    int k;

    k = gm_TakeOverCharacter(*(int *)&m->data[0], *(int *)&m->data[4]);
    if (s != 0) {
        s->state = 1;
        if (k >= 0) {
            c = gm_FindChar(*(int *)&m->data[4]);
            if (gv_FindHeldEntry(c, &y, &x) != 0)
                gv_OrderBury(c, y, *x, gv_EntryIndex(*(int *)(y + 0x65), x));
            s->unk01 = 1;
            s->value = (int)ai_Chars[k];
        } else {
            s->unk01 = 0;
            s->value = 0;
        }
    }
    return 0;
}
/* 0x4c6ec0, handler of type 0xe: removes the object id */

int ai_Unk4c6ec0(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z;

    *(unsigned short *)&m->data[0] = (unsigned short)ai_Unk4c4450((int)m, *(unsigned short *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(unsigned short *)&m->data[0]) == 0)
        return 1;
    if (x != 0) {
        if (gm_FreeBuilding(x) != 0)
            return 1;
    } else {
        if (y != 0)
            return 1;
        gm_KillSpieler(*(unsigned short *)z, 1);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c6f60, handler of type 0xf: moves n of the player good k from a to b (checked) */

int ai_Unk4c6f60(G17MSG *m, G17STAT *s)
{
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[8];
    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_TakeObjekt(*(int *)&m->data[4], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]) == 0)
            return 1;
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        if (gm_AddObjekt(*(int *)&m->data[0], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]) == 0)
            return 1;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7010, handler of type 0x10: as 0xf, unchecked (0x500ae0) */

int ai_Unk4c7010(G17MSG *m, G17STAT *s)
{
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[8];
    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        gm_AddNegative(*(int *)&m->data[4], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]);
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        gm_AddObjekt(*(int *)&m->data[0], *(unsigned short *)(ai_PlayerSlots[m->data[0xc]] + 0x54), *(int *)&m->data[0xd]);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c70b0, handler of type 0x11, "cm_ExSellObjekt": moves n of item from data[4] (seller) to data[0] (buyer) at
   the price data[0x13] each (money in the player good k) */

int ai_Unk4c70b0(G17MSG *m, G17STAT *s)
{
    char *r1, *y, *o2, *ys, *y2, *x2, *z, *x1, *o, *r2, *g;
    int *p, *q;
    unsigned int t;
    char buf[0x100];
    char text[0x7c];

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
        ai_NextObjectId = *(int *)&m->data[0xa];
    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_FindById(&x1, &y, &z, *(int *)&m->data[4]) == 0)
            return 1;
        if (x1 != 0) {
            p = (int *)(x1 + 0x5d);
            if (*(unsigned short *)(x1 + 0x27) != 0xffff)
                r1 = ai_Chars[*(unsigned short *)(x1 + 0x27)];
            else
                r1 = 0;
        } else if (y != 0) {
            if (*(short *)y == 0x2a || *(short *)y == 0x1db || *(short *)y == 0x1dc || *(short *)y == 0x116 || *(short *)y == 0x1dd)
                ys = y;
            p = (int *)(y + 0x14);
            r1 = gm_ObjectBuildingOwner(y);
            if (r1 == 0)
                return 1;
        } else {
            r1 = z;
            p = (int *)(z + 0x178);
        }
        o = gm_Query(*p, 1, 0, *(unsigned short *)&m->data[8]);
        if (o == 0) {
            sprintf(buf, "cm_ExSellObjekt(): Object not found: ObjProt:%i Menge:%i CmdCnt:", *(unsigned short *)&m->data[8], *(int *)&m->data[0xf], m->seq);
            return 1;
        }
        if (ys != 0 && gm_ListAmount(ys, o) < *(int *)&m->data[0xf]) {
            sprintf(buf, "cm_ExSellObjekt() Not enough Objekts at source: ObjProt:%i Menge:%i CmdCnt:%i", *(unsigned short *)&m->data[8], *(int *)&m->data[0xf], m->seq);
            return 1;
        }
        if (*(int *)(o + 0xe) < *(int *)&m->data[0xf]) {
            sprintf(buf, "cm_ExSellObjekt() Not enough Objekts at source: ObjProt:%i Menge:%i CmdCnt:%i", *(unsigned short *)&m->data[8], *(int *)&m->data[0xf], m->seq);
            return 1;
        }
        if (*(int *)&m->data[0x13] != 0 && r1 != 0)
            gm_SumSlotAmounts(*(int *)(r1 + 0x178), m->data[0xe]);
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        if (gm_FindById(&x2, &y, &z, *(int *)&m->data[0]) == 0)
            return 1;
        if (x2 != 0) {
            q = (int *)(x2 + 0x5d);
            if (*(unsigned short *)(x2 + 0x27) != 0xffff)
                r2 = ai_Chars[*(unsigned short *)(x2 + 0x27)];
            else
                r2 = 0;
        } else if (y != 0) {
            if (*(short *)y == 0x2a || *(short *)y == 0x1db || *(short *)y == 0x1dc || *(short *)y == 0x116) {
                y2 = y;
                if (gm_FreeCapacity(y, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                    return 1;
            } else if (*(short *)y == 0x1dd) {
                if (gm_FreeCapacity(y, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                    return 1;
            }
            q = (int *)(y + 0x14);
            r2 = gm_ObjectBuildingOwner(y);
        } else {
            q = (int *)(z + 0x178);
            r2 = z;
        }
        if (ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 0x1d)
            o2 = 0;
        else
            o2 = gm_Query(*q, 1, 0, *(unsigned short *)&m->data[8]);
        if (*(int *)&m->data[0x13] != 0 && r2 != 0) {
            if (gm_SumSlotAmounts(*(int *)(r2 + 0x178), m->data[0xe]) < *(int *)&m->data[0xf] * *(int *)&m->data[0x13])
                return 1;
        }
    }
    if (ys != 0) {
        if (r2 == 0)
            goto money;
        if (gm_ObjectBuildingOwner(o) != r2 && *(int *)(o + 0x1c) != 0) {
            t = (unsigned int)(gm_ListStock(ys, *(unsigned short *)o) * *(int *)(o + 0x1c)) >> 2;
            if ((unsigned int)(gm_ListAmount(ys, o) - t) < (unsigned int)*(int *)&m->data[0xf])
                return 1;
        }
    }
    if (r2 != 0 && x2 == 0 && y2 == 0 && y == 0) {
        *(int *)&m->data[0xf] = gm_TakeableCount(r2, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]);
        if (*(int *)&m->data[0xf] == 0)
            return 1;
    }
money:
    if (*(int *)&m->data[0x13] != 0) {
        if (y2 == 0 && y != 0 && *(short *)y == 0x1dd) {
            g = gm_FindGoods(ai_OwnPlayer, *(unsigned short *)&m->data[8]);
            if (g != 0)
                *(int *)(g + 0x18) += *(int *)&m->data[0xf];
        }
        if (ys != 0 && *(short *)ys == 0x1dd) {
            g = gm_FindGoods(ai_OwnPlayer, *(unsigned short *)&m->data[8]);
            if (g != 0)
                *(int *)(g + 0x1c) += *(int *)&m->data[0xf];
        }
    }
    if (*(int *)&m->data[4] != -1) {
        *(int *)(o + 0xe) -= *(int *)&m->data[0xf];
        if (*(int *)(o + 0xe) <= 0) {
            if (ai_Goods[*(unsigned short *)&m->data[8] * 0x41] != 2 && ai_Goods[*(unsigned short *)&m->data[8] * 0x41] != 6) {
                gm_RemoveObject(p, *(unsigned short *)&m->data[8]);
            } else {
                if (x1 == 0)
                    return 1;
                gm_RemoveObjectFromBuilding(x1, *(unsigned short *)&m->data[8]);
            }
        }
        if (*(int *)&m->data[0x13] != 0) {
            gm_AddObjekt(*(int *)(r1 + 4), *(unsigned short *)(ai_PlayerSlots[m->data[0xe]] + 0x54), *(int *)&m->data[0xf] * *(int *)&m->data[0x13]);
            if (ys != 0 && *(short *)ys == 0x1db)
                *(int *)(r1 + 0x1a4) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else if (ys != 0 && *(short *)ys == 0x1dc)
                *(int *)(r1 + 0x1a0) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else
                *(int *)(r1 + 0x19c) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
        }
    }
    if (*(int *)&m->data[0] != -1) {
        if (o2 != 0) {
            *(int *)(o2 + 0xe) += *(int *)&m->data[0xf];
        } else if (ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 2 || ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 6) {
            if (x2 == 0)
                return 1;
            o2 = gm_AllocRaum(x2, *(unsigned short *)&m->data[8]);
            if (o2 == 0)
                return 1;
        } else {
            o2 = gm_CreateObject(*(int *)&m->data[0], *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]);
            if (o2 == 0)
                return 1;
            if (y2 != 0) {
                (*(int *)(o2 + 0xe))++;
                if ((ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 0x17 || ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 0x25)
                    && (*(short *)y2 == 0x2a || *(short *)y2 == 0x116) && r2 != 0 && (r2[2] == 6 || r2[2] == 7))
                    *(int *)(o2 + 0x1c) = 4;
            }
        }
        cm_LastObjectId = *(int *)(o2 + 2);
        if (*(int *)&m->data[0x13] != 0 && r2 != 0) {
            gm_TakeObjekt(*(int *)(r2 + 4), *(unsigned short *)(ai_PlayerSlots[m->data[0xe]] + 0x54), *(int *)&m->data[0xf] * *(int *)&m->data[0x13]);
            if (y2 != 0 && *(short *)y2 == 0x1db)
                *(int *)(r2 + 0x1a0) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else if (y2 != 0 && *(short *)y2 == 0x1dc)
                *(int *)(r2 + 0x1a4) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else
                *(int *)(r2 + 0x198) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
        }
    }
    if (ys != 0 && (*(short *)ys == 0x2a || *(short *)ys == 0x116) && r1 != 0 && *(int *)&m->data[0x13] != 0
        && *(unsigned short *)r1 == ai_PlayerId) {
        x1 = gm_FindBuildingById(*(int *)(ys + 6));
        if (x1 != 0) {
            txt_Format(text, 0x7c, *(unsigned char *)x1 * 14 + 0x48e, *(int *)&m->data[0x13]);
            sv_SetStatusText(text);
            sb_PlaySampleI(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        }
    }
    if (s != 0) {
        s->state = 1;
        if (*(int *)&m->data[0] != -1) {
            s->unk01 = 3;
            s->value = (int)o2;
        }
    }
    return 0;
}
/* 0x4c78c0, handler of type 0x12: workshop y produces up to n of item from its ingredients (good + 0x26 amounts,
   + 0x2e ids, + 0x36 output per run) */

int ai_Unk4c78c0(G17MSG *m, G17STAT *s)
{
    char *y, *g, *r, *o, *st, *e;
    unsigned short *pp;
    unsigned short id;
    int n, i, v;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0xa];
    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(0, &y, 0, *(int *)&m->data[0]) == 0)
        return 1;
    if (*(short *)y != 0x2a && *(short *)y != 0x116)
        return 1;
    r = gm_ObjectBuildingOwner(y);
    if (r == 0)
        return 1;
    g = ai_Goods + *(unsigned short *)&m->data[4] * 0x41;
    n = *(int *)&m->data[6];
    pp = (unsigned short *)(g + 0x26);
    for (i = 4; i != 0; i--, pp++) {
        if (pp[4] != 0) {
            o = gm_Query(*(int *)(y + 0x14), 1, 0, pp[4]);
            if (o != 0) {
                if (n >= gm_ListAmount(y, o) / *pp)
                    n = gm_ListAmount(y, o) / *pp;
            } else {
                n = 0;
            }
        }
    }
    if (n >= gm_FreeCapacity(y, *(unsigned short *)&m->data[4], *(unsigned short *)(g + 0x36) * n) / *(unsigned short *)(g + 0x36))
        n = gm_FreeCapacity(y, *(unsigned short *)&m->data[4], *(unsigned short *)(g + 0x36) * n) / *(unsigned short *)(g + 0x36);
    if (n == 0) {
        if (s != 0)
            s->unk01 = 8;
        return 1;
    }
    for (i = 0; i < 4; i++) {
        id = ((unsigned short *)(g + 0x2e))[i];
        if (id != 0) {
            o = gm_Query(*(int *)(y + 0x14), 1, 0, id);
            if (o == 0) {
                if (s != 0)
                    s->unk01 = 8;
                return 1;
            }
            *(int *)(o + 0xe) -= ((unsigned short *)(g + 0x26))[i] * n;
            st = gm_FindGoods(ai_OwnPlayer, id);
            if (st != 0)
                *(int *)(st + 0x14) += *(unsigned short *)(g + 0x36) * n;
            if (*(int *)(o + 0xe) == 0)
                gm_RemoveObject((int *)(y + 0x14), id);
        }
    }
    o = gm_Query(*(int *)(y + 0x14), 1, 0, *(unsigned short *)&m->data[4]);
    if (o != 0) {
        *(int *)(o + 0xe) += *(unsigned short *)(g + 0x36) * n;
    } else {
        o = gm_CreateObject(*(int *)(y + 2), *(unsigned short *)&m->data[4], *(unsigned short *)(g + 0x36) * n);
        if (o == 0) {
            if (s != 0)
                s->unk01 = 6;
            return 1;
        }
        if (*(short *)y == 0x2a)
            (*(int *)(o + 0xe))++;
        if ((g[0] == 0x17 || g[0] == 0x25) && (*(short *)y == 0x2a || *(short *)y == 0x116) && (r[2] == 6 || r[2] == 7))
            *(int *)(o + 0x1c) = 4;
    }
    v = (int)(gm_ItemPrice(*(unsigned short *)&m->data[4], 100) * n);
    e = gm_ObjectBuilding(y);
    if (e != 0)
        *(int *)(e + 0x4d) += v;
    st = gm_FindGoods(ai_OwnPlayer, *(unsigned short *)&m->data[4]);
    if (st != 0)
        *(int *)(st + 0x10) += *(unsigned short *)(g + 0x36) * n;
    e = gm_PlayerRecord(r);
    if (e != 0)
        *(int *)(e + 0x24) += v;
    if (s != 0) {
        s->state = 1;
        if (*(int *)&m->data[0] != -1) {
            s->unk01 = 3;
            s->value = (int)o;
        }
    }
    return 0;
}
/* 0x4c7c30, handler of type 0x13: moves the object data[8] from the list of data[4] to the list of data[0] */

int ai_Unk4c7c30(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *o, *prev;
    int *p1, *p2;
    int c;

    *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[4]) == 0)
        return 1;
    if (x != 0)
        p1 = (int *)(x + 0x5d);
    else if (y != 0)
        p1 = (int *)(y + 0x14);
    else
        p1 = (int *)(z + 0x178);
    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    if (x != 0)
        p2 = (int *)(x + 0x5d);
    else if (y != 0)
        p2 = (int *)(y + 0x14);
    else
        p2 = (int *)(z + 0x178);
    c = ai_Unk4c4450((int)m, *(int *)&m->data[8]);
    *(int *)&m->data[8] = c;
    prev = 0;
    for (o = (char *)*p1; o != 0; o = *(char **)(o + 0x3f)) {
        if (*(int *)(o + 2) == c) {
            if (prev == 0)
                *p1 = *(int *)(o + 0x3f);
            else
                *(int *)(prev + 0x3f) = *(int *)(o + 0x3f);
            break;
        }
        prev = o;
    }
    if (o == 0)
        return 1;
    *(int *)(o + 6) = *(int *)&m->data[0];
    *(int *)(o + 0x3f) = *p2;
    *p2 = (int)o;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7d80, handler of type 0x14: removes the single item data[4] from workshop data[0] */

int ai_Unk4c7d80(G17MSG *m, G17STAT *s)
{
    char *y, *o;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(0, &y, 0, *(int *)&m->data[0]) == 0)
        return 1;
    if (*(short *)y != 0x2a && *(short *)y != 0x116)
        return 1;
    o = gm_Query(*(int *)(y + 0x14), 1, 0, *(unsigned short *)&m->data[4]);
    if (o == 0)
        return 1;
    if (*(int *)(o + 0xe) != 1)
        return 1;
    gm_RemoveObject((int *)(y + 0x14), *(unsigned short *)&m->data[4]);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7e20, handler of type 0x15: one item data[4] for data[0], with 0x1f bytes of data[6] */

int ai_Unk4c7e20(G17MSG *m, G17STAT *s)
{
    char *o;

    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0x25];
    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    o = gm_CreateObject(*(int *)&m->data[0], *(unsigned short *)&m->data[4], 1);
    if (o == 0)
        return 1;
    cm_LastObjectId = *(int *)(o + 2);
    memcpy(o + 0x1c, &m->data[6], 0x1f);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7e90, handler of type 0x16: adds the field block (deltas) to the object data[0] */

int ai_Unk4c7e90(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *base;
    unsigned char *p;
    unsigned int i, n;
    int size, id;

    id = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, id) == 0)
        return 1;
    *(int *)&m->data[0] = id;
    if (x != 0)
        base = x;
    else if (y != 0)
        base = y;
    else
        base = z;
    p = &m->data[5];
    for (i = 0; i < m->data[4]; i++) {
        size = (char)p[0];
        n = (char)p[1];
        if (size == 1) {
            unsigned char *d = (unsigned char *)base + *(unsigned short *)(p + 2);
            p += 4;
            for (; n != 0; n--)
                *d++ += *p++;
        } else if (size == 2) {
            short *d = (short *)(base + *(unsigned short *)(p + 2));
            p += 4;
            for (; n != 0; n--) {
                *d++ += *(short *)p;
                p += 2;
            }
        } else if (size == 4) {
            int *d = (int *)(base + *(unsigned short *)(p + 2));
            p += 4;
            for (; n != 0; n--) {
                *d++ += *(int *)p;
                p += 4;
            }
        } else {
            p += 4;
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c7f70, handler of type 0x17: writes the field block (values) to the object data[0] */

int ai_Unk4c7f70(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *base;
    unsigned char *p;
    unsigned int i, n;
    int size;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    if (x != 0)
        base = x;
    else if (y != 0)
        base = y;
    else
        base = z;
    p = &m->data[5];
    for (i = 0; i < m->data[4]; i++) {
        size = (char)p[0];
        n = (char)p[1];
        if (size == 1) {
            unsigned char *d = (unsigned char *)base + *(unsigned short *)(p + 2);
            p += 4;
            for (; n != 0; n--)
                *d++ = *p++;
        } else if (size == 2) {
            short *d = (short *)(base + *(unsigned short *)(p + 2));
            p += 4;
            for (; n != 0; n--) {
                *d++ = *(short *)p;
                p += 2;
            }
        } else if (size == 4) {
            int *d = (int *)(base + *(unsigned short *)(p + 2));
            p += 4;
            for (; n != 0; n--) {
                *d++ = *(int *)p;
                p += 4;
            }
        } else {
            p += 4;
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8050, handler of type 0x18: adds the (k, value) pairs of the second field block to the floats at 0x90 + 12 k
   of character data[0], each kept in 0..1000 */

int ai_Unk4c8050(G17MSG *m, G17STAT *s)
{
    char *z;
    unsigned char *p;
    unsigned int i;
    float *f;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(0, 0, &z, *(int *)&m->data[0]) == 0)
        return 1;
    p = &m->data[5];
    for (i = 0; i < m->data[4]; i++) {
        f = (float *)(z + 0x90 + p[0] * 12);
        *f += *(float *)(p + 1);
        p += 5;
        *f = min(max(*f, 0.0), 1000.0);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8110, handler of type 0x19: field data[4] (size data[8]) of object data[0]: clears mask data[0x10], sets data[0xc] */

int ai_Unk4c8110(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *a;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    a = x;
    if (a == 0) {
        a = y;
        if (a == 0)
            a = z;
    }
    a += *(int *)&m->data[4];
    switch (*(int *)&m->data[8]) {
    case 1:
        *(unsigned char *)a &= ~*(unsigned char *)&m->data[0x10];
        *(unsigned char *)a |= *(unsigned char *)&m->data[0xc];
        break;
    case 2:
        *(unsigned short *)a &= ~*(unsigned short *)&m->data[0x10];
        *(unsigned short *)a |= *(unsigned short *)&m->data[0xc];
        break;
    case 4:
        *(int *)a &= ~*(int *)&m->data[0x10];
        *(int *)a |= *(int *)&m->data[0xc];
        break;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c81d0, handler of type 0x1a: adds the float data[8] to the field data[4] of object data[0] */

int ai_Unk4c81d0(G17MSG *m, G17STAT *s)
{
    char *x, *y, *z, *a;

    *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    if (gm_FindById(&x, &y, &z, *(int *)&m->data[0]) == 0)
        return 1;
    a = z;
    if (a == 0) {
        a = y;
        if (a == 0)
            a = x;
    }
    *(float *)(a + *(int *)&m->data[4]) += *(float *)&m->data[8];
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8240, handler of type 0x1b: changes the relations (0x7e5360 / 0x889f80, 0x300 x 0x300) between the characters
   data[0] and data[4]; kind data[0xc]: 4 drifts all relations by level data[0x14], 3 scales column b by data[0x10],
   0..2 add data[8] */

int ai_Unk4c8240(G17MSG *m, G17STAT *s)
{
    int a, b, kind, i, j, v, hi, lo, dn, dp;
    unsigned int n;

    a = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
    *(int *)&m->data[0] = a;
    b = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
    kind = *(int *)&m->data[0xc];
    *(int *)&m->data[4] = b;
    if (kind == 4) {
        n = *(unsigned int *)&m->data[0x14];
        if (n != 0 && n >= 4) {
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
                if (i != j && *(unsigned short *)ai_Chars[i] != 0xffff && *(unsigned short *)ai_Chars[j] != 0xffff) {
                    if (v > hi)
                        v += dn;
                    else if (v < lo)
                        v += dp;
                    if (v <= -0x7f)
                        v = -0x7f;
                    else if (v >= 0x7f)
                        v = 0x7f;
                    ai_Relations[i][j] = (char)v;
                }
            }
        }
    } else {
        for (i = 0; i < 0x300; i++)
            if (*(int *)(ai_Chars[i] + 4) == b)
                break;
        if (i >= 0x300)
            return 1;
        if (*(unsigned short *)ai_Chars[i] == 0xffff)
            return 1;
        if (kind == 3) {
            v = *(int *)(ai_Chars[i] + 0x210);
            for (j = 0; j < 0x300; j++)
                if (*(int *)(ai_Chars[j] + 4) != v)
                    ai_RelationMemory[j][i] = (char)(int)(ai_RelationMemory[j][i] * *(float *)&m->data[0x10]);
        } else {
            for (j = 0; j < 0x300; j++)
                if (*(int *)(ai_Chars[j] + 4) == a)
                    break;
            if (j >= 0x300)
                return 1;
            if (*(unsigned short *)ai_Chars[j] == 0xffff)
                return 1;
            if (kind == 2) {
                v = ai_Relations[j][i];
                ai_RelationMemory[j][i] = 0;
                v += *(int *)&m->data[8];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[j][i] = (char)v;
            } else if (kind == 1) {
                v = ai_Relations[j][i] + *(int *)&m->data[8];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[j][i] = (char)v;
                if (*(int *)&m->data[8] >= 0)
                    v /= 2;
                v += ai_RelationMemory[j][i];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_RelationMemory[j][i] = (char)v;
            } else if (kind == 0) {
                v = ai_Relations[j][i] + *(int *)&m->data[8];
                if (v < -0x7f)
                    v = -0x7f;
                else if (v >= 0x7f)
                    v = 0x7f;
                ai_Relations[j][i] = (char)v;
            }
        }
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c85c0, handler of type 0x1c: creates the job / message box of the long message (0xf8 bytes, AIMSG) and its
   texts (after it in ai_Unka68d40) */

int ai_Unk4c85c0(G17MSG *m, G17STAT *s)
{
    char msg[0xf8];
    char *c, *o, *t1, *t2;

    memcpy(msg, ai_Unka68d40, 0xf8);
    c = gm_FindChar(*(int *)(msg + 8));
    if (net_Conn.s != INVALID_SOCKET) {
        if (msg[0x36] & 1) {
            if (c == 0)
                return 1;
            if (c[2] == 7)
                return 1;
            if (c[2] != 6) {
                if (s == 0)
                    return 1;
                goto set;
            }
        }
        if (s == 0 && (msg[0x36] & 2))
            msg[0x36] = (msg[0x36] & ~2) | 4;
set:
        ai_NextEventId = *(int *)&m->data[0];
    }
    if (msg[4] == 0x11 && he_IsMessageKindFiltered(*(int *)(msg + 0x58)) != 0)
        return 0;
    o = he_Unk53e040(msg);
    if (o == 0)
        return 1;
    if (o[0] == 0x11) {
        t1 = (char *)ai_Unka68d40 + 0xf8;
        if (o[0xf0] & 0x10) {
            t2 = t1 + strlen(t1) + 1;
            *(char **)(o + 0x7c) = mem_Alloc(strlen(t1) + strlen(t2) + 2, "messagebox");
            strcpy(*(char **)(o + 0x7c), t1);
            strcpy(*(char **)(o + 0x7c) + strlen(t1) + 1, t2);
            *(int *)(o + 0x80) = strlen(t2) + strlen(t1) + 2;
        } else {
            *(char **)(o + 0x7c) = mem_Alloc(strlen(t1) + 1, "messagebox");
            strcpy(*(char **)(o + 0x7c), t1);
            *(int *)(o + 0x80) = strlen(*(char **)(o + 0x7c)) + 1;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 4;
        s->value = (int)o;
    }
    return 0;
}
/* 0x4c87a0, handler of type 0x1d, "cm_ExSetHE": sets the three times and the text block of job data[0] */

int ai_Unk4c87a0(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    char *p;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 4;
        s->value = 0;
    }
    p = he_FindEvent(1, 1, *(int *)&m->data[0]);
    if (p == 0) {
        sprintf(buf, "cm_ExSetHE(): He not found. id:%i name:%s", *(int *)&m->data[0], &m->data[0x2f]);
        return 1;
    }
    *(G17T14 *)(p + 0x44) = *(G17T14 *)&m->data[4];
    *(G17T14 *)(p + 0x52) = *(G17T14 *)&m->data[0x12];
    *(G17T14 *)(p + 0x60) = *(G17T14 *)&m->data[0x20];
    *(int *)(p + 0x70) = (char)m->data[0x2e];
    memcpy(p + 0xac, ai_Unka68d40, 0xa0);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 4;
        s->value = (int)p;
    }
    return 0;
}
/* 0x4c88a0, handler of type 0x1e: the game time (an AITIME); when it is later than ours, the clock moves on */

int ai_Unk4c88a0(G17MSG *m, G17STAT *s)
{
    char *t = (char *)m + 0x10;
    char *p;

    if (gm_CompareTime(ai_Time, t) < 0) {
        *(G17T14 *)ai_Time = *(G17T14 *)t;
        if (ai_Unk624578)
            he_Unk53ed70();
        if (ai_Unk624574) {
            ord_AdvanceRatingBase();
            if (*(int *)&ai_Time[6] % 6 == 0)
                gm_CountCharacters();
        }
        if (ai_Unk624570)
            ai_UpdateNeeds();
        memset(ai_Unkf895a0, 0, 0xb00);
        if (ai_Unk624570) {
            ai_TradeMsgCount = 0;
            for (p = ai_Chars[0]; (int)p < (int)ai_Chars[0x300]; p += 0x21c) {
                if (*(unsigned short *)p != 0xffff)
                    ai_Unk47e990(p);
            }
            ai_UpdateCharacters();
        }
        ai_Unk5ba750();
        sv_Unk52c420();
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c8990, handler of type 0x1f */

int ai_Unk4c8990(G17MSG *m, G17STAT *s)
{
    sv_PopTo(main_GameLoop);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c89b0, handler of type 0x20 (system message): data[0] is the kind (0..0x14) */

int ai_Unk4c89b0(G17MSG *m, G17STAT *s)
{
    G17MSG msg = *m;
    char buf[0x80];
    int ret = 1;
    int i, k;
    char *p, *q;
    int t[4];
    short t4;
    int save;

    switch (msg.data[0]) {
    case 0:
        t[0] = 0;
        t[1] = G17_I(msg, 1);
        t[2] = 0;
        t[3] = 0;
        t4 = 0;
        util_Seed(G17_I(msg, 5));
        sv_SetupWeather(t, 1);
        break;
    case 2:
        cm_DayEndRequest = 1;
        if (G17_I(msg, 1) != 0)
            cm_NetFlags = (cm_NetFlags & ~0x10) | 8;
        else
            cm_NetFlags = (cm_NetFlags & ~8) | 0x10;
        break;
    case 3:
        p = tb_IsFunctionOff(sv_UpdateClock);
        if (p == 0)
            tb_SetFunctionOff(sv_UpdateClock, 1);
        *(int *)ai_Time = G17_I(msg, 1);
        *(short *)&ai_Time[4] = G17_S(msg, 5);
        *(int *)&ai_Time[6] = G17_I(msg, 7);
        *(int *)&ai_Time[10] = G17_I(msg, 0xb);
        *(G17T14 *)&ai_TimeCopy = *(G17T14 *)ai_Time;
        if (p == 0)
            tb_SetFunctionOff(sv_UpdateClock, 0);
        break;
    case 4:
        p = gm_BuildQuery(1, 1, G17_I(msg, 1));
        if (p != 0) {
            q = gm_Query(*(int *)(p + 0x5d), 1, 1, G17_I(msg, 5));
            if (q != 0)
                init_ShowBuildingScene(p, *(unsigned short *)q);
            else
                init_ShowBuildingScene(p, 0xfd);
        }
        break;
    case 5:
        util_Seed(G17_I(msg, 1));
        hst_SeedPending = 0;
        break;
    case 6:
        ai_NumPlayers = msg.data[1];
        break;
    case 7:
        ai_NetPlayer = G17_I(msg, 1);
        break;
    case 8:
        if (ai_MissionPartner == -1) {
            k = G17_I(msg, 1);
            if (gm_FindChar(k) != 0) {
                ai_MissionPartner = k;
                break;
            }
        }
        ret = 2;
        break;
    case 9:
        ai_MissionMode = msg.data[1];
        ai_MissionSeed = G17_I(msg, 5);
        break;
    case 10:
        if (ai_MissionPartner == G17_I(msg, 1)) {
            ai_MissionPartner = -1;
            if (G17_I(msg, 0x11) == 1)
                ai_GameOver = 1;
            if (ai_MissionMode == 5)
                ai_Unk4b3bd0(ai_CharInts[ai_PlayerId][0], G17_I(msg, 0x21));
        } else
            ret = 2;
        break;
    case 12:
        save = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        d3sm_Build(*(int *)(ai_Unk5fb5b0 + 0xb0), ai_Unk633738);
        d3if_SelectScene(save, 1);
        ai_Unk631084 = 0;
        break;
    case 13:
        sv_ColourBanners(G17_I(msg, 1));
        break;
    case 15:
        cm_RequestedSaveSlot = G17_I(msg, 1);
        i = 0;
        do {
            cm_RequestedSaveName[i] = msg.data[5 + i];
            i++;
        } while (msg.data[4 + i] != 0);
        break;
    case 16:
        for (i = 0; i < 8; i++) {
            if (cm_SaveAcks[i][0] == -1) {
                cm_SaveAcks[i][0] = G17_I(msg, 1);
                cm_SaveAcks[i][1] = G17_I(msg, 5);
                break;
            }
        }
        break;
    case 17:
        if (ai_PlayerId != 0xffff) {
            if (ai_CharInts[ai_PlayerId][0x82] != G17_I(msg, 1) || G17_I(msg, 1) == -1)
                cm_NoticeCount++;
        }
        break;
    case 18:
        k = G17_I(*m, 1);
        if (k >= 4)
            k = 4;
        cm_SpeedLevel = k;
        ai_Unk60bd80 = ai_Unk5edc10[k];
        sv_SetStatusText(ai_Unk14413c8[k]);
        ai_ApplyOptions();
        break;
    case 19:
        cm_LeftPlayerId = G17_I(msg, 1);
        break;
    case 20:
        hmt_SetPendingElection(G17_I(msg, 1));
        break;
    }
    sprintf(buf, "cm_ExSysMessage() type: %i", msg.data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->state = ret;
        s->value = 0;
    }
    return 0;
}
/* 0x4c8de0, handler of type 0x21: a character (data[0]) is removed / re-set */

int ai_Unk4c8de0(G17MSG *m, G17STAT *s)
{
    int ok = 0;
    char *c;

    if (s != 0)
        s->state = 2;
    c = gm_FindChar(G17_I(*m, 0));
    if (c != 0) {
        ok = 1;
        sv_MoveToGroup(0, 0, *(unsigned short *)c, 0);
        if (*(char **)(c + 0x184) != 0 && *(int *)(*(char **)(c + 0x184) + 0x28) != -1)
            evt_Stop(evt_FindById(*(int *)(*(char **)(c + 0x184) + 0x28)));
        gm_KillSpieler(*(unsigned short *)c, G17_I(*m, 4));
    }
    if (s != 0 && ok)
        s->state = 1;
    return 0;
}
/* 0x4c8e70, handler of type 0x22, "cm_ExAddStraftat": a crime record (0x2d bytes) in ai_Crimes, the matching
   jobs are ended */

int ai_Unk4c8e70(G17MSG *m, G17STAT *s)
{
    char buf[0x1f0];
    char *c, *rec, *o, *j, *c2;
    int slot, v;
    int a, b;
    int pos[3];

    if (ai_Unk1460ae0 != -1)
        ai_NextCrimeId = G17_I(*m, 0);
    else
        ai_NextCrimeId = ai_NextCrimeId + 1;
    slot = gv_FreeCrimeSlot();
    if (slot == -1)
        return -1;
    c = gm_FindChar(G17_I(*m, 0x16));
    if (c != 0 && c[2] == 6)
        ai_Unk4b44e0(c, 0x17);
    rec = ai_Crimes + slot * 0x2d;
    memcpy(rec, &m->data[0], 0x2d);
    *(int *)rec = ai_NextCrimeId;
    sprintf(buf, "cm_ExAddStraftat(): Executed---- st_id = %i, taeter = %i, opfer = %i", ai_NextCrimeId,
            G17_I(*m, 0x16), G17_I(*m, 0x12));
    v = G17_I(*m, 0x21);
    if (v == -1) {
        c = gm_FindChar(G17_I(*m, 0x12));
        if (c != 0) {
            if (*(char **)(c + 0x170) != 0)
                v = *(int *)(*(char **)(c + 0x170) + 1);
            else if (*(char **)(c + 0x16c) != 0)
                v = *(int *)(*(char **)(c + 0x16c) + 1);
            else
                util_Seed(G17_I(*m, 0x29));
        } else
            util_Seed(G17_I(*m, 0x29));
    }
    ord_AddGridAmount(v, m->data[0x1c], &b, &a);
    gm_FindById(&o, 0, 0, v);
    if (o == 0)
        unk_Unk4f86a0(b, a, pos);
    else if (*(char **)(o + 0x61) != 0) {
        pos[0] = *(int *)(*(char **)(o + 0x61) + 0x4c);
        pos[1] = *(int *)(*(char **)(o + 0x61) + 0x50);
        pos[2] = *(int *)(*(char **)(o + 0x61) + 0x54);
    }
    for (j = he_FindEvent(1, 0, 0x18); j != 0; j = he_NextEvent()) {
        if (*(int *)(j + 0xc4) == *(int *)(rec + 0x16)) {
            if (gv_AddEvidence(*(int *)rec, ai_CharInts[*(unsigned short *)(j + 8)][0]) == 0)
                break;
            if (s != 0) {
                c2 = gm_FindChar(*(int *)(j + 0xc4));
                if (c2 != 0)
                    ai_Unk5a9170(c2, ai_Chars[*(unsigned short *)(j + 8)]);
            }
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 6;
        s->value = (int)rec;
    }
    return 0;
}
/* 0x4c9090, handler of type 0x23 */

int ai_Unk4c9090(G17MSG *m, G17STAT *s)
{
    switch (gv_RetireCrime(G17_I(*m, 0), G17_I(*m, 4))) {
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
/* 0x4c90d0, handler of type 0x24 */

int ai_Unk4c90d0(G17MSG *m, G17STAT *s)
{
    int i;

    i = gv_EvidenceSlot(G17_I(*m, 4), G17_I(*m, 0));
    if (i > -1) {
        ai_Evidence[i][0] = G17_I(*m, 4);
        ai_Evidence[i][1] = G17_I(*m, 0);
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9110, handler of type 0x25: clears the entry (data[4], data[0]) of ai_Evidence */

int ai_Unk4c9110(G17MSG *m, G17STAT *s)
{
    int i;

    for (i = 0; i < 0x800; i++) {
        if (ai_Evidence[i][0] == G17_I(*m, 4) && ai_Evidence[i][1] == G17_I(*m, 0)) {
            ai_Evidence[i][0] = -1;
            ai_Evidence[i][1] = -1;
            if (s != 0)
                s->state = 1;
            return 0;
        }
    }
    return 1;
}
/* 0x4c9170, handler of type 0x26, "cm_ExSendCutInfo": the 0x35c bytes of the joined message go to the cutscene
   slot of player data[0] */

int ai_Unk4c9170(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    char *rec;
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == G17_I(*m, 0))
            break;
    }
    if (i < 16) {
        if (ai_Unk6f0d90 != 0 && *ai_Unk6f0d90 == G17_I(*m, 4)) {
            rec = ai_Unk92cae0[i];
            memcpy(rec, ai_Unka68d40, 0x35c);
            *(int *)(rec + 0xc) = 0;
            sprintf(buf, "cm_ExSendCutInfo: PlayerId:%i Valid:%i  Ready:%i  Time:%i", *(int *)(rec + 0x10),
                    *(int *)(rec + 8), *(int *)(rec + 4), d3_Time);
        }
    }
    if (i >= 16) {
        if (ai_Unk6f0d90 != 0 && *ai_Unk6f0d90 == G17_I(*m, 4))
            sprintf(buf, "cm_ExSendCutInfo(): io-buffer has not been written, because player not found (player id %i)",
                    G17_I(*m, 0));
        return 1;
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = (int)ai_Unk92cae0[i];
    }
    return 0;
}
/* 0x4c9280, handler of type 0x27: a cutscene object from the joined message (0x114 bytes) */

int ai_Unk4c9280(G17MSG *m, G17STAT *s)
{
    char buf[0x114];
    char *o;
    int ret = 0;

    memcpy(buf, ai_Unka68d40, 0x114);
    if (ai_Unk1460ae0 != -1)
        ai_NextPartyId = G17_I(*m, 0);
    o = ai_Unk4e5150(buf);
    if (o != 0) {
        if (ai_GameFlags & 4)
            o[0x26] |= 0x80;
        *(int *)(o + 0x78) = G17_I(*m, 4);
        if (s != 0) {
            s->state = 1;
            s->unk01 = 9;
            s->value = (int)o;
        }
    } else
        ret = 1;
    if (o[8] == 5) {
        if (ai_CharInts[ai_PlayerId][0] != *(int *)(o + 0x34) && !(ai_GameFlags & 4))
            o[0x26] &= 0x7f;
    }
    ai_NextPartyId++;
    return ret;
}
/* 0x4c9340, handler of type 0x28: allocates a cutscene ("cm_ExAllocCutscene") */

int ai_Unk4c9340(G17MSG *m, G17STAT *s)
{
    char buf[0x114];
    char *o;

    memcpy(buf, ai_Unka68d40, 0x114);
    o = ai_Unk4e5150(buf);
    if (o == 0) {
        err_Message("cm_ExAllocCutscene(): Cutscene-Queue is full!");
        return 1;
    }
    if (ai_GameFlags & 4)
        o[0x26] |= 0x80;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 9;
        s->value = (int)o;
    }
    return 0;
}
/* 0x4c93b0, handler of type 0x29 */

int ai_Unk4c93b0(G17MSG *m, G17STAT *s)
{
    if (cm_PartyById(G17_I(*m, 0)) == 0)
        return 1;
    if (ai_Unk4e51b0(G17_I(*m, 0), G17_I(*m, 4)) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9400, handler of type 0x2a */

int ai_Unk4c9400(G17MSG *m, G17STAT *s)
{
    if (cm_PartyById(G17_I(*m, 0)) == 0)
        return 1;
    if (ai_Unk4e5210(G17_I(*m, 0), G17_I(*m, 4)) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9450, handler of type 0x2b: finds the object data[0] as one of three views (gm_FindById) and calls
   gm_RemoveOwned on its list with data[8] */

int ai_Unk4c9450(G17MSG *m, G17STAT *s)
{
    char *a, *b, *c;
    int r;

    if (s != 0)
        s->state = 2;
    gm_FindById(&a, &b, &c, G17_I(*m, 0));
    if (a != 0) {
        if (G17_I(*m, 4) == -1)
            r = gm_RemoveOwned(a + 0x5d, G17_I(*m, 8));
        else {
            b = gm_Query(*(int *)(a + 0x5d), 2, 6, 1, G17_I(*m, 4));
            if (b == 0)
                return -2;
            r = gm_RemoveOwned(b + 0x14, G17_I(*m, 8));
        }
    } else if (c != 0)
        r = gm_RemoveOwned(c + 0x178, G17_I(*m, 8));
    else if (b != 0)
        r = gm_RemoveOwned(b + 0x14, G17_I(*m, 8));
    else
        return -1;
    if (r != 0)
        return r;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4c9520, handler of type 0x2c: a place (data[8], data[9], word data[0xa], flag data[0xc], id data[0xd]) of the
   building data[4] is taken or freed */

int ai_Unk4c9520(G17MSG *m, G17STAT *s)
{
    char r = 2;
    int flag = 0;
    int save = d3_CurrentScene;
    char *o = 0;
    char *b, *x;

    if (ai_Unk1460ae0 != -1)
        veg_NextId = G17_I(*m, 0);
    b = gm_BuildQuery(1, 1, G17_I(*m, 4));
    if (b != 0) {
        x = veg_FindType(G17_S(*m, 0xa));
        if (x != 0 && m->data[0xc] == 0) {
            if (veg_Fits(*(int *)(b + 0x71), m->data[8], m->data[9], (unsigned char)x[0x42]) != 0)
                flag = 1;
        } else
            flag = 1;
        if (m->data[0xc] != 0) {
            o = veg_FindPlantAt(*(int *)(b + 0x71), m->data[8], m->data[9]);
            if (o != 0 && *(int *)(o + 0x10) != -1)
                return 1;
        } else if (G17_I(*m, 0xd) != -1) {
            o = veg_FindPlantAt(*(int *)(b + 0x71), m->data[8], m->data[9]);
            if (o != 0 && *(int *)(o + 0x10) == G17_I(*m, 0xd)) {
                d3if_SelectScene((char)init_BuildingUniverse[*(unsigned char *)(ai_BTypes + (unsigned char)b[0] * 0x24d)], 1);
                d3if_Delete((struct D3OBJECT *)*(int *)(o + 0x14));
                *(int *)(o + 0x14) = 0;
                veg_LoadModel(o, 0);
                *(int *)(o + 0x10) = -1;
                d3if_SelectScene(save, 1);
            }
            if (s != 0) {
                s->value = (int)o;
                s->state = 1;
                s->unk01 = 5;
            }
            return 0;
        }
        if (flag != 0) {
            d3if_SelectScene((char)init_BuildingUniverse[*(unsigned char *)(ai_BTypes + (unsigned char)b[0] * 0x24d)], 1);
            o = veg_SetPlant(*(int *)(b + 0x71), m->data[8], m->data[9], G17_S(*m, 0xa), m->data[0xc]);
            d3if_SelectScene(save, 1);
        }
        if (o != 0) {
            r = 1;
            *(int *)(o + 4) = *(int *)(b + 1);
            if (m->data[0xc] != 0)
                *(int *)(o + 0x10) = G17_I(*m, 0xd);
        }
    }
    if (s != 0) {
        s->value = (int)o;
        s->state = r;
        s->unk01 = 5;
    }
    return 0;
}
/* 0x4c9740, handler of type 0x2d: creates an object of kind 0x1b5 (ids -2 / -3 / -4 resolved) */

int ai_Unk4c9740(G17MSG *m, G17STAT *s)
{
    char *o;
    int k;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0);
    k = ai_Unk4c4450((int)m, G17_I(*m, 4));
    G17_I(*m, 4) = k;
    o = gm_CreateObject(k, 0x1b5, 1);
    if (o == 0)
        return 1;
    cm_LastObjectId = *(int *)(o + 2);
    o[0x12] = 0x40;
    *(G17B7 *)(o + 0x1c) = *(G17B7 *)&m->data[8];
    o[0x22] = 0;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 3;
        s->value = (int)o;
    }
    return 0;
}
/* 0x4c97c0, handler of type 0x2e */

int ai_Unk4c97c0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p != 0) {
        ch_AddAction(p, 0x2d, G17_I(*m, 4), G17_I(*m, 8), G17_I(*m, 0xc));
        if (s != 0) {
            s->state = 2;
            s->unk01 = 0;
            s->value = 0;
        }
    }
    return 0;
}
/* 0x4c9830, handler of type 0x2f: the person "sp_<data[0]>" gets a place (building data[4] / room data[8]) and sits
   down, or gets the action 0x39 */

int ai_Unk4c9830(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p, *o, *r;
    int i = 0;
    char *b = 0;
    int save;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    if (G17_I(*m, 8) != -1)
        i = gm_FindObjectInst(G17_I(*m, 8));
    else
        b = gm_FindBuildingById(G17_I(*m, 4));
    save = d3_CurrentScene;
    if (G17_I(*m, 8) != -1) {
        i = init_ObjectSceneUniverse(i);
        if (i == -1)
            return 2;
    } else {
        if (G17_I(*m, 4) != -1) {
            i = init_BuildingSceneUniverse(b);
            if (i == -1)
                return 2;
        } else
            i = 0;
    }
    if (init_SceneDirty[i * 0x3e0] != 0)
        return 3;
    if (m->data[0xc] != 0) {
        d3if_SelectScene(i, 1);
        o = d3_FindObject(0, 0x160, 0, &m->data[0xc]);
        d3if_SelectScene(save, 1);
        if (o == 0)
            return 4;
        ch_StandUp(p, "sitzend/aufstehen");
        r = ch_WalkToObject(p, o);
        if (r != 0)
            memcpy(r + 0x70, ai_Unk60c4a0, 20);
    } else {
        ch_StandUp(p, "sitzend/aufstehen");
        ch_AddAction(p, 0x39, i);
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c99b0, handler of type 0x30: the person "sp_<data[0]>" is sent to a building / room (action 0x33) */

int ai_Unk4c99b0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p, *b = 0, *q = 0, *r, *c, *o;
    int x = -1;
    int k, v, save;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    if (G17_I(*m, 4) != -1) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 4));
        if (b == 0)
            return 2;
    }
    if (G17_I(*m, 8) != -1) {
        q = gm_Query(*(int *)(b + 0x5d), 1, 1, G17_I(*m, 8));
        if (q == 0)
            return 3;
        if (*(unsigned short *)q != 0xfd) {
            k = init_ObjectSceneUniverse(q);
            goto chk;
        }
    }
    if (b != 0 && gm_IsHomeType(b) != 0) {
        k = init_BuildingSceneUniverse(b);
        q = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
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
        v = *(int *)(q + 2);
    if (b != 0)
        x = *(int *)(b + 1);
    if (k != 0) {
        c = gm_FindChar(G17_I(*m, 0));
        if (c != 0)
            gm_SelectModelVariant(c);
    }
    ch_AddAction(p, 0x38, 0);
    if (k != 0) {
        r = ch_AddAction(p, 0x33, k, x, v);
        if (r != 0)
            strcpy(r + 0xf0, (char *)&m->data[0xc]);
    } else if (*(char **)(b + 0x61) != 0) {
        r = ch_AddAction(p, 0x33, 0, -1, -1);
        if (r != 0) {
            save = d3_CurrentScene;
            d3if_SelectScene(0, 1);
            o = d3_FindObject(*(int *)(b + 0x61), 0x100, 0, &m->data[0xc]);
            if (o != 0) {
                d3_PointToWorld(o, o + 0x4c, r + 0x180);
                r[0x17c] = 1;
            } else
                strcpy(r + 0xf0, (char *)&m->data[0xc]);
            d3if_SelectScene(save, 1);
        }
    }
    ch_AddAction(p, 0x38, 1);
end:
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9c10, handler of type 0x31 */

int ai_Unk4c9c10(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 2;
    ch_StopActions(p);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9c80, handler of type 0x32 */

int ai_Unk4c9c80(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    ch_SetPos(p, &m->data[4], 1);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9d00, handler of type 0x33 */

int ai_Unk4c9d00(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    ch_AddAction(p, 0x38, G17_I(*m, 4));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4c9d80, handler of type 0x34 */

int ai_Unk4c9d80(G17MSG *m, G17STAT *s)
{
    char *o;

    o = gm_PlaceOnStage(G17_I(*m, 0), G17_I(*m, 4), G17_I(*m, 8), 0, 0);
    if (o == 0)
        return 1;
    if (s != 0) {
        s->state = 2;
        s->unk01 = 7;
        s->value = (int)o;
    }
    return 0;
}

#define G17_NAME(r) strcpy((char *)(r) + 0x70, (char *)&m->data[0x2d])

/* 0x4c9dc0, handler of type 0x35, the "cm_ExChrGotoBuilding" texts: the person "sp_<data[0]>" is sent from its
   building (p + 0x2c) to the building data[4] (room data[0x18], id data[0x1c] / data[0x1d]): the actions 0x2d (walk
   to a position), 0x33 (gate, between 0x38 actions), 0x39 (sit down); the 0x15-byte names are copied to the action
   records. Rough draft: the control flow is from the disassembly, not checked. */
int ai_Unk4c9dc0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char gate[0x80];
    float v[3];
    int A, B, W;
    char *p, *from, *to, *cb, *q, *q2, *o, *r;
    int save, scene, k, i;

    cb = 0;
    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    from = gm_BuildQuery(1, 0, 0x44);
    if (G17_I(*m, 4) != -1) {
        to = gm_BuildQuery(1, 1, G17_I(*m, 4));
        if (to == 0)
            return 1;
    } else
        to = from;
    if (*(int *)(p + 0x2c) != -1)
        cb = gm_BuildQuery(1, 1, *(int *)(p + 0x2c));
    save = d3_CurrentScene;
    if (cb != 0 && gm_IsHomeType(cb) != 0) {
        scene = d3t_SceneIndex(*(int *)(p + 0x88));
        d3if_SelectScene(scene, 1);
        if (scene < 0)
            return 0;
        if (*(int *)(p + 0x30) != -1) {
            q = gm_Query(*(int *)(cb + 0x5d), 1, 1, *(int *)(p + 0x30));
            if (q != 0 && *(unsigned short *)q != 0xfd) {
                sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)q * 0x41 + 1));
                q2 = gm_Query(*(int *)(cb + 0x5d), 1, 0, 0xfd);
                k = init_BuildingSceneUniverse(cb);
                if (k >= 0 && q2 != 0) {
                    ch_StandUp(p, "sitzend/aufstehen");
                    r = ch_UseGate(p, gate, "dummy_gate_ob_AUSSEN_RAUM", k, *(int *)(cb + 1), *(int *)(q2 + 2));
                    if (r != 0)
                        G17_NAME(r);
                }
            }
        }
        if (*(int *)(to + 1) != *(int *)(cb + 1)) {
            k = init_BuildingSceneUniverse(cb);
            if (k < 0)
                return 0;
            d3if_SelectScene(k, 1);
            o = d3_FindObject(0, 0x100, 0, "dummy_TRANSPORTER_ENTRY");
            if (o != 0) {
                d3_PointToWorld(o, o + 0x4c, v);
                if (d3_GetHeightAt(*(int *)(ai_Unkfd7c50 + k * 0x3e0), v, &B, &A, &W) != 0) {
                    r = ch_AddAction(p, 0x2d, B, A, k);
                    if (r != 0)
                        G17_NAME(r);
                }
            }
            ch_AddAction(p, 0x38, 0);
            r = ch_AddAction(p, 0x33, 0, -1, -1);
            ch_AddAction(p, 0x38, 1);
            if (r == 0) {
                d3if_SelectScene(save, 1);
                return 1;
            }
            G17_NAME(r);
            if (*(char **)(cb + 0x61) != 0) {
                d3if_SelectScene(0, 1);
                o = d3_FindObject(*(int *)(cb + 0x61), 0x100, 0, "dummy_TUER");
                if (o == 0)
                    o = d3_FindObjectNoCase(*(int *)(cb + 0x61), 0x100, 0, "dummy_TUER");
                if (o != 0)
                    d3_PointToWorld(o, o + 0x4c, v);
                else
                    d3_PointToWorld(*(char **)(cb + 0x61), *(char **)(cb + 0x61) + 0x4c, v);
                *(float *)(r + 0x180) = v[0];
                *(float *)(r + 0x184) = v[1];
                r[0x17c] = 1;
                *(float *)(r + 0x188) = v[2];
            }
        }
    } else if (*(int *)(p + 0x2c) != *(int *)(to + 1) && *(int *)(p + 0x2c) != -1) {
        scene = d3t_SceneIndex(*(int *)(p + 0x88));
        if (scene < 0)
            return 0;
        d3if_SelectScene(scene, 1);
        ch_StandUp(p, "sitzend/aufstehen");
        o = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, v);
            if (d3_GetHeightAt(*(int *)(ai_Unkfd7c50 + scene * 0x3e0), v, &B, &A, &W) != 0) {
                r = ch_AddAction(p, 0x2d, B, A, scene);
                if (r != 0)
                    G17_NAME(r);
            }
        }
        ch_AddAction(p, 0x38, 0);
        r = ch_AddAction(p, 0x33, 0, -1, -1);
        ch_AddAction(p, 0x38, 1);
        if (r != 0) {
            G17_NAME(r);
            memcpy(r + 0xf0, "dummy_TUER", 11);
            if (cb != 0) {
                o = d3_FindObject(*(int *)(cb + 0x61), 0x100, 0, "dummy_TUER");
                if (o != 0) {
                    d3_PointToWorld(o, o + 0x4c, v);
                } else if (*(char **)(cb + 0x61) != 0) {
                    d3_PointToWorld(*(char **)(cb + 0x61), *(char **)(cb + 0x61) + 0x4c, v);
                } else
                    err_Message("cm_ExChrGotoBuilding(): start->D3Object is NULL!");
                r[0x17c] = 1;
                *(float *)(r + 0x180) = v[0];
                *(float *)(r + 0x184) = v[1];
                *(float *)(r + 0x188) = v[2];
            }
        }
    }
    if (gm_IsHomeType(to) == 0) {
        d3if_SelectScene(0, 1);
        o = 0;
        if (m->data[0xd] != 0)
            o = d3_FindObject(0, 0x100, 0, &m->data[0xd]);
        else if (*(char **)(to + 0x61) != 0) {
            if (*(int *)(p + 0x124) != 0)
                o = (char *)he_Unk555210(*(int *)(to + 0x61));
            else {
                o = d3_FindObject(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
                if (o == 0)
                    o = d3_FindObjectNoCase(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
            }
            if (o == 0)
                o = *(char **)(to + 0x61);
        }
        if (o != 0) {
            d3_PointToWorld(o, o + 0x4c, v);
            if (d3_GetHeightAt(*(int *)ai_Unkfd7c50, v, &B, &A, &W) != 0) {
                r = ch_AddAction(p, 0x2d, B, A, 0);
                if (r != 0)
                    G17_NAME(r);
            }
        }
        if (m->data[0xc] == 0) {
            if (G17_I(*m, 0x18) != -1) {
                i = init_ObjectSceneUniverse(gm_Query(*(int *)(to + 0x5d), 1, 1, G17_I(*m, 0x18)));
                if (i < 0) {
                    err_Message("cm_ExChrGotoBuilding(): Could not find universe...");
                    goto sit;
                }
            } else
                i = 0;
            if (G17_I(*m, 0x18) == -1)
                err_Message("cm_ExChrGotoBuilding(): invalid room-id (cm_ID_NONE)");
            else if (gm_Query(*(int *)(to + 0x5d), 1, 1, G17_I(*m, 0x18)) == 0)
                err_Message("cm_ExChrGotoBuilding(): invalid room-id (raum==NULL)");
            ch_AddAction(p, 0x38, 0);
            r = ch_AddAction(p, 0x33, i, *(int *)(to + 1), G17_I(*m, 0x18));
            if (r != 0)
                G17_NAME(r);
            ch_AddAction(p, 0x38, 1);
sit:
            r = ch_AddAction(p, 0x39, i);
            if (r != 0)
                G17_NAME(r);
        }
    } else {
        if (*(int *)(to + 1) != *(int *)(p + 0x2c)) {
            d3if_SelectScene(0, 1);
            o = d3_FindObject(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
            if (o == 0)
                o = d3_FindObjectNoCase(*(int *)(to + 0x61), 0x100, 0, "dummy_TUER");
            if (o != 0) {
                d3_PointToWorld(o, o + 0x4c, v);
                if (d3_GetHeightAt(*(int *)ai_Unkfd7c50, v, &B, &A, &W) != 0) {
                    r = ch_AddAction(p, 0x2d, B, A, 0);
                    if (r != 0)
                        G17_NAME(r);
                }
            }
        }
        if (m->data[0xc] == 0) {
            q = gm_Query(*(int *)(to + 0x5d), 1, 1, G17_I(*m, 0x18));
            if (q == 0) {
                q = gm_Query(*(int *)(to + 0x5d), 1, 0, 0xfd);
                if (q == 0) {
                    i = init_BuildingSceneUniverse(to);
                    if (i >= 0) {
                        ch_AddAction(p, 0x38, 0);
                        r = ch_AddAction(p, 0x33, i, *(int *)(to + 1), -1);
                        if (r != 0) {
                            G17_NAME(r);
                            memcpy(r + 0xf0, "dummy_TRANSPORTER", 18);
                        }
                        ch_AddAction(p, 0x38, 1);
                    }
                    goto done;
                }
            }
            if (*(unsigned short *)q == 0xfd) {
                if (*(int *)(to + 1) != *(int *)(p + 0x2c)) {
                    k = init_BuildingSceneUniverse(to);
                    if (k < 0)
                        return 0;
                    ch_AddAction(p, 0x38, 0);
                    r = ch_AddAction(p, 0x33, k, *(int *)(to + 1), *(int *)(q + 2));
                    if (r != 0) {
                        G17_NAME(r);
                        memcpy(r + 0xf0, "dummy_TRANSPORTER_ENTRY", 24);
                    }
                    ch_AddAction(p, 0x38, 1);
                    d3if_SelectScene(k, 1);
                    o = d3_FindObject(0, 0x100, 0, *(int *)(p + 0x124) != 0 ? "dummy_TRANSPORTER" : "dummy_EINGANG");
                    if (o != 0) {
                        d3_PointToWorld(o, o + 0x4c, v);
                        if (d3_GetHeightAt(*(int *)(ai_Unkfd7c50 + k * 0x3e0), v, &B, &A, &W) != 0) {
                            r = ch_AddAction(p, 0x2d, B, A, k);
                            if (r != 0)
                                G17_NAME(r);
                        }
                    }
                }
            } else {
                i = init_ObjectSceneUniverse(q);
                sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)q * 0x41 + 1));
                r = ch_UseGate(p, "dummy_EINGANG", gate, i, *(int *)(to + 1), *(int *)(q + 2));
                if (r != 0)
                    G17_NAME(r);
            }
        }
    }
done:
    d3if_SelectScene(save, 1);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 7;
        s->value = (int)p;
    }
    return 0;
}
/* 0x4ca8a0, handler of type 0x36: the person "sp_<data[0]>" uses the gates of building data[4] (the room data[8]) */

int ai_Unk4ca8a0(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char gate[0x80];
    char gate2[0x80];
    char *p, *b, *q, *r;
    int k;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 1;
    b = gm_BuildQuery(1, 1, G17_I(*m, 4));
    q = gm_Query(*(int *)(b + 0x5d), 1, 1, G17_I(*m, 8));
    if (*(int *)(p + 0x30) != -1)
        r = gm_Query(*(int *)(b + 0x5d), 1, 1, *(int *)(p + 0x30));
    else
        r = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
    if (*(unsigned short *)q == 0xfd) {
        k = init_BuildingSceneUniverse(b);
        sprintf(gate2, "dummy_gate_ob_AUSSEN_RAUM");
    } else {
        k = init_ObjectSceneUniverse(q);
        sprintf(gate2, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)q * 0x41 + 1));
    }
    sprintf(gate, "dummy_gate_ob_%s", _strupr(ai_Goods + *(unsigned short *)r * 0x41 + 1));
    if (k >= 0)
        ch_UseGate(p, gate, gate2, k, *(int *)(b + 1), *(int *)(q + 2));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4caa20, handler of type 0x37 */

int ai_Unk4caa20(G17MSG *m, G17STAT *s)
{
    char buf[0x80];
    char *p;

    sprintf(buf, "sp_%i", G17_I(*m, 0));
    p = ch_FindByName(buf);
    if (p == 0)
        return 2;
    ch_QueueAnim(p, &m->data[4], m->data[0x34]);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4caaa0, handler of type 0x38 */

int ai_Unk4caaa0(G17MSG *m, G17STAT *s)
{
    char *b, *c1, *c2;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    c1 = gm_FindChar(G17_I(*m, 8));
    if (c1 == 0 && G17_I(*m, 8) != -1)
        return 1;
    c2 = gm_FindChar(G17_I(*m, 4));
    if (c2 == 0 && G17_I(*m, 4) != -1)
        return 2;
    gm_AssignBuildingK(b, *(unsigned short *)c2, *(unsigned short *)c1);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cab40, handler of type 0x39: building data[0] gets the character data[4] as its boss (+0x170) */

int ai_Unk4cab40(G17MSG *m, G17STAT *s)
{
    unsigned short n;
    char *b, *c, *o;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    c = gm_FindChar(G17_I(*m, 4));
    if (c == 0 && G17_I(*m, 4) != -1)
        return 1;
    if (ai_BTypes[(unsigned char)b[0] * 0x24d] == 1) {
        ord_GetKindRecord((unsigned char)b[0], &n);
        if ((unsigned char)b[0x65] >= n)
            return 1;
        b[0x65]++;
    }
    o = *(char **)(c + 0x170);
    if (o != 0) {
        if (ai_BTypes[(unsigned char)o[0] * 0x24d] == 1 && (unsigned char)o[0x65] > 0)
            o[0x65]--;
    }
    *(char **)(c + 0x170) = b;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cac40, handler of type 0x3a, "cm_ExGebUpgrade": building data[0] goes one level up (byte 0 of the object):
   the next model "<geb dir>/<name>[_<level>].ogr" is loaded and its object group replaces the old one.
   Rough draft: the float parts and the effect call are from the disassembly, not checked. */

int ai_Unk4cac40(G17MSG *m, G17STAT *s)
{
    char nm[0x80];
    char path[0x100];
    char msg[0x180];
    char buf2[0x100];
    float v[3], u[3], w1, w2, w3, y;
    char *b, *d, *o, *sp;
    int i, save, ok;
    int e, ebx;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    d = ai_BTypes + (unsigned char)b[0] * 0x24d;
    if ((unsigned char)d[0x247] >= (unsigned char)d[0x248]) {
        err_Message("cm_ExSetGebUpgrade(): already highest level");
        return 1;
    }
    b[0]++;
    b[0x5c] = b[0x5c] + (100 - b[0x5c]) / 2;
    gm_SetCapacities(b);
    gm_StockBuildingObjects(b);
    if (d[0] == 0x1d)
        gv_GrowGraveyard(b);
    strcpy(nm, ai_BTypes + (unsigned char)b[0] * 0x24d + 1);
    _strupr(nm + 2);
    for (i = 0; i < 4; i++) {
        if (d[0] == 0x21) {
            if (ai_Unk770b89[*(unsigned short *)(b + 0x27) * 0x21c] == 1)
                sprintf(path, "%sgebaeude/*%s_A.ogr", ai_DataPath, nm);
            else
                sprintf(path, "%sgebaeude/*%s_B.ogr", ai_DataPath, nm);
        } else if (i != 0)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, nm, (char)(i + 0x40));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, nm);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, buf2) != 0)
            break;
    }
    if (i < 4) {
        save = d3_CurrentScene;
        d3if_SelectScene(0, 1);
        d3_PointToWorld(*(int *)(b + 0x61), *(char **)(b + 0x61) + 0x4c, v);
        o = (char *)d3_LoadGroup(path, 0, 0);
        if (o == 0) {
            sprintf(msg, "cm_ExGebUpgrade(): Could not load 3D-Object-Group '%s'...", path);
            err_Message(msg);
        } else {
            d3_DirToWorld(*(int *)(b + 0x61), ai_AxisPair, u);
            u[0] = 0;
            u[1] = 0;
            u[2] = 0;
            u[1] = d3_VecAngle(ai_AxisPair, u);
            d3_SetObjectPos(o, &w1);
            d3_SetObjectRot(o, &w2);
            ai_Unk58feb0(o, &w3);
            if (d3_QuadTree != 0)
                d3q_RemoveObject((D3OBJECT *)*(int *)(b + 0x61), (D3QNODE *)d3_QuadTree);
            d3if_Delete((struct D3OBJECT *)*(int *)(b + 0x61));
            if (*(int *)(o + 0x1cc) == 0)
                d3_InflateObject(o);
            if (*(int *)(o + 0x1ec) != 0) {
                if (d3t_GetBoundingSphere(o, sv_CameraObj, &y, &e) != 0) {
                    ebx = (int)(y * ai_Unk5edc28);
                    sp = (char *)evt_Load("Effekte/geb_upgrade.esc");
                    if (sp != 0) {
                        evt_RunScriptArgs((EVTSCRIPT *)sp, 6, (int)w3, (int)w2, (int)y, ebx, o);
                        *(int *)(sp + 0x104) = 0;
                    }
                }
            }
            init_HookObject(o, b, 2);
            sv_CreateBanners(b);
            if (*(int *)(b + 0x61) != 0) {
                ok = gm_ClockSeason(ai_Time);
                if (ok == 1)
                    init_TextureSet = 1;
                else if (ok == 0)
                    init_TextureSet = 0;
                else if (ok == 2)
                    init_TextureSet = 2;
                else if (ok == 3)
                    init_TextureSet = 3;
                init_TextureSetHook(*(int *)(b + 0x61));
                d3_ForEachObject2(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x2c0, unk_Unk511600);
                y = (float)sv_DayPhase + sv_DayPhaseFrac;
                d3_ForEachObject(ai_Unk5fb5b0, *(int *)(b + 0x61), 0x21e, d3_SetLightKeyCB, (void *)(int)y);
                d3_RelightObjects(*(D3OBJECT **)(b + 0x61));
                if (*(int *)(b + 0x95) != -1) {
                    sp = (char *)evt_FindById(*(int *)(b + 0x95));
                    if (sp != 0)
                        evt_Stop(sp);
                    *(int *)(b + 0x95) = -1;
                    sv_UpdateChimneySmoke(b);
                }
            }
        }
        d3if_SelectScene(save, 1);
    }
    gm_RefreshGoodsLists(ai_OwnPlayer);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb100, handler of type 0x3b: removes building data[0] (its jobs, character references, effect) */

int ai_Unk4cb100(G17MSG *m, G17STAT *s)
{
    int save = d3_CurrentScene;
    char *b, *o, *p;
    int e;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    if (b == sv_SelectedObject)
        sv_ResetSelection();
    for (o = gm_Query(0, 2, 7, 4, 0x1d); o != 0; o = gm_QueryNext()) {
        if (*(int *)(o + 0x1c) == *(int *)(b + 1))
            gm_Unk5034b0(o);
    }
    for (p = ai_Unk770cec; (int)p < (int)(ai_Unk770cec + 0x300 * 0x21c); p += 0x21c) {
        if (*(char **)p == b)
            *(char **)p = 0;
    }
    d3if_SelectScene(0, 1);
    if (d3_QuadTree != 0)
        d3q_RemoveObject((D3OBJECT *)*(int *)(b + 0x61), (D3QNODE *)d3_QuadTree);
    d3if_Delete((struct D3OBJECT *)*(int *)(b + 0x61));
    if (*(int *)(b + 0x95) != -1) {
        o = (char *)evt_FindById(*(int *)(b + 0x95));
        if (o != 0)
            evt_Stop(o);
        *(int *)(b + 0x95) = -1;
    }
    d3if_SelectScene(save, 1);
    gm_FreeBuilding(b);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb210, handler of type 0x3c */

int ai_Unk4cb210(G17MSG *m, G17STAT *s)
{
    char r = 2;

    if (gv_HandOverCrimes(G17_I(*m, 0), G17_I(*m, 4), G17_I(*m, 8), G17_I(*m, 0xc)) > 0)
        r = 1;
    if (s != 0) {
        s->state = r;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb250, handler of type 0x3d: creates a character (kind data[8]) for building data[0] under the boss data[4] */

int ai_Unk4cb250(G17MSG *m, G17STAT *s)
{
    char *b, *c, *bo = 0, *t;
    int *r;
    int k;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0xf);
    else
        G17_I(*m, 0x13) = rand();
    util_Seed(G17_I(*m, 0x13));
    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b == 0)
        return 1;
    c = gm_FindChar(G17_I(*m, 9));
    if (G17_I(*m, 4) != -1) {
        bo = gm_BuildQuery(1, 1, G17_I(*m, 4));
        if (bo == 0)
            return 1;
    } else if (c != 0)
        bo = *(char **)(c + 0x170);
    if (c == 0 || *(int *)(c + 0x16c) != 0)
        c = gm_SpawnCharacter(6, m->data[8], 0x12, m->data[0xe], m->data[0xd], bo);
    if (c == 0) {
        if (s != 0) {
            s->state = 2;
            s->unk01 = 1;
            s->value = 0;
        }
        return 2;
    }
    if (m->data[0xd] != 0)
        c[0x165] = m->data[0xd];
    if (m->data[0xe] != 0)
        c[0x164] = m->data[0xe];
    else
        c[0x164] = 0;
    c[2] = m->data[8];
    if (*(int *)(c + 0x184) == 0) {
        if (bo != 0) {
            t = gm_HomeObject(bo);
            if (t != 0 && *(int *)(t + 2) != -1)
                gm_PlaceOnStage(*(int *)(c + 4), *(int *)(bo + 1), *(int *)(t + 2), 0, 0);
        } else {
            t = gm_BuildingMainObject(b);
            if (t != 0 && *(int *)(t + 2) != -1)
                gm_PlaceOnStage(*(int *)(c + 4), *(int *)(b + 1), *(int *)(t + 2), 0, 0);
        }
    }
    k = m->data[0xd];
    if (k != 2 && k != 1 && k != 0x10 && k != 0xf && k != 0x14 && k != 0x13) {
        r = (int *)ai_Unk4cda50(c);
        if (*r != 0) {
            gm_TakeObjekt(*(int *)(c + 4), *r, 1);
            cm_UpdateHandObject(c);
        }
    } else {
        r = (int *)ai_Unk4cda50(c);
        if (*r == 0) {
            gm_CreateObject(*(int *)(c + 4), 0x154, 1);
            cm_UpdateHandObject(c);
        }
    }
    *(char **)(c + 0x16c) = b;
    if (bo != 0)
        unk_Unk4fda10(c, bo);
    *(int *)(c + 0x174) = 0;
    *(int *)(c + 0x1c8) &= ~0x2000;
    ai_ResetJob(c);
    *(G17B16 *)(c + 0x1b4) = *(G17B16 *)(b + 0x99);
    if (m->data[0xe] != 0)
        *(int *)(c + 0x1b8) = *(int *)(b + 0x39);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)c;
    }
    return 0;
}
/* 0x4cb4a0, handler of type 0x3e: the character data[0] gets a new 3D object group (name data[4], like
   "<kind>_<a>_<b>"); its names are split into +0x130 / +0x170 of the record at +0x184 */

int ai_Unk4cb4a0(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    float v1[3], v2[3];
    char *c, *g, *o, *t, *p, *q;
    int save, e;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0 || *(char **)(c + 0x184) == 0)
        return 1;
    g = *(char **)(c + 0x184);
    save = d3_CurrentScene;
    d3if_SelectScene(d3t_SceneIndex(*(int *)(g + 0x88)), 1);
    v1[0] = *(float *)(*(char **)(g + 0x34) + 0x4c);
    v1[1] = *(float *)(*(char **)(g + 0x34) + 0x50);
    v1[2] = *(float *)(*(char **)(g + 0x34) + 0x54);
    v2[0] = *(float *)(*(char **)(g + 0x34) + 0x84);
    v2[1] = *(float *)(*(char **)(g + 0x34) + 0x88);
    v2[2] = *(float *)(*(char **)(g + 0x34) + 0x8c);
    e = *(int *)(*(char **)(g + 0x34) + 0x200);
    t = d3_CreateObject(0, v1, v2, &m->data[4]);
    if (t == 0) {
        d3if_SelectScene(save, 1);
        return 1;
    }
    strcpy(t, *(char **)(g + 0x34));
    strcpy(g + 8, (char *)&m->data[4]);
    d3if_Delete((struct D3OBJECT *)*(int *)(g + 0x34));
    *(char **)(g + 0x34) = t;
    *(int *)(*(char **)(g + 0x34) + 0x200) = e;
    *(int *)(*(char **)(g + 0x34) + 0x48) = 0x3000000;
    d3_RelightObject(*(D3OBJECT **)(g + 0x34));
    o = *(char **)(g + 0x34);
    o[0x212] = (o[0x212] & 0xf7) | 4;
    *(int *)(*(char **)(g + 0x34) + 0x220) = 2;
    strcpy(buf, (char *)&m->data[4]);
    p = strchr(buf, '_');
    if (p != 0) {
        p++;
        q = strchr(p, '_');
        if (q != 0)
            *q = 0;
        strcpy(g + 0x130, p);
        p[-1] = 0;
        strcpy(g + 0x170, buf);
    } else
        g[0x130] = ch_EmptyName;
    d3if_SelectScene(save, 1);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 1;
        s->value = (int)c;
    }
    return 0;
}
/* 0x4cb6e0, handler of type 0x3f: a 0x80-byte record (id byte data[0], word data[1]) of gm_FindGoods is replaced */

int ai_Unk4cb6e0(G17MSG *m, G17STAT *s)
{
    char *r;

    r = gm_FindGoods(m->data[0], *(unsigned short *)&m->data[1]);
    if (r == 0)
        return 1;
    memcpy(r, &m->data[1], 0x80);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb730, handler of type 0x40: the same record gets deltas added (3 ints, 4 floats) */

int ai_Unk4cb730(G17MSG *m, G17STAT *s)
{
    char *r;

    r = gm_FindGoods(m->data[0], *(unsigned short *)&m->data[1]);
    if (r == 0)
        return 1;
    *(int *)(r + 0x10) += G17_I(*m, 3);
    *(int *)(r + 0x14) += G17_I(*m, 7);
    *(int *)(r + 0x18) += G17_I(*m, 0xb);
    *(float *)(r + 0x20) += *(float *)&m->data[0xf];
    *(float *)(r + 0x24) += *(float *)&m->data[0x13];
    *(float *)(r + 0x2c) += *(float *)&m->data[0x17];
    *(float *)(r + 0x38) += *(float *)&m->data[0x1b];
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb7c0, handler of type 0x41 */

int ai_Unk4cb7c0(G17MSG *m, G17STAT *s)
{
    ord_LoadStatistics(&m->data[0]);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb7f0, handler of type 0x42 */

int ai_Unk4cb7f0(G17MSG *m, G17STAT *s)
{
    char *b;

    b = gm_BuildQuery(1, 1, G17_I(*m, 0));
    if (b != 0 && *(int *)(b + 0x61) != 0 && gm_IsHomeType(b) == 0)
        sv_UpdateChimneySmoke(b);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb850, handler of type 0x43 */

int ai_Unk4cb850(G17MSG *m, G17STAT *s)
{
    char *c;

    c = gm_FindChar(G17_I(*m, 0));
    if (c != 0 && *(char **)(c + 0x170) != 0)
        gm_ShowBuildingObjects(*(char **)(c + 0x170));
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb890, handler of type 0x44 (the result of amt_AcceptApplication is also the return value, inverted) */

int ai_Unk4cb890(G17MSG *m, G17STAT *s)
{
    int r;

    r = amt_AcceptApplication(&m->data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->state = (r == 0) + 1;
        s->value = 0;
    }
    return r == 0;
}
/* 0x4cb8d0, handler of type 0x45 */

int ai_Unk4cb8d0(G17MSG *m, G17STAT *s)
{
    int r;

    r = amt_ExecuteOfficeMessage(&m->data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->state = (r == 0) + 1;
        s->value = 0;
    }
    return r == 0;
}
/* 0x4cb910, handler of type 0x46 */

int ai_Unk4cb910(G17MSG *m, G17STAT *s)
{
    int r;

    r = gv_OnDecisionValue(&m->data[0]);
    if (s != 0) {
        s->unk01 = 0;
        s->value = 0;
        s->state = (r == 0) + 1;
    }
    return 0;
}
/* 0x4cb940, handler of type 0x47, "cm_ExChangePlayerHe" */

int ai_Unk4cb940(G17MSG *m, G17STAT *s)
{
    char *c;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        err_Message("cm_ExChangePlayerHe(): invalid sp->id");
    if (*(unsigned short *)c != 0xffff)
        sv_MoveToGroup(*(int *)(c + 0x16c), G17_I(*m, 4), *(unsigned short *)c, 0);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cb9b0, handler of type 0x48 */

int ai_Unk4cb9b0(G17MSG *m, G17STAT *s)
{
    char *c;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    gm_SetJob(*(unsigned short *)c, m->data[4]);
    gm_ChooseHeadIcon(c);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cba10, handler of type 0x49: creates a character (kind data[4], boss building data[7], place data[0xb]) in the
   slot ai_Chars[] */

int ai_Unk4cba10(G17MSG *m, G17STAT *s)
{
    char *bo = 0, *t;
    int id, k;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0);
    else
        G17_I(*m, 0x12) = timeGetTime();
    if (s != 0) {
        s->state = 2;
        s->unk01 = 1;
        s->value = 0;
    }
    util_Seed(G17_I(*m, 0x12));
    if (G17_I(*m, 7) != -1) {
        k = ai_Unk4c4450((int)m, G17_I(*m, 7));
        G17_I(*m, 7) = k;
        bo = gm_BuildQuery(1, 1, k);
        if (bo == 0)
            return 1;
    }
    id = gm_CreateCharacter(m->data[4], -1, -1, 0x14, 0, m->data[0xf], m->data[0x10], 0);
    if (id == 0xffff)
        return 1;
    if (G17_I(*m, 0xb) != -1 && (t = gm_FindObjectInst(G17_I(*m, 0xb))) != 0)
        gm_PlaceOnStage(ai_CharInts[id][0], *(int *)(bo + 1), *(int *)(t + 2), 0, 0);
    else
        gm_PlaceOnStage(ai_CharInts[id][0], *(int *)(bo + 1), -1, 0, 0);
    *(int *)(ai_Chars[id] + 0x16c) = 0;
    ai_Chars[id][2] = 0xd;
    if (m->data[0x11] != 0) {
        gm_CreateObject(ai_CharInts[id][0], m->data[0x11], 1);
        cm_UpdateHandObject(ai_Chars[id]);
    }
    cm_LastCharId = ai_CharInts[id][0];
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
/* 0x4cbb90, handler of type 0x4a */

int ai_Unk4cbb90(G17MSG *m, G17STAT *s)
{
    char *t;

    t = gm_FindObjectInst(G17_I(*m, 0));
    if (t == 0)
        return 1;
    gm_Unk5034b0(t);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cbbd0, handler of type 0x4b: a chat text (data[0x20]) is appended to cm_ChatLog (reset after 5 lines) when
   it is for us (the player ids data[0..0x1f]) or sent by ourselves (s != 0) */

int ai_Unk4cbbd0(G17MSG *m, G17STAT *s)
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
        if (*(int *)&m->data[i * 4] == ai_CharInts[ai_PlayerId][0]) {
            found = 1;
            break;
        }
    }
    if (s == 0 && !found)
        return 0;
    strcat(cm_ChatLog, (char *)&m->data[0x20]);
    sv_ChatTime = d3_Time;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cbca0, handler of type 0x4c, "cm_ExCreateGeb": a building of kind data[8] (owner = the character data[0]) at the
   position data[9..0x14]; its model "<geb dir>/<name>[_<level>].ogr" is loaded. Rough draft. */

int ai_Unk4cbca0(G17MSG *m, G17STAT *s)
{
    char nm[0x100];
    char path[0x200];
    char path2[0x100];
    char msg[0x100], msg2[0x100], msg3[0x100];
    char tmp[0x100];
    char z[0x54];
    char zz[0x84];
    char *c, *b, *d, *o, *bp;
    int sc, save, i, n;
    int w1[3], w2[3];
    int p1, p2, p3;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    b = gm_CreateBuilding(m->data[8], *(unsigned short *)c);
    if (b == 0)
        return 1;
    cm_LastBuildingId = *(int *)(b + 1);
    sc = ai_Unk5f8b10;
    d = ai_BTypes + (unsigned char)b[0] * 0x24d;
    strcpy(nm, d + 1);
    _strupr(nm + 2);
    for (i = 0; i < 4; i++) {
        if (i != 0)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, nm, (char)(i + 0x40));
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, nm);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, path2) != 0)
            break;
    }
    if (i >= 4)
        goto done;
    save = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    p1 = G17_I(*m, 9);
    p2 = G17_I(*m, 0xd);
    p3 = G17_I(*m, 0x11);
    o = d3_LoadGroup(path, 0, 0);
    if (o == 0) {
        sprintf(msg, "cm_ExCreateGeb(): Could not load 3D-Object-Group '%s'...", path);
    } else {
        memset(z, 0, sizeof(z));
        d3_SetObjectPos(o, w1);
        d3_SetObjectRot(o, &m->data[0x19]);
        ai_Unk58feb0(o, &bp);
        init_HookObject(o, bp, 2);
        ai_BuildingName((unsigned char)bp[0], tmp);
        strcpy(z, tmp);
        sprintf(msg2, "cm_ExCreateGeb(): bauplatze name:%s", tmp);
        if (lc_GetGebaeudeBauplatz(z + 0xc, bp) != 0) {
            memset(zz, 0, sizeof(zz));
            d3_ForEachObject(ai_Unk5fb5b0, sc, 0x2c0, ai_VgFilter, zz);
            n = *(int *)(z + 0x10);
            for (i = 0; i < n; i++) {
                if (d3_QuadTree != 0)
                    d3q_RemoveObject((D3OBJECT *)*(int *)(zz + i * 4), (D3QNODE *)d3_QuadTree);
                sprintf(msg3, "cm_ExCreateGeb(): killing objectptr: %s", *(char **)(zz + i * 4));
                sprintf(msg3, "cm_ExCreateGeb(): killing objectname: %s", *(char **)(zz + 0xc + i * 4));
                d3if_Delete((struct D3OBJECT *)*(int *)(zz + 0x18 + i * 4));
            }
        } else
            err_Message("cm_ExCreateGeb(): could not lc_GetGebaeudeBauplatz()...");
        sv_CreateBanners(b);
        rd_BuildRoad(*(int *)(b + 0x61), (unsigned char)b[0]);
    }
    d3if_SelectScene(save, 1);
done:
    if (s != 0) {
        s->state = 1;
        s->unk01 = 2;
        s->value = (int)b;
    }
    return 0;
}
/* 0x4cbff0, handler of type 0x4d */

int ai_Unk4cbff0(G17MSG *m, G17STAT *s)
{
    char *c, *g;

    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    g = *(char **)(c + 0x184);
    if (g != 0 && *(int *)(g + 0x28) != -1)
        evt_Stop(evt_FindById(*(int *)(g + 0x28)));
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc050, handler of type 0x4e: person data[0] (with s: walks there, else the action 0x3a) */

int ai_Unk4cc050(G17MSG *m, G17STAT *s)
{
    char *p, *q = 0, *r;
    int sc;

    p = ch_FindByName((char *)&m->data[0]);
    if (p == 0)
        return 1;
    if (m->data[0] != 0)
        q = ch_FindByName((char *)&m->data[0x20]);
    sc = d3t_SceneIndex(*(int *)(p + 0x88));
    if (s != 0) {
        r = ch_AddAction(p, 0x2d, G17_I(*m, 0x18), G17_I(*m, 0x1c), sc);
        *(int *)(r + 0x160) |= 2;
        if (q != 0) {
            strcpy(r + 0x114, (char *)&m->data[0x20]);
            *(int *)(r + 0x160) |= 8;
        }
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
        return 0;
    }
    ch_AddAction(p, 0x3a, G17_I(*m, 0x18), G17_I(*m, 0x1c), sc);
    return 0;
}
/* 0x4cc110, handler of type 0x4f: two bytes of the table +0xf4 of the action record (type 0x3a) of person data[0] */

int ai_Unk4cc110(G17MSG *m, G17STAT *s)
{
    char *p, *a, *q;
    int idx, i;

    p = ch_FindByName((char *)&m->data[0]);
    if (p == 0)
        return 1;
    a = *(char **)(p + 0x128);
    if (a == 0 || a[9] != 0x3a)
        return 2;
    if (*(int *)(a + 0xf4) != 0) {
        *(int *)(a + 0xf0) = G17_I(*m, 0x1c);
        idx = G17_I(*m, 0x18);
        q = (char *)&m->data[0x21];
        for (i = 0; i < 2 && idx < *(int *)(a + 0xf0); i++) {
            (*(char **)(a + 0xf4))[idx * 2] = q[-1];
            (*(char **)(a + 0xf4))[idx * 2 + 1] = q[0];
            q += 2;
            idx++;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc1b0, handler of type 0x50: the cutscene record data[0] (when it is our player's) gets the entry data[4] */

int ai_Unk4cc1b0(G17MSG *m, G17STAT *s)
{
    char *rec, *c, *r;
    int i;

    if (m->data[0x30] != ai_CharInts[ai_PlayerId][0x82])
        return 1;
    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == G17_I(*m, 0))
            break;
    }
    if (i < 16) {
        rec = ai_Unk92cae0[i];
        c = ai_Unk4ce130(G17_I(*m, 4));
        if (c == 0)
            c = gm_FindChar(G17_I(*m, 4));
        if (c != 0) {
            r = ai_Unk4cdf00(rec, c);
            memcpy(r, &m->data[4], 0x2c);
            if (*(int *)(r + 4) == 0)
                *(int *)r = -1;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc270, handler of type 0x51 */

int ai_Unk4cc270(G17MSG *m, G17STAT *s)
{
    char *r;
    int i;

    if (m->data[0x30] != ai_CharInts[ai_PlayerId][0x82])
        return 1;
    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == G17_I(*m, 0))
            break;
    }
    if (i < 16) {
        r = ai_Unk4cdf00(ai_Unk92cae0[i], ai_Unk4ce130(G17_I(*m, 4)));
        if (*(int *)(r + 4) == G17_I(*m, 8)) {
            if (*(int *)(r + 4) != 2 || *(int *)(r + 0x10) == G17_I(*m, 0x14))
                memcpy(r, &m->data[4], 0x2c);
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc330, handler of type 0x52 */

int ai_Unk4cc330(G17MSG *m, G17STAT *s)
{
    char *r;

    r = ai_Unk4ce130(G17_I(*m, 0));
    if (r != 0) {
        ai_Unk4cedc0(r, G17_I(*m, 4), 0);
        *(int *)(r + 0x24) -= G17_I(*m, 4);
        r[8] = m->data[8];
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc390, handler of type 0x53: the sub-commands 'recl' (reset the slots of the job 0x12c of building data[8]) and
   'buy ' (character data[4] takes one of its three slots) */

int ai_Unk4cc390(G17MSG *m, G17STAT *s)
{
    char *b, *c, *q;
    int i, k;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (G17_I(*m, 0) == 0x636c6572) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12c);
        if (q == 0)
            return 1;
        if (*(int *)ai_Time - *(int *)(q + 0x28) <= 1)
            return 1;
        *(int *)(q + 0x1c) = -1;
        *(int *)(q + 0x20) = -1;
        *(int *)(q + 0x24) = -1;
        *(int *)(q + 0x28) = *(int *)ai_Time - 1;
    } else if (G17_I(*m, 0) == 0x62757920) {
        c = gm_FindChar(G17_I(*m, 4));
        if (c == 0)
            return 1;
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12c);
        if (q == 0)
            return 1;
        k = G17_I(*m, 0xc);
        for (i = 0; i < 3; i++) {
            if (*(int *)(q + 0x1c + i * 4) == k)
                break;
        }
        if (i >= 3)
            return 1;
        *(int *)(q + 0x1c + i * 4) = -1;
        if (ai_PlayerId == *(unsigned short *)(b + 0x27))
            ai_Unk5a8c50(c, ai_Chars[ai_PlayerId], b, G17_I(*m, 0x10));
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cc510, handler of type 0x54: the sub-commands 'join' / 'vael' (leave) / 'recl' of the four slots of the job
   0x12d of building data[8] (character data[4]) */

int ai_Unk4cc510(G17MSG *m, G17STAT *s)
{
    char *b, *q, *ch;
    int i, j, id;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (G17_I(*m, 0) == 0x6a6f696e) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        for (i = 0; i < 4; i++) {
            if (*(int *)(q + 0x1c + i * 4) == G17_I(*m, 4))
                return 1;
        }
        for (j = 0; j < 4; j++) {
            if (*(int *)(q + 0x1c + j * 4) == -1)
                break;
        }
        if (j >= 4)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_Unk541b40(G17_I(*m, 0), b, q, G17_I(*m, 4));
        }
        *(int *)(q + 0x1c + j * 4) = G17_I(*m, 4);
        return 0;
    }
    if (G17_I(*m, 0) == 0x6c656176) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        for (j = 0; j < 4; j++) {
            if (*(int *)(q + 0x1c + j * 4) == G17_I(*m, 4))
                break;
        }
        if (j >= 4)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_Unk541b40(G17_I(*m, 0), b, q, G17_I(*m, 4));
        }
        *(int *)(q + 0x1c + j * 4) = -1;
        return 0;
    }
    if (G17_I(*m, 0) == 0x636c6572) {
        b = gm_BuildQuery(1, 1, G17_I(*m, 8));
        if (b == 0)
            return 1;
        id = *(unsigned short *)(b + 0x27);
        if (id != 0xffff) {
            ch = ai_Chars[id];
            if (ch != 0 && ch != ai_PlayerCharC && ch[8] != 0 && ch[2] != 0xf)
                return 1;
        }
        q = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d);
        if (q == 0)
            return 1;
        if (s != 0) {
            s->state = 1;
            he_Unk541b40(G17_I(*m, 0), b, q, G17_I(*m, 4));
        }
        for (j = 0; j < 4; j++) {
            if (*(int *)(q + 0x1c + j * 4) == G17_I(*m, 4))
                *(int *)(q + 0x1c + j * 4) = -1;
        }
        return 0;
    }
    return 1;
}
/* 0x4cc720, sets the position (a) and the direction (b) of the model of the character record c and its id x (+0x24);
   a vector that is not within 8.0 of the current one replaces it */

void ai_Unk4cc720(char *c, float *a, float *b, int x)
{
    char *o;

    if (c != 0 && *(char **)(c + 0x184) != 0) {
        o = *(char **)(*(char **)(c + 0x184) + 0x34);
        if (d3_VecEqual((float *)(o + 0x4c), a, 8.0f) == 0)
            d3_SetObjectPos(o, a);
        o = *(char **)(*(char **)(c + 0x184) + 0x34);
        if (d3_VecEqual((float *)(o + 0x84), b, 8.0f) == 0)
            d3_SetObjectRot(o, b);
        *(int *)(c + 0x24) = x;
    }
}
/* 0x4cc7b0, handler of type 0x55: sub-commands data[4] (1..7) for the character data[0x25] when the message is
   for our player */

int ai_Unk4cc7b0(G17MSG *m, G17STAT *s)
{
    char *c;

    if (ai_Unk6f0d90 == 0 || *ai_Unk6f0d90 != G17_I(*m, 0))
        return 1;
    c = ai_Unk4ce130(G17_I(*m, 0x25));
    switch (m->data[4]) {
    case 2:
        if (c[8] != 0) {
            ai_Unk4cc720(c, (float *)&m->data[5], (float *)&m->data[0x15], G17_I(*m, 0x21));
            ai_Unk4d6ba0(&m->data[0x25]);
        }
        break;
    case 6:
        if (c[8] != 0) {
            ai_Unk4cc720(c, (float *)&m->data[5], (float *)&m->data[0x15], G17_I(*m, 0x21));
            ai_Unk4d6f60(&m->data[0x25]);
        }
        break;
    case 1:
        if (c[8] != 0)
            ai_Unk4d6f90(&m->data[0x25]);
        break;
    case 4:
        if (c[8] != 0)
            ai_Unk4d7000(&m->data[0x25]);
        break;
    case 5:
        if (c[8] != 0)
            ai_Unk4d70a0(&m->data[0x25]);
        break;
    case 7:
        ai_Unk4d7150(&m->data[0x25]);
        break;
    case 3:
        if (c[8] != 0) {
            ai_Unk4cc720(c, (float *)&m->data[5], (float *)&m->data[0x15], G17_I(*m, 0x21));
            ai_Unk4d7190(&m->data[0x25]);
        }
        break;
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 8;
        s->value = 0;
    }
    return 0;
}
/* 0x4cc8e0, handler of type 0x56 */

int ai_Unk4cc8e0(G17MSG *m, G17STAT *s)
{
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ord_HandleMessageRecord(&m->data[0]) == 0)
        return 1;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cc920, handler of type 0x57: frees the record +0x184 of character data[0] */

int ai_Unk4cc920(G17MSG *m, G17STAT *s)
{
    char *c;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    c = gm_FindChar(G17_I(*m, 0));
    if (c != 0 && *(int *)(c + 0x184) != 0) {
        ch_Delete(*(int *)(c + 0x184));
        *(int *)(c + 0x184) = 0;
        if (s != 0)
            s->state = 1;
        return 0;
    }
    return 1;
}
/* 0x4cc980, handler of type 0x58: ends the group data[0] (its members' link +0x20c is cleared) */

int ai_Unk4cc980(G17MSG *m, G17STAT *s)
{
    char *q, *c;
    int i, id;

    q = cm_PartyById(G17_I(*m, 0));
    if (q == 0)
        return 1;
    for (i = 0; i < (unsigned char)q[0x30]; i++) {
        id = *(int *)(q + 0x34 + i * 4);
        if (id != -1) {
            c = gm_FindChar(id);
            if (c != 0 && *(int *)(c + 0x20c) == *(int *)q)
                *(int *)(c + 0x20c) = -1;
        }
    }
    ai_Unk4e5180(G17_I(*m, 0));
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4cca10, handler of type 0x59: the group data[0] gets the 16 members data[4..] */

int ai_Unk4cca10(G17MSG *m, G17STAT *s)
{
    char *q, *c;
    int i;

    if (ai_Unk4c69c0(m) != 0) {
        if (s != 0)
            s->state = 2;
        return 1;
    }
    if (s != 0)
        s->state = 2;
    q = cm_PartyById(G17_I(*m, 0));
    if (q != 0)
        *(int *)(q + 0x28) = 1;
    for (i = 0; i < 16; i++) {
        c = gm_FindChar(*(int *)&m->data[4 + i * 4]);
        if (c != 0)
            *(int *)(c + 0x20c) = G17_I(*m, 0);
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 0;
        s->value = 0;
    }
    return 0;
}
/* 0x4ccaa0, handler of type 0x5a: adds data[4] to +0x194 of the character data[0] (0..0x32) */

int ai_Unk4ccaa0(G17MSG *m, G17STAT *s)
{
    char *c;
    int v;

    c = gm_FindChar(G17_I(*m, 0));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (c == 0)
        return 1;
    v = G17_I(*m, 4) + *(int *)(c + 0x194);
    v = v < 0 ? 0 : v;
    if (v >= 0x32)
        v = 0x32;
    *(int *)(c + 0x194) = v;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4ccb10, handler of type 0x5b: adds data[4] to the byte +0x1b1 of the character data[0] (0..0xff) */

int ai_Unk4ccb10(G17MSG *m, G17STAT *s)
{
    char *c;
    int v;

    c = gm_FindChar(G17_I(*m, 0));
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (c == 0)
        return 1;
    v = (unsigned char)c[0x1b1] + G17_I(*m, 4);
    v = v < 0 ? 0 : v;
    if (v >= 0xff)
        v = 0xff;
    c[0x1b1] = v;
    v = (unsigned char)c[0x1b1] != 0 ? (unsigned char)c[0x1b1] : 0;
    c[0x1b1] = v;
    if (*(unsigned short *)c == ai_PlayerId && ai_GameState != 0 && (unsigned char)v != 0)
        sv_PopTo(ai_GameFrame);
    amt_ClearOfficesIfAll(c, G17_I(*m, 8));
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4ccbd0, handler of type 0x5c */

int ai_Unk4ccbd0(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    r = amt_ExecuteSwapMessage(&m->data[0]);
    if (s != 0 && r != 0)
        s->state = 1;
    if (r == 0)
        return 1;
    return 0;
}
/* 0x4ccc20, handler of type 0x5d: adds data[8] to the skill byte data[4] (0..4) of character data[0] (0..252) */

int ai_Unk4ccc20(G17MSG *m, G17STAT *s)
{
    char *c;
    int k;
    float f;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    c = gm_FindChar(G17_I(*m, 0));
    if (c == 0)
        return 1;
    k = G17_I(*m, 4);
    if (k < 0 || k > 4)
        return 1;
    f = (float)((unsigned char)c[0x80 + k] + G17_I(*m, 8));
    f = f >= 252.0f ? 252.0f : (f <= 0.0f ? 0.0f : f);
    c[0x80 + k] = (char)f;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4ccce0, handler of type 0x5e: the job record 0xca of building data[4] (for character data[0x16]) is set */

int ai_Unk4ccce0(G17MSG *m, G17STAT *s)
{
    char *a, *b, *q;
    int v;

    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0);
    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    a = gm_FindBuildingById(G17_I(*m, 0x16));
    if (a == 0)
        return 1;
    b = gm_FindBuildingById(G17_I(*m, 4));
    if (b == 0)
        return 1;
    q = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xca);
    while (q != 0) {
        if (*(int *)(q + 0x2a) == *(int *)(a + 1))
            goto found;
        q = gm_QueryNext();
    }
    q = gm_CreateObject(*(int *)(b + 1), 0xca, 1);
    if (q == 0)
        return 1;
    cm_LastObjectId = *(int *)(q + 2);
found:
    *(G17T14 *)(q + 0x1c) = *(G17T14 *)&m->data[8];
    *(int *)(q + 0x2a) = G17_I(*m, 0x16);
    *(int *)(q + 0x32) = G17_I(*m, 0x1e);
    q[0x36] = m->data[0x22];
    q[0x37] = q[0x37] + m->data[0x23];
    if ((unsigned char)q[0x37] > 0x64)
        v = 0x64;
    else
        v = (unsigned char)q[0x37];
    q[0x37] = v;
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cce00, handler of type 0x5f: the same as 0x4cd870 */

int ai_Unk4cce00(G17MSG *m, G17STAT *s)
{
    return ai_Unk4cd870(m, s);
}
/* 0x4cce10, handler of type 0x60: kind data[0] (0..6) picks the creator */

int ai_Unk4cce10(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 0x17);
    else
        G17_I(*m, 0x1b) = rand();
    switch (G17_I(*m, 0)) {
    case 0:
        r = gv_BuryCharacter(&m->data[0]);
        break;
    case 1:
        r = gv_ReserveGrave(&m->data[0]);
        break;
    case 2:
        r = gv_ExtendGrave(&m->data[0]);
        break;
    case 3:
        r = gv_SetGraveHolder(&m->data[0]);
        break;
    case 4:
        r = gv_OpenSize(&m->data[0]);
        break;
    case 5:
        r = gv_AdjustEntry(&m->data[0]);
        break;
    case 6:
        r = gv_AddNewDead(&m->data[0]);
        break;
    default:
        return -1;
    }
    if (s != 0 && r == 0)
        s->state = 1;
    return r;
}
/* 0x4cced0, handler of type 0x61 */

int ai_Unk4cced0(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    r = gv_MoveGrave(&m->data[0]);
    if (s != 0 && r == 0)
        s->state = 1;
    return r;
}
/* 0x4ccf10, handler of type 0x62: creates a character from the message (seed data[0]) */

int ai_Unk4ccf10(G17MSG *m, G17STAT *s)
{
    int id;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    else
        G17_I(*m, 0) = rand();
    util_Seed(G17_I(*m, 0));
    id = gv_RaiseDead((struct GVRAISE *)&m->data[0]);
    if (id == 0xffff)
        return -1;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
/* 0x4ccf90, handler of type 0x63 */

int ai_Unk4ccf90(G17MSG *m, G17STAT *s)
{
    int id;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (ai_Unk1460ae0 != -1)
        ai_NextObjectId = G17_I(*m, 4);
    else
        G17_I(*m, 0) = rand();
    util_Seed(G17_I(*m, 0));
    id = ahm_CreateAlchemistI((int *)&m->data[0]);
    if (id == 0xffff)
        return -1;
    if (s != 0) {
        s->state = 1;
        s->unk01 = 1;
        s->value = (int)ai_Chars[id];
    }
    return 0;
}
/* 0x4cd010, handler of type 0x64: the four-letter command data[0] ('SmSa', 'ErFi', 'SsCa', 'cRSa', 'Rdaa', 'Rdsa')
   for the character data[4] */

int ai_Unk4cd010(G17MSG *m, G17STAT *s)
{
    char *c, *g;
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    c = gm_FindChar(G17_I(*m, 4));
    if (c == 0)
        return -1;
    if (ahm_HasRecord(c) == 0)
        return -2;
    switch (G17_I(*m, 0)) {
    case 0x61536d53:
        r = ahm_AddValue(&m->data[0], *(int *)(*(char **)(c + 0x16c) + 0x65));
        break;
    case 0x46697245:
        g = *(char **)(c + 0x16c);
        r = ahm_DeleteAlchemist(&m->data[0], *(int *)(g + 0x65), g);
        break;
    case 0x61437353:
        r = ahm_SetShare(&m->data[0], *(int *)(*(char **)(c + 0x16c) + 0x65));
        break;
    case 0x61535263:
        r = ahm_SetPlaceValue(&m->data[0], *(int *)(*(char **)(c + 0x16c) + 0x65));
        break;
    case 0x61616452:
        g = *(char **)(c + 0x16c);
        r = ahm_AddPlace(&m->data[0], *(int *)(g + 0x65), g);
        break;
    case 0x61736452:
        g = *(char **)(c + 0x16c);
        r = ahm_RemovePlace(&m->data[0], *(int *)(g + 0x65), g);
        break;
    default:
        return -3;
    }
    if (s != 0 && r != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd130, handler of type 0x65 */

int ai_Unk4cd130(G17MSG *m, G17STAT *s)
{
    char *q;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    q = cm_PartyById(G17_I(*m, 0));
    if (q == 0)
        return 1;
    *(int *)(q + 0xc) = G17_I(*m, 4);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd180, handler of type 0x66: 'nCTa' / 'nCTd' */

int ai_Unk4cd180(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    if (G17_I(*m, 0) == 0x6154436e)
        r = gv_ReserveEffect(G17_I(*m, 4));
    else if (G17_I(*m, 0) == 0x6454436e)
        r = gv_StopEffect(G17_I(*m, 4));
    else
        return -1;
    if (s != 0 && r != 0)
        s->state = 1;
    if (r != 0)
        return 0;
    return -1;
}
/* 0x4cd1e0, handler of type 0x67 */

int ai_Unk4cd1e0(G17MSG *m, G17STAT *s)
{
    int r;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    r = gv_StartShot((struct GVSHOTMSG *)&m->data[0], s);
    if (s != 0 && r != 0)
        s->state = 1;
    if (r != 0)
        return 0;
    return -1;
}
/* 0x4cd220, handler of type 0x68: the data[4] records (0x40 bytes each) of the joined message become the list of the
   object data[0] (+0x29, rt_CopyPoints; the old list is freed) */

int ai_Unk4cd220(G17MSG *m, G17STAT *s)
{
    char buf[0x240];
    char *q;
    int n, i, prev;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    q = gm_FindObjectInst(G17_I(*m, 0));
    if (q == 0)
        return -1;
    if (*(int *)(q + 0x29) != 0) {
        rt_FreePoints(*(int *)(q + 0x29));
        *(int *)(q + 0x29) = 0;
    }
    n = G17_I(*m, 4);
    memcpy(buf, ai_Unka68d40, n << 6);
    prev = 0;
    for (i = 0; i < n; i++) {
        *(int *)(buf + i * 0x40 + 0x34) = prev;
        *(int *)(buf + i * 0x40 + 0x38) = (int)(buf + i * 0x40 + 0x3c);
        prev = (int)(buf + i * 0x40 + 0x3c) - 0x40;
    }
    *(int *)(buf + n * 0x40 - 4) = 0;
    *(int *)(q + 0x29) = rt_CopyPoints(buf);
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd2f0, handler of type 0x69: frees the list +0x29 of the object data[0] */

int ai_Unk4cd2f0(G17MSG *m, G17STAT *s)
{
    char *q;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    q = gm_FindObjectInst(G17_I(*m, 0));
    if (q == 0)
        return -1;
    if (*(int *)(q + 0x29) != 0) {
        rt_FreePoints(*(int *)(q + 0x29));
        *(int *)(q + 0x29) = 0;
    }
    if (s != 0)
        s->state = 1;
    return 0;
}
/* 0x4cd350, a free slot of the 8 records (0x57 bytes) of ai_Unk9364e0, 0: none */

char *ai_Unk4cd350()
{
    int i = 0;
    char *p;

    for (p = ai_Unk9364e0; (int)p < (int)(ai_Unk9364e0 + 8 * 0x57); p += 0x57) {
        if (*(int *)p == 0)
            return ai_Unk9364e0 + i * 0x57;
        i++;
    }
    return 0;
}
/* 0x4cd380, the record with id, else a free one */

char *ai_Unk4cd380(int id)
{
    int i;
    char *p;

    if (id == 0)
        return 0;
    i = 0;
    for (p = ai_Unk9364e0; (int)p < (int)(ai_Unk9364e0 + 8 * 0x57); p += 0x57) {
        if (*(int *)p == id)
            return ai_Unk9364e0 + i * 0x57;
        i++;
    }
    return ai_Unk4cd350();
}
/* 0x4cd3c0, appends a line (text) of the player id (with x) to the ring ai_Unk935da0[15] */

void ai_Unk4cd3c0(int id, int x, char *text)
{
    char *p;
    int n;

    p = cm_LobbyFindPlayer(id);
    if (p != 0) {
        sprintf(ai_Unk935da0[ai_Unk9363f4].name, "%s %s", p + 0x12, p + 0x22);
        n = ai_Unk9363f4;
    } else {
        n = ai_Unk9363f4;
        *(int *)ai_Unk935da0[n].name = ai_Unk60c7b0[0];
        *(int *)(ai_Unk935da0[n].name + 4) = ai_Unk60c7b0[1];
    }
    ai_Unk935da0[n].id = id;
    ai_Unk935da0[n].x = x;
    strcpy(ai_Unk935da0[n].text, text);
    n++;
    ai_Unk9363f4 = n;
    if (n >= 15)
        ai_Unk9363f4 = 0;
    if (ai_Unk6f0cd8 < 15)
        ai_Unk6f0cd8++;
}
/* 0x4cd480, the oldest line of the ring (0: none) */

G18LOG *ai_Unk4cd480()
{
    int i;

    if (ai_Unk6f0cd8 == 0)
        return 0;
    i = ai_Unk9363f4 - ai_Unk6f0cd8;
    ai_Unk6f0cdc = i;
    if (i < 0) {
        i = ai_Unk9363f4 + 1;
        ai_Unk6f0cdc = i;
    }
    return &ai_Unk935da0[i];
}
/* 0x4cd4c0, the next line of the ring (0: no more) */

G18LOG *ai_Unk4cd4c0()
{
    int i = ai_Unk6f0cdc;

    if (i == ai_Unk9363f4)
        return 0;
    i++;
    ai_Unk6f0cdc = i;
    if (i == ai_Unk9363f4)
        return 0;
    if (i == 15) {
        i = 0;
        ai_Unk6f0cdc = 0;
    }
    return &ai_Unk935da0[i];
}
/* 0x4cd500 */

void cm_LobbyClearChat()
{
    ai_Unk9363f4 = 0;
    ai_Unk6f0cd8 = 0;
}
/* 0x4cd510, all players of the list (ai_Unk936424[8]) have state 1 and their number is the one asked for */

int cm_LobbyAllReady()
{
    unsigned int n;
    int cnt = 0;
    int *p;
    char *q;

    if (cm_NetGame.mode == 0x3f)
        n = cm_NetGame.hdr.players;
    else
        n = cm_NetGame.hdr.players;
    if (ai_Unk936420[0] < n)
        return 0;
    for (p = ai_Unk936424; (int)p < (int)(ai_Unk936424 + 8); p++) {
        if (*p != 0) {
            q = cm_LobbyFindPlayer(*p);
            if (q == 0)
                return 0;
            if (*(int *)(q + 4) == 1)
                cnt++;
        }
    }
    return cnt == n;
}
/* 0x4cd580 */

void ai_Unk4cd580(int a, int b)
{
    memset(ai_Unk936420, 0, 0x24);
    memset(ai_Unk9364e0, 0, 0x2b8);
    ai_Unk936400 = 1;
    ai_Unk9367d0 = a;
    ai_Unk9367d4 = b;
    ai_Unk936408 = 0;
}
/* 0x4cd5d0, the lobby's player list ai_Unk936420 (the count, then the eight player ids) */

unsigned int *cm_LobbyPlayerList()
{
    return ai_Unk936420;
}
/* 0x4cd5e0 */

char *cm_LobbyFindPlayer(int id)
{
    return ai_Unk4cd380(id);
}
/* 0x4cd5f0, this player's lobby record (the one of the id ai_Unk9367d0) */

CMLOBBYPLAYER *cm_LobbyMyPlayer()
{
    return (CMLOBBYPLAYER *)ai_Unk4cd380(ai_Unk9367d0);
}
/* 0x4cd600, the lobby state: [0] the lobby is open (ai_Unk4cd580), [1] a lobby message came in (ai_Unk4cd870) */

int *cm_LobbyState()
{
    return &ai_Unk936400;
}
/* 0x4cd610, sends the lobby message 0x5f kind 0 (asks for the player list); returns the send slot (cm_GetSlotState) */

int cm_LobbyRequestList()
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    G17_I(m, 0) = 0;
    m.type = 0x5f;
    return ai_Unk4c39c0(&m);
}
/* 0x4cd650, kind 1: this player's ready flag v; returns the send slot */

int cm_LobbySetReady(int v)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    G17_I(m, 4) = ai_Unk9367d0;
    m.type = 0x5f;
    G17_I(m, 0) = 1;
    G17_I(m, 8) = v;
    return ai_Unk4c39c0(&m);
}
/* 0x4cd6a0, kind 3: this player leaves the lobby; returns the send slot */

int cm_LobbyLeave()
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    G17_I(m, 0) = 3;
    G17_I(m, 4) = ai_Unk9367d0;
    return ai_Unk4c39c0(&m);
}
/* 0x4cd6e0, kind 6: a chat line (0x3f characters) of this player to the player id to (0: everybody); returns the
   send slot */

int cm_LobbySendChat(int to, char *text)
{
    G17MSG m;
    struct {
        int a, b;
        char name[0x40];
    } t;

    memset(m.data, 0, 0x80);
    t.a = ai_Unk9367d0;
    t.b = to;
    strncpy(t.name, text, 0x3f);
    m.type = 0x5f;
    t.name[0x3f] = 0;
    G17_I(m, 0) = 6;
    memcpy(&m.data[4], &t, 0x48);
    return ai_Unk4c39c0(&m);
}
/* 0x4cd760, kind 2: this player joins with the record p (its id is set here); returns the send slot */

int cm_LobbyJoin(CMLOBBYPLAYER *p)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    p->id = ai_Unk9367d0;
    G17_I(m, 0) = 2;
    memcpy(&m.data[4], p, 0x57);
    return ai_Unk4c39c0(&m);
}
/* 0x4cd7c0, kind 4: this player's changed record p (its id is set here); returns the send slot */

int cm_LobbyUpdatePlayer(CMLOBBYPLAYER *p)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    p->id = ai_Unk9367d0;
    G17_I(m, 0) = 4;
    memcpy(&m.data[4], p, 0x57);
    return ai_Unk4c39c0(&m);
}
/* 0x4cd820, kind 5 */

void ai_Unk4cd820(int v)
{
    G17MSG m;

    memset(m.data, 0, 0x80);
    m.type = 0x5f;
    G17_I(m, 0) = 5;
    G17_I(m, 4) = v;
    ai_Unk4c39c0(&m);
}
/* 0x4cd870, the handler of type 0x5f (also 0x4cce00): data[0] is the kind (0..6) of the player list message */

int ai_Unk4cd870(G17MSG *m, G17STAT *s)
{
    char *p;
    struct {
        int a, b;
        char name[0x40];
    } t;

    if (s != 0)
        s->state = 2;
    switch (G17_I(*m, 0)) {
    case 0:
        memcpy(ai_Unk936420, &m->data[4], 0x24);
        break;
    case 1:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        *(int *)p = G17_I(*m, 4);
        *(int *)(p + 4) = G17_I(*m, 8);
        break;
    case 2:
        p = ai_Unk4cd350();
        if (p == 0)
            return -1;
        memcpy(p, &m->data[4], 0x57);
        break;
    case 3:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        if (G17_I(*m, 8) == 1)
            ai_Unk936408 = 1;
        memset(p, 0, 0x57);
        break;
    case 4:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        memcpy(p, &m->data[4], 0x57);
        break;
    case 5:
        p = ai_Unk4cd380(G17_I(*m, 4));
        if (p == 0)
            return -2;
        memcpy(p, &m->data[4], 0x57);
        break;
    case 6:
        memcpy(&t, &m->data[4], 0x48);
        ai_Unk4cd3c0(t.a, t.b, t.name);
        break;
    default:
        return 1;
    }
    if (s != 0)
        s->state = 1;
    ai_Unk936404 = 1;
    return 0;
}
/* 0x4cd9d0, the record of the 10 (0x5c bytes, ai_Unk60c7c0) with id */

char *ai_FindWeaponRecord(int id)
{
    int i = 0;
    char *p;

    for (p = ai_Unk60c7c0; (int)p < (int)(ai_Unk60c7c0 + 10 * 0x5c); p += 0x5c) {
        if (*(int *)p == id)
            return ai_Unk60c7c0 + i * 0x5c;
        i++;
    }
    return 0;
}
/* 0x4cda00, the first record (ai_FindWeaponRecord) of the entries of kind 5 of the list +0x178 of the character */

char *ai_Unk4cda00(char *c)
{
    char *q, *r;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        r = ai_FindWeaponRecord(*(unsigned short *)q);
        if (r != 0)
            return r;
    }
    return ai_FindWeaponRecord(0);
}
/* 0x4cda50, as 0x4cda00; a record of kind 1 / 2 whose entry has nothing left (+0x20) is not taken */

char *ai_Unk4cda50(char *c)
{
    char *q, *r;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        r = ai_FindWeaponRecord(*(unsigned short *)q);
        if (r != 0) {
            if ((*(int *)(r + 0x58) == 1 || *(int *)(r + 0x58) == 2) && *(int *)(q + 0x20) <= 0)
                return ai_FindWeaponRecord(0);
            return r;
        }
    }
    return ai_FindWeaponRecord(0);
}
/* 0x4cdac0, the entry (not the record) of that search, 0: none */

char *ai_Unk4cdac0(char *c)
{
    char *q, *r;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        r = ai_FindWeaponRecord(*(unsigned short *)q);
        if (r != 0) {
            if ((*(int *)(r + 0x58) == 1 || *(int *)(r + 0x58) == 2) && *(int *)(q + 0x20) <= 0)
                return 0;
            return q;
        }
    }
    return 0;
}
/* 0x4cdb20 */

void ai_Unk4cdb20(char *c)
{
    char *q;
    int id;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        id = *(unsigned short *)q;
        if (ai_FindWeaponRecord(id) != 0)
            cm_SendExSellObject(-1, *(int *)(c + 4), id, *(int *)(q + 0xe), ai_OwnPlayer, 0);
    }
}
/* 0x4cdb80, a character with the flag 0x20000000: with chance (byte +0x1c5) / 10 it is hit (1), else the flag is
   cleared (0) */

char ai_Unk4cdb80(char *c)
{
    char buf[0x800];
    char b = 1;
    char neg;
    char *ch, *g;
    int *r;
    int id;

    if (c != 0 && (*(int *)(c + 0x1c8) & 0x20000000) != 0) {
        neg = c[0x1c5];
        if (util_RandomU(10) < (unsigned char)neg) {
            neg = -neg;
            ai_Unk4cdb20(c);
            cm_BeginFieldBlock(c, *(int *)(c + 4));
            cm_AddFieldValues(1, 1, (int)(c - cm_FieldObject) + 0x1c5, &neg);
            cm_SendFieldDeltas();
            g = *(char **)(c + 0x16c);
            if (g != 0 && *(unsigned short *)(g + 0x27) != 0xffff) {
                id = *(unsigned short *)(g + 0x27);
                ch = ai_Chars[id];
                if (ch[2] == 6 || ch[2] == 7) {
                    r = (int *)ai_Unk4cda50(c);
                    if (*r != 0) {
                        txt_Format(buf, 0x1bca, *r * 2 + 0x9a8, *(unsigned short *)c, g);
                        he_ShowNews(*(int *)(ch + 4), -1, buf, 0, 0x5f2, 0);
                    }
                }
            }
            return 1;
        }
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldValues(1, 1, (int)(c - cm_FieldObject) + 0x1c5, &b);
        cm_SendFieldDeltas();
        cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0, 0x20000000);
    }
    return 0;
}
/* 0x4cdcf0, a factor by the record kind of the character (0x154 .. 0x176) */

float ai_Unk4cdcf0(char *c)
{
    float f = 0.5f;
    float g;
    int *p;

    g = gm_GetSkill(c, 3);
    p = (int *)ai_Unk4cda50(c);
    if (p != 0) {
        switch (*p) {
        case 0x154:
            return gm_HealthRatio(c) * g * 0.5f;
        case 0x156:
        case 0x16e:
        case 0x172:
            return gm_HealthRatio(c) * g * 0.8f;
        case 0x158:
            return gm_HealthRatio(c) * g * 1.0f;
        case 0x15e:
        case 0x160:
            return gm_HealthRatio(c) * g * 0.7f;
        case 0x174:
        case 0x176:
            return gm_HealthRatio(c) * g * 0.9f;
        default:
            f = 0.3f;
        }
    }
    return gm_HealthRatio(c) * g * f;
}
/* 0x4cde20, the model name "ob_<NAME>" of the record kind (+0x64 of the record +0x184) of the character */

void cm_UpdateHandObject(char *c)
{
    char buf[0x80];
    int *r;
    char *g;

    r = (int *)ai_Unk4cda50(c);
    if (r != 0 && *r != 0) {
        if (*(char **)(c + 0x184) == 0)
            return;
        sprintf(buf, "ob_%s", _strupr(ai_Goods + *r * 0x41 + 1));
        g = *(char **)(c + 0x184);
        if (*(int *)(g + 0x64) != 0 && strcmp(*(char **)(g + 0x64), buf) == 0)
            return;
        g = *(char **)(c + 0x184);
        if (*(int *)(g + 0x64) != 0)
            ch_SetHandObject(g, 2, 0);
        ch_SetHandObject(*(char **)(c + 0x184), 2, buf);
        return;
    }
    g = *(char **)(c + 0x184);
    if (g != 0 && *(int *)(g + 0x64) != 0)
        ch_SetHandObject(g, 2, 0);
}
/* 0x4cdf00, the record of the character c in the 16 records (0x2c bytes, id at +0) of rec + 0x94, else a free one
   (id -1), 0: none */

int *ai_Unk4cdf00(char *rec, char *c)
{
    int id = *(int *)(c + 4);
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0x94 + i * 0x2c) == id)
            return (int *)(rec + 0x94 + i * 0x2c);
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0x94 + i * 0x2c) == -1) {
            memset(rec + 0x94 + i * 0x2c, 0, 0x2c);
            *(int *)(rec + i * 0x2c + 0x98) = 0;
            *(int *)(rec + 0x94 + i * 0x2c) = *(int *)(c + 4);
            return (int *)(rec + 0x94 + i * 0x2c);
        }
    }
    return 0;
}
/* 0x4cdf80, the record of the character c in the first (ai_Unk6f0d04 + 0x30) cutscene slots */

int *ai_Unk4cdf80(char *c)
{
    int n = *(unsigned char *)(ai_Unk6f0d04 + 0x30);
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < 16; j++) {
            if (*(int *)(ai_Unk92cae0[i] + 0x94 + j * 0x2c) == *(int *)(c + 4))
                return (int *)(ai_Unk92cae0[i] + 0x94 + j * 0x2c);
        }
    }
    return 0;
}
/* 0x4cdfe0, 1 when the cell (x, y) of the map ai_Unk5fb5b0 + 0xb0 is inside and passable */

int ai_Unk4cdfe0(int x, int y)
{
    char *g = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int w = *(int *)(g + 0x20);

    if (x > w || x < 0 || y < 0 || y > w || x >= w || y >= w)
        return 0;
    if (*(int *)(*(char **)(g + 0x24) + (w * y + x) * 0x1c) == 0 || *(int *)(*(char **)(g + 0x24) + (w * y + x) * 0x1c) == 0xd)
        return 0;
    return 1;
}
/* 0x4ce040, the nearest passable cell around (a, b) (rings of the distance d = 0 .. 0x3f): *ox, *oy */

int ai_Unk4ce040(int a, int b, int *ox, int *oy)
{
    int w = *(int *)(*(char **)(ai_Unk5fb5b0 + 0xb0) + 0x20);
    int d, y, x, k, ylo, yhi, xlo, xhi, t;

    for (d = 0; d < 0x40; d++) {
        yhi = w - 1 < b + d ? w - 1 : b + d;
        ylo = b - d < 0 ? 0 : b - d;
        for (y = ylo; y <= yhi; y++) {
            t = b - y;
            k = d - (t < 0 ? -t : t);
            xhi = w - 1 < k + a ? w - 1 : k + a;
            xlo = a - k < 0 ? 0 : a - k;
            for (x = xlo; x <= xhi; x++) {
                if (ai_Unk4cdfe0(x, y) != 0) {
                    *ox = x;
                    *oy = y;
                    return 1;
                }
            }
        }
    }
    return 0;
}
/* 0x4ce130, the record (0x21c bytes) with id (+4) of the 32 of ai_Unk930600 */

char *ai_Unk4ce130(int id)
{
    int i = 0;
    char *p;

    for (p = ai_Unk930600 + 4; (int)p < (int)(ai_Unk930600 + 4 + 32 * 0x21c); p += 0x21c) {
        if (*(unsigned short *)(p - 4) != 0xffff && *(int *)p == id)
            return ai_Unk930600 + i * 0x21c;
        i++;
    }
    return 0;
}
/* 0x4ce170 */

int ai_LedSelectBits(int idx, char *c)
{
    int r = 0;

    if (*(char **)(c + 0x16c) == ai_Chars[idx])
        r = 0x800;
    if (gm_AllowAll != 0)
        r = 0x800;
    return r;
}
/* 0x4ce1b0, the distance of the character c's model to the object o (x / z) */

float ai_Unk4ce1b0(char *c, char *o)
{
    char *p = *(char **)(*(char **)(c + 0x184) + 0x34);
    float dx = *(float *)(o + 0x4c) - *(float *)(p + 0x4c);
    float dz = *(float *)(o + 0x54) - *(float *)(p + 0x54);

    return (float)sqrt(dx * dx + dz * dz);
}
/* 0x4ce200, the distance of the character c's model to the cell (x, y) */

float ai_Unk4ce200(char *c, int x, int y)
{
    float v[3];
    char *p;
    float dx, dz;

    d3sm_GetVertexI(*(int *)(ai_Unk5fb5b0 + 0xb0), x, y, v);
    v[1] = 0;
    p = *(char **)(*(char **)(c + 0x184) + 0x34);
    dx = v[0] - *(float *)(p + 0x4c);
    dz = v[2] - *(float *)(p + 0x54);
    return (float)sqrt(dz * dz + dx * dx);
}
/* 0x4ce270, 1 when the two characters are not in the same building (+0x16c) */

int ai_Unk4ce270(char *a, char *b)
{
    return *(int *)(a + 0x16c) != *(int *)(b + 0x16c);
}
/* 0x4ce290, a bomb object "ob_BOMBE" at the position p in a free slot of the 32 (object, time) of cm_Bombs */

char *ai_Unk4ce290(char *p)
{
    float v[3];
    int i;
    char *o;

    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    for (i = 0; i < 32; i++) {
        if (*(int *)(cm_Bombs + i * 8) == 0)
            break;
    }
    if (i >= 32)
        return 0;
    o = d3_CreateObject(0, p, v, "ob_BOMBE");
    *(char **)(cm_Bombs + i * 8) = o;
    *(int *)(cm_Bombs + i * 8 + 4) = d3_Time;
    return cm_Bombs + i * 8;
}
/* 0x4ce300, updates the bombs of cm_Bombs: a bomb older than 0x15e ticks explodes ("Explosion_s", the effects
   0x416340 / 0x4168e0 / 0x416040) and hurts the characters within 170.0 (bear hug action 0x60cb80); returns the number
   of used slots. Rough draft: the effect arguments are from the disassembly, not checked. */

int ai_Unk4ce300()
{
    float v[8];
    float pos[3];
    char col[4];
    float dx, dy, dz, dist;
    char *r;
    char **pp;
    char *rec, *c, *g;
    int cnt = 0;
    int n, t1, amt;
    unsigned int k;
    double d1;
    char *e;

    r = ai_FindWeaponRecord(0x174);
    for (rec = cm_Bombs; (int)rec < (int)(cm_Bombs + 0x100); rec += 8) {
        if (*(int *)rec == 0)
            continue;
        cnt++;
        if (d3_Time <= (unsigned int)(*(int *)(rec + 4) + 0x15e))
            continue;
        sb_PlaySampleI(ai_Unk6f0d14, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        v[0] = -243.0f;
        v[1] = -90.0f;
        v[2] = 190.0f;
        v[3] = 0;
        v[4] = -50.0f;
        v[5] = 0;
        v[6] = 38.0f;
        v[7] = 0;
        e = *(char **)rec;
        col[0] = 0xc0;
        col[1] = 0xc0;
        col[2] = 0xc0;
        col[3] = 0x3f;
        v[0] = *(float *)(e + 0x4c);
        v[1] = *(float *)(e + 0x50);
        v[2] = *(float *)(e + 0x54);
        d3t_CreateSmokeK(0, v, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, pos, 0x190, 6, *(int *)col);
        d3t_CreateEarthK(0, col, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, *(int *)col);
        d3t_CreateExplosionK(0, v, 0x1e, v, 120.0f, 80.0f, 20.0f, 60.0f, 0x3c);
        for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            c = *pp;
            if (c == 0)
                continue;
            g = *(char **)(c + 0x184);
            if (c[8] == 0)
                continue;
            e = *(char **)rec;
            dx = *(float *)(e + 0x4c) - *(float *)(*(char **)(g + 0x34) + 0x4c);
            dy = *(float *)(e + 0x50) - *(float *)(*(char **)(g + 0x34) + 0x50);
            dz = *(float *)(e + 0x54) - *(float *)(*(char **)(g + 0x34) + 0x54);
            dist = (float)sqrt(dz * dz + dy * dy + dx * dx);
            if (dist < 170.0f) {
                n = ai_Unk4e52f0((unsigned char)r[6] - (unsigned char)r[5]);
                k = n + (unsigned char)r[5];
                d1 = (double)k * *(float *)(c + 0x1c) * 0.01;
                t1 = (int)d1;
                amt = (int)(t1 * ((1.0 - dist * 0.0058823530562222f)));
                *(int *)(*pp + 0x24) -= amt;
                ai_Unk4cedc0(*pp, amt, *(int *)(*pp + 0x16c) == ai_Unk6f0cec ? 1 : 2);
                if (ai_Unk4d3390(c) == 0)
                    ch_QueueAnim(g, "kampf/bauch_greifen", 1);
            }
        }
        d3if_Delete((struct D3OBJECT *)*(int *)rec);
        *(int *)rec = 0;
    }
    return cnt;
}
/* 0x4ce5d0, a bomb: the first free slot of the 32 (0x10 bytes) of ai_Unk935a00 gets the object "ob_BOMBE" at the
   position p (y + 60.0) and a particle block thrown towards the cell (x, y); (object, effect, x, y) */

char *ai_Unk4ce5d0(float *p, int x, int y)
{
    float o[3];
    float d[3];
    char s[0x160];
    int i;
    char *obj, *slot;
    char *g = *(char **)(ai_Unk5fb5b0 + 0xb0);
    float pos[3];

    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    for (i = 0; i < 32; i++) {
        if (*(int *)(ai_Unk935a00 + i * 0x10) == 0)
            break;
    }
    if (i >= 32)
        return 0;
    pos[0] = p[0];
    pos[1] = (float)(p[1] + 60.0);
    pos[2] = p[2];
    obj = d3_CreateObject(0, pos, o, "ob_BOMBE");
    *(char **)(ai_Unk935a00 + i * 0x10) = obj;
    obj[0x211] |= 4;
    d3sm_GetVertexI(g, x, y, d);
    d[0] = d[0] - p[0];
    d[1] = d[1] - p[1];
    d[2] = d[2] - p[2];
    memset(s, 0, sizeof(s));
    *(int *)(s + 0x00) = 0x12;
    *(int *)(s + 0x58) = 0x12;
    *(float *)(s + 0x5c) = (float)(d[0] * 0.5);
    *(float *)(s + 0x60) = (float)(d[1] * 0.5 + 50.0);
    *(float *)(s + 0x64) = (float)(d[2] * 0.5);
    *(float *)(s + 0x6c) = 3.1415927f;
    *(float *)(s + 0x70) = 3.1415927f;
    *(float *)(s + 0x74) = 3.1415927f;
    *(int *)(s + 0xb0) = 1;
    d[1] = (float)(d[1] - 60.0);
    *(float *)(s + 0xb4) = d[0];
    *(float *)(s + 0xb8) = d[1];
    *(float *)(s + 0xbc) = d[2];
    *(float *)(s + 0xc4) = 3.1415927f;
    *(float *)(s + 0xc8) = 3.1415927f;
    *(float *)(s + 0xcc) = 3.1415927f;
    *(float *)(s + 0x10c) = d[0];
    *(float *)(s + 0x110) = d[1];
    *(float *)(s + 0x114) = d[2];
    *(float *)(s + 0x11c) = 3.1415927f;
    *(float *)(s + 0x120) = 3.1415927f;
    *(float *)(s + 0x124) = 3.1415927f;
    slot = ai_Unk935a00 + i * 0x10;
    *(int *)(slot + 4) = ((D3ANIM *(*)(void *, void *, int, void *))d3anim_CreatePath)(*(int *)slot, o, 4, s);
    *(int *)(slot + 0xc) = y;
    *(int *)(slot + 8) = x;
    return slot;
}
/* 0x4ce830, as 0x4ce300 for the grenades of ai_Unk935a00 (0x10 bytes: object, effect, cell x, cell y): the effect
   ends (flag 0x20 of its byte +0x2d), the cell of the map ai_Unk633738 loses 5 (and its mark is cleared), the
   characters within 100.0 are hurt (action "kampf/arm_greifen"). Rough draft. */

int ai_Unk4ce830()
{
    float v[8];
    float pos[3];
    char col[4];
    float dx, dy, dz, dist;
    char *r, *g;
    char **pp;
    char *rec, *c, *gg, *e;
    int cnt = 0;
    int n, t1, amt;
    unsigned int k;
    double d1;
    int *map;

    g = *(char **)(ai_Unk5fb5b0 + 0xb0);
    r = ai_FindWeaponRecord(0x176);
    for (rec = ai_Unk935a00; (int)rec < (int)(ai_Unk935a00 + 0x200); rec += 0x10) {
        if (*(int *)rec == 0)
            continue;
        cnt++;
        if ((*(char *)(*(char **)(rec + 4) + 0x2d) & 0x20) == 0)
            continue;
        v[0] = -243.0f;
        v[1] = -90.0f;
        v[2] = 190.0f;
        v[3] = 0;
        v[4] = -50.0f;
        v[5] = 0;
        v[6] = 38.0f;
        v[7] = 0;
        e = *(char **)rec;
        col[0] = 0xc0;
        col[1] = 0xc0;
        col[2] = 0xc0;
        col[3] = 0x3f;
        v[0] = *(float *)(e + 0x4c);
        v[1] = *(float *)(e + 0x50);
        v[2] = *(float *)(e + 0x54);
        d3t_CreateSmokeK(0, v, 0x1e, 15.0f, 50.0f, 100.0f, 150.0f, 140.0f, 35.0f, pos, 0x190, 6, *(int *)col);
        d3t_CreateEarthK(0, col, 0x1e, 5.0f, 2.5f, 1.0f, 3.0f, -3.0f, 4.0f, 7.0f, 8.0f, *(int *)col);
        d3t_CreateExplosionK(0, v, 0x1e, 70.0f, 50.0f, 20.0f, 60.0f, 0x3c);
        map = (int *)ai_Unk633738;
        if (*(char *)(*(char **)(map + 5) + *(int *)(rec + 8) + *(int *)(rec + 0xc) * map[0]) != 0) {
            *(char *)(*(char **)(map + 4) + *(int *)(rec + 8) + *(int *)(rec + 0xc) * map[0]) -= 5;
            *(char *)(*(char **)(((int *)ai_Unk633738) + 5) + *(int *)(rec + 8) + *(int *)(rec + 0xc) * *(int *)ai_Unk633738) = 0;
            d3fl_MakeDivide(ai_Unk633738);
            d3fl_MakeLight(ai_Unk633738);
        }
        sb_PlaySampleI(ai_Unk6f0d14, "Explosion_s", 1, 0x3f, 0x7f, 0, 0);
        for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            c = *pp;
            if (c == 0)
                continue;
            gg = *(char **)(c + 0x184);
            if (c[8] == 0)
                continue;
            d3sm_GetVertexI(g, *(int *)(rec + 8), *(int *)(rec + 0xc), v);
            dx = v[0] - *(float *)(*(char **)(gg + 0x34) + 0x4c);
            dy = v[1] - *(float *)(*(char **)(gg + 0x34) + 0x50);
            dz = v[2] - *(float *)(*(char **)(gg + 0x34) + 0x54);
            dist = (float)sqrt(dz * dz + dy * dy + dx * dx);
            if (dist < 100.0f) {
                n = ai_Unk4e52f0((unsigned char)r[6] - (unsigned char)r[5]);
                k = n + (unsigned char)r[5];
                d1 = (double)k * *(float *)(c + 0x1c) * 0.01;
                t1 = (int)d1;
                amt = (int)(t1 * (1.0 - dist * 0.01f));
                *(int *)(*pp + 0x24) -= amt;
                ai_Unk4cedc0(*pp, amt, *(int *)(*pp + 0x16c) == ai_Unk6f0cec ? 1 : 2);
                if (ai_Unk4d3390(c) == 0)
                    ch_QueueAnim(gg, "kampf/arm_greifen", 1);
            }
        }
        d3if_Delete((struct D3OBJECT *)*(int *)rec);
        *(int *)rec = 0;
    }
    return cnt;
}
/* 0x4ceb70 */

int ai_Unk4ceb70(char *c)
{
    return *(int *)(c + 0x200);
}
/* 0x4ceb80, waits for the event id to end, then stops it */

void ai_Unk4ceb80(int id)
{
    char *e;

    if (cm_SkipEvents != 0)
        return;
    if (sv_RunFrame(0, 0x7d6c6) != 0) {
        while (dm_L.rup == 0 && kb_Last == 0) {
            if (evt_FindById(id) == 0)
                return;
            if (sv_RunFrame(0, 0x7d6c6) == 0)
                break;
        }
    }
    e = (char *)evt_FindById(id);
    if (e != 0)
        evt_Stop(e);
    di_ClearButtons();
}
/* 0x4cec00, closes the 32 windows of ai_Unk935d20 */

void ai_Unk4cec00()
{
    int *p;

    for (p = ai_Unk935d20; (int)p < (int)(ai_Unk935d20 + 0x20); p++) {
        if (*p != -1) {
            d2_RemoveWindow(*p);
            *p = -1;
        }
    }
}
/* 0x4cec30, opens a window (text box 110+ wide) around the model box of the character c; Rough draft */

void ai_Unk4cec30(int a, char *c)
{
    int v[8];
    int x0, y0, w, h, i, hnd;

    if (d3if_ScreenBox(*(int *)(*(char **)(c + 0x184) + 0x34), v) == 0)
        return;
    x0 = v[4] - 0x10;
    w = v[6] - v[4] + 0x20;
    y0 = v[5] - 0x10;
    if (w <= 0x6e)
        w = 0x6e;
    h = v[7] - v[5] + 0x20;
    for (i = 0; i < 0x20; i++) {
        if (ai_Unk935d20[i] == -1)
            break;
    }
    if (i >= 0x20)
        return;
    ai_Unk4cdf00((char *)a, c);
    ai_Unk935d20[i] = d2_CreateWindow(x0, y0, w, h, 0x10);
    d2_WinOpenPic(0, 0, 0x7b0, d2_LastWindow);
    d2_WinOpenPic(w - 0x18, 0, 0x7b1, d2_LastWindow);
    hnd = d2_LastWindow;
    *(short *)(ai_Unkf8f39c + hnd * 0x3c4) = 0x54;
    sv_ValueBar(4, 4, gm_SkillPoints(c, 3), 0x4df);
    hnd = (int)(gm_HealthRatio(c) * 100.0);
    d2_WinOpenSlider(0, 0xe, 0x40, 0, 0x64, 0x75, 0x442, d2_LastWindow);
    d2_SetObjValue(d2_LastWindow, 0, 0x64, hnd);
}
/* 0x4ced80 */

void ai_Unk4ced80(int a)
{
    char *p;

    ai_Unk4cec00();
    for (p = ai_Unk930600; (int)p < (int)(ai_Unk930600 + 0x4380); p += 0x21c) {
        if (p[0x188] != 0)
            ai_Unk4cec30(a, p);
    }
}
/* 0x4cedc0, a floating number (v / (c's +0x1c * 0.01)) over the character c in a free slot of the 16 of
   ai_Unk9304c0 (kind 1 / 2 with signs, else plain) */

void ai_Unk4cedc0(char *c, int v, int kind)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (ai_Unk9304c0[i].v == 0)
            break;
    }
    if (i >= 16)
        return;
    ai_Unk9304c0[i].v = (int)((double)v / (*(float *)(c + 0x1c) * 0.01));
    ai_Unk9304c0[i].c = c;
    ai_Unk9304c0[i].t = 64.0f;
    ai_Unk9304c0[i].kind = kind;
}
/* 0x4cee20, shows / moves the floating numbers; one that rose for 128 steps is removed. Rough draft. */

void ai_Unk4cee20()
{
    char buf[0x40];
    int box[4];
    int i, x, y0;
    G18POP *p;

    for (i = 0, p = ai_Unk9304c0; (int)p < (int)(ai_Unk9304c0 + 16); i++, p++) {
        if (p->v == 0)
            continue;
        if (d3if_ScreenBox(*(int *)(*(char **)(p->c + 0x184) + 0x34), box) == 0) {
            if (ai_Unk9304c0[i].h != -1)
                d2_EnableObject(ai_Unk9304c0[i].h, 0);
            return;
        }
        x = box[0] + 0x10;
        y0 = box[1] - 0x30;
        if (p->h == -1) {
            if (p->kind == 1)
                sprintf(buf, "%c-%i%c", 0x5c, p->v, 0x5d);
            else if (p->kind == 2)
                sprintf(buf, "%c-%i%c", 0x5e, p->v, 0x5f);
            else
                sprintf(buf, "-%i", p->v);
            p->h = d2_CreateScreenText(x, (int)(y0 + p->t), buf);
        } else
            d2_MoveObj(x, (int)(y0 + p->t), p->h);
        if (p->h != -1)
            d2_EnableObject(p->h, 1);
        p->t = (float)(p->t - 0.5);
        if (p->t <= 0.0f) {
            d2_RemoveObj(p->h);
            p->v = 0;
            p->h = -1;
            p->c = 0;
        }
    }
}
/* 0x4cef80 */

void ai_Unk4cef80()
{
    G18POP *p;

    for (p = ai_Unk9304c0; (int)p < (int)(ai_Unk9304c0 + 16); p++) {
        p->h = -1;
        p->v = 0;
        p->c = 0;
    }
}
/* 0x4cefa0, removes all floating numbers */

void ai_Unk4cefa0()
{
    G18POP *p;

    for (p = ai_Unk9304c0; (int)p < (int)(ai_Unk9304c0 + 16); p++) {
        if (p->h != -1)
            d2_RemoveObj(p->h);
        p->h = -1;
        p->v = 0;
        p->c = 0;
    }
}
/* 0x4cefe0 */

void ai_Unk4cefe0()
{
    if (ai_Unk60cb60 != -1) {
        d2_CloseView(ai_Unk60cb60);
        ai_Unk60cb60 = -1;
    }
}
/* 0x4cf000, the per-tick update of the characters (ai_SceneChars[]): the counter byte +8 of each counts down (when
   the clock of the player record ai_Unk6f0d74 matches, every 0x15e ticks), the record 0x172's characters are
   shaded by their distance to the others (rest 255), characters at a cutscene slot turn towards their partner;
   ends with the floating numbers (0x4cee20). Rough draft. */

void ai_Unk4cf000()
{
    char **pp, **qq;
    char *c, *d, *r, *mdl, *e, *o, *g;
    int *rr;
    int fl, found, n, v, t;
    unsigned int col;
    float best, dist, ang;
    unsigned char b;

    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && d3_Time % 0x15e == 0) {
        for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            c = *pp;
            if (c == 0)
                continue;
            b = c[8];
            if (b > 4 || b <= 0)
                continue;
            b--;
            c[8] = b;
            if (b == 1) {
                c[8] = 100;
                continue;
            }
            r = ai_FindWeaponRecord(0x16e);
            n = ai_Unk4e52f0((unsigned char)r[6] - (unsigned char)r[5]);
            v = (int)((double)(unsigned int)(n + (unsigned char)r[5]) * *(float *)(c + 0x1c) * 0.01);
            ai_Unk4c5d00(*(int *)(c + 4), v, (unsigned char)c[8]);
        }
    }
    for (pp = (char **)ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        c = *pp;
        if (c == 0)
            continue;
        rr = (int *)ai_Unk4cda50(c);
        if (rr != 0 && *rr == 0x172) {
            fl = 0;
            if (ai_Unk6f0d74 != 0)
                fl = ai_LedSelectBits(*(unsigned short *)ai_Unk6f0d74, c);
            if (fl & 0x800) {
                col = 0x80;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                d3if_ShowTree(mdl, 1);
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                mdl[0x212] &= 0xfd;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                ((void (*)(void *, void *, int))d3_SetObjectAlpha)(mdl, (void *)(mdl + 0x1cc), col);
            } else {
                best = 999999.0f;
                found = 0;
                for (qq = (char **)ai_Unk7603a0V; (int)qq < (int)ai_Unk760420V; qq++) {
                    d = *qq;
                    if (d != 0 && ai_Unk4ce270(c, d) != 0 && d[8] != 0) {
                        dist = ai_Unk4ce1b0(c, *(char **)(*(char **)(d + 0x184) + 0x34));
                        if (dist < 100.0f) {
                            found = 1;
                            if (dist < best)
                                best = dist;
                        }
                    }
                }
                d3if_ShowTree(*(char **)(*(char **)(c + 0x184) + 0x34), found);
                if (found == 0)
                    goto next;
                col = 0;
                t = (int)(255.0f - best * 0.01f * 255.0f);
                col = (unsigned char)t;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                mdl[0x212] &= 0xfd;
                mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
                ((void (*)(void *, void *, int))d3_SetObjectAlpha)(mdl, (void *)(mdl + 0x1cc), col);
            }
            mdl = *(char **)(*(char **)(c + 0x184) + 0x34);
            mdl[0x212] |= 2;
        }
next:
        rr = (int *)ai_Unk4cdf80(c);
        if (rr != 0 && rr[1] == 2 && *((char *)rr + 0x1d) != 0) {
            o = ai_Unk4ce130(rr[4]);
            if (o != 0) {
                if (d3_VecEqual((float *)(*(char **)(*(char **)(o + 0x184) + 0x34) + 0x4c),
                                 (float *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c), 10.0f) == 0) {
                    ang = d3_YawTo(*(char **)(*(char **)(c + 0x184) + 0x34), *(char **)(*(char **)(o + 0x184) + 0x34) + 0x4c);
                    g = *(char **)(c + 0x184);
                    if (*(char **)(g + 0x128) == 0 || ((*(char **)(g + 0x128))[9] != 0x2d && (*(char **)(g + 0x128))[9] != 0x3a)) {
                        if (fabs(ang) <= 0.08) {
                            mdl = *(char **)(g + 0x34);
                            d3_SetObjectRotXYZ(mdl, *(int *)(mdl + 0x84), *(float *)(mdl + 0x88) + ang * 0.25, *(int *)(mdl + 0x8c));
                        }
                    }
                }
            }
        }
    }
    ai_Unk4cee20();
}
/* 0x4cf370, draws the scroll arrows (sprite ids 0 .. 7 of the sheet d2_GetPic(0x52c)): the mouse hit tests against
   the boxes at sv_CameraObj + 0xb4 / 0xcc / 0xe4 / 0xfc pick the pressed (2) or the free (1) look. Rough draft. */

void cm_DrawScrollArrows()
{
    int e = 0, up = 0, a = 0, b = 0;
    int t;

    t = d2_GetPic(0x52c);
    if (sv_BorderViewX == -1) {
        up = 1;
        if (d3_VecEqual((float *)(sv_CameraObj + 0xfc), ai_Unk5ebba4, 0.2f) == 0)
            up = 2;
    } else if (sv_BorderViewX == 1) {
        e = 1;
        if (d3_VecEqual((float *)(sv_CameraObj + 0xe4), ai_Unk5ebba4, 0.2f) == 0)
            e = 2;
    } else if (sv_BorderViewX == 0) {
        if (d3_VecEqual((float *)(sv_CameraObj + 0xe4), ai_Unk5ebba4, 0.2f) == 0)
            e = 1;
        if (d3_VecEqual((float *)(sv_CameraObj + 0xfc), ai_Unk5ebba4, 0.2f) == 0)
            up = 1;
    }
    if (sv_BorderViewY == -1) {
        a = 1;
        if (d3_VecEqual((float *)(sv_CameraObj + 0xb4), ai_Unk5ebba4, 0.2f) == 0)
            a = 2;
    } else if (sv_BorderViewY == 1) {
        b = 1;
        if (d3_VecEqual((float *)(sv_CameraObj + 0xcc), ai_Unk5ebba4, 0.2f) == 0)
            b = 2;
    } else if (sv_BorderViewY == 0) {
        if (d3_VecEqual((float *)(sv_CameraObj + 0xb4), ai_Unk5ebba4, 0.2f) == 0)
            a = 1;
        if (d3_VecEqual((float *)(sv_CameraObj + 0xcc), ai_Unk5ebba4, 0.2f) == 0)
            b = 1;
    }
    d2_SetClipRect(0, 0, *(int *)((char *)d2_ScreenSurf + 4), *(int *)((char *)d2_ScreenSurf + 8));
    if (a == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * ai_Unk5edc50 - ai_Unk5eb81c), d2_ScreenSurf, t, 0);
    else if (a == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * ai_Unk5edc50 - ai_Unk5eb81c), d2_ScreenSurf, t, a);
    if (e == 2)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, e);
    else if (e == 1)
        d2_DrawShape(8, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 3);
    if (up == 2)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 4);
    else if (up == 1)
        d2_DrawShape(d2_ScreenW - 0x28, d2_ScreenH / 2 + 0x14, d2_ScreenSurf, t, 5);
    if (b == 2)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * ai_Unk5eb7fc), d2_ScreenSurf, t, 6);
    else if (b == 1)
        d2_DrawShape(d2_ScreenW / 2 - 0x14, (int)(d2_ScreenScale * ai_Unk5eb7fc), d2_ScreenSurf, t, 7);
}
/* 0x4cf670, the mouse pick: the object under the cursor (ai_PickedObject, from the scene, else the ground point) and the
   cursor look d2_PointerLook by the selected characters' kind (0x160 -> 0x18 .. 0x176 -> 0x25; more than one selected: 0x23;
   "sp_ESCAPE" 0x1c, "sp_CONQUER" 0x1b, "WARE" 0xe). Rough draft. */

void ai_Unk4cf670()
{
    int n = 0, cur = 0x23;
    char *sc, *c, *r, **pp;
    int px, py;
    float v[3];
    float h;

    sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    ai_PickedObject = 0;
    d2_PointerLook = 0;
    if (dm_L.x > 0 && dm_L.x < d2_ScreenW && dm_L.y > 0 && dm_L.y < d2_ScreenH &&
        d2_PointerObj == -1) {
        ai_PickedObject = d3if_Pick((float)dm_L.x, (float)dm_L.y, 0x40, 1);
        if (ai_PickedObject == 0 && ai_Unk6f0d08 != 0) {
            if ((d3sm_PickK(sc, dm_L.x + 0x1a, dm_L.y + 0x1a, &py, &px) & 0xff) != -1) {
                d3sm_GetVertexI(sc, py, px, v);
                h = v[1] + ai_Unk5ebb68;
                d2_PointerLook = 0;
            }
        } else if (ai_Unk6f0cf0 != 0) {
            d3if_Show(ai_Unk6f0cf0, 0);
        }
    }
    if (ai_PickedObject == 0) {
        ai_Unk6f0ce0 = 0;
        ai_PickedChar = 0;
        return;
    }
    for (pp = ai_Unk7603a0V; (int)pp < ((unsigned)&ai_SceneChars + 0x7c); pp++) {
        c = *pp;
        if (c != 0 && c[0x188] != 0) {
            if (ai_LedSelectBits(*(unsigned short *)ai_Unk6f0d74, c) & 0x800) {
                if (kb_Down[0x36] == 0) {
                    r = ai_Unk4cda50(c);
                    if (r != 0) {
                        switch (*(int *)r) {
                        case 0x160: cur = 0x18; break;
                        case 0x174: cur = 0x19; break;
                        case 0x16e: cur = 0x1e; break;
                        case 0x154: cur = 0x1a; break;
                        case 0x158: cur = 0x1f; break;
                        case 0x15e: cur = 0x21; break;
                        case 0x156: cur = 0x23; break;
                        case 0x176: cur = 0x25; break;
                        }
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
            ai_Unk6f0ce0 = ai_PickedObject;
            return;
        }
        if (strncmp(ai_PickedObject, "sp_CONQUER", 10) == 0 && n != 0) {
            d2_PointerLook = 0x1b;
            ai_Unk6f0ce0 = ai_PickedObject;
            return;
        }
        if (strncmp(ai_PickedObject, "WARE", 4) == 0 && n != 0) {
            d2_PointerLook = 0xe;
            ai_Unk6f0ce0 = ai_PickedObject;
            return;
        }
        if (*(int *)(ai_PickedObject + 0x220) == 2 && n != 0) {
            if (!(ai_LedSelectBits(*(unsigned short *)ai_Unk6f0d74, *(int *)(ai_PickedObject + 0x200)) & 0x800) ||
                kb_Down[0x36] != 0) {
                ai_Unk6f0ce0 = ai_PickedObject;
                d2_PointerLook = cur;
                return;
            }
        } else
            d2_PointerLook = 0;
        ai_Unk6f0ce0 = ai_PickedObject;
        return;
    }
    d2_PointerLook = 0;
}

#define G19_E(e, o) (*(int *)((e) + (o)))
#define G19_5176D0 ((void (*)(char *, char *, ...))txt_Format)

/* 0x4cf9c0, the portrait boxes ai_Unk930400[6] of 0x20 bytes (+0 scene, +4 / +8 window objects, +0x10 text, +0x14 picture,
   +0x18 second text, +0x1c the character): shown for the selected characters, with the load (x 100 as "%1N1 %i%"), the
   picture of the job and its count. Rough draft. */
void ai_Unk4cf9c0()
{
    char buf[0x100];
    char *e, *c, *o, *p;
    int t;

    if (d2_ClickedId != -1) {
        for (e = ai_Unk930400; e < ai_Unk930400 + 0xc0; e += 0x20) {
            if (G19_E(e, 8) != -1 && d2_PointerObj == G19_E(e, 0)) {
                if (*(char **)(d2_ObjsC + G19_E(e, 0) * 0x2ec + 0x28) != 0)
                    *(char *)(*(char **)(e + 0x1c) + 0x188) = 1;
                else
                    *(char *)(*(char **)(e + 0x1c) + 0x188) = 0;
            }
        }
    }
    d2_ClearPointerItems();
    ai_Unk6f0d08 = 0;
    for (e = ai_Unk930400; e < ai_Unk930400 + 0xc0; e += 0x20) {
        if (G19_E(e, 8) == -1)
            continue;
        c = *(char **)(e + 0x1c);
        if (*(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) == 1) {
            d2_EnableObject(*(int *)(ai_Unkf8f39c - 0x10 + G19_E(e, 8) * 0x3c4), 0);
            d2_EnableObject(*(int *)(ai_Unkf8f39c - 0x10 + G19_E(e, 4) * 0x3c4), 0);
            continue;
        }
        if (c[0x188] != 0) {
            d2_AddPointerItem(*(int *)(c + 0x18c), 1);
            d2_SetObjValue(G19_E(e, 0), 1);
            ai_Unk6f0d08++;
        } else
            d2_SetObjValue(G19_E(e, 0), 0);
        t = (int)(gm_HealthRatio(c) * 100.0);
        G19_5176D0(buf, "%1N1 %i%", *(unsigned short *)c, t);
        d2_SetObjText(G19_E(e, 0x10), buf);
        o = ai_Unk4cda00(c);
        if (o == 0 || *(int *)o == 0)
            continue;
        if (G19_E(e, 0x14) != -1) {
            if (*(int *)(d2_ObjsC + G19_E(e, 0x14) * 0x2ec + 8) != *(int *)o + 0xdf) {
                if (G19_E(e, 0x14) != -1)
                    d2_RemoveObj(G19_E(e, 0x14));
                d2_LastWindow = G19_E(e, 8);
                G19_E(e, 0x14) = d2_WinOpenPic(0x54, 0xf, *(int *)o + 0xdf, G19_E(e, 8));
                d2_ScaleObject(G19_E(e, 0x14), -2);
            }
        } else {
            d2_LastWindow = G19_E(e, 8);
            G19_E(e, 0x14) = d2_WinOpenPic(0x54, 0xf, *(int *)o + 0xdf, G19_E(e, 8));
            d2_ScaleObject(G19_E(e, 0x14), -2);
        }
        if (G19_E(e, 0x18) == -1)
            continue;
        if (*(int *)(o + 0x58) == 1 || *(int *)(o + 0x58) == 2) {
            p = ai_Unk4cdac0(c);
            if (p != 0) {
                sprintf(buf, "%i", *(int *)(p + 0x20));
                d2_SetObjText(G19_E(e, 0x18), buf);
                continue;
            }
        }
        if (G19_E(e, 0x18) != -1)
            d2_SetObjText(G19_E(e, 0x18), &ch_EmptyName);
    }
}

#define G19_R(id, o, t) (*(t *)(d2_ObjsC + (id) * 0x2ec + (o)))

/* 0x4cfc10, builds the portrait boxes of ai_Unk930400[6] (see 0x4cf9c0) for the selected characters ai_SceneChars[]:
   window objects, the picture of the job, the texts, 0x80 pixels apart. Rough draft. */
void ai_Unk4cfc10()
{
    char *e, *o, **pp;
    int x = 0, t;

    for (e = ai_Unk930400; e < ai_Unk930400 + 0xc0; e += 0x20) {
        G19_E(e, 8) = -1;
        G19_E(e, 0) = -1;
        G19_E(e, 0xc) = -1;
        G19_E(e, 0x10) = -1;
        G19_E(e, 0x18) = -1;
        G19_E(e, 0x14) = -1;
    }
    e = ai_Unk930400;
    for (pp = ai_Unk7603a0V; e < ai_Unk930400 + 0xc0 && (int)pp < ((unsigned)&ai_SceneChars + 0x7c); pp++) {
        if (*pp == 0)
            continue;
        if (!(ai_LedSelectBits(*(unsigned short *)ai_Unk6f0d74, *pp) & 0x800))
            continue;
        G19_E(e, 4) = d2_CreatePicObj(x + 0x18, 0xf, 0x5da);
        G19_E(e, 8) = d2_CreateWindow(x, 0, 0x80, 0x4b, 0x110);
        o = ai_Unk4cda50(*pp);
        if (o != 0 && *(int *)o != 0) {
            G19_E(e, 0x14) = d2_WinOpenPic(0x54, 0xf, *(int *)o + 0xdf, d2_LastWindow);
            d2_ScaleObject(G19_E(e, 0x14), -2);
            G19_E(e, 0x18) = d2_CreateWinText(0x54, 0x26, &ch_EmptyName, d2_LastWindow);
            G19_R(G19_E(e, 0x18), 0x74, short) = 0x54;
            G19_R(G19_E(e, 0x18), 0x14, short) = 0x18;
            G19_R(G19_E(e, 0x18), 0x5c, int) = 1;
        }
        G19_E(e, 0x1c) = (int)*pp;
        *(short *)(ai_Unkf8f39c - 0x268 + G19_E(e, 8) * 0x3c4) = 0;
        *(short *)(ai_Unkf8f39c + G19_E(e, 8) * 0x3c4) = 0x54;
        G19_E(e, 0) = d2_WinOpenPic(0x28, 0x11, *(int *)(*pp + 0x18c), d2_LastWindow);
        G19_R(G19_E(e, 0), 0x48, int) = 1;
        G19_E(e, 0x10) = d2_CreateWinText(0, 0, &ch_EmptyName, d2_LastWindow);
        G19_R(G19_E(e, 0x10), 0x74, short) = 0x54;
        G19_R(G19_E(e, 0x10), 0x14, short) = 0x80;
        G19_R(G19_E(e, 0x10), 0x5c, int) = 1;
        t = gm_SkillPoints();
        sv_ValueBar(0x24, 0x41, (int)(double)t, 0x4df);
        x += 0x80;
        e += 0x20;
    }
    ai_Unk4cf9c0();
}
/* 0x4cfe70, removes the portrait boxes ai_Unk930400[6] again */

void ai_Unk4cfe70()
{
    int *p = (int *)(ai_Unk930400 + 8);
    int t;

    do {
        if (p[0] != -1) {
            d2_RemoveWindow(p[0]);
            t = p[-1];
            p[0] = -1;
            d2_RemoveObj(t);
        }
        p += 8;
    } while ((int)p < (int)((unsigned)&ai_Unk930400 + 0xc8));
}
/* 0x4cfeb0, the click on an own object of the pick ai_PickedObject (type 2) while characters are selected: the first selected
   character that stands still gets the comment "_ARBEITER_KOMMENTARE_KLICK", the object is marked (+0x188). Rough draft. */

void ai_Unk4cfeb0()
{
    char *t = ai_Unk6f0d74;
    char *c, *sel, *o, **pp;

    ai_ClickedObject = 0;
    for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        c = *pp;
        if (c != 0 && c[0x188] != 0 && (ai_LedSelectBits(*(unsigned short *)t, c) & 0x800) && *(int *)(c + 0x184) != 0 &&
            c[8] != 0)
            break;
    }
    if (dm_L.lup != 0 && d2_PointerObj == -1) {
        sel = ai_PickedObject;
        if (sel != 0 && *(int *)(sel + 0x220) == 2) {
            o = *(char **)(sel + 0x200);
            if ((ai_LedSelectBits(*(unsigned short *)t, o) & 0x800) && kb_Down[0x36] == 0 &&
                (c == 0 || ai_Unk4ce270(c, o) == 0)) {
                ai_Unk4d08c0();
                o[0x188] = 1;
                if (*(int *)(o + 0x16c) == ai_Unk6f0cec)
                    snd_QueueVoice(*(unsigned short *)o, ai_Unk6f0d24, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                else
                    snd_QueueVoice(*(unsigned short *)o, ai_Unk6f0d28, "_ARBEITER_KOMMENTARE_KLICK", -1, 0);
                sel = ai_PickedObject;
            }
        }
        ai_ClickedObject = (int)sel;
    }
}
/* 0x4cffd0, an order for the character c of the player record rec to the object / ground point (tgt, x, z): the order
   record (0x2c bytes, the one of 4cdf00 copied; L[1] = 2 the order kind, L[4..6] target id / x / z, byte +0x1c a flag)
   is passed to 0x4c5c40; characters of the kinds 0x15e .. 0x176 need a worker count in 4cdac0 (else the message
   ai_Unk143e8a4 .. a8 / ac). Rough draft: 0x45f170 writes through the addresses of the parameters in the original. */

int ai_Unk4cffd0(char *rec, char *c, char *tgt, int x, int z)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *k, *q, *r;
    int L[11];
    int h;

    if (c == 0)
        return -1;
    k = (char *)ai_Unk4cda50(c);
    if (tgt == 0 || *(char **)(*(char **)(tgt + 0x184) + 0x34) == 0)
        return -1;
    if ((char)d3_GetHeightAt(sc, *(char **)(*(char **)(tgt + 0x184) + 0x34) + 0x4c, &x, &z, &h) == 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    if (q == 0)
        return -1;
    q[0x1c] = 0;
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    if (k == 0 || *(int *)k == 0x154 || *(int *)k == 0x156 || *(int *)k == 0x158 || *(int *)k == 0x16e ||
        *(int *)k == 0x172 || *(int *)k == 0) {
        L[4] = *(int *)(tgt + 4);
        L[0] = *(int *)(c + 4);
        L[1] = 2;
    } else if (*(int *)k == 0x15e || *(int *)k == 0x160) {
        r = ai_Unk4cdac0(c);
        if (r == 0 || *(int *)(r + 0x20) <= 0) {
            sv_SetStatusText((char *)ai_Unk143e8a4);
            return -1;
        }
        L[0] = *(int *)(c + 4);
        L[4] = *(int *)(tgt + 4);
        L[5] = x;
        L[6] = z;
        L[1] = 2;
    } else if (*(int *)k == 0x174) {
        r = ai_Unk4cdac0(c);
        if (r == 0 || *(int *)(r + 0x20) <= 0) {
            sv_SetStatusText((char *)ai_Unk143e8a8);
            return -1;
        }
        L[0] = *(int *)(c + 4);
        L[4] = 0;
        L[5] = x;
        L[6] = z;
        L[1] = 2;
    } else if (*(int *)k == 0x176) {
        r = ai_Unk4cdac0(c);
        if (r == 0 || *(int *)(r + 0x20) <= 0) {
            sv_SetStatusText((char *)ai_Unk143e8ac);
            return -1;
        }
        L[4] = 0;
        L[5] = x;
        L[6] = z;
        L[0] = *(int *)(c + 4);
        L[1] = 2;
    }
    if (c[0x1c4] == 1 || c[0x1c4] == 2 || c[0x1c4] == 3)
        ((char *)L)[0x1c] = 1;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
/* 0x4d0200, like 0x4cffd0, the order kind 3 with the ground point of tgt (record 4cdf00 copied; L[0] the character id,
   L[4] / L[5] the arguments c / tgt as they are in the original). Rough draft. */

int ai_Unk4d0200(char *rec, char *c, char *tgt)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    float v[3];
    int L[11];
    int a, b, h;

    if (tgt == 0 || c == 0 || rec == 0)
        return -1;
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &a, &b, &h) == 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    if (q == 0)
        return -1;
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[4] = (int)tgt;
    L[0] = *(int *)(c + 4);
    L[1] = 3;
    L[5] = (int)c;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
/* 0x4d02d0, an order kind 4 for the character c with the name of the object tgt (strcpy to +0x10) and its ground
   point (x, z); returns 1, 0 when the point is not on the ground. Rough draft. */

int ai_Unk4d02d0(char *rec, char *c, char *tgt)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    float v[4];
    int x, z, h;

    ai_Unk4cda50(c);
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &x, &z, &h) == 0)
        return 0;
    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    strcpy(q + 0x10, tgt);
    *(int *)q = *(int *)(c + 4);
    *(int *)(q + 4) = 4;
    *(int *)(q + 0x20) = x;
    *(int *)(q + 0x24) = z;
    return 1;
}
/* 0x4d0380, an order kind 6 with the name of the object tgt, copied to +0x10 of the order record that goes to 0x4c5c40;
   -1 when the point is not on the ground or the character c has a job (+0x1ac). Rough draft. */

int ai_Unk4d0380(char *rec, char *c, char *tgt, unsigned char a)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    float v[4];
    int L[11];
    int x, z, h;

    ai_Unk4cda50(c);
    d3_PointToWorld(tgt, tgt + 0x4c, v);
    if ((char)d3_GetHeightAt(sc, v, &x, &z, &h) == 0 || *(int *)(c + 0x1ac) != 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[0] = *(int *)(c + 4);
    L[1] = 6;
    strcpy((char *)&L[4], tgt);
    ((char *)L)[0x1c] = a;
    ((char *)L)[0x1d] = ((char *)v)[0];
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
/* 0x4d0460, an order kind 7 to the ground point of the character c (4d2170 checks the way): L[4] = the argument,
   L[5] a float of the lookup; -1 when the point is not on the ground. Rough draft (0x45f170 / 0x4d2170 write through the
   addresses of the parameters in the original). */

int ai_Unk4d0460(char *rec, char *c, int a)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *q;
    int L[11];
    int x, z;

    if ((char)d3_GetHeightAt(sc, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, &a, &x, &z) == 0)
        return -1;
    if (ai_Unk4d2170(c, z, x, 8, &a, &z) == 0)
        return -1;
    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[0] = *(int *)(c + 4);
    L[4] = a;
    L[1] = 7;
    L[5] = z;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}
/* 0x4d0520, an order kind 8 for the character c */

void ai_Unk4d0520(char *rec, char *c)
{
    char *q;
    int L[11];

    q = (char *)ai_Unk4cdf00(rec, c);
    q[0xc] = 0;
    memcpy(L, q, 0x2c);
    L[0] = *(int *)(c + 4);
    L[1] = 8;
    ai_Unk4c5c40(*(int *)(rec + 0x10), L);
}

#define G19_4D0380 ((int (*)(char *, char *, char *))ai_Unk4d0380)

/* 0x4d0570, the order of the right click for the character c of the player record rec to the picked object
   ai_ClickedObject (type 2: 0x4cffd0; "sp_ESCAPE" 0x4d0200; "sp_CONQUER" 0x4d02d0; "WARE" 0x4d0380) or, without a pick, to
   the ground point under the cursor (kind 1); returns 1 when an order was given. Rough draft. */
char ai_Unk4d0570(char *rec, char *c)
{
    char ret = 0;
    char *sel, *o, *q;
    int L[11];
    int x, z;

    if (dm_L.lup != 0) {
        sel = (char *)ai_ClickedObject;
        if (sel != 0) {
            if (*(int *)(sel + 0x220) == 2) {
                o = *(char **)(sel + 0x200);
                if (ai_Unk4ce270(c, o) != 0 || kb_Down[0x36] != 0) {
                    if (ai_Unk4cffd0(rec, c, o, 0, 0) != -1)
                        ret = 1;
                }
            } else if (strcmp(sel, "sp_ESCAPE") == 0) {
                ai_Unk4d0200(rec, c, sel);
                ret = 1;
            } else if (strncmp(sel, "sp_CONQUER", 10) == 0) {
                ai_Unk4d02d0(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            } else if (strncmp((char *)ai_ClickedObject, "WARE", 4) == 0) {
                G19_4D0380(rec, c, (char *)ai_ClickedObject);
                ret = 1;
            }
        } else if ((char)d3sm_PickK(*(char **)(ai_Unk5fb5b0 + 0xb0), dm_L.x + 0x1a, dm_L.y + 0x1a, &x, &z)) {
            q = (char *)ai_Unk4cdf00(rec, c);
            q[0xc] = 0;
            memcpy(L, q, 0x2c);
            L[0] = *(int *)(c + 4);
            L[4] = x;
            L[1] = 1;
            L[5] = z;
            ai_Unk4c5c40(*(int *)(rec + 0x10), L);
            return 1;
        } else
            sv_SetStatusText((char *)ai_Unk143e8b0);
    }
    return ret;
}
/* 0x4d0740, the per-tick command handling for the player record rec: the click orders of the selected characters
   (0x4d0570, then the comment "_K_C2C_BEFEHL_ERHALTEN" and the mark +0x188 is cleared), the highlight of the selected
   characters' models (bit 2 of +0x212 as the mark). Rough draft. */

void ai_Unk4d0740(char *rec)
{
    char *c, *m, *mdl, **pp;

    if (dm_L.lhold == 0 && sv_BandActive != 0)
        sv_EndBand();
    if (dm_L.lhold != 0 && dm_L.rdown == 0) {
        if (sv_BandActive == 0 && di_Absolute != 0 && d2_PointerObj == -1) {
            ai_Unk4d08c0();
            sv_StartBand();
        }
    } else if (dm_L.lup != 0) {
        for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            c = *pp;
            if (c != 0 && c[0x188] != 0 && (ai_LedSelectBits(*(unsigned short *)ai_Unk6f0d74, c) & 0x800) &&
                *(int *)(c + 0x184) != 0 && c[8] != 0) {
                if (ai_Unk4d0570(rec, c)) {
                    if (ai_Unk6f0d74 == (char *)ai_Unk6f0cec)
                        snd_PlayVoice(*(unsigned short *)c, (int)ai_Unk6f0d34, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    else
                        snd_PlayVoice(*(unsigned short *)c, (int)ai_Unk6f0d38, "_K_C2C_BEFEHL_ERHALTEN", -1);
                    c[0x188] = 0;
                }
            }
        }
    }
    for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        c = *pp;
        if (c != 0) {
            m = *(char **)(c + 0x184);
            if (!(m[0x8c] & 0x40)) {
                mdl = *(char **)(m + 0x34);
                if (c[0x188]) {
                    if (!(mdl[0x212] & 2))
                        d3t_SetGreyK(mdl, 1);
                } else if ((mdl[0x212] & 2) == 2)
                    d3t_SetGreyK(mdl, 0);
            }
        }
    }
    if (dm_L.rup != 0)
        ai_Unk4d08c0();
}
/* 0x4d08c0, clears the marks of the 32 records ai_Unk930600[0x21c] (+0x188) and the highlight of their models */

void ai_Unk4d08c0()
{
    char *r;

    for (r = ai_Unk930600; (int)(r + 0x184) < ((unsigned)&ai_Unk930600 + 0x4504); r += 0x21c) {
        if (r[0x188] != 0)
            r[0x188] = 0;
        if (*(int *)(r + 0x184) != 0) {
            char *mdl = *(char **)(*(char **)(r + 0x184) + 0x34);

            if ((mdl[0x212] & 2) == 2)
                d3t_SetColorK(mdl, 0, 0, 0);
        }
    }
    d2_ClearPointerItems();
    ai_Unk4cec00();
    ai_Unk4cefe0();
}
/* 0x4d0920, a record of the 32 ai_Unk930600[0x21c] for the character c: the first free one (+0 == 0xffff) gets a copy of
   the template of 0x505df0 (0x87 ints), an object (4fbb80) with the name "%i" of its id and the speed from the
   character's kind; the kind 2 gets the count 6, else 0xf. Rough draft. */

char *ai_Unk4d0920(char *c, int a)
{
    char *t, *r, *k, *w;
    int i;

    t = gm_FindChar((int)c);
    for (i = 0, r = ai_Unk930600; *(unsigned short *)r != 0xffff; i++, r += 0x21c) {
        if ((int)(r + 0x21c) >= ((unsigned)&ai_Unk930600 + 0x4380))
            return 0;
    }
    memcpy(r, t, 0x21c);
    *(int *)(r + 0x1ac) = 0;
    *(int *)(r + 0x184) = 0;
    gm_CreateCharModel(r, d3_CurrentScene, 0, 0, 0, a);
    *(char *)(*(char **)(r + 0x184) + 0x240) = 4;
    *(char *)(*(char **)(r + 0x184) + 0x8d) |= 0x80;
    cm_UpdateHandObject(r);
    k = ai_Unk4cda50(c);
    *(float *)(*(char **)(r + 0x184) + 0x1a0) = (float)(unsigned char)k[9] * ai_Unk5eb9b8;
    sprintf(*(char **)(*(char **)(r + 0x184) + 0x34), "%i", *(int *)(r + 4));
    w = ai_Unk4cdac0(c);
    if (w != 0) {
        if (*(int *)(k + 0x58) == 2)
            *(int *)(w + 0x20) = 6;
        else
            *(int *)(w + 0x20) = 0xf;
    }
    return r;
}
/* 0x4d0a30, fills ai_SceneChars[] (32 pointers) with the records of ai_Unk930600 whose object belongs to the scene ai_Unk5fb5b0 */

void ai_Unk4d0a30()
{
    char **p;
    char *r;

    memset(ai_Unk7603a0V, 0, 0x80);
    p = ai_Unk7603a0V;
    for (r = ai_Unk930600; (int)p < ((unsigned)&ai_SceneChars + 0x7c) && (int)(r + 0x184) < ((unsigned)&ai_Unk930600 + 0x4504); r += 0x21c) {
        if (*(int *)(r + 0x184) != 0 && *(char **)(*(char **)(r + 0x184) + 0x88) == ai_Unk5fb5b0)
            *p++ = r;
    }
}
/* 0x4d0a80, a callback of the object walk: the dummies "dummy_sp_ESCAPE" get a flag object "ub_FLUCHTFLAGGE" (named
   "sp_ESCAPE", the animation "anim_FLUCHTFLAGGE.baf"), "dummy_CONQUER" one "ub_CONQUER" (named "sp_CONQUER_<n>"), both
   kept in ai_Unk935980[ai_Unk6f0d58++]; returns 1. Rough draft. */

char ai_Unk4d0a80(char *o)
{
    char *r;
    float pos[4];
    float rot[3];

    if (strstr(o, "dummy_sp_ESCAPE") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(o, o + 0x4c, pos);
        r = (char *)d3_CreateObject(0, pos, rot, "ub_FLUCHTFLAGGE");
        ai_Unk935980[ai_Unk6f0d58] = r;
        strcpy(r, "sp_ESCAPE");
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x220) = 4;
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x224) = 1;
        ai_Unk935980[ai_Unk6f0d58][0x212] = (ai_Unk935980[ai_Unk6f0d58][0x212] & 0xf7) | 4;
        ai_Unk935980[ai_Unk6f0d58][0x212] |= 0x40;
        d3t_PlayAnim(ai_Unk935980[ai_Unk6f0d58], "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
        ai_Unk6f0d58++;
        return 1;
    }
    if (strstr(o, "dummy_CONQUER") != 0) {
        rot[0] = 0;
        rot[1] = 0;
        rot[2] = 0;
        d3_PointToWorld(o, o + 0x4c, pos);
        r = (char *)d3_CreateObject(0, pos, rot, "ub_CONQUER");
        ai_Unk935980[ai_Unk6f0d58] = r;
        d3if_SelectTextureSet(r, *(int *)(r + 0x1cc), 1, 1);
        sprintf(ai_Unk935980[ai_Unk6f0d58], "sp_CONQUER_%i", ai_Unk6f0d58);
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x200) = 0;
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x220) = 4;
        *(int *)(ai_Unk935980[ai_Unk6f0d58] + 0x224) = 1;
        ai_Unk935980[ai_Unk6f0d58][0x212] = (ai_Unk935980[ai_Unk6f0d58][0x212] & 0xf7) | 4;
        ai_Unk6f0d58++;
    }
    return 1;
}
/* 0x4d0ca0, walks the objects of the scene with the callback 0x4d0a80 (the flag dummies) */

void ai_Unk4d0ca0()
{
    ai_Unk6f0d58 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0x100, ai_Unk4d0a80, 0);
}
/* 0x4d0cd0, the first free (-1) of 16 slots, else -1 */

int ai_Unk4d0cd0(int *a)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (a[i] == -1)
            return i;
    }
    return -1;
}

#define G19_GRANT(K, B)                                                                                 \
    i = ai_Unk4d0cd0((int *)(rec + 0x80));                                                              \
    *(int *)(rec + 0x80 + i * 4) = *(int *)(ai_KindBChars[i] + 4);                                       \
    ai_Unk4cdb20(ai_KindBChars[i]);                                                                      \
    cm_SendExSellObject(*(int *)(ai_KindBChars[i] + 4), -1, K, 1, ai_OwnPlayer, 0);                             \
    gm_AddHealth(*(unsigned short *)ai_KindBChars[i], -*(int *)(ai_KindBChars[i] + 0x24));                \
    buf[0] = B;                                                                                         \
    cm_BeginFieldBlock(ai_KindBChars[i], *(int *)(ai_KindBChars[i] + 4));                                       \
    cm_AddFieldDelta(1, 1, (unsigned short)(*(unsigned short *)&ai_KindBChars[i] - (unsigned short)cm_FieldObject + 0x83), buf)

/* 0x4d0cf0, the reward of the quest rec (+0x80 .. +0xbc the 16 ids already given, +0x110 the kind a): per kind one or
   two of the slots ai_KindBChars[] are taken, their characters get the property 0x156 (kind 1; 0x158 / 0x160 for 2 / 3)
   and the field block 0x83 + id is sent with the value 0xa8 / 0xd2. Rough draft. */
void ai_Unk4d0cf0(char *rec, unsigned char a)
{
    unsigned char buf[0x10];
    int i;

    if (a != 0)
        *(int *)(rec + 0x110) = a;
    if (a == 1) {
        G19_GRANT(0x156, 0xa8);
        cm_SendFieldDeltas();
        G19_GRANT(0x156, 0xa8);
        cm_SendFieldDeltas();
    } else if (a == 2 || a == 3) {
        G19_GRANT(0x158, 0xd2);
        cm_SendFieldDeltas();
        G19_GRANT(0x160, 0xd2);
        cm_SendFieldDeltas();
    }
}
/* 0x4d0f80, a callback: an object of kind 3 gets +0x224 = 0; returns 1 */

char ai_Unk4d0f80(char *o)
{
    if (*(int *)(o + 0x220) == 3)
        *(int *)(o + 0x224) = 0;
    return 1;
}

#define G19_SNDS(a, b, c, d, e, s)     a = snd_LoadVoiceBank(S_##s##1, 0);      b = snd_LoadVoiceBank(S_##s##2, 0);      c = snd_LoadVoiceBank(S_##s##3, 0);      d = snd_LoadVoiceBank(S_##s##4, 0);      e = snd_LoadVoiceBank(S_##s##5, 0)
#define S_DIEBE1 "ARBEITER_KOMMENTARE_KLICK_DIEBE.sbf"
#define S_DIEBE2 "K_C2C_ANGRIFF_DIEBE.sbf"
#define S_DIEBE3 "K_C2C_BEFEHL_ERHALTEN_DIEBE.sbf"
#define S_DIEBE4 "K_C2C_HOHN_DIEBE.sbf"
#define S_DIEBE5 "K_C2C_SCHMERZ_DIEBE.sbf"
#define S_RAEUBER1 "ARBEITER_KOMMENTARE_KLICK_RAEUBER.sbf"
#define S_RAEUBER2 "K_C2C_ANGRIFF_RAEUBER.sbf"
#define S_RAEUBER3 "K_C2C_BEFEHL_ERHALTEN_RAEUBER.sbf"
#define S_RAEUBER4 "K_C2C_HOHN_RAEUBER.sbf"
#define S_RAEUBER5 "K_C2C_SCHMERZ_RAEUBER.sbf"
#define S_SOELDNER1 "ARBEITER_KOMMENTARE_KLICK_SOELDNER.sbf"
#define S_SOELDNER2 "K_C2C_ANGRIFF_SOELDNER.sbf"
#define S_SOELDNER3 "K_C2C_BEFEHL_ERHALTEN_SOELDNER.sbf"
#define S_SOELDNER4 "K_C2C_HOHN_SOELDNER.sbf"
#define S_SOELDNER5 "K_C2C_SCHMERZ_SOELDNER.sbf"

/* 0x4d0fa0, sets up the fight scene for the fight record rec (+0x7c flags, +0x10 / +0x14 the attacker's and the
   defender's buildings, +0x34 / +0x38 the two sides, +0x80 / +0xc0 the 16 defender / attacker characters, +0x100 the three
   carts, +0x10c the building whose kind chooses the sounds): the scenario "Kampfszenario_<place>.ed3" by the flags or the
   building kind of the defender (0x4e2e60), the portrait / record tables cleared, the weapon sounds (kampf\*.sbf), the
   speech sets of both sides (types 0 mercenaries / 1 robbers / 2 thieves), the characters placed at the dummies
   "dummy_OFFENSE_ENTRY" / "dummy_DEFENSE_ENTRY" (0x4d0920), the carts "ob_KARREN_<size>_KAMPF.ogr" with their goods
   "WARE_0<n>", the border cells of the scene's map set to 0xd. Rough draft. */
void ai_Unk4d0fa0(char *rec)
{
    char buf[0x40], nm[0x40], path[0x40];
    char t[16];
    int q[4];
    char *sc, *b, *bl, *o, *a, *c, *h, *w, *r, *p, *k;
    char **pp;
    int nmax = 10;
    int i, j, n, kind, ty, wd;
    int *cell;
    unsigned char fl = rec[0x7c];

    if (fl & 1) {
        if (fl & 8)
            ai_Unk4e2e60("Kampfszenario_STADT.ed3");
        else
            ai_Unk4e2e60("Kampfszenario_BERGPASS.ed3");
    } else if (fl & 4) {
        ai_Unk4e2e60("Kampfszenario_BERGPASS.ed3");
    } else {
        bl = gm_BuildQuery(1, 1, *(int *)(rec + 0x14));
        k = &ai_BTypes[(unsigned char)bl[0] * 0x24d + (unsigned char)bl[0]];
        kind = (unsigned char)k[0];
        if (gm_IsHomeType(bl) != 0) {
            if (kind == 0x10) {
                ai_Unk4e2e60("Kampfszenario_RAEUBERLAGER.ed3");
                nmax = 15;
            } else if (kind == 0xc)
                ai_Unk4e2e60("Kampfszenario_STEINBRUCH.ed3");
            else if (kind == 0xb)
                ai_Unk4e2e60("Kampfszenario_WALDSTUECK.ed3");
            else if (kind == 0xd)
                ai_Unk4e2e60("Kampfszenario_MINE.ed3");
        } else {
            if (kind == 4) {
                if (k[0x247] == 1)
                    ai_Unk4e2e60("Kampfszenario_SCHMUGGLERLOCH.ed3");
                else if (k[0x247] == 2)
                    ai_Unk4e2e60("Kampfszenario_UNTERSCHLUPF.ed3");
                else if (k[0x247] == 3)
                    ai_Unk4e2e60("Kampfszenario_DIEBESGILDE.ed3");
            } else if (kind == 0x13) {
                if (k[0x247] == 1)
                    ai_Unk4e2e60("Kampfszenario_STADTWACHE_KLEIN.ed3");
                else if (k[0x247] == 2)
                    ai_Unk4e2e60("Kampfszenario_STADTWACHE_MITTEL.ed3");
                else if (k[0x247] == 3)
                    ai_Unk4e2e60("Kampfszenario_STADTWACHE_GROSS.ed3");
            }
        }
    }
    sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    ai_Unk4e3650();
    if (ai_Unk633738 != 0) {
        *(int *)(ai_Unkfd7c50 + d3_CurrentScene * 0x3e0 + 8) = *(int *)(ai_Unkfd7c50 + 8);
        ai_Unk633dd4 = *(int *)(ai_Unkfd7c50 + 8);
        sv_UpdateWeather();
        d3sky_Init(ai_Unk633dd4, 1);
    }
    memset(ai_Unk935d20, -1, 0x80);
    for (p = (char *)ai_Unk9304c0 + 4; (int)p < ((unsigned)((unsigned)ai_Unk9304c0 + 0x140) + 0x4); p += 0x14) {
        *(int *)(p + 4) = -1;
        *(int *)p = 0;
        *(int *)(p + 8) = 0;
    }
    memset((char *)((unsigned)&ai_Unk930600 + 0x4380), 0, 0x1000);
    memset(cm_Bombs, 0, 0x100);
    memset(ai_Unk935a00, 0, 0x200);
    memset(ai_Unk930600, 0, 0x4380);
    for (p = ai_Unk930600; (int)p < ((unsigned)&ai_Unk930600 + 0x4380); p += 0x21c)
        *(unsigned short *)p = 0xffff;
    for (p = ai_Unk930400 + 8; (int)p < ((unsigned)((unsigned)ai_Unk930400 + 0xc0) + 0x8); p += 0x20)
        *(int *)p = -1;
    ai_Unk6f0d0c = sb_LoadSampleBank("kampf.sbf", 1);
    ai_Unk6f0d10 = sb_LoadSampleBank("kampf\\armbrust.sbf", 1);
    ai_Unk6f0d14 = sb_LoadSampleBank("kampf\\bombeundtopf.sbf", 1);
    ai_Unk6f0d18 = sb_LoadSampleBank("kampf\\dolche.sbf", 1);
    ai_Unk6f0d1c = sb_LoadSampleBank("kampf\\pistole.sbf", 1);
    ai_Unk6f0d20 = sb_LoadSampleBank("kampf\\schwerter.sbf", 1);
    b = gm_BuildQuery(1, 1, *(int *)(rec + 0x10c));
    if (b != 0)
        ai_Unk6f0d50 = ai_Unk4d4050(b);
    if (ai_Unk6f0d50 == 2) {
        G19_SNDS(ai_Unk6f0d24, ai_Unk6f0d2c, ai_Unk6f0d34, ai_Unk6f0d3c, ai_Unk6f0d44, DIEBE);
    } else if (ai_Unk6f0d50 == 1) {
        G19_SNDS(ai_Unk6f0d24, ai_Unk6f0d2c, ai_Unk6f0d34, ai_Unk6f0d3c, ai_Unk6f0d44, RAEUBER);
    } else if (ai_Unk6f0d50 == 0) {
        G19_SNDS(ai_Unk6f0d24, ai_Unk6f0d2c, ai_Unk6f0d34, ai_Unk6f0d3c, ai_Unk6f0d44, SOELDNER);
    }
    b = gm_BuildQuery(1, 1, *(int *)(rec + 0x14));
    if (b != 0)
        ai_Unk6f0d54 = ai_Unk4d4050(b);
    if (ai_Unk6f0d54 == ai_Unk6f0d50) {
        ai_Unk6f0d28 = ai_Unk6f0d24;
        ai_Unk6f0d30 = ai_Unk6f0d2c;
        ai_Unk6f0d38 = ai_Unk6f0d34;
        ai_Unk6f0d40 = ai_Unk6f0d3c;
        ai_Unk6f0d48 = ai_Unk6f0d44;
    } else if (ai_Unk6f0d54 == 2) {
        G19_SNDS(ai_Unk6f0d28, ai_Unk6f0d30, ai_Unk6f0d38, ai_Unk6f0d40, ai_Unk6f0d48, DIEBE);
    } else if (ai_Unk6f0d54 == 1) {
        G19_SNDS(ai_Unk6f0d28, ai_Unk6f0d30, ai_Unk6f0d38, ai_Unk6f0d40, ai_Unk6f0d48, RAEUBER);
    } else if (ai_Unk6f0d54 == 0) {
        G19_SNDS(ai_Unk6f0d28, ai_Unk6f0d30, ai_Unk6f0d38, ai_Unk6f0d40, ai_Unk6f0d48, SOELDNER);
    }
    ai_Unk6f0d5c = d3io_LoadObjectRef("ub_STIEFEL", "ub_STIEFEL");
    if (sc == 0) {
        *(char **)(ai_Unk5fb5b0 + 0xb0) = (char *)d3sm_Create(ai_Unk633738, *(int *)(ai_Unk5fb5b0 + 0xb4), 2);
        sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    }
    ai_Unk935d00 = *(float *)(sc + 0x10) * ai_Unk5edc58;
    sv_ScrollSpeed = 0.15f;
    sv_CameraEaseOn = 0;
    ai_Unk6f0d04 = rec;
    ai_Unk4d0ca0();
    ai_Unk6f0cec = (int)gm_FindChar(*(int *)(rec + 0x34));
    ai_Unk6f0ce8 = (int)gm_FindChar(*(int *)(rec + 0x38));
    if (ai_CharInts[ai_PlayerId][0] == ai_Unk60cb5c && (rec[0x7c] & 1)) {
        cm_BeginSendGroup();
        for (i = 0; i < 3; i++) {
            if (*(int *)(rec + 0x100 + i * 4) != -1)
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
        if (*(int *)(rec + 0xc0 + i * 4) != -1) {
            r = ai_Unk4d0920((char *)*(int *)(rec + 0xc0 + i * 4), (int)"dummy_OFFENSE_ENTRY");
            *(int *)(r + 0x16c) = ai_Unk6f0cec;
            if (*(char **)(r + 0x184) != 0) {
                o = *(char **)(*(char **)(r + 0x184) + 0x34);
                d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), 5, 1);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0x80 + i * 4) != -1) {
            r = ai_Unk4d0920((char *)*(int *)(rec + 0x80 + i * 4), (int)"dummy_DEFENSE_ENTRY");
            *(int *)(r + 0x16c) = ai_Unk6f0ce8;
            *(float *)(*(char **)(r + 0x184) + 0x1a0) = *(float *)(*(char **)(r + 0x184) + 0x1a0) * ai_Unk5eb8b8;
            if (*(char **)(r + 0x184) != 0) {
                o = *(char **)(*(char **)(r + 0x184) + 0x34);
                d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), 4, 1);
            }
        }
    }
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, ai_Unk4d0f80);
    memset(ai_Unk7603a0V, 0, 0x80);
    pp = ai_Unk7603a0V;
    for (p = ai_Unk930600; (int)p < ((unsigned)&ai_Unk930600 + 0x4380); p += 0x21c) {
        if (*(unsigned short *)p != 0xffff && *(char **)(p + 0x184) != 0 &&
            *(char **)(*(char **)(p + 0x184) + 0x88) == ai_Unk5fb5b0)
            *pp++ = p;
    }
    for (i = 1; i < 9; i++) {
        sprintf(nm, "WARE_0%i", i);
        o = (char *)d3_FindObject(0, 0x40, 0, nm);
        if (o != 0)
            d3if_Show(o, 0);
    }
    n = 0;
    for (j = 0; j < 3; j++) {
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        gm_ShuffledBytes(9, t);
        sprintf(nm, "dummy_KARREN_0%i", j + 1);
        o = (char *)d3_FindObject(0, 0x100, 0, nm);
        if (o != 0) {
            if (*(int *)(rec + 0x100 + j * 4) == -1)
                d3if_ShowTree(o, 0);
            else {
                a = gm_Query(0, 2, 7, 1, *(int *)(rec + 0x100 + j * 4));
                if (a == 0) {
                    gm_FindObjectInst(*(int *)(rec + 0x100 + j * 4));
                    *(int *)(rec + 0x100 + j * 4) = -1;
                } else {
                    c = gm_Query(*(int *)(a + 0x14), 1, 0, 0x1dd);
                    n = 0;
                    while ((w = gm_Query(*(int *)(c + 0x14), 1, 5)) != 0 && n < 3) {
                        q[n++] = (int)w;
                        gm_QueryNext();
                    }
                    d3_PointToWorld(o, o + 0x4c, buf);
                    ty = *(unsigned short *)a;
                    if (ty == 0x134)
                        sprintf(path, "%s*ob_KARREN_KLEIN_KAMPF.ogr", ai_DataPath);
                    else if (ty == 0x135)
                        sprintf(path, "%s*ob_KARREN_MITTEL_KAMPF.ogr", ai_DataPath);
                    else if (ty == 0x136)
                        sprintf(path, "%s*ob_KARREN_GROSS_KAMPF.ogr", ai_DataPath);
                    h = (char *)d3_LoadGroup(path, 0, 0);
                    d3_SetObjectPos(h, buf);
                    *(int *)(h + 0x200) = (int)a;
                    for (i = 0; i < 3; i++) {
                        sprintf(nm, "WARE_0%i", t[i] + 1);
                        w = (char *)d3_FindObject(0, 0xc0, 0, nm);
                        if (w != 0) {
                            if (i > n - 1)
                                d3if_Show(w, 0);
                            else {
                                *(int *)(w + 0x200) = q[i];
                                d3if_Show(w, 1);
                                *(int *)(w + 0x224) = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    d3sm_Build(*(char **)(ai_Unk5fb5b0 + 0xb0), ai_Unk633738);
    for (i = 0; i < nmax; i++) {
        wd = *(int *)(sc + 0x20);
        cell = *(int **)(sc + 0x24);
        for (j = i; j < wd; j++) {
            cell[((wd - 1) * i + j) * 7] = 0xd;
            cell[((wd - i - 1) * wd + j) * 7] = 0xd;
            cell[(wd * j + i) * 7] = 0xd;
            cell[(wd * (j + 1) - i - 1) * 7] = 0xd;
        }
    }
    if (b != 0)
        ai_Unk60cb58 = (float)gm_BuildingSkill(b, 6, 1) + ai_Unk5eb878;
    else
        ai_Unk60cb58 = 1.0f;
}
/* 0x4d1a10, ends the fight scene (the counterpart of 0x4d0fa0): releases the sound sets, the flag objects
   ai_Unk935980[], the characters' objects (the strength effect of non-kind-0xb characters 0x4fca40) and resets the scene */

void ai_Unk4d1a10(char *rec)
{
    char *r, *k;
    int i;

    ai_Unk4e3060();
    if (ai_Unk6f0cf0 != 0) {
        d3if_Delete((struct D3OBJECT *)ai_Unk6f0cf0);
        ai_Unk6f0cf0 = 0;
    }
    sb_UnloadSampleBank(ai_Unk6f0d0c);
    sb_UnloadSampleBank(ai_Unk6f0d10);
    sb_UnloadSampleBank(ai_Unk6f0d14);
    sb_UnloadSampleBank(ai_Unk6f0d18);
    sb_UnloadSampleBank(ai_Unk6f0d1c);
    sb_UnloadSampleBank(ai_Unk6f0d20);
    sb_UnloadSampleBank(ai_Unk6f0d24);
    sb_UnloadSampleBank(ai_Unk6f0d2c);
    sb_UnloadSampleBank(ai_Unk6f0d34);
    sb_UnloadSampleBank(ai_Unk6f0d3c);
    sb_UnloadSampleBank(ai_Unk6f0d44);
    if (ai_Unk6f0d50 != ai_Unk6f0d54) {
        sb_UnloadSampleBank(ai_Unk6f0d28);
        sb_UnloadSampleBank(ai_Unk6f0d30);
        sb_UnloadSampleBank(ai_Unk6f0d38);
        sb_UnloadSampleBank(ai_Unk6f0d40);
        sb_UnloadSampleBank(ai_Unk6f0d48);
    }
    for (i = 0; i < ai_Unk6f0d58; i++)
        d3if_Delete((struct D3OBJECT *)ai_Unk935980[i]);
    if (ai_Unk6f0d5c != 0) {
        d3io_ReleaseObject(ai_Unk6f0d5c);
        ai_Unk6f0d5c = 0;
    }
    for (r = ai_Unk930600; (int)(r + 0x184) < ((unsigned)&ai_Unk930600 + 0x4504); r += 0x21c) {
        if (*(unsigned short *)r != 0xffff && *(int *)(r + 0x184) != 0) {
            if (*(int *)(rec + 0xc) == ai_CharInts[ai_PlayerId][0] || ai_Unk1460ae0 == -1) {
                k = gm_FindChar(*(int *)(r + 4));
                if (k[2] != 0xb)
                    gm_AddHealth(*(unsigned short *)k, *(int *)(r + 0x24) - *(int *)(k + 0x24));
            }
            ch_Delete(*(int *)(r + 0x184));
            *(int *)(r + 0x184) = 0;
        }
    }
    sv_ScrollSpeed = 1.0f;
    if (ai_Unk633738 != 0) {
        ai_Unk633dd4 = 0;
        *(int *)(ai_Unkfd7c50 + d3_CurrentScene * 0x3e0 + 8) = 0;
    }
    ai_Unk4e30c0();
    snd_EndSpecialTrack();
    sv_CameraEaseOn = 1;
    d2_CopyCaption((char *)&ch_EmptyName);
}
/* 0x4d1c20, places the characters ids[16] (side of the fight: dummies "dummy_<side>_NAHK_%02i" / "_FERN_%02i" for the
   melee / ranged kinds 0x174 0x176 0x15e 0x160) on the ground at their dummy (0x45f170 / 0x45f100 / 0x4072a0 /
   0x4025c0 / 0x43b860). Rough draft. */

void ai_Unk4d1c20(char *side, int id)
{
    char buf[0x60];
    float v[4], w[4], u[4];
    int a, b, h, n1 = 1, n2 = 1, i;
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *c, *k, *o;
    int *ids = (int *)side;

    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = ai_Unk4ce130(ids[i]);
        if (c == 0)
            continue;
        k = ai_Unk4cda50(c);
        if (k != 0 && (*(int *)k == 0x174 || *(int *)k == 0x176 || *(int *)k == 0x15e || *(int *)k == 0x160))
            sprintf(buf, "dummy_%s_FERN_%02i", (char *)id, n2++);
        else
            sprintf(buf, "dummy_%s_NAHK_%02i", (char *)id, n1++);
        o = (char *)d3_FindObject(0, 0x100, 0, buf);
        if (o == 0)
            continue;
        d3_PointToWorld(o, o + 0x4c, v);
        if ((char)d3_GetHeightAt(sc, v, &a, &b, &h) == 0)
            return;
        ch_FindFreeCellK(sc, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, a, b, w, 2, 0);
        d3sm_GetVertexI(sc, a, b, w);
        ch_SetPos(*(char **)(c + 0x184), u, 1);
        u[0] = 0;
        u[1] = 0;
        u[2] = 0;
        d3_DirToWorld(o, (unsigned)&ai_AxisPair, v);
        d3_VecAngle((unsigned)&ai_AxisPair, v);
        d3_SetObjectRot(*(char **)(*(char **)(c + 0x184) + 0x34), u);
    }
}
/* 0x4d1e00, the intro of the fight: a random fight track (when the music is on) and the cutscene "Kampf_Intro_Fluege" */

void ai_Unk4d1e00()
{
    int r;
    char *s;

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
    sv_UpdateSkyPhase(ai_Time);
    s = (char *)evt_Load("cutscenes\\kampf\\Kampf_Intro_Fluege.esc");
    if (s != 0) {
        evt_RunScript((EVTSCRIPT *)s);
        ai_Unk4ceb80(*(int *)(s + 0x100));
    }
    d2_CutsceneActive = 0;
}
/* 0x4d1eb0, the outro: stops the stream ai_Unk6f0d4c and runs the cutscene "Kampf_outro_flug" */

void ai_Unk4d1eb0()
{
    char *s;

    if (ai_Unk6f0d4c != 0)
        msx_StopAmbient(ai_Unk6f0d4c);
    sv_FrameMask = 0x7d6c6;
    d2_CutsceneActive = 1;
    s = (char *)evt_Load("cutscenes\\kampf\\Kampf_outro_flug.esc");
    if (s != 0) {
        evt_RunScript((EVTSCRIPT *)s);
        ai_Unk4ceb80(*(int *)(s + 0x100));
    }
    d2_CutsceneActive = 0;
}
/* 0x4d1f10, the selected character that is closest to the character c (distance 0x4ce1b0 times the factor 0x4fc8e0,
   limit 1e6), or 0; the two loops are the same for the kinds 2 / other of c. Rough draft. */

char *ai_Unk4d1f10(char *c)
{
    float best = 1000000.0f;
    float w;
    char *res = 0;
    char **pp;

    if (*(int *)(ai_Unk4cda50(c) + 0x58) == 2) {
        for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
            if (*pp != 0 && ai_Unk4ce270(c, *pp) != 0 && (*pp)[8] != 0) {
                w = ai_Unk4ce1b0(c, *(char **)(*(char **)(*pp + 0x184) + 0x34));
                w = gm_HealthRatio(*pp) * w;
                if (w < best) {
                    res = *pp;
                    best = w;
                }
            }
        }
        return res;
    }
    for (pp = ai_Unk7603a0V; (int)pp < (int)ai_Unk760420V; pp++) {
        if (*pp != 0 && ai_Unk4ce270(c, *pp) != 0 && (*pp)[8] != 0) {
            w = ai_Unk4ce1b0(c, *(char **)(*(char **)(*pp + 0x184) + 0x34));
            w = gm_HealthRatio(*pp) * w;
            if (w < best) {
                res = *pp;
                best = w;
            }
        }
    }
    return res;
}
/* 0x4d2010, the balance of forces at the ground point (a, b) for the character c: each living enemy within the radius adds
   5eb798 / distance, each other friend subtracts it (the sides are the 16 ids of the fight record +0x80 / +0xc0). */

float ai_Unk4d2010(char *c, int a, int b, float limit)
{
    float acc = 0.0f;
    float v[4];
    float d, dx, dy, dz;
    char *own, *oth, *o, *m;
    char *rec = ai_Unk6f0d04;
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int i;

    if (*(int *)(c + 0x16c) == ai_Unk6f0cec) {
        own = rec + 0xc0;
        oth = rec + 0x80;
    } else {
        own = rec + 0x80;
        oth = rec + 0xc0;
    }
    d3sm_GetVertexI(sc, a, b, v);
    for (i = 0; i < 16; i++) {
        if (*(int *)(oth + i * 4) != -1) {
            o = ai_Unk4ce130(*(int *)(oth + i * 4));
            if (o != 0 && (unsigned char)o[8] > 0) {
                m = *(char **)(*(char **)(o + 0x184) + 0x34);
                dx = v[0] - *(float *)(m + 0x4c);
                dy = v[1] - *(float *)(m + 0x50);
                dz = v[2] - *(float *)(m + 0x54);
                d = (float)sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    acc += 70.0f / d;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(own + i * 4) != -1) {
            o = ai_Unk4ce130(*(int *)(own + i * 4));
            if (o != 0 && (unsigned char)o[8] > 0 && o != c) {
                m = *(char **)(*(char **)(o + 0x184) + 0x34);
                dx = v[0] - *(float *)(m + 0x4c);
                dy = v[1] - *(float *)(m + 0x50);
                dz = v[2] - *(float *)(m + 0x54);
                d = (float)sqrt(dx * dx + dy * dy + dz * dz);
                if (d <= limit)
                    acc -= 70.0f / d;
            }
        }
    }
    return acc;
}
/* 0x4d2170, the best ground point (*ox, *oy) around the cell (cx, cy) within the rings 0 .. r - 1 of the scene's map: the
   cell types other than 0 / 0xd are rated with 0x4d2010 (limit r * radius * 2), the least value wins; returns 0 when no
   cell was rated. Rough draft. */

int ai_Unk4d2170(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    float best = 1000000000.0f;
    float f;
    int k, row, col, hi, hic, m, cell;

    for (k = 0; k < r; k++) {
        hi = cy + k;
        if (*(int *)(sc + 0x20) - 1 < hi)
            hi = *(int *)(sc + 0x20) - 1;
        for (row = (cy - k < 0) ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = cx + m;
            if (*(int *)(sc + 0x20) - 1 < hic)
                hic = *(int *)(sc + 0x20) - 1;
            for (col = (cx - m < 0) ? 0 : cx - m; col <= hic; col++) {
                cell = (*(int **)(sc + 0x24))[(*(int *)(sc + 0x20) * row + col) * 7];
                if (cell != 0 && cell != 0xd) {
                    f = ai_Unk4d2010(c, col, row, (float)(r * *(float *)(sc + 0x10) * 2));
                    if (f < best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    return best != ai_Unk5edc60;
}
/* 0x4d22d0, like 0x4d2170 but the greatest rating wins (and the threshold 5eb988 must be exceeded); rough draft */

int ai_Unk4d22d0(char *c, int cx, int cy, int r, int *ox, int *oy)
{
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    float best = 0.0f;
    float thr = 0.0f;
    float f;
    int k, row, col, hi, hic, m, cell;

    for (k = 0; k < r; k++) {
        hi = cy + k;
        if (*(int *)(sc + 0x20) - 1 < hi)
            hi = *(int *)(sc + 0x20) - 1;
        for (row = (cy - k < 0) ? 0 : cy - k; row <= hi; row++) {
            m = k - abs(cy - row);
            hic = cx + m;
            if (*(int *)(sc + 0x20) - 1 < hic)
                hic = *(int *)(sc + 0x20) - 1;
            for (col = (cx - m < 0) ? 0 : cx - m; col <= hic; col++) {
                cell = (*(int **)(sc + 0x24))[(*(int *)(sc + 0x20) * row + col) * 7];
                if (cell != 0 && cell != 0xd) {
                    f = ai_Unk4d2010(c, col, row, (float)(r * *(float *)(sc + 0x10) * 2));
                    if (f > best) {
                        best = f;
                        *ox = col;
                        *oy = row;
                    }
                }
            }
        }
    }
    if (best != ai_Unk5eb788 && thr > ai_Unk5eb988)
        return 1;
    return 0;
}
/* 0x4d2450, a callback of the object walk: collects the objects named "sp_ESCAPE" into list[ai_Unk6f0d60++] */

char ai_Unk4d2450(char *o, int *list)
{
    if (_stricmp(o, "sp_ESCAPE") == 0) {
        list[ai_Unk6f0d60] = (int)o;
        ai_Unk6f0d60++;
    }
    return 1;
}
/* 0x4d2490, the closest "sp_ESCAPE" object (0x4d2450) to the character c, or 0 (limit 1e6) */

char *ai_Unk4d2490(char *c)
{
    int list[0x20];
    float v[4];
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    char *m, *o;
    int i, n;

    ai_Unk6f0d60 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0xc0, ai_Unk4d2450, list);
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    d3_PointToWorld(m, m + 0x4c, v);
    n = ai_Unk6f0d60;
    for (i = 0; i < n; i++) {
        o = (char *)list[i];
        dx = v[0] - *(float *)(o + 0x4c);
        dy = v[1] - *(float *)(o + 0x50);
        dz = v[2] - *(float *)(o + 0x54);
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = o;
        }
    }
    return res;
}
/* 0x4d2550, a callback: collects the objects "WARE_" whose owner (+0x200, a record with the side in +0xa) is not the side
   of the character c into ai_Unk930380[ai_Unk6f0d60++] */

char ai_Unk4d2550(char *o, char *c)
{
    char *side = *(char **)(c + 0x16c);

    if (strncmp(o, "WARE_", 5) == 0 && *(int *)(*(char **)(o + 0x200) + 0xa) != *(unsigned short *)side)
        ai_Unk930380[ai_Unk6f0d60++] = o;
    return 1;
}
/* 0x4d25a0, the closest object of 0x4d2550 to the character c, or 0 */

char *ai_Unk4d25a0(char *c)
{
    float v[4];
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    char *m, *o;
    int i, n;

    ai_Unk6f0d60 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0x40, ai_Unk4d2550, c);
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    d3_PointToWorld(m, m + 0x4c, v);
    n = ai_Unk6f0d60;
    for (i = 0; i < n; i++) {
        o = ai_Unk930380[i];
        dx = v[0] - *(float *)(o + 0x4c);
        dy = v[1] - *(float *)(o + 0x50);
        dz = v[2] - *(float *)(o + 0x54);
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        if (d < best) {
            best = d;
            res = o;
        }
    }
    return res;
}
/* 0x4d2650, a callback: collects the objects "sp_CONQUER*" into ai_Unk930380[ai_Unk6f0d60++] */

char ai_Unk4d2650(char *o)
{
    if (strncmp(o, "sp_CONQUER", 10) == 0)
        ai_Unk930380[ai_Unk6f0d60++] = o;
    return 1;
}
/* 0x4d2680, the closest "sp_CONQUER" object to the character c, the distance weighted by the owner (+0x200: 5eb990 for
   an own friend, 5eb7b0 for another one that is not a friend, 5ebbe8 otherwise); 0 when it is owned by c itself. Rough draft. */

char *ai_Unk4d2680(char *c)
{
    float v[3];
    float best = 1000000.0f;
    float d, dx, dy, dz;
    char *res = 0;
    char *m, *o, *e;
    int i;

    ai_Unk6f0d60 = 0;
    d3_ForEachObject(ai_Unk5fb5b0, 0, 0x40, ai_Unk4d2650, c);
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    d3_PointToWorld(m, m + 0x4c, v);
    for (i = 0; i < ai_Unk6f0d60; i++) {
        o = ai_Unk930380[i];
        dx = v[0] - *(float *)(o + 0x4c);
        dy = v[1] - *(float *)(o + 0x50);
        dz = v[2] - *(float *)(o + 0x54);
        d = (float)sqrt(dx * dx + dy * dy + dz * dz);
        e = *(char **)(o + 0x200);
        if (e != 0) {
            if (ai_Unk4ce270(c, e) != 0)
                d = (float)(d * ai_Unk5eb990);
            else if (e != 0) {
                if (ai_Unk4ce270(c, e) == 0 && e != c)
                    d = (float)(d * ai_Unk5eb7b0);
                else if (e != 0)
                    d = (float)(d * ai_Unk5ebbe8);
            }
        }
        if (d < best) {
            best = d;
            res = o;
        }
    }
    if (*(char **)(res + 0x200) == c)
        return 0;
    return res;
}
/* 0x4d27a0, builds the table ai_Unk9300a0[6][5][6] of floats (the values by (row, column) for the six kinds, stored one by one,
   then each row of 6 accumulated with the factor 5ece60: v[c] = v[c] * k + v[c - 1], and the repeats of the previous column
   cleared). Rough draft. */

void ai_BuildFloatTable()
{
    int i, j, k, b, e;
    float t;
    float *p;

    memset(ai_Unk9300a0, 0, 0x2d0);
    ai_Unk9300a0[0x0] = 10.0f;
    ai_Unk9300a0[0x1] = 10.0f;
    ai_Unk9300a0[0x4] = 30.0f;
    ai_Unk9300a0[0x5] = 50.0f;
    ai_Unk9300a0[0x6] = 40.0f;
    ai_Unk9300a0[0x7] = 20.0f;
    ai_Unk9300a0[0xa] = 20.0f;
    ai_Unk9300a0[0xb] = 20.0f;
    ai_Unk9300a0[0xc] = 80.0f;
    ai_Unk9300a0[0xd] = 20.0f;
    ai_Unk9300a0[0x10] = 0;
    ai_Unk9300a0[0x11] = 0;
    ai_Unk9300a0[0x12] = 70.0f;
    ai_Unk9300a0[0x13] = 30.0f;
    ai_Unk9300a0[0x16] = 0;
    ai_Unk9300a0[0x17] = 0;
    ai_Unk9300a0[0x18] = 50.0f;
    ai_Unk9300a0[0x19] = 50.0f;
    ai_Unk9300a0[0x1c] = 0;
    ai_Unk9300a0[0x1d] = 0;
    ai_Unk9300a0[0x1e] = 10.0f;
    ai_Unk9300a0[0x1f] = 10.0f;
    ai_Unk9300a0[0x22] = 30.0f;
    ai_Unk9300a0[0x23] = 50.0f;
    ai_Unk9300a0[0x24] = 80.0f;
    ai_Unk9300a0[0x25] = 20.0f;
    ai_Unk9300a0[0x28] = 0;
    ai_Unk9300a0[0x29] = 0;
    ai_Unk9300a0[0x2a] = 80.0f;
    ai_Unk9300a0[0x2b] = 20.0f;
    ai_Unk9300a0[0x2e] = 0;
    ai_Unk9300a0[0x2f] = 0;
    ai_Unk9300a0[0x30] = 60.0f;
    ai_Unk9300a0[0x31] = 40.0f;
    ai_Unk9300a0[0x34] = 0;
    ai_Unk9300a0[0x35] = 0;
    ai_Unk9300a0[0x36] = 50.0f;
    ai_Unk9300a0[0x37] = 50.0f;
    ai_Unk9300a0[0x3a] = 0;
    ai_Unk9300a0[0x3b] = 0;
    ai_Unk9300a0[0x3c] = 5.0f;
    ai_Unk9300a0[0x3e] = 5.0f;
    ai_Unk9300a0[0x40] = 20.0f;
    ai_Unk9300a0[0x41] = 70.0f;
    ai_Unk9300a0[0x42] = 50.0f;
    ai_Unk9300a0[0x44] = 10.0f;
    ai_Unk9300a0[0x46] = 20.0f;
    ai_Unk9300a0[0x47] = 20.0f;
    ai_Unk9300a0[0x48] = 80.0f;
    ai_Unk9300a0[0x4a] = 20.0f;
    ai_Unk9300a0[0x4c] = 0;
    ai_Unk9300a0[0x4d] = 0;
    ai_Unk9300a0[0x4e] = 70.0f;
    ai_Unk9300a0[0x50] = 30.0f;
    ai_Unk9300a0[0x52] = 0;
    ai_Unk9300a0[0x53] = 0;
    ai_Unk9300a0[0x54] = 50.0f;
    ai_Unk9300a0[0x56] = 50.0f;
    ai_Unk9300a0[0x58] = 0;
    ai_Unk9300a0[0x59] = 0;
    ai_Unk9300a0[0x5a] = 20.0f;
    ai_Unk9300a0[0x5c] = 80.0f;
    ai_Unk9300a0[0x5e] = 0;
    ai_Unk9300a0[0x5f] = 0;
    ai_Unk9300a0[0x60] = 30.0f;
    ai_Unk9300a0[0x62] = 70.0f;
    ai_Unk9300a0[0x64] = 0;
    ai_Unk9300a0[0x65] = 0;
    ai_Unk9300a0[0x66] = 40.0f;
    ai_Unk9300a0[0x68] = 60.0f;
    ai_Unk9300a0[0x6a] = 0;
    ai_Unk9300a0[0x6b] = 0;
    ai_Unk9300a0[0x6c] = 60.0f;
    ai_Unk9300a0[0x70] = 0;
    ai_Unk9300a0[0x71] = 0;
    ai_Unk9300a0[0x76] = 0;
    ai_Unk9300a0[0x77] = 0;
    ai_Unk9300a0[0x7a] = 0;
    ai_Unk9300a0[0x80] = 0;
    ai_Unk9300a0[0x86] = 0;
    ai_Unk9300a0[0x88] = 0;
    ai_Unk9300a0[0x89] = 0;
    ai_Unk9300a0[0x8c] = 0;
    ai_Unk9300a0[0x8e] = 0;
    ai_Unk9300a0[0x8f] = 0;
    ai_Unk9300a0[0x92] = 0;
    ai_Unk9300a0[0x94] = 0;
    ai_Unk9300a0[0x95] = 0;
    ai_Unk9300a0[0x98] = 0;
    ai_Unk9300a0[0x9e] = 0;
    ai_Unk9300a0[0xa4] = 0;
    ai_Unk9300a0[0xa6] = 0;
    ai_Unk9300a0[0xa7] = 0;
    ai_Unk9300a0[0xaa] = 0;
    ai_Unk9300a0[0xac] = 0;
    ai_Unk9300a0[0xad] = 0;
    ai_Unk9300a0[0xb0] = 0;
    ai_Unk9300a0[0xb2] = 0;
    ai_Unk9300a0[0xb3] = 0;
    ai_Unk9300a0[0x6e] = 40.0f;
    ai_Unk9300a0[0x72] = 70.0f;
    ai_Unk9300a0[0x74] = 30.0f;
    ai_Unk9300a0[0x78] = 20.0f;
    ai_Unk9300a0[0x7c] = 10.0f;
    ai_Unk9300a0[0x7d] = 70.0f;
    ai_Unk9300a0[0x7e] = 30.0f;
    ai_Unk9300a0[0x82] = 20.0f;
    ai_Unk9300a0[0x83] = 50.0f;
    ai_Unk9300a0[0x84] = 100.0f;
    ai_Unk9300a0[0x8a] = 100.0f;
    ai_Unk9300a0[0x90] = 100.0f;
    ai_Unk9300a0[0x96] = 20.0f;
    ai_Unk9300a0[0x9a] = 10.0f;
    ai_Unk9300a0[0x9b] = 70.0f;
    ai_Unk9300a0[0x9c] = 30.0f;
    ai_Unk9300a0[0xa0] = 20.0f;
    ai_Unk9300a0[0xa1] = 50.0f;
    ai_Unk9300a0[0xa2] = 100.0f;
    ai_Unk9300a0[0xa8] = 100.0f;
    ai_Unk9300a0[0xae] = 100.0f;
    for (p = ai_Unk9300a0; (int)p < ((unsigned)ai_Unk9300a0 + 0x2d0);) {
        for (j = 0; j < 5; j++) {
            for (k = 0; k < 6; k++, p++) {
                t = (float)(*p * ai_Unk5ece60);
                *p = t;
                if (k != 0)
                    *p = t + p[-1];
            }
        }
    }
    for (b = 0; b < 30; b += 5) {
        for (i = 0; i < 5; i++) {
            for (j = 1; j < 6; j++) {
                for (k = 0; k < 6 - j; k++) {
                    e = (i + b) * 6 + j;
                    if (ai_Unk9300a0[e + k] == ai_Unk9300a0[e - 1])
                        ai_Unk9300a0[e + k] = 0;
                }
            }
        }
    }
}
/* 0x4d2c00, the row of 6 floats of ai_Unk9300a0 for the side (kind 0 .. 5 by the fight flags +0x7c bit 0 / 1 / 2 and the side
   being the attacker ai_Unk6f0cec or the defender ai_Unk6f0ce8, else 6) and the strength level 0 .. 4 (the ratio of the two
   sides' strengths 0x4cdcf0 against the limits 5ebc1c / 5ecebc / 5eb94c / 5eb7ac), lowered by the number of "sp_CONQUER"
   flags the attacker owns. Rough draft. */

float *ai_Unk4d2c00(int side)
{
    unsigned char fl = *(unsigned char *)(ai_Unk6f0d04 + 0x7c);
    int kind = 6;
    int lvl, cnt = 0, i;
    float a = 0.0f, b = 0.0f, r;
    char *o;

    if ((fl & 1) && ai_Unk6f0cec == side)
        kind = 0;
    else if ((fl & 1) && ai_Unk6f0ce8 == side)
        kind = 1;
    else if ((fl & 2) && ai_Unk6f0cec == side)
        kind = 2;
    else if ((fl & 2) && ai_Unk6f0ce8 == side)
        kind = 3;
    else if ((fl & 4) && ai_Unk6f0cec == side)
        kind = 4;
    else if ((fl & 4) && ai_Unk6f0ce8 == side)
        kind = 5;
    for (i = 0x80; i < 0xc0; i += 4) {
        if (*(int *)(ai_Unk6f0d04 + i) != -1) {
            o = ai_Unk4ce130(*(int *)(ai_Unk6f0d04 + i));
            if (o[8] != 0 && o[8] != 4)
                a = ai_Unk4cdcf0(o) + a;
        }
    }
    for (i = 0xc0; i < 0x100; i += 4) {
        if (*(int *)(ai_Unk6f0d04 + i) != -1) {
            o = ai_Unk4ce130(*(int *)(ai_Unk6f0d04 + i));
            if (o[8] != 0 && o[8] != 4)
                b = ai_Unk4cdcf0(o) + b;
        }
    }
    for (i = 0; i < ai_Unk6f0d58; i++) {
        if (strstr(ai_Unk935980[i], "sp_CONQUER") != 0) {
            float v[4];

            d3_PointToWorld(ai_Unk935980[i], ai_Unk935980[i] + 0x4c, v);
            o = *(char **)(ai_Unk935980[i] + 0x200);
            if (o != 0 && *(int *)(o + 0x16c) == ai_Unk6f0cec)
                cnt++;
        }
    }
    if (side == ai_Unk6f0cec)
        r = b / a;
    else
        r = a / b;
    if (!(r < ai_Unk5ebc1c))
        lvl = 4;
    else if (!(r < ai_Unk5ecebc))
        lvl = 3;
    else if (!(r < ai_Unk5eb94c))
        lvl = 2;
    else if (!(r < 0.5f))
        lvl = 1;
    else
        lvl = 0;
    if (side == ai_Unk6f0ce8) {
        if (cnt == 1)
            lvl = cnt;
        else if (cnt == 2 || cnt == 3)
            lvl = 0;
    }
    return &ai_Unk9300a0[(kind * 5 + lvl) * 6];
}
/* 0x4d2e30, the rating of the character c for the formation type 0 .. 5 (0 the strength times a factor 1 / 0.5 / 0.2 by the
   state +0x1c4 and an empty weapon count, 1 the inverse, 2 / 3 constants, 4 / 5 5eb8b0 or 5eb780 / 0x4fc8e0), else 5eb788 */

float ai_Unk4d2e30(char *c, int type)
{
    float v;
    int k;
    unsigned char st;

    switch (type) {
    case 0:
        v = 1.0f;
        if (c[0x1c4] != 0)
            v = 0.5f;
        k = *(int *)(ai_Unk4cda50(c) + 0x58);
        if ((k == 1 || k == 2) && *(int *)(ai_Unk4cdac0(c) + 0x20) <= 0)
            v = 0.2f;
        return ai_Unk4cdcf0(c) * v;
    case 1:
        st = c[0x1c4];
        if (st == 1)
            v = 1.0f;
        else {
            v = 0.4f;
            if (st != 0)
                v = 0.5f;
        }
        k = *(int *)(ai_Unk4cda50(c) + 0x58);
        if ((k == 1 || k == 2) && *(int *)(ai_Unk4cdac0(c) + 0x20) <= 0)
            v = 1.0f;
        return v / ai_Unk4cdcf0(c);
    case 2:
        if (c[0x1c4] != 2)
            return 0.5f;
        return ai_FloatOne;
    case 3:
        return ai_FloatOne;
    case 4:
    case 5:
        if (c[0x1c4] == 2)
            return 0.2f;
        return ai_FloatOne / gm_HealthRatio(c);
    }
    return ai_Unk5eb788;
}
/* 0x4d2f80, gives the characters of a side (the ids +0x80 / +0xc0 of the fight record) their formation: per character a
   random roll against the row 0x4d2c00 picks the type 0 .. 5, the character with the best rating 0x4d2e30 for it takes it
   (+0x1c4) unless it already has one and a second roll fails; then 0x4d3120 sets up each. Rough draft. */

void ai_Unk4d2f80(int side)
{
    char *rec = ai_Unk6f0d04;
    char *c[16];
    int *arr;
    float *row;
    char *rec2 = 0;
    char *sel;
    float r, best, w;
    int cnt = 0, i, j, k, n = 0, selidx = 0;

    row = ai_Unk4d2c00(side);
    for (i = 0; i < (unsigned char)rec[0x30]; i++) {
        if (*(int *)(rec + 0x34 + i * 4) == *(int *)((char *)side + 4)) {
            rec2 = ai_Unk92cae0[i];
            break;
        }
    }
    if (side == ai_Unk6f0cec)
        arr = (int *)(rec + 0xc0);
    else
        arr = (int *)(rec + 0x80);
    for (i = 0; i < 16; i++) {
        if (arr[i] != -1) {
            c[i] = ai_Unk4ce130(arr[i]);
            n++;
        } else
            c[i] = 0;
    }
    k = 0;
    for (cnt = n; cnt > 0; cnt--) {
        r = util_RandomF();
        for (j = 0; j < 6; j++) {
            if (r <= row[j]) {
                k = j;
                break;
            }
        }
        best = -1.0f;
        for (i = 0; i < 16; i++) {
            if (c[i] != 0) {
                w = ai_Unk4d2e30(c[i], k);
                if (w > best) {
                    best = w;
                    selidx = i;
                }
            }
        }
        sel = c[selidx];
        if (sel[0x1c4] == 0 || ai_Unk4e5360() > ai_Unk5eb9b0) {
            sel[0x1c4] = (char)k;
            c[selidx] = 0;
        }
    }
    for (i = 0; i < 16; i++) {
        if (arr[i] != -1)
            ai_Unk4d3120(rec2, ai_Unk4ce130(arr[i]));
    }
}
/* 0x4d3120, the formation order for the character c of the fight record rec2 by its formation type (+0x1c4: 0 the best free
   cell near the character for the kind 2 (0x4d22d0 / 0x4cffd0), else to the closest selected (0x4d1f10); 1 the closest "WARE_"
   0x4d25a0 -> 0x4d0380; 2 the closest "sp_CONQUER" 0x4d2680 -> 0x4d02d0; 3 0x4d0520; 4 0x4d0460; 5 the closest "sp_ESCAPE"
   0x4d2490 -> 0x4d0200). Rough draft. */

void ai_Unk4d3120(char *rec, char *c)
{
    char *sc, *k, *o;
    int x, y, h;
    float v[4];
    int r;

    if (c != 0 && *(int *)(c + 0x184) != 0) {
        if (c[8] == 0)
            return;
        if (*(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) == 1)
            return;
    }
    switch ((unsigned char)c[0x1c4]) {
    case 0:
        k = ai_Unk4cda50(c);
        if ((*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2) && *(int *)(ai_Unk4cdac0(c) + 0x20) <= 0)
            return;
        if (*(int *)(k + 0x58) == 2) {
            sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
            if ((char)d3_GetHeightAt(sc, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, &x, &y, &h) == 0)
                return;
            r = (int)(500.0f / *(float *)(sc + 0x10));
            if (ai_Unk4d22d0(c, x, y, r, &x, &y) == 0)
                return;
            ai_Unk4cffd0(rec, c, 0, x, y);
            return;
        }
        o = ai_Unk4d1f10(c);
        if (o == 0)
            return;
        ai_Unk4cffd0(rec, c, o, 0, 0);
        return;
    case 5:
        o = ai_Unk4d2490(c);
        if (o == 0)
            return;
        ai_Unk4d0200(rec, c, o);
        return;
    case 1:
        o = ai_Unk4d25a0(c);
        if (o == 0)
            return;
        G19_4D0380(rec, c, o);
        return;
    case 2:
        o = ai_Unk4d2680(c);
        if (o == 0)
            return;
        ai_Unk4d02d0(rec, c, o);
        return;
    case 4:
        ((int (*)(char *, char *))ai_Unk4d0460)(rec, c);
        return;
    case 3:
        ai_Unk4d0520(rec, c);
        return;
    }
}
/* 0x4d3300, a character falls: the sound "Koerperfaellt" and a blood puddle "ub_Blutlache" (random turn) at its place */

void ai_Unk4d3300(char *c)
{
    float rot[3];
    char *b;

    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
    rot[1] = (float)(ai_Unk4e52f0(0x168) * ai_Unk5eb7dc * ai_Unk5edc68);
    sb_PlaySampleI(0, "Koerperfaellt", 1, 0x3f, 0x7f, 0, 0);
    b = (char *)d3_CreateObject(0, *(char **)(*(char **)(c + 0x14) + 0x34) + 0x4c, rot, "ub_Blutlache");
    d3t_PlayAnim(b, "sonstiges\\ub_Blutlache.baf", 0x12);
}
/* 0x4d3390, the death of the character c when its strength 0x4fc8e0 is below 5edbd0: the model is stopped (0x415e90 / 0x405610),
   an object with the callback 0x4d3300 (blood puddle) and the sound name "kampf/todumfallen" is created (+0xf0), the character
   marked dead (+8 = 0), the sound "Schmerzschrei", a random text 0x1181 / 0x1182 sent to the side, the carried object (+0x1ac)
   put down and given to the defender; returns 1, else 0. Rough draft. */

int ai_Unk4d3390(char *c)
{
    char buf[0x100];
    char *m, *b, *p;
    int o;

    ai_Unk4cda50(c);
    if (!(gm_HealthRatio(c) < ai_Unk5edbd0))
        return 0;
    d3t_CreateBloodK(*(char **)(c + 0x184) + 0, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
    ch_StopActions(*(char **)(c + 0x184));
    b = (char *)ch_AddActionCBK(*(char **)(c + 0x184), ai_Unk4d3300, 0x14, 0x36, 1);
    strcpy(b + 0xf0, "kampf/todumfallen");
    c[8] = 0;
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    *(int *)(m + 0x220) = 0;
    sb_PlaySampleI(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
    if (ai_Unk4e52f0(100) > 0x32) {
        *(int *)(c + 0x24) = (int)((double)(-(int)*(float *)(c + 0x1c)) - ai_Unk5eb8e0);
        G19_5176D0(buf, (char *)0x1181, *(unsigned short *)c);
    } else {
        *(int *)(c + 0x24) = (int)(ai_Unk5eb990 - (double)(int)*(float *)(c + 0x1c));
        G19_5176D0(buf, (char *)0x1182, *(unsigned short *)c);
    }
    o = *(int *)(c + 0x1ac);
    if (o != 0) {
        d3_SetWorldPosRot(o, m + 0x4c, m + 0x18c);
        d3if_Show(o, 1);
        *(int *)(c + 0x1ac) = 0;
        p = (char *)ai_Unk4ceb70(*(char **)ai_Unk6f0ce8);
        *(int *)(p + 0xa) = *(unsigned short *)ai_Unk6f0ce8;
    }
    sv_SetStatusText(buf);
    return 1;
}

#define G19_FIGHTER_SYNC(ch)                                                          \
    r = ai_Unk4cdac0(ch);                                                             \
    if (r != 0) {                                                                     \
        vv = -1;                                                                      \
        cm_BeginFieldBlock(r, *(int *)(r + 2));                                             \
        cm_AddFieldValues(4, 1, (unsigned short)(r - cm_FieldObject + 0x20), &vv);           \
        cm_SendFieldDeltas();                                                               \
    }

/* 0x4d3550, the hit of the fight event ev (+0x17c the attacker, +0x180 the target, +0x184 its record): the weapon range test
   0x402140 / 0x4ce1b0, the worker count of the player's characters synced (+0x20 -> field), damage 0x4cedc0, the pain
   speech "_K_C2C_SCHMERZ" or the sound "Schmerzschrei", the hit / miss sound by the attacker's kind ("schuss_s", "Treffer",
   "Daneben_s"), 0x4d3390 for a death, else the target's model flinches (0x415e90 / 0x405610 / 0x405810 with
   "kampf/stich_abwehr" / "kampf/ausweichen_seite", the text 0x1188 for the kind 0x16e). Rough draft. */
void ai_Unk4d3550(char *ev)
{
    char buf[0x80];
    char tbl[0x14];
    char *att = *(char **)(ev + 0x17c);
    char *tgt = *(char **)(ev + 0x180);
    char *ka, *kt, *r;
    int vv, hit, rnd, a, b;
    int hitv = 0;
    int kt1 = 0;

    ka = ai_Unk4cda50(att);
    kt = ai_Unk4cda50(tgt);
    ch_GetHMap(tbl, att, tgt);
    if (kt != 0 && ai_Unk4ce1b0(tgt, *(char **)(*(char **)(att + 0x184) + 0x34)) <= *(float *)(kt + 0xc))
        ;
    else {
        hitv = 0;
        kt1 = 0;
    }
    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && (*(int *)(kt + 0x58) == 1 || *(int *)(kt + 0x58) == 2)) {
        G19_FIGHTER_SYNC(tgt);
    }
    if (hitv != 0) {
        rnd = ai_Unk4e52f0(10);
        if (*(int *)(att + 0x16c) == ai_Unk6f0cec)
            ai_Unk4cedc0(att, hitv, 1);
        else
            ai_Unk4cedc0(att, hitv, 2);
        if (rnd > 8) {
            if (*(int *)(ev + 0x188) != *(int *)(ai_Unk6f0cec + 4))
                snd_QueueVoice(*(unsigned short *)att, ai_Unk6f0d44, "_K_C2C_SCHMERZ", -1, 500);
            else
                snd_QueueVoice(*(unsigned short *)att, ai_Unk6f0d48, "_K_C2C_SCHMERZ", -1, 500);
        } else if (rnd > 5)
            sb_PlaySampleI(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
    }
    if (att[8] != 0 && tgt[8] != 0 && kt1 != 0) {
        switch (*(int *)ka) {
        case 0x156:
        case 0x158:
            sb_PlaySampleI(hitv ? 0 : ai_Unk6f0d20, hitv ? "Treffer" : "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
            break;
        case 0x154:
        case 0x16e:
        case 0x172:
            sb_PlaySampleI(hitv ? 0 : ai_Unk6f0d18, hitv ? "Treffer" : "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
            break;
        case 0x15e:
            sb_PlaySampleI(ai_Unk6f0d1c, "schuss_s", 1, 0x3f, 0x7f, 0, 0);
            break;
        case 0x160:
            if (hitv != 0)
                sb_PlaySampleI(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            break;
        default:
            if (*(int *)ka == 0 && hitv == 0)
                sb_PlaySampleI(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            break;
        }
        if (ai_Unk4d3390(att) == 0) {
            if (hitv != 0) {
                d3t_CreateBloodK(*(char **)(*(char **)(att + 0x184) + 0x34), 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
                if (*(char **)(*(char **)(att + 0x184) + 0x128) == 0 || (*(char **)(*(char **)(att + 0x184) + 0x128))[9] == 0x3b) {
                    ch_StopActions(*(char **)(att + 0x184));
                    a = ai_Unk4e52f0(2);
                    ai_Unk405810(*(char **)(att + 0x184), buf + a * 0x20, 1, 1.4f);
                }
                if (*(int *)ka == 0x16e) {
                    att[8] = 4;
                    G19_5176D0(buf, (char *)0x1188, *(int *)(tbl + 0x54), *(unsigned short *)att);
                    sv_SetStatusText(buf);
                }
            } else if (*(char **)(*(char **)(att + 0x184) + 0x128) == 0 ||
                       (*(char **)(*(char **)(att + 0x184) + 0x128))[9] == 0x3b) {
                ch_StopActions(*(char **)(att + 0x184));
                if (*(int *)ka == 0x15e || *(int *)ka == 0x160)
                    ai_Unk405810(*(char **)(att + 0x184), "kampf/ausweichen_seite", 1, 1.4f);
                else
                    ai_Unk405810(*(char **)(att + 0x184), "kampf/stich_abwehr", 1, 1.4f);
            }
        }
    }
}
/* 0x4d3980, an explosion event: the fuse sound "ZuendschnurBrennt_l_01", the attacker's worker count synced, 0x4ce290 */

void ai_Unk4d3980(char *ev)
{
    char *att = *(char **)(ev + 0x180);
    char *k, *r;
    int vv;

    k = ai_Unk4cda50(att);
    ai_Unk4ce290(*(char **)(*(char **)(att + 0x184) + 0x34) + 0x4c);
    sb_PlaySampleI(ai_Unk6f0d14, "ZuendschnurBrennt_l_01", 3, 0x3f, 0x7f, 0, 0);
    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && (*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2)) {
        G19_FIGHTER_SYNC(att);
    }
}
/* 0x4d3a30, a bomb event: the attacker's worker count synced, 0x4ce5d0 at the target */

void ai_Unk4d3a30(char *ev)
{
    char *att = *(char **)(ev + 0x17c);
    char *r;
    int vv;

    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c) {
        G19_FIGHTER_SYNC(att);
    }
    ai_Unk4ce5d0(*(float **)(*(char **)(att + 0x184) + 0x34) + 0x13, *(int *)(ev + 0x180), *(int *)(ev + 0x184));
}
/* 0x4d3ac0, 0x402480 on the attacker's record */

void ai_Unk4d3ac0(char *ev)
{
    ch_SetVisible(*(char **)(*(char **)(ev + 0x17c) + 0x184), 0);
}
/* 0x4d3ae0, the target puts its carried object down: the object (+0x1ac of the attacker) belongs to the defender's side now */

void ai_Unk4d3ae0(char *ev)
{
    char *att = *(char **)(ev + 0x17c);
    int o = *(int *)(ev + 0x180);
    char *p;

    p = ai_Unk4ceb70((char *)o);
    if (p != 0)
        *(int *)(p + 0xa) = *(unsigned short *)ai_Unk6f0cec;
    *(int *)(att + 0x1ac) = o;
    d3if_ShowTree(o, 0);
}
/* 0x4d3b30, the winning side of a fight record: the character record of the side (ai_Unk6f0cec attacker, ai_Unk6f0ce8
   defender) that has nobody left standing opposite or owns every "sp_CONQUER" object of the scene; the winner gets
   message 0x50 with its fight record (0x4cdf00) marked 5. 0 / the pending result 0x6f0cf4 if no winner yet */

char *ai_Unk4d3b30(char *rec)
{
    char *w = 0;
    char *ret;
    int total = 0;
    int mine = 0;
    int na = 0, nb = 0;
    int out;
    int i, id, n;
    char *c, *o, *rr;
    int sl[11];

    if (ai_Unk6f0cf4 != 0)
        return (char *)ai_Unk6f0cf4;
    for (i = 0; i < 16; i++) {
        id = *(int *)(rec + 0x80 + i * 4);
        if (id != -1) {
            c = ai_Unk4ce130(id);
            if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                na++;
        }
    }
    for (i = 0; i < 16; i++) {
        id = *(int *)(rec + 0xc0 + i * 4);
        if (id != -1) {
            c = ai_Unk4ce130(id);
            if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                nb++;
        }
    }
    if (nb > 0) {
        if (na <= 0)
            w = (char *)ai_Unk6f0ce8;
        ret = w;
    } else
        ret = (char *)ai_Unk6f0cec;
    for (i = 0; i < ai_Unk6f0d58; i++) {
        if (strcmp(ai_Unk935980[i], "sp_CONQUER") != 0) {
            o = ai_Unk935980[i];
            d3_PointToWorld(o, o + 0x4c, &out);
            total = out + 1;
            if (*(char **)(o + 0x200) != 0 && *(int *)(*(char **)(o + 0x200) + 0x16c) == ai_Unk6f0cec)
                mine++;
        }
    }
    if (total != 0 && mine == total)
        w = ret = (char *)ai_Unk6f0ce8;
    if (ret != 0) {
        n = (unsigned char)ai_Unk6f0d04[0x30];
        for (i = 0; i < n && *(int *)(ai_Unk6f0d04 + 0x34 + i * 4) == -1; i++)
            ;
        rr = (char *)ai_Unk4cdf00(ai_Unk92cae0[i], ret);
        memcpy(sl, rr, 0x2c);
        sl[0] = *(int *)(ret + 4);
        sl[1] = 5;
        ai_Unk4c5c40(*(int *)(ai_Unk92cae0[i] + 0x10), sl);
    }
    return ret;
}

#define G19_4C4880U ((void (*)(int, int, int, int, unsigned char, int))cm_SendExSellObject)

/* 0x4d3d10, loot of a won fight: a random share (0.07 + 0.001 * 0 .. 49 of the winner's strength 0x509e60, at most 0x9c400)
   goes to the money message 0x4c47e0, then each object of the loser's stock (0x4ffe90 list 1) is moved to the winner
   (0x4c4880, 80 .. 99 % of its count) and its price added; the value goes to the loser's record (0x506430) */
int ai_Unk4d3d10(char *a, char *b, char *d, char *t)
{
    char *oa, *ob, *q;
    int cnt, v;
    float *fp;

    oa = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x116);
    ob = gm_Query(*(int *)(a + 0x5d), 2, 6, 0, 0x2a);
    v = gm_GetGoodsValue(t);
    v = v < 0 ? 0 : v;
    v = v < 0x9c400 ? v : 0x9c400;
    v = (int)(((double)(unsigned int)ai_Unk4e52f0(0x32) * 0.001 + 0.07) * v);
    cm_SendMoveGoods(*(int *)(d + 4), *(int *)(t + 4), ai_OwnPlayer, v);
    if (ob != 0 && oa != 0) {
        for (q = gm_Query(*(int *)(ob + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
            cnt = (int)(((double)(unsigned int)(ai_Unk4e52f0(0x14) + 0x50) * 0.01) * (double)*(int *)(q + 0xe));
            if (cnt < 1)
                cnt = 1;
            G19_4C4880U(*(int *)(oa + 2), *(int *)(ob + 2), *(unsigned short *)q, cnt, ai_OwnPlayer, 0);
            v = (int)(gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * cnt + v);
        }
    }
    q = gm_PlayerRecord(d);
    if (q != 0)
        *(int *)(q + 0x1c) += v;
    return v;
}
/* 0x4d3ea0, the loot of the carts of a won fight: for each of the 3 cart ids at +0x100 of a, the goods of the cart
   (0x4ffe90 chains) go to the winner (0x4c4880, at least 1, f of the count; a free pick-up with b == 0) and their
   price is added; with no winner record half of the value goes to the money message 0x4c47e0; the value goes to
   the record of the attacker side (0x506430) */

int ai_Unk4d3ea0(char *a, char *b, char *d, int unused, float f)
{
    char *ob, *q, *p;
    int cnt, i;
    int dmg = 0;
    float x;

    if (ai_Unk6f0cec == 0 || ai_Unk6f0ce8 == 0)
        return 0;
    {
        ob = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x116);
        for (i = 0; i < 3; i++) {
            if (*(int *)(a + 0x100 + i * 4) != -1) {
                p = gm_Query(0, 2, 7, 1, *(int *)(a + 0x100 + i * 4));
                p = gm_Query(*(int *)(p + 0x14), 1, 0, 0x1dd);
                for (q = gm_Query(*(int *)(p + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
                    gm_ObjectBuildingOwner(q);
                    x = (float)*(int *)(q + 0xe) * f;
                    if (1.0f > x)
                        x = 1.0f;
                    cnt = (int)x;
                    if (ob != 0)
                        G19_4C4880U(*(int *)(ob + 2), *(int *)(q + 6), *(unsigned short *)q, cnt, ai_OwnPlayer, 0);
                    else
                        G19_4C4880U(-1, *(int *)(q + 6), *(unsigned short *)q, cnt, ai_OwnPlayer, 0);
                    dmg = (int)(gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * cnt + dmg);
                }
                if (ob == 0) {
                    dmg = (int)(dmg * 0.5);
                    cm_SendMoveGoods(*(int *)(d + 4), -1, ai_OwnPlayer, dmg);
                }
                q = gm_PlayerRecord((char *)ai_Unk6f0cec);
                if (q != 0)
                    *(int *)(q + 0x1c) += dmg;
            }
        }
    }
    return dmg;
}
/* 0x4d4050, the fight role of a building kind (first byte of its 0x24d-byte record): 0x13 -> 0, 0x10 -> 1, 4 -> 2, else 0 */

int ai_Unk4d4050(unsigned char *b)
{
    char *r = ai_BTypes + *b * 0x24d;
    char k = *r;

    if (k == 0x13)
        return 0;
    if (k == 0x10)
        return 1;
    return k == 4 ? 2 : 0;
}
/* 0x4d4090, the loot list of a won fight: for each of the 3 cart ids at +0x100 of ai_Unk6f0d04 the goods (mode 0 all,
   1 only those of the player, 2 only the others) get a cell of 5 per row (120 x 96 pixels from x, y): their picture
   (0x525 frame, 0xdf + id), the count "%i " plus a suffix, the value "%T"; returns the sum of the values */

int ai_Unk4d4090(int x, int y, int mode)
{
    int off;
    int j = 0;
    int total = 0;
    int px, py;
    int t, t2;
    char *q;
    char buf[0x100];

    for (off = 0x100; off < 0x10c; off += 4) {
        if (*(int *)(off + ai_Unk6f0d04) == -1)
            continue;
        q = gm_Query(0, 2, 7, 1, *(int *)(off + ai_Unk6f0d04));
        q = gm_Query(*(int *)(q + 0x14), 1, 0, 0x1dd);
        for (q = gm_Query(*(int *)(q + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {
            if (mode != 0 && !(mode == 1 && *(int *)(q + 0xa) == ai_PlayerId) &&
                !(mode == 2 && *(int *)(q + 0xa) != ai_PlayerId))
                continue;
            px = x + (j % 5) * 120;
            py = y + (j / 5) * 96;
            d2_WinOpenPic(px, py, 0x525, d2_LastWindow);
            d2_WinOpenPic(px, py, *(unsigned short *)q + 0xdf, d2_LastWindow);
            t = d2_CreateWinText(px - 0x10, py - 0xf, &ch_EmptyName, d2_LastWindow);
            G19_R(t, 0x74, short) = 0x56;
            G19_R(t, 0x14, short) = 0x50;
            G19_R(t, 0x5c, int) = 1;
            sprintf(buf, "%i ", *(int *)(q + 0xe));
            strcat(buf, ai_Unk144a160);
            d2_SetObjText(t, buf);
            t2 = d2_CreateWinText(px - 0x10, py + 0x34, &ch_EmptyName, d2_LastWindow);
            G19_R(t2, 0x74, short) = 0x56;
            G19_R(t2, 0x14, short) = 0x50;
            G19_R(t2, 0x5c, int) = 1;
            G19_5176D0(buf, "%T", (int)gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * *(int *)(q + 0xe));
            d2_SetObjText(t2, buf);
            total = (int)(gm_GoodsPrice(*(unsigned short *)q, ai_OwnPlayer) * *(int *)(q + 0xe) + total);
            j++;
        }
    }
    return total;
}
/* 0x4d4340, the portrait row of the fight: for each of the 16 ids (-1 = none) a column (width = (0x6ed980 + 8 - 0x30) /
   (count + 1), 6 per row, rows of 100 pixels from x, y) with the frame 0x5da, the portrait, the health bar (0x504e00),
   the name and health text ("%1N1 %i%"), the building icon and the state icon (0x3b / 0x37) of the character */

void ai_Unk4d4340(int *ids, int x, int y)
{
    int i, n = 0;
    int w, px, py, v, t, f, ic;
    char *c, *o;
    char buf[0x100];

    for (i = 0; i < 16; i++) {
        if (ids[i] != -1)
            n++;
    }
    w = (*(short *)(d2_LastWindowPtrC + 8) - 0x30) / (n + 1);
    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = ai_Unk4ce130(ids[i]);
        px = (i % 6 + 1) * w + x;
        py = y + i / 6 * 100;
        d2_WinOpenPic(px - 0x10, py, 0x5da, d2_LastWindow);
        d2_WinOpenPic(px - 4, py, *(int *)(c + 0x18c), d2_LastWindow);
        v = gm_SkillPoints(c, 3);
        sv_ValueBar(px, py + 0x3c, (int)(double)(float)v, 0x4eb);
        t = d2_CreateWinText(px - 0x24, py - 0x11, &ch_EmptyName, d2_LastWindow);
        G19_R(t, 0x14, short) = 0x78;
        G19_R(t, 0x5c, int) = 1;
        G19_R(t, 0x74, short) = 0x56;
        G19_5176D0(buf, "%1N1 %i%", *(unsigned short *)c, (int)(gm_HealthRatio(c) * 100.0));
        d2_SetObjText(t, buf);
        o = ai_Unk4cda50(c);
        if (o != 0 && *(int *)o != 0) {
            ic = d2_WinOpenPic(px + 0x28, py + 0xf, *(int *)o + 0xdf, d2_LastWindow);
            d2_ScaleObject(ic, -2);
        }
        if (c[8] == 0) {
            f = (int)-*(float *)(c + 0x1c);
            if ((double)*(int *)(c + 0x24) == (double)f - 20.0)
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
   the loot was taken. Returns 1 when the fight goes on. Rough draft. */

int ai_Unk4d4540(char *a)
{
    int res = 1;
    int st = 0;
    int win, ca, cb, role, edi, i, n, r, v, sel, esi, total;
    int b1, b2, b3, b4;
    float fv;
    char *p;
    int rc[0x2e];
    char tb[0x3f4];
    char tc[0x3f0];

    win = d2_OpenForm(0, 0, "cutscenes\\combat_start");
    ((void (*)(int, int))d2_PlaceMainWindow)(win, 3);
    ai_Unk6f0cf8 = 0;
    ca = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x14));
    cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
    role = ai_Unk4d4050((unsigned char *)cb);
    d2_SetForm(win, 1);
    if (*(char *)(ai_Unk6f0cec + 2) == 6) {
        if (a[0x7c] & 2) {
            io_PrintText(0x1138, role + 0x1125, cb, *(unsigned short *)ai_Unk6f0ce8, ca);
            d2_SetForm(win, 2);
            edi = io_PrintText(0x113a, *(unsigned short *)ai_Unk6f0ce8, ca);
        } else if (a[0x7c] & 1) {
            edi = st;
            for (i = 0; i < 3; i++) {
                if (*(int *)(a + 0x100 + i * 4) != -1)
                    edi++;
            }
            st = edi;
            fv = gm_GetSkill(ai_Unk6f0cec, 3);
            io_PrintText(0x112e, cb, *(unsigned short *)ai_Unk6f0ce8);
            d2_SetForm(win, 2);
            io_PrintText(0x112f, edi, 0xc10);
            if (fv >= 0.5) {
                if (*(int *)(a + 0x110) != 0)
                    io_PrintText(0x1134);
                else
                    io_PrintText(0x1135);
            }
            if (fv >= 0.75) {
                v = gm_SkillPoints(ai_Unk6f0ce8, 3);
                r = (int)(v * 0.023809524f);
                if (r > 4)
                    r = 4;
                r = r < 0 ? 0 : r;
                io_PrintText(0x1131, r + 0x1128);
            }
            if (*(char *)(ai_Unk6f0ce8 + 9) == 0)
                edi = io_PrintText(0x1133, 0x1136);
            else
                edi = io_PrintText(0x1133, 0x1137);
        } else if (a[0x7c] & 4) {
            io_PrintText(0x113b, cb, *(unsigned short *)ai_Unk6f0ce8);
            d2_SetForm(win, 2);
            edi = io_PrintText(0x113c, *(unsigned short *)ai_Unk6f0ce8, ca);
        }
        d2_SetForm(win, 1);
        io_PrintText(0x1132);
        ai_Unk4d4340((int *)(a + 0xc0), 0, 0xb4);
        d2_GetWinObj(win, 2, edi);
        edi++;
        d2_GetWinObj(win, 2, edi);
        while (sv_RunFrame(ai_Unk4d4540, 0x7d6c6)) {
            if (dm_L.rup != 0) {
                sv_PopRequest = 1;
                ai_Unk6f0cf8 = 1;
                res = 0;
            }
            if (d2_ClickedId == 0) {
                res = 1;
                sv_PopRequest = 1;
            } else if (d2_ClickedId == 1) {
                ai_Unk6f0cf8 = 1;
                res = 0;
                sv_PopRequest = 1;
            }
        }
    }
    if ((a[0x7c] & 1) && *(char *)(ai_Unk6f0ce8 + 2) == 6 && ai_Unk6f0cf8 == 0) {
        memset(rc, 0, sizeof(rc));
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
                io_PrintText(0x113d, st, 0xc10, p, *(unsigned short *)ai_Unk6f0cec, esi);
        }
        v = gm_SkillPoints(ai_Unk6f0ce8, 3);
        r = (int)(v * 0.023809524f);
        if (r > 4)
            r = 4;
        if (*(int *)(a + 0x110) != 0)
            io_PrintText(0x113e, (r < 0 ? 0 : r) + 0x1128);
        else
            io_PrintText(0x113f);
        ai_Unk4d4090(0x10, 0x96, 0);
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
        esi = io_PrintText(0x1141, (int)(v * 0.1), (int)(v * 0.5), (int)(v * 0.8));
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
        while (sv_RunFrame(ai_Unk4d4540, 0x7d6c6)) {
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
                if ((unsigned short)util_RandomU(0x5a) + 10 > st) {
                    txt_Format(tc, 0x1143);
                    ai_Unk4e5470(tc, 0);
                    sv_PopRequest = 1;
                    res = 1;
                } else {
                    cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                    txt_Format(tb, 0x1142);
                    ai_Unk4e5470(tb, 0);
                    fv = (float)(st * 0.01);
                    sv_PopRequest = 1;
                    b4 = 0;
                    ai_Unk6f0cf8 = 1;
                    ai_Unk6f0cfc = ai_Unk4d3ea0(ai_Unk6f0d04, cb, ai_Unk6f0cec, ai_Unk6f0ce8, fv);
                }
            }
        }
    } else if ((a[0x7c] & 1) && *(char *)(ai_Unk6f0ce8 + 2) != 6 && ai_Unk6f0cf8 == 0) {
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
            if ((unsigned short)util_RandomU(0x5a) + 10 > st) {
                ai_Unk6f0cfc = -1;
                res = 1;
            } else {
                cb = (int)gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
                fv = (float)(st * 0.01);
                ai_Unk6f0cf8 = 1;
                ai_Unk6f0cfc = ai_Unk4d3ea0(ai_Unk6f0d04, cb, ai_Unk6f0cec, ai_Unk6f0ce8, fv);
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
#define G20_ATT ((char *)ai_Unk6f0ce8)
#define G20_DEF ((char *)ai_Unk6f0cec)
/* the pump of the dialog: the click on a button (f8a668 == 0) or the abort flag closes it */
#define G20_PUMP()                                                                                      \
    while (sv_RunFrame(ai_Unk4d4d70, 0x7d6c6)) {                                                       \
        if (dm_L.rup != 0 || d2_ClickedId == 0)                                                    \
            sv_PopRequest = 1;                                                                           \
    }
/* the winner takes a share of the loser's money (message 0x4c47e0) and of the goods of ob (80 .. 99 %), the text lines
   go to ba; the money part v is added to the winner's record afterwards (0x506430, +0x4d of the building) */
#define G20_TAKE()                                                                                      \
    ba[0] = ch_EmptyName;                                                                               \
    memset(ba + 1, 0, 0x3ff);                                                                           \
    txt_Format(ba, 0x11a5, G20_US(G20_DEF), ca);                                                      \
    ai_Unk4e5470(ba, 0);                                                                                \
    ob = gm_Query(*(int *)(ca + 0x5d), 2, 6, 0, 0x2a);                                              \
    if (ob == 0)                                                                                        \
        ob = gm_Query(*(int *)(ca + 0x5d), 2, 6, 0, 0x116);                                         \
    v = gm_GetGoodsValue(ai_Unk6f0ce8);                                                                     \
    v = v < 0x9c400 ? v : 0x9c400;                                                                      \
    v = (int)(((double)(unsigned int)ai_Unk4e52f0(0x32) * 0.001 + 0.07) * (v < 0 ? 0 : v));             \
    txt_Format(ba, "$Z%T$A", v);                                                                      \
    cm_SendMoveGoods(*(int *)(G20_DEF + 4), *(int *)(G20_ATT + 4), ai_OwnPlayer, v);                        \
    if (ob != 0 && obj != 0) {                                                                          \
        for (q = gm_Query(*(int *)(ob + 0x14), 1, 5); q != 0; q = (char *)gm_QueryNext()) {         \
            cnt = (int)((double)(unsigned int)(ai_Unk4e52f0(0x14) + 0x50) * 0.01 * *(int *)(q + 0xe));  \
            if (cnt < 1)                                                                                \
                cnt = 1;                                                                                \
            sprintf(tmp, "%i %s$A", cnt, G20_NAME(G20_US(q)));                                          \
            strcat(ba, tmp);                                                                            \
            if (G20_ME == ai_Unk60cb5c)                                                                 \
                G19_4C4880U(*(int *)(obj + 2), *(int *)(ob + 2), G20_US(q), cnt, ai_OwnPlayer, 0);      \
            v = (int)(gm_GoodsPrice(G20_US(q), ai_OwnPlayer) * cnt + v);                                 \
        }                                                                                               \
    }                                                                                                   \
    d2_SetForm(win, 1);                                                                                 \
    io_PrintText(ba);                                                                                   \
    q = gm_PlayerRecord(G20_DEF);                                                                          \
    if (q != 0)                                                                                         \
        *(int *)(q + 0x1c) += v;                                                                        \
    if (*(int *)(a + 0x10c) != -1) {                                                                    \
        q = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));                                                    \
        *(int *)(q + 0x4d) += v;                                                                        \
    }
/* the winner loses face: the text 0x11a6 and a drop of 0x14 - random of the defender's mood (0x50cad0) */
#define G20_FACE()                                                                                      \
    ba[0] = ch_EmptyName;                                                                               \
    memset(ba + 1, 0, 0x3ff);                                                                           \
    txt_Format(ba, 0x11a6, G20_US(G20_DEF), ca);                                                      \
    ai_Unk4e5470(ba, 0);                                                                                \
    if (G20_ME == ai_Unk60cb5c)                                                                         \
        gm_ChangeBuildingState(ca, 0x14 - ai_Unk4e52f0(0x14));
/* nothing taken: the text 0x11a7 and the fight with the tavern keepers' men: every character of the table of 0x770b80
   that is a guard (kinds 10 .. 12) gets 0x4c4b00 */
#define G20_GUARDS()                                                                                    \
    txt_Format(bb, 0x11a7, G20_US(G20_DEF), G20_US(G20_DEF));                                         \
    ai_Unk4e5470(bb, 0);                                                                                \
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
   2 a share of money and goods is taken, 3 the winner loses face, 0 the guards of the town are called). Rough draft. */
void ai_Unk4d4d70(char *a)
{
    char ba[0x400];
    char bb[0x400];
    char tmp[0x100];
    char *obj, *side, *c, *ca, *ob, *q, *p, *kr;
    int win, i, n, role = 0, v, cnt, total, na, nb, id, k;
    int h, row, col, px, py;

    d2_CopyCaption(&ch_EmptyName);
    sv_SetStatusText(&ch_EmptyName);
    obj = 0;
    if (*(int *)(a + 0x10c) != -1) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        obj = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x116);
        if (obj == 0)
            obj = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x2a);
    }
    if (ai_Unk60cb5c == G20_ME) {
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0x80 + i * 4);
            if (id != -1 && (c = gm_FindChar(id)) != 0)
                ai_Unk4cdb80(c);
        }
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0xc0 + i * 4);
            if (id != -1 && (c = gm_FindChar(id)) != 0)
                ai_Unk4cdb80(c);
        }
    }
    if (ai_Unk6f0cf4 == 0) {
        na = 0;
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0x80 + i * 4);
            if (id != -1) {
                c = ai_Unk4ce130(id);
                if (c[8] != 0 && c[8] != 4 && *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x218) != 1)
                    na++;
            }
        }
        nb = 0;
        for (i = 0; i < 16; i++) {
            id = *(int *)(a + 0xc0 + i * 4);
            if (id != -1) {
                c = ai_Unk4ce130(id);
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
        if (ai_Unk6f0cf4 != ai_Unk6f0cec)
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
    ai_Unk4d4340((int *)(a + 0xc0), 0, 100);
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
    ai_Unk4d4340((int *)(a + 0x80), 0, 0x12c);
    G20_PUMP();
    d2_SetForm(win, 1);
    io_PrintText("$C");
    io_PrintText(0x1197);
    if (a[0x7c] & 1) {
        if (G20_US(&ai_PlayerId) == G20_US(G20_ATT))
            ai_Unk4d4090(0x24, 0x32, 2);
        else if (G20_US(&ai_PlayerId) == G20_US(G20_DEF)) {
            v = ai_Unk4d4090(0x24, 0x32, side == G20_DEF ? 0 : 1);
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
                    if (obj != 0 && ai_Unk60cb5c == G20_ME) {
                        gm_ObjectBuildingOwner(q);
                        G19_4C4880U(*(int *)(obj + 2), *(int *)(q + 6), G20_US(q), *(int *)(q + 0xe), ai_OwnPlayer, 0);
                    }
                    gm_GoodsPrice(G20_US(q), ai_OwnPlayer);
                }
            }
        }
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
        n = ai_Unk4e5470(ba, 0x11);
        k = ca[0];
        if (k == 0x1e || k == 0x20 || k == 0x1f)
            n++;
        if (n == 1) {
            kr = ai_BTypes + (unsigned char)ca[0] * 0x24d;
            if (*kr == 4 || *kr == 0x10)
                ai_Unk4b44e0(G20_DEF, 0x1c);
            cm_SendOwnership(*(int *)(ca + 1), *(int *)(G20_DEF + 4), *(int *)(G20_DEF + 4));
            txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
            ai_Unk4e5470(ba, 0);
        } else if (n == 2) {
            G20_TAKE();
            G20_PUMP();
        } else if (n == 3) {
            G20_FACE();
        } else if (n == 0) {
            G20_GUARDS();
        }
    } else if (a[0x7c] & 4) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        if (ai_BTypes[(unsigned char)c[0] * 0x24d] == 0x13) {
            total = 0;
            for (i = 0; i < 16; i++) {
                id = *(int *)(a + 0x80 + i * 4);
                if (id == -1)
                    continue;
                c = ai_Unk4ce130(id);
                col = i % 4;
                row = i / 4 + 1;
                px = col * 80 + 0x30;
                py = row * 100;
                d2_WinOpenPic(px, py, 0x5db, d2_LastWindow);
                d2_WinOpenPic(px + 2, py, *(int *)(c + 0x18c), d2_LastWindow);
                sv_ValueBar(px, py + 0x3c, (unsigned char)c[0x83], 0x4eb);
                h = 0;
                if (c[8] == 0) {
                    v = (int)((unsigned char)c[0x83] * 0.023809524f + 1.0f);
                    d2_WinOpenPic(px + 6, py + 6, 0x3b, d2_LastWindow);
                    h = gm_PlayerFactor(v * 40, d2_LastWindow);
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
            G20_PUMP();
        }
    } else if ((a[0x7c] & 2) && side == G20_DEF && G20_DEF[2] != 6 && G20_DEF[2] != 7) {
        gm_TitleJob(G20_US(G20_DEF), gm_BuildingJob((unsigned char)*kr));
        k = (unsigned short)util_RandomU(3) + 1;
        if (!gm_TitleJob(G20_US(G20_DEF), 0) || *kr == 0xd || *kr == 0xc || *kr == 0xb) {
            if (k == 2) {
                G20_TAKE();
                G20_PUMP();
            } else if (k == 3) {
                G20_FACE();
            } else if (k == 1) {
                G20_GUARDS();
            } else {
                txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
                ai_Unk4e5470(ba, 0);
            }
        } else {
            txt_Format(ba, 0x11a4, G20_US(G20_DEF), ca);
            ai_Unk4e5470(ba, 0);
        }
    }
    di_ClearEvents(0x10);
    d2_CloseView(win);
}
/* 0x4d60b0, the speech texts of a fight (a: the fight record): for both leaders (the characters of +0x34 / +0x38) one
   line each, by the kind of the fight (bit 1: 0x11e0 / 0x11e1, bit 2: 0x11e3 / 0x11e4 with the role of the target
   building 0x4d4050, bit 4: 0x11e6 / 0x11e7), the suffix of the kind (0x143e948 / 0x143e954 / 0x143e960) appended and
   handed to 0x4e48b0. 0 when a leader is missing or the leaders are the same, a cart has no goods (0x50aa00) or the
   target is gone. Rough draft. */

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
        ai_Unk4e48b0(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e1, G20_US(ca));
        strcat(buf, ai_Unk143e948);
    } else if (a[0x7c] & 2) {
        role = ai_Unk4d4050((unsigned char *)gm_FindBuildingById(*(int *)(a + 0x10c))) + 0x1125;
        txt_Format(buf, 0x11e3, role, G20_US(cb), edi);
        strcat(buf, ai_Unk143e954);
        ai_Unk4e48b0(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e4, G20_US(ca), role, edi);
        strcat(buf, ai_Unk143e954);
    } else if (a[0x7c] & 4) {
        txt_Format(buf, 0x11e6, G20_US(cb));
        strcat(buf, ai_Unk143e960);
        ai_Unk4e48b0(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e7, G20_US(ca));
        strcat(buf, ai_Unk143e960);
    } else
        return 1;
    ai_Unk4e48b0(cb, a, buf, &ch_EmptyName);
    return 1;
}
/* 0x4d63e0, a fight starts (a: the fight record): characters that are gone are dropped from both id lists, a cart
   without goods or no defender ends it (bad = 2); with the player present (0x6f0d8c == 0) the fight scene is built and
   fought (0x4d0fa0 .. 0x4d1e00), 0x4d4540 asks for the ransom and 0x4d4d70 shows the end, else (remote) it is decided
   at once by strength (sum of 0x4cdcf0 + random 30 per side, losers lose up to 20 * random 10 health) and
   the loot goes through 0x4d3d10 / 0x4d3ea0. Rough draft. */

void ai_Unk4d63e0(char *a)
{
    char buf[0x800];
    int rc[0x2e];
    int bad = 0, nb = 0, seed, i, id, sa, sb, w, n, r;
    char *c, *p, *win, *e1, *e2;
    int *e;

    seed = ai_Unk4e52c0();
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0x80 + i * 4);
        if (id != -1 && gm_FindChar(id) == 0) {
            bad = 1;
            *(int *)(a + 0x80 + i * 4) = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0xc0 + i * 4);
        if (id != -1) {
            if (gm_FindChar(id) == 0) {
                bad = 1;
                *(int *)(a + 0xc0 + i * 4) = -1;
            } else
                nb++;
        }
    }
    if (a[0x7c] & 1) {
        for (i = 0; i < 3; i++) {
            id = *(int *)(a + 0x100 + i * 4);
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
    if (gm_BuildQuery(1, 1, *(int *)(a + 0x10c)) == 0 || bad == 2 || nb == 0)
        return;
    if (cm_SkipEvents == 0) {
        ai_Unk60d364 = 200;
        ai_Unk60cb5c = *(int *)(a + 0xc);
        ai_Unk4d0fa0(a);
        ai_Unk4d0a30();
        ai_Unk4d1c20(a + 0xc0, (int)"OFF");
        ai_Unk4d1c20(a + 0x80, (int)"DEF");
        ai_Unk4d1e00(a);
        if (ai_Unk6f244c != 0)
            d3_SetLightSet(2, 0, 0x3f800000, 1);
        else
            sv_UpdateSkyPhase(ai_Time);
        ai_Unk6f0cf4 = 0;
        ai_Unk6f0cf8 = 0;
        ai_Unk6f0cfc = 0;
        ai_Unk4d4540(a);
        ai_Unk4e52a0(seed);
        ai_Unk4d0a30();
        ai_Unk4d0a30();
        ai_Unk4e3830(a, 0);
        if (ai_Unk6f0cf4 != 0 || ai_Unk6f0cf8 == 0) {
            if (ai_Unk6f0cf4 == 0)
                ai_Unk4e3830(a, 0);
        } else
            ai_Unk4e3830(a, 0);
        if (ai_Unk6f0cf8 == 0) {
            for (i = 0; i < 16; i++) {
                if (ai_Unk9304c0[i].h != -1)
                    d2_RemoveObj(ai_Unk9304c0[i].h);
                ai_Unk9304c0[i].h = -1;
                ai_Unk9304c0[i].v = 0;
                ai_Unk9304c0[i].c = 0;
            }
            ai_Unk4d4d70(a);
            ai_Unk4d1eb0(a);
            ai_Unk4d1a10(a);
            return;
        }
        if (ai_Unk6f0cfc != 0 && G20_DEF[2] == 6) {
            e1 = 0;
            c = gm_BuildQuery(1, 1, *(int *)(ai_Unk6f0d04 + 0x10c));
            if (c != 0)
                e1 = gm_Query(*(int *)(c + 0x5d), 2, 6, 0, 0x116);
            if (ai_Unk6f0cfc > 0) {
                if (e1 == 0)
                    txt_Format(buf, 0x1144, ai_Unk6f0cfc);
                else
                    txt_Format(buf, 0x1145, ai_Unk6f0cfc);
            } else
                txt_Format(buf, 0x1146);
            ai_Unk4e5470(buf, 0x10);
        }
        ai_Unk4d1a10(a);
        return;
    }
    sa = 0;
    sb = 0;
    ai_Unk6f0cec = gm_FindChar(*(int *)(a + 0x34));
    ai_Unk6f0ce8 = gm_FindChar(*(int *)(a + 0x38));
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0x80 + i * 4);
        if (id != -1 && (c = gm_FindChar(id)) != 0 && c[8] != 0 && c[8] != 4) {
            sa = (int)(ai_Unk4cdcf0(c) + sa);
            r = (unsigned short)util_RandomU(10);
            gm_AddHealth(G20_US(c), -r * 20);
        }
    }
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0xc0 + i * 4);
        if (id != -1 && (c = gm_FindChar(id)) != 0 && c[8] != 0 && c[8] != 4) {
            sb = (int)(ai_Unk4cdcf0(c) + sb);
            r = (unsigned short)util_RandomU(10);
            gm_AddHealth(G20_US(c), -r * 20);
        }
    }
    if ((unsigned short)util_RandomU(30) + sb > (unsigned short)util_RandomU(30) + sa)
        win = G20_DEF;
    else
        win = G20_ATT;
    for (i = 0; i < 16; i++) {
        id = *(int *)(a + 0x80 + i * 4);
        if (id != -1 && (c = gm_FindChar(id)) != 0)
            ai_Unk4cdb80(c);
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(a + 0xc0 + i * 4) != -1 && (c = gm_FindChar(*(int *)(a + 0x80 + i * 4))) != 0)
            ai_Unk4cdb80(c);
    }
    if (a[0x7c] & 2) {
        if (win == G20_DEF) {
            e1 = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
            e2 = gm_BuildQuery(1, 1, *(int *)(a + 0x14));
            ai_Unk4d3d10(e2, e1, G20_DEF, G20_ATT);
        }
    } else if (!(a[0x7c] & 4) && (a[0x7c] & 1) && win == G20_DEF) {
        c = gm_BuildQuery(1, 1, *(int *)(a + 0x10c));
        ai_Unk4d3ea0(a, c, G20_DEF, ai_Unk6f0ce8, 100.0f);
    }
}
/* 0x4d6930, the result of the fight in the record r (+0x354 / +0x358): 0x6f0cf8 set -> 0 and the ransom 0x6f0cfc, else 1 */

void ai_Unk4d6930(char *r)
{
    if (ai_Unk6f0cf8 != 0) {
        *(int *)(r + 0x354) = 0;
        *(int *)(r + 0x358) = ai_Unk6f0cfc;
    } else
        *(int *)(r + 0x354) = 1;
}
/* 0x4d6970, one frame of the fight UI (r: the fight record): the view 0x60cb64 sized to the fight area, the redraws
   0x525ff0 .. 0x4ce830, and the click on the button 0x60cb68 (while 0x6f0d88 / 0x6f0cf4 are clear) sends message 0x50 with
   the fight record of the player's character (0x4cdf00) marked 5, else the dialog is closed (0x6f22a0) */

void ai_Unk4d6970(char *r)
{
    int sl[11];
    int *q;

    d2_SetObjValue(ai_Unk60cb64, 0, ai_Unk60d364, ai_Unk6f0d84 - ai_Unk6f0d80 + 1);
    sv_BorderCameraView();
    ai_Unk4cf9c0();
    ai_Unk4cf670();
    ai_Unk4cfeb0();
    ai_Unk4d0740(r);
    ai_Unk4cf000();
    ai_Unk4ced80(r);
    ai_Unk4ce300();
    ai_Unk4ce830();
    if (ai_Unk6f0d88 == 0 && ai_Unk6f0cf4 == 0) {
        if (d2_ClickedId != -1 && d2_PointerObj == ai_Unk60cb68) {
            q = ai_Unk4cdf00(r, ai_Unk6f0d74);
            memcpy(sl, q, 0x2c);
            sl[0] = *(int *)(ai_Unk6f0d74 + 4);
            sl[1] = 5;
            ai_Unk4c5c40(*(int *)(r + 0x10), sl);
        }
    } else
        sv_PopRequest = 1;
}
/* 0x4d6a50, builds the fight UI: the view "cutscenes\combat" (0x6f0d64), the button 0x60cb68 (text 0x143e8b4) and the
   bar 0x60cb64 at the bottom, then the portrait boxes (0x4cfe70 / 0x4cfc10) */

void ai_Unk4d6a50()
{
    int v, t;

    v = d2_OpenForm(0, 0, "cutscenes\\combat");
    ai_Unk6f0d64 = v;
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    t = d2_WinCreateSlider(0x10, 0xc, 0xbf, ai_Unk143e8b4, d2_LastWindow);
    ai_Unk60cb68 = t;
    G19_R(t, 0x5c, int) = 1;
    d2_CenterObj(ai_Unk60cb68);
    t = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 0x190) / 2, 0x37, 0x190, 0, ai_Unk60d364, 0x83, 0x42, d2_LastWindow);
    ai_Unk60cb64 = t;
    G19_R(t, 0x74, short) = 0x54;
    G19_R(*(int *)(ai_Unkf8f39c - 0x10 + d2_LastWindow * 0x3c4), 0x4c, int) = 1;
    d2_SetForm(ai_Unk60d368, 0);
    io_PrintText("$C");
    ai_Unk4cfe70();
    ai_Unk4cfc10();
}
/* 0x4d6b70, closes the fight UI: the view 0x6f0d64, 0x4d08c0, the portrait boxes */

void ai_Unk4d6b70()
{
    d2_CloseView(ai_Unk6f0d64);
    ai_Unk6f0d64 = 0;
    ai_Unk4d08c0();
    ai_Unk4cfe70();
}
/* 0x4d6ba0, the action of one fighter slot f (+0 id, +4 active, +0x10 target id, +0x14 / +0x18 the attacked place,
   +0x1d hit, +0x20 kill, +0x24 damage, +0x28 the target's new health): the target's health is set, and the attacker
   gets the strike animation / event 0x4d3550 (damage 1.4), for the kinds 0x174 / 0x176 the bomb events 0x4d3980 / 0x4d3a30
   (0x174 also picks the pot spot 0x4d2170 and sends 0x4c5bc0). The slot is cleared after a kill. Rough draft. */

void ai_Unk4d6ba0(char *f)
{
    char *h = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *c, *tgt, *k, *side, *x, *ev, *s, *d;
    int dmg, kill, ox, oy, mode;
    float py;

    if (*(int *)(f + 4) == 0 || *(int *)f == -1)
        return;
    c = ai_Unk4ce130(*(int *)f);
    k = ai_Unk4cda50(c);
    side = *(char **)(c + 0x16c);
    tgt = ai_Unk4ce130(*(int *)(f + 0x10));
    if (tgt != 0)
        *(int *)(tgt + 0x24) = *(int *)(f + 0x28);
    if (!(k != 0 && (*(int *)k == 0x174 || *(int *)k == 0x176))) {
        if (f[0x1d] != 0 && ((x = *(char **)(*(char **)(c + 0x184) + 0x128)) == 0 || x[9] == 0x2d || x[9] == 0x3a)) {
            if ((unsigned short)util_RandomU(10) > 8) {
                if (*(int *)(side + 4) == *(int *)(G20_DEF + 4))
                    snd_QueueVoice(G20_US(c), ai_Unk6f0d2c, "_K_C2C_ANGRIFF", -1, 500);
                else
                    snd_QueueVoice(G20_US(c), ai_Unk6f0d30, "_K_C2C_ANGRIFF", -1, 500);
            }
            kill = *(int *)(f + 0x20);
            dmg = *(int *)(f + 0x24);
            if (kill != 0)
                *(int *)(tgt + 0x24) -= dmg;
            if (k == 0)
                ch_QueueAnim(*(char **)(c + 0x184), "kampf/schlagen1", 1);
            else {
                if (*(int *)k == 0)
                    mode = kill != 0 ? 8 : 2;
                else
                    mode = kill != 0 ? 6 : 2;
                ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3550, mode, 0x2e, 1);
                *(float *)(ev + 0x178) = 1.4f;
                *(char **)(ev + 0x17c) = tgt;
                *(char **)(ev + 0x180) = c;
                *(int *)(ev + 0x184) = dmg;
                *(int *)(ev + 0x188) = *(int *)(side + 4);
                for (s = k + 0x11, d = ev + 0xf0; (*d++ = *s++) != 0;)
                    ;
                if ((*(int *)k == 0x16e || *(int *)k == 0x172) && kill != 0) {
                    *(int *)f = -1;
                    *(int *)(f + 4) = 0;
                }
            }
            ch_AddAction(*(char **)(c + 0x184), 0x3b, ai_Unk4e52f0(0x1e) + 0x8c);
        }
    }
    if (*(int *)k == 0x174 && *(int *)(f + 0x20) != 0) {
        ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3980, 6, 0x2e, 1);
        *(char **)(ev + 0x17c) = tgt;
        *(char **)(ev + 0x180) = c;
        *(int *)(ev + 0x184) = *(int *)(f + 0x14);
        *(int *)(ev + 0x188) = *(int *)(f + 0x18);
        strcpy(ev + 0xf0, "spezial/einhaendig_vom_boden_nehmen");
        if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c) {
            if (!d3_GetHeightAt(h, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, &ai_Unkfa5f00, &ai_Unkfa5f04, &py))
                return;
            ai_Unk4d2170(c, ai_Unkfa5f00, ai_Unkfa5f04, 8, &ox, &oy);
            ai_Unk4c5bc0(*(char **)(*(char **)(c + 0x184) + 0x34), ox, oy, 0);
            *(int *)f = -1;
            *(int *)(f + 4) = 0;
        }
    }
    if (*(int *)k == 0x176 && f[0x1d] != 0) {
        ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3a30, 5, 0x2e, 1);
        *(char **)(ev + 0x17c) = c;
        *(int *)(ev + 0x180) = *(int *)(f + 0x14);
        *(int *)(ev + 0x184) = *(int *)(f + 0x18);
        *(int *)(ev + 0x188) = *(int *)(side + 4);
        for (s = k + 0x11, d = ev + 0xf0; (*d++ = *s++) != 0;)
            ;
        ch_AddAction(*(char **)(c + 0x184), 0x3b, ai_Unk4e52f0(0x1e) + 0x8c);
        sb_PlaySampleI(ai_Unk6f0d14, "topfwerfen_s", 1, 0x3f, 0x7f, 0, 0);
        *(int *)f = -1;
        *(int *)(f + 4) = 0;
    }
}
/* 0x4d6f60, the fighter slot f (+0 id, +4 active) of an active character: 0x405610 on its object */

void ai_Unk4d6f60(char *f)
{
    char *c;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        if (c != 0)
            ch_StopActions(*(char **)(c + 0x184));
    }
}
/* 0x4d6f90, the cheer of the winner in the slot f: event 0x4d3ac0 with the animation "gestik/jubeln" */

void ai_Unk4d6f90(char *f)
{
    char *c, *ev;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3ac0, 5, 0x2e, 1);
        *(char **)(ev + 0x17c) = c;
        strcpy(ev + 0xf0, "gestik/jubeln");
    }
}
/* 0x4d7000, the fighter slot f picks up the object lying at its place (+0x10): the object (0x449690) is marked as owned
   by the character's side, event 0x4d3ae0 */

void ai_Unk4d7000(char *f)
{
    char *c, *side, *o, *ev;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        side = *(char **)(c + 0x16c);
        ch_StopActions(*(char **)(c + 0x184));
        o = (char *)d3_FindObject(0, 0x40, 0, f + 0x10);
        if (o != 0) {
            *(int *)(*(char **)(o + 0x200) + 0xa) = G20_US(side);
            ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3ae0, 5, 0x2e, 1);
            *(char **)(ev + 0x17c) = c;
            *(char **)(ev + 0x180) = o;
            strcpy(ev + 0xf0, "spezial/einhaendig_vom_boden_nehmen");
        }
    }
}
/* 0x4d70a0, the fighter slot f puts down the object it carries (+0x1ac) at its place */

void ai_Unk4d70a0(char *f)
{
    char *c, *o, *p, *a;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        ch_StopActions(*(char **)(c + 0x184));
        d3_FindObject(0, 0x40, 0, f + 0x10);
        o = *(char **)(c + 0x1ac);
        if (o != 0) {
            p = *(char **)(*(char **)(c + 0x184) + 0x34);
            d3_SetWorldPosRot(o, p + 0x4c, p + 0x18c);
            d3if_Show(o, 1);
            *(int *)(c + 0x1ac) = 0;
        }
        a = (char *)ch_AddAction(*(char **)(c + 0x184), 0x2e, 1);
        strcpy(a + 0xf0, "spezial/einhaendig_vom_boden_nehmen");
    }
}
/* 0x4d7150, the character of the record r is the winner so far (0x6f0cf4): text 0x11b7 */

void ai_Unk4d7150(char *r)
{
    char buf[0x100];
    char *c;

    c = gm_FindChar(*(int *)r);
    ai_Unk6f0cf4 = (int)c;
    txt_Format(buf, 0x11b7, G20_US(c));
    sv_SetStatusText(buf);
}
/* 0x4d7190, the flag of the escape dummy: the object o at the place of the fighter slot f (+0x10) gets the character of
   the slot as owner (+0x200): the old flag (+0x204) is deleted and a new "ub_FLUCHTFLAGGE" object with the animation
   (and the texture set of the character's side) is put at the dummy; the character's flag bit 4 and text 0x11b8 for the
   slot's character. Rough draft. */

void ai_Unk4d7190(char *f)
{
    char *c = 0;
    char *o, *r, *side;
    char buf[0x100];
    float pos[4];
    float rot[3];
    char k;

    if (*(int *)(f + 4) != 0) {
        o = (char *)d3_FindObject(0, 0x40, 0, f + 0x10);
        if (*(int *)f != -1) {
            c = ai_Unk4ce130(*(int *)f);
            (*(char **)(c + 0x184))[0x8d] |= 4;
            txt_Format(buf, 0x11b8, G20_US(c));
            sv_SetStatusText(buf);
        }
        if (*(char **)(o + 0x200) != c) {
            if (*(int *)(o + 0x204) != 0) {
                d3if_Delete((struct D3OBJECT *)*(int *)(o + 0x204));
                *(int *)(o + 0x204) = 0;
            }
            if (c != 0) {
                rot[0] = 0;
                rot[1] = 0;
                rot[2] = 0;
                d3_PointToWorld(o, o + 0x4c, pos);
                r = (char *)d3_CreateObject(0, pos, rot, "ub_FLUCHTFLAGGE");
                ai_Unk935980[ai_Unk6f0d58] = (char *)d3_CreateObject(0, pos, rot, &ch_EmptyName);
                d3t_PlayAnim(r, "sonstiges/anim_FLUCHTFLAGGE.baf", 1);
                side = *(char **)(c + 0x16c);
                if (side != 0 && *(int *)(side + 0x54) != 0 && (side[2] == 5 || side[2] == 6 || side[2] == 7)) {
                    k = side[0x54] + 0x67;
                    if (k != 0)
                        d3if_SelectTextureSet(r, *(int *)(r + 0x1cc), k, 1);
                }
                sprintf(r, "CONQUER_FLAG");
                *(int *)(r + 0x200) = 0;
                r[0x212] = (r[0x212] & 0xf7) | 0x44;
                *(int *)(r + 0x220) = 4;
                *(int *)(r + 0x224) = 1;
                *(char **)(o + 0x204) = r;
            } else
                *(int *)(o + 0x204) = 0;
        }
        *(char **)(o + 0x200) = c;
    }
}
/* 0x4d7360, the attack of the character c on the character g in the fight record rec: -1 when c cannot attack (no
   ammunition for the ranged kinds, the animation of c is busy, g not on the map); in range the hit is rolled
   (k+4 percent of c's skill against random 255) with the damage (k+5 .. k+6 percent of g's health, 0x5ba6b0 / 0x2b9
   scales it), and the action record (0x2c bytes, id of c, 2, id of g, the hit / damage) goes to message 0x4c5dc0;
   a player's attack also arms the AI of the other side (0x4cffd0). Out of range c walks to g (0x4c5bc0). Rough draft. */

int ai_Unk4d7360(char *rec, char *c, char *g)
{
    char *h = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int s[11];
    char *k, *x, *other, *r2;
    int i, n, hit, dmg, ix, iz, id;
    unsigned short r;
    float py;

    memset(s, 0, sizeof(s));
    s[0] = *(int *)(c + 4);
    s[1] = 2;
    s[4] = *(int *)(g + 4);
    if (c[0x1c4] == 1 || c[0x1c4] == 2 || c[0x1c4] == 3)
        ((char *)s)[0x1c] = 1;
    k = ai_Unk4cda50(c);
    if (*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2) {
        x = ai_Unk4cdac0(c);
        if (x != 0 && *(int *)(x + 0x20) <= 0)
            return -1;
    }
    if (*(int *)k == 0x154 || *(int *)k == 0x156 || *(int *)k == 0x158 || *(int *)k == 0x16e || *(int *)k == 0x172 ||
        *(int *)k == 0 || *(int *)k == 0x176) {
        if (ai_Unk4ce1b0(c, *(char **)(*(char **)(g + 0x184) + 0x34)) > *(float *)(k + 0xc))
            goto walk;
    }
    if (*(int *)(*(char **)(c + 0x184) + 0x128) != 0)
        return -1;
    r = util_RandomU(0xff);
    x = *(char **)(c + 0x16c);
    if (r > 0xff - (int)((unsigned char)k[4] * 0.01 * (unsigned char)c[0x83])) {
        n = (unsigned char)k[6] - (unsigned char)k[5];
        hit = 1;
        dmg = (int)((double)(unsigned int)(ai_Unk4e52f0(n) + (unsigned char)k[5]) * *(float *)(g + 0x1c) * 0.01);
    } else {
        hit = 0;
        dmg = 0;
    }
    if (x != 0 && ai_InventoryHas(x, 0x2b9) != 0)
        dmg = (int)(dmg * 1.5f);
    ((char *)s)[0x1d] = 1;
    s[8] = hit;
    s[9] = dmg;
    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && *(int *)(k + 0x58) == 0) {
        other = 0;
        n = (unsigned char)ai_Unk6f0d04[0x30];
        for (i = 0; i < n; i++) {
            if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1 && rec != ai_Unk92cae0[i]) {
                other = ai_Unk92cae0[i];
                break;
            }
        }
        r2 = (char *)ai_Unk4cdf00(other, g);
        if (*(int *)(r2 + 8) == -1 || cm_GetSlotState(*(int *)(r2 + 8)) == 0) {
            id = *(int *)(r2 + 4);
            *(int *)(r2 + 8) = -1;
            if (id == 0) {
                x = ai_Unk4cdac0(g);
                if (x != 0 && *(int *)(x + 0x20) > 0)
                    *(int *)(r2 + 8) = ai_Unk4cffd0(other, g, c, 0, 0);
            }
        }
    }
    cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
    ai_Unk4c5dc0(c, 2, s);
    return 0;
walk:
    if (*(int *)(*(char **)(c + 0x184) + 0x128) != 0 || ((char *)s)[0x1c] != 0)
        return -1;
    if (!d3_GetHeightAt(h, *(char **)(g + 0x34) + 0x4c, &ix, &iz, &py))
        return -1;
    if (ai_Unk4ce040(ix, iz, &ix, &iz) == 0)
        return -1;
    ai_Unk4c5bc0(*(char **)(*(char **)(c + 0x184) + 0x34), ix, iz, 0);
    return 0;
}

#define G21_BC0 ((int (*)(const char *, int, int, const char *))ai_Unk4c5bc0)
#define G21_C40 ((int (*)(int, void *))ai_Unk4c5c40)
#define G21_CA0 ((int (*)(int, void *))ai_Unk4c5ca0)
#define G21_DC0 ((int (*)(char *, int, void *))ai_Unk4c5dc0)
#define G21_CH(o) (*(char **)((o) + 0x184))
#define G21_POS(o) (*(char **)(G21_CH(o) + 0x34) + 0x4c)

/* 0x4d76a0, the fight commands of one fighter record: 16 slots of 0x2c bytes at rec + 0x94 (kind, state, request, ...); each kind
   (1 walk, 2 attack, 3 .., 4 follow, 5 stand, 6 flee / goods, 7, 8) sends its messages. rough draft from the disassembly */
void ai_Unk4d76a0(int *rec)
{
    char *h = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int m[11];
    char buf[48];
    char *c, *k, *x, *other, *r2, *nm;
    int *p;
    int slot, i, n, res, hit, dmg, ix, iz, id, b;
    float f, q[4];
    unsigned short r;
    int tmp[3];

    for (slot = 0; slot < 16; slot++) {
        if (ai_Unk6f0cf4 != 0)
            return;
        p = rec + slot * 11 + 0x25;
        memcpy(m, p, 0x2c);
        if (p[1] != 0 && p[0] != -1) {
            if (p[1] == 5)
                c = gm_FindChar(0);
            else
                c = ai_Unk4ce130(p[0]);
            k = ai_Unk4cda50(c);
            if ((c == 0 || c[8] == 0) && k != 0) {
                p[0] = -1;
                p[1] = 0;
                return;
            }
            switch (p[1]) {
            case 1:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if ((char)p[3] == 0) {
                        if (ai_Unk4ce040(p[4], p[5], &ix, &iz) != 0) {
                            sprintf(buf, "cmb_%i", *(int *)(c + 4));
                            ((char *)p)[0xc] = 1;
                            ai_Unk4c5dc0(c, 6, m);
                            res = G21_BC0(buf, ix, iz, 0);
                            goto done;
                        }
                    } else {
                        d3sm_GetVertexI(h, p[4], p[5], q);
                        if (d3_VecEqual(q, (float *)G21_POS(c), 30.0f) != 0) {
                            m[0] = *(int *)(c + 4);
                            m[1] = 0;
                            ai_Unk4c5dc0(c, 6, m);
                            goto send;
                        }
                    }
                }
                break;
            case 2:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    x = ai_Unk4ce130(p[4]);
                    if (x != 0 && (x[8] == 0 || *(int *)(*(char **)(G21_CH(x) + 0x34) + 0x218) == 1 || c == 0 || c[8] == 0)) {
                        m[0] = *(int *)(c + 4);
                        id = 6;
                        goto stop;
                    }
                    if ((*(int *)(k + 0x58) == 1 || *(int *)(k + 0x58) == 2) && (other = ai_Unk4cdac0(c)) != 0 && *(int *)(other + 0x20) < 1) {
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        res = G21_C40(rec[4], m);
                        goto done;
                    }
                    b = 1;
                    if (*(int *)k == 0x174) {
                        b = 0;
                        if (((char *)p)[0x1d] == 0 && (char)p[3] == 0) {
                            if (ai_Unk4ce040(p[5], p[6], &ix, &iz) != 0) {
                                p[5] = ix;
                                p[6] = iz;
                                ai_Unk4c5bc0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                            }
                            ((char *)p)[0xc] = 1;
                        } else if (*(int *)(G21_CH(c) + 0x128) == 0 && (char)p[3] != 0) {
                            d3sm_GetVertexI(h, p[5], p[6], q);
                            b = d3_VecEqual(q, (float *)G21_POS(c), 120.0f) != 0;
                        }
                    } else {
                        if (x == 0)
                            f = ai_Unk4ce200(c, p[5], p[6]);
                        else
                            f = ai_Unk4ce1b0(c, *(char **)(G21_CH(x) + 0x34));
                        if (*(int *)k == 0x176) {
                            b = f <= *(float *)(k + 0xc);
                            if ((char)p[3] == 0 && f < *(float *)(k + 0xc) * 0.1f)
                                b = 0;
                        } else if (!(f <= *(float *)(k + 0xc)))
                            b = 0;
                    }
                    if ((b != ((char *)p)[0x1d] || *(int *)(G21_CH(c) + 0x128) == 0) && (((char *)p)[0x1d] = b, b != 0)) {
                        if (*(int *)(k + 0x58) != 2) {
                            r = util_RandomU(0xff);
                            if (0xff - (int)((unsigned char)k[4] * 0.01 * (unsigned char)c[0x83]) < (int)r)
                                hit = 1;
                            else
                                hit = 0;
                        } else
                            hit = 1;
                        if (hit != 0) {
                            n = ai_Unk4e52f0((unsigned char)k[6] - (unsigned char)k[5]) + (unsigned char)k[5];
                            if (x != 0)
                                dmg = (int)((double)(unsigned int)n * *(float *)(x + 0x1c) * 0.01);
                            else
                                dmg = (int)((double)(unsigned int)n * 9.0);
                        } else
                            dmg = 0;
                        p[8] = hit;
                        p[9] = dmg;
                        if (hit != 0 && *(int *)(c + 0x16c) != 0 && ai_InventoryHas(*(int *)(c + 0x16c), 0x2b9) != 0)
                            p[9] = (int)(p[9] * 1.25f);
                        if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && *(int *)(k + 0x58) == 0 && x != 0) {
                            other = 0;
                            n = (unsigned char)ai_Unk6f0d04[0x30];
                            for (i = 0; i < n; i++) {
                                if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1 && (char *)rec != ai_Unk92cae0[i]) {
                                    other = ai_Unk92cae0[i];
                                    break;
                                }
                            }
                            r2 = (char *)ai_Unk4cdf00(other, x);
                            if ((*(int *)(r2 + 8) == -1 || cm_GetSlotState(*(int *)(r2 + 8)) != 0) &&
                                (*(int *)(r2 + 8) = -1, *(int *)(r2 + 4) == 0) &&
                                ai_Unk4cdac0(x) != 0 && *(int *)(ai_Unk4cdac0(x) + 0x20) > 0) {
                                cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
                                *(int *)(r2 + 8) = ai_Unk4cffd0(other, x, c, 0, 0);
                            }
                        }
                    }
                    if (*(int *)(k + 0x58) == 0) {
                        if (((char *)p)[0x1d] == 0) {
                            if (*(int *)(G21_CH(c) + 0x128) == 0 && (char)p[7] == 0) {
                                if (d3_GetHeightAt(h, G21_POS(x), &ix, &iz, tmp) == 0)
                                    break;
                                p[5] = ix;
                                p[6] = iz;
                                sprintf(buf, "cmb_%i", *(int *)(c + 4));
                                res = G21_BC0(buf, ix, iz, *(char **)(G21_CH(x) + 0x34));
                                p[2] = res;
                            }
                            goto test1d;
                        }
                        goto attack;
                    } else {
                        if (*(int *)(k + 0x58) == 1 || *(int *)k == 0x176) {
                            if (((char *)p)[0x1d] != 0)
                                goto attack;
                            if (*(int *)(G21_CH(c) + 0x128) == 0 && (char)p[7] == 0) {
                                if (d3_GetHeightAt(h, G21_POS(c), &q[2], &q[3], tmp) == 0 ||
                                    (x != 0 && d3_GetHeightAt(h, G21_POS(x), p + 5, p + 6, tmp) == 0))
                                    break;
                                if (ai_Unk4d2170(c, p[5], p[6], 0, &ix, &iz) != 0) {
                                    res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                    p[2] = res;
                                }
                            }
                        }
test1d:
                        if (((char *)p)[0x1d] != 0)
                            goto attack;
                    }
                    if ((*(int *)k == 0x16e || *(int *)k == 0x172) && p[8] != 0 &&
                        d3_GetHeightAt(h, G21_POS(c), &ai_Unkfa5f00, &ai_Unkfa5f04, tmp) != 0) {
                        if (ai_Unk4d2170(c, ai_Unkfa5f00, ai_Unkfa5f04, 0xc, &ix, &iz) != 0)
                            p[2] = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        res = G21_C40(rec[4], m);
                        goto done;
                    }
                }
                break;
attack:
                if (*(int *)(G21_CH(c) + 0x128) == 0) {
                    if (*(int *)(k + 0x58) == 2 && p[8] != 0) {
                        cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
                        ai_Unk4c5dc0(c, 2, p);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        if (d3_GetHeightAt(h, G21_POS(c), &ai_Unkfa5f00, &ai_Unkfa5f04, tmp) == 0)
                            break;
                        if (ai_Unk4d2170(c, ai_Unkfa5f00, ai_Unkfa5f04, 0xc, &ix, &iz) != 0)
                            p[2] = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                        res = G21_C40(rec[4], m);
                    } else {
                        cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x20000000, 0);
                        res = G21_DC0(c, 2, p);
                    }
                    p[2] = res;
                }
                break;
            case 3:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if ((char)p[3] == 0 && d3_VecEqual(q, (float *)G21_POS(c), 50.0f) == 0 &&
                        ai_Unk4ce040(p[4], p[5], &ix, &iz) != 0) {
                        sprintf(buf, "cmb_%i", *(int *)(c + 4));
                        ((char *)p)[0xc] = 1;
                        p[2] = G21_DC0(c, 6, m);
                        p[2] = G21_BC0(buf, ix, iz, 0);
                    }
                    d3sm_GetVertexI(h, p[4], p[5], q);
                    if (d3_VecEqual(q, (float *)G21_POS(c), 50.0f) != 0) {
                        ai_Unk4c5dc0(c, 1, m);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        res = G21_C40(rec[4], m);
                        goto done;
                    }
                }
                break;
            case 4:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    id = d3_FindObject(0, 0x40, 0, p + 4);
                    n = 0;
                    if (id != 0) {
                        n = *(int *)(id + 0x200);
                        d3_PointToWorld(id, id + 0x4c, q);
                        if (n == 0 || (ai_Unk4ce270(c, (char *)n) != 0 &&
                                       (*(char *)(n + 8) == 0 || d3_VecEqual(q, (float *)G21_POS((char *)n), 49.5f) == 0))) {
                            if ((char)p[3] == 0 && d3_VecEqual(q, (float *)G21_POS(c), 49.5f) == 0 &&
                                ai_Unk4ce040(p[8], p[9], &ix, &iz) != 0) {
                                ((char *)p)[0xc] = 1;
                                p[8] = ix;
                                p[9] = iz;
                                ai_Unk4c5dc0(c, 6, m);
                                res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                goto done;
                            }
                            if ((char)p[3] == 1 && *(int *)(G21_CH(c) + 0x128) == 0 && d3_VecEqual(q, (float *)G21_POS(c), 49.5f) == 0)
                                ((char *)p)[0xc] = 0;
                            else if (d3_VecEqual(q, (float *)G21_POS(c), 49.5f) != 0) {
                                ai_Unk4c5dc0(c, 3, m);
                                m[0] = *(int *)(c + 4);
                                m[1] = 0;
                                res = G21_C40(rec[4], m);
                                goto done;
                            }
                            break;
                        }
                    }
                    d3_PointToWorld(id, id + 0x4c, q);
                    if (ai_Unk4ce270(c, *(char **)(id + 0x200)) != 0 && *(char *)(n + 8) != 0) {
                        if (d3_VecEqual(q, (float *)G21_POS((char *)n), 49.5f) != 0) {
                            res = ai_Unk4d7360((char *)rec, c, *(char **)(id + 0x200));
                            goto done;
                        }
                    }
                    if (*(char *)(n + 8) != 0)
                        break;
                    m[0] = *(int *)(c + 4);
                    m[1] = 0;
                    res = G21_C40(rec[4], m);
                    goto done;
                }
                break;
            case 5:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    m[0] = *(int *)(c + 4);
                    id = 7;
stop:
                    m[1] = 0;
                    ai_Unk4c5dc0(c, id, m);
                    res = G21_C40(rec[4], m);
                    goto done;
                }
                break;
            case 6:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if (d3_FindObject(0, 0x40, 0, p + 4) == 0) {
                        ai_Unk4c5dc0(c, 6, m);
                        m[0] = *(int *)(c + 4);
                        m[1] = 0;
                        p[2] = G21_C40(rec[4], m);
                    }
                    n = *(int *)(G21_CH(c) + 0x128);
                    switch (((char *)p)[0x22]) {
                    case 0:
                        if (n == 0) {
                            ((char *)p)[0xc] = 1;
                            ai_Unk4c5dc0(c, 6, m);
                            if (ai_Unk4ce040((char)p[8], ((char *)p)[0x21], &ix, &iz) != 0) {
                                res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                goto done;
                            }
                        } else {
                            d3sm_GetVertexI(h, (char)p[8], ((char *)p)[0x21], q);
                            if (d3_VecEqual(q, (float *)G21_POS(c), 50.0f) != 0) {
                                ai_Unk4c5dc0(c, 4, m);
                                ((char *)m)[0x2a] = 1;
                                res = G21_CA0(rec[4], m);
                                goto done;
                            }
                        }
                        break;
                    case 1:
                        if (n == 0) {
                            nm = *(int *)(c + 0x16c) == ai_Unk6f0cec ? "dummy_sp_OFF_WARENSAVE" : "dummy_sp_DEF_WARENSAVE";
                            id = d3_FindObject(0, 0x100, 0, nm);
                            d3_PointToWorld(id, id + 0x4c, q);
                            if (d3_GetHeightAt(h, q, &ix, &iz, tmp) != 0 && ai_Unk4ce040(ix, iz, &ix, &iz) != 0) {
                                ((char *)m)[0x28] = (char)ix;
                                ((char *)m)[0x29] = (char)iz;
                                ((char *)m)[0x2a] = 2;
                                ai_Unk4c5ca0(rec[4], m);
                                res = G21_BC0(*(char **)(G21_CH(c) + 0x34), ix, iz, 0);
                                goto done;
                            }
                        }
                        break;
                    case 2:
                        d3sm_GetVertexI(h, (char)p[8], ((char *)p)[0x21], q);
                        if (d3_VecEqual(q, (float *)G21_POS(c), 50.0f) != 0) {
                            ai_Unk4c5dc0(c, 5, m);
                            m[0] = *(int *)(c + 4);
                            m[1] = 0;
                            res = G21_C40(rec[4], m);
                            goto done;
                        }
                        break;
                    }
                }
                break;
            case 7:
                if (p[2] == 1 || cm_GetSlotState(p[2]) != 0) {
                    p[2] = -1;
                    if ((char)p[3] == 0) {
                        if (ai_Unk4ce040(p[4], p[5], &ix, &iz) != 0) {
                            sprintf(buf, "cmb_%i", *(int *)(c + 4));
                            ((char *)p)[0xc] = 1;
                            ai_Unk4c5dc0(c, 6, m);
                            res = G21_BC0(buf, ix, iz, 0);
                            goto done;
                        }
                    } else {
                        d3sm_GetVertexI(h, p[4], p[5], q);
                        if (d3_VecEqual(q, (float *)G21_POS(c), 30.0f) != 0) {
                            m[0] = *(int *)(c + 4);
                            m[1] = 0;
                            ai_Unk4c5dc0(c, 6, m);
                            res = G21_C40(rec[4], m);
                            goto done;
                        }
                    }
                }
                break;
            case 8:
                if ((p[2] == 1 || cm_GetSlotState(p[2]) != 0) && (p[2] = -1, (char)p[3] == 0)) {
                    ((char *)p)[0xc] = 1;
                    res = G21_DC0(c, 6, m);
                    goto done;
                }
                break;
            }
            ai_Unk92bec0 += 0x1f;
            continue;
send:
            m[1] = 0;
            res = G21_C40(rec[4], m);
done:
            p[2] = res;
            ai_Unk92bec0 += 0x1f;
        }
    }
}
/* 0x4d88c0, the fight loop: starts one of two fight tracks, then runs until the fight ends (ai_Unk6f0cf4 / ai_Unk6f0cf8): the pause
   toggle (key 0x39 = space) with the character animations and the "pause" text, the frame of the UI, and every 0x15e timer calls
   the characters' fight-AI (0x4d2f80) and the fighters' commands (0x4d76a0). Ends with the end-of-fight code 0x4d6b70. */

void ai_Unk4d88c0(char *a)
{
    unsigned int t;
    int i, r;
    char *o;

    t = tb_Calls - 0x15e;
    r = util_RandomU(100);
    ai_Unk4d0a30();
    ai_Unk4d6a50(a);
    ai_Unk6f0d4c = 0;
    if (r < 0x32)
        ai_Unk6f0d4c = msx_PlayAmbient("Athmos\\Kampfathmo_1.mp3", 0xb);
    else
        ai_Unk6f0d4c = msx_PlayAmbient("Athmos\\Kampfathmo_2.mp3", 0xb);
    ai_Unk6f0ce4 = 0;
    while (sv_RunFrame(ai_Unk4d88c0, 0xfd6c6) != 0 && ai_Unk6f0cf8 == 0) {
        if ((ai_GameFlags & 4) == 0 && kb_Last == 0x39) {
            ai_Unk6f0ce4 = ai_Unk6f0ce4 == 0;
            for (i = 0; i < 32; i++) {
                o = ai_Unk7603a0V[i];
                if (o != 0 && *(char **)(*(char **)(o + 0x184) + 0x88) == ai_Unk5fb5b0)
                    ch_PauseAnim(*(char **)(o + 0x184), ai_Unk6f0ce4);
            }
            sv_SetStatusText(&ch_EmptyName);
        }
        if (ai_Unk6f0ce4 != 0)
            sv_SetStatusText(ai_Unk143e674);
        else
            ai_Unk4d6970(a);
        if (ai_Unk6f0cf4 == 0 && ai_Unk6f0cf8 == 0 && ai_Unk6f0ce4 == 0) {
            if (tb_Calls > t + 0x1a4) {
                for (i = 0; i < (unsigned char)ai_Unk6f0d04[0x30]; i++) {
                    if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1) {
                        o = gm_FindChar(*(int *)(ai_Unk6f0d04 + 0x34 + i * 4));
                        if (o != 0 && o[2] != 6 && o[2] != 7)
                            ai_Unk4d2f80((int)o);
                    }
                }
                t = tb_Calls;
            }
            if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c) {
                ai_Unk4d3b30(ai_Unk6f0d04);
                for (i = 0; i < (unsigned char)ai_Unk6f0d04[0x30]; i++) {
                    if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1)
                        ai_Unk4d76a0((int *)ai_Unk92cae0[i]);
                }
            }
        }
        if (ai_Unk6f0cf4 != 0 || ai_Unk6f0cf8 != 0)
            sv_PopRequest = 1;
    }
    ai_Unk4d6b70();
}
/* 0x4d8b10 */

void ai_Unk4d8b10(char *a)
{
    if (ai_Unk6f0d7c == 0) {
        ai_Unk4d6930(a);
        return;
    }
    ai_Unk4d88c0(a);
}
/* 0x4d8b30, clears the 16 ints at 0x92ca80 */

void ai_Unk4d8b30()
{
    memset(ai_Unk92ca80, 0, 0x40);
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

void ai_Unk4d8c40()
{
    int i;

    for (i = 0; i < 16; i++) {
        if (ai_Unk92ca80[i] != 0)
            ai_Unk4e3250(ai_Unk92ca80[i]);
    }
}

/* ---- cut.c (joined) ---- */

/* 0x4d8c70, draws the next picture 0x4df of a stack (o + 0x2c counts them, 10 up each) on the window 0x6ed938 */
void ai_Unk4d8c70(int unused, char *o)
{
    ((int (*)(short, short, int, int))d2_WinOpenPic)(0x8c, (short)(0x44 - *(short *)(o + 0x2c) * 10), 0x4df, d2_LastWindow);
    *(int *)(o + 0x2c) = *(int *)(o + 0x2c) + 1;
}
/* 0x4d8cb0, the fight result list (a) or one more picture of its three stacks: b set: the three headings (texts 0x12d3 / 0x12d4 / 0x12d5)
   in the window 0x6ed938 and the counters 0x6f0d6c / 0x6f0d68 / 0x6f0d70 cleared; else the next picture 0x4df of stack c (0, 1, 2) */

void ai_Unk4d8cb0(int a, int b, int c)
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
        ai_Unk6f0d6c = 0;
        ai_Unk6f0d68 = 0;
        ai_Unk6f0d70 = 0;
        return;
    }
    if (c == 0) {
        d2_WinOpenPic((ai_Unk6f0d6c * 5 + 0x28) * 2, 0x20, 0x4df, d2_LastWindow);
        ai_Unk6f0d6c++;
    } else if (c == 1) {
        d2_WinOpenPic((ai_Unk6f0d68 * 5 + 0x28) * 2, 0x3e, 0x4df, d2_LastWindow);
        ai_Unk6f0d68++;
    } else if (c == 2) {
        d2_WinOpenPic((ai_Unk6f0d70 * 5 + 0x28) * 2, 0x2f, 0x4df, d2_LastWindow);
        ai_Unk6f0d70++;
    }
}
/* 0x4d8e20, plays one council session ("sitzung_*" cutscenes; a: the 0x114-byte party record, +0x30 count, +0x34 ids, copied to the
   stack and back): for each of the 6 kinds (6 .. 1: the six sitting groups, "Sitzung_Wohnsitz" .. "Sitzung_zunft" .ed3, "sitzungen_flug_*"
   .esc) it collects the participants (0x4e6fc0), shows the heads of candidates (kind 2) and voters (kind 3) in the window 0x6ed938,
   and walks the vote: the votes per participant, the winner (a tie is drawn with 0x4e52f0), the texts 0x12c4 .. 0x12e5, then the
   dismissal ("_ABSETZEN_*") for the office of the player. ROUGH draft from the disassembly: the control flow of the two vote loops
   and the exact argument orders are not checked. */

void ai_Unk4d8e20(char *a)
{
    char flug[6][0x20] = {"sitzungen_flug_wohnsitz.esc", "sitzungen_flug_kirche.esc", "sitzungen_flug_rat.esc", "sitzungen_flug_diebe.esc",
                          "sitzungen_flug_wache.esc", "sitzungen_flug_zunft.esc"};
    char ed3[6][0x30] = {"Sitzung_Wohnsitz", "Sitzung_Kirche.ed3", "Sitzung_Rat.ed3", "Sitzung_Diebe.ed3", "Sitzung_Wache.ed3", "Sitzung_zunft.ed3"};
    int kinds[6] = {6, 5, 4, 3, 2, 1};
    int copy[0x45];
    G22ENT ent[16];
    char name[0x100];
    int cand[8], votes[8], voted[8];
    int *p;
    int ids[8], idx[8];
    int kind, k, n, nc, nv, nid, i, j, w, tie, best, win, win2, view1, view2, view3, track, ob, cur;
    int sound;

    memcpy(copy, a, 0x114);
    track = ai_Unk4e3270((unsigned)&full_data_60d5b4, 0);
    cur = ai_Unk4e3270((unsigned)&full_data_60d5a4, 0);
    ai_Unk4e3620(0);
    nid = 0;
    for (i = 0; i < (unsigned char)a[0x30]; i++) {
        p = (int *)gm_FindChar(*(int *)(a + 0x34 + i * 4));
        if (p != 0)
            ids[nid++] = (int)p;
    }
    for (i = nid; i < 8; i++)
        ids[i] = 0;
    for (k = 0; k < 6; k++) {
        kind = kinds[k];
        n = amt_GetColumnOffices(kind, 6, ent);
        nc = 0;
        for (i = 0; i < n; i++) {
            p = (int *)gm_FindChar(ent[i].id);
            if (p != 0 && *((char *)p + 0x1b1) == 0) {
                idx[nc] = i;
                cand[nc++] = (int)p;
            }
        }
        sv_Unk52fd80(&ch_EmptyName);
        if (amt_IsColumnDecided(kind, 0) == 0) {
            if (ai_CharInts[ai_PlayerId * 0x21c / 0x21c * 0 + ai_PlayerId][0] == *(int *)(a + 0xc))
                amt_ElectOffices(kind, n, ent);
            goto next;
        }
        for (i = n; i < 6; i++)
            cand[i] = 0;
        for (i = 0; i < nc; i++) {
            *(int *)(a + 0x34 + i * 4) = *(int *)(cand[i] + 4);
        }
        *(unsigned char *)(a + 0x30) = (unsigned char)nc;
        sv_Unk52fd80(&ch_EmptyName);
        view1 = d2_OpenForm(0, 0, "cutscenes\\sitzung_main");
        d2_SetViewEnabled(view1, 0);
        d2_PlaceMainWindow(view1, 3);
        nv = 0;
        for (i = 0; i < n; i++) {
            if (ent[i].kind == 2)
                nv++;
        }
        if (nv != 0) {
            ai_Unk4e2e60(ed3[k]);
            sound = msx_PlayAmbient("Athmos\\Athmo_sitzung.mp3", 0x40);
            for (i = 0; i < nc; i++) {
                if (gm_CharNameRecord(*(unsigned short *)cand[i]) != 0)
                    sprintf(name, "%i %s", idx[i], gm_CharNameRecord(*(unsigned short *)cand[i]) + 4);
                else
                    sprintf(name, "%i stallbursche_MANN", idx[i]);
                ai_Unk4e2c30("cutscenes\\sitzungen\\sitzungen_create.esc", name);
            }
            for (i = 0; i < nc; i++) {
                sprintf(name, "%i", idx[i]);
                ai_Unk4e2c30("cutscenes\\sitzungen\\sitzungen_create_2.esc", name);
            }
            txt_Format(name, "cutscenes\\sitzungen\\%s", flug[k]);
            ai_Unk4e2c70(ai_Unk4e2c30(name, &ch_EmptyName));
            sv_Unk52fd80(&ch_EmptyName);
        }
        for (j = 0; j < n; j++) {
            if (nv == 0)
                break;
            if (ent[j].kind != 2 || ent[j].flag == 0)
                continue;
            nv--;
            /* the head of a candidate: the views "sitzung_bottom" / "sitzung_top_absetzen", the number of supporters (0x4e75c0) in a row
               of heads (0x5be1d0 / 0x5be3b0), the texts 0x12c4 / 0x12c5 and the pause 0x9c4 */
            view2 = d2_OpenForm(0, 0, "cutscenes\\sitzung_bottom");
            d2_PlaceMainWindow(view2, 3);
            view3 = d2_OpenForm(0, 0, "cutscenes\\sitzung_top_absetzen");
            d2_PlaceMainWindow(view3, 3);
            d2_SetForm(view2, 1);
            ai_AmountRowStatus(ent);
            ai_AmountRow(0x28, 0, d2_LastWindow, 0x54, ent, 1);
            d2_SetForm(view2, 2);
            ai_Unk4d8cb0(d2_LastWindow, 1, 0);
            ai_Unk4e32d0(-8, ids[0], "_INTRO", nc - 1, 0x32);
            io_PrintText(0x12c4);
            io_PrintText(0x12c5, *(unsigned short *)ent[j].id, j + 0x12bb, j + 0x12a9);
            ai_Unk4e3570(0x9c4);
            /* each voter shows its head and votes (texts 0x12c7 .. 0x12d1; kinds 0 / 1 / 2: yes / no / abstain, picked with 0x4e52f0),
               4d8cb0(.., 0, 0 / 1 / 2) adds a picture to the stack of its answer */
            win = -1;
            best = -1;
            tie = 0;
            for (i = 0; i < nc; i++) {
                if (votes[i] > best) {
                    tie = 0;
                    best = votes[i];
                    win = i;
                } else if (votes[i] == best)
                    tie = 1;
            }
            if (tie != 0 && win != -1)
                win = ai_Unk4e52f0(nc);
            if (win != -1) {
                ai_Unk4e32d0(-8, ids[0], "_ERGEBNIS", 100, 3);
                io_PrintText(0x12e1);
                ai_Unk4e3570(0x3e8);
                cm_SendRelations(ent[j].id, ent[win].id, 0, 1);
            }
            d2_SetForm(view1, 0);
            io_PrintText((unsigned)"$C");
            d2_SetViewEnabled(view1, 0);
            d2_CloseView(view3);
            d2_CloseView(view2);
            ai_Unk4e3570(0x3e8);
            sv_Unk52fd80(&ch_EmptyName);
        }
        if (nv != 0) {
            ai_Unk4e3060();
            ai_Unk4e3570(0x3e8);
            ai_Unk4e2d40(ai_Unk4e2c30("cutscenes\\sitzungen\\sitzungen_exit.esc", &ch_EmptyName));
            ai_Unk4e30c0();
        }
next:
        d2_CloseView(view1);
    }
    if (ai_CharInts[ai_PlayerId][0] == *(int *)(a + 0xc))
        amt_UpdateOffices();
    ai_Unk4e3290(cur);
    ai_Unk4e3290(track);
    if (sound != 0)
        msx_StopAmbient(sound);
    ai_Unk4e3640();
    memcpy(a, copy, 0x114);
    ai_CampaignStep(0x30, 0, 0, 0);
}
/* 0x4da9e0, the vote dialog "cutscenes\\sitzungen" (buttons from the text 0x12c6): the answer of the player's choice; *out the click */

int ai_Unk4da9e0(char *a, char *b, char *c, int *out)
{
    int v, b1, b2, b3, dlg, r;
    int tmp[1];

    *out = 0;
    v = d2_OpenForm(0, 0, "cutscenes\\sitzungen");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 1);
    d2_SetForm(v, 0);
    r = io_PrintText(0x12c6, *(unsigned short *)b, *(unsigned short *)a);
    b1 = d2_GetWinObj(v, 0, r);
    d2_GetWinObj(v, 0, r + 1);
    b3 = d2_GetWinObj(v, 0, r + 2);
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, 0x5dc, 1);
    r = ai_TrustedOfTwo(a, b, c, tmp);
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_Unk4da9e0, ai_Unk60d360) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
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
/* 0x4dab50, the dialog of one candidate vote: up to 4 candidates of the list c (ids), returns the id picked in *out / the result */

int ai_Unk4dab50(int a, char *b, int *c, int *out)
{
    int hdr[2], kind[2], cand[4], btn[4], rec[8];
    int n, i, r, v, dlg, res, k;
    unsigned int f;
    int tmp[1];

    *out = 0;
    if (amt_GetOfficeI(a, hdr) == 0)
        return -1;
    f = (unsigned char)hdr[2];
    if (amt_GetKind(f, rec) == 0)
        return -1;
    k = amt_GetOfficeApplicants(f, 2, cand);
    n = 0;
    for (i = 0; i < 4; i++) {
        r = gm_FindChar(c[i]);
        if (r != 0)
            cand[n++] = r;
    }
    if (n == 0)
        return -1;
    res = ai_TrustedOfFour(a, b, c, tmp);
    v = d2_OpenForm(0, 0, "cutscenes\\sitzungen");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 1);
    d2_SetForm(v, 0);
    if (k < 2)
        io_PrintText(0x12dc, *(unsigned short *)b, *(unsigned short *)b, f + 0x253);
    else
        io_PrintText(0x12dd, *(unsigned short *)b, *(unsigned short *)b, f + 0x253);
    for (i = 0; i < n; i++)
        btn[i] = d2_GetWinObj(v, 0, io_PrintText("$a%1N3$A", *(unsigned short *)cand[i]));
    for (; i < 4; i++)
        btn[i] = -1;
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, 0x5dc, 1);
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_Unk4da9e0, ai_Unk60d360) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            for (i = 0; i < n; i++) {
                if (d2_PointerObj == btn[i]) {
                    res = *(int *)(cand[i] + 4);
                    sv_PopRequest = 1;
                    break;
                }
            }
        }
    }
    d2_CloseView(v);
    return res;
}
/* 0x4dada0, answers the session request m (kind 1: a vote between two characters, kind 2: a pick among candidates) of the player's
   record r: +0x94 the result, +0x98 the click, +0x9c a flag; the name at +0x14 cleared */

void ai_Unk4dada0(char *r, int *m)
{
    int c, x, y, res, click;

    *(int *)(r + 0x94) = -1;
    *(int *)(r + 0x98) = -1;
    if (*(int *)(r + 0x9c) != 0)
        return;
    c = gm_FindChar(*(int *)(r + 0x10));
    if (c == 0 || *(char *)(c + 0x166) == 0 || *(char *)(c + 0x1b1) != 0)
        return;
    if (m[0] == 1) {
        x = gm_FindChar(m[3]);
        y = gm_FindChar(m[4]);
        if (x == 0 || y == 0)
            return;
        res = ai_Unk4da9e0((char *)c, (char *)x, (char *)y, &click);
        *(int *)(r + 0x98) = click;
    } else {
        if (m[0] != 2)
            return;
        if (amt_CanTakeOffice(c, m[2]) == 0)
            return;
        res = ai_Unk4dab50((char)m[1], (char *)c, m + 3, &click);
        *(int *)(r + 0x98) = click;
    }
    *(int *)(r + 0x94) = res;
    sprintf(r + 0x14, "%s", &ch_EmptyName);
}
/* 0x4dae90, like 0x4dada0 for a record of the other kind (the AI side: 0x4aab90 / 0x4aad30 decide instead of the session pickers); the out
   value is written through the first parameter slot */

void ai_Unk4dae90(char *r, int *m)
{
    char *p = r;
    int c, x, y, res;

    *(int *)(p + 0x94) = *(int *)(p + 0x98) = -1;
    if (*(int *)(p + 0x9c) != 0)
        return;
    c = gm_FindChar(*(int *)(p + 0x10));
    if (c == 0 || *(char *)(c + 0x166) == 0 || *(unsigned char *)(c + 0x1b1) > 0)
        return;
    if (m[0] == 1) {
        x = gm_FindChar(m[3]);
        y = gm_FindChar(m[4]);
        if (x == 0 || y == 0)
            return;
        res = ai_TrustedOfTwo(c, x, y, &r);
        *(char **)(p + 0x98) = r;
    } else {
        if (m[0] != 2)
            return;
        if (amt_CanTakeOffice(c, m[2]) == 0)
            return;
        res = ai_TrustedOfFour(*(unsigned char *)(m + 1), c, m + 3, &r);
        *(char **)(p + 0x98) = r;
    }
    *(int *)(p + 0x94) = res;
    strcpy(p + 0x14, &ch_EmptyName);
}
/* 0x4daf80, a trial / session request p (+0x34 .. +0x44 five character ids, +0x48 a sixth one when the byte +0x30 is >= 6): every character must
   exist, not be of kind 0xf, be alive (+8) and, when +0x1b1 is set, not be among the characters of the list 0x53e400 / 0x53e300 with the same
   +0xac as its +4 (code 6; 7 while the list is scanned). A bad slot i gives the error code i + 1 (slots 3 and 4 both 4, the sixth 5), the
   error texts 0x11d8 .. 0x11df are shown through 0x53dda0 (-1, text, 0x5f5); with no error the messages 0x11d0 .. 0x11d7 go to the
   participants of kind 6 / 7 (0x5176d0 formats, 0x4e48b0 sends). ROUGH, the notification part is only sketched */

void ai_Unk4daf80(char *p)
{
    char text[0x200];
    char *ch[6];
    int ids[6], code, i, k;
    char *c, *n;

    code = 0;
    ch[0] = 0;
    ids[0] = *(int *)(p + 0x34);
    ids[1] = *(int *)(p + 0x38);
    ids[2] = *(int *)(p + 0x3c);
    ids[3] = *(int *)(p + 0x40);
    ids[4] = *(int *)(p + 0x44);
    ids[5] = *(int *)(p + 0x48);
    for (i = 0; i < 6; i++) {
        c = (char *)gm_FindChar(ids[i]);
        ch[i] = c;
        if (code != 0)
            continue;
        if (i == 5 && *(unsigned char *)(p + 0x30) < 6)
            continue;
        if (c == 0 || c[2] == 0xf || c[8] == 0) {
            code = (i == 5) ? 5 : (i == 4) ? 4 : i + 1;
            continue;
        }
        if (*(unsigned char *)(c + 0x1b1) != 0) {
            code = 7;
            for (n = (char *)he_FindEvent(1, 0, 0x3d); n != 0; n = (char *)he_NextEvent()) {
                if (*(int *)(n + 0xac) == *(int *)(c + 4)) {
                    code = 6;
                    break;
                }
            }
        }
    }
    if (code != 0) {
        switch (code) {
        case 1: txt_Format(text, 0x11d8); break;
        case 2: txt_Format(text, 0x11d9); break;
        case 3: txt_Format(text, 0x11da); break;
        case 4: txt_Format(text, 0x11db); break;
        case 5: txt_Format(text, 0x11dc); break;
        case 6: txt_Format(text, 0x11de); break;
        case 7: txt_Format(text, 0x11df); break;
        }
        ((int (*)(int, int, int))he_ShowNews)(-1, text, 0x5f5);
        return;
    }
    for (i = 0; i < 6; i++) {
        k = (unsigned char)ch[i][2];
        if (k == 6 || k == 7) {
            txt_Format(text, 0x11d0 + i, *(unsigned short *)ch[i]);
            strcat(text, &ch_EmptyName);
            ai_Unk4e48b0(ch[i], 0, text);
        }
    }
    ((int (*)(int, int, int))he_ShowNews)(-1, text, 0x5f5);
    G22_4C5580();
}

extern char ai_Unk60de4c;
extern char ai_Unk60dd68;
extern char ai_Unk60dd24;
/* ai_Unk92caf0: view of ai_Unk92cae0 (tools/data_views.py) */
#define G26_BOX(sw) ai_Unk4e2bd0(ai_Unk60d368, (sw)); d2_SetForm(ai_Unk60d368, 0)
#define G26_WAITV(h) if ((h) != 0) { while (sb_IsPlaying(h) != 0) ai_Unk4e3420(1); } ai_Unk4e3420(1)

/* 0x4db700 */
void ai_Unk4db700(char *m)
{
    int a, b, c;
    int ids[6];
    char *ch[6];
    char names[0x200];
    int i, n, h, v, guilty, mode, scene;

    n = *(unsigned char *)(m + 0x30);
    for (i = 0; i < 6; i++)
        ids[i] = *(int *)(m + 0x34 + i * 4);
    a = ids[0];
    b = ids[1];
    c = ids[2];
    gv_GetTableRecord(a, (int *)names);     /* ROUGH: the original pushes (edi, a local buffer) */
    G26_BOX(0);
    ai_Unk4e2e60("Gericht.ed3");
    for (i = 0; i < 5; i++) {
        ch[i] = (char *)gm_FindChar(ids[i]);
        if (ch[i] == 0)
            goto done;
    }
    ch[5] = (char *)gm_FindChar(ids[5]);
    if (ch[5] == 0 && (unsigned char)m[0x30] > 2 && *(int *)(m + 0x48) != -1)
        goto done;
    if (cm_SkipEvents == 0)
        msx_PlayAmbient(0x40, "Athmos\\Athmo_sitzung.mp3");
    scene = ai_Unk4e52f0(2);
    ai_Unk4e3620("cd2\\ImKerker.mp3");
    ai_Unk4e3270(0, "ADDON\\PROZESS_NEU.sbf");
    ai_Unk4947a0(a, b);
    sprintf(names, "%s ", (char *)gm_CharNameRecord(*(unsigned short *)ch[0]) + 4);
    for (i = 1; i < 4; i++)
        sprintf(names + strlen(names), "%s ", (char *)gm_CharNameRecord(*(unsigned short *)ch[i]) + 4);
    sprintf(names + strlen(names), "%s", (char *)gm_CharNameRecord(*(unsigned short *)ch[4]) + 4);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-create2.esc", names);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter.esc", &ai_Unk60de4c);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer1.esc", &ai_Unk60de4c);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer2.esc", &ai_Unk60de4c);
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\prozess\\prozess-intro.esc", &ch_EmptyName));

    /* the judge opens the session */
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
    G26_BOX(1);
    io_PrintText(0x1491);
    h = ai_Unk4e32b0(5, "_PROZESS_NEU", 0, 0);
    G26_WAITV(h);
    G26_BOX(0);
    io_PrintText(0x1492);
    ai_Unk4e3420(4);

    /* the accuser states the charge */
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-klaeger.esc", &ai_Unk60dd68);
    G26_BOX(0);
    io_PrintText(0x1499);
    h = ai_Unk4e32b0(0, "_PROZESS_NEU", 0, 0);
    G26_WAITV(h);
    ai_Unk4e2c70(0);

    /* the witnesses */
    for (i = 0; i < n; i++) {
        if (ids[i] == -1)
            continue;
        gv_GetTableRecord(ids[i], i);
        v = gv_CrimeText(ids[i], i);
        if (v == -1)
            continue;
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-klaeger.esc", &ai_Unk60dd68);
        G26_BOX(0);
        io_PrintText(0x14b0);
        h = ai_Unk4e32b0(i, "_PROZESS_GESETZ", 0, 0);
        G26_WAITV(h);
        ai_Unk4e52f0(0x1e);
        ai_Unk4e2c70(0);
    }
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
    io_PrintText(0x14ba);
    h = ai_Unk4e32b0(0, "_PROZESS_NEU", 0, 0);
    G26_WAITV(h);
    G26_BOX(0);
    io_PrintText("$C");

    /* the accused answers: guilty / not guilty */
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-angeklagter.esc", &ai_Unk60dd24);
    ai_Unk4e2bd0(ai_Unk60d368, 0);
    ai_Unk4e3830(m, 0);
    for (i = 0; i < n && *(int *)(ai_Unk92caf0 + i * 0x18) != ids[0]; i++)
        ;
    guilty = *(int *)(ai_Unk92caf0 + i * 0x18 + 0x84);
    mode = *(int *)(ai_Unk92caf0 + i * 0x18 + 0x88);
    G26_BOX(1);
    io_PrintText("$C");
    if (guilty == 1) {
        gm_Liking(guilty, mode, 0);
        ai_Unk4e3270(0, "PROZESS_3_SCHULDIG.sbf");
        ai_Unk4e32d0(0, 0, "_PROZESS_3_SCHULDIG", 0, 0);
        io_PrintText(0x14bd, 0, 0x14be);
    } else {
        ai_Unk4e3270(0, "PROZESS_3_NICHT_SCHULDIG.sbf");
        ai_Unk4e32d0(0, 0, "_PROZESS_3_NICHT_SCHULDIG", 0, 0);
        io_PrintText(0x14bd, 0, 0x14c4);
    }
    ai_Unk4e3420(2);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer1.esc", &ai_Unk60dd68);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-beisitzer2.esc", &ai_Unk60dd68);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ai_Unk60dd68);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter.esc", &ch_EmptyName);

    /* the torture (the instrument names 0x60dad0 .., "cutscenes\\prozess\\folter\\<name>.esc") */
    if (ai_Unk4e52f0(2) == 1) {
        ai_Unk494830(a, b);
        G26_BOX(1);
        io_PrintText("$C", 0x14ca);
        ai_Unk4e3420(4);
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-folter-1.esc", &ch_EmptyName);
        G26_BOX(0);
        ai_Unk4e3830(m, 0);
        ai_Unk4e2c70(0);
        for (i = 0; i < n && *(int *)(ai_Unk92caf0 + i * 0x18) != ids[0]; i++)
            ;
        G26_BOX(1);
        io_PrintText("$C");
        if (*(int *)(ai_Unk92caf0 + i * 0x18 + 0x88) == 1) {
            ai_Unk4e52f0(6);
            ai_Unk4e3270(0, "PROZESS_4_FOLTER_1.sbf");
            ai_Unk4e3270(0, "PROZESS_4_FOLTER_2.sbf");
            ai_Unk4e32d0(0x32, 0, "_PROZESS_4_FOLTER_1", 0, 0);
            ai_Unk4e32d0(0x12c, 0, "_PROZESS_4_FOLTER_2", 0, 0);
            io_PrintText(0x14d9);
            ai_Unk4e3420(1);
            ai_Unk4e2c30("cutscenes\\prozess\\prozess-folter-3.esc", &ch_EmptyName);
        } else {
            ai_Unk4e2c30("cutscenes\\prozess\\prozess-folter-2.esc", &ch_EmptyName);
            io_PrintText(0x14d8);
        }
        ai_Unk4e3420(4);
        ai_Unk4e2e60("ob_GEFAENGNIS_FOLTERKAMMER.ed3");
        ai_Unk4e2c30("cutscenes\\prozess\\folter-create.esc", &ch_EmptyName);
        G26_BOX(1);
        io_PrintText(0x14e4);
        h = ai_Unk4e32b0(4, "_PROZESS_NEU", 0, 0);
        G26_WAITV(h);
        ai_Unk4e3420(5);
        ai_Unk4e52f0(5);
        io_PrintText(0x14ec);
        ai_Unk4e3420(5);
        ai_Unk494300(0);
        ai_Unk4e2c70(0);
        ai_Unk4e32d0(0x1f4, 0, "_PROZESS_5_FOLTER2_UNSCHULDSBETEUERUNG", 0, 0);
        ai_Unk4e32d0(0, 0, "_PROZESS_5_FOLTER2_GESTEHEN", 0, 0);
        ai_Unk4e32d0(0x1f4, 0, "_PROZESS_5_FOLTER2_WEITER", 0, 0);
        io_PrintText(0x1510);
        ai_Unk4e3420(4);
        ai_Unk4e2c30("cutscenes\\prozess\\folterexit.esc", &ch_EmptyName);
        if (cm_SkipEvents == 0)
            d3if_SelectScene(scene, 0);
        ai_Unk4e3270(0, "PROZESS_6_FOLTERERGEBNIS.sbf");
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
        ai_Unk4e32d0(0, "_PROZESS_6_FOLTERERGEBNIS", 0, 0);
        io_PrintText(0x1520, 0, 0x1522);
        ai_Unk4e3420(4);
    }
    ai_Unk4e3270(0, "PROZESS_6_ANKUENDIGUNG.sbf");
    ai_Unk4e32d0(0, 0, "_PROZESS_6_ANKUENDIGUNG", 0, 0);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
    io_PrintText(0x1523);
    ai_Unk4e3420(4);

    /* the vote */
    ai_Unk4e3270(0, "PROZESS_6_ABSTIMMUNG_ERGEBNISSE.sbf");
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-3.esc", &ch_EmptyName);
    G26_BOX(1);
    io_PrintText(0x1528);
    ai_Unk4e3420(2);
    ai_Unk4e52f0(100, 3);
    ai_Unk4e32d0(0, "_PROZESS_6_ABSTIMMUNG_ERGEBNISSE", 0, 0);
    ai_Unk4e3420(2);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-1.esc", &ch_EmptyName);
    ai_Unk4e3420(2);
    ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-2.esc", &ch_EmptyName);
    ai_Unk4e3420(2);

    /* the verdict */
    ai_Unk4e3270(0, "PROZESS_8.sbf");
    if (guilty < 2) {
        ai_Unk4e3270(0, "PROZESS_7_SCHULDIG.sbf");
        ai_Unk4e3270(0, "PROZESS_7_STRAFEN.sbf");
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-3.esc", &ch_EmptyName);
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-richter-reden.esc", &ch_EmptyName);
        ai_Unk4e2bd0(ai_Unk60d368, 0);
        ai_Unk4e3830(m, 0);
        if (mode >= 4 && ch[0][2] == 6 && ai_Unk4e3290(0) != 0)
            ai_Unk4b44e0(0x13, ch[0]);
        G26_BOX(1);
        io_PrintText(0x152f);
        io_PrintText(0x1530);
        ai_Unk4e32d0(0x64, 0, "_PROZESS_7_SCHULDIG", 0, 0);
        ai_Unk4e32d0(0xfa, 2, "_PROZESS_7_SCHULDIG", 0, 0);
        ai_Unk4e3420(3);
        gv_CheckSentence(mode, 0);
        unk_Unk4f48f0(0, mode, 0);
        ai_Unk4e32d0(0x64, mode, "_PROZESS_7_STRAFEN", 0, 0);
        ai_Unk4e32d0(0xc8, 0, "_PROZESS_8", 0, 0);
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0])
            gv_PassSentence(0, 0, 0, mode, 0);
        io_PrintText(0x1531);
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
        }
        ai_Unk4e3420(6);
        io_PrintText("$C", 0x1557);
        ai_Unk4e3420(4);
    } else {
        ai_Unk4e3270(0, "PROZESS_7_NICHT_SCHULDIG.sbf");
        ai_Unk4e32d0(0x64, 0, "_PROZESS_7_NICHT_SCHULDIG", 0, 0);
        ai_Unk4e32d0(0xc8, 0, "_PROZESS_8", 0, 0);
        ai_Unk4e2c30("cutscenes\\prozess\\prozess-abstimmen-3.esc", &ch_EmptyName);
        G26_BOX(1);
        io_PrintText("$C", 0x152e);
        ai_Unk4e3420(4);
        io_PrintText("$C", 0x1557);
        ai_Unk4e3420(4);
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
            cm_SendMoveGoodsUnchecked(mode, ai_OwnPlayer, 0, 0);
        }
    }
    /* the player's reputation: opinion changes of +-26 (0x4c4b00) when 0x4fca40 / 0x5ba6b0 (0x22e) allow it */
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        if (gm_AddHealth(0, 0) != 0 && ai_InventoryHas(0x22e, 0) != 0) {
            cm_SendRelations(1, -26, 0, 0);
            cm_SendRelations(1, 26, 0, 0);
            cm_SendRelations(1, -26, 0, 0);
            cm_SendRelations(1, 26, 0, 0);
            cm_SendRelations(1, -26, 0, 0);
            cm_SendRelations(1, 26, 0, 0);
        }
    }
    ai_Unk4e3060();
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\prozess\\prozess-ende.esc", &ch_EmptyName));
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\prozess\\prozess-exit.esc", &ch_EmptyName));
done:
    if (cm_SkipEvents == 0)
        msx_StopAmbient(0);
    ai_Unk4e3640();
    snd_ClearQueue();
    /* the sound slots made by 0x4e3270 are freed one by one (0x4e3290) */
    ai_Unk4e3290(0);
}

/* 0x4dd960, the court's (and torture's) question dialogs of the player for a request m of kinds 0 .. 6 ("cutscenes\\prozess": a yes / no
   box, "cutscenes\\folterwahl": three choices); r +0x10 the asking character, +0x94 the answer, +0x98 a value. The bodies stand in the order
   0, 1, 2, 3, 6, 4, 5 (jump table 0x4de324). ROUGH */
#define CUT_BOX() d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow)

/* 0x4dd960 */
void ai_Unk4dd960(char *r, int *m)
{
    char tab[8] = {8, 10, 12, 15, 18, 21, 24, 0};
    int sel[7] = {-1, -1, -1, -1, -1, -1, -1};
    int btn[7];
    char *si, *di;
    int v, dlg, n, i, t, sum, k;

    *(int *)(r + 0x94) = -1;
    si = (char *)gm_FindChar(*(int *)(r + 0x10));
    di = (char *)gm_FindChar(m[6]);
    switch (m[0]) {
    case 0:
        if (*(int *)(r + 0x10) != m[6])
            return;
        *(float *)(r + 0x98) = (float)*(int *)(di + 0x24) + *(float *)(di + 0x1c);
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14bc, *(unsigned short *)si);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 0;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 1;
            }
        }
        d2_CloseView(v);
        return;
    case 1:
        if (*(int *)(r + 0x10) != m[5])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14cb, *(unsigned short *)si, *(unsigned short *)di);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 1;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 0;
            }
        }
        d2_CloseView(v);
        return;
    case 2:
        if (*(int *)(r + 0x10) != m[5])
            return;
        gm_Shuffled(7, btn);
        sum = gm_GetWorth(*(unsigned short *)si) + gm_GetWorth(*(unsigned short *)di);
        sum /= 1000;
        v = d2_OpenForm(0, 0, "cutscenes\\folterwahl");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14db, *(unsigned short *)si);
        for (i = 0; i < 3; i++) {
            k = btn[i];
            t = io_PrintText(0x14dc, k * 2 + 0xb20, (unsigned char)tab[k] * sum);
            sel[i] = d2_GetWinObj(v, 0, t);
        }
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = btn[ai_Unk4e52f0(3)];
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                for (i = 0; i < 3; i++)
                    if (d2_PointerObj == sel[i])
                        break;
                if (i < 3)
                    *(int *)(r + 0x94) = btn[i];
                sv_PopRequest = 1;
            }
        }
        d2_CloseView(v);
        return;
    case 3:
        if (*(int *)(r + 0x10) != m[10])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\folterwahl");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x14e3, *(unsigned short *)si, *(unsigned short *)di, *(unsigned short *)di);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 0;
                else if (d2_ClickedId == 0) {
                    *(int *)(r + 0x94) = 1;
                    cm_SendRelations(*(int *)(si + 4), *(int *)(di + 4), 0x23, 1);
                }
            }
        }
        d2_CloseView(v);
        return;
    case 6:
        if (*(int *)(r + 0x10) != m[6])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x1512, *(unsigned short *)si);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 0;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 1;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 0;
            }
        }
        d2_CloseView(v);
        return;
    case 4:
        t = *(int *)(r + 0x10);
        if (t != m[7] && t != m[8] && t != m[9])
            return;
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x1529, *(unsigned short *)si);
        io_PrintText(0x152a);
        io_PrintText(0x152b);
        d2_ClickedId = -1;
        *(int *)(r + 0x94) = 1;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_ClickedId == 1)
                    *(int *)(r + 0x94) = 1;
                else if (d2_ClickedId == 0)
                    *(int *)(r + 0x94) = 0;
            }
        }
        d2_CloseView(v);
        return;
    case 5:
        if (*(int *)(r + 0x10) != m[7])
            return;
        sel[0] = sel[1] = sel[2] = -1;
        n = ai_PlanRank(0);
        v = d2_OpenForm(0, 0, "cutscenes\\prozess");
        ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
        d2_SetForm(v, 0);
        io_PrintText(0x1532, *(unsigned short *)si);
        for (i = 0; i < 3; i++) {
            t = io_PrintText(0x1533, n * 3 + 0x1534 + i * 3);
            sel[i] = d2_GetWinObj(v, 0, t);
            if (gv_CheckSentence(di, i + n) != 0)
                d2_SetObjEnabled(sel[i], 0);
        }
        if (sel[0] != -1)
            *(int *)(r + 0x94) = n + 1;
        else if (ai_Unk4e52f0(2) != 0)
            *(int *)(r + 0x94) = n + 2;
        else
            *(int *)(r + 0x94) = n;
        d2_ClickedId = -1;
        dlg = CUT_BOX();
        ai_Unk4e53e0(dlg, 0x5dc, 1);
        while (sv_RunFrame(ai_Unk4dd960, ai_Unk60d360) != 0) {
            if (ai_Unk4e53e0(dlg, 0, 0) == 0)
                sv_PopRequest = 1;
            else if (d2_ClickedId != -1) {
                sv_PopRequest = 1;
                if (d2_PointerObj == sel[0])
                    *(int *)(r + 0x94) = n;
                else if (d2_PointerObj == sel[1])
                    *(int *)(r + 0x94) = n + 1;
                else if (d2_PointerObj == sel[2])
                    *(int *)(r + 0x94) = n + 2;
            }
        }
        d2_CloseView(v);
        return;
    }
}
/* 0x4de340, answers a request m of the AI's session code (kinds 0 .. 6; the bodies of 6, 4, 5 in this order) of the record r (+0x10 the asking character, +0x94 the result, +0x98 a
   value): each kind asks one of the decision functions 0x494020 .. 0x4946c0 when the character matches the one named in m */

void ai_Unk4de340(char *r, int *m)
{
    int c, id, res;

    *(int *)(r + 0x94) = -1;
    switch (m[0]) {
    case 0:
        if (*(int *)(r + 0x10) != m[6])
            return;
        c = gm_FindChar(m[6]);
        *(float *)(r + 0x98) = (float)*(int *)(c + 0x24) + *(float *)(c + 0x1c);
        *(int *)(r + 0x94) = ai_Unk494020(m);
        return;
    case 1:
        if (*(int *)(r + 0x10) != m[5])
            return;
        *(int *)(r + 0x94) = ai_Unk4941b0(m);
        return;
    case 2:
        if (*(int *)(r + 0x10) != m[5])
            return;
        *(int *)(r + 0x94) = ai_Unk4941d0(m);
        return;
    case 3:
        if (*(int *)(r + 0x10) != m[10])
            return;
        res = ai_Unk4941e0(m);
        *(int *)(r + 0x94) = res;
        if (res == 1)
            cm_SendRelations(*(int *)(r + 0x10), m[6], 0x23, res);
        return;
    case 6:
        if (*(int *)(r + 0x10) != m[6])
            return;
        *(int *)(r + 0x94) = util_RandomU(2) & 0xffff;
        return;
    case 4:
        id = *(int *)(r + 0x10);
        if (id != m[7] && id != m[8] && id != m[9])
            return;
        c = gm_FindChar(*(int *)(ai_Unk6f0d74 + 4));
        *(int *)(r + 0x94) = ai_Unk494410(c, m);
        return;
    case 5:
        if (*(int *)(r + 0x10) != m[7])
            return;
        c = gm_FindChar(m[7]);
        *(int *)(r + 0x94) = ai_Unk4946c0(c, m);
        return;
    }
}
/* 0x4de4b0, a callback of the fall animation: a blood puddle ("ub_Blutlache") at the position of the object o->+0x14 */

void ai_Unk4de4b0(float a, char *o)
{
    int v[3];

    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    a = (float)ai_Unk4e52f0(0x168) * ai_Unk5eb7dc * ai_Unk5edc68;
    d3t_PlayAnim(d3_CreateObject(0, *(char **)(*(char **)(o + 0x14) + 0x34) + 0x4c, v, "ub_Blutlache"), "sonstiges\\ub_Blutlache.baf", 0x12);
}
/* 0x4de530, a fighter whose life (0x4fc8e0) is under 0.2 sinks to the ground ("bewegung/zu_boden_sinken"): 1 */

int ai_Unk4de530(char *c, int dmg)
{
    char *o;

    if (gm_HealthRatio(c) < 0.2) {
        d3t_CreateBloodK(*(int *)(*(char **)(c + 0x184) + 0x34), 0x40, 0x3e800000, 0x3dcccccd, 0x3e0f5c29, 0x425c0000);
        ch_StopActions(*(char **)(c + 0x184));
        o = (char *)ch_AddActionCBK(*(char **)(c + 0x184), ai_Unk4de4b0, 0x12, 0x36, 1);
        strcpy(o + 0xf0, "bewegung/zu_boden_sinken");
        *(int *)(*(char **)(*(char **)(c + 0x184) + 0x34) + 0x220) = 0;
        ai_Unk6f0d94 = 1;
        return 1;
    }
    return 0;
}
/* 0x4de5e0, fills the 0x92bcc0 table (0x505ca0) for n entries */

void ai_Unk4de5e0(int n)
{
    gm_Shuffled(n, ai_Unk92bcc0);
    ai_Unk60d370 = n;
    ai_Unk6f0db0 = 0;
}
/* 0x4de610, the next number of the table modulo n */

int ai_Unk4de610(int n)
{
    int i, r;

    i = ai_Unk6f0db0 % ai_Unk60d370;
    r = ai_Unk92bcc0[i] % n;
    ai_Unk6f0db0 = ai_Unk6f0db0 + 1;
    return r;
}
/* 0x4de640, the pistol shot of a duel (callback of the object made by 0x4de900: o + 0x180 shoots at o + 0x17c): the hit chance
   0x504ec0(shooter, 3) * 0.8 (* 1.5 with the skill 0x16f, * 0.66 for a player who lost the insult round), the damage 5 + (0..15) or
   15 + (0..30) (* 0.75 with the skill 0x2b5) added to the counters 0x6f0da8 / 0x6f0da9, the life 0x4cedc0, the texts 0x1102 / 0x1109 */

void ai_Unk4de640(char *o)
{
    char *sh = *(char **)(o + 0x180);
    char *tg = *(char **)(o + 0x17c);
    float acc, b;
    int hit, dmg, n;

    d2_SetForm(ai_Unk60d368, 0);
    acc = (float)gm_GetSkill(sh, 3);
    gm_GetSkill(tg, 3);
    acc = (float)(acc * 0.8);
    if (ai_InventoryHas(sh, 0x16f) != 0)
        acc = (float)(acc * 1.5);
    if (((int)sh == ai_Unk92bca4 && ai_Unk6f0da8[3] != 0) || ((int)sh == ai_Unk92bca0 && ai_Unk6f0daa != 0))
        acc = (float)(acc * 0.66);
    if (ai_Unk4e5360() < acc) {
        hit = 1;
        if (((int)sh == ai_Unk92bca4 && ai_Unk6f0da8[5] != 0) || ((int)sh == ai_Unk92bca0 && ai_Unk6f0da8[4] != 0))
            dmg = ai_Unk4e52f0(0xf) + 5;
        else
            dmg = ai_Unk4e52f0(0x1e) + 0xf;
        if (ai_InventoryHas(tg, 0x2b5) != 0)
            dmg = (int)(dmg * 0.75f);
        if ((int)tg == ai_Unk92bca4)
            ai_Unk6f0da8[0] += dmg;
        else
            ai_Unk6f0da8[1] += dmg;
        ai_Unk4cedc0(tg, (int)(*(float *)(tg + 0x1c) * 0.01 * dmg), 0);
    } else {
        hit = 0;
        dmg = 0;
    }
    sb_PlaySampleI(0, "Schuss", 1, 0x3f, 0x7f, 0, 0);
    if (ai_Unk4de530(tg, dmg) != 0) {
        ai_Unk6f0d94 = 1;
        return;
    }
    if (hit != 0) {
        d3t_CreateBloodK(*(int *)(*(char **)(tg + 0x184) + 0x34), 0x40, 0x3e800000, 0x3dcccccd, 0x3e0f5c29, 0x425c0000);
        if (*(int *)(*(char **)(tg + 0x184) + 0x128) == 0)
            ch_QueueAnim(*(char **)(tg + 0x184), "kampf/taumeln", 1);
        d2_SetForm(ai_Unk60d368, 0);
        n = ai_Unk4de610(6);
        io_PrintText(n + 0x1102, *(unsigned short *)tg);
        snd_PlayVoice(*(unsigned short *)tg, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_TREFFE", n);
        b = (float)(*(float *)(tg + 0x1c) * dmg * 0.01);
        *(int *)(tg + 0x24) = (int)(*(int *)(tg + 0x24) - b);
        gm_AddHealth(*(unsigned short *)tg, (int)-b);
    } else {
        if (*(int *)(*(char **)(tg + 0x184) + 0x128) == 0)
            ch_QueueAnim(*(char **)(tg + 0x184), "kampf/stich_ausweichen", 1);
        n = ai_Unk4de610(9);
        io_PrintText(n + 0x1109, *(unsigned short *)tg);
        snd_PlayVoice(*(unsigned short *)tg, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN_DANEBE", n);
    }
}
/* 0x4de900, the intro text of the duel round of the fighter a against b ("_DUELL_INTRO_C2_AUSWAHL_*": kind of the round at +0x94 of the
   record 0x4e50e0: 2 insult (laestern: the better insult value 0x504ec0(.., 4) + a roll wins, flag 0x6f0daa / 0x6f0dab), 3 a roll against
   0x504ec0(a, 2) (flag 0x6f0dac / 0x6f0dad), 4 the pistol shot (an object with the callback 0x4de640, a / b in +0x17c / +0x180)). ROUGH */

void ai_Unk4de900(char *a, char *b)
{
    char *rec;
    int n;
    double fa, fb, ra, rb;

    if (ai_Unk6f0d94 != 0)
        return;
    rec = (char *)ai_Unk4e50e0(*(int *)(a + 4));
    d2_SetForm(ai_Unk60d368, 0);
    io_PrintText("$C");
    if (rec[0x94] == 2) {
        d2_SetForm(ai_Unk60d368, 0);
        io_PrintText(0x10e8, *(unsigned short *)a);
        n = ai_Unk4de610(8);
        io_PrintText(n + 0x10e9);
        snd_PlayVoice(*(unsigned short *)a, 0, "_DUELL_INTRO_C2_AUSWAHL_LAESTERN", n);
        io_PrintText("$N");
        ch_QueueAnim(*(char **)(a + 0x184), "gestik/abfaellig", 1);
        ai_Unk4e3420(2);
        fa = gm_GetSkill(a, 4);
        fb = gm_GetSkill(b, 4);
        ra = ai_Unk4e5360();
        rb = ai_Unk4e5360();
        d2_SetForm(ai_Unk60d368, 0);
        if ((float)(ra + (float)fa) <= rb + (float)fb) {
            if (b[2] == 6 || b[2] == 7)
                io_PrintText(0x10f4, *(unsigned short *)b);
            else
                io_PrintText(0x10f5, *(unsigned short *)b);
        } else {
            if (b[2] == 6 || b[2] == 7)
                io_PrintText(0x10f2, *(unsigned short *)b);
            else
                io_PrintText(0x10f3, *(unsigned short *)b);
            if ((int)a == ai_Unk92bca4)
                ai_Unk6f0da8[2] = 1;
            else
                ai_Unk6f0da8[3] = 1;
        }
    } else if (rec[0x94] == 3) {
        d2_SetForm(ai_Unk60d368, 0);
        fa = gm_GetSkill(a, 2);
        fb = ai_Unk4e5360();
        if ((float)fa <= fb) {
            if (a[2] == 6 || a[2] == 7)
                io_PrintText(0x10f8, *(unsigned short *)a);
            else
                io_PrintText(0x10f9, *(unsigned short *)a);
        } else {
            if (a[2] == 6 || a[2] == 7)
                io_PrintText(0x10f6, *(unsigned short *)a);
            else
                io_PrintText(0x10f7, *(unsigned short *)a);
            if ((int)a == ai_Unk92bca4)
                ai_Unk6f0da8[4] = 1;
            else
                ai_Unk6f0da8[5] = 1;
        }
    } else if (rec[0x94] == 4) {
        d2_SetForm(ai_Unk60d368, 0);
        n = ai_Unk4de610(7);
        io_PrintText(n + 0x10fa, *(unsigned short *)a);
        snd_PlayVoice(*(unsigned short *)a, 0, "_DUELL_INTRO_C2_AUSWAHL_SCHIESSEN", n);
        ai_Unk4e3420(2);
        io_PrintText("$N");
        n = ch_AddActionCBK(*(char **)(a + 0x184), ai_Unk4de640, 6, 0x2e, 1);
        *(char **)(n + 0x17c) = b;
        *(char **)(n + 0x180) = a;
        strcpy((char *)n + 0xf0, "kampf/schiessen_pistole");
        ai_Unk4e3420(4);
        return;
    } else {
        ai_Unk4e3420(4);
        return;
    }
    io_PrintText("$N");
    ai_Unk4e3420(4);
}
/* 0x4debe0, announces the result of a duel (the message m: +0x34 / +0x38 the duellists, +0xc the owner): the winner is the one with the
   better 0x504ec0(.., 3); when he is a person of the player's side the text 0x10c7 (+ 0x10c8 for a woman) goes to every character of the
   kinds 6 / 7 (0x53dda0) */

void ai_Unk4debe0(char *m)
{
    char buf[0x400];
    char *a, *b, *w;
    float fa;
    int ids[2];
    int i;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0)
        return;
    fa = (float)gm_GetSkill(a, 3);
    w = b;
    if (gm_GetSkill(b, 3) < fa)
        w = a;
    if (w[0x166] == 0)
        return;
    if (amt_FindOffice(w, buf + 0x18, ids) == 0)
        return;
    if (*(int *)(m + 0xc) != ai_CharInts[ai_PlayerId][0])
        return;
    amt_SendOfficeMessage(ids[0], 0, 0, 4, 0xff);
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i][2] == 6 || ai_Chars[i][2] == 7) {
            txt_Format(buf, 0x10c7, *(unsigned short *)w, (w[9] != 0) + 0x10c8);
            he_ShowNews(*(int *)(ai_Chars[i] + 4), -1, buf, 0, 0x5fe, &ch_EmptyName);
        }
    }
}
/* 0x4ded10, "cut_MainDuell": the duel of the characters m + 0x34 / m + 0x38 (m + 0x7c the scene; with 0x6f0d8c set only the result is
   announced, 0x4debe0): the scene "cutscenes\\duell\\enter_DUELL.esc" with music, the two duellists (copies of the character records, put
   at "dummy_DUELLANT_A_ENTRY" / "_B_ENTRY"), the pistols "ob_PISTOLE", then three rounds (0x4de900 for both sides: the intro texts, the
   shot 0x4de640) until 0x6f0d94 (a fighter down), the result texts 0x111c .. 0x1123 and the damage dealt (0x6f0da8 / 0x6f0da9) taken off
   the life with 0x4fca40, the doctor ("_DUELL_INTRO_C2_DOKTOR"), and the message to the other characters. ROUGH: control flow of
   the shot rounds and the text numbers from the disassembly, argument orders of the scene helpers unchecked. */

void ai_Unk4ded10(char *m)
{
    char sc[6][0x20] = {"duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3", "duell.ed3"};
    char ca[0x21c], cb[0x21c];
    char buf[0x400];
    char *a, *b, *w, *rec, *oa, *ob;
    int i, k, n, r, snd1, snd2, scene, hita, hitb, obj;
    int ids[2];

    r = (unsigned short)util_RandomU(100) - 5;
    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0) {
        err_Message("cut_MainDuell(): Could not create character");
        return;
    }
    if (cm_SkipEvents != 0) {
        ai_Unk4debe0(m);
        return;
    }
    ai_Unk4e36a0();
    ai_Unk4e2e60(sc[*(char *)(m + 0x7c)]);
    ai_Unk4de5e0(10);
    ai_Unk4e3650();
    k = gm_ClockSeason(&ai_Time);
    if (k == 3)
        k = 0;
    else if (k == 0 || k == 2)
        k = 1;
    else if (k == 1)
        k = 2;
    d3_SetLightSet(k, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 1, 0);
    ai_Unk4cef80();
    if (k == 0)
        ai_Unk4e3620("cd2\\satisfaktion.mp3");
    else if (k == 1)
        ai_Unk4e3620("cd2\\DerSeuchenzug.mp3");
    else
        ai_Unk4e3620("cd2\\AmKuehlenGrabe.mp3");
    snd1 = sb_LoadSampleBank("duell_sfx.sbf", 0);
    snd2 = snd_LoadVoiceBank("duell.sbf", 0);
    scene = ai_Unk4e2bf0("cutscenes\\duell\\enter_DUELL.esc");
    memcpy(ca, a, 0x21c);
    memcpy(cb, b, 0x21c);
    ai_Unk92bca4 = (int)ca;
    ai_Unk92bca0 = (int)cb;
    ai_Unk6f0da8[0] = 0;
    ai_Unk6f0da8[1] = 0;
    gm_CreateCharModel(ca + 0x10, d3_CurrentScene, 0, 0, 0, "dummy_DUELLANT_A_ENTRY");
    gm_CreateCharModel(cb + 0x10, d3_CurrentScene, 0, 0, 0, "dummy_DUELLANT_B_ENTRY");
    oa = *(char **)(ca + 0x1b4 - 0x10 + 0x10);
    ob = *(char **)(cb + 0x1b4 - 0x10 + 0x10);
    if (oa == 0 || ob == 0) {
        err_Message("cut_MainDuell(): Could not create character");
        return;
    }
    ch_SetHandObject(oa, 2, "ob_PISTOLE");
    ch_SetHandObject(ob, 2, "ob_PISTOLE");
    *(unsigned char *)(oa + 0x240) = 4;
    *(unsigned char *)(ob + 0x240) = 4;
    for (i = 0; i < 3; i++) {
        d2_SetForm(ai_Unk60d368, 0);
        io_PrintText("$C");
        io_PrintText(*(char *)(m + 0x7c) * 3 + 0x10cb + i);
        sprintf(buf, "_DUELL_INTRO_NEU_0%d", i);
        n = ai_Unk4e32b0(-7, buf, 0, -1);
        while (n != 0 && sb_IsPlaying(n) != 0)
            ai_Unk4e3420(2);
    }
    ch_WalkToObject(oa, d3_FindObject(0, 0x100, 0, "dummy_DUELL_START_A"));
    ch_AddAction(oa, 0x35, d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_A"));
    ch_WalkToObject(ob, d3_FindObject(0, 0x100, 0, "dummy_DUELL_START_B"));
    ch_AddAction(ob, 0x35, d3_FindObject(0, 0x100, 0, "dummy_DUELL_SCHUSS_B"));
    d2_SetForm(ai_Unk60d368, 0);
    io_PrintText("$C");
    io_PrintText(0x10dd);
    ai_Unk4e3420(4);
    io_PrintText("$C");
    snd_PlayVoice(-7, "_DUELL_INTRO_BEGRUESSUNG_RUECKEN", 0, -1);
    ai_Unk4e3830(scene, 0);
    rec = (char *)ai_Unk4e50e0(*(int *)(ca + 0x10 - 0x10 + 0));
    /* the seconds: with the first round of kind 1 each side gets a shot object (callback 0x4de640, "kampf/schiessen_pistole") */
    hita = 0;
    hitb = 0;
    for (i = 0; i < 3 && ai_Unk6f0d94 == 0; i++) {
        ai_Unk4e3830(scene, scene);
        rec = (char *)ai_Unk4e50e0(*(int *)(m + 0x34));
        ai_Unk6f0da8[2] = 0;
        ai_Unk6f0da8[3] = 0;
        ai_Unk6f0da8[4] = 0;
        ai_Unk6f0da8[5] = 0;
        if (rec[0x94] == 2 || rec[0x94] == 3) {
            ai_Unk4de900(cb, "B");
            ai_Unk4de900(ca, "A");
        } else {
            ai_Unk4de900(ca, "A");
            ai_Unk4de900(cb, "B");
        }
        if ((ai_Unk6f0da8[2] != 0 || ai_Unk6f0da8[4] != 0) && (ai_Unk6f0da8[3] != 0 || ai_Unk6f0da8[5] != 0)) {
            n = ai_Unk4e52f0(9);
            d2_SetForm(ai_Unk60d368, 0);
            io_PrintText(n + 0x1113);
            snd_PlayVoice(-5, "_DUELL_INTRO_C2_DOKTOR", 0, n);
            ai_Unk4e3420(5);
        }
        d2_SetForm(ai_Unk60d368, 0);
        io_PrintText("$C");
    }
    /* result: who is down (the lower life 0x4fc8e0), the texts 0x111c / 0x111d, the damage 0x6f0da8 / 0x6f0da9 off the life */
    if (ai_Unk6f0d94 != 0)
        w = gm_HealthRatio(cb) < gm_HealthRatio(ca) ? cb : ca;
    else
        w = 0;
    if (w != 0 && w[0x166] != 0 && amt_FindOffice(w, buf, ids) != 0 && *(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        amt_SendOfficeMessage(ids[0], 0, 0, 4, 0xff);
        for (i = 0; i < 768; i++) {
            if (ai_Chars[i][2] == 6 || ai_Chars[i][2] == 7) {
                txt_Format(buf, 0x10c7, *(unsigned short *)w, (w[9] != 0) + 0x10c8);
                he_ShowNews(*(int *)(ai_Chars[i] + 4), -1, buf, 0, 0x5fe, &ch_EmptyName);
            }
        }
    }
    ai_Unk4e5360();
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        cm_SendRelations(*(int *)(cb + 4), *(int *)(ca + 4), 0xf, 1);
        cm_SendRelations(*(int *)(ca + 4), *(int *)(cb + 4), 0xf, 1);
    }
    ai_Unk4e3060();
    ai_Unk4e3250(oa);
    ai_Unk4e3250(ob);
    ai_Unk4e2d90(scene);
    ai_Unk4e2d40(scene);
    ai_Unk4e30c0();
    sb_UnloadSampleBank(snd1);
    sb_UnloadSampleBank(snd2);
    ai_Unk4cefa0();
    ai_Unk4e3640();
}
/* 0x4dfa20, after a duel (the message m: +0x34 / +0x38 the duellists): when one of them is down (life 0x4fc8e0 <= 0) the owner of the
   other gets the text 0x143e8f8 (0x53dda0, kind 0x5ec); when both stand each side with the kind 6 / 7 gets "0x11ca" + the text 0x143e8f4
   (0x4e48b0) and the relation is changed (0x4f2e30); a caught person (+0x1b1) gets the text 0x143e8fc. Returns 0 */

int ai_Unk4dfa20(char *m)
{
    char buf[0x200];
    char *a, *b;
    int flag;

    flag = 0;
    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0)
        return 0;
    if (b[0x1b1] == 0 && a[0x1b1] == 0) {
        if (gm_HealthRatio(a) <= 0.0f) {
            if (b[2] != 6 && b[2] != 7)
                return 0;
            he_ShowNews(ai_CharInts[*(unsigned short *)b][0], -1, ai_Unk143e8f8, 0, 0x5ec, 0);
            return 0;
        }
        if (gm_HealthRatio(b) <= 0.0f) {
            if (a[2] != 6 && a[2] != 7)
                return 0;
            he_ShowNews(ai_CharInts[*(unsigned short *)a][0], -1, ai_Unk143e8f8, 0, 0x5ec, 0);
            return 0;
        }
        if (b[2] == 6 || b[2] == 7) {
            txt_Format(buf, 0x11ca, *(unsigned short *)b);
            strcat(buf + 12, ai_Unk143e8f4);
            ai_Unk4e48b0(a, *(int *)(m + 0x10), buf + 12, &ch_EmptyName);
            flag = 1;
        }
        if (a[2] == 6 || a[2] == 7) {
            txt_Format(buf, 0x11ca, *(unsigned short *)a);
            strcat(buf + 12, ai_Unk143e8f4);
            ai_Unk4e48b0(b, *(int *)(m + 0x10), buf + 12, &ch_EmptyName);
            flag = 1;
        }
        if (flag != 0) {
            int ra = -1;

            if (*(int *)(a + 0x170) != 0)
                ra = *(int *)(*(int *)(a + 0x170) + 1);
            else if (*(int *)(b + 0x170) != 0)
                ra = *(int *)(*(int *)(b + 0x170) + 1);
            gv_Accuse(0x14, 1, *(int *)(a + 4), *(int *)(b + 4), ra);
        }
        return flag;
    }
    if (b[2] == 6 || b[2] == 7)
        he_ShowNews(ai_CharInts[*(unsigned short *)b][0], -1, ai_Unk143e8fc, 0, 0x5ec, 0);
    if (a[2] == 6 || a[2] == 7)
        he_ShowNews(ai_CharInts[*(unsigned short *)a][0], -1, ai_Unk143e8fc, 0, 0x5ec, 0);
    return 0;
}
/* 0x4dfcb0, the question "accept the duel?" (text 0x10de, "cutscenes\\duell", a box with the two answers): m + 0x94 = 1 for the first answer, 0 for the second */

void ai_Unk4dfcb0(char *m)
{
    int v, dlg;

    v = d2_OpenForm(0, 0, "cutscenes\\duell");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    io_PrintText(0x10de, *(int *)ai_Unk6f0d74);
    m[0x94] = 0;
    m[0x14] = ch_EmptyName;
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, ai_Unk60d364 * 0x4b, 1);
    while (sv_RunFrame(ai_Unk4dfcb0, ai_Unk60d360) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            m[0x94] = 1;
            sv_PopRequest = 1;
        } else if (d2_ClickedId == 1) {
            m[0x94] = 0;
            sv_PopRequest = 1;
        }
    }
    d2_CloseView(v);
}
/* 0x4dfdd0, the choice of the challenged person (text 0x10e6, three answers: m + 0x94 = 2 / 3 / 4); the player's character is the
   one of m + 0x34 / m + 0x38 that is not the first */

void ai_Unk4dfdd0(char *m)
{
    char *a, *b, *w;
    int v, b1, b2, b3, dlg;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    w = b;
    if (*(int *)(a + 4) != *(int *)(ai_Unk6f0d74 + 4))
        w = a;
    v = d2_OpenForm(0, 0, "cutscenes\\duell");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    {
        int t = io_PrintText(0x10e6, ai_Unk6f0d7c, *(unsigned short *)ai_Unk6f0d74, *(unsigned short *)w);

        b1 = d2_GetWinObj(v, 0, t);
        b2 = d2_GetWinObj(v, 0, t + 1);
        b3 = d2_GetWinObj(v, 0, t + 2);
    }
    m[0x94] = 4;
    m[0x14] = ch_EmptyName;
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, ai_Unk60d364 * 0x4b, 1);
    while (sv_RunFrame(ai_Unk4dfdd0, ai_Unk60d360) != 0) {
        ai_Unk4cee20();
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == b1)
                m[0x94] = 2;
            else if (d2_PointerObj == b2)
                m[0x94] = 3;
            else if (d2_PointerObj == b3)
                m[0x94] = 4;
            sv_PopRequest = 1;
        }
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(v);
}
/* 0x4dff80, the answer of a computer challenged person (m + 0x94): with 0x6f0d7c == 0 the question of 0x4dfcb0 (1 / 0 by a roll of 10 > 7),
   else the choice of 0x4dfdd0 (4 with 50 %, 2 with 35 %, 3 with 15 %) */

void ai_Unk4dff80(char *m)
{
    int r;

    if (ai_Unk6f0d7c == 0) {
        if ((unsigned int)ai_Unk4e52f0(10) > 7)
            m[0x94] = 1;
        else
            m[0x94] = 0;
        return;
    }
    r = ai_Unk4e52f0(100);
    if (r > 0x32)
        m[0x94] = 4;
    else if (r > 0xf)
        m[0x94] = 2;
    else
        m[0x94] = 3;
}
/* 0x4dfff0, asks the player (0x4dfcb0 without the flag 0x6f0d7c, else the choice 0x4dfdd0) */

void ai_Unk4dfff0(char *m)
{
    if (ai_Unk6f0d7c == 0)
        ai_Unk4dfcb0(m);
    else
        ai_Unk4dfdd0(m);
}
/* 0x4e0010, plays the scene "cutscene_toby.ed3" and closes it */

int ai_Unk4e0010(int scene)
{
    ai_Unk4e2e60("cutscene_toby.ed3");
    ai_Unk4e3830(scene, 0);
    ai_Unk4e3830(scene, 0);
    ai_Unk4e3420(5);
    return ai_Unk4e30c0();
}
/* 0x4e0040, tells the others of the party m (+0x30 count, +0x34 ids; +0x7d: the challenger is not the first) about the duel: the text
   0x11c8 (the name of the first) to every one of the kinds 6 / 7 after the first (0x4e48b0), with 0x11c5 + the text 0x143e8e4 for the first
   when m + 0x7d is 0. Returns 1 */

int ai_Unk4e0040(char *m)
{
    char buf[0x200];
    char *p, *q;
    int i;

    if (m[0x7d] == 0) {
        p = (char *)gm_FindChar(*(int *)(m + 0x34));
        if (p == 0)
            return 0;
        if (p[2] == 6 || p[2] == 7) {
            txt_Format(buf, 0x11c5);
            strcat(buf + 8, ai_Unk143e8e4);
            ai_Unk4e48b0(p, m, buf + 8, &ch_EmptyName);
        }
        txt_Format(buf, 0x11c8, *(unsigned short *)p);
    } else {
        p = (char *)gm_FindChar(*(int *)(m + 0x34));
        if (p == 0)
            return 0;
        txt_Format(buf, 0x11c8, *(unsigned short *)p);
    }
    for (i = 1; i < (unsigned char)m[0x30]; i++) {
        q = (char *)gm_FindChar(*(int *)(m + 0x34 + i * 4));
        if (q != 0 && (q[2] == 6 || q[2] == 7))
            ai_Unk4e48b0(q, m, buf, &ch_EmptyName);
    }
    return 1;
}
/* 0x4e01e0, one execution (the message m: +0x34 the delinquent, +0x38 / +0x3c / +0x40 the others, +0x7c the way 0 .. 4: head, gallows (1, 2),
   shovel, cannon; +0xc the owner): the scene "Richtplatz.ed3" with a random sky, the names of the four in the text, the intro, the
   way's own scene, the pause dur[kind], the exit scene; the delinquent's life goes down by (life + 5). ROUGH */

void ai_Unk4e01e0(char *m)
{
    int dur[5] = {0x36b0, 0x2ee0, 0x2ee0, 0x3a98, 0x2710};
    char buf[0x100];
    char *v, *p1, *p2, *p3, *nm;
    int w;

    w = ai_Unk4e52f0(3);
    v = (char *)gm_FindChar(*(int *)(m + 0x34));
    if (v == 0)
        return;
    p1 = (char *)gm_FindChar(*(int *)(m + 0x38));
    p2 = (char *)gm_FindChar(*(int *)(m + 0x3c));
    p3 = (char *)gm_FindChar(*(int *)(m + 0x40));
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    ai_Unk4e2e60("Richtplatz.ed3");
    d3_SetLightSet(0, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 0, 0);
    if (w == 1)
        ai_Unk4e32f0(sv_SkyHeavy + ai_Unk4e52f0(3) * 0x4c);
    else if (w == 2)
        ai_Unk4e32f0(sv_SkyHeavy - 0xe8 + ai_Unk4e52f0(3) * 0x4c);
    else
        ai_Unk4e32f0(sv_SkyHeavy - 0x218 + ai_Unk4e52f0(4) * 0x4c);
    ai_Unk4e3620("cd2\\ArmerDelinquent.mp3");
    if (w == 1)
        ai_Unk4e33d0(0x1f4);
    nm = v != 0 ? (char *)gm_CharNameRecord(*(unsigned short *)v) : (char *)gm_NameRecord((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf, "%s ", nm + 4);
    nm = p1 != 0 ? (char *)gm_CharNameRecord(*(unsigned short *)p1) : (char *)gm_NameRecord((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf + strlen(buf), "%s ", nm + 4);
    nm = p2 != 0 ? (char *)gm_CharNameRecord(*(unsigned short *)p2) : (char *)gm_NameRecord((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf + strlen(buf), "%s ", nm + 4);
    nm = p3 != 0 ? (char *)gm_CharNameRecord(*(unsigned short *)p3) : (char *)gm_NameRecord((unsigned short)util_RandomU(2), (unsigned short)util_RandomU(0x5e) + 1);
    sprintf(buf + strlen(buf), "%s", nm + 4);
    ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-create", buf);
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-intro.", &ch_EmptyName));
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0])
        gm_AddHealth(*(unsigned short *)v, (int)-(*(float *)(v + 0x1c) + 5.0f));
    switch (m[0x7c]) {
    case 0:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-kopf.e", &ch_EmptyName);
        break;
    case 1:
    case 2:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-galgen", &ch_EmptyName);
        break;
    case 3:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-shaufe", &ch_EmptyName);
        break;
    case 4:
        ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-kanone", &ch_EmptyName);
        break;
    }
    ai_Unk4e3570(dur[m[0x7c]]);
    ai_Unk4e3060();
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\hinrichtung\\hinrichtung-exit.e", &ch_EmptyName));
    if (v != 0)
        ai_Unk4e33f0();
    ai_Unk4e3390();
    cm_FightStart = 0;
    ai_Unk4e30c0();
    d2_CutsceneActive = 0;
    ai_Unk4e3640();
}
/* 0x4e0580, announces a wedding (the message m: +0x34 / +0x38 the two, of different sex): the text 0x11c3 for the first (0x4e48b0), the text
   0x11c4 (both names) to every character of the kinds 6 / 7 who is neither of them (0x53dda0); 1 when the first is a person of the kinds
   6 / 7, else the two get the state 0x1c8 (0x4c4a60) */

int ai_Unk4e0580(char *m)
{
    char buf[0x200];
    char *a, *b;
    int i;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a != 0) {
        if (b != 0 && a[9] != b[9]) {
            if (a[2] == 6 || a[2] == 7) {
                txt_Format(buf, 0x11c3, *(unsigned short *)b);
                ai_Unk4e48b0(a, m, buf, "_PA_WERBUNG_EVENT_ZUSAGE_M1_00");
            }
            if (a[2] == 6 || a[2] == 7 || a[2] == 5) {
                txt_Format(buf, 0x11c4, *(unsigned short *)a, *(unsigned short *)b);
                for (i = 0; i < 768; i++) {
                    if ((ai_Chars[i][2] == 6 || ai_Chars[i][2] == 7) && ai_CharInts[i][0] != *(int *)(a + 4) &&
                        ai_CharInts[i][0] != *(int *)(b + 4))
                        he_ShowNews(ai_CharInts[i][0], -1, buf, 0, 0x5fe, "_PA_WERBUNG_EVENT_NACHRICHT_HOCHZEIT_M1_00");
                }
            }
            if (a[2] == 6 || a[2] == 7)
                return 1;
        }
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000);
    }
    if (b != 0)
        cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x40000);
    return 0;
}
/* 0x4e06d0, ends a wedding scene s (s[0] the scene, s[1] its view, s[2] a sample bank, s[3] a playing sample): the exit scene
   "cutscenes\\hochzeit\\hochzeit-exit.esc", the globals reset. Returns 1 */

int ai_Unk4e06d0(int *s)
{
    int v;

    ai_Unk4e3060();
    if (s[3] != 0)
        sb_Stop(s[3], 0);
    v = s[2];
    s[3] = 0;
    sb_UnloadSampleBank(v);
    if (s[1] != -1)
        d2_CloseView(s[1]);
    if (s[0] != -1)
        ai_Unk4e2d90(s[0]);
    s[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-exit.esc", &ch_EmptyName);
    ai_Unk4e2c70(s[0]);
    s[0] = -1;
    s[1] = -1;
    cm_FightStart = 0;
    ai_Unk4e30c0();
    ai_Unk4e3640();
    d2_CutsceneActive = 0;
    return 1;
}

/* 0x4e0770, the wedding (the message m: +0x34 / +0x38 the couple): the scenes "cutscenes\\hochzeit\\hochzeit-create / -intro / -reden1 .. -reden3",
   "KIRCHE_HOCHZEIT.ed3", "cd2\\Hochzeit.mp3", the sample bank "ADDON\\HOCHZEIT_NEU.sbf"; each speech: the text (0x1aa9 .. 0x1aad), the voice
   "_HOCHZEIT_NEU_MAIN_0n" waited for with 0x426f20, pauses 0x3e8 / 0x1f4 / 0x5dc through 0x4e3490, which calls the cleanup 0x4e06d0 and
   returns nonzero when the player aborts. ROUGH: the pauses between the speeches are from the disassembly, not checked */
#define G25_STEP(ms) if (ai_Unk4e3490((ms), ai_Unk4e06d0, st) != 0) goto done
#define G25_VOICE(tag)     h = ai_Unk4e32b0(-7, tag, 0);     if (sb_IsPlaying(h) != 0) {         do {             G25_STEP(0x3e8);         } while (sb_IsPlaying(h) != 0);     }     G25_STEP(0x3e8)

/* 0x4e0770 */
int ai_Unk4e0770(char *m)
{
    char buf[0x100];
    char *a, *b, *t;
    int st[4];
    int v, h;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    if (a == 0)
        return 0;
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (b == 0)
        return 0;
    if (a[2] == 6)
        ai_Unk4b44e0(a, 0x28);
    if (b[2] == 6)
        ai_Unk4b44e0(b, 0x28);
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000);
    cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x40000);
    if (a[9] != 0) {
        t = a;
        a = b;
        b = t;
    }
    sprintf(buf, "%s %s %i", gm_CharNameRecord(*(unsigned short *)a) + 4, gm_CharNameRecord(*(unsigned short *)b) + 4, 1);
    st[2] = ai_Unk4e3270("ADDON\\HOCHZEIT_NEU.sbf", 0);
    ai_Unk4e2e60("KIRCHE_HOCHZEIT.ed3");
    ai_Unk4e3620("cd2\\Hochzeit.mp3");
    v = d2_OpenForm(0, 0, "cutscenes\\hochzeit");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetViewEnabled(v, 0);
    st[1] = v;
    st[3] = 0;
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-create.esc", buf);
    ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-intro.esc", &ch_EmptyName);
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden1.esc", &ch_EmptyName);
    G25_STEP(0x5dc);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aa9, *(unsigned short *)b, *(unsigned short *)a);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_00");
    d2_SetForm(v, 0);
    io_PrintText("$C");
    d2_SetViewEnabled(v, 0);
    if (ai_Unk4e2cc0(st[0], ai_Unk4e06d0, st) != 0)
        goto done;
    G25_STEP(0x3e8);
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden2.esc", &ch_EmptyName);
    d2_SetForm(v, 0);
    d2_SetViewEnabled(v, 1);
    io_PrintText(0x1aac, *(unsigned short *)b, *(unsigned short *)a);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_03");
    d2_SetForm(v, 0);
    io_PrintText("$C");
    d2_SetViewEnabled(v, 0);
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aab);
    G25_VOICE("_HOCHZEIT_NEU_M");
    if (ai_Unk4e2cc0(st[0], ai_Unk4e06d0, st) != 0)
        goto done;
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden3.esc", &ch_EmptyName);
    d2_SetViewEnabled(v, 0);
    d2_SetForm(v, 0);
    io_PrintText("$C");
    G25_STEP(0x5dc);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aaa, *(unsigned short *)b, *(unsigned short *)a);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_01");
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 0);
    d2_SetForm(v, 0);
    io_PrintText("$C");
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aab);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_02");
    G25_STEP(0x1f4);
    if (ai_Unk4e2cc0(st[0], ai_Unk4e06d0, st) != 0)
        goto done;
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden4.esc", &ch_EmptyName);
    d2_SetForm(v, 0);
    io_PrintText(0x1aad);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_04");
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 0);
    ai_Unk4e06d0(st);
done:
    return 1;
}
/* 0x4e0e90, a divorce / separation message m (+0x34 / +0x38 the two): when the partner (+0x5c of the first) is not of the kind 0xf the
   first, if a person of the kinds 6 / 7, gets the text 0x11c1 + 0x143e8c8 (0x4e48b0) and 1 is returned; else the second gets the state
   (0x4c4e00, 1). Returns 0 otherwise */

int ai_Unk4e0e90(char *m)
{
    char buf[0x200];
    char *a, *b, *p;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a != 0 && b != 0) {
        p = (char *)gm_FindChar(*(int *)(a + 0x5c));
        if (p != 0 && p[2] != 0xf) {
            if (a[2] == 6 || a[2] == 7) {
                txt_Format(buf, 0x11c1);
                strcat(buf + 8, ai_Unk143e8c8);
                ai_Unk4e48b0(a, m, buf + 8, &ch_EmptyName);
            }
            return 1;
        }
        cm_SendKillCharacter(*(int *)(b + 4), 1);
    }
    return 0;
}
/* 0x4e0f70, a birth (the message m: +0x34 the mother, +0x38 the child; +0x30 count): the scene "Geburt.ed3" with "cutscenes\\geburt\\enter_Geburt.esc",
   the screams "_GEBURT_GESCHREI" (2 .. 5 of them, random pauses), the view "cutscenes\\geburt" with the text 0x1aae (a boy) / 0x1aaf and
   the name box ("$L%t" + the child's name), waits for a key (0x1468410 == 0x1c) or the end of the frame wait, then the field
   blocks (0x4c3ba0 / 0x4c3c00) for the new person */

int ai_Unk4e0f70(char *m)
{
    char name[0x20];
    char *a, *b, *t;
    int snd, sc, v, w, n;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a == 0 || b == 0)
        return 0;
    n = 8;
    cm_BeginFieldBlock(b, *(int *)(b + 4));
    cm_AddFieldDelta(1, 1, 2, &n);
    cm_SendFieldDeltas();
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    snd = ai_Unk4e3270("GEBURT.sbf", 0);
    ai_Unk4e3620("cd2\\Geburt.mp3");
    ai_Unk4e2e60("Geburt.ed3");
    sc = ai_Unk4e2bf0("cutscenes\\geburt\\enter_Geburt.esc");
    if (ai_Unk4e3490(0x1964, 0, 0) == 0) {
        if (a[9] == 0)
            n = ai_Unk4e52f0(2) + (b[9] == 0 ? 0 : 2);
        else
            n = ai_Unk4e52f0(2) + (b[9] == 0 ? 4 : 6);
        ai_Unk4e32d0(-8, "_GEBURT", n, 0);
        if (ai_Unk4e3490(0x3e8, 0, 0) == 0) {
            for (n = ai_Unk4e52f0(4) + 2; n > 0; n--)
                ai_Unk4e32d0(-8, "_GEBURT_GESCHREI", ai_Unk4e52f0(5), (unsigned short)util_RandomU(0x800) + 0x3e8);
            ai_Unk4e3490(0x1b58, 0, 0);
        }
    }
    v = d2_OpenForm(0, 0, "cutscenes\\geburt");
    d2_CentreViewWindows(v);
    d2_SetForm(v, 1);
    io_PrintText(b[9] == 0 ? 0x1aae : 0x1aaf);
    d2_SetForm(v, 2);
    w = d2_GetWinObj(v, 2, io_PrintText("$L%t", b + 0x30));
    d2_SetObjMax(w, 0xf);
    d2_FocusInput(w);
    do {
        if (kb_Last == 0x1c || d2_FocusedInput != 0)
            break;
        d2_GetObjValue(w);
        strcpy(name, (char *)d2_GetObjValue(w));
        if (name[0] != 0)
            break;
        d2_FocusInput(w);
    } while (sv_RunFrame(0, ai_Unk60d360) != 0 && ai_Unk6f2438 == 0);
    cm_BeginFieldBlock(b, *(int *)(b + 4));
    cm_AddFieldDelta(1, 0x10, 0x30, &n);
    cm_SendFieldDeltas();
    d2_CloseView(v);
    ai_Unk4e3060();
    ai_Unk4e2d90(sc);
    ai_Unk4e2c70(sc);
    snd_ClearQueue();
    ai_Unk4e3290(snd);
    cm_FightStart = 0;
    ai_Unk4e30c0();
    ai_Unk4e3640();
    d2_CutsceneActive = 0;
    return 1;
}
/* 0x4e1220, 1 when the life (0x4fc820) of the first of m is not above 0 */

int ai_Unk4e1220(char *m)
{
    if (gm_CharacterHealth(gm_FindChar(*(int *)(m + 0x34))) <= 0.0f)
        return 1;
    return 0;
}
/* 0x4e1250, a death (the message m: +0x34 the dying person): the scene "Tod.ed3" with "cutscenes\\tod\\enter_TOD.esc", the bells "GLOCKEN_r", the
   voices "_TOD_HS" 0 .. 6 with the pauses 0xbb8 / 0xfa0 / 0x7d0 / 0x1f4, a fade out over 0x339 ticks (0x44a170); then by the flags +0x1cc
   (1 = heir / 2) of the person and whether 0x506480 finds a successor the text 0x40fb / 0x40fc / 0x1ab3 / 0x1ab4 (0x4e5470) and the
   field blocks 0x1c / 0x24 are sent (0x4c3c00), or the death is reported to the player's owner (0x5c4150). ROUGH */

int ai_Unk4e1250(char *m)
{
    char buf[0x400];
    char big[0x1c00];
    char *a;
    int ages[6] = {8, 10, 12, 15, 18, 20};
    int stage, i, sc, snd, t0, rc, ok;
    float f, v[4];

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    stage = 6;
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    for (i = 0; i < 6; i++) {
        if ((unsigned short)ai_TimeHour < ages[i]) {
            stage = i;
            break;
        }
    }
    ai_Unk4e3620("cd2\\AmKuehlenGrabe.mp3");
    snd = snd_LoadVoiceBank("tod.sbf", 1);
    ai_Unk4e2e60("Tod.ed3");
    ai_Unk4e32f0("Sky_Schoen_02");
    d3_SetLightSet(stage, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 0, 0);
    sc = ai_Unk4e2c30("cutscenes\\tod\\enter_TOD.esc", &ch_EmptyName);
    rc = ai_Unk4e3490(0xbb8, 0, 0);
    if (rc == 0) {
        for (i = 0; i < 7; i++)
            ai_Unk4e32d0(-8, "_TOD_HS", i, i == 0 ? 0 : i == 1 ? 0 : i == 2 ? 0xfa0 : i == 3 ? 0xbb8 : i == 4 ? 0x7d0 : i == 5 ? 0xfa0 : 0x1f4);
        rc = ai_Unk4e3490(0x2af8, 0, 0);
        if (rc == 0) {
            t0 = d3_Time;
            while (d3_Time < t0 + 0x339 && rc == 0) {
                if (sv_RunFrame(0, ai_Unk60d360) != 0 && dm_L.rup != 0)
                    rc = 1;
                f = (float)(d3_Time - t0) * 0.0012121212f;
                if (!(f < 1.0f))
                    f = 1.0f;
                d3_SetFogSet(0, 1, f);
            }
            d3_SetFogSet(0, 1, 0x3f800000);
            ai_Unk4e3490(0x2cec, 0, 0);
            rc = 0;
        }
    }
    sb_PlaySampleI(0, "GLOCKEN_r", 1, 0x3f, 0x7f, 0, 0);
    d2_CutsceneActive = 0;
    ok = gm_Unk506480(a, v);
    if (ok != 0) {
        if ((*(unsigned int *)(a + 0x1cc) & 1) != 0 && (*(unsigned int *)(a + 0x1cc) & 2) == 0) {
            txt_Format(buf, 0x40fb, 0x40f5, *(unsigned short *)a);
            if (ai_Unk4e5470(buf, 0x10) == 0)
                ai_FamilyTreeWindow(ai_CharInts[ai_PlayerId][0], 0xa, ai_Unk1440c88, ai_Unk60d360);
            else {
                v[2] = 100.0f;
                cm_BeginFieldBlock(a, *(int *)(a + 4));
                cm_AddFieldDelta(4, 1, 0x1c, &v[1]);
                cm_AddFieldDelta(4, 1, 0x24, &v[2]);
                cm_SendFieldDeltas();
                cm_SendSetField(*(int *)(a + 4), 0x1cc, 4, 2, 0);
            }
        } else
            ai_FamilyTreeWindow(ai_CharInts[ai_PlayerId][0], 0xa, ai_Unk1440c88, ai_Unk60d360);
    } else {
        if ((*(unsigned int *)(a + 0x1cc) & 1) != 0 && (*(unsigned int *)(a + 0x1cc) & 2) == 0) {
            txt_Format(buf, 0x40fc, 0x40f5, *(unsigned short *)a);
            ai_Unk4e5470(buf, 0x10);
            cm_BeginFieldBlock(a, *(int *)(a + 4));
            cm_AddFieldDelta(4, 1, 0x1c, &v[1]);
            cm_AddFieldDelta(4, 1, 0x24, &v[2]);
            cm_SendFieldDeltas();
            cm_SendSetField(*(int *)(a + 4), 0x1cc, 4, 2, 0);
        } else if ((ai_GameFlags & 8) != 0) {
            a[8] = 0;
            ai_GameOver = 1;
            if (ok != 0)
                txt_Format(buf, 0x1ab4);
            else
                txt_Format(buf, 0x1ab3, *(unsigned short *)a);
            ai_Unk4e5470(buf, 0);
            sv_PopTo((unsigned)&main_GameLoop);
        } else {
            gm_ForfeitEstate(a, big, 1);
            ai_Unk4e5470(big, 0x10);
        }
    }
    ai_Unk4e3060();
    ai_Unk4e2d90(sc);
    ai_Unk4e2c70(sc);
    ai_Unk4e3390();
    ai_Unk4e30c0();
    snd_ClearQueue();
    sb_UnloadSampleBank(snd);
    ai_Unk4e3640();
    cm_FightStart = 0;
    return 0;
}
/* 0x4e1700, a bankruptcy (the message m: +0x34 the person): the scene "Pleite.ed3" with "cutscenes\\pleite\\enter_Pleite.esc", a sky by the age
   0x770ae4, the voices "_PLEITE_HS" 0 .. 5 (pauses 0x9c4 / 0x1f4 / 0x251c / 0x1194), the text 0x1ab1 (0x3fda with the flag 0x6f23f8 & 4)
   as a message (0x4e5470); with the flag & 8 the person is removed (+8 = 0, 0x53bb20). ROUGH */

int ai_Unk4e1700(char *m)
{
    char buf[0x200];
    char big[0x1c00];
    char *a;
    int ages[6] = {8, 10, 12, 15, 18, 20};
    int i, stage, sc, snd, rc;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    stage = 6;
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    for (i = 0; i < 6; i++) {
        if ((unsigned short)ai_TimeHour < ages[i]) {
            stage = i;
            break;
        }
    }
    snd = snd_LoadVoiceBank("pleite.sbf", 1);
    ai_Unk4e3620("cd2\\DerSeuchenzug.mp3");
    ai_Unk4e2e60("Pleite.ed3");
    d3_SetLightSet(stage, 0, 0x3f800000, 1);
    d3_SetFogSet(0, 0, 0);
    ai_Unk4e32f0("sky_dunkel_01");
    sprintf(buf, "%i", stage);
    sc = ai_Unk4e2c30("cutscenes\\pleite\\enter_Pleite.esc", buf);
    rc = ai_Unk4e3490(0x1770, 0, 0);
    if (rc == 0) {
        ai_Unk4e32d0(-8, "_PLEITE_HS", 0, 0);
        ai_Unk4e32d0(-8, "_PLEITE_HS", 1, 0x9c4);
        ai_Unk4e32d0(-8, "_PLEITE_HS", 2, 0x1f4);
        rc = ai_Unk4e3490(0x3e80, 0, 0);
        if (rc == 0) {
            ai_Unk4e33d0(0xfa);
            rc = ai_Unk4e3490(0x2af8, 0, 0);
            if (rc == 0) {
                ai_Unk4e32d0(-8, "_PLEITE_HS", 3, 0x251c);
                ai_Unk4e32d0(-8, "_PLEITE_HS", 4, 0x1194);
                ai_Unk4e32d0(-8, "_PLEITE_HS", 5, 0x1f4);
                ai_Unk4e3490(0x5dc0, 0, 0);
            }
        }
    }
    d2_CutsceneActive = 0;
    if ((ai_GameFlags & 4) != 0)
        txt_Format(buf, 0x3fda, *(unsigned short *)a);
    else
        txt_Format(buf, 0x1ab1, *(unsigned short *)a);
    ai_Unk4e5470(buf, 0);
    ai_Unk4e3060();
    ai_Unk4e2d90(sc);
    ai_Unk4e2c70(sc);
    ai_Unk4e3390();
    ai_Unk4e33f0();
    ai_Unk4e30c0();
    snd_ClearQueue();
    sb_UnloadSampleBank(snd);
    ai_Unk4e3640();
    cm_FightStart = 0;
    if ((ai_GameFlags & 8) != 0) {
        a[8] = 0;
        ai_GameOver = 1;
        sv_PopTo((unsigned)&main_GameLoop);
        return 0;
    }
    gm_ForfeitEstate(a, big, 0);
    return 0;
}
/* 0x4e19a0, tells the party m (+0x30 count, +0x34 ids) about the thing +0x7c / +0x80 (text 0x3fd9, then 0x143e8c0 / 0x143e8bc by the flag
   0x6f23f8 & 4): every one of the kinds 6 / 7 gets it (0x4e48b0). Returns 1 */

int ai_Unk4e19a0(char *m)
{
    char buf[0x200];
    char *p;
    int i, o;

    o = gm_FindBuildingById(*(int *)(m + 0x7c));
    if (o == 0)
        return 0;
    txt_Format(buf, 0x3fd9, o, *(int *)(m + 0x80));
    if ((ai_GameFlags & 4) != 0)
        strcat(buf, ai_Unk143e8c0);
    else
        strcat(buf, ai_Unk143e8bc);
    for (i = 0; i < (unsigned char)m[0x30]; i++) {
        p = (char *)gm_FindChar(*(int *)(m + 0x34 + i * 4));
        if (p != 0 && (p[2] == 6 || p[2] == 7))
            ai_Unk4e48b0(p, m, buf, &ch_EmptyName);
    }
    return 1;
}
/* 0x4e1a80, an auction (the message m: +0x7c the building / place, +0x30 count, +0x34 the bidders): for the building kinds 0xb / 0xc / 0xd (forest
   "WALDSTUECK", quarry "STEINBRUCH", mine "MINE") the scene "Versteigerung.ed3" with "ub_STAFELEI" (texture set by the kind), the auctioneer
   "Auktionator" (dummy_AUKTIONATOR) and up to 8 persons on the places "dummy_RP_%02i" (the bidders' own models, else a random
   "buerger_MANN" / "buerger2_MANN" / "handwerker3_MANN"); the voices "_VERSTEIGERUNG_INTRO_%s", "_VERSTEIGERUNG_GEBOTE" (bids, texts 0x1aa2 ..
   0x1aa5: intro, the bid, the winner, nobody) and "_VERSTEIGERUNG_KOMMENTARE"; the winner pays (0x4c3c00 fields 0x69 / 0x6d) and the
   opinion of the others changes (0x4c4830). ROUGH: the bid loop (slots 0x92cb74, the bid steps) is only sketched */

void ai_Unk4e1a80(char *m)
{
    char buf[0x100];
    char who[0x40];
    int ids[8];
    int who_ok[8];
    char *b, *kind;
    int i, n, bid, best, round, pl, h, v, scene;

    b = (char *)gm_BuildQuery(1, 1, *(int *)(m + 0x7c));
    best = -1;
    scene = ai_Unk4e52f0(3);
    ai_Unk4e3620(0);
    if (b == 0)
        goto done;
    kind = (char *)ai_BTypes + *(unsigned char *)b * 0x24d;
    if (kind[0] == 0xb)
        sprintf(who, "%s", "WALDSTUECK");
    else if (kind[0] == 0xd)
        sprintf(who, "%s", "MINE");
    else if (kind[0] == 0xc)
        sprintf(who, "%s", "STEINBRUCH");
    else
        goto done;
    n = *(unsigned char *)(m + 0x30);
    for (i = 0; i < n; i++) {
        who_ok[i] = 0;
        ids[i] = *(int *)(m + 0x34 + i * 4);
        who_ok[i] = gm_FindChar(ids[i]) != 0 ? 1 : 0;
    }
    for (; i < 8; i++) {
        ids[i] = -1;
        who_ok[i] = 0;
    }
    ai_Unk4e3270(0, "VERSTEIGERUNG_INTRO.sbf");
    ai_Unk4e3270(0, "VERSTEIGERUNG_GEBOTE.sbf");
    ai_Unk4e3270(0, "VERSTEIGERUNG_KOMMENTARE.sbf");
    ai_Unk4e2e60("Versteigerung.ed3");
    if (d3_FindObject("ub_STAFELEI", 0, 0x40, 0) != 0) {
        if (kind[0] == 0xb)
            v = 0;
        else if (kind[0] == 0xc)
            v = 1;
        else
            v = 2;
        d3if_SelectTextureSet(v, 0, 1, 2);
    }
    /* the auctioneer */
    sprintf(buf, "%s", v == 0 ? "buerger3_DICKER" : v == 1 ? "wirt2_DICKER" : "buergerin2_FRAU");
    sprintf(buf, "%s", "W1");
    h = d3_FindObject("dummy_AUKTIONATOR", 0, 0x100, 0);
    if (h != 0)
        d3_PointToWorld(0, 0, h);
    pl = ai_Unk4e3230(0, 0);
    if (pl != 0)
        ch_PlaceAtObject(pl, h);
    sprintf(who + strlen(who), "%s", "Auktionator");
    /* the persons on the places dummy_RP_00 .. */
    for (i = 0; i < n; i++) {
        sprintf(buf, "dummy_RP_%02i", i);
        h = d3_FindObject(buf, 0, 0x100, 0);
        if (h != 0)
            d3_PointToWorld(0, 0, h);
        if (who_ok[i] != 0 && gm_CharNameRecord(ids[i]) != 0)
            sprintf(buf, "%s", (char *)gm_CharNameRecord(ids[i]) + 4);
        else if (ai_Unk4e52f0(3) == 1)
            strcpy(buf, "buerger_MANN");
        else if (ai_Unk4e52f0(3) == 2)
            strcpy(buf, "buerger2_MANN");
        else
            strcpy(buf, "handwerker3_MANN");
        pl = ai_Unk4e3230(0, 0);
        if (pl != 0)
            ch_PlaceAtObject(pl, h);
    }
    ai_Unk4e2c30("cutscenes\\auktion\\behavior_auktionator.esc", &ch_EmptyName);
    ai_Unk4e2c30("cutscenes\\auktion\\Enterfly_auktion.esc", &ch_EmptyName);
    sprintf(buf, "_VERSTEIGERUNG_INTRO_%s_00", who);
    ai_Unk4e32d0(0, -1, buf, 0, -7);
    sprintf(buf, "_VERSTEIGERUNG_INTRO_%s", who);
    ai_Unk4e32d0(0x1f4, ai_Unk4e52f0(3), buf, 0, 0);
    v = d2_OpenForm("cutscenes\\pacht_main", 0, 0);
    if (cm_SkipEvents == 0)
        d2_SetViewEnabled(v, 0);
    ((void (*)(int, int))d2_PlaceMainWindow)(3, v);
    d2_SetForm(0, v);
    io_PrintText(0x1aa2, 0, 0);
    ai_Unk4e2c70(0);
    /* the bids: up to 6 rounds, the bidders raise by turns (the slots 0x92cb74 hold the bids) */
    for (round = 0; round <= 5; round++) {
        if (ai_Unk4e2c70(0) != 0)
            break;
        ai_Unk4e3830(m, 0);
        if (n > 6 && ai_Unk4e52f0(3) != 0) {
            ai_Unk4e52f0(0x200);
            ai_Unk4e52f0(6, 0);
            ai_Unk4e32d0(ai_Unk4e52f0(4), "_VERSTEIGERUNG_KOMMENTARE", 0, 0);
        }
        for (i = 0; i < n; i++)
            if (*(int *)(ai_Unk92caf0 + i * 0x18 + 0x84) != 0 && best < *(int *)(ai_Unk92caf0 + i * 0x18 + 0x84))
                best = i;
        if (best < 2 || n <= 6)
            ai_Unk4e32d0(ai_Unk4e52f0(2), "_VERSTEIGERUNG_GEBOTE", 0, 0);
        else
            ai_Unk4e32d0(ai_Unk4e52f0(4), "_VERSTEIGERUNG_GEBOTE", 0, 0);
        d2_SetForm(0, v);
        io_PrintText(0x1aa3, 0, 5, 0, best, 0);
        ai_Unk4e3420(5);
    }
    if (best > -1) {
        bid = ai_Time;
        ai_Unk4e32d0(6, "_VERSTEIGERUNG_GEBOTE", 0, 0);
        d2_SetForm(0, v);
        io_PrintText(0x1aa4, bid, 0);
        if (ids[best] == ai_CharInts[ai_PlayerId][0]) {
            cm_BeginFieldBlock(0, best);
            cm_AddFieldDelta(0, 0x69, 1, 4);
            cm_AddFieldDelta(0, 0x6d, 1, 4);
            cm_SendFieldDeltas();
            cm_SendOwnership(ai_PlayerCharC, 0, 0);
            gm_FirstBuilding(1);
            gm_Query(0x115, 0, 6, 2, 0);
            cm_SendMoveGoodsUnchecked(util_RandomU(5) % 5, 0, 0, 0);
        }
    } else {
        ai_Unk4e32d0(7, "_VERSTEIGERUNG_GEBOTE", 0, 0);
        d2_SetForm(0, v);
        io_PrintText(0x1aa5, 0);
        if (ids[0] == ai_CharInts[ai_PlayerId][0])
            cm_SendOwnership(ai_PlayerCharC, 0, 0);
    }
    ai_Unk4e32d0(0x1f4, 8, "_VERSTEIGERUNG_GEBOTE", 0, 0);
    ai_Unk4e2c70(ai_Unk4e2c30("cutscenes\\auktion\\Auktion_Outro.esc", &ch_EmptyName));
    d2_CloseView(v);
    ai_Unk4e3060();
    ai_Unk4e2c70(ai_Unk4e2d90(0));
    for (i = 0; i < n; i++)
        if (ai_Unk4e3230(0, 0) != 0)
            ai_Unk4e3250(0);
    ai_Unk4e30c0();
    snd_ClearQueue();
    ai_Unk4e3290(0);
done:
    ai_Unk4e3640();
}
/* 0x4e24c0, the player's answer to an offer m (+0x20 the price, +0x24 .. the persons in the deal, +0x44 the object, +0x48 their count, +0x4c a
   factor): when the asked character r + 0x10 is among them: the dialog "cutscenes\\pacht" (texts 0x1aa6 / 0x1aa7) with a slider between the two
   limits (0x5bd060 / 0x5085b0 of the price), r + 0x98 = 1 and r + 0x94 = the picked value when the player confirms (0xf8a668 == 0).
   ROUGH: the limits are only read from the calls */

void ai_Unk4e24c0(char *r, char *m)
{
    int i, n, id, lo, hi, v, dlg, sl, cur, t, lim[8];
    char *c;

    *(int *)(r + 0x98) = 0;
    n = *(int *)(m + 0x48);
    id = *(int *)(r + 0x10);
    for (i = 0; i < n && *(int *)(m + 0x24 + i * 4) != id; i++)
        ;
    if (i >= n || *(int *)(m + i * 4) == -1)
        return;
    *(int *)(r + 0x94) = *(int *)(m + 0x20);
    gm_BuildQuery(1, 1, *(int *)(m + 0x44));
    c = (char *)gm_FindChar(*(int *)(r + 0x10));
    if (c == 0)
        return;
    cur = *(int *)(m + 0x20);
    hi = gm_GetGoodsValue(c);
    ai_Unk5bd060(cur, hi, 0x20, lim);
    lo = gm_ConvertMoney(cur, ai_OwnPlayer);
    sl = gm_ConvertMoney(lim[0], ai_OwnPlayer);
    if (hi < cur)
        return;
    v = d2_OpenForm("cutscenes\\pacht", 0, 0);
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_ClickedId = -1;
    d2_SetForm(v, 0);
    t = io_PrintText(0x1aa6, ai_OwnPlayer);
    t = io_PrintText(0x1aa7);
    dlg = d2_GetWinObj(v, 0, t);
    d2_SetObjValue(dlg, lo, sl, lo);
    dlg = d2_WinOpenSlider((*(short *)(d2_LastWindowPtrC + 8) - 300) / 2 - 8, 8, 300, 0, 100, 0x75, 0x42, d2_LastWindow);
    ai_Unk4e53e0(dlg, 0x5dc, 1);
    d2_ClickedId = -1;
    while (sv_RunFrame(ai_Unk60d360, ai_Unk4e24c0) != 0) {
        if (ai_Unk4e53e0(dlg, 0, 0) == 0)
            sv_PopRequest = 1;
        else if (d2_ClickedId != -1) {
            sv_PopRequest = 1;
            if (d2_ClickedId != 1 && d2_ClickedId == 0) {
                *(int *)(r + 0x98) = 1;
                *(int *)(r + 0x94) = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(dlg, ai_OwnPlayer));
            }
        }
    }
    d2_CloseView(v);
}
/* 0x4e26d0, the answer of a computer character to the offer m of 0x4e24c0: the price (+0x20) is compared with a value the character's
   resources (0x509e60) and a random factor (0x4e5360, 0x5eba9c / 0x5ebb30) give; r + 0x98 = 1 and r + 0x94 = the counter offer rounded to
   a multiple of 32 when it is acceptable. ROUGH: the float expressions are only roughly in the order of the code */

void ai_Unk4e26d0(char *r, char *m)
{
    int i, n, id, pay, v, x;
    float lim, k;
    char *c;

    *(int *)(r + 0x98) = 0;
    n = *(int *)(m + 0x48);
    id = *(int *)(r + 0x10);
    for (i = 0; i < n && *(int *)(m + 0x24 + i * 4) != id; i++)
        ;
    if (i >= n || *(int *)(m + i * 4) == -1)
        return;
    *(int *)(r + 0x94) = *(int *)(m + 0x20);
    gm_BuildQuery(1, 1, *(int *)(m + 0x44));
    pay = *(int *)(m + 0x20);
    c = (char *)gm_FindChar(*(int *)(r + 0x10));
    if (c == 0)
        return;
    lim = (float)(*(int *)(m + 0x4c) * 32000 + pay);
    k = ai_Unk4e5360() * 0.16666667f + 0.3f;
    x = gm_GetGoodsValue(c);
    if ((float)x * k > lim)
        v = (int)lim;
    else {
        k = ai_Unk4e5360() * 0.16666667f + 0.3f;
        x = gm_GetGoodsValue(c);
        v = (int)(lim * x);
    }
    if (v < pay)
        return;
    if (k < ai_Unk4e5360() / v)
        return;
    if (v > pay * 2)
        v = pay * 2;
    v -= pay;
    *(int *)(r + 0x98) = 1;
    x = (int)(ai_Unk4e52f0(v) * *(int *)(m + 0x4c) * 0.1f + pay);
    *(int *)(r + 0x94) = x - x % 32;
}
/* 0x4e2860, the scene of a sale (the message m: +0x14 the object, +0x34 the seller, +0x38 .. +0x44 the buyers): the scene of the object
   (0x4fea50 -> 0x4e2910 with 0x17) or "ob_SALON.ed3", a pause of 5, then every buyer changes his opinion of the seller by +10 (0x4c4b00, both ways) */

void ai_Unk4e2860(char *m)
{
    int o, f, i;
    int *p;

    f = 0;
    ai_Unk4e3620(0);
    gm_FindById(&o, 0, 0, *(int *)(m + 0x14));
    if (o != 0)
        ai_Unk4e2910(o, 0x17);
    else {
        f = 1;
        ai_Unk4e2e60("ob_SALON.ed3");
    }
    ai_Unk4e3420(5);
    p = (int *)(m + 0x38);
    for (i = 0; i < 4; i++, p++) {
        if (*p != -1) {
            cm_SendRelations(*(int *)(m + 0x34), *p, 10, 1);
            cm_SendRelations(*p, *(int *)(m + 0x34), 10, 1);
        }
    }
    if (f != 0) {
        ai_Unk4e3060();
        ai_Unk4e30c0();
    }
    ai_Unk4e3640();
}

/* 0x4e2910, leaves the scene of the object o (+0x29 the id, +0x5d its owner / place, +0 the building kind) for the place id: draws the
   frozen screen (0x6ed924 / 0x6ed92c, the window flag +0x26 & 0x20 of 0x6f0d90) and fades to black ("BLACK", 0x5a ticks) when o is
   the current object (0x6f2308 / 0x6f230c) with that id; else selects the scene of the place (0x76faa0[id], or of the building kind
   0x76fa60[kind]) and runs 0x513050; at the end the stop flag of 0x63349c / 0x6f0d98 is cleared (0x43deb0). Not in the replay mode. ROUGH */
#define G27_FRAME(a, b) d2_BlitPart(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, (a), 0, 0, (b))
/* 0x4e2910 */
void ai_Unk4e2910(char *o, int id)
{
    char *bld;
    int k;

    if (cm_SkipEvents != 0)
        return;
    if (((char *)ai_PlayerBuildingT == o && *(unsigned short *)(o + 0x29) == id) || ((char *)ai_FallbackBuildingT == o && *(unsigned short *)(o + 0x29) == id)) {
        if (((char *)ai_Unk6f0d90)[0x26] & 0x20) {
            d2_ClearSurface(d2_ScreenSurf);
            G27_FRAME(d2_ScreenSurf, ai_Unk6ed92c);
            sv_RunFrame(0, 0x86);
            G27_FRAME(ai_Unk6ed92c, d2_ScreenSurf);
        }
        if (ai_Unk6f0da0 != 0) {
            d2_ReleaseFade(ai_Unk6f0da0);
            ai_Unk6f0da0 = 0;
            d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
        }
        return;
    }
    if (gm_Query(*(int *)(o + 0x5d), 1, 0, id) != 0) {
        if (gm_IsHomeType(o) == 0) {
            if (init_ObjectUniverse[id] != (char)0xfe) {
                if (init_ObjectUniverse[id] == (char)0xff || *(char *)(ai_Unkfd7c50 + 0x325 + init_ObjectUniverse[id] * 0x3e0) != 0)
                    init_LoadObjectScene(id);
                d3if_SelectScene(init_ObjectUniverse[id], 0);
                init_ShowBuildingScene(o, id);
            }
        } else {
            bld = (char *)ai_BTypes + *(unsigned char *)o * 0x24d;
            k = *(unsigned char *)bld;
            if (init_BuildingUniverse[k] != 0) {
                if (init_BuildingUniverse[k] == (char)0xff || *(char *)(ai_Unkfd7c50 + 0x325 + init_BuildingUniverse[k] * 0x3e0) != 0)
                    init_LoadBuildingScene(k);
                k = *(unsigned char *)((char *)ai_BTypes + *(unsigned char *)o * 0x24d);
                d3if_SelectScene(init_BuildingUniverse[k], 0);
                init_ShowBuildingScene(o, 0xfd);
            }
        }
    }
    if (ai_Unk63349c != 0) {
        ai_Unk6f0d98 = ai_Unk63349c;
        d3_InflateScene(ai_Unk63349c & ~2);
        ai_Unk6f0d98 = ai_Unk6f0d98 & ~2;
    }
    if (((char *)ai_Unk6f0d90)[0x26] & 0x20) {
        d2_ClearSurface(d2_ScreenSurf);
        G27_FRAME(d2_ScreenSurf, ai_Unk6ed92c);
        sv_RunFrame(0, 0x86);
        G27_FRAME(ai_Unk6ed92c, d2_ScreenSurf);
    }
    if (ai_Unk6f0da0 != 0) {
        d2_ReleaseFade(ai_Unk6f0da0);
        ai_Unk6f0da0 = 0;
        d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
    }
}

/* ai_Unk925538: view of ai_Unk925520 (tools/data_views.py) */

/* the random seed of the scenes (0x92c4e0), a linear congruence; the number is the high word modulo 0x7fff */
#define G28_HI (*(unsigned short *)((char *)&ai_Unk92c4e0 + 2))  /* 0x92c4e2 */


/* 0x4e2bd0, the window call 0x475590 of the scene helpers; in the replay mode (0x6f0d8c) the second argument is 0 */
int ai_Unk4e2bd0(int a, int b)
{
    if (cm_SkipEvents != 0)
        b = 0;
    return ((int (*)(int, int))d2_SetViewEnabled)(a, b);
}
/* 0x4e2bf0, loads the named thing (0x4287b0, then 0x4299a0) and returns its +0x100; 0 in the replay mode */

int ai_Unk4e2bf0(char *name)
{
    char *p;

    if (cm_SkipEvents == 0) {
        p = (char *)evt_Load(name);
        if (p != 0) {
            evt_RunScript((EVTSCRIPT *)p);
            return *(int *)(p + 0x100);
        }
    }
    return 0;
}
/* 0x4e2c30, the same with a second argument for 0x429c40 */

int ai_Unk4e2c30(char *name, int arg)
{
    char *p;

    if (cm_SkipEvents == 0) {
        p = (char *)evt_Load(name);
        if (p != 0) {
            evt_RunCommand((EVTSCRIPT *)p, arg);
            return *(int *)(p + 0x100);
        }
    }
    return 0;
}
/* 0x4e2c70, runs the frame wait (0x5309a0, flag 0x8000) and 0x4284a0(a) until it says 0 */

int ai_Unk4e2c70(int a)
{
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(0, ai_Unk60d360 | 0x8000) != 0) {
            do {
                if (evt_FindById(a) == 0)
                    break;
            } while (sv_RunFrame(0, ai_Unk60d360 | 0x8000) != 0);
        }
    }
}
/* 0x4e2cc0, like 0x4e2c70, but ends with the callback cb(arg) and 1 as soon as 0x14673a0 is set */

int ai_Unk4e2cc0(int a, int (*cb)(int), int arg)
{
    if (cm_SkipEvents != 0)
        return 0;
    while (sv_RunFrame(0, ai_Unk60d360 | 0x8000) != 0) {
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

int ai_Unk4e2d40(int a)
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

int ai_Unk4e2d90(int a)
{
    int r;

    if (cm_SkipEvents == 0) {
        r = (char *)evt_FindById(a);
        if (r != 0)
            return evt_Stop((EVTSCRIPT *)r);
    }
}
/* 0x4e2dc0, 0x429e90 of every used slot (of 0x80 x 0xc98 bytes at 0x6332b4) whose +0x104 is -2 and +0x124 has bit 1 */

int ai_Unk4e2dc0(void)
{
    int i;
    char *e;

    for (i = 0; i < 0x80; i++) {
        e = ai_Unk6332b4 + i * 0xc98;
        if (*(int *)(e + 0x100) != -1 && (*(unsigned char *)(e + 0x124) & 1) && *(int *)(e + 0x104) == -2)
            evt_Stop((EVTSCRIPT *)e);
    }
    return 0;
}
/* 0x4e2e00, waits (frame wait 0x79706) until no used slot of 0x6332b4 has +0x104 == -2: -1 */

int ai_Unk4e2e00(void)
{
    int i, none;
    char *e;

    none = -1;
    if (sv_RunFrame(0, 0x79706) != 0) {
        do {
            e = ai_Unk6332b4 + 0x104;
            for (i = 0; i < 0x80; i++, e += 0xc98) {
                if (*(int *)(e - 4) != none && *(int *)e == -2)
                    goto wait;
            }
            return none;
        wait:;
        } while (sv_RunFrame(0, 0x79706) != 0);
    }
}
/* 0x4e2e60, loads the scene file name into a free scene ("scenes/*%s"), selects it, sets the detail flags of its +0x1c70 byte by the
   setting 0x6f35ca (bits 3 / 4), freezes the screen (0x6ed924 / 0x6ed92c) when the window flag +0x26 & 0x20 of 0x6f0d90 is set, runs the
   scene's callback (+0x3c8 -> +0x210) once, counts the scene depth 0x6f0d9c and fades from black ("BLACK", 0x5a ticks). Not in the replay mode */

void ai_Unk4e2e60(char *name)
{
    char path[0x80];
    int idx, sh, t, k;
    int (*cb)();

    if (cm_SkipEvents != 0)
        return;
    idx = d3t_FindFreeScene();
    d3if_SelectScene(idx, 0);
    sprintf(path, "scenes/*%s", name);
    d3_LoadScene(path, 0, 0);
    d3if_SelectScene(0, 1);
    d3if_SelectScene(idx, 0);
    if (ai_Unk633738 != 0) {
        if (ai_Unk6f35ca == 0) {
            *(unsigned char *)((char *)ai_Unk633738 + 0x1c70) = (*(unsigned char *)((char *)ai_Unk633738 + 0x1c70) & 0xeb) | 0x8;
            sh = 2;
        } else {
            *(unsigned char *)((char *)ai_Unk633738 + 0x1c70) = (*(unsigned char *)((char *)ai_Unk633738 + 0x1c70) & 0xe7) | 0x4;
            sh = 1;
        }
        d3fl_SetDetail(0x40);
        d3fl_SetShift((struct D3FLMAP *)ai_Unk633738, sh >> 1, 1);
    }
    if (((char *)ai_Unk6f0d90)[0x26] & 0x20) {
        d2_ClearSurface(d2_ScreenSurf);
        G27_FRAME(d2_ScreenSurf, ai_Unk6ed92c);
        sv_RunFrame(0, 0x86);
        G27_FRAME(ai_Unk6ed92c, d2_ScreenSurf);
    }
    if (gm_ClockSeason(ai_Time) == 3)
        sv_UpdateSkyColors();
    k = ai_Unk6f0d9c + 1;
    evt_NoGroup = -2;
    ai_Unk6f0d9c = k;
    if (ai_Unk5fb5b0 != 0) {
        t = *(int *)(ai_Unk5fb5b0 + 0x3c8);
        if (t != 0) {
            cb = *(int (**)())(t + 0x210);
            if (cb != 0)
                cb(ai_Unk5fb5b0, 4, t + 0x214, 0);
        }
    }
    ai_Unk60d36c = ai_Unk5f8438;
    d3if_Prerender(1);
    if (ai_Unk6f0da0 != 0) {
        d2_ReleaseFade(ai_Unk6f0da0);
        ai_Unk6f0da0 = 0;
    }
    d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 0xa);
}
/* 0x4e3060, fades the screen to black ("BLACK", 0x5a ticks) and waits until the fade has reached its end flag (+0 & 4) */

int ai_Unk4e3060(void)
{
    if (cm_SkipEvents == 0) {
        ai_Unk6f0da0 = (char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
        if (!(*ai_Unk6f0da0 & 4)) {
            do {
                sv_RunFrame(0, ai_Unk60d360 | 0x80);
            } while (!(*ai_Unk6f0da0 & 4));
        }
    }
}
/* 0x4e30c0, leaves the scene (depth 0x6f0d9c): waits for the objects 0x60d36c / 0x5f8438 (0x4284a0, bit 1 of +0x124), clears the scene objects (0x4e2dc0 /
   0x4e2e00), the table 0x5fb5b0 +0xb0 (0x45ef60), 0x433b10 (0xfc2dec), 0x6310e8 / e4 and 0x4473f0. ROUGH */

int ai_Unk4e30c0(void)
{
    char *t, *o;
    int id;

    if (cm_SkipEvents != 0 || ai_Unk6f0d9c <= 0)
        return cm_SkipEvents;
    if (ai_Unk60d36c != -1) {
        o = (char *)evt_FindById(ai_Unk60d36c);
        if (o != 0)
            evt_Stop((EVTSCRIPT *)o);
    }
    t = (char *)ai_Unk5fb5b0;
    if (t != 0 && *(int *)(t + 0x3c8) != 0 && *(int *)(*(char **)(t + 0x3c8) + 0x294) != 0)
        (*(int (**)(char *, int, char *, int))(*(char **)(t + 0x3c8) + 0x294))(t, 5, *(char **)(t + 0x3c8) + 0x298, 0);
    id = ai_Unk5f8438;
    if (id != -1) {
        o = (char *)evt_FindById(id);
        while (o != 0 && (*(unsigned char *)(o + 0x124) & 1)) {
            sv_RunFrame(0, ai_Unk60d360 | 0x80);
            o = (char *)evt_FindById(ai_Unk5f8438);
        }
    }
    ai_Unk4e2dc0();
    ai_Unk4e2e00();
    id = ai_Unk60d36c;
    if (id != -1) {
        o = (char *)evt_FindById(id);
        while (o != 0 && (*(unsigned char *)(o + 0x124) & 1)) {
            sv_RunFrame(0, ai_Unk60d360 | 0x80);
            o = (char *)evt_FindById(ai_Unk60d36c);
        }
    }
    t = (char *)ai_Unk5fb5b0;
    if (*(int *)(t + 0xb0) != 0) {
        d3sm_Destroy(*(int *)(t + 0xb0));
        *(int *)((char *)ai_Unk5fb5b0 + 0xb0) = 0;
    }
    d3anim_FreePath(sv_CameraObj);
    d3t_CamMoveI = 0;
    d3t_CamMoving = 0;
    d3if_Reset();
    ai_Unk6f0d9c--;
}
/* 0x4e3210, the scene's tick counter 0x6f0d80: 0x6f0d88 tells whether it was past 0x6f0d84 */

int ai_Unk4e3210(void)
{
    ai_Unk6f0d88 = ai_Unk6f0d80 > ai_Unk6f0d84;
    ai_Unk6f0d80++;
}
/* 0x4e3230, 0x403390 (arguments, if any, are passed on untouched) unless the replay mode is on */

int ai_Unk4e3230(void)
{
    if (cm_SkipEvents != 0)
        return 0;
    return ch_Create();
}
/* 0x4e3250, the scene helpers 0x4e3250 .. 0x4e3640 pass their arguments on to the sound / object calls unless the replay mode (0x6f0d8c) is on */

int ai_Unk4e3250(int a)
{
    if (cm_SkipEvents != 0)
        return 1;
    return ch_Delete(a);
}
/* 0x4e3270 */

int ai_Unk4e3270(int a, int b)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_LoadVoiceBank(a, b);
}
/* 0x4e3290 */

int ai_Unk4e3290(int a)
{
    if (cm_SkipEvents != 0)
        return 0;
    return sb_UnloadSampleBank(a);
}
/* 0x4e32b0 */

int ai_Unk4e32b0(int a, int b, int c, int d)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_PlayVoice(a, b, c, d);
}
/* 0x4e32d0 */

int ai_Unk4e32d0(int a, int b, int c, int d, int e)
{
    if (cm_SkipEvents != 0)
        return 0;
    return snd_QueueVoice(a, b, c, d, e);
}
/* 0x4e32f0, sets up the scene's light / camera object 0x6f0db8 (0x45da30 -2.25, -0.2, 600.0) with the attachment 0x6f0db4 (0x45d880, 3.0, 0.8, -0.25;
   the colour 0 and a the tint) and fades it in (0x45d960, 0x45d9a0 255, 10.0) */

int ai_Unk4e32f0(int a)
{
    int v, id;

    if (cm_SkipEvents == 0) {
        id = d3sky_CreateK(0xc0100000, 0xbe4ccccd, 0x44160000);
        v = 0;
        ai_Unk6f0db8 = ai_Unk633dd4 = id;
        ai_Unk6f0db4 = d3sky_CreateLayerK(id, 0, a, (((char *)&v)[2] = 0, v), 0x40400000, 0, 0x3f4ccccd, 0xbe800000);
        d3sky_SetLayerSpeed(ai_Unk6f0db8, ai_Unk6f0db4, 0);
        d3sky_FadeLayer(ai_Unk6f0db8, ai_Unk6f0db4, 0xff, 0x41200000);
    }
}
/* 0x4e3390, releases the scene's sound / object slot 0x6f0db8 (0x45d800 first when 0x6f0db4 is set) */

int ai_Unk4e3390(void)
{
    if (cm_SkipEvents == 0) {
        if (ai_Unk6f0db4 != 0)
            d3sky_DeleteLayer(ai_Unk6f0db8, ai_Unk6f0db4);
        d3sky_Destroy(ai_Unk6f0db8);
        ai_Unk633dd4 = 0;
    }
}
/* 0x4e33d0, remembers the result of 0x413b30(a) in 0x6f0dbc */

int ai_Unk4e33d0(int a)
{
    if (cm_SkipEvents == 0)
        cm_SceneRain = d3t_CreateRain(a);
}
/* 0x4e33f0, frees the thing 0x4e33d0 remembered */

int ai_Unk4e33f0(void)
{
    if (cm_SkipEvents == 0) {
        d3t_DeleteRain(cm_SceneRain);
        cm_SceneRain = 0;
    }
}
/* 0x4e3420, runs the scene for a ticks (frame wait with the flag 0x8000): the end sets the stop flag 0x6f22a0 */

int ai_Unk4e3420(int a)
{
    int end;

    end = ai_Unk6f0d80;
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(ai_Unk4e3420, ai_Unk60d360 | 0x8000) != 0) {
            end += a;
            do {
                if (ai_Unk6f0d80 >= end)
                    sv_PopRequest = 1;
                ai_Unk4cee20();
                ai_Unk4e37a0(ai_Unk6f0d90);
            } while (sv_RunFrame(ai_Unk4e3420, ai_Unk60d360 | 0x8000) != 0);
        }
    }
}
/* 0x4e3490, like 0x4e3570 for a ticks, but a click (0x14673a0) ends it at once with cb(a) (1 without cb) as the result */

int ai_Unk4e3490(int a, int (*cb)(int))
{
    int start, res;
    double lim;

    start = d3_Time;
    res = 0;
    if (cm_SkipEvents != 0)
        return 0;
    if (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0) {
        do {
            if (dm_L.rup != 0) {
                if (cb != 0)
                    res = cb(a);
                else
                    res = 1;
                sv_PopRequest = 1;
            } else {
                lim = (double)a * (1.0 / 14) + start;
                if ((double)d3_Time >= lim)
                    sv_PopRequest = 1;
            }
            ai_Unk4cee20();
            ai_Unk4e37a0(ai_Unk6f0d90);
        } while (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0);
    }
    return res;
}
/* 0x4e3570, runs the scene for a ticks of 14 ms (d3_Time based): the end sets the stop flag 0x6f22a0 */

int ai_Unk4e3570(int a)
{
    int start;
    double lim;

    start = d3_Time;
    if (cm_SkipEvents == 0) {
        if (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0) {
            lim = (double)a * (1.0 / 14) + start;
            do {
                if ((double)d3_Time >= lim)
                    sv_PopRequest = 1;
                ai_Unk4cee20();
                ai_Unk4e37a0(ai_Unk6f0d90);
            } while (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0);
        }
    }
}
/* 0x4e3620 */

int ai_Unk4e3620(int a)
{
    if (cm_SkipEvents == 0) {
        snd_ClearQueue();
        return ((int (*)(char *))snd_PlaySpecialTrack)(a);
    }
}
/* 0x4e3640 */

int ai_Unk4e3640(void)
{
    if (cm_SkipEvents == 0)
        return ((int (*)())snd_EndSpecialTrack)();
}
/* 0x4e3650, sets the object tables 0x5fb5b0 up with the two 0xc0 byte callbacks 0x512de0 / 0x512e40 (0x438e60) after 0x4fe350, then 0x5128d0 */

void ai_Unk4e3650(void)
{
    gm_ClockSeason(&ai_Time);
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, init_PlantTextureSetHook);
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, init_TextureSetHook);
    init_SetSeasonTextures();
}
/* 0x4e36a0, resets the 16 member slots of 0x92cae0 for the party m (+0x34 the ids, +0x30 the count, +8 the kind): used, nothing announced, the id
   (-1 without a party); for a kind 3 party the 16 sub entries (+0x98.., 0x2c bytes) of its used slots are cleared. ROUGH */

int ai_Unk4e36a0(char *m)
{
    int j, i, q;
    char *s, *t;
    int *ids;

    ids = (int *)(m + 0x34);
    for (j = 0, s = ai_Unk92cae0; j < 16; j++, s += 0x35c, ids++) {
        *(int *)s = 1;
        *(int *)(s + 8) = 0;
        *(int *)(s + 4) = 0;
        *(int *)(s + 0xc) = 1;
        if (m != 0) {
            *(int *)(s + 0x10) = *ids;
            if (m[8] == 3 && (unsigned char)m[0x30] > 0) {
                for (i = 0, t = ai_Unk92cae0; i < (unsigned char)m[0x30]; i++, t += 0x35c) {
                    if (*(int *)(t + 8) == -1)
                        continue;
                    for (q = 0; q < 16; q++) {
                        *(int *)(t + 0x94 + q * 0x2c) = -1;
                        *(int *)(t + 0x98 + q * 0x2c) = 0;
                    }
                }
            }
        } else
            *(int *)(s + 0x10) = -1;
        s[0x14] = 0;
    }
}
/* 0x4e3750, 1 when no member slot of the party m is used (+0), alive (+8 == 0) and waiting (+0x10 != -1) */

int ai_Unk4e3750(char *m)
{
    int i, n, ok;
    char *s;

    ok = 1;
    i = 0;
    n = (unsigned char)m[0x30];
    if (n > 0) {
        s = ai_Unk92cae0;
        do {
            if (*(int *)s != 0 && *(int *)(s + 8) == 0 && *(int *)(s + 0x10) != -1)
                return 0;
            i++;
            s += 0x35c;
        } while (i < n);
    }
    return ok;
}
/* 0x4e37a0, the scene party m (+0x30 count): every member slot of 0x92cae0 that is used and not yet announced (+0xc) shows its text (+0x14, then 0x60c6c0) */

int ai_Unk4e37a0(char *m)
{
    int i;
    char *s;

    s = ai_Unk92cae0;
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

int ai_Unk4e3800(char *m, int n)
{
    int i;

    for (i = n - 1; i > 0; i--)
        if (*(int *)(m + 0x34 + i * 4) != -1)
            return i;
    return 0;
}
/* 0x4e3830, runs the scene party m (+0 / +0xc the owner, +8 the kind of the scene, +0x2c the window, +0x30 count, +0x34 the character ids) for
   the scene context ctx: the table 0x92c500 (5 ints per kind: +4 the callback of the kind 6 members, +8 the callback of the others, +0xc 1 when
   the members answer one after the other, 0 when all are asked at once) is walked over the member slots of 0x92cae0 (0x35c bytes each: +0
   the id, +8 the state, +0x14 the text). With 0x6f23f8 & 8 (the local run) every member's callback runs and its text is shown; with & 4 (the
   net run) the owner's machine asks the members (0x4c4480 / 0x4c4f50 send the answers, 0x4c3ae0 waits for them with the frame wait 0x52fe00,
   0x4e37a0 shows the texts, 0x4e3750 tells when nobody is left). The counter 0x6f0d7c grows by one per run. ROUGH: the net run is only sketched */

void ai_Unk4e3830(char *m, int ctx)
{
    char msg[0x7c];
    int saved, i, j, n, k, who, h;
    char *c, *slot;
    int *tab;
    int (*cb)();

    saved = ai_Unk4e52c0();
    ai_Unk4e36a0(m);
    if (ai_GameFlags & 4) {
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0] && cm_SkipEvents == 0) {
            memset(msg, 0, sizeof(msg));
            *(int *)msg = *(int *)m;
            sprintf(msg + 4, "cut_type:%i", (unsigned char)m[8]);
            cm_SendExSysMessage(0x11, msg);
        }
        if (cm_SkipEvents == 0)
            sv_Unk52fd80(ai_Unk1441488);
    }
    sv_SetStatusText(&ch_EmptyName);
    if (ai_GameFlags & 8) {
        n = *(unsigned char *)(m + 0x30);
        slot = ai_Unk92cae0[0];
        for (i = 0; i < n; i++, slot += 0x35c) {
            ai_Unk6f0d78 = i;
            who = *(int *)(m + 0x34 + i * 4);
            if (who == -1)
                continue;
            c = (char *)gm_FindChar(who);
            ai_Unk6f0d74 = c;
            if (c == 0) {
                *(int *)(m + 0x34 + i * 4) = -1;
                continue;
            }
            if (c[2] == 6) {
                ai_Unk6f0d84 = ai_Unk60d364 + ai_Unk6f0d80;
                ai_Unk6f0d88 = 0;
                ((int (*)())ai_Unk92c504[*(unsigned char *)(m + 8) * 5])(slot, ctx);
            } else {
                cb = (int (*)())ai_Unk92c508[*(unsigned char *)(m + 8) * 5];
                if (cb != 0)
                    cb(slot, ctx);
            }
            d2_SetForm(*(int *)(m + 0x2c), 0);
            io_PrintText(slot + 0x14);
            io_PrintText(&cm_TextNewline);
        }
    } else if (ai_GameFlags & 4) {
        ai_Unk6f0d74 = (char *)ai_CharInts[ai_PlayerId] - 4;
        k = ai_Unk92c50c[*(unsigned char *)(m + 8) * 20];
        if (k == 1) {
            if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
                /* the owner asks every member in turn (the slots of the humans are sent, the computer ones answered at once) */
                n = *(unsigned char *)(m + 0x30);
                slot = ai_Unk92cae0[0];
                for (i = 0; i < n; i++, slot += 0x35c) {
                    c = (char *)gm_FindChar(*(int *)slot);
                    if (c == 0 || c[2] == 6 || c[2] == 7 || *(int *)(slot + 8) != 0)
                        continue;
                    ai_Unk6f0d74 = c;
                    cb = (int (*)())ai_Unk92c508[*(unsigned char *)(m + 8) * 5];
                    if (cb != 0)
                        cb(slot, ctx);
                    ai_Unk4c4480(0x35c, slot);
                    ai_Unk4c4f50(*(int *)slot, 0);
                    h = cm_GetSlotState(*(int *)slot);
                    while (h == 0)
                        h = cm_GetSlotState(*(int *)slot) , sv_PumpNet();
                    ai_Unk4e37a0(m);
                    sv_RunFrame(ai_Unk60d360, 0);
                }
            } else {
                /* the others wait for the owner's slots and answer for their own character */
                n = *(unsigned char *)(m + 0x30);
                for (j = 0; j < n; j++)
                    if (*(int *)(ai_Unk92cae0[j]) == ai_CharInts[ai_PlayerId][0])
                        break;
                if (j < n) {
                    ai_Unk6f0d84 = ai_Unk60d364 + ai_Unk6f0d80;
                    ai_Unk6f0d88 = 0;
                    ai_Unk4c4480(0x35c, ai_Unk92cae0[j]);
                    ai_Unk4c4f50(ai_Unk6f0d74 != 0, 0);
                    while (cm_GetSlotState(0) == 0)
                        sv_PumpNet();
                }
                while (ai_Unk4e3750(m) == 0) {
                    ai_Unk4e37a0(m);
                    sv_SetStatusText(&ch_EmptyName);
                    sv_RunFrame(ai_Unk60d360, 0);
                }
            }
        } else if (k == 0) {
            /* all members answer at once: the owner waits for the slots (state +8 == 1), the others for their own */
            n = *(unsigned char *)(m + 0x30);
            ai_Unk4e37a0(m);
            sv_RunFrame(ai_Unk60d360, 0);
            while (ai_Unk4e3750(m) == 0) {
                ai_Unk4e37a0(m);
                sv_RunFrame(ai_Unk60d360, 0);
            }
        }
    }
    ai_Unk6f0d7c = ai_Unk6f0d7c + 1;
    ai_Unk4e52a0(saved);
}
/* 0x4e3fe0, switches the scene mode on: 0x42ea10 (0x5315c0, 1), then 0x404c20 (o, 1) for every object whose +0x8c bit 4 is not set */

int ai_Unk4e3fe0(void)
{
    char **pp;

    tb_SetFunctionOff(sv_UpdateClock, 1);
    for (pp = ai_Unk1468ae0; (int)pp < (int)(ai_Unk1468ae0 + 0x300); pp++) {
        if (*pp != 0 && !(*(unsigned char *)(*pp + 0x8c) & 4))
            ch_PauseAnim(*pp, 1);
    }
}
/* 0x4e4020, the reverse: the objects with the bit 4 get 0x404c20 (o, 0) */

int ai_Unk4e4020(void)
{
    char **pp;

    tb_SetFunctionOff(sv_UpdateClock, 0);
    for (pp = ai_Unk1468ae0; (int)pp < (int)(ai_Unk1468ae0 + 0x300); pp++) {
        if (*pp != 0 && (*(unsigned char *)(*pp + 0x8c) & 4))
            ch_PauseAnim(*pp, 0);
    }
}
/* 0x4e4060, runs a scene party m (the main function of the cutscene kinds: +0 id, +8 the kind, +0xc the owner, +0x26 flags (4 running, 0x20 own view,
   0x40 no fade), +0x2c the window, +0x78 the seed): saves the view state (0x5f1290, 0x631064, the viewport 0xfb9060 .., the sky / fog 0x75fb80 .., the objects of
   0xf8f080 that are shown), clears the scene table 0x92c600, decides the replay mode (0x6f0d8c: the run of the kind 6 members is only watched), fades out
   ("BLACK", 0x5a ticks), opens the window "cutscenes\\announcement" (or "..\\<own view>"), calls the run function of the kind (0x92c500, with m),
   closes everything again, fades in, and gives every member back its field +0x20c = -1 (0x4c3d90). ROUGH: the viewport arithmetic (0x43b950) and the
   decisions of the replay mode are only roughly in the order of the code */

void ai_Unk4e4060(char *m)
{
    char buf[0x240];
    float sv_fog;
    int sv_time, sv_view[4], sv_sky[6], sv_byte, shown, replay, i, j, n, win_old, id, ours;
    char *c;
    unsigned int fl;

    sv_fog = cm_SpeedFactor;
    sv_time = ai_Unk631064;
    sprintf(buf, "Exec Cutscene(Mainfunc start) <Type = %i> <id = %i>", (unsigned char)m[8], *(int *)m);
    shown = 0;
    sv_view[2] = ai_Unkfb9068;
    sv_view[0] = d2_DirtyRectLeft;
    sv_view[1] = ai_Unkfb9064;
    sv_view[3] = ai_Unkfb906c;
    ai_Unk925510 = d3_CurrentScene;
    ai_Unk92cac0 = evt_NoGroup;
    ai_Unk6f0d7c = 0;
    ai_Unk4cef80();
    AI_ZERO_AT(offset ai_Unk92c600, 0x480);  /* @vc6 clang's MS asm cannot take the offset of an incomplete array */
    cm_SkipEvents = 0;
    m[0x26] |= 4;
    if (sv_FrameMask & 0x200000)
        m[0x26] |= 0x40;
    ai_Unk60d360 = 0x717c6;
    if (m[0x26] & 0x40)
        ai_Unk60d360 = 0x716c6;
    replay = 0;
    if (ai_GameFlags & 8) {
        ours = 1;
        n = *(unsigned char *)(m + 0x30);
        for (i = 0; i < n; i++) {
            id = *(int *)(m + 0x34 + i * 4);
            if (id != -1 && (c = (char *)gm_FindChar(id)) != 0 && c[2] == 6) {
                ours = 0;
                break;
            }
        }
        for (j = 0; j < n; j++) {
            id = *(int *)(m + 0x34 + j * 4);
            if (id != -1 && (c = (char *)gm_FindChar(id)) != 0 && c[2] == 6)
                return;
        }
    }
    if (m[0x26] & 2) {
        if (ai_GameFlags & 8) {
            if (ours != 0)
                replay = 1;
        } else if ((ai_GameFlags & 4) && ai_Unk4e4870(ai_CharInts[ai_PlayerId][0], m) == 0)
            replay = 1;
    }
    if (replay != 0)
        cm_SkipEvents = 1;
    if (cm_SkipEvents == 0) {
        fl = (sv_FrameMask & ~8) | 0x10080;
        ai_Unk6f0da0 = (char *)d2_StartFade(0, 0, (short)d2_ScreenW, (short)d2_ScreenH, "BLACK", 0x5a, 1);
        while ((*ai_Unk6f0da0 & 4) == 0)
            sv_RunFrame(0, fl);
        sv_sky[0] = sv_ViewPosX;
        sv_sky[1] = sv_ViewPosY;
        sv_sky[2] = sv_ViewPosZ;
        sv_sky[3] = sv_ViewRotX;
        sv_sky[4] = sv_ViewRotY;
        sv_sky[5] = sv_ViewRotZ;
        if (ai_GameFlags & 8)
            ai_Unk4e3fe0();
        tb_OpenFunction(0x46, ai_Unk4e3210);
        sv_byte = d3_FogByDistance;
        d3_FogByDistance = 0;
        cm_SpeedFactor = 0.6f;
        d3o_FogScale = 1.0f;
        ai_Unk631064 = 0;
        d2_SetPointerSprite(0, 0);
        sv_ClearSelection();
        d2_ClearPointerItems();
        sv_ResetScreenClip();
        d2_CaptionCopy[0] = 0;
        for (i = 0, shown = 0; i < 0x8c400 / 0x2ec; i++) {
            if (*(int *)(d2_ObjsC + i * 0x2ec + 0x18) != 0 && *(int *)(d2_ObjsC + i * 0x2ec + 0x38) == 0) {
                ai_Unk92bee0[shown] = i;
                *(int *)(d2_ObjsC + i * 0x2ec + 0x38) = 1;
                shown++;
            }
        }
        ((int (*)())d3io_NoopCallback)();
        ai_Unk4d8b30();
    } else if (ai_Unk6f0d90 == 0)
        ai_Unk4e36a0(m);
    win_old = ai_Unk60d368;
    *(int *)(m + 0x2c) = d2_OpenForm(m[8] == 2 ? "cutscenes\\prozess_main" : (m[0x26] & 0x20) ? "cutscenes\\announcement_cinemascope" : "cutscenes\\announcement", 0, 0);
    ai_Unk60d368 = *(int *)(m + 0x2c);
    ((void (*)(int, int))d2_PlaceMainWindow)(ai_Unk60d368, 3);
    if (cm_SkipEvents != 0)
        d2_SetViewEnabled(ai_Unk60d368, 0);
    ai_Unk4e52a0(*(int *)(m + 0x78));
    sprintf(buf, "Exec Cutscene(Mainfunc) <Type = %i>", (unsigned char)m[8]);
    ai_Unk6f0d90 = (int *)m;
    ((void (*)())ai_Unk92c500[(unsigned char)m[8] * 5])(m);
    d2_CloseView(*(int *)(m + 0x2c));
    ai_Unk60d368 = win_old;
    if (cm_SkipEvents == 0) {
        ai_Unk4d8c40();
        ai_Unk631064 = sv_time;
        if (ai_GameFlags & 8)
            ai_Unk4e4020();
        ai_Unk60d364 = 10;
        tb_CloseFunction(ai_Unk4e3210);
        for (i = 0; i < shown; i++)
            *(int *)(d2_ObjsC + ai_Unk92bee0[i] * 0x2ec + 0x38) = 0;
        ai_Unk4e36a0(m);
    } else
        ai_Unk4e36a0(m);
    if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
        for (i = 0; i < 16; i++) {
            *(int *)(ai_Unk92cae0[i] + 4) = 0;
            *(int *)(ai_Unk92cae0[i]) = 0;
            *(int *)(ai_Unk92cae0[i] + 8) = 0;
            id = *(int *)(m + 0x34 + i * 4);
            if (id != -1 && (c = (char *)gm_FindChar(id)) != 0) {
                cm_BeginFieldBlock(c, *(int *)(c + 4));
                id = -1;
                cm_AddFieldValue(4, 1, (unsigned short)(c - cm_FieldObject + 0x20c), &id);
                cm_SendFieldValues();
            }
        }
    }
    id = *(int *)(m + 0xc);
    if (id != -1 && (c = (char *)gm_FindChar(id)) != 0) {
        id = -1;
        cm_BeginFieldBlock(c, *(int *)(c + 4));
        cm_AddFieldValue(4, 1, (unsigned short)(c - cm_FieldObject + 0x20c), &id);
        cm_SendFieldValues();
    }
    ai_Unk6f0d90 = 0;
    evt_NoGroup = ai_Unk92cac0;
}
/* 0x4e4870, 1 when the character id is one of the party m (+0x30 count, +0x34 ids) and still exists */

int ai_Unk4e4870(int id, char *m)
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
/* 0x4e48b0, sends a message of the type 0x11 (0x5ec) for the party m (a +4 the owner) from the object b (+0x14 the building, +0
   its id, +0x26 bit 4: a text follows): the text `text` (and the second one, joined, flag 0x10) after the header at +0xf8 of the buffer;
   +0x60 gets the kind of the building's thing (0x501ce0 / 0x4ffe90 +2), -1 without. ROUGH */

void ai_Unk4e48b0(int *a, char *b, char *text, char *extra)
{
    char buf[0x8f8];
    char *o, *p;
    int x, len;

    memset(buf, 0, 0xf8);
    buf[4] = 0x11;
    *(int *)(buf + 0xc) = -1;
    buf[0x36] = 9;
    *(int *)(buf + 8) = a[1];
    *(int *)(buf + 0x58) = 0x5ec;
    if (*(int *)(b + 0x14) != -1 && (b[0x26] & 0x10))
        buf[0x9c] = 0xe;
    else
        buf[0x9c] = 0xc;
    *(int *)(buf + 0x5c) = *(int *)(b + 0x14);
    *(int *)(buf + 0x64) = *(int *)b;
    x = -1;
    *(int *)(buf + 0x60) = -1;
    if (*(int *)(b + 0x14) != -1) {
        o = (char *)gm_FindBuildingById(*(int *)(b + 0x14));
        if (o != 0) {
            if (gm_IsHomeType(o) == 0) {
                p = (char *)gm_GetGebaeudeHauptraum(o);
                if (p != 0)
                    *(int *)(buf + 0x60) = *(int *)(p + 2);
            } else {
                p = (char *)gm_Query(*(int *)(o + 0x5d), 1, 0, 0xfd);
                if (p != 0)
                    *(int *)(buf + 0x60) = *(int *)(p + 2);
            }
        }
    }
    strcpy(buf + 0xf8, text);
    len = strlen(buf + 0xf8) + 1;
    if (extra != 0 && *extra != 0) {
        strcpy(buf + 0xf8 + len, extra);
        buf[0x9c] |= 0x10;
        ai_Unk4c4bc0(buf, len + strlen(extra) + 1, buf + 0xf8);
    } else
        ai_Unk4c4bc0(buf, strlen(text) + 1, text);
}
/* 0x4e4a50, "cut_CheckMaster": 1 when the owner (m +0xc) is one of the party m (+0x30 count, +0x34 ids); else the first member of the kind 6 / 7 that
   still exists (as long as none was found, 16 slots) becomes the new owner: the log "new master for id %i => %i", a request (0x4c61f0) and the wait for its
   answer (0x4c3ae0 with the frame wait 0x52fe00); 0 then. The log "invalid master for id %i" comes first. ROUGH */

int ai_Unk4e4a50(char *m)
{
    char buf[0x80];
    int flag, i, h;
    char *c;

    if (ai_Unk4e4870(*(int *)(m + 0xc), m) == 0) {
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
            h = ((int (*)())ai_Unk4c61f0)(m, c);
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
            return 0;
        }
    }
    return 1;
}
/* 0x4e4b50, "cut_CutsceneReady": the party m (+0 the id, +0x26 bit 7: the owner side, +0x30 count, +0x34 ids) is asked whether every member is ready: in the
   owner's run (bit 7 clear) a request (0x4c4fd0) goes to every member that does not exist / belongs to another master (kinds 6 / 7), the last one is waited
   for; otherwise the first such member ends the run with 0. A bad master (0x4e4a50) gives -1; else the message (id + the 0x10 ids) is sent
   (0x4c4540 / 0x4c5f20 / 0x4c4580) and the answer 2 of the wait makes 0, anything else 1. ROUGH */

int ai_Unk4e4b50(char *m)
{
    int buf[0x12];
    int i, h;
    int *p;
    char *c;

    memset(buf, 0, sizeof(buf));
    i = 0;
    if ((signed char)m[0x26] >= 0) {
        h = -1;
        p = (int *)(m + 0x34);
        for (i = 0; i < (unsigned char)m[0x30]; i++, p++) {
            if (*p == -1)
                continue;
            c = (char *)gm_FindChar(*p);
            if (c == 0 || (*(int *)(c + 0x20c) != -1 && *(int *)(c + 0x20c) != *(int *)m && (c[2] == 6 || c[2] == 7)))
                h = ((int (*)())ai_Unk4c4fd0)(*(int *)m, *p);
        }
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
    } else {
        p = (int *)(m + 0x34);
        for (i = 0; i < (unsigned char)m[0x30]; i++, p++) {
            if (*p == -1)
                continue;
            c = (char *)gm_FindChar(*p);
            if (c != 0 && *(int *)(c + 0x20c) != -1 && *(int *)(c + 0x20c) != *(int *)m && (c[2] == 6 || c[2] == 7))
                return 0;
        }
    }
    if (ai_Unk4e4a50(m) == 0)
        return -1;
    buf[0] = *(int *)m;
    memcpy(buf + 1, m + 0x34, 0x40);
    cm_BeginGroup("cut_CutsceneReady");
    h = ((int (*)())ai_Unk4c5f20)(buf);
    cm_EndGroup("cut_CutsceneReady");
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    return cm_GetSlotState(h) != 2;
}
/* 0x4e4cc0, the per-frame update of the scene parties (the records of 0x114 bytes from 0x925538: +0x18 used, +0xe flags (2: the owner answers, 5: idle, 0x80:
   a member run), +0x10 state, +4 the age in ticks; the party m itself starts 0x18 before the record: +0 the id, -0x14 done, -0x10 the kind, -0xc the owner):
   the timers (0x4fe140) are fed, the owner's own party asks "ready?" (0x4e4b50: -1 gives the timer 5, 0 the timers 0x1e / 9 (old ones cancel the party
   0x4c5ef0), else the callback of the kind (0x92c510) runs), a finished party is run (0x4e4060) and dropped (0x4e5180). ROUGH */

int cm_UpdateParties(void)
{
    char *r;
    G17T14 tmp;
    int x, k;

    for (r = (char *)ai_Unk925520 + 0x18; (unsigned int)r < ((unsigned)ai_Unk925520 + 0x6780); r += 0x114) {
        if (r[0x18] == 0 || (r[0xe] & 5))
            continue;
        tmp = *(G17T14 *)r;
        gm_AddTime(&tmp, 0, -60, 0);
        if (*(int *)(r - 0xc) == -1 && ((cm_NetFlags & 8) || (ai_GameFlags & 8)))
            *(int *)(r - 0xc) = ai_CharInts[ai_PlayerId][0];
        if (ai_CharInts[ai_PlayerId][0] == *(int *)(r - 0xc) && *(int *)(r + 0x10) == 0 && gm_CompareTime(ai_Time, (char *)&x) > 0) {
            k = ai_Unk4e4b50(r - 0x18);
            if (k == -1) {
                gm_AddTime(r, 0, 5, 0);
                if (*(unsigned short *)(r + 4) >= 0x16)
                    gm_AddTime(r, 9, 0, 0);
                continue;
            }
            if (k == 0) {
                gm_AddTime(r, 0, 0x1e, 0);
                if (*(unsigned short *)(r + 4) >= 0x16)
                    gm_AddTime(r, 9, 0, 0);
                continue;
            }
            if (ai_Unk92c510[*(unsigned char *)(r - 0x10) * 5] != 0 && ((int (*)())ai_Unk92c510[*(unsigned char *)(r - 0x10) * 5])(r - 0x18) == 0) {
                ai_Unk4c5ef0(*(int *)(r - 0x18));
                continue;
            }
        }
        if (ai_Unk92c510[*(unsigned char *)(r - 0x10) * 5] == 0 && gm_CompareTime(ai_Time, r) > 0)
            *(int *)(r - 0x14) = 1;
        if (gm_CompareTime(ai_Time, r) > 0 && *(int *)(r - 0x14) == 0) {
            if (ai_Unk4e4870(ai_CharInts[ai_PlayerId][0], r - 0x18) != 0 && (signed char)r[0xe] < 0) {
                if (ai_CharInts[ai_PlayerId][0x208 / 4] == *(int *)(r - 0x18))
                    *(int *)(r - 0x14) = 1;
            } else if (*(int *)(r - 0xc) == ai_CharInts[ai_PlayerId][0] && (r[0xe] & 2))
                *(int *)(r - 0x14) = 1;
            else
                ai_Unk4e5180(*(int *)(r - 0x18));
        }
        if (gm_CompareTime(ai_Time, (char *)&x) > 0 && *(int *)(r - 0x14) != 0) {
            sv_PanelFlag = 0;
            ai_Unk4e4060(r - 0x18);
            ai_Unk4e5180(*(int *)(r - 0x18));
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
        if (ai_Unk925520[i].used != 0 && ai_Unk925520[i].id != -1 && (ai_Unk925520[i].flags & 1) != 0 && best >= (unsigned int)ai_Unk925520[i].id)
            best = ai_Unk925520[i].id;
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
        if (ai_Unk4e4870(ai_CharInts[pl][0], (char *)p) != 0) {
            ai_CharInts[pl][0x208 / 4] = *(int *)p;
            ai_Unk4e4060(p);
            ai_CharInts[pl][0x208 / 4] = -1;
            ret = 1;
        }
        ai_Unk4e5180(*(int *)p);
        p = ai_Unk4e4f40();
    }
    cm_BeginFieldBlock((char *)ai_CharInts[pl] - 4, ai_CharInts[pl][0]);
    cm_AddFieldValue(4, 1, (unsigned short)(pl * 0x21c - (int)cm_FieldObject + (int)ai_Unk770d8c), &save);
    cm_SendFieldValues();
    return ret;
}
/* 0x4e5050, the used party of the kind a with the thing b: its record, 0 without */

int ai_Unk4e5050(int a, int b)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].used != 0 && ai_Unk925520[i].kind == a && ai_Unk925520[i].thing == b)
            return (int)&ai_Unk925520[i];
    }
    return 0;
}
/* 0x4e50a0, the used party with the id: its record, 0 without */

int cm_PartyById(int id)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].used != 0 && ai_Unk925520[i].id == id)
            return (int)&ai_Unk925520[i];
    }
    return 0;
}
/* 0x4e50e0, the member slot (0x92cae0, 0x35c bytes; the id at +0x10) with the id */

int ai_Unk4e50e0(int id)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == id)
            return (int)ai_Unk92cae0[i];
    }
    return 0;
}
/* 0x4e5120, the first free party record (id -1), 0 without */

int ai_Unk4e5120(void)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].id == -1)
            return (int)&ai_Unk925520[i];
    }
    return 0;
}
/* 0x4e5150, stores a copy of the party src (0x114 bytes) in a free record, with the next id 0x61a658; the byte +0x31 defaults to 8; the record */

int ai_Unk4e5150(int *src)
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

void ai_Unk4e5180(int id)
{
    G28REC *r;

    r = (G28REC *)cm_PartyById(id);
    if (r != 0) {
        memset(r, 0, 0x114);
        r->id = -1;
    }
}
/* 0x4e51b0, adds the character who to the party id (+0x30 count, +0x34 ids, at most 16) unless he is in it: 1 when added */

int ai_Unk4e51b0(int id, int who)
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

int ai_Unk4e5210(int id, int who)
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
    if (ai_Unk6f0d90 != 0 && *ai_Unk6f0d90 == id) {
        for (i = 0; i < 16; i++) {
            if (*(int *)(ai_Unk92cae0[i] + 0x10) == who) {
                *(int *)(ai_Unk92cae0[i] + 0x10) = -1;
                break;
            }
        }
    }
    return 1;
}
/* 0x4e52a0, sets the seed (logged "cut_randseed: %i") */

void ai_Unk4e52a0(unsigned int seed)
{
    char buf[0x40];

    ai_Unk92c4e0 = seed;
    sprintf(buf, "cut_randseed: %i", seed);
}
/* 0x4e52c0, the seed (logged "cut_getrandseed: %i") */

unsigned int ai_Unk4e52c0(void)
{
    char buf[0x40];

    sprintf(buf, "cut_getrandseed: %i", ai_Unk92c4e0);
    return ai_Unk92c4e0;
}
/* 0x4e52f0, a random number below n (logged "cut_rnd: %i"), 0 for n == 0 */

unsigned int ai_Unk4e52f0(unsigned int n)
{
    char buf[0x40];

    if (n == 0)
        return 0;
    ai_Unk92c4e0 = ai_Unk92c4e0 * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rnd: %i", G28_HI % 0x7fffu);
    return (G28_HI % 0x7fffu) % n;
}
/* 0x4e5360, a random float in [0, 1) (logged "cut_rndfloat: %f") */

double ai_Unk4e5360(void)
{
    char buf[0x40];

    ai_Unk92c4e0 = ai_Unk92c4e0 * 0x41c64e6d + 0x3039;
    sprintf(buf, "cut_rndfloat: %f", (float)(G28_HI % 0x7fffu) * 3.0518509e-5f);
    return (float)(G28_HI % 0x7fffu) * 3.0518509e-5f;
}
/* 0x4e53e0, the progress of the dialog dlg: with c set the clock 0x6333cc and the duration a are stored (and the window cleared, 0x475e40);
   else the left percent of the duration is the value; the slider (0x475cb0, 0 .. 100) is set to it, returned */

int ai_Unk4e53e0(int dlg, int a, int c)
{
    int v;

    v = 100;
    if (c != 0) {
        ai_Unk6f0dc0 = ai_ClockMs;
        ai_Unk60d374 = a;
        d2_SetObjEnabled(dlg, 0);
    } else {
        v = (int)((1.0f - (float)(unsigned int)(ai_ClockMs - ai_Unk6f0dc0) / ai_Unk60d374) * 100.0f);
        v = v > 0 ? v : 0;
    }
    d2_SetObjValue(dlg, 0, 100, v);
    return v;
}
/* 0x4e5470, a message box for the text id with the flags (2: "Messagebox_Autosize", 0x10: "_BIG", 0x20: "_VERY_BIG", 0x100: "_NONE_PERGA" (form 1), else "misc\\Messagebox";
   8: the frame wait flags 0x717c6, 0x80: a timer slider (0x500 ms), 0x40: ends on the first click of the dialog's own click, 4: ends after 500 ms): the dialog's
   objects are put into a radio group (all but the type 0x40), the loop runs the frame wait 0x5309a0 until the stop flag; the answer: the clicked
   button index + 1 (0xf8aad0 == id, the clicked object's result in 0xfa5aa8), else -1; not reentrant (0x6f0dc4). ROUGH */

int ai_Unk4e5470(int id, int flags)
{
    int t0, ret, win, radio, i, n, k, slider, fl, timed, quick;
    int *list;
    char *dlg;

    t0 = ai_ClockMs;
    ret = -1;
    win = 0;
    if (ai_Unk6f0dc4 != 0)
        return 0;
    fl = sv_FrameMask & 0xc7;
    if (flags & 2)
        win = d2_OpenForm("misc\\Messagebox_Autosize", 0, 0);
    else if (flags & 0x10)
        win = d2_OpenForm("misc\\Messagebox_BIG", 0, 0);
    else if (flags & 0x20)
        win = d2_OpenForm("misc\\Messagebox_VERY_BIG", 0, 0);
    else if (flags & 0x100) {
        win = d2_OpenForm("misc\\Messagebox_NONE_PERGA", 0, 0);
        d2_SetForm(win, 1);
    } else
        win = d2_OpenForm("misc\\Messagebox", 0, 0);
    if (flags & 8)
        fl = 0x717c6;
    io_PrintText(id);
    d2_CentreViewWindows(fl);
    ai_Unk6f0dc4 = 1;
    radio = d2_CreateRadioGroup(0);
    dlg = d2_LastWindowPtrC;
    i = 0;
    if (*(short *)(dlg + 0x1c) > 0) {
        do {
            k = (*(int **)(dlg + 0x18))[i];
            if (*(int *)(d2_ObjsC + k * 0x2ec + 0x18) != 0x40)
                d2_RadioAdd(radio, k);
            dlg = d2_LastWindowPtrC;
            i++;
        } while (i < *(short *)(dlg + 0x1c));
    }
    d2_RadioSelect(radio, 0);
    d2_SetForm(win, 0);
    timed = flags & 0x80;
    if (timed != 0)
        slider = d2_WinOpenSlider(0x1c, *(short *)(d2_LastWindowPtrC + 0xa) - 0x18, *(short *)(d2_LastWindowPtrC + 8) - 0x40, 0, 0x1f4, 0x83, 0x42, d2_LastWindow);
    else
        slider = -1;
    if (sv_RunFrame(fl, ai_Unk4e5470) != 0) {
        quick = flags & 0x40;
        n = flags & 4;
        do {
            d2_MenuNavigate(radio);
            if (quick != 0)
                d2_RaiseViewWindows(win);
            if (d2_ClickedId == 0) {
                if (d2_HitWindow == id) {
                    ret = d2_RadioSel[radio * 35] + 1;
                    sv_PopRequest = 1;
                } else if (kb_Last == 1) {
                    ret = 0;
                    sv_PopRequest = 1;
                }
            } else if (d2_ClickedId == 1 && d2_HitWindow == id || kb_Last == 1) {
                ret = 0;
                sv_PopRequest = 1;
            }
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            if (timed != 0 && slider != -1)
                d2_SetObjValue(slider, 0, 0x1f4, ai_ClockMs - t0);
            if (n != 0 && ai_ClockMs > t0 + 0x1f4)
                sv_PopRequest = 1;
        } while (sv_RunFrame(fl, ai_Unk4e5470) != 0);
    }
    ai_Unk6f0dc4 = 0;
    d2_RadioClear(radio);
    d2_CloseView(win);
    return ret;
}
/* 0x4e5730, registers the scene kind: the run function (+0), the callbacks of the members of the kind 6 (+4) and of the others (+8), the check (+0x10) and
   the flag +0xc (the members answer one after the other) */

void ai_Unk4e5730(int kind, int run, int cb4, int cb8, int check, char flag)
{
    ai_Unk92c500[kind * 5] = run;
    ai_Unk92c504[kind * 5] = cb4;
    ai_Unk92c508[kind * 5] = cb8;
    ai_Unk92c510[kind * 5] = check;
    ai_Unk92c50c[kind * 20] = flag;
}
/* 0x4e5770, the init of the scene code: the party records are cleared (id -1) and the scene kinds are registered: 0 the picture show (0x4e0010), 1 the council
   votes (0x4d8e20), 2 the trial (0x4db700), 3 the fight (0x4d63e0), 4 the duel, 5 the execution, 6 the wedding, 7 the birth, 8 the death, 9 the bankruptcy, 10
   the auction (0x4e1a80), 11 the sale (0x4e2860) */

void ai_Unk4e5770(void)
{
    int i;

    memset(ai_Unk925520, 0, 0x6780);
    for (i = 0; i < 96; i++)
        ai_Unk925520[i].id = -1;
    ai_Unk4e5730(0, (int)ai_Unk4e0010, (int)d3io_NoopCallback, 0, 0, 0);
    ai_Unk4e5730(1, (int)ai_Unk4d8e20, (int)ai_Unk4dada0, (int)ai_Unk4dae90, 0, 0);
    ai_Unk4e5730(2, (int)ai_Unk4db700, (int)ai_Unk4dd960, (int)ai_Unk4de340, (int)ai_Unk4daf80, 0);
    ai_Unk4e5730(3, (int)ai_Unk4d63e0, (int)ai_Unk4d8b10, 0, (int)ai_Unk4d60b0, 1);
    ai_Unk4e5730(4, (int)ai_Unk4ded10, (int)ai_Unk4dfff0, (int)ai_Unk4dff80, (int)ai_Unk4dfa20, 0);
    ai_Unk4e5730(5, (int)ai_Unk4e01e0, 0, 0, (int)ai_Unk4e0040, 0);
    ai_Unk4e5730(6, (int)ai_Unk4e0770, 0, 0, (int)ai_Unk4e0580, 0);
    ai_Unk4e5730(7, (int)ai_Unk4e0f70, 0, 0, (int)ai_Unk4e0e90, 0);
    ai_Unk4e5730(8, (int)ai_Unk4e1250, 0, 0, (int)ai_Unk4e1220, 0);
    ai_Unk4e5730(9, (int)ai_Unk4e1700, 0, 0, 0, 0);
    ai_Unk4e5730(10, (int)ai_Unk4e1a80, (int)ai_Unk4e24c0, (int)ai_Unk4e26d0, (int)ai_Unk4e19a0, 1);
    ai_Unk4e5730(11, (int)ai_Unk4e2860, 0, 0, 0, 0);
    ai_Unk4e36a0(0);
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
/* ai_Unk770b89: view of ai_Chars (tools/data_views.py) */
/* ai_Unk770cec: view of ai_Chars (tools/data_views.py) */
extern int ai_Evidence[0x800][2];
extern char ai_Crimes[0x5a00];
extern int ai_Unk925510;
extern int ai_Unk92bca0;
extern int ai_Unk92bca4;
extern int ai_Unk92bcc0[128];
extern int ai_Unk92bec0;
extern unsigned short ai_Unk92bee0[768];
extern unsigned int ai_Unk92c4e0;
extern int ai_Unk92c500[1];
extern int ai_Unk92c504[1];
extern int ai_Unk92c508[1];
extern char ai_Unk92c50c[4];
extern int ai_Unk92c510[60];
extern char ai_Unk92c600[1152];
extern int ai_Unk92ca80[16];
extern int ai_Unk92cac0;
extern char ai_Unk92cae0[16][0x35c];
extern float ai_Unk9300a0[0xb4];
extern char * ai_Unk930380[32];
extern char ai_Unk930400[192];
extern G18POP ai_Unk9304c0[16];
extern char ai_Unk930600[21376];
extern char * ai_Unk935980[32];
extern char ai_Unk935a00[512];
extern float ai_Unk935d00;
extern int ai_Unk935d20[32];
extern G18LOG ai_Unk935da0[15];
extern int ai_Unk9363f4;
extern int ai_Unk936400;
extern int ai_Unk936404;
extern int ai_Unk936408;
extern unsigned int ai_Unk936420[9];
extern CMNETGAME cm_NetGame;
extern char ai_Unk9364e0[752];
extern int ai_Unk9367d0;
extern int ai_Unk9367d4;
extern G17MSG cm_RecvPool[0x2000];
extern int cm_LastSyncCount;
extern int cm_NextNetStep;
extern unsigned char ai_Unka68800[1024];
extern G17MSG * cm_RecvFree;
extern unsigned int cm_GroupEndSeq;
extern int ai_Unka68c08;
extern G17FIELDS ai_Unka68c20;
extern G17FIELDS ai_Unka68ca0;
extern unsigned int ai_Unka68d1c;
extern int ai_Unka68d20;
extern G17MSG * cm_RecvQueue;
extern char * cm_FieldObject;
extern int ai_Unka68d30;
extern char * ai_Unka68d34;
extern unsigned char ai_Unka68d40[1024];
extern G17MSG cm_SendRing[0x8000];
extern char * cm_LoadBuf;
extern int cm_DayEndRequest;
extern G17STAT cm_SendStates[0x8000];
extern G17MSG * cm_SendQueue;
extern unsigned int ai_Unkf81164;
extern G17MSG cm_RecvMsg;
extern unsigned int cm_GroupFirstSeq;
extern char ai_Unkf895a0[0xb00];
extern char d2_CaptionCopy[0x80];
extern int ai_Unkfa5f00;
extern int ai_Unkfa5f04;
/* ai_Unkfd7c50: view of d3_Scenes (tools/data_views.py) */
/* ai_Unk143a610: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143a680: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143a9f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143ab0c: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b3d8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143b3e4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e674: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8a4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8a8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8ac: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8b0: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8b4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8bc: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8c0: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8e4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8f4: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8f8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e8fc: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e948: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e954: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk143e960: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1440c88: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk14413c8: view of txt_Ptrs (build/mk_txt_views.py) */
/* ai_Unk1441488: view of txt_Ptrs (build/mk_txt_views.py) */
extern char * ai_Unk1468ae0[0x300];
float ai_FloatOne = 1.0f;  /* 0x5eb780 */
float ai_Unk5eb788 = 0.0f;  /* 0x5eb788 */
/* 0x5eb798 / 0x5eb79c are the pooled floats 70.0f / 0.25f (0x428c0000 / 0x3e800000), not a
   double: a 8-byte definition here claims two literals another unit owns. */
/* 0x5eb7ac is the pooled float 0.5f (0x3f000000): use the literal at the call sites. */
double ai_Unk5eb7b0 = 15.0;  /* 0x5eb7b0 */
float ai_Unk5eb7dc = 3.14159274f;  /* 0x5eb7dc */
float ai_Unk5eb7fc = 75.0f;  /* 0x5eb7fc */
float ai_Unk5eb81c = 30.0f;  /* 0x5eb81c */
float ai_Unk5eb878 = 1.39999998f;  /* 0x5eb878 */
/* 0x5eb8b0 is the pooled float 0.2f (0x3e4ccccd): use the literal at the call sites. */
double ai_Unk5eb8b8 = 1.2;  /* 0x5eb8b8 */
double ai_Unk5eb8e0 = 20.0;  /* 0x5eb8e0 */
float ai_Unk5eb94c = 0.75f;  /* 0x5eb94c */
float ai_Unk5eb988 = 5.0f;  /* 0x5eb988 */
double ai_Unk5eb990 = 10.0;  /* 0x5eb990 */
double ai_Unk5eb9b0 = 0.25;  /* 0x5eb9b0 */
const double ai_Unk5eb9b8 = 0.2857142857142857;  /* 0x5eb9b8 */
const char ai_AxisPair[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128, 63, 0, 0, 0, 0, 0, 0, -128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* 0x5ebb74 */
const float ai_Unk5ebba4[] = { 0.0f, 0.0f, 0.0f, 0.0f };  /* 0x5ebba4, the origin (d3_Origin): read-only, the original keeps it in .rdata */
double ai_Unk5ebbe8 = 25.0;  /* 0x5ebbe8 */
float ai_Unk5ebc1c = 1.75f;  /* 0x5ebc1c */
double ai_Unk5ece60 = 0.01;  /* 0x5ece60 */
const float ai_Unk5ecebc = 1.25f;  /* 0x5ecebc */
double ai_Unk5edbd0 = 0.050000000000000003;  /* 0x5edbd0 */
const int ai_Unk5edc10[] = { 1155, 937, 770, 577, 385, 0 };  /* 0x5edc10 */
const double ai_Unk5edc28 = 1.8;  /* 0x5edc28 */
const float ai_Unk5edc50 = 525.0f;  /* 0x5edc50 */
const double ai_Unk5edc58 = 1.1000000000000001;  /* 0x5edc58, read-only: the original keeps it in .rdata */
const double ai_Unk5edc60 = 6.5012084524677184e-315;  /* 0x5edc60 */
const double ai_Unk5edc68 = 0.0055555555555555558;  /* 0x5edc68 */
int ai_Unk5f8438 = -1;  /* 0x5f8438 */
int ai_Unk5f8b10 = 6251111;  /* 0x5f8b10 */
float d2_ScreenScale = 1.0f;  /* 0x5fe708 */
int ai_Unk60bd80 = 770;  /* 0x60bd80 */
unsigned int cm_SpeedLevel = 2;  /* 0x60bd84, = 2 (0..4) */
int cm_LastCharId = -1;  /* 0x60bd88, = -1 */
int cm_LastBuildingId = -1;  /* 0x60bd8c, = -1 */
int cm_LastObjectId = -1;  /* 0x60bd90, = -1 */
int cm_LeftPlayerId = -1;  /* 0x60bd94, = -1 */
int (*ai_Unk60bd98[0x6a])(G17MSG *m, G17STAT *s) = { ai_Unk4c6a20, ai_Unk4c6a50, ai_AlwaysTrue, ai_Unk4c6a20, ai_Unk4c6a60, ai_Unk4c6a20, ai_Unk4c6a20, ai_Unk4c6a20, ai_Unk4c6a90, ai_Unk4c6ad0, ai_Unk4c6b20, ai_Unk4c6bf0, ai_Unk4c6cd0, ai_Unk4c6e10, ai_Unk4c6ec0, ai_Unk4c6f60, ai_Unk4c7010, ai_Unk4c70b0, ai_Unk4c78c0, ai_Unk4c7c30, ai_Unk4c7d80, ai_Unk4c7e20, ai_Unk4c7e90, ai_Unk4c7f70, ai_Unk4c8050, ai_Unk4c8110, ai_Unk4c81d0, ai_Unk4c8240, ai_Unk4c85c0, ai_Unk4c87a0, ai_Unk4c88a0, ai_Unk4c8990, ai_Unk4c89b0, ai_Unk4c8de0, ai_Unk4c8e70, ai_Unk4c9090, ai_Unk4c90d0, ai_Unk4c9110, ai_Unk4c9170, ai_Unk4c9280, ai_Unk4c9340, ai_Unk4c93b0, ai_Unk4c9400, ai_Unk4c9450, ai_Unk4c9520, ai_Unk4c9740, ai_Unk4c97c0, ai_Unk4c9830, ai_Unk4c99b0, ai_Unk4c9c10, ai_Unk4c9c80, ai_Unk4c9d00, ai_Unk4c9d80, ai_Unk4c9dc0, ai_Unk4ca8a0, ai_Unk4caa20, ai_Unk4caaa0, ai_Unk4cab40, ai_Unk4cac40, ai_Unk4cb100, ai_Unk4cb210, ai_Unk4cb250, ai_Unk4cb4a0, ai_Unk4cb6e0, ai_Unk4cb730, ai_Unk4cb7c0, ai_Unk4cb7f0, ai_Unk4cb850, ai_Unk4cb890, ai_Unk4cb8d0, ai_Unk4cb910, ai_Unk4cb940, ai_Unk4cb9b0, ai_Unk4cba10, ai_Unk4cbb90, ai_Unk4cbbd0, ai_Unk4cbca0, ai_Unk4cbff0, ai_Unk4cc050, ai_Unk4cc110, ai_Unk4cc1b0, ai_Unk4cc270, ai_Unk4cc330, ai_Unk4cc390, ai_Unk4cc510, ai_Unk4cc7b0, ai_Unk4cc8e0, ai_Unk4cc920, ai_Unk4cc980, ai_Unk4cca10, ai_Unk4ccaa0, ai_Unk4ccb10, ai_Unk4ccbd0, ai_Unk4ccc20, ai_Unk4ccce0, ai_Unk4cce00, ai_Unk4cce10, ai_Unk4cced0, ai_Unk4ccf10, ai_Unk4ccf90, ai_Unk4cd010, ai_Unk4cd130, ai_Unk4cd180, ai_Unk4cd1e0, ai_Unk4cd220, ai_Unk4cd2f0 };  /* 0x60bd98, the handler of each message type */
int (*ai_Unk60bf40[0x6a])(G17MSG *m) = { sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c6350, sv_ReturnZero, ai_Unk4c63e0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c65b0, sv_ReturnZero, sv_ReturnZero, ai_Unk4c66c0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c6750, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c6780, ai_Unk4c67a0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c67c0, ai_Unk4c6870, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, ai_Unk4c69c0, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero, sv_ReturnZero };  /* 0x60bf40, the check of each type (groups 5..6) */
char ai_Unk60c4a0[20] = "cm_ExChrWalkToDummy";  /* 0x60c4a0 */
char cm_TextNewline[4] = { 36, 65, 0, 0 };  /* 0x60c6c0 */
int ai_Unk60c7b0[2] = { 1061109567, 4144959 };  /* 0x60c7b0 */
char ai_Unk60c7c0[920] = { 84, 1, 0, 0, 120, 5, 20, 0, 1, 4, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0, 86, 1, 0, 0, 95, 10, 15, 0, 1, 4, 1, 0, 0, 0, -96, 66, 5, 107, 97, 109, 112, 102, 47, 115, 116, 114, 101, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 17, 0, 0, 0, 0, 0, 0, 88, 1, 0, 0, 80, 20, 40, 0, 1, 4, 1, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 104, 105, 101, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 17, 0, 0, 0, 0, 0, 0, 114, 1, 0, 0, 110, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 17, 0, 0, 0, 0, 0, 0, 110, 1, 0, 0, 100, 5, 10, 0, 1, 5, 0, 0, 0, 0, -96, 66, 10, 107, 97, 109, 112, 102, 47, 115, 116, 105, 99, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 17, 0, 0, 0, 0, 0, 0, 116, 1, 0, 0, 100, 0, 30, 1, 2, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 17, 0, 0, 2, 0, 0, 0, 118, 1, 0, 0, 90, 0, 30, 1, 1, 6, 0, 0, 0, 0, 72, 67, 2, 107, 97, 109, 112, 102, 47, 119, 101, 114, 102, 101, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 17, 0, 0, 2, 0, 0, 0, 94, 1, 0, 0, 60, 5, 20, 1, 1, 3, 0, 0, 0, 0, -56, 67, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 112, 105, 115, 116, 111, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 106, 17, 0, 0, 1, 0, 0, 0, 96, 1, 0, 0, 70, 10, 30, 1, 1, 3, 0, 0, 0, 0, 22, 68, 5, 107, 97, 109, 112, 102, 47, 115, 99, 104, 105, 101, 115, 115, 101, 110, 95, 97, 114, 109, 98, 114, 117, 115, 116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, 17, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, -106, 2, 8, 0, 1, 5, 1, 0, 0, 0, -96, 66, 2, 107, 97, 109, 112, 102, 47, 115, 99, 104, 108, 97, 103, 101, 110, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 17, 0, 0, 0, 0, 0, 0 };  /* 0x60c7c0, the ten 0x5c-byte records ai_FindWeaponRecord searches (G12REC10, hst.c); the last has id 0, the fallback */
float ai_Unk60cb58 = 1.0f;  /* 0x60cb58 */
int ai_Unk60cb5c = -1;  /* 0x60cb5c */
int ai_Unk60cb60 = -1;  /* 0x60cb60 */
int ai_Unk60cb64 = -1;  /* 0x60cb64 */
int ai_Unk60cb68 = -1;  /* 0x60cb68 */
int ai_Unk60d360 = 464838;  /* 0x60d360, the frame wait of the dialogs ( */
int ai_Unk60d364 = 10;  /* 0x60d364 */
int ai_Unk60d368 = -1;  /* 0x60d368 */
int ai_Unk60d36c = -1;  /* 0x60d36c, -1 or the object waited for ( */
int ai_Unk60d370 = 1;  /* 0x60d370 */
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
char sv_SkyHeavy[] = { 115, 107, 121, 95, 115, 99, 104, 119, 101, 114, 95, 48, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -51, -52, 76, 63, 0, 0, -128, -66, 115, 107, 121, 95, 115, 99, 104, 119, 101, 114, 95, 48, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -51, -52, 76, 63, 0, 0, -128, -66, 115, 107, 121, 95, 100, 117, 110, 107, 101, 108, 95, 48, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 64, -51, -52, 76, 63, 0, 0, -128, -66, 0, 0, 0, 0 };  /* 0x623ac0, 76-byte sky records: Sky_Schwer / Sky_Mittel / Sky_Schoen */
int ai_Unk624570 = 1;  /* 0x624570 */
int ai_Unk624574 = 1;  /* 0x624574 */
int ai_Unk624578 = 1;  /* 0x624578 */
char ai_Unk625820[115][0x20] = { "HE_NULL", "HE_HISTORIE", "HE_UPGRADE_DUMMY", "HE_ABBAU", "HE_PRODUKTION", "HE_PRODUKTION_FERTIG_MSG", "HE_ARBEITER_EINSTELLEN", "HE_MEISTER_EINSTELLEN", "HE_VORKOMMEN_SUCHEN", "HE_NEUE_VORKOMMEN_SUCHEN", "HE_NEUE_VORKOMMEN_SUCHEN_MSG", "HE_AI_WATCH_TRANSPORT", "HE_OBJEKT_BAUEN", "HE_GEBAEUDE_BAUEN", "HE_MAUER_BAUEN", "HE_TRANSPORT", "HE_REPARATUR", "HE_MESSAGEBOX", "HE_LAGERSLOTS_BAUEN", "HE_LAGER_ERWEITERN", "HE_KRAEUTER_SAMMELN", "HE_GETIER_FANGEN", "HE_SPIELER_TRAINIEREN", "HE_GEHEIM_TREFFEN", "HE_SPIONAGE", "HE_SABOTAGE", "HE_PRUEGEL", "HE_GEB_VERKAUFEN", "HE_GEB_RENOVIEREN", "HE_GEB_VERMIETEN_DUMMY", "HE_GEB_UPGRADEN", "HE_GEB_RAUM_UPGRADEN", "HE_GEB_ABREISSEN", "HE_GEB_KAUFANGEBOT", "HE_SITZUNG", "HE_KREDIT", "HE_MEISTER_PRUEFUNG", "HE_MEISTER_PRUEFUNG_MSG", "HE_MEISTER_BESTANDEN", "HE_GELDBOTE", "HE_ERNTEN", "HE_STAMMTISCH_MSG", "HE_STAMMTISCH", "HE_GOTTESDIENST", "HE_PROZESS", "HE_DROHEN_MSG", "HE_DROHEN", "HE_ANSCHWAERZEN_MSG", "HE_BUCH_SCHREIBEN", "HE_ENTLASSEN", "HE_BEFOERDERN", "HE_PERSONAL_KUENDIGEN", "HE_PERSONAL_STEHLEN", "HE_PAMPHLET", "HE_FEST_MSG", "HE_FEST", "HE_GELAGE", "HE_KERKER_MEISTER_BESTECHEN", "HE_SIM_PERSONAL", "HE_MARKT_SUPERVISOR", "HE_ENTFUEHREN", "HE_SPIELER_ENTFUEHRT", "HE_LOESEGELD", "HE_EINBRUCH", "HE_GEB_SPIONAGE", "HE_WERBUNG", "HE_RANDOM_EVENT", "HE_STREIFE", "HE_RAZZIA", "HE_ERPRESSUNG", "HE_BELEIDIGUNG_MSG", "HE_BELEIDIGUNG", "HE_UEBERFALL", "HE_ANGRIFF", "HE_RANDOM_GESELLE", "HE_FURCHT_EINFLOESSEN", "HE_MISSION_SUPERVISOR", "HE_MISSION_DONE_MSG", "HE_EVT_GROSS_BRAND", "HE_EVT_BRAND", "HE_EVT_WIRBELSTURM", "HE_EVT_RAUBRITTER", "HE_EVT_KOMMENTARE", "HE_EVT_STADTFEST", "HE_EVT_SPENDE", "HE_EVT_BETEILIGUNG_MSG", "HE_EVT_BETEILIGUNG", "HE_EVT_SOELDNER_PLUENDERN", "HE_EVT_SOELDNER_MARODIEREN", "HE_EVT_PEST", "HE_ESKORTE_VERHINDERN", "HE_GELDSTRAFE", "HE_PRANGER", "HE_NACHWUCHS", "HE_BERUFSAUSBILDUNG", "HE_UNI", "HE_TALENT_SPIELER", "HE_TASCHENDIEB", "HE_SCHUTZGELD", "HE_SCHUTZGELD_MSG", "HE_BETRIEBS_PRUEFUNG", "HE_UMLAND_PATROL", "HE_SIM_UNFALL", "HE_PACHT", "HE_SIM_WIRTSHAUS", "HE_TITEL_VERGABE", "HE_AMT_VERGABE", "HE_BARDE", "HE_HEILEN", "HE_SIM_EINWANDERUNG", "HE_INIT_BETRIEB", "HE_GELIEBTE", "HE_SIM_KRANKHEITEN", "HE_DIE_PLAYER", "HE_KILL_PLAYER" };  /* 0x625820, names ("HE_NULL", ..) */
int ai_Unk631064 = 0;  /* 0x631064 */
int ai_Unk631084 = 0;  /* 0x631084 */
char * ai_Unk6332b4 = 0;  /* 0x6332b4 */
int ai_ClockMs = 0;  /* 0x6333cc, a clock */
unsigned int ai_Unk63349c = 0;  /* 0x63349c */
int ai_Unk6f0cd0 = 0;  /* 0x6f0cd0, received messages */
int cm_NoticeCount = 0;  /* 0x6f0cd4 */
int ai_Unk6f0cd8 = 0;  /* 0x6f0cd8 */
int ai_Unk6f0cdc = 0;  /* 0x6f0cdc */
char * ai_Unk6f0ce0 = 0;  /* 0x6f0ce0 */
int ai_Unk6f0ce4 = 0;  /* 0x6f0ce4, fight paused */
int ai_Unk6f0ce8 = 0;  /* 0x6f0ce8 */
int ai_Unk6f0cec = 0;  /* 0x6f0cec */
char * ai_Unk6f0cf0 = 0;  /* 0x6f0cf0 */
int ai_Unk6f0cf4 = 0;  /* 0x6f0cf4 */
int ai_Unk6f0cf8 = 0;  /* 0x6f0cf8 */
int ai_Unk6f0cfc = 0;  /* 0x6f0cfc */
char * ai_Unk6f0d04 = 0;  /* 0x6f0d04 */
int ai_Unk6f0d08 = 0;  /* 0x6f0d08 */
int ai_Unk6f0d0c = 0;  /* 0x6f0d0c */
int ai_Unk6f0d10 = 0;  /* 0x6f0d10 */
int ai_Unk6f0d14 = 0;  /* 0x6f0d14 */
int ai_Unk6f0d18 = 0;  /* 0x6f0d18 */
int ai_Unk6f0d1c = 0;  /* 0x6f0d1c */
int ai_Unk6f0d20 = 0;  /* 0x6f0d20 */
int ai_Unk6f0d24 = 0;  /* 0x6f0d24 */
int ai_Unk6f0d28 = 0;  /* 0x6f0d28 */
int ai_Unk6f0d2c = 0;  /* 0x6f0d2c */
int ai_Unk6f0d30 = 0;  /* 0x6f0d30 */
int ai_Unk6f0d34 = 0;  /* 0x6f0d34 */
int ai_Unk6f0d38 = 0;  /* 0x6f0d38 */
int ai_Unk6f0d3c = 0;  /* 0x6f0d3c */
int ai_Unk6f0d40 = 0;  /* 0x6f0d40 */
int ai_Unk6f0d44 = 0;  /* 0x6f0d44 */
int ai_Unk6f0d48 = 0;  /* 0x6f0d48 */
int ai_Unk6f0d4c = 0;  /* 0x6f0d4c */
int ai_Unk6f0d50 = 0;  /* 0x6f0d50 */
int ai_Unk6f0d54 = 0;  /* 0x6f0d54 */
int ai_Unk6f0d58 = 0;  /* 0x6f0d58 */
int ai_Unk6f0d5c = 0;  /* 0x6f0d5c */
int ai_Unk6f0d60 = 0;  /* 0x6f0d60 */
int ai_Unk6f0d64 = 0;  /* 0x6f0d64 */
int ai_Unk6f0d68 = 0;  /* 0x6f0d68 */
int ai_Unk6f0d6c = 0;  /* 0x6f0d6c */
int ai_Unk6f0d70 = 0;  /* 0x6f0d70 */
char * ai_Unk6f0d74 = 0;  /* 0x6f0d74 */
int ai_Unk6f0d78 = 0;  /* 0x6f0d78 */
int ai_Unk6f0d7c = 0;  /* 0x6f0d7c */
int ai_Unk6f0d80 = 0;  /* 0x6f0d80 */
int ai_Unk6f0d84 = 0;  /* 0x6f0d84 */
int ai_Unk6f0d88 = 0;  /* 0x6f0d88 */
int cm_SkipEvents = 0;  /* 0x6f0d8c */
int * ai_Unk6f0d90 = 0;  /* 0x6f0d90 */
int ai_Unk6f0d94 = 0;  /* 0x6f0d94 */
unsigned int ai_Unk6f0d98 = 0;  /* 0x6f0d98 */
int ai_Unk6f0d9c = 0;  /* 0x6f0d9c, the scene nesting depth */
char * ai_Unk6f0da0 = 0;  /* 0x6f0da0, the fade */
unsigned int cm_FightStart = 0;  /* 0x6f0da4, fight start (d3_Time) */
unsigned char ai_Unk6f0da8[2] = {0};  /* 0x6f0da8, +2 / +3: the left / right insult won, +4 / +5 the shot won */
unsigned char ai_Unk6f0daa = 0;  /* 0x6f0daa */
int ai_Unk6f0db0 = 0;  /* 0x6f0db0 */
int ai_Unk6f0db4 = 0;  /* 0x6f0db4 */
int ai_Unk6f0db8 = 0;  /* 0x6f0db8 */
int cm_SceneRain = 0;  /* 0x6f0dbc */
int ai_Unk6f0dc0 = 0;  /* 0x6f0dc0 */
char ai_Unk6f0dc4 = 0;  /* 0x6f0dc4 */
char * ai_PickedObject = 0;  /* 0x6f22e8 */
int ai_PickedChar = 0;  /* 0x6f22f8 */
int ai_Unk6f2438 = 0;  /* 0x6f2438 */
int ai_Unk6f244c = 0;  /* 0x6f244c */
int cm_NetFlags = 0;  /* 0x6f2594, flags */
int cm_RequestedSaveSlot = 0;  /* 0x6f25c8 */
char cm_RequestedSaveName[72] = {0};  /* 0x6f25d0 */
/* ai_Unk6f35ca: view of ai_GfxFloor (tools/data_views.py) */
unsigned int ai_MissionSeed = 0;  /* 0x75e120, the random seed of the missions (as unk_4b39f0.h) */
int ai_TimeCopy[4] = {0};  /* 0x75e130, the clock copy 0x75e130..0x75e13e (14 bytes, written whole by cm.c / sv.c): [0] the year; ai_Unk75e134 / 75e138 / 75e13c are views */
char * sv_SelectedObject = 0;  /* 0x75ec60 */
char cm_ChatLog[2048] = {0};  /* 0x75fba0 */
int cm_SaveAcks[8][2] = {0};  /* 0x76fa20 */
/* ai_Unk770d8c: view of ai_Chars (tools/data_views.py) */
int ai_Evidence[0x800][2] = {0}; /* 0x91b6e0 */
char ai_Crimes[0x5a00] = {0}; /* 0x91f700, the crime records (0x2d bytes each) up to 0x925100; f3gm's goods
                                    table ai_Unk91f716 is the same storage, a view at +0x16 */
int ai_Unk925510 = 0;  /* 0x925510 */
G28REC ai_Unk925520[96] = {0};  /* 0x925520 */
int ai_Unk92bca0 = 0;  /* 0x92bca0 */
int ai_Unk92bca4 = 0;  /* 0x92bca4 */
int ai_Unk92bcc0[128] = {0};  /* 0x92bcc0 */
int ai_Unk92bec0 = 0;  /* 0x92bec0 */
unsigned short ai_Unk92bee0[768] = {0};  /* 0x92bee0 */
unsigned int ai_Unk92c4e0 = 0;  /* 0x92c4e0, the random seed of the scenes */
int ai_Unk92c500[1] = {0};  /* 0x92c500, the scene kind table: run, callbacks, flag, check */
int ai_Unk92c504[1] = {0};  /* 0x92c504, the scene kind table (5 ints per kind) */
int ai_Unk92c508[1] = {0};  /* 0x92c508 */
char ai_Unk92c50c[4] = {0};  /* 0x92c50c */
int ai_Unk92c510[60] = {0};  /* 0x92c510, the answer callbacks of the kind table (5 ints per kind) */
char ai_Unk92c600[1152] = {0};  /* 0x92c600 */
int ai_Unk92ca80[16] = {0};  /* 0x92ca80 */
int ai_Unk92cac0 = 0;  /* 0x92cac0 */
char ai_Unk92cae0[16][0x35c] = {0};  /* 0x92cae0 */
float ai_Unk9300a0[0xb4] = {0};  /* 0x9300a0 */
char * ai_Unk930380[32] = {0};  /* 0x930380 */
char ai_Unk930400[192] = {0};  /* 0x930400 */
G18POP ai_Unk9304c0[16] = {0};  /* 0x9304c0 */
char ai_Unk930600[21376] = {0};  /* 0x930600 */
char * ai_Unk935980[32] = {0};  /* 0x935980 */
char ai_Unk935a00[512] = {0};  /* 0x935a00 */
float ai_Unk935d00 = 0;  /* 0x935d00 */
int ai_Unk935d20[32] = {0};  /* 0x935d20 */
G18LOG ai_Unk935da0[15] = {0};  /* 0x935da0 */
int ai_Unk9363f4 = 0;  /* 0x9363f4 */
int ai_Unk936400 = 0;  /* 0x936400 */
int ai_Unk936404 = 0;  /* 0x936404 */
int ai_Unk936408 = 0;  /* 0x936408 */
unsigned int ai_Unk936420[9] = {0};  /* 0x936420, the count then the eight player states (0x24 bytes) */
CMNETGAME cm_NetGame = {0};  /* 0x936460, the network game (hdr.players 0x936496, mode 0x9364dc) */
char ai_Unk9364e0[752] = {0};  /* 0x9364e0 */
int ai_Unk9367d0 = 0;  /* 0x9367d0 */
int ai_Unk9367d4 = 0;  /* 0x9367d4 */
G17MSG cm_RecvPool[0x2000] = {0};  /* 0x9367e0, the free receive records */
int cm_LastSyncCount = 0;  /* 0xa687e0, "cm_LastSyncCount" (net.h) */
int cm_NextNetStep = 0;  /* 0xa687e4, d3_Time of the start */
unsigned char ai_Unka68800[1024] = {0};  /* 0xa68800, a long message to split (ai_Unka68d30 bytes) */
G17MSG * cm_RecvFree = 0;  /* 0xa68c00, the free list */
unsigned int cm_GroupEndSeq = 0;  /* 0xa68c04 */
int ai_Unka68c08 = 0;  /* 0xa68c08, bytes used in ai_Unka68c20.buf */
G17FIELDS ai_Unka68c20 = {0};  /* 0xa68c20 */
G17FIELDS ai_Unka68ca0 = {0};  /* 0xa68ca0 */
unsigned int ai_Unka68d1c = 0;  /* 0xa68d1c, "cm_LastRequCount" */
int ai_Unka68d20 = 0;  /* 0xa68d20, bytes used in ai_Unka68ca0.buf */
G17MSG * cm_RecvQueue = 0;  /* 0xa68d24, the received messages */
char * cm_FieldObject = 0;  /* 0xa68d2c, the object of the field block (fields are offsets into it) */
int ai_Unka68d30 = 0;  /* 0xa68d30, length of ai_Unka68800 */
char * ai_Unka68d34 = 0;  /* 0xa68d34 */
unsigned char ai_Unka68d40[1024] = {0};  /* 0xa68d40, a joined long message */
G17MSG cm_SendRing[0x8000] = {0};  /* 0xa69140, the send ring */
char * cm_LoadBuf = 0;  /* 0xf31140, the load buffer ("cm_LoadBuf") */
int cm_DayEndRequest = 0;  /* 0xf31144 */
G17STAT cm_SendStates[0x8000] = {0};  /* 0xf31160 */
G17MSG * cm_SendQueue = 0;  /* 0xf81160, the messages to send */
unsigned int ai_Unkf81164 = 0;  /* 0xf81164, length of ai_Unka68d40 */
G17MSG cm_RecvMsg = {0};  /* 0xf81180, the message being received */
unsigned int cm_GroupFirstSeq = 0;  /* 0xf8121c */
char ai_Unkf895a0[0xb00] = {0}; /* 0xf895a0 */
char d2_CaptionCopy[0x80] = {0}; /* 0xf8aae0, the d2 caption / status text buffer (gfx.h's d2_CaptionCopy, 0x80 bytes up to d2_InputDragValue at 0xf8ab60) */
/* ai_Unkf8f39c: view of d2_Wins (tools/data_views.py) */
int ai_Unkfa5f00 = {0}; /* 0xfa5f00 */
int ai_Unkfa5f04 = {0}; /* 0xfa5f04 */
/* ai_Unk144a160: view of txt_Ptrs (build/mk_txt_views.py) */
char * ai_Unk1468ae0[0x300] = {0}; /* 0x1468ae0, the objects */
