/* 0x4b09f0, the GESETZ text command (callback 0x601b50[13], name 0x601070[13])
   "-RECHTSPRECHUNG_HAERTE_n" / "-AEMTEREINKOMMEN_n": sets the AI decision 0 / 15 to n, clamped to its range
   (refused out of range when its byte 1 is set) */

int gv_SetTableRecord(short *c, int i, int v);                                        /* 0x4f2750, sets the AI decision table record i of the character c to v (clamped) */

int ai_CommandLaw(int a, int b, char *fmt)
{
    if (*fmt != '-')
        return 0;
    {
        char name[2][32] = { "RECHTSPRECHUNG_HAERTE", "AEMTEREINKOMMEN" };
        int kinds[2];
        int i, n, k;
        AIDEC d;

        kinds[0] = 0;
        kinds[1] = 0xf;
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        n = atoi(fmt + 1);
        k = kinds[i];
        if (!gv_GetTableRecord(k, &d))
            return 0;
        if ((n < d.min || n > d.max) && ((unsigned char *)&d)[1] != 0)
            return 0;
        gv_SetTableRecord(0, k, max(min(d.max, n), d.min));
        return 1;
    }
}
