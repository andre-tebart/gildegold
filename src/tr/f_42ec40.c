/* 0x42ec40 */

float util_RandomSq(void)
{
    float x = util_RandomF() * 2.0f - 1.0f;
    float s;

    s = x < 0.0f ? -1.0f : 1.0f;
    return s * x * x;
}
