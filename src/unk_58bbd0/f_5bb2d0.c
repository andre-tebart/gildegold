/* 0x5bb2d0, whether an order kind fits the building kind (112 B): the record +0x16c of param_1 (0 when absent);
   the kind record 0x7e3f30 + kind * 0x24d must be 4 for the kinds 0x172 / 0x174, 0x13 for 0x158 / 0x160 and
   0x10 for 0x16e / 0x176; 1 otherwise */

int ai_Unk5bb2d0(int param_1, int param_2)
{
    char *kind;
    int rec;

    if (param_1 != 0 && (rec = *(int *)(param_1 + 0x16c)) != 0) {
        kind = (char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)rec * 0x24d);
        if (((param_2 == 0x172 || param_2 == 0x174) && *kind != 4) ||
            ((param_2 == 0x158 || param_2 == 0x160) && *kind != 0x13) ||
            ((param_2 == 0x16e || param_2 == 0x176) && *kind != 0x10))
            return 0;
        return 1;
    }
    return 0;
}
