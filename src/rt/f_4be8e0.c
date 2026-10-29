/* 0x4be8e0, removes the three controls of out (0x4be790) */

void rt_RemoveControls(int *out)
{
    if (out[0] != -1)
        d2_RemoveObj(out[0]);
    if (out[1] != -1)
        d2_RemoveObj(out[1]);
    if (out[2] != -1)
        d2_RemoveObj(out[2]);
    out[0] = -1;
    out[1] = -1;
    out[2] = -1;
}
