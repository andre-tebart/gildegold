/* 0x4cd480, the oldest line of the ring (0: none) */

G18LOG *ai_Unk4cd480()
{
    int i;

    if (ai_Unk6f0cd8 == 0)
        return 0;
    i = ai_Unk9363f4 - ai_Unk6f0cd8;
    ai_Unk6f0cdc = i;
    if (i < 0) {
        i = ai_Unk9363f4 + 1;
        ai_Unk6f0cdc = i;
    }
    return &ai_Unk935da0[i];
}
