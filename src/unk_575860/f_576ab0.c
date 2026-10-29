/* 0x576ab0, the kinds 0x19 / 0x1a with a record whose word is 0x13: the kind 0x19 only in the pending state 4
   (state 0xa), the kind 0x1a in any state (state 0xb); else 3 */

int ai_Unk576ab0(int *p)
{
    if (p[0] == 0x19 && p[1] && U_I(S + 4) == 4) {
        if (*(unsigned short *)p[1] != 0x13)
            return 3;
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    if (p[0] == 0x1a && p[1] && *(unsigned short *)p[1] == 0x13) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    return 3;
}
