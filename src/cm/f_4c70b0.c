/* 0x4c70b0, handler of type 0x11, "cm_ExSellObjekt": moves n of item from data[4] (seller) to data[0] (buyer) at
   the price data[0x13] each (money in the player good k) */

int ai_Unk4c70b0(G17MSG *m, G17STAT *s)
{
    char *r1, *y, *o2, *ys, *y2, *x2, *z, *x1, *o, *r2, *g;
    int *p, *q;
    unsigned int t;
    char buf[0x100];
    char text[0x7c];

    r1 = 0;
    y = 0;
    o2 = 0;
    ys = 0;
    y2 = 0;
    p = 0;
    x1 = 0;
    x2 = 0;
    z = 0;
    o = 0;
    r2 = 0;
    if (net_Conn.s != INVALID_SOCKET)
        ai_NextObjectId = *(int *)&m->data[0xa];
    if (*(int *)&m->data[4] != -1) {
        *(int *)&m->data[4] = ai_Unk4c4450((int)m, *(int *)&m->data[4]);
        if (gm_FindById(&x1, &y, &z, *(int *)&m->data[4]) == 0)
            return 1;
        if (x1 != 0) {
            p = (int *)(x1 + 0x5d);
            if (*(unsigned short *)(x1 + 0x27) != 0xffff)
                r1 = ai_Chars[*(unsigned short *)(x1 + 0x27)];
            else
                r1 = 0;
        } else if (y != 0) {
            if (*(short *)y == 0x2a || *(short *)y == 0x1db || *(short *)y == 0x1dc || *(short *)y == 0x116 || *(short *)y == 0x1dd)
                ys = y;
            p = (int *)(y + 0x14);
            r1 = gm_Unk50a050(y);
            if (r1 == 0)
                return 1;
        } else {
            r1 = z;
            p = (int *)(z + 0x178);
        }
        o = gm_Query(*p, 1, 0, *(unsigned short *)&m->data[8]);
        if (o == 0) {
            sprintf(buf, "cm_ExSellObjekt(): Object not found: ObjProt:%i Menge:%i CmdCnt:", *(unsigned short *)&m->data[8], *(int *)&m->data[0xf], m->seq);
            return 1;
        }
        if (ys != 0 && gm_ListAmount(ys, o) < *(int *)&m->data[0xf]) {
            sprintf(buf, "cm_ExSellObjekt() Not enough Objekts at source: ObjProt:%i Menge:%i CmdCnt:%i", *(unsigned short *)&m->data[8], *(int *)&m->data[0xf], m->seq);
            return 1;
        }
        if (*(int *)(o + 0xe) < *(int *)&m->data[0xf]) {
            sprintf(buf, "cm_ExSellObjekt() Not enough Objekts at source: ObjProt:%i Menge:%i CmdCnt:%i", *(unsigned short *)&m->data[8], *(int *)&m->data[0xf], m->seq);
            return 1;
        }
        if (*(int *)&m->data[0x13] != 0 && r1 != 0)
            gm_SumSlotAmounts(*(int *)(r1 + 0x178), m->data[0xe]);
    }
    if (*(int *)&m->data[0] != -1) {
        *(int *)&m->data[0] = ai_Unk4c4450((int)m, *(int *)&m->data[0]);
        if (gm_FindById(&x2, &y, &z, *(int *)&m->data[0]) == 0)
            return 1;
        if (x2 != 0) {
            q = (int *)(x2 + 0x5d);
            if (*(unsigned short *)(x2 + 0x27) != 0xffff)
                r2 = ai_Chars[*(unsigned short *)(x2 + 0x27)];
            else
                r2 = 0;
        } else if (y != 0) {
            if (*(short *)y == 0x2a || *(short *)y == 0x1db || *(short *)y == 0x1dc || *(short *)y == 0x116) {
                y2 = y;
                if (gm_FreeCapacity(y, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                    return 1;
            } else if (*(short *)y == 0x1dd) {
                if (gm_FreeCapacity(y, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]) < *(int *)&m->data[0xf])
                    return 1;
            }
            q = (int *)(y + 0x14);
            r2 = gm_Unk50a050(y);
        } else {
            q = (int *)(z + 0x178);
            r2 = z;
        }
        if (ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 0x1d)
            o2 = 0;
        else
            o2 = gm_Query(*q, 1, 0, *(unsigned short *)&m->data[8]);
        if (*(int *)&m->data[0x13] != 0 && r2 != 0) {
            if (gm_SumSlotAmounts(*(int *)(r2 + 0x178), m->data[0xe]) < *(int *)&m->data[0xf] * *(int *)&m->data[0x13])
                return 1;
        }
    }
    if (ys != 0) {
        if (r2 == 0)
            goto money;
        if (gm_Unk50a050(o) != r2 && *(int *)(o + 0x1c) != 0) {
            t = (unsigned int)(gm_ListStock(ys, *(unsigned short *)o) * *(int *)(o + 0x1c)) >> 2;
            if ((unsigned int)(gm_ListAmount(ys, o) - t) < (unsigned int)*(int *)&m->data[0xf])
                return 1;
        }
    }
    if (r2 != 0 && x2 == 0 && y2 == 0 && y == 0) {
        *(int *)&m->data[0xf] = gm_Unk50ad70(r2, *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]);
        if (*(int *)&m->data[0xf] == 0)
            return 1;
    }
money:
    if (*(int *)&m->data[0x13] != 0) {
        if (y2 == 0 && y != 0 && *(short *)y == 0x1dd) {
            g = gm_FindGoods(ai_OwnPlayer, *(unsigned short *)&m->data[8]);
            if (g != 0)
                *(int *)(g + 0x18) += *(int *)&m->data[0xf];
        }
        if (ys != 0 && *(short *)ys == 0x1dd) {
            g = gm_FindGoods(ai_OwnPlayer, *(unsigned short *)&m->data[8]);
            if (g != 0)
                *(int *)(g + 0x1c) += *(int *)&m->data[0xf];
        }
    }
    if (*(int *)&m->data[4] != -1) {
        *(int *)(o + 0xe) -= *(int *)&m->data[0xf];
        if (*(int *)(o + 0xe) <= 0) {
            if (ai_Goods[*(unsigned short *)&m->data[8] * 0x41] != 2 && ai_Goods[*(unsigned short *)&m->data[8] * 0x41] != 6) {
                gm_RemoveObject(p, *(unsigned short *)&m->data[8]);
            } else {
                if (x1 == 0)
                    return 1;
                gm_RemoveObjectFromBuilding(x1, *(unsigned short *)&m->data[8]);
            }
        }
        if (*(int *)&m->data[0x13] != 0) {
            gm_AddObjekt(*(int *)(r1 + 4), *(unsigned short *)(ai_PlayerSlots[m->data[0xe]] + 0x54), *(int *)&m->data[0xf] * *(int *)&m->data[0x13]);
            if (ys != 0 && *(short *)ys == 0x1db)
                *(int *)(r1 + 0x1a4) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else if (ys != 0 && *(short *)ys == 0x1dc)
                *(int *)(r1 + 0x1a0) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else
                *(int *)(r1 + 0x19c) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
        }
    }
    if (*(int *)&m->data[0] != -1) {
        if (o2 != 0) {
            *(int *)(o2 + 0xe) += *(int *)&m->data[0xf];
        } else if (ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 2 || ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 6) {
            if (x2 == 0)
                return 1;
            o2 = unk_Unk5031b0(x2, *(unsigned short *)&m->data[8]);
            if (o2 == 0)
                return 1;
        } else {
            o2 = gm_CreateObject(*(int *)&m->data[0], *(unsigned short *)&m->data[8], *(int *)&m->data[0xf]);
            if (o2 == 0)
                return 1;
            if (y2 != 0) {
                (*(int *)(o2 + 0xe))++;
                if ((ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 0x17 || ai_Goods[*(unsigned short *)&m->data[8] * 0x41] == 0x25)
                    && (*(short *)y2 == 0x2a || *(short *)y2 == 0x116) && r2 != 0 && (r2[2] == 6 || r2[2] == 7))
                    *(int *)(o2 + 0x1c) = 4;
            }
        }
        ai_Unk60bd90 = *(int *)(o2 + 2);
        if (*(int *)&m->data[0x13] != 0 && r2 != 0) {
            gm_TakeObjekt(*(int *)(r2 + 4), *(unsigned short *)(ai_PlayerSlots[m->data[0xe]] + 0x54), *(int *)&m->data[0xf] * *(int *)&m->data[0x13]);
            if (y2 != 0 && *(short *)y2 == 0x1db)
                *(int *)(r2 + 0x1a0) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else if (y2 != 0 && *(short *)y2 == 0x1dc)
                *(int *)(r2 + 0x1a4) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
            else
                *(int *)(r2 + 0x198) += *(int *)&m->data[0xf] * *(int *)&m->data[0x13];
        }
    }
    if (ys != 0 && (*(short *)ys == 0x2a || *(short *)ys == 0x116) && r1 != 0 && *(int *)&m->data[0x13] != 0
        && *(unsigned short *)r1 == ai_PlayerId) {
        x1 = gm_FindBuildingById(*(int *)(ys + 6));
        if (x1 != 0) {
            txt_Format(text, 0x7c, *(unsigned char *)x1 * 14 + 0x48e, *(int *)&m->data[0x13]);
            sv_SetStatusText(text);
            sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        }
    }
    if (s != 0) {
        s->state = 1;
        if (*(int *)&m->data[0] != -1) {
            s->unk01 = 3;
            s->value = (int)o2;
        }
    }
    return 0;
}
