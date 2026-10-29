/* 0x4af1b0, text token (mode 2 only): fmt "...-n" gives n percent of the wealth (0x50a640) of the character
   args[k], as a money text (0x5085b0)
   @deep behaviour: 49 code hunks. Read: the guard (mode == 2, args != 0, k != 8, gm_FindChar, ai_IsLivingCharacter) matches the original branch for branch, and so does v = gm_GetWorth(c->id). The differences are (1) the frame 0x204 vs 0x200 - ours spills c to [ebp-4], the original keeps it in eax; (2) the ai_SplitTokenFormat prologue slots: the original keeps n1 = 0x200 in the mode parameter slot [ebp+8], n2 = (int)buf in the args slot [ebp+0xc], v in the k slot [ebp+0x10], and AI_ZERO's _zp reads the n2 slot directly - ours allocates [_zn]/[ebp-4] and writes 0x200 twice; (3) the money tail: the original does fild n1 / fmul 0.01f (0x5eb7f0) / fimul v with al = ai_OwnPlayer, mov cl,al, then pushes 3 dwords (ecx = zero-extended ai_OwnPlayer, eax = the atoi result, then the __ftol (0x5de518) result) before gm_ConvertMoney with 'add esp,8' - a 3-push/2-pop shape (the leaked dword is dropped by mov esp,ebp) that a plain 2-arg gm_ConvertMoney((int)(n1*0.01f*v), ai_OwnPlayer) does not produce; the source form that yields it is unknown (the argument evaluation order and the __ftol call site). Q2 fixed this session: gm_ConvertMoney/gm_ConvertMoney -> gm_ConvertMoney (project-wide, 60 references, no byte change); Q1 name has evidence (callback 0x601b10[8] = 0x4af1b0, name 0x601730[8] GELD) */

int ai_TokenMoney(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int v;
    AICHAR *c;

    if (mode == 2 && args != 0 && k != 8 && (c = gm_FindChar(args[k].id)) != 0
        && ai_IsLivingCharacter(c->id)) {
        v = gm_GetWorth(c->id);
        n1 = 0x200;
        n2 = (int)buf;
        AI_ZERO(buf, n1);
        ai_SplitTokenFormat(fmt, buf, &n1, &n2);
        n1 = atoi(fmt + n1);
        txt_FormatString(out, buf, gm_ConvertMoney((int)(n1 * 0.01f * v), ai_OwnPlayer));
        return 1;
    }
    return 0;
}
