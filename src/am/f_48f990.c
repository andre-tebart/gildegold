/* 0x48f990, execute of action 0x48 */

int ai_Unk48f990(AIPX *p, AIARG *s1, AIARG *s2)
{
    AIBLD *bl;
    AIOBJ *o, *q;
    AIGDX *g;
    AIUSE u;

    if (s1->kind == 2) {
        if (s1->unk10 == 1) {
            cm_SendExSellObjectI(p->netId, -1, s1->unk04, 1, ai_OwnPlayer, 0);
            cm_SendSetField(p->netId, 0x1cc, 4, 0x40000000, 0);
            return 0x48;
        }
        if (s1->unk10 == 2 && s2->kind == 4) {
            bl = AI_501190_5(2, 1, s2->unk0c, 5, 0x1e);
            if (bl) {
                o = gm_Query(bl->objList, 2, 6, 0, 0x2a);
                if (o) {
                    q = gm_Query(o->unk14, 2, 6, 0, s1->unk04);
                    if (q && q->unk0e > 1) {
                        cm_SendExSellObjectI(p->netId, o->objId, s1->unk04, 1, ai_OwnPlayer, 0);
                        return 0x48;
                    }
                }
            }
        }
    } else if (s1->kind == 1) {
        o = gm_Query(p->inventory, 1, 1, s1->unk04);
        if (!o)
            return 0;
        g = ai_Unk5ba620(o->id);
        if (g) {
            u.obj = o;
            u.unk04 = s2->unk04;
            u.unk08 = s2->unk08;
            u.unk0c = 0;
            u.unk10 = 0;
            if (ai_Unk5cf8a0(p, &u) == 1) {
                if (u.unk0c == 1) {
                    ai_Unkf868b4 = ai_Unkf86894 * g->unk0c;
                    return 0x1c;
                }
                if (u.unk0c == -1)
                    ai_Unkf868b4 = ai_Unkf86894 * g->unk0c * -0.5f;
                return 0x1c;
            }
        }
    }
    return 0;
}
