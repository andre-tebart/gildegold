/* 0x4d0cd0, the first free (-1) of 16 slots, else -1 */

int ai_Unk4d0cd0(int *a)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (a[i] == -1)
            return i;
    }
    return -1;
}
