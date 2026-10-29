/* 0x50c520, the opinion of the character a of the character b (0x7f for oneself) */

int gm_Opinion(int a, int b)
{
    if (a == b)
        return 0x7f;
    return ai_Relations[a][b];
}
