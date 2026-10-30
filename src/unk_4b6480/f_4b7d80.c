/* 0x4b7d80, message for c with secondOffice 0x1e..0x21: every character (not kind 6 / 7) of the same 0x504ad0 group gets
   0x4c4b00 1..8 from c; v = 1..4 % of 0x50a640, text 0x2a3 + secondOffice - 0x1e (not with item 0x290) */

int ai_Unk4b7d80(G14MSG *m)
{
    AICHAR *c;
    int g, i, s, v;
    double k;
    char buf[0x400];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->secondOffice >= 0x1e && c->secondOffice <= 0x21 && (g = gm_TitleGroup(c->jobId)) != 0
        && !ai_InventoryHas(c, 0x290)) {
        for (i = 0; i < 768; i++) {
            if (ai_CharsT[i].kind != 6 && ai_CharsT[i].kind != 7 && g == gm_TitleGroup(ai_CharsT[i].jobId))
                cm_SendRelations(c->netId, ai_CharsT[i].netId, util_RandomU(8) + 1, 1);
        }
        k = util_RandomU(4) + 1.0;
        v = (int)(gm_GetWorth(c->id) * k * 0.01);
        cm_SendMoveGoodsUnchecked(-1, m->id, ai_OwnPlayer, v);
        if (c->secondOffice == 0x1e)
            s = 0;
        else if (c->secondOffice == 0x1f)
            s = 1;
        else if (c->secondOffice == 0x20)
            s = 2;
        else if (c->secondOffice == 0x21)
            s = 3;
        else
            return 0x400;
        txt_Format(buf, m->type + 0x12e7, s + 0x2a3, v);
        he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
        return 0;
    }
    return 0x400;
}
