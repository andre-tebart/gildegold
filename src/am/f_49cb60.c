/* 0x49cb60, execute callback 0x26: pays the rent (s2->kind 0x12, with a message) or takes over the rooms
   (s2->kind 0xf) of the building s1->unk04 */

int ai_Unk49cb60(G5CH *p, AIARG *s1, AIARG *s2)
{
    unsigned char *b;
    AIOBJ *o;
    int n, d;

    if (s1->kind != 4)
        return 0;
    b = (unsigned char *)gm_BuildQuery(1, 1, s1->unk04);
    if (b == 0 || *(unsigned short *)(b + 0x27) == 0xffff)
        return 0;
    o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0xf2);
    if (s2->kind == 0x12) {
        n = s1->unk10;
        d = (int)(n / log10(n));
        cm_BeginFieldBlock((AIPER *)p, p->netId);
        cm_AddFieldValues(4, 1, 0x1ac, &s1->unk10);
        cm_SendFieldDeltas();
        G8_4C4830(-1, p->netId, ai_OwnPlayer, s1->unk10);
        G8_4C4830(ai_Chars[*(unsigned short *)(b + 0x27)].netId, -1, ai_OwnPlayer, d);
        unk_Unk4f3a30(p->netId, (int)s1->unk14);
        ai_Unk5a8b80(p, &ai_Chars[*(unsigned short *)(b + 0x27)], b, d, s1->unk10);
        G5_ACTS[0x26].w3[0].w = G5_ACTS[0x26].w1[0].w * s1->unk14;
        G5_ACTS[0x26].w3[1].w = G5_ACTS[0x26].w1[1].w * s1->unk14;
        G5_ACTS[0x26].w3[2].w = G5_ACTS[0x26].w1[2].w * s1->unk14;
        return 0x26;
    }
    if (s2->kind == 0xf && o != 0) {
        unk_Unk4f3a30(p->netId, (int)s1->unk14);
        gm_SendMoneyDelta(p->netId, -s1->unk10);
        gm_SendMoneyDelta(ai_Chars[*(unsigned short *)(b + 0x27)].netId, 1);
        ai_Unk5a8c00(p, &ai_Chars[*(unsigned short *)(b + 0x27)], b);
        return 0x26;
    }
    return 0;
}
