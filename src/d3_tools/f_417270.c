/* 0x417270, smooth Hermite step: t = clamp((x - a) / (b - a), 0, 1), returns (3 - 2t) * t * t (0 below a,
   1 above b) */

float d3t_SmoothStep(float a, float b, float x)
{
    float t = min(max((x - a) / (b - a), 0.0f), 1.0f);
    return (3.0f - (t + t)) * t * t;
}
