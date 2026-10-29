/* 0x52b2a0, collects the (up to 0x20) characters 0x770d04 + n * 0x21c that belong to the group b (b == 0: the ones with no group, +0x2c == -1) into the table 0x7603a0 */

void sv_CollectGroupCharacters(char *b)
{
    int i, *p, *q;

    p = (int *)(unsigned)&ai_Unk7603a0;
    for (i = 0x20; i != 0; i--)
        *p++ = 0;
    if (b == 0) {
        p = (int *)(unsigned)&ai_Unk7603a0;
        q = (int *)((unsigned)&ai_Unk770cec + 0x18);
        do {
            if ((int)p > ((unsigned)&ai_Unk7603a0 + 0x7b))
                return;
            if (*q != 0 && *(int *)(*q + 0x2c) == -1)
                *p++ = (int)(q - 0x61);
            q += 0x87;
        } while ((int)q < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182));
    } else {
        p = (int *)(unsigned)&ai_Unk7603a0;
        q = (int *)((unsigned)&ai_Unk770cec + 0x18);
        while ((int)p < ((unsigned)&ai_Unk7603a0 + 0x7c)) {
            i = *q;
            if (i != 0 && *(int *)(i + 0x2c) == *(int *)(b + 1)) {
                *p++ = (int)(q - 0x61);
                if ((int)p > ((unsigned)&ai_Unk7603a0 + 0x20))
                    ch_SetVisible(i, 0);
            }
            q += 0x87;
            if ((int)q > ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x181))
                return;
        }
    }
}
