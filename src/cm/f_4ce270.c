/* 0x4ce270, 1 when the two characters are not in the same building (+0x16c) */

int ai_Unk4ce270(char *a, char *b)
{
    return *(int *)(a + 0x16c) != *(int *)(b + 0x16c);
}
