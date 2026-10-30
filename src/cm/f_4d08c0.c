/* 0x4d08c0, clears the marks of the 32 records ai_Unk930600[0x21c] (+0x188) and the highlight of their models */

void ai_Unk4d08c0()
{
    char *r;

    for (r = ai_Unk930600; (int)(r + 0x184) < ((unsigned)&ai_Unk930600 + 0x4504); r += 0x21c) {
        if (r[0x188] != 0)
            r[0x188] = 0;
        if (*(int *)(r + 0x184) != 0) {
            char *mdl = *(char **)(*(char **)(r + 0x184) + 0x34);

            if ((mdl[0x212] & 2) == 2)
                d3t_SetColorK(mdl, 0, 0, 0);
        }
    }
    d2_ClearPointerItems();
    ai_Unk4cec00();
    ai_Unk4cefe0();
}
