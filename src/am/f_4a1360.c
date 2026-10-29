/* 0x4a1360, execute callback 0x3a: message 0x5e to the child s1->unk04 (kind 7): the building class s2->unk04
   (kind 0x17) and the price s2->unk10 */

int ai_Unk4a1360(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;

    if (s1->kind == 7 && s2->kind == 0x17) {
        m.to = s1->unk04;
        m.unk36 = 0x12;
        *(int *)&m.unk58 = p->netId;
        m.type = 0x5e;
        m.unk0c = -1;
        *(unsigned char *)&m.from = (unsigned char)s2->unk04;
        m.unk60 = s2->unk10;
        cm_SendLongMessage((AIMSG *)&m);
        return 0x3a;
    }
    return 0;
}
