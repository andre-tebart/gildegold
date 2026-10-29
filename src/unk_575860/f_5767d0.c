/* 0x5767d0, the kind 0xd with a building whose table byte is 0xa moves the pending state to 0xb (1), else 3 */

int ai_Unk5767d0(int *p)
{
    int kind;

    if (p[0] == 0xd && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0xa) {
            U_I(S) = 0xb;
            U_I(S + 4) = 0xb;
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
            return 1;
        }
    }
    return 3;
}
