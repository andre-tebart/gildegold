/* 0x4dff80, the answer of a computer challenged person (m + 0x94): with 0x6f0d7c == 0 the question of 0x4dfcb0 (1 / 0 by a roll of 10 > 7),
   else the choice of 0x4dfdd0 (4 with 50 %, 2 with 35 %, 3 with 15 %) */

void ai_Unk4dff80(char *m)
{
    int r;

    if (ai_Unk6f0d7c == 0) {
        if ((unsigned int)ai_Unk4e52f0(10) > 7)
            m[0x94] = 1;
        else
            m[0x94] = 0;
        return;
    }
    r = ai_Unk4e52f0(100);
    if (r > 0x32)
        m[0x94] = 4;
    else if (r > 0xf)
        m[0x94] = 2;
    else
        m[0x94] = 3;
}
