/* 0x4f97c0, the ratio of the sums of the bytes +0x247 and +0x248 of the building records (ai_BTypes, 0x24d bytes per kind) of the 32 slots (0xa9 bytes each, the byte +0xa9 of the table 0x889f74 points to = the kind, 0 = free); the first sum over the second, 0 when the second is 0 */

float ord_BuildingCapacityRatio(void)
{
    int a;
    int b;
    int i;
    char *p;
    char *r;

    a = 0;
    b = 0;
    p = ai_Blds + 0xa9;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0) {
            r = ai_BTypes + (unsigned char)*p * 0x24d;
            a += (unsigned char)r[0x247];
            b += (unsigned char)r[0x248];
        }
        p += 0xa9;
    }
    if (b != 0)
        return (float)a / b;
    return 0.0f;
}
