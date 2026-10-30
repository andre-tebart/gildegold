/* 0x4aec00, the same as 0x4ae600 for anybody (no sex test); mode 2 gives no token @deep behaviour: far draft, 77 code hunks left (unit-wide frame and search-loop fixes of this session applied, as in ai_TokenRandomGuildMaster's @deep reason). Remaining: the AI_ZERO _zp temp slot (frame 0x20c vs 0x208), the predicate evaluation order (G12_TITLED / ai_NotInLists / the living helpers) and the c = pick return-tail fold. Q1 name has evidence (callback 0x601b10[5] = 0x4aec00, name 0x601730[5] RND_AMTSPERSON) */

int ai_TokenRandomOfficePerson(int mode, G12ARG *args, int k, char *fmt, char *out)
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
    if (mode == 2)
        return 0;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingCharacter((unsigned short)i) && G12_TITLED(i) && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i) && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                    if (ai_NotInLists(ai_CharsT[i].netId))
                        pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && G12_TITLED(i)
                           && ai_NotInLists(ai_CharsT[i].netId)) {
                    pick = &ai_CharsT[i];
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
