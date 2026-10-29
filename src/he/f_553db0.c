/* 0x553db0, 1 unless a record of type 0x35 of the player *a (kind 2, 2) has +0xbc = the id (+4) of b (then 0); *count (when given) receives the number of records visited */

int he_Unk553db0(unsigned short *a, int b, int *count)
{
    char *e;
    int n = 0;

    for (e = he_FindEvent(2, 2, *a, 0, 0x35); e != 0; e = he_NextEvent()) {
        if (*(int *)(b + 4) == *(int *)(e + 0xbc))
            return 0;
        n++;
    }
    if (count != 0)
        *count = n;
    return 1;
}
