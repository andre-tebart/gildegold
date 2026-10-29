/* 0x50e690, the start population of a new town: the ages t[0..29] (the six groups of 4 / 6 shuffled by
   gm_Shuffle, each plus a random 0 .. 2), the characters already there (the first 12 who ai_WorksForMaster and
   are not kind 5) into f0, the free slots filled through cm_SendCreateCharacter (kind 3, up to three of the kind
   0x10 beyond the 13th), then for each character its age (t[k], through gm_AgeCharacter), job and money (the
   office of ids[k]: amt_GetOffice -> amt_GetKind, the age / money tables t[0x1e..0x27] / t[0x28..0x31]:
   1 3 3 4 4 5 6 7 7 7 and 1000 10000 10000 20000 20000 40000 50000 60000 80000 100000, the money through
   gm_PlayerFactor), the office flags (field 0x1cc) and the first office of the kinds 0x23 .. 0x25, then the
   spouses of the unmarried (cm_SendRelations both ways, a child of kind 8 when the age gap is above 5), and
   eight extra pairs of the kind 3 characters. Sends the field blocks / characters over the network; returns 1.
   @deep B: draft, 846 code-hunk instructions (568 differing asm of 809) after the whole original 0x50e690..0x50f0a1 was read. Meaning fixed this session (all read against the disassembly): msg5 = (char)((util_Random(0x2a) & 0xffff) + (kind.level/3 + 1) * 21.0f) (was a bare random, the lost float operand of __ftol); the money = gm_PlayerFactor(w - (int)(util_RandomF() * w * -0.5f), ai_OwnPlayer) (the original pushes both args; was one arg and two RandomF calls); msg4 = (util_Random(8)+1) * fl * (msg3 * 0.01f) (a percentage of the purse, was (unsigned)fl); cm_SendRelations takes 4 args (the trailing 0 was missing); gm_TitleNumber(gm_TitleGroup(job), 1) (was gm_TitleGroup(job, 1)); the field-0x1cc block is two branches (0x80 / 0x40) not one condition + ternary; c is a char (byte compares); the scan has two breaks (n > 0x1d || n > 0xb); cm_SendCreateCharacter called through the canonical prototype (unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char), not all-int. Readability done: Q1..Q7 pass (INIT_CHAR gained female 0x09 / age 0x0a / partner 0x5c / jobId 0x164 / officeKind 0x166, INIT_OFFICE.idWord, new INIT_KIND, local prototypes, INIT_CHARACTERS_END). Last-bytes pass (spell.py): the cmp-test 50e690 entry = the age-table loop as a pointer walk (int *tp; tp < t + 30) - kept, 596 -> 568 differing; the negated while (!cm_GetSlotState(j)) spelling for the cmp-test 50edf7 / 50ef21 / 50f04c entries - no change, reverted; the dup-tail 50ec34 entry = the field-0x1cc value computed first (0x80 / 0x40 / 0) and one shared cm_SendSetField call after the branch - WORSE (568 -> 578), reverted; the dup-tail 50e8e6 / 50e945 / 50ef78 / 50f04c entries are the cm_GetSlotState wait loops already in the original's source form (the try-forms of the menu are the two spellings above). What remains is allocation / scheduling: the frame 0x2cc vs 0x2c0 (12 bytes; every local shifted), the -1 arguments kept in ebp instead of pushed as immediates, the t[] init store order, the msg0 / msg1 constants pushed early, and the loop branch layout. No Q failure left, nothing in this list is a meaning error. */


