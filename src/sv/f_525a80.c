/* 0x525a80, a callback (the object o, a flag f): sets the float +0x1a4 of the record +0x1e8 of o to a random 0.3 .. 0.9 (f = 0) or -0.3 + 0.6 * ... (f != 0); returns 1 */

char sv_Unk525a80(char *o, int f)
{
    double v;

    if (f == 0)
        v = util_RandomF() * 0.6 + 0.3;
    else
        v = -0.3 - util_RandomF() * 0.6;
    if (*(int *)(o + 0x1e8) != 0)
        *(float *)(*(int *)(o + 0x1e8) + 0x1a4) = (float)v;
    return 1;
}
