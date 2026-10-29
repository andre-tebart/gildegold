/* 0x50ccd0, 1 when the character c (flag +0x1b1) has no building of the type 4 (a home) of his id (+0x65) */

int gm_Unk50ccd0(char *c)
{
    int i;
    unsigned char *b;

    if (c[0x1b1] != 0) {
        i = 0;
        b = (unsigned char *)ai_Blds;
        while (*b == 0 || ai_BTypes[*b * 0x24d] != 4 || *(int *)(b + 0x65) != *(int *)(c + 4)) {
            b += 0xa9;
            i++;
            if (i > 0xff)
                return 1;
        }
    }
    return 0;
}
