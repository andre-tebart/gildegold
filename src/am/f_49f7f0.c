/* 0x49f7f0, execute callback 0x34: builds what 0x49f5c0 chose (0x4a5320), sets flag 0x1000000 of the
   character's field 0x1c8 and records the building class s1->unk10 */

int ai_Unk49f7f0(G5CH *p, AIARG *s1, AIARG *s2)
{
    if (ai_Unk4a5320((AIPER *)p, s1, s2) == 0)
        return 0;
    cm_SendSetField(p->netId, 0x1c8, 4, ((unsigned)&txt_Keys + 0x181c0), 0);
    ai_Unk4c59b0(p->netId, s1->unk10);
    gm_SendMoneyDelta(p->netId, 8);
    return 0x34;
}
