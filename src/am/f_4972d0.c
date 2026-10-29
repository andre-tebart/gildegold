/* 0x4972d0, execute of 0x496e40: pays the companion, with a message to the player */

int ai_Unk4972d0(AIPER *p, AIARG *s1, AIARG *s2)
{
    char buf[0x400];
    AIPER *c;
    float k;

    if (s1->kind != 7)
        return 0;
    if (s2->kind != 0x12)
        return 0;
    c = AI_505DF0(s1->unk04);
    if (!c)
        return 0;
    cm_SendMoveGoods(c->netId, p->netId, ai_OwnPlayer, s2->unk08);
    if (p->flags1cc & 0x800)
        k = 6.0f;
    else
        k = 4.0f;
    cm_SendRelations(p->netId, c->netId, (int)((s2->unk10 + 1) * k), 1);
    cm_SendSetField(p->netId, 0x1e8, 4, 0x400, 0);
    if (c->kind == 6 || c->kind == 7) {
        txt_Format(buf, 0x1751, p->id, s2->unk08);
        he_ShowNews(c->netId, -1, buf, 0, 0x5fb, "_NACHRICHTEN_HS_22");
    }
    return 9;
}
