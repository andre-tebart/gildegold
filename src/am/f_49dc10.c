/* 0x49dc10, execute callback 0x2e: upgrades ("upgr_arbeiter_unterk", s2->kind 4) or builds
   ("bau_arbeiter_unterk") the workers' lodging, paid to 0x504a80(1) */

int ai_Unk49dc10(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *c;

    c = (unsigned char *)gm_PlayerSeat(1);
    if (c == 0 || ((unsigned char *)p)[0x166] != 0xf)
        return 0;
    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.type = 0x1e;
        m.to = p->netId;
        m.unk0c = s2->unk04;
        m.unk36 = 1;
        m.time = ai_TimeT;
        *(int *)&m.unk58 = s2->unk04;
        cm_BeginGroup("upgr_arbeiter_unterk");
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("upgr_arbeiter_unterk");
        return 0x2e;
    }
    cm_BeginGroup("bau_arbeiter_unterk");
    if (ai_Unk4a5320((AIPER *)ai_PlayerChar, s1, s2) != 0) {
        cm_SendMoveGoods(-1, *(int *)(c + 2), ai_OwnPlayer, gm_BuildingKindValue(s1->unk04));
        cm_EndGroup("bau_arbeiter_unterk");
        return 0x2e;
    }
    cm_EndGroup("bau_arbeiter_unterk");
    return 0;
}
