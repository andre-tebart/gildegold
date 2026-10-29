/* 0x5d06e0, a message callback (156 B): param_1 the record (+0x2c the flag dword, +4 the handle).
   When the flag is set, 0x4c4a60 (the handle, 0x2c, 4, 0, the first matching class) with the classes in
   test order 0xf -> 7, 0xf0 -> 0x30, 0xf00 -> 0x300, 0x3000 -> 0x1000, 0x1c000 -> 0xc000, 0xe0000 ->
   0x20000, 0x700000 -> 0x100000, 0x1800000 -> 0x800000, 0x1e000000 -> 0x6000000. Returns 1. */

int ai_Unk5d06e0(int param_1)
{
    int f;
    int flag;

    f = *(int *)(param_1 + 0x2c);
    if (f != 0) {
        flag = 0;
        if (f & 0xf)
            flag = 7;
        else if (f & 0xf0)
            flag = 0x30;
        else if (f & 0xf00)
            flag = 0x300;
        else if (f & 0x3000)
            flag = 0x1000;
        else if (f & 0x1c000)
            flag = 0xc000;
        else if (f & 0xe0000)
            flag = 0x20000;
        else if (f & ((unsigned)&he_Events + 0x54c0))
            flag = 0x100000;
        else if (f & 0x1800000)
            flag = ((unsigned)&ai_Relations + 0x1aca0);
        else if (f & 0x1e000000)
            flag = 0x6000000;
        cm_SendSetField(*(int *)(param_1 + 4), 0x2c, 4, 0, flag);
    }
    return 1;
}
