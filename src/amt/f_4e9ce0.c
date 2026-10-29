/* 0x4e9ce0, executes the office-power request m of the mayor c, picked by the office c[0x169] (the offices 0x1e .. 0x21):
   0x1e (m = {1, target}): the target (a character with an office, not c) is denounced - its office goes out as the office
   message 4 and its opinion of c drops; 0x1f / 0x21 (m = {4, district 1 .. 6}): every pair of the district's office owners
   loses the damage roll of c's skill, the opinion of both ways; 0x20: the same one-sided (only the office owners' opinion of c).
   Every power takes 8 money of c (cm_SendMoneyDelta), sets the power bit 0x10000000 of +0x1c8 and tells the news (texts
   0x166e .. 0x1685, the channels 0x5fb / 0x5fd / 0x5fe) to the other characters and, when the kind of c is 6, to c itself.
   Returns -1 when the damage roll is <= 0.3 and 1 otherwise, 0 when the message does not fit the office kind.
   @deep B3: 517 differing asm lines, ours 2048 vs the original 2148 bytes (session start 726 / 1968). The whole original 0x4e9ce0..0x4ea543 was read against the source (tools/disasm.py, ghidra/4e9ce0.c) and the statement sequence now matches: the call groups (RandomF, gm_FindChar, amt_FindOffice, SendOfficeMessage, MoneyDelta, SetField, txt_Format, he_ShowNews, MessageBox, GetColumnOffices, GetSkill, __ftol, SendRelations), the m[0]/m[1] guards, the found-in-list news loop, the pair loop, the ternary text 0x166e/0x166f and the shared he_ShowNews tails. Meaning fixes this session: the damage roll is (util_RandomF() + 1.0f) * gm_GetSkill(c, 4) * 15.0f then (int)((f + 5.0) * 2.55f) (ours was (int)(skill * random); the fp sequence now matches), the character id is an unsigned short (AMTCHARVIEW.id zero-extends where *(short *)t signed), cid dropped for AMT_NETID(c) per use (the original reloads [esi+4]; 678 -> 517 lines), the district guard as (unsigned int)m[1] >= 1 && <= 6 (the original's cmp 1/jb + cmp 6/ja), AMT_CHARS_END, the AMTCHARVIEW field views (Q4) and the rf-arm polarity if (rf > 0.3f) (the original's fall-through else blocks; 581 -> 517). Left: allocation only. (1) Prologue: ours emits call util_RandomF before push ebx/ebp/esi/edi (VC6 shrink-wraps), the original pushes the four registers first. (2) Register roles: original c=esi, t=ebx, ret=ebp; ours c=ebp, t=esi, ret spilled to [esp+0x18] (spell 4e9ce6 / 4ea2c1 reload hints). (3) Slots: spell 4e9ffb slots reports our AMTKIND k at [esp+0x44] and the AMTREC list[6] at [esp+0x5c] where the original has them swapped (its o, list, k are contiguous in declaration order); the declaration order and the scope of k were tried both ways with no code change, so only slotmap/c2prio can move them. (4) spell dup-tails at 4e9e26 (13), 4e9e94 (7), 4ea041, 4ea117, 4ea305, 4ea4a3: the both-arms / pull-after-branch / polarity spellings were tried (the kept polarity flip is the gain above); fill at 4ea4dd. Q1-Q7 pass and the frame now matches (0x8dc), no statement is missing. */

/* the text msg to every character of the kind 6 / 7 except the ones with the ids a and b (0x53dda0, channel chan) */
/* unlinked */
static __inline void amt_Tell(int a, int b, char *msg, int chan)
{
    int *q;

    q = ai_CharInts[0];
    do {
        if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != a && *q != b)
            he_ShowNews(*q, -1, msg, 0, chan, ch_EmptyName);
        q += 0x87;
    } while ((int)q < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
}

