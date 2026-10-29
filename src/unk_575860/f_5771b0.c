/* 0x5771b0, a transition callback on the flag S+0x38: it is raised to 1 by the kind 0x25 with the step word
   0x155, and to 2 by the kind 0x24 on a building with the table byte 0xa (1); else 0, and the answer of 0x5764b0
   when the flag is below the needed value or no record is given */

int ai_Unk5771b0(int *p)
{
    int kind;

    if (U_I(S + 0x38) >= 0 && p[0] == 0x25 && p[3] == 0x155) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    if (U_I(S + 0x38) >= 1 && p[1] && p[0] == 0x24) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0xa) {
            U_I(S + 0x38) = 2;
            return 1;
        }
        return 0;
    }
    return ai_Unk5764b0(p);
}
