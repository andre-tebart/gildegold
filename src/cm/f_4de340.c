/* 0x4de340, answers a request m of the AI's session code (kinds 0 .. 6; the bodies of 6, 4, 5 in this order) of the record r (+0x10 the asking character, +0x94 the result, +0x98 a
   value): each kind asks one of the decision functions 0x494020 .. 0x4946c0 when the character matches the one named in m */

void ai_Unk4de340(char *r, int *m)
{
    int c, id, res;

    *(int *)(r + 0x94) = -1;
    switch (m[0]) {
    case 0:
        if (*(int *)(r + 0x10) != m[6])
            return;
        c = gm_FindChar(m[6]);
        *(float *)(r + 0x98) = (float)*(int *)(c + 0x24) + *(float *)(c + 0x1c);
        *(int *)(r + 0x94) = ai_Unk494020(m);
        return;
    case 1:
        if (*(int *)(r + 0x10) != m[5])
            return;
        *(int *)(r + 0x94) = ai_Unk4941b0(m);
        return;
    case 2:
        if (*(int *)(r + 0x10) != m[5])
            return;
        *(int *)(r + 0x94) = ai_Unk4941d0(m);
        return;
    case 3:
        if (*(int *)(r + 0x10) != m[10])
            return;
        res = ai_Unk4941e0(m);
        *(int *)(r + 0x94) = res;
        if (res == 1)
            cm_SendRelations(*(int *)(r + 0x10), m[6], 0x23, res);
        return;
    case 6:
        if (*(int *)(r + 0x10) != m[6])
            return;
        *(int *)(r + 0x94) = util_RandomU(2) & 0xffff;
        return;
    case 4:
        id = *(int *)(r + 0x10);
        if (id != m[7] && id != m[8] && id != m[9])
            return;
        c = gm_FindChar(*(int *)(ai_Unk6f0d74 + 4));
        *(int *)(r + 0x94) = ai_Unk494410(c, m);
        return;
    case 5:
        if (*(int *)(r + 0x10) != m[7])
            return;
        c = gm_FindChar(m[7]);
        *(int *)(r + 0x94) = ai_Unk4946c0(c, m);
        return;
    }
}
