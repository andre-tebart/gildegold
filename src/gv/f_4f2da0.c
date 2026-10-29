/* 0x4f2da0, the highest level (0x5092e0 (b, 10, 1)) among the school buildings (kind 7) of the character *c's, times 0.01; 0.75 when one has exactly 0x4b */

float unk_Unk4f2da0(short *c)
{
    int best, i, v;
    unsigned char *b;

    best = 0;
    i = 0;
    b = (unsigned char *)ai_Blds;
    do {
        if (*b != 0 && *(short *)(b + 0x27) == *c && ai_BTypes[*b * 0x24d] == 7) {
            v = gm_BuildingSkill(b, 10, 1);
            if (v == 0x4b)
                return 0.75f;
            if (v > best)
                best = v;
        }
        i++;
        b += 0xa9;
    } while (i < 0x100);
    return (float)best * 0.01f;
}
