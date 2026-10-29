/* 0x4e5e50, the rating of the character a (+0x164 / +0x166 the office rank and party bytes) as seen by b: the float tables 0x5edc90 (15 rows of 10: id, 9
   columns) and 0x5edef0 (5 rows) by the rank (amt_KindColumn, 1 .. 6) and the status bits +0x1cc (0x400 .. 0x2000, 0x200 give the keys 0x24 .. 0x27, 0x23);
   mode (>= 2: plus the absolute value of the entry, >= 3: plus 10.0); with flag the friendship value of amt_GetKind / 0x4f2720 is taken off (a = b's party: added,
   mode 2: times 0.75). Every branch checked against the original on 2026-10-25 (the tabA / tabB loops, the status-key
   chain, the +10.0 and the flag block); the remaining instruction difference is register allocation / scheduling
   @deep B3 323 code hunks, 171 of 187 instructions differ; every branch read against the original (tabA/tabB loops, status-key chain, +10.0, flag block, tail) and the statement sequence is the same - the hunks are the same instructions with the values in other registers/slots from instruction 0 (original k=ebx, a=[esp+0x54], b=ebp, r=[esp+0x58]); allocation/scheduling only. spell.py: dup-tail (explicit returns in the arm, merged tail) and reload ([esp+4], parameter a) tried, one edit one build each, no gain. Readability Q1-Q7 pass (AMT_STATUS/AMT_BITS/AMTKINDF views; amt_Rating/amt_KindColumn/amt_GetKind/gv_GetTableRecord) */

float amt_Rating(char *a, char *b, int mode, int flag)
{
    float r, fl;
    char buf[0x40];
    int k, i, key, n;
    unsigned int f2;

    r = 0.0f;
    k = amt_KindColumn((unsigned char)b[0x166]);
    if (k > 0 && k < 7) {
        for (i = 0; amt_TabA[i * 10] != (float)gm_TitleGroup((unsigned char)a[0x164]) && (int)&amt_TabA[i * 10] < AMT_TABA_END; i++)
            ;
        r = amt_TabA[i * 10 + k + 1];
        if ((unsigned int)mode >= 2)
            r = r + (float)abs((int)amt_TabA[i * 10 + k + 1]);
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
        if ((f2 & 0x3c00) == 0 && ((f2 & 0x200) ? 0x23 : 0) == 0) {
            for (n = 0; n < 5 && AMT_BITS(&amt_TabB[n * 10]) != key; n++)
                ;
            if (n < 5)
                r = r + amt_TabB[n * 10 + amt_KindColumn((unsigned char)b[0x166]) + 1];
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
