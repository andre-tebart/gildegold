/* 0x4fe570, loads the building and object tables of the data path root: resets the player records at 0x7d6a64 (stride 0xaa4 ints, each gets the 0xaa0 ints 0x6178c8), allocates the building prototypes (0xca78 = 0x58 * 0x24d, "f3_gm:lpGebaeudePROT") and instances (0xa900, "..INST"), reads "<root>A_Geb.dat" (0x58 records of 0x24d bytes), the object prototypes (0xb9dc = 0x2dc * 0x41, "..PROT"), instances (0x14f218) and "<root>A_Obj.dat"; builds the lookup tables (0x4fe370, 0x503e80) and adds the count of the flagged (bit 15) objects of the kinds 2 / 6 (except 0xfd) to each building's byte +0x21. Returns 0, or -1 .. -6 for the failed step. ROUGH */

int gm_LoadTables(char *root)
{
    char path[512];
    char *p;
    FILE *f;
    int i, n, cnt;
    unsigned short *w;
    unsigned short u;
    char k;
    int *dst;

    p = ai_Unk7e3ff8;
    dst = ai_Unk7d6a64;
    do {
        dst[-1] = *(int *)(p - 0x50);
        dst[0] = 1000;
        dst[1] = *(int *)p;
        dst[2] = 1000;
        memcpy(dst + 3, ai_Unk6178c8, 0xaa0 * 4);
        p += 0xbd * 4;
        dst += 0xaa4;
    } while ((int)p < ((unsigned)((unsigned)ai_Unk7e3ff8 + 0xb28) + 0x39c));
    ai_BTypes = (char *)mem_Alloc(0xca78, ai_Unk61aa38);
    if (ai_BTypes == 0)
        return -1;
    ai_Blds = (char *)mem_Alloc(0xa900, ai_Unk61aa20);
    memset(ai_Blds, 0, 0xa900);
    if (ai_Blds == 0)
        return -2;
    sprintf(path, "%s%s", root, "A_Geb.dat");
    f = fopen(path, "rb");
    if (f == 0)
        return -3;
    fread(ai_BTypes, 0x24d, 0x58, f);
    fclose(f);
    ai_Goods = (char *)mem_Alloc(0xb9dc, ai_Unk61aa00);
    if (ai_Goods == 0)
        return -4;
    ai_Objects = (char *)mem_Alloc(0x14f218, ai_Unk61a9ec);
    if (ai_Objects == 0)
        return -5;
    memset(ai_Objects, 0, 0x14f218);
    sprintf(path, "%s%s", root, "A_Obj.dat");
    f = fopen(path, "rb");
    if (f == 0)
        return -6;
    fread(ai_Goods, 0x41, 0x2dc, f);
    fclose(f);
    gm_InitBuildingTables();
    gm_Unk503e80();
    for (i = 0; i < 0xca78; i += 0x24d) {
        cnt = 0;
        w = (unsigned short *)(ai_BTypes + i + 0x23);
        for (n = 0x40; n != 0; n--, w++) {
            u = *w;
            if (u != 0xffff) {
                k = ai_Goods[(u & 0x7fff) * 0x41];
                if ((k == 2 || k == 6) && (u & 0x7fff) != 0xfd && (short)u < 0)
                    cnt++;
            }
        }
        ai_BTypes[i + 0x21] += (char)cnt;
    }
    util_Randomize();
    return 0;
}
