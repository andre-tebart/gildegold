/* hmt.c -- historical events and persons and the council elections ("hmt_CreateBinaryFile", historical.ini /
   historical.dfn), 0x4ac140..0x4adf40; name from the "hmt_" tag. The table of 51 records at 0x5ed1e8
   (0x4ac140..0x4ac2f0: size, random picks, text ids 0x1e2b / 0x1e35 + n, prices, applying a record) has no .data;
   it is put here because its .rdata lies between unk_4a7140.c's last float and hmt's first, and the .data block of
   0x4ac440.. starts at 0x600f60 right after unk_4a7140.c's ("Blutrache" 0x600f54). Starts after the registration
   0x4abed0 that ends unk_4a7140.c; ends with "_NACHRICHTEN_NEU_01" 0x601044 (the next block is hst.c's callback
   table). Names are ours.
   Open: not included yet (dcp headers conflicts hmt HEADER): main.h; held by own declarations that differ from
   the owner's: GetModuleFileNameA.
   unit of rea 8cef7d5b (src/game/hmt.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmt.h"
#include "data_views.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#include "cm_types.h"
#include "ahm.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#include "he.h"
#include "hst.h"
#include "io_text.h"
#include "ls.h"
#include "mem.h"
#include "sv.h"
#include "tr.h"
#include "ch.h"

/* the SDK calls of hmt_CreateBinaryFile; windows.h is not included (its min/max and the other SDL macros change the
   emitted code of the unit), the two entry points are declared here as the SDK does, __stdcall */
__declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void *hModule, char *file, unsigned long size);  /* @vc6 winbase.h is not in this unit; its HMODULE is spelled void * here */
__declspec(dllimport) unsigned long __stdcall GetPrivateProfileStringA(const char *sec, const char *key,
                                                const char *def, char *buf, unsigned long size, const char *file);

/* ai.h declares cm_AddFieldValues with an int field; the caller 0x4ac2f0 passes an unsigned short (no masking) */
#define CM_AddFieldValuesShort ((void (*)(int, int, unsigned short, void *))cm_AddFieldValues)

/* 0x4ac140, the number of records */
unsigned short hmt_RecordCount(void)
{
    return 0x33;
}
/* 0x4ac150, a random record other than k */

unsigned short hmt_RandomRecord(short k)
{
    unsigned short r;

    do
        r = util_RandomU(0x33);
    while ((short)r == k);
    return r;
}
/* 0x4ac170, a random text offset 0..9 for record k, 0x1e2b if there is none @deep code hunks 39: the original loads e->textSet (+0x8) into ecx without using it and tail-calls util_Random(10) with 0x1e2b live in esi; our same-meaning source emits neither; the dead load and the live range are source-shape work (match-deep) */

unsigned short hmt_RandomTextOffset(unsigned short k)
{
    unsigned short ret;
    G11REC *e;

    ret = 0x1e2b;
    e = hmt_GetRecord(k);
    if (e != 0)
        return util_RandomU(10);
    return ret;
}
/* 0x4ac1a0, the text id of line n (at most 9) of record k: 0x1e2b + n, or 0x1e35 + n if textSet is set @deep code hunks 53: the original keeps n only in its argument slot and reloads it after the call (m in esi), ours holds n in edi across the call (extra push/pop); statements and result are equal, register allocation differs (match-deep) */

unsigned short hmt_RecordTextId(unsigned short k, unsigned short n)
{
    unsigned short m;
    G11REC *e;

    m = n;
    if (m >= 9)
        m = 9;
    e = hmt_GetRecord(k);
    if (e != 0) {
        if (e->textSet == 0)
            return m + 0x1e2b;
        return m + 0x1e35;
    }
    return n;
}
/* 0x4ac1e0, record k, 0 if out of range */

G11REC *hmt_GetRecord(unsigned short k)
{
    if (k >= 0x33)
        return 0;
    return &hmt_Records[k];
}
/* 0x4ac200, a price for character p: 500 * its worth / the richest character's worth, clamped to 1600..16000;
   0 if p's goods value does not exceed it */

int hmt_GetPrice(AICHAR *p)
{
    short id;
    int total;
    int goods;
    int v;

    gm_FindRichest(&id, &total);
    goods = gm_GetGoodsValue(p);
    v = (int)(min(16000.0f, max(1600.0f, (float)gm_GetWorth(p->id) * 500.0f / total)));
    if (goods > v)
        return v;
    return 0;
}
/* 0x4ac2f0, applies record k to character p (message to the player d): kind 0 (ids 0..4) raises skill id by
   8..15, at most to 252; kind 1 sends a message from every other character whose class (0x504ad0) is id */

void hmt_ApplyRecord(AICHAR *p, int d, unsigned short k)
{
    G11REC *e;
    int i;
    unsigned char v;

    cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, d);
    cm_SendSetField(p->netId, 0x1c8, 4, 0x20000, 0);
    e = hmt_GetRecord(k);
    if (e != 0) {
        if (e->kind == 0 && e->id <= 4) {
            v = util_RandomU(8) + 8;
            v = (unsigned char)min((float)(char)v, 252.0f - ((unsigned char *)p)[e->id + 0x80]);
            cm_BeginFieldBlock((AIPER *)p, p->netId);
            CM_AddFieldValuesShort(1, 1, e->id + 0x80, &v);
            cm_SendFieldDeltas();
        } else if (e->kind == 1) {
            for (i = 0; i < 768; i++) {
                if (ai_CharsT[i].netId != p->netId && gm_TitleGroup(ai_CharsT[i].jobId) == e->id)
                    cm_SendRelations(p->netId, ai_CharsT[i].netId, util_RandomU(4) + 3, 0);
            }
        }
    }
}
/* 0x4ac440, "hmt_CreateBinaryFile": reads the persons (p_%i) and the events (k_%i) of historical.ini next to the
   exe and writes them to historical.dfn (0x4ac8a0); 0 or -1
   @deep code hunks 227 (131 ours / 135 original instructions, calls 11 = 11): every hunk is the same statements in another source shape - the two atoi checks compile as test eax,eax / jg with the out-of-line return -1 (or eax,-1) blocks of the original against our cmp eax,0 / jle with one shared return, the record loops walk the 0x2c-byte records with a byte offset in the original, and 'historical.ini' is an inlined strcat whose word/byte loads take other registers; no missing statement, constant or call left to fix */

