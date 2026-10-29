/* 0x4ea5c0, the mean liking (gm_Liking / 100) of the owners of the offices of the district d (1 .. 7) for the
   character c; 0 when there is none */

float amt_GetDistrictLiking(short *c, int d)
{
    AMTREC list[6];
    float sum;
    int n, i, cnt;
    short *o;

    sum = 0.0f;
    if (d >= 1 && d <= 7) {
        n = amt_GetColumnOffices(d, 6, list);
        cnt = 0;
        for (i = 0; i < n; i++) {
            if (list[i].owner != AMT_NETID(c) && list[i].owner != -1) {
                o = (short *)gm_FindChar(list[i].owner);
                if (o != 0) {
                    sum = (float)gm_Liking(((AMTCHARVIEW *)c)->id, ((AMTCHARVIEW *)o)->id, 1) * 0.01f + sum;
                    cnt++;
                }
            }
        }
        if (cnt != 0)
            sum = sum / (float)cnt;
        return sum;
    }
    return 0.0f;
}
