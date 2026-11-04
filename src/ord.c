/* ord.c -- original file name unknown (ord is a guess). Order handlers of the characters (the table 0x611768:
   one handler per order, 0x4f4e20 .. 0x4f8140), the dispatcher 0x4f81c0, the privilege dialog 0x4f8200 and the
   grid statistics that follow (0x4f82a0 ..). Split from gv.c: the table sits right after gv's strings, and
   0x4f5d50 matches only when 0x4f2870 is not defined above it in the same file.
   Open: no ord.h yet; the unit's declarations and views live in gv.h, shared with gv.c.
   Open: gv.h's ORDBLD duplicates ai.h's AIBLD.
   unit of rea 8cef7d5b (src/game/ord.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gv.h"
#include "di_input.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define d3sm_GetVertexI ((int (*)())d3sm_GetVertex)  /* 0x45f100 (d3_sm.c: char, D3SM *, int, int, D3VECTOR *): @original this unit's callers read an int result (byte-matched ord_RandomCellPoint (ord.c): no 0f be c0 after the call) */
#include "d3_types.h"
#include "ahm.h"
#include "ai.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#include "he.h"
#include "io_text.h"
#include "sv.h"
#include "tr.h"
#include "unk_58bbd0.h"
#include "ch.h"
#include "d2.h"
#include "d3_sm.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */

#define ORD_FLAG_BLOCKED 0x400000  /* the privilege flag that blocks the misappropriation order (0x4f6b40) */

/* ai_RatingBase as the int out[6] of the statistics record: the original moves the float's bits with a dword mov
   (0x4f9e22), so the caller's record takes them as they are */
#define ORD_RATINGBASE_BITS (*(int *)&ai_RatingBase)  /* 0x6f1b9c */

/* the clock's hour as am.h / cm.h / amt.h declare it: a view of gv.h's ai_Time (0x770ae0 + 4), so it is not
   an object of its own in the data area */

/* the engine's generic unaligned block copy (D3_COPY in d3.h, which needs the DirectX headers): size and
   destination through temps, the source named directly in the asm (0x4f9190 copies the record 0x91b480) */
#define ORD_CAT(a, b) a##b
#define ORD_LABEL(name, n) ORD_CAT(name, n)
#define ORD_COPY(dst, src, size) {                                  \
    int _cn = (size); void *_cd = (dst);                            \
    __asm mov edi, _cd                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ORD_LABEL(ord_copy_tail, __LINE__)                    \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm ORD_LABEL(ord_copy_tail, __LINE__): add ecx, ebx          \
    __asm rep movsb                                                 \
}
/* both operands through temps (0x4f9220 copies the local record into the message out) */
#define ORD_COPY3(dst, src, size) {                                 \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);         \
    __asm mov edi, _cd                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ORD_LABEL(ord_copy3_tail, __LINE__)                   \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm ORD_LABEL(ord_copy3_tail, __LINE__): add ecx, ebx         \
    __asm rep movsb                                                 \
}
/* both operands named directly in the asm (0x4f9710 copies the message into the record 0x91b480) */
#define ORD_COPY_AT(dvar, svar, size) {                             \
    int _cn = (size);                                               \
    __asm mov edi, dvar                                             \
    __asm mov esi, svar                                             \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle ORD_LABEL(ord_copyat_tail, __LINE__)                  \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm ORD_LABEL(ord_copyat_tail, __LINE__): add ecx, ebx        \
    __asm rep movsb                                                 \
}

/* the statistics record views of 0x4f9020 (unit-private: gv.h's ORDCHAR is the shared character view) */
typedef struct ORDSAVE {                /* the saved record of 0x4f9020 (15 words) */
    float col[5];                       /* 0x00, ORDSTATS */
    float rating;                       /* 0x14 */
    float base;                         /* 0x18 */
    float v1c, v20, score;              /* 0x1c .. 0x24 */
    float rate;                         /* 0x28, ord_RatingRate */
    AITIME clock;                       /* 0x2c, the clock stamp (ord_StatisticsClock) */
} ORDSAVE;
typedef struct ORDCHARKIND {            /* the character record seen from its kind byte */
    unsigned char kind;                 /* 0x00 (ORDCHAR + 0x02) */
    char pad01[5];
    unsigned char health;               /* 0x06 (ORDCHAR + 0x08) */
    char pad07[4];
    unsigned char level;                /* 0x0b (ORDCHAR + 0x0d) */
    char pad0c[0x72];
    unsigned char stats[5];             /* 0x7e (ORDCHAR + 0x80), the five skill bytes 0x4f9020 averages */
    char pad83[0x21c - 0x83];
} ORDCHARKIND;
#define ORD_CHARKINDS ((ORDCHARKIND *)((char *)ai_Chars + 2))
unsigned char ai_OwnPlayer = 0;  /* 0x6f1bc5: defined here because ord.c does not read it (the ai data area) */
AITIME ord_StatisticsClock = {0}; /* 0x91b470, the clock of the last rating step (ord_AdvanceRatingBase) */
ORDSTATS ord_Statistics = {0}; /* 0x91b4c0, the last statistics (ord_UpdateStatistics) */

/* 0x4f4e20, the order "apply for office" (table 0x611768 entry 0; c the character, m the message: m[0] the
   kind 1 = do / 3 = ask, the text at m + 5): only when c has no partner (partner = -1) or the partner record
   (0x505df0) is gone / not in use (health 0) or of kind 0xf; the ask runs the office application 0x5c6d00
   (its result | 2), the do of a player (kind 6) looks for the player's first office event (0x53e400 (1, 0,
   0x41)) and writes the text 0x1ce2 with the event's target id, else the text 0x1ce1, into m + 5; returns 1
   (not handled) or 2
   @deep meaning settled, only allocation/layout left: the original's m = ebx, c = edi, r = esi (ours m = edi, c = ebx, r = esi), its return-1 block sits at the end of the function (ours inline before the body), and its loop return is mov eax,esi where ours constant-folds mov eax,2. Read vs tools/disasm.py 0x4f4e20. Tried and kept (no WORSE, 240 -> 191 differing bytes): r = 2 + if/else-if with a shared return r (the 0x4f4ff0 idiom), typed ORDCHAR/ORDEVENT views (gv.h). Tried and reverted (build shows the same m = edi allocation): spell.py menu push-pop/dup-tail/reload leads; early-return guard if (!(...)) return 1; explicit else return 1; goto fail: label at the end; int partner = c->partner temp; char kind = c->kind temp; partner + kind temps; declaration-order permutations of o/p/r; unsigned r; unsigned char *o; int *m copy (int *msg = m, copy-propagated back). Needs match-deep's register-allocation toolbox (spell 'push-pop: the block the callee-saved register crosses' / vargen ren). */

unsigned ord_ApplyForOffice(ORDCHAR *c, int *m)
{
    ORDEVENT *ev;
    ORDCHAR *ch;
    unsigned short *p;
    int r;

    if ((m[0] == 3 || m[0] == 1)
        && (c->partner == -1
            || ((ch = (ORDCHAR *)gm_FindChar(c->partner)) != 0 && (ch->health == 0 || ch->kind == 0x0f)))) {
        r = 2;
        if (m[0] == 3) {
            r = ai_OfficeApplication((short *)c, (int)m);
            r |= 2;
        } else if (m[0] == 1 && c->kind == 6) {
            ev = (ORDEVENT *)he_FindEvent(1, 0, 0x41);
            while (ev != 0) {
                if (ev->ownerId == ORD_CHARS[ai_PlayerId].netId) {
                    p = (unsigned short *)gm_FindChar(ev->targetId);
                    txt_Format(m + 5, 0x1ce2, *p);
                    return r;
                }
                ev = (ORDEVENT *)he_NextEvent();
            }
            txt_Format(m + 5, "%s", 0x1ce1);
        }
        return r;
    }
    return 1;
}
/* 0x4f4f20, an order handler (c the character, m the message, kind 1 or 3): characters of level 3 or more: 4; a player (+2 = 6) with kind 3 gets the text 0x1d34 (level + 0x114, + 10 with +9) shown through 0x4f8200; kind 1 writes text 0x1d33 into m + 0x14; 1 for other kinds, else 2 @deep meaning settled and readable; remaining diff is allocation/scheduling only (97/208 bytes, 38 code bytes): the original keeps m in esi and r in edi (ours m = edi, r = esi, the same rank order), its r = 4 return block sits at 0x4f4fad between the kind-3 text block and the kind-1 text block (ours puts it after the if/else), and the r = 2 setup order differs. Won by the last-bytes pass: the complete-constant ternary (c[9] != 0 ? 0x11e : 0x114) + level instead of (c[9] ? 10 : 0) + 0x114 + level gives the original's and eax,0xa; add eax,0x114; add eax,ecx (notes/vc6-matching.md round entry; the same spelling is owed to the other level/job handlers, 0x253 variants: (c[9] ? 0x27b : 0x253) + job), and dropping the unsigned char lv local (read c[0xd] twice) keeps the byte load in cl and removes the 4-byte frame slot. Tried and reverted: level check forms (>=3 with r=4 inline, nested if/else, inverted <3 with else r=4, early return 4), r = 2 before/after the level check, a single final return r vs early returns, declaration order of r and buf. The 0x4f4ff0 idiom and the else-if chain gave the best byte counts. Needs match-deep's allocation toolbox (vargen ren / the push-pop: block the callee-saved register crosses lead). */

int ord_LevelOrder(char *c, int *m)
{
    char buf[0x100];
    int r;

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] >= 3) {
        r = 4;
    } else {
        r = 2;
        if (m[0] == 3 && c[2] == 6) {
            txt_Format(buf, 0x1d34, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
            ord_PrivilegeDialog(buf, 0, 0x657c7);
        } else if (m[0] == 1 && c[2] == 6)
            txt_Format(m + 5, "%s", 0x1d33);
    }
    return r;
}
/* 0x4f4ff0, an order handler for a player (+2 = 6) or a kind 5 character; kind 3 of a player goes to 0x5cd400 (| 2), kind 1 of a player writes text 0x1d35 into m + 0x14; 1 when not handled, else 2 */

int ord_EspionageResultOrder(char *c, int *m)
{
    int r;

    r = 2;
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (c[2] != 6 && c[2] != 5)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        r = ai_EspionageResult(c, m);
        r |= 2;
    } else if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(ORD_MSG(m)->text, "%s", 0x1d35);
    return r;
}
/* 0x4f5070, the medicus order: a sick character (v024 below -20, the int read as a double, or v02c's
   bits above 0xf) that is not already waiting on the flag 0x40 opens the medicus privilege window (kind 3)
   or, for a player's do (kind 1, kind 6), gets the text 0x1d3a into m + 0x14; else 1 (not handled), 2 */

int ord_MedicusOrder(char *c, int *m)
{
    int k;

    k = m[0];
    if ((k == 3 || k == 1) && (ORD_CHAR(c)->flags1c8 & 0x40) == 0
        && (ORD_CHAR(c)->v024 < -20.0 || (ORD_CHAR(c)->v02c & 0xfffffff0) != 0)) {
        if (k == 3)
            return ai_MedicusHandler(c, m);
        if (k == 1 && ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1d3a);
        return 2;
    }
    return 1;
}
/* 0x4f50f0, an order handler (c, m; kinds 1 / 3): characters with the byte +0x166 = 0xd are not handled; so are those that already own an order 0x2c (0x53e400 (1, 0, 0x2c) .. +0xac = c's id); kind 3 goes to 0x5cde90, kind 1 of a player gets the text 0x1d44; else 2 */

int ord_SpyTargetOrder(char *c, int *m)
{
    char *o;

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (ORD_CHAR(c)->job == 0xd)
        return 1;
    o = he_FindEvent(1, 0, 0x2c);
    if (o != 0) {
        do {
            if (((ORDEVENT *)o)->ownerId == ORD_CHAR(c)->netId)
                return 1;
            o = he_NextEvent();
        } while (o != 0);
    }
    if (m[0] == 3) {
        ai_SpyTargetHandler(c, m);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d44);
    return 2;
}
/* 0x4f5190, an order handler (c, m; kinds 1 / 3): not for characters with the flag 0x100 of +0x1c8; kind 3 goes to 0x5c9550, kind 1 of a player writes the text 0x1d54; else 2 */

