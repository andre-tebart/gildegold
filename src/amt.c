/* amt.c -- the offices ("Aemter": the office table 0x925120 and its code; name from the tags "amt_calc_zuenfte()" 0x60f11c and
   "amt_fio_LoadAemter" 0x60f174), starts at 0x4e5cd0 (the init of the table; the cut_ code of cm.c ends with the debug dump 0x4e5bf0).
   The unit's .data starts at 0x60ec90 (a float table, first user 0x4e60b0). The end at 0x4eaa50, where ahm.c's
   building code and .data start, rests on subject and .data order only. Names are ours.
   Open: AIMSG (ai.h) has no view of the 0xd build message's pos / angles / name (0x80 / 0x90 / 0x9c); amt_CalcZuenfte
   and am.c's amt_ExecGebaeudeBauen both build it with raw casts, so the typed view is an ai.h + am.c decision.
   unit of rea 8cef7d5b (src/game/amt.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "amt.h"
#include "data_views.h"

/* full rebuild: the symbols whose original addresses the code above spells literally */
extern int gm_PlayerRecords[688];

#define AMT_TABA_END ((int)&amt_TabA[15 * 10])  /* 0x5edee8, the end of the 15 rows of amt_TabA */
#define AMT_MONEY_UNIT 64  /* money in the character messages counts in multiples of this: the amounts of the 0x4c4830 sites are built as n * 0x40 */

#include "time_view.h"
#include "ahm.h"
#include "ai.h"
#include "cm.h"
#include "f3gm.h"
#include "gv.h"
#include "he.h"
#include "io_text.h"
#include "ls.h"
#include "sv.h"
#include "tr.h"
#include "unk_4a7140.h"
#include "unk_58bbd0.h"
#include "vfs.h"
#include "ch.h"
#include "d3_interface.h"
#include "err.h"
#include "unk_458810.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */

/* 0x4e5cd0, clears the office table (42 records of 0x18 bytes at 0x925120) and numbers them: +0 the index, +4 and +0x14 -1, +8 the kind of the office
   (0 .. 1 the first two, then 2, 3, 4, 4, 5, 6, 7, 7, and i - 2 for the records 10 .. 41); the table loader 0x4ea7a0 calls it first */

void amt_InitTable(void)
{
    int i;

    AI_ZERO_AT(offset amt_Table, 0x3f0);
    for (i = 0; i < 42; i++) {
        amt_Table[i].idx = i;
        amt_Table[i].owner = -1;
        amt_Table[i].x14 = -1;
    }
    amt_Table[2].kind = 2;
    amt_Table[0].kind = 1;
    amt_Table[1].kind = 1;
    amt_Table[3].kind = 3;
    amt_Table[4].kind = 4;
    amt_Table[5].kind = 4;
    amt_Table[6].kind = 5;
    amt_Table[7].kind = 6;
    amt_Table[8].kind = 7;
    amt_Table[9].kind = 7;
    amt_Table[10].kind = 8;
    amt_Table[11].kind = 9;
    amt_Table[12].kind = 10;
    amt_Table[13].kind = 11;
    amt_Table[14].kind = 12;
    amt_Table[15].kind = 13;
    amt_Table[16].kind = 14;
    amt_Table[17].kind = 15;
    amt_Table[18].kind = 16;
    amt_Table[19].kind = 17;
    amt_Table[20].kind = 18;
    amt_Table[21].kind = 19;
    amt_Table[22].kind = 20;
    amt_Table[23].kind = 21;
    amt_Table[24].kind = 22;
    amt_Table[25].kind = 23;
    amt_Table[26].kind = 24;
    amt_Table[27].kind = 25;
    amt_Table[28].kind = 26;
    amt_Table[29].kind = 27;
    amt_Table[30].kind = 28;
    amt_Table[31].kind = 29;
    amt_Table[32].kind = 30;
    amt_Table[33].kind = 31;
    amt_Table[34].kind = 32;
    amt_Table[35].kind = 33;
    amt_Table[36].kind = 34;
    amt_Table[37].kind = 35;
    amt_Table[38].kind = 36;
    amt_Table[39].kind = 37;
    amt_Table[40].kind = 38;
    amt_Table[41].kind = 39;
}
/* 0x4e5e50, the rating of the character a (+0x164 / +0x166 the office rank and party bytes) as seen by b: the float tables 0x5edc90 (15 rows of 10: an int id, 9
   columns) and 0x5edef0 (5 rows, an int status key, 9 columns) by the rank (amt_KindColumn, 1 .. 6) and the status bits +0x1cc (0x400 .. 0x2000, 0x200 give the keys
   0x24 .. 0x27, 0x23); mode (>= 2: plus the absolute value of the entry, >= 3: plus 10.0); with flag the friendship value of amt_GetKind / 0x4f2720 is taken off
   (a = b's party: added, mode 2: times 0.75) @deep Read whole against the original and fixed: the table ids are compared as ints (mov ecx,[esi] / cmp eax,ecx; the TabA search calls gm_TitleGroup per row and compares it with AMT_BITS), both table searches are for loops with the hit's body inside and a break (a miss leaves r untouched, the old code read the row after the end), a's status test is & 0x3c00 then the 0x200 key (the old 0x3e00 mask came from a merged condition). Left, unchanged by goto / break-then-test / assignment-in-condition variants: VC6 folds our akey (= 0x200 ? 0x23 : 0) into the 0x3c00 test (ours test ah,0x3e / jne, the original test ch,0x3c / jne then and/neg/sbb/and 0x23 stored to [esp+0x54], a's dead home slot), places the TabA hit block after the status chain (je at +0x4f: ours to +0x73, the original's to +0x5f), and keeps r in a frame slot of its own (frame 0x44 vs 0x40; the original keeps r in b's home slot [esp+0x58]) with the key in ecx where the original has eax. */

float amt_Rating(char *a, char *b, int mode, int flag)
{
    float r, fl;
    char buf[0x40];
    int k, i, key, akey, n;
    unsigned int f2;

    r = 0.0f;
    k = amt_KindColumn((unsigned char)b[0x166]);
    if (k > 0 && k < 7) {
        for (i = 0; i < 15; i++) {
            if (gm_TitleGroup((unsigned char)a[0x164]) == AMT_BITS(&amt_TabA[i * 10])) {
                r = amt_TabA[i * 10 + k + 1];
                if ((unsigned int)mode >= 2)
                    r = r + (float)abs((int)amt_TabA[i * 10 + k + 1]);
                break;
            }
        }
    }
    f2 = AMT_STATUS(b);
    if (f2 & 0x1000)
        key = 0x26;
    else if (f2 & 0x2000)
        key = 0x27;
    else if (f2 & 0x800)
        key = 0x25;
    else if (f2 & 0x400)
        key = 0x24;
    else
        key = (f2 & 0x200) ? 0x23 : 0;
    if (key != 0) {
        f2 = AMT_STATUS(a);
        if ((f2 & 0x3c00) == 0) {
            /* a's own status key (only 0x23 is left possible): a without status gets b's from amt_TabB */
            akey = (f2 & 0x200) ? 0x23 : 0;
            if (akey == 0) {
                for (n = 0; n < 5; n++) {
                    if (AMT_BITS(&amt_TabB[n * 10]) == key) {
                        r = r + amt_TabB[n * 10 + amt_KindColumn((unsigned char)b[0x166]) + 1];
                        break;
                    }
                }
            }
        }
        if ((unsigned int)mode >= 3)
            r = r + 10.0f;
    }
    if (flag != 0) {
        amt_GetKind((unsigned char)b[0x166], (AMTKIND *)buf);
        gv_GetTableRecord(2, buf + 0x20);
        fl = ((AMTKINDF *)buf)->value;
        if (mode == 2)
            fl = fl * 0.75f;
        if (a[0xc] == b[0xc])
            return fl + r;
        if (mode == 0)
            return r - fl;
    }
    return r;
}
/* 0x4e6030, the office of the character c (the table 0x925120, first 30 records, found by the id +4): copies the record to *office and the record of its kind
   (0x60ed58, 0x18 bytes) to *kind; 1 when found (c exists, +0x166 non-zero)
   @deep B3 80 code hunks + 3 fill, 55 differing of 50/49 instructions; the whole original disassembly (0x4e6030..0x4e60a8) was read against the source: same statement sequence (guard on c / id 0xffff / c[0x166], 30-record loop on amt_Table[i].owner, copy office then amt_Kinds[kind], return 1/0); the differences are the registers from instruction 0 (original c=ecx, i=edx, netId=esi, office/kind=edi, eax=return; ours c=ecx, byte=dl, ...) - allocation only. spell.py dup-tail: duplicate return 0 in the if and at the end, merged single return, early-return guard/polarity swap tried, one edit one build each, same 89/128 bytes and 55 lines every time. Readability Q1-Q7 pass (AMTCHARVIEW id/netId fields); the amt_FindOffice alias spellings in am.c/cm.c/gv.c/he.c/unk_58bbd0.c are the older type views (G5CH* /void*), merging them needs call-site casts (recorded lead) */

int amt_FindOffice(char *c, AMTKIND *kind, AMTREC *office)
{
    int i;

    if (c == 0 || ((AMTCHARVIEW *)c)->id == 0xffff || c[0x166] == 0)
        return 0;
    for (i = 0; i < 30; i++) {
        if (((AMTCHARVIEW *)c)->netId == amt_Table[i].owner) {
            *office = amt_Table[i];
            *kind = amt_Kinds[amt_Table[i].kind];
            return 1;
        }
    }
    return 0;
}
/* 0x4e60b0, the rate (the float table 0x60ec90, 7 per row) of the office x for the character c (+0x166 his rank, +0x167 the next one): 1 and *out when the
   level of x is within one above the level of +0x167 and above that of +0x166 */

int amt_GetOfficeRate(char *c, int x, float *out)
{
    if (c != 0 && ((AMTCHARVIEW *)c)->id != 0xffff && x != 0 && x < 0x1e && (unsigned char)c[0x166] < 0x1e && (unsigned char)c[0x167] < 0x1e) {
        if (amt_Kinds[(unsigned char)c[0x167]].level + 1 >= amt_Kinds[x].level && amt_Kinds[(unsigned char)c[0x166]].level < amt_Kinds[x].level) {
            *out = amt_Rates[amt_Kinds[(unsigned char)c[0x166]].col * 7 + amt_Kinds[x].col];
            return 1;
        }
    }
    return 0;
}
/* 0x4e6180, applies character c for an office of the kind `kind` (the first of the 30 offices with that kind, a second one for the kinds that take two): when an office
   is open (owner -1, state 3, fewer than 4 applicants) sends the application (cm_SendApplication) between the "Amtsbewerbung" group
   brackets (cm_BeginGroup / cm_EndGroup); -1 when not. The search walks the kind byte of the office records (0x18 apart, the original's
   pointer induction, hence the do-while: a while is rotated and peels the first test); the two-person kinds then repeat it for the second
   office (f1: the first office took it, f2/f1 together: an office was open) and send the second office's index as m.o2. The final checks
   repeat the open test of the plain kind.
   @deep B3 61 differing instructions of 165: the whole original 0x4e6180..0x4e635f was read against the source and the statements match (guard c[0x168]; kind search over 30 records; kind record copy; the two-person second search with f1/f2; the three send paths; the plain-path open checks) - the rest is register allocation and basic-block order (the original keeps c in ecx and re-reads [esp+0x34] before m.id, re-reads [edx+0x4] / [eax+0x4] at the send sites, hoists the bracket push above the m stores and merges the argument cleanup into one add esp,0xc; the two 0xff stores go through cl) and those were tried one edit one build each: kind-pointer and record-pointer search loops (do-while(1) avoids the first-test peel, 100 -> 61 asm lines), i >= 30 and cnt >= 4 spellings, m.o2 = m.o1 / m.o1 = m.o2 / chained 0xff stores, if (f2) arm polarity, goto-found layout, bracket call before the stores (worse, reverted); spell.py: reload [esp+0x34] / [edx+0x4] / [eax+0x4] and dup-tail, no local holds those values (the CSE is of the parameter and the netId expression). Readability Q1-Q7 pass (the three cm callees named cm_BeginGroup / cm_EndGroup / cm_SendApplication, the latter now int). */

int amt_ApplyForOffice(char *c, unsigned int kind)
{
    unsigned char *p;
    int i, f1, f2;
    AMTAPP m;
    AMTKIND k;

    if (c[0x168] != 0)
        return -1;
    i = 0;
    p = &amt_Table[0].kind;
    do {
        if (*p == kind)
            break;
        p += sizeof(AMTREC);
        i++;
        if ((int)p >= (int)&amt_Table[30].kind)
            return -1;
    } while (1);
    if (i >= 30)
        return -1;
    m.kind = (unsigned char)kind;
    k = amt_Kinds[amt_Table[i].kind];
    m.o1 = 0xff;
    m.o2 = 0xff;
    if (k.two != 0) {
        f2 = 0;
        f1 = 0;
        if (amt_Table[i].owner == -1 && amt_Table[i].state == 3 && amt_Table[i].cnt < 4) {
            m.o1 = amt_Table[i].idx;
            f2 = 1;
            m.id = ((AMTCHARVIEW *)c)->netId;
            f1 = 1;
        }
        i++;
        if (i < 30) {
            do {
                if (amt_Table[i].kind == kind) {
                    if (i < 30 && amt_Table[i].owner == -1 && amt_Table[i].state == 3 && amt_Table[i].cnt < 4) {
                        if (!f2) {
                            m.o1 = amt_Table[i].idx;
                            m.id = ((AMTCHARVIEW *)c)->netId;
                            cm_BeginGroup("Amtsbewerbung");
                            goto send;
                        }
                        m.o2 = amt_Table[i].idx;
                    }
                    break;
                }
                i++;
            } while (i < 30);
        }
        if (f1) {
            cm_BeginGroup("Amtsbewerbung");
            goto send;
        }
    }
    if (amt_Table[i].owner != -1)
        return -1;
    if (amt_Table[i].state != 3)
        return -1;
    if (amt_Table[i].cnt >= 4)
        return -1;
    m.o1 = amt_Table[i].idx;
    m.id = ((AMTCHARVIEW *)c)->netId;
    cm_BeginGroup("Amtsbewerbung");
send:
    i = cm_SendApplication(&m);
    cm_EndGroup("Amtsbewerbung");
    return i;
}
/* 0x4e6360, checks the application m (AMTAPP: +0 the character, +4 the first office, +5 the second or 0xff, +6 the kind): the character exists
   and has not applied yet, the office m.o1 (and m.o2, when not 0xff) is one of the first 30 records, open (owner -1, state 3, fewer than 4
   applicants) and of that kind; 1 when it may be executed (amt_AcceptApplication) */

int amt_CheckApplication(AMTAPP *m)
{
    char *c;
    AMTREC *p;
    int i, j;

    c = gm_FindChar(m->id);
    if (c != 0 && c[0x168] == 0) {
        i = 0;
        p = amt_Table;
        do {
            if (p->idx == m->o1)
                break;
            p++;
            i++;
            if ((int)p >= (int)&amt_Table[30])
                return 0;
        } while (1);
        if (i >= 30)
            return 0;
        if (amt_Table[i].owner != -1)
            return 0;
        if (amt_Table[i].state != 3)
            return 0;
        if (amt_Table[i].cnt >= 4)
            return 0;
        if (m->kind != amt_Table[i].kind)
            return 0;
        if (m->o2 != 0xff) {
            j = 0;
            p = amt_Table;
            do {
                if (p->idx == m->o2)
                    break;
                p++;
                j++;
                if ((int)p >= (int)&amt_Table[30])
                    return 0;
            } while (1);
            if (j >= 30)
                return 0;
            if (amt_Table[i].kind != amt_Table[j].kind)
                return 0;
            if (amt_Table[j].owner != -1)
                return 0;
            if (amt_Table[j].state != 3)
                return 0;
            if (amt_Table[j].cnt >= 4)
                return 0;
        }
        return 1;
    }
    return 0;
}
/* 0x4e6430, executes the application m (AMTAPP, see amt_CheckApplication): the character takes the kind in its +0x168 and the office
   (the two offices of a two-person kind) counts one applicant more; 0 when the character is unknown / 0xffff / has already applied, the
   office record or its kind record contradicts the message (kind id, two-person flag) or an office is not open
   @deep B3 45 differing instructions of 133 (raw 81 code-hunk instructions): the whole original 0x4e6430..0x4e65d8 was read against the source and every statement matches (the character guard 0/0xffff/+0x168, the two 30-record index searches with the 0x9253f0 bound, the kind byte local, the one-office and two-office paths with the k id / two-person checks and the cnt >= 4 bounds, the +0x168 store and the two cnt++); the rest is the prologue (the original pushes edi, ours uses esi and saves edi only around the k copy), the kind local slot (ours [esp+0x28], the original [esp+0x2c], the same slot under the extra push) and the register roles of the -1 / 4 compare values and the k.id reload - allocation, no menu rule. Tried one edit one build each: the search loops as do-while(1) with the named 30-record bound (156 -> 115 code hunks), i >= 30, cnt >= 4, the o2 != 0xff then-arm layout so the single-office path is the out-of-line else (115 -> 81), Q4 via AMTCHARVIEW.id. spell.py: slots 0x28/0x2c and reload [esp+0x20] (k.two) - both read the prologue's push edi. Readability Q1-Q7 pass (gm_FindChar, AMTCHARVIEW.id, amt_CheckApplication). */

