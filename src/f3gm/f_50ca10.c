/* 0x50ca10, tells every character that has the flag byte at +0x166 the message a about b */

void gm_Unk50ca10(unsigned short a, int b)
{
    int i;
    char *p;

    i = 0;
    p = ai_Chars[0] + 0x166;
    do {
        if (*p != 0)
            cm_SendRelations(a, i, b, 1);
        p += 0x21c;
        i++;
    } while ((int)p < (int)((unsigned)&ai_Chars + 0x65566));
}
