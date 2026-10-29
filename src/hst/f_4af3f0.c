/* 0x4af3f0 @deep behaviour: far draft, 105 code hunks, 20 slot hunks (the RND_KIRCHENBERUF token, callback 0x601b10[10] = 0x4af3f0, name 0x601730[10]): the reading matches (a random character with a church profession) but the frame/slot layout, the loops and the folded return tails remain as in ai_TokenRandomGuildMaster's @deep reason */

/* 0x4af3f0, the same as 0x4aec00 for a character whose byte 0x164 is 13..18 */
#define G12_AGED(i) (G12_B(i, 0x164) > 12 && G12_B(i, 0x164) < 19)

int ai_TokenRandomChurchProfession(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_Random(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingCharacter((unsigned short)i) && G12_AGED(i) && ai_NotInLists(ai_Chars[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_Chars[i];
        } else if (sel == 1) {
            i = util_Random(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && G12_AGED(i) && ai_NotInLists(ai_Chars[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_Chars[i];
        } else {
            pick = 0;
            i = util_Random(0x300);
            for (n = 0x300; n != 0; n--) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_AGED(i)) {
                    if (ai_NotInLists(ai_Chars[i].netId))
                        pick = &ai_Chars[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && G12_AGED(i)
                           && ai_NotInLists(ai_Chars[i].netId)) {
                    pick = &ai_Chars[i];
                    break;
                }
                i = (i + 1) % 0x300;
            }
        }
        c = pick;
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
