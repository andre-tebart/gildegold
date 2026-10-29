/* 0x4de610, the next number of the table modulo n */

int ai_Unk4de610(int n)
{
    int i, r;

    i = ai_Unk6f0db0 % ai_Unk60d370;
    r = ai_Unk92bcc0[i] % n;
    ai_Unk6f0db0 = ai_Unk6f0db0 + 1;
    return r;
}
