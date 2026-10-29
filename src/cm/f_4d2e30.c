/* 0x4d2e30, the rating of the character c for the formation type 0 .. 5 (0 the strength times a factor 1 / 0.5 / 0.2 by the
   state +0x1c4 and an empty weapon count, 1 the inverse, 2 / 3 constants, 4 / 5 5eb8b0 or 5eb780 / 0x4fc8e0), else 5eb788 */

float ai_Unk4d2e30(char *c, int type)
{
    float v;
    int k;
    unsigned char st;

    switch (type) {
    case 0:
        v = 1.0f;
        if (c[0x1c4] != 0)
            v = 0.5f;
        k = *(int *)(ai_Unk4cda50(c) + 0x58);
        if ((k == 1 || k == 2) && *(int *)(ai_Unk4cdac0(c) + 0x20) <= 0)
            v = 0.2f;
        return ai_Unk4cdcf0(c) * v;
    case 1:
        st = c[0x1c4];
        if (st == 1)
            v = 1.0f;
        else {
            v = 0.4f;
            if (st != 0)
                v = 0.5f;
        }
        k = *(int *)(ai_Unk4cda50(c) + 0x58);
        if ((k == 1 || k == 2) && *(int *)(ai_Unk4cdac0(c) + 0x20) <= 0)
            v = 1.0f;
        return v / ai_Unk4cdcf0(c);
    case 2:
        if (c[0x1c4] != 2)
            return 0.5f;
        return ai_Unk5eb780;
    case 3:
        return ai_Unk5eb780;
    case 4:
    case 5:
        if (c[0x1c4] == 2)
            return 0.2f;
        return ai_Unk5eb780 / unk_Unk4fc8e0(c);
    }
    return ai_Unk5eb788;
}