int ord_BlackmailBuildingOrder(char *c, int *m)
{
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1c8 & 0x100) != 0)
        return 1;
    if (m[0] == 3) {
        ai_BlackmailBuildingListener(c, m);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d54);
    return 2;
}
/* 0x4f5200, an order handler (c the character, m the message, kind 1 or 3) for level 3 (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d5f (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1d5e into m + 0x14; else 2 */

int ord_Level3Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] > 3)
        return 4;
    if ((unsigned char)c[0xd] < 3)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d5f, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d5e);
    return 2;
}
/* 0x4f52e0, an order handler (c, m; kinds 1 / 3): characters below level 3 (c[0xd]) are not handled; kind 3 goes to 0x5c9ff0, kind 1 of a player writes the text 0x1d60; else 2 */

int ord_BlackmailContactVariantOrder(char *c, int *m)
{
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 3)
        return 1;
    if (m[0] == 3) {
        ai_BlackmailContactVariant(c, m);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d60);
    return 2;
}
/* 0x4f5350, an order handler (c, m; kinds 1 / 3): not for level below 3 or the flag 0x400 of +0x1c8; kind 3 goes to 0x5c9c50, kind 1 of a player writes the text 0x1d65; else 2 */

int ord_BlackmailContactOrder(char *c, int *m)
{
    if ((m[0] == 3 || m[0] == 1) && (unsigned char)c[0xd] >= 3 && (ORD_CHAR(c)->flags1c8 & 0x400) == 0) {
        if (m[0] == 3) {
            ai_BlackmailContact(c, m);
            return 2;
        }
        if (m[0] == 1 && c[2] == 6)
            txt_Format(m + 5, "%s", 0x1d65);
        return 2;
    }
    return 1;
}
/* 0x4f53c0, an order handler (c the character, m the message, kind 1 or 3) for level 4 (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d85 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1d84 into m + 0x14; else 2 */

int ord_Level4Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] > 4)
        return 4;
    if ((unsigned char)c[0xd] < 4)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d85, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d84);
    return 2;
}
/* 0x4f54a0, an order handler (c the character, m the message, kind 1 or 3) for level 4 (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d87 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1d86 into m + 0x14; else 2 */

int ord_Level4Order2(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] > 4)
        return 4;
    if ((unsigned char)c[0xd] < 4)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d87, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d86);
    return 2;
}
/* 0x4f5580, an order handler (c the character, m the message, kind 1 or 3) for levels 4 .. 5 (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d89 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1d88 into m + 0x14; else 2 */

int ord_Level4To5Order(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] >= 6)
        return 4;
    if ((unsigned char)c[0xd] < 4)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d89, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d88);
    return 2;
}
/* 0x4f5660, an order handler (c the character, m the message, kind 1 or 3) for level 5 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d8b (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1d8a into m + 0x14; else 2 */

int ord_Level5UpOrder(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 5)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d8b, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d8a);
    return 2;
}
/* 0x4f5730, an order handler (c the character, m the message, kind 1 or 3) for level 5 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d8d (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1d8c into m + 0x14; else 2 */

int ord_Level5UpOrder2(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 5)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d8d, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d8c);
    return 2;
}
/* 0x4f5800, an order handler (c, m; kinds 1 / 3) for characters of level 5 or more: m[0x17] (+0x5c) is a character id of the table 0x505df0 (-1 or none: 1), the record needs its byte +8; a character table entry that is in use must exist (the scan below only finds the first used record; its index must be below 0x300); kind 3 goes to 0x5c8ef0, kind 1 of a player writes the text 0x1d8e; else 2 @deep meaning settled (three fixes this session: the partner id comes from the character c + 0x5c, not the message m[0x17] -- the original reads [ebx+0x5c] with ebx = c at 0x4f5857; the scan breaks when the job byte is <= 0x15, not > 0x15 (0x4f5845: cmp cl,0x15; jbe exit, both tests jump to the exit block); the id compare is the unsigned short field against 0xffff). 153 -> 147 differing bytes, 133 code bytes left, all the register roles: the original keeps m in edi and c in ebx (ours m = esi, c = edi) and, because its loop counter owns edx, compares the id with the immediate 0xffff where ours hoists 0xffff into edx (cmp WORD PTR [eax-0x166],dx). Tried: int vs unsigned loop counter, short vs unsigned short id compare (-1 vs 0xffff). Readability NOT done: Q4 needs the scan rewritten through the ORDCHAR view (the current char* p at +0x166 with p[-0x166] keeps the original's [eax-0x166] / [eax] displacements and the 0x7d60e6 bound; a record-start pointer changes both the displacements and the relocation in the bound), Q3 the 0x7d60e6 literal (= ORD_CHARS + 0x300 + 0x166), Q2/Q7 the kind-3 callee 0x5c8ef0: its definition in unk_58bbd0.c takes ONE parameter while this call pushes two (0x4f58a2 push m; push c), so the prototype is a cross-unit fix for that unit (the window is 'privillegien_scheidung', the divorce privilege handler -- the name ai_DivorceDialog is ready). Q1 name ready too (ord_DivorceOrder: table handler, level 5+, opens the divorce dialog). */

int unk_Unk4f5800(char *c, int *m)
{
    int i;
    char *p;
    char *o;

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 5)
        return 1;
    i = 0;
    p = &ai_Chars[0][0x166];
    do {
        if (*(unsigned short *)(p - 0x166) != 0xffff) {
            if (*p != 0x12 || (unsigned char)*p <= 0x15)
                break;
        }
        p += 0x21c;
        i++;
    } while ((int)p < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x164));
    if (*(int *)(c + 0x5c) == -1)
        return 1;
    if (i >= 0x300)
        return 1;
    o = gm_FindChar(*(int *)(c + 0x5c));
    if (o == 0)
        return 1;
    if (o[8] == 0)
        return 1;
    if (m[0] == 3)
        return ai_Unk5c8ef0(c, m);
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d8e);
    return 2;
}
/* 0x4f58e0, an order handler (c the character, m the message, kind 1 or 3) for level 6 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d93 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1d92 into m + 0x14; else 2 */

int ord_Level6UpOrder(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 6)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d93, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d92);
    return 2;
}
/* 0x4f59b0, an order handler (c the character, m the message, kind 1 or 3) for level 6 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1d95 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1d94 into m + 0x14; else 2 */

int ord_Level6UpOrder2(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 6)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1d95, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d94);
    return 2;
}
/* 0x4f5a80, the wage report order (table 0x611768 entry 17; c the character, m the message, kinds 1 / 3) for
   characters with a profession (job / job2, 0x166 / 0x169) or the privilege flags 0xe00 of +0x1cc: a player
   (+2 = 6) with kind 3 gets the title text 0x1d96 through "$Z%s$N", then one 0x1d97 line per profession and per
   wage record of gm_CharacterWages (0x4fa700; the profession text 0x253 + the job, 0x27b with textVariant set,
   the amount at +4 / +8 / +0xc), shown through ord_PrivilegeDialog (mode 1); kind 1 writes the text 0x1d96 into
   m + 5; else 2
   @deep meaning settled and readable; only the callee-saved save placement differs: the original pushes esi at entry, ebx at 0x4f5ada after the two return-1 blocks and edi inside the kind-3 block, ours pushes ebx in the prologue (frame 0x410, 149 instructions and register roles equal); tried: declarations inside the kind-3 block, spell push-pop/order hints, early-return guards -> needs the register-allocation toolbox
   pushes esi in the entry block, ebx at 0x4f5ada (after the two return-1 blocks) and edi inside the kind-3
   block, and pops ebx before esi; ours pushes ebx in the prologue. The frame (0x410), the 149 instructions and
   the register roles (ebx = base, esi = c, edi = buf) are otherwise equal (tools/asmdiff.py). Tried and
   reverted: o / buf / base declared inside the kind-3 block (no change), spell push-pop / order hints, the
   early-return guards (the two return-1 blocks are already separate). Needs match-deep's register-allocation
   toolbox. */

int ord_WageReportOrder(char *c, int *m)
{
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (ORD_CHAR(c)->job == 0 && ORD_CHAR(c)->job2 == 0 && (ORD_CHAR(c)->flags1cc & 0xe00) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        ORDWAGES o;
        char buf[0x400];
        int base;

        gm_CharacterWages(ORD_CHAR(c)->id, 0, (unsigned char *)&o);
        memset(buf, 0, sizeof buf);
        base = (ORD_CHAR(c)->textVariant != 0 ? 0x27b : 0x253);
        txt_Format(buf, "$Z%s$N", 0x1d96);
        if (ORD_CHAR(c)->job != 0)
            txt_Format(buf + strlen(buf), 0x1d97, ORD_CHAR(c)->job + base, o.amount);
        if (ORD_CHAR(c)->job2 != 0)
            txt_Format(buf + strlen(buf), 0x1d97, ORD_CHAR(c)->job2 + base, o.amount2);
        if (o.kind != 0)
            txt_Format(buf + strlen(buf), 0x1d97, o.kind + base, o.amount3);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1d96);
    return 2;
}
/* 0x4f5c30, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4): clears the text at m + 0x14; kinds 3 / 1: only for characters with +0x166 != 0 (text 0x1d98 for a player), 9 when they have the flag 0x200 of +0x1c8, 1 with 0x200000, kind 3 runs 0x5ca560, result | 2; kinds 4 / 2: only when c has the flag 0x100 and the message's character m[0x87] has +0x166 != 0 and not 0x200000; kind 4 runs 0x5ca560, result | 0xa */

int ord_EmbezzlementOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->job == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1d98);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & 0x200000) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_EmbezzlementPrivilege(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & 0x200000) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_EmbezzlementPrivilege(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1d98);
        return r | 0xa;
    }
    return 1;
}
/* 0x4f5d50, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f5d50: the table record 1 of 0x4f2870 (the byte rec[0] goes to m[1] and, for a player, into the text at m + 0x14 through 0x4f2fe0), the flag 0x80000 of +0x1c8 of the character (and of m[0x87] for 2 / 4) blocks; kind 3 / 4 sets m[0x85] = 1 and runs the law window 0x5ca150; result | 2 (| 0xa for 4 / 2) */

int ord_LawOrder(char *c, int *m)
{
    int rec[9];
    int r;
    char *t;

    r = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->job == 0)
            return 1;
        if (gv_FindTableRecord(c, 1, rec) == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            gv_TableRecordText(ORD_MSG(m)->text, rec[0] & 0xff);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & 0x80000) != 0)
            return 1;
        m[1] = rec[0] & 0xff;
        if (m[0] == 3) {
            ORD_MSG(m)->answerFlag = 1;
            r = ai_LawPrivilegeWindow(c, m);
        }
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & 0x80000) != 0)
            return 1;
        if (gv_FindTableRecord(t, 1, rec) == 0)
            return 1;
        m[1] = rec[0] & 0xff;
        if (m[0] == 4) {
            ORD_MSG(m)->answerFlag = 1;
            r = ai_LawPrivilegeWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            gv_TableRecordText(ORD_MSG(m)->text, rec[0] & 0xff);
        return r | 0xa;
    }
    return 1;
}
/* 0x4f5f00, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f5d50: the table record 2 of 0x4f2870 (the byte rec[0] goes to m[1] and, for a player, into the text at m + 0x14 through 0x4f2fe0), the flag 0x100000 of +0x1c8 of the character (and of m[0x87] for 2 / 4) blocks; kind 3 / 4 sets ORD_MSG(m)->answerFlag = 2 and runs 0x5ca150; result | 2 (| 0xa for 4 / 2) */

