/* 0x49d460, execute callback 0x2a: sets flag 0x200 of +0x1e8 */

int ai_Unk49d460(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (ai_Unk4aaae0(p, s1, s2) != 0) {
        cm_SendSetField(p->netId, 0x1e8, 4, 0x200, 0);
        return 0x2a;
    }
    return 0;
}
