/* 0x4a0410, a district for character p: few neighbours closer than 66 */

int ai_Unk4a0410(G5CH *p)
{
    int order[6];
    unsigned int k;
    struct { char pad[8]; unsigned int kind; char pad2[12]; } info;
    struct { int unk00; int id; char pad[16]; } list[6];
    float sum;
    int i, n, t, a, b;

    for (i = 0; i < 6; i++)
        order[i] = i + 1;
    if (p->unk168 != 0)
        k = p->unk168;
    else if (((unsigned char *)p)[0x166] != 0)
        k = ((unsigned char *)p)[0x166];
    else if (((unsigned char *)p)[0x167] != 0)
        k = ((unsigned char *)p)[0x167];
    if (k != 0) {
        if (amt_GetKind((unsigned char)k, &info) == 0)
            return 0;
        i = ((int)(info.kind & 0xff) - 1) / 3;
        if (i == 1) {
            for (i = 0; i < 6; i++)
                order[i] = (i + 4) % 6 + 1;
        } else if (i == 2) {
            for (i = 0; i < 6; i++)
                order[i] = 6 - i;
        }
    }
    for (i = 6; i != 0; i--) {
        a = util_Random(3);
        b = util_Random(3);
        t = order[a];
        order[a] = order[b];
        order[b] = t;
        a = util_Random(3);
        b = util_Random(3) + 3;
        t = order[a + 3];
        order[a + 3] = order[b];
        order[b] = t;
    }
    for (i = 0; i <= 4; i++) {
        n = amt_GetColumnOfficesOwned(order[i], 6, list);
        if (n > 2) {
            sum = 0.0f;
            for (t = 0; t < n; t++)
                sum += gm_Liking(p->id, ((AICHAR *)gm_FindChar(list[t].id))->id, 1);
            if (sum / (float)n < 66.0f)
                return order[i];
        }
    }
    return 0;
}
