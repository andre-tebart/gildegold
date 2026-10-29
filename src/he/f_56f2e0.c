/* 0x56f2e0, a message when the person a and the record b (its word at 0 is the target) can meet: b not 0 and (0x504ec0 (a, 2) - 0x504ec0 (b, 2) * 0.5f) + 0x42ec20 () >= 1.0; then 0x4fca40 (*b, (-1 - c) * 5) */

int he_Unk56f2e0(int a, unsigned short *b, int c)
{
    float fa, fb;

    if (b != 0) {
        fa = gm_GetSkill(a, 2);
        fb = gm_GetSkill(b, 2);
        if (fa - fb * 0.5f + util_RandomF() >= 1.0f) {
            ((float (*)(unsigned short, int))gm_AddHealth)(*b, (-1 - c) * 5);
            return 1;
        }
    }
    return 0;
}
