/* 0x573200, 1 when an event of the type 0x8d exists whose +0x10 argument is the word after `a` (searched with 0x53e400 (2, 3, .., 0, 0x8d)) and whose +0xb0 is `a` and byte +0xb4 is `b` */

int he_Unk573200(char *a, int b)
{
    char *r;

    r = he_FindEvent(2, 3, *(int *)(a + 1), 0, 0x8d);
    while (r != 0) {
        if (HM_I(r, 0xb0) == (int)a && HM_B(r, 0xb4) == b)
            return 1;
        r = he_NextEvent();
    }
    return 0;
}