int amt_AcceptApplication(AMTAPP *m)
{
    char *c;
    AMTREC *p;
    AMTKIND k;
    int i, j;
    unsigned char kind;

    c = gm_FindChar(m->id);
    if (c != 0 && ((AMTCHARVIEW *)c)->id != 0xffff && c[0x168] == 0) {
        i = 0;
        p = amt_Table;
        do {
            if (p->idx == m->o1)
                break;
            p++;
            i++;
            if ((int)p >= (int)&amt_Table[30])
                return 0;
        } while (1);
        if (i >= 30)
            return 0;
        kind = m->kind;
        if (kind != amt_Table[i].kind)
            return 0;
        if (m->o2 != 0xff) {
        j = 0;
        p = amt_Table;
        do {
            if (p->idx == m->o2)
                break;
            p++;
            j++;
            if ((int)p >= (int)&amt_Table[30])
                return 0;
        } while (1);
        if (j >= 30)
            return 0;
        if (amt_Table[i].kind != amt_Table[j].kind)
            return 0;
        k = amt_Kinds[amt_Table[i].kind];
        if (k.id != kind)
            return 0;
        if (k.two == 0)
            return 0;
        if (amt_Table[i].owner != -1)
            return 0;
        if (amt_Table[i].state != 3)
            return 0;
        if (amt_Table[i].cnt >= 4)
            return 0;
        if (amt_Table[j].owner != -1)
            return 0;
        if (amt_Table[j].cnt >= 4)
            return 0;
        c[0x168] = kind;
        amt_Table[i].cnt++;
        amt_Table[j].cnt++;
        return 1;
        }
        if (amt_Table[i].owner != -1)
            return 0;
        if (amt_Table[i].state != 3)
            return 0;
        if (amt_Table[i].cnt >= 4)
            return 0;
        c[0x168] = kind;
        amt_Table[i].cnt++;
        return 1;
    }
    return 0;
}
/* 0x4e65e0, amt_SendOfficeMessage of the office a as free (kind 3, code 0xff): the owner is gone or not a candidate (0x4e85d0) or lost the election (0x4e7c60).
   The parameter is a char: the caller 0x4e85d0 passes the idx byte without an extension (notes/vc6-matching.md) */

int amt_SendOfficeFree(char a)
{
    return amt_SendOfficeMessage(a, 0, 0, 3, 0xff);
}
/* 0x4e6600, amt_SendOfficeMessage of the office of the character c (amt_FindOffice) with the rival x (kind 2, code 1); -1 when c has no office or x is 0 */

int amt_SendOfficeCandidates(char *c, char *x)
{
    AMTKIND k;
    AMTREC o;

    if (amt_FindOffice(c, &k, &o) == 0)
        return -1;
    if (x == 0)
        return -1;
    return amt_SendOfficeMessage(o.idx, c, x, 2, 1);
}
/* 0x4e6650, amt_SendOfficeMessage of the office a with the character b as its owner (kind 1, the code c); -1 when b is 0 */

int amt_SendOfficeOwner(char a, char *b, unsigned char c)
{
    if (b == 0)
        return -1;
    return amt_SendOfficeMessage(a, b, 0, 1, c);
}
/* 0x4e6680, sends the office message (cm_SendOfficeRecord, message 0x45): the office a, the ids of the characters b and c (their +4, or -1),
   the kind d and the code e; returns the send slot of the message, -1 when a is not one of the 42 office records */

int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e)
{
    AMTMSG m;
    int i, netIdB, netIdC;
    AMTREC *p;

    netIdB = -1;
    netIdC = -1;
    if (b != 0)
        netIdB = ((AMTCHARVIEW *)b)->netId;
    if (c != 0)
        netIdC = ((AMTCHARVIEW *)c)->netId;
    i = 0;
    p = amt_Table;
    do {
        if (p->idx == a)
            break;
        p++;
        i++;
        if ((int)p >= (int)&amt_Table[42])
            return -1;
    } while (1);
    if (i >= 42)
        return -1;
    m.office = a;
    m.a = netIdB;
    m.d = d;
    m.e = e;
    m.b = netIdC;
    return cm_SendOfficeRecord(&m);
}
/* 0x4e6710, checks the office message m (AMTMSG: +0 the office, +1 / +7 two character ids or -1, +6 the code): with a code (not 0xff) the
   office is one of the 42 records, in the state 1 and without a second candidate (+0x14), and both characters exist; 1 when it may be
   executed (amt_ExecuteOfficeMessage) */

int amt_CheckOfficeMessage(AMTMSG *m)
{
    AMTREC *q;
    int i;

    if (m->e != 0xff) {
        i = 0;
        q = amt_Table;
        do {
            if (q->idx == m->office)
                break;
            q++;
            i++;
            if ((int)q >= (int)&amt_Table[42])
                return 0;
        } while (1);
        if (i >= 42)
            return 0;
        if (amt_Table[i].state != 1)
            return 0;
        if (amt_Table[i].x14 != -1)
            return 0;
        if (m->b != -1) {
            if (gm_FindChar(m->b) == 0)
                return 0;
        }
        if (m->a != -1) {
            if (gm_FindChar(m->a) == 0)
                return 0;
        }
    }
    return 1;
}
/* 0x4e67a0, executes the office message p (AMTSET: +0 the office, +1 the new owner or -1, +5 the new state, +7 the x14
   character; message 0x45, the only caller is cm.c's handler of the type 0x45): the old owner loses the rank / the status
   bit of the office, the new owner takes it (officeKind +0x166, prevOffice +0x167, secondOffice +0x169, status bits
   +0x1cc), the applicants' counts are settled; 0 when a character does not exist
   @deep B3: 136 of 286 instructions differ (135 code hunks). The whole original 0x4e67a0..0x4e6b33 was read against the source and the statement sequence is complete and equal: the x14/owner guards (gm_FindChar, 0 on a missing character), the 42-record scan (named bound &amt_Table[42], i >= 42), the old owner's officeKind/secondOffice/status-bit clears, the k > 0x1b dispatch (0x1c..0x22 second-office scan over the records 30..36 from &amt_Table[30].owner to &amt_Table[37].owner, the 0x23/0x24/0x25 status masks + the 768-record mask loop), the normal path (owner scan, kind level compare, officeKind/prevOffice/appliedOffice, 0x0f/0x15/0x1b status bits, the two-person applicant count), the done block and the ai_Unk5a8cb0 call. Left: the out-of-line block order (the original puts the two-scan body at 0x4e6a41 and the k > 0x1b dispatch at 0x4e6a7c after the return; ours keeps the dispatch inline) and the register roles from instruction 0 (original c=esi, x=[esp+0x10], kind reloaded per use; ours c=edi, x slot, k local) - allocation/scheduling, no menu rule. spell.py leads tried one edit one build each: the first scan as while(cond1 && cond2) (259 diff, reverted), as while + inner if-return (261, reverted), as do-while(1) with the named bound (146, kept); the kind level compare written from the other side (amt_Kinds[kind].level >= ... - 146 -> 136, kept); the Q3 bound 0x92550f -> &amt_Table[42] and 0x92549c -> &amt_Table[37].owner; the Q4/Q2 typed AMTCHARVIEW fields and ai_Chars (byte-neutral). Q2 left: ai_Unk5a8cb0 (0x5a8cb0) has no evidenced name - its only caller is this function, its body sends the text 0x20bb (the text table is loaded at runtime, not in the exe) with the x14 character's id to the new owner (event 0x5fd, only for a player character kind 6/7); naming it needs a match-deep decision. Unit note amt.md has the details. */

int amt_ExecuteOfficeMessage(AMTSET *p)
{
    int i, j, owner;
    char *c, *o, *x;
    unsigned int m, f;
    unsigned char k;
    AMTREC *q;
    int *r;

    i = 0;
    owner = -1;
    x = 0;
    if (p->x14 != -1) {
        x = (char *)gm_FindChar(p->x14);
        if (x == 0)
            return 0;
    }
    c = 0;
    if (p->owner != -1) {
        c = (char *)gm_FindChar(p->owner);
        if (c == 0)
            return 0;
        owner = ((AMTCHARVIEW *)c)->netId;
    }
    q = amt_Table;
    do {
        if (q->idx == p->idx)
            break;
        q++;
        i++;
        if ((int)q >= (int)&amt_Table[42])
            return 0;
    } while (1);
    if (i >= 42)
        return 0;
    o = (char *)gm_FindChar(amt_Table[i].owner);
    if (o != 0 && ((AMTCHARVIEW *)o)->netId != p->owner) {
        if (((AMTCHARVIEW *)o)->officeKind == amt_Table[i].kind)
            ((AMTCHARVIEW *)o)->officeKind = 0;
        else if (((AMTCHARVIEW *)o)->secondOffice == amt_Table[i].kind)
            ((AMTCHARVIEW *)o)->secondOffice = 0;
        else {
            if (amt_Table[i].kind == 0x23)
                f = ((AMTCHARVIEW *)o)->status & 0xfffffdff;
            else if (amt_Table[i].kind == 0x24)
                f = ((AMTCHARVIEW *)o)->status & 0xfffffbff;
            else if (amt_Table[i].kind == 0x25)
                f = ((AMTCHARVIEW *)o)->status & 0xfffff7ff;
            else
                goto next;
            ((AMTCHARVIEW *)o)->status = f;
        }
    }
next:
    if (c == 0)
        goto done;
    k = amt_Table[i].kind;
    if (k > 0x1b) {
        if (k > 0x1b && k < 0x23) {
            if (((AMTCHARVIEW *)c)->secondOffice != 0) {
                r = &amt_Table[30].owner;
                do {
                    if (((unsigned char *)r)[4] == ((AMTCHARVIEW *)c)->secondOffice &&
                        *r == ((AMTCHARVIEW *)c)->netId)
                        *r = -1;
                    r += sizeof(AMTREC) / sizeof(int);
                } while ((int)r < (int)&amt_Table[37].owner);
            }
            ((AMTCHARVIEW *)c)->secondOffice = amt_Table[i].kind;
            goto done;
        }
        m = 0;
        f = ((AMTCHARVIEW *)c)->status & 0xfffff1ff;
        ((AMTCHARVIEW *)c)->status = f;
        if (amt_Table[i].kind == 0x23) {
            f = f | 0x200;
            m = 0x4000;
            ((AMTCHARVIEW *)c)->status = f;
        } else {
            if (amt_Table[i].kind == 0x24) {
                f = f | 0x400;
                m = 0x8000;
                ((AMTCHARVIEW *)c)->status = f;
            } else if (amt_Table[i].kind == 0x25) {
                f = f | 0x800;
                m = 0x10000;
                ((AMTCHARVIEW *)c)->status = f;
            }
        }
        for (j = 0; j < 768; j++)                       /* the 0x300 character records */
            ((AMTCHARVIEW *)ai_Chars)[j].status &= ~m;
        goto done;
    }
    if (((AMTCHARVIEW *)c)->officeKind != 0) {
        for (j = 0; j < 30; j++) {
            if (amt_Table[j].owner == owner) {
                amt_Table[j].owner = -1;
                amt_Table[j].x14 = -1;
                amt_Table[j].state = 4;
                amt_Table[j].cnt = 0;
                break;
            }
        }
    }
    if (amt_Kinds[amt_Table[i].kind].level >= amt_Kinds[((AMTCHARVIEW *)c)->officeKind].level)
        ((AMTCHARVIEW *)c)->prevOffice = amt_Table[i].kind;
    ((AMTCHARVIEW *)c)->officeKind = amt_Table[i].kind;
    if (((AMTCHARVIEW *)c)->appliedOffice == amt_Table[i].kind)
        ((AMTCHARVIEW *)c)->appliedOffice = 0;
    if (amt_Table[i].kind == 0x0f)
        ((AMTCHARVIEW *)c)->status |= 0x40;
    else if (amt_Table[i].kind == 0x15)
        ((AMTCHARVIEW *)c)->status |= 0x80;
    else if (amt_Table[i].kind == 0x1b)
        ((AMTCHARVIEW *)c)->status |= 0x100;
    if (amt_Kinds[amt_Table[i].kind].two != 0) {
        for (j = 0; j < 30; j++) {
            if (amt_Table[j].kind == amt_Table[i].kind && j != i) {
                if (amt_Table[j].state == 3 && amt_Table[j].cnt != 0) {
                    amt_Table[j].cnt--;
                    if (amt_Table[j].cnt == 0)
                        amt_Table[j].state = 4;
                }
                break;
            }
        }
    }
done:
    amt_Table[i].owner = owner;
    amt_Table[i].x14 = p->x14;
    amt_Table[i].state = p->state;
    if (p->state == 3)
        amt_Table[i].cnt = 0;
    if (x != 0 && c != 0 && ((AMTCHARVIEW *)ai_Chars)[ai_PlayerId].netId == ((AMTCHARVIEW *)c)->netId)
        ai_Unk5a8cb0((unsigned short *)x, c);
    return 1;
}
/* 0x4e6b40, the office of the kind 0x26 (the historical event's office) gets the owner id */

void amt_SetEventOwner(int id)
{
    int i;

    i = 41;
    do {
        if (amt_Table[i].kind == 0x26) {
            amt_Table[i].owner = id;
            return;
        }
        i--;
    } while (i != 0);
}
/* 0x4e6b70, the same for the kind 0x27 (the person's office) */

void amt_SetPersonOwner(int id)
{
    int i;

    i = 41;
    do {
        if (amt_Table[i].kind == 0x27) {
            amt_Table[i].owner = id;
            return;
        }
        i--;
    } while (i != 0);
}
/* 0x4e6ba0, sends the office-swap request (message 0x5c, the AMTTRADE m) of the requester a for the characters b and
   c (their offices from 0x4e6030) when both offices exist and their kinds have the same column and level; m.v = 6.
   Returns the message slot (the return value of cm_SendTradeMessage), -1 when a, b or c is 0 or has no office. The
   counter-espionage wrapper 0x5cb1e0 (unk_58bbd0.c) waits on the slot. */

int amt_SendOfficeSwap(char *requester, char *b, char *c)
{
    AMTKIND k1, k2;
    AMTREC o1, o2;
    AMTTRADE m;

    if (requester != 0 && b != 0 && c != 0) {
        if (amt_FindOffice(b, &k1, &o1) != 0) {
            if (amt_FindOffice(c, &k2, &o2) != 0 && k1.level == k2.level && k1.col == k2.col) {
                m.id = ((AMTCHARVIEW *)requester)->netId;
                m.a = o1.idx;
                m.b = o2.idx;
                m.v = 6;
                return ((int (*)(char *))cm_SendTradeMessage)(&m);
            }
        }
    }
    return -1;
}
/* 0x4e6c50, executes the swap message m (AMTTRADE: +0 the requester, +4 the first / +5 the second office index, +6 the
   price; message 0x5c, the only caller is cm.c's handler of the type 0x5c): the owners of the two offices exchange their
   office kinds and the office owners, the price is taken off the requester's money (+0x194); 0 when anything is missing */

int amt_ExecuteSwapMessage(AMTTRADE *m)
{
    char *c, *o1, *o2;
    AMTREC *p, *p1, *p2;
    int i, j, t;

    c = gm_FindChar(m->id);
    if (c == 0)
        return 0;
    if (((AMTCHARVIEW *)c)->money < m->v)
        return 0;
    i = 0;
    p = amt_Table;
    do {
        if (p->idx == m->a)
            break;
        p++;
        i++;
        if ((int)p >= (int)&amt_Table[30])
            return 0;
    } while (1);
    p1 = &amt_Table[i];
    if (p1 == 0)
        return 0;
    o1 = (char *)gm_FindChar(p1->owner);
    if (o1 == 0)
        return 0;
    j = 0;
    p = amt_Table;
    do {
        if (p->idx == m->b)
            break;
        p++;
        j++;
        if ((int)p >= (int)&amt_Table[30])
            return 0;
    } while (1);
    p2 = &amt_Table[j];
    if (p2 == 0)
        return 0;
    o2 = (char *)gm_FindChar(p2->owner);
    if (o2 == 0)
        return 0;
    ai_SendSwapText((unsigned short *)c, (unsigned short *)o1, (unsigned short *)o2);
    ((AMTCHARVIEW *)o1)->officeKind = p2->kind;
    ((AMTCHARVIEW *)o2)->officeKind = p1->kind;
    t = p1->owner;
    p1->owner = p2->owner;
    p2->owner = t;
    ((AMTCHARVIEW *)c)->money -= m->v;
    return 1;
}
/* 0x4e6d80, takes the character c out of the offices (owner or second character of any of the 42: the owner's office
   becomes free (state 3, or 4 after the 12th hour of the game clock), a second office returns to state 1), takes back its
   application (the applicant counts of its kind) and clears its ranks (officeKind / secondOffice / appliedOffice);
   1 when it held an office or an application
   @deep B3: 32 of ~120 instructions differ (51 code hunks; ours 63 instructions, the original 57). The whole original 0x4e6d80..0x4e6e16 was read against the source and the statement sequence is equal: d = the clock hour (ai_TimeHour = ai_Time.hour 0x770ae4), the 42-record loop testing owner then x14 (owner: free state (hour > 11) + 3, cnt 0, r |= 1; x14: state 1), the appliedOffice loop (kind match + cnt > 0: r |= 1, cnt--), the three rank bytes cleared, return r. Left: the loop's induction base (the original walks &amt_Table[i].x14 with [ecx-0x10] = owner, [ecx-8] = cnt, [ecx-4] = state; ours uses &amt_Table[i].owner), the -1 constant (the original stores the immediate, ours hoists -1 into edi and pushes it - 3 extra instructions) and the netId cache in edx - allocation/strength-reduction, no menu rule. spell.py reload lead tried: the netId is already the typed expression at both uses (the original reloads [ebp+4] once per branch, ours CSEs it); the r = 1 -> r |= 1 spelling was applied (the original's or al,0x1) and the typed AMTCHARVIEW fields / ai_TimeHour view are byte-neutral. Readability Q1-Q7 pass (amt_ClearOffices, AMTCHARVIEW fields). */

