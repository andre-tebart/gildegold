/* 0x576920, the same as 0x5768d0 for the kind 0x19 and the record word 0x110 */

int ai_Unk576920(int *p)
{
    if (p[0] == 0x19 && p[1] && *(unsigned short *)p[1] == 0x110) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    return 3;
}
