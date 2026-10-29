/* 0x49d280, execute callback 0x29: sets flag 0x100 of +0x1e8 */

int ai_Unk49d280(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (ai_Unk4aa940(p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x100, 0);
        return 0x29;
    }
    return 0;
}
