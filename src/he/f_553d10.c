/* 0x553d10, 1 unless a record of type 0x2e with +0xac = the id (+4) of b and +0xb0 = the id of a is running (then 0) */

int he_Unk553d10(int a, int b)
{
    char *e;

    for (e = he_FindEvent(1, 0, 0x2e); e != 0; e = he_NextEvent()) {
        if (*(int *)(b + 4) == *(int *)(e + 0xac) && *(int *)(e + 0xb0) == *(int *)(a + 4))
            return 0;
    }
    return 1;
}
