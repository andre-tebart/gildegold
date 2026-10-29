/* 0x56fbc0, the progress value of a building record (rec + 0x18 .. the value +0x1c, the steps +0x14, the start +0x20): num not above +0x20 -> 0x4c4ac0 (rec + 4, 0x1c, rec + 0x18 - rec + 0x1c); else (num - +0x20) steps of v = v - step + step / v from v = num - +0x20, then 0x4c4ac0 (rec + 4, 0x1c, v - rec + 0x1c) */

void he_Unk56fbc0(int rec, unsigned short num)
{
    float f, v, fnum;
    int n;

    if (rec == 0)
        return;
    fnum = (float)num;
    if (fnum <= *(float *)(rec + 0x20)) {
        ((int (*)(int, int, float))ai_Unk4c4ac0)(*(int *)(rec + 4), 0x1c, *(float *)(rec + 0x18) - *(float *)(rec + 0x1c));
        return;
    }
    f = *(float *)(rec + 0x14);
    v = fnum - *(float *)(rec + 0x20);
    for (n = (int)v; n > 0; n--)
        v = v - f + f / v;
    ((int (*)(int, int, float))ai_Unk4c4ac0)(*(int *)(rec + 4), 0x1c, v - *(float *)(rec + 0x1c));
}
