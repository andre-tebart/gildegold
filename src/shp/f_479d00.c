/* 0x479d00, makes the light table: every 16 bit colour brightened by add @deep 38 code hunks / 22 regs, register allocation only, and the unit note's Open work records this exact case as tried (r=g=b=0, temp pointer, assign-in-if): the original materialises the zero in ebx at entry (xor ebx,ebx), stores the three byte locals as bl, compares the mem_Alloc result with cmp eax,ebx and uses edi for lim / esi for the 0x10000 counter; ours stores immediates, uses test eax,eax and swaps lim / counter (esi / edi). The rest of the body read against the original 0x479d00..0x479def: mem_Alloc(0x20000, 'd2:shp:shp_LightTable'), lim = 0xff - add, the 0x10000 loop with dd_SplitRGB(i,&r,&g,&b), the three b/g/r clamps against lim (add or 0xff), dd_RGB and the 16 bit table store, return 1 / 0 - every constant, callee and condition corresponds. Readability: Q1 only (the mem tag gives the role). */
int d2_MakeLightTable(char add)
{
    int i;
    int lim;
    unsigned char r, g, b;

    r = g = b = 0;
    d2_ShpLightTable = mem_Alloc(0x20000, "d2:shp:shp_LightTable");
    if (d2_ShpLightTable != 0) {
        lim = 0xff - add;
        for (i = 0; i < 0x10000; i++) {
            dd_SplitRGB(i, &r, &g, &b);
            if (b < lim)
                b += add;
            else
                b = 0xff;
            if (g < lim)
                g += add;
            else
                g = 0xff;
            if (r < lim)
                r += add;
            else
                r = 0xff;
            d2_ShpLightTable[i] = (unsigned short)dd_RGB(r, g, b);
        }
        return 1;
    }
    return 0;
}
