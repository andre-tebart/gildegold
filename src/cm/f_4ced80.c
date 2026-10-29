/* 0x4ced80 */

void ai_Unk4ced80(int a)
{
    char *p;

    ai_Unk4cec00();
    for (p = ai_Unk930600; (int)p < (int)(ai_Unk930600 + 0x4380); p += 0x21c) {
        if (p[0x188] != 0)
            ai_Unk4cec30(a, p);
    }
}
