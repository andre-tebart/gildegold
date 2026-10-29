/* 0x45fdd0, a + (b - a) * t / n, rounded; a and b are swapped if a > b */

int ts_Lerp(float a, float b, float n, float t)
{
    float tmp;

    if (a > b) {
        tmp = a;
        a = b;
        b = tmp;
    }
    return (int)((b - a) / n * t + a + 0.5);
}
