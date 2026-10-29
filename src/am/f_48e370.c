/* 0x48e370, execute of 0x48e120: pays for the upgrade, with a message to the player */

int ai_Unk48e370(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    AIMSG msg;
    int cost;

    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.to = p->netId;
        TMX(msg) = NOWX;
        msg.type = 0x1e;
        msg.fromId = s2->unk04;
        msg.flags = 1;
        msg.car = s2->unk04;
        cm_BeginGroup("building_upgrade");
        cost = (int)(gm_BuildingKindValue(s2->unk04) * 0.3f);
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, cost);
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x40;
    }
    return ai_Unk4a5320(p, s1, s2) ? 0x40 : 0;
}