int ord_LawOrder2(char *c, int *m)
{
    int rec[9];
    int r;
    char *t;

    r = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->job == 0)
            return 1;
        if (gv_FindTableRecord(c, 2, rec) == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            gv_TableRecordText(ORD_MSG(m)->text, rec[0] & 0xff);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & 0x100000) != 0)
            return 1;
        m[1] = rec[0] & 0xff;
        if (m[0] == 3) {
            ORD_MSG(m)->answerFlag = 2;
            r = ai_LawPrivilegeWindow(c, m);
        }
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & 0x100000) != 0)
            return 1;
        if (gv_FindTableRecord(t, 2, rec) == 0)
            return 1;
        m[1] = rec[0] & 0xff;
        if (m[0] == 4) {
            ORD_MSG(m)->answerFlag = 2;
            r = ai_LawPrivilegeWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            gv_TableRecordText(ORD_MSG(m)->text, rec[0] & 0xff);
        return r | 0xa;
    }
    return 1;
}
/* 0x4f60b0, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da0 (0x25e, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1d9f into m + 0x14; else 2 */

int ord_Job11Order(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (c[0x166] != 0xb)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da0, (c[9] != 0 ? 0x286 : 0x25e));
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1d9f);
    return 2;
}
/* 0x4f6180, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da2 (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da1 into m + 0x14; else 2 */

int ord_JobMultiOrder(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x1b && (unsigned char)c[0x166] != 0xf && (unsigned char)c[0x166] != 0x15 && (unsigned char)c[0x166] != 0xd)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da2, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x166]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da1);
    return 2;
}
/* 0x4f6270, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da4 (0x26f, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da3 into m + 0x14; else 2 */

int ord_Job28Order(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x169] != 0x1c)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da4, (c[9] != 0 ? 0x297 : 0x26f));
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da3);
    return 2;
}
/* 0x4f6340, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da6 (0x270, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da5 into m + 0x14; else 2 */

int ord_Job29Order(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x169] != 0x1d)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da6, (c[9] != 0 ? 0x298 : 0x270));
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da5);
    return 2;
}
/* 0x4f6410, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1da8 (0x275, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da7 into m + 0x14; else 2 */

int ord_Job34Order(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x169] != 0x22)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1da8, (c[9] != 0 ? 0x29d : 0x275));
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da7);
    return 2;
}
/* 0x4f64e0, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1daa (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1da9 into m + 0x14; else 2 */

int ord_JobRangeOrder(char *c, int *m)
{
    char buf[0x200];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x169] < 0x1e || (unsigned char)c[0x169] > 0x21)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1daa, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x169]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1da9);
    return 2;
}
/* 0x4f65c0, an order handler (c, m; kinds 1 / 3) for the characters of one kind of job (+0x166 / +0x169 tested at the top; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dac (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dab into m + 0x14; else 2 */

int ord_JobMultiOrder2(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x14 && (unsigned char)c[0x166] != 0x18 && (unsigned char)c[0x166] != 0x19)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dac, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x166]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dab);
    return 2;
}
/* 0x4f66b0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) for the jobs 0x16 / 0x19 / 0x12 (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): a player's text 0x1dad (m + 0x14), 9 with the flag 0x200, kind 3 / 4 run 0x5cacd0, 0xa for 4 / 2, else 2 */

int ord_EmbezzlementContractOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (c[2] == 6)
            txt_Format(m + 5, "%s", 0x1dad);
        j = ORD_CHAR(c)->job;
        if (j != 0x16 && j != 0x19 && j != 0x12)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EmbezzlementContract(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x16 && j != 0x19 && j != 0x12)
            return 1;
        if (m[0] == 4) {
            ai_EmbezzlementContract(c, m);
            return 0xa;
        }
        if (c[2] == 6)
            txt_Format(m + 5, "%s", 0x1dad);
        return 0xa;
    }
    return 1;
}
/* 0x4f67c0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the jobs 0x13 / 0x14 / 0x18 / 0x1a, runs 0x5cae00, text 0x1db6 */

int ord_CounterEspionageOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (c[2] == 6)
            txt_Format(m + 5, "%s", 0x1db6);
        j = ORD_CHAR(c)->job;
        if (j != 0x13 && j != 0x14 && j != 0x18 && j != 0x1a)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_CounterEspionage(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x13 && j != 0x14 && j != 0x18 && j != 0x1a)
            return 1;
        if (m[0] == 4) {
            ai_CounterEspionage(c, m);
            return 0xa;
        }
        if (c[2] == 6)
            txt_Format(m + 5, "%s", 0x1db6);
        return 0xa;
    }
    return 1;
}
/* 0x4f68d0, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dbc ((c[9] != 0 ? 0x28c : 0x264)) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dbb into m + 0x14; else 2 */

int ord_Job17Order(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x11)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dbc, (c[9] != 0 ? 0x28c : 0x264));
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dbb);
    return 2;
}
/* 0x4f69a0, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dbe ((c[9] != 0 ? 0x28c : 0x264)) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dbd into m + 0x14; else 2 */

int ord_Job17Order2(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x11)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dbe, (c[9] != 0 ? 0x28c : 0x264));
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dbd);
    return 2;
}
/* 0x4f6a70, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dc0 (ORD_CHAR(c)->id) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dbf into m + 0x14; else 2 */

int ord_JobMultiOrder4(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0xe && (unsigned char)c[0x166] != 0xa)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dc0, ORD_CHAR(c)->id);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dbf);
    return 2;
}
/* 0x4f6b40, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) for the job 0xe, text 0x1dc1, blocked by the flag 0x400000 of +0x1c8, runs 0x5ca860, result | 2 (| 0xa) */

int ord_MisappropriationOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1dc1);
        if (ORD_CHAR(c)->job != 0xe)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1c8 & ORD_FLAG_BLOCKED) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_MisappropriationWindow(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if (ORD_CHAR(t)->job != 0xe)
            return 1;
        if ((ORD_CHAR(t)->flags1c8 & ORD_FLAG_BLOCKED) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_MisappropriationWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(ORD_MSG(m)->text, "%s", 0x1dc1);
        return r | 0xa;
    }
    return 1;
}
/* 0x4f6c60, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dc5 (ORD_CHAR(c)->id) shown through 0x4f8200 (mode 2); kind 1 writes text 0x1dc4 into m + 0x14; else 2 */

int ord_Job10Order(char *c, int *m)
{
    char buf[0x800];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0xa)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dc5, ORD_CHAR(c)->id);
        ord_PrivilegeDialog(buf, 2, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dc4);
    return 2;
}
/* 0x4f6d30, an order handler (c, m; kinds 1 / 3) for one job (+0x166; others: 1): a player (+2 = 6) with kind 3 gets the text 0x1dc7 (ORD_CHAR(c)->id) shown through 0x4f8200 (mode 2); kind 1 writes text 0x1dc6 into m + 0x14; else 2 */

int ord_Job15Order(char *c, int *m)
{
    char buf[0x800];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0xf)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dc7, ORD_CHAR(c)->id);
        ord_PrivilegeDialog(buf, 2, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dc6);
    return 2;
}
/* 0x4f6e00, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0xf, 0x15, 0x1b (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cb250, a player's text 0x1dc8 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_CounterEspionageWindowOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dc8);
        j = ORD_CHAR(c)->job;
        if (j != 0xf && j != 0x15 && j != 0x1b)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_CounterEspionageWindow(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0xf && j != 0x15 && j != 0x1b)
            return 1;
        if (m[0] == 4) {
            ai_CounterEspionageWindow(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dc8);
        return 0xa;
    }
    return 1;
}
/* 0x4f6f10, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x18 (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cb4f0, a player's text 0x1dce (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionageWindowOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dce);
        j = ORD_CHAR(c)->job;
        if (j != 0x18)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionageWindow(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x18)
            return 1;
        if (m[0] == 4) {
            ai_EspionageWindow(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dce);
        return 0xa;
    }
    return 1;
}
/* 0x4f7000, an order handler (c, m; kinds 1 / 3) for the jobs 0x17 / 0x13 (+0x166): a player (+2 = 6) with kind 3 gets the text 0x1dd5 (0x253 + the job, + 0x28 with +9) shown through 0x4f8200 (mode 1); kind 1 writes text 0x1dd4 into m + 0x14; else 2 */

int ord_JobMultiOrder3(char *c, int *m)
{
    char buf[0x400];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0x166] != 0x17 && (unsigned char)c[0x166] != 0x13)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        memset(buf, 0, sizeof buf);
        txt_Format(buf, 0x1dd5, (c[9] != 0 ? 0x27b : 0x253) + (unsigned char)c[0x166]);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1dd4);
    return 2;
}
/* 0x4f70f0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x17, 0x13, 0x14, 0xd (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cba90, a player's text 0x1dd6 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionagePaymentOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dd6);
        j = ORD_CHAR(c)->job;
        if (j != 0x17 && j != 0x13 && j != 0x14 && j != 0xd)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionagePayment(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x17 && j != 0x13 && j != 0x14 && j != 0xd)
            return 1;
        if (m[0] == 4) {
            ai_EspionagePayment(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dd6);
        return 0xa;
    }
    return 1;
}
/* 0x4f7200, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x16, 0x19 (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cbe00, a player's text 0x1ddb (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionageOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1ddb);
        j = ORD_CHAR(c)->job;
        if (j != 0x16 && j != 0x19)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionageOrderHandler(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x16 && j != 0x19)
            return 1;
        if (m[0] == 4) {
            ai_EspionageOrderHandler(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1ddb);
        return 0xa;
    }
    return 1;
}
/* 0x4f7300, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x15, 0x1a (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cc700, a player's text 0x1de0 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionagePaymentOrder2(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1de0);
        j = ORD_CHAR(c)->job;
        if (j != 0x15 && j != 0x1a)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionagePayment2(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x15 && j != 0x1a)
            return 1;
        if (m[0] == 4) {
            ai_EspionagePayment2(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1de0);
        return 0xa;
    }
    return 1;
}
/* 0x4f7400, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x1b (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cc2b0, a player's text 0x1de3 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionageRelationOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1de3);
        j = ORD_CHAR(c)->job;
        if (j != 0x1b)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionageRelation(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x1b)
            return 1;
        if (m[0] == 4) {
            ai_EspionageRelation(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1de3);
        return 0xa;
    }
    return 1;
}
/* 0x4f74f0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x18 (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cca40, a player's text 0x1de9 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionageAmountOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1de9);
        j = ORD_CHAR(c)->job;
        if (j != 0x18)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionageAmountWindow(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x18)
            return 1;
        if (m[0] == 4) {
            ai_EspionageAmountWindow(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1de9);
        return 0xa;
    }
    return 1;
}
/* 0x4f75e0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x12, 0x16 (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5ce1a0, a player's text 0x1dec (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_SpyTargetFlagOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dec);
        j = ORD_CHAR(c)->job;
        if (j != 0x12 && j != 0x16)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_SpyTargetFlagHandler(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x12 && j != 0x16)
            return 1;
        if (m[0] == 4) {
            ai_SpyTargetFlagHandler(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1dec);
        return 0xa;
    }
    return 1;
}
/* 0x4f76e0, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4) like 0x4f66b0 for the job(s) 0x1a (+0x166 of c; of m[0x87] for 2 / 4, where c needs the flag 0x100 of +0x1c8): runs 0x5cd2e0, a player's text 0x1df5 (m + 0x14), 9 with the flag 0x200, 0xa for 4 / 2, else 2 */

int ord_EspionageOverviewOrder(char *c, int *m)
{
    char *t;
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1df5);
        j = ORD_CHAR(c)->job;
        if (j != 0x1a)
            return 1;
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if (m[0] == 3)
            ai_EspionageOverviewHandler(c, m);
        return 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        j = ORD_CHAR(t)->job;
        if (j != 0x1a)
            return 1;
        if (m[0] == 4) {
            ai_EspionageOverviewHandler(c, m);
            return 0xa;
        }
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1df5);
        return 0xa;
    }
    return 1;
}
/* 0x4f77d0, an order handler (c, m; kind 1 or 3) for level 7 (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1e02 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e01 into m + 0x14; else 2 */

