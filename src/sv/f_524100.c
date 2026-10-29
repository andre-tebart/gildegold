/* 0x524100, enters the character c (index) into the first free of the 8 member slots (+0x8c) of the group record g, when it is not in already */

void sv_Unk524100(int c, char *g)
{
    int i;
    int *p;

    p = (int *)(g + 0x8c);
    i = 0;
    do {
        if (*p == *(int *)(ai_Chars[c] + 4)) {
            if (i < 8)
                return;
            break;
        }
        i++;
        p++;
    } while (i < 8);
    p = (int *)(g + 0x8c);
    i = 0;
    do {
        if (*p == -1) {
            *(int *)(g + 0x8c + i * 4) = *(int *)(ai_Chars[c] + 4);
            return;
        }
        i++;
        p++;
    } while (i < 8);
}
