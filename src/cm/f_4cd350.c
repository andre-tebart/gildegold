/* 0x4cd350, a free slot of the 8 records (0x57 bytes) of ai_Unk9364e0, 0: none */

char *ai_Unk4cd350()
{
    int i = 0;
    char *p;

    for (p = ai_Unk9364e0; (int)p < (int)(ai_Unk9364e0 + 8 * 0x57); p += 0x57) {
        if (*(int *)p == 0)
            return ai_Unk9364e0 + i * 0x57;
        i++;
    }
    return 0;
}
