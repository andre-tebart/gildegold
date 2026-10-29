/* 0x4eff50, upgrades the graveyard record of the object o to the next size (kind - 1 .. : the building's kind byte is one more than the record's +4): allocates a new one (0x4efc10), copies the first grave
   records, the entries of the lists and the four flag bytes (+0x18 .. +0x1b) over, frees the old one (0x4f0180); returns the new record, or 0 (the old one stays) when it does not fit. ROUGH */

int unk_Unk4eff50(unsigned char *o)
{
    int *old, *nw;
    char *b;
    int *s, *d;
    int i, n0, n1, n2, k;

    old = *(int **)(o + 0x65);
    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] == 0x1d && old != 0 && old[1] < 3 && (unsigned char)b[0x247] - old[1] == 1) {
        *(int *)(o + 0x65) = 0;
        nw = gv_AllocGraveyard(o);
        if (nw != 0) {
            s = *(int **)(old[2] + 0x10);
            d = *(int **)(nw[2] + 0x10);
            if ((unsigned char)b[0x247] == 2) {
                k = 0;
                n0 = 8;
                n1 = 4;
                for (i = 0x88; i != 0; i--)
                    *d++ = *s++;
                s = *(int **)(old[3] + 0x10);
                d = *(int **)(nw[3] + 0x10);
            } else {
                k = 2;
                n0 = 0xc;
                n1 = 6;
                for (i = 0xcc; i != 0; i--)
                    *d++ = *s++;
                s = *(int **)(old[3] + 0x10);
                d = *(int **)(nw[3] + 0x10);
                for (i = 0xcc; i != 0; i--)
                    *d++ = *s++;
                s = *(int **)(old[4] + 0x10);
                d = *(int **)(nw[4] + 0x10);
            }
            for (i = 0x88; i != 0; i--)
                *d++ = *s++;
            if (old[2] != 0 && n0 != 0) {
                for (i = 0; n0 != 0; n0--, i += 0x14) {
                    *(int *)(i + nw[2]) = *(int *)(i + old[2]);
                    *(int *)(nw[2] + 4 + i) = *(int *)(i + 4 + old[2]);
                    *(short *)(nw[2] + 8 + i) = *(short *)(i + 8 + old[2]);
                    *(short *)(nw[2] + 10 + i) = *(short *)(i + 10 + old[2]);
                    *(char *)(nw[2] + 0xc + i) = *(char *)(i + 0xc + old[2]);
                }
            }
            if (old[3] != 0 && n1 != 0) {
                for (i = 0; n1 != 0; n1--, i += 0x14) {
                    *(int *)(i + nw[3]) = *(int *)(i + old[3]);
                    *(int *)(i + 4 + nw[3]) = *(int *)(i + 4 + old[3]);
                    *(short *)(i + 8 + nw[3]) = *(short *)(i + 8 + old[3]);
                    *(short *)(i + 10 + nw[3]) = *(short *)(i + 10 + old[3]);
                    *(char *)(i + 0xc + nw[3]) = *(char *)(i + 0xc + old[3]);
                }
            }
            if (old[4] != 0 && k != 0) {
                for (i = 0; k != 0; k--, i += 0x14) {
                    *(int *)(i + nw[4]) = *(int *)(i + old[4]);
                    *(int *)(i + 4 + nw[4]) = *(int *)(i + 4 + old[4]);
                    *(short *)(i + 8 + nw[4]) = *(short *)(i + 8 + old[4]);
                    *(short *)(i + 10 + nw[4]) = *(short *)(i + 10 + old[4]);
                    *(char *)(i + 0xc + nw[4]) = *(char *)(i + 0xc + old[4]);
                }
            }
            *((char *)nw + 0x18) = *((char *)old + 0x18);
            *((char *)nw + 0x19) = *((char *)old + 0x19);
            *((char *)nw + 0x1a) = *((char *)old + 0x1a);
            *((char *)nw + 0x1b) = *((char *)old + 0x1b);
            gv_FreeGraveyard(old);
            return *(int *)(o + 0x65);
        }
        *(int **)(o + 0x65) = old;
    }
    return 0;
}
