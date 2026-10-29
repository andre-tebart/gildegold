/* 0x5c39a0, the same filter test with the mask 0xf82806f / 2 against the 0x40000000 flag (528 B): the
   empty record test is +8 / +0x10 only, the dead check param_3 +0x5a bit 1 needs param_2 +0xc < 0, and
   the accept is (both empty or a mask hit) && the target case && the +0x1c exclusions */

int ai_Unk5c39a0(short *param_1, char *param_2, unsigned char *param_3)
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
    if (*(int *)(param_2 + 8) == 0 && *(int *)(param_2 + 0x10) == 0)
        return 1;
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *param_3 * 0x24d);
    if (kind < 0x20) {
        blo = 1 << (kind & 0x1f);
        bhi = 0;
    } else {
        blo = 0;
        bhi = 1 << ((kind - 0x20) & 0x1f);
    }
    if (((blo & 0xf82806f) != 0 || (bhi & 2) != 0) && (*(unsigned int *)(param_2 + 0xc) & 0x40000000) != 0)
        return 0;
    exclude = 1;
    if ((int)*(unsigned int *)(param_2 + 0xc) < 0 && (param_3[0x5a] & 1))
        return 0;
    mask = (*(unsigned int *)(param_2 + 8) == 0 && *(unsigned int *)(param_2 + 0xc) == 0) ||
           (blo & *(unsigned int *)(param_2 + 8)) != 0 || (bhi & *(unsigned int *)(param_2 + 0xc)) != 0;
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
