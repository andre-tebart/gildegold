/* 0x52c420, for every group record (0x4ffe90 (0, 2, 7, 4, 0x1d)) sets bit 1 of +0x20 when some character (0x770cec stride 0x21c: kind 1, owner +0x1c matching, flag +0x48 & 1) belongs to it */

void sv_Unk52c420(void)
{
    char *g;
    int *p;

    g = (char *)gm_Query(0, 2, 7, 4, 0x1d);
    while (g != 0) {
        *(int *)(g + 0x20) = 0;
        p = (int *)(unsigned)&ai_Unk770cec;
        do {
            if ((unsigned short)p[-0x5b] != 0xffff && *(char *)((int)p - 0x16a) == 1 && *p != 0 && *(int *)(*p + 1) == *(int *)(g + 0x1c) && (*(unsigned char *)(p + 0x12) & 1) != 0)
                *(unsigned *)(g + 0x20) |= 1;
            p += 0x87;
        } while ((int)p < (int)((unsigned)&ai_Unk770cec + 0x65400));
        g = gm_QueryNext();
    }
}
