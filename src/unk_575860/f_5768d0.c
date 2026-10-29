/* 0x5768d0, the kind 0x19 with a record whose word is 0x10e moves the pending state to 0xb (1), else 3 */

int ai_Unk5768d0(int *p)
{
    if (p[0] == 0x19 && p[1] && *(unsigned short *)p[1] == 0x10e) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    return 3;
}
