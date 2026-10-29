/* 0x49fb80, execute callback 0x35: sends the join / leave request of 0x49f850 (0x4c5d80) and sets flag 0x2000000
   of the character's field 0x1c8 ('noop' only sets the flag) */

int ai_Unk49fb80(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8TABLE r;

    if (s1->unk08 == 'join') {
        r.tag = s1->unk08;
        r.bld = s1->unk04;
        r.ch = p->netId;
        cm_BeginGroup("stammtisch join AI-Plr");
        ai_Unk4c5d80(&r);
        cm_SendSetField(p->netId, 0x1c8, 4, 0x2000000, 0);
        cm_EndGroup("stammtisch join AI-Plr");
        return 0x35;
    }
    if (s1->unk08 == 'leav') {
        r.tag = s1->unk08;
        r.bld = s1->unk04;
        r.ch = p->netId;
        cm_BeginGroup("stammtisch leave AI-Plr");
        ai_Unk4c5d80(&r);
        cm_SendSetField(p->netId, 0x1c8, 4, 0x2000000, 0);
        cm_EndGroup("stammtisch leave AI-Plr");
        return 0x35;
    }
    if (s1->unk08 == 'noop') {
        cm_SendSetField(p->netId, 0x1c8, 4, 0x2000000, 0);
        return 0x35;
    }
    return 0;
}
