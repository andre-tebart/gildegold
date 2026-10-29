/* 0x4cedc0, a floating number (v / (c's +0x1c * 0.01)) over the character c in a free slot of the 16 of
   ai_Unk9304c0 (kind 1 / 2 with signs, else plain) */

void ai_Unk4cedc0(char *c, int v, int kind)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (ai_Unk9304c0[i].v == 0)
            break;
    }
    if (i >= 16)
        return;
    ai_Unk9304c0[i].v = (int)((double)v / (*(float *)(c + 0x1c) * 0.01));
    ai_Unk9304c0[i].c = c;
    ai_Unk9304c0[i].t = 64.0f;
    ai_Unk9304c0[i].kind = kind;
}
