/* 0x5206b0, the fix-up after a load, the counterpart of 0x519a00 before a save: it turns the loaded ids back into pointers: the player's character (ai_PlayerChar = gm_FindChar, 0 = fail), the market building (ai_MarketBuilding = gm_BuildQuery (1, 5, 10)), the eight kind 0xb characters (ai_KindBChars), the reference fields of the ai_Chars records (gm_FindById into +0x16c and +0x170, he_FindEvent into +0x17c, +0x184 cleared), the objects of ai_Objects (an object with an owner is chained into the owner's list: the heads at +0x5d building / +0x14 object / +0x178 character, +0x3f the next link; objects of the good type 0x1d lose their +0x24 word), the event records of he_Events (the kinds 0x29 0x2d 0x2f 0x11 0x25 5 10 0x26 or flagged 8 are re-initialised through he_RunHandlers[kind] with ai_GameState = 2, the stored +0x70 saved and put back; the kinds 0xd / 0xf / 0x78 / 0x76 / 0x14 get their own fixes), the first word of the 0xa4-byte records at 0x74e1e0 (an event owner id, he_FindEvent (1, 1, id)) and the target words of the reference records (ls_RefTargets, by kind: 1 gm_FindChar, 3 gm_FindObjectInst, 2 gm_FindBuildingById, 9 cm_PartyById); it closes with gm_ResetStock. Returns 1, 0 when the player's character is gone @deep B3: code hunks 223, slots 18, regs 12, fill 6. All hunks read this session and in the 2026-10-24/25 sessions: every statement, constant, callee, field offset and size matches the original 0x5206b0..0x520a2f; the 0xffff / 0xef immediates, the kind bytes and the callback literals were scanned twice. Last-bytes leads tried: the IV based at 0x770cf0 (iv-shift, written as the original's biased pointer) read 226 asm lines, reverted; the compare written from the >= side (ls_Version >= 0x10017) 221 -> 217, kept; the loop based at the other member 0x770cec with the bound 0x7d60ec 221 -> 190 asm lines (code 333 -> 290), kept; the id test written as *(unsigned short *)id != 0xffff (the same form that materialises 0xffff in edi in 0x520b80) 190 -> 179, kept. Readability then advanced to 552 bytes / 165 asm lines: the Q3 symbols (ai_Chars, ai_KindBChars, he_Events, he_QuickList, ls_RefTargets), the Q4 typed pointer views (id, otype, owner, list, next, ev70, ev10, evb8, v24) and a/b scoped into the object loop, which removed the frame hunk (0x14 -> 0xc). Also tried and reverted: he_Events + 0x53000 as the event loop bound (WORSE, 188 asm lines) and the packed LSREF struct view of the reference records (WORSE, 186). Left for the deep toolbox: the character loop's bound computed as lea esi-4 against the original's direct cmp esi (the VC6 IV base, ours +4), the object loop's ai_Objects preheader/reload order, the a/b/c slot order ([esp+0x14]/[esp+0x18] against [esp+0x10]/[esp+0x14]) and the hoisted push ebp/edi. Q3 gap: 0x74e1e0 and 0x7585e0, the 256 records of 0xa4 bytes whose first word is an event owner id (he_FindEvent kind 1); he.c only memsets the block at init, the save writer 0x51bdc0 writes it and no unit names it, so there is no evidence for a name and the two literals stay. Q4 gap: the AIOBJ word at +0x24 (cleared for the objects of the good type 0x1d) - ai.h pads it and no unit reads it with a meaning, and an unkNN field name is itself a Q2 placeholder, so one raw access stays (a shared-header field decision). Q1/Q2/Q5/Q6/Q7 pass. */


