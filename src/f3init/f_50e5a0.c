/* 0x50e5a0, lays out the n offices of the list ids (the office indexes, a byte each) in a row: every office x
   is the slot table tab[] (24 39 51 63 69 18 57 6 12 30 75 94 88 81 45, a back-and-forth of the 232-wide row)
   minus half the level of the office's kind (amt_GetOffice -> the AMTREC kind at +8, amt_GetKindLevel), written
   to out[i]; the slot index starts at a random one and wraps through the table. Returns the number of wraps
   (the last (s + 1) / 15), i.e. 0 when n <= 15.
   @deep behaviour draft, 3 differing asm lines (2 code hunks + 1 fill) after the whole original 0x50e5a0..0x50e690 was read: the only real difference is the store of the random slot index - the original does a 16-bit store (mov si,ax) then zero-extends once before the loop (and esi,0xffff), ours does mov esi,eax with the same extension (the compiler knows util_Random's unsigned short return). Tried one edit one build each: r/s both unsigned short (the mov matches but the extension moves into the loop, 39 lines), s = util_Random(0xf) with r assigned only in the loop (mov matches, the extension is not hoisted, 19 lines), r = util_Random(0xf) & 0xffff (no change), the call through a (int (*)(unsigned short)) cast of util_Random (no change). The rest is equal, including the frame (rec[0x18] + tab[15]) and the amt_GetOffice byte argument. Readability done: Q1 init_PlaceOffices, Q2 cleared (amt_GetOffice, amt_GetKindLevel), Q3 none, Q4 cleared by the INIT_OFFICE view, Q5 draft marker removed, Q6 described. */


unsigned short init_PlaceOffices(int n, int *out, int *ids)
{
    int tab[15];
    unsigned short r;
    unsigned s;
    int *q;
    int w;
    INIT_OFFICE rec;
    unsigned u;


    tab[0] = 0x18;
    tab[1] = 0x27;
    tab[2] = 0x33;
    tab[3] = 0x3f;
    tab[4] = 0x45;
    tab[5] = 0x12;
    tab[6] = 0x39;
    tab[7] = 6;
    tab[8] = 0xc;
    tab[9] = 0x1e;
    tab[10] = 0x4b;
    tab[11] = 0x5e;
    tab[12] = 0x58;
    tab[13] = 0x51;
    tab[14] = 0x2d;
    r = util_RandomU(0xf);
    s = r;
    if (0 < n) {
        q = out;
        do {
            amt_GetOffice(((unsigned char *)ids)[(char *)q - (char *)out], &rec);
            u = rec.kindWord;
            w = amt_GetKindLevel(u & 0xff);
            *q = tab[s] - w / 2;
            r = (int)(s + 1) / 0xf;
            s = (int)(s + 1) % 0xf;
            q++;
        } while (--n != 0);
    }
    return r;
}
