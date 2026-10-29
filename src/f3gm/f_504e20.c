/* 0x504e20, mixes the two 0x504ec0 values u = f (a, c) and v = f (b, c): ((u * u - v) * 0.4 + 0.6 + (u - v * v) * 0.6 + 0.4) * 0.5 */

float gm_Mix(int a, int b, int c)
{
    float u, v;

    u = gm_GetSkill(a, c);
    v = gm_GetSkill(b, c);
    return (float)((((double)u * u - v) * 0.4 + 0.6 + ((double)u - (double)v * v) * 0.6 + 0.4) * 0.5);
}