int ord_Level7Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] > 7)
        return 4;
    if ((unsigned char)c[0xd] < 7)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1e02, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1e01);
    return 2;
}
/* 0x4f78b0, an order handler (c, m; kind 1 or 3) for level 8 (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1e04 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e03 into m + 0x14; else 2 */

int ord_Level8Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] > 8)
        return 4;
    if ((unsigned char)c[0xd] < 8)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1e04, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1e03);
    return 2;
}
/* 0x4f7990, an order handler (c, m; kind 1 or 3) for level 9 and up (c[0xd]; above: 4, below: 1); a player (+2 = 6) with kind 3 gets the text 0x1e06 (level + 0x114, + 10 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e05 into m + 0x14; else 2 */

int ord_Level9UpOrder(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] < 9)
        return 1;
    if (m[0] == 3 && c[2] == 6) {
        txt_Format(buf, 0x1e06, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && c[2] == 6)
        txt_Format(m + 5, "%s", 0x1e05);
    return 2;
}
/* 0x4f7a60, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4): clears the text at m + 0x14; characters need the flag 0x800 of +0x1cc (and m[0x87] for 2 / 4), 0x80000 of +0x1cc blocks; text 0x1e07 for a player, 9 with the flag 0x200 of +0x1c8, kind 3 / 4 run 0x5ce610, result | 2 (| 0xa for 4 / 2) */

int ord_InterrogationOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] == 3 || m[0] == 1) {
        if ((ORD_CHAR(c)->flags1cc & 0x800) == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1e07);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1cc & 0x80000) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_InterrogationWindow(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1cc & 0x800) == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1cc & 0x80000) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_InterrogationWindow(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1e07);
        return r | 0xa;
    }
    return 1;
}
/* 0x4f7b80, an order handler (c, m; kinds 1 / 3 and the answers 2 / 4): clears the text at m + 0x14; characters need the flag 0x800 of +0x1cc (and m[0x87] for 2 / 4), 0x40000 of +0x1cc blocks; text 0x1e0d for a player, 9 with the flag 0x200 of +0x1c8, kind 3 / 4 run 0x5ce950, result | 2 (| 0xa for 4 / 2) */

int ord_DismissalOrder(char *c, int *m)
{
    int r;
    char *t;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] == 3 || m[0] == 1) {
        if ((ORD_CHAR(c)->flags1cc & 0x800) == 0)
            return 1;
        if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1e0d);
        if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
            return 9;
        if ((ORD_CHAR(c)->flags1cc & 0x40000) != 0)
            return 1;
        if (m[0] == 3)
            r = ai_DismissalHandler(c, m);
        return r | 2;
    }
    if (m[0] == 4 || m[0] == 2) {
        if ((ORD_CHAR(c)->flags1c8 & 0x100) == 0 || (t = (char *)ORD_MSG(m)->partner) == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1cc & 0x800) == 0)
            return 1;
        if ((ORD_CHAR(t)->flags1cc & 0x40000) != 0)
            return 1;
        if (m[0] == 4) {
            r = ai_DismissalHandler(c, m);
        } else if (ORD_CHAR(c)->kind == 6)
            txt_Format(m + 5, "%s", 0x1e0d);
        return r | 0xa;
    }
    return 1;
}
/* 0x4f7ca0, an order handler (c, m; kinds 1 / 3) for characters with the flag 0x800 of +0x1cc: a player (+2 = 6) with kind 3 gets the text 0x1e12 (0x278, + 0x28 with +9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e11 into m + 0x14; else 2 */

int ord_Flag800Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x800) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        txt_Format(buf, 0x1e12, (ORD_CHAR(c)->textVariant != 0 ? 0x2a0 : 0x278));
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e11);
    return 2;
}
/* 0x4f7d70, an order handler (c, m; kinds 1 / 3) for characters with the flag 0x800 of +0x1cc: a player (+2 = 6) with kind 3 gets the text 0x1e14 (0x278, + 0x28 with +9, extra argument 0x16b9) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e13 into m + 0x14; else 2 */

int ord_Flag800Order2(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x800) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        txt_Format(buf, 0x1e14, (ORD_CHAR(c)->textVariant != 0 ? 0x2a0 : 0x278), 0x16b9);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e13);
    return 2;
}
/* 0x4f7e40, an order handler (c, m; kinds 1 / 3): clears the text at m + 0x14; not for characters with the flag 0x100 of +0x1c8, they need 0x200 of +0x1cc; text 0x1e15 for a player, 9 with the flag 0x200, 1 with 0x40000 of +0x1c8; kind 3 runs 0x5cede0, result | 2 */

int ord_InterrogationOrder2(char *c, int *m)
{
    int r;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1c8 & 0x100) != 0)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x200) == 0)
        return 1;
    if (ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e15);
    if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
        return 9;
    if ((ORD_CHAR(c)->flags1c8 & 0x40000) != 0)
        return 1;
    if (m[0] == 3)
        r = ai_InterrogationWindow1(c, m);
    return r | 2;
}
/* 0x4f7ef0, an order handler (c, m; kinds 1 / 3) for characters with the flag 0x200 of +0x1cc: a player (+2 = 6) with kind 3 gets the text 0x1e1b (0x276, + 0x28 with +9, extra argument 0x16ba) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e1a into m + 0x14; else 2 */

int ord_Flag200Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x200) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        txt_Format(buf, 0x1e1b, (ORD_CHAR(c)->textVariant != 0 ? 0x29e : 0x276), 0x16ba);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e1a);
    return 2;
}
/* 0x4f7fc0, an order handler (c, m; kinds 1 / 3): clears the text at m + 0x14; not for characters with the flag 0x100 of +0x1c8, they need 0x400 of +0x1cc; text 0x1e1c for a player, 9 with the flag 0x200, 1 with 0x40000 of +0x1c8; kind 3 runs 0x5cf020, result | 2 */

int ord_InterrogationOrder3(char *c, int *m)
{
    int r;

    r = 0;
    ORD_MSG(m)->text[0] = 0;
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1c8 & 0x100) != 0)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x400) == 0)
        return 1;
    if (ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e1c);
    if ((ORD_CHAR(c)->flags1c8 & 0x200) != 0)
        return 9;
    if ((ORD_CHAR(c)->flags1c8 & 0x40000) != 0)
        return 1;
    if (m[0] == 3)
        r = ai_InterrogationWindow2(c, m);
    return r | 2;
}
/* 0x4f8070, an order handler (c, m; kinds 1 / 3) for characters with the flag 0x400 of +0x1cc: a player (+2 = 6) with kind 3 gets the text 0x1e22 (0x277, + 0x28 with +9, extra argument 0x16bb) shown through 0x4f8200 (mode 0); kind 1 writes text 0x1e21 into m + 0x14; else 2 */

int ord_Flag400Order(char *c, int *m)
{
    char buf[0x100];

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((ORD_CHAR(c)->flags1cc & 0x400) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        txt_Format(buf, 0x1e22, (ORD_CHAR(c)->textVariant != 0 ? 0x29f : 0x277), 0x16bb);
        ord_PrivilegeDialog(buf, 0, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1e21);
    return 2;
}
/* 0x4f8140, an order handler (c, m; kinds 1 / 3) for the jobs 1 .. 0x1b (+0x166): kind 3 of a player runs 0x5ce7f0, kind 1 writes the text 0x1e23 (m + 0x14); else 2 (1 for other jobs and kinds) @deep meaning settled and readable; the remaining 85 code bytes are allocation only: the original keeps m[0] in ecx and the constant 2 in eax (hoisted to the prologue, re-materialised after the txt_Format call) and pushes ebx+esi (job in bl), ours uses eax/ecx+edx and pushes esi only - the same statements in the same order (tools/disasm.py 0x4f8140 vs asmdiff). Tried and reverted: r = 2 as a separate assignment and as an initialiser with if/else-if and one final return r, the return-2/return-1 form, single-return with r = 1 in both else branches (that one pushes edi and is worse). Needs match-deep's register-allocation toolbox */

int ord_DismissWorkerOrder(char *c, int *m)
{
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        j = ORD_CHAR(c)->job;
        if (j != 0 && j <= 0x1b) {
            if (m[0] == 3 && ORD_CHAR(c)->kind == 6)
                return ai_WorkerDismissalWindow(c, m);
            if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
                txt_Format(ORD_MSG(m)->text, "%s", 0x1e23);
            return 2;
        }
    }
    return 1;
}
/* 0x4f81c0, the order dispatcher: the handler number id (below 0x3a, else 0x40) of the table 0x611768 (0x4f4e20 .. 0x4f8140, one per order) runs on the character c (0x40 without one, 1 for a character whose kind +2 is 10 or more) and the message m */

int ord_DispatchOrder(int id, char *c, int *m)
{
    if (id >= 0x3a)
        return 0x40;
    if (c == 0)
        return 0x40;
    if ((unsigned char)c[2] >= 10)
        return 1;
    return ord_OrderHandlers[id](c, m);
}
/* 0x4f8200, a privilege dialog: loads the form privillegien\\prv_small (mode 0), prv_big (1) or prv_very_big, shows the text, then waits through 0x5309a0 (this function as its callback, arg its argument), setting the flag 0x6f22a0 while the redraw flag 0x14673a0 is set; closes the form */

