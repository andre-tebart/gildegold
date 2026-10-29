/* 0x52f360, opens a book ("f3_sv:OpenBook", 0x274 bytes: +0 the book object, +4 its parent, +0x228 the page widgets (n of them, +0x271), +0x270 the first shown page, +0x268 / +0x26c the page button ids, +0x272 the
   saved 0x63342d): the book model a (0x411900), optionally put at the scene object b (0x446e10); n page widgets (0x474b30 (0, 0, text)) with the "Buch_Seite<i>_oben / unten_ns_nm" pictures; the n texts follow
   as variable arguments; plays "Aufklappen_s"; ROUGH */

int *sv_Unk52f360(int a, int b, char n, ...)
{
    int *bk;
    int i, w, *p;
    char top[0x60], bottom[0x60];
    int *arg;

    bk = (int *)mem_Alloc(0x274, "f3_sv:OpenBook");
    if (bk == 0)
        return 0;
    w = d3t_FindObject(a, 1);
    bk[0] = w;
    if (w == 0) {
lfail:
        mem_Free(bk);
        return 0;
    }
    if (b == 0) {
        bk[1] = 0;
    } else {
        d3if_Show(w, 0);
        w = bk[0];
        bk[1] = w;
        w = d3_CreateObject(*(int *)(w + 0x1f8), w + 0x4c, w + 0x84, b);
        bk[0] = w;
    }
    *(unsigned char *)(bk[0] + 0x210) &= 0xdf;
    d3_RelightObject(bk[0]);
    *((char *)bk + 0x271) = n;
    w = (int)n;
    *((char *)(bk + 0x9c)) = 0;
    bk[0x9a] = -1;
    bk[0x9b] = -1;
    if (0 < w) {
        arg = (int *)(&n + 4);
        p = bk + 0x8a;
        i = 0;
        do {
            *p = d2_LoadView(0, 0, *arg++);
            if (*p == 0)
                goto lfail;
            sprintf(top, "Buch_Seite%i_oben_ns_nm", i + 1);
            sprintf(bottom, "Buch_Seite%i_unten_ns_nm", i + 1);
            d2_FillViewTextures(*p, top, bottom);
            d2_RedrawView(*p);
            if (2 < i)
                d2_EnableViewObjects(*p, 0);
            p++;
            i++;
        } while (i < w);
    }
    if (SV_B((unsigned)&d3d_ZEnabled) != 0) {
        *((char *)bk + 0x272) = SV_B((unsigned)&d3d_ZEnabled);
        d3d_SetWBuffer(0);
    }
    sb_PlaySample(SV_I((unsigned)&full_data_6f240c), "Aufklappen_s", 1, 0x3f, 0x7f, 0, 0);
    return bk;
}
