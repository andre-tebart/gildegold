/* 0x49ea90, execute callback 0x2f: upgrades ("upgr_brunnen", s2->kind 4) or builds ("bau_brunnen"), paid to
   0x504a80(1); doubles need s1->unk10 (1..3) */

int ai_Unk49ea90(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;
    int n;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0 || ((unsigned char *)p)[0x166] != 0xa)
        return 0;
    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.unk0c = s2->unk04;
        m.unk36 = 1;
        m.time = ai_TimeT;
        *(int *)&m.unk58 = s2->unk04;
        cm_BeginGroup("upgr_brunnen");
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("upgr_brunnen");
    } else {
        cm_BeginGroup("bau_brunnen");
        if (ai_Unk4a5320((AIPER *)ai_PlayerChar, s1, s2) == 0) {
            cm_EndGroup("bau_brunnen");
            return 0;
        }
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, gm_BuildingKindValue(s1->unk04));
        cm_EndGroup("bau_brunnen");
    }
    n = s1->unk10;
    if (n > 0 && n < 4)
        G5_ACTS[0x2f].w3[n].w = G5_ACTS[0x2f].w1[0].w * 2.0f;
    return 0x2f;
}
