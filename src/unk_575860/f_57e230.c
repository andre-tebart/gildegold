/* 0x57e230, stores one 0x38-byte record of a 3-entry table (61 B): src (0 .. 2 in src[0]) copied 0xe dwords
   into 0x6f9f60 + src[0] * 0xe and 0x6f2664 = 1; 1, else 0 */

int ai_Unk57e230(int *src)
{
    int i;

    if (!src || src[0] < 0 || src[0] >= 3)
        return 0;
    for (i = 0; i < 0xe; i++)
        ((int *)(unsigned)&full_data_6f9f60)[src[0] * 0xe + i] = src[i];
    U_I((unsigned)&full_data_6f2664) = 1;
    return 1;
}