int init_CreateStartCharacters(void)
{
    int t[50];
    int ids[30];
    int out[30];
    INIT_CHAR *f0[30];
    INIT_CHAR *p, *ch, *q, *u, *v;
    INIT_OFFICE rec, ob1, ob2, ob3;
    INIT_KIND kind;
    int cnt, i, n, k, j, h, w;
    unsigned msg0, msg1, msg4, msg3;
    char msg5;
    char c;
    float fl;

    t[0] = 0x10; t[1] = 0x12; t[2] = 0x14; t[3] = 0x18;
    t[4] = 0x10; t[5] = 0x12; t[6] = 0x14; t[7] = 0x18;
    t[8] = 0x10; t[9] = 0x12; t[10] = 0x14; t[11] = 0x18;
    t[12] = 0x16; t[13] = 0x18; t[14] = 0x19; t[15] = 0x1b; t[16] = 0x1d; t[17] = 0x1d;
    t[18] = 0x16; t[19] = 0x18; t[20] = 0x19; t[21] = 0x1b; t[22] = 0x1d; t[23] = 0x1d;
    t[24] = 0x19; t[25] = 0x1b; t[26] = 0x1d; t[27] = 0x20; t[28] = 0x22; t[29] = 0x25;
    cnt = 0;
    gm_Shuffle(4, t);
    gm_Shuffle(4, t + 4);
    gm_Shuffle(4, t + 8);
    gm_Shuffle(6, t + 12);
    gm_Shuffle(6, t + 18);
    gm_Shuffle(6, t + 24);
    memset(f0, 0, sizeof(f0));
    {
        int *tp = t;

        do {
            *tp += util_Random(3) & 0xffff;
            tp++;
        } while (tp < t + 30);
    }
    n = 0;
    p = (INIT_CHAR *)ai_Chars[0];
    do {
        if (n > 0x1d)
            break;
        if (n > 0xb)
            break;
        if (ai_WorksForMaster((char *)p) != 0 && p->kind != 5) {
            f0[n] = p;
            n++;
        }
        p = (INIT_CHAR *)((char *)p + 0x21c);
    } while ((int)p < INIT_CHARACTERS_END);
    for (i = 0; i < 30; i++)
        ids[i] = i;
    gm_Shuffle(4, ids);
    gm_Shuffle(4, ids + 4);
    gm_Shuffle(4, ids + 8);
    gm_Shuffle(6, ids + 12);
    gm_Shuffle(6, ids + 18);
    gm_Shuffle(6, ids + 24);
    init_PlaceOffices(30 - n, out + n, ids + n);
    for (i = n; i < 30; i++) {
        cm_BeginSendGroup();
        if (cnt < 3 && i * 4 > 0x30 && util_Random(4) == 0) {
            h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0x10, -1, -1, 0x10, -1, 0, 0, 0);
            cnt++;
        } else {
            h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(3, -1, -1, 0x10, -1, (unsigned char)out[i], 0, 2);
        }
        cm_EndSendGroup();
        while (cm_SendGroupState() == 0) {
            cm_FlushSendQueue();
            cm_ReceiveMessages();
            cm_ExecCommands();
        }
        f0[i] = (INIT_CHAR *)cm_GetSlotValue(h);
    }
    t[0x1e] = 1; t[0x1f] = 3; t[0x20] = 3; t[0x21] = 4; t[0x22] = 4; t[0x23] = 5;
    t[0x24] = 6; t[0x25] = 7; t[0x26] = 7; t[0x27] = 7;
    t[0x28] = 1000; t[0x29] = 10000; t[0x2a] = 10000; t[0x2b] = 20000; t[0x2c] = 20000;
    t[0x2d] = 40000; t[0x2e] = 50000; t[0x2f] = 60000; t[0x30] = 80000; t[0x31] = 100000;
    for (k = 0; k < 30; k++) {
        unsigned short age;

        msg0 = 2;
        amt_GetOffice((unsigned char)ids[k], &rec);
        amt_GetKind(rec.kindWord & 0xff, &kind);
        age = (unsigned short)t[k];
        msg5 = (char)((util_Random(0x2a) & 0xffff) + ((kind.levelWord & 0xff) / 3 + 1) * 21.0f);
        msg0 = (util_Random(2) & 0xffff) + t[(kind.levelWord & 0xff) + 0x1e];
        w = t[(kind.levelWord & 0xff) + 0x28];
        j = (int)(util_RandomF() * w * -0.5f);
        msg3 = gm_PlayerFactor(w - j, ai_OwnPlayer);
        ch = f0[k];
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, msg3);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(1, 1, 0xd, &msg0);
        if (ch->kind == 3) {
            cm_AddFieldDelta(1, 1, 0x84, &msg5);
            if (util_Random(2) != 0) {
                fl = util_RandomF() + 1.0f;
                msg4 = (unsigned)(((util_Random(8) & 0xffff) + 1) * fl * (msg3 * 0.01f));
                cm_AddFieldDelta(4, 1, 0x1ac, &msg4);
            }
        }
        cm_SendFieldDeltas();
        gm_AgeCharacter((char *)ch, age);
        j = amt_SendOfficeOwner((unsigned char)ids[k], (char *)ch, 0xff);
        if (j != -1) {
            while (cm_GetSlotState(j) == 0)
                sv_PumpNet();
        }
        c = ch->officeKind;
        if (c == 0x18 || c == 0x1a || c == 0x1b) {
            j = ((int (*)(void *, int, int, int, int))cm_SendSetField)(ch->netId, 0x1cc, 4, 0x80, 0);
            if (j != -1) {
                while (cm_GetSlotState(j) == 0)
                    sv_PumpNet();
            }
        } else if (c == 0x16 || c == 0x17 || c == 0x19) {
            j = ((int (*)(void *, int, int, int, int))cm_SendSetField)(ch->netId, 0x1cc, 4, 0x40, 0);
            if (j != -1) {
                while (cm_GetSlotState(j) == 0)
                    sv_PumpNet();
            }
        }
        c = ch->officeKind;
        if (c == 0x1b) {
            amt_GetFirstOffice(0x25, &ob1);
            amt_SendOfficeOwner(ob1.idWord, (char *)ch, 0xff);
        } else if (c == 0x1a) {
            amt_GetFirstOffice(0x24, &ob2);
            amt_SendOfficeOwner(ob2.idWord, (char *)ch, 0xff);
        } else if (c == 0x19) {
            amt_GetFirstOffice(0x23, &ob3);
            amt_SendOfficeOwner(ob3.idWord, (char *)ch, 0xff);
        }
    }
    for (;;) {
        u = 0;
        v = 0;
        for (k = 0; k < 30 && !(u && v); k++) {
            q = f0[k];

            if (q->age > 0x16) {
                if (u == 0 && q->female == 0 && q->partner == -1)
                    u = q;
                else if (v == 0 && q->female == 1 && q->partner == -1)
                    v = q;
            }
        }
        if (u == 0 || v == 0)
            break;
        n = v->age - 0x12;
        cm_BeginFieldBlock((char *)u, u->netId);
        cm_AddFieldDelta(4, 1, 0x5c, &v->netId);
        cm_SendFieldDeltas();
        cm_BeginFieldBlock((char *)v, v->netId);
        cm_AddFieldDelta(4, 1, 0x5c, &u->netId);
        cm_SendFieldDeltas();
        if (n > 5) {
            w = gm_TitleNumber(gm_TitleGroup(util_Random(2) == 0 ? v->jobId : u->jobId), 1);
            msg4 = 2;
            j = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(8, u->netId, v->netId, n, -1, w, 0, 2);
            if (j != -1) {
                while (cm_GetSlotState(j) == 0)
                    sv_PumpNet();
            }
            ch = (INIT_CHAR *)cm_GetSlotValue(j);
            cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, ((util_Random(0x400) & 0xffff) + 0x200) * 0x20);
            cm_BeginFieldBlock((char *)ch, ch->netId);
            cm_AddFieldDelta(1, 1, 0xd, &msg4);
            cm_SendFieldDeltas();
        }
        ((int (*)(int, int, int, int))cm_SendRelations)(u->netId, v->netId, (util_Random(0x20) & 0xffff) + 0x60, 0);
        j = ((int (*)(int, int, int, int))cm_SendRelations)(v->netId, u->netId, (util_Random(0x20) & 0xffff) + 0x60, 0);
        if (j != -1) {
            while (cm_GetSlotState(j) == 0)
                sv_PumpNet();
        }
    }
    for (i = 8; i != 0; i--) {
        msg0 = 2;
        msg1 = (util_Random(9) & 0xffff) + 1;
        w = gm_TitleNumber((util_Random(0xc) & 0xffff) + 1, (util_Random(2) & 0xffff) + 1);
        n = util_Random(4);
        h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(3, -1, -1, n + 0xf, -1, w, 0, 0);
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
        ch = (INIT_CHAR *)cm_GetSlotValue(h);
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, ((util_Random(0x400) & 0xffff) + 0x200) * 0x20);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(1, 1, 0xd, &msg0);
        cm_AddFieldDelta(1, 1, 0x167, &msg1);
        cm_SendFieldDeltas();
        w = gm_TitleNumber((util_Random(0xc) & 0xffff) + 1, (util_Random(2) & 0xffff) + 1);
        n = util_Random(4);
        h = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(3, -1, -1, n + 0xe, -1, w, 0, 1);
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
        ch = (INIT_CHAR *)cm_GetSlotValue(h);
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, ((util_Random(0x400) & 0xffff) + 0x200) * 0x20);
        cm_BeginFieldBlock((char *)ch, ch->netId);
        cm_AddFieldDelta(1, 1, 0xd, &msg0);
        cm_AddFieldDelta(1, 1, 0x167, &msg1);
        cm_SendFieldDeltas();
    }
    hmt_Begin();
    return 1;
}
