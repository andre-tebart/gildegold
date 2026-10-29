/* 0x4ae600, the RND_AMTSTRAEGERIN token of a history text (callback 0x601b10[3], name 0x601730[3]): a character
   with byte 9 == 1, an ordinary title (G12_TITLED) and in none of the lists of 0x4adfb0; the selection as in
   ai_TokenRandomGuildMaster
   @deep behaviour: far draft, 73 code hunks left (was 100; meaning fixes of this session applied: n2 no longer initialised to 0x200, so VC6 puts it in the dead fmt parameter slot like the original, AI_ZERO((char *)n1, 0x200), and the sel 0 / sel 1 search loops as 'for (n = 0x300; n != 0; n--) { if (found) break; i = (i + 1) % 0x300; } if (n == 0) return 0; pick = &ai_Chars[i];' - the same fixes that took ai_TokenRandomGuildMaster from 175 to 65 code hunks). Remaining: the same three issues as that function's @deep reason - (1) frame 0x20c vs 0x208: our AI_ZERO _zp temp gets its own slot (-0xc) and is read for edi where the original reads n1's slot (-0x4) (tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..)); (2) the register/layout work of the three selection loops and the tail after 'c = pick; if (c == 0) return 0;' (the folded return tail); (3) the G12_B/G12_TITLED macro expansions vs the original's order. Q1 name has evidence (callback 0x601b10[3] = 0x4ae600, name 0x601730[3] RND_AMTSTRAEGERIN), description written */

#define G12_B(i, o) (((unsigned char *)&ai_Chars[i])[o])
#define G12_TITLED(i) (ai_Chars[i].officeKind != 0 && ai_Chars[i].officeKind != 0x0f && ai_Chars[i].officeKind != 0x1a \
                       && ai_Chars[i].officeKind != 0x15)

int ai_TokenRandomOfficeHolderFemale(int mode, G12ARG *args, int k, char *fmt, char *out)
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
            i = util_Random(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 1 && ai_IsLivingCharacter((unsigned short)i) && ai_NotInLists(ai_Chars[i].netId)
                    && G12_TITLED(i))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_Chars[i];
        } else if (sel == 1) {
            i = util_Random(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 1 && ai_IsLivingNonPlayer((unsigned short)i) && ai_NotInLists(ai_Chars[i].netId)
                    && G12_TITLED(i))
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
                if (G12_B(i, 9) == 1) {
                    if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                        if (ai_NotInLists(ai_Chars[i].netId))
                            pick = &ai_Chars[i];
                    } else if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_Chars[i].netId)
                               && G12_TITLED(i)) {
                        pick = &ai_Chars[i];
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