int ord_PrivilegeDialog(char *text, int mode, int arg)
{
    int v;

    if (mode == 0)
        v = d2_OpenForm(0, 0, "privillegien\\prv_small");
    else if (mode == 1)
        v = d2_OpenForm(0, 0, "privillegien\\prv_big");
    else
        v = d2_OpenForm(0, 0, "privillegien\\prv_very_big");
    d2_CentreViewWindows(v);
    d2_SetForm(v, 1);
    io_PrintText(text);
    d2_ClickedId = -1;
    while (sv_RunFrame(ord_PrivilegeDialog, arg) != 0) {
        if (dm_L.rup != 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(v);
}
/* 0x4f82a0, resets the statistics at the start of a game (the only caller is main_StartGame, 0x537e3f, after
   the graveyard clear): clears the message record 0x91b480 (0x24 bytes, then its first int = -1 = no event),
   the income history gm_IncomeHistory (0x154), the character statistics 0x91b500 (0x68), the 8 x 8 grid
   ai_Grid (0x600), the statistics history (0x140), the last statistics record (0x28) and the base rating
   ai_RatingBase. No result. */

void ord_ResetStatistics(void)
{
    memset(&ord_MessageRecord, 0, 0x24);
    memset(gm_IncomeHistory, 0, 0x154);
    memset(gm_CharacterStatistics, 0, 0x68);
    memset(ai_Grid, 0, 0x600);
    memset(ord_StatisticsHistory, 0, 0x140);
    memset(&ord_Statistics, 0, 0x28);
    ord_MessageRecord.enabled = -1;
    ai_RatingBase = 0;
}
/* 0x4f8300, the 8 x 8 grid cell of a position: d3_GetHeightAt gives the cell on scene 0's height map (d3_Scenes[0].hmap) and x / y are divided by the map's cell count per side (D3SM.size, +0x20) over 8; 0 when the call fails */

int ord_GetGridCell(char *p, int *x, int *y)
{
    D3SM *hmap;
    int n;
    int *py;

    py = y;
    hmap = (D3SM *)d3_Scenes[0].hmap;
    n = hmap->size / 8;
    if (d3_GetHeightAt(hmap, p, x, y, &y) != 0) {
        *x = *x / n;
        *py = *py / n;
        return 1;
    }
    return 0;
}

#define ORD_CELLS ((AIGRID *)ai_Grid)   /* the grid as flat cells (row y, column x = y * 8 + x) */

/* 0x4f8360, fills the 8 x 8 grid ai_Grid (cells of 0x18 bytes: +4 the number of objects, +8 the average):
   counts the objects of kind 6 (0x501190 / 0x501030 iteration; +0x61 a pointer, its +0x4c the position through
   0x4f8300 -> x, y) per cell with the value 0x509b10 (o, o[0x5c]) summed up in a local float table, then cell +8
   = sum / count (0 for empty cells). No result.
   @deep behaviour draft, allocation only: the count-clear loop's induction variable. The original keeps the row base in edx and copies it to the cursor (mov edx,0x91ae64 hoisted above the prologue pushes, mov eax,edx in the loop head, mov edx,eax after the inner loop, cmp eax,0x91b464); ours lets VC6 collapse the nested 8x8 clear into one pointer induction variable (mov eax,0x91ae64 in the preheader after the rep stos, cmp eax,0x604, jl). Everything else matches: the object loop, the average loop (ebx counter, esi cursor, cmp 0x91b608), the frame 0x10c and the three callee-saved pushes; the remaining hunks are 4 code (those 3 instructions) + slots + fill. Read against tools/disasm.py 0x4f8360 0x100: same statements, constants and callees. Tried and reverted (each built, obj checked): for(i=0;i<8;i++)for(j=0;j<8;j++) grid[i][j].used=0 flat and 2D-typed (extern AIGRID grid[8][8], also via a flat (AIGRID*) cast); AIGRID *row = grid[i] with an inner j; char *row walk from grid with *(row+j*0x18+4)=0 and from grid+4 with row[j*0x18]; unsigned char *row over &grid[0].used (folds to one IV too); the clear before vs after the memset(f,0,sizeof f); separate i/j counters instead of the address-taken x/y. spell.py: reload/dup-tail/slots leads describe the same allocation (the x/y stack slots 0x10/0x14 are swapped: original x at 0x14, ours at 0x10; declaration order of x/y does not change it). Q4 done via ai.h's AIGRID and the ORDBLD view for the building record; Q1 ord_CountGridObjects, Q2 clean, Q7 through gv.h. Needs match-deep's induction-variable / allocation toolbox. */
void ord_CountGridObjects(void)
{
    float f[64];
    ORDBLD *b;
    int y;
    int x;
    int i;
    int j;
    memset(f, 0, sizeof f);
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            ai_Grid[i][j].used = 0;
    b = (ORDBLD *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (b->object != 0 && ord_GetGridCell((char *)&b->object->pos, &x, &y) != 0) {
            i = y + x * 8;
            f[i] = (float)gm_EquipmentValue((unsigned char *)b, b->level);
            ORD_CELLS[i].used++;
        }
        b = (ORDBLD *)gm_BuildNext();
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (ai_Grid[i][j].used != 0)
                ai_Grid[i][j].average = (int)(f[i * 8 + j] / ai_Grid[i][j].used);
            else
                ai_Grid[i][j].average = 0;
        }
    }
}
/* 0x4f86a0, a random point inside the cell (x, y) of scene 0's height map (d3_Scenes[0].hmap; the map's cell count per side (D3SM.size, +0x20) over 8 is the span of the grid cell): d3sm_GetVertex (0x45f100) with the third argument z */

int ord_RandomCellPoint(int x, int y, int z)
{
    D3SM *hmap;
    int n;
    unsigned a;
    unsigned b;

    hmap = (D3SM *)d3_Scenes[0].hmap;
    n = hmap->size / 8;
    a = util_RandomU(n) + n * x;
    b = util_RandomU(n) + n * y;
    return d3sm_GetVertexI(hmap, a, b, z);
}

/* the kind table 0x617468: one 8-byte record per kind (ORD_KIND reads the same pair as four shorts) */
typedef struct ORDKIND {
    int value;                      /* 0x0, the kind's value */
    int flags;                      /* 0x4, the kind's flags */
} ORDKIND;
#define ORD_KINDS ((ORDKIND *)ord_KindTable)

/* 0x4f8890, copies the kind record i (below 0x58) of ord_KindTable to out; 0 for a bad i */
int ord_GetKindRecord(int i, int *out)
{
    if (i >= 0x58)
        return 0;
    out[0] = ORD_KINDS[i].value;
    out[1] = ORD_KINDS[i].flags;
    return 1;
}
/* 0x4f8460, adds a random amount (0x42ec00 (8)) to the sums (+2, a short) of the cells of the 8 x 8 grid 0x91ae60 around the position of an object (0x4fea50 gives the object of who; 0x4f8300 the cell x, y of its +0x61 record's +0x4c): the 3 x 3 neighbourhood, clamped to the grid; the record kind of 0x4f2720 is read first (unused); the cell x, y go to *px / *py @deep behaviour draft, allocation / register-save placement only (109 code hunks, every statement equal). Read against tools/disasm.py 0x4f8460 0x130 and asmdiff: gv_GetTableRecord(kind, rec), gm_FindById(&b, 0, 0, who), amount = util_Random(8), y = util_Random(8), the o / o->object guard, ord_GetGridCell((char *)&b->object->pos, &x, &y), the +2 sum add (add WORD PTR [eax*8+0x91ae62],bx), the xhi / yhi clamps (mov [esp+0x3c],8 + cmp/jg), the xlo / ylo setl-dec-and clamps and the 3x3 add loop with the index edx + esi*8 all match. Diff: the original does NOT save ebx / esi / edi in the prologue - it saves them inside the if (0x4f84c6) and ebp inside the outer loop (0x4f8532) - and spills amount to [esp+0x20] until ebx is free (the add then uses bx); ours pushes edi in the prologue and keeps amount in edi, so the x / y slots and the register roles swap (ours amount=edi, y=ebx; original amount=ebx, y=edi). Tried: int amount (kept), unsigned short amount / y (the original's add uses bx and a 32-bit slot store, so they are ints), the ORDBLD typed view (kept, byte-neutral), gm_FindById prototyped in gv.h (Q7). Q1 ord_AddGridAmount, Q4 through ORDBLD + AIGRID, Q7 through gv.h. Needs match-deep's register-allocation / deferred-callee-save toolbox. */

void ord_AddGridAmount(int who, unsigned char kind, int *px, int *py)
{
    int rec[9];
    ORDBLD *b;
    int amount;
    int x;
    int y;
    int xhi;
    int yhi;
    int xlo;
    int ylo;
    int i;
    int j;

    gv_GetTableRecord(kind, rec);
    gm_FindById((int *)&b, 0, 0, who);
    amount = util_RandomU(8);
    y = util_RandomU(8);
    if (b != 0 && b->object != 0) {
        ord_GetGridCell((char *)&b->object->pos, &x, &y);
        ORD_CELLS[y + x * 8].b += amount;
        xhi = x + 1 > 8 ? 8 : x + 1;
        yhi = y + 1 > 8 ? 8 : y + 1;
        xlo = x - 1 < 0 ? 0 : x - 1;
        for (i = xlo; i <= xhi; i++) {
            ylo = y - 1 < 0 ? 0 : y - 1;
            for (j = ylo; j <= yhi; j++)
                ORD_CELLS[j + i * 8].a += amount;
        }
        *px = x;
        *py = y;
    }
}
/* 0x4f8590, the counterpart of 0x4f8460: subtracts the random amount from the cells (+2 of the cell of the object, +0 of its 3 x 3 neighbourhood); no outputs @deep behaviour draft, the same allocation / register-save placement as its sibling ord_AddGridAmount (0x4f8460, escalated in this session with the full reading). Read against tools/disasm.py 0x4f8590: the identical statements with the two subtracts (sub WORD PTR [eax*8+0x91ae62],bx for the cell's +2 sum and the 3x3 loop's +0 sum) and no px / py outputs - no missing or extra statement, constant or offset. Diff: the original saves ebx / esi / edi inside the if and ebp inside the outer loop, spills amount to a stack slot until ebx is free, and keeps y in edi; ours saves edi in the prologue and keeps amount in edi. Tried: unsigned short amount / y (the original's 32-bit slot store and bx add say int), the ORDBLD typed view (kept). Q1 ord_SubGridAmount, Q4 through ORDBLD + AIGRID, Q7 through gv.h. Needs match-deep's register-allocation / deferred-callee-save toolbox. */

void ord_SubGridAmount(int who, unsigned char kind)
{
    int rec[9];
    ORDBLD *b;
    int amount;
    int x;
    int y;
    int xhi;
    int yhi;
    int xlo;
    int ylo;
    int i;
    int j;

    gv_GetTableRecord(kind, rec);
    gm_FindById((int *)&b, 0, 0, who);
    amount = util_RandomU(8);
    y = util_RandomU(8);
    if (b != 0 && b->object != 0) {
        ord_GetGridCell((char *)&b->object->pos, &x, &y);
        ORD_CELLS[y + x * 8].b -= amount;
        xhi = x + 1 > 8 ? 8 : x + 1;
        yhi = y + 1 > 8 ? 8 : y + 1;
        xlo = x - 1 < 0 ? 0 : x - 1;
        for (i = xlo; i <= xhi; i++) {
            ylo = y - 1 < 0 ? 0 : y - 1;
            for (j = ylo; j <= yhi; j++)
                ORD_CELLS[j + i * 8].a -= amount;
        }
    }
}
/* 0x4f8700, picks a random object id for the character c (-1 without one): the ids of its two buildings
   (bld / bld2, when their object is present) and, up to 0x20 times, of up to 3 random buildings (3 of 4
   chances each) of a random one of the 20 kinds of the table (4 x 5), through gm_BuildQuery(1, 5, kind),
   until 16 are collected; then one of them at random */

int ord_RandomObjectId(char *c)
{
    int kinds[20];
    int list[16];
    int n;
    int tries;
    int k;
    int idx;
    ORDBLD *o;

    kinds[0] = 1;
    kinds[1] = 2;
    kinds[2] = 5;
    kinds[3] = 7;
    kinds[4] = 10;
    kinds[5] = 6;
    kinds[6] = 4;
    kinds[7] = 0x13;
    kinds[8] = 0x16;
    kinds[9] = 9;
    kinds[10] = 0xf;
    kinds[11] = 0x17;
    kinds[12] = 0x19;
    kinds[13] = 0x1a;
    kinds[14] = 0x18;
    kinds[15] = 8;
    kinds[16] = 0xe;
    kinds[17] = 0x12;
    kinds[18] = 0x14;
    kinds[19] = 0x15;
    if (c == 0)
        return -1;
    n = 0;
    tries = 0x20;
    if (ORD_CHAR(c)->bld != 0 && ORD_CHAR(c)->bld->object != 0) {
        list[0] = ORD_CHAR(c)->bld->id;
        n = 1;
    }
    if (ORD_CHAR(c)->bld2 != 0 && ORD_CHAR(c)->bld2->object != 0) {
        list[n] = ORD_CHAR(c)->bld2->id;
        n++;
    }
    while (n < 16 && tries != 0) {
        tries--;
        idx = util_RandomU(4) * 5 + util_RandomU(5);
        k = 0;
        o = (ORDBLD *)gm_BuildQuery(1, 5, kinds[idx]);
        while (o != 0 && k < 3) {
            if (n >= 16)
                goto done;
            if (util_RandomU(4) < 3) {
                list[n] = o->id;
                n++;
                k++;
            }
            o = (ORDBLD *)gm_BuildNext();
        }
    }
done:
    return list[util_RandomU(n)];
}
/* 0x4f88c0, shifts the 15 rounds of ord_StatisticsHistory down by one (the oldest drops out) and fills the last one from ord_Statistics (out[1] .. out[3] and the score out[9]) and ai_RatingBase truncated */

void ord_AppendStatisticsRound(void)
{
    memmove(&ord_StatisticsHistory[0], &ord_StatisticsHistory[1], 0x12c);
    ord_StatisticsHistory[15].noFirst = ord_Statistics.col[1];
    ord_StatisticsHistory[15].combined = ord_Statistics.col[2];
    ord_StatisticsHistory[15].noSecond = ord_Statistics.col[3];
    ord_StatisticsHistory[15].score = ord_Statistics.score;
    ord_StatisticsHistory[15].base = (int)ai_RatingBase;
}
/* 0x4f8920, rebuilds the three float sums (+0xc, +0x10, +0x14) of every cell of the 8 x 8 grid 0x91ae60 from the 0x200 records of 0x2d bytes at 0x91f700 (+0x25 a pointer, +0 an id, -1 = free): the object with the id at +0x21 (0x501190 (1, 1, id)) must have a +0x61 record and a building kind (ai_BTypes[kind * 0x24d]) outside 0xb .. 0xd and not 0x10; its cell x, y (0x4f8300), the table record of the byte +0x1c (0x4f2720): w = (short)(+0x22) / rec[2] * rec[1]; the center cell gets w * 0x5eb7ac at +0xc, each cell of the 3 x 3 neighbourhood w at +0x10 and w * 0x5eb7ac at +0xc @deep far draft (111 code / 16 fpu / 1 fill hunks, 378 of 448 bytes). Read against tools/disasm.py 0x4f8920 0x260: the per-cell float clear (3 stores of a zero register, 8 x 8 nested), the 0x91f700 record walk (p < ai_Crimes + 0x5a00, stride 0x2d, skip when the +0x25 int is 0 or the +0 id is -1), the 0x2d-byte copy, gm_BuildQuery(1, 1, r[0x21]), the guards (o[0] != 0, o->object != 0), the ai_BTypes kind byte (kind * 0x24d, rejected 0xb..0xd and 0x10), ord_GetGridCell(object + 0x4c, &x, &y), gv_GetTableRecord(r[0x1c], rec), w = (short)r[0x22] / rec[2] * rec[1] (the original's fild/fidiv/fimul - my double spelling compiled identically, so the float spelling is kept for the reading), the +0xc += w * 0.5f of the cell, the xhi/yhi/xlo/ylo clamps and the 3 x 3 loop (+0x10 += w, +0xc += w * 0.5f per cell) all match - no missing or extra statement, constant or offset. What is left is structural: the original's nested 8 x 8 clear (the same two-level induction variable that blocks 0x4f8360) and its countdown inner loop of the 3 x 3 block (fld w / fadd [eax+4] / fld w2 / fadd [eax-0x18] with the pointer advanced 0x18), plus the ebx/ebp/esi/edi save placement (the original saves them after the clear, ebp never at all) - allocation / scheduling = match-deep. Tried and measured: the float conversion spelling (byte-identical); the loop bound is ai_Crimes + 0x5a00 (the end of the records, 0x925100 is gv_Cannon: no second name gv_GraveyardEnd; with that name 36 bytes fewer differed). Q1 ord_RebuildGridSums, Q2 clean (ai_Crimes named from gv.c's own wording, AIGRID's floats named halfWeightSum / weightSum / buildingCount). Q4 left as the evidence gap: 5 raw accesses into the 0x2d-byte graveyard record (the int at +0x25, the id at +0, and the copy's +0x1c / +0x21 / +0x22); gv.c reads the same record with raw offsets and names no field, so the +0x1c kind byte / +0x21 id / +0x22 short / +0x25 state have no evidenced names to put in a typed view (a shared-header decision for match-deep). */

void ord_RebuildGridSums(void)
{
    char *p;
    char r[0x2d];
    int rec[9];
    int x;
    int y;
    float w;
    float w2;
    char *o;
    char *b;
    char *q;
    int i;
    int j;
    int xhi;
    int yhi;
    int xlo;
    int ylo;
    unsigned char k;

    for (i = 0; i < 64; i++) {
        ORD_CELLS[i].halfWeightSum = 0;
        ORD_CELLS[i].weightSum = 0;
        ORD_CELLS[i].buildingCount = 0;
    }
    for (p = ai_Crimes; p < ai_Crimes + 0x5a00; p += 0x2d) {
        if (*(int *)(p + 0x25) == 0 || *(int *)p == -1)
            continue;
        memcpy(r, p, 0x2d);
        o = (char *)gm_BuildQuery(1, 1, *(int *)(r + 0x21));
        if (o == 0 || o[0] == 0)
            continue;
        b = ai_BTypes + (unsigned char)o[0] * 0x24d;
        q = *(char **)(o + 0x61);
        if (q == 0)
            continue;
        k = (unsigned char)b[0];
        if ((k >= 0xb && k <= 0xd) || k == 0x10)
            continue;
        if (ord_GetGridCell(q + 0x4c, &x, &y) == 0)
            continue;
        gv_GetTableRecord((unsigned char)r[0x1c], rec);
        w = (float)*(short *)(r + 0x22) / (float)rec[2] * (float)rec[1];
        w2 = w * 0.5f;
        ORD_CELLS[y + x * 8].halfWeightSum += w2;
        xhi = x + 1 > 8 ? 8 : x + 1;
        yhi = y + 1 > 8 ? 8 : y + 1;
        xlo = x - 1 < 0 ? 0 : x - 1;
        for (i = xlo; i <= xhi; i++) {
            ylo = y - 1 < 0 ? 0 : y - 1;
            for (j = ylo; j <= yhi; j++) {
                ORD_CELLS[j + i * 8].weightSum += w;
                ORD_CELLS[j + i * 8].halfWeightSum += w2;
            }
        }
    }
}

#define ORD_GRID(c) ((char *)ai_Grid + (c) * 0x18)
#define ORD_KIND(k) ((unsigned short *)(ord_KindTable + (k) * 8))
float gm_IncomeRating();                       /* 0x4f9ec0 */
void cm_SendMessage41(void *p);                         /* 0x4c5800 */

/* ord_StatisticsRating as ord_ComputeStatistics calls it: @original the call pushes the record buffer (lea edx,
   [esp+0x48]; push edx; call 0x4f9ce0; add esp, 8 together with ord_WeightedRecordSum's), which the callee does
   not read */
#define ord_StatisticsRatingR ((float (*)(int *))ord_StatisticsRating)

/* 0x4f8ae0, the statistics of the characters and buildings into the 10-dword record out (floats but for the ints
   out[4], out[7], out[8]): counts the characters of kind 0 / 1 and their two buildings (bld / bld2), adds 1.0 to
   the grid cell of each building object (but kinds 0xb .. 0xd and 0x10); sums the kind table values of the 0x100
   building records (kind below 0x44, with an owner) and the building kind bytes +0x247 / +0x248; then normalises
   the grid cells and rates the result (ord_WeightedRecordSum, ord_StatisticsRating, gm_IncomeRating). out[0] the
   income rating, out[1] / out[3] the shares without a bld / bld2, out[2] the combined rating, out[4] the number of
   characters, out[5] the rating, out[7] / out[8] the kind sums, out[9] the overall score @deep Read all of it against the original and rebuilt: the character loop is an index loop (cmp esi, end; jl) with the two buildings unrolled (ORDCHAR gains bld / bld2 at 0x16c / 0x170, block-scoped x, y share the n0 / n1 slots as in the original), the building-kind pointer computed before the object test, ORDBLD gains owner (0x27), out[4] / out[7] / out[8] are ints, the minimum is the kind table's record 1 (0x617470, was 0x44), the volume rating multiplies 1.0 - (f * 0.7f + rating * 0.3), ord_StatisticsRating gets the record buffer (@original push), the grid compares as the original's (!(m > half), ws > 1.0f ? ws : 1.0f). 301 = 301 instructions; diffclass has no code hunk left, only fpu 58 / slots 58 / order 24 and one branch target: ours turns n1 / cntf and n0 / cntf into a shared reciprocal (fld 1.0; fdiv cntf; fild; fmul st(1); ... fstp st(0)) where the original divides twice (fild; fdiv dword ptr [esp+0x18]), which shifts the grid loop head 8 bytes (jne at +0x350: ours to +0x309, the original's +0x301); the final score keeps out[0] in st (fld dword ptr [esi]) where ours re-reads it (fadd dword ptr [esi]). Tried: (float)cnt instead of the cntf local (no reciprocal, but the temp lands in the parameter slot, frame 0x60), inv before out[3] (worse). */
void ord_ComputeStatistics(float *out)
{
    int cnt, n1, n0, i, j, k;
    int sumA, sumC, sumD, s58, s54, s50, s4c;
    float cntf, f, m, sum, w, g, inv;
    ORDCHAR *c;
    ORDBLD *p;
    unsigned char *b;
    unsigned char *bt;
    unsigned short *t;
    int rec[12];

    cnt = 0;
    n1 = 0;
    n0 = 0;
    sumA = 0;
    sumD = 0;
    sumC = 0;
    m = 0;
    sum = 0;
    ord_RebuildGridSums();
    for (i = 0; i < 0x300; i++) {
        c = &ORD_CHARS[i];
        if (c->id == 0xffff || c->kind != 0 && c->kind != 1)
            continue;
        cnt++;
        if (c->bld != 0) {
            int x, y;

            n0++;
            bt = (unsigned char *)ai_BTypes + c->bld->type * 0x24d;
            if (c->bld->object != 0 && (*bt < 0xb || *bt > 0xd) && *bt != 0x10
                && ord_GetGridCell((char *)&c->bld->object->pos, &x, &y) != 0)
                ai_Grid[x][y].buildingCount += 1.0f;
        }
        if (c->bld2 != 0) {
            int x, y;

            n1++;
            bt = (unsigned char *)ai_BTypes + c->bld2->type * 0x24d;
            if (c->bld2->object != 0 && (*bt < 0xb || *bt > 0xd) && *bt != 0x10
                && ord_GetGridCell((char *)&c->bld2->object->pos, &x, &y) != 0)
                ai_Grid[x][y].buildingCount += 1.0f;
        }
    }
    s50 = 0;
    s58 = 0;
    s4c = 0;
    s54 = 0;
    p = (ORDBLD *)ai_Blds;
    for (i = 0; i < 0x100; i++, p++) {
        if (p->type == 0 || p->type >= 0x44 || p->owner == 0xffff)
            continue;
        t = ORD_KIND(p->type);
        b = (unsigned char *)ai_BTypes + p->type * 0x24d;
        sumA += t[0];
        sumC += t[2];
        sumD += t[1];
        if (t[1] > 0) {
            s58 += b[0x247];
            s54 += b[0x248];
        }
        if (t[3] != 0) {
            s50 += b[0x247];
            s4c += b[0x248];
        }
    }
    k = (unsigned short)ORD_KINDS[1].value * 2;
    if (sumA <= k)
        sumA = k;
    cntf = (float)cnt;
    ((int *)out)[7] = sumC;
    ((int *)out)[4] = cnt;
    ((int *)out)[8] = sumA;
    out[5] = cntf / sumA * ((float)s58 / s54 * ((float)s50 / s4c) + 0.5) * sumD;
    out[0] = gm_IncomeRating();
    out[3] = 1.0f - n1 / cntf;
    out[1] = 1.0f - n0 / cntf;
    inv = 1.0f / (cntf + cntf);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (!(m > ai_Grid[i][j].halfWeightSum))
                m = ai_Grid[i][j].halfWeightSum;
            w = ai_Grid[i][j].weightSum > 1.0f ? ai_Grid[i][j].weightSum : 1.0f;
            ai_Grid[i][j].weightSum = w;
            g = inv * ai_Grid[i][j].buildingCount;
            ai_Grid[i][j].buildingCount = g;
            sum += ai_Grid[i][j].halfWeightSum / w * g;
        }
    }
    f = ord_WeightedRecordSum(rec);
    out[2] = (float)((1.0 - (f * 0.7f + ord_StatisticsRatingR(rec) * 0.3)) * sum);
    out[9] = (((1.0f - out[2]) + out[0] + (1.0f - out[1]) + (1.0f - out[3])) * 0.25f
              + (1.0f - out[2]) * out[0] * (1.0f - out[1]) * (1.0f - out[3]) * 2) * 0.33333334f;
}
/* 0x4f8ed0, recomputes the statistics (0x4f8ae0) into the record 0x91b4c0 (10 floats), smooths the rating out[5] into the floats 0x6f1b9c / 0x6f1ba0 (first time: 0x6f1b9c = 0.95 * rating), stores the time stamp and sends the
   message 0x4c5800 with the record, the two floats, the change rate 0x91aaf4 and the clock. ROUGH
   @deep far draft (119 code / 4 fill hunks, 183 of 288 bytes; ROUGH from the layout session). Read against tools/asmdiff 0x4f8ed0: the original fuses the rating smoothing into the FPU stack (fst / fld / fmul QWORD with the 0.95 double constant, the 0.5 factor and the fsub/fmul/fadd chain) where ours reloads the globals; it copies the 10-float record with rep movs before the smoothing (ours after the first-value test), keeps cur / base in edi / edx and builds the 15-int message in place. The statements (ord_ComputeStatistics(st), first = ai_RatingBase == 0.0f, memcpy of the record, the first / else rating update on ai_Rating / ai_RatingBase, base / cur, ord_RatingRate = (cur - base) * 0.0007336182f, the 15-int m build with m[5] / m[6] the two floats, m[10] the rate, the 14-byte time stamp copied to ord_StatisticsClock and into m[11..14], cm_SendMessage41(m)) are the same, but the float / double spellings (0.95f vs 0.95 in the original's fmul QWORD) and the block order were not settled, so the ROUGH marker stays and that is the first lead. Q4 left: 14 raw accesses - the ((float *)m)[5] / ((int *)st)[7] record/message views (needs one named 15-int message view) and the float bits of ai_RatingBase where the F view does not fit. Done here: Q1 name kept, Q2 clean (cm_SendMessage41 for 0x4c5800 = cm.c's 'message 0x41', ord_StatisticsClock for 0x91b470), Q4 down from 23 to 14 and the byte diff 4932 -> 4894 by giving 0x91aaf4 (ord_RatingRate, float) and 0x6f1ba0 (ai_Rating) and the float view of 0x6f1b9c (ai_RatingBase) their own names instead of char-pointer arithmetic. */

