/* 0x498a80, execute callback: upgrades a building (message "building_upgrade") */

int ai_Unk498a80(G5CH *p, AIARG *s1, AIARG *s2)
{
    G6MSG m;

    if (s2->kind == 4) {
        memset(&m, 0, sizeof(m));
        m.unk08 = p->netId;
        m.time = ai_Time;
        m.type = 0x1e;
        m.unk0c = s2->unk04;
        m.unk36 = 1;
        m.unk58 = s2->unk04;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
        cm_SendLongMessage((AIMSG *)&m);
        cm_EndGroup("building_upgrade");
        return 0x11;
    }
    return ai_Unk4a5320((AIPER *)p, s1, s2) ? 0x11 : 0;
}
