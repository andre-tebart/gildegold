/* 0x5552c0, sets the speed factor (+0x1a0 of the record at +0x3b of `p`) from the event's value +0xc0: * the first constant when 0x76061c points to a value above 100, else * the second when 0x760618 points above 200, else unchanged; a second stage (+0xf1 == 2) multiplies it once more */

void he_Unk5552c0(int ev, int p)
{
    int rec = *(int *)(p + 0x3b);

    if (U_I((unsigned)&ai_Unk76061c) != 0 && *(int *)U_I((unsigned)&ai_Unk76061c) > 0x64)
        *(float *)(rec + 0x1a0) = (float)(*(float *)(ev + 0xc0) * 0.7);
    else if (U_I((unsigned)&ai_Unk760618) != 0 && *(int *)U_I((unsigned)&ai_Unk760618) > 0xc8)
        *(float *)(rec + 0x1a0) = (float)(*(float *)(ev + 0xc0) * 0.8);
    else
        *(int *)(rec + 0x1a0) = *(int *)(ev + 0xc0);
    if (*(unsigned char *)(ev + 0xf1) == 2)
        *(float *)(rec + 0x1a0) = (float)(*(float *)(rec + 0x1a0) * 0.8);
}
