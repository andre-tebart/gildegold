/* 0x49c8e0, decide callback 0x26: on its day / hour slot, the character rents rooms (?) in a building
   (1, 5, 7) of another owner; the price depends on its fame */

int ai_Unk49c8e0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int n1, n2, i, money, w, c;
    float x, t;
    unsigned char *b;
    unsigned char *list[10];

    if (f != 0)
        return 0;
    if ((p->netId & 3) != ai_TimeT.year % 4)
        return 0;
    if ((p->netId & 7) != (int)ai_TimeT.hour % 8)
        return 0;
    n1 = unk_Unk4f39e0(p->netId);
    n2 = unk_Unk4f39b0(p->netId);
    if (util_RandomU(8) + 8 > n1)
        return 0;
    i = 0;
    for (b = (unsigned char *)gm_BuildQuery(1, 5, 7); b != 0; b = (unsigned char *)gm_BuildNext()) {
        if (*(unsigned short *)(b + 0x27) != 0xffff && *(unsigned short *)(b + 0x27) != p->id
            && !(b[0x5a] & 1))
            list[i++] = b;
    }
    if (i == 0)
        return 0;
    b = list[util_RandomU(i)];
    if (*(int *)((char *)p + 0x194) < 4) {
        money = gm_Unk509ee0((AIPER *)p, ai_OwnPlayer);
        x = gm_GetWorth(p->id) * 0.005f;
        w = (int)((200.0f - gm_Liking(p->id, *(unsigned short *)(b + 0x27), 1)) * 0.01f * n1 * x);
        t = w * 0.01;
        if (t > 3200.0) {
            if (!(t < 320000.0))
                t = 320000.0;
        } else
            t = 3200.0;
        w = (int)t;
        if (ai_InventoryHas(p, 0x2c6))
            w /= 2;
        if (money * 0.44f < w)
            return 0;
        if (n2 <= 1)
            n2 = 1;
        o1->kind = 4;
        o1->unk04 = *(int *)(b + 1);
        o1->unk10 = w;
        o1->unk14 = (float)n2;
        o2->kind = 0x12;
        return 0x26;
    }
    c = *(int *)((char *)p + 0x194) / 2;
    c += c & 1;
    w = c / 2;
    if (w > n2) {
        w = n2;
        c = n2 * 2;
    }
    o1->kind = 4;
    o1->unk04 = *(int *)(b + 1);
    o1->unk10 = c;
    o1->unk14 = (float)w;
    o2->kind = 0xf;
    return 0x26;
}
