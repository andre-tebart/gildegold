/* 0x4e3750, 1 when no member slot of the party m is used (+0), alive (+8 == 0) and waiting (+0x10 != -1) */

int ai_Unk4e3750(char *m)
{
    int i, n, ok;
    char *s;

    ok = 1;
    i = 0;
    n = (unsigned char)m[0x30];
    if (n > 0) {
        s = ai_Unk92cae0;
        do {
            if (*(int *)s != 0 && *(int *)(s + 8) == 0 && *(int *)(s + 0x10) != -1)
                return 0;
            i++;
            s += 0x35c;
        } while (i < n);
    }
    return ok;
}