void ord_UpdateStatistics(void)
{
    float st[10];
    int first;
    int m[15];
    float cur, base;

    ord_ComputeStatistics(st);
    first = ai_RatingBase == 0.0f;
    memcpy(&ord_Statistics, st, 0x28);
    if (first) {
        ai_Rating = st[5];
        ai_RatingBase = st[5] * 0.95f;
    } else
        ai_Rating = (st[5] - ai_Rating) * 0.5f + ai_Rating;
    base = ai_RatingBase;
    cur = ai_Rating;
    ord_RatingRate = (cur - base) * 0.0007336182f;
    memcpy(m, st, 0x14);
    ((float *)m)[5] = cur;
    ((float *)m)[6] = base;
    m[7] = ((int *)st)[7];
    m[8] = ((int *)st)[8];
    m[9] = ((int *)st)[9];
    m[10] = *(int *)&ord_RatingRate;
    ord_StatisticsClock = ai_Time;
    m[11] = ai_Time.year;
    m[12] = *(int *)&ai_Time.hour;
    m[13] = *(int *)((char *)&ai_Time + 8);
    *(short *)(m + 14) = *(short *)((char *)&ai_Time + 0xc);
    cm_SendMessage41(m);
}
/* 0x4f9020, restores the statistics record from a saved record of 15 words (the fields of ORDSAVE): the five
   columns, ai_Rating / ai_RatingBase, the three values, the rate and the clock stamp; then rebuilds the rating
   bytes 0x91b460[0..4] from the characters' skill bytes +0x80..+0x84 (kind below 10, health above 0) and [5] from
   the weighted level +0xd, and runs ord_ReportStatistics. The inverse of ord_UpdateStatistics (0x4f8ed0).
   @deep readable, 69 asm / 160 B left; allocation only: frame 0x18 vs 0x1c (7 vs 8 slots, the original zeroes a slot at 0x8), push ebx at entry vs after fld p[6], the walk base is health +8 vs kind +2, epilogue order; tried and kept the clock struct copy, a packed 3-int clock view, q = p + 11 base, ORDCHARKIND walk, declaration order; reverted the health pointer; next slotmap / vargen */

