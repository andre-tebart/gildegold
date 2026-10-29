/* 0x4ce170 */

int ai_Unk4ce170(int idx, char *c)
{
    int r = 0;

    if (*(char **)(c + 0x16c) == ai_Chars[idx])
        r = 0x800;
    if (ai_Unk6f2448 != 0)
        r = 0x800;
    return r;
}
