/* 0x4ae900, the same as 0x4ae600 for a woman (byte 9 == 0) @deep behaviour: far draft, 95 code hunks left (was more; this session's unit-wide fixes applied: n2 no longer initialised so it takes the dead fmt parameter slot, AI_ZERO((char *)n1, 0x200), the sel 0/1 loops as 'for (n = 0x300; n != 0; n--) { if (found) break; ... } if (n == 0) return 0; pick = &ai_Chars[i];' - the same idiom that took ai_TokenRandomGuildMaster 175 -> 65 hunks). Remaining: the frame 0x20c vs 0x208 (_zp temp materialised in its own slot instead of reading n1's -0x4; tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..)), the G12_B/G12_TITLED/ai_NotInLists evaluation order in the three predicates, and the c = pick return-tail fold - all as written in ai_TokenRandomGuildMaster's @deep reason. Q1 name has evidence (callback 0x601b10[4] = 0x4ae900, name 0x601730[4] RND_AMTSTRAEGER) */

int ai_TokenRandomOfficeHolder(int mode, G12ARG *args, int k, char *fmt, char *out)
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
        if (c == 0 || c->health == 0 || c->officeKind == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 0 && ai_IsLivingCharacter((unsigned short)i) && G12_TITLED(i)
                    && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)
                    && ai_NotInLists(ai_CharsT[i].netId))
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
                if (G12_B(i, 9) == 0) {
                    if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                        if (ai_NotInLists(ai_CharsT[i].netId))
                            pick = &ai_CharsT[i];
                    } else if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                               && G12_TITLED(i)) {
                        pick = &ai_CharsT[i];
                        break;
                    }
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