void ord_LoadStatistics(void *p)
{
    ORDSAVE *s = p;
    AITIME *clk;
    ORDCHAR *c;
    ORDCHARKIND *k;
    float rating, base;
    int sum[5];
    int n, w, i;

    ord_Statistics.col[0] = s->col[0];
    ord_Statistics.col[1] = s->col[1];
    ord_Statistics.col[2] = s->col[2];
    ord_Statistics.col[3] = s->col[3];
    ord_Statistics.col[4] = s->col[4];
    rating = s->rating;
    ord_Statistics.rating = rating;
    base = s->base;
    ord_Statistics.base = base;
    ord_Statistics.v1c = s->v1c;
    ord_Statistics.v20 = s->v20;
    ord_Statistics.score = s->score;
    ord_RatingRate = s->rate;
    clk = &s->clock;
    ord_StatisticsClock = *clk;
    ai_RatingBase = base;
    ai_Rating = rating;
    n = 0;
    w = 0;
    for (i = 0; i < 5; i++)
        sum[i] = 0;
    for (k = ORD_CHARKINDS; k < ORD_CHARKINDS + 768; k++) {
        c = (ORDCHAR *)((char *)k - 2);
        if (c->id != 0xffff && k->kind < 10 && k->health > 0) {
            n++;
            w += k->level * 5;
            for (i = 0; i < 5; i++)
                sum[i] += k->stats[i];
        }
    }
    for (i = 0; i < 5; i++)
        ord_RatingBytes[i] = (char)(int)((double)sum[i] / (double)n);
    ord_RatingBytes[5] = (char)(int)((double)w / (double)n);
    ord_ReportStatistics();
}

typedef struct ORDINITMSG {             /* the message 0x4c5e80 payload: the record and its tag at +0x24 */
    ORDMSGREC record;                   /* 0x00 */
    int tag;                            /* 0x24, 'init' / 'set ' */
} ORDINITMSG;

/* 0x4f9170, copies the 9 ints of the record 0x91b480 (the clear-able table of 0x4f82a0) to out */
void ord_GetMessageRecord(int *out)
{
    memcpy(out, &ord_MessageRecord, 0x24);
}
/* 0x4f9190, sends the message 0x4c5e80 built from the record 0x91b480 with its fields replaced by
   (a, 0, ai_Time, b, c, 0, d, 0, 0) and the tag 'init' at +0x24: the first frame of a game */

void ord_InitMessageRecord(int a, float b, float c, float d)
{
    ORDINITMSG m;

    ORD_COPY(&m, offset ord_MessageRecord, 0x24);
    m.tag = 0x696e6974;
    m.record.enabled = a;
    m.record.count = 0;
    m.record.time = ai_Time.year;
    m.record.base = b;
    m.record.scale = c;
    m.record.value = 0.0f;
    m.record.limit = d;
    m.record.phase = 0;
    m.record.stage = 0;
    cm_SendMessage56(&m);
}

int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */

/* the record 0x91b480 as its ORDMSGREC */

#pragma pack(push, 1)
typedef struct ORDLONGMSG {             /* 0xf8 bytes, the long message of cm_SendLongMessage as 0x4f9220 fills it */
    char pad00[4];
    unsigned char kind;                 /* 0x04, the event: 0x59 / 0x4e / 0x50 */
    char pad05[3];
    int netId;                          /* 0x08, the character's net id */
    int target;                         /* 0x0c, -1 = none */
    char pad10[0x18];
    AITIME time;                        /* 0x28, when: the game clock, moved on by gm_AddTime */
    unsigned char mode;                 /* 0x36 */
    char pad37[0x21];
    int value;                          /* 0x58, -1 or a random amount */
    char pad5c[0x40];
    int limit;                          /* 0x9c, 1000 */
    char padA0[0x58];
} ORDLONGMSG;
#pragma pack(pop)

/* 0x4f9220, advances the message event record 0x91b480 (ORDMSGREC): disabled it does nothing; idle (phase 0) it
   adds random * scale + base to the accumulator and, above the limit, counts the message, timestamps the record,
   picks the next phase from a 16-entry table chosen by the stage (stage 0 goes straight to phase 3, stage 4 resets
   the stage), sends the long message 0x59 / 0x4e / 0x50 to the player (character ai_PlayerChar, gm_AddTime later)
   and finally the whole record (message 0x4c5e80, tag 'set '); running, it returns while he_FindEvent (1, 0, the
   phase's event) exists, else clears the phase and the accumulator. @deep Read all of it against the original and rebuilt: typed ORDMSGREC record (ord_MessageRecord), the running branch tests the global's phase with one he_FindEvent call per phase (cross-jumped as the original's push 0x59/0x4e/0x50), the stage tables are three 16-int initializers (the original's hoisted common stores tbl[0,1,7,8,14,15] then the five-store runs match instruction for instruction), the long message is an ORDLONGMSG (kind 0x04, netId 0x08, target 0x0c, clock 0x28, mode 0x36, value 0x58, limit 0x9c, the int at 0x2e cleared in phases 1/2), the hour tests read ai_Time's hour word (cmp ax, 0x17), phases 2/3 test hour >= 0x17 first as the original. 350 vs 338 instructions, the 12 are VC6 cross-jumping: the original merges the four gm_AddTime calls of phases 1 and 3 into one (phase 1's plain path: lea ecx,[ebp-0x140]; push eax; push ecx; jmp 0x4f968e, its +7 path jmp 0x4f9686), ours keeps phase 1's two calls and the push &msg / cm_SendLongMessage tails apart; that follows from the per-phase store schedule and register choice (phase 3: hour in eax / -1 in ecx in the original, ours hour in ecx / -1 in eax; phase 2: the -1 stores before the netId store), which reordering the phase-1 statements did not change. */
void ord_UpdateMessageEvent(void)
{
    ORDMSGREC r;
    ORDINITMSG out;
    ORDLONGMSG msg;

    if (ord_MessageRecord.enabled != 1)
        return;
    r = ord_MessageRecord;
    if (ord_MessageRecord.phase > 0) {
        if (ord_MessageRecord.phase == 1) {
            if (he_FindEvent(1, 0, 0x59) != 0)
                return;
        } else if (ord_MessageRecord.phase == 2) {
            if (he_FindEvent(1, 0, 0x4e) != 0)
                return;
        } else if (ord_MessageRecord.phase == 3) {
            if (he_FindEvent(1, 0, 0x50) != 0)
                return;
        }
        r.phase = 0;
        r.value = 0.0f;
    } else if ((r.value = util_RandomF() * r.scale + r.base + r.value) > r.limit) {
        memset(&msg, 0, sizeof(msg));
        r.count++;
        r.time = ai_Time.year;
        if (r.stage >= 4) {
            r.stage = 0;
            r.phase = 3;
        } else if (r.stage == 0) {
            r.phase = 3;
        } else if (r.stage == 1) {
            int t[16] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2 };

            r.phase = t[util_RandomU(0x10)] + 1;
        } else if (r.stage == 2) {
            int t[16] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2 };

            r.phase = t[util_RandomU(0x10)] + 1;
        } else {
            int t[16] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2 };

            r.phase = t[util_RandomU(0x10)] + 1;
        }
        if (r.phase == 1) {
            msg.kind = 0x59;
            msg.netId = ORD_CHAR(ai_PlayerChar)->netId;
            msg.target = -1;
            msg.time = ai_Time;
            msg.mode = 2;
            msg.value = util_RandomU(3) * 3;
            msg.time = ai_Time;
            msg.time.min = 0;
            if (ai_Time.hour < 0x17 && ai_Time.hour >= 7)
                gm_AddTime((struct AITIME *)&msg.time, util_RandomU(4), util_RandomU(2) * 0x1e, 0);
            else
                gm_AddTime((struct AITIME *)&msg.time, util_RandomU(4) + 7, util_RandomU(2) * 0x1e, 0);
            cm_SendLongMessage((char *)&msg);
        } else if (r.phase == 2) {
            msg.kind = 0x4e;
            msg.netId = ORD_CHAR(ai_PlayerChar)->netId;
            msg.target = -1;
            msg.value = -1;
            msg.time = ai_Time;
            msg.mode = 1;
            msg.limit = 1000;
            msg.time.min = 0;
            if (ai_Time.hour >= 0x17)
                gm_AddTime((struct AITIME *)&msg.time, util_RandomU(4) + 7, util_RandomU(2) * 0x1e, 0);
            else
                gm_AddTime((struct AITIME *)&msg.time, util_RandomU(4), util_RandomU(2) * 0x1e, 0);
            cm_SendLongMessage((char *)&msg);
        } else if (r.phase == 3) {
            msg.kind = 0x50;
            msg.netId = ORD_CHAR(ai_PlayerChar)->netId;
            msg.time = ai_Time;
            msg.target = -1;
            msg.mode = 2;
            msg.value = -1;
            msg.limit = 1000;
            if (ai_Time.hour >= 0x17)
                gm_AddTime((struct AITIME *)&msg.time, util_RandomU(4) + 7, util_RandomU(2) * 0x1e, 0);
            else
                gm_AddTime((struct AITIME *)&msg.time, util_RandomU(4), util_RandomU(2) * 0x1e, 0);
            cm_SendLongMessage((char *)&msg);
        }
        r.stage = r.phase;
    }
    ORD_COPY3(&out, &r, 0x24);
    out.tag = 0x73657420;
    cm_SendMessage56(&out);
}
/* 0x4f9850, ord_ReportStatistics, in a game year >= 8 divisible by 4: averages the last four rounds of the
   statistics history (ord_StatisticsHistory[12..15]) into a = the share without the second object, b = the share
   without the first object, c = the combined score and d = the overall score; rates a / b / c against 0.15 (the
   texts 0x1c11 .. 0x1c14 when the share is low, 0x1c15 .. 0x1c18 when it is high) and the income rating of
   gm_IncomeRating (0x4f9ec0) against 0 (0x1c13 / 0x1c17). The lowest m of a / b / c and the rating above 0.2
   picks the tier phrase, k being the last column greater than the minimum so far (the original's test, not the
   column of the minimum): below 0.3 the base text id ids[k], below
   0.5 ids[k + 2], else ids[k + 4], formatted as ids[x] and ids[x] + 1 with "%s%s"; the four base ids are 0x1bf5 /
   0x1bfb / 0x1c01 / 0x1c07. d above 0.55 formats the sentence 0x1c0d (ending 0x1c0e), below 0.45 the sentence
   0x1c0f (ending 0x1c10), and sends it (flag 1) and then the tier phrase to every player character (kind 6, its
   netId) through he_ShowNews (0x53dda0, event 0x5f8); with a d in between the tier phrase goes out alone. */

