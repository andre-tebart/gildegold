/* 0x5551c0, an object callback of 0x438d40: objects whose name contains "dummy_TRANSPORTER" are collected into the context (32 pointers, +0x80 the count); returns 1 while there is room */


char he_Unk5551c0(char *name, int *ctx)
{
    if (strstr(name, "dummy_TRANSPORTER") != 0) {
        ctx[ctx[0x20]] = (int)name;
        ctx[0x20] = ctx[0x20] + 1;
    }
    return ctx[0x20] < 0x20;
}