int amt_ClearOffices(char *c)
{
    unsigned short d;
    int r, i;

    d = ai_TimeHour;
    r = 0;
    for (i = 0; i < 42; i++) {
        if (amt_Table[i].owner == ((AMTCHARVIEW *)c)->netId) {
            amt_Table[i].owner = -1;
            amt_Table[i].cnt = 0;
            amt_Table[i].state = (d > 11) + 3;
            r |= 1;
            amt_Table[i].x14 = -1;
        } else if (amt_Table[i].x14 == ((AMTCHARVIEW *)c)->netId) {
            amt_Table[i].state = 1;
            amt_Table[i].x14 = -1;
        }
    }
    if (((AMTCHARVIEW *)c)->appliedOffice != 0) {
        for (i = 0; i < 42; i++) {
            if (amt_Table[i].kind == ((AMTCHARVIEW *)c)->appliedOffice && amt_Table[i].cnt > 0) {
                r |= 1;
                amt_Table[i].cnt--;
            }
        }
    }
    ((AMTCHARVIEW *)c)->officeKind = 0;
    ((AMTCHARVIEW *)c)->secondOffice = 0;
    ((AMTCHARVIEW *)c)->appliedOffice = 0;
    return r;
}
/* 0x4e6e20, amt_ClearOffices without the result: the owner's office is released only when `all` is set (the second
   office always returns to state 1), the application is always taken back, the ranks (officeKind / secondOffice) are
   cleared only when `all` is set
   @deep B3: the sibling of amt_ClearOffices (0x4e6d80) with the same body and the same causes: 51 code hunks from the loop's induction base (the original walks &amt_Table[i].x14 with [ecx-0x10] = owner, [ecx-8] = cnt, [ecx-4] = state; ours uses &amt_Table[i].owner), the -1 constant hoisted into a pushed register instead of the original's immediate stores, and the netId cache in edx - allocation / strength reduction, no menu rule. The whole original 0x4e6e20..0x4e6e2b was read against the source: the owner branch gated by all != 0 (release, cnt 0, state (hour > 11) + 3, x14 -1), the x14 branch (state 1), the appliedOffice count take-back, appliedOffice = 0 and the officeKind / secondOffice clears under all != 0 - every statement matches. spell.py: the reload lead (the netId is the typed expression at both tests) and the r = 1 spelling (no r here) tried; typed AMTCHARVIEW fields and ai_TimeHour are byte-neutral. Readability Q1-Q7 pass (amt_ClearOfficesIfAll). */

void amt_ClearOfficesIfAll(char *c, int all)
{
    unsigned short d;
    int i;

    d = ai_TimeHour;
    for (i = 0; i < 42; i++) {
        if (amt_Table[i].owner == ((AMTCHARVIEW *)c)->netId && all != 0) {
            amt_Table[i].owner = -1;
            amt_Table[i].cnt = 0;
            amt_Table[i].state = (d > 11) + 3;
            amt_Table[i].x14 = -1;
        } else if (amt_Table[i].x14 == ((AMTCHARVIEW *)c)->netId) {
            amt_Table[i].state = 1;
            amt_Table[i].x14 = -1;
        }
    }
    if (((AMTCHARVIEW *)c)->appliedOffice != 0) {
        for (i = 0; i < 42; i++) {
            if (amt_Table[i].kind == ((AMTCHARVIEW *)c)->appliedOffice && amt_Table[i].cnt > 0)
                amt_Table[i].cnt--;
        }
    }
    ((AMTCHARVIEW *)c)->appliedOffice = 0;
    if (all != 0) {
        ((AMTCHARVIEW *)c)->officeKind = 0;
        ((AMTCHARVIEW *)c)->secondOffice = 0;
    }
}
/* 0x4e6ec0, copies the first office of the kind k to *out; 1 when there is one @deep B3: 5 code hunks left of 36 instructions (one differing branch). The whole original 0x4e6ec0..0x4e6f09 was read against the source: the kind scan over the record kind byte from &amt_Table[0].kind (named bound &amt_Table[42].kind, jl), the found check i >= 42 (the original cmp ecx,0x2a), the 0x18-byte copy *out = amt_Table[i] (rep movs 6 dwords) and the return 1 / loop-exit return 0 all match; Q3 0x925518 replaced by the bound. Left: the i >= 42 check's block placement - the original emits cmp ecx,0x2a; jl <copy> with the return 0 in the fallthrough, ours emits cmp; jge <out-of-line return>. Menu leads tried one edit one build each: if (i < 42) { copy; return 1; } with the return after it (9 -> 5 hunks), the same with an explicit else (no change), the break spelling (no change) - the dup-tail prescriptions are used up, the rest is VC6's block ordering. Q1-Q7 pass (amt_GetFirstOffice, the kind byte pointer, the named table bounds). */

int amt_GetFirstOffice(unsigned int k, AMTREC *out)
{
    unsigned char *pk;
    int i;

    i = 0;
    pk = &amt_Table[0].kind;
    do {
        if (*pk == k) {
            if (i >= 42)
                break;
            *out = amt_Table[i];
            return 1;
        }
        pk += sizeof(AMTREC);
        i++;
    } while ((int)pk < (int)&amt_Table[42].kind);
    return 0;
}
/* 0x4e6f10, the column (rank class) of the kind k (0 for k > 39) */

int amt_KindColumn(int k)
{
    if (k >= 40)
        return 0;
    return amt_Kinds[k].col;
}
/* 0x4e6f30, copies the office with the index i to *out; 1 when there is one @deep B3: 4 code hunks left of 51 instructions (one differing branch, the same block-order item as amt_GetFirstOffice 0x4e6ec0). The whole original 0x4e6f30..0x4e6f73 was read against the source: the idx scan over the record byte from amt_Table (named bound &amt_Table[42] = 0x925510), the found check n >= 42 (the original cmp ecx,0x2a), the 0x18-byte copy *out = amt_Table[n] (rep movs 6 dwords) and both return 0 blocks match; the unsigned char idx gives the original's byte compare. Left: the n >= 42 check's block placement - the original emits cmp ecx,0x2a; jl <copy> with the return 0 in the fallthrough, ours emits cmp; jge <out-of-line return 0> (VC6 merges it with the loop-exit return). Menu leads tried one edit one build each: if (n < 42) { copy; return 1; } return 0; (no change), if (n >= 42) return 0; copy; return 1; (no change) - the dup-tail prescriptions are used up. Q1-Q7 pass (amt_GetOffice). */

int amt_GetOffice(unsigned char i, AMTREC *out)
{
    AMTREC *p;
    int n;

    n = 0;
    p = amt_Table;
    do {
        if (p->idx == i) {
            if (n >= 42)
                return 0;
            *out = amt_Table[n];
            return 1;
        }
        p++;
        n++;
    } while ((int)p < (int)&amt_Table[42]);
    return 0;
}
/* 0x4e6f80, copies the kind record k to *out (the first one for k > 39); 1 when k is valid */

int amt_GetKind(int k, AMTKIND *out)
{
    if (k >= 40) {
        *out = amt_Kinds[0];
        return 0;
    }
    *out = amt_Kinds[k];
    return 1;
}
/* 0x4e6fc0, collects up to max offices of the column col (1 .. 6: the first 30 records, 8: all 42), from the last backwards, into out; returns their number @deep B3: 13 code hunks (13 differing of ~61 instructions), all in the prologue/epilogue. The whole original 0x4e6fc0..0x4e702d was read against the source and matches: the col guards (col == 0 none, col <= 6 -> 30, col == 8 -> 42, else none), n = 0, the for (i--; i >= 0; i--) walk backwards over amt_Table, the n >= max return n, the kind column test amt_Kinds[kind].col == col, the 0x18-byte copy *out = amt_Table[i] with out++ and the shared return 0 (the goto none label reproduces the original's out-of-line return 0 used by both col guards). Left: the callee-saved push placement (the original shrink-wraps ebp/esi/edi to just before the loop and separates dec ebx from a test ebx,ebx/jl; ours pushes them in the function prologue and fuses dec/js) - VC6 shrink-wrapping, no menu rule. Menu leads tried one edit one build each: the col bound spellings (col < 7 / col != 8 -> col <= 6 / col == 8 else) fixed those hunks; the while + break form (47 code hunks) -> for (i--; i >= 0; i--) with return n (13); the early returns inline vs the shared goto none label (45/47 -> 13). Q1-Q7 pass (amt_GetColumnOffices). */

int amt_GetColumnOffices(int col, int max, AMTREC *out)
{
    int n, i;

    if (col == 0)
        goto none;
    if (col <= 6)
        i = 30;
    else {
        if (col == 8)
            i = 42;
        else
            goto none;
    }
    n = 0;
    for (i--; i >= 0; i--) {
        if (n >= max)
            return n;
        if (amt_Kinds[amt_Table[i].kind].col == col) {
            n++;
            *out = amt_Table[i];
            out++;
        }
    }
    return n;
none:
    return 0;
}
/* 0x4e7040, collects up to max offices of the column col (1 .. 6: the first 30 records, 5: all 42) that have an owner who exists, from the last backwards, into out;
   returns their number
   @deep B3 code 61 hunks / 33 differing of 68 instructions, one merged hunk. Meaning read against the whole original 0x4e7040..0x4e70e6 and complete: col guards (0 none, <=6 -> 30, col 5 -> 42), n = 0, for (i--; i >= 0; i--), if (n >= max) break (the original jge targets the shared epilogue; the break spelling took 103 -> 61 hunks and made the callee-saved pushes shrink-wrap like the original), the col test, gm_FindChar(owner), the 0x18 copy. Menu/leads tried one build each: dup-tail (return 0 in both guard arms: identical code, reverted to the goto none the original block layout shows; break: kept), i-- as for-init vs a separate statement (same code), out++/n++ statement order (-1 byte, kept), comma-init for (n = 0, i--; ...) (same code). Left, all allocation/scheduling: (1) loop-entry test ebp,ebp; jl vs our js; (2) the out parameter home-slot init mov eax,[out]; mov [out],eax and its double reload in the copy block (spell reload: the parameter is used directly, no local to drop); (3) the rep movs block regs/schedule (n in ecx after the copy vs eax before, edi vs eax for out, rep placement). Q1-Q7 pass. */

int amt_GetColumnOfficesOwned(int col, int max, AMTREC *out)
{
    int n, i;

    if (col == 0)
        goto none;
    if (col <= 6)
        i = 30;
    else {
        if (col == 5)
            i = 42;
        else
            goto none;
    }
    n = 0;
    for (i--; i >= 0; i--) {
        if (n >= max)
            break;
        if (amt_Kinds[amt_Table[i].kind].col == col && gm_FindChar(amt_Table[i].owner) != 0) {
            *out = amt_Table[i];
            out++;
            n++;
        }
    }
    return n;
none:
    return 0;
}
/* 0x4e70f0, collects up to max of the column-7 offices (the table records 30 .. 36: the deputies 0x1c / 0x1d, the town mayors 0x1e .. 0x21 and 0x22) into out,
   forward; a record is taken when its kind's column is 7 and, with `all` 0, when it has an owner (-1 = free; the yearly check 0x4e9260 passes all 1). Writes the
   records and returns their number (0 .. max); the scan stops at max */

int amt_GetColumn7Offices(int all, int max, AMTREC *out)
{
    int n, i;

    n = 0;
    i = 30;
    do {
        if (n >= max)
            return n;
        if (amt_Kinds[amt_Table[i].kind].col == 7 && (all != 0 || amt_Table[i].owner != -1)) {
            n++;
            *out = amt_Table[i];
            out++;
        }
        i++;
    } while (i < 37);
    return n;
}
/* 0x4e7150, the offices the character c may apply for, best first: the open ones (state 3, fewer than 4 applicants) among the first 30 table records (the offices of the
   guild columns) whose kind has a rate for c (amt_GetOfficeRate); one entry per kind, sorted by level and, at the same level, by rate (an empty place is -100.0 / kind 0 /
   level 0). Writes up to n entries to out (n * 4 bytes of it are cleared first), returns their number capped at n; the callers are the application list 0x4ea8e0 and the
   AI decision 0x497490 (am.c), which applies for a random candidate @deep Read whole against the original and fixed: the duplicate search runs with a found flag (for i = n-1; i >= 0 && !found; the test edx / jne at the loop top), the place search sets a stop flag on the first entry that is not lower (level unsigned, jb / jne, then the rate test ah,0x41), a run to -1 re-checks out[0] (the original's second level / rate test at +0x124) and only a stop inserts, the record walk is an index loop over the first 30 (signed jl on the strength-reduced pointer, ours had an unsigned pointer compare), the cap cnt >= n. Left: VC6 walks our records from &amt_Table[j].cnt (+0xc: mov bl,[eax-4] for kind) where the original walks from &amt_Table[j].kind (+8, mov bl,[eax]; moving the kind load first made it worse, 76 ins), the locals sit in other ebp slots (rate [ebp-8] vs [ebp-4], cnt [ebp-4] vs [ebp-0xc], the walk pointer [ebp-0xc] vs [ebp-8]) and our AI_ZERO copies out through a slot first (mov ecx,[ebp+0x10] / mov [ebp-0xc],ecx / mov edi,[ebp-0xc] where the original loads edi directly). */

int amt_GetOfficeCandidates(char *c, int n, AMTCAND *out)
{
    int cnt, i, j, k, kind, found, stop;
    float rate;
    AMTREC *p;

    cnt = 0;
    if (n <= 0)
        return 0;
    AI_ZERO(out, n * 4);
    for (i = n - 1; i >= 0; i--) {
        out[i].rate = -100.0f;
        out[i].kind = 0;
        out[i].level = 0;
    }
    for (j = 0; j < 30; j++) {
        p = &amt_Table[j];
        if (p->state != 3 || p->cnt >= 4)
            continue;
        kind = p->kind;
        if (amt_GetOfficeRate(c, kind, &rate) == 0)
            continue;
        found = 0;
        for (i = n - 1; i >= 0 && !found; i--) {
            if (out[i].kind == kind)
                found = 1;
        }
        if (found)
            continue;
        /* the place: below the last entry of a higher level or of the same level with a better rate */
        stop = 0;
        for (i = n - 1; i >= 0; i--) {
            if (out[i].level < amt_Kinds[kind].level)
                continue;
            if (out[i].level != amt_Kinds[kind].level || !(out[i].rate <= rate)) {
                stop = 1;
                break;
            }
        }
        if (i == -1) {
            i = 0;
            if (out[0].level < amt_Kinds[kind].level || (out[0].level == amt_Kinds[kind].level && out[0].rate <= rate))
                stop = 1;
        }
        if (!stop)
            continue;
        cnt++;
        for (k = n - 1; k > i; k--) {
            out[k].level = out[k - 1].level;
            out[k].rate = out[k - 1].rate;
            out[k].kind = out[k - 1].kind;
        }
        out[i].level = amt_Kinds[p->kind].level;
        out[i].rate = rate;
        out[i].kind = p->kind;
    }
    if (cnt >= n)
        cnt = n;
    return cnt;
}
/* 0x4e7320, the same candidate list as amt_GetOfficeCandidates, with the full offices counted: a rated office with 4 applicants is skipped for the list and sets
   *full to 1 (when full is not 0). Returns the number of candidates, capped at n; the town hall window 0x5a0590 uses *full to tell "all offices full" from "no
   office open" @deep Read whole against the original and fixed as its sibling amt_GetOfficeCandidates: the duplicate search with a found flag (i >= 0 && !found), the place search with the stop flag, the re-check of out[0] after a run to -1 and the insert only on a stop, the cap cnt >= n. The statements, constants, field offsets and the walk (index loop, &amt_Table[j].kind as the base, signed jl) now agree; left is allocation only: our walk pointer lives in edi and its slot [ebp-0xc] where the original reloads it from [ebp-4] into eax / edx (+0x4e739d cmp byte ptr [eax+8],3 vs ours [edi+8]; +0x4e73c4 mov edx,[ebp-4] / cmp [edx+4],4), cnt and the out copy swap slots ([ebp-4] / [ebp-0xc] / [ebp-8]), ours zeroes through eax where the original uses edx, reloads out into edi on other paths (+0x4e73e0, +0x4e7416, +0x4e747c), and keeps the capped cnt in a slot (mov [ebp-4],esi / mov eax,[ebp-4] vs mov eax,esi). */

