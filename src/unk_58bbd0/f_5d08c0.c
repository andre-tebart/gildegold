/* 0x5d08c0, the trial step of a player (262 B), called from he.c 0x549770 twice (the bytes +0xb8 /
   +0xb9): param_1 the player record (+0x80 + i the step counter, +4 the message handle), param_2 the
   step index 0 .. 4 (>= 5 -> 0). The counter n = the byte +0x80 + i: 252.0 or more -> 0. The chance
   x = (252.0f - n) * (1/252), y = x * 0.6f + 0.2f, v = (int)(((1.0f - y) + 0x42ec20 () * y) * 42.0f *
   x + 1.0f); the low byte of v is the amount. When n + amount is still above 252.0 the amount becomes
   the low byte of (int)(252.0f - n) instead. A nonzero amount goes through 0x4c6020 (the record + 4,
   the index, the amount). Returns 1 (0 when the index is 5 or more or the counter is 252.0 or more). */

int ai_Unk5d08c0(int param_1, int param_2)
{
    unsigned char n;
    unsigned char amount;
    int total;
    float x;
    float y;
    int v;

    if (param_2 >= 5)
        return 0;
    n = *(unsigned char *)(param_1 + param_2 + 0x80);
    if ((float)n >= 252.0f)
        return 0;
    x = (252.0f - n) * 0.003968254f;
    y = x * 0.6f + 0.2f;
    v = (int)(((1.0f - y) + util_RandomF() * y) * 42.0f * x + 1.0f);
    amount = (unsigned char)v;
    total = n + amount;
    if ((float)total > 252.0f)
        amount = (unsigned char)(int)(252.0f - n);
    if (amount != 0)
        ai_Unk4c6020(param_1 + 4, param_2, amount);
    return 1;
}
