/* 0x48f1e0, execute of 0x48f010 */

int ai_Unk48f1e0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    AIBLD *x, *y;

    if (s1->kind != 4 || s2->kind != 4)
        return 0;
    gm_FindById((int)&x, 0, 0, s1->unk04);
    if (!x)
        return 0;
    gm_FindById((int)&y, 0, 0, s2->unk04);
    if (!y)
        return 0;
    return ai_Unk4a6ae0(p, x) ? 0x42 : 0;
}
