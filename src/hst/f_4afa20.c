/* 0x4afa20, one of the five richest characters (0x50a640) passing 0x4adff0 whose byte 0x164 is
   19..69 but not 31..33, 40..45, 52..57
   @deep behaviour: far draft, 82 code hunks (the RND_HANDELSHERR token, callback 0x601b10[13] = 0x4afa20, name 0x601730[13]): the selection reads (richness of the merchants) but the frame/slot layout and the folded return tails remain as in ai_TokenRandomGuildMaster's @deep reason; the -10000000 start values are HST_NO_WORTH now (Q3) */

int ai_TokenRandomMerchant(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int score[5];
    int found;
    unsigned short ids[5];
    unsigned char g;
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
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        for (i = 0; i < 0x300; i++) {
            g = G12_B(i, 0x164);
            if (g > 18 && g < 70 && (g < 31 || g > 33) && (g < 40 || g > 45) && (g < 52 || g > 57)
                && ai_IsLivingCharacter((unsigned short)i) && (v = gm_GetWorth((unsigned short)i)) > score[4]
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
