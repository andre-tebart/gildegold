/* 0x53eea0, counts the 0x100 entries of 0x74e1e0 (0xa4 bytes, the byte at +4 is a counter) down, clearing the first word of an expired one */

void he_Unk53eea0(void)
{
    char c;
    char *p = (char *)((unsigned)&he_QuickList + 0x6a4);

    do {
        c = *p;
        *p = c - 1;
        if ((char)(c - 1) == 0) {
            p[-4] = 0;
            p[-3] = 0;
            p[-2] = 0;
            p[-1] = 0;
        }
        p += 0xa4;
    } while ((int)p < (unsigned)&ai_Unk7585e4);
}
