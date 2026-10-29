/* 0x50c980, the mean opinion (0x50c550 for the persons of the list ids[0 .. n - 1] about the character c) * 0.01; 0.5 without any */


float gm_Unk50c980(int c, int n, int *ids)
{
    int cnt, i, id;
    float s;
    unsigned short *p;

    cnt = 0;
    i = 0;
    s = 0.0f;
    if (0 < n) {
        do {
            id = ids[i];
            if (id != -1 && (p = (unsigned short *)gm_FindChar(id)) != 0) {
                s = (float)(gm_Liking(c, *p, 0) + s);
                cnt++;
            }
            i++;
        } while (i < n);
        if (cnt != 0)
            return s * 0.01 / cnt;
    }
    return 0.5f;
}
