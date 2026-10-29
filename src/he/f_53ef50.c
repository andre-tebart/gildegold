/* 0x53ef50, counts the AP events down; an expired one loses its values */

void he_Unk53ef50(void)
{
    char c;
    char *p = (char *)((unsigned)&he_RunHandlers + 0x6a0);

    do {
        c = *p;
        *p = c - 1;
        if ((char)(c - 1) < 1) {
            p[8] = 0; p[9] = 0; p[10] = 0; p[11] = 0;
            p[12] = 0; p[13] = 0; p[14] = 0; p[15] = 0;
            p[16] = 0;
        }
        p += 0x50;
    } while ((int)p < (unsigned)((unsigned)he_RunHandlers + 0x56a0));
}
