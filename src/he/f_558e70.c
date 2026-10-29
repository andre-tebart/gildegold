/* 0x558e70, the allocation handler of the "SABOTAGE" message event: due now + 1; with no building (+0x10 = -1) the first of the kind 0x16 (else 0xf) is taken; +0xc4 = -1 and +0xc8 = 0x4c59f0 (0, +0x10, -1, 0, 1, 0x174, "SABOTAGE") */

void he_Unk558e70(int ev)
{
    int b;

    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    if (*(int *)(ev + 0x10) == -1) {
        b = gm_BuildQuery(1, 5, 0x16);
        if (b == 0)
            b = gm_BuildQuery(1, 5, 0xf);
        if (b != 0)
            *(int *)(ev + 0x10) = *(int *)(b + 1);
    }
    *(int *)(ev + 0xc4) = -1;
    *(int *)(ev + 0xc8) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0, *(int *)(ev + 0x10), -1, 0, 1, 0x174, "SABOTAGE");
}
