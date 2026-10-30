/* 0x4ae280, the BISCHOF token of a history text (callback 0x601b10[1], name 0x601730[1]): a bishop (officeKind
   0x15 or 0x1a); otherwise as ai_TokenMayor */

int ai_TokenBishop(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    AICHAR *c;
    int i, n1, n2;

    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind != 0x15)
            return 0;
    } else {
        i = 0;
        while (!(ai_IsLivingCharacter((unsigned short)i) && ai_CharsT[i].officeKind == 0x1a
                 || ai_CharsT[i].officeKind == 0x15)) {
            if (++i >= 768)
                return 0;
        }
        c = &ai_CharsT[i];
        if (mode == 0)
            args[k].id = c->netId;
        if (c == 0)
            return 0;
    }
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    txt_FormatString(out, buf, c->id);
    return 1;
}
