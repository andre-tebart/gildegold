/* 0x53eec0, registers an AP event in the first free of the 0x100 entries at 0x758ca0 (0x50 bytes: counter byte 2, values +4 / +8 / +0xc, name +0x10, 0x3f characters); 0 when full */

int *he_Unk53eec0(int a, char *name, int c, int d)
{
    int *p;
    int i = 0;

    p = (int *)((unsigned)&he_RunHandlers + 0x6ac);
    while (p[-1] != 0 || *p != 0 || (char)p[1] != 0) {
        p += 0x14;
        i++;
        if ((int)p > ((unsigned)((unsigned)he_RunHandlers + 0x56a0) + 0xb)) {
            err_Message("he_RegisterApEvent(): Too many ApEvents...");
            return 0;
        }
    }
    if (i > 0xff) {
        err_Message("he_RegisterApEvent(): Too many ApEvents...");
        return 0;
    }
    U_I(((unsigned)&he_RunHandlers + 0x6a4) + i * 0x50) = a;
    U_I(((unsigned)&he_RunHandlers + 0x6a8) + i * 0x50) = c;
    U_I(((unsigned)&he_RunHandlers + 0x6ac) + i * 0x50) = d;
    *(char *)(((unsigned)&he_RunHandlers + 0x6a0) + i * 0x50) = 2;
    strncpy((char *)(((unsigned)&he_RunHandlers + 0x6b0) + i * 0x50), name, 0x3f);
    return (int *)(((unsigned)&he_RunHandlers + 0x6a0) + i * 0x50);
}
