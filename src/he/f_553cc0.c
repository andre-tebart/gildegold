/* 0x553cc0, 1 unless a record of type 0x62 with +0xb0 = the id (+1 of `p`) and +0xb8 == 1 is running (then 0); a null p gives 1 */

int he_Unk553cc0(int p)
{
    char *e;

    if (p == 0)
        return 1;
    for (e = he_FindEvent(1, 0, 0x62); e != 0; e = he_NextEvent()) {
        if (*(int *)(e + 0xb0) == *(int *)(p + 1) && *(int *)(e + 0xb8) == 1)
            return 0;
    }
    return 1;
}
