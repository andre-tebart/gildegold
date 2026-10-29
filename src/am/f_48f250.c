/* 0x48f250, finds the building of a character: 2 found, 1 it has none but needs one, 0 nothing to do */

int ai_Unk48f250(AIPX *p, AIBLD **out)
{
    AIBLD *b;

    if (p->kind != 3) {
        b = AI_501190_5(2, 5, 0x21, 4, p->id);
        if (out)
            *out = b;
        if (b)
            return 2;
        if (p->kind == 5)
            return 1;
        if ((p->kind == 2 || p->kind == 4) && p->unk00d > 6)
            return 1;
    }
    return 0;
}
