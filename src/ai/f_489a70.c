/* 0x489a70, a master hands a weapon out of a slot to a character of his building */

#define SELL_FMT "cm_RequestSellObjekt(%i, %i, %i, %i, %i, %i)"

void ai_MeisterBewaffnen(AICHAR *ch, AIOBJ *b)
{
    AICHAR *c;
    AISLOT *sel, *q;
    unsigned short *p;
    int *t;
    int i, j, found;
    char buf[1024];

    i = 0;
    do {
        c = &ai_CharsT[i];
        if (c->id != 0xffff && c->bld == ch->bld && c->title && c->model && c->model->bldId == c->bld->id) {
            sel = NULL;
            p = (unsigned short *)gm_Query(c->inventory, 1, 4, 12);
            if (!p)
                return;
            found = 0;
            q = &ai_Slots[1];
            t = &ai_WeaponList[1];
            while (!found) {
                if (!t[-1] || !t[0])
                    break;
                t[-1] = *p;
                if (t[-1] != 0) {
                    j = 0;
                    while (!found) {
                        if (j >= ai_NSlots)
                            break;
                        if (ai_Slots[j].item == t[0]) {
                            sel = q;
                            found = 1;
                        }
                        j++;
                    }
                }
                t++;
                q++;
            }
            if (!found) {
                q = &ai_Slots[1];
                t = &ai_WeaponList2[1];
                while (!found) {
                    if (!t[-1] || !t[0])
                        break;
                    t[-1] = *p;
                    if (t[-1] != 0) {
                        j = 0;
                            while (!found) {
                            if (j >= ai_NSlots)
                                break;
                            if (ai_Slots[j].item == t[0]) {
                                sel = q;
                                found = 1;
                            }
                            j++;
                            }
                    }
                    t++;
                    q++;
                }
            }
            if (sel && sel->amount && ai_Unk4cd9d0(sel->item) && ai_Unk5bb2d0(ch, sel->item)) {
                sprintf(buf, "Meister %s bewaffnet %s mit %s", ch->name, c->name, ai_GoodsT[sel->item].name);
                sprintf(buf, SELL_FMT, c->netId, b->objId, sel->item, 1, ai_OwnPlayer, 0);
                cm_SendExSellObjectI(c->netId, b->objId, sel->item, 1, ai_OwnPlayer, 0);
                sel->amount--;
            }
        }
        i++;
    } while (i < 768);
}
