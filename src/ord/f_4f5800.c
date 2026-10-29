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