int amt_ExecuteMayorPowerMessage(char *c, int *m)
{
    float rf, f;
    int ret, n, dmg, i, j, id;
    char kind;
    char *t;
    AMTREC o;
    AMTREC list[6];
    AMTKIND k;
    char msg[1024], msg2[1024];

    rf = util_RandomF();
    ret = rf <= 0.3f ? -1 : 1;
    kind = c[0x169 - 0];
    if (kind == 0x1e) {
        if (m[0] == 1 && (t = gm_FindChar(m[1])) != 0 && t[0x166] != 0 && t != (char *)c && amt_FindOffice(t, &k, &o) != 0) {
            if (rf > 0.3f)
                amt_SendOfficeMessage((char)o.idx, 0, 0, 4, 0xff);
            cm_SendMoneyDelta(AMT_NETID(c), -8);
            cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x10000000, 0);
            kind = t[2];
            if (rf > 0.3f) {
                if (kind == 7 || kind == 6) {
                    txt_Format(msg, 0x1684);
                    he_ShowNews(AMT_NETID(t), -1, msg, 0, 0x5fb, ch_EmptyName);
                }
                txt_Format(msg, 0x1682, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                amt_Tell(AMT_NETID(t), AMT_NETID(c), msg, 0x5fe);
                if (c[2] == 6) {
                    txt_Format(msg, 0x1680, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                    sv_MessageBox(msg, 0);
                }
            } else {
                if (kind == 7 || kind == 6) {
                    txt_Format(msg, 0x1685);
                    he_ShowNews(AMT_NETID(t), -1, msg, 0, 0x5fb, ch_EmptyName);
                }
                txt_Format(msg, 0x1683, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                amt_Tell(AMT_NETID(t), AMT_NETID(c), msg, 0x5fe);
                if (c[2] == 6) {
                    txt_Format(msg, 0x1681, ((AMTCHARVIEW *)t)->id, ((AMTCHARVIEW *)t)->id);
                    sv_MessageBox(msg, 0);
                }
            }
            return ret;
        }
    } else if (kind == 0x1f || kind == 0x21) {
        if (m[0] == 4 && (unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 6) {
            n = amt_GetColumnOffices(m[1], 6, list);
            if (rf > 0.3f) {
                f = gm_GetSkill((char *)c, 4);
                f = (util_RandomF() + 1.0f) * f * 15.0f;
                dmg = (int)((f + 5.0) * 2.55f);
                if (n != 1) {
                    for (i = 0; i < n - 1; i++) {
                        id = list[i].owner;
                        if (id != AMT_NETID(c) && id != -1) {
                            for (j = i + 1; j < n; j++) {
                                if (list[j].owner != AMT_NETID(c) && list[j].owner != -1) {
                                    cm_SendRelations(list[j].owner, id, -dmg, 1);
                                    cm_SendRelations(id, list[j].owner, -dmg, 1);
                                }
                            }
                        }
                    }
                }
            }
            cm_SendMoneyDelta(AMT_NETID(c), -8);
            cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x10000000, 0);
            if (rf > 0.3f) {
                txt_Format(msg, 0x1672, m[1] + 0x1663);
                txt_Format(msg2, 0x1670, m[1] + 0x1663);
            } else {
                txt_Format(msg, 0x1673, m[1] + 0x1663);
                txt_Format(msg2, 0x1671, m[1] + 0x1663);
            }
            {
                int *q;

                q = ai_CharInts[0];
                do {
                    if ((((char *)q)[-2] == 7 || ((char *)q)[-2] == 6) && *q != AMT_NETID(c)) {
                        for (i = 0; i < n; i++) {
                            if (list[i].owner == *q)
                                break;
                        }
                        if (i < n)
                            he_ShowNews(*q, -1, msg, 0, 0x5fb, ch_EmptyName);
                        else
                            he_ShowNews(*q, -1, msg2, 0, 0x5fe, ch_EmptyName);
                    }
                    q += 0x87;
                } while ((int)q < (int)AMT_CHARS_END);
            }
            if (c[2] == 6) {
                txt_Format(msg, rf <= 0.3f ? 0x166f : 0x166e, m[1] + 0x1663);
                sv_MessageBox(msg, 0);
            }
            return ret;
        }
    } else if (kind == 0x20 && m[0] == 4 && (unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 6) {
        if (rf > 0.3f) {
            n = amt_GetColumnOffices(m[1], 6, list);
            f = gm_GetSkill((char *)c, 4);
            f = (util_RandomF() + 1.0f) * f * 15.0f;
            dmg = (int)((f + 5.0) * 2.55f);
            for (i = 0; i < n; i++) {
                id = list[i].owner;
                if (id != AMT_NETID(c) && id != -1)
                    cm_SendRelations(AMT_NETID(c), id, dmg, 1);
            }
        }
        cm_SendMoneyDelta(AMT_NETID(c), -8);
        cm_SendSetField(AMT_NETID(c), 0x1c8, 4, 0x10000000, 0);
        if (rf > 0.3f) {
            txt_Format(msg, 0x167a, m[1] + 0x1663);
            amt_Tell(AMT_NETID(c), -2, msg, 0x5fd);
            if (c[2] == 6) {
                txt_Format(msg, 0x1678, m[1] + 0x1663);
                sv_MessageBox(msg, 0);
            }
        } else {
            txt_Format(msg, 0x167b, m[1] + 0x1663);
            amt_Tell(AMT_NETID(c), -2, msg, 0x5fe);
            if (c[2] == 6) {
                txt_Format(msg, 0x1679, m[1] + 0x1663);
                sv_MessageBox(msg, 0);
            }
        }
        return ret;
    }
    return 0;
}
