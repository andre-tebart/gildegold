/* 0x499f20, execute of 0x499c10 */

int ai_Unk499f20(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIPER *c;

    if (s1->kind != 7)
        return 0;
    c = AI_505DF0(s1->unk04);
    if (!c)
        return 0;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x1a;
    msg.to = p->netId;
    if (p->bld2)
        msg.fromId = p->bld2->id;
    else
        msg.fromId = -1;
    msg.flags = 1;
    msg.u.s.unk84 = s1->unk10;
    msg.ch = c->netId;
    msg.arg[0] = unk_Unk4f8700(c);
    ai_Unkf84cf4[0] = ai_Unkf84cd4[0] * s1->unk14;
    ai_Unkf84cf4[2] = ai_Unkf84cd4[2] * s1->unk14;
    ai_Unkf84cf4[4] = ai_Unkf84cd4[4] * s1->unk14;
    ai_Unkf84cf4[6] = ai_Unkf84cd4[6] * s1->unk14;
    cm_BeginGroup("pruegel");
    cm_SendLongMessage(&msg);
    cm_EndGroup("pruegel");
    return 0x18;
}
