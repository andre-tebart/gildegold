/* 0x49e230, execute callback 0x31: upgrades the town hall ("upgr_rathaus"), paid to 0x504a80(1) */

int ai_Unk49e230(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0)
        return 0;
    if (((unsigned char *)p)[0x166] != 0xf)
        return 0;
    if (s2->kind != 4 && s1->unk10 != 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.to = p->netId;
    m.unk0c = s2->unk04;
    m.unk36 = 1;
    m.time = ai_TimeT;
    *(int *)&m.unk58 = s2->unk04;
    cm_BeginGroup("upgr_rathaus");
    cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_rathaus");
    return 0x31;
}