int amt_GetOfficeCandidatesFull(char *c, int n, AMTCAND *out, int *full)
{
    int cnt, i, j, k, kind, f, found, stop;
    float rate;

    cnt = 0;
    f = 0;
    if (n <= 0)
        return 0;
    AI_ZERO(out, n * 4);
    for (i = n - 1; i >= 0; i--) {
        out[i].rate = -100.0f;
        out[i].kind = 0;
        out[i].level = 0;
    }
    for (j = 0; j < 30; j++) {
        if (amt_Table[j].state != 3)
            continue;
        kind = amt_Table[j].kind;
        if (amt_GetOfficeRate(c, kind, &rate) == 0)
            continue;
        if (amt_Table[j].cnt >= 4) {
            f = 1;
            continue;
        }
        found = 0;
        for (i = n - 1; i >= 0 && !found; i--) {
            if (out[i].kind == kind)
                found = 1;
        }
        if (found)
            continue;
        stop = 0;
        for (i = n - 1; i >= 0; i--) {
            if (out[i].level < amt_Kinds[kind].level)
                continue;
            if (out[i].level != amt_Kinds[kind].level || !(out[i].rate <= rate)) {
                stop = 1;
                break;
            }
        }
        if (i == -1) {
            i = 0;
            if (out[0].level < amt_Kinds[kind].level || (out[0].level == amt_Kinds[kind].level && out[0].rate <= rate))
                stop = 1;
        }
        if (!stop)
            continue;
        cnt++;
        for (k = n - 1; k > i; k--) {
            out[k].level = out[k - 1].level;
            out[k].rate = out[k - 1].rate;
            out[k].kind = out[k - 1].kind;
        }
        out[i].level = amt_Kinds[amt_Table[j].kind].level;
        out[i].rate = rate;
        out[i].kind = amt_Table[j].kind;
    }
    if (full != 0)
        *full = f;
    if (cnt >= n)
        cnt = n;
    return cnt;
}
/* 0x4e7510, the character who holds the first office of the kind k (the AI decision 0x490e00 asks for the kind 0x11 holder); 0 when no record has the kind or the
   owner does not exist (gm_FindChar) */

char *amt_GetOfficeHolder(int k)
{
    int i;

    for (i = 0; i < 42; i++)
        if (amt_Table[i].kind == k)
            return gm_FindChar(amt_Table[i].owner);
    return 0;
}
/* 0x4e7550, the character who holds the office the kind k is the deputy of (0x24 -> kind 0x27, the person's office; 0x23 / 0x25 -> kind 0x26, the historical
   event's office); 0 when there is none. The election hmt_Election uses it as the reference object (hmt_Score) of the year's office type */

char *amt_GetSuperiorOfficeHolder(int k)
{
    AMTREC o;

    if (k == 0x24) {
        if (amt_GetFirstOffice(0x27, &o) != 0)
            return gm_FindChar(o.owner);
    } else if (k == 0x25 || k == 0x23) {
        if (amt_GetFirstOffice(0x26, &o) != 0)
            return gm_FindChar(o.owner);
    }
    return 0;
}
/* 0x4e75c0, 1 when the character c may take the office of the kind k (1 .. 27): the kind must exist, its column must be the column of c's current office rank
   (+0x166) and the levels must follow the cycle of three - level % 3 of k vs of c's rank, (0 / not 0, 2 / 0, 1 / 2); 0 otherwise. The office-application checks
   (cm.c 0x4dada0 / 0x4dae90) and the candidate scan 0x4e76b0 use it */

int amt_CanTakeOffice(char *c, int k)
{
    AMTKIND a, b;

    if (k != 0 && k <= 0x1b) {
        if (amt_GetKind(k, &a) != 0) {
            if (amt_GetKind((unsigned char)c[0x166], &b) != 0 && a.col == b.col) {
                if (a.level % 3 == 0 && b.level % 3 != 0)
                    return 1;
                if (a.level % 3 == 2 && b.level % 3 == 0)
                    return 1;
                if (a.level % 3 == 1 && b.level % 3 == 2)
                    return 1;
            }
        }
    }
    return 0;
}
/* 0x4e76b0, the applicants for the office of the kind k: the characters (up to n, at most 6 offices looked at) holding an office of the column of the kind k whom
   amt_CanTakeOffice accepts for k, into out[]; returns their number. The AI partner choice (0x4a0240, 0x4a3af0) and the vote dialog (cm.c 0x4dab50) use it */

int amt_GetOfficeApplicants(int k, int n, char **out)
{
    AMTKIND a;
    AMTREC list[6];
    int cnt, m, i;
    char *c;

    if (amt_GetKind(k, &a) == 0)
        return 0;
    m = n;
    if (n >= 6)
        m = 6;
    m = amt_GetColumnOffices(a.col, m, list);
    if (m == 0)
        return 0;
    if (m >= n)
        m = n;
    cnt = 0;
    for (i = 0; i < m; i++) {
        c = gm_FindChar(list[i].owner);
        if (c != 0 && amt_CanTakeOffice(c, k) != 0)
            out[cnt++] = c;
    }
    return cnt;
}
/* 0x4e7770, the open offices (state 3) of the column of the office kind c holds (+0x166) that c is eligible for by level: a level
   divisible by 3 or one below the level of c's office (none when that level is 1 mod 3), into out[] at most n; the
   scan stops after an office that takes two persons; returns their number. Only reached through the page fill 0x5bfd90
   (the special-amount window), which counts the characters who applied for these offices */

int amt_GetEligibleOffices(char *c, int n, int *out)
{
    AMTKIND a;
    AMTREC list[6];
    int m, i, cnt, want, id, level;

    if (c[0x166] == 0)
        return 0;
    if (amt_GetKind((unsigned char)c[0x166], &a) == 0)
        return 0;
    m = amt_GetColumnOffices(a.col, 6, list);
    if (m == 0)
        return 0;
    level = a.level;
    if (level % 3 == 1)
        want = 0;
    else
        want = level - 1;
    cnt = 0;
    for (i = 0; i < m && cnt < n; i++) {
        if (list[i].state == 3) {
            id = list[i].kind;
            amt_GetKind(id, &a);
            level = a.level;
            if (level % 3 == 0)
                out[cnt++] = id;
            else if (want == level) {
                out[cnt++] = id;
                if (a.two != 0)
                    break;
            }
        }
    }
    return cnt;
}

float amt_Rates[] = { 1.0f, -2.5f, -2.5f, -2.5f, -5.0f, -5.0f, -10.0f, 1.0f, 0.0f, -2.5f, -5.0f, -2.5f, -7.0f, -10.0f, 1.0f, -2.5f, 0.0f, -2.5f, -5.0f, -5.0f, -10.0f, 1.0f, -5.0f, -2.5f, 0.0f, -7.5f, -2.5f, -10.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -5.0f, -10.0f, 1.0f, 1.0f, 1.0f, 1.0f, -5.0f, 0.0f, -10.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };  /* 0x60ec90, 7 floats per row */
AMTKIND amt_Kinds[40] = { { 0, 0, 0, "", 0, 0, 1084227584 }, { 1, 1, 1, "", 500, 1, 1084227584 }, { 2, 1, 2, "", 700, 0, 1084227584 }, { 3, 1, 3, "", 1000, 0, 1084227584 }, { 4, 2, 1, "", 500, 1, 1084227584 }, { 5, 2, 2, "", 700, 0, 1084227584 }, { 6, 2, 3, "", 1000, 0, 1084227584 }, { 7, 3, 1, "", 500, 1, 1084227584 }, { 8, 3, 2, "", 700, 0, 1084227584 }, { 9, 3, 3, "", 1000, 0, 1084227584 }, { 10, 4, 4, "", 2000, 0, 1084227584 }, { 11, 4, 4, "", 2000, 0, 1084227584 }, { 12, 4, 4, "", 2000, 0, 1084227584 }, { 13, 4, 5, "", 2500, 0, 1084227584 }, { 14, 4, 5, "", 2500, 0, 1084227584 }, { 15, 4, 6, "", 3000, 0, 1084227584 }, { 16, 5, 4, "", 2000, 0, 1086324736 }, { 17, 5, 4, "", 2000, 0, 1086324736 }, { 18, 5, 4, "", 2000, 0, 1086324736 }, { 19, 5, 5, "", 2500, 0, 1086324736 }, { 20, 5, 5, "", 2500, 0, 1090519040 }, { 21, 5, 6, "", 3000, 0, 1090519040 }, { 22, 6, 7, "", 5000, 0, 1088421888 }, { 23, 6, 7, "", 5000, 0, 1088421888 }, { 24, 6, 7, "", 5000, 0, 1092616192 }, { 25, 6, 8, "", 7000, 0, 1088421888 }, { 26, 6, 8, "", 7000, 0, 1094713344 }, { 27, 6, 9, "", 10000, 0, 1088421888 }, { 28, 7, 4, "", 3000, 0, 1084227584 }, { 29, 7, 4, "", 3000, 0, 1084227584 }, { 30, 7, 4, "", 3000, 0, 1084227584 }, { 31, 7, 4, "", 3000, 0, 1084227584 }, { 32, 7, 4, "", 3000, 0, 1084227584 }, { 33, 7, 4, "", 3000, 0, 1084227584 }, { 34, 7, 4, "", 3000, 0, 1090519040 }, { 35, 8, 10, "", 20000, 0, 1084227584 }, { 36, 8, 10, "", 20000, 0, 1084227584 }, { 37, 8, 11, "", 30000, 0, 1084227584 }, { 38, 8, 12, "", 50000, 0, 1084227584 }, { 39, 8, 12, "", 50000, 0, 1084227584 } };  /* 0x60ed58 */
int amt_Flip = 2;                       /* 0x60f118 */
AMTREC amt_Table[42] = {0};  /* 0x925120 */
const float amt_TabA[] = { 1.54142831e-44f, 0.0f, -3.0f, -5.0f, -3.0f, -8.0f, -8.0f, -12.0f, 0.0f, -15.0f, 1.68155816e-44f, 0.0f, -3.0f, -5.0f, -3.0f, -8.0f, -8.0f, -12.0f, 0.0f, -15.0f, 1.821688e-44f, 0.0f, -3.0f, -3.0f, -3.0f, -5.0f, -10.0f, -10.0f, 0.0f, -15.0f, 1.96181785e-44f, 0.0f, -5.0f, -5.0f, -5.0f, -8.0f, -12.0f, -12.0f, 0.0f, -15.0f, 1.40129846e-45f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 3.0f, 0.0f, 0.0f, -15.0f, 2.80259693e-45f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 3.0f, 0.0f, 0.0f, -15.0f, 4.20389539e-45f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 3.0f, 0.0f, 0.0f, -15.0f, 2.1019477e-44f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f, 3.0f, 0.0f, 0.0f, -15.0f, 9.80908925e-45f, 0.0f, 3.0f, 3.0f, -5.0f, -8.0f, 10.0f, 0.0f, 0.0f, -15.0f, 5.60519386e-45f, 0.0f, 3.0f, 5.0f, 5.0f, 0.0f, -5.0f, -5.0f, 0.0f, -15.0f, 1.12103877e-44f, 0.0f, 5.0f, -5.0f, 5.0f, 5.0f, -5.0f, -5.0f, 0.0f, -15.0f, 8.40779079e-45f, 0.0f, 5.0f, 0.0f, -5.0f, 0.0f, -12.0f, -5.0f, 0.0f, -15.0f, 7.00649232e-45f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, -10.0f, -5.0f, 0.0f, -15.0f, 1.26116862e-44f, 0.0f, 5.0f, 5.0f, 5.0f, 0.0f, -10.0f, -10.0f, 0.0f, -15.0f, 1.40129846e-44f, 0.0f, 5.0f, 10.0f, 5.0f, 5.0f, 5.0f, 5.0f, 0.0f, -15.0f, 2.1019477e-44f, 0.0f };  /* 0x5edc90, 15 rows of 10 floats (id, 9 columns by the rank) */
const float amt_TabB[] = { 4.90454463e-44f, -15.0f, -10.0f, -10.0f, -10.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 5.04467447e-44f, -15.0f, -10.0f, -10.0f, -10.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.18480432e-44f, -15.0f, -10.0f, -10.0f, -10.0f, -5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 5.46506401e-44f, -20.0f, -15.0f, -15.0f, -15.0f, -10.0f, -5.0f, -5.0f, 0.0f, 0.0f, 5.32493416e-44f, -20.0f, -15.0f, -15.0f, -15.0f, -5.0f, -10.0f, -5.0f, 0.0f, 0.0f, 7.00649232e-45f };  /* 0x5edef0, 5 rows of 10 */
const float amt_Pay[5] = { 1.10000002f, 1.04999995f, 1.0f, 0.949999988f, 0.899999976f };  /* 0x5edfbc */

/* 0x4e78a0, the highest office kind <= k (at most 0x1b) that is open for applications (state 3) and has at most one
   applicant, so that another one can still apply; 0 when there is none. The two-person event 0x552920 asks it for the office
   whose two new characters it creates and applies for */
int amt_GetOpenOffice(int k)
{
    AMTREC o;

    if (k == 0)
        return 0;
    if (k >= 0x1b)
        k = 0x1b;
    else if (k <= 0)
        return k;
    do {
        if (amt_GetFirstOffice(k, &o) != 0 && o.state == 3 && o.cnt <= 1)
            return k;
        k--;
    } while (k > 0);
    return k;
}
/* 0x4e78f0, the creation ranges of the characters the office events create (0x4e7aa0) and set up (0x4e7b60), by the office level of
   the kind k (1 .. 9): the age range (s0 .. s1, the 4th argument of cm_SendCreateCharacter, 16 .. 32), the starting money range
   (i2 .. i3, the amount of the 0x4c4830 message, a round count of AMT_MONEY_UNIT), the range of the level field 0xd (i4 .. i5,
   3 .. 8) and the job group (i6 .. i7, the 2nd argument of gm_TitleNumber, 2 .. 6); 1 when the level has a row */

int amt_GetCreateRanges(int k, AMTRANGE *r)
{
    AMTKIND a;

    if (amt_GetKind(k, &a) != 0) {
        switch (a.level) {
        case 1:
        case 2:
            r->s0 = 0x10;
            r->s1 = 0x14;
            r->i2 = 10000 * AMT_MONEY_UNIT;
            r->i3 = 20000 * AMT_MONEY_UNIT;
            r->i4 = 3;
            r->i5 = 4;
            r->i6 = 2;
            r->i7 = 3;
            return 1;
        case 3:
        case 4:
            r->s0 = 0x14;
            r->s1 = 0x18;
            r->i2 = 20000 * AMT_MONEY_UNIT;
            r->i3 = 30000 * AMT_MONEY_UNIT;
            r->i4 = 4;
            r->i5 = 5;
            r->i6 = 3;
            r->i7 = 4;
            return 1;
        case 5:
            r->s0 = 0x18;
            r->s1 = 0x1c;
            r->i2 = 30000 * AMT_MONEY_UNIT;
            r->i3 = 40000 * AMT_MONEY_UNIT;
            r->i4 = 5;
            r->i5 = 6;
            r->i6 = 3;
            r->i7 = 4;
            return 1;
        case 6:
            r->s0 = 0x18;
            r->s1 = 0x1c;
            r->i2 = 35000 * AMT_MONEY_UNIT;
            r->i3 = 50000 * AMT_MONEY_UNIT;
            r->i4 = 6;
            r->i5 = 7;
            r->i6 = 3;
            r->i7 = 5;
            return 1;
        case 7:
        case 8:
        case 9:
            r->s0 = 0x1c;
            r->s1 = 0x20;
            r->i2 = 50000 * AMT_MONEY_UNIT;
            r->i3 = 75000 * AMT_MONEY_UNIT;
            r->i4 = 7;
            r->i5 = 8;
            r->i6 = 5;
            r->i7 = 6;
            return 1;
        }
    }
    return 0;
}
/* 0x4e7aa0, creates an applicant for the office kind k: rolls the job group (1 .. 12) and the rank (the range i6 .. i7) from
   the ranges of amt_GetCreateRanges, sends the create-character message (cm_SendCreateCharacter, character kind 0x13) with
   them and the title of the group / rank (gm_TitleNumber) and returns its handle; the alternating byte 0x60f118 (0 / 1, starts
   at 2 = random) is the last argument and flips afterwards. The age is rolled from the two overlapping dwords at +0 / +2 of the
   range record, as the original does (its shorts read as dwords), so it is not the age the range means
   @deep B3 71 code hunks + 1 fill (46 differing of 68 instructions, asmdiff). The whole original 0x4e7aa0..0x4e7b5b was read and the meaning is complete and equal: the amt_GetCreateRanges guard (return -1), the amt_Flip == 2 roll (Random(2) & 0xffff), the rank roll Random(r.i7 - r.i6 + 1) + r.i6, the group roll Random(12) & 0xffff + 1, gm_TitleNumber(group, rank), the age roll from the two overlapping dwords at +0 / +2 of the range record (the original reads the shorts as dwords; typed as AMTRANGEW), cm_SendCreateCharacter(0x13, -1, -1, age, -1, title, 0, amt_Flip) and amt_Flip = 1 - amt_Flip. Leads tried one edit one build each: (1) the callee's declared types - the 8th parameter unsigned char (gives the original's mov cl,[amt_Flip] instead of mov ecx/[and ecx,255]) and the 4th int instead of void* (no truncation; 103 -> 77 differing bytes, 61 -> 46 asm lines, kept); (2) the second roll written inside the argument list (e = gm_TitleNumber((x = util_Random(12)) + 1, v)) - no change, reverted. spell.py menu: only the 4e7ae2 width entry (unsigned short x) - x cannot become int, the original masks twice with and eax,0xffff. Left, all allocation: the original needs only esi (r.i6 in esi, v in eax pushed before the second util_Random) while we keep r.i6 in edi (an extra push edi/pop edi) and v in esi across the second call, and later [r+0] in edi / the title in esi vs the original's esi / eax - register allocation and the argument evaluation order (v pushed early), c2prio/slotmap. Q1-Q7 pass (named from the two-person event 0x552920 that creates its two applicants for the office amt_GetOpenOffice picked; gm_TitleNumber called under its canonical name, the overlapping reads typed as AMTRANGEW). */