void ord_ReportStatistics(void)
{
    float a, b, c, d, v, m;
    int txt0, txt1, txt2, txt3, k, i;
    int ids[4];                         /* the base text id of the four columns; the higher tiers are the entries
                                           2 / 4 further, as the original indexes them (the tie k on a 4-element
                                           table is the original's own indexing, kept for the byte check) */
    char sentence[0x200];
    char tierText[0x200];
    int any;

    if (ai_Time.year < 8 || ai_Time.year % 4 != 0)
        return;
    a = (ord_StatisticsHistory[15].noSecond + ord_StatisticsHistory[14].noSecond
         + ord_StatisticsHistory[13].noSecond + ord_StatisticsHistory[12].noSecond) * 0.25f;
    b = (ord_StatisticsHistory[15].noFirst + ord_StatisticsHistory[14].noFirst
         + ord_StatisticsHistory[13].noFirst + ord_StatisticsHistory[12].noFirst) * 0.25f;
    c = (ord_StatisticsHistory[15].combined + ord_StatisticsHistory[14].combined
         + ord_StatisticsHistory[13].combined + ord_StatisticsHistory[12].combined) * 0.25f;
    d = (ord_StatisticsHistory[15].score + ord_StatisticsHistory[14].score
         + ord_StatisticsHistory[13].score + ord_StatisticsHistory[12].score) * 0.25f;
    v = gm_IncomeRating();
    txt0 = a <= 0.15 ? 0x1c11 : 0x1c15;
    txt1 = b <= 0.15 ? 0x1c12 : 0x1c16;
    txt2 = v >= 0.0 ? 0x1c13 : 0x1c17;
    txt3 = c <= 0.15 ? 0x1c14 : 0x1c18;
    k = 0;
    any = 0;
    m = a;
    if (b > m)
        k = 1;
    m = m < b ? m : b;
    if (v > m)
        k = 2;
    m = m < v ? m : v;
    if (c > m)
        k = 3;
    m = m < c ? m : c;
    if (m > 0.2) {
        ids[0] = 0x1bf5;
        ids[1] = 0x1bfb;
        ids[2] = 0x1c01;
        ids[3] = 0x1c07;
        if (m < 0.3)
            k = ids[k];
        else if (m < 0.5)
            k = ids[k + 2];
        else
            k = ids[k + 4];
        txt_Format(tierText, "%s%s", k, k + 1);
        any = 1;
    }
    if (d > 0.55) {
        txt_Format(sentence, 0x1c0d, txt0, txt1, txt2, txt3, 0x1c0e);
        for (i = 0; i < 768; i++)
            if (ORD_CHARS[i].kind == 6
                && (he_ShowNews(ORD_CHARS[i].netId, -1, sentence, 1, 0x5f8, 0), any))
                he_ShowNews(ORD_CHARS[i].netId, -1, tierText, 0, 0x5f8, 0);
        return;
    }
    if (d < 0.45) {
        txt_Format(sentence, 0x1c0f, txt0, txt1, txt2, txt3, 0x1c10);
        for (i = 0; i < 768; i++)
            if (ORD_CHARS[i].kind == 6
                && (he_ShowNews(ORD_CHARS[i].netId, -1, sentence, 1, 0x5f8, 0), any))
                he_ShowNews(ORD_CHARS[i].netId, -1, tierText, 0, 0x5f8, 0);
        return;
    }
    if (any) {
        for (i = 0; i < 768; i++)
            if (ORD_CHARS[i].kind == 6)
                he_ShowNews(ORD_CHARS[i].netId, -1, tierText, 0, 0x5f8, 0);
    }
}
/* 0x4f9710, the handler of the message 0x4c5e80 sends (0x4f9190): the tag at +0x24 'init' / 'set ' (stored
   byte-swapped, as the message carries it) copies the 9 ints to the record 0x91b480 (1); 'dsbl' / 'enbl' clear /
   set its first int (1); anything else 0 */

int ord_HandleMessageRecord(int *m)
{
    switch ((unsigned)m[9]) {
    case 0x656e626c:                    /* 'enbl' */
        ord_MessageRecord.enabled = 1;
        break;
    case 0x6473626c:                    /* 'dsbl' */
        ord_MessageRecord.enabled = 0;
        break;
    case 0x696e6974:                    /* 'init' */
    case 0x73657420:                    /* 'set ' */
        ORD_COPY_AT(offset ord_MessageRecord, m, 0x24);
        break;
    default:
        return 0;
    }
    return 1;
}
/* 0x4f97a0, byte i (below 5) of 0x91b460, 0x7e for others */

unsigned char ord_GetRatingByte(int i)
{
    if (i >= 5)
        return 0x7e;
    return ord_RatingBytes[i];
}
/* 0x4f97c0, the ratio of the sums of the bytes +0x247 and +0x248 of the building records (ai_BTypes, 0x24d bytes per kind) of the 32 slots (0xa9 bytes each, the byte +0xa9 of the table 0x889f74 points to = the kind, 0 = free); the first sum over the second, 0 when the second is 0 */

float ord_BuildingCapacityRatio(void)
{
    int a;
    int b;
    int i;
    char *p;
    char *r;

    a = 0;
    b = 0;
    p = ai_Blds + 0xa9;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0) {
            r = ai_BTypes + (unsigned char)*p * 0x24d;
            a += (unsigned char)r[0x247];
            b += (unsigned char)r[0x248];
        }
        p += 0xa9;
    }
    if (b != 0)
        return (float)a / b;
    return 0.0f;
}
/* 0x4f9ce0, a rating from the table records 0 and 1 of 0x4f2720 (their int +0x18): (4 - rec0[6]) * 0.25 * 0.75 + (1 - rec1[6]) * 0.25 */

float ord_StatisticsRating(void)
{
    int ra[9];
    int rb[9];

    gv_GetTableRecord(0, ra);
    gv_GetTableRecord(1, rb);
    return (float)(4 - ra[6]) * 0.25f * 0.75 + (1 - rb[6]) * 0.25;
}
/* 0x4f9d40, the weighted sum of the +0x18 ints (rec[6]) of the twelve table records 0x11 .. 0x1c of gv_GetTableRecord (0x4f2720) with the weights 0.06 0.05 0.08 0.04 0.13 0.1 0.13 0.12 0.14 0.15 and two zeros (the array is twelve long like the record range); the ints themselves are stored to out[0 .. 11] @deep meaning read and settled against tools/disasm 0x4f9d40; 40 asm / 172 B -> 19 asm / 45 B this session. Fixed: the weight table is one initializer (float w[12] = {twelve values}) - the original zero-fills w[10] / w[11] through eax when the zeros are the initializer's implicit tail, and stores the ten explicit weights as immediates; sum is declared and initialized before the table (sum = 0 as an immediate at [esp+0xc], not the register store the later statement gave). The frame (0x58), the rec[6] / w[i - 0x11] addressing, the gv_GetTableRecord(i, rec) call and the fild/fmul/fadd/fstp accumulation all match. Tried and reverted: the sum statement after the table (33 asm / 114 B), the w[10]/w[11] statements (the register-form zero stores disappear), the two body statements swapped (WORSE, 43 asm / 68 B). Left: one hunk, the loop's register and schedule - ours keeps i in esi and &w in edi (mov esi,0x11 / lea edi,[esp+0x34], fild after the store, fmul [edi-4]) where the original has i in edi and &w in esi (mov edi,0x11 / lea esi,[esp+0x34], fild before the out store, fmul [esi]); that is the esi/edi allocation plus the fild / fmul / add / inc order = match-deep. Readability Q1-Q7 pass. */

float ord_WeightedRecordSum(int *out)
{
    float sum = 0;
    float w[12] = {0.06f, 0.05f, 0.08f, 0.04f, 0.13f, 0.1f, 0.13f, 0.12f, 0.14f, 0.15f};
    int rec[9];
    int i;

    for (i = 0x11; i < 0x1d; i++) {
        gv_GetTableRecord(i, rec);
        out[i - 0x11] = rec[6];
        sum += (float)rec[6] * w[i - 0x11];
    }
    return sum;
}
/* 0x4f9e00, copies the last statistics record (ord_Statistics, ten floats) to o, overwrites out[6] with the float
   bits of ord_Statistics's base field's live value (ai_RatingBase) and returns out[9] (the score); 0 for no o */

float ord_GetStatistics(int *o)
{
    if (o == 0)
        return ai_FloatZero;
    memcpy(o, &ord_Statistics, 0x28);
    o[6] = ORD_RATINGBASE_BITS;
    return ((ORDSTATS *)o)->score;
}
/* 0x4f9c00, advances the float 0x6f1b9c by the time since the stamp 0x91b470 (14 bytes, the layout of the clock 0x770ae0): 0x4fe280 (stamp, clock) gives the elapsed steps k; no step: the stamp is set to the clock; otherwise, when the float 0x6f1ba0 is not 0 and the clock's hour (word at +4) is between 7 and 0x14, 0x6f1b9c += k * the float 0x91aaf4, clamped to 0x6f1ba0 * 0.25, and the stamp = the clock */

void ord_AdvanceRatingBase(void)
{
    int k;
    double lim;

    k = gm_MinutesBetween(&ord_StatisticsClock, &ai_Time);  /* @original: gv.h's shared K&R declaration (the call
         pushes the two clock pointers, 0x4f9c01 push 0x770ae0 / 0x4f9c06 push 0x91b470); a prototype in ord.c
         is a lint cross-TU conflict with he.c / sv.c / gv.c, which call it with an int address literal */
    if (k == 0) {
        ord_StatisticsClock = ai_Time;
        return;
    }
    if (ai_Rating != 0 && ai_Time.hour > 6
        && ai_Time.hour < 0x15) {
        ai_RatingBase = k * ord_RatingRate + ai_RatingBase;
        lim = ai_Rating * 0.25;
        if (ai_RatingBase < lim)
            ai_RatingBase = (float)lim;
        ord_StatisticsClock = ai_Time;
    }
}
