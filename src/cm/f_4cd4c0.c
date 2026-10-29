/* 0x4cd4c0, the next line of the ring (0: no more) */

G18LOG *ai_Unk4cd4c0()
{
    int i = ai_Unk6f0cdc;

    if (i == ai_Unk9363f4)
        return 0;
    i++;
    ai_Unk6f0cdc = i;
    if (i == ai_Unk9363f4)
        return 0;
    if (i == 15) {
        i = 0;
        ai_Unk6f0cdc = 0;
    }
    return &ai_Unk935da0[i];
}
