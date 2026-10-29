/* 0x4b4aa0, mission condition: 0x4b45c0 and at least three buildings of kind 7 */

int ai_Unk4b4aa0(AICHAR *c, int x, G13TYPE *t)
{
    AIBLD *b;
    int n;

    if (!ai_Unk4b45c0(c, x, t))
        return 0;
    n = 0;
    for (b = gm_BuildQuery(2, 4, c->id, n, 7); b != 0; b = gm_BuildNext())
        n++;
    return n >= 3;
}
