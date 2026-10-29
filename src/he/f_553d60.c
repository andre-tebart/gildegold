/* 0x553d60, 1 unless a record of type 0x4b with +0xb0 = the id (+4) of b and +0xac = the id of a is running (then 0) */

int he_Unk553d60(int a, int b)
{
    char *e;

    for (e = he_FindEvent(1, 0, 0x4b); e != 0; e = he_NextEvent()) {
        if (*(int *)(b + 4) == *(int *)(e + 0xb0) && *(int *)(a + 4) == *(int *)(e + 0xac))
            return 0;
    }
    return 1;
}
