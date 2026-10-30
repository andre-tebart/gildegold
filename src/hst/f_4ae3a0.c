/* 0x4ae3a0, the RND_GILDENMEISTER token of a history text (callback 0x601b10[2], name 0x601730[2]): a random
   character holding a guild master office (secondOffice 0x1e..0x21); fmt "...-n" selects 0 = anybody living,
   1 = no human player (default), 2 = a computer one, else a human one; mode 1 checks args[k].id, mode 0 stores
   the pick
   @deep behaviour: 65 code hunks left (was 175). Meaning fixes applied this session (all kept): c->health <= 0 in the mode 1 arm (the original jbe, not je), secondOffice >= 0x1e && <= 0x21 in all four predicates (the original compares 0x1e / 0x21, not > 0x1d / < 0x22), the sel 0 / sel 1 search loops written as 'for (n = 0x300; n != 0; n--) { if (found) break; i = (i + 1) % 0x300; } if (n == 0) return 0; pick = &ai_Chars[i];' (that gives the original's bottom test 'dec esi; test esi,esi; jne' and the inline return 0), and n2 no longer initialised to 0x200 so VC6 puts it in the dead fmt parameter slot (ai_SplitTokenFormat(fmt, buf, &n1, &n2) pushes lea ecx,[ebp+0x14] like the original) with AI_ZERO((char *)n1, 0x200) giving the 0x200 store to [_zn]. Remaining: (1) frame 0x20c vs 0x208 - VC6 materialises the AI_ZERO _zp temp in its own slot (-0xc) and reads it for edi, the original reads n1's slot directly (mov edi,[ebp-0x4]); tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..) - all give the temp slot, so the dst form/coalescing is a slot-allocation decision; (2) the sel == 2 branch: ours pick in esi / n in edi, the original pick in edi / n in esi (30 regs hunks); (3) the tail after 'c = pick; if (c == 0) return 0;' - ours folds it to an earlier return-0 block (lea esi; test esi,esi; mov eax,esi) where the original keeps lea eax; test eax,eax / jne / mov eax,edi; test eax,eax (the same folded-return-tail effect as ai_TokenMayor's @deep reason). Q1 name has evidence (callback 0x601b10[2] = 0x4ae3a0, name 0x601730[2] RND_GILDENMEISTER), description written, Q2 clean */

int ai_TokenRandomGuildMaster(int mode, G12ARG *args, int k, char *fmt, char *out)
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
        if (c == 0 || c->health <= 0 || c->secondOffice == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingCharacter((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21)
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21)
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
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21) {
                    pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                           && ai_CharsT[i].secondOffice <= 0x21) {
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
