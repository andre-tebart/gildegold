/* 0x4e9a00, executes the guild-house gift message m (kind 2, +4 the object id, +8 the goods amount) of the character c: when the object's
   owner (+0x27) is not c and not -1, c's gift (the skill 4 of c, times a random factor 1 .. 2, times 15, plus 5, times 2.55) costs every
   character at the object (the +0x16c pointer, the owner itself skipped) that many relations points (cm_SendRelations), the amount m[2]
   leaves c (cm_SendMoveGoodsUnchecked) and m[2] / count + 0x20 goes back to each of them; c pays 5 money and uses its power flag
   0x8000000, the owner gets the text 0x1663 as a quick jump, every player character the text 0x1662 (channel 0x5fd) and c the text 0x1661
   as a message box. Returns 1 when done, 0 for another message, a missing object or its own / no owner. */

int amt_ExecuteShareMessage(short *c, int *m)
{
    AMTBLDVIEW *o;
    int loss, cnt, i;
    unsigned int k;
    float f;
    int *q;
    char msg[1024];

    if (m[0] == 2) {
        gm_FindById((int *)&o, 0, 0, m[1]);
        if (o != 0) {
            if (o->owner != (unsigned short)*c && o->owner != 0xffff) {
                f = gm_GetSkill((char *)c, 4);
                f = (util_RandomF() + 1.0f) * f * 15.0f;
                loss = (int)((f + 5.0) * 2.55f);
                cm_SendMoveGoodsUnchecked(-1, AMT_NETID(c), ai_OwnPlayer, m[2]);
                k = 0;
                cnt = 0;
                q = (int *)((char *)ai_CharInts + 0x168);
                do {
                    if ((short)q[-0x5b] != -1 && *q == (int)o && o->owner != k) {
                        cm_SendRelations(ai_CharInts[o->owner][0], q[-0x5a], -loss, 1);
                        cnt++;
                    }
                    q += 0x87;
                    k++;
                } while ((int)q < (int)AMT_CHARS_END + 0x168);
                if (cnt != 0) {
                    loss = (int)((float)m[2] / cnt) + 0x20;
                    q = (int *)((char *)ai_CharInts + 0x168);
                    k = 0;
                    do {
                        if ((short)q[-0x5b] != -1 && *q == (int)o && o->owner != k)
                            cm_SendMoveGoodsUnchecked(q[-0x5a], -1, ai_OwnPlayer, loss);
                        q += 0x87;
                        k++;
                    } while ((int)q < (int)AMT_CHARS_END + 0x168);
                }
                cm_SendMoneyDelta(AMT_NETID(c), -5);
                cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x8000000, 0);
                if (ai_Chars[o->owner][2] == 7 || ai_Chars[o->owner][2] == 6) {
                    txt_Format(msg, 0x1663, o);
                    he_MessageBoxQuickjump(ai_CharInts[o->owner][0], -1, msg, 0, 0x5fb, o->id, -1, 0, ch_EmptyName);
                }
                txt_Format(msg, 0x1662, o->owner, o);
                q = ai_CharInts[0];
                do {
                    if (((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) {
                        if (AMT_NETID(c) != *q && *q != ai_CharInts[o->owner][0])
                            he_ShowNews(*q, -1, msg, 0, 0x5fd, ch_EmptyName);
                    }
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
                if (((char *)c)[2] == 6) {
                    txt_Format(msg, 0x1661, o->owner, o);
                    sv_MessageBox(msg, 0);
                }
                return 1;
            }
        }
    }
    return 0;
}
