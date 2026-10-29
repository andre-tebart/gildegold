/* 0x4cd380, the record with id, else a free one */

char *ai_Unk4cd380(int id)
{
    int i = 0;
    char *p;

    if (id == 0)
        return 0;
    for (p = ai_Unk9364e0; (int)p < (int)(ai_Unk9364e0 + 8 * 0x57); p += 0x57) {
        if (*(int *)p == id)
            return ai_Unk9364e0 + i * 0x57;
        i++;
    }
    return ai_Unk4cd350();
}
