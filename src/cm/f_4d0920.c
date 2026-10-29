/* 0x4d0920, a record of the 32 ai_Unk930600[0x21c] for the character c: the first free one (+0 == 0xffff) gets a copy of
   the template of 0x505df0 (0x87 ints), an object (4fbb80) with the name "%i" of its id and the speed from the
   character's kind; the kind 2 gets the count 6, else 0xf. Rough draft. */

char *ai_Unk4d0920(char *c, int a)
{
    char *t, *r, *k, *w;
    int i;

    t = gm_FindChar((int)c);
    for (i = 0, r = ai_Unk930600; *(unsigned short *)r != 0xffff; i++, r += 0x21c) {
        if ((int)(r + 0x21c) >= ((unsigned)&ai_Unk930600 + 0x4380))
            return 0;
    }
    memcpy(r, t, 0x21c);
    *(int *)(r + 0x1ac) = 0;
    *(int *)(r + 0x184) = 0;
    unk_Unk4fbb80(r, d3_CurrentScene, 0, 0, 0, a);
    *(char *)(*(char **)(r + 0x184) + 0x240) = 4;
    *(char *)(*(char **)(r + 0x184) + 0x8d) |= 0x80;
    ai_Unk4cde20(r);
    k = ai_Unk4cda50(c);
    *(float *)(*(char **)(r + 0x184) + 0x1a0) = (float)(unsigned char)k[9] * ai_Unk5eb9b8;
    sprintf(*(char **)(*(char **)(r + 0x184) + 0x34), "%i", *(int *)(r + 4));
    w = ai_Unk4cdac0(c);
    if (w != 0) {
        if (*(int *)(k + 0x58) == 2)
            *(int *)(w + 0x20) = 6;
        else
            *(int *)(w + 0x20) = 0xf;
    }
    return r;
}
