/* 0x4af8d0, text token for the first character in the lists of 0x4adfb0 who passes 0x4ae120; fmt "...-n" as in
   0x4aef10 (sex filter)
   @deep behaviour: far draft, 180 code hunks, 22 slot hunks (the RND_NPC_EINWOHNER token, callback 0x601b10[12] = 0x4af8d0, name 0x601730[12]): Q2 fixed this session - ai_IsLivingInhabitant is ai_IsLivingInhabitant now (its only caller is this token, so kind 0 is the plain inhabitant kind); left: the frame/slot layout, the loop form and the folded return tails as in ai_TokenRandomGuildMaster's @deep reason */

int ai_TokenRandomNpcInhabitant(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel, i;
    unsigned int sex = 0;
    unsigned int filt;
    unsigned int cnt = 0x200;
    AICHAR *c = 0;

    n1 = (int)buf;
    AI_ZERO((char *)n1, cnt);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 == 1) {
        sel = atoi(fmt + n1);
        if (sel >= 0 && sel < 3) {
            if (sel != 0)
                sex = sel != 1;
            filt = sel != 0;
            if (mode == 1) {
                c = gm_FindChar(args[k].id);
                if (c != 0 && c->health != 0) {
                    txt_FormatString(out, buf, c->id);
                    return 1;
                }
            } else {
                for (i = 0; i < 0x300; i++) {
                    if (ai_NotInLists(ai_Chars[i].netId)) {
                        c = &ai_Chars[i];
                        if (filt == 0 || G12_B(i, 9) == sex) {
                            if (ai_IsLivingInhabitant((unsigned short)i))
                                break;
                            c = 0;
                        }
                    }
                }
                if (c != 0) {
                    if (mode == 0)
                        args[k].id = c->netId;
                    txt_FormatString(out, buf, c->id);
                    return 1;
                }
            }
        }
    }
    return 0;
}
