/* 0x576970, the same for the kind 0x1a and the record word 0x110 */

int ai_Unk576970(int *p)
{
    if (p[0] == 0x1a && p[1] && *(unsigned short *)p[1] == 0x110) {
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
    return 3;
}
