/* 0x49e680, execute callback 0x3c: upgrades ("upgr_kerker"), paid to 0x504a80(1) */

int ai_Unk49e680(G5CH *p, AIARG *s1, AIARG *s2)
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
    m.time = ai_Time;
    *(int *)&m.unk58 = s2->unk04;
    cm_BeginGroup("upgr_kerker");
    cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_kerker");
    return 0x3c;
}
