/* 0x4cde20, the model name "ob_<NAME>" of the record kind (+0x64 of the record +0x184) of the character */

void ai_Unk4cde20(char *c)
{
    char buf[0x80];
    int *r;
    char *g;

    r = (int *)ai_Unk4cda50(c);
    if (r != 0 && *r != 0) {
        if (*(char **)(c + 0x184) == 0)
            return;
        sprintf(buf, "ob_%s", _strupr(ai_Goods + *r * 0x41 + 1));
        g = *(char **)(c + 0x184);
        if (*(int *)(g + 0x64) != 0 && strcmp(*(char **)(g + 0x64), buf) == 0)
            return;
        g = *(char **)(c + 0x184);
        if (*(int *)(g + 0x64) != 0)
            ch_SetHandObject(g, 2, 0);
        ch_SetHandObject(*(char **)(c + 0x184), 2, buf);
        return;
    }
    g = *(char **)(c + 0x184);
    if (g != 0 && *(int *)(g + 0x64) != 0)
        ch_SetHandObject(g, 2, 0);
}