int amt_CreateApplicant(int k)
{
    AMTRANGE r;
    int e, v;
    unsigned short x;

    if (amt_GetCreateRanges(k, &r) == 0)
        return -1;
    if (amt_Flip == 2)
        amt_Flip = util_RandomU(2) & 0xffff;
    x = util_RandomU(r.i7 - r.i6 + 1);
    v = x + r.i6;
    x = util_RandomU(12);
    e = gm_TitleNumber(x + 1, v);
    x = util_RandomU(((AMTRANGEW *)&r)->word1 - ((AMTRANGEW *)&r)->word0 + 1);
    v = ((int (*)(int, int, int, int, int, int, int, unsigned char))cm_SendCreateCharacter)(0x13, -1, -1, x + ((AMTRANGEW *)&r)->word0, -1, e, 0, amt_Flip);
    amt_Flip = 1 - amt_Flip;
    return v;
}
/* 0x4e7b60, initializes the created applicant c (kind 0x13) for the office kind k: rolls its money (the range i2 .. i3, sent
   with the move-goods message 0x10) and its level (the range i4 .. i5, the field delta 0xd) from the ranges of
   amt_GetCreateRanges, sets its kind (field 2 = 3) and the office it comes from (field 0x167 = k), sends the deltas and
   applies for the kind (amt_ApplyForOffice); 1 when done. The two-person event 0x552920 calls it with the character of each
   created handle */

int amt_InitApplicant(char *c, int k)
{
    AMTRANGE r;
    int a, b, three;
    unsigned short x;

    three = 3;
    if (((AMTCHARVIEW *)c)->kind != 0x13)
        return 0;
    if (amt_GetCreateRanges(k, &r) == 0)
        return 0;
    x = util_RandomU(r.i3 - r.i2 + 1);
    a = x + r.i2;
    cm_SendMoveGoodsUnchecked(((AMTCHARVIEW *)c)->netId, -1, ai_OwnPlayer, a);
    x = util_RandomU(r.i5 - r.i4 + 1);
    b = x + r.i4;
    cm_BeginFieldBlock(c, ((AMTCHARVIEW *)c)->netId);
    cm_AddFieldDelta(1, 1, 0xd, &b);
    cm_AddFieldDelta(1, 1, 2, &three);
    cm_AddFieldDelta(1, 1, 0x167, &k);
    cm_SendFieldDeltas();
    amt_ApplyForOffice(c, k);
    return 1;
}
/* 0x4e7c40, the level of the office kind k (0 when the kind has no record); the callers use it as the rank of a character's
   office: the nearest partner 0x4a0240, ai_FillNearRing 0x4a3af0, the election score hmt_Score, f3init.c and unk_4a7140.c */

int amt_GetKindLevel(int k)
{
    AMTKIND a;

    amt_GetKind(k, &a);
    return a.level;
}
/* 0x4e7c60, settles the n offices of the list recs (a table of records as AMTREC): the offices in the state 2 (two candidates: the owner and the x14 character) are
   decided by the votes of the other office holders (0x4aab90 gives 0 / 1 / 2 per voter; opinions change by 0x4c4b00), the offices in the state 3 (applications, the
   applicants are the characters whose appliedOffice is the office kind) by their votes (0x4aad30), a tie by chance; each result is sent with 0x4e6680 and waited for
   (0x4c3ae0 / 0x52fe00). The first parameter is not used. The whole original (Ghidra 0x4e7c60 + the disassembly) was read against this draft: the statement sequence
   is equal (the two passes, the vote loops, the 4-candidate scan ai_Chars .. ai_Chars + 768, the best / tie / random pick, the message sends and the slot waits);
   what differs is the frame (ours 0x60, the original 0x58: cnt2 / i / cnt3 spill in ours, the original keeps them in registers) and the register roles, i.e.
   allocation only
   @deep B3: 669 code hunks / 4 regs, 1137/1312 bytes, 499 differing asm lines - allocation (frame 0x60 vs 0x58, cnt2/i/cnt3 spill in ours); the whole original was read in the previous session and the statement sequence is equal. Readability Q1-Q7 pass except Q3: 0x7d5f80 is the end of ai_Chars (+ 768 * 0x21c). Symbol spellings measured worse: (char *)ai_Chars + 768*0x21c -> 1182/1328 bytes, 510 lines; (char *)ai_CharInts + 768*0x21c - 4 the same; the literal keeps 1137/1312, 499 (its init emits mov OFFSET;sub 4 and the second loop's register allocation then matches better), so the no-WORSE rule blocks it - same dead end as ls.c's 0x7d6a42 bound. This session: Q1 amt_ElectOffices (the unit's own comment 'lost the election 0x4e7c60'); Q2 ai_CharInts -> ai_CharInts project-wide (80 refs, the character records ai_Chars + 4 as int[][0x87], byte-neutral); Q4 the 8 raw *(int *)(x + 4) -> AMTCHARVIEW netId (byte-neutral); Q5 ROUGH removed; Q7 amt.h's K&R ai_TrustedOfTwo / ai_TrustedOfFour / cm_GetSlotState -> real prototypes (struct AIPER *, int; lint E 0, X 0 new). Typed fields for c[0x166] / ch[0x168] cost +4 asm lines and stay raw. spell.py: no menu rule on the function. */

void amt_ElectOffices(int unused, int n, AMTREC *recs)
{
    int cnt2, cnt3, i, j, m, k, r, sel, left;
    int cnt[4];
    char *cand[4];
    int ids[4];
    int votes[6];
    int best, bi, id, v, s;
    int tie;
    char *o, *x, *c, *ch;
    AMTREC *p;
    char buf[4];

    cnt2 = 0;
    cnt3 = 0;
    for (i = 0; i < n; i++) {
        if (recs[i].state == 2)
            cnt2++;
    }
    for (i = 0; i < n; i++) {
        if (cnt2 == 0)
            break;
        cnt[0] = 0;
        cnt[1] = 0;
        cnt[2] = 0;
        if (recs[i].state == 2) {
            cnt2--;
            o = gm_FindChar(recs[i].owner);
            x = gm_FindChar(recs[i].x14);
            if (o != 0 && x != 0) {
                for (j = 0; j < n; j++) {
                    votes[j] = 3;
                    c = gm_FindChar(recs[j].owner);
                    if (c != 0 && c[0x166] != 0 && c[0x1b1] == 0) {
                        s = ai_TrustedOfTwo(c, o, x, buf);
                        votes[j] = s;
                        cnt[s]++;
                    }
                }
                if (cnt[1] < cnt[0]) {
                    for (j = 0; j < n; j++) {
                        id = recs[j].owner;
                        if (id != ((AMTCHARVIEW *)o)->netId && id != -1 && (unsigned int)votes[j] < 3) {
                            if (votes[j] == 0)
                                cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, -10, 1);
                            if (votes[j] == 1)
                                cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, 10, 1);
                            else
                                cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, -4, 1);
                        }
                    }
                    r = amt_SendOfficeMessage((char)recs[i].idx, 0, 0, 3, 0xff);
                } else {
                    for (j = 0; j < n; j++) {
                        id = recs[j].owner;
                        if (id != ((AMTCHARVIEW *)o)->netId && id != -1 && (unsigned int)votes[j] < 3) {
                            if (votes[j] == 0)
                                v = -20;
                            else if (votes[j] == 1)
                                v = 20;
                            else
                                v = 4;
                            cm_SendRelations(id, ((AMTCHARVIEW *)o)->netId, v, 1);
                        }
                    }
                    r = amt_SendOfficeMessage((char)recs[i].idx, o, 0, 1, 0xff);
                }
                if (r != -1) {
                    while (cm_GetSlotState(r) == 0)
                        sv_PumpNet();
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        if (recs[i].state == 3)
            cnt3++;
    }
    p = recs;
    for (i = 0; i < n; i++, p++) {
        if (cnt3 == 0)
            return;
        cnt[0] = 0;
        cnt[1] = 0;
        cnt[2] = 0;
        cnt[3] = 0;
        for (j = 0; j < 6; j++)
            votes[j] = -1;
        if (p->state == 3 && p->cnt != 0) {
            cnt3--;
            k = 0;
            ch = (char *)ai_CharInts - 4;
            do {
                if (p->cnt <= k || k > 3)
                    break;
                if (ch[0x168] == (char)p->kind) {
                    ids[k] = ((AMTCHARVIEW *)ch)->netId;
                    cand[k] = ch;
                    k++;
                }
                ch += 0x21c;
            } while ((int)ch < (unsigned)((unsigned)ai_CharInts + 0x653fc));
            if (k != 0) {
                if (k == 1) {
                    r = amt_SendOfficeMessage((char)p->idx, cand[0], 0, 1, 0xff);
                    if (r != -1) {
                        while (cm_GetSlotState(r) == 0)
                            sv_PumpNet();
                    }
                } else {
                    for (m = k; m < 4; m++) {
                        ids[m] = -1;
                        cand[m] = 0;
                    }
                    for (j = 0; j < n; j++) {
                        votes[j] = -1;
                        c = gm_FindChar(recs[j].owner);
                        if (c != 0 && amt_CanTakeOffice(c, p->kind) != 0 && c[0x1b1] == 0)
                            votes[j] = ai_TrustedOfFour((char)p->idx, c, ids, buf);
                    }
                    for (j = 0; j < n; j++) {
                        v = votes[j];
                        if (v != -1) {
                            for (m = 0; m < k; m++) {
                                if (((AMTCHARVIEW *)cand[m])->netId == v)
                                    cnt[m]++;
                            }
                        }
                    }
                    m = 0;
                    best = -1;
                    bi = -1;
                    tie = 1;
                    do {
                        if (best < cnt[m]) {
                            tie = 0;
                            bi = m;
                            best = cnt[m];
                        } else if (cnt[m] == best)
                            tie = 1;
                        m++;
                    } while (m < k);
                    if (!tie) {
                        r = amt_SendOfficeMessage((char)p->idx, cand[bi], 0, 1, 0xff);
                        if (r != -1) {
                            while (cm_GetSlotState(r) == 0)
                                sv_PumpNet();
                        }
                        continue;
                    }
                    m = util_RandomU(k) & 0xffff;
                    left = k;
                    do {
                        sel = m;
                        if (cnt[m] == best)
                            break;
                        left--;
                        m = (m + 1) % k;
                        sel = bi;
                    } while (left != 0);
                    r = amt_SendOfficeMessage((char)p->idx, cand[sel], 0, 1, 0xff);
                    if (r != -1) {
                        while (cm_GetSlotState(r) == 0)
                            sv_PumpNet();
                    }
                }
            }
        }
    }
}
/* 0x4e8160, checks the six office columns 6 .. 1 (amt_GetColumnOffices): per column, bit 1 (occupied) is set when an office with two candidates has two living
   ones (health +8 > 0) or an application's applicant (appliedOffice +0x168) has the kind 6 / 7 and the flag +0x1b1 clear; bit 2 (decided) is set when an office
   of the column is held by a character of the kind 6 / 7 with +0x1b1 clear. A column that is occupied but not decided runs the election amt_ElectOffices
   (0x4e7c60) once; the function returns 1 when any column was decided. The character scan walks the records ai_Chars + 2 (the kind byte, 0x21c apart) up to
   ai_Chars + 2 + 768 * 0x21c. The remaining difference to the original is the register / block allocation (tools/asmdiff.py)
   @deep B3 draft: 26 code hunks / 4 regs, 353/608 bytes, 30 differing asm lines (156 hunks / 96 lines at session start), function 16 B longer than the original's 592. Meaning fixes this session (all measured): health +8 / +0x1b1 tests are unsigned ('health <= 0' -> jbe), the character scan starts at ai_Chars + 2 (offsets +0x166 / +0 / +0x1af, bound ai_Chars + 2 + 768*0x21c), fl zeroed by six ascending statements before the cols stores, the two 'fl[i] |= 1 / |= 2' stores split (was r | 3), the state-2 branch merged into if (a != 0 && health > 0) { if (b != 0 && health > 0) fl |= 1; else send(0,0,4); } else send(b,0,1);, the tail written as 'if (fl[i] & 2) ret = 1; else if ...' and the redundant '&& n > 0' guard dropped. Readability Q1-Q7 pass (amt_CheckOfficeColumns, the unit's own comment 'the check of 0x4e8160 for the column col'; 0x7d5f82 -> (char *)ai_Chars + 2 + 768 * 0x21c, byte-neutral here). spell.py: 4e8264 / 4e82a4 no menu rule; 4e83a7 dup-tail - polarity swap, both arms and pull-after-branch tried (the polarity form is in, the goto form of the state-2 branch measures 33 lines vs 30). Left: the original's extra 'test cl,cl; je' (its goto-shaped state-2 source), the dl vs cl health register and the wait-loop placement - allocation / scheduling. */

int amt_CheckOfficeColumns(void)
{
    unsigned int fl[6];
    int cols[6];
    AMTREC list[6];
    int ret, i, n, j, r;
    char *a, *b, *ch;

    fl[0] = 0;
    fl[1] = 0;
    fl[2] = 0;
    fl[3] = 0;
    fl[4] = 0;
    fl[5] = 0;
    cols[0] = 6;
    cols[1] = 5;
    cols[2] = 4;
    cols[3] = 3;
    cols[4] = 2;
    cols[5] = 1;
    ret = 0;
    for (i = 0; i < 6; i++) {
        n = amt_GetColumnOffices(cols[i], 6, list);
        for (j = 0; j < n; j++) {
            if (list[j].state == 2) {
                a = gm_FindChar(list[j].x14);
                b = gm_FindChar(list[j].owner);
                if (a != 0 && ((AMTCHARVIEW *)a)->health > 0) {
                    if (b != 0 && ((AMTCHARVIEW *)b)->health > 0)
                        fl[i] |= 1;
                    else {
                        r = amt_SendOfficeMessage((char)list[j].idx, 0, 0, 4, 0xff);
                        if (r != -1) {
                            while (cm_GetSlotState(r) == 0)
                                sv_PumpNet();
                        }
                    }
                } else {
                    r = amt_SendOfficeMessage((char)list[j].idx, b, 0, 1, 0xff);
                    if (r != -1) {
                        while (cm_GetSlotState(r) == 0)
                            sv_PumpNet();
                    }
                }
            } else if (list[j].state == 3 && list[j].cnt > 0) {
                ch = (char *)ai_Chars + 2;     /* the AICHAR kind byte */
                do {
                    if (ch[0x166] == (char)list[j].kind) {
                        fl[i] |= 1;
                        if ((ch[0] == 6 || ch[0] == 7) && ch[0x1af] == 0) {
                            fl[i] |= 2;
                            break;
                        }
                    }
                    ch += 0x21c;
                } while ((int)ch < (int)((char *)ai_Chars + 2 + 768 * 0x21c));
            }
        }
        if ((fl[i] & 1) != 0) {
            for (j = 0; j < n; j++) {
                a = gm_FindChar(list[j].owner);
                if (a != 0 && (a[2] == 6 || a[2] == 7) && a[0x1b1] == 0) {
                    fl[i] |= 2;
                    break;
                }
            }
        }
        if ((fl[i] & 2) != 0)
            ret = 1;
        else if ((fl[i] & 1) != 0 && ret == 0)
            amt_ElectOffices(cols[i], n, list);
    }
    return ret;
}
/* 0x4e83b0, the column check of 0x4e8160 (amt_CheckOfficeColumns) for one column: bit 1 (occupied) is set when an office with two candidates has two
   living ones (health +8 > 0, +0x1b1 clear) whose x14 holds an office of the column, or when an application's applicant (appliedOffice +0x166 of the kind
   byte ai_Chars + 2) exists; bit 2 (decided) is set when that applicant is a character of the kinds 6 / 7 with +0x1af clear, or when an office of the
   column is held by one. Returns 1 when the column is decided (the council session 0x4d8e20 runs amt_ElectOffices when it is 0). The tail keeps the
   original's `if ((char)f & 2) return 1;` spelling: VC6 compiles it to movsx / and 2 / shr 1, `(f & 2) != 0` to shr / and (notes/vc6-matching.md) */

int amt_IsColumnDecided(int col)
{
    unsigned int f;
    AMTREC list[6];
    int n, j, k;
    char *a, *b, *ch;

    f = 0;
    n = amt_GetColumnOffices(col, 6, list);
    for (j = 0; j < n; j++) {
        if (list[j].state == 2) {
            a = gm_FindChar(list[j].x14);
            b = gm_FindChar(list[j].owner);
            if (a != 0 && ((AMTCHARVIEW *)a)->health > 0 && a[0x1b1] == 0 && b != 0 && ((AMTCHARVIEW *)b)->health > 0) {
                for (k = 0; k < n; k++) {
                    if (list[k].owner == list[j].x14) {
                        f |= 1;
                        break;
                    }
                }
            }
        } else if (list[j].state == 3 && list[j].cnt > 0) {
            ch = (char *)ai_Chars + 2;
            do {
                if (ch[0x166] == (char)list[j].kind) {
                    f |= 1;
                    if ((ch[0] == 6 || ch[0] == 7) && ch[0x1af] == 0) {
                        f |= 2;
                        break;
                    }
                }
                ch += 0x21c;
            } while ((int)ch < (int)((char *)ai_Chars + 2 + 768 * 0x21c));
        }
    }
    if ((f & 1) != 0 && n > 0) {
        j = 0;
        do {
            a = gm_FindChar(list[j].owner);
            if (a != 0 && (a[2] == 6 || a[2] == 7) && a[0x1b1] == 0) {
                f |= 2;
                break;
            }
            j++;
        } while (j < n);
    }
    if ((char)f & 2)
        return 1;
    return 0;
}
/* 0x4e8530, 1 when the office r (state 2) of the column col has two valid candidates and its x14 character is in the list of the column */

