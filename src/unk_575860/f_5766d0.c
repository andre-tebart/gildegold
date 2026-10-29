/* 0x5766d0, a transition callback on the step arguments: 1 when the step applied a state change (the kind 0xa
   step p[2] 0 / 1 drives the states 4 <-> 9 / 0xa -> 0xb), 3 otherwise; the kind 7 with a record whose byte +2
   is 5 moves the pending state 9 to 0xa */

int ai_Unk5766d0(int *p)
{
    if (p[0] == 0xa) {
        if (p[2] == 0 && U_I(S + 4) == 4) {
            U_I(S) = 9;
            U_I(S + 4) = 9;
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
            return 1;
        }
        if (p[2] == 1) {
            if (U_I(S + 4) == 9) {
                U_I(S) = 4;
                U_I(S + 4) = 4;
                U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
                return 1;
            }
            if (U_I(S + 4) == 0xa) {
                U_I(S) = 0xb;
                U_I(S + 4) = 0xb;
                U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
                return 1;
            }
        }
        return 3;
    }
    if (p[0] == 7) {
        if (!p[1])
            return 3;
        if (U_I(S + 4) != 9)
            return 3;
        if (*(unsigned char *)(p[1] + 2) != 5)
            return 3;
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    return 3;
}
