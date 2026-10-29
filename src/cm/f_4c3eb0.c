/* 0x4c3eb0, adds the pair (k, v) to the second field block */

int ai_Unk4c3eb0(unsigned char k, float v)
{
    unsigned char *p;
    int o;

    o = ai_Unka68d20;
    if (ai_Unka68d20 + 5 > 0x76)
        return 1;
    p = &ai_Unka68ca0.buf[ai_Unka68d20];
    ai_Unka68ca0.n++;
    ai_Unka68d20 += 5;
    *p = k;
    *(float *)&ai_Unka68ca0.buf[o + 1] = v;
    return 0;
}
