/* 0x499a90, execute of 0x499660 */

int ai_Unk499a90(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIBLD *bl;

    if (s1->kind != 4)
        return 0;
    bl = gm_FindBuildingById(s1->unk04);
    if (!bl)
        return 0;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x19;
    msg.to = p->netId;
    if (p->bld2)
        msg.fromId = p->bld2->id;
    else
        msg.fromId = -1;
    msg.flags = 1;
    msg.arg[0] = bl->id;
    msg.arg[9] = s1->unk10;
    cm_BeginGroup("sabotage");
    cm_SendLongMessage(&msg);
    cm_EndGroup("sabotage");
    ai_Unkf84c60[0] = ai_Unkf84c40[0] * s1->unk14;
    ai_Unkf84c60[2] = ai_Unkf84c40[2] * s1->unk14;
    ai_Unkf84c60[4] = ai_Unkf84c40[4] * s1->unk14;
    ai_Unkf84c60[6] = ai_Unkf84c40[6] * s1->unk14;
    return 0x17;
}
