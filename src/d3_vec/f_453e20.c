/* 0x453e20, cubic Hermite spline between p0 and p1 with tangents m0, m1 (missed by Ghidra) */

float d3_Hermite(float p0, float p1, float m0, float m1, float t)
{
    float t2 = t * t, t3 = t2 * t;

    float r = p0 * (2.0f * t3 - 3.0f * t2 + 1.0f) + p1 * (3.0f * t2 - 2.0f * t3);

    r += m0 * (t3 - 2.0f * t2 + t);
    r += m1 * (t3 - t2);
    return r;
}
