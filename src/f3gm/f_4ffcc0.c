/* 0x4ffcc0, the next match of the object query set up by 0x4ffe90: walks the instance list from the current record (0x6f1bf4; with the scan flag through the whole table by 0x43-byte steps, else along the next pointers at +0x3f, with the child stack 0x8753e0 when descending), tests the query (prototype 0x61a67c, owner id 0x61a680, 0x50a0b0 value 0x61a684, prototype kind 0x61a686) and returns the last match (the first with flag 0x6f1bea), 0 when none. ROUGH */

unsigned short *gm_QueryNext(void)
{
    unsigned short *p, *q, *res;
    int m, sp;
    short s;

    p = ai_Unk6f1bf4;
    res = 0;
    m = 0;
    if (ai_Unk6f1bf4 == 0)
        return 0;
    if (ai_Unk6f1be0 != 0) {
        ai_Unk6f1be0 = 0;
        if (ai_Unk6f1be9 != 0 && *(int *)(ai_Unk6f1bf4 + 10) != 0) {
            ai_Unk8753e0[ai_Unk7e3f38] = *(int *)(ai_Unk6f1bf4 + 10);
            ai_Unk7e3f38++;
        }
    }
    sp = ai_Unk7e3f38;
    if (ai_Unk6f1be4 < f3gm_ObjectCount) {
        do {
            if (p == 0)
                return res;
            if (m)
                return res;
            q = ai_Unk6f1bf4;
            while (ai_Unk6f1bf4 = p, *p == 0) {
                q = (unsigned short *)((char *)ai_Unk6f1bf4 + 0x43);
                p = q;
            }
            if (ai_Unk6f1bea != 0)
                m = 1;
            if (ai_Unk61a67c == 0x7fff || *ai_Unk6f1bf4 == ai_Unk61a67c) {
                if (ai_Unk61a67c != 0x7fff)
                    m = 1;
                if (ai_Unk61a680 != -1) {
                    if (*(int *)(ai_Unk6f1bf4 + 1) != ai_Unk61a680)
                        goto no;
                    m = 1;
                }
                if (ai_Unk61a684 != -1) {
                    s = (short)gm_Unk50a0b0(ai_Unk6f1bf4);
                    sp = ai_Unk7e3f38;
                    if (s != ai_Unk61a684)
                        goto no;
                    m = 1;
                }
                if (ai_Unk61a686[0] != -1)
                    m = ai_Goods[*ai_Unk6f1bf4 * 0x41] == ai_Unk61a686[0];
            } else {
            no:
                m = 0;
            }
            if (ai_Unk6f1be8 == 0) {
                if (m)
                    res = ai_Unk6f1bf4;
                p = *(unsigned short **)((char *)ai_Unk6f1bf4 + 0x3f);
                ai_Unk6f1bf4 = p;
                if (p == 0) {
                    if (ai_Unk6f1be9 != 0 && sp != 0) {
                        p = (unsigned short *)ai_Unk8753e0[sp - 1];
                        sp--;
                        ai_Unk6f1bf4 = p;
                        ai_Unk7e3f38 = sp;
                        if (*(int *)(p + 10) != 0) {
                            ai_Unk8753e0[sp] = *(int *)(p + 10);
                            sp++;
                            ai_Unk7e3f38 = sp;
                        }
                    }
                } else if (*(int *)(p + 10) != 0 && ai_Unk6f1be9 != 0) {
                    ai_Unk8753e0[sp] = *(int *)(p + 10);
                    sp++;
                    ai_Unk7e3f38 = sp;
                }
            } else {
                if (m)
                    res = ai_Unk6f1bf4;
                p = (unsigned short *)((char *)ai_Unk6f1bf4 + 0x43);
                ai_Unk6f1be4++;
                ai_Unk6f1bf4 = p;
            }
        } while (ai_Unk6f1be4 < f3gm_ObjectCount);
    }
    return res;
}
