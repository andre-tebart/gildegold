/* 0x4eb0b0, the progress the character c makes training the share i in *cap minutes (*cap gets the minutes used): 14400 * (1 - share) is the time the rest of the
   share needs; returns (used / time) * (1 - share), 0.0 when the share is not in [0, 1) */

float ahm_ShareProgress(char *c, int i, int *cap)
{
    float f;
    int t, m;

    f = ahm_Share(c, i);
    if (f < 0.0f || f >= 1.0f)
        return 0.0f;
    t = (int)(14400.0f * (1.0f - f));
    m = *cap < t ? *cap : t;
    *cap = m;
    return (float)m / t * (1.0f - f);
}
