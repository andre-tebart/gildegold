/* 0x4cd9d0, the record of the 10 (0x5c bytes, ai_Unk60c7c0) with id */

char *ai_Unk4cd9d0(int id)
{
    int i = 0;
    char *p;

    for (p = ai_Unk60c7c0; (int)p < (int)(ai_Unk60c7c0 + 10 * 0x5c); p += 0x5c) {
        if (*(int *)p == id)
            return ai_Unk60c7c0 + i * 0x5c;
        i++;
    }
    return 0;
}
