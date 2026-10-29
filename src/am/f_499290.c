/* 0x499290, execute of 0x498c70: pays a job, upgrades a building or runs 0x4a5320 */

int ai_Unk499290(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIJOBX *j;

    if (s1->kind == 4 && s2->kind == 8) {
        j = (AIJOBX *)he_FindEvent(1, 1, s2->unk04);
        if (!j)
            return 0;
        gm_Unk502fd0(s1->unk04, p->netId, j->unkb0, j, 1);
        return 0x13;
    }
    if (s2->kind == 4) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x1e;
        msg.to = p->netId;
        msg.fromId = s2->unk04;
        msg.flags = 1;
        msg.time = ai_Time;
        msg.arg[0] = s2->unk04;
        cm_BeginGroup("building_upgrade");
        cm_SendMoveGoods(-1, p->netId, ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
        cm_SendLongMessage(&msg);
        cm_EndGroup("building_upgrade");
        return 0x13;
    }
    if (ai_Unk4a5320(p, s1, s2)) {
        cm_SendSetField(p->netId, 0x1c8, 4, ((unsigned)&txt_Keys + 0x181c0), 0);
        return 0x13;
    }
    return 0;
}