int amt_Unk4e8530(int col, AMTREC *r)
{
    AMTREC list[6];
    int n, j;
    char *a, *b;

    if (r->state == 2) {
        n = amt_GetColumnOffices(col, 6, list);
        a = gm_FindChar(r->x14);
        b = gm_FindChar(r->owner);
        if (a != 0 && (unsigned char)a[8] > 0 && a[0x1b1] == 0 && b != 0 && (unsigned char)b[8] > 0) {
            for (j = 0; j < n; j++) {
                if (list[j].owner == r->x14)
                    return 1;
            }
        }
    }
    return 0;
}
/* 0x4e85d0, brings the 30 offices and the characters in line after an election (main.c runs it when amt_CheckOfficeColumns decided nothing, cm.c at the
   end of the council session): an office whose owner is gone (or not alive: health +8 zero) is announced free (amt_SendOfficeFree) unless it is open
   (state 3) and the owner's officeKind (+0x166) is cleared; an office not in the state 1 is announced to its living owner (amt_SendOfficeMessage); then every
   application (appliedOffice +0x168) is cleared. The tail walks the records ai_Chars + 0x168 (the appliedOffice byte, 0x21c apart), ch - 0x168 is the record */

void amt_UpdateOffices(void)
{
    int r, v, i;
    AMTCHARVIEW *c;
    char *ch;

    for (i = 0; i < 30; i++) {
        c = (AMTCHARVIEW *)gm_FindChar(amt_Table[i].owner);
        r = -1;
        if (c == 0 || c->health == 0) {
            if (amt_Table[i].state != 3)
                r = amt_SendOfficeFree(amt_Table[i].idx);
            if (c != 0 && c->officeKind != 0) {
                v = 0;
                cm_BeginFieldBlock((char *)c, c->netId);
                cm_AddFieldDelta(1, 1, 0x166, &v);
                cm_SendFieldDeltas();
            }
        } else if (amt_Table[i].state != 1)
            r = amt_SendOfficeMessage((char)amt_Table[i].idx, (char *)c, 0, 1, 0xff);
        while (r != -1 && cm_GetSlotState(r) == 0)
            sv_PumpNet();
    }
    ch = (char *)ai_Chars + 0x168;
    do {
        if (ch[0] != 0) {
            v = 0;
            cm_BeginFieldBlock(ch - 0x168, ((AMTCHARVIEW *)(ch - 0x168))->netId);
            cm_AddFieldDelta(1, 1, 0x168, &v);
            cm_SendFieldDeltas();
        }
        ch += 0x21c;
    } while ((int)ch < (int)((char *)ai_Chars + 0x168 + 768 * 0x21c));
}
/* 0x4e86e0, 1 when one of the offices of the column 7 (the records 36 .. 1, at most 7 free ones skipped) has an owner who exists (the special-amount
   menu 0x5c1130 enables its list button with it) */

int amt_HasColumn7Holder(void)
{
    int i, n;

    n = 7;
    i = 36;
    do {
        if (n == 0)
            return 0;
        if (amt_Kinds[amt_Table[i].kind].col == 7) {
            if (amt_Table[i].owner != -1) {
                if (gm_FindChar(amt_Table[i].owner) != 0)
                    return 1;
            } else
                n--;
        }
        i--;
    } while (i != 0);
    return 0;
}
/* 0x4e8730, sends the office news 0x1c19 about the office p (+0 the index as an unsigned short, +2 the kind 5 .. 7, +9 a flag) with the number
   v + 0x253, or v + 0x27b (0x28 more) when the flag is set, to the characters of the kinds 6 and 7 (the text channels 0x5fd / 0x5fe) */

void amt_SendOfficeNews(unsigned short *p, int v)
{
    char buf[512];
    int *q;
    char k;

    k = ((char *)p)[2];
    if (k == 6 || k == 7 || k == 5) {
        txt_Format(buf, 0x1c19, *p, 0x1c1b, (((char *)p)[9] != 0 ? 0x27b : 0x253) + v);
        q = ai_CharInts[0];
        do {
            if (((char *)q)[-2] == 6)
                he_ShowNews(*q, -1, buf, 0, 0x5fd, ch_EmptyName);
            else if (((char *)q)[-2] == 7)
                he_ShowNews(*q, -1, buf, 0, 0x5fe, ch_EmptyName);
            q += 0x87;
        } while ((int)q < (int)((char *)ai_CharInts + 768 * 0x21c));
    }
}
/* 0x4e87e0, amt_SendOfficeNews with the text 0x1c21 (no second text): the character p lost the office v (the election replaces it, or it no longer
   fits the kind, 0x4e9260); nothing when p is 0 */

void amt_SendOfficeLost(unsigned short *p, int v)
{
    char buf[512];
    int *q;
    char k;

    if (p != 0 && ((k = ((char *)p)[2]) == 6 || k == 7 || k == 5)) {
        txt_Format(buf, 0x1c21, *p, (((char *)p)[9] != 0 ? 0x27b : 0x253) + v);
        q = ai_CharInts[0];
        do {
            if (((char *)q)[-2] == 6)
                he_ShowNews(*q, -1, buf, 0, 0x5fd, ch_EmptyName);
            else if (((char *)q)[-2] == 7)
                he_ShowNews(*q, -1, buf, 0, 0x5fe, ch_EmptyName);
            q += 0x87;
        } while ((int)q < (int)((char *)ai_CharInts + 768 * 0x21c));
    }
}
/* 0x4e88a0, elects the two deputy offices (the kinds 0x1c and 0x1d): the candidates are the owner characters (AIBLD.owner) of the buildings of type 4
   with the rank 1 .. 6 and of the buildings of type 0x10 with the rank 0x28 .. 0x2d, each character once, found through the build query 0x501190; when
   at least 3 were seen, the richest (gm_GetWorth 0x50a640) replaces the holder of the 0x1c office (its rank 1 .. 6) or, when the rank distance to the
   holder is above 5, gets the 0x1d office; a free office of the two is filled directly. The old holder gets the lost-office news (amt_SendOfficeLost)
   and the new one the office news (amt_SendOfficeNews). No return value, messages only.
   @deep B2 draft (255 code / 48 regs / 2 fill hunks, 417/896 differing bytes, 233 lines): the whole original 0x4e88a0..0x4e8c20 was read against the source (build/ghidra/4e88a0.c + tools/disasm) and every statement now matches - the two build queries (1,5,4) / (1,5,0x10) with the owner / 0xffff / player / flags5a guard, total and the 16-candidate dedupe, the rank ranges (>= 1 && <= 6, >= 0x28 && <= 0x2d), total >= 3 && n != 0, the 36..0 scan with the rep movs into a (0x1d) / b (0x1c), v = b.owner then a.owner, cur = 0 / gm_FindChar / gm_GetWorth, the best for-loop and the four send arms (d > 5 with the rank 1..6 -> b arm, the rank test after SendOfficeLost/News, the cur == 0 arm with b <- pick first). Left is the register allocation: original n=edi / best=ebx / j=ebp and -1 in ebx, ours n=ebx / best=ebp and -1 in eax (the permutation inverts the vote loop and the prologue from the first push; n's home slot [esp+0x10] already matches). spell.py dup-tail 4e8a33 / 4e8baa / 4e8c1b all describe that allocation; the structural leads were applied (the n > 0 wrapper pulled into the for - single test like the original; the init order a.owner/b.owner/j/n/total; ch = ai_Chars[id] hoisted before the rank compare). Q1-Q7 pass; readability done (AMTBLDVIEW pack(1) added to amt.h). Left for match-deep: c2prio register priority and the slot/register permutation. */

void amt_ElectDeputies(void)
{
    AMTREC a, b;
    char *cand[16];
    char *it, *cur, *pick, *cp, *ch;
    int n, total, i, j, left, best, s, rb, rc, d, v;
    unsigned short id;
    unsigned char k;

    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    memset(cand, 0, sizeof(cand));
    a.owner = -1;
    b.owner = -1;
    j = 0;
    n = 0;
    total = 0;
    it = gm_BuildQuery(1, 5, 4);
    while (it != 0 && n < 16) {
        id = ((AMTBLDVIEW *)it)->owner;
        if (id != 0xffff && id != *ai_PlayerChar && (((AMTBLDVIEW *)it)->flags5a & 1) == 0) {
            total++;
            ch = ai_Chars[id];
            k = ((AMTCHARVIEW *)ch)->rank;
            if (k >= 1 && k <= 6) {
                for (i = 0; i < 16; i++) {
                    if (cand[i] == ch)
                        break;
                }
                if (i == 16) {
                    cand[n] = ch;
                    n++;
                }
            }
        }
        it = gm_BuildNext();
    }
    it = gm_BuildQuery(1, 5, 0x10);
    while (it != 0 && n < 16) {
        id = ((AMTBLDVIEW *)it)->owner;
        if (id != 0xffff && id != *ai_PlayerChar && (((AMTBLDVIEW *)it)->flags5a & 1) == 0) {
            total++;
            ch = ai_Chars[id];
            k = ((AMTCHARVIEW *)ch)->rank;
            if (k >= 0x28 && k <= 0x2d) {
                for (i = 0; i < 16; i++) {
                    if (cand[i] == ch)
                        break;
                }
                if (i == 16) {
                    cand[n] = ch;
                    n++;
                }
            }
        }
        it = gm_BuildNext();
    }
    if (total >= 3 && n != 0) {
        left = 2;
        i = 36;
        do {
            if (left == 0)
                break;
            if (amt_Table[i].kind == 0x1d) {
                a = amt_Table[i];
                left--;
            } else if (amt_Table[i].kind == 0x1c) {
                b = amt_Table[i];
                left--;
            }
            i--;
        } while (i >= 0);
        cur = 0;
        v = b.owner;
        if (v == -1)
            v = a.owner;
        if (v == -1) {
            best = 0;
        } else {
            cur = gm_FindChar(v);
            if (cur == 0)
                best = 0;
            else
                best = gm_GetWorth(((AMTCHARVIEW *)cur)->id);
        }
        pick = 0;
        for (j = 0; j < n; j++) {
            cp = cand[j];
            s = gm_GetWorth(((AMTCHARVIEW *)cp)->id);
            if (s > best) {
                pick = cp;
                best = s;
            }
        }
        if (pick != 0 && pick != cur) {
            if (cur != 0) {
                rb = ((AMTCHARVIEW *)pick)->rank;
                rc = ((AMTCHARVIEW *)cur)->rank;
                d = rc - rb;
                if (d < 0)
                    d = -d;
                if (d > 5) {
                    if (rb == 0 || rb > 6) {
                        amt_SendOfficeMessage((char)b.idx, 0, 0, 1, 0xff);
                        amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                        v = 0x1d;
                    } else {
                        amt_SendOfficeMessage((char)b.idx, pick, 0, 1, 0xff);
                        amt_SendOfficeMessage((char)a.idx, 0, 0, 1, 0xff);
                        v = 0x1c;
                    }
                    amt_SendOfficeLost((unsigned short *)cur, ((AMTCHARVIEW *)cur)->secondOffice);
                    amt_SendOfficeNews((unsigned short *)pick, v);
                    return;
                }
                amt_SendOfficeLost((unsigned short *)cur, ((AMTCHARVIEW *)cur)->secondOffice);
                amt_SendOfficeNews((unsigned short *)pick, ((AMTCHARVIEW *)cur)->secondOffice);
                if (((AMTCHARVIEW *)pick)->rank != 0 && ((AMTCHARVIEW *)pick)->rank < 7) {
                    amt_SendOfficeMessage((char)b.idx, pick, 0, 1, 0xff);
                    return;
                }
                amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                return;
            }
            if (((AMTCHARVIEW *)pick)->rank >= 1 && ((AMTCHARVIEW *)pick)->rank <= 6) {
                amt_SendOfficeMessage((char)b.idx, pick, 0, 1, 0xff);
                v = 0x1c;
            } else {
                amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                v = 0x1d;
            }
            amt_SendOfficeNews((unsigned short *)pick, v);
        }
    }
}
/* 0x4e8c20, elects the office of the kind 0x22: the candidates are the owner characters (AIBLD.owner) of the buildings of type 7 with the rank 13 .. 18
   (the job group 7 of gm_TitleGroup), each character once, found through the build query 0x501190; when at least 3 were seen, the richest (gm_GetWorth
   0x50a640) replaces the holder of the office. The old holder gets the lost-office news (amt_SendOfficeLost), the new one the office news 0x22. No return
   value, messages only. */

void amt_ElectKind22(void)
{
    AMTREC a;
    char *cand[16];
    char *it, *cur, *pick, *cp, *ch;
    int n, total, i, j, left, best, s;
    unsigned short id;
    unsigned char k;

    memset(&a, 0, sizeof(a));
    memset(cand, 0, sizeof(cand));
    a.owner = -1;
    j = 0;
    n = 0;
    total = 0;
    it = gm_BuildQuery(1, 5, 7);
    if (it == 0)
        return;
    do {
        if (n >= 16)
            break;
        id = ((AMTBLDVIEW *)it)->owner;
        if (id != 0xffff && (((AMTBLDVIEW *)it)->flags5a & 1) == 0) {
            total++;
            ch = ai_Chars[id];
            k = ((AMTCHARVIEW *)ch)->rank;
            if (k >= 0xd && k <= 0x12) {
                for (i = 0; i < 16; i++) {
                    if (cand[i] == ch)
                        break;
                }
                if (i == 16) {
                    cand[n] = ch;
                    n++;
                }
            }
        }
        it = gm_BuildNext();
    } while (it != 0);
    if (total < 3)
        return;
    if (n == 0)
        return;
    left = 1;
    i = 36;
    do {
        if (left == 0)
            break;
        if (amt_Table[i].kind == 0x22) {
            left--;
            a = amt_Table[i];
        }
        i--;
    } while (i >= 0);
    cur = 0;
    if (a.owner == -1 || (cur = gm_FindChar(a.owner)) == 0)
        best = 0;
    else
        best = gm_GetWorth(((AMTCHARVIEW *)cur)->id);
    pick = 0;
    for (j = 0; j < n; j++) {
        cp = cand[j];
        s = gm_GetWorth(((AMTCHARVIEW *)cp)->id);
        if (s > best) {
            pick = cp;
            best = s;
        }
    }
    if (pick != 0 && pick != cur) {
        if (cur != 0) {
            amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
            amt_SendOfficeLost((unsigned short *)cur, 0x22);
            amt_SendOfficeNews((unsigned short *)pick, 0x22);
            return;
        }
        amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
        amt_SendOfficeNews((unsigned short *)pick, 0x22);
    }
}
/* 0x4e8dc0, fills the office of the kind `kind` (0x1e .. 0x21, the mayors of the four towns) from the characters on the screen objects of the matching kind with
   the ranks of the town: the best by 0x50a640 takes it. When the town's object (0x17 .. 0x1a) does not exist the town is built: a building (0x38 / 0x3b / 0x3e / 0x41) is
   picked from the building records, a free place is looked for ("amt_calc_zuenfte(): could not find a free place...") and the order message (0xd) is sent
   (0x4c4b70). The name is the function's own error string at 0x60f11c.
   @deep B3 far draft, 919/1200 bytes (our function is 32 B longer than the original 1168), 209 differing instructions of 351 (ours 356). This session fixed the frame: the original's AMTREC a (0x1c), pos (0x1c..0x27, overlapping a - a is dead on the return path) and v3 (0x34) give 0x368; our pos[6] and the function-scope pos/v3 gave 0x400 - int pos[3] + pos/v3 declared inside the build branch (+ nm[128] removed, the gb_ name built in buf) now give the same 0x368 frame and the first 14 instructions match. Also int lo/hi (the original stores both as dwords at [esp+0x10]/[esp+0x18]; as unsigned char they were byte stores) - 251 -> 209 differing instructions, but our code is 32 B longer (unitdiff MIXED) because the block-local declarations changed the allocation. Left: allocation only - hi enregistered in ebx vs the original's [esp+0x18], cand[16] at 0x138 vs 0x40, n/edi vs ebx (the whole kind selection and memset block). Q4 blocked header: the 26 raw accesses are the message build (type 0xd) - AIMSG in ai.h has type/to/fromId/time/flags/arg at the same offsets but NOT pos at 0x80 (inside arg[10]) or angles at 0x90 or name at 0x9c, and am.c's ai_Unk4a5320 (amt_ExecGebaeudeBauen) uses the same raw casts, so the view belongs in ai.h + am.c (shared-header decision). Q2/Q7 blocked: d3if_SelectScene = d3if_SelectScene, d3_CurrentScene = d3_CurrentScene, err_Message = err_Message are one-line renames (prototypes in am.h/d3.h), but d3_PointToWorld = d3_PointToWorld needs the canonical (D3OBJECT *, D3VECTOR *, D3VECTOR *) with the call site passing an int object pointer and (char *)j + 0x4c - a type debt; ai_BuildingName / ai_FindBauplaetze are now real prototypes in amt.h (Q7 for those two gone). Q1/Q5 done this session: amt_CalcZuenfte (the function's own error string at 0x60f11c) and ROUGH removed. */

