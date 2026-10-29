/* 0x49ee20, execute callback */

int ai_Unk49ee20(G5CH *p, AIARG *s1, AIARG *s2)
{
    unsigned char k = s1->kind;

    if (k == 4 && s2->kind == 1) {
        if (ai_Unk4aafb0(p, s1, s2) != 0)
            return 0x32;
    } else if (k == 0x13 && s2->kind == 4) {
        cm_SendSetField(p->netId, 0x1cc, 4, 0x2000000, 0);
        if (ai_Unk4ab2c0(p, s1, s2) != 0)
            return 0x32;
    } else if (k == 0) {
        cm_SendSetField(p->netId, 0x1cc, 4, 0x2000000, 0);
        return 0x32;
    }
    return 0;
}
