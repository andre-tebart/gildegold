/* 0x4e9660, executes the guild-house price message m (kind 3, +4 the job number 1 .. 7, +8 = 1 / -1 raise / lower) of the character c:
   the best building of the district (the type's level +0x247) selects the row of amt_PriceRows (0x15 bytes: the building kind and 10 goods
   ids), gm_Shuffled picks one or two of those goods and their price records (gm_FindGoods) get the factor gm_GetSkill(c, 2) times a random
   1.2 .. 2.0 (raise) or 0.5 .. 1.0 (lower); the goods go back through cm_SendGoodsDelta, m[3] / m[4] return them, c uses its power flag
   0x4000000 and pays 3 money (cm_SendMoneyDelta), every player character except c is told the news text (0x1655 / 0x1659 + the goods
   names) and c gets it as a message box when it is a player character (kind 6). Always 0.
   @deep @deep B3 360 code / 4 regs / 2 slots hunks, 208 of 275 instructions differ, ours 896 vs 928 bytes: the whole original 0x4e9660..0x4e99ff was read against the source and every statement now matches (the m[0]==3 / |m[2]|==1 / district 1..7 guards, the best-building gm_BuildQuery walk with the type level at ai_BTypes[type*0x24d+0x247], the amt_PriceRows row scan (37 rows of 0x15, bound &amt_PriceRows[0x25*0x15]) and the 10 goods ids, gm_Shuffled picking one or two, the gm_GetSkill(c,2) factor with 1.2 + 0.4*s and 1.0 - (0.2 + 0.3*s) (the lower arm's 0.2 / 1.0 are doubles, kept), the two goods records copied/updated through AMTGOODS and cm_SendGoodsDelta, the power flag + cm_SendMoneyDelta(-3), the two txt_Format messages (texts 0x1655..0x165a, goods names id*2+0x9a8) and the player-character news loop over ai_CharInts up to 0x7d5f84). Meaning fixes applied: best int -> the signed lvl compare, the district guard as two unsigned compares, the typed AMTGOODS / AMTBLDVIEW / AMTCHARVIEW views (Q4 clean), amt_PriceRows / AMT_CHARS_END externs (Q3), 709 -> 581 differing bytes. Left: the frame (ours 0xab8 vs the original 0xad8 = 32 bytes of extra locals) and the register roles from instruction 0: the original keeps c in ebp (reloaded once, spell reload) and the zero constant in ebx, ours has the zero in edi/esi and c in a slot; the news loop tail is placed differently (spell dup-tail at 0x4e99cd: for / do-while / swapped polarity same bytes) and 4e96f0 width is unchanged by the unsigned char cast. Allocation / slotmap / c2prio work is all that is left; Q1-Q7 pass. */

int amt_ExecutePriceMessage(char *c, int *m)
{
    unsigned char lvl, best;
    AMTBLDVIEW *it;
    unsigned int id, id2, r;
    int i, n, t, pick[2], cands[10];
    unsigned short *row;
    float f, rf;
    AMTGOODS rec;
    AMTGOODS *p;
    int *q;
    char buf[512], msg[2048];
    unsigned char *tab;

    best = 0;
    id = 0;
    id2 = 0;
    m[3] = 0;
    m[4] = 0;
    if (m[0] == 3 && (m[2] == 1 || m[2] == -1) && (unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 7) {
        t = gm_JobBuildingType(m[1]);
        if (t != 0) {
            it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, t);
            if (it != 0) {
                do {
                    lvl = (unsigned char)ai_BTypes[it->type * 0x24d + 0x247];
                    if (best < lvl) {
                        id = it->type;
                        best = lvl;
                    }
                    it = (AMTBLDVIEW *)gm_BuildNext();
                } while (it != 0);
                if (id != 0) {
                    i = 0;
                    tab = amt_PriceRows;
                    while (*tab != id) {
                        tab += 0x15;
                        i++;
                        if (tab >= &amt_PriceRows[0x25 * 0x15])
                            return 0;
                    }
                    if (i < 0x25) {
                        n = 0;
                        row = (unsigned short *)&amt_PriceRows[1 + i * 0x15];
                        for (t = 0; t < 10; t++, row++) {
                            if (*row != 0) {
                                cands[n] = *row;
                                n++;
                            }
                        }
                        if (n != 0) {
                            gm_Shuffled(n, pick);
                            if (n == 1)
                                id = cands[pick[0]];
                            else {
                                id2 = cands[pick[1]];
                                id = cands[pick[0]];
                            }
                            f = gm_GetSkill(c, 2);
                            rf = util_RandomF();
                            if (m[2] == 1)
                                f = (rf + 1.0f) * f * 0.4f + 1.2f;
                            else
                                f = 1.0f - ((rf + 1.0f) * f * 0.3f + 0.2f);
                            p = (AMTGOODS *)gm_FindGoods(ai_OwnPlayer, id);
                            if (p != 0) {
                                rec = *p;
                                rec.mean = rec.amount * f;
                                rec.price = gm_GoodsValue(ai_OwnPlayer, id);
                                cm_SendGoodsDelta((char *)&rec, ai_OwnPlayer);
                                m[3] = id;
                                p = (AMTGOODS *)gm_FindGoods(ai_OwnPlayer, id2);
                                if (p != 0) {
                                    rec = *p;
                                    rec.mean = rec.amount * f;
                                    rec.price = gm_GoodsValue(ai_OwnPlayer, id2);
                                    cm_SendGoodsDelta((char *)&rec, ai_OwnPlayer);
                                    m[4] = id2;
                                }
                                cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x4000000, 0);
                                cm_SendMoneyDelta(AMT_NETID(c), -3);
                                r = util_RandomU(3) & 0xffff;
                                if (m[2] == 1) {
                                    txt_Format(buf, r + 0x1656, m[1] + 0x1648);
                                    t = 0x1655;
                                } else {
                                    txt_Format(buf, r + 0x165a, m[1] + 0x1648);
                                    t = 0x1659;
                                }
                                txt_Format(msg, t, buf, id * 2 + 0x9a8, id2 * 2 + 0x9a8);
                                for (q = ai_CharInts[0]; q < AMT_CHARS_END; q += 0x87) {
                                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && AMT_NETID(c) != *q)
                                        he_ShowNews(*q, -1, msg, 1, 0x5ec, ch_EmptyName);
                                }
                                if (AMT_KIND(c) == 6)
                                    sv_MessageBox(msg, 0x10);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