void amt_CalcZuenfte(unsigned int kind)
{
    AMTREC a;
    char *cand[16];
    char *it, *cur, *pick, *cp, *q;
    int n, total, i, j, best, s, bld, cnt, scene, k1, k2;
    unsigned char k;
    int lo, hi;
    unsigned short id;
    char buf[128], name[128];
    int list[64];
    char msg[0xf8];

    memset(&a, 0, sizeof(a));
    a.owner = -1;
    if (kind == 0x1e) {
        k1 = 0xe; k2 = 0x17; lo = 0x22; hi = 0x27; bld = 0x38;
    } else if (kind == 0x1f) {
        k1 = 0x12; k2 = 0x18; lo = 0x2e; hi = 0x33; bld = 0x3b;
    } else if (kind == 0x20) {
        k1 = 0x14; k2 = 0x19; lo = 0x3a; hi = 0x3f; bld = 0x3e;
    } else {
        if (kind != 0x21)
            return;
        k1 = 0x15; k2 = 0x1a; lo = 0x40; hi = 0x45; bld = 0x41;
    }
    memset(cand, 0, sizeof(cand));
    n = 0;
    total = 0;
    it = gm_BuildQuery(1, 5, k1);
    if (it != 0) {
        do {
            if (n > 15)
                break;
            id = *(unsigned short *)(it + 0x27);
            if (id != 0xffff && (it[0x5a] & 1) == 0) {
                total++;
                k = ai_Chars[id][0x164];
                if (lo <= k && k <= hi) {
                    for (i = 0; i < 16; i++) {
                        if (cand[i] == ai_Chars[id])
                            break;
                    }
                    if (i == 16) {
                        cand[n] = ai_Chars[id];
                        n++;
                    }
                }
            }
            it = gm_BuildNext();
        } while (it != 0);
        if (total > 2 && n != 0) {
            it = gm_BuildQuery(1, 5, k2);
            scene = d3_CurrentScene;
            if (it == 0) {
                int pos[3], v3[3];

                d3if_SelectScene(0, 1);
                q = ai_BTypes + bld * 0x24d;
                ai_BuildingName(bld, buf);
                cnt = ai_FindBauplaetze(buf, list);
                j = list[util_RandomU(cnt) & 0xffff];
                i = 0;
                while (j == 0) {
                    if (cnt <= i) {
                        err_Message("amt_calc_zuenfte(): could not find a free place...");
                        return;
                    }
                    if (list[i] != 0)
                        j = list[i];
                    i++;
                }
                d3_PointToWorld(j, j + 0x4c, pos);
                ai_ObjectAngle(v3, j);
                d3if_SelectScene(scene, 1);
                strcpy(buf, "gb_");
                strcat(buf, q + 1);
                _strupr(buf + 2);
                sprintf(name, "%sgebaeude/*%s.ogr", ai_DataPath, buf);
                memset(msg, 0, sizeof(msg));
                msg[4] = 0xd;
                *(int *)(msg + 0x28) = *(int *)ai_Time;
                *(int *)(msg + 0x8) = *(int *)(*(char **)&ai_PlayerChar + 4);
                *(int *)(msg + 0xc) = -1;
                *(int *)(msg + 0x2c) = *(int *)&ai_TimeHour;
                *(int *)(msg + 0x30) = *(int *)(ai_Time + 8);
                *(short *)(msg + 0x34) = *(short *)(ai_Time + 12);
                msg[0x36] = 2;
                *(int *)(msg + 0x80) = pos[0];
                *(int *)(msg + 0x58) = *(int *)(*(char **)&ai_PlayerChar + 4);
                *(int *)(msg + 0x88) = pos[2];
                *(int *)(msg + 0x84) = pos[1];
                *(int *)(msg + 0x90) = v3[0];
                *(int *)(msg + 0x5c) = bld;
                *(int *)(msg + 0x60) = 0;
                *(int *)(msg + 0x94) = v3[1];
                *(int *)(msg + 0x98) = v3[2];
                strcpy(msg + 0x9c, name);
                cm_SendLongMessage(msg);
                return;
            }
            if ((it[0x5a] & 1) == 0) {
                i = 36;
                j = 1;
                do {
                    if (j == 0)
                        break;
                    if (amt_Table[i].kind == kind) {
                        j--;
                        a = amt_Table[i];
                    }
                    i--;
                } while (i >= 0);
                best = 0;
                cur = 0;
                if (a.owner != -1 && (cur = gm_FindChar(a.owner)) != 0)
                    best = gm_GetWorth(*(unsigned short *)cur);
                pick = 0;
                if (n > 0) {
                    for (j = 0; j < n; j++) {
                        cp = cand[j];
                        s = gm_GetWorth(*(unsigned short *)cp);
                        if (best < s) {
                            best = s;
                            pick = cp;
                        }
                    }
                    if (pick != 0 && pick != cur) {
                        if (cur != 0) {
                            amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                            amt_SendOfficeLost((unsigned short *)cur, kind);
                            amt_SendOfficeNews((unsigned short *)pick, kind);
                            return;
                        }
                        amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                        amt_SendOfficeNews((unsigned short *)pick, kind);
                    }
                }
            }
        }
    }
}
/* 0x4e9260, the office check run at the start of a round (main 0x53b2e0): every holder of a column-7 office (0x4e70f0: the deputies 0x1c / 0x1d,
   the town offices 0x1e .. 0x21, 0x22) whose character is gone, dead, holds another kind (+0x169) or is outside the kind's rank range
   (1 .. 6, 0x28 .. 0x2d, 0x22 .. 0x45, 0xd .. 0x12) loses the office with the kind-4 message; then, when the event 0x80 is not running,
   the four town offices are recalculated and the deputies and the 0x22 office are elected. No return value. */

void amt_CheckOffices(void)
{
    AMTREC list[7];
    AMTREC *p;
    int n, r, lo, hi;
    AMTCHARVIEW *c;

    n = amt_GetColumn7Offices(1, 7, list);
    for (p = list; n > 0; n--, p++) {
        if (p->owner != -1) {
            switch (p->kind) {
            case 0x1c: lo = 1; hi = 6; break;
            case 0x1d: lo = 0x28; hi = 0x2d; break;
            case 0x1e: lo = 0x22; hi = 0x27; break;
            case 0x1f: lo = 0x2e; hi = 0x33; break;
            case 0x20: lo = 0x3a; hi = 0x3f; break;
            case 0x21: lo = 0x40; hi = 0x45; break;
            case 0x22: lo = 0xd; hi = 0x12; break;
            default:
                continue;
            }
            c = (AMTCHARVIEW *)gm_FindChar(p->owner);
            if (c != 0 && c->health != 0 && (char)c->secondOffice == (char)p->kind &&
                c->rank >= lo && c->rank <= hi)
                continue;
            amt_SendOfficeLost((unsigned short *)c, p->kind);
            r = amt_SendOfficeMessage((char)p->idx, 0, 0, 4, 0xff);
            while (r != -1 && cm_GetSlotState(r) == 0)
                sv_PumpNet();
        }
    }
    if (he_FindEvent(1, 0, 0x80) == 0) {
        amt_CalcZuenfte(0x1e);
        amt_CalcZuenfte(0x1f);
        amt_CalcZuenfte(0x20);
        amt_CalcZuenfte(0x21);
    }
    amt_ElectDeputies();
    amt_ElectKind22();
}
/* 0x4e9400, 1 when the town object of the title group k (1 .. 3, 5: the buildings 0x18, 0x19, 0x1a, 0x17) exists and is not flagged
   (+0x5a bit 0); with the net flag 0x80 set it recalculates the four town offices instead and returns 0 (the income collector 0x4fa4b0) */

int amt_HasTownObject(int k)
{
    int v;
    AMTBLDVIEW *it;

    if (he_FindEvent(1, 0, 0x80) != 0) {
        amt_CalcZuenfte(0x1e);
        amt_CalcZuenfte(0x1f);
        amt_CalcZuenfte(0x20);
        amt_CalcZuenfte(0x21);
        return 0;
    }
    switch (k) {
    case 1: v = 0x18; break;
    case 2: v = 0x19; break;
    case 3: v = 0x1a; break;
    default:
        return 0;
    case 5: v = 0x17;
    }
    it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, v);
    if (it != 0) {
        if ((it->flags5a & 1) == 0)
            return 1;
    }
    return 0;
}
/* 0x4e94a0, a mayor's (+0x169 = 0x1e .. 0x21) bonus step: -1 when the bit 0x4000000 of +0x1c8 is set, else 1 or -2 by the money +0x194 > 2 */

int amt_MayorBonusStep(char *c)
{
    if ((unsigned char)c[0x169] >= 0x1e && (unsigned char)c[0x169] <= 0x21) {
        if ((*(unsigned int *)(c + 0x1c8) & 0x4000000) != 0)
            return -1;
        return (((*(int *)(c + 0x194) >= 3) - 1) & -3) + 1;
    }
    return 0;
}
/* 0x4e94e0, whether the town mayor c (secondOffice 0x1e .. 0x21) may use the second step of the town's guild-house power: the town's
   building (0x17 .. 0x1a by the office) must exist, be unflagged and of a type whose level (+0x247) is 2 or more, and c must have 5+
   money; returns 1 when it may, -1 when the power is used already (bit 0x8000000 of +0x1c8), 0 when the building does not qualify
   (the AI decide callback 0x4a0090, the office window 0x5a0f10 / 0x5a1000). No side effects */

int amt_CanUseMayorPower2(char *c)
{
    AMTCHARVIEW *cv;
    int v;
    AMTBLDVIEW *it;

    cv = (AMTCHARVIEW *)c;
    if (cv->secondOffice >= 0x1e && cv->secondOffice <= 0x21) {
        switch (cv->secondOffice) {
        case 0x1f: v = 0x18; break;
        case 0x20: v = 0x19; break;
        case 0x21: v = 0x1a; break;
        case 0x1e: v = 0x17; break;
        default:
            return 0;
        }
        it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, v);
        if (it != 0 && (it->flags5a & 1) == 0 && (unsigned char)ai_BTypes[it->type * 0x24d + 0x247] >= 2) {
            if ((cv->powerFlags & 0x8000000) != 0)
                return -1;
            return (((cv->money >= 5) - 1) & -3) + 1;
        }
    }
    return 0;
}
/* 0x4e95a0, the third step of the same power (a type level of 3+, bit 0x10000000, money 8+); see amt_CanUseMayorPower2 */

int amt_CanUseMayorPower3(char *c)
{
    AMTCHARVIEW *cv;
    int v;
    AMTBLDVIEW *it;

    cv = (AMTCHARVIEW *)c;
    if (cv->secondOffice >= 0x1e && cv->secondOffice <= 0x21) {
        switch (cv->secondOffice) {
        case 0x1f: v = 0x18; break;
        case 0x20: v = 0x19; break;
        case 0x21: v = 0x1a; break;
        case 0x1e: v = 0x17; break;
        default:
            return 0;
        }
        it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, v);
        if (it != 0 && (it->flags5a & 1) == 0 && (unsigned char)ai_BTypes[it->type * 0x24d + 0x247] >= 3) {
            if ((cv->powerFlags & 0x10000000) != 0)
                return -1;
            return (((cv->money >= 8) - 1) & -3) + 1;
        }
    }
    return 0;
}
/* 0x4e9660, executes the guild-house price message m (kind 3, +4 the job number 1 .. 7, +8 = 1 / -1 raise / lower) of the character c:
   the best building of the district (the type's level +0x247) selects the row of amt_PriceRows (0x15 bytes: the building kind and 10 goods
   ids), gm_Shuffled picks one or two of those goods and their price records (gm_FindGoods) get the factor gm_GetSkill(c, 2) times a random
   1.2 .. 2.0 (raise) or 0.5 .. 1.0 (lower); the goods go back through cm_SendGoodsDelta, m[3] / m[4] return them, c uses its power flag
   0x4000000 and pays 3 money (gm_SendMoneyDelta), every player character except c is told the news text (0x1655 / 0x1659 + the goods
   names) and c gets it as a message box when it is a player character (kind 6). Always 0. */