int hmt_CreateBinaryFile(void)
{
    HMTFILE h;
    char sec[32];
    char buf[256];
    char path[260];
    int i, n;

    h.np = 0;
    h.nk = 0;
    h.p = 0;
    h.k = 0;
    GetModuleFileNameA(main_Instance, path, 0x104);
    strrchr(path, '\\')[1] = 0;
    strcat(path, "historical.ini");
    GetPrivateProfileStringA("main", "kcnt", ch_EmptyName, buf, 0x100, path);
    h.nk = atoi(buf);
    if (h.nk <= 0)
        return -1;
    GetPrivateProfileStringA("main", "pcnt", ch_EmptyName, buf, 0x100, path);
    h.np = atoi(buf);
    if (h.np <= 0)
        return -1;
    h.k = mem_Alloc(h.nk * sizeof(HMTREC), "hmt_CreateBinaryFile");
    if (h.k != 0) {
        h.p = mem_Alloc(h.np * sizeof(HMTREC), "hmt_CreateBinaryFile");
        if (h.p != 0) {
            n = 0;
            memset(h.k, 0, h.nk * sizeof(HMTREC));
            memset(h.p, 0, h.np * sizeof(HMTREC));
            for (i = 0; i < h.nk; i++) {
                h.k[i].idx = n++;
                sprintf(sec, "k_%i", i);
                GetPrivateProfileStringA(sec, "begin", "-1", buf, 0x100, path);
                h.k[i].begin = atoi(buf);
                GetPrivateProfileStringA(sec, "vname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.k[i].vname, buf, 0xf);
                GetPrivateProfileStringA(sec, "nname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.k[i].nname, buf, 0xf);
                GetPrivateProfileStringA(sec, "end", "-1", buf, 0x100, path);
                h.k[i].end = atoi(buf);
            }
            for (i = 0; i < h.np; i++) {
                h.p[i].idx = n++;
                sprintf(sec, "p_%i", i);
                GetPrivateProfileStringA(sec, "begin", "-1", buf, 0x100, path);
                h.p[i].begin = atoi(buf);
                GetPrivateProfileStringA(sec, "vname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.p[i].vname, buf, 0xf);
                GetPrivateProfileStringA(sec, "nname", ch_EmptyName, buf, 0x100, path);
                strncpy(h.p[i].nname, buf, 0xf);
                GetPrivateProfileStringA(sec, "end", "-1", buf, 0x100, path);
                h.p[i].end = atoi(buf);
            }
            if (hmt_WriteBinaryFile(&h) == 0) {
                mem_Free(h.k);
                h.k = 0;
                mem_Free(h.p);
                return 0;
            }
        }
        if (h.k != 0) {
            mem_Free(h.k);
            h.k = 0;
        }
    }
    if (h.p != 0)
        mem_Free(h.p);
    return -1;
}

/* vfs_fwrite counts as unsigned here (size_t, fwrite): `> 0` gives the jbe of the original */
#define HMT_FW(b, s, n, f) ((unsigned int)vfs_fwrite(b, s, n, f) > 0)

/* 0x4ac8a0, writes h to /gamedata/ai/historical.dfn (magic "SaFh", the counts, the records); 0 or -1 */
int hmt_WriteBinaryFile(HMTFILE *h)
{
    int magic;
    VFSFILE *f;
    int i;

    magic = 0x68466153;
    f = vfs_Tfopen("/gamedata/ai/historical.dfn", "wb", (int)vfs_Root);
    if (f != 0) {
        if (HMT_FW(&magic, 4, 1, f) && HMT_FW(&h->np, 4, 1, f) && HMT_FW(&h->nk, 4, 1, f)) {
            for (i = 0; i < h->np; i++) {
                if (!HMT_FW(&h->p[i].idx, 4, 1, f) || !HMT_FW(&h->p[i].begin, 4, 1, f)
                    || !HMT_FW(&h->p[i].end, 4, 1, f) || !HMT_FW(h->p[i].vname, 0x10, 1, f)
                    || !HMT_FW(h->p[i].nname, 0x10, 1, f))
                    goto fail;
            }
            for (i = 0; i < h->nk; i++) {
                if (!HMT_FW(&h->k[i].idx, 4, 1, f) || !HMT_FW(&h->k[i].begin, 4, 1, f)
                    || !HMT_FW(&h->k[i].end, 4, 1, f) || !HMT_FW(h->k[i].vname, 0x10, 1, f)
                    || !HMT_FW(h->k[i].nname, 0x10, 1, f))
                    goto fail;
            }
            vfs_fclose(f);
            return 0;
        }
fail:
        vfs_fclose(f);
    }
    return -1;
}
/* 0x4aca90, loads historical.dfn into hmt_File; the current person / event become the first ones @deep code hunks 12: the original keeps hmt_File.p / .k in ecx / edx after the call, ours in eax / edx (same statements and order); register allocation (match-deep; ret local and !x tried before) */

void hmt_Init(void)
{
    hmt_CurrentPerson = -1;
    hmt_CurrentEvent = -1;
    hmt_Person = -1;
    hmt_Event = -1;
    hmt_PendingElection = -1;
    if (hmt_ReadBinaryFile(&hmt_File) == 0) {
        hmt_Person = hmt_File.p->idx;
        hmt_Event = hmt_File.k->idx;
    }
}

#define HMT_FR(b, s, n, f) ((unsigned int)vfs_fread(b, s, n, f) > 0)

/* 0x4acae0, reads /gamedata/ai/historical.dfn into h (frees what h held); 0 or -1 (h emptied) */
int hmt_ReadBinaryFile(HMTFILE *h)
{
    int magic;
    VFSFILE *f;
    int i;

    magic = 0x68466153;
    f = vfs_Tfopen("/gamedata/ai/historical.dfn", "rb", (int)vfs_Root);
    i = 0;
    if (f != 0) {
        if (HMT_FR(&magic, 4, 1, f) && magic == 0x68466153 && HMT_FR(&h->np, 4, 1, f)
            && HMT_FR(&h->nk, 4, 1, f) && h->np > 0 && h->nk > 0) {
            if (h->k != 0) {
                mem_Free(h->k);
                h->k = 0;
            }
            if (h->p != 0) {
                mem_Free(h->p);
                h->p = 0;
            }
            h->k = mem_Alloc(h->nk * sizeof(HMTREC), "hmt_CreateBinaryFile");
            if (h->k != 0) {
                h->p = mem_Alloc(h->np * sizeof(HMTREC), "hmt_CreateBinaryFile");
                if (h->p != 0) {
                    memset(h->k, 0, h->nk * sizeof(HMTREC));
                    memset(h->p, 0, h->np * sizeof(HMTREC));
                    for (i = 0; i < h->np; i++) {
                        if (!HMT_FR(&h->p[i].idx, 4, 1, f) || !HMT_FR(&h->p[i].begin, 4, 1, f)
                            || !HMT_FR(&h->p[i].end, 4, 1, f) || !HMT_FR(h->p[i].vname, 0x10, 1, f)
                            || !HMT_FR(h->p[i].nname, 0x10, 1, f))
                            goto fail;
                    }
                    for (i = 0; i < h->nk; i++) {
                        if (!HMT_FR(&h->k[i].idx, 4, 1, f) || !HMT_FR(&h->k[i].begin, 4, 1, f)
                            || !HMT_FR(&h->k[i].end, 4, 1, f) || !HMT_FR(h->k[i].vname, 0x10, 1, f)
                            || !HMT_FR(h->k[i].nname, 0x10, 1, f))
                            goto fail;
                    }
                    vfs_fclose(f);
                    return 0;
                }
            }
        }
fail:
        vfs_fclose(f);
    }
    if (h->k != 0) {
        mem_Free(h->k);
        h->k = 0;
    }
    if (h->p != 0) {
        mem_Free(h->p);
        h->p = 0;
    }
    h->np = 0;
    h->nk = 0;
    return -1;
}
/* 0x4acdc0, back to the first person / event */

void hmt_Reset(void)
{
    hmt_Person = hmt_File.p->idx;
    hmt_Event = hmt_File.k->idx;
}
/* 0x4acde0, frees hmt_File */

int hmt_Exit(void)
{
    if (hmt_File.k != 0) {
        mem_Free(hmt_File.k);
        hmt_File.k = 0;
    }
    if (hmt_File.p != 0) {
        mem_Free(hmt_File.p);
        hmt_File.p = 0;
    }
    hmt_File.np = 0;
    hmt_File.nk = 0;
    hmt_CurrentPerson = -1;
    hmt_CurrentEvent = -1;
    hmt_Person = -1;
    hmt_Event = -1;
    hmt_PendingElection = -1;
    return 0;
}
/* 0x4ace40, once a year is reached: the end of the current event (object 0x26) and the current person (object
   0x27) are announced to the human players (kind 6 / 7) */

void hmt_Update(void)
{
    int year;
    HMTOBJ ev, ps;
    AICHAR *ce, *cp;
    HMTREC *r;
    int kind;
    char text[256];
    int i;

    year = ai_TimeT.year + 0x578;
    if (amt_GetFirstOffice(0x26, &ev) && amt_GetFirstOffice(0x27, &ps)) {
        ce = gm_FindChar(ev.id);
        cp = gm_FindChar(ps.id);
        if (ev.id != -1 && hmt_CurrentEvent != -1) {
            r = hmt_Find(hmt_CurrentEvent, &kind);
            if (r != 0 && r->end <= year) {
                amt_SendOfficeMessage(ev.obj, 0, 0, 1, 0xff);
                cm_SendKillCharacter(ev.id, 1);
                if (ce != 0) {
                    txt_Format(text, 0x42b2, ce->id);
                    for (i = 0; i < 768; i++) {
                        if (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7)
                            he_ShowNews(ai_CharsT[i].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_00");
                    }
                }
            }
        }
        if (ps.id != -1 && hmt_CurrentPerson != -1) {
            r = hmt_Find(hmt_CurrentPerson, &kind);
            if (r != 0 && r->end <= year) {
                amt_SendOfficeMessage(ps.obj, 0, 0, 1, 0xff);
                cm_SendKillCharacter(ps.id, 1);
                if (cp != 0) {
                    txt_Format(text, 0x42b4, cp->id);
                    for (i = 0; i < 768; i++) {
                        if (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7)
                            he_ShowNews(ai_CharsT[i].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_02");
                    }
                }
            }
        }
    }
}
/* 0x4ad010, the record with running number idx; *kind = 0x27 for a person, 0x26 for an event */

HMTREC *hmt_Find(int idx, int *kind)
{
    int i;

    for (i = 0; i < hmt_File.np; i++) {
        if (hmt_File.p[i].idx == idx) {
            if (kind != 0)
                *kind = 0x27;
            return &hmt_File.p[i];
        }
    }
    for (i = 0; i < hmt_File.nk; i++) {
        if (hmt_File.k[i].idx == idx) {
            if (kind != 0)
                *kind = 0x26;
            return &hmt_File.k[i];
        }
    }
    return 0;
}
/* 0x4ad0a0, when the current event / person (hmt_Event / hmt_Person) has begun and object 0x26 / 0x27 is free,
   it is created (0x4c4650 with 0x15 / 0x16, a delay of 25..32) */

void hmt_Begin(void)
{
    int year;
    HMTOBJ ev, ps;
    HMTREC *r;
    int kind;

    year = ai_TimeT.year + 0x578;
    if (amt_GetFirstOffice(0x26, &ev) && amt_GetFirstOffice(0x27, &ps)) {
        if (ev.id == -1 && hmt_Event != -1) {
            r = hmt_Find(hmt_Event, &kind);
            if (r != 0 && r->begin <= year)
                cm_SendCreateCharacter(0x15, hmt_Event, -1, util_RandomU(8) + 0x19, -1, 0, 0, 0);
        }
        if (ps.id == -1 && hmt_Person != -1) {
            r = hmt_Find(hmt_Person, &kind);
            if (r != 0 && r->begin <= year)
                cm_SendCreateCharacter(0x16, hmt_Person, -1, util_RandomU(8) + 0x19, -1, 0, 0, 0);
        }
    }
}
/* 0x4ad190, the object type of election hmt_PendingElection (0..2), 0 otherwise */

int hmt_GetElectionObjType(void)
{
    if ((unsigned int)hmt_PendingElection >= 3)
        return 0;
    return hmt_ElectionObjTypes[hmt_PendingElection];
}
/* 0x4ad1b0, sets the election to decide next (0..2, -1 for none; message type 20, 0x4ad190 reads it) */

void hmt_SetPendingElection(int v)
{
    hmt_PendingElection = v;
}

/* bytes in AICHAR's pad009 (a view in ai.h changed ai_Unk483a60's code) */
#define HMT_B(c, o) (((unsigned char *)(c))[o])

/* 0x4ad1c0, the yearly election (years 1..3 of 4, ai_Time.year % 4): the four best candidates (score 0x4ad3e0)
   among the characters of the right group (hmt_ElectionGroupBits) without the excluded one (hmt_ElectionExcludedBits) get the
   office hmt_ElectionOfficeBits; human players are told
   @deep B: 175 code hunks read against the original: the frame is 8 bytes smaller here (sub esp,0x138 vs 0x140), the candidate scan keeps the character in edi where the original holds it in esi and the best[] base in ebp where the original holds it in edi, the score compare is our and eax,0x4100/jne against the original's test ah,0x41/jnp (the same fcom outcome), the insertion copy runs 4-esi against 3-ecx and copies [eax-8]->[eax] against [eax-4]->[eax+4], and cm_SendSetField's 4 is a literal here but the register ebp in the original (same value, the n clamp): every hunk is the same operation spelled differently, no missing statement/constant/callee found. Q2 also needs the 0x5176d0/0x53dda0 renames (blocked like hmt_Update) and amt_GetSuperiorOfficeHolder */
void hmt_Election(void)
{
    HMTCAND best[4];
    char text[260];
    unsigned int mask, excl;
    int type, office;
    int m, n, i, j, k;
    void *obj;
    AICHAR *c;
    float s;

    if (ai_TimeT.year >= 4) {
        m = ai_TimeT.year % 4;
        if (m != 0) {
            mask = hmt_ElectionGroupBits[m - 1];
            type = hmt_ElectionObjTypes[m - 1];
            excl = hmt_ElectionExcludedBits[m - 1];
            office = hmt_ElectionOfficeBits[m - 1];
            hmt_SendElectionMessage(m - 1);
            obj = amt_GetSuperiorOfficeHolder(type);
            if (obj != 0) {
                n = 0;
                memset(best, 0, sizeof(best));
                for (i = 0; i < 768; i++) {
                    c = &ai_CharsT[i];
                    if ((excl & c->flags1cc) == 0 && (mask & c->flags1cc) != 0 && HMT_B(c, 0x0d) >= 7
                        && (c->kind == 6 || c->kind == 7 || c->kind == 5 || c->kind == 4 || c->kind == 3
                            || c->kind == 2)) {
                        s = hmt_Score(c, obj, type, 0);
                        for (j = 4; j != 0; j--) {
                            if (s <= best[j - 1].score)
                                break;
                        }
                        if (j < 4) {
                            for (k = 3; k > j; k--) {
                                best[k].c = best[k - 1].c;
                                best[k].score = best[k - 1].score;
                            }
                            best[j].score = s;
                            n++;
                            best[j].c = c;
                        }
                    }
                }
                if (n >= 4)
                    n = 4;
                for (i = 0; i < n; i++) {
                    c = best[i].c;
                    if (c == 0)
                        return;
                    cm_SendSetField(c->netId, 0x1cc, 4, office, 0);
                    if (c->kind == 6 || c->kind == 7) {
                        txt_Format(text, 0x42b1, (HMT_B(c, 0x09) ? 0x28 : 0) + 0x253 + type);
                        he_ShowNews(c->netId, -1, text, 0, 0x5fd, "_NACHRICHTEN_NEU_14");
                    }
                }
            }
        }
    }
}
/* 0x4ad3b0, the election step message: sends sub type 0x14 with v (the election index, -1 for none) */

void hmt_SendElectionMessage(int v)
{
    int msg[31];

    msg[0] = v;
    cm_SendExSysMessage(0x14, msg);
}
/* 0x4ad3e0, the election score of candidate c for the office of obj: relation to obj's holder (0x50c550, * 0.01,
   twice if `twice`), (age byte + 1) * 0.1 * 2, and the two titles (0x166 / 0x167) weighted against title 0x1b
   @deep code 107 + slots 16 (71 ours / 74 original instructions, calls 4 = 4): the original's code is the same expression - rel = 0x50c550(c->id, obj's word, 1) * 0.01 doubled when twice, age = (byte +0xd + 1) * 0.1 doubled, w1/w2 = 0x4e7c40(title)/0x4e7c40(0x1b), t = 0x4ea5c0(c, 0x4e6f10(title)), return t1*w1 + t2*w2 + age + rel - with the four float locals in other stack slots and the x87 stack in another order (fild/fld/faddp positions differ); no constant, callee or statement differs */

float hmt_Score(AICHAR *c, void *obj, int type, int twice)
{
    float rel, age, t1, t2, w1, w2;

    rel = gm_Liking(c->id, *(unsigned short *)obj, 1) * 0.01f;
    if (twice)
        rel = rel + rel;
    age = (HMT_B(c, 0x0d) + 1) * 0.1f;
    w1 = (float)amt_GetKindLevel(c->officeKind) / (float)amt_GetKindLevel(0x1b);
    w2 = (float)amt_GetKindLevel(c->prevOffice) / (float)amt_GetKindLevel(0x1b);
    t1 = 0;
    if (c->officeKind)
        t1 = amt_GetDistrictLiking(c, amt_KindColumn(c->officeKind));
    t2 = 0.0f;
    if (c->prevOffice)
        t2 = amt_GetDistrictLiking(c, amt_KindColumn(c->prevOffice));
    return t1 * w1 + t2 * w2 + (age + age) + rel;
}
/* 0x4ad510, in election years 1..3 the holder of the office object (hmt_ElectionObjTypes) is cleared @deep code hunk 40 at instruction 10 is register allocation only: the original loads hmt_ElectionObjTypes[m-1] into eax and pushes it, ours into edx and pushes edx; the other 10 differing instructions are the same eax<->edx<->ecx swaps (12 asmdiff lines, all register renames, no instruction added or changed) */

void hmt_ClearOffices(void)
{
    int m;
    HMTOBJ o;

    if (ai_TimeT.year >= 4) {
        m = ai_TimeT.year % 4;
        if (m != 0) {
            if (amt_GetFirstOffice(hmt_ElectionObjTypes[m - 1], &o) && gm_FindChar(o.id) != 0)
                amt_SendOfficeMessage(o.obj, 0, 0, 1, 0xff);
        }
    }
}
/* 0x4ad580, decides election hmt_PendingElection: the best scored (0x4ad3e0, relation counted twice) of up to four
   candidates gets the office object, the human players get message 0xa5; without a holder the candidates lose
   the office bit
   @deep B: 50 code hunks left after the one meaning fix (the original compares hmt_PendingElection unsigned: (unsigned int) < 3, jae; now equal there). The rest are the same operation spelled differently: the candidate limit n > 3 here against cmp edi,0x4/jge in the original, the best test fld best/fcomp cand here against fld cand/fcomp best there (with test ah,0x5/jp against and eax,0x4100/jne), and the AIMSG field stores in another order (same offsets once the pushed esp is accounted for: type base+4, unk08 base+8, time base+0x28, arg[5] base+0x6c). No missing statement, constant or callee left. Q1/Q2 also open (placeholder name, cm_SendLongMessage and the 0x5176d0/0x53dda0 rename block) */

void hmt_Unk4ad580(void)
{
    HMTCAND cand[4];
    HMTOBJ o;
    AIMSG msg;
    float best;
    int type;
    unsigned int bit;
    int n, i;
    void *obj;
    AICHAR *win;

    if ((unsigned int)hmt_PendingElection < 3) {
        type = hmt_ElectionObjTypes[hmt_PendingElection];
        bit = hmt_ElectionOfficeBits[hmt_PendingElection];
        if (amt_GetFirstOffice(type, &o)) {
            memset(cand, 0, sizeof(cand));
            n = 0;
            for (i = 0; i < 768; i++) {
                if (n > 3)
                    break;
                if (ai_CharsT[i].flags1cc & bit) {
                    cand[n].c = &ai_CharsT[i];
                    n++;
                }
            }
            obj = amt_GetSuperiorOfficeHolder(type);
            i = 0;
            if (obj == 0) {
                for (i = 0; i < n; i++) {
                    if (cand[i].c == 0)
                        break;
                    cm_SendSetField(cand[i].c->netId, 0x1cc, 4, 0, bit);
                }
            } else {
                win = 0;
                best = -1.0f;
                for (i = 0; i < n; i++)
                    cand[i].score = hmt_Score(cand[i].c, obj, type, 1);
                for (i = 0; i < n; i++) {
                    if (best < cand[i].score) {
                        best = cand[i].score;
                        win = cand[i].c;
                    }
                }
                if (win != 0) {
                    amt_SendOfficeMessage(o.obj, (int)win, 0, 1, 0xff);
                    if (win->kind == 6 || win->kind == 7 || win->kind == 5) {
                        msg.to = win->netId;
                        msg.time = ai_TimeT;
                        msg.type = 0xa5;
                        msg.fromId = -1;
                        msg.flags = 1;
                        msg.arg[4] = msg.to;
                        msg.arg[5] = type;
                        cm_SendLongMessage(&msg);
                        for (i = 0; i < 768; i++) {
                            if (ai_CharsT[i].id != 0xffff && &ai_CharsT[i] != win
                                && (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7)) {
                                msg.to = ai_CharsT[i].netId;
                                cm_SendLongMessage(&msg);
                            }
                        }
                    }
                }
            }
            hmt_SendElectionMessage(-1);
        }
    }
}
/* 0x4ada80, loads the hmt state from the savegame f; on failure it is reset; 1 or 0 @deep code hunks 8: the original's success return is out of line (jne over the success block), ours returns inline (je); if / else with an empty then tried before, no change (match-deep) */

int hmt_Load(void *f, int version)
{
    if (ls_Read(&hmt_CurrentPerson, 4, 1, f) && ls_Read(&hmt_Person, 4, 1, f)
        && ls_Read(&hmt_CurrentEvent, 4, 1, f) && ls_Read(&hmt_Event, 4, 1, f)
        && ls_Read(&hmt_PendingElection, 4, 1, f)) {
    } else {
        hmt_CurrentPerson = -1;
        hmt_CurrentEvent = -1;
        hmt_Person = -1;
        hmt_Event = -1;
        hmt_PendingElection = -1;
        return 0;
    }
    return 1;
}
/* 0x4adb20, saves the hmt state to the savegame f */

int hmt_Save(VFSFILE *f)
{
    if (!HMT_FW(&hmt_CurrentPerson, 4, 1, f))
        return 0;
    if (!HMT_FW(&hmt_Person, 4, 1, f))
        return 0;
    if (!HMT_FW(&hmt_CurrentEvent, 4, 1, f))
        return 0;
    if (!HMT_FW(&hmt_Event, 4, 1, f))
        return 0;
    return HMT_FW(&hmt_PendingElection, 4, 1, f);
}
/* 0x4ad7a0, at the start of a game: counts the members of the three councils (each other character counts 0.33,
   a new one 0.67 more) and creates new characters (0x4c4650 type 3) until each has two, with age 7, title
   hmt_Titles[i], money (1 + rand) * 0.5 * 8000000 and the council bits
   @deep B: 16 code hunks read: all are the same operation spelled differently, no meaning error. The council loop (for i = 2; i >= 0) exits on lea eax,[edi-2]/test/jge here against lea eax,[edi-1]/test/jne in the original (edi is i+2: the original tests i != -1), with sub ecx,4 and dec edi swapped; the wait loop while (!cm_GetSlotState(h)) sv_PumpNet() is rotated (ours tests after the call at 4ad989, the original jumps back to a top test at 4ad977); the money expression multiplies fmul 0x5eb7ac before fimul hmt_Unk5ee114 in the original, after it here; two xor edi,edi / xor ebx,ebx stores swapped. Q1 name and Q2 (ai_Unk5... callees, HMT_ macros) still open */

void hmt_Unk4ad7a0(void)
{
    int age;
    int title;
    float cnt[3];
    int last[3];
    int titles[3];
    int ids[15];
    int i, j, k, n, id, h, money;
    unsigned int bits;
    AICHAR *p;

    ids[0] = 1;
    ids[1] = 2;
    ids[2] = 3;
    ids[3] = 4;
    ids[4] = 5;
    ids[5] = 6;
    ids[6] = 7;
    ids[7] = 8;
    ids[8] = 9;
    ids[9] = 10;
    ids[10] = 11;
    ids[11] = 12;
    ids[12] = 13;
    ids[13] = 14;
    ids[14] = 15;
    cnt[0] = 0;
    cnt[1] = 0;
    cnt[2] = 0;
    last[0] = -1;
    last[1] = -1;
    last[2] = -1;
    titles[0] = 15;
    titles[1] = 0x15;
    titles[2] = 0x1b;
    for (j = 0; j < 768; j++) {
        p = &ai_CharsT[j];
        if (HMT_B(p, 0x0d) >= 7
            && (p->kind == 6 || p->kind == 7 || p->kind == 5 || p->kind == 4 || p->kind == 3 || p->kind == 2)) {
            for (i = 2; i >= 0; i--) {
                if ((p->flags1cc & hmt_ElectionExcludedBits[i]) == 0 && (hmt_ElectionGroupBits[i] & p->flags1cc) != 0) {
                    if (p->netId != last[(i + 1) % 3] && p->netId != last[(i + 2) % 3]) {
                        last[i] = p->netId;
                        cnt[i] += 0.67f;
                    }
                    cnt[i] += 0.33f;
                }
            }
        }
    }
    gm_Shuffle(15, ids);
    k = 0;
    for (i = 0; i < 3; i++) {
        for (n = (int)cnt[i]; n < 2; n++) {
            id = gm_TitleNumber(ids[k++], util_RandomU(3) + 4);
            h = ((int (*)(int, int, int, int, int, int, int, int))cm_SendCreateCharacter)(3, -1, -1, util_RandomU(8) + 0x19, -1, id, 0, gm_Unk503f30(id));
            if (h != -1) {
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
            }
            if (cm_GetSlotState(h) != 1)
                return;
            age = 7;
            money = (int)((util_RandomF() + 1.0f) * 0.5f * hmt_Unk5ee114);
            title = titles[i];
            p = cm_GetSlotValue(h);
            cm_SendMoveGoods(p->netId, -1, ai_OwnPlayer, money);
            cm_BeginFieldBlock((AIPER *)p, p->netId);
            cm_AddFieldDelta(1, 1, 0xd, &age);
            cm_AddFieldDelta(1, 1, 0x167, &title);
            cm_SendFieldDeltas();
            bits = hmt_ElectionGroupBits[i];
            if (i == 2)
                bits |= hmt_ElectionGroupBits[util_RandomU(i)];
            cm_SendSetField(p->netId, 0x1cc, 4, bits, 0);
        }
    }
}

#define HMT_I(c, o) (*(int *)((char *)(c) + (o)))
#define HMT_F(c, o) (*(float *)((char *)(c) + (o)))
#define HMT_S(c, o) (*(short *)((char *)(c) + (o)))
/* the word at +0x14 of the 0x2f4-byte player records that start at 0x7e3f80 */
#define HMT_PLAYERW(i) (*(unsigned short *)((char *)&ai_Unk7e3f80 + 0x14 + (i) * 0x2f4))

/* 0x4adbb0, makes c the historical event (type 0x15, bit 0x1000) or person (bit 0x2000) hmt_Event / hmt_Person:
   new relations to everybody, default values, skills 210..251, age 9, the names of the record; tells the first
   human player and moves on to the next record
   @deep B: 17 code hunks left after the one meaning fix (the character scan's i was unsigned short; the original strength-reduces an int counter to a pointer walk with a signed cmp/jl and cmp cx,0x300/jb does not come from a 16-bit induction variable: now int i, 43 -> 17 hunks). The rest are spelling/order: the hmt_Find argument is a ?: here (mov eax,evt; cmp; je; mov eax,per) against the original's if/else jump shape (cmp; jne; mov ecx,evt; lea; push; push; jmp), lea edi,[ebp+0x5c] before the rep stos here vs after the or, xor ecx,ecx earlier; Q2 (amt_SetEventOwner/amt_SetPersonOwner/500970/505d30/5043d0, HMT_ macros, the 0x5176d0/0x53dda0 rename block) still open */
void hmt_Create(int type, int unk, unsigned short v, AICHAR *c)
{
    char text[256];
    int kind;
    HMTREC *r;
    unsigned short pl;
    int i, j, k;

    if (type == 0x15) {
        c->flags1cc |= 0x1000;
        amt_SetEventOwner(c->netId);
    } else {
        c->flags1cc |= 0x2000;
        amt_SetPersonOwner(c->netId);
    }
    HMT_S(c, 0x0a) = v;
    HMT_B(c, 0x0c) = 0;
    for (j = 0; j < 768; j++) {
        for (k = 0; k < 768; k++) {
            ai_Relations[j][k] = (char)util_RandomU(0x40) - 0x20;
            ai_RelationMemory[j][k] = 0;
        }
    }
    HMT_S(c, 0x50) = 0x7fff;
    HMT_I(c, 0x7c) = 0;
    HMT_B(c, 0x58) = 0;
    memset((char *)c + 0x5c, 0xff, 0x20);
    HMT_I(c, 0x170) = 0;
    HMT_I(c, 0x16c) = 0;
    HMT_F(c, 0x1c) = 1000.0f;
    HMT_F(c, 0x14) = 1.0f;
    HMT_F(c, 0x20) = 12.0f;
    HMT_F(c, 0x10) = 330.0f;
    HMT_F(c, 0x18) = 1000.0f;
    for (j = 0; j < 5; j++)
        HMT_B(c, 0x80 + j) = (unsigned char)(util_RandomU(0x2a) + 210.0f);
    HMT_B(c, 0x0d) = 9;
    gm_AddObjekt(c->netId, HMT_PLAYERW(ai_OwnPlayer), &ai_Unkf42400);
    pl = 0xffff;
    for (i = 0; i < 768; i++) {
        if (pl != 0xffff)
            break;
        if (ai_CharsT[i].kind == 6)
            pl = i;
    }
    r = hmt_Find(type == 0x15 ? hmt_Event : hmt_Person, &kind);
    strcpy(c->name, r->vname);
    strcpy(c->name + 0x10, r->nname);
    if (type == 0x15) {
        if (ai_TimeT.year > 0 && pl != 0xffff) {
            txt_Format(text, 0x42b3, c->id);
            he_ShowNews(ai_CharsT[pl].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_01");
        }
        HMT_I(c, 0x18c) = hmt_RandomHeadIcon();
        hmt_NextEvent();
    } else {
        if (ai_TimeT.year > 0 && pl != 0xffff) {
            txt_Format(text, 0x42b5, c->id);
            he_ShowNews(ai_CharsT[pl].netId, -1, text, 0, 0x602, "_NACHRICHTEN_NEU_03");
        }
        HMT_I(c, 0x18c) = hmt_RandomHeadIcon();
        hmt_NextPerson();
    }
    c->kind = 3;
}
/* 0x4ade60, hmt_Event becomes the previous one (hmt_CurrentEvent), the next event record the current one */

void hmt_NextEvent(void)
{
    int i;

    for (i = 0; i < hmt_File.nk; i++) {
        if (hmt_File.k[i].idx == hmt_Event) {
            hmt_CurrentEvent = hmt_Event;
            if (i < hmt_File.nk - 1)
                hmt_Event = hmt_File.k[i + 1].idx;
            else
                hmt_Event = -1;
            return;
        }
    }
}
/* 0x4adec0, the same for the persons */

void hmt_NextPerson(void)
{
    int i;

    for (i = 0; i < hmt_File.np; i++) {
        if (hmt_File.p[i].idx == hmt_Person) {
            hmt_CurrentPerson = hmt_Person;
            if (i < hmt_File.np - 1)
                hmt_Person = hmt_File.p[i + 1].idx;
            else
                hmt_Person = -1;
            return;
        }
    }
}
/* 0x4adf20, 1593 or 1594 */

int hmt_RandomHeadIcon(void)
{
    return util_RandomU(2) + 0x639;
}

int hmt_CurrentPerson = 0;  /* 0x6f0710, idx of the person in play, -1 for none */
int hmt_Person = 0;  /* 0x6f0714, idx of the person to appear next */
int hmt_CurrentEvent = 0;  /* 0x6f0718, idx of the event in play, -1 for none */
int hmt_Event = 0;  /* 0x6f071c, idx of the event to appear next */
int hmt_PendingElection = 0;  /* 0x6f0720, the election to decide (0..2), -1 for none */
/* ai_Unkf42400: view of ai_Unkf31160 (tools/data_views.py) */
const G11REC hmt_Records[51] = { { 0, 1, 0, 1, "`\036HANDWERKSKUNST" }, { 0, 2, 0, 0, "c\036NACHTUNDNEBEL" }, { 0, 3, 0, 1, "f\036KAMPF_1" }, { 0, 3, 0, 0, "i\036KAMPF_2" }, { 0, 3, 0, 0, "l\036KAMPF_3" }, { 0, 3, 0, 0, "o\036KAMPF_4" }, { 0, 4, 0, 0, "r\036RHETORIK" }, { 1, 1, 0, 0, "u\036SCHMIED_1" }, { 1, 1, 0, 0, "x\036SCHMIED_2" }, { 1, 1, 0, 0, "\355\036SCHMIED_3" }, { 1, 2, 0, 1, "{\036STEINMETZ" }, { 1, 3, 0, 1, "~\036TISCHLER" }, { 1, 4, 0, 1, "\201\036WIRT" }, { 1, 5, 0, 0, "\204\036PARFUMEUR_1" }, { 1, 5, 0, 0, "\207\036PARFUMEUR_2" }, { 1, 6, 0, 0, "\212\036KRAEUTERHAENDLER_1" }, { 1, 6, 0, 0, "\215\036KRAEUTERHAENDLER_2" }, { 1, 7, 0, 0, "\220\036PRIESTER_1" }, { 1, 7, 0, 0, "\223\036PRIESTER_2" }, { 1, 8, 0, 1, "\226\036FERNHAENDLER" }, { 1, 9, 0, 0, "\231\036BANKIER" }, { 1, 10, 0, 0, "\234\036GARDIST" }, { 1, 11, 0, 1, "\237\036DIEB_1" }, { 1, 11, 0, 0, "\242\036DIEB_2" }, { 1, 12, 0, 1, "\245\036RAEUBER_1" }, { 1, 12, 0, 1, "\250\036RAEUBER_2" }, { 1, 12, 0, 0, "\253\036RAEUBER_3" }, { 1, 12, 0, 0, "\256\036RAEUBER_4" }, { 2, 0, 0, 0, "\261\036BUERGERMEISTER" }, { 2, 0, 0, 0, "\264\036RICHTER" }, { 2, 0, 0, 0, "\267\036KAEMMERER" }, { 2, 0, 0, 0, "\272\036SCHILFLIEDER" }, { 2, 0, 0, 1, "\275\036EDELSTEINE" }, { 2, 0, 0, 1, "\300\036NOVEMBERLAND" }, { 2, 0, 0, 0, "\303\036BUERGER_FRIEDBERT" }, { 2, 0, 0, 0, "\306\036DIE_LETZTE_WACHT" }, { 2, 0, 0, 0, "\311\036DIE_GUTEN_ALTEN_ZEITEN" }, { 2, 0, 0, 0, "\314\036NEBELKRAEHE" }, { 2, 0, 0, 0, "\317\036PEST" }, { 2, 0, 0, 0, "\322\036HERBSTBESUCH" }, { 2, 0, 0, 0, "\325\036SONNENUNTERGANG" }, { 2, 0, 0, 0, "\330\036HUHNS_END" }, { 2, 0, 0, 0, "\333\036SPION" }, { 2, 0, 0, 0, "\336\036DON_IM_GRAS" }, { 2, 0, 0, 0, "\341\036UEBERFALL" }, { 2, 0, 0, 0, "\344\036GOPPOCKS_GESCHICHT" }, { 2, 0, 0, 0, "\347\036BOKTORUS" }, { 2, 0, 0, 0, "\352\036ABLASSBRIEF" }, { 2, 0, 0, 0, "\360\036DIE_LETZTE_KERZE" }, { 2, 0, 0, 0, "\363\036GRAF_ADALBERT" }, { 2, 0, 0, 0, "\366\036LEBENSLAUF" } };  /* 0x5ed1e8 */
const int hmt_ElectionObjTypes[3] = { 35, 36, 37 };  /* 0x5edb78, .rdata: object type per election */
const unsigned int hmt_ElectionGroupBits[3] = { 64, 128, 256 };  /* 0x5edb84, .rdata: candidates' group bits */
const unsigned int hmt_ElectionExcludedBits[3] = { 15872, 15872, 15872 };  /* 0x5edb90, .rdata: excluded group bits */
const int hmt_ElectionOfficeBits[3] = { 16384, 32768, 65536 };  /* 0x5edb9c, .rdata: the office */
const int hmt_Unk5ee114 = 8000000;  /* 0x5ee114, 8000000 */
HMTFILE hmt_File = { -1, -1, 0, 0 };  /* 0x600f60, {-1, -1, 0, 0} */
