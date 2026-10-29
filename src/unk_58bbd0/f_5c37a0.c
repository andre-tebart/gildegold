/* 0x5c37a0, a law / profession filter test (512 B): param_2 = the filter record (+8 / +0xc the two mask
   words of the building kinds -8 .. -1, +0x10 the target cases 0 .. 7, +0x14 bit 1 with the four +0x1c
   ids), param_3 = the building record (+0 the kind, +0x27 the owner word, +0x5a bit 1 the dead flag,
   +0x61 the home), param_1 = the comparing character (+0x27 against the own character 0x6f1bf8 for
   case 7). 1 = accepted */

int gm_ResearchClass(); int sv_ReturnZero(); void *ai_Unk5c3bb0();

int ai_Unk5c37a0(short *param_1, char *param_2, unsigned char *param_3)
{
    unsigned char kind;
    unsigned int blo;
    unsigned int bhi;
    int mask;
    int target;
    int exclude;
    int i;
    short v;
    short own;

    if (param_2 == 0)
        return 1;
    if (param_3 == 0)
        return 0;
    if (param_1 == 0)
        return 0;
    if (*(int *)(param_2 + 8) == 0 && *(int *)(param_2 + 0xc) == 0 && *(int *)(param_2 + 0x10) == 0)
        return 1;
    exclude = 1;
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *param_3 * 0x24d);
    if (kind < 0x20) {
        blo = 1 << (kind & 0x1f);
        bhi = 0;
    } else {
        blo = 0;
        bhi = 1 << ((kind - 0x20) & 0x1f);
    }
    if ((int)*(unsigned int *)(param_2 + 0xc) < 0) {
        if (param_3[0x5a] & 1)
            return 0;
        if (*(int *)(param_3 + 0x61) == 0)
            return 0;
    }
    mask = (*(unsigned int *)(param_2 + 8) & blo) != 0 || (*(unsigned int *)(param_2 + 0xc) & bhi) != 0;
    target = 0;
    v = *(short *)(param_3 + 0x27);
    switch (*(int *)(param_2 + 0x10)) {
    case 0:
        target = 1;
        break;
    case 1:
        target = v == -1;
        break;
    case 2:
        target = v != -1;
        break;
    case 3:
        target = v == *param_1 || v == -1;
        break;
    case 4:
        target = v == *param_1;
        break;
    case 5:
        target = v != *param_1;
        break;
    case 6:
        target = v != *param_1 && v != -1;
        break;
    case 7:
        own = *(short *)U_I((unsigned)&ai_PlayerChar);
        target = v != *param_1 && v != own && v != -1;
        break;
    }
    if ((*(unsigned char *)(param_2 + 0x14) & 1) && v != -1) {
        for (i = 0; i < 4; i++) {
            if (v == *(short *)(param_2 + 0x1c + i * 2) && *(short *)(param_2 + 0x1c + i * 2) != -1) {
                exclude = 0;
                break;
            }
        }
    }
    return mask && target && exclude;
}
