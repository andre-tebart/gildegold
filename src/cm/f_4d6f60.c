/* 0x4d6f60, the fighter slot f (+0 id, +4 active) of an active character: 0x405610 on its object */

void ai_Unk4d6f60(char *f)
{
    char *c;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        if (c != 0)
            ch_StopActions(*(char **)(c + 0x184));
    }
}
