/* 0x505df0, the character with the id (int +4) in the character table, 0 when none */

short *gm_FindChar(int id)
{
    int *p;
    int i;

    i = 0;
    p = (int *)(ai_Chars[0] + 4);
    while ((short)p[-1] == -1 || *p != id) {
        p += 0x87;
        i++;
        if ((int)p >= (int)((unsigned)&ai_Chars + 0x65404))
            return 0;
    }
    return (short *)ai_Chars[i];
}
