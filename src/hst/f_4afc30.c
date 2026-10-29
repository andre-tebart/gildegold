/* 0x4afc30, text token for a random character passing 0x4ae0d0 / 0x4adfb0, searched with a random step
   (3, 5, 7 or 11)
   @deep behaviour: far draft, 70 code hunks (the RND_SPIELER token, callback 0x601b10[14] = 0x4afc30, name 0x601730[14]): the reading matches (a random living player 6/7) but the frame/slot layout, the loop form and the folded return tails remain as in ai_TokenRandomGuildMaster's @deep reason */

int ai_TokenRandomPlayer(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int steps[4];
    int step, i, n;
    AICHAR *c, *pick = 0;

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
        steps[0] = 3;
        steps[1] = 5;
        steps[2] = 7;
        steps[3] = 11;
        do {
            step = steps[(unsigned short)util_Random(4)];
            i = (unsigned short)util_Random(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_Chars[i].netId)) {
                    pick = &ai_Chars[i];
                    break;
                }
                i = (step + i) % 0x300;
            }
        } while (i + 1 < 0x300);
        if (pick == 0)
            return 0;
        c = pick;
        if (mode == 0)
            args[k].id = pick->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