int amt_ExecutePriceMessage(char *c, int *m)
{
    int lvl, best;
    AMTBLDVIEW *it;
    unsigned int id, id2, r;
    int i, n, t, pick[10], cands[10];
    unsigned short *row;
    float f;
    AMTGOODS rec;
    AMTGOODS *p;
    int *q;
    char buf[512], msg[2048];

    best = 0;
    id = 0;
    id2 = 0;
    m[3] = 0;
    m[4] = 0;
    if (m[0] == 3 && (m[2] == 1 || m[2] == -1) && (unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 7) {
        t = gm_JobBuildingType(m[1]);
        if (t != 0) {
            it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, t);
            if (it != 0) {
                do {
                    lvl = (unsigned char)ai_BTypes[it->type * 0x24d + 0x247];
                    if (lvl > best) {
                        best = lvl;
                        id = it->type;
                    }
                    it = (AMTBLDVIEW *)gm_BuildNext();
                } while (it != 0);
                if (id != 0) {
                    for (i = 0; i < 0x25; i++) {
                        if (amt_PriceRows[i * 0x15] == id)
                            break;
                    }
                    if (i < 0x25) {
                        n = 0;
                        row = (unsigned short *)&amt_PriceRows[1 + i * 0x15];
                        for (t = 0; t < 10; t++, row++) {
                            if (*row != 0) {
                                cands[n] = *row;
                                n++;
                            }
                        }
                        if (n != 0) {
                            gm_Shuffled(n, pick);
                            if (n == 1)
                                id = cands[pick[0]];
                            else {
                                id = cands[pick[0]];
                                id2 = cands[pick[1]];
                            }
                            f = gm_GetSkill(c, 2);
                            if (m[2] == 1)
                                f = (util_RandomF() + 1.0f) * f * 0.4f + 1.2f;
                            else
                                f = 1.0 - ((util_RandomF() + 1.0f) * f * 0.3f + 0.2);
                            p = (AMTGOODS *)gm_FindGoods(ai_OwnPlayer, id);
                            if (p != 0) {
                                rec = *p;
                                rec.mean = rec.amount * f;
                                rec.price = gm_GoodsValue(ai_OwnPlayer, id);
                                cm_SendGoodsDelta((struct GMSTOCK *)&rec, ai_OwnPlayer);
                                m[3] = id;
                                p = (AMTGOODS *)gm_FindGoods(ai_OwnPlayer, id2);
                                if (p != 0) {
                                    rec = *p;
                                    rec.mean = rec.amount * f;
                                    rec.price = gm_GoodsValue(ai_OwnPlayer, id2);
                                    cm_SendGoodsDelta((struct GMSTOCK *)&rec, ai_OwnPlayer);
                                    m[4] = id2;
                                }
                                cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x4000000, 0);
                                gm_SendMoneyDelta(AMT_NETID(c), -3);
                                r = util_RandomU(3) & 0xffff;
                                if (m[2] == 1) {
                                    txt_Format(buf, r + 0x1656, m[1] + 0x1648);
                                    txt_Format(msg, 0x1655, buf, id * 2 + 0x9a8, id2 * 2 + 0x9a8);
                                } else {
                                    txt_Format(buf, r + 0x165a, m[1] + 0x1648);
                                    txt_Format(msg, 0x1659, buf, id * 2 + 0x9a8, id2 * 2 + 0x9a8);
                                }
                                for (i = 0; i < 768; i++) {
                                    q = ai_CharInts[i];
                                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && AMT_NETID(c) != *q)
                                        he_ShowNews(*q, -1, msg, 1, 0x5ec, ch_EmptyName);
                                }
                                if (AMT_KIND(c) == 6)
                                    sv_MessageBox(msg, 0x10);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x4e9a00, executes the guild-house gift message m (kind 2, +4 the object id, +8 the goods amount) of the character c: when the object's
   owner (+0x27) is not c and not -1, c's gift (the skill 4 of c, times a random factor 1 .. 2, times 15, plus 5, times 2.55) costs every
   character at the object (the +0x16c pointer, the owner itself skipped) that many relations points (cm_SendRelations), the amount m[2]
   leaves c (cm_SendMoveGoodsUnchecked) and m[2] / count + 0x20 goes back to each of them; c pays 5 money and uses its power flag
   0x8000000, the owner gets the text 0x1663 as a quick jump, every player character the text 0x1662 (channel 0x5fd) and c the text 0x1661
   as a message box. Returns 1 when done, 0 for another message, a missing object or its own / no owner. */

int amt_ExecuteShareMessage(short *c, int *m)
{
    AMTBLDVIEW *o;
    int loss, cnt, i;
    unsigned int k;
    float f;
    int *q;
    char msg[1024];

    if (m[0] == 2) {
        gm_FindById((int *)&o, 0, 0, m[1]);
        if (o != 0) {
            if (o->owner != (unsigned short)*c && o->owner != 0xffff) {
                f = gm_GetSkill((char *)c, 4);
                f = (util_RandomF() + 1.0f) * f * 15.0f;
                loss = (int)((f + 5.0) * 2.55f);
                cm_SendMoveGoodsUnchecked(-1, AMT_NETID(c), ai_OwnPlayer, m[2]);
                k = 0;
                cnt = 0;
                q = (int *)((char *)ai_CharInts + 0x168);
                do {
                    if ((unsigned short)q[-0x5b] != 0xffff && *q == (int)o && o->owner != k) {
                        cm_SendRelations(ai_CharInts[o->owner][0], q[-0x5a], -loss, 1);
                        cnt++;
                    }
                    q += 0x87;
                    k++;
                } while ((int)q < (int)AMT_CHARS_END + 0x168);
                if (cnt != 0) {
                    loss = (int)((float)m[2] / cnt) + 0x20;
                    q = (int *)((char *)ai_CharInts + 0x168);
                    k = 0;
                    do {
                        if ((unsigned short)q[-0x5b] != 0xffff && *q == (int)o && o->owner != k)
                            cm_SendMoveGoodsUnchecked(q[-0x5a], -1, ai_OwnPlayer, loss);
                        q += 0x87;
                        k++;
                    } while ((int)q < (int)AMT_CHARS_END + 0x168);
                }
                gm_SendMoneyDelta(AMT_NETID(c), -5);
                cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x8000000, 0);
                if (ai_Chars[o->owner][2] == 7 || ai_Chars[o->owner][2] == 6) {
                    txt_Format(msg, 0x1663, o);
                    he_MessageBoxQuickjump(ai_CharInts[o->owner][0], -1, msg, 0, 0x5fb, o->id, -1, 0, ch_EmptyName);
                }
                txt_Format(msg, 0x1662, o->owner, o);
                q = ai_CharInts[0];
                do {
                    if (((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) {
                        if (AMT_NETID(c) != *q && *q != ai_CharInts[o->owner][0])
                            he_ShowNews(*q, -1, msg, 0, 0x5fd, ch_EmptyName);
                    }
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
                if (((char *)c)[2] == 6) {
                    txt_Format(msg, 0x1661, o->owner, o);
                    sv_MessageBox(msg, 0);
                }
                return 1;
            }
        }
    }
    return 0;
}

/* 0x4e9ce0, executes the office-power request m of the mayor c, picked by the office c[0x169] (the offices 0x1e .. 0x21):
   0x1e (m = {1, target}): the target (a character with an office, not c) is denounced - its office goes out as the office
   message 4 and its opinion of c drops; 0x1f / 0x21 (m = {4, district 1 .. 6}): every pair of the district's office owners
   loses the damage roll of c's skill, the opinion of both ways; 0x20: the same one-sided (only the office owners' opinion of c).
   Every power takes 8 money of c (cm_SendMoneyDelta), sets the power bit 0x10000000 of +0x1c8 and tells the news (texts
   0x166e .. 0x1685, the channels 0x5fb / 0x5fd / 0x5fe) to the other characters and, when the kind of c is 6, to c itself.
   Returns -1 when the damage roll is <= 0.3 and 1 otherwise, 0 when the message does not fit the office kind.
   @deep read against the original in full and rewritten: ret as if/else in a register, the tell loops
   inline (re-reading t/c netId, no -2 compare in the 0x20 case), gm_SendMoneyDelta (the thunk the original
   calls), GetColumnOffices in both rf branches, no n!=1 guard, duplicated branch tails with their own
   returns; message/loop/return layout now equal. Left: allocation - c sits in ebx (original esi) and t in
   esi (original ebx), so the 0x20 dmg loop puts dmg in esi and reloads c from [esp+0x8f0] (asmdiff
   0x4ea04a: ours mov ecx,[esp+0x8f0]/mov ecx,[ecx+4], original mov ecx,[esi+4]; 0x4ea07a ours reloads c
   into ebx before gm_SendMoneyDelta), m kept in ebp in the 0x1f/0x20 heads (original eax, reloaded into
   ebx later), rf slot 0x14 vs 0x10; declaration order tried, no change (size: 660 vs 646 instructions,
   explained: ours=660 original=646: the 14 extra are the reloads of c and m from the stack (mov
   ebx,[esp+0x8f0], mov edx,[esp+0x8f0] in the dmg loops, mov ebp,[esp+0x8f4]) the other register
   assignment costs) */
int amt_ExecuteMayorPowerMessage(char *c, int *m)
{
    float rf, f;
    int ret, n, dmg, i, j, id;
    char kind;
    char *t;
    int *q;
    AMTREC o;
    AMTREC list[6];
    AMTKIND k;
    char msg[1024], msg2[1024];

    rf = util_RandomF();
    if (rf > 0.3f)
        ret = 1;
    else
        ret = -1;
    kind = c[0x169];
    if (kind == 0x1e) {
        if (m[0] == 1 && (t = gm_FindChar(m[1])) != 0 && t[0x166] != 0 && t != c && amt_FindOffice(t, &k, &o) != 0) {
            if (rf > 0.3f)
                amt_SendOfficeMessage((char)o.idx, 0, 0, 4, 0xff);
            gm_SendMoneyDelta(AMT_NETID(c), -8);
            cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x10000000, 0);
            kind = t[2];
            if (rf > 0.3f) {
                if (kind == 7 || kind == 6) {
                    txt_Format(msg, 0x1684);
                    he_ShowNews(AMT_NETID(t), -1, msg, 0, 0x5fb, ch_EmptyName);
                }
                txt_Format(msg, 0x1682, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                q = ai_CharInts[0];
                do {
                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(t) && *q != AMT_NETID(c))
                        he_ShowNews(*q, -1, msg, 0, 0x5fe, ch_EmptyName);
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
                if (c[2] == 6) {
                    txt_Format(msg, 0x1680, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                    sv_MessageBox(msg, 0);
                }
                return ret;
            } else {
                if (kind == 7 || kind == 6) {
                    txt_Format(msg, 0x1685);
                    he_ShowNews(AMT_NETID(t), -1, msg, 0, 0x5fb, ch_EmptyName);
                }
                txt_Format(msg, 0x1683, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                q = ai_CharInts[0];
                do {
                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(t) && *q != AMT_NETID(c))
                        he_ShowNews(*q, -1, msg, 0, 0x5fe, ch_EmptyName);
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
                if (c[2] == 6) {
                    txt_Format(msg, 0x1681, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                    sv_MessageBox(msg, 0);
                }
            }
            return ret;
        }
    } else if (kind == 0x1f || kind == 0x21) {
        if (m[0] == 4 && (unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 6) {
            if (rf > 0.3f) {
                n = amt_GetColumnOffices(m[1], 6, list);
                f = gm_GetSkill(c, 4);
                f = (util_RandomF() + 1.0f) * f * 15.0f;
                dmg = (int)((f + 5.0) * 2.55f);
                for (i = 0; i < n - 1; i++) {
                    id = list[i].owner;
                    if (id != AMT_NETID(c) && id != -1) {
                        for (j = i + 1; j < n; j++) {
                            if (list[j].owner != AMT_NETID(c) && list[j].owner != -1) {
                                cm_SendRelations(list[j].owner, id, -dmg, 1);
                                cm_SendRelations(id, list[j].owner, -dmg, 1);
                            }
                        }
                    }
                }
            } else {
                n = amt_GetColumnOffices(m[1], 6, list);
            }
            gm_SendMoneyDelta(AMT_NETID(c), -8);
            cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x10000000, 0);
            if (rf > 0.3f) {
                txt_Format(msg, 0x1672, m[1] + 0x1663);
                txt_Format(msg2, 0x1670, m[1] + 0x1663);
                q = ai_CharInts[0];
                do {
                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(c)) {
                        for (i = 0; i < n; i++) {
                            if (list[i].owner == *q)
                                break;
                        }
                        if (i < n)
                            he_ShowNews(*q, -1, msg, 0, 0x5fb, ch_EmptyName);
                        else
                            he_ShowNews(*q, -1, msg2, 0, 0x5fe, ch_EmptyName);
                    }
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
                if (c[2] == 6) {
                    txt_Format(msg, 0x166e, m[1] + 0x1663);
                    sv_MessageBox(msg, 0);
                }
                return ret;
            } else {
                txt_Format(msg, 0x1673, m[1] + 0x1663);
                txt_Format(msg2, 0x1671, m[1] + 0x1663);
                q = ai_CharInts[0];
                do {
                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(c)) {
                        for (i = 0; i < n; i++) {
                            if (list[i].owner == *q)
                                break;
                        }
                        if (i < n)
                            he_ShowNews(*q, -1, msg, 0, 0x5fb, ch_EmptyName);
                        else
                            he_ShowNews(*q, -1, msg2, 0, 0x5fe, ch_EmptyName);
                    }
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
                if (c[2] == 6) {
                    txt_Format(msg, 0x166f, m[1] + 0x1663);
                    sv_MessageBox(msg, 0);
                }
            }
            return ret;
        }
    } else if (kind == 0x20 && m[0] == 4 && (unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 6) {
        if (rf > 0.3f) {
            n = amt_GetColumnOffices(m[1], 6, list);
            f = gm_GetSkill(c, 4);
            f = (util_RandomF() + 1.0f) * f * 15.0f;
            dmg = (int)((f + 5.0) * 2.55f);
            for (i = 0; i < n; i++) {
                id = list[i].owner;
                if (id != AMT_NETID(c) && id != -1)
                    cm_SendRelations(AMT_NETID(c), id, dmg, 1);
            }
        }
        gm_SendMoneyDelta(AMT_NETID(c), -8);
        cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x10000000, 0);
        if (rf > 0.3f) {
            txt_Format(msg, 0x167a, m[1] + 0x1663);
            q = ai_CharInts[0];
            do {
                if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(c))
                    he_ShowNews(*q, -1, msg, 0, 0x5fd, ch_EmptyName);
                q += 0x87;
            } while ((int)q < (int)AMT_CHARS_END);
            if (c[2] == 6) {
                txt_Format(msg, 0x1678, m[1] + 0x1663);
                sv_MessageBox(msg, 0);
            }
        } else {
            txt_Format(msg, 0x167b, m[1] + 0x1663);
            q = ai_CharInts[0];
            do {
                if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(c))
                    he_ShowNews(*q, -1, msg, 0, 0x5fe, ch_EmptyName);
                q += 0x87;
            } while ((int)q < (int)AMT_CHARS_END);
            if (c[2] == 6) {
                txt_Format(msg, 0x1679, m[1] + 0x1663);
                sv_MessageBox(msg, 0);
            }
        }
        return ret;
    }
    return 0;
}
/* 0x4ea550, changes the opinion (cm_SendRelations, by v) of the owners of the offices of the district d (1 .. 7) of the
   character c, except c's own office */

void amt_ChangeDistrictOpinion(char *c, int d, int v)
{
    AMTREC list[6];
    int n, i;

    if (d >= 1 && d <= 7) {
        n = amt_GetColumnOffices(d, 6, list);
        if (n > 0) {
            for (i = 0; i < n; i++) {
                if (list[i].owner != AMT_NETID(c) && list[i].owner != -1)
                    cm_SendRelations(AMT_NETID(c), list[i].owner, v, 1);
            }
        }
    }
}
/* 0x4ea5c0, the mean liking (gm_Liking / 100) of the owners of the offices of the district d (1 .. 7) for the
   character c; 0 when there is none */

float amt_GetDistrictLiking(short *c, int d)
{
    AMTREC list[6];
    float sum;
    int n, i, cnt;
    short *o;

    sum = 0.0f;
    if (d >= 1 && d <= 7) {
        n = amt_GetColumnOffices(d, 6, list);
        cnt = 0;
        for (i = 0; i < n; i++) {
            if (list[i].owner != AMT_NETID(c) && list[i].owner != -1) {
                o = (short *)gm_FindChar(list[i].owner);
                if (o != 0) {
                    sum = (float)gm_Liking(((AMTCHARVIEW *)c)->id, ((AMTCHARVIEW *)o)->id, 1) * 0.01f + sum;
                    cnt++;
                }
            }
        }
        if (cnt != 0)
            sum = sum / (float)cnt;
        return sum;
    }
    return 0.0f;
}
/* 0x4ea690, saves the office table to the file f: the count 0x2a, then per record idx (1), owner (4), kind (1), cnt (4),
   state (1), x14 (4); 1 when all was written (the original's "amt_fio_SaveAemter")
   @deep B3: 31 code + 3 fill hunks, 47/272 bytes differ, our size = the original's 0x110 (142 code hunks at session start). The original 0x4ea690..0x4ea792 was read (disasm) and the statement sequence now matches: count 0x2a, the f == 0 guard, the count write with the 'amt_fio_SaveAemter(): count is %i' sprintf, and the 6 field writes of all 42 records in the original order (idx 1, owner 4, kind 1, cnt 4, state 1, x14 4) with return 0 on the first failed write and return 1 after the walk. Meaning/shape fixes this session: the index loop (i / i > 41) became the pointer walk ('for (p = amt_Table; (int)p < (int)&amt_Table[42]; p++)'; the original's signed jl against &amt_Table[42], esi walking 0x18 at a time), and the early-return shape ('if (vfs_fwrite(&count, 4, 1, f) == 0) return 0;' with the sprintf and the loop at the function's top level (the original's inline pop/xor eax/ret epilogues: its own copy for f == 0 at 0x4ea6ab, the count write and the six loop returns sharing 0x4ea6cc), which took 64 -> 31 hunks and 183 -> 47 bytes. Left: allocation only - the pointer base the loop register holds: the original keeps esi = &record->owner (idx at [esi-4], x14 at [esi+0x10], the loop test compares esi with 0x925514 = &amt_Table[42].owner), ours keeps esi = &record->kind (idx at [esi-8], the test lea edx,[esi-8] then cmp the bound). Same source try: index walk, do-while, for, and the p = amt_Table / p++ spellings give the same base. Q1-Q7 pass. */

int amt_SaveAemter(void *f)
{
    AMTREC *p;
    int count;
    char buf[256];

    count = 0x2a;
    if (f == 0)
        return 0;
    if (vfs_fwrite(&count, 4, 1, f) == 0)
        return 0;
    sprintf(buf, "amt_fio_SaveAemter(): count is %i", count);
    for (p = amt_Table; (int)p < (int)&amt_Table[42]; p++) {
        if (vfs_fwrite(&p->idx, 1, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->owner, 4, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->kind, 1, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->cnt, 4, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->state, 1, 1, f) == 0)
            return 0;
        if (vfs_fwrite(&p->x14, 4, 1, f) == 0)
            return 0;
    }
    return 1;
}
/* 0x4ea7a0, reads the office table from the file f (amt_InitTable first); the saves up to the version 0x10050 hold 0x25
   records, later ones 0x2a ("amt_fio_LoadAemter(): count read is %i, expected %i" when the count does not fit); 1 when
   all was read */

int amt_LoadAemter(void *f, int version)
{
    unsigned int expect, i, count;
    char buf[256];

    expect = (((version < 0x10051) - 1) & 5) + 0x25;
    amt_InitTable();
    if (f == 0)
        return 0;
    if (ls_Read(&count, 4, 1, f) == 0)
        return 0;
    if (count != expect) {
        sprintf(buf, "amt_fio_LoadAemter(): count read is %i, expected %i", count, 0x2a);
        return 0;
    }
    for (i = 0; i < expect; i++) {
        if (ls_Read(&amt_Table[i].idx, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].owner, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].kind, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].cnt, 4, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].state, 1, 1, f) == 0)
            return 0;
        if (ls_Read(&amt_Table[i].x14, 4, 1, f) == 0)
            return 0;
    }
    return 1;
}
/* 0x4ea8e0, tells the character c (+0xd > 1, +4 the id) the offices it could apply for (0x4e7150, 6 of them): the text 0x13ac and the names 0x253 + kind (0x5ec channel) */

void amt_TellOpenOffices(char *c)
{
    AMTCAND cand[6];
    char buf[2048];
    int n, i;

    if ((unsigned char)c[0xd] >= 2 && (n = amt_GetOfficeCandidates(c, 6, cand)) != 0) {
        memset(buf, 0, sizeof(buf));
        txt_Format(buf, 0x13ac);
        for (i = 0; i < n; i++)
            txt_Format(buf + strlen(buf), "%s$A", cand[i].kind + 0x253);
        he_ShowNews(*(int *)(c + 4), -1, buf, 1, 0x5ec, 0);
    }
}
/* 0x4ea990, the yearly pay of all the owned offices: the salary of each office's kind (AMTKIND.salary, times 32) times
   the pay rate, summed over the 42 records. The rate is the factor amt_Pay[buf[6]] of the decision record 0xf that
   gv_GetTableRecord fills (9 ints) */

int amt_GetOwnedOfficePay(void)
{
    int buf[9];
    int pay, sum, i;

    gv_GetTableRecord(0xf, buf);
    pay = (int)amt_Pay[buf[6]];
    sum = 0;
    for (i = 0; i < 42; i++) {
        if (amt_Table[i].owner != -1)
            sum += (int)((double)(amt_Kinds[amt_Table[i].kind].salary << 5) * pay);
    }
    return sum;
}
/* 0x4eaa00, announces the office of the kind k as free (0x4e6680, the kind 3) */

void amt_AnnounceOfficeFree(unsigned int k)
{
    int i;

    i = 0;
    do {
        if (amt_Table[i].kind == k) {
            amt_SendOfficeMessage((char)amt_Table[i].idx, 0, 0, 3, 0xff);
            return;
        }
        i++;
    } while (i < 30);
}
