/* 0x4ce130, the record (0x21c bytes) with id (+4) of the 32 of ai_Unk930600 */

char *ai_Unk4ce130(int id)
{
    int i = 0;
    char *p;

    for (p = ai_Unk930600 + 4; (int)p < (int)(ai_Unk930600 + 4 + 32 * 0x21c); p += 0x21c) {
        if (*(unsigned short *)(p - 4) != 0xffff && *(int *)p == id)
            return ai_Unk930600 + i * 0x21c;
        i++;
    }
    return 0;
}