int ls_FixupAfterLoad(void)
{
    int *p, *q;
    char *r;
    int i, saved, v, c;
    unsigned char kind, *pb;
    unsigned short *id, *otype;
    int *owner, *ev70, *ev10, *evb8, *list, *next;
    unsigned char **v24;

    ai_PlayerCharC = (char *)gm_FindChar((int)ai_PlayerCharC);
    if (ai_PlayerCharC == 0)
        return 0;
    ai_MarketBuilding = (int)gm_BuildQuery(1, 5, 10);
    if (ls_Version >= 0x10017) {
        for (q = (int *)ai_KindBChars; (int)q < (int)(ai_KindBChars + 8); q++)
            *q = (int)gm_FindChar(*q);
    }
    for (p = (int *)((char *)ai_Chars + 0x16c); (int)p < (int)(ai_Chars + 768) + 0x16c; p = (int *)((char *)p + 0x21c)) {
        id = (unsigned short *)((char *)p - 0x16c);
        if (*id != 0xffff) {
            if (p[0] == -1)
                p[0] = 0;
            else
                gm_FindById(p, 0, 0, p[0]);
            if (p[1] == -1)
                p[1] = 0;
            else
                gm_FindById(p + 1, 0, 0, p[1]);
            if (p[4] == -1)
                p[4] = 0;
            else
                p[4] = (int)he_FindEvent(1, 1, p[4]);
            p[6] = 0;
        }
    }
    for (i = 0; i < f3gm_ObjectCount; i++) {
        int a, b;

        r = (char *)ai_Objects + i * 0x43;
        otype = (unsigned short *)r;
        owner = (int *)(r + 6);
        if (ai_Goods[*otype * 0x41] == 0x1d)
            *(int *)(r + 0x24) = 0;
        if (*owner != -1 && gm_FindById(&a, &b, &c, *owner) != 0) {
            int link;

            if (a == 0) {
                if (b == 0) {
                    list = (int *)(c + 0x178);
                    link = *list;
                    if (link == 0) {
                        *list = (int)r;
                        continue;
                    }
                } else {
                    list = (int *)(b + 0x14);
                    link = *list;
                    if (link == 0) {
                        *list = (int)r;
                        continue;
                    }
                }
            } else {
                list = (int *)(a + 0x5d);
                link = *list;
                if (link == 0) {
                    *list = (int)r;
                    continue;
                }
            }
            next = (int *)(link + 0x3f);
            while (*next != 0) {
                link = *next;
                next = (int *)(link + 0x3f);
            }
            *next = (int)r;
        }
    }
    saved = d3_CurrentScene;
    d3if_SelectScene(0, 0);
    for (pb = he_Events; (int)pb < (int)he_QuickList; pb += 0x14c) {
        kind = *pb;
        if (kind != 0) {
            if ((kind == 0x29 || kind == 0x2d || kind == 0x2f || kind == 0x11 || kind == 0x25 || kind == 5
                 || kind == 10 || kind == 0x26 || (pb[0x78] & 8) != 0)
                && (v = *(ev70 = (int *)(pb + 0x70))) != 0) {
                *ev70 = 0;
                ai_GameState = 2;
                if (kind == 0x11)
                    pb[0xf0] &= 0xef;
                he_RunHandlers[kind](pb);
                ai_GameState = 0;
                *ev70 = v;
            }
            kind = *pb;
            if (kind != 0xc) {
                if (kind == 0xd) {
                    evb8 = (int *)(pb + 0xb8);
                    gm_FindById(&c, 0, 0, *evb8);
                    if (c != 0)
                        he_ShowConstruction((int)pb);
                } else if (kind == 0xf) {
                    ev10 = (int *)(pb + 0x10);
                    v = gm_FindObjectInst(*ev10);
                    if (v != 0) {
                        v24 = (unsigned char **)(v + 0x24);
                        *v24 = pb;
                    }
                } else if (kind == 0x78) {
                    memset(pb + 200, 0, 8);
                } else if (kind == 0x76) {
                    memset(pb + 0xb0, 0, 4);
                } else if (kind == 0x14) {
                    sv_CountGroupMembers((char *)pb);
                    pb[0xac] = pb[0x14] + pb[0x18];
                }
            }
        }
    }
    d3if_SelectScene(saved, 0);
    for (q = (int *)((unsigned)&he_QuickList + 0x6a0); (int)q < ((unsigned)&he_QuickList + 0xaaa0); q += 0x29) {
        if (*q != 0)
            *q = (int)he_FindEvent(1, 1, *q);
    }
    for (q = ls_RefTargets; (int)q < (int)ls_RefTargets + 0x50000; q = (int *)((char *)q + 10)) {
        if (*q != 0) {
            if (*((char *)q - 5) == 1)
                *q = (int)gm_FindChar(*q);
            if (*((char *)q - 5) == 3)
                *q = gm_FindObjectInst(*q);
            if (*((char *)q - 5) == 2)
                *q = (int)gm_FindBuildingById(*q);
            if (*((char *)q - 5) == 9)
                *q = cm_PartyById(*q);
        }
    }
    gm_ResetStock();
    return 1;
}
