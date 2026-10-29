/* 0x4e37a0, the scene party m (+0x30 count): every member slot of 0x92cae0 that is used and not yet announced (+0xc) shows its text (+0x14, then 0x60c6c0) */

int ai_Unk4e37a0(char *m)
{
    int i;
    char *s;

    s = ai_Unk92cae0;
    for (i = 0; i < (unsigned char)m[0x30]; i++, s += 0x35c) {
        if (*(int *)(s + 0xc) == 0 && *(int *)(s + 8) != 0) {
            d2_SetForm(*(int *)(m + 0x2c), 0);
            io_PrintText(s + 0x14);
            io_PrintText((unsigned)&ai_Unk60c6c0);
            *(int *)(s + 0xc) = 1;
        }
    }
}
