/* 0x4af6c0, text token for one of the five richest characters (0x50a640) passing a predicate: fmt "...-n"
   0 = 0x4adff0, 2 = 0x4ae0d0, else 0x4ae080
   @deep behaviour: far draft, 173 code hunks (the RND_REICH token, callback 0x601b10[11] = 0x4af6c0, name 0x601730[11]): the reading matches (the top-5 worth lists of the unit note) but the frame/slot layout, the loop form and the c = pick return-tail fold remain - the same three issues as ai_TokenRandomGuildMaster's @deep reason; the -10000000 list start values are HST_NO_WORTH now (Q3) */

int ai_TokenRandomRich(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int score[5];
    int found;
    unsigned short ids[5];
    int (*pred)(unsigned short);
    int i, j, v, t;
    unsigned short u;
    AICHAR *c;

    score[0] = HST_NO_WORTH;
    score[1] = HST_NO_WORTH;
    score[2] = HST_NO_WORTH;
    score[3] = HST_NO_WORTH;
    score[4] = HST_NO_WORTH;
    found = 0;
    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    ids[4] = 0xffff;
    if (mode == 2)
        return 0;
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    pred = ai_IsLivingNonPlayer;
    if (n2 == 1 && (v = atoi(fmt + n1)) >= 0 && v < 3) {
        if (v == 0)
            pred = ai_IsLivingCharacter;
        else if (v != 1)
            pred = ai_IsLivingHuman;
    }
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        for (i = 0; i < 0x300; i++) {
            if (pred((unsigned short)i) && (v = gm_GetWorth((unsigned short)i)) > score[4]
                && ai_NotInLists(ai_Chars[i].netId)) {
                score[4] = v;
                found |= 1;
                ids[4] = ai_Chars[i].id;
                for (j = 3; j >= 0; j--) {
                    t = score[j];
                    if (t < score[j + 1]) {
                        u = ids[j];
                        score[j] = score[j + 1];
                        score[j + 1] = t;
                        ids[j] = ids[j + 1];
                        ids[j + 1] = u;
                    }
                }
            }
        }
        if (found == 0)
            return 0;
        i = (unsigned short)util_Random(5);
        c = &ai_Chars[ids[i]];
        if (mode == 0)
            args[k].id = ai_Chars[ids[i]].netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
