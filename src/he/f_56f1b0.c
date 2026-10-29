/* 0x56f1b0, a meeting message when the slot he_Unk56f260 (c + 3) is free and (0x504ec0 (a, 4) - 0x504ec0 (b, 4) * 0.5f) + 0x42ec20 () >= 1.0: 0x4f2e30 (slot, 1, b + 4, a + 4, the id +1 of the record at b + 0x170 / +0x16c, else -1); 0 when not sent */

int he_Unk56f1b0(int a, int b, int c)
{
    float fa, fb;
    int slot = he_Unk56f260(c);
    int id;

    if (slot >= 0) {
        fa = gm_GetSkill(a, 4);
        fb = gm_GetSkill(b, 4);
        if (fa - fb * 0.5f + util_RandomF() >= 1.0f) {
            if (*(int *)(b + 0x170) != 0)
                id = *(int *)(*(int *)(b + 0x170) + 1);
            else if (*(int *)(b + 0x16c) != 0)
                id = *(int *)(*(int *)(b + 0x16c) + 1);
            else
                id = -1;
            unk_Unk4f2e30(slot, 1, *(int *)(b + 4), *(int *)(a + 4), id);
            return 1;
        }
    }
    return 0;
}
