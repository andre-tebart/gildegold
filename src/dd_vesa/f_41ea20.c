/* 0x41ea20, three 256-entry tables mapping 8-bit channel values to pixel bits */

void dd_MakeColorTables(DWORD rmask, DWORD gmask, DWORD bmask, DWORD *tab)
{
    unsigned char rl, rs, gl, gs, bl, bs;
    DWORD i;

    dd_GetMaskShifts(rmask, gmask, bmask, &rl, &rs, &gl, &gs, &bl, &bs);
    for (i = 0; i < 256; i++) {
        tab[i] = i >> rl << rs;
        tab[256 + i] = i >> gl << gs;
        tab[512 + i] = i >> bl << bs;
    }
}
