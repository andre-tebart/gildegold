/* 0x50b170, the rank (1 = best) of the character c among the living persons by the score at +0x1ac (plus d) */

int gm_Unk50b170(int c, int d)
{
    int r, i;
    char *p;

    r = 1;
    if (c >= 0 && c < 0x300) {
        i = 0;
        p = ai_Chars[0];
        do {
            if (*(int *)(ai_Chars[c] + 0x1ac) + d <= *(int *)(p + 0x1ac) && *(short *)p != -1 && (unsigned char)p[2] < 10 && i != c)
                r++;
            p += 0x21c;
            i++;
        } while ((int)p < (unsigned)((unsigned)ai_Chars + 0x65400));
        return r;
    }
    return 0;
}
