/* 0x4b4670, mission condition: at least t->goal of c's five values (bytes 0x80..0x84, / 42) reach t->unk18 */

int ai_Unk4b4670(AICHAR *c, G13TYPE *t)
{
    int i, n;

    for (i = 0, n = 0; i < 5; i++) {
        if (G13_B(c, 0x80 + i) * 0.023809524f >= t->unk18)
            n++;
    }
    return n >= t->goal;
}
