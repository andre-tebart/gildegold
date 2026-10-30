/* 0x487ea0 */

void ai_Unk487ea0(AICHAR *ch)
{
    AIBLD *bld;
    unsigned int fl;
    int i, n;

    if (ai_TimeT.hour == 7 || ai_TimeT.hour == 0xc || ai_TimeT.hour == 0x12) {
        fl = ch->flags1b4;
        if (!(fl & 0x40)) {
            bld = ch->bld;
            fl |= 0x40;
            ch->flags1b4 = fl;
            if ((ai_CharsT[bld->owner].kind != 6 && ai_CharsT[bld->owner].kind != 7) || (fl & 2)) {
                n = 0;
                for (i = 0; i < 768; i++) {
                    if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].bld == bld && ai_CharsT[i].title != 0)
                        n++;
                }
                if (n < ai_BTypesT[bld->type].k231 + ai_BTypesT[bld->type].k232) {
                    if (n == 0 || (!(ch->flags1c8 & 8) && util_RandomU(0x48) >= (0x12 - n) * 2))
                        gv_SendGrave(ch->bld);
                }
            }
        }
    } else {
        ch->flags1b4 &= ~0x40;
    }
}
