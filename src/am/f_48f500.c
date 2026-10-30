/* 0x48f500, execute of action 0x43 */

int ai_Unk48f500(AIPX *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    int v;

    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->unk04;
        msg.flags = 1;
        msg.time = ai_TimeT;
        msg.arg[0] = s2->unk04;
        cm_BeginGroup("building_upgrade");
        v = gm_BuildingKindValue(s1->unk04);
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(v * 0.3f));
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x43;
    }
    return ai_Unk4a5320(p, s1, s2) ? 0x43 : 0;
}
