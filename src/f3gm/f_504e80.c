/* 0x504e80, the same mix for two floats */

float gm_Mix2(float u, float v)
{
    return (float)((((double)u * u - v) * 0.4 + 0.6 + ((double)u - (double)v * v) * 0.6 + 0.4) * 0.5);
}
