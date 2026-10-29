/* 0x50b1e0, the five best characters by the score at +0x1ac: ids[0 .. 4] (0xffff = none), sc[0 .. 4] */

void gm_Unk50b1e0(unsigned *ids, int *sc)
{
    int i, j;
    char *p;

    for (i = 0; i < 5; i++) {
        ids[i] = 0xffff;
        sc[i] = 0;
    }
    p = ai_Chars[0];
    do {
        if (*(short *)p != -1 && (unsigned char)p[2] < 10 && sc[4] < *(int *)(p + 0x1ac)) {
            sc[4] = *(int *)(p + 0x1ac);
            ids[4] = *(unsigned short *)p;
            for (j = 3; j >= 0; j--) {
                if (*(int *)(p + 0x1ac) <= sc[j] || (unsigned char)p[2] > 9)
                    break;
                sc[j + 1] = sc[j];
                ids[j + 1] = ids[j];
                sc[j] = *(int *)(p + 0x1ac);
                ids[j] = *(unsigned short *)p;
            }
        }
        p += 0x21c;
    } while ((int)p < (unsigned)((unsigned)ai_Chars + 0x65400));
}
