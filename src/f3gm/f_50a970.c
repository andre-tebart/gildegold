/* 0x50a970, the effect of a kind of event (kinds 1 .. 3 start with a float 0): the pseudocode lost both float operands. ROUGH placeholder */

void gm_Unk50a970(int a, int kind)
{
    float z = 0.0f;

    if (kind == 1 || kind == 2 || kind == 3)
        (void)(int)z;
    (void)(int)z;
}
