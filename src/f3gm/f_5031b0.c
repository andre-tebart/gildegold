/* 0x5031b0, "gm_AllocRaum": creates the building b's (kind) object n with all its sub-objects (the object words after it up to the next kind 2 / 6 object that are marked negative), sets the counts of its first kind 0x2a / 0x116 objects (the bytes at +0x23d / +0x23e / +0x23f of the building prototype, at least 2 / 6), and for the market stalls 0x92 .. 0x97 / 0x2db the store room ob_LAGERFLAECHE_ALLGEMEIN with the player's goods. ROUGH */

unsigned short *unk_Unk5031b0(unsigned char *b, unsigned n)
{
    unsigned short *o, *s;
    int i, bi, k, pl, cap;
    char *bp;
    unsigned short w;
    char cl;
    char *g;
    int *gp;
    unsigned char c1, c2;
    int *goods;
    int *tab;

    if (ai_Goods == 0 || ai_BTypes == 0 || f3gm_ObjectCount + 0x40 > 0x5007)
        return 0;
    o = (unsigned short *)gm_CreateObject(*(int *)(b + 1), n, 1);
    i = 0;
    bp = ai_BTypes + (unsigned)*b * 0x24d;
    k = (int)(bp + 0x23);
    w = *(unsigned short *)(bp + 0x23);
    while ((w & 0x7fff) != n) {
        k += 2;
        i++;
        w = *(unsigned short *)k;
    }
    while (i++, i < 0x40) {
        w = *(unsigned short *)(bp + 0x23 + i * 2);
        if (w == 0)
            break;
        if (w != 0xffff) {
            cl = ai_Goods[(w & 0x7fff) * 0x41];
            if (cl == 2 || cl == 6)
                i = 0x40;
            else if ((short)w < 0)
                gm_CreateObject(*(int *)(o + 1), w & 0x7fff, 1);
        }
    }
    g = gm_Query(*(int *)(o + 10), 1, 0, 0x2a);
    if (g != 0) {
        *(int *)(g + 0xe) = 2;
        *(int *)(g + 6) = *(int *)(b + 1);
        c1 = (unsigned char)bp[0x23d];
        if (c1 == 0 && bp[0x23e] == 0 && bp[0x23f] != 0)
            g[0x1c] = bp[0x23f];
        else {
            if (c1 < 2)
                c1 = 2;
            g[0x1c] = c1;
            c2 = (unsigned char)bp[0x23e];
            if (c2 < 2)
                c2 = 2;
            g[0x1d] = c2;
        }
    }
    g = gm_Query(*(int *)(o + 10), 2, 0, 0x116, 6);
    if (g != 0) {
        *(int *)(g + 0xe) = 2;
        *(int *)(g + 6) = *(int *)(o + 1);
        c1 = (unsigned char)bp[0x23d];
        if (c1 == 0 && bp[0x23e] == 0 && (c2 = (unsigned char)bp[0x23f]) != 0) {
            if (c2 < 6)
                g[0x1c] = 6;
            else
                g[0x1c] = c2;
        } else {
            if (c1 < 6)
                c1 = 6;
            g[0x1c] = c1;
            c2 = (unsigned char)bp[0x23e];
            if (c2 < 6)
                c2 = 6;
            g[0x1d] = c2;
        }
    }
    w = *o;
    if ((0x91 < w && w < 0x98) || w == 0x2db) {
        pl = ai_OwnPlayer;
        g = gm_Query(*(int *)(o + 10), 1, 0, 0x116);
        if (g == 0)
            err_Message("gm_AllocRaum(): ob_LAGERFLAECHE_ALLGEMEIN is missing in ob_MARKTSTAND!");
        else {
            g[0x1c] = 0xc;
            g[0x1d] = 0;
            *(int *)(g + 0xe) = 3;
        }
        bi = gm_CreateObject(*(int *)(o + 1), 0xff, 1);
        cap = 0;
        goods = ai_Unk6178c8;
        gp = (int *)(init_StockBlocks + 4 + pl * 0x2a90);
        do {
            if (*(unsigned short *)(gp + 0xe) == *o && *gp != 0 && cap < 0x10) {
                gm_CreateObject(*(int *)(bi + 2), *(unsigned short *)goods, 1);
                cap++;
            }
            goods += 0x10;
            gp += 0x20;
        } while ((int)goods < (unsigned)((unsigned)ai_Unk6178c8 + 0x2a80));
    }
    return o;
}
